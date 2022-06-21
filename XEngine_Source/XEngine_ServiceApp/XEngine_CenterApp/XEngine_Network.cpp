#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2022/01/20  14:35:20
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp\XEngine_CenterApp\XEngine_Network.cpp
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp\XEngine_CenterApp
//    File Base:   XEngine_Network
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     网络IO相关代码
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////下面是业务网络IO相关代码处理函数
BOOL __stdcall Network_Callback_CenterLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	//客户端连接后要把客户端插入心跳管理器中才有效
	SocketOpt_HeartBeat_InsertAddrEx(xhCenterHeart, lpszClientAddr);
	//并且还要创建一个TCP包管理器对象,不然无法组包
	HelpComponents_Datas_CreateEx(xhCenterPacket, lpszClientAddr, 0);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,连接到服务器"), lpszClientAddr);
	return TRUE;
}
void __stdcall Network_Callback_CenterRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	//接受到数据后直接投递给TCP包管理器,因为可能不是一个完整的包,所以我们的期望是通过此得到一个完整的包
	if (!HelpComponents_Datas_PostEx(xhCenterPacket, lpszClientAddr, lpszRecvMsg, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,投递数据包到组包队列失败，错误:%lX"), lpszClientAddr, Packets_GetLastError());
		return;
	}
	//需要激活一次
	SocketOpt_HeartBeat_ActiveAddrEx(xhCenterHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("业务客户端:%s,投递数据包到组包队列成功,大小:%d"), lpszClientAddr, nMsgLen);
}
void __stdcall Network_Callback_CenterLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	//交给指定函数来处理客户端离开消息
	XEngine_Network_Close(lpszClientAddr, FALSE, XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP_CENTER);
}
void __stdcall Network_Callback_CenterHeart(LPCSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam)
{
	//同上
	XEngine_Network_Close(lpszClientAddr, TRUE, XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP_CENTER);
}
//////////////////////////////////////////////////////////////////////////HTTP
BOOL __stdcall Network_Callback_HttpLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	SocketOpt_HeartBeat_InsertAddrEx(xhHttpHeart, lpszClientAddr);
	RfcComponents_HttpServer_CreateClientEx(xhHttpPacket, lpszClientAddr, 0);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,连接到服务器"), lpszClientAddr);
	return TRUE;
}
void __stdcall Network_Callback_HttpRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	if (!RfcComponents_HttpServer_InserQueueEx(xhHttpPacket, lpszClientAddr, lpszRecvMsg, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,投递HTTP数据包到消息队列失败，错误:%lX"), lpszClientAddr, HttpServer_GetLastError());
		return;
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhHttpHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("HTTP客户端:%s,投递数据包到组包队列成功,大小:%d"), lpszClientAddr, nMsgLen);
}
void __stdcall Network_Callback_HttpLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, FALSE, XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP_HTTP);
}
void __stdcall Network_Callback_HttpHeart(LPCSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, TRUE, XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP_HTTP);
}
//////////////////////////////////////////////////////////////////////////网络IO关闭操作
void XEngine_Network_Close(LPCTSTR lpszClientAddr, BOOL bHeart, int nClientType)
{
	//先关闭网络和心跳,他们主动回调的数据我们可以不用主动调用关闭
	if (bHeart)
	{
		if (XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP_CENTER == nClientType)
		{
			//心跳超时属于主动关闭,所以要主动调用网络关闭
			NetCore_TCPXCore_CloseForClientEx(xhCenterSocket, lpszClientAddr);
		}
		else
		{
			NetCore_TCPXCore_CloseForClientEx(xhHttpSocket, lpszClientAddr);
		}
	}
	else
	{
		if (XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP_CENTER == nClientType)
		{
			//同上
			SocketOpt_HeartBeat_DeleteAddrEx(xhCenterHeart, lpszClientAddr);
		}
		else
		{
			SocketOpt_HeartBeat_DeleteAddrEx(xhHttpHeart, lpszClientAddr);
		}
	}
	
	if (XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP_CENTER == nClientType)
	{
		//需要主动删除与客户端对应的组包器队列中的资源
		HelpComponents_Datas_DeleteEx(xhCenterPacket, lpszClientAddr);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,离开服务器"), lpszClientAddr);
	}
	else
	{
		//需要主动删除与客户端对应的组包器队列中的资源
		RfcComponents_HttpServer_CloseClinetEx(xhHttpPacket, lpszClientAddr);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,离开服务器"), lpszClientAddr);
	}
}
//////////////////////////////////////////////////////////////////////////
BOOL XEngine_Network_Send(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, int nClientType)
{
	//发送数据给指定客户端

	if (XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP_CENTER == nClientType)
	{
		if (!NetCore_TCPXCore_SendEx(xhCenterSocket, lpszClientAddr, lpszMsgBuffer, nMsgLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,发送数据失败，错误:%lX"), lpszClientAddr, NetCore_GetLastError());
			return FALSE;
		}
		//发送成功激活一次心跳
		SocketOpt_HeartBeat_ActiveAddrEx(xhCenterHeart, lpszClientAddr);
	}
	else
	{
		if (!NetCore_TCPXCore_SendEx(xhHttpSocket, lpszClientAddr, lpszMsgBuffer, nMsgLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,发送数据失败，错误:%lX"), lpszClientAddr, NetCore_GetLastError());
			return FALSE;
		}
		//发送成功激活一次心跳
		SocketOpt_HeartBeat_ActiveAddrEx(xhHttpHeart, lpszClientAddr);
	}
	return TRUE;
}