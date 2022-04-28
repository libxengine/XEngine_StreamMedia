#include "JT1078App_Hdr.h"
/********************************************************************
//    Created:     2022/04/25  14:53:54
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_JT1078App\JT1078App_Client.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_JT1078App
//    File Base:   JT1078App_Client
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     客户端操作类
//    History:
*********************************************************************/
void XEngine_Client_CBRecv(XNETHANDLE xhNet, ENUM_NETCLIENT_TCPEVENTS enTCPClientEvents, LPCSTR lpszMsgBuffer, int nLen, LPVOID lParam)
{
	if (ENUM_XENGINE_XCLIENT_SOCKET_TCP_EVENT_RECV == enTCPClientEvents)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%lld,接受到数据,大小:%d,内容:%s！"), xhNet, nLen, lpszMsgBuffer);
	}
	else if (ENUM_XENGINE_XCLIENT_SOCKET_TCP_EVENT_CLOSE == enTCPClientEvents)
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("客户端：%lld,断开了服务器的连接！"), xhNet);
	}
}
