#if !defined(AFX_PROCESSMODULE_H__5B871989_1246_4A5B_B747_BA2AD2C31B9E__INCLUDED_)
#define AFX_PROCESSMODULE_H__5B871989_1246_4A5B_B747_BA2AD2C31B9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProcessModule.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProcessModule dialog

class CProcessModule : public CDialog
{
// Construction
public:
	CProcessModule(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProcessModule)
	enum { IDD = IDD_DIALOG1 };
	CListCtrl	m_ModuleList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProcessModule)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
    public:
    //ÁÐ¾ÙÄ£¿é
    void  ListModules();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProcessModule)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSMODULE_H__5B871989_1246_4A5B_B747_BA2AD2C31B9E__INCLUDED_)
