// ExportTable.cpp : implementation file
//

#include "stdafx.h"
#include "EasyDbg.h"
#include "ExportTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExportTable dialog

extern DWORD g_dwExportRva;

extern PIMAGE_SECTION_HEADER g_SecHeader;
extern DWORD g_SecNum;

extern char* g_pFile;


CExportTable::CExportTable(CWnd* pParent /*=NULL*/)
	: CDialog(CExportTable::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExportTable)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CExportTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportTable)
	DDX_Control(pDX, IDC_LIST1, m_expList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExportTable, CDialog)
	//{{AFX_MSG_MAP(CExportTable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportTable message handlers

void CExportTable::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
}

BOOL CExportTable::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

    UIinit();
    GetExportInfo();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//界面初始化
void CExportTable::UIinit()
{
    m_expList.InsertColumn(0,TEXT("序号"),LVCFMT_LEFT,80);
    m_expList.InsertColumn(1,TEXT("RVA"),LVCFMT_LEFT,80);
    m_expList.InsertColumn(2,TEXT("文件偏移"),LVCFMT_LEFT,100);
    m_expList.InsertColumn(3,TEXT("名称"),LVCFMT_LEFT,260);
    
    m_expList.SetExtendedStyle(m_expList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
  


}

//获得导出表信息
void CExportTable::GetExportInfo()
{
    m_expList.DeleteAllItems();

    DWORD dwExportOffset=0;
    //获得导入表的文件偏移
    dwExportOffset=RvaToFileOffset(g_dwExportRva,g_SecNum,g_SecHeader);
    PIMAGE_EXPORT_DIRECTORY pExp=NULL;
    pExp=(PIMAGE_EXPORT_DIRECTORY)(g_pFile+dwExportOffset);
    CString szText;
    szText.Format("%08X",pExp->Characteristics);
    SetDlgItemText(IDC_Characteristics,szText);
    szText.Format("%08X",pExp->TimeDateStamp);
    SetDlgItemText(IDC_TimeDateStamp,szText);
    szText.Format("%08X",pExp->Name);
    SetDlgItemText(IDC_NAMERVA,szText);
    szText.Format("%08X",pExp->Base);
    SetDlgItemText(IDC_BASE,szText);

    szText.Format("%08X",pExp->NumberOfFunctions);
    SetDlgItemText(IDC_NumberOfFunctions,szText);
    szText.Format("%08X",pExp->NumberOfNames);
    SetDlgItemText(IDC_NumberOfNames,szText);
    szText.Format("%08X",pExp->AddressOfFunctions);
    SetDlgItemText(IDC_AddressOfFunctions,szText);
    szText.Format("%08X",pExp->AddressOfNameOrdinals);
    SetDlgItemText(IDC_AddressOfNameOrdinals,szText);

    szText.Format("%08X",pExp->AddressOfNames);
    SetDlgItemText(IDC_AddressOfNames,szText);
    
    DWORD dwNameOffset=0;
    dwNameOffset=RvaToFileOffset(pExp->Name,g_SecNum,g_SecHeader);
    char*pName=NULL;
    pName=(char*)(g_pFile+dwNameOffset);
    szText.Format("%s",pName);
    SetDlgItemText(IDC_NAME,szText);

    DWORD dwBase=0;
    dwBase=pExp->Base;
    for (DWORD j=0;j<pExp->NumberOfFunctions;j++)
    {
        //先遍历函数地址数组
        PDWORD pAddr=(PDWORD)(g_pFile+RvaToFileOffset(pExp->AddressOfFunctions,g_SecNum,g_SecHeader));
        //地址有效
        if (pAddr[j]!=0)
        {
            szText.Format("%04X",dwBase+j);
            m_expList.InsertItem(j,szText);
            szText.Format("%08X",pAddr[j]);
            m_expList.SetItemText(j,1,szText);
            szText.Format("%08X",RvaToFileOffset(pAddr[j],g_SecNum,g_SecHeader));
            m_expList.SetItemText(j,2,szText);
            //通过序号得到相应函数名数组下标
            //序号数组
            PWORD pNum=(PWORD)(g_pFile+RvaToFileOffset(pExp->AddressOfNameOrdinals,g_SecNum,g_SecHeader));
            for (WORD k=0;k<pExp->NumberOfNames;k++ )
            {
                //在序号数组里找序号相同的 找到下标然后读函数名
                if (j==pNum[k])
                {
                    //导出函数名(或变量名数组) 得到的是RVA
                    PDWORD pName=(PDWORD)(g_pFile+RvaToFileOffset(pExp->AddressOfNames,g_SecNum,g_SecHeader));
                    //注意要转换为文件偏移在读取名字
                    char *pszName=(char*)(g_pFile+RvaToFileOffset(pName[k],g_SecNum,g_SecHeader));
                    szText.Format("%s",pszName);
                    m_expList.SetItemText(j,3,szText);
                    
                    break;
                }
            }
            
            
        }
        
    }




  

}


//参数一 导入表的RVA 参数2区块表的数目 参数3区块表的首地址
DWORD CExportTable::RvaToFileOffset(DWORD dwRva,DWORD dwSecNum,PIMAGE_SECTION_HEADER pSec)
{
    if (dwSecNum==0)
    {
        return 0;
    }
    
    for (DWORD i=0;i<dwSecNum;i++)
    {
        
        if (dwRva>=pSec[i].VirtualAddress&&dwRva<pSec[i].VirtualAddress+pSec[i].SizeOfRawData)
        {
            
            return dwRva-pSec[i].VirtualAddress+pSec[i].PointerToRawData;
            
        }
    }
    return 0;
    
}


