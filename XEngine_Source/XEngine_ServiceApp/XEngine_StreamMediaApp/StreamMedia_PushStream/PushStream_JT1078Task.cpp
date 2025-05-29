#include "../XEngine_Hdr.h"
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
XHTHREAD XCALLBACK PushStream_JT1078Task_Thread(XPVOID lParam)
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
		XENGINE_MANAGEPOOL_TASKEVENT**ppSt_PacketClient;
		HelpComponents_PKTCustom_GetPoolEx(xhJT1078Pkt, nThreadPos, &ppSt_PacketClient, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			for (int j = 0; j < ppSt_PacketClient[i]->nPktCount; j++)
			{
				int nMsgLen = 0;
				int nHDRLen = 0;
				XCHAR* ptszMsgBuffer;
				XCHAR tszHdrBuffer[XPATH_MAX];
				memset(tszHdrBuffer, '\0', XPATH_MAX);

				if (HelpComponents_PKTCustom_GetMemoryEx(xhJT1078Pkt, ppSt_PacketClient[i]->hSocket, &ptszMsgBuffer, &nMsgLen, tszHdrBuffer, &nHDRLen))
				{
					PushStream_JT1078Task_Handle(ppSt_PacketClient[i]->hSocket, ptszMsgBuffer, nMsgLen, tszHdrBuffer, nHDRLen);
					BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
				}
			}
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_PacketClient, nListCount);
	}
	return 0;
}
bool PushStream_JT1078Task_Handle(XSOCKET hSocket, LPCXSTR lpszMsgBuffer, int nMsgLen, LPCXSTR lpszHDRBuffer, int nHDRLen)
{
	XCHAR tszClientAddr[128];
	memset(tszClientAddr, '\0', sizeof(tszClientAddr));

	if (nHDRLen == sizeof(XENGINE_RTPPACKETHDR) + sizeof(XENGINE_RTPPACKETTAIL) + sizeof(XSHOT))
	{
		XENGINE_RTPPACKETHDR st_RTPHdr;
		XENGINE_RTPPACKETTAIL st_RTPTail;

		memset(&st_RTPHdr, '\0', sizeof(XENGINE_RTPPACKETHDR));
		memset(&st_RTPTail, '\0', sizeof(XENGINE_RTPPACKETTAIL));

		memcpy(&st_RTPHdr, lpszHDRBuffer, sizeof(XENGINE_RTPPACKETHDR) - sizeof(XSHOT));
		memcpy(&st_RTPTail, lpszHDRBuffer + sizeof(XENGINE_RTPPACKETHDR) - sizeof(XSHOT), sizeof(XENGINE_RTPPACKETTAIL));

		XSocket_Api_GetAddress(hSocket, tszClientAddr, false);
		PushStream_JT1078Task_Handle(tszClientAddr, lpszMsgBuffer, nMsgLen, &st_RTPHdr, &st_RTPTail);
	}
	else
	{
		
	}
	return true;
}

bool PushStream_JT1078Task_Handle(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_RTPPACKETHDR* pSt_RTPHdr, XENGINE_RTPPACKETTAIL* pSt_RTPTail)
{
	int nRVLen = 0;
	int nSDLen = 0;
	XCHAR tszSMSAddr[XPATH_MAX];
	XCHAR tszDeviceNumber[128];
	XCHAR* ptszRVBuffer = (XCHAR*)ManagePool_Memory_Alloc(xhMemoryPool, XENGINE_MEMORY_SIZE_MAX);
	XCHAR* ptszSDBuffer = (XCHAR*)ManagePool_Memory_Alloc(xhMemoryPool, XENGINE_MEMORY_SIZE_MAX);

	memset(tszSMSAddr, '\0', sizeof(tszSMSAddr));
	memset(tszDeviceNumber, '\0', sizeof(tszDeviceNumber));

	ModuleHelp_JT1078_BCDToString(pSt_RTPHdr->bySIMNumber, tszDeviceNumber);
	_xstprintf(tszSMSAddr, _X("live/%s_%d"), tszDeviceNumber, pSt_RTPHdr->byChannel);

	XCHAR tszPushAddr[128];
	memset(tszPushAddr, '\0', sizeof(tszPushAddr));
	if (!ModuleSession_PushStream_FindStream(tszSMSAddr, tszPushAddr))
	{
		//没有就创建
		ModuleSession_PushStream_Create(lpszClientAddr, tszSMSAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_JT1078);
		ModuleQueue_JT1078_Create(lpszClientAddr);
		XEngine_AVPacket_AVCreate(lpszClientAddr);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("JT1078:%s,流媒体服务创建,设备ID：%s,通道:%d,创建地址:%s"), lpszClientAddr, tszDeviceNumber, pSt_RTPHdr->byChannel, tszSMSAddr);
	}
	
	if (3 == pSt_RTPHdr->byType)
	{
		//音频
		ModuleQueue_JT1078_Insert(lpszClientAddr, lpszMsgBuffer, nMsgLen);
		if (pSt_RTPHdr->byM)
		{
			int nMLen = 0;
			XCHAR* ptszMsgBuffer = NULL;
			ModuleQueue_JT1078_Get(lpszClientAddr, &ptszMsgBuffer, &nMLen);
			if (nMLen > 0)
			{
				XEngine_AVPacket_AVHdr(lpszClientAddr, ptszMsgBuffer, nMLen, 1, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_JT1078);
				BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
			}
		}
	}
	else if (4 == pSt_RTPHdr->byType)
	{
		//透传
	}
	else
	{
		//视频
		ModuleQueue_JT1078_Insert(lpszClientAddr, lpszMsgBuffer, nMsgLen);
		if (pSt_RTPHdr->byM)
		{
			int nMLen = 0;
			XCHAR* ptszMsgBuffer = NULL;
			ModuleQueue_JT1078_Get(lpszClientAddr, &ptszMsgBuffer, &nMLen);
			if (nMLen > 0)
			{
				if (0 == pSt_RTPHdr->byType)
				{
					XEngine_AVPacket_AVHdr(lpszClientAddr, ptszMsgBuffer, nMLen, 0, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_JT1078);
				}
				XEngine_AVPacket_AVFrame(ptszSDBuffer, &nSDLen, ptszRVBuffer, &nRVLen, lpszClientAddr, ptszMsgBuffer, nMLen, 0);
			}
		}
	}
	ManagePool_Memory_Free(xhMemoryPool, ptszRVBuffer);
	ManagePool_Memory_Free(xhMemoryPool, ptszSDBuffer);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("JT1078:%s,开始推送数据,设备ID：%s,通道:%d,大小:%d"), lpszClientAddr, tszDeviceNumber, pSt_RTPHdr->byChannel, nMsgLen);
	return true;
}