#if !defined(AFX_IMPORTTABLE_H__C5930AF1_9DBA_4787_9753_F07FDE714995__INCLUDED_)
#define AFX_IMPORTTABLE_H__C5930AF1_9DBA_4787_9753_F07FDE714995__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImportTable.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImportTable dialog

class CImportTable : public CDialog
{
// Construction
public:
	CImportTable(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CImportTable)
	enum { IDD = IDD_DIALOG4 };
	CListCtrl	m_FunList;
	CListCtrl	m_DllList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImportTable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

    public:
        //初始化界面
        void UIinit();
        //由RVA计算文件偏移
        DWORD RvaToFileOffset(DWORD dwRva,DWORD dwSecNum,PIMAGE_SECTION_HEADER pSec);
        //枚举导入的DLL
        void ListDll();
        //列出某一个DLL的导入函数
        void ListFun(PIMAGE_THUNK_DATA pThunK);
        //存放PIMAGE_THUNK_DATA的值的数组
         DWORD m_dwThunk[100];

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImportTable)
	virtual BOOL OnInitDialog();
    virtual void OnOK();
	afx_msg void OnClickDll(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMPORTTABLE_H__C5930AF1_9DBA_4787_9753_F07FDE714995__INCLUDED_)
