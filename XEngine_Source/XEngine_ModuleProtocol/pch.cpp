#include "pch.h"
#include "ModuleProtocol_Parse/ModuleProtocol_Parse.h"
#include "ModuleProtocol_Packet/ModuleProtocol_Packet.h"
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
CModuleProtocol_Parse m_ProtocolParse;
CModuleProtocol_Packet m_ProtocolPacket;
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
/*                         导出的协议封装函数                           */
/************************************************************************/
extern "C" BOOL ModuleProtocol_Packet_Comm(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, int nCode, LPCTSTR lpszMsgBuffer, LPCTSTR lpszToken)
{
	return m_ProtocolPacket.ModuleProtocol_Packet_Comm(ptszMsgBuffer, pInt_MsgLen, nCode, lpszMsgBuffer, lpszToken);
}
extern "C" BOOL ModuleProtocol_Packet_Create(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLDEVICE * pSt_ProtocolDevice)
{
	return m_ProtocolPacket.ModuleProtocol_Packet_Create(ptszMsgBuffer, pInt_MsgLen, pSt_ProtocolDevice);
}
extern "C" BOOL ModuleProtocol_Packet_Push(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLDEVICE * pSt_ProtocolDevice, int nMsgLen, int nMsgType)
{
	return m_ProtocolPacket.ModuleProtocol_Packet_Push(ptszMsgBuffer, pInt_MsgLen, pSt_ProtocolDevice, nMsgLen, nMsgType);
}
extern "C" BOOL ModuleProtocol_Packet_Destroy(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLDEVICE * pSt_ProtocolDev)
{
	return m_ProtocolPacket.ModuleProtocol_Packet_Destroy(ptszMsgBuffer, pInt_MsgLen, pSt_ProtocolDev);
}
extern "C" BOOL ModuleProtocol_Packet_ForwardList(TCHAR * ptszMsgBuffer, int* pInt_MsgLen, MODULESESSION_FORWARDINFO * **pppSt_Forward, int nListCount)
{
	return m_ProtocolPacket.ModuleProtocol_Packet_ForwardList(ptszMsgBuffer, pInt_MsgLen, pppSt_Forward, nListCount);
}
/************************************************************************/
/*                         导出的协议解析函数                           */
/************************************************************************/
extern "C" BOOL ModuleProtocol_Parse_HTTPForward(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR * ptszAVUrl)
{
	return m_ProtocolParse.ModuleProtocol_Parse_HTTPForward(lpszMsgBuffer, nMsgLen, ptszAVUrl);
}