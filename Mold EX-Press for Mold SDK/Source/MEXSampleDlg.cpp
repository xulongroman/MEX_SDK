
// MEXSampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MEXSample.h"
#include "MEXSampleDlg.h"
#include "DlgProxy.h"
#include "TTComboBox.h"
#include "CSVparser.hpp"
//#include "3DViewDlg.h"
#include <map>
#include <HtmlHelp.h>
#include "MessageDlg.h"

#pragma  comment(lib,"Htmlhelp.lib")
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//************************************
// Function:  Find the string from the specified language ini file
// Returns:   CString
// Parameter: CString szID
//************************************
CString g_LoadString(CString szID, CString szLanguagePath)
{
	CString szValue;
	DWORD dwSize = 1000;
	GetPrivateProfileString(_T("String"),szID,_T("Not found"),
		szValue.GetBuffer(dwSize),dwSize,szLanguagePath);
	szValue.ReleaseBuffer();

	szValue.Replace(_T("\\n"),_T("\n"));

	return szValue;
}

wstring ANSIToUnicode( const string& str )
{
	int len = 0;
	len = str.length();
	int unicodeLen = ::MultiByteToWideChar( CP_ACP,
	             0,
	             str.c_str(),
	             -1,
	             NULL,
	             0 ); 
	wchar_t * pUnicode; 
	pUnicode = new wchar_t[unicodeLen+1]; 
	memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t)); 
	::MultiByteToWideChar( CP_ACP,
	          0,
	          str.c_str(),
	          -1,
	          (LPWSTR)pUnicode,
	          unicodeLen ); 
	wstring rt; 
	rt = ( wchar_t* )pUnicode;
	delete pUnicode; 
	return rt; 
}

string UnicodeToANSI( const wstring& str )
{
	char*  pElementText;
	int    iTextLen;
	// wide char to multi char
	iTextLen = WideCharToMultiByte( CP_ACP,
		0,
		str.c_str(),
		-1,
		NULL,
		0,
		NULL,
		NULL );
	pElementText = new char[iTextLen + 1];
	memset( ( void* )pElementText, 0, sizeof( char ) * ( iTextLen + 1 ) );
	::WideCharToMultiByte( CP_ACP,
		0,
		str.c_str(),
		-1,
		pElementText,
		iTextLen,
		NULL,
		NULL );
	string strText;
	strText = pElementText;
	delete[] pElementText;
	return strText;
}

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMEXSampleDlg dialog




IMPLEMENT_DYNAMIC(CMEXSampleDlg, CDialog);

CMEXSampleDlg::CMEXSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMEXSampleDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
	m_iMessageShow = 1;
}

CMEXSampleDlg::~CMEXSampleDlg()
{
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to NULL, so it knows
	//  the dialog has been deleted.
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
}

void CMEXSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MAIN, m_comboMain);
	DDX_Control(pDX, IDC_COMBO_SUB, m_comboSub);
}

BEGIN_MESSAGE_MAP(CMEXSampleDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_MODULE_INIT, &CMEXSampleDlg::OnCbnSelchangeComboModuleInit)
	ON_BN_CLICKED(IDC_BUTTON_MODULE_INIT, &CMEXSampleDlg::OnBnClickedButtonModuleInit)
	ON_CBN_SELCHANGE(IDC_COMBO_MAIN, &CMEXSampleDlg::OnCbnSelchangeComboMain)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CMEXSampleDlg::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CMEXSampleDlg::OnBnClickedButtonModify)
	ON_EN_CHANGE(IDC_EDIT_BOM_NUMBER, &CMEXSampleDlg::OnEnChangeEditBomNumber)
	ON_BN_CLICKED(IDC_BUTTON_BOM, &CMEXSampleDlg::OnBnClickedButtonBom)
	ON_CBN_SELCHANGE(IDC_COMBO_LANG_SETTING, &CMEXSampleDlg::OnCbnSelchangeComboLangSetting)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_HELP, &CMEXSampleDlg::OnNMClickSyslinkHelp)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_1, &CMEXSampleDlg::OnNMClickSyslink1)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_2, &CMEXSampleDlg::OnNMClickSyslink2)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_3, &CMEXSampleDlg::OnNMClickSyslink3)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_4, &CMEXSampleDlg::OnNMClickSyslink4)
	ON_BN_CLICKED(IDC_BUTTON_MODEL_FILE, &CMEXSampleDlg::OnBnClickedButtonModelFile)
	ON_BN_CLICKED(IDC_BUTTON_PROPERTY_FILE, &CMEXSampleDlg::OnBnClickedButtonPropertyFile)
END_MESSAGE_MAP()


// CMEXSampleDlg message handlers
BOOL CMEXSampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL,szPath,MAX_PATH); 
	m_szCurPath = CString(szPath);

	m_szCurPath = m_szCurPath.Left(m_szCurPath.ReverseFind('\\'));
	m_szCurPath += _T("\\"); 

	m_szSettingFile = m_szCurPath + _T("setting.ini");

	// TODO: Add extra initialization here
	CButton* run_btn = (CButton*)GetDlgItem(IDC_BUTTON_RUN);
	run_btn->EnableWindow(FALSE);

	CButton* modify_btn = (CButton*)GetDlgItem(IDC_BUTTON_MODIFY);
	modify_btn->EnableWindow(FALSE);

	CButton* bom_btn = (CButton*)GetDlgItem(IDC_BUTTON_BOM);
	bom_btn->EnableWindow(FALSE);

	CEdit* bomNumber_edit = (CEdit*)GetDlgItem(IDC_EDIT_BOM_NUMBER);
	bomNumber_edit->SetWindowText(_T("1"));

// 	0: 中文简体CHS
//  1: 英语ENG
// 	2: 日语JPN
// 	3: 繁体中文CHT
// 	4: 韩文KOR
// 	5: 泰文TAI
	CComboBox* lang_combo = (CComboBox*)GetDlgItem(IDC_COMBO_LANG_SETTING);
	lang_combo->AddString(_T("简体中文"));
	lang_combo->AddString(_T("English"));
	lang_combo->AddString(_T("日本語"));
	lang_combo->AddString(_T("繁體中文"));
	lang_combo->AddString(_T("한국어"));
	lang_combo->AddString(_T("มีมิติเท่ากัน"));

	int iLange = 1;
	{
		CString szSection = _T("Setting");
		CString szKey = _T("Language"), szLang;

		DWORD dwSize = 256;
		GetPrivateProfileString(szSection,szKey,_T("1"),szLang.GetBuffer(dwSize),dwSize,m_szSettingFile);
		iLange = _ttoi(szLang);
		szLang.ReleaseBuffer();
	}
	lang_combo->SetCurSel(iLange);


	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_COMBO_MODULE_INIT);
	combo->AddString(_T("SAT&DWG-[oInit:1]"));
	combo->AddString(_T("SAT-[oInit:4]"));
	combo->AddString(_T("SAT&STP-[oInit:5]"));
	combo->AddString(_T("SAT&DWG&X_T-[oInit:6]"));

	int iInit = 1;
	{
		CString szSection = _T("Setting");
		CString szKey = _T("Init"), szInit;

		DWORD dwSize = 256;
		GetPrivateProfileString(szSection,szKey,_T("1"),szInit.GetBuffer(dwSize),dwSize,m_szSettingFile);
		iInit = _ttoi(szInit);
		szInit.ReleaseBuffer();
	}
	combo->SetCurSel(iInit);

	//CListCtrl
	CListCtrl* listctrl = (CListCtrl*)GetDlgItem(IDC_LIST_BOM);

	
	LV_COLUMN lvColumn;
	lvColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_CENTER;
	for (int i=0; i<8; i++)
	{
		CString szID;
		szID.Format(_T("IDS_GML_LISTCTRL_R0C%d"), i);
		lvColumn.iSubItem = i;
		lvColumn.cx = i < 2 ? 100 : 80; 
		TCHAR szColumn[100];
		_tcscpy(szColumn, g_LoadString(szID,  GetLanguagePath()));
		lvColumn.pszText = szColumn;
		listctrl->InsertColumn(i, &lvColumn);
	}

	listctrl->DeleteAllItems();
	
	((CButton*)GetDlgItem(IDC_CHECK_CLEAR_EXISTING_BOM))->SetCheck(TRUE);

	//SysLink Control
	CString szLinker = _T("<A HREF=\"\">HELP</A>");
	CLinkCtrl* linkctrl = (CLinkCtrl*)GetDlgItem(IDC_SYSLINK_HELP);
	CFont font;
	font.CreateFont(
		20,12,0,0,FW_MEDIUM,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH,	_T("Calibri"));
	linkctrl->SetFont(&font);
	linkctrl->SetWindowText(szLinker);
	font.DeleteObject();

	CString szLinker2 = _T("<A HREF=\"\">?</A>");
	CFont font2;
	font2.CreateFont(
		20,10,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH,	_T("Calibri"));

	linkctrl = (CLinkCtrl*)GetDlgItem(IDC_SYSLINK_1);
	linkctrl->SetFont(&font2);
	linkctrl->SetWindowText(szLinker2);

	linkctrl = (CLinkCtrl*)GetDlgItem(IDC_SYSLINK_2);
	linkctrl->SetFont(&font2);
	linkctrl->SetWindowText(szLinker2);

	linkctrl = (CLinkCtrl*)GetDlgItem(IDC_SYSLINK_3);
	linkctrl->SetFont(&font2);
	linkctrl->SetWindowText(szLinker2);

	linkctrl = (CLinkCtrl*)GetDlgItem(IDC_SYSLINK_4);
	linkctrl->SetFont(&font2);
	linkctrl->SetWindowText(szLinker2);

	font2.DeleteObject();

	m_brush.CreateSolidBrush(RGB(255, 255, 255));

	UpdateMainLang();
	UpdateInterfaceLang();

//	m_szGMLPath = _wgetenv(_T("Misumi_Path_GML"));

	bool bWow64 = false;
	SYSTEM_INFO si; 
	GetNativeSystemInfo(&si); 

	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||    
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 ) 
	{ 
		//64 位操作系统
		bWow64 = true;
	} 

	HKEY hKey = NULL;
	long lRet= RegOpenKeyEx(HKEY_CLASSES_ROOT, _T("Mold_EX_Press_Global_M.Out\\CLSID"), 0, KEY_READ, &hKey);

	CString strGuid;
	if (hKey != NULL)
	{
		DWORD dwSize     = 0;
		DWORD dwDataType = 0;
		LPBYTE lpValue   = NULL;
		lRet = RegQueryValueEx(hKey, 
			L"",
			0,
			&dwDataType,
			lpValue,  // NULL
			&dwSize); // will contain the data size

		// Alloc the buffer
		lpValue = (LPBYTE)malloc(dwSize);

		// Call twice RegQueryValueEx to get the value
		lRet = RegQueryValueEx(hKey, 
			L"",
			0,
			&dwDataType,
			lpValue,
			&dwSize);

		char* chkGuid = new char[dwSize];
		int j = 0;
		for (int i = 0; i<dwSize; i++)
		{
			if (lpValue[i] != '\0')
			{
				chkGuid[j] = lpValue[i];
				j++;
			}
		}
		chkGuid[j] = '\0';
		strGuid = CString(chkGuid);
		free(lpValue);
		delete [] chkGuid;

		RegCloseKey(hKey);
	}

	if (bWow64)
	{
		strGuid = _T("Wow6432Node\\CLSID\\") + strGuid + _T("\\LocalServer32");
//		AfxMessageBox(L"OS is 64bit");
	}
	else
	{
		strGuid = _T("CLSID\\") + strGuid + _T("\\LocalServer32");
//		AfxMessageBox(L"OS is 32bit");
	}
//	AfxMessageBox(L"HKEY_CLASSES_ROOT\\"+strGuid);

	lRet= RegOpenKeyEx(HKEY_CLASSES_ROOT, strGuid, 0, KEY_READ, &hKey);
	CString strPath;
	if (hKey != NULL)
	{
		DWORD dwSize     = 0;
		DWORD dwDataType = 0;
		LPBYTE lpValue   = NULL;
		lRet = RegQueryValueEx(hKey, 
			L"",
			0,
			&dwDataType,
			lpValue,  // NULL
			&dwSize); // will contain the data size

		// Alloc the buffer
		lpValue = (LPBYTE)malloc(dwSize);

		// Call twice RegQueryValueEx to get the value
		lRet = RegQueryValueEx(hKey, 
			L"",
			0,
			&dwDataType,
			lpValue,
			&dwSize);

		char* chkPath = new char[dwSize];
		int j = 0;
		for (int i = 0; i<dwSize; i++)
		{
			if (lpValue[i] != '\0')
			{
				chkPath[j] = lpValue[i];
				j++;
			}
		}
		chkPath[j] = '\0';
		strPath = CString(chkPath);
		RegCloseKey(hKey);
		free(lpValue);
		delete [] chkPath;
	}


	int pos = strPath.ReverseFind('\\');
	strPath = strPath.Left(pos+1);
	if (strPath[0] == '"'){
		strPath = strPath.Right(strPath.GetLength() - 1);
	}

	m_szGMLPath = strPath;

//	AfxMessageBox(m_szGMLPath);

	CString szModelFilePath, szPropertyFilePath;
	CString szSection = _T("Setting");
	DWORD dwSize = 256;

	CString szKey = _T("ModelFilePath");
	GetPrivateProfileString(szSection,szKey,_T(""),szModelFilePath.GetBuffer(dwSize),dwSize,m_szSettingFile);

	szKey = _T("PropertyFilePath");
	GetPrivateProfileString(szSection,szKey,_T(""),szPropertyFilePath.GetBuffer(dwSize),dwSize,m_szSettingFile);

	szKey = _T("InstallPath");
	WritePrivateProfileString(szSection,szKey,m_szGMLPath,m_szSettingFile);


	if (szModelFilePath == _T(""))
	{
		if (m_szGMLPath != _T(""))
		{
			szModelFilePath = m_szGMLPath + _T("Preview_JAP\\dwgtmp\\");
		}
		else
		{
			szModelFilePath = _T("");
		}

	}
	CEdit* modelFile_edit = (CEdit*)GetDlgItem(IDC_EDIT_MODEL_FILE);
	modelFile_edit->SetWindowText(szModelFilePath);
	szModelFilePath.ReleaseBuffer();
	modelFile_edit->EnableWindow(FALSE);


	if (szPropertyFilePath == _T(""))
	{
		if (m_szGMLPath != _T(""))
		{
			szPropertyFilePath = m_szGMLPath + _T("Preview_JAP\\dwgtmp\\");
		}
		else
		{
			szPropertyFilePath = _T("");
		}
	}
	CEdit* propertyFile_edit = (CEdit*)GetDlgItem(IDC_EDIT_PROPERTY_FILE);
	propertyFile_edit->SetWindowText(szPropertyFilePath);
	szPropertyFilePath.ReleaseBuffer();
	propertyFile_edit->EnableWindow(FALSE);

	m_bitmap.LoadBitmap(IDB_BITMAP_OPEN_FOLDER);
	HBITMAP hBitmap = (HBITMAP)m_bitmap.GetSafeHandle();
 
	CButton* modelFile_btn = (CButton*)GetDlgItem(IDC_BUTTON_MODEL_FILE);
	modelFile_btn->EnableWindow(FALSE);
	if (modelFile_btn->GetBitmap() == NULL)
		modelFile_btn->SetBitmap(hBitmap);

	CButton* propertyFile_btn = (CButton*)GetDlgItem(IDC_BUTTON_PROPERTY_FILE);
	propertyFile_btn->EnableWindow(FALSE);
	if (propertyFile_btn->GetBitmap() == NULL)
		propertyFile_btn->SetBitmap(hBitmap);

	CString szOpen;
	szKey = _T("ModelFilePath_Open");
	GetPrivateProfileString(szSection,szKey,_T("0"),szOpen.GetBuffer(dwSize),dwSize,m_szSettingFile);
	((CButton*)(GetDlgItem(IDC_CHECK_MODEL_FILE)))->SetCheck(_ttoi(szOpen));

	szKey = _T("PropFile_Open");
	GetPrivateProfileString(szSection,szKey,_T("0"),szOpen.GetBuffer(dwSize),dwSize,m_szSettingFile);
	((CButton*)(GetDlgItem(IDC_CHECK_PROP_FILE)))->SetCheck(_ttoi(szOpen));

	szKey =_T("Message_Show"); 
	CString szMessageShow;
	GetPrivateProfileString(szSection,szKey,_T("1"),szMessageShow.GetBuffer(dwSize),dwSize,m_szSettingFile);
	m_iMessageShow = _ttoi(szMessageShow);

	//Read CSV
	csv::Parser file = csv::Parser(L"Custom_info_GML.csv");
	for (int i=0; i<file.rowCount(); i++)
	{
		Model_Info* model_info = new Model_Info;
		wstring name, value;
		CString strName;
		for (int j=0; j<file.columnCount(); j++ )
		{
			name = file.getHeaderElement(j);
			value = file[i][j];
			model_info->AttrNames.push_back(name);
			model_info->AttrValues.push_back(value);
// 			if (strName == L"TYPE")
// 			{
// 				model_info->_TYPE = value;
// 			}
			if (name == L"P_ID")
			{
				model_info->_P_ID = value;
			}
		}
// 		if (model_info->_TYPE != L"")
// 		{
// 			m_ModelInfoMapping.insert(pair<wstring, Model_Info*>(model_info->_TYPE, model_info));
// 		}
		if (model_info->_P_ID != L"")
		{
			m_ModelInfoMapping.insert(pair<wstring, Model_Info*>(model_info->_P_ID, model_info));
		}
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

CString CMEXSampleDlg::GetLanguagePath()
{
	CComboBox* lang_combo = (CComboBox*)GetDlgItem(IDC_COMBO_LANG_SETTING);
	int index = lang_combo->GetCurSel();
// 	0: 中文简体CHS
//  1: 英语ENG
// 	2: 日语JPN
// 	3: 繁体中文CHT
// 	4: 韩文KOR
// 	5: 泰文TAI
	switch(index)
	{
	case 0:
		{
			m_szLangFile = _T("lang_chs.ini");
		}
		break;
	case 1:
		{
			m_szLangFile = _T("lang_en.ini");
		}
		break;
	case 2:
		{
			m_szLangFile = _T("lang_jp.ini");
		}
		break;
	case 3:
		{
			m_szLangFile = _T("lang_cht.ini");
		}
		break;
	case 4:
		{
			m_szLangFile = _T("lang_kr.ini");
		}
		break;
	case 5:
		{
			m_szLangFile = _T("lang_tha.ini");
		}
		break;
	}

	return m_szCurPath + m_szLangFile;
}

void CMEXSampleDlg::UpdateInterfaceLang()
{

	CString szLanguagePath = GetLanguagePath();

	SetWindowText(g_LoadString(_T("IDS_GML_DLG_TITLE"), szLanguagePath));
	GetDlgItem(IDC_STATIC_MODULE_INIT)->SetWindowText(g_LoadString(_T("IDS_GML_DLG_1"), szLanguagePath));
	GetDlgItem(IDC_BUTTON_MODULE_INIT)->SetWindowText(g_LoadString(_T("IDS_GML_DLG_2"), szLanguagePath));
	GetDlgItem(IDC_STATIC_LANG_SETTING)->SetWindowText(g_LoadString(_T("IDS_GML_DLG_3"), szLanguagePath));
	GetDlgItem(IDC_STATIC_IMPORT_SETTING)->SetWindowText(g_LoadString(_T("IDS_GML_DLG_5"), szLanguagePath));
	GetDlgItem(IDC_BUTTON_RUN)->SetWindowText(g_LoadString(_T("IDS_GML_DLG_6"), szLanguagePath));
	GetDlgItem(IDC_STATIC_MODIFY)->SetWindowText(g_LoadString(_T("IDS_GML_DLG_7"), szLanguagePath));
	GetDlgItem(IDC_STATIC_MODIFY_STRING)->SetWindowText(g_LoadString(_T("IDS_GML_DLG_8"), szLanguagePath));
	GetDlgItem(IDC_BUTTON_MODIFY)->SetWindowText(g_LoadString(_T("IDS_GML_DLG_9"), szLanguagePath));
	GetDlgItem(IDC_STATIC_BOM)->SetWindowText(g_LoadString(_T("IDS_GML_DLG_10"), szLanguagePath));
	GetDlgItem(IDC_STATIC_BOM_NUMBER)->SetWindowText(g_LoadString(_T("IDS_GML_DLG_11"), szLanguagePath));
	GetDlgItem(IDC_CHECK_CLEAR_EXISTING_BOM)->SetWindowText(g_LoadString(_T("IDS_GML_DLG_12"), szLanguagePath));
	GetDlgItem(IDC_STATIC_BOM_STRING)->SetWindowText(g_LoadString(_T("IDS_GML_DLG_13"), szLanguagePath));
	GetDlgItem(IDC_BUTTON_BOM)->SetWindowText(g_LoadString(_T("IDS_GML_DLG_14"), szLanguagePath));
	GetDlgItem(IDC_STATIC_FILE_PATH)->SetWindowText(g_LoadString(_T("IDS_GML_DLG_15"), szLanguagePath));
	GetDlgItem(IDC_STATIC_MODEL_FILE)->SetWindowText(g_LoadString(_T("IDS_GML_DLG_16"), szLanguagePath));
	GetDlgItem(IDC_STATIC_PROPERTY_FILE)->SetWindowText(g_LoadString(_T("IDS_GML_DLG_17"), szLanguagePath));
	GetDlgItem(IDC_STATIC_AUTO_OPEN)->SetWindowText(g_LoadString(_T("IDS_GML_DLG_19"), szLanguagePath));
	Invalidate(true);
}

void CMEXSampleDlg::UpdateMainLang()
{
	CString strMainName, szLanguagePath;
	szLanguagePath = GetLanguagePath();

	m_comboMain.ResetContent();

	strMainName = g_LoadString(_T("IDS_GML_1"), szLanguagePath);
	strMainName += _T("-[oShowInterfaceDlg1:0]");
	m_comboMain.AddString(strMainName);

	strMainName = g_LoadString(_T("IDS_GML_2"), szLanguagePath);
	m_comboMain.AddString(strMainName);

	strMainName = g_LoadString(_T("IDS_GML_3"), szLanguagePath);
	m_comboMain.AddString(strMainName);

	strMainName = g_LoadString(_T("IDS_GML_4"), szLanguagePath);
	m_comboMain.AddString(strMainName);

	strMainName = g_LoadString(_T("IDS_GML_5"), szLanguagePath);
	strMainName += _T("-[oShowInterfaceDlg1:1]");
	m_comboMain.AddString(strMainName);

	strMainName = g_LoadString(_T("IDS_GML_6"), szLanguagePath);
	strMainName += _T("-[oShowInterfaceDlg1:2]");
	m_comboMain.AddString(strMainName);

	strMainName = g_LoadString(_T("IDS_GML_7"), szLanguagePath);
	strMainName += _T("-[oShowInterfaceDlg1:3]");
	m_comboMain.AddString(strMainName);

	strMainName = g_LoadString(_T("IDS_GML_8"), szLanguagePath);
	strMainName += _T("-[oShowInterfaceDlg1:4]");
	m_comboMain.AddString(strMainName);

	strMainName = g_LoadString(_T("IDS_GML_9"), szLanguagePath);
	strMainName += _T("-[oShowInterfaceDlg1:5]");
	m_comboMain.AddString(strMainName);

	strMainName = g_LoadString(_T("IDS_GML_13"), szLanguagePath);
	m_comboMain.AddString(strMainName);

	strMainName = g_LoadString(_T("IDS_GML_14"), szLanguagePath);
	m_comboMain.AddString(strMainName);

	strMainName = g_LoadString(_T("IDS_GML_15"), szLanguagePath);
	strMainName += _T("-[oShowDlg_Menu_Mold_Jap:30]");
	m_comboMain.AddString(strMainName);

	m_comboMain.SetCurSel(0);
	m_comboSub.ResetContent();
	m_comboSub.EnableWindow(FALSE);

	//CListCtrl
	CListCtrl* listctrl = (CListCtrl*)GetDlgItem(IDC_LIST_BOM);
	LV_COLUMN lvColumn;
	lvColumn.mask = LVCF_FMT|LVCF_WIDTH|LVCF_TEXT|LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_CENTER;

	for (int i=0; i<8; i++)
	{
		CString szID;
		szID.Format(_T("IDS_GML_LISTCTRL_R0C%d"), i);
		lvColumn.iSubItem = i;
		lvColumn.cx = i < 2 ? 100 : 80; 
		TCHAR szColumn[100];
		_tcscpy(szColumn, g_LoadString(szID,  GetLanguagePath()));
		lvColumn.pszText = szColumn;
		listctrl->SetColumn(i, &lvColumn);
	}
}

void CMEXSampleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMEXSampleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMEXSampleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CMEXSampleDlg::OnClose()
{
	CString szSection = _T("Setting");
	DWORD dwSize = 256;
	CString szKey = _T("ModelFilePath");

	CEdit* modelFile_edit =(CEdit*)GetDlgItem(IDC_EDIT_MODEL_FILE);
	szKey = _T("ModelFilePath");
	CString szModelFilePath;
	modelFile_edit->GetWindowText(szModelFilePath);
	WritePrivateProfileString(szSection,szKey,szModelFilePath,m_szSettingFile);


	CEdit* propertyFile_edit =(CEdit*)GetDlgItem(IDC_EDIT_PROPERTY_FILE);
	szKey = _T("PropertyFilePath");
	CString szPropertyFilePath;
	propertyFile_edit->GetWindowText(szPropertyFilePath);
	WritePrivateProfileString(szSection,szKey,szPropertyFilePath,m_szSettingFile);

	szKey = _T("ModelFilePath_Open");
	int bOpen = ((CButton*)(GetDlgItem(IDC_CHECK_MODEL_FILE)))->GetCheck();
	CString szOpen;
	szOpen.Format(_T("%d"), bOpen);
	WritePrivateProfileString(szSection,szKey,szOpen,m_szSettingFile);

	szKey = _T("PropFile_Open");
	bOpen = ((CButton*)(GetDlgItem(IDC_CHECK_PROP_FILE)))->GetCheck();
	szOpen.Format(_T("%d"), bOpen);
	WritePrivateProfileString(szSection,szKey,szOpen,m_szSettingFile);

	m_iOut.oUninit();


	for(map<wstring, Model_Info*>::iterator it = m_ModelInfoMapping.begin(); it != m_ModelInfoMapping.end(); it++)
	{
		delete it->second;
	}
	m_ModelInfoMapping.clear();

	if (CanExit())
		CDialog::OnClose();
}

void CMEXSampleDlg::OnOK()
{
	if (CanExit())
		CDialog::OnOK();
}

void CMEXSampleDlg::OnCancel()
{
	if (CanExit())
		CDialog::OnCancel();
}

BOOL CMEXSampleDlg::CanExit()
{
	// If the proxy object is still around, then the automation
	//  controller is still holding on to this application.  Leave
	//  the dialog around, but hide its UI.
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}


void CMEXSampleDlg::OnCbnSelchangeComboModuleInit()
{
	// TODO: Add your control notification handler code here
	CString szSection = _T("Setting");
	DWORD dwSize = 256;
	CString szKey =_T("Init");

	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_COMBO_MODULE_INIT);
	szKey = _T("Init");
	int iInit = combo->GetCurSel();
	CString szInit;
	szInit.Format(_T("%d"), iInit);
	WritePrivateProfileString(szSection,szKey,szInit,m_szSettingFile);
}

void CMEXSampleDlg::OnBnClickedButtonModuleInit()
{
	// TODO: Add your control notification handler code here
	if( !m_szGMLPath.GetLength() || !m_iOut.CreateDispatch(_T("Mold_EX_Press_Global_M.Out")))
	{
		AfxMessageBox(g_LoadString(_T("IDS_GML_CREATE_DISPATCH_FAIL"), GetLanguagePath()));
		return;
	}

	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_COMBO_MODULE_INIT);
	int cur = combo->GetCurSel();
	int init = 4;
	if (cur == 0)
	{
		init = 1;
	}
	else if (cur == 2)
	{
		init = 5;
	}
	else if (cur == 3)
	{
		init = 6;
	}
	m_iOut.oInit(init);
	combo->EnableWindow(FALSE);

	CButton* init_btn = (CButton*)GetDlgItem(IDC_BUTTON_MODULE_INIT);
	init_btn->EnableWindow(FALSE);

	CButton* run_btn = (CButton*)GetDlgItem(IDC_BUTTON_RUN);
	run_btn->EnableWindow(TRUE);

	CButton* modify_btn = (CButton*)GetDlgItem(IDC_BUTTON_MODIFY);
	modify_btn->EnableWindow(TRUE);

	CButton* bom_btn = (CButton*)GetDlgItem(IDC_BUTTON_BOM);
	bom_btn->EnableWindow(TRUE);

	CButton* modelFile_btn = (CButton*)GetDlgItem(IDC_BUTTON_MODEL_FILE);
	modelFile_btn->EnableWindow(TRUE);

	CButton* propertyFile_btn = (CButton*)GetDlgItem(IDC_BUTTON_PROPERTY_FILE);
	propertyFile_btn->EnableWindow(TRUE);

	CEdit* modelFile_edit = (CEdit*)GetDlgItem(IDC_EDIT_MODEL_FILE);
	modelFile_edit->EnableWindow(TRUE);

	CEdit* propertyFile_edit = (CEdit*)GetDlgItem(IDC_EDIT_PROPERTY_FILE);
	propertyFile_edit->EnableWindow(TRUE);

//	m_szGMLPath = _wgetenv(_T("Misumi_Path_GML"));

	AfxMessageBox(g_LoadString(_T("IDS_GML_INIT_SUCCESS"), GetLanguagePath()));
}

void CMEXSampleDlg::OnCbnSelchangeComboMain()
{
	// TODO: Add your control notification handler code here
	m_comboSub.ResetContent();
	int index = m_comboMain.GetCurSel();
	CString strSubName, szLanguagePath;
	szLanguagePath = GetLanguagePath();
	switch(index)
	{
	case 0:
		m_comboSub.EnableWindow(FALSE);
		break;
	case 1:
		m_comboSub.EnableWindow(TRUE);

		strSubName = g_LoadString(_T("IDS_GML_2_1"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:0]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_2_2"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:1]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_2_3"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:2]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_2_4"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:3]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_2_5"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:4]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_2_6"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:5]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_2_7"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:6]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_2_8"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:7]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_2_9"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:8]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_2_10"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:9]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_2_11"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:10]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_2_12"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:11]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_2_13"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:12]");
		m_comboSub.AddString(strSubName);

		m_comboSub.SetCurSel(0);
		break;
	case 2:
		m_comboSub.EnableWindow(TRUE);

		strSubName = g_LoadString(_T("IDS_GML_3_1"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:13]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_3_2"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:14]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_3_3"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:15]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_3_4"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:16]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_3_5"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:17]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_3_6"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:18]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_3_7"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:19]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_3_8"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:20]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_3_9"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:21]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_3_10"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:22]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_3_11"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:23]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_3_12"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:24]");
		m_comboSub.AddString(strSubName);

		m_comboSub.SetCurSel(0);
		break;
	case 3:
		m_comboSub.EnableWindow(TRUE);

		strSubName = g_LoadString(_T("IDS_GML_4_1"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:25]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_4_2"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:26]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_4_3"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:27]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_4_4"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:28]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_4_5"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:29]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_4_6"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:30]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_4_7"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:31]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_4_8"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:32]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_4_9"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:33]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_4_10"), szLanguagePath);
		strSubName += _T("-[oShowInterfaceDlg2:34]");
		m_comboSub.AddString(strSubName);

		m_comboSub.SetCurSel(0);
		break;
	case 4:
		m_comboSub.EnableWindow(FALSE);
		break;
	case 5:
		m_comboSub.EnableWindow(FALSE);
		break;
	case 6:
		m_comboSub.EnableWindow(FALSE);
		break;
	case 7:
		m_comboSub.EnableWindow(FALSE);
		break;
	case 8:
		m_comboSub.EnableWindow(FALSE);
		break;
	case 9:
		m_comboSub.EnableWindow(TRUE);

		strSubName = g_LoadString(_T("IDS_GML_13_1"), szLanguagePath);
		strSubName += _T("-[oShowDlg_Menu_Mold_Jap:0]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_13_2"), szLanguagePath);
		strSubName += _T("-[oShowDlg_Menu_Mold_Jap:1]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_13_3"), szLanguagePath);
		strSubName += _T("-[oShowDlg_Menu_Mold_Jap:2]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_13_4"), szLanguagePath);
		strSubName += _T("-[oShowDlg_Menu_Mold_Jap:3]");
		m_comboSub.AddString(strSubName);

		m_comboSub.SetCurSel(0);
		break;
	case 10:
		m_comboSub.EnableWindow(TRUE);

		strSubName = g_LoadString(_T("IDS_GML_14_1"), szLanguagePath);
		strSubName += _T("-[oShowDlg_Menu_Mold_Jap:4]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_14_2"), szLanguagePath);
		strSubName += _T("-[oShowDlg_Menu_Mold_Jap:5]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_14_3"), szLanguagePath);
		strSubName += _T("-[oShowDlg_Menu_Mold_Jap:6]");
		m_comboSub.AddString(strSubName);

		strSubName = g_LoadString(_T("IDS_GML_14_4"), szLanguagePath);
		strSubName += _T("-[oShowDlg_Menu_Mold_Jap:7]");
		m_comboSub.AddString(strSubName);

		m_comboSub.SetCurSel(0);
		break;
	case 11:
		m_comboSub.EnableWindow(FALSE);
		break;
	}
}

FILE* OpenFileForDump( const wchar_t* ipFilePath)
{
	FILE* _fp = NULL;
	_fp = _wfopen( ipFilePath, L"w+,ccs=UTF-8" );

	if( NULL == _fp)              
		AfxMessageBox(L"Could not open output file.\n");         

	return _fp;
}

void CMEXSampleDlg::RunInterfaceDlg1(int i)
{
	m_iOut.oShowInterfaceDlg1(i);
	GenerateModel();
}

void CMEXSampleDlg::RunInterfaceDlg2(int i)
{
	m_iOut.oShowInterfaceDlg2(i, FALSE, 0);
	GenerateModel();
}



void CMEXSampleDlg::GenerateModel()
{
	CString strOrder = m_iOut.oGetOrder();

	//Note: the return Type value can be used to check if end-user import model successfully; 
	//if Type is null, that means end-user close Mold EX-Press before importing. 
	CString strType = m_iOut.oGetTypeName();
	if(strType == _T(""))
		return;
	CString strP_ID;

	m_BOMInfo.szPartNumber = strOrder;

	int count = m_iOut.oGetParaCount();
	CStringArray CustPropNames;
	CStringArray CustPropValues;
	map<CString, CString> NameValueMap;

	//modify string
	CString strFX;
	bool bEnd = false;

	int Index = 0;

	for (int i = 0; i < count; i++)
	{
		CString strName = m_iOut.oGetParaName(i);
		CString strValue = m_iOut.oGetParaVlaue(i);
		CustPropNames.Add(strName);
		CustPropValues.Add(strValue);
		NameValueMap.insert(map<CString, CString>::value_type(strName, strValue));

		if (strName == _T("Measure"))
		{
			bEnd = true;
			Index = i;
		}

		if (!bEnd)
			strFX = strFX+strName+_T("=")+strValue+_T(";");

		if (strName==_T("AddinType"))
			strFX = strFX+strName+_T("=")+strValue+_T("%");
	}
	strFX = _T("%Type=")+strType+_T(";")+strFX;

	//BOM string
	CString strBOM;
	{
		CString strTemp, strSubBOM;
		map<CString, CString>::iterator it;
		strBOM = strBOM + strOrder;

		it = NameValueMap.find(_T("P_ID"));
		if (it != NameValueMap.end())
		{
			strP_ID = it->second;
		}

		//If "Photo" attribute cannot be found, use "ABCDEFGHIJKLMNOPQRSTUVWXYZ" as replacement
		it = NameValueMap.find(_T("Photo"));
		if (it != NameValueMap.end())
		{
			strTemp = it->second;
			m_BOMInfo.szPhoto = strTemp;
		}
		else
		{
			strTemp = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
			m_BOMInfo.szPhoto = _T("->");
		}	
		strBOM = strBOM + _T("@") + strTemp;


		//If "PartName" attribute cannot be found, use "ABCDEFGHIJKLMNOPQRSTUVWXYZ" as replacement
		it = NameValueMap.find(_T("PartName"));
		if (it != NameValueMap.end())
		{
			strTemp = it->second;
			m_BOMInfo.szPartName = strTemp;
		}
		else
		{
			strTemp = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
			m_BOMInfo.szPartName = _T("->");
		}	
		strBOM = strBOM + _T("@") + strTemp;


		//If "Brand" attribute cannot be found, use "ABCDEFGHIJKLMNOPQRSTUVWXYZ" as replacement
		it = NameValueMap.find(_T("Brand"));
		if (it != NameValueMap.end())
		{
			strTemp = it->second;
			m_BOMInfo.szProductSeries = strTemp;
		}
		else
		{
			strTemp = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
			m_BOMInfo.szProductSeries = _T("->");
		}
		strBOM = strBOM + _T("@") + strTemp;


		//If "Material" attribute cannot be found, use "ABCDEFGHIJKLMNOPQRSTUVWXYZ" as replacement
		it = NameValueMap.find(_T("Material"));
		if (it != NameValueMap.end())
		{
			strTemp = it->second;
			m_BOMInfo.szMaterial = strTemp;
		}
		else
		{
			strTemp = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
			m_BOMInfo.szMaterial = _T("->");
		}
		strBOM = strBOM + _T("@") + strTemp;


		//If "Delivery" attribute cannot be found, use "ABCDEFGHIJKLMNOPQRSTUVWXYZ" as replacement
		it = NameValueMap.find(_T("Delivery"));
		if (it != NameValueMap.end())
		{
			strTemp = it->second;
			m_BOMInfo.szDelivery = strTemp;
		}
		else
		{
			strTemp = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
			m_BOMInfo.szDelivery = _T("->");
		}
		strBOM = strBOM + _T("@") + strTemp;

		strBOM = strBOM + _T("@") + strFX;


		//If "Mass" attribute cannot be found, use "ABCDEFGHIJKLMNOPQRSTUVWXYZ" as replacement
	    it = NameValueMap.find(_T("Mass"));
		if (it != NameValueMap.end())
			strTemp = it->second;
		else
			strTemp = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		strBOM = strBOM + _T("@") + strTemp;


		//If "PartName" attribute cannot be found, use "ABCDEFGHIJKLMNOPQRSTUVWXYZ" as replacement
		it = NameValueMap.find(_T("PartNo"));
		if (it != NameValueMap.end())
			strTemp = it->second;
		else
			strTemp = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		strBOM = strBOM + _T("@") + strTemp;


		//If "Comment" attribute cannot be found, use "ABCDEFGHIJKLMNOPQRSTUVWXYZ" as replacement
		it = NameValueMap.find(_T("Comment"));
		if (it != NameValueMap.end())
		{
			strTemp = it->second;
			m_BOMInfo.szRemark = strTemp;
		}
		else
		{
			strTemp = _T("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
			m_BOMInfo.szRemark = _T("->");
		}
		strBOM = strBOM + _T("@") + strTemp;
		
		CString strNumber;
		CEdit* bomNumber_edit = (CEdit*)GetDlgItem(IDC_EDIT_BOM_NUMBER);
		bomNumber_edit->GetWindowText(strNumber);
		int iNumber = _ttoi(strNumber);

		int totalpos = 0;
		strTemp.Format(_T("@%d@X@X@X"), iNumber);

		strSubBOM = strBOM;

		int pos = strSubBOM.ReverseFind('@');
		totalpos += strSubBOM.GetLength() - pos;
		strSubBOM = strSubBOM.Left(pos);

		pos = strSubBOM.ReverseFind('@');
		totalpos += strSubBOM.GetLength() - pos;
		strSubBOM = strSubBOM.Left(pos);

		pos = strSubBOM.ReverseFind('@');
		totalpos += strSubBOM.GetLength() - pos;
		strSubBOM = strSubBOM.Left(pos);

		pos = strSubBOM.ReverseFind('@');
		totalpos += strSubBOM.GetLength() - pos;
		strSubBOM = strSubBOM.Left(pos);

		strSubBOM += strTemp;

		strBOM=strBOM.Right(totalpos);

		strBOM = strSubBOM+strBOM;
	}



	CTime time = CTime::GetCurrentTime();
	CString szFileName;
	szFileName.Format(_T("%d_%d_%d_%d_%d_%d"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	szFileName = strOrder + _T("_") + szFileName + _T(".txt");


	CString szPropertyFilePath, szModelFilePath;
	CEdit* propertyFile_edit =(CEdit*)GetDlgItem(IDC_EDIT_PROPERTY_FILE);
	propertyFile_edit->GetWindowText(szPropertyFilePath);

	CEdit* modelFile_edit =(CEdit*)GetDlgItem(IDC_EDIT_MODEL_FILE);
	modelFile_edit->GetWindowText(szModelFilePath);

	char chk = szPropertyFilePath.GetAt(szPropertyFilePath.GetLength()-1);
	if (chk != _T('\\'))
	{
		szPropertyFilePath += _T("\\"); 
	}

	chk = szModelFilePath.GetAt(szModelFilePath.GetLength()-1);
	if (chk != _T('\\'))
	{
		szModelFilePath += _T("\\"); 
	}

	CopyFile(m_szGMLPath+_T("Preview_JAP\\dwgtmp\\msmdwgtmp.sat"), szModelFilePath+_T("msmdwgtmp.sat"), FALSE);

	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_COMBO_MODULE_INIT);
	int cur = combo->GetCurSel();
	int init = 4;

	if (cur == 0)
	{
		init = 1;
	}
	else if (cur == 2)
	{
		init = 5;
	}
	else if (cur == 3)
	{
		init = 6;
	}

	if (init==1)
	{
		CopyFile(m_szGMLPath+_T("Preview_JAP\\dwg\\")+strOrder+_T(".dwg"), szModelFilePath+strOrder+_T(".dwg"), FALSE);
		CopyFile(m_szGMLPath+_T("Preview_JAP\\dwg\\")+strOrder+_T("_1.dwg"), szModelFilePath+strOrder+_T("_1.dwg"), FALSE);
		CopyFile(m_szGMLPath+_T("Preview_JAP\\dwg\\")+strOrder+_T("_2.dwg"), szModelFilePath+strOrder+_T("_2.dwg"), FALSE);
		CopyFile(m_szGMLPath+_T("Preview_JAP\\dwg\\")+strOrder+_T("_3.dwg"), szModelFilePath+strOrder+_T("_3.dwg"), FALSE);
	}
	else if (init==5)
	{
		CopyFile(m_szGMLPath+_T("Preview_JAP\\dwgtmp\\msmstpfile_prt.stp"), szModelFilePath+_T("msmstpfile_prt.stp"), FALSE);
	}
	else if (init==6)
	{
		CopyFile(m_szGMLPath+_T("Preview_JAP\\dwg\\")+strOrder+_T(".dwg"), szModelFilePath+strOrder+_T(".dwg"), FALSE);
		CopyFile(m_szGMLPath+_T("Preview_JAP\\dwg\\")+strOrder+_T("_1.dwg"), szModelFilePath+strOrder+_T("_1.dwg"), FALSE);
		CopyFile(m_szGMLPath+_T("Preview_JAP\\dwg\\")+strOrder+_T("_2.dwg"), szModelFilePath+strOrder+_T("_2.dwg"), FALSE);
		CopyFile(m_szGMLPath+_T("Preview_JAP\\dwg\\")+strOrder+_T("_3.dwg"), szModelFilePath+strOrder+_T("_3.dwg"), FALSE);
		CopyFile(m_szGMLPath+_T("Preview_JAP\\dwgtmp\\msmparasolidfile.x_t"), szModelFilePath+_T("msmparasolidfile.x_t"), FALSE);
	}

	CString szPropertyFile = szPropertyFilePath + szFileName;
	FILE* fp = OpenFileForDump(szPropertyFile);

	WCHAR message[1024];
	swprintf(message, _T("Model Attribute:\n"));
	if( fp )
		fwrite(message,sizeof(wchar_t),wcslen(message),fp);

	for(int j = 0; j < CustPropNames.GetCount(); j++)
	{
		WCHAR message[1024];
		USES_CONVERSION;
		swprintf(message, _T("[Name] %s : [Value] %s\n"), T2CW(CustPropNames.GetAt(j)), T2CW(CustPropValues.GetAt(j)));
		if( fp )
			fwrite(message,sizeof(wchar_t),wcslen(message),fp);
	}

	USES_CONVERSION;
	swprintf(message, _T("[Name] Order : [Value] %s\n"), T2CW(strOrder));
	if( fp )
		fwrite(message,sizeof(wchar_t),wcslen(message),fp);

	swprintf(message, _T("[Name] Type : [Value] %s\n"), T2CW(strType));
	if( fp )
		fwrite(message,sizeof(wchar_t),wcslen(message),fp);

	swprintf(message, _T("[Name] FX : [Value] %s\n"), T2CW(strFX));
	if( fp )
		fwrite(message,sizeof(wchar_t),wcslen(message),fp);

	swprintf(message, _T("[Name] BOM : [Value] %s\n"), T2CW(strBOM));
	if( fp )
		fwrite(message,sizeof(wchar_t),wcslen(message),fp);

	map<wstring, Model_Info*>::iterator it = m_ModelInfoMapping.begin();
	wstring wskey = strP_ID.GetBuffer();
	it = m_ModelInfoMapping.find(wskey);
	if(it != m_ModelInfoMapping.end())
	{
		Model_Info* model_info = it->second;

		for (size_t i=0; i<model_info->AttrNames.size(); i++)
		{
			swprintf(message, _T("[Name] %s : [Value] %s\n"), (model_info->AttrNames[i]).c_str(), (model_info->AttrValues[i]).c_str());
			if( fp )
				fwrite(message,sizeof(wchar_t),wcslen(message),fp);
		}
	}

	if( fp )
		fclose(fp);


	//CListCtrl
	CListCtrl* listctrl = (CListCtrl*)GetDlgItem(IDC_LIST_BOM);
	listctrl->DeleteAllItems();

	listctrl->InsertItem(LVIF_TEXT | LVIF_STATE, 0, m_BOMInfo.szPartName, 0, 0, 0, 0);

	listctrl->SetItemText(0, 1, m_BOMInfo.szPartNumber);
	CString strNumber;
	CEdit* bomNumber_edit = (CEdit*)GetDlgItem(IDC_EDIT_BOM_NUMBER);
	bomNumber_edit->GetWindowText(strNumber);
	listctrl->SetItemText(0, 2, strNumber);
	listctrl->SetItemText(0, 3, m_BOMInfo.szDelivery);
	listctrl->SetItemText(0, 4, m_BOMInfo.szMaterial);
	listctrl->SetItemText(0, 5, m_BOMInfo.szPhoto);
	listctrl->SetItemText(0, 6, m_BOMInfo.szRemark);
	listctrl->SetItemText(0, 7, m_BOMInfo.szProductSeries);


	CEdit* modify_edit =(CEdit*)GetDlgItem(IDC_EDIT_MODIFY_STRING);
	modify_edit->SetWindowText(strFX);

	CEdit* bomString_edit =(CEdit*)GetDlgItem(IDC_EDIT_BOM_STRING);
	bomString_edit->SetWindowText(strBOM);

	CButton* checkBox = (CButton*)GetDlgItem(IDC_CHECK_MODEL_FILE);
	if (checkBox->GetCheck())
	{
		ShellExecute(NULL,_T("open"), szModelFilePath, NULL, NULL, SW_SHOWNORMAL);
	}

	checkBox = (CButton*)GetDlgItem(IDC_CHECK_PROP_FILE);
	if (checkBox->GetCheck())
	{
		ShellExecute(NULL,_T("open"), szPropertyFile, NULL, NULL, SW_SHOWNORMAL);
		if (szPropertyFilePath != szModelFilePath)
			ShellExecute(NULL,_T("open"), szPropertyFilePath, NULL, NULL, SW_SHOWNORMAL);
	}

	CString szInfo1 = g_LoadString(_T("IDS_GML_MODEL_SAVED_PATH"), GetLanguagePath()) + _T("\n");
	szInfo1 += _T("<a>");//
	szInfo1 += szModelFilePath;
	szInfo1 += _T("</a>");
	szInfo1 += _T("msmdwgtmp.sat");
	if (init==1)
	{
		szInfo1 += _T("|")+strOrder+_T(".dwg")+_T("|")+strOrder+_T("_1.dwg")+_T("|")+strOrder+_T("_2.dwg")+_T("|")+strOrder+_T("_3.dwg");
	}
	else if (init==5)
	{
		szInfo1 += _T("|msmstpfile_prt.stp");
	}
	else if (init==6)
	{
		szInfo1 += _T("|msmparasolidfile.x_t|")+strOrder+_T(".dwg")+_T("|")+strOrder+_T("_1.dwg")+_T("|")+strOrder+_T("_2.dwg")+_T("|")+strOrder+_T("_3.dwg");
	}


	CString szInfo2 = g_LoadString(_T("IDS_GML_ATTRIBUTE_DUMPED_FILE"), GetLanguagePath()) + _T("\n");
	szInfo2 += _T("<a>");
	szInfo2 += szPropertyFile;
	szInfo2 += _T("</a>");

	if (m_iMessageShow)
	{
		CMessageDlg dlg(NULL, szInfo1, szInfo2);
		dlg.DoModal();
	}

}

void CMEXSampleDlg::OnBnClickedButtonRun()
{
	// TODO: Add your control notification handler code here
	int index = m_comboMain.GetCurSel();

	switch(index)
	{
	case 0:
		{
			RunInterfaceDlg1(0);
		}
		break;
	case 1:
		{
			int index2 = m_comboSub.GetCurSel();
			RunInterfaceDlg2(index2);
		}
		break;
	case 2:
		{
			int index2 = m_comboSub.GetCurSel();
			RunInterfaceDlg2(index2+13);
		}
		break;
	case 3:
		{
			int index2 = m_comboSub.GetCurSel();
			RunInterfaceDlg2(index2+25);
		}
		break;
	case 4:
		{
			RunInterfaceDlg1(1);
		}
		break;
	case 5:
		{
			RunInterfaceDlg1(2);
		}
		break;
	case 6:
		{
			RunInterfaceDlg1(3);
		}
		break;
	case 7:
		{
			RunInterfaceDlg1(4);
		}
		break;
	case 8:
		{
			RunInterfaceDlg1(5);
		}
		break;
	case 9:
		{
			int index2 = m_comboSub.GetCurSel();
			m_iOut.oShowDlg_Menu_Mold_Jap(index2);
		}
		break;
	case 10:
		{
			int index2 = m_comboSub.GetCurSel();
			m_iOut.oShowDlg_Menu_Mold_Jap(index2+4);
		}
		break;
	case 11:
		{
			m_iOut.oShowDlg_Menu_Mold_Jap(30);
		}
		break;
	}

}

void CMEXSampleDlg::OnBnClickedButtonModify()
{
	// TODO: Add your control notification handler code here
	CString strFX;
	CEdit* modify_edit =(CEdit*)GetDlgItem(IDC_EDIT_MODIFY_STRING);
	modify_edit->GetWindowText(strFX);
	if (strFX==_T(""))
		return;
	m_iOut.oShowInterfaceDlg3(strFX);
	GenerateModel();
}

void CMEXSampleDlg::OnEnChangeEditBomNumber()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString strNumber;
	CEdit* bomNumber_edit = (CEdit*)GetDlgItem(IDC_EDIT_BOM_NUMBER);
	bomNumber_edit->GetWindowText(strNumber);
	int iNumber = _ttoi(strNumber);
	if (iNumber<1)
	{
		AfxMessageBox(_T("BOM Number should exceed 0!"));
		bomNumber_edit->SetWindowText(_T("1"));
	}

	CString strBOM;
	CEdit* bomString_edit =(CEdit*)GetDlgItem(IDC_EDIT_BOM_STRING);
	bomString_edit->GetWindowText(strBOM);
	if (strBOM==_T(""))
		return;
	
	{
		CString strTemp, strSubBOM;
		int totalpos = 0;
		strTemp.Format(_T("@%d"), iNumber);

		strSubBOM = strBOM;

		//ABCDEFGHIJKLMNOPQRSTUVWXYZ
		int pos = strSubBOM.ReverseFind('@');
		if (pos<1)
			return;
		totalpos += strSubBOM.GetLength() - pos;
		strSubBOM = strSubBOM.Left(pos);

		//ABCDEFGHIJKLMNOPQRSTUVWXYZ
		pos = strSubBOM.ReverseFind('@');
		if (pos<1)
			return;
		totalpos += strSubBOM.GetLength() - pos;
		strSubBOM = strSubBOM.Left(pos);

		//ABCDEFGHIJKLMNOPQRSTUVWXYZ
		pos = strSubBOM.ReverseFind('@');
		if (pos<1)
			return;
		totalpos += strSubBOM.GetLength() - pos;
		strSubBOM = strSubBOM.Left(pos);

		//strFX
		pos = strSubBOM.ReverseFind('@');
		if (pos<1)
			return;
		totalpos += strSubBOM.GetLength() - pos;
		strSubBOM = strSubBOM.Left(pos);

		//X
		pos = strSubBOM.ReverseFind('@');
		if (pos<1)
			return;
		totalpos += strSubBOM.GetLength() - pos;
		strSubBOM = strSubBOM.Left(pos);

		//X
		pos = strSubBOM.ReverseFind('@');
		if (pos<1)
			return;
		totalpos += strSubBOM.GetLength() - pos;
		strSubBOM = strSubBOM.Left(pos);

		//X
		pos = strSubBOM.ReverseFind('@');
		if (pos<1)
			return;
		totalpos += strSubBOM.GetLength() - pos;
		strSubBOM = strSubBOM.Left(pos);

		//Number
		pos = strSubBOM.ReverseFind('@');
		if (pos<1)
			return;
		strSubBOM = strSubBOM.Left(pos);

		strSubBOM += strTemp;

		strBOM=strBOM.Right(totalpos);

		strBOM = strSubBOM+strBOM;
	}
	bomString_edit->SetWindowText(strBOM);

	CListCtrl* listctrl = (CListCtrl*)GetDlgItem(IDC_LIST_BOM);
	listctrl->SetItemText(0, 2, strNumber);
}

void CMEXSampleDlg::OnBnClickedButtonBom()
{
	// TODO: Add your control notification handler code here
	CString strBOM;
	CEdit* bomString_edit =(CEdit*)GetDlgItem(IDC_EDIT_BOM_STRING);
	bomString_edit->GetWindowText(strBOM);
	if (strBOM==_T(""))
		return;

	if (((CButton*)GetDlgItem(IDC_CHECK_CLEAR_EXISTING_BOM))->GetCheck())
		m_iOut.oClearBom();

	m_iOut.oAddToBom(strBOM);
	m_iOut.oShowBom();
}

void CMEXSampleDlg::OnCbnSelchangeComboLangSetting()
{
	// TODO: Add your control notification handler code here
	UpdateMainLang();
	UpdateInterfaceLang();

	CString szSection = _T("Setting");
	DWORD dwSize = 256;
	CString szKey = _T("Language");

	CComboBox* lang_combo = (CComboBox*)GetDlgItem(IDC_COMBO_LANG_SETTING);
	int iLang = lang_combo->GetCurSel();
	CString szLang;
	szLang.Format(_T("%d"), iLang);
	WritePrivateProfileString(szSection,szKey,szLang,m_szSettingFile);
}

void CMEXSampleDlg::OnNMClickSyslinkHelp(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	CString szLanguagePath = GetLanguagePath();
	::HtmlHelp(NULL, g_LoadString(_T("IDS_GML_LINK_HELP"), szLanguagePath), HH_DISPLAY_TOPIC, 0); 
}

HBRUSH CMEXSampleDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (nCtlColor==CTLCOLOR_STATIC && (pWnd->GetDlgCtrlID() == IDC_STATIC_TAG_1 ||  pWnd->GetDlgCtrlID() == IDC_STATIC_TAG_2 ||
		pWnd->GetDlgCtrlID() == IDC_STATIC_TAG_3 || pWnd->GetDlgCtrlID() == IDC_STATIC_TAG_4))
	{
		CFont font;
		font.CreateFont(
			25,15,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH, _T("Calibri"));
		pDC->SelectObject(&font);
		font.DeleteObject();
	}
 	return hbr;
}

void CMEXSampleDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	CString szLanguagePath = GetLanguagePath();
	::HtmlHelp(NULL, g_LoadString(_T("IDS_GML_LINK_1"), szLanguagePath), HH_DISPLAY_TOPIC, 0); 
}

void CMEXSampleDlg::OnNMClickSyslink2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	CString szLanguagePath = GetLanguagePath();
	::HtmlHelp(NULL, g_LoadString(_T("IDS_GML_LINK_2"), szLanguagePath), HH_DISPLAY_TOPIC, 0); 
}

void CMEXSampleDlg::OnNMClickSyslink3(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	CString szLanguagePath = GetLanguagePath();
	::HtmlHelp(NULL, g_LoadString(_T("IDS_GML_LINK_3"), szLanguagePath), HH_DISPLAY_TOPIC, 0); 
}

void CMEXSampleDlg::OnNMClickSyslink4(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	CString szLanguagePath = GetLanguagePath();
	::HtmlHelp(NULL, g_LoadString(_T("IDS_GML_LINK_4"), szLanguagePath), HH_DISPLAY_TOPIC, 0); 
}

static int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM , LPARAM lpData)     
{  
	if(uMsg == BFFM_INITIALIZED)  
	{  
		CTreeCtrl   treePath;  
		HTREEITEM   hItemSel;  
		::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);  
		treePath.SubclassWindow(::GetDlgItem(hWnd, 0x3741));  
		hItemSel    = treePath.GetSelectedItem();  
		treePath.Expand(hItemSel, TVE_COLLAPSE);  
		treePath.UnsubclassWindow();  
	}   
	return 0;    
}  

void CMEXSampleDlg::SetFolderPath(UINT uid)  
{  
	BROWSEINFO   bi;     
	TCHAR   szPath[MAX_PATH];     
	LPITEMIDLIST   pList   =   NULL;     
	ZeroMemory(szPath,   MAX_PATH);    
 
	GetDlgItemText(uid, szPath, MAX_PATH);  
	memset(&bi, 0, sizeof(BROWSEINFO));  

	bi.hwndOwner   =  m_hWnd;  
	bi.pidlRoot   =   pList;     
	bi.pszDisplayName  =  szPath;     
	
	CString szLanguagePath = GetLanguagePath();
	CString szTitle = g_LoadString(_T("IDS_GML_DLG_18"), szLanguagePath);
	bi.lpszTitle   =  szTitle;  
	bi.ulFlags   =  BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_NEWDIALOGSTYLE; 
	bi.lpfn   =   BrowseCallbackProc;     
	bi.lParam   =   (LPARAM)szPath;   
	bi.iImage   =   0;  
 
	if((pList = SHBrowseForFolder(&bi))   !=   NULL)     
	{      
		if (SHGetPathFromIDList(pList,   szPath))  
		{  
			SetDlgItemText(uid, szPath);  
		}  
	}  
} 

void CMEXSampleDlg::OnBnClickedButtonModelFile()
{
	// TODO: Add your control notification handler code here
	SetFolderPath(IDC_EDIT_MODEL_FILE);  
}

void CMEXSampleDlg::OnBnClickedButtonPropertyFile()
{
	// TODO: Add your control notification handler code here
	SetFolderPath(IDC_EDIT_PROPERTY_FILE); 
}
