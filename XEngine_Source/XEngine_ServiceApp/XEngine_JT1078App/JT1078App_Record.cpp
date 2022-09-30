#include "JT1078App_Hdr.h"
/********************************************************************
//    Created:     2022/04/24  14:40:21
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_JT1078App\JT1078App_Record.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_JT1078App
//    File Base:   JT1078App_Record
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     录像操作
//    History:
*********************************************************************/
XHTHREAD CALLBACK XEngine_Record_Thread(LPVOID lParam)
{
	int nThreadPos = *(int*)lParam;
	nThreadPos++;

	while (bIsRun)
	{
		if (!HelpComponents_PKTCustom_WaitEventEx(xhRecordPkt, nThreadPos))
		{
			continue;
		}
		int nListCount = 0;
		HELPCOMPONENT_PACKET_CLIENT** ppSt_ListClient;
		HelpComponents_PKTCustom_GetPoolEx(xhRecordPkt, nThreadPos, &ppSt_ListClient, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			for (int j = 0; j < ppSt_ListClient[i]->nPktCount; j++)
			{
				int nMsgLen = 0;
				int nHDRLen = 0;
				TCHAR* ptszMsgBuffer;
				TCHAR tszHDRBuffer[MAX_PATH];

				memset(tszHDRBuffer, '\0', MAX_PATH);
				if (!HelpComponents_PKTCustom_GetMemoryEx(xhRecordPkt, ppSt_ListClient[i]->hSocket, &ptszMsgBuffer, &nMsgLen, tszHDRBuffer, &nHDRLen))
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("录像端，获取一个随机包失败，错误：%lX"), Packets_GetLastError());
					break;
				}
				XEngine_Record_Handle(ppSt_ListClient[i]->hSocket, ptszMsgBuffer, nMsgLen, tszHDRBuffer, nHDRLen);
				BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
			}
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListClient, nListCount);
	}
	return 0;
}
BOOL XEngine_Record_Handle(SOCKET hSocket, LPCTSTR lpszMsgBuffer, int nMsgLen, LPCTSTR lpszHDRBuffer, int nHDRLen)
{
	TCHAR tszClientAddr[128];
	memset(tszClientAddr, '\0', sizeof(tszClientAddr));

	NetXApi_Socket_GetAddress(hSocket, tszClientAddr, FALSE);
	if (nHDRLen + sizeof(XENGINE_RTPPACKETHDR2016) + sizeof(XENGINE_RTPPACKETTAIL) + sizeof(WORD))
	{
		XENGINE_RTPPACKETHDR2016 st_RTPHdr;
		XENGINE_RTPPACKETTAIL st_RTPTail;

		memset(&st_RTPHdr, '\0', sizeof(XENGINE_RTPPACKETHDR2016));
		memset(&st_RTPTail, '\0', sizeof(XENGINE_RTPPACKETTAIL));

		memcpy(&st_RTPHdr, lpszHDRBuffer, sizeof(XENGINE_RTPPACKETHDR2016) - sizeof(WORD));
		memcpy(&st_RTPTail, lpszHDRBuffer + sizeof(XENGINE_RTPPACKETHDR2016) - sizeof(WORD), sizeof(XENGINE_RTPPACKETTAIL));

		XEngine_Record_Handle2016(tszClientAddr, lpszMsgBuffer, nMsgLen, &st_RTPHdr, &st_RTPTail);
	}
	else
	{
		XENGINE_RTPPACKETHDR2014 st_RTPHdr;
		XENGINE_RTPPACKETTAIL st_RTPTail;

		memset(&st_RTPHdr, '\0', sizeof(XENGINE_RTPPACKETHDR2014));
		memset(&st_RTPTail, '\0', sizeof(XENGINE_RTPPACKETTAIL));

		memcpy(&st_RTPHdr, lpszHDRBuffer, sizeof(XENGINE_RTPPACKETHDR2014) - sizeof(WORD));
		memcpy(&st_RTPTail, lpszHDRBuffer + sizeof(XENGINE_RTPPACKETHDR2014) - sizeof(WORD), sizeof(XENGINE_RTPPACKETTAIL));

		XEngine_Record_Handle2014(tszClientAddr, lpszMsgBuffer, nMsgLen, &st_RTPHdr, &st_RTPTail);
	}
	
	return TRUE;
}
BOOL XEngine_Record_Handle2014(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_RTPPACKETHDR2014* pSt_RTPHdr, XENGINE_RTPPACKETTAIL* pSt_RTPTail)
{
	return TRUE;
}
BOOL XEngine_Record_Handle2016(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, XENGINE_RTPPACKETHDR2016* pSt_RTPHdr, XENGINE_RTPPACKETTAIL* pSt_RTPTail)
{
	int nSDLen = 0;
	XNETHANDLE xhClient = 0;
	TCHAR tszSDBuffer[2048];
	XENGINE_PROTOCOLDEVICE st_ProtoDevice;

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(&st_ProtoDevice, '\0', sizeof(XENGINE_PROTOCOLDEVICE));

	st_ProtoDevice.bLive = FALSE;
	st_ProtoDevice.nChannel = pSt_RTPHdr->byChannel;
	ModuleHelp_JT1078_BCDToString(pSt_RTPHdr->bySIMNumber, st_ProtoDevice.tszDeviceNumber);

	if (!ModuleSession_Client_Exist(&xhClient, lpszClientAddr, st_ProtoDevice.tszDeviceNumber, pSt_RTPHdr->byChannel, FALSE))
	{
		//先移除老的,无论如何
		ModuleSession_Client_DeleteNumber(st_ProtoDevice.tszDeviceNumber, pSt_RTPHdr->byChannel, FALSE);
		ModuleSession_Client_Get(&xhClient);   //得到新的可用客户端
		if (!ModuleSession_Client_Insert(xhClient, lpszClientAddr, st_ProtoDevice.tszDeviceNumber, pSt_RTPHdr->byChannel, FALSE))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("录像端：%s,登录到服务,设备ID：%s,通道：%d,绑定客户端失败,错误：%lX"), lpszClientAddr, st_ProtoDevice.tszDeviceNumber, pSt_RTPHdr->byChannel, ModuleSession_GetLastError());
			return FALSE;
		}
		//创建流
		ModuleProtocol_Packet_Create(tszSDBuffer, &nSDLen, &st_ProtoDevice);
		XClient_TCPSelect_SendEx(xhClient, tszSDBuffer, &nSDLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("录像端：%s,登录到服务,设备ID：%s,通道：%d,推流端客户端：%lld,设备版本：2016"), lpszClientAddr, st_ProtoDevice.tszDeviceNumber, pSt_RTPHdr->byChannel, xhClient);
	}
	ModuleProtocol_Packet_Push(tszSDBuffer, &nSDLen, &st_ProtoDevice, nMsgLen, 0);
	XClient_TCPSelect_SendEx(xhClient, tszSDBuffer, &nSDLen);
	XClient_TCPSelect_SendEx(xhClient, lpszMsgBuffer, &nMsgLen);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("录像端：%s,开始推送数据,设备ID：%s,通道：%d,推送客户端：%lld,大小：%d"), lpszClientAddr, st_ProtoDevice.tszDeviceNumber, pSt_RTPHdr->byChannel, xhClient, nSDLen);
	return TRUE;
}