
// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "MEXSample.h"
#include "DlgProxy.h"
#include "MEXSampleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMEXSampleDlgAutoProxy

IMPLEMENT_DYNCREATE(CMEXSampleDlgAutoProxy, CCmdTarget)

CMEXSampleDlgAutoProxy::CMEXSampleDlgAutoProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CMEXSampleDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CMEXSampleDlg)))
		{
			m_pDialog = reinterpret_cast<CMEXSampleDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CMEXSampleDlgAutoProxy::~CMEXSampleDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CMEXSampleDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CMEXSampleDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CMEXSampleDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// Note: we add support for IID_IMEXSample to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {D91792D8-9C66-4C63-9FBD-B888B8652F52}
static const IID IID_IMEXSample =
{ 0xD91792D8, 0x9C66, 0x4C63, { 0x9F, 0xBD, 0xB8, 0x88, 0xB8, 0x65, 0x2F, 0x52 } };

BEGIN_INTERFACE_MAP(CMEXSampleDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CMEXSampleDlgAutoProxy, IID_IMEXSample, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {46A67CF8-8865-4DF4-9394-4CDC8BA24EF7}
IMPLEMENT_OLECREATE2(CMEXSampleDlgAutoProxy, "MEXSample.Application", 0x46a67cf8, 0x8865, 0x4df4, 0x93, 0x94, 0x4c, 0xdc, 0x8b, 0xa2, 0x4e, 0xf7)


// CMEXSampleDlgAutoProxy message handlers
