
// MEXSampleDlg.h : header file
//

#pragma once
#include "COut.h"
#include "TTComboBox.h"
#include <string>
#include <map>
#include <vector>

using namespace std;
class CMEXSampleDlgAutoProxy;


struct MiBOMInfo
{
	CString szPartName;
	CString szPartNumber;
	CString szDelivery;
	CString szMaterial;
	CString szPhoto;
	CString szRemark;
	CString szProductSeries;
};

class Model_Info
{
public:
//	wstring _TYPE;
	wstring _P_ID;
	vector<wstring> AttrNames;
	vector<wstring> AttrValues;
public:
	Model_Info()
	{
	}
	~Model_Info(void)
	{
		AttrNames.clear();
		AttrValues.clear();
	}

protected:
private:
};



// CMEXSampleDlg dialog
class CMEXSampleDlg : public CDialog
{
	DECLARE_DYNAMIC(CMEXSampleDlg);
	friend class CMEXSampleDlgAutoProxy;

// Construction
public:
	CMEXSampleDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CMEXSampleDlg();

// Dialog Data
	enum { IDD = IDD_MEXSAMPLE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	CMEXSampleDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

protected:
	//Display the first class interface of Mold EX-Press for Mold
	void RunInterfaceDlg1(int i);

	//Display the second class interface of Mold EX-Press for Mold for each category
	void RunInterfaceDlg2(int i);

	//Generate model
	void GenerateModel();

	//Update language for main combo box in "Import & Setting"
	void UpdateMainLang();

	//Update language for MEXSample interface
	void UpdateInterfaceLang();

	CString GetLanguagePath();

	void SetFolderPath(UINT uid);

	CString m_szLangFile;
	CString m_szGPLPath;
	CString m_szCurPath;
	MiBOMInfo m_BOMInfo;
	CBrush m_brush;
	int m_iMessageShow;

	CBitmap		m_bitmap;

	CString m_szSettingFile;

	CTTComboBox m_comboMain;
	CTTComboBox m_comboSub;
	map<wstring, Model_Info*> m_ModelInfoMapping;

public:
	COut m_iOut;
	afx_msg void OnCbnSelchangeComboModuleInit();
	afx_msg void OnBnClickedButtonModuleInit();
	afx_msg void OnCbnSelchangeComboMain();
	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnEnChangeEditBomNumber();
	afx_msg void OnBnClickedButtonBom();
	afx_msg void OnCbnSelchangeComboLangSetting();
//	afx_msg void OnBnClickedButtonLangDefault();
	afx_msg void OnNMClickSyslinkHelp(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickSyslink2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickSyslink3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickSyslink4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonModelFile();
	afx_msg void OnBnClickedButtonPropertyFile();
};
