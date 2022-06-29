#include "pch.h"
#include "ModuleProtocol_JT1078/ModuleProtocol_JT1078.h"
/********************************************************************
//    Created:     2022/03/01  09:27:08
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleProtocol\pch.cpp
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleProtocol
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出协议模块实现
//    History:
*********************************************************************/
BOOL ModuleProtocol_IsErrorOccur = FALSE;
DWORD ModuleProtocol_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CModuleProtocol_JT1078 m_ProtocolJT1078;
//////////////////////////////////////////////////////////////////////////
///                        导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD ModuleProtocol_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return ModuleProtocol_dwErrorCode;
}
/************************************************************************/
/*                         导出的JT1078协议函数                         */
/************************************************************************/
extern "C" BOOL ModuleProtocol_JT1078_StreamCreate(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLDEVICE * pSt_ProtocolDevice)
{
	return m_ProtocolJT1078.ModuleProtocol_JT1078_StreamCreate(ptszMsgBuffer, pInt_MsgLen, pSt_ProtocolDevice);
}
extern "C" BOOL ModuleProtocol_JT1078_StreamPush(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLDEVICE * pSt_ProtocolDevice, LPCTSTR lpszMsgBuffer, int nMsgLen, int nMsgType)
{
	return m_ProtocolJT1078.ModuleProtocol_JT1078_StreamPush(ptszMsgBuffer, pInt_MsgLen, pSt_ProtocolDevice, lpszMsgBuffer, nMsgLen, nMsgType);
}
extern "C" BOOL ModuleProtocol_JT1078_StreamDestroy(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLDEVICE * pSt_ProtocolDev)
{
	return m_ProtocolJT1078.ModuleProtocol_JT1078_StreamDestroy(ptszMsgBuffer, pInt_MsgLen, pSt_ProtocolDev);
}