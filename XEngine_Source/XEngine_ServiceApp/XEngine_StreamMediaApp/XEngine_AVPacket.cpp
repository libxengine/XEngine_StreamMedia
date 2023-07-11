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
		if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_XSTREAM == enClientType)
		{
			XENGINE_PROTOCOLSTREAM st_ProtocolStream;
			memset(&st_ProtocolStream, '\0', sizeof(XENGINE_PROTOCOLSTREAM));

			memcpy(&st_ProtocolStream, lpszMsgBuffer, sizeof(XENGINE_PROTOCOLSTREAM));
			//创建流
			FLVProtocol_Packet_Insert(lpszClientAddr, st_ProtocolStream.st_AVInfo.st_VideoInfo.bEnable, st_ProtocolStream.st_AVInfo.st_AudioInfo.bEnable);
			int nPLen = 0;
			int nHLen = 0;
			XCHAR tszHDRBuffer[1024];
			memset(tszHDRBuffer, '\0', sizeof(tszHDRBuffer));
			//打包FLV
			FLVProtocol_Packet_FrameHdr(lpszClientAddr, tszHDRBuffer, &nPLen);
			nHLen += nPLen;
			st_ProtocolStream.st_AVInfo.st_VideoInfo.enAVCodec = 7;
			_tcsxcpy(st_ProtocolStream.st_AVInfo.tszPktName, _X("Lavf58.76.100"));

			FLVProtocol_Packet_FrameScript(lpszClientAddr, tszHDRBuffer + nHLen, &nPLen, &st_ProtocolStream.st_AVInfo);
			nHLen += nPLen;
			if (st_ProtocolStream.st_AVInfo.st_VideoInfo.bEnable)
			{
				FLVProtocol_Packet_FrameAVCConfigure(lpszClientAddr, tszHDRBuffer + nHLen, &nPLen, &st_ProtocolStream.st_AVInfo);
				nHLen += nPLen;
			}
			if (st_ProtocolStream.st_AVInfo.st_AudioInfo.bEnable)
			{
				FLVProtocol_Packet_FrameAACConfigure(lpszClientAddr, tszHDRBuffer + nHLen, &nPLen, &st_ProtocolStream.st_AVInfo);
				nHLen += nPLen;
			}
			//创建会话
			ModuleSession_PushStream_Create(lpszClientAddr, st_ProtocolStream.tszSMSAddr);
			ModuleSession_PushStream_SetHDRBuffer(lpszClientAddr, tszHDRBuffer, nHLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_FLV);
		}
		else if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP == enClientType)
		{
			int nHLen = 0;
			ModuleSession_PushStream_GetHDRBuffer(lpszClientAddr, NULL, &nHLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_FLV);
			if (0 != nHLen)
			{
				return true;
			}

			XENGINE_PROTOCOL_AVINFO st_AVInfo;
			ModuleSession_PushStream_GetAVInfo(lpszClientAddr, &st_AVInfo);
			if (0 == byAVType)
			{
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
					int nHLen = 0;
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

					ModuleSession_PushStream_SetHDRBuffer(lpszClientAddr, tszMsgBuffer, nHLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_FLV);
				}
			}
			else
			{
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
					int nHLen = 0;
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

					ModuleSession_PushStream_SetHDRBuffer(lpszClientAddr, tszMsgBuffer, nHLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_FLV);
				}
			}
		}
	}
	if (st_ServiceConfig.st_XPull.st_PullRtmp.bEnable)
	{
		int nHLen = 0;
		ModuleSession_PushStream_GetHDRBuffer(lpszClientAddr, NULL, &nHLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_FLV);
		if (0 != nHLen)
		{
			return true;
		}
		XENGINE_PROTOCOL_AVINFO st_AVInfo;
		ModuleSession_PushStream_GetAVInfo(lpszClientAddr, &st_AVInfo);
		if (0 == byAVType)
		{
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
				int nHLen = 0;
				int nPLen = 0;
				XCHAR tszMsgBuffer[1024];
				memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

				RTMPProtocol_Packet_FrameAVScript(tszMsgBuffer + nHLen, &nPLen, &st_AVInfo);
				nHLen += nPLen;
				RTMPProtocol_Packet_FrameAVCConfigure(tszMsgBuffer + nHLen, &nPLen, &st_AVInfo);
				nHLen += nPLen;
				RTMPProtocol_Packet_FrameAACConfigure(tszMsgBuffer + nHLen, &nPLen, &st_AVInfo);
				nHLen += nPLen;

				ModuleSession_PushStream_SetHDRBuffer(lpszClientAddr, tszMsgBuffer, nHLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_RTMP);
			}
		}
		else
		{
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
				int nHLen = 0;
				int nPLen = 0;
				XCHAR tszMsgBuffer[1024];

				memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

				RTMPProtocol_Packet_FrameAVScript(tszMsgBuffer + nHLen, &nPLen, &st_AVInfo);
				nHLen += nPLen;
				RTMPProtocol_Packet_FrameAVCConfigure(tszMsgBuffer + nHLen, &nPLen, &st_AVInfo);
				nHLen += nPLen;
				RTMPProtocol_Packet_FrameAACConfigure(tszMsgBuffer + nHLen, &nPLen, &st_AVInfo);
				nHLen += nPLen;

				ModuleSession_PushStream_SetHDRBuffer(lpszClientAddr, tszMsgBuffer, nHLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_RTMP);
			}
		}
	}
	return true;
}
bool XEngine_AVPacket_AVFrame(list<AVPACKET_MSGFRAME> *pStl_ListFrame, XCHAR* ptszRVBuffer, int* pInt_RVLen, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nTimeStamp, XBYTE byAVType, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enClientType)
{
	if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_XSTREAM == enClientType)
	{
		AVPACKET_MSGFRAME st_MSGFrame;
		memset(&st_MSGFrame, '\0', sizeof(AVPACKET_MSGFRAME));

		st_MSGFrame.enMSGType = ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_FLV;
		st_MSGFrame.ptszMsgBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);
		if (NULL == st_MSGFrame.ptszMsgBuffer);
		{
			return false;
		}
		if (0 == byAVType)
		{
			FLVProtocol_Packet_FrameVideo(lpszClientAddr, ptszRVBuffer, pInt_RVLen, lpszMsgBuffer + sizeof(XENGINE_PROTOCOL_AVDATA), nMsgLen - sizeof(XENGINE_PROTOCOL_AVDATA), nTimeStamp);
		}
		else
		{
			FLVProtocol_Packet_FrameAudio(lpszClientAddr, ptszRVBuffer, pInt_RVLen, lpszMsgBuffer + sizeof(XENGINE_PROTOCOL_AVDATA), nMsgLen - sizeof(XENGINE_PROTOCOL_AVDATA), nTimeStamp);
		}
		st_MSGFrame.nMsgLen = _xstprintf(st_MSGFrame.ptszMsgBuffer, _X("%x\r\n"), *pInt_RVLen);
		memcpy(st_MSGFrame.ptszMsgBuffer + st_MSGFrame.nMsgLen, ptszRVBuffer, *pInt_RVLen);
		st_MSGFrame.nMsgLen += *pInt_RVLen;

		memcpy(st_MSGFrame.ptszMsgBuffer + st_MSGFrame.nMsgLen, _X("\r\n"), 2);
		st_MSGFrame.nMsgLen += 2;
		pStl_ListFrame->push_back(st_MSGFrame);
	}
	else if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP == enClientType)
	{
		if (st_ServiceConfig.st_XPull.st_PullFlv.bEnable)
		{
			AVPACKET_MSGFRAME st_MSGFrame;
			memset(&st_MSGFrame, '\0', sizeof(AVPACKET_MSGFRAME));

			st_MSGFrame.enMSGType = ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_FLV;
			st_MSGFrame.ptszMsgBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);
			if (NULL == st_MSGFrame.ptszMsgBuffer);
			{
				return false;
			}
			if (0 == byAVType)
			{
				FLVProtocol_Packet_FrameCustom(lpszClientAddr, ptszRVBuffer, pInt_RVLen, lpszMsgBuffer, nMsgLen, -1, 9);
				st_MSGFrame.nMsgLen = _xstprintf(st_MSGFrame.ptszMsgBuffer, _X("%x\r\n"), *pInt_RVLen);
				memcpy(st_MSGFrame.ptszMsgBuffer + st_MSGFrame.nMsgLen, ptszRVBuffer, *pInt_RVLen);
				st_MSGFrame.nMsgLen += *pInt_RVLen;

				memcpy(st_MSGFrame.ptszMsgBuffer + st_MSGFrame.nMsgLen, _X("\r\n"), 2);
				st_MSGFrame.nMsgLen += 2;
			}
			else
			{
				FLVProtocol_Packet_FrameCustom(lpszClientAddr, ptszRVBuffer, pInt_RVLen, lpszMsgBuffer, nMsgLen, -1, 8);
				st_MSGFrame.nMsgLen = _xstprintf(st_MSGFrame.ptszMsgBuffer, _X("%x\r\n"), *pInt_RVLen);
				memcpy(st_MSGFrame.ptszMsgBuffer + st_MSGFrame.nMsgLen, ptszRVBuffer, *pInt_RVLen);
				st_MSGFrame.nMsgLen += *pInt_RVLen;

				memcpy(st_MSGFrame.ptszMsgBuffer + st_MSGFrame.nMsgLen, _X("\r\n"), 2);
				st_MSGFrame.nMsgLen += 2;
			}
			pStl_ListFrame->push_back(st_MSGFrame);
		}
		if (st_ServiceConfig.st_XPull.st_PullRtmp.bEnable)
		{
			AVPACKET_MSGFRAME st_MSGFrame;
			memset(&st_MSGFrame, '\0', sizeof(AVPACKET_MSGFRAME));

			st_MSGFrame.enMSGType = ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_FLV;
			st_MSGFrame.ptszMsgBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);
			if (NULL == st_MSGFrame.ptszMsgBuffer);
			{
				return false;
			}
			if (0 == byAVType)
			{
				RTMPProtocol_Packet_FrameCustom(lpszClientAddr, ptszRVBuffer, pInt_RVLen, lpszMsgBuffer, nMsgLen, -1, 9);
				st_MSGFrame.nMsgLen = _xstprintf(st_MSGFrame.ptszMsgBuffer, _X("%x\r\n"), *pInt_RVLen);
				memcpy(st_MSGFrame.ptszMsgBuffer + st_MSGFrame.nMsgLen, ptszRVBuffer, *pInt_RVLen);
				st_MSGFrame.nMsgLen += *pInt_RVLen;

				memcpy(st_MSGFrame.ptszMsgBuffer + st_MSGFrame.nMsgLen, _X("\r\n"), 2);
				st_MSGFrame.nMsgLen += 2;
			}
			else
			{
				RTMPProtocol_Packet_FrameCustom(lpszClientAddr, ptszRVBuffer, pInt_RVLen, lpszMsgBuffer, nMsgLen, -1, 8);
				st_MSGFrame.nMsgLen = _xstprintf(st_MSGFrame.ptszMsgBuffer, _X("%x\r\n"), *pInt_RVLen);
				memcpy(st_MSGFrame.ptszMsgBuffer + st_MSGFrame.nMsgLen, ptszRVBuffer, *pInt_RVLen);
				st_MSGFrame.nMsgLen += *pInt_RVLen;

				memcpy(st_MSGFrame.ptszMsgBuffer + st_MSGFrame.nMsgLen, _X("\r\n"), 2);
				st_MSGFrame.nMsgLen += 2;
			}
			pStl_ListFrame->push_back(st_MSGFrame);
		}
	}
	return true;
}