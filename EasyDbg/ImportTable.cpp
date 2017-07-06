// ImportTable.cpp : implementation file
//

#include "stdafx.h"
#include "EasyDbg.h"
#include "ImportTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImportTable dialog

extern PIMAGE_SECTION_HEADER g_SecHeader;
extern DWORD g_SecNum;

extern char* g_pFile;

extern DWORD g_dwImportRva;

CImportTable::CImportTable(CWnd* pParent /*=NULL*/)
	: CDialog(CImportTable::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImportTable)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    memset(m_dwThunk,0,sizeof(m_dwThunk));
}


void CImportTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportTable)
	DDX_Control(pDX, IDC_FUN, m_FunList);
	DDX_Control(pDX, IDC_DLL, m_DllList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportTable, CDialog)
	//{{AFX_MSG_MAP(CImportTable)
	ON_NOTIFY(NM_CLICK, IDC_DLL, OnClickDll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportTable message handlers

void CImportTable::OnOK() 
{
    // TODO: Add extra validation here
    
    
    
    //CDialog::OnOK();
}

BOOL CImportTable::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	UIinit();
    ListDll();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CImportTable::UIinit()
{
    m_DllList.InsertColumn(0,TEXT("DLL名"),LVCFMT_LEFT,100);
    m_DllList.InsertColumn(1,TEXT("OriginalFirstThunk"),LVCFMT_LEFT,120);
    m_DllList.InsertColumn(2,TEXT("TimeDateStamp"),LVCFMT_LEFT,100);
    m_DllList.InsertColumn(3,TEXT("ForwarderChain"),LVCFMT_LEFT,120);
    m_DllList.InsertColumn(4,TEXT("Name"),LVCFMT_LEFT,70);
    m_DllList.InsertColumn(5,TEXT("FirstThunk"),LVCFMT_LEFT,120);

    m_DllList.SetExtendedStyle(m_DllList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

    m_FunList.InsertColumn(0,TEXT("序号"),LVCFMT_LEFT,70);
    m_FunList.InsertColumn(1,TEXT("API名称"),LVCFMT_LEFT,180);
    m_FunList.SetExtendedStyle(m_FunList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);    
}


//参数一 导入表的RVA 参数2区块表的数目 参数3节表的首地址
DWORD CImportTable::RvaToFileOffset(DWORD dwRva,DWORD dwSecNum,PIMAGE_SECTION_HEADER pSec)
{
    if (dwSecNum==0 || pSec==NULL)
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

//枚举导入的DLL
void CImportTable::ListDll()
{
    m_DllList.DeleteAllItems();
    
    DWORD dwImportFileOffset=0;
    dwImportFileOffset=RvaToFileOffset(g_dwImportRva,g_SecNum,g_SecHeader);
    if (dwImportFileOffset==0)
    {
        AfxMessageBox("函数出错");
        return;
    }
    //获得导入表结构
    PIMAGE_IMPORT_DESCRIPTOR pImport=(PIMAGE_IMPORT_DESCRIPTOR)(g_pFile+dwImportFileOffset);
    CString szText;
    //全0 的结构体表示结束
    int i=0;
    while (pImport->Name)
    {
        DWORD dwNameOffset=RvaToFileOffset(pImport->Name,g_SecNum,g_SecHeader);
        char *pName=(char*)(g_pFile+dwNameOffset);
        szText.Format("%s",pName);
        m_DllList.InsertItem(i,szText);
        szText.Format("%08X",pImport->OriginalFirstThunk);
        m_DllList.SetItemText(i,1,szText);
        szText.Format("%08X",pImport->TimeDateStamp);
        m_DllList.SetItemText(i,2,szText);
        szText.Format("%08X",pImport->ForwarderChain);
        m_DllList.SetItemText(i,3,szText);
        szText.Format("%08X",pImport->Name);
        m_DllList.SetItemText(i,4,szText);
        szText.Format("%08X",pImport->FirstThunk);
        m_DllList.SetItemText(i,5,szText);
        DWORD dwDataHunk=RvaToFileOffset(pImport->OriginalFirstThunk,g_SecNum,g_SecHeader);
        PIMAGE_THUNK_DATA pThunK=(PIMAGE_THUNK_DATA)(g_pFile+dwDataHunk);
        //保存pThunk的值
        m_dwThunk[i]=(DWORD)pThunK;

        pImport++;
        i++;
    }



}





//单击列表控件消息
void CImportTable::OnClickDll(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    //得到位置

    POSITION pos=NULL;
    pos=m_DllList.GetFirstSelectedItemPosition();
    
    if (pos==NULL)
    {
        return;
    }
    
    int nIndex=m_DllList.GetNextSelectedItem(pos);
    PIMAGE_THUNK_DATA pThunK=(PIMAGE_THUNK_DATA)m_dwThunk[nIndex];
    ListFun(pThunK);
    
	*pResult = 0;
}


//列出某一个DLL的导入函数
void CImportTable::ListFun(PIMAGE_THUNK_DATA pThunK)
{
    m_FunList.DeleteAllItems();
    PIMAGE_IMPORT_BY_NAME pDllName=NULL;
    CString szText;
    //IMAGE_THUNK_DATA数组以全0结束
    int i=0;
    while(pThunK->u1.AddressOfData)
    {
        pDllName=(PIMAGE_IMPORT_BY_NAME)(g_pFile+RvaToFileOffset((DWORD)pThunK->u1.AddressOfData,g_SecNum,g_SecHeader));
        //判断函数是不是只由序号导出
        if (*(DWORD*)pThunK&IMAGE_ORDINAL_FLAG32)
        {
            
            szText.Format("%04X",*(WORD*)pThunK);
            m_FunList.InsertItem(i,szText);
            m_FunList.SetItemText(i,1,TEXT("函数按序号导出"));
            i++;
            pThunK++;
            continue;
        }
        szText.Format("%04X",pDllName->Hint);
        m_FunList.InsertItem(i,szText);
        
        szText.Format("%s",pDllName->Name);
        m_FunList.SetItemText(i,1,szText);
        pThunK++;
        i++;
    }


}
