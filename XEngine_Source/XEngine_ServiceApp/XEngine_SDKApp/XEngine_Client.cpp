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
void CALLBACK XEngine_Client_CBRecv(XNETHANDLE xhNet, ENUM_NETCLIENT_TCPEVENTS enTCPClientEvents, LPCSTR lpszMsgBuffer, int nLen, LPVOID lParam)
{
	if (ENUM_XENGINE_XCLIENT_SOCKET_TCP_EVENT_RECV == enTCPClientEvents)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%lld,接受到数据,大小:%d,内容:%s！"), xhNet, nLen, lpszMsgBuffer);
	}
	else if (ENUM_XENGINE_XCLIENT_SOCKET_TCP_EVENT_CLOSE == enTCPClientEvents)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%lld,断开了服务器的连接！"), xhNet);
	}
	else if (ENUM_XENGINE_XCLIENT_SOCKET_TCP_EVENT_CONNECT == enTCPClientEvents)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%lld,重新连接到服务器！"), xhNet);
	}
}
