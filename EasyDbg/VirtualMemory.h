#if !defined(AFX_VIRTUALMEMORY_H__A0554930_507B_44CA_B5E7_6E55AD59B6B8__INCLUDED_)
#define AFX_VIRTUALMEMORY_H__A0554930_507B_44CA_B5E7_6E55AD59B6B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VirtualMemory.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVirtualMemory dialog

class CVirtualMemory : public CDialog
{
// Construction
public:
	CVirtualMemory(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVirtualMemory)
	enum { IDD = IDD_VIRTUALMEMORY };
	CListCtrl	m_MemoryList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVirtualMemory)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
    public:
        //ÁÐ¾ÙÄÚ´æ
       void  ListVirtualMemory();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVirtualMemory)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIRTUALMEMORY_H__A0554930_507B_44CA_B5E7_6E55AD59B6B8__INCLUDED_)
