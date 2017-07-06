#if !defined(AFX_EXPORTTABLE_H__14C9EA6F_5FB9_4B38_BEBA_0FA80DEA6D1A__INCLUDED_)
#define AFX_EXPORTTABLE_H__14C9EA6F_5FB9_4B38_BEBA_0FA80DEA6D1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportTable.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExportTable dialog

class CExportTable : public CDialog
{
// Construction
public:
	CExportTable(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExportTable)
	enum { IDD = IDD_DIALOG5 };
	CListCtrl	m_expList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExportTable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
    public:
        //界面初始化
        void UIinit();
        //获得导出表信息
        void GetExportInfo();
        //RVA转文件偏移
        DWORD RvaToFileOffset(DWORD dwRva,DWORD dwSecNum,PIMAGE_SECTION_HEADER pSec);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExportTable)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPORTTABLE_H__14C9EA6F_5FB9_4B38_BEBA_0FA80DEA6D1A__INCLUDED_)
