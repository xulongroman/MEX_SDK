// MessageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MessageDlg.h"


// CMessageDlg dialog

IMPLEMENT_DYNAMIC(CMessageDlg, CDialog)

CMessageDlg::CMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMessageDlg::IDD, pParent)
{

}

CMessageDlg::CMessageDlg(CWnd* pParent, CString message1, CString message2)
: CDialog(CMessageDlg::IDD, pParent), m_szMessage1(message1), m_szMessage2(message2)
{
}


CMessageDlg::~CMessageDlg()
{
}

void CMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMessageDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CMessageDlg::OnNMClickSyslink1)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK2, &CMessageDlg::OnNMClickSyslink2)
END_MESSAGE_MAP()


// CMessageDlg message handlers

BOOL CMessageDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CFont font;
	font.CreateFont(
		20,8,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH,	_T("Calibri"));
	GetDlgItem(IDC_SYSLINK1)->SetFont(&font);
	SetDlgItemText(IDC_SYSLINK1, m_szMessage1);
	font.DeleteObject();

	font.CreateFont(
		20,8,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH,	_T("Calibri"));
	GetDlgItem(IDC_SYSLINK2)->SetFont(&font);
	SetDlgItemText(IDC_SYSLINK2, m_szMessage2);
	font.DeleteObject();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMessageDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int start = m_szMessage1.Find(_T("<a>"));
	int length = m_szMessage1.GetLength();
	CString path = m_szMessage1.Right(length-start-3);
	int end = path.Find(_T("</a>"));
	path = path.Left(end);
	ShellExecute(NULL,_T("open"), path, NULL, NULL, SW_SHOWNORMAL);
	*pResult = 0;
}

void CMessageDlg::OnNMClickSyslink2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int start = m_szMessage2.Find(_T("<a>"));
	int length = m_szMessage2.GetLength();
	CString path = m_szMessage2.Right(length-start-3);
	int end = path.Find(_T("</a>"));
	path = path.Left(end);
	ShellExecute(NULL,_T("open"), path, NULL, NULL, SW_SHOWNORMAL);
	*pResult = 0;
}
