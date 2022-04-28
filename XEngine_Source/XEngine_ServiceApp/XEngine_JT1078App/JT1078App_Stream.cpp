#include "JT1078App_Hdr.h"
/********************************************************************
//    Created:     2022/04/24  15:35:14
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_JT1078App\JT1078App_Stream.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_JT1078App
//    File Base:   JT1078App_Stream
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     直播流
//    History:
*********************************************************************/
XHTHREAD CALLBACK XEngine_Stream_Thread(LPVOID lParam)
{
	int nThreadPos = *(int*)lParam;
	nThreadPos++;

	while (bIsRun)
	{
		if (!HelpComponents_PKTCustom_WaitEventEx(xhStreamPkt, nThreadPos))
		{
			continue;
		}
		int nListCount = 0;
		HELPCOMPONENT_PACKET_CLIENT **ppSt_PacketClient;
		HelpComponents_PKTCustom_GetPoolEx(xhStreamPkt, nThreadPos, &ppSt_PacketClient, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			for (int j = 0; j < ppSt_PacketClient[i]->nPktCount; j++)
			{
				int nMsgLen = 0;
				int nHDRLen = 0;
				CHAR* ptszMsgBuffer;
				TCHAR tszHdrBuffer[MAX_PATH];
				memset(tszHdrBuffer, '\0', MAX_PATH);

				if (!HelpComponents_PKTCustom_GetMemoryEx(xhStreamPkt, ppSt_PacketClient[i]->hSocket, &ptszMsgBuffer, &nMsgLen, tszHdrBuffer, &nHDRLen))
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("实时端,获取一个随机包失败,错误：%lX"), Packets_GetLastError());
					break;
				}
				XEngine_Stream_Handle(ppSt_PacketClient[i]->hSocket, ptszMsgBuffer, nMsgLen, tszHdrBuffer, nHDRLen);
				BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
			}
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_PacketClient, nListCount);
	}
	return 0;
}
BOOL XEngine_Stream_Handle(SOCKET hSocket, LPCTSTR lpszMsgBuffer, int nMsgLen, LPCTSTR lpszHDRBuffer, int nHDRLen)
{
	TCHAR tszClientAddr[128];
	memset(tszClientAddr, '\0', sizeof(tszClientAddr));

	NetXApi_Socket_GetAddress(hSocket, tszClientAddr, FALSE);
	if (nHDRLen == sizeof(XENGINE_RTPPACKETHDR2016) + sizeof(XENGINE_RTPPACKETTAIL))
	{
		XENGINE_RTPPACKETHDR2016 st_RTPHdr;
		XENGINE_RTPPACKETTAIL st_RTPTail;

		memset(&st_RTPHdr, '\0', sizeof(XENGINE_RTPPACKETHDR2016));
		memset(&st_RTPTail, '\0', sizeof(XENGINE_RTPPACKETTAIL));

		memcpy(&st_RTPHdr, lpszHDRBuffer, sizeof(XENGINE_RTPPACKETHDR2016) - sizeof(WORD));
		memcpy(&st_RTPTail, lpszHDRBuffer + sizeof(XENGINE_RTPPACKETHDR2016) - sizeof(WORD), sizeof(XENGINE_RTPPACKETTAIL));

		XEngine_Stream_Handle2016(tszClientAddr, lpszMsgBuffer, nMsgLen, &st_RTPHdr, &st_RTPTail);
	}
	else
	{
		XENGINE_RTPPACKETHDR2014 st_RTPHdr;
		XENGINE_RTPPACKETTAIL st_RTPTail;

		memset(&st_RTPHdr, '\0', sizeof(XENGINE_RTPPACKETHDR2014));
		memset(&st_RTPTail, '\0', sizeof(XENGINE_RTPPACKETTAIL));

		memcpy(&st_RTPHdr, lpszHDRBuffer, sizeof(XENGINE_RTPPACKETHDR2014) - sizeof(WORD));
		memcpy(&st_RTPTail, lpszHDRBuffer + sizeof(XENGINE_RTPPACKETHDR2014) - sizeof(WORD), sizeof(XENGINE_RTPPACKETTAIL));

		XEngine_Stream_Handle2014(tszClientAddr, lpszMsgBuffer, nMsgLen, &st_RTPHdr, &st_RTPTail);
	}
	return TRUE;
}
BOOL XEngine_Stream_Handle2014(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_RTPPACKETHDR2014* pSt_RTPHdr, XENGINE_RTPPACKETTAIL* pSt_RTPTail)
{
	return TRUE;
}
BOOL XEngine_Stream_Handle2016(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_RTPPACKETHDR2016* pSt_RTPHdr, XENGINE_RTPPACKETTAIL* pSt_RTPTail)
{
	int nSDLen = 0;
	XNETHANDLE xhClient = 0;
	TCHAR tszSDBuffer[2048];
	TCHAR tszDeviceNumber[64];

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszDeviceNumber, '\0', sizeof(tszDeviceNumber));

	ModuleHelp_JT1078_BCDToString(pSt_RTPHdr->bySIMNumber, tszDeviceNumber);
	if (!ModuleSession_JT1078Client_Exist(&xhClient, lpszClientAddr, tszDeviceNumber, pSt_RTPHdr->byChannel, TRUE))
	{
		XENGINE_PROTOCOLDEVICE st_ProtoDevice;
		XENGINE_PROTOCOLSTREAM st_ProtoAttr;

		memset(&st_ProtoDevice, '\0', sizeof(XENGINE_PROTOCOLDEVICE));
		memset(&st_ProtoAttr, '\0', sizeof(XENGINE_PROTOCOLSTREAM));

		st_ProtoDevice.bLive = TRUE;
		st_ProtoDevice.nChannel = pSt_RTPHdr->byChannel;
		_tcscpy(st_ProtoDevice.tszDeviceNumber, tszDeviceNumber);
		//先移除老的,无论如何
		ModuleSession_JT1078Client_DeleteNumber(tszDeviceNumber, pSt_RTPHdr->byChannel, TRUE);
		ModuleSession_JT1078Client_Get(&xhClient);   //得到新的可用客户端
		if (!ModuleSession_JT1078Client_Insert(xhClient, lpszClientAddr, tszDeviceNumber, pSt_RTPHdr->byChannel, TRUE))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("实时端：%s,登录到服务,设备ID：%s,通道：%d,绑定客户端失败,错误：%lX"), lpszClientAddr, tszDeviceNumber, pSt_RTPHdr->byChannel, ModuleSession_GetLastError());
			return FALSE;
		}
		ModuleDB_JT1078_DeviceInsert(lpszClientAddr, tszDeviceNumber, pSt_RTPHdr->byChannel, xhClient, TRUE, "2016");
		//查询音视频数据,没有将无法使用特性
		memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
		_tcscpy(st_ProtoAttr.tszDeviceNumber, tszDeviceNumber);
		if (ModuleDB_JT1078_InfoQuery(&st_ProtoAttr))
		{
			ModuleProtocol_JT1078_StreamCreate(tszSDBuffer, &nSDLen, &st_ProtoDevice, &st_ProtoAttr);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("实时端：%s,登录到服务,设备ID：%s,通道：%d,从数据库缓存获取音视频属性成功！"), lpszClientAddr, tszDeviceNumber, pSt_RTPHdr->byChannel);
		}
		else
		{
			ModuleProtocol_JT1078_StreamCreate(tszSDBuffer, &nSDLen, &st_ProtoDevice);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("实时端：%s,登录到服务,设备ID：%s,通道：%d,音视频数据不存在数据库,请插入！"), lpszClientAddr, tszDeviceNumber, pSt_RTPHdr->byChannel);
		}
		//创建流
		XClient_TCPSelect_SendEx(xhClient, tszSDBuffer, &nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("实时端：%s,登录到服务,设备ID：%s,通道：%d,推流端客户端：%lld,设备版本：2016"), lpszClientAddr, tszDeviceNumber, pSt_RTPHdr->byChannel, xhClient);
	}
	ModuleProtocol_JT1078_StreamPush(tszSDBuffer, &nSDLen, pSt_RTPHdr, pSt_RTPTail, lpszMsgBuffer, nMsgLen, TRUE);
	XClient_TCPSelect_SendEx(xhClient, tszSDBuffer, &nSDLen);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("实时端：%s,开始推送数据,设备ID：%s,通道：%d,推送客户端：%lld,大小：%d"), lpszClientAddr, tszDeviceNumber, pSt_RTPHdr->byChannel, xhClient, nSDLen);
	return TRUE;
}