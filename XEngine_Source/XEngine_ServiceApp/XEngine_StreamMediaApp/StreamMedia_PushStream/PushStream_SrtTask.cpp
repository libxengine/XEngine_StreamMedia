﻿#include "../XEngine_Hdr.h"
/********************************************************************
//    Created:     2023/07/28  16:12:02
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PushStream\PushStream_SrtTask.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PushStream
//    File Base:   PushStream_SrtTask
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     SRT推拉流代码
//    History:
*********************************************************************/
bool PushStream_SrtTask_Connct(LPCXSTR lpszClientAddr, XSOCKET hSocket)
{
	bool bPublish = false;
	XCHAR tszSMSAddr[MAX_PATH];
	memset(tszSMSAddr, '\0', sizeof(tszSMSAddr));

	if (!ModuleHelp_SrtCore_GetStreamID(hSocket, tszSMSAddr, &bPublish))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("SRT客户端：获取流ID信息失败,错误码：%lX"), ModuleHelp_GetLastError());
		return false;
	}

	if (bPublish)
	{
		//创建会话
		ModuleSession_PushStream_Create(lpszClientAddr, tszSMSAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_SRT);
		XEngine_AVPacket_AVCreate(lpszClientAddr);
		//need to parse ts stream
		HLSProtocol_TSParse_Insert(lpszClientAddr);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("SRT客户端：%s,创建流成功,推流地址：%s,类型:推流端"), lpszClientAddr, tszSMSAddr);
	}
	else
	{
		XCHAR tszPushAddr[128];
		memset(tszPushAddr, '\0', sizeof(tszPushAddr));

		bool bSMSFound = false;
		//得到推流地址
		if (ModuleSession_PushStream_FindStream(tszSMSAddr, tszPushAddr))
		{
			bSMSFound = true;
		}
		else
		{
			if (!st_ServiceConfig.st_XPull.st_PullSrt.bPrePull)
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("SRT客户端：%s,请求拉流的参数不正确:%s,错误:%lX"), lpszClientAddr, tszSMSAddr, ModuleSession_GetLastError());
				return false;
			}
			bSMSFound = false;
		}
		ModuleSession_PullStream_Insert(lpszClientAddr, tszSMSAddr, tszPushAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_SRT);
		if (bSMSFound)
		{
			ModuleSession_PushStream_ClientInsert(tszPushAddr, lpszClientAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_SRT);
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("SRT客户端：%s,拉取流成功,拉流地址：%s,类型:拉流端"), lpszClientAddr, tszSMSAddr);
	}
	return true;
}

bool PushStream_SrtTask_Handle(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	if (!HLSProtocol_TSParse_Send(lpszClientAddr, lpszMsgBuffer, nMsgLen))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("SRT客户端：%s,请求数据推流,错误:%lX"), lpszClientAddr, HLSProtocol_GetLastError());
		return false;
	}
	return true;
}

XHTHREAD CALLBACK PushStream_SRTTask_Thread(XPVOID lParam)
{
	//任务池是编号1开始的.
	int nThreadPos = *(int*)lParam;
	nThreadPos++;

	XCHAR* ptszMsgBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);
	while (bIsRun)
	{
		//等待编号1的任务池触发一个组完包的事件
		if (!HLSProtocol_TSParse_WaitEvent(nThreadPos))
		{
			continue;
		}
		int nListCount = 0;
		XENGINE_MANAGEPOOL_TASKEVENT** ppSst_ListAddr;
		//获得编号1的所有待处理任务的客户端列表(也就是客户端发送过来的数据已经组好了一个包,需要我们处理)
		HLSProtocol_TSParse_GetPool(nThreadPos, &ppSst_ListAddr, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			//再循环客户端拥有的任务个数
			for (int j = 0; j < ppSst_ListAddr[i]->nPktCount; j++)
			{
				int nMsgLen = 0;                             //客户端发送的数据大小,不包括头
				XBYTE byAVType = 0;
				//得到一个指定客户端的完整数据包
				if (HLSProtocol_TSParse_Recv(ppSst_ListAddr[i]->tszClientAddr, ptszMsgBuffer, &nMsgLen, &byAVType))
				{
					//在另外一个函数里面处理数据
					PushStream_SrtTask_ThreadProcess(ppSst_ListAddr[i]->tszClientAddr, ptszMsgBuffer, nMsgLen, byAVType);
				}
			}
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSst_ListAddr, nListCount);
	}
	free(ptszMsgBuffer);
	ptszMsgBuffer = NULL;
	return 0;
}
bool PushStream_SrtTask_ThreadProcess(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XBYTE byAVType)
{
	int nRVLen = 0;
	int nSDLen = 0;
	XCHAR* ptszRVBuffer = (XCHAR*)ManagePool_Memory_Alloc(xhMemoryPool, XENGINE_MEMORY_SIZE_MAX);
	XCHAR* ptszSDBuffer = (XCHAR*)ManagePool_Memory_Alloc(xhMemoryPool, XENGINE_MEMORY_SIZE_MAX);

	if (0x1b == byAVType)
	{
		int nPos = 0;
		int nNALLen = 0;
		int nFIXLen = 0;
		XENGINE_AVCODEC_VIDEOFRAMETYPE enFrameType;
		
		AVHelp_Parse_NaluHdr(lpszMsgBuffer, nMsgLen, &nNALLen, &nFIXLen);
		AVHelp_Parse_NaluType(lpszMsgBuffer + nPos, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264, &enFrameType);
		//如果是AUD单元,跳过AUD
		if (ENUM_XENGINE_AVCODEC_VIDEO_FRAMETYPE_AUD == enFrameType)
		{
			nPos = nNALLen;
			//重新获取
			AVHelp_Parse_NaluType(lpszMsgBuffer + nPos, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264, &enFrameType);
		}
		//如果是关键帧
		if (ENUM_XENGINE_AVCODEC_VIDEO_FRAMETYPE_SPS == enFrameType || ENUM_XENGINE_AVCODEC_VIDEO_FRAMETYPE_PPS == enFrameType || ENUM_XENGINE_AVCODEC_VIDEO_FRAMETYPE_SEI == enFrameType)
		{
			XEngine_AVPacket_AVHdr(lpszClientAddr, lpszMsgBuffer + nPos, nMsgLen - nPos, 0, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_SRT);
		}
		XEngine_AVPacket_AVFrame(ptszSDBuffer, &nSDLen, ptszRVBuffer, &nRVLen, lpszClientAddr, lpszMsgBuffer + nPos, nMsgLen - nPos, 0);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("SRT推流端：%s,接受视频推流数据,数据大小:%d,帧类型:%d,跳过AUD:%d"), lpszClientAddr, nMsgLen, enFrameType, nPos);
		
	}
	else if (0x24 == byAVType)
	{
		//H265
	}
	else if (0x0f == byAVType)
	{
		//AAC
		XEngine_AVPacket_AVHdr(lpszClientAddr, lpszMsgBuffer, nMsgLen, 1, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_SRT);
		XEngine_AVPacket_AVFrame(ptszSDBuffer, &nSDLen, ptszRVBuffer, &nRVLen, lpszClientAddr, lpszMsgBuffer + 7, nMsgLen - 7, 1);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("SRT推流端：%s,接受音频推流数据,数据大小:%d"), lpszClientAddr, nMsgLen);
	}

	ManagePool_Memory_Free(xhMemoryPool, ptszRVBuffer);
	ManagePool_Memory_Free(xhMemoryPool, ptszSDBuffer);
	return true;
}