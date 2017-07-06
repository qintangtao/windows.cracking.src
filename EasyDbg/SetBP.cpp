// SetBP.cpp : implementation file
//

#include "stdafx.h"
#include "EasyDbg.h"
#include "SetBP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSetBP dialog


CSetBP::CSetBP(CWnd* pParent /*=NULL*/)
	: CDialog(CSetBP::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetBP)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_dwLength=0;
    m_dwBpAddress=0;
    m_Select=0;
    m_dwAttribute=0;

}


void CSetBP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetBP)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetBP, CDialog)
	//{{AFX_MSG_MAP(CSetBP)
	ON_BN_CLICKED(IDC_INT3, OnInt3)
	ON_BN_CLICKED(IDC_HARD, OnHard)
	ON_BN_CLICKED(IDC_MEMORY, OnMemory)
	ON_BN_CLICKED(BTN_OK, OnOk)
	ON_BN_CLICKED(BTN_CANCLE, OnCancle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetBP message handlers



//设置INT3断点
void CSetBP::OnInt3() 
{
	// TODO: Add your control notification handler code here
    //使长度和类型编辑框变无效
    GetDlgItem(IDC_LENGTH)->EnableWindow(FALSE);
    GetDlgItem(IDC_ATTRIBUTE)->EnableWindow(FALSE);
    m_Select=1;
	
}

//设置硬件断点
void CSetBP::OnHard() 
{
	// TODO: Add your control notification handler code here
    GetDlgItem(IDC_LENGTH)->EnableWindow(TRUE);
    GetDlgItem(IDC_ATTRIBUTE)->EnableWindow(TRUE);
    m_Select=2;
	
}
//设置内存断点
void CSetBP::OnMemory() 
{
	// TODO: Add your control notification handler code here
    GetDlgItem(IDC_LENGTH)->EnableWindow(TRUE);
    GetDlgItem(IDC_ATTRIBUTE)->EnableWindow(TRUE);
    m_Select=3;
	
}

void CSetBP::OnOk() 
{
	// TODO: Add your control notification handler code here

    //把输入的数据当做16进制处理,也就是只接受16进制数据 
    char szBuffer[40];
    GetDlgItemText(IDC_ADDRESS,szBuffer,sizeof(szBuffer));
    
    //提取16进制数据  获得断点地址
    sscanf(szBuffer, "%x", &m_dwBpAddress) ; //因为有%x所以会把输入的数据转换成16进制
    
    GetDlgItemText(IDC_LENGTH,szBuffer,sizeof(szBuffer));
    
    //提取16进制数据  获得断点长度
    sscanf(szBuffer, "%x", &m_dwLength) ; 
    //获得断点类型
    GetDlgItemText(IDC_ATTRIBUTE,szBuffer,sizeof(szBuffer));
    sscanf(szBuffer,"%x",&m_dwAttribute);

    if (m_dwBpAddress==0)
    {
        AfxMessageBox("没有填入断点地址");
        return;
    }
    if (m_Select==3||m_Select==2)
    {
        if (m_dwLength==0)
        {
            AfxMessageBox("没有填入长度");
            return;
        }
    }
    
    CDialog::OnOK();
  
	
}

void CSetBP::OnCancle() 
{
	// TODO: Add your control notification handler code here
      CDialog::OnCancel();
	
}
