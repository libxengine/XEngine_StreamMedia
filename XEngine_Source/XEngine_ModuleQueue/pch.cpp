#include "pch.h"
#include "ModuleQueue_JT1078/ModuleQueue_JT1078.h"
/********************************************************************
//    Created:     2023/09/19  11:26:28
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleQueue\pch.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleQueue
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出实现
//    History:
*********************************************************************/
bool Queue_IsErrorOccur = false;
XLONG Queue_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CModuleQueue_JT1078 m_QueueJT1078;
//////////////////////////////////////////////////////////////////////////
//                       导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG ModuleQueue_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return Queue_dwErrorCode;
}
/*********************************************************************************
*                          JHT1078导出队列函数                                   *
*********************************************************************************/
extern "C" bool ModuleQueue_JT1078_Create(LPCXSTR lpszClientAddr)
{
	return m_QueueJT1078.ModuleQueue_JT1078_Create(lpszClientAddr);
}
extern "C" bool ModuleQueue_JT1078_Destroy(LPCXSTR lpszClientAddr)
{
	return m_QueueJT1078.ModuleQueue_JT1078_Destroy(lpszClientAddr);
}
extern "C" bool ModuleQueue_JT1078_Insert(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	return m_QueueJT1078.ModuleQueue_JT1078_Insert(lpszClientAddr, lpszMsgBuffer, nMsgLen);
}
extern "C" bool ModuleQueue_JT1078_Get(LPCXSTR lpszClientAddr, XCHAR * *pptszMsgBuffer, int* pInt_MsgLen)
{
	return m_QueueJT1078.ModuleQueue_JT1078_Get(lpszClientAddr, pptszMsgBuffer, pInt_MsgLen);
}