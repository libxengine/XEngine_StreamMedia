#include "JT1078App_Hdr.h"
/********************************************************************
//    Created:     2022/04/24  14:39:04
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_JT1078App\JT1078App_Net.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_JT1078App
//    File Base:   JT1078App_Net
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     网络操作
//    History:
*********************************************************************/
///////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK XEngine_Callback_StreamLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	SocketOpt_HeartBeat_InsertSocketEx(xhStreamHeart, hSocket);
	HelpComponents_PKTCustom_CreateEx(xhStreamPkt, hSocket, 0);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("实时端：%s，进入了服务器"), lpszClientAddr);
	return TRUE;
}
void CALLBACK XEngine_Callback_StreamRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	if (!HelpComponents_PKTCustom_PostEx(xhStreamPkt, hSocket, lpszRecvMsg, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("实时端：%s，投递包失败，大小：%d，错误：%lX"), lpszClientAddr, nMsgLen, Packets_GetLastError());
		SocketOpt_HeartBeat_ForceOutAddrEx(xhStreamHeart, lpszClientAddr);
		return;
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhStreamHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("实时端：%s，投递包成功，大小：%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK XEngine_Callback_StreamLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, hSocket, ENUM_XENGINE_STREAMAPP_DEVICE_TYPE_STREAM, FALSE);
}
void CALLBACK XEngine_Callback_StreamHBLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, hSocket, ENUM_XENGINE_STREAMAPP_DEVICE_TYPE_STREAM, TRUE);
}
///////////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK XEngine_Callback_RecordLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	SocketOpt_HeartBeat_InsertSocketEx(xhRecordHeart, hSocket);
	HelpComponents_PKTCustom_CreateEx(xhRecordPkt, hSocket);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("录像端：%s，进入了服务器"), lpszClientAddr);

	return TRUE;
}
void CALLBACK XEngine_Callback_RecordRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam)
{
	if (!HelpComponents_PKTCustom_PostEx(xhRecordPkt, hSocket, lpszRecvMsg, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("录像端：%s，投递包失败，大小：%d，错误：%lX"), lpszClientAddr, nMsgLen, Packets_GetLastError());
		return;
	}
	SocketOpt_HeartBeat_ActiveAddrEx(xhRecordHeart, lpszClientAddr);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("录像端：%s，投递包成功，大小：%d"), lpszClientAddr, nMsgLen);
}
void CALLBACK XEngine_Callback_RecordLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, hSocket, ENUM_XENGINE_STREAMAPP_DEVICE_TYPE_RECORD, FALSE);
}
void CALLBACK XEngine_Callback_RecordHBLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam)
{
	XEngine_Net_CloseClient(lpszClientAddr, hSocket, ENUM_XENGINE_STREAMAPP_DEVICE_TYPE_RECORD, TRUE);
}
//////////////////////////////////////////////////////////////////////////
BOOL XEngine_Net_CloseClient(LPCTSTR lpszClientAddr, SOCKET hSocket, ENUM_XENGINE_STREAMAPP_DEVICE_TYPE enDeviceType, BOOL bHBLeave)
{
	int nSDLen = 0;
	TCHAR tszSDBuffer[2048];
	XNETHANDLE xhClient = 0;
	XENGINE_PROTOCOLDEVICE st_ProtocolDev;
	LPCTSTR lpszLeaveMsg = bHBLeave ? _T("心跳超时") : _T("主动断开");

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(&st_ProtocolDev, '\0', sizeof(XENGINE_PROTOCOLDEVICE));

	if (ENUM_XENGINE_STREAMAPP_DEVICE_TYPE_STREAM == enDeviceType)
	{
		if (bHBLeave)
		{
			NetCore_TCPXCore_CloseForClientEx(xhStreamNet, lpszClientAddr);
		}
		else
		{
			SocketOpt_HeartBeat_DeleteAddrEx(xhStreamHeart, lpszClientAddr);
		}
		HelpComponents_PKTCustom_DeleteEx(xhStreamPkt, hSocket);
		ModuleDB_JT1078_DeviceDelete(lpszClientAddr);

		if (ModuleSession_JT1078Client_DeleteAddr(lpszClientAddr, &xhClient, st_ProtocolDev.tszDeviceNumber, &st_ProtocolDev.nChannel, &st_ProtocolDev.bLive))
		{
			ModuleProtocol_JT1078_StreamDestroy(tszSDBuffer, &nSDLen, &st_ProtocolDev);
			XClient_TCPSelect_SendEx(xhClient, tszSDBuffer, &nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("实时端：%s，设备：%s，通道：%d，与服务器断开，原因：%s"), lpszClientAddr, st_ProtocolDev.tszDeviceNumber, st_ProtocolDev.nChannel, lpszLeaveMsg);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("实时端：%s，与服务器断开，设备未登录，原因：%s"), lpszClientAddr, lpszLeaveMsg);
		}
	}
	else if (ENUM_XENGINE_STREAMAPP_DEVICE_TYPE_RECORD == enDeviceType)
	{
		if (bHBLeave)
		{
			NetCore_TCPXCore_CloseForClientEx(xhRecordNet, lpszClientAddr);
		}
		else
		{
			SocketOpt_HeartBeat_DeleteAddrEx(xhRecordHeart, lpszClientAddr);
		}
		HelpComponents_PKTCustom_DeleteEx(xhRecordPkt, hSocket);
		ModuleDB_JT1078_DeviceDelete(lpszClientAddr);

		if (ModuleSession_JT1078Client_DeleteAddr(lpszClientAddr, &xhClient, st_ProtocolDev.tszDeviceNumber, &st_ProtocolDev.nChannel, &st_ProtocolDev.bLive))
		{
			ModuleProtocol_JT1078_StreamDestroy(tszSDBuffer, &nSDLen, &st_ProtocolDev);
			XClient_TCPSelect_SendEx(xhClient, tszSDBuffer, &nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("录像端：%s，设备：%s，通道：%d，与服务器断开，原因：%s"), lpszClientAddr, st_ProtocolDev.tszDeviceNumber, st_ProtocolDev.nChannel, lpszLeaveMsg);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("录像端：%s，与服务器断开，设备未登录，原因：%s"), lpszClientAddr, lpszLeaveMsg);
		}
	}
	return TRUE;
}
BOOL XEngine_Net_SendMsg(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, ENUM_XENGINE_STREAMAPP_DEVICE_TYPE enDeviceType)
{
	DWORD dwRet = 0;
	if (ENUM_XENGINE_STREAMAPP_DEVICE_TYPE_STREAM == enDeviceType)
	{
		if (!NetCore_TCPXCore_SendEx(xhStreamNet, lpszClientAddr, lpszMsgBuffer, nMsgLen))
		{
			dwRet = NetCore_GetLastError();
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("实时端：%s，发送数据失败，发送大小：%d，错误：%lX,%d"), lpszClientAddr, nMsgLen, dwRet, errno);
			return FALSE;
		}
	}
	else if (ENUM_XENGINE_STREAMAPP_DEVICE_TYPE_RECORD == enDeviceType)
	{
		if (!NetCore_TCPXCore_SendEx(xhRecordNet, lpszClientAddr, lpszMsgBuffer, nMsgLen))
		{
			dwRet = NetCore_GetLastError();
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("录像端：%s，发送数据失败，发送大小：%d，错误：%lX,%d"), lpszClientAddr, nMsgLen, dwRet, errno);
			return FALSE;
		}
	}
	return TRUE;
}
