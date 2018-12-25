
// DlgProxy.h: header file
//

#pragma once

class CMEXSampleDlg;


// CMEXSampleDlgAutoProxy command target

class CMEXSampleDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CMEXSampleDlgAutoProxy)

	CMEXSampleDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CMEXSampleDlg* m_pDialog;

// Operations
public:

// Overrides
	public:
	virtual void OnFinalRelease();

// Implementation
protected:
	virtual ~CMEXSampleDlgAutoProxy();

	// Generated message map functions

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CMEXSampleDlgAutoProxy)

	// Generated OLE dispatch map functions

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

