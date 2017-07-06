// Help.cpp : implementation file
//

#include "stdafx.h"
#include "EasyDbg.h"
#include "Help.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHelp dialog


CHelp::CHelp(CWnd* pParent /*=NULL*/)
	: CDialog(CHelp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHelp)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CHelp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHelp)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHelp, CDialog)
	//{{AFX_MSG_MAP(CHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelp message handlers

void CHelp::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
}
