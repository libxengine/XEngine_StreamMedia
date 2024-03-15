#include "pch.h"
#include "ModuleHelp_JT1078/ModuleHelp_JT1078.h"
#include "ModuleHelp_Srt/ModuleHelp_SrtCore.h"
#include "ModuleHelp_Rtsp/ModuleHelp_Rtsp.h"
#include "ModuleHelp_Rtmp/ModuleHelp_Rtmp.h"
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
CModuleHelp_Rtsp m_HelpRtsp;
CModuleHelp_Rtmp m_HelpRtmp;
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
extern "C" bool ModuleHelp_SrtCore_SetCallback(CALLBACK_NETCORE_SOCKET_NETEVENT_LOGIN fpCallePoll_Login, CALLBACK_NETCORE_SOCKET_NETEVENT_RECV fpCallePoll_Recv, CALLBACK_NETCORE_SOCKET_NETEVENT_LEAVE fpCallePoll_Leave, XPVOID lPLogin, XPVOID lPRecv, XPVOID lPLeave)
{
	return m_SrtCore.ModuleHelp_SrtCore_SetCallback(fpCallePoll_Login, fpCallePoll_Recv, fpCallePoll_Leave, lPLogin, lPRecv, lPLeave);
}
extern "C" bool ModuleHelp_SrtCore_Send(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	return m_SrtCore.ModuleHelp_SrtCore_Send(lpszClientAddr, lpszMsgBuffer, nMsgLen);
}
extern "C" bool ModuleHelp_SrtCore_GetStreamID(XSOCKET hSocket, XCHAR * ptszSMSAddr, bool* pbPublish)
{
	return m_SrtCore.ModuleHelp_SrtCore_GetStreamID(hSocket, ptszSMSAddr, pbPublish);
}
extern "C" bool ModuleHelp_SrtCore_Close(LPCXSTR lpszClientAddr, XSOCKET hSocket)
{
	return m_SrtCore.ModuleHelp_SrtCore_Close(lpszClientAddr, hSocket);
}
extern "C" bool ModuleHelp_SrtCore_Destory()
{
	return m_SrtCore.ModuleHelp_SrtCore_Destory();
}
/************************************************************************/
/*                         导出的RTSP函数                               */
/************************************************************************/
extern "C" bool ModuleHelp_Rtsp_GetSMSAddr(LPCXSTR lpszURLStr, XCHAR * ptszSMSAddr)
{
	return m_HelpRtsp.ModuleHelp_Rtsp_GetSMSAddr(lpszURLStr, ptszSMSAddr);
}
extern "C" bool ModuleHelp_Rtsp_CreateClient(LPCXSTR lpszClientID, int nVControlID, int nAControlID)
{
	return m_HelpRtsp.ModuleHelp_Rtsp_CreateClient(lpszClientID, nVControlID, nAControlID);
}
extern "C" bool ModuleHelp_Rtsp_DeleteClient(LPCXSTR lpszClientID)
{
	return m_HelpRtsp.ModuleHelp_Rtsp_DeleteClient(lpszClientID);
}
extern "C" bool ModuleHelp_Rtsp_SetClient(LPCXSTR lpszClientID, int nRTPPort, int nRTCPPort, int nTrackID)
{
	return m_HelpRtsp.ModuleHelp_Rtsp_SetClient(lpszClientID, nRTPPort, nRTCPPort, nTrackID);
}
extern "C" bool ModuleHelp_Rtsp_GetClient(LPCXSTR lpszClientID, int* pInt_RTPPort, int* pInt_RTCPPort, bool bVideo)
{
	return m_HelpRtsp.ModuleHelp_Rtsp_GetClient(lpszClientID, pInt_RTPPort, pInt_RTCPPort, bVideo);
}
extern "C" bool ModuleHelp_Rtsp_SetSession(LPCXSTR lpszClientID, LPCXSTR lpszSessionStr)
{
	return m_HelpRtsp.ModuleHelp_Rtsp_SetSession(lpszClientID, lpszSessionStr);
}
extern "C" bool ModuleHelp_Rtsp_GetSession(LPCXSTR lpszClientID, XCHAR * ptszSessionStr)
{
	return m_HelpRtsp.ModuleHelp_Rtsp_GetSession(lpszClientID, ptszSessionStr);
}
extern "C" bool ModuleHelp_Rtsp_SetSsrc(LPCXSTR lpszClientID, LPCXSTR lpszSsrcStr, bool bVideo)
{
	return m_HelpRtsp.ModuleHelp_Rtsp_SetSsrc(lpszClientID, lpszSsrcStr, bVideo);
}
extern "C" bool ModuleHelp_Rtsp_GetSsrc(LPCXSTR lpszClientID, XCHAR * ptszSsrcStr, bool bVideo)
{
	return m_HelpRtsp.ModuleHelp_Rtsp_GetSsrc(lpszClientID, ptszSsrcStr, bVideo);
}
extern "C" bool ModuleHelp_Rtsp_GetTrack(LPCXSTR lpszClientID, int nTrackID, bool* pbVideo)
{
	return m_HelpRtsp.ModuleHelp_Rtsp_GetTrack(lpszClientID, nTrackID, pbVideo);
}
extern "C" bool ModuleHelp_Rtsp_GetRTPAddr(LPCXSTR lpszClientID, XCHAR * ptszADDRStr, bool bVideo)
{
	return m_HelpRtsp.ModuleHelp_Rtsp_GetRTPAddr(lpszClientID, ptszADDRStr, bVideo);
}
extern "C" bool ModuleHelp_Rtsp_GetRTCPAddr(LPCXSTR lpszClientID, XCHAR * ptszADDRStr, bool bVideo)
{
	return m_HelpRtsp.ModuleHelp_Rtsp_GetRTCPAddr(lpszClientID, ptszADDRStr, bVideo);
}
/************************************************************************/
/*                         导出的RTMP函数                               */
/************************************************************************/
extern "C" bool ModuleHelp_Rtmp_CreateSession(LPCXSTR lpszURLAddr, LPCXSTR lpszURLStr)
{
	return m_HelpRtmp.ModuleHelp_Rtmp_CreateSession(lpszURLAddr, lpszURLStr);
}
extern "C" bool ModuleHelp_Rtmp_GetSession(LPCXSTR lpszURLAddr, XCHAR * ptszLiveName)
{
	return m_HelpRtmp.ModuleHelp_Rtmp_GetSession(lpszURLAddr, ptszLiveName);
}
extern "C" bool ModuleHelp_Rtmp_DeleteSession(LPCXSTR lpszURLAddr)
{
	return m_HelpRtmp.ModuleHelp_Rtmp_DeleteSession(lpszURLAddr);
}