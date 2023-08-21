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
	return true;
}
bool XEngine_AVPacket_AVHdr(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XBYTE byAVType, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enClientType)
{
	if (st_ServiceConfig.st_XPull.st_PullFlv.bEnable)
	{
		//启用了FLV,把所有流转为FLV
		if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_XSTREAM == enClientType)
		{
			XENGINE_PROTOCOLSTREAM st_ProtocolStream;
			memset(&st_ProtocolStream, '\0', sizeof(XENGINE_PROTOCOLSTREAM));

			memcpy(&st_ProtocolStream, lpszMsgBuffer, sizeof(XENGINE_PROTOCOLSTREAM));
			//创建流
			FLVProtocol_Packet_Insert(lpszClientAddr, st_ProtocolStream.st_AVInfo.st_VideoInfo.bEnable, st_ProtocolStream.st_AVInfo.st_AudioInfo.bEnable);
			FLVProtocol_Packet_SetTime(lpszClientAddr, st_ProtocolStream.st_AVInfo.st_VideoInfo.nFrameRate, st_ProtocolStream.st_AVInfo.st_AudioInfo.nSampleRate);

			st_ProtocolStream.st_AVInfo.st_VideoInfo.enAVCodec = 7;
			_tcsxcpy(st_ProtocolStream.st_AVInfo.tszPktName, _X("Lavf58.76.100"));
			//创建会话
			ModuleSession_PushStream_SetAVInfo(lpszClientAddr, &st_ProtocolStream.st_AVInfo);
			ModuleSession_PushStream_Create(lpszClientAddr, st_ProtocolStream.tszSMSAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_XSTREAM);
		}
		else if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP == enClientType)
		{
			XENGINE_PROTOCOL_AVINFO st_AVInfo;
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

				RTMPProtocol_Help_ParseVideo(&st_RTMPVideo, st_AVInfo.st_VideoInfo.tszVInfo, &st_AVInfo.st_VideoInfo.nVLen, lpszMsgBuffer + sizeof(XENGINE_RTMPVIDEO), nMsgLen - sizeof(XENGINE_RTMPVIDEO), &st_RTMPVParam);
				ModuleSession_PushStream_SetAVInfo(lpszClientAddr, &st_AVInfo);
			}
			else
			{
				//音频参数信息是否存在
				if (0 == st_AVInfo.st_AudioInfo.nALen)
				{
					st_AVInfo.st_AudioInfo.nALen = 7;
					AVHelp_Packet_AACHdr((XBYTE *)st_AVInfo.st_AudioInfo.tszAInfo, st_AVInfo.st_AudioInfo.nSampleRate, st_AVInfo.st_AudioInfo.nChannel, 0);
					ModuleSession_PushStream_SetAVInfo(lpszClientAddr, &st_AVInfo);
				}
			}
		}
	}
	if (st_ServiceConfig.st_XPull.st_PullRtmp.bEnable)
	{
		//启用了RTMP,把所有流转RTMP
		if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_XSTREAM == enClientType)
		{
			XENGINE_PROTOCOLSTREAM st_ProtocolStream;
			memset(&st_ProtocolStream, '\0', sizeof(XENGINE_PROTOCOLSTREAM));

			memcpy(&st_ProtocolStream, lpszMsgBuffer, sizeof(XENGINE_PROTOCOLSTREAM));
			//创建流
			RTMPProtocol_Packet_Insert(lpszClientAddr);
			RTMPProtocol_Packet_SetTime(lpszClientAddr, st_ProtocolStream.st_AVInfo.st_VideoInfo.nFrameRate, st_ProtocolStream.st_AVInfo.st_AudioInfo.nSampleRate);
			//打包FLV
			st_ProtocolStream.st_AVInfo.st_VideoInfo.enAVCodec = 7;
			_tcsxcpy(st_ProtocolStream.st_AVInfo.tszPktName, _X("Lavf58.76.100"));
			//创建会话
			ModuleSession_PushStream_SetAVInfo(lpszClientAddr, &st_ProtocolStream.st_AVInfo);
			ModuleSession_PushStream_Create(lpszClientAddr, st_ProtocolStream.tszSMSAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_XSTREAM);
		}
		else if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP == enClientType)
		{
			//推流端类型是rtmp
			XENGINE_PROTOCOL_AVINFO st_AVInfo;
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

				RTMPProtocol_Help_ParseVideo(&st_RTMPVideo, st_AVInfo.st_VideoInfo.tszVInfo, &st_AVInfo.st_VideoInfo.nVLen, lpszMsgBuffer + sizeof(XENGINE_RTMPVIDEO), nMsgLen - sizeof(XENGINE_RTMPVIDEO), &st_RTMPVParam);
				ModuleSession_PushStream_SetAVInfo(lpszClientAddr, &st_AVInfo);
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
	}
	if (st_ServiceConfig.st_XPull.st_PullXStream.bEnable)
	{
		if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_XSTREAM == enClientType)
		{
			XENGINE_PROTOCOLSTREAM st_ProtocolStream;
			memset(&st_ProtocolStream, '\0', sizeof(XENGINE_PROTOCOLSTREAM));

			memcpy(&st_ProtocolStream, lpszMsgBuffer, sizeof(XENGINE_PROTOCOLSTREAM));
			//创建会话
			ModuleSession_PushStream_Create(lpszClientAddr, st_ProtocolStream.tszSMSAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_XSTREAM);
			ModuleSession_PushStream_SetAVInfo(lpszClientAddr, &st_ProtocolStream.st_AVInfo);
		}
	}
	return true;
}
bool XEngine_AVPacket_AVFrame(XCHAR* ptszSDBuffer, int* pInt_SDLen, XCHAR* ptszRVBuffer, int* pInt_RVLen, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nTimeStamp, XBYTE byAVType, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enClientType)
{
	if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_XSTREAM == enClientType)
	{
		if (st_ServiceConfig.st_XPull.st_PullXStream.bEnable)
		{
			*pInt_SDLen = _xstprintf(ptszSDBuffer, _X("%x\r\n"), nMsgLen);
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
					break;
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
						FLVProtocol_Packet_FrameVideo(lpszClientAddr, ptszRVBuffer, pInt_RVLen, lpszMsgBuffer + sizeof(XENGINE_PROTOCOL_AVDATA), nMsgLen - sizeof(XENGINE_PROTOCOL_AVDATA), nTimeStamp);
					}
					else
					{
						FLVProtocol_Packet_FrameAudio(lpszClientAddr, ptszRVBuffer, pInt_RVLen, lpszMsgBuffer + sizeof(XENGINE_PROTOCOL_AVDATA), nMsgLen - sizeof(XENGINE_PROTOCOL_AVDATA), nTimeStamp);
					}
					*pInt_SDLen = _xstprintf(ptszSDBuffer, _X("%x\r\n"), *pInt_RVLen);
					memcpy(ptszSDBuffer + *pInt_SDLen, ptszRVBuffer, *pInt_RVLen);
					*pInt_SDLen += *pInt_RVLen;

					memcpy(ptszSDBuffer + *pInt_SDLen, _X("\r\n"), 2);
					*pInt_SDLen += 2;

					XEngine_Network_Send(stl_ListIteratorClient->tszClientID, ptszSDBuffer, *pInt_SDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
					ModuleSession_PullStream_FLVTagSet(stl_ListIteratorClient->tszClientID, nTagSize);
					break;
				}
			}
		}
		if (st_ServiceConfig.st_XPull.st_PullRtmp.bEnable)
		{
			if (0 == byAVType)
			{
				RTMPProtocol_Packet_FrameVideo(lpszClientAddr, ptszSDBuffer, pInt_SDLen, lpszMsgBuffer + sizeof(XENGINE_PROTOCOL_AVDATA), nMsgLen - sizeof(XENGINE_PROTOCOL_AVDATA), nTimeStamp);
			}
			else
			{
				RTMPProtocol_Packet_FrameAudio(lpszClientAddr, ptszSDBuffer, pInt_SDLen, lpszMsgBuffer + sizeof(XENGINE_PROTOCOL_AVDATA), nMsgLen - sizeof(XENGINE_PROTOCOL_AVDATA), nTimeStamp);
			}
			//是否有客户端需要发送XStream流
			list<STREAMMEDIA_SESSIONCLIENT> stl_ListClient;
			ModuleSession_PushStream_ClientList(lpszClientAddr, &stl_ListClient);
			for (auto stl_ListIteratorClient = stl_ListClient.begin(); stl_ListIteratorClient != stl_ListClient.end(); ++stl_ListIteratorClient)
			{
				if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_RTMP == stl_ListIteratorClient->enClientType)
				{
					XEngine_Network_Send(stl_ListIteratorClient->tszClientID, ptszSDBuffer, *pInt_SDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
					break;
				}
			}
		}
	}
	else if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP == enClientType)
	{
		if (st_ServiceConfig.st_XPull.st_PullXStream.bEnable)
		{
			int nPos = 0;
			XENGINE_RTMPVIDEO st_RTMPVideo;
			XENGINE_PROTOCOL_AVDATA st_ProtocolAVData;

			memset(&st_RTMPVideo, '\0', sizeof(XENGINE_RTMPVIDEO));
			memset(&st_ProtocolAVData, '\0', sizeof(XENGINE_PROTOCOL_AVDATA));

			memcpy(&st_RTMPVideo, lpszMsgBuffer, sizeof(XENGINE_RTMPVIDEO));

			st_ProtocolAVData.byAVType = byAVType;
			st_ProtocolAVData.nTimeStamp = nTimeStamp;
			st_ProtocolAVData.byFrameType = st_RTMPVideo.byFrameType;
			
			memcpy(ptszRVBuffer, &st_ProtocolAVData, sizeof(XENGINE_PROTOCOL_AVDATA));
			nPos += sizeof(XENGINE_PROTOCOL_AVDATA);

			RTMPProtocol_Help_ParseVideo(&st_RTMPVideo, ptszRVBuffer + nPos, pInt_RVLen, lpszMsgBuffer + sizeof(XENGINE_RTMPVIDEO), nMsgLen - sizeof(XENGINE_RTMPVIDEO));
			*pInt_RVLen += nPos;

			*pInt_SDLen = _xstprintf(ptszSDBuffer, _X("%x\r\n"), *pInt_RVLen);
			memcpy(ptszSDBuffer + *pInt_SDLen, ptszRVBuffer, *pInt_RVLen);
			*pInt_SDLen += *pInt_RVLen;

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
					break;
				}
			}
		}
		if (st_ServiceConfig.st_XPull.st_PullFlv.bEnable)
		{
			//是否有客户端需要发送FLV流
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
						FLVProtocol_Packet_FrameCustom(lpszClientAddr, ptszRVBuffer, pInt_RVLen, lpszMsgBuffer, nMsgLen, -1, 9, &nTagSize);
						*pInt_SDLen = _xstprintf(ptszSDBuffer, _X("%x\r\n"), *pInt_RVLen);
						memcpy(ptszSDBuffer + *pInt_SDLen, ptszRVBuffer, *pInt_RVLen);
						*pInt_SDLen += *pInt_RVLen;

						memcpy(ptszSDBuffer + *pInt_SDLen, _X("\r\n"), 2);
						*pInt_SDLen += 2;
					}
					else
					{
						FLVProtocol_Packet_FrameCustom(lpszClientAddr, ptszRVBuffer, pInt_RVLen, lpszMsgBuffer, nMsgLen, -1, 8, &nTagSize);
						*pInt_SDLen = _xstprintf(ptszSDBuffer, _X("%x\r\n"), *pInt_RVLen);
						memcpy(ptszSDBuffer + *pInt_SDLen, ptszRVBuffer, *pInt_RVLen);
						*pInt_SDLen += *pInt_RVLen;

						memcpy(ptszSDBuffer + *pInt_SDLen, _X("\r\n"), 2);
						*pInt_SDLen += 2;
					}
					XEngine_Network_Send(stl_ListIteratorClient->tszClientID, ptszSDBuffer, *pInt_SDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
					ModuleSession_PullStream_FLVTagSet(stl_ListIteratorClient->tszClientID, nTagSize);
					break;
				}
			}
		}
		if (st_ServiceConfig.st_XPull.st_PullRtmp.bEnable)
		{
			if (0 == byAVType)
			{
				RTMPProtocol_Packet_FrameCustom(lpszClientAddr, ptszSDBuffer, pInt_SDLen, lpszMsgBuffer, nMsgLen, -1, 9);
			}
			else
			{
				RTMPProtocol_Packet_FrameCustom(lpszClientAddr, ptszSDBuffer, pInt_SDLen, lpszMsgBuffer, nMsgLen, -1, 8);
			}
			//是否有客户端需要发送RTMP流
			list<STREAMMEDIA_SESSIONCLIENT> stl_ListClient;
			ModuleSession_PushStream_ClientList(lpszClientAddr, &stl_ListClient);
			for (auto stl_ListIteratorClient = stl_ListClient.begin(); stl_ListIteratorClient != stl_ListClient.end(); ++stl_ListIteratorClient)
			{
				if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_RTMP == stl_ListIteratorClient->enClientType)
				{
					XEngine_Network_Send(stl_ListIteratorClient->tszClientID, ptszSDBuffer, *pInt_SDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP);
					break;
				}
			}
		}
	}
	return true;
}