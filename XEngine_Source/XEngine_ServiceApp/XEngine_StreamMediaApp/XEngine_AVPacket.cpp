#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2023/07/10  16:58:28
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\XEngine_AVPacket.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp
//    File Base:   XEngine_AVPacket
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     bool XEngine_AVPacket_AVHdr()
//    History:
*********************************************************************/
bool XEngine_AVPacket_AVCreate(LPCXSTR lpszClientAddr)
{
	if (st_ServiceConfig.st_XPull.st_PullFlv.bEnable)
	{
		FLVProtocol_Packet_Insert(lpszClientAddr);
	}
	if (st_ServiceConfig.st_XPull.st_PullRtmp.bEnable)
	{
		RTMPProtocol_Packet_Insert(lpszClientAddr);
	}
	if (st_ServiceConfig.st_XPull.st_PullHls.bEnable)
	{
		HLSProtocol_TSPacket_Insert(lpszClientAddr, 100);
	}
	return true;
}
bool XEngine_AVPacket_AVDelete(LPCXSTR lpszClientAddr)
{
	if (st_ServiceConfig.st_XPull.st_PullFlv.bEnable)
	{
		FLVProtocol_Packet_Delete(lpszClientAddr);
	}
	if (st_ServiceConfig.st_XPull.st_PullRtmp.bEnable)
	{
		RTMPProtocol_Packet_Delete(lpszClientAddr);
	}
	if (st_ServiceConfig.st_XPull.st_PullHls.bEnable)
	{
		HLSProtocol_TSPacket_Delete(lpszClientAddr);
	}
	return true;
}
bool XEngine_AVPacket_AVSetTime(LPCXSTR lpszClientAddr, int nVideoParament, int nAudioParament)
{
	if (st_ServiceConfig.st_XPull.st_PullFlv.bEnable)
	{
		FLVProtocol_Packet_SetTime(lpszClientAddr, nVideoParament, nAudioParament);
	}
	if (st_ServiceConfig.st_XPull.st_PullRtmp.bEnable)
	{
		RTMPProtocol_Packet_SetTime(lpszClientAddr, nVideoParament, nAudioParament);
	}
	if (st_ServiceConfig.st_XPull.st_PullHls.bEnable)
	{
		HLSProtocol_TSPacket_SetTime(lpszClientAddr, nVideoParament, nAudioParament);
	}
	return true;
}
bool XEngine_AVPacket_AVHdr(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XBYTE byAVType, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enClientType)
{
	if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_XSTREAM == enClientType)
	{
		//XSTREAM推流
		XENGINE_PROTOCOL_AVINFO st_AVInfo;
		memset(&st_AVInfo, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));

		ModuleSession_PushStream_GetAVInfo(lpszClientAddr, &st_AVInfo);
		if (0 == byAVType)
		{
			int nPos = 0;
			int nSPSLen = 0;
			int nPPSLen = 0;
			XBYTE uszSPSBuffer[512];
			XBYTE uszPPSBuffer[512];

			memset(uszSPSBuffer, '\0', sizeof(uszSPSBuffer));
			memset(uszPPSBuffer, '\0', sizeof(uszPPSBuffer));
			if (!AVHelp_Parse_VideoHdr(lpszMsgBuffer, nMsgLen, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264, NULL, uszSPSBuffer, uszPPSBuffer, NULL, NULL, &nSPSLen, &nPPSLen, NULL, &nPos))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("XStream推流端:%s,解析SPS,PPS,VPS失败,错误:%lX"), lpszClientAddr, AVHelp_GetLastError());
				return false;
			}
			_tcsxcpy(st_AVInfo.tszPktName, "Lavf59.27.100");
			st_AVInfo.st_VideoInfo.enAVCodec = 7;
			st_AVInfo.st_VideoInfo.nVLen = nPos;
			memset(st_AVInfo.st_VideoInfo.tszVInfo, '\0', sizeof(st_AVInfo.st_VideoInfo.tszVInfo));
			memcpy(st_AVInfo.st_VideoInfo.tszVInfo, lpszMsgBuffer, nPos);

			ModuleSession_PushStream_SetAVInfo(lpszClientAddr, &st_AVInfo);
		}
		else
		{
			//音频参数信息是否存在
			if (0 == st_AVInfo.st_AudioInfo.nALen)
			{
				int nProfile = 0;
				int nConfig = 0;
				if (AVHelp_Parse_AACInfo((const XBYTE*)lpszMsgBuffer, nMsgLen, &st_AVInfo.st_AudioInfo.nChannel, &st_AVInfo.st_AudioInfo.nSampleRate, &nProfile, &nConfig))
				{
					st_AVInfo.st_AudioInfo.bEnable = true;
					st_AVInfo.st_AudioInfo.nALen = 7;
					st_AVInfo.st_AudioInfo.enAVCodec = 10;
					st_AVInfo.st_AudioInfo.nSampleFmt = 16;
					st_AVInfo.st_AudioInfo.nChannel = 1;
					AVHelp_Packet_AACHdr((XBYTE*)st_AVInfo.st_AudioInfo.tszAInfo, st_AVInfo.st_AudioInfo.nSampleRate, st_AVInfo.st_AudioInfo.nChannel, 0);
					ModuleSession_PushStream_SetAVInfo(lpszClientAddr, &st_AVInfo);
				}
			}
		}
	}
	else if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP == enClientType)
	{
		//RTMP推流的所有开关选项都是一样的代码
		XENGINE_PROTOCOL_AVINFO st_AVInfo;
		memset(&st_AVInfo, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));

		ModuleSession_PushStream_GetAVInfo(lpszClientAddr, &st_AVInfo);
		if (0 == byAVType)
		{
			XENGINE_RTMPVIDEO st_RTMPVideo;
			XENGINE_RTMPVIDEOPARAM st_RTMPVParam;

			memset(&st_RTMPVideo, '\0', sizeof(XENGINE_RTMPVIDEO));
			memset(&st_RTMPVParam, '\0', sizeof(XENGINE_RTMPVIDEOPARAM));
			memcpy(&st_RTMPVideo, lpszMsgBuffer, sizeof(XENGINE_RTMPVIDEO));

			st_AVInfo.st_VideoInfo.nVLen = 0;
			memset(st_AVInfo.st_VideoInfo.tszVInfo, '\0', sizeof(st_AVInfo.st_VideoInfo.tszVInfo));

			if (RTMPProtocol_Help_ParseVideo(&st_RTMPVideo, st_AVInfo.st_VideoInfo.tszVInfo, &st_AVInfo.st_VideoInfo.nVLen, lpszMsgBuffer + sizeof(XENGINE_RTMPVIDEO), nMsgLen - sizeof(XENGINE_RTMPVIDEO), &st_RTMPVParam))
			{
				ModuleSession_PushStream_SetAVInfo(lpszClientAddr, &st_AVInfo);
			}
		}
		else
		{
			//音频参数信息是否存在
			if (0 == st_AVInfo.st_AudioInfo.nALen)
			{
				st_AVInfo.st_AudioInfo.nALen = 7;
				AVHelp_Packet_AACHdr((XBYTE*)st_AVInfo.st_AudioInfo.tszAInfo, st_AVInfo.st_AudioInfo.nSampleRate, st_AVInfo.st_AudioInfo.nChannel, 0);
				ModuleSession_PushStream_SetAVInfo(lpszClientAddr, &st_AVInfo);
			}
		}
	}
	else if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_SRT == enClientType)
	{
		//SRT推流
		XENGINE_PROTOCOL_AVINFO st_AVInfo;
		memset(&st_AVInfo, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));

		ModuleSession_PushStream_GetAVInfo(lpszClientAddr, &st_AVInfo);
		if (0 == byAVType)
		{
			int nPos = 0;
			int nSPSLen = 0;
			int nPPSLen = 0;
			XBYTE uszSPSBuffer[512];
			XBYTE uszPPSBuffer[512];

			memset(uszSPSBuffer, '\0', sizeof(uszSPSBuffer));
			memset(uszPPSBuffer, '\0', sizeof(uszPPSBuffer));
			if (!AVHelp_Parse_VideoHdr(lpszMsgBuffer, nMsgLen, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264, NULL, uszSPSBuffer, uszPPSBuffer, NULL, NULL, &nSPSLen, &nPPSLen, NULL, &nPos))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("SRT推流端:%s,解析SPS,PPS,VPS失败,错误:%lX"), lpszClientAddr, AVHelp_GetLastError());
				return false;
			}
			AFHELP_FRAMESPS st_SPSFrame;
			memset(&st_SPSFrame, '\0', sizeof(AFHELP_FRAMESPS));

			AVHelp_Parse_SPSInfo((LPCXSTR)uszSPSBuffer + 1, nSPSLen - 1, &st_SPSFrame);
			_tcsxcpy(st_AVInfo.tszPktName, "Lavf59.27.100");
			st_AVInfo.st_VideoInfo.bEnable = true;
			st_AVInfo.st_VideoInfo.enAVCodec = 7;
			st_AVInfo.st_VideoInfo.nWidth = st_SPSFrame.nFrameWidth;
			st_AVInfo.st_VideoInfo.nHeight = st_SPSFrame.nFrameHeigth;
			st_AVInfo.st_VideoInfo.nFrameRate = st_SPSFrame.nFrameFPS;
			st_AVInfo.st_VideoInfo.nVLen = nPos;
			memset(st_AVInfo.st_VideoInfo.tszVInfo, '\0', sizeof(st_AVInfo.st_VideoInfo.tszVInfo));
			memcpy(st_AVInfo.st_VideoInfo.tszVInfo, lpszMsgBuffer, nPos);
			XEngine_AVPacket_AVSetTime(lpszClientAddr, st_AVInfo.st_VideoInfo.nFrameRate, st_AVInfo.st_AudioInfo.nSampleRate);
			ModuleSession_PushStream_SetAVInfo(lpszClientAddr, &st_AVInfo);
		}
		else
		{
			//音频参数信息是否存在
			if (0 == st_AVInfo.st_AudioInfo.nALen)
			{
				int nProfile = 0;
				int nConfig = 0;
				if (AVHelp_Parse_AACInfo((const XBYTE*)lpszMsgBuffer, nMsgLen, &st_AVInfo.st_AudioInfo.nChannel, &st_AVInfo.st_AudioInfo.nSampleRate, &nProfile, &nConfig))
				{
					st_AVInfo.st_AudioInfo.bEnable = true;
					st_AVInfo.st_AudioInfo.nALen = 7;
					st_AVInfo.st_AudioInfo.enAVCodec = 10;
					st_AVInfo.st_AudioInfo.nSampleFmt = 16;
					st_AVInfo.st_AudioInfo.nChannel = 1;
					XEngine_AVPacket_AVSetTime(lpszClientAddr, st_AVInfo.st_VideoInfo.nFrameRate, st_AVInfo.st_AudioInfo.nSampleRate);
					AVHelp_Packet_AACHdr((XBYTE*)st_AVInfo.st_AudioInfo.tszAInfo, st_AVInfo.st_AudioInfo.nSampleRate, st_AVInfo.st_AudioInfo.nChannel, 0);
					ModuleSession_PushStream_SetAVInfo(lpszClientAddr, &st_AVInfo);
				}
			}
		}
	}
	else if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_JT1078 == enClientType)
	{
		//JT1078推流
		XENGINE_PROTOCOL_AVINFO st_AVInfo;
		memset(&st_AVInfo, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));

		ModuleSession_PushStream_GetAVInfo(lpszClientAddr, &st_AVInfo);
		if (0 == byAVType)
		{
			int nPos = 0;
			int nSPSLen = 0;
			int nPPSLen = 0;
			XBYTE uszSPSBuffer[512];
			XBYTE uszPPSBuffer[512];

			memset(uszSPSBuffer, '\0', sizeof(uszSPSBuffer));
			memset(uszPPSBuffer, '\0', sizeof(uszPPSBuffer));
			if (!AVHelp_Parse_VideoHdr(lpszMsgBuffer, nMsgLen, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264, NULL, uszSPSBuffer, uszPPSBuffer, NULL, NULL, &nSPSLen, &nPPSLen, NULL, &nPos))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("JT1078推流端:%s,解析SPS,PPS,VPS失败,错误:%lX"), lpszClientAddr, AVHelp_GetLastError());
				return false;
			}
			AFHELP_FRAMESPS st_SPSFrame;
			memset(&st_SPSFrame, '\0', sizeof(AFHELP_FRAMESPS));

			AVHelp_Parse_SPSInfo((LPCXSTR)uszSPSBuffer + 1, nSPSLen - 1, &st_SPSFrame);
			_tcsxcpy(st_AVInfo.tszPktName, "Lavf59.27.100");
			st_AVInfo.st_VideoInfo.bEnable = true;
			st_AVInfo.st_VideoInfo.enAVCodec = 7;
			st_AVInfo.st_VideoInfo.nWidth = st_SPSFrame.nFrameWidth;
			st_AVInfo.st_VideoInfo.nHeight = st_SPSFrame.nFrameHeigth;
			st_AVInfo.st_VideoInfo.nFrameRate = st_SPSFrame.nFrameFPS;
			st_AVInfo.st_VideoInfo.nVLen = nPos;
			memset(st_AVInfo.st_VideoInfo.tszVInfo, '\0', sizeof(st_AVInfo.st_VideoInfo.tszVInfo));
			memcpy(st_AVInfo.st_VideoInfo.tszVInfo, lpszMsgBuffer, nPos);
			XEngine_AVPacket_AVSetTime(lpszClientAddr, st_AVInfo.st_VideoInfo.nFrameRate, st_AVInfo.st_AudioInfo.nSampleRate);
			ModuleSession_PushStream_SetAVInfo(lpszClientAddr, &st_AVInfo);
		}
		else
		{
			//音频参数信息是否存在
			if (0 == st_AVInfo.st_AudioInfo.nALen)
			{
				int nProfile = 0;
				int nConfig = 0;
				if (AVHelp_Parse_AACInfo((const XBYTE*)lpszMsgBuffer, nMsgLen, &st_AVInfo.st_AudioInfo.nChannel, &st_AVInfo.st_AudioInfo.nSampleRate, &nProfile, &nConfig))
				{
					st_AVInfo.st_AudioInfo.bEnable = true;
					st_AVInfo.st_AudioInfo.nALen = 7;
					st_AVInfo.st_AudioInfo.enAVCodec = 10;
					st_AVInfo.st_AudioInfo.nSampleFmt = 16;
					st_AVInfo.st_AudioInfo.nChannel = 1;
					XEngine_AVPacket_AVSetTime(lpszClientAddr, st_AVInfo.st_VideoInfo.nFrameRate, st_AVInfo.st_AudioInfo.nSampleRate);
					AVHelp_Packet_AACHdr((XBYTE*)st_AVInfo.st_AudioInfo.tszAInfo, st_AVInfo.st_AudioInfo.nSampleRate, st_AVInfo.st_AudioInfo.nChannel, 0);
					ModuleSession_PushStream_SetAVInfo(lpszClientAddr, &st_AVInfo);
				}
			}
		}
	}
	return true;
}
bool XEngine_AVPacket_AVFrame(XCHAR* ptszSDBuffer, int* pInt_SDLen, XCHAR* ptszRVBuffer, int* pInt_RVLen, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nTimeStamp, XBYTE byAVType, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enClientType)
{
	XBYTE byFrameType = 0;
	if (0 == byAVType)
	{
		XENGINE_AVCODEC_VIDEOFRAMETYPE enFrameType;
		AVHelp_Parse_NaluType(lpszMsgBuffer, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264, &enFrameType);
		//如果是关键帧
		if (ENUM_XENGINE_AVCODEC_VIDEO_FRAMETYPE_SPS == enFrameType || ENUM_XENGINE_AVCODEC_VIDEO_FRAMETYPE_PPS == enFrameType || ENUM_XENGINE_AVCODEC_VIDEO_FRAMETYPE_SEI == enFrameType)
		{
			byFrameType = 1;
		}
		else
		{
			byFrameType = 0;
		}
	}
	
	if (st_ServiceConfig.st_XPull.st_PullXStream.bEnable)
	{
		XENGINE_PROTOCOL_AVDATA st_ProtocolAVInfo;
		memset(&st_ProtocolAVInfo, '\0', sizeof(XENGINE_PROTOCOL_AVDATA));

		st_ProtocolAVInfo.byAVType = byAVType;
		st_ProtocolAVInfo.byFrameType = 0;
		st_ProtocolAVInfo.nFrameSize = nMsgLen;
		st_ProtocolAVInfo.nTimeStamp = nTimeStamp;

		*pInt_SDLen = _xstprintf(ptszSDBuffer, _X("%x\r\n"), nMsgLen);

		memcpy(ptszSDBuffer + *pInt_SDLen, &st_ProtocolAVInfo, sizeof(XENGINE_PROTOCOL_AVDATA));
		*pInt_SDLen += sizeof(XENGINE_PROTOCOL_AVDATA);

		memcpy(ptszSDBuffer + *pInt_SDLen, lpszMsgBuffer, nMsgLen);
		*pInt_SDLen += nMsgLen;

		memcpy(ptszSDBuffer + *pInt_SDLen, _X("\r\n"), 2);
		*pInt_SDLen += 2;
		//是否有客户端需要发送XStream流
		list<STREAMMEDIA_SESSIONCLIENT> stl_ListClient;
		ModuleSession_PushStream_ClientList(lpszClientAddr, &stl_ListClient);
		for (auto stl_ListIteratorClient = stl_ListClient.begin(); stl_ListIteratorClient != stl_ListClient.end(); ++stl_ListIteratorClient)
		{
			if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_XSTREAM == stl_ListIteratorClient->enClientType)
			{
				XEngine_Network_Send(stl_ListIteratorClient->tszClientID, ptszSDBuffer, *pInt_SDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			}
		}
	}
	if (st_ServiceConfig.st_XPull.st_PullFlv.bEnable)
	{
		//是否有客户端需要发送XStream流
		list<STREAMMEDIA_SESSIONCLIENT> stl_ListClient;
		ModuleSession_PushStream_ClientList(lpszClientAddr, &stl_ListClient);
		for (auto stl_ListIteratorClient = stl_ListClient.begin(); stl_ListIteratorClient != stl_ListClient.end(); ++stl_ListIteratorClient)
		{
			if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_FLV == stl_ListIteratorClient->enClientType)
			{
				int nTagSize = 0;
				ModuleSession_PullStream_FLVTagGet(stl_ListIteratorClient->tszClientID, &nTagSize);

				if (0 == byAVType)
				{
					FLVProtocol_Packet_FrameVideo(lpszClientAddr, ptszRVBuffer, pInt_RVLen, lpszMsgBuffer, nMsgLen, byFrameType);
				}
				else
				{
					FLVProtocol_Packet_FrameAudio(lpszClientAddr, ptszRVBuffer, pInt_RVLen, lpszMsgBuffer, nMsgLen);
				}
				*pInt_SDLen = _xstprintf(ptszSDBuffer, _X("%x\r\n"), *pInt_RVLen);
				memcpy(ptszSDBuffer + *pInt_SDLen, ptszRVBuffer, *pInt_RVLen);
				*pInt_SDLen += *pInt_RVLen;

				memcpy(ptszSDBuffer + *pInt_SDLen, _X("\r\n"), 2);
				*pInt_SDLen += 2;

				XEngine_Network_Send(stl_ListIteratorClient->tszClientID, ptszSDBuffer, *pInt_SDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
				ModuleSession_PullStream_FLVTagSet(stl_ListIteratorClient->tszClientID, nTagSize);
			}
		}
	}
	if (st_ServiceConfig.st_XPull.st_PullRtmp.bEnable)
	{
		if (0 == byAVType)
		{
			RTMPProtocol_Packet_FrameVideo(lpszClientAddr, ptszSDBuffer, pInt_SDLen, lpszMsgBuffer, nMsgLen, byFrameType);
		}
		else
		{
			RTMPProtocol_Packet_FrameAudio(lpszClientAddr, ptszSDBuffer, pInt_SDLen, lpszMsgBuffer, nMsgLen);
		}

		//是否有客户端需要发送XStream流
		list<STREAMMEDIA_SESSIONCLIENT> stl_ListClient;
		ModuleSession_PushStream_ClientList(lpszClientAddr, &stl_ListClient);
		for (auto stl_ListIteratorClient = stl_ListClient.begin(); stl_ListIteratorClient != stl_ListClient.end(); ++stl_ListIteratorClient)
		{
			if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_RTMP == stl_ListIteratorClient->enClientType)
			{
				XEngine_Network_Send(stl_ListIteratorClient->tszClientID, ptszSDBuffer, *pInt_SDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
			}
		}
	}
	if (st_ServiceConfig.st_XPull.st_PullHls.bEnable)
	{
		int nListCount = 0;
		XBYTE** pptszMsgBuffer = NULL;
		if (0 == byAVType)
		{
			HLSProtocol_TSPacket_AVPacket(lpszClientAddr, &pptszMsgBuffer, &nListCount, 0x100, lpszMsgBuffer, nMsgLen);
		}
		else
		{
			HLSProtocol_TSPacket_AVPacket(lpszClientAddr, &pptszMsgBuffer, &nListCount, 0x101, lpszMsgBuffer, nMsgLen);
		}
		//是否有客户端需要发送XStream流
		list<STREAMMEDIA_SESSIONCLIENT> stl_ListClient;
		ModuleSession_PushStream_ClientList(lpszClientAddr, &stl_ListClient);
		for (auto stl_ListIteratorClient = stl_ListClient.begin(); stl_ListIteratorClient != stl_ListClient.end(); ++stl_ListIteratorClient)
		{
			if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_TS == stl_ListIteratorClient->enClientType)
			{
				for (int i = 0; i < nListCount; i++)
				{
					XEngine_Network_Send(stl_ListIteratorClient->tszClientID, (LPCXSTR)pptszMsgBuffer[i], 188, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_TS);
				}
			}
		}
	}
	return true;
}