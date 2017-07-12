#include "ntddk.h"

#pragma pack(1)
typedef struct ServiceDescriptorEntry {
	unsigned int *ServiceTableBase;
	unsigned int *ServiceCounterTableBase; 
	unsigned int NumberOfServices;
	unsigned char *ParamTableBase;
} ServiceDescriptorTableEntry_t, *PServiceDescriptorTableEntry_t;
#pragma pack()

NTSTATUS
	PsLookupProcessByProcessId(
	IN HANDLE ProcessId,
	OUT PEPROCESS *Process
	);
__declspec(dllimport) ServiceDescriptorTableEntry_t KeServiceDescriptorTable;

typedef NTSTATUS(*MYNTOPENPROCESS)(
	OUT PHANDLE             ProcessHandle,
	IN ACCESS_MASK          AccessMask,
	IN POBJECT_ATTRIBUTES   ObjectAttributes,
	IN PCLIENT_ID           ClientId );//定义一个指针函数，用于下面对O_NtOpenProcess进行强制转换
ULONG O_NtOpenProcess;

BOOLEAN ProtectProcess(HANDLE ProcessId,char *str_ProtectObjName)
{
	NTSTATUS status;
	PEPROCESS process_obj;
	if(!MmIsAddressValid(str_ProtectObjName))//这个条件是用来判断目标进程名是否有效
	{
		return FALSE;
	}
	if(ProcessId==0)//这个条件是用来排除System Idle Process进程的干扰
	{
		return FALSE;
	}
	status=PsLookupProcessByProcessId(ProcessId,&process_obj);//这句用来获取目标进程的EPROCESS结构
	if(!NT_SUCCESS(status))
	{
		KdPrint(("我错了，这个是错误号:%X---这个是进程ID:%d",status,ProcessId));
		return FALSE;
	}
	if(!strcmp((char *)process_obj+0x174,str_ProtectObjName))//进行比较
	{
		ObDereferenceObject(process_obj);//对象计数器减1，为了恢复对象管理器计数，便于回收
		return TRUE;
	}
	ObDereferenceObject(process_obj);
	return FALSE;
}
NTSTATUS MyNtOpenProcess (
	__out PHANDLE ProcessHandle,
	__in ACCESS_MASK DesiredAccess,
	__in POBJECT_ATTRIBUTES ObjectAttributes,
	__in_opt PCLIENT_ID ClientId
	)
{
	//KdPrint(("%s",(char *)PsGetCurrentProcess()+0x174));
	if(ProtectProcess(ClientId->UniqueProcess,"calc.exe"))
	{
		KdPrint(("%s想打开我吗？不可能。哈哈。。",(char *)PsGetCurrentProcess()+0x174));
		return STATUS_UNSUCCESSFUL;
	}
	//KdPrint(("Hook Success!"));
	return ((MYNTOPENPROCESS)O_NtOpenProcess)(ProcessHandle,//处理完自己的任务后，调用原来的函数，让其它进程正常工作
		DesiredAccess,
		ObjectAttributes,
		ClientId);
}

void PageProtectOff()//关闭页面保护
{
	__asm{
		cli
			mov  eax,cr0
			and  eax,not 10000h
			mov  cr0,eax
		}
}
void PageProtectOn()//打开页面保护
{
	__asm{
		mov  eax,cr0
		or   eax,10000h
		mov  cr0,eax
		sti
	}
}

void UnHookSsdt()
{
	PageProtectOff();
	KeServiceDescriptorTable.ServiceTableBase[122]=O_NtOpenProcess;//恢复ssdt中原来的函数地址
	PageProtectOn();
}
NTSTATUS ssdt_hook()
{
	//int i;
	//for(i=0;i<KeServiceDescriptorTable.NumberOfServices;i++)
	//{
	//	KdPrint(("NumberOfService[%d]-------%x",i,KeServiceDescriptorTable.ServiceTableBase[i]));
	//}
	O_NtOpenProcess=KeServiceDescriptorTable.ServiceTableBase[122];//保存原来的函数地址
	PageProtectOff();
	//将原来ssdt中所要hook的函数地址换成我们自己的函数地址
	KeServiceDescriptorTable.ServiceTableBase[122]=(unsigned int)MyNtOpenProcess;
	PageProtectOn();
	return STATUS_SUCCESS;
}
void DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	UnHookSsdt();
	KdPrint(("Driver Unload Success !"));
}
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject,PUNICODE_STRING pRegsiterPath)
{
	DbgPrint("This is My First Driver!");
	ssdt_hook();
	pDriverObject->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}