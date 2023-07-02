#include "../XEngine_Hdr.h"
/********************************************************************
//    Created:     2023/07/02  18:00:14
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PushStream\PushStream_RTMPTask.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PushStream
//    File Base:   PushStream_RTMPTask
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     RTMP流处理协议
//    History:
*********************************************************************/
XHTHREAD CALLBACK PushStream_RTMPTask_Thread(XPVOID lParam)
{
	//任务池是编号1开始的.
	int nThreadPos = *(int*)lParam;
	nThreadPos++;
	while (bIsRun)
	{
		//等待编号1的任务池触发一个组完包的事件
		if (!RTMPProtocol_Parse_WaitEvent(nThreadPos))
		{
			continue;
		}
		int nListCount = 0;
		XENGINE_MANAGEPOOL_TASKEVENT** ppSst_ListAddr;
		//获得编号1的所有待处理任务的客户端列表(也就是客户端发送过来的数据已经组好了一个包,需要我们处理)
		RTMPProtocol_Parse_GetPool(nThreadPos, &ppSst_ListAddr, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			//再循环客户端拥有的任务个数
			for (int j = 0; j < ppSst_ListAddr[i]->nPktCount; j++)
			{
				int nMsgLen = 0;                             //客户端发送的数据大小,不包括头
				XCHAR* ptszMsgBuffer = NULL;                 //客户端发送的数据
				XENGINE_RTMPHDR st_RTMPHdr; 
				XENGINE_RTMPVIDEO st_RTMPVideo; 
				XENGINE_RTMPAUDIO st_RTMPAudio;
				//得到一个指定客户端的完整数据包
				if (RTMPProtocol_Parse_Recv(ppSst_ListAddr[i]->hSocket, &ptszMsgBuffer, &nMsgLen, &st_RTMPHdr, &st_RTMPVideo, &st_RTMPAudio))
				{
					//在另外一个函数里面处理数据
					PushStream_RTMPTask_Handle(&st_RTMPHdr, &st_RTMPVideo, &st_RTMPAudio, ppSst_ListAddr[i]->tszClientAddr, ptszMsgBuffer, nMsgLen);
					//释放内存
					BaseLib_OperatorMemory_FreeCStyle((VOID**)&ptszMsgBuffer);
				}
			}
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSst_ListAddr, nListCount);
	}
	return 0;
}
bool PushStream_RTMPTask_Handle(XENGINE_RTMPHDR* pSt_RTMPHdr, XENGINE_RTMPVIDEO* pSt_RTMPVideo, XENGINE_RTMPAUDIO* pSt_RTMPAudio, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nRVLen = 0;
	int nSDLen = 0;
	XCHAR* ptszRVBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);
	XCHAR* ptszSDBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);

	memset(ptszRVBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);
	memset(ptszSDBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);
	
	if (ENUM_XENGINE_STREAMMEDIA_RTMP_PARSE_PKTTYPE_CONNREQ == pSt_RTMPHdr->enPKTType)
	{
		XBYTE byVersion = 0;
		XENGINE_RTMPCONNECT st_RTMPClient;
		XENGINE_RTMPCONNECT st_RTMPServer;

		memset(&st_RTMPClient, '\0', sizeof(XENGINE_RTMPCONNECT));
		memset(&st_RTMPServer, '\0', sizeof(XENGINE_RTMPCONNECT));

		RTMPProtocol_Help_ParseConnect(&byVersion, NULL, &st_RTMPClient, lpszMsgBuffer, nMsgLen);
		RTMPProtocol_Help_PKTConnect(ptszSDBuffer, &nSDLen, &st_RTMPClient, byVersion, &st_RTMPServer);

		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTMP推流端：%s,请求连接成功,连接版本:%d,推流时间:%d,推流ID：%s"), lpszClientAddr, byVersion, st_RTMPClient.nTime, st_RTMPClient.tszRandomStr);
	}
	else if (ENUM_XENGINE_STREAMMEDIA_RTMP_PARSE_PKTTYPE_CONNACK == pSt_RTMPHdr->enPKTType)
	{
		XBYTE byVersion = 0;
		XENGINE_RTMPCONNECT st_RTMPClient;
		XENGINE_RTMPCONNECT st_RTMPServer;

		memset(&st_RTMPClient, '\0', sizeof(XENGINE_RTMPCONNECT));
		memset(&st_RTMPServer, '\0', sizeof(XENGINE_RTMPCONNECT));

		RTMPProtocol_Help_ParseConnect(&byVersion, &st_RTMPServer, &st_RTMPClient, lpszMsgBuffer, nMsgLen);
		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTMP推流端：%s,请求连接确认成功,连接版本:%d,推流时间:%d,推流ID：%s"), lpszClientAddr, byVersion, st_RTMPClient.nTime, st_RTMPClient.tszRandomStr);
	}
	else if (ENUM_XENGINE_STREAMMEDIA_RTMP_PARSE_PKTTYPE_CONTROL == pSt_RTMPHdr->enPKTType)
	{
		XENGINE_RTMPCONTROL st_RTMPControl;
		memset(&st_RTMPControl, '\0', sizeof(XENGINE_RTMPCONTROL));

		RTMPProtocol_Help_ParseControl(&st_RTMPControl, pSt_RTMPHdr->byTypeID, ptszRVBuffer, nRVLen);
		RTMPProtocol_Help_PKTControl(ptszSDBuffer, &nSDLen, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_CONTROL_WINDOWSIZE, 2500000);
		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);

		RTMPProtocol_Help_PKTControl(ptszSDBuffer, &nSDLen, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_CONTROL_BANDWIDTH, 2500000, 0x02);
		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);

		RTMPProtocol_Help_PKTControl(ptszSDBuffer, &nSDLen, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_CONTROL_SETCHUNK, 60000);
		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTMP推流端：%s,请求控制命令设置完成"), lpszClientAddr);
	}
	else if (ENUM_XENGINE_STREAMMEDIA_RTMP_PARSE_PKTTYPE_DATA == pSt_RTMPHdr->enPKTType)
	{
		XNETHANDLE xhToken = 0;
		XENGINE_RTMPDATA st_RTMPData;
		XENGINE_PROTOCOL_AVINFO st_AVInfo;

		memset(&st_RTMPData, '\0', sizeof(XENGINE_RTMPDATA));
		memset(&st_AVInfo, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));

		RTMPProtocol_Help_ParseData(&st_RTMPData, lpszMsgBuffer, nMsgLen);
		for (int i = 0; i < st_RTMPData.nCount; i++)
		{
			st_AVInfo.st_VideoInfo.bEnable = true;
			if (0 == _tcsxnicmp("duration", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, 8))
			{
				memcpy(&st_AVInfo.dlTime, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.nMLen);
			}
			else if (0 == _tcsxnicmp("fileSize", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, 8))
			{
				memcpy(&st_AVInfo.nSize, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.nMLen);
			}
			else if (0 == _tcsxnicmp("width", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, 8))
			{
				memcpy(&st_AVInfo.st_VideoInfo.nWidth, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.nMLen);
			}
			else if (0 == _tcsxnicmp("height", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, 8))
			{
				memcpy(&st_AVInfo.st_VideoInfo.nHeight, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.nMLen);
			}
			else if (0 == _tcsxnicmp("videocodecid", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, 8))
			{
				memcpy(&st_AVInfo.st_VideoInfo.enAVCodec, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.nMLen);
			}
			else if (0 == _tcsxnicmp("videodatarate", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, 8))
			{
				memcpy(&st_AVInfo.st_VideoInfo.nBitRate, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.nMLen);
			}
			else if (0 == _tcsxnicmp("videodatarate", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, 8))
			{
				memcpy(&st_AVInfo.st_VideoInfo.nBitRate, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.nMLen);
			}
			else if (0 == _tcsxnicmp("framerate", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, 8))
			{
				memcpy(&st_AVInfo.st_VideoInfo.nFrameRate, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.nMLen);
			}
			else if (0 == _tcsxnicmp("audiocodecid", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, 8))
			{
				memcpy(&st_AVInfo.st_AudioInfo.enAVCodec, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.nMLen);
			}
			else if (0 == _tcsxnicmp("audiodatarate", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, 8))
			{
				memcpy(&st_AVInfo.st_AudioInfo.nBitRate, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.nMLen);
			}
			else if (0 == _tcsxnicmp("audiosamplerate", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, 8))
			{
				memcpy(&st_AVInfo.st_AudioInfo.nSampleRate, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.nMLen);
			}
			else if (0 == _tcsxnicmp("audiosamplesize", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, 8))
			{
				memcpy(&st_AVInfo.st_AudioInfo.nFrameSize, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.nMLen);
			}
			else if (0 == _tcsxnicmp("audiochannels", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, 8))
			{
				memcpy(&st_AVInfo.st_AudioInfo.nChannel, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.nMLen);
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTMP推流端：%s,请求数据协议解析成功,数据列表个数:%d,索引:%d,名称:%s"), st_RTMPData.nCount, i, st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer);
		}
		int nPLen = 0;
		ModuleSession_PushStream_GetTokenForAddr(lpszClientAddr, &xhToken);
		FLVProtocol_Packet_FrameHdr(xhToken, ptszRVBuffer, &nPLen);
		nRVLen += nPLen;
		FLVProtocol_Packet_FrameScript(xhToken, ptszRVBuffer + nRVLen, &nPLen, &st_AVInfo);
		nRVLen += nPLen;

		if (st_AVInfo.st_VideoInfo.bEnable)
		{
			FLVProtocol_Packet_FrameAVCConfigure(xhToken, ptszRVBuffer + nRVLen, &nPLen, &st_AVInfo);
			nRVLen += nPLen;
		}
		if (st_AVInfo.st_AudioInfo.bEnable)
		{
			FLVProtocol_Packet_FrameAACConfigure(xhToken, ptszRVBuffer + nRVLen, &nPLen, &st_AVInfo);
			nRVLen += nPLen;
		}
		ModuleSession_PushStream_SetHDRBuffer(lpszClientAddr, ptszRVBuffer, nRVLen);
		
	}
	else if (ENUM_XENGINE_STREAMMEDIA_RTMP_PARSE_PKTTYPE_COMMAND == pSt_RTMPHdr->enPKTType)
	{
		XENGINE_RTMPCOMMAND st_RTMPCommand;
		memset(&st_RTMPCommand, '\0', sizeof(XENGINE_RTMPCOMMAND));

		RTMPProtocol_Help_ParseCommand(&st_RTMPCommand, lpszMsgBuffer, nMsgLen);
		for (int i = 0; i < st_RTMPCommand.nProCount; i++)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTMP推流端：%s,请求命令协议解析成功,属性个数:%d,索引:%d,命令:%s,Key:s,Value:%s"), st_RTMPCommand.nProCount, i, st_RTMPCommand.tszCMDName, st_RTMPCommand.ppSt_CMDProperty[i]->tszKeyBuffer, st_RTMPCommand.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer);
		}
		for (int i = 0; i < st_RTMPCommand.nObCount; i++)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTMP推流端：%s,请求命令协议解析成功,对象个数:%d,索引:%d,命令:%s,Key:s,Value:%s"), st_RTMPCommand.nProCount, i, st_RTMPCommand.tszCMDName, st_RTMPCommand.ppSt_CMDObject[i]->byType, st_RTMPCommand.ppSt_CMDObject[i]->tszMsgBuffer);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDProperty, st_RTMPCommand.nProCount);
		BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDObject, st_RTMPCommand.nObCount);

		st_RTMPCommand.nProCount = 0;
		st_RTMPCommand.nObCount = 0;
		if (0 == _tcsxnicmp(XENGINE_STREAMMEDIA_RTMP_MSGTYPE_COMMAND_PUBLISH, st_RTMPCommand.tszCMDName, strlen(XENGINE_STREAMMEDIA_RTMP_MSGTYPE_COMMAND_PUBLISH)))
		{
			//创建流
			XNETHANDLE xhToken = 0;
			XCHAR tszSMSAddr[MAX_PATH];
			memset(tszSMSAddr, '\0', MAX_PATH);

			_xstprintf(tszSMSAddr, _X("%s/%s"), st_RTMPCommand.ppSt_CMDObject[1]->tszMsgBuffer, st_RTMPCommand.ppSt_CMDObject[0]->tszMsgBuffer);
			FLVProtocol_Packet_Create(&xhToken, true, false);
			ModuleSession_PushStream_Create(lpszClientAddr, tszSMSAddr, xhToken);
			//准备返回数据
			st_RTMPCommand.nProCount = 4;
			BaseLib_OperatorMemory_Malloc((XPPPMEM)&st_RTMPCommand.ppSt_CMDProperty, st_RTMPCommand.nProCount, sizeof(XENGINE_RTMPCMDPROPERTY));

			strcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_COMMAND_ONSTATUS);

			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[0]->tszKeyBuffer, "level");
			st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_MSGTYPE_AFM0_STRING;
			st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.nMLen = 6;
			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.tszMsgBuffer, "status");

			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[1]->tszKeyBuffer, "code");
			st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_MSGTYPE_AFM0_STRING;
			st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.nMLen = 23;
			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.tszMsgBuffer, "NetStream.Publish.Start");

			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[2]->tszKeyBuffer, "description");
			st_RTMPCommand.ppSt_CMDProperty[2]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_MSGTYPE_AFM0_STRING;
			st_RTMPCommand.ppSt_CMDProperty[2]->st_CMDOBJect.nMLen = 26;
			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[2]->st_CMDOBJect.tszMsgBuffer, "Started publishing stream.");

			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[3]->tszKeyBuffer, "clientid");
			st_RTMPCommand.ppSt_CMDProperty[3]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_MSGTYPE_AFM0_STRING;
			st_RTMPCommand.ppSt_CMDProperty[3]->st_CMDOBJect.nMLen = 8;
			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[3]->st_CMDOBJect.tszMsgBuffer, "ASAICiss");

			RTMPProtocol_Help_PKTCommand(ptszSDBuffer, &nSDLen, 5, &st_RTMPCommand);
		}
		else
		{
			strcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_COMMAND_RESULT);
			RTMPProtocol_Help_PKTCommand(ptszSDBuffer, &nSDLen, pSt_RTMPHdr->nChunkType, &st_RTMPCommand);
		}
		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTMP推流端：%s,请求的命令协议解析处理完成"), lpszClientAddr);
	}
	else if (ENUM_XENGINE_STREAMMEDIA_RTMP_PARSE_PKTTYPE_AUDIO == pSt_RTMPHdr->enPKTType)
	{
		XENGINE_RTMPAUDIO st_RTMPAudio;
		memset(&st_RTMPAudio, '\0', sizeof(XENGINE_RTMPAUDIO));

		memcpy(&st_RTMPAudio, lpszMsgBuffer, sizeof(XENGINE_RTMPAUDIO));

		printf("ENUM_XENGINE_STREAMMEDIA_RTMP_PARSE_PKTTYPE_AUDIO:%d = %d %d %d %d\n", nMsgLen, st_RTMPAudio.byAudioFmt, st_RTMPAudio.byAudioRate, st_RTMPAudio.byAudioSize, st_RTMPAudio.byAudioType);
	}
	else if (ENUM_XENGINE_STREAMMEDIA_RTMP_PARSE_PKTTYPE_VIDEO == pSt_RTMPHdr->enPKTType)
	{
		XENGINE_RTMPVIDEO st_RTMPVideo;
		memset(&st_RTMPVideo, '\0', sizeof(XENGINE_RTMPVIDEO));

		memcpy(&st_RTMPVideo, lpszMsgBuffer, sizeof(XENGINE_RTMPVIDEO));

		printf("ENUM_XENGINE_STREAMMEDIA_RTMP_PARSE_PKTTYPE_VIDEO:%d = %d %d\n", nMsgLen, st_RTMPVideo.byCodecID, st_RTMPVideo.byFrameType);
	}
	
	free(ptszRVBuffer);
	free(ptszSDBuffer);
	ptszRVBuffer = NULL;
	ptszSDBuffer = NULL;
	return true;
}