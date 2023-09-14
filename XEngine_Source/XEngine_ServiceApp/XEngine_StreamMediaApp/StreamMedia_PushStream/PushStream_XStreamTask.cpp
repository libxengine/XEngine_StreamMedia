#include "../XEngine_Hdr.h"
/********************************************************************
//    Created:     2023/07/09  15:30:53
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PushStream\PushStream_XStreamTask.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PushStream
//    File Base:   PushStream_XStreamTask
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     X推流任务处理代码
//    History:
*********************************************************************/
XHTHREAD CALLBACK PushStream_XStreamTask_Thread(XPVOID lParam)
{
	//任务池是编号1开始的.
	int nThreadPos = *(int*)lParam;
	nThreadPos++;
	while (bIsRun)
	{
		//等待编号1的任务池触发一个组完包的事件
		if (!HelpComponents_Datas_WaitEventEx(xhXStreamPacket, nThreadPos))
		{
			continue;
		}
		int nListCount = 0;
		XENGINE_MANAGEPOOL_TASKEVENT** ppSst_ListAddr;
		//获得编号1的所有待处理任务的客户端列表(也就是客户端发送过来的数据已经组好了一个包,需要我们处理)
		HelpComponents_Datas_GetPoolEx(xhXStreamPacket, nThreadPos, &ppSst_ListAddr, &nListCount);
		//先循环客户端
		for (int i = 0; i < nListCount; i++)
		{
			//再循环客户端拥有的任务个数
			for (int j = 0; j < ppSst_ListAddr[i]->nPktCount; j++)
			{
				int nMsgLen = 0;                             //客户端发送的数据大小,不包括头
				XCHAR* ptszMsgBuffer = NULL;                 //客户端发送的数据
				XENGINE_PROTOCOLHDR st_ProtocolHdr;          //客户端发送的数据的协议头
				//得到一个指定客户端的完整数据包
				if (HelpComponents_Datas_GetMemoryEx(xhXStreamPacket, ppSst_ListAddr[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
				{
					//在另外一个函数里面处理数据
					PushStream_XStreamTask_Handle(&st_ProtocolHdr, ppSst_ListAddr[i]->tszClientAddr, ptszMsgBuffer, nMsgLen);
					//释放内存
					BaseLib_OperatorMemory_FreeCStyle((VOID**)&ptszMsgBuffer);
				}
			}
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSst_ListAddr, nListCount);
	}
	return 0;
}
bool PushStream_XStreamTask_Handle(XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nRVLen = 0;
	int nSDLen = 0;
	XCHAR* ptszRVBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);
	XCHAR* ptszSDBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);
	
	if (ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_HEARTBEAT == pSt_ProtocolHdr->unOperatorType)
	{
		if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_HB_SYN == pSt_ProtocolHdr->unOperatorCode)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("XStream推流端：%s,接受流媒体返回的心跳"), lpszClientAddr);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("XStream推流端：%s,接受返回了无法处理的心跳协议类型"), lpszClientAddr, pSt_ProtocolHdr->unOperatorCode);
		}
	}
	else if (ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_SMS == pSt_ProtocolHdr->unOperatorType)
	{
		if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REQCREATE == pSt_ProtocolHdr->unOperatorCode)
		{
			XENGINE_PROTOCOLSTREAM st_ProtocolStream;
			memset(&st_ProtocolStream, '\0', sizeof(XENGINE_PROTOCOLSTREAM));

			memcpy(&st_ProtocolStream, lpszMsgBuffer, sizeof(XENGINE_PROTOCOLSTREAM));

			XEngine_AVPacket_AVCreate(lpszClientAddr);
			XEngine_AVPacket_AVSetTime(lpszClientAddr, st_ProtocolStream.st_AVInfo.st_VideoInfo.nFrameRate, st_ProtocolStream.st_AVInfo.st_AudioInfo.nSampleRate);
			//创建会话
			ModuleSession_PushStream_Create(lpszClientAddr, st_ProtocolStream.tszSMSAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_XSTREAM);
			ModuleSession_PushStream_SetAVInfo(lpszClientAddr, &st_ProtocolStream.st_AVInfo);

			pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REPCREATE;
			ModuleProtocol_Packet_Comm(ptszSDBuffer, &nSDLen, pSt_ProtocolHdr);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_XSTREAM);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("XStream推流端：%s,创建流成功,推流地址：%s,视频：%d,音频：%d"), lpszClientAddr, st_ProtocolStream.tszSMSAddr, st_ProtocolStream.st_AVInfo.st_VideoInfo.bEnable, st_ProtocolStream.st_AVInfo.st_AudioInfo.bEnable);
		}
		else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REQDESTROY == pSt_ProtocolHdr->unOperatorCode)
		{
			XCHAR tszSMSAddr[MAX_PATH];
			memset(tszSMSAddr, '\0', sizeof(tszSMSAddr));

			XEngine_AVPacket_AVDelete(lpszClientAddr);
			if (ModuleSession_PushStream_GetAddrForAddr(lpszClientAddr, tszSMSAddr))
			{
				ModuleSession_PushStream_Destroy(tszSMSAddr);
			}
			pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REPDESTROY;
			ModuleProtocol_Packet_Comm(ptszSDBuffer, &nSDLen, pSt_ProtocolHdr);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_XSTREAM);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("XStream推流端：%s,接受请求了销毁流消息,销毁地址：%s,"), lpszClientAddr, tszSMSAddr);
		}
		else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REQPUSH == pSt_ProtocolHdr->unOperatorCode)
		{
			XENGINE_PROTOCOL_AVDATA st_ProtocolAVInfo;

			memset(&st_ProtocolAVInfo, '\0', sizeof(XENGINE_PROTOCOL_AVDATA));
			memcpy(&st_ProtocolAVInfo, lpszMsgBuffer, sizeof(XENGINE_PROTOCOL_AVDATA));

			if (1 == st_ProtocolAVInfo.byFrameType)
			{
				XEngine_AVPacket_AVHdr(lpszClientAddr, lpszMsgBuffer, nMsgLen, 0, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_XSTREAM);
			}
			//转封装
			XEngine_AVPacket_AVFrame(ptszSDBuffer, &nSDLen, ptszRVBuffer, &nRVLen, lpszClientAddr, lpszMsgBuffer, nMsgLen, st_ProtocolAVInfo.nTimeStamp, st_ProtocolAVInfo.byAVType, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_XSTREAM);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("XStream推流端：%s,接受推流数据"), lpszClientAddr);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("XStream推流端：%s,接受返回了无法处理的推流协议类型"), lpszClientAddr, pSt_ProtocolHdr->unOperatorCode);
		}
	}
	else
	{
		//我们可以给客户端发送一条错误信息
		pSt_ProtocolHdr->wReserve = 0xFF;        //表示不支持的协议
		pSt_ProtocolHdr->unPacketSize = 0;       //设置没有后续数据包
		XEngine_Network_Send(lpszClientAddr, (LPCXSTR)pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR), ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_XSTREAM);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("XStream推流端:%s,主协议错误,协议：%x 不支持"), lpszClientAddr, pSt_ProtocolHdr->unOperatorType);
	}
	free(ptszRVBuffer);
	free(ptszSDBuffer);
	ptszRVBuffer = NULL;
	ptszSDBuffer = NULL;
	return true;
}