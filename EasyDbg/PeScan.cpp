// PeScan.cpp : implementation file
//

#include "stdafx.h"
#include "EasyDbg.h"
#include "PeScan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPeScan dialog

//供导入表模块使用
extern PIMAGE_SECTION_HEADER g_SecHeader=NULL;
extern DWORD g_SecNum=0;

extern DWORD g_dwImportRva=0;

//用于传给导入导出表模块
extern char* g_pFile=NULL;

extern DWORD g_dwExportRva=0;


//从调试线程传来 被调试程序的映射基址
extern char* pFile;

CPeScan::CPeScan(CWnd* pParent /*=NULL*/)
	: CDialog(CPeScan::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPeScan)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    memset(m_FilePath,0,sizeof(m_FilePath));
    m_pFile=NULL;
    m_pDos=NULL;
    m_pFileHeader=NULL;
    m_pOption=NULL;
    m_Section=NULL;
}


void CPeScan::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPeScan)
	DDX_Control(pDX, IDC_LIST1, m_SecList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPeScan, CDialog)
	//{{AFX_MSG_MAP(CPeScan)
	ON_BN_CLICKED(BTN_SELECT, OnSelect)
	ON_BN_CLICKED(BTN_IMPORT, OnImport)
	ON_BN_CLICKED(BTN_EXPORT, OnExport)
    ON_WM_DROPFILES()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPeScan message handlers

void CPeScan::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
}

//选择文件
void CPeScan::OnSelect() 
{
	// TODO: Add your control notification handler code here
    OPENFILENAME file = {0} ;
    file.lpstrFile = m_FilePath ;
    file.lStructSize = sizeof(OPENFILENAME) ;
    file.nMaxFile = 256 ;
    file.lpstrFilter = "Executables\0*.exe\0All Files\0*.*\0\0" ;
    file.nFilterIndex = 1 ;
    
    if(!::GetOpenFileName(&file))
    {
        //点了取消按钮就退出函数
        return;
    }
    SetDlgItemText(IDC_FILEPATH,m_FilePath);
    if (!MapPEFile())
    {
        AfxMessageBox("文件映射失败");
        return;
    }

    GetBasicPEInfo();


	
}

BOOL CPeScan::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
 

    m_SecList.InsertColumn(0,TEXT("区块名"),LVCFMT_LEFT,70);
    m_SecList.InsertColumn(1,TEXT("VirtualAddress"),LVCFMT_LEFT,100);
    m_SecList.InsertColumn(2,TEXT("VirtualSize"),LVCFMT_LEFT,80);
    m_SecList.InsertColumn(3,TEXT("PointerToRawData"),LVCFMT_LEFT,100);
    m_SecList.InsertColumn(4,TEXT("SizeOfRawData"),LVCFMT_LEFT,100);
  
    
    m_SecList.SetExtendedStyle(m_SecList.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
    //默认显示被调试程序的信息
    if (pFile!=NULL)
    {
     
      m_pFile=pFile;
      g_pFile=pFile;
      GetBasicPEInfo();
      
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//映射PE文件
BOOL CPeScan::MapPEFile()
{
    HANDLE hFile=NULL;
    //打开文件获得文件句柄
    hFile=CreateFile(m_FilePath,GENERIC_ALL,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
    if (hFile==INVALID_HANDLE_VALUE)
    {
        DWORD dwErrorCode=GetLastError();
        AfxMessageBox("打开文件失败");
        return FALSE;
    }
    HANDLE hFileMap=NULL;
    //创建文件映射
    hFileMap=CreateFileMapping(hFile,NULL,PAGE_READWRITE,0,0,NULL);
    if (hFileMap==NULL)
    {
        AfxMessageBox("创建内存映射失败");
        CloseHandle(hFile);
        return FALSE;
    }
    //映射文件
    m_pFile=(char*)MapViewOfFile(hFileMap,FILE_MAP_ALL_ACCESS,0,0,0);
    if (m_pFile==NULL)
    {
        AfxMessageBox("映射失败");
        CloseHandle(hFile);
        CloseHandle(hFileMap);
        return FALSE;
    }
    CloseHandle(hFile);
    CloseHandle(hFileMap);
    g_pFile=m_pFile;

   return TRUE;

}


//获得PE基本相关信息 并判断是否是有效PE
BOOL CPeScan::GetBasicPEInfo()
{

   //获取各结构的指针
    m_pDos=(PIMAGE_DOS_HEADER)m_pFile;
    PIMAGE_NT_HEADERS pNt=(PIMAGE_NT_HEADERS)(m_pFile+m_pDos->e_lfanew);
    m_pFileHeader=(PIMAGE_FILE_HEADER)(m_pFile+m_pDos->e_lfanew+4);
    m_pOption=(PIMAGE_OPTIONAL_HEADER)((char*)m_pFileHeader+sizeof(IMAGE_FILE_HEADER));
    m_Section=(PIMAGE_SECTION_HEADER)((char*)m_pOption+m_pFileHeader->SizeOfOptionalHeader);

    g_SecHeader=m_Section;

    //检查MZ PE 两个标志
    if (m_pDos->e_magic!=IMAGE_DOS_SIGNATURE || pNt->Signature!=IMAGE_NT_SIGNATURE)
    {
        AfxMessageBox("不是有效的PE文件");
        return FALSE;
    }
    CString szText;
    //节数目
    szText.Format("%04X",m_pFileHeader->NumberOfSections);
    SetDlgItemText(IDC_SECTIONNUM,szText);

    g_SecNum=m_pFileHeader->NumberOfSections;

    //选项头大小
    szText.Format("%04X",m_pFileHeader->SizeOfOptionalHeader);
    SetDlgItemText(IDC_OPTION,szText);
    //OEP
    szText.Format("%08X",m_pOption->AddressOfEntryPoint);
    SetDlgItemText(IDC_ENTRY,szText);
    //ImageBase
    szText.Format("%08X",m_pOption->ImageBase);
    SetDlgItemText(IDC_IMAGE,szText);
    //SizeofImage
    szText.Format("%08X",m_pOption->SizeOfImage);
    SetDlgItemText(IDC_IMAGESIZE,szText);
    //BaseOfCode
    szText.Format("%08X",m_pOption->BaseOfCode);
    SetDlgItemText(IDC_CODEBASE,szText);
    //BaseOfData
    szText.Format("%08X",m_pOption->BaseOfData);
    SetDlgItemText(IDC_DATABASE,szText);
    //SectionAlignment
    szText.Format("%08X",m_pOption->SectionAlignment);
    SetDlgItemText(IDC_SECTIONALIGNMENT,szText);
    //FileAlignment
    szText.Format("%08X",m_pOption->FileAlignment);
    SetDlgItemText(IDC_FILEALIGNMENT,szText);
    //Subsystem
    szText.Format("%04X",m_pOption->Subsystem);
    SetDlgItemText(IDC_SUBSYSTEM,szText);
    //CheckSum

    szText.Format("%08X",m_pOption->CheckSum);
    SetDlgItemText(IDC_CHECKSUM,szText);
    //SizeOfHeaders
    szText.Format("%08X",m_pOption->SizeOfHeaders);
    SetDlgItemText(IDC_SIZEOFHEADER,szText);
    //EXPORT
    szText.Format("%08X",m_pOption->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    SetDlgItemText(IDC_EXPORTRVA,szText);
    szText.Format("%08X",m_pOption->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size);
    SetDlgItemText(IDC_EXPORTSIZE,szText);
    //import
    szText.Format("%08X",m_pOption->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
    SetDlgItemText(IDC_IMPORTRVA,szText);
    szText.Format("%08X",m_pOption->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size);
    SetDlgItemText(IDC_IMPORTSIZE,szText);
    
    //赋值
    g_dwImportRva=m_pOption->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;

    g_dwExportRva=m_pOption->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

    if (m_pOption->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress==0)
    {
        //如果没有导出表让其按钮变灰
        GetDlgItem(BTN_EXPORT)->EnableWindow(FALSE);
    }
    else
    {
        GetDlgItem(BTN_EXPORT)->EnableWindow(TRUE);

    }

    if (m_pOption->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress==0)
    {
        //如果没有导入表让其按钮变灰
        GetDlgItem(BTN_IMPORT)->EnableWindow(FALSE);
    }

    else
    {
        GetDlgItem(BTN_IMPORT)->EnableWindow(TRUE);

    }


    


    m_SecList.DeleteAllItems();
    //区块信息
    for (int i=0;i<m_pFileHeader->NumberOfSections;i++)
    {
        

        //区块名
        szText.Format("%s",m_Section[i].Name);
        m_SecList.InsertItem(i,szText);
        //RVA
        szText.Format("%08X",m_Section[i].VirtualAddress);
        m_SecList.SetItemText(i,1,szText);
        //V大小
        szText.Format("%08X",m_Section[i].Misc.VirtualSize);
        m_SecList.SetItemText(i,2,szText);
        //文件偏移
        szText.Format("%08X",m_Section[i].PointerToRawData);
        m_SecList.SetItemText(i,3,szText);
        //R大小
        szText.Format("%08X",m_Section[i].SizeOfRawData);
        m_SecList.SetItemText(i,4,szText);



    }



    return TRUE;
}


//查看导出表
void CPeScan::OnImport() 
{
	// TODO: Add your control notification handler code here
    CImportTable dlg;
    dlg.DoModal();
	
}

//查看导出表
void CPeScan::OnExport() 
{
	// TODO: Add your control notification handler code here

    CExportTable dlg;
    dlg.DoModal();
	
}

void CPeScan::OnDropFiles( HDROP hDropInfo )
{
  
    
    ::DragQueryFile 
        (hDropInfo,0,m_FilePath,sizeof(m_FilePath)); //0表示取第一个被拖拽的文件名
    SetDlgItemText(IDC_FILEPATH,m_FilePath); 
    
    ::DragFinish (hDropInfo); //释放内存

    if (!MapPEFile())
    {
        AfxMessageBox("文件映射失败");
        return;
    }


    GetBasicPEInfo();

   
      
}


//清理工作 把映射撤销
void CPeScan::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
    //被调试进程的映射就不撤销了
//     if (pFile!=NULL)
//     {
//         UnmapViewOfFile(pFile);
//         pFile=NULL;
//     }
    if (m_pFile!=NULL)
    {
        //如果是被调试进程的映射基址的话就不撤销
        if (m_pFile!=pFile)
        {
            UnmapViewOfFile(m_pFile);
           m_pFile=NULL;
        }

    }
	
	CDialog::OnClose();
}
