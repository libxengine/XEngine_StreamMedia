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
XHTHREAD CALLBACK XEngine_Stream_Thread(XPVOID lParam)
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
bool XEngine_Stream_Handle(XSOCKET hSocket, LPCXSTR lpszMsgBuffer, int nMsgLen, LPCXSTR lpszHDRBuffer, int nHDRLen)
{
	TCHAR tszClientAddr[128];
	memset(tszClientAddr, '\0', sizeof(tszClientAddr));

	NetXApi_Socket_GetAddress(hSocket, tszClientAddr, false);
	if (nHDRLen == sizeof(XENGINE_RTPPACKETHDR2016) + sizeof(XENGINE_RTPPACKETTAIL) + sizeof(WORD))
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
	return true;
}
bool XEngine_Stream_Handle2014(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_RTPPACKETHDR2014* pSt_RTPHdr, XENGINE_RTPPACKETTAIL* pSt_RTPTail)
{
	return true;
}
bool XEngine_Stream_Handle2016(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_RTPPACKETHDR2016* pSt_RTPHdr, XENGINE_RTPPACKETTAIL* pSt_RTPTail)
{
	int nSDLen = 0;
	XNETHANDLE xhToken = 0;
	TCHAR tszSDBuffer[2048];
	XENGINE_PROTOCOLDEVICE st_ProtoDevice;
	
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(&st_ProtoDevice, '\0', sizeof(XENGINE_PROTOCOLDEVICE));

	st_ProtoDevice.bLive = true;
	st_ProtoDevice.nChannel = pSt_RTPHdr->byChannel;
	ModuleHelp_JT1078_BCDToString(pSt_RTPHdr->bySIMNumber, st_ProtoDevice.tszDeviceNumber);
	
	if (!ModuleSession_Client_Exist(&xhToken, lpszClientAddr, st_ProtoDevice.tszDeviceNumber, pSt_RTPHdr->byChannel, true))
	{
		//先移除老的,无论如何
		ModuleSession_Client_DeleteNumber(st_ProtoDevice.tszDeviceNumber, pSt_RTPHdr->byChannel, true);
		ModuleSession_Client_Get(&xhToken);   //得到新的可用客户端
		if (!ModuleSession_Client_Insert(xhToken, lpszClientAddr, st_ProtoDevice.tszDeviceNumber, pSt_RTPHdr->byChannel, true))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("实时端：%s,登录到服务,设备ID：%s,通道：%d,绑定客户端失败,错误：%lX"), lpszClientAddr, st_ProtoDevice.tszDeviceNumber, pSt_RTPHdr->byChannel, ModuleSession_GetLastError());
			return false;
		}
		//创建流
		ModuleProtocol_Packet_Create(tszSDBuffer, &nSDLen, &st_ProtoDevice);
		XClient_TCPSelect_SendEx(xhClient, xhToken, tszSDBuffer, nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("实时端：%s,登录到服务,设备ID：%s,通道：%d,推流端客户端：%lld,设备版本：2016"), lpszClientAddr, st_ProtoDevice.tszDeviceNumber, pSt_RTPHdr->byChannel, xhClient);
	}
	ModuleProtocol_Packet_Push(tszSDBuffer, &nSDLen, &st_ProtoDevice, nMsgLen, 0);
	XClient_TCPSelect_SendEx(xhClient, xhToken, tszSDBuffer, nSDLen);
	XClient_TCPSelect_SendEx(xhClient, xhToken, lpszMsgBuffer, nMsgLen);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("实时端：%s,开始推送数据,设备ID：%s,通道：%d,推送客户端：%lld,大小：%d"), lpszClientAddr, st_ProtoDevice.tszDeviceNumber, pSt_RTPHdr->byChannel, xhClient, nSDLen);
	return true;
}