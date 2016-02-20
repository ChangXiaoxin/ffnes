// ����ͷ�ļ�
#include "adev.h"
#include "log.h"

// �ڲ����Ͷ���
typedef struct
{
    HWAVEOUT hWaveOut;
    WAVEHDR *pWaveHdr;
    HANDLE   bufsem;
    int      bufnum;
    int      buflen;
    int      head;
    int      tail;
} ADEV_CONTEXT;

// �ڲ�����ʵ��
static void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
    ADEV_CONTEXT *dev = (ADEV_CONTEXT*)dwInstance;
    switch (uMsg)
    {
    case WOM_DONE:
        if (++dev->head == dev->bufnum) dev->head = 0;
        ReleaseSemaphore(dev->bufsem, 1, NULL);
        break;
    }
}

// �ӿں���ʵ��
static void* adev_waveout_create(int bufnum, int buflen)
{
    ADEV_CONTEXT *dev = NULL;
    WAVEFORMATEX  wfx = {0};
    BYTE         *pwavbuf;
    int           i;

    // allocate adev context
    dev = malloc(sizeof(ADEV_CONTEXT));
    if (!dev) {
        log_printf("failed to allocate adev context !\n");
        exit(0);
    }

    dev->bufnum   = bufnum;
    dev->buflen   = buflen;
    dev->head     = 0;
    dev->tail     = 0;
    dev->pWaveHdr = (WAVEHDR*)malloc(bufnum * (sizeof(WAVEHDR) + buflen));
    dev->bufsem   = CreateSemaphore(NULL, bufnum, bufnum, NULL);
    if (!dev->pWaveHdr || !dev->bufsem) {
        log_printf("failed to allocate waveout buffer and waveout semaphore !\n");
        exit(0);
    }

    // init for audio
    wfx.cbSize          = sizeof(wfx);
    wfx.wFormatTag      = WAVE_FORMAT_PCM;
    wfx.wBitsPerSample  = 16;    // 16bit
    wfx.nSamplesPerSec  = 44100; // 44.1k
//  wfx.nSamplesPerSec  = 48000; // 48.0k
    wfx.nChannels       = 1;     // stereo
    wfx.nBlockAlign     = wfx.nChannels * wfx.wBitsPerSample / 8;
    wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;
    waveOutOpen(&(dev->hWaveOut), WAVE_MAPPER, &wfx, (DWORD_PTR)waveOutProc, (DWORD)dev, CALLBACK_FUNCTION);

    // init wavebuf
    memset(dev->pWaveHdr, 0, bufnum * (sizeof(WAVEHDR) + buflen));
    pwavbuf = (BYTE*)(dev->pWaveHdr + bufnum);
    for (i=0; i<bufnum; i++) {
        dev->pWaveHdr[i].lpData         = (LPSTR)(pwavbuf + i * buflen);
        dev->pWaveHdr[i].dwBufferLength = buflen;
        waveOutPrepareHeader(dev->hWaveOut, &(dev->pWaveHdr[i]), sizeof(WAVEHDR));
    }

    return dev;
}

static void adev_waveout_destroy(void *ctxt)
{
    ADEV_CONTEXT *dev = (ADEV_CONTEXT*)ctxt;
    int i;

    // unprepare
    for (i=0; i<dev->bufnum; i++) {
        waveOutUnprepareHeader(dev->hWaveOut, &(dev->pWaveHdr[i]), sizeof(WAVEHDR));
    }

    waveOutClose(dev->hWaveOut);
    CloseHandle(dev->bufsem);
    free(dev->pWaveHdr);
    free(dev);
}

static void adev_waveout_bufrequest(void *ctxt, AUDIOBUF **ppab)
{
    ADEV_CONTEXT *dev = (ADEV_CONTEXT*)ctxt;
    WaitForSingleObject(dev->bufsem, -1);
    *ppab = (AUDIOBUF*)&(dev->pWaveHdr[dev->tail]);
}

static void adev_waveout_bufpost(void *ctxt, AUDIOBUF *pab)
{
    ADEV_CONTEXT *dev = (ADEV_CONTEXT*)ctxt;
    waveOutWrite(dev->hWaveOut, (LPWAVEHDR)pab, sizeof(WAVEHDR));
    if (++dev->tail == dev->bufnum) dev->tail = 0;
}

// ȫ�ֱ�������
ADEV DEV_WAVEOUT =
{
    adev_waveout_create,
    adev_waveout_destroy,
    adev_waveout_bufrequest,
    adev_waveout_bufpost,
};


