#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2022/06/22  10:51:07
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_SDKApp\XEngine_Client.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_SDKApp
//    File Base:   XEngine_Client
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     客户端操作
//    History:
*********************************************************************/
void CALLBACK XEngine_Client_CBRecv(XHANDLE xhToken, SOCKET hSocket, ENUM_NETCLIENT_TCPEVENTS enTCPClientEvents, LPCSTR lpszMsgBuffer, int nLen, LPVOID lParam)
{
	XNETHANDLE xhDevice = *(XNETHANDLE *)lParam;
	if (ENUM_XENGINE_XCLIENT_SOCKET_TCP_EVENT_RECV == enTCPClientEvents)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("TCP客户端：%d,接受到数据,大小:%d,内容:%s！"), hSocket, nLen, lpszMsgBuffer);
	}
	else if (ENUM_XENGINE_XCLIENT_SOCKET_TCP_EVENT_CLOSE == enTCPClientEvents)
	{
		int nListCount = 0;
		MODULESESSION_SDKCLIENT** ppSt_SDKClient;
		
		ModuleSession_SDKDevice_GetList(xhDevice, &ppSt_SDKClient, &nListCount, xhToken);
		for (int i = 0; i < nListCount; i++)
		{
			ModuleSession_SDKDevice_Delete(xhDevice, ppSt_SDKClient[i]->nChannel, ppSt_SDKClient[i]->bLive);
			ModulePlugin_Core_Stop(xhDevice, ppSt_SDKClient[i]->nChannel);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("TCP客户端：%d,Device:%lld,断开了服务器的连接,关闭推流通道:%d,%d！"), hSocket, xhDevice, ppSt_SDKClient[i]->nChannel, ppSt_SDKClient[i]->bLive);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_SDKClient, nListCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("TCP客户端：%d,Device:%lld,断开了服务器的连接,销毁推流个数:%d！"), hSocket, xhDevice, nListCount);
	}
	else if (ENUM_XENGINE_XCLIENT_SOCKET_TCP_EVENT_CONNECT == enTCPClientEvents)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("TCP客户端：%d,重新连接到服务器！"), hSocket);
	}
}
