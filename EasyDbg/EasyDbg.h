// EasyDbg.h : main header file for the EASYDBG application
//

#if !defined(AFX_EASYDBG_H__85F0D73C_8AEA_465C_A797_1D1C16F46173__INCLUDED_)
#define AFX_EASYDBG_H__85F0D73C_8AEA_465C_A797_1D1C16F46173__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols



/////////////////////////////////////////////////////////////////////////////
// CEasyDbgApp:
// See EasyDbg.cpp for the implementation of this class
//

class CEasyDbgApp : public CWinApp
{
public:
	CEasyDbgApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEasyDbgApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CEasyDbgApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EASYDBG_H__85F0D73C_8AEA_465C_A797_1D1C16F46173__INCLUDED_)
