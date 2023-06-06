﻿#include "../XEngine_Hdr.h"
/********************************************************************
//    Created:     2023/06/04  12:56:07
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PushStream\PushStream_JT1078Task.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PushStream
//    File Base:   PushStream_JT1078Task
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     JT1078流处理器
//    History:
*********************************************************************/
XHTHREAD CALLBACK PushStream_JT1078Task_Thread(XPVOID lParam)
{
	int nThreadPos = *(int*)lParam;
	nThreadPos++;

	while (bIsRun)
	{
		if (!HelpComponents_PKTCustom_WaitEventEx(xhJT1078Pkt, nThreadPos))
		{
			continue;
		}
		int nListCount = 0;
		HELPCOMPONENT_PACKET_CLIENT **ppSt_PacketClient;
		HelpComponents_PKTCustom_GetPoolEx(xhJT1078Pkt, nThreadPos, &ppSt_PacketClient, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			for (int j = 0; j < ppSt_PacketClient[i]->nPktCount; j++)
			{
				int nMsgLen = 0;
				int nHDRLen = 0;
				XCHAR* ptszMsgBuffer;
				XCHAR tszHdrBuffer[MAX_PATH];
				memset(tszHdrBuffer, '\0', MAX_PATH);

				if (!HelpComponents_PKTCustom_GetMemoryEx(xhJT1078Pkt, ppSt_PacketClient[i]->hSocket, &ptszMsgBuffer, &nMsgLen, tszHdrBuffer, &nHDRLen))
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("实时端,获取一个随机包失败,错误：%lX"), Packets_GetLastError());
					break;
				}
				PushStream_JT1078Task_Handle(ppSt_PacketClient[i]->hSocket, ptszMsgBuffer, nMsgLen, tszHdrBuffer, nHDRLen);
				BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
			}
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_PacketClient, nListCount);
	}
	return 0;
}
bool PushStream_JT1078Task_Handle(XSOCKET hSocket, LPCXSTR lpszMsgBuffer, int nMsgLen, LPCXSTR lpszHDRBuffer, int nHDRLen)
{
	XCHAR tszClientAddr[128];
	memset(tszClientAddr, '\0', sizeof(tszClientAddr));

	if (nHDRLen == sizeof(XENGINE_RTPPACKETHDR2016) + sizeof(XENGINE_RTPPACKETTAIL) + sizeof(XSHOT))
	{
		XENGINE_RTPPACKETHDR2016 st_RTPHdr;
		XENGINE_RTPPACKETTAIL st_RTPTail;

		memset(&st_RTPHdr, '\0', sizeof(XENGINE_RTPPACKETHDR2016));
		memset(&st_RTPTail, '\0', sizeof(XENGINE_RTPPACKETTAIL));

		memcpy(&st_RTPHdr, lpszHDRBuffer, sizeof(XENGINE_RTPPACKETHDR2016) - sizeof(XSHOT));
		memcpy(&st_RTPTail, lpszHDRBuffer + sizeof(XENGINE_RTPPACKETHDR2016) - sizeof(XSHOT), sizeof(XENGINE_RTPPACKETTAIL));

		PushStream_JT1078Task_Handle2016(tszClientAddr, lpszMsgBuffer, nMsgLen, &st_RTPHdr, &st_RTPTail);
	}
	else
	{
		XENGINE_RTPPACKETHDR2014 st_RTPHdr;
		XENGINE_RTPPACKETTAIL st_RTPTail;

		memset(&st_RTPHdr, '\0', sizeof(XENGINE_RTPPACKETHDR2014));
		memset(&st_RTPTail, '\0', sizeof(XENGINE_RTPPACKETTAIL));

		memcpy(&st_RTPHdr, lpszHDRBuffer, sizeof(XENGINE_RTPPACKETHDR2014) - sizeof(XSHOT));
		memcpy(&st_RTPTail, lpszHDRBuffer + sizeof(XENGINE_RTPPACKETHDR2014) - sizeof(XSHOT), sizeof(XENGINE_RTPPACKETTAIL));

		PushStream_JT1078Task_Handle2014(tszClientAddr, lpszMsgBuffer, nMsgLen, &st_RTPHdr, &st_RTPTail);
	}
	return true;
}
bool PushStream_JT1078Task_Handle2014(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_RTPPACKETHDR2014* pSt_RTPHdr, XENGINE_RTPPACKETTAIL* pSt_RTPTail)
{
	return true;
}
bool PushStream_JT1078Task_Handle2016(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_RTPPACKETHDR2016* pSt_RTPHdr, XENGINE_RTPPACKETTAIL* pSt_RTPTail)
{
	int nSDLen = 0;
	XNETHANDLE xhToken = 0;
	XCHAR tszSDBuffer[2048];
	XCHAR tszSMSAddr[MAX_PATH];
	XCHAR tszDeviceNumber[128];
	XENGINE_PROTOCOLSTREAM st_ProtocolStream;
	
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszSMSAddr, '\0', sizeof(tszSMSAddr));
	memset(tszDeviceNumber, '\0', sizeof(tszDeviceNumber));
	memset(&st_ProtocolStream, '\0', sizeof(XENGINE_PROTOCOLSTREAM));

	st_ProtocolStream.bLive = true;
	ModuleHelp_JT1078_BCDToString(pSt_RTPHdr->bySIMNumber, tszDeviceNumber);
	_xstprintf(tszSMSAddr, _X("%s_%d"), tszDeviceNumber, pSt_RTPHdr->byChannel);
	

	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("实时端：%s,开始推送数据,设备ID：%s,通道：%d,推送客户端：%lld,大小：%d"), lpszClientAddr, tszDeviceNumber, pSt_RTPHdr->byChannel, nSDLen);
	return true;
}