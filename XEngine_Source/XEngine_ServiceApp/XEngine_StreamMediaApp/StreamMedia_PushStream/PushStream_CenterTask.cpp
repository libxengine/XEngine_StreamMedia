#include "../XEngine_Hdr.h"
/********************************************************************
//    Created:     2023/06/04  12:57:37
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PushStream\PushStream_CenterTask.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PushStream
//    File Base:   PushStream_CenterTask
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     X推流任务处理代码
//    History:
*********************************************************************/
XHTHREAD CALLBACK PushStream_CenterTask_Thread(XPVOID lParam)
{
	//任务池是编号1开始的.
	int nThreadPos = *(int*)lParam;
	nThreadPos++;
	while (bIsRun)
	{
		//等待编号1的任务池触发一个组完包的事件
		if (!HelpComponents_Datas_WaitEventEx(xhCenterPacket, nThreadPos))
		{
			continue;
		}
		int nListCount = 0;
		HELPCOMPONENT_PACKET_CLIENT** ppSst_ListAddr;
		//获得编号1的所有待处理任务的客户端列表(也就是客户端发送过来的数据已经组好了一个包,需要我们处理)
		HelpComponents_Datas_GetPoolEx(xhCenterPacket, nThreadPos, &ppSst_ListAddr, &nListCount);
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
				if (HelpComponents_Datas_GetMemoryEx(xhCenterPacket, ppSst_ListAddr[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
				{
					//在另外一个函数里面处理数据
					PushStream_CenterTask_Handle(&st_ProtocolHdr, ppSst_ListAddr[i]->tszClientAddr, ptszMsgBuffer, nMsgLen);
					//释放内存
					BaseLib_OperatorMemory_FreeCStyle((VOID**)&ptszMsgBuffer);
				}
			}
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSst_ListAddr, nListCount);
	}
	return 0;
}
bool PushStream_CenterTask_Handle(XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nRVLen = 0;
	int nSDLen = 0;
	XCHAR tszRVBuffer[10240];
	XCHAR tszSDBuffer[10240];
	
	memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

	if (ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_HEARTBEAT == pSt_ProtocolHdr->unOperatorType)
	{
		if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_HB_SYN == pSt_ProtocolHdr->unOperatorCode)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("XEngine推流端：%s,接受流媒体返回的心跳"), lpszClientAddr);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("XEngine推流端：%s,接受返回了无法处理的心跳协议类型"), lpszClientAddr, pSt_ProtocolHdr->unOperatorCode);
		}
	}
	else if (ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_SMS == pSt_ProtocolHdr->unOperatorType)
	{
		if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REQCREATE == pSt_ProtocolHdr->unOperatorCode)
		{
			XENGINE_PROTOCOLSTREAM st_ProtocolStream;
			memset(&st_ProtocolStream, '\0', sizeof(XENGINE_PROTOCOLSTREAM));

			memcpy(&st_ProtocolStream, lpszMsgBuffer, sizeof(XENGINE_PROTOCOLSTREAM));
			//创建流
			XNETHANDLE xhToken = 0;
			FLVProtocol_Packet_Create(&xhToken, st_ProtocolStream.st_AVInfo.st_VideoInfo.bEnable, st_ProtocolStream.st_AVInfo.st_AudioInfo.bEnable);
			int nPLen = 0;
			int nHLen = 0;
			XCHAR tszHDRBuffer[1024];
			memset(tszHDRBuffer, '\0', sizeof(tszHDRBuffer));
			//打包FLV
			FLVProtocol_Packet_FrameHdr(xhToken, tszHDRBuffer, &nPLen);
			nHLen += nPLen;
			st_ProtocolStream.st_AVInfo.st_VideoInfo.enAVCodec = 7;
			_tcsxcpy(st_ProtocolStream.st_AVInfo.tszPktName, _X("Lavf58.76.100"));

			FLVProtocol_Packet_FrameScript(xhToken, tszHDRBuffer + nHLen, &nPLen, &st_ProtocolStream.st_AVInfo);
			nHLen += nPLen;
			if (st_ProtocolStream.st_AVInfo.st_VideoInfo.bEnable)
			{
				FLVProtocol_Packet_FrameAVCConfigure(xhToken, tszHDRBuffer + nHLen, &nPLen, &st_ProtocolStream.st_AVInfo);
				nHLen += nPLen;
				if (st_ServiceConfig.bDebug)
				{
					pSt_VFile = _xtfopen(_X("./XEngine_Debug/Video.flv"), _X("wb"));
					fwrite(tszHDRBuffer, 1, nHLen, pSt_VFile);
				}
			}
			if (st_ProtocolStream.st_AVInfo.st_AudioInfo.bEnable)
			{
				FLVProtocol_Packet_FrameAACConfigure(xhToken, tszHDRBuffer + nHLen, &nPLen, &st_ProtocolStream.st_AVInfo);
				nHLen += nPLen;
				if (st_ServiceConfig.bDebug)
				{
					pst_AFile = _xtfopen(_X("./XEngine_Debug/Audio.flv"), _X("wb"));
					fwrite(tszHDRBuffer, 1, nHLen, pst_AFile);
				}
			}
			//创建会话
			ModuleSession_PushStream_Create(lpszClientAddr, st_ProtocolStream.tszSMSAddr, xhToken);
			ModuleSession_PushStream_SetHDRBuffer(lpszClientAddr, tszHDRBuffer, nHLen);

			pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REPCREATE;
			ModuleProtocol_Packet_Comm(tszSDBuffer, &nSDLen, pSt_ProtocolHdr);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_CENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("XEngine推流端：%s,创建流成功,推流地址：%s,视频：%d,音频：%d"), lpszClientAddr, st_ProtocolStream.tszSMSAddr, st_ProtocolStream.bLive, st_ProtocolStream.st_AVInfo.st_VideoInfo.bEnable, st_ProtocolStream.st_AVInfo.st_AudioInfo.bEnable);
		}
		else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REQDESTROY == pSt_ProtocolHdr->unOperatorCode)
		{
			XNETHANDLE xhToken = 0;
			XCHAR tszSMSAddr[MAX_PATH];

			memset(tszSMSAddr, '\0', sizeof(tszSMSAddr));

			if (ModuleSession_PushStream_GetTokenForAddr(lpszClientAddr, &xhToken))
			{
				FLVProtocol_Packet_Destory(xhToken);
			}
			if (ModuleSession_PushStream_GetAddrForAddr(lpszClientAddr, tszSMSAddr))
			{
				ModuleSession_PushStream_Destroy(tszSMSAddr);
			}
			if (st_ServiceConfig.bDebug)
			{
				if (NULL != pSt_VFile)
				{
					fclose(pSt_VFile);
					pSt_VFile = NULL;
				}
				if (NULL != pst_AFile)
				{
					fclose(pst_AFile);
					pst_AFile = NULL;
				}
			}
			pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REPDESTROY;
			ModuleProtocol_Packet_Comm(tszSDBuffer, &nSDLen, pSt_ProtocolHdr);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_CENTER);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("XEngine推流端：%s,接受请求了销毁流消息,销毁地址：%s,"), lpszClientAddr, tszSMSAddr);
		}
		else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REQPUSH == pSt_ProtocolHdr->unOperatorCode)
		{
			XNETHANDLE xhToken = 0;
			XENGINE_PROTOCOLDATA st_ProtocolAVInfo;

			memset(&st_ProtocolAVInfo, '\0', sizeof(XENGINE_PROTOCOLDATA));

			nSDLen = _xstprintf(tszSDBuffer, _X("%x\r\n"), nMsgLen - sizeof(XENGINE_PROTOCOLDATA));

			ModuleSession_PushStream_GetTokenForAddr(lpszClientAddr, &xhToken);
			if (0 == st_ProtocolAVInfo.byAVType)
			{
				FLVProtocol_Packet_FrameVideo(xhToken, tszRVBuffer, &nRVLen, st_ProtocolAVInfo.nTimeStamp, lpszMsgBuffer + sizeof(XENGINE_PROTOCOLDATA), nMsgLen - sizeof(XENGINE_PROTOCOLDATA));
				if (NULL != pSt_VFile)
				{
					fwrite(tszRVBuffer, 1, nRVLen, pSt_VFile);
				}
			}
			else
			{
				FLVProtocol_Packet_FrameAudio(xhToken, tszRVBuffer, &nRVLen, st_ProtocolAVInfo.nTimeStamp, lpszMsgBuffer + sizeof(XENGINE_PROTOCOLDATA), nMsgLen - sizeof(XENGINE_PROTOCOLDATA));
				if (NULL != pst_AFile)
				{
					fwrite(tszRVBuffer, 1, nRVLen, pst_AFile);
				}
			}
			memcpy(tszSDBuffer + nSDLen, tszRVBuffer, nRVLen);
			nSDLen += nRVLen;

			memcpy(tszSDBuffer + nSDLen, _X("\r\n"), 2);
			nSDLen += 2;
			//发送TAG
			list<xstring> stl_ListClient;
			ModuleSession_PushStream_ClientList(lpszClientAddr, &stl_ListClient);
			for (auto stl_ListIterator = stl_ListClient.begin(); stl_ListIterator != stl_ListClient.end(); stl_ListIterator++)
			{
				XEngine_Network_Send(stl_ListIterator->c_str(), tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("XEngine推流端：%s,接受推流数据"), lpszClientAddr);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("XEngine推流端：%s,接受返回了无法处理的推流协议类型"), lpszClientAddr, pSt_ProtocolHdr->unOperatorCode);
		}
	}
	else
	{
		//我们可以给客户端发送一条错误信息
		pSt_ProtocolHdr->wReserve = 0xFF;        //表示不支持的协议
		pSt_ProtocolHdr->unPacketSize = 0;       //设置没有后续数据包
		XEngine_Network_Send(lpszClientAddr, (LPCXSTR)pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR), ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_CENTER);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("XEngine推流端:%s,主协议错误,协议：%x 不支持"), lpszClientAddr, pSt_ProtocolHdr->unOperatorType);
	}
	return true;
}