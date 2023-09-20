#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2023/06/04  10:53:14
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\XEngine_Network.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp
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
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,连接到服务器"), lpszClientAddr);
	return true;
}
void CALLBACK Network_Callback_HttpRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	if (!HttpProtocol_Server_InserQueueEx(xhHttpPacket, lpszClientAddr, lpszRecvMsg, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,投递HTTP数据包到消息队列失败，错误:%lX"), lpszClientAddr, HttpProtocol_GetLastError());
		return;
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhHttpHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("HTTP客户端:%s,投递数据包到组包队列成功,大小:%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK Network_Callback_HttpLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, hSocket, false, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
}
void CALLBACK Network_Callback_HttpHeart(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, hSocket, true, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
}
//////////////////////////////////////////////////////////////////////////下面是X推流网络IO相关代码处理函数
bool CALLBACK Network_Callback_XStreamLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	//客户端连接后要把客户端插入心跳管理器中才有效
	SocketOpt_HeartBeat_InsertAddrEx(xhXStreamHeart, lpszClientAddr);
	//并且还要创建一个TCP包管理器对象,不然无法组包
	HelpComponents_Datas_CreateEx(xhXStreamPacket, lpszClientAddr, 0);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("XStream推流端:%s,连接到服务器"), lpszClientAddr);
	return true;
}
void CALLBACK Network_Callback_XStreamRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	//接受到数据后直接投递给TCP包管理器,因为可能不是一个完整的包,所以我们的期望是通过此得到一个完整的包
	if (!HelpComponents_Datas_PostEx(xhXStreamPacket, lpszClientAddr, lpszRecvMsg, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("XStream推流端:%s,投递数据包到组包队列失败，错误:%lX"), lpszClientAddr, Packets_GetLastError());
		return;
	}
	//需要激活一次
	SocketOpt_HeartBeat_ActiveAddrEx(xhXStreamHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("XStream推流端:%s,投递数据包到组包队列成功,大小:%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK Network_Callback_XStreamLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	//交给指定函数来处理客户端离开消息
	XEngine_Network_Close(lpszClientAddr, hSocket, false, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_XSTREAM);
}
void CALLBACK Network_Callback_XStreamHeart(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam)
{
	//同上
	XEngine_Network_Close(lpszClientAddr, hSocket, true, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_XSTREAM);
}
//////////////////////////////////////////////////////////////////////////RTMP推流相关
bool CALLBACK Network_Callback_RTMPLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	RTMPProtocol_Parse_Insert(lpszClientAddr);
	SocketOpt_HeartBeat_InsertSocketEx(xhRTMPHeart, hSocket);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTMP推流端：%s，进入了服务器"), lpszClientAddr);
	return true;
}
void CALLBACK Network_Callback_RTMPRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	if (!RTMPProtocol_Parse_Send(lpszClientAddr, lpszRecvMsg, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("RTMP推流端：%s，投递包失败，大小：%d，错误：%lX"), lpszClientAddr, nMsgLen, RTMPProtocol_GetLastError());
		SocketOpt_HeartBeat_ForceOutAddrEx(xhJT1078Heart, lpszClientAddr);
		return;
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhJT1078Heart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("RTMP推流端：%s，投递包成功，大小：%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK Network_Callback_RTMPLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, hSocket, false, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
}
void CALLBACK Network_Callback_RTMPHeart(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, hSocket, true, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
}
//////////////////////////////////////////////////////////////////////////JT1078协议流
bool CALLBACK Network_Callback_JT1078Login(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	SocketOpt_HeartBeat_InsertSocketEx(xhJT1078Heart, hSocket);
	HelpComponents_PKTCustom_CreateEx(xhJT1078Pkt, hSocket, 0);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("JT1078推流端：%s，进入了服务器"), lpszClientAddr);
	return true;
}
void CALLBACK Network_Callback_JT1078Recv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	if (!HelpComponents_PKTCustom_PostEx(xhJT1078Pkt, hSocket, lpszRecvMsg, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("JT1078推流端：%s，投递包失败，大小：%d，错误：%lX"), lpszClientAddr, nMsgLen, Packets_GetLastError());
		SocketOpt_HeartBeat_ForceOutAddrEx(xhJT1078Heart, lpszClientAddr);
		return;
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhJT1078Heart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("JT1078推流端：%s，投递包成功，大小：%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK Network_Callback_JT1078Leave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, hSocket, false, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_JT1078);
}
void CALLBACK Network_Callback_JT1078HBLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, hSocket, true, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_JT1078);
}
//////////////////////////////////////////////////////////////////////////SRT
bool CALLBACK Network_Callback_SRTLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	PushStream_SrtTask_Connct(lpszClientAddr, hSocket);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("SRT客户端：%s，进入了服务器"), lpszClientAddr);
	return true;
}
void CALLBACK Network_Callback_SRTRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam)
{
	PushStream_SrtTask_Handle(lpszClientAddr, hSocket, lpszRecvMsg, nMsgLen);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("SRT客户端：%s，接受到数据,数据大小:%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK Network_Callback_SRTLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam)
{
	XEngine_Network_Close(lpszClientAddr, hSocket, false, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_SRT);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("SRT客户端：%s，离开了服务器"), lpszClientAddr);
}
//////////////////////////////////////////////////////////////////////////网络IO关闭操作
void XEngine_Network_Close(LPCXSTR lpszClientAddr, XSOCKET hSocket, bool bHeart, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enClientType)
{
	if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP == enClientType)
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
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,离开服务器"), lpszClientAddr);
	}
	else if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_XSTREAM == enClientType)
	{
		//先关闭网络和心跳,他们主动回调的数据我们可以不用主动调用关闭
		if (bHeart)
		{
			NetCore_TCPXCore_CloseForClientEx(xhXStreamSocket, lpszClientAddr);
		}
		else
		{
			SocketOpt_HeartBeat_DeleteAddrEx(xhXStreamHeart, lpszClientAddr);
		}
		//需要主动删除与客户端对应的组包器队列中的资源
		HelpComponents_Datas_DeleteEx(xhXStreamPacket, lpszClientAddr);
		//停止推流
		XEngine_AVPacket_AVDelete(lpszClientAddr);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("XEngine推流端:%s,离开服务器,心跳标志:%d"), lpszClientAddr, bHeart);
	}
	else if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_JT1078 == enClientType)
	{
		if (bHeart)
		{
			NetCore_TCPXCore_CloseForClientEx(xhJT1078Socket, lpszClientAddr);
		}
		else
		{
			SocketOpt_HeartBeat_DeleteAddrEx(xhJT1078Heart, lpszClientAddr);
		}
		HelpComponents_PKTCustom_DeleteEx(xhJT1078Pkt, hSocket);
		XEngine_AVPacket_AVDelete(lpszClientAddr);
		ModuleQueue_JT1078_Destroy(lpszClientAddr);
		ModuleSession_PushStream_Destroy(lpszClientAddr);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("JT1078推流端:%s,离开服务器,心跳标志:%d"), lpszClientAddr, bHeart);
	}
	else if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP == enClientType)
	{
		if (bHeart)
		{
			NetCore_TCPXCore_CloseForClientEx(xhRTMPSocket, lpszClientAddr);
		}
		else
		{
			SocketOpt_HeartBeat_DeleteAddrEx(xhRTMPHeart, lpszClientAddr);
		}
		RTMPProtocol_Parse_Delete(lpszClientAddr);
		XEngine_AVPacket_AVDelete(lpszClientAddr);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTMP推流端:%s,离开服务器,心跳标志:%d"), lpszClientAddr, bHeart);
	}
	else if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_SRT == enClientType)
	{
		ModuleHelp_SrtCore_Close(lpszClientAddr);
		HLSProtocol_TSParse_delete(lpszClientAddr);
		XEngine_AVPacket_AVDelete(lpszClientAddr);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("SRT客户端:%s,离开服务器,心跳标志:%d"), lpszClientAddr, bHeart);
	}
	XCHAR tszSMSAddr[MAX_PATH];
	XCHAR tszPushAddr[MAX_PATH];

	memset(tszSMSAddr, '\0', sizeof(tszSMSAddr));
	memset(tszPushAddr, '\0', sizeof(tszPushAddr));
	//可能是推流也可能是拉流
	if (ModuleSession_PullStream_GetPushAddr(lpszClientAddr, tszPushAddr))
	{
		ModuleSession_PullStream_Delete(lpszClientAddr);
		ModuleSession_PushStream_ClientDelete(tszPushAddr, lpszClientAddr);
	}
	if (ModuleSession_PushStream_GetAddrForAddr(lpszClientAddr, tszSMSAddr))
	{
		ModuleSession_PullStream_PublishDelete(tszSMSAddr);
		ModuleSession_PushStream_Destroy(lpszClientAddr);
	}
}
bool XEngine_Network_Send(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enClientType)
{
	if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP == enClientType)
	{
		//发送数据给指定客户端
		if (!NetCore_TCPXCore_SendEx(xhHttpSocket, lpszClientAddr, lpszMsgBuffer, nMsgLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,发送数据失败，错误:%lX"), lpszClientAddr, NetCore_GetLastError());
			return false;
		}
		//发送成功激活一次心跳
		SocketOpt_HeartBeat_ActiveAddrEx(xhHttpHeart, lpszClientAddr);
	}
	else if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_XSTREAM == enClientType)
	{
		//发送数据给指定客户端
		if (!NetCore_TCPXCore_SendEx(xhXStreamSocket, lpszClientAddr, lpszMsgBuffer, nMsgLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("XStream推流端:%s,发送数据失败，错误:%lX"), lpszClientAddr, NetCore_GetLastError());
			return false;
		}
		//发送成功激活一次心跳
		SocketOpt_HeartBeat_ActiveAddrEx(xhXStreamHeart, lpszClientAddr);
	}
	else if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_JT1078 == enClientType)
	{
		//发送数据给指定客户端
		if (!NetCore_TCPXCore_SendEx(xhJT1078Socket, lpszClientAddr, lpszMsgBuffer, nMsgLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("JT1078推流端:%s,发送数据失败，错误:%lX"), lpszClientAddr, NetCore_GetLastError());
			return false;
		}
		//发送成功激活一次心跳
		SocketOpt_HeartBeat_ActiveAddrEx(xhJT1078Heart, lpszClientAddr);
	}
	else if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP == enClientType)
	{
		//发送数据给指定客户端
		if (!NetCore_TCPXCore_SendEx(xhRTMPSocket, lpszClientAddr, lpszMsgBuffer, nMsgLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("RTMP推流端:%s,发送数据失败，错误:%lX"), lpszClientAddr, NetCore_GetLastError());
			return false;
		}
		//发送成功激活一次心跳
		SocketOpt_HeartBeat_ActiveAddrEx(xhRTMPHeart, lpszClientAddr);
	}
	else if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_SRT == enClientType)
	{
		if (!ModuleHelp_SrtCore_Send(lpszClientAddr, lpszMsgBuffer, nMsgLen))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("SRT服务端:%s,发送数据失败，错误:%lX"), lpszClientAddr, ModuleHelp_GetLastError());
			return false;
		}
	}
	return true;
}