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
				//得到一个指定客户端的完整数据包
				if (RTMPProtocol_Parse_Recv(ppSst_ListAddr[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_RTMPHdr))
				{
					//在另外一个函数里面处理数据
					PushStream_RTMPTask_Handle(&st_RTMPHdr, ppSst_ListAddr[i]->tszClientAddr, ptszMsgBuffer, nMsgLen);
					//释放内存
					BaseLib_OperatorMemory_FreeCStyle((VOID**)&ptszMsgBuffer);
				}
			}
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSst_ListAddr, nListCount);
	}
	return 0;
}
bool PushStream_RTMPTask_Handle(XENGINE_RTMPHDR* pSt_RTMPHdr, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nRVLen = 0;
	int nSDLen = 0;
	int nPMLen = 0;
	XCHAR* ptszRVBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);
	XCHAR* ptszSDBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);
	XCHAR* ptszMSGBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);
	
	if (XENGINE_STREAMMEDIA_RTMP_MSGTYPE_CONNREQ == pSt_RTMPHdr->byTypeID)
	{
		XBYTE byVersion = 0;
		XENGINE_RTMPCONNECT st_RTMPClient;
		XENGINE_RTMPCONNECT st_RTMPServer;

		memset(&st_RTMPClient, '\0', sizeof(XENGINE_RTMPCONNECT));
		memset(&st_RTMPServer, '\0', sizeof(XENGINE_RTMPCONNECT));

		RTMPProtocol_Help_ParseConnect(&byVersion, NULL, &st_RTMPClient, lpszMsgBuffer, nMsgLen);
		RTMPProtocol_Help_PKTConnect(ptszSDBuffer, &nSDLen, &st_RTMPClient, byVersion, &st_RTMPServer);
		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTMP推流端：%s,请求连接成功,连接版本:%d,推流时间:%d"), lpszClientAddr, byVersion, st_RTMPClient.nTime);
	}
	else if (XENGINE_STREAMMEDIA_RTMP_MSGTYPE_CONNACK == pSt_RTMPHdr->byTypeID)
	{
		XBYTE byVersion = 0;
		XENGINE_RTMPCONNECT st_RTMPClient;
		XENGINE_RTMPCONNECT st_RTMPServer;

		memset(&st_RTMPClient, '\0', sizeof(XENGINE_RTMPCONNECT));
		memset(&st_RTMPServer, '\0', sizeof(XENGINE_RTMPCONNECT));

		RTMPProtocol_Help_ParseConnect(&byVersion, &st_RTMPServer, &st_RTMPClient, lpszMsgBuffer, nMsgLen);
		//XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTMP推流端：%s,请求连接确认成功,推流时间:%d"), lpszClientAddr, st_RTMPServer.nTime);
	}
	else if (XENGINE_STREAMMEDIA_RTMP_MSGTYPE_SETCHUNK == pSt_RTMPHdr->byTypeID)
	{
		XENGINE_RTMPPROTOCOLCONTROL st_RTMPControl;
		memset(&st_RTMPControl, '\0', sizeof(XENGINE_RTMPPROTOCOLCONTROL));

		RTMPProtocol_Help_ParseProtocolControl(&st_RTMPControl, pSt_RTMPHdr->byTypeID, lpszMsgBuffer, nMsgLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTMP推流端：%s,请求控制命令设置完成"), lpszClientAddr);
	}
	else if (XENGINE_STREAMMEDIA_RTMP_MSGTYPE_CONTROL == pSt_RTMPHdr->byTypeID)
	{
		XENGINE_RTMPUSERCONTROL st_RTMPControl;
		memset(&st_RTMPControl, '\0', sizeof(XENGINE_RTMPUSERCONTROL));

		RTMPProtocol_Help_ParseUserControl(&st_RTMPControl, lpszMsgBuffer, nMsgLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTMP推流端：%s,请求控制命令设置完成"), lpszClientAddr);
	}
	else if (XENGINE_STREAMMEDIA_RTMP_MSGTYPE_WINDOWSIZE == pSt_RTMPHdr->byTypeID)
	{
		XENGINE_RTMPPROTOCOLCONTROL st_RTMPControl;
		memset(&st_RTMPControl, '\0', sizeof(XENGINE_RTMPPROTOCOLCONTROL));

		RTMPProtocol_Help_ParseProtocolControl(&st_RTMPControl, pSt_RTMPHdr->byTypeID, lpszMsgBuffer, nMsgLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTMP客户端：%s,请求控制命令滑动窗口大小:%d 成功"), lpszClientAddr, st_RTMPControl.nValue32);
	}
	else if (XENGINE_STREAMMEDIA_RTMP_MSGTYPE_DATA == pSt_RTMPHdr->byTypeID)
	{
		double dlValue = 0;
		XENGINE_RTMPDATA st_RTMPData;
		XENGINE_PROTOCOL_AVINFO st_AVInfo;

		memset(&st_RTMPData, '\0', sizeof(XENGINE_RTMPDATA));
		memset(&st_AVInfo, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));

		RTMPProtocol_Help_ParseData(&st_RTMPData, lpszMsgBuffer, nMsgLen);
		for (int i = 0; i < st_RTMPData.nCount; i++)
		{
			st_AVInfo.st_VideoInfo.bEnable = true;
			st_AVInfo.st_AudioInfo.bEnable = true;
			if (0 == _tcsxnicmp("duration", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, _tcsxlen(st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer)))
			{
				BaseLib_OperatorEndain_ToR64Hex(st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, (__int64u*)&dlValue, true);
				st_AVInfo.dlTime = dlValue;
			}
			else if (0 == _tcsxnicmp("fileSize", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, _tcsxlen(st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer)))
			{
				BaseLib_OperatorEndain_ToR64Hex(st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, (__int64u*)&dlValue, true);
				st_AVInfo.nSize = (__int64x)dlValue;
			}
			else if (0 == _tcsxnicmp("width", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, _tcsxlen(st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer)))
			{
				BaseLib_OperatorEndain_ToR64Hex(st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, (__int64u*)&dlValue, true);
				st_AVInfo.st_VideoInfo.nWidth = (int)dlValue;
			}
			else if (0 == _tcsxnicmp("height", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, _tcsxlen(st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer)))
			{
				BaseLib_OperatorEndain_ToR64Hex(st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, (__int64u*)&dlValue, true);
				st_AVInfo.st_VideoInfo.nHeight = (int)dlValue;
			}
			else if (0 == _tcsxnicmp("videocodecid", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, _tcsxlen(st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer)))
			{
				BaseLib_OperatorEndain_ToR64Hex(st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, (__int64u*)&dlValue, true);
				st_AVInfo.st_VideoInfo.enAVCodec = (int)dlValue;
			}
			else if (0 == _tcsxnicmp("videodatarate", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, _tcsxlen(st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer)))
			{
				BaseLib_OperatorEndain_ToR64Hex(st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, (__int64u*)&dlValue, true);
				st_AVInfo.st_VideoInfo.nBitRate = (__int64x)dlValue;
			}
			else if (0 == _tcsxnicmp("framerate", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, _tcsxlen(st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer)))
			{
				BaseLib_OperatorEndain_ToR64Hex(st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, (__int64u*)&dlValue, true);
				st_AVInfo.st_VideoInfo.nFrameRate = (int)dlValue;
			}
			else if (0 == _tcsxnicmp("audiocodecid", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, _tcsxlen(st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer)))
			{
				BaseLib_OperatorEndain_ToR64Hex(st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, (__int64u*)&dlValue, true);
				st_AVInfo.st_AudioInfo.enAVCodec = (int)dlValue;
			}
			else if (0 == _tcsxnicmp("audiodatarate", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, _tcsxlen(st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer)))
			{
				BaseLib_OperatorEndain_ToR64Hex(st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, (__int64u*)&dlValue, true);
				st_AVInfo.st_AudioInfo.nBitRate = (__int64x)dlValue;
			}
			else if (0 == _tcsxnicmp("audiosamplerate", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, _tcsxlen(st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer)))
			{
				BaseLib_OperatorEndain_ToR64Hex(st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, (__int64u*)&dlValue, true);
				st_AVInfo.st_AudioInfo.nSampleRate = (int)dlValue;
			}
			else if (0 == _tcsxnicmp("audiosamplesize", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, _tcsxlen(st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer)))
			{
				BaseLib_OperatorEndain_ToR64Hex(st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, (__int64u*)&dlValue, true);
				st_AVInfo.st_AudioInfo.nSampleFmt = (int)dlValue;
			}
			else if (0 == _tcsxnicmp("audiochannels", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, _tcsxlen(st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer)))
			{
				BaseLib_OperatorEndain_ToR64Hex(st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, (__int64u*)&dlValue, true);
				st_AVInfo.st_AudioInfo.nChannel = (int)dlValue;
			}
			//XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTMP推流端：%s,请求数据协议解析成功,数据列表个数:%d,索引:%d,名称:%s"), lpszClientAddr, st_RTMPData.nCount, i, st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer);
		}
		_tcsxcpy(st_AVInfo.tszPktName, "Lavf59.27.100");
		XEngine_AVPacket_AVSetTime(lpszClientAddr, st_AVInfo.st_VideoInfo.nFrameRate, st_AVInfo.st_AudioInfo.nSampleRate);
		ModuleSession_PushStream_SetAVInfo(lpszClientAddr, &st_AVInfo);
		BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPData.ppSt_CMDProperty, st_RTMPData.nCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTMP推流端：%s,请求数据协议解析成功,数据列表个数:%d"), lpszClientAddr, st_RTMPData.nCount);
	}
	else if (XENGINE_STREAMMEDIA_RTMP_MSGTYPE_COMMAND == pSt_RTMPHdr->byTypeID)
	{
		XCHAR tszCMDBuffer[MAX_PATH];
		XENGINE_RTMPCOMMAND st_RTMPCommand;

		memset(tszCMDBuffer, '\0', MAX_PATH);
		memset(&st_RTMPCommand, '\0', sizeof(XENGINE_RTMPCOMMAND));

		RTMPProtocol_Help_ParseCommand(&st_RTMPCommand, lpszMsgBuffer, nMsgLen);
		_tcsxcpy(tszCMDBuffer, st_RTMPCommand.tszCMDName);
		/*
		for (int i = 0; i < st_RTMPCommand.nProCount; i++)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTMP推流端：%s,请求命令协议解析成功,属性个数:%d,索引:%d,命令:%s,Key:s,Value:%s"), lpszClientAddr, st_RTMPCommand.nProCount, i, st_RTMPCommand.tszCMDName, st_RTMPCommand.ppSt_CMDProperty[i]->tszKeyBuffer, st_RTMPCommand.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer);
		}
		for (int i = 0; i < st_RTMPCommand.nObCount; i++)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTMP推流端：%s,请求命令协议解析成功,对象个数:%d,索引:%d,命令:%s,Value:%s"), lpszClientAddr, st_RTMPCommand.nProCount, i, st_RTMPCommand.tszCMDName, st_RTMPCommand.ppSt_CMDObject[i]->tszMsgBuffer);
		}*/
		if (0 == _tcsxnicmp(XENGINE_STREAMMEDIA_RTMP_COMMAND_PUBLISH, st_RTMPCommand.tszCMDName, strlen(XENGINE_STREAMMEDIA_RTMP_COMMAND_PUBLISH)))
		{
			XCHAR tszSMSAddr[2048];
			memset(tszSMSAddr, '\0', sizeof(tszSMSAddr));
			_xstprintf(tszSMSAddr, _X("%s/%s"), st_RTMPCommand.ppSt_CMDObject[1]->tszMsgBuffer, st_RTMPCommand.ppSt_CMDObject[0]->tszMsgBuffer);

			BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDProperty, st_RTMPCommand.nProCount);
			BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDObject, st_RTMPCommand.nObCount);
			st_RTMPCommand.nObCount = 0;
			st_RTMPCommand.nProCount = 2;
			BaseLib_OperatorMemory_Malloc((XPPPMEM)&st_RTMPCommand.ppSt_CMDProperty, st_RTMPCommand.nProCount, sizeof(XENGINE_RTMPCMDPROPERTY));

			st_RTMPCommand.nCMDId = 0;
			strcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_COMMAND_ONPUBLISH);

			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[0]->tszKeyBuffer, "code");
			st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_STRING;
			st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.nMLen = 23;
			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.tszMsgBuffer, "NetStream.Publish.Start");

			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[1]->tszKeyBuffer, "description");
			st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_STRING;
			st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.nMLen = 26;
			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.tszMsgBuffer, "Started publishing stream.");

			RTMPProtocol_Help_PKTCommand(ptszSDBuffer, &nSDLen, (XBYTE)pSt_RTMPHdr->nChunkType, &st_RTMPCommand, true, false);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
			//////////////////////////////////////////////////////////////////////////
			//创建流
			XEngine_AVPacket_AVCreate(lpszClientAddr);
			ModuleSession_PushStream_Create(lpszClientAddr, tszSMSAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
			//释放内存
			BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDProperty, st_RTMPCommand.nProCount);
			BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDObject, st_RTMPCommand.nObCount);
			memset(st_RTMPCommand.tszCMDName, '\0', sizeof(st_RTMPCommand.tszCMDName));
			st_RTMPCommand.nProCount = 0;
			st_RTMPCommand.nObCount = 0;
			//准备返回数据
			st_RTMPCommand.nProCount = 4;
			BaseLib_OperatorMemory_Malloc((XPPPMEM)&st_RTMPCommand.ppSt_CMDProperty, st_RTMPCommand.nProCount, sizeof(XENGINE_RTMPCMDPROPERTY));

			strcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_COMMAND_ONSTATUS);

			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[0]->tszKeyBuffer, "level");
			st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_STRING;
			st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.nMLen = 6;
			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.tszMsgBuffer, "status");

			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[1]->tszKeyBuffer, "code");
			st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_STRING;
			st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.nMLen = 23;
			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.tszMsgBuffer, "NetStream.Publish.Start");

			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[2]->tszKeyBuffer, "description");
			st_RTMPCommand.ppSt_CMDProperty[2]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_STRING;
			st_RTMPCommand.ppSt_CMDProperty[2]->st_CMDOBJect.nMLen = 26;
			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[2]->st_CMDOBJect.tszMsgBuffer, "Started publishing stream.");

			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[3]->tszKeyBuffer, "clientid");
			st_RTMPCommand.ppSt_CMDProperty[3]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_STRING;
			st_RTMPCommand.ppSt_CMDProperty[3]->st_CMDOBJect.nMLen = 8;
			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[3]->st_CMDOBJect.tszMsgBuffer, "ASAICiss");

			RTMPProtocol_Help_PKTCommand(ptszSDBuffer, &nSDLen, 5, &st_RTMPCommand);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
		}
		else if (0 == _tcsxnicmp(XENGINE_STREAMMEDIA_RTMP_COMMAND_CREATE, st_RTMPCommand.tszCMDName, strlen(XENGINE_STREAMMEDIA_RTMP_COMMAND_CREATE)))
		{
			BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDProperty, st_RTMPCommand.nProCount);
			BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDObject, st_RTMPCommand.nObCount);

			st_RTMPCommand.nObCount = 1;
			BaseLib_OperatorMemory_Malloc((XPPPMEM)&st_RTMPCommand.ppSt_CMDObject, st_RTMPCommand.nObCount, sizeof(XENGINE_RTMPCMDOBJECT));

			strcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_COMMAND_RESULT);

			double dlValue = 1;
			st_RTMPCommand.ppSt_CMDObject[0]->byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_INT64;
			st_RTMPCommand.ppSt_CMDObject[0]->nMLen = 8;
			memcpy(st_RTMPCommand.ppSt_CMDObject[0]->tszMsgBuffer, &dlValue, 8);

			RTMPProtocol_Help_PKTCommand(ptszSDBuffer, &nSDLen, (XBYTE)pSt_RTMPHdr->nChunkType, &st_RTMPCommand, true, false);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
		}
		else if (0 == _tcsxnicmp(XENGINE_STREAMMEDIA_RTMP_COMMAND_UNPUBLISH, st_RTMPCommand.tszCMDName, strlen(XENGINE_STREAMMEDIA_RTMP_COMMAND_UNPUBLISH)))
		{
			BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDProperty, st_RTMPCommand.nProCount);
			BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDObject, st_RTMPCommand.nObCount);
			st_RTMPCommand.nProCount = 0;
			st_RTMPCommand.nObCount = 0;
			RTMPProtocol_Help_PKTCommand(ptszSDBuffer, &nSDLen, 5, &st_RTMPCommand);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
		}
		else if (0 == _tcsxnicmp(XENGINE_STREAMMEDIA_RTMP_COMMAND_CONNECT, st_RTMPCommand.tszCMDName, strlen(XENGINE_STREAMMEDIA_RTMP_COMMAND_CONNECT)))
		{
			BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDProperty, st_RTMPCommand.nProCount);
			BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDObject, st_RTMPCommand.nObCount);
			st_RTMPCommand.nProCount = 0;
			st_RTMPCommand.nObCount = 0;
			memset(st_RTMPCommand.tszCMDName, '\0', sizeof(st_RTMPCommand.tszCMDName));
			//RTMP拉流端
			RTMPProtocol_Help_PKTProtocolControl(ptszSDBuffer, &nSDLen, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_WINDOWSIZE, 2500000);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);

			RTMPProtocol_Help_PKTProtocolControl(ptszSDBuffer, &nSDLen, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_BANDWIDTH, 2500000, 0x02);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);

			RTMPProtocol_Help_PKTProtocolControl(ptszSDBuffer, &nSDLen, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_SETCHUNK, 16777215);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);

			strcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_COMMAND_RESULT);
			RTMPProtocol_Help_PKTCommand(ptszSDBuffer, &nSDLen, 3, &st_RTMPCommand);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
		}
		else if (0 == _tcsxnicmp(XENGINE_STREAMMEDIA_RTMP_COMMAND_PLAY, st_RTMPCommand.tszCMDName, strlen(XENGINE_STREAMMEDIA_RTMP_COMMAND_PLAY)))
		{
			XCHAR tszSMSAddr[2048];
			XCHAR tszPushAddr[MAX_PATH];

			memset(tszSMSAddr, '\0', sizeof(tszSMSAddr));
			memset(tszPushAddr, '\0', sizeof(tszPushAddr));

			_xstprintf(tszSMSAddr, _X("live/%s"), st_RTMPCommand.ppSt_CMDObject[0]->tszMsgBuffer);

			BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDProperty, st_RTMPCommand.nProCount);
			BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDObject, st_RTMPCommand.nObCount);
			memset(st_RTMPCommand.tszCMDName, '\0', sizeof(st_RTMPCommand.tszCMDName));
			st_RTMPCommand.nProCount = 0;
			st_RTMPCommand.nObCount = 0;
			//RTMP拉流端
			XENGINE_RTMPUSERCONTROL st_RTMPControl;
			memset(&st_RTMPControl, '\0', sizeof(XENGINE_RTMPUSERCONTROL));

			st_RTMPControl.nControlID = XENGINE_STREAMMEDIA_RTMP_CONTROL_STREAMBEGIN;
			st_RTMPControl.nValue32 = 1;
			//流开始
			RTMPProtocol_Help_PKTUserControl(ptszSDBuffer, &nSDLen, &st_RTMPControl);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
			//状态,有没有这个流
			st_RTMPCommand.nProCount = 3;
			BaseLib_OperatorMemory_Malloc((XPPPMEM)&st_RTMPCommand.ppSt_CMDProperty, st_RTMPCommand.nProCount, sizeof(XENGINE_RTMPCMDPROPERTY));

			strcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_COMMAND_ONSTATUS);

			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[0]->tszKeyBuffer, "level");
			st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_STRING;
			st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.nMLen = 6;
			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[0]->st_CMDOBJect.tszMsgBuffer, "status");

			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[1]->tszKeyBuffer, "code");
			st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_STRING;
			st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.nMLen = 23;
			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[1]->st_CMDOBJect.tszMsgBuffer, "NetStream.Publish.Start");

			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[2]->tszKeyBuffer, "description");
			st_RTMPCommand.ppSt_CMDProperty[2]->st_CMDOBJect.byType = XENGINE_STREAMMEDIA_RTMP_PLTYPE_AFM0_STRING;
			st_RTMPCommand.ppSt_CMDProperty[2]->st_CMDOBJect.nMLen = 10;
			_tcsxcpy(st_RTMPCommand.ppSt_CMDProperty[2]->st_CMDOBJect.tszMsgBuffer, "Start live");
			RTMPProtocol_Help_PKTCommand(ptszSDBuffer, &nSDLen, 5, &st_RTMPCommand);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
			//发送RTMP包头
			if (!ModuleSession_PushStream_FindStream(tszSMSAddr, tszPushAddr))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("拉流端:%s,请求拉流的参数不正确:%s,错误:%lX"), lpszClientAddr, tszSMSAddr, ModuleSession_GetLastError());
				return false;
			}
			//配置头
			int nHLen = 0;
			int nPLen = 0;
			XENGINE_PROTOCOL_AVINFO st_AVInfo;
			memset(&st_AVInfo, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));

			ModuleSession_PushStream_GetAVInfo(tszPushAddr, &st_AVInfo);

			RTMPProtocol_Packet_FrameAVScript(ptszSDBuffer + nHLen, &nPLen, &st_AVInfo);
			nHLen += nPLen;
			if (st_AVInfo.st_VideoInfo.bEnable)
			{
				RTMPProtocol_Packet_FrameAVCConfigure(ptszSDBuffer + nHLen, &nPLen, &st_AVInfo);
				nHLen += nPLen;
			}
			if (st_AVInfo.st_AudioInfo.bEnable)
			{
				RTMPProtocol_Packet_FrameAACConfigure(ptszSDBuffer + nHLen, &nPLen, &st_AVInfo);
				nHLen += nPLen;
			}
			//返回数据,
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nHLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);

			ModuleSession_PullStream_Insert(lpszClientAddr, tszSMSAddr, tszPushAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_RTMP);
			ModuleSession_PushStream_ClientInsert(tszPushAddr, lpszClientAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_RTMP);
		}
		else
		{
			BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDProperty, st_RTMPCommand.nProCount);
			BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDObject, st_RTMPCommand.nObCount);
			memset(st_RTMPCommand.tszCMDName, '\0', sizeof(st_RTMPCommand.tszCMDName));
			st_RTMPCommand.nProCount = 0;
			st_RTMPCommand.nObCount = 0;

			strcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_COMMAND_RESULT);
			RTMPProtocol_Help_PKTCommand(ptszSDBuffer, &nSDLen, (XBYTE)pSt_RTMPHdr->nChunkType, &st_RTMPCommand);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTMP推流端：%s,请求的命令:%s 协议解析处理完成"), lpszClientAddr, tszCMDBuffer);
	}
	else if (XENGINE_STREAMMEDIA_RTMP_MSGTYPE_AUDIO == pSt_RTMPHdr->byTypeID)
	{
		XENGINE_RTMPAUDIO st_RTMPAudio;
		memset(&st_RTMPAudio, '\0', sizeof(XENGINE_RTMPAUDIO));

		memcpy(&st_RTMPAudio, lpszMsgBuffer, sizeof(XENGINE_RTMPAUDIO));
		if (0 == st_RTMPAudio.byPKTType)
		{
			XEngine_AVPacket_AVHdr(lpszClientAddr, lpszMsgBuffer, nMsgLen, 1, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
		}
		RTMPProtocol_Help_ParseAudio(&st_RTMPAudio, ptszMSGBuffer, &nPMLen, lpszMsgBuffer + sizeof(XENGINE_RTMPAUDIO), nMsgLen - sizeof(XENGINE_RTMPAUDIO));
		XEngine_AVPacket_AVFrame(ptszSDBuffer, &nSDLen, ptszRVBuffer, &nRVLen, lpszClientAddr, ptszMSGBuffer, nPMLen, 1);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("RTMP推流端：%s,接受推流数据"), lpszClientAddr);
	}
	else if (XENGINE_STREAMMEDIA_RTMP_MSGTYPE_VIDEO == pSt_RTMPHdr->byTypeID)
	{
		XENGINE_RTMPVIDEO st_RTMPVideo;
		memset(&st_RTMPVideo, '\0', sizeof(XENGINE_RTMPVIDEO));

		memcpy(&st_RTMPVideo, lpszMsgBuffer, sizeof(XENGINE_RTMPVIDEO));
		if (0 == st_RTMPVideo.byAVCType)
		{
			XEngine_AVPacket_AVHdr(lpszClientAddr, lpszMsgBuffer, nMsgLen, 0, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
		}
		RTMPProtocol_Help_ParseVideo(&st_RTMPVideo, ptszMSGBuffer, &nPMLen, lpszMsgBuffer + sizeof(XENGINE_RTMPVIDEO), nMsgLen - sizeof(XENGINE_RTMPVIDEO));
		XEngine_AVPacket_AVFrame(ptszSDBuffer, &nSDLen, ptszRVBuffer, &nRVLen, lpszClientAddr, ptszMSGBuffer, nPMLen, 0);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("RTMP推流端：%s,接受推流数据,数据大小:%d"), lpszClientAddr, nMsgLen);
	}
	
	free(ptszRVBuffer);
	free(ptszSDBuffer);
	free(ptszMSGBuffer);
	ptszRVBuffer = NULL;
	ptszSDBuffer = NULL;
	ptszMSGBuffer = NULL;
	return true;
}