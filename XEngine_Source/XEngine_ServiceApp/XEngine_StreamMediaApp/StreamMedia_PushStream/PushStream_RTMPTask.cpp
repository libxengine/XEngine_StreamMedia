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
	XCHAR* ptszRVBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);
	XCHAR* ptszSDBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);
	
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
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTMP推流端：%s,请求连接成功,连接版本:%d,推流时间:%d,推流ID：%s"), lpszClientAddr, byVersion, st_RTMPClient.nTime, st_RTMPClient.tszRandomStr);
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
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTMP推流端：%s,请求连接确认成功,连接版本:%d,推流时间:%d,推流ID：%s"), lpszClientAddr, byVersion, st_RTMPClient.nTime, st_RTMPClient.tszRandomStr);
	}
	else if (XENGINE_STREAMMEDIA_RTMP_MSGTYPE_SETCHUNK == pSt_RTMPHdr->byTypeID)
	{
		XENGINE_RTMPCONTROL st_RTMPControl;
		memset(&st_RTMPControl, '\0', sizeof(XENGINE_RTMPCONTROL));

		RTMPProtocol_Help_ParseControl(&st_RTMPControl, pSt_RTMPHdr->byTypeID, ptszRVBuffer, nRVLen);
		RTMPProtocol_Help_PKTControl(ptszSDBuffer, &nSDLen, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_WINDOWSIZE, 2500000);
		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);

		RTMPProtocol_Help_PKTControl(ptszSDBuffer, &nSDLen, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_BANDWIDTH, 2500000, 0x02);
		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);

		RTMPProtocol_Help_PKTControl(ptszSDBuffer, &nSDLen, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_SETCHUNK, 60000);
		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTMP推流端：%s,请求控制命令设置完成"), lpszClientAddr);
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
				APIHelp_NetWork_ToR64Hex(st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, (__int64u*)&dlValue, true);
				st_AVInfo.dlTime = dlValue;
			}
			else if (0 == _tcsxnicmp("fileSize", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, _tcsxlen(st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer)))
			{
				APIHelp_NetWork_ToR64Hex(st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, (__int64u*)&dlValue, true);
				st_AVInfo.nSize = (__int64x)dlValue;
			}
			else if (0 == _tcsxnicmp("width", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, _tcsxlen(st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer)))
			{
				APIHelp_NetWork_ToR64Hex(st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, (__int64u*)&dlValue, true);
				st_AVInfo.st_VideoInfo.nWidth = (int)dlValue;
			}
			else if (0 == _tcsxnicmp("height", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, _tcsxlen(st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer)))
			{
				APIHelp_NetWork_ToR64Hex(st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, (__int64u*)&dlValue, true);
				st_AVInfo.st_VideoInfo.nHeight = (int)dlValue;
			}
			else if (0 == _tcsxnicmp("videocodecid", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, _tcsxlen(st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer)))
			{
				APIHelp_NetWork_ToR64Hex(st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, (__int64u*)&dlValue, true);
				st_AVInfo.st_VideoInfo.enAVCodec = (int)dlValue;
			}
			else if (0 == _tcsxnicmp("videodatarate", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, _tcsxlen(st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer)))
			{
				APIHelp_NetWork_ToR64Hex(st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, (__int64u*)&dlValue, true);
				st_AVInfo.st_VideoInfo.nBitRate = (__int64x)dlValue;
			}
			else if (0 == _tcsxnicmp("framerate", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, _tcsxlen(st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer)))
			{
				APIHelp_NetWork_ToR64Hex(st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, (__int64u*)&dlValue, true);
				st_AVInfo.st_VideoInfo.nFrameRate = (int)dlValue;
			}
			else if (0 == _tcsxnicmp("audiocodecid", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, _tcsxlen(st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer)))
			{
				APIHelp_NetWork_ToR64Hex(st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, (__int64u*)&dlValue, true);
				st_AVInfo.st_AudioInfo.enAVCodec = (int)dlValue;
			}
			else if (0 == _tcsxnicmp("audiodatarate", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, _tcsxlen(st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer)))
			{
				APIHelp_NetWork_ToR64Hex(st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, (__int64u*)&dlValue, true);
				st_AVInfo.st_AudioInfo.nBitRate = (__int64x)dlValue;
			}
			else if (0 == _tcsxnicmp("audiosamplerate", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, _tcsxlen(st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer)))
			{
				APIHelp_NetWork_ToR64Hex(st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, (__int64u*)&dlValue, true);
				st_AVInfo.st_AudioInfo.nSampleRate = (int)dlValue;
			}
			else if (0 == _tcsxnicmp("audiosamplesize", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, _tcsxlen(st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer)))
			{
				APIHelp_NetWork_ToR64Hex(st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, (__int64u*)&dlValue, true);
				st_AVInfo.st_AudioInfo.nFrameSize = (int)dlValue;
			}
			else if (0 == _tcsxnicmp("audiochannels", st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer, _tcsxlen(st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer)))
			{
				APIHelp_NetWork_ToR64Hex(st_RTMPData.ppSt_CMDProperty[i]->st_CMDOBJect.tszMsgBuffer, (__int64u*)&dlValue, true);
				st_AVInfo.st_AudioInfo.nChannel = (int)dlValue;
			}
			//XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTMP推流端：%s,请求数据协议解析成功,数据列表个数:%d,索引:%d,名称:%s"), lpszClientAddr, st_RTMPData.nCount, i, st_RTMPData.ppSt_CMDProperty[i]->tszKeyBuffer);
		}
		_tcsxcpy(st_AVInfo.tszPktName, "Lavf59.27.100");
		FLVProtocol_Packet_SetTime(lpszClientAddr, st_AVInfo.st_VideoInfo.nFrameRate, st_AVInfo.st_AudioInfo.nSampleRate);
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
		st_RTMPCommand.nProCount = 0;
		st_RTMPCommand.nObCount = 0;
		if (0 == _tcsxnicmp(XENGINE_STREAMMEDIA_RTMP_COMMAND_PUBLISH, st_RTMPCommand.tszCMDName, strlen(XENGINE_STREAMMEDIA_RTMP_COMMAND_PUBLISH)))
		{
			//创建流
			XCHAR tszSMSAddr[2048];
			memset(tszSMSAddr, '\0', sizeof(tszSMSAddr));

			_xstprintf(tszSMSAddr, _X("%s/%s"), st_RTMPCommand.ppSt_CMDObject[1]->tszMsgBuffer, st_RTMPCommand.ppSt_CMDObject[0]->tszMsgBuffer);
			FLVProtocol_Packet_Insert(lpszClientAddr, true, true);
			ModuleSession_PushStream_Create(lpszClientAddr, tszSMSAddr);
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
		}
		else if (0 == _tcsxnicmp(XENGINE_STREAMMEDIA_RTMP_COMMAND_UNPUBLISH, st_RTMPCommand.tszCMDName, strlen(XENGINE_STREAMMEDIA_RTMP_COMMAND_UNPUBLISH)))
		{
			RTMPProtocol_Parse_Delete(lpszClientAddr);
			FLVProtocol_Packet_Delete(lpszClientAddr);
			ModuleSession_PushStream_Destroy(lpszClientAddr);

			RTMPProtocol_Help_PKTCommand(ptszSDBuffer, &nSDLen, 5, &st_RTMPCommand);
		}
		else if (0 == _tcsxnicmp(XENGINE_STREAMMEDIA_RTMP_COMMAND_CONNECT, st_RTMPCommand.tszCMDName, strlen(XENGINE_STREAMMEDIA_RTMP_COMMAND_CONNECT)))
		{
			//RTMP拉流端
			RTMPProtocol_Help_PKTControl(ptszSDBuffer, &nSDLen, XENGINE_STREAMMEDIA_RTMP_MSGTYPE_WINDOWSIZE, 2500000);
			XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
		}
		else
		{
			strcpy(st_RTMPCommand.tszCMDName, XENGINE_STREAMMEDIA_RTMP_COMMAND_RESULT);
			RTMPProtocol_Help_PKTCommand(ptszSDBuffer, &nSDLen, pSt_RTMPHdr->nChunkType, &st_RTMPCommand);
		}
		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
		BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDProperty, st_RTMPCommand.nProCount);
		BaseLib_OperatorMemory_Free((XPPPMEM)&st_RTMPCommand.ppSt_CMDObject, st_RTMPCommand.nObCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTMP推流端：%s,请求的命令:%s 协议解析处理完成"), lpszClientAddr, tszCMDBuffer);
	}
	else if (XENGINE_STREAMMEDIA_RTMP_MSGTYPE_AUDIO == pSt_RTMPHdr->byTypeID)
	{
		int nHLen = 0;
		XENGINE_RTMPAUDIO st_RTMPAudio;
		memset(&st_RTMPAudio, '\0', sizeof(XENGINE_RTMPAUDIO));

		RTMPProtocol_Help_ParseAudio(&st_RTMPAudio, ptszRVBuffer, &nRVLen, lpszMsgBuffer, nMsgLen);
		ModuleSession_PushStream_GetHDRBuffer(lpszClientAddr, NULL, &nHLen);
		if (0 == nHLen && 0 == st_RTMPAudio.byPKTType)
		{
			XENGINE_PROTOCOL_AVINFO st_AVInfo;
			ModuleSession_PushStream_GetAVInfo(lpszClientAddr, &st_AVInfo);
			//音频参数信息是否存在
			if (0 == st_AVInfo.st_AudioInfo.nALen)
			{
				st_AVInfo.st_AudioInfo.nALen = nMsgLen;
				memcpy(st_AVInfo.st_AudioInfo.tszAInfo, lpszMsgBuffer, nMsgLen);
				ModuleSession_PushStream_SetAVInfo(lpszClientAddr, &st_AVInfo);
			}
			//是否全部设置完毕
			if (st_AVInfo.st_AudioInfo.nALen > 0 && st_AVInfo.st_VideoInfo.nVLen > 0)
			{
				//配置头
				int nPLen = 0;
				XCHAR tszMsgBuffer[1024];

				memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

				FLVProtocol_Packet_FrameHdr(lpszClientAddr, tszMsgBuffer, &nPLen);
				nHLen += nPLen;
				FLVProtocol_Packet_FrameScript(lpszClientAddr, tszMsgBuffer + nHLen, &nPLen, &st_AVInfo);
				nHLen += nPLen;
				FLVProtocol_Packet_FrameCustom(lpszClientAddr, tszMsgBuffer + nHLen, &nPLen, st_AVInfo.st_VideoInfo.tszVInfo, st_AVInfo.st_VideoInfo.nVLen, 0, 9);
				nHLen += nPLen;
				FLVProtocol_Packet_FrameCustom(lpszClientAddr, tszMsgBuffer + nHLen, &nPLen, st_AVInfo.st_AudioInfo.tszAInfo, st_AVInfo.st_AudioInfo.nALen, 0, 8);
				nHLen += nPLen;

				ModuleSession_PushStream_SetHDRBuffer(lpszClientAddr, tszMsgBuffer, nHLen);
			}
		}
		FLVProtocol_Packet_FrameCustom(lpszClientAddr, ptszRVBuffer, &nRVLen, lpszMsgBuffer, nMsgLen, -1, 8);
		nSDLen = _xstprintf(ptszSDBuffer, _X("%x\r\n"), nRVLen);
		memcpy(ptszSDBuffer + nSDLen, ptszRVBuffer, nRVLen);
		nSDLen += nRVLen;

		memcpy(ptszSDBuffer + nSDLen, _X("\r\n"), 2);
		nSDLen += 2;
		//发送TAG
		list<xstring> stl_ListClient;
		ModuleSession_PushStream_ClientList(lpszClientAddr, &stl_ListClient);
		for (auto stl_ListIterator = stl_ListClient.begin(); stl_ListIterator != stl_ListClient.end(); stl_ListIterator++)
		{
			XEngine_Network_Send(stl_ListIterator->c_str(), ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("RTMP推流端：%s,接受推流数据"), lpszClientAddr);
	}
	else if (XENGINE_STREAMMEDIA_RTMP_MSGTYPE_VIDEO == pSt_RTMPHdr->byTypeID)
	{
		int nHLen = 0;
		XENGINE_RTMPVIDEO st_RTMPVideo;
		memset(&st_RTMPVideo, '\0', sizeof(XENGINE_RTMPVIDEO));

		RTMPProtocol_Help_ParseVideo(&st_RTMPVideo, ptszRVBuffer, &nRVLen, lpszMsgBuffer, nMsgLen);
		ModuleSession_PushStream_GetHDRBuffer(lpszClientAddr, NULL, &nHLen);
		if (0 == nHLen && 0 == st_RTMPVideo.byAVCType)
		{
			XENGINE_PROTOCOL_AVINFO st_AVInfo;
			ModuleSession_PushStream_GetAVInfo(lpszClientAddr, &st_AVInfo);

			//视频参数信息是否存在
			if (0 == st_AVInfo.st_VideoInfo.nVLen)
			{
				st_AVInfo.st_VideoInfo.nVLen = nMsgLen;
				memcpy(st_AVInfo.st_VideoInfo.tszVInfo, lpszMsgBuffer, nMsgLen);
				ModuleSession_PushStream_SetAVInfo(lpszClientAddr, &st_AVInfo);
			}
			//是否全部设置完毕
			if (st_AVInfo.st_AudioInfo.nALen > 0 && st_AVInfo.st_VideoInfo.nVLen > 0)
			{
				//配置头
				int nPLen = 0;
				XCHAR tszMsgBuffer[1024];
				memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

				FLVProtocol_Packet_FrameHdr(lpszClientAddr, tszMsgBuffer, &nPLen);
				nHLen += nPLen;
				FLVProtocol_Packet_FrameScript(lpszClientAddr, tszMsgBuffer + nHLen, &nPLen, &st_AVInfo);
				nHLen += nPLen;
				FLVProtocol_Packet_FrameCustom(lpszClientAddr, tszMsgBuffer + nHLen, &nPLen, st_AVInfo.st_VideoInfo.tszVInfo, st_AVInfo.st_VideoInfo.nVLen, 0, 9);
				nHLen += nPLen;
				FLVProtocol_Packet_FrameCustom(lpszClientAddr, tszMsgBuffer + nHLen, &nPLen, st_AVInfo.st_AudioInfo.tszAInfo, st_AVInfo.st_AudioInfo.nALen, 0, 8);
				nHLen += nPLen;

				ModuleSession_PushStream_SetHDRBuffer(lpszClientAddr, tszMsgBuffer, nHLen);
			}
		}
		FLVProtocol_Packet_FrameCustom(lpszClientAddr, ptszRVBuffer, &nRVLen, lpszMsgBuffer, nMsgLen, -1, 9);
		nSDLen = _xstprintf(ptszSDBuffer, _X("%x\r\n"), nRVLen);
		memcpy(ptszSDBuffer + nSDLen, ptszRVBuffer, nRVLen);
		nSDLen += nRVLen;

		memcpy(ptszSDBuffer + nSDLen, _X("\r\n"), 2);
		nSDLen += 2;
		//发送TAG
		list<xstring> stl_ListClient;
		ModuleSession_PushStream_ClientList(lpszClientAddr, &stl_ListClient);
		for (auto stl_ListIterator = stl_ListClient.begin(); stl_ListIterator != stl_ListClient.end(); stl_ListIterator++)
		{
			XEngine_Network_Send(stl_ListIterator->c_str(), ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _X("RTMP推流端：%s,接受推流数据,数据大小:%d"), lpszClientAddr, nMsgLen);
	}
	
	free(ptszRVBuffer);
	free(ptszSDBuffer);
	ptszRVBuffer = NULL;
	ptszSDBuffer = NULL;
	return true;
}