// ffndbdebugDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ffemulator.h"
#include "ffndbdebugDlg.h"

// CffndbdebugDlg dialog

IMPLEMENT_DYNAMIC(CffndbdebugDlg, CDialog)

CffndbdebugDlg::CffndbdebugDlg(CWnd* pParent, NES *pnes)
    : CDialog(CffndbdebugDlg::IDD, pParent)
{
    // save nes pointer
    m_pNES = pnes;
}

CffndbdebugDlg::~CffndbdebugDlg()
{
}

void CffndbdebugDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CffndbdebugDlg, CDialog)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BTN_NES_RESET    , &CffndbdebugDlg::OnBnClickedBtnNesReset)
    ON_BN_CLICKED(IDC_BTN_NES_RUN_PAUSE, &CffndbdebugDlg::OnBnClickedBtnNesRunPause)
    ON_BN_CLICKED(IDC_BTN_NES_DEBUG_CPU, &CffndbdebugDlg::OnBnClickedBtnNesDebugCpu)
    ON_BN_CLICKED(IDC_BTN_NES_DEBUG_PPU, &CffndbdebugDlg::OnBnClickedBtnNesDebugPpu)
END_MESSAGE_MAP()

// CffndbdebugDlg message handlers
void CffndbdebugDlg::OnOK()
{
    CDialog::OnOK();
    DestroyWindow();
}

void CffndbdebugDlg::OnCancel()
{
    CDialog::OnCancel();
    DestroyWindow();
}

BOOL CffndbdebugDlg::OnInitDialog()
{
    // update button
    CWnd *pwnd = GetDlgItem(IDC_BTN_NES_RUN_PAUSE);
    if (m_pNES->isrunning) {
        pwnd->SetWindowText("running");
    }
    else pwnd->SetWindowText("paused");

    pwnd = GetDlgItem(IDC_LST_OPCODE);
    pwnd->MoveWindow(9, 187, 736, 342, FALSE);

    return TRUE;
}

void CffndbdebugDlg::OnDestroy()
{
    CDialog::OnDestroy();

    // delete self
    delete this;
}

void CffndbdebugDlg::OnBnClickedBtnNesReset()
{
    nes_reset(m_pNES);
}

void CffndbdebugDlg::OnBnClickedBtnNesRunPause()
{
    CWnd *pwnd = GetDlgItem(IDC_BTN_NES_RUN_PAUSE);
    if (m_pNES->isrunning) {
        nes_pause(m_pNES);
        pwnd->SetWindowText("paused");
    }
    else {
        nes_run(m_pNES);
        pwnd->SetWindowText("running");
    }
}

void CffndbdebugDlg::OnBnClickedBtnNesDebugCpu()
{
    for (int i=IDC_GRP_CPU_CONTROL; i<=IDC_TXT_CPUINFO; i++)
    {
        CWnd *pwnd = GetDlgItem(i);
        pwnd->ShowWindow(SW_SHOW);
    }
}

void CffndbdebugDlg::OnBnClickedBtnNesDebugPpu()
{
    for (int i=IDC_GRP_CPU_CONTROL; i<=IDC_TXT_CPUINFO; i++)
    {
        CWnd *pwnd = GetDlgItem(i);
        pwnd->ShowWindow(SW_HIDE);
    }
}