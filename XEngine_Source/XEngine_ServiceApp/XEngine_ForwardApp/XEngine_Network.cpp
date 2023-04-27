#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2022/10/17  11:00:05
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_ForwardApp\XEngine_Network.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_ForwardApp
//    File Base:   XEngine_Network
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     网络IO相关代码
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////HTTP
bool CALLBACK Network_Callback_HttpLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	SocketOpt_HeartBeat_InsertAddrEx(xhHttpHeart, lpszClientAddr);
	HttpProtocol_Server_CreateClientEx(xhHttpPacket, lpszClientAddr, 0);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,连接到服务器"), lpszClientAddr);
	return true;
}
void CALLBACK Network_Callback_HttpRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	if (!HttpProtocol_Server_InserQueueEx(xhHttpPacket, lpszClientAddr, lpszRecvMsg, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,投递HTTP数据包到消息队列失败，错误:%lX"), lpszClientAddr, HttpProtocol_GetLastError());
		return;
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhHttpHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("HTTP客户端:%s,投递数据包到组包队列成功,大小:%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK Network_Callback_HttpLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, false);
}
void CALLBACK Network_Callback_HttpHeart(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, true);
}
//////////////////////////////////////////////////////////////////////////网络IO关闭操作
void XEngine_Network_Close(LPCXSTR lpszClientAddr, bool bHeart)
{
	//先关闭网络和心跳,他们主动回调的数据我们可以不用主动调用关闭
	if (bHeart)
	{
		NetCore_TCPXCore_CloseForClientEx(xhHttpSocket, lpszClientAddr);
	}
	else
	{
		SocketOpt_HeartBeat_DeleteAddrEx(xhHttpHeart, lpszClientAddr);
	}
	//需要主动删除与客户端对应的组包器队列中的资源
	HttpProtocol_Server_CloseClinetEx(xhHttpPacket, lpszClientAddr); 
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,离开服务器"), lpszClientAddr);
}
//////////////////////////////////////////////////////////////////////////
bool XEngine_Network_Send(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	//发送数据给指定客户端
	if (!NetCore_TCPXCore_SendEx(xhHttpSocket, lpszClientAddr, lpszMsgBuffer, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,发送数据失败，错误:%lX"), lpszClientAddr, NetCore_GetLastError());
		return false;
	}
	//发送成功激活一次心跳
	SocketOpt_HeartBeat_ActiveAddrEx(xhHttpHeart, lpszClientAddr);
	return true;
}