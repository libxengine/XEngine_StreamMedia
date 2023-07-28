#include "pch.h"
#include "ModuleHelp_JT1078/ModuleHelp_JT1078.h"
#include "ModuleHelp_Srt/ModuleHelp_SrtCore.h"
/********************************************************************
//    Created:     2022/04/24  16:28:09
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\pch.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp
//    File Base:   pch
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出函数实现
//    History:
*********************************************************************/
bool ModuleHelp_IsErrorOccur = false;
XLONG ModuleHelp_dwErrorCode = 0;
//////////////////////////////////////////////////////////////////////////
CModuleHelp_JT1078 m_HelpJT1078;
CModuleHelp_SrtCore m_SrtCore;
//////////////////////////////////////////////////////////////////////////
///                        导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG ModuleHelp_GetLastError(int* pInt_SysError)
{
	if (NULL != pInt_SysError)
	{
		*pInt_SysError = errno;
	}
	return ModuleHelp_dwErrorCode;
}
/************************************************************************/
/*                         导出的JT1078帮助函数                         */
/************************************************************************/
extern "C" bool ModuleHelp_JT1078_BCDToString(XBYTE bySIMNumber[6], XCHAR * ptszMsgBuffer)
{
	return m_HelpJT1078.ModuleHelp_JT1078_BCDToString(bySIMNumber, ptszMsgBuffer);
}
extern "C" bool ModuleHelp_JT1078_StringToBCD(LPCXSTR lpszMsgBuffer, XBYTE * pbySIMNumber)
{
	return m_HelpJT1078.ModuleHelp_JT1078_StringToBCD(lpszMsgBuffer, pbySIMNumber);
}
/************************************************************************/
/*                         导出的SRT函数                                */
/************************************************************************/
extern "C" bool ModuleHelp_SrtCore_Start(int nPort)
{
	return m_SrtCore.ModuleHelp_SrtCore_Start(nPort);
}
extern "C" bool ModuleHelp_SrtCore_Close(LPCXSTR lpszClientAddr, SRTSOCKET hSocket)
{
	return m_SrtCore.ModuleHelp_SrtCore_Close(lpszClientAddr, hSocket);
}
extern "C" bool ModuleHelp_SrtCore_SetCallback(CALLBACK_NETCORE_SOCKET_NETEVENT_LOGIN fpCallePoll_Login, CALLBACK_NETCORE_SOCKET_NETEVENT_RECV fpCallePoll_Recv, CALLBACK_NETCORE_SOCKET_NETEVENT_LEAVE fpCallePoll_Leave, XPVOID lPLogin, XPVOID lPRecv, XPVOID lPLeave)
{
	return m_SrtCore.ModuleHelp_SrtCore_SetCallback(fpCallePoll_Login, fpCallePoll_Recv, fpCallePoll_Leave, lPLogin, lPRecv, lPLeave);
}
extern "C" bool ModuleHelp_SrtCore_GetStreamID(SRTSOCKET hSocket, XCHAR * ptszSMSAddr, bool* pbPublish)
{
	return m_SrtCore.ModuleHelp_SrtCore_GetStreamID(hSocket, ptszSMSAddr, pbPublish);
}
extern "C" bool ModuleHelp_SrtCore_Destory()
{
	return m_SrtCore.ModuleHelp_SrtCore_Destory();
}