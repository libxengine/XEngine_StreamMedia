#include "../XEngine_Hdr.h"
/********************************************************************
//    Created:     2023/12/20  14:59:42
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PullStream\PullStream_ClientRtsp.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PullStream
//    File Base:   PullStream_ClientRtsp
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     RTSP推流服务
//    History:
*********************************************************************/
bool PullStream_ClientRtsp_RTCPProcess(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	if (nMsgLen > 4)
	{
		int nPos = 0;
		RTCPPROTOCOL_RTCPHDR st_RTCPHdr = {};
		RTCPProtocol_Parse_Header(lpszMsgBuffer, nMsgLen, &st_RTCPHdr);
		nPos += sizeof(RTCPPROTOCOL_RTCPHDR);

		int nListCount;
		RTCPPROTOCOL_RTCPRECVER** ppSt_ListRecvInfo;
		RTCPProtocol_Parse_Recver(lpszMsgBuffer + nPos, nMsgLen - nPos, &st_RTCPHdr, &ppSt_ListRecvInfo, &nListCount);
		nPos += sizeof(RTCPPROTOCOL_RTCPRECVER);
		//后续是否还有数据
		if (nMsgLen - nPos > 0)
		{
			//是SDES
			st_RTCPHdr = {};
			int nListCount = 0;
			STREAMMEDIA_RTCPPROTOCOL_SDESINFO** ppSt_ListSdeser;

			RTCPProtocol_Parse_Header(lpszMsgBuffer + nPos, nMsgLen - nPos, &st_RTCPHdr);
			nPos += sizeof(RTCPPROTOCOL_RTCPHDR);
			
			if (ENUM_STREAMMEDIA_RTCPPROTOCOL_PACKET_TYPE_SDES == st_RTCPHdr.byPT)
			{
				nPos -= sizeof(uint32_t);
			}
			RTCPProtocol_Parse_Sdeser(lpszMsgBuffer + nPos, nMsgLen - nPos, &st_RTCPHdr, &ppSt_ListSdeser, &nListCount);
			BaseLib_Memory_Free((XPPPMEM)&ppSt_ListSdeser, nListCount);
		}
		/*
		int nSDLen = 0;
		XCHAR tszMSGBuffer[1024] = {};
		RTCPPROTOCOL_RTCPSENDER st_SendInfo = {};

		RTCPProtocol_Packet_Sender(tszMSGBuffer, &nSDLen, st_RTCPHdr.unSsrc, &st_SendInfo);
		NetCore_UDPXCore_SendEx(xhVRTCPSocket, lpszClientAddr, tszMSGBuffer, nSDLen);

		nListCount = 1;
		STREAMMEDIA_RTCPPROTOCOL_SDESINFO** ppSt_SDESList;
		BaseLib_Memory_Malloc((XPPPMEM)&ppSt_SDESList, nListCount, sizeof(STREAMMEDIA_RTCPPROTOCOL_SDESINFO));

		memset(tszMSGBuffer, '\0', sizeof(tszMSGBuffer));
		ppSt_SDESList[0]->enSDESType = ENUM_STREAMMEDIA_RTCPPROTOCOL_SDES_TYPE_CNAME;
		ppSt_SDESList[0]->xhSsrc = st_RTCPHdr.unSsrc;
		ppSt_SDESList[0]->nMLen = _tcsxlen(XENGINE_NAME_STR);
		_tcsxcpy(ppSt_SDESList[0]->tszMSGBuffer, XENGINE_NAME_STR);
		RTCPProtocol_Packet_Sdeser(tszMSGBuffer, &nSDLen, &ppSt_SDESList, nListCount);
		NetCore_UDPXCore_SendEx(xhVRTCPSocket, lpszClientAddr, tszMSGBuffer, nSDLen);
		BaseLib_Memory_Free((XPPPMEM)&ppSt_SDESList, nListCount);*/
	}
	return true;
}
bool PullStream_ClientRtsp_Handle(RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR*** ppptszParamList, int nParamCount, XCHAR*** ppptszHDRList, int nHDRList)
{
	int nRVLen = 0;
	int nSDLen = 0;
	XCHAR tszRVBuffer[4096] = {};
	XCHAR tszSDBuffer[4096] = {};
	RTSPPROTOCOL_REQUEST st_RTSPRequest = {};
	RTSPPROTOCOL_RESPONSE st_RTSPResponse = {};

	RTSPProtocol_REQParse_Request(&st_RTSPRequest, pSt_HTTPParam->tszHttpMethod, pSt_HTTPParam->tszHttpUri, pSt_HTTPParam->tszHttpVer, ppptszHDRList, nHDRList);

	st_RTSPResponse.nCode = 200;
	st_RTSPResponse.nCSeq = st_RTSPRequest.nCseq;
	if (ENUM_RTSPPROTOCOL_METHOD_TYPE_OPTIONS == st_RTSPRequest.enMethod)
	{
		//OPTIONS rtsp://10.0.1.89:554/480p.264 RTSP/1.0
		XCHAR tszKeyStr[XPATH_MAX];
		XCHAR tszPushAddr[XPATH_MAX];
		XCHAR tszSMSAddr[XPATH_MAX];

		memset(tszPushAddr, '\0', sizeof(tszPushAddr));
		memset(tszSMSAddr, '\0', sizeof(tszSMSAddr));

		BaseLib_String_GetKeyValue((*ppptszParamList)[1], "=", tszKeyStr, tszSMSAddr);
		if (!ModuleSession_PushStream_FindStream(tszSMSAddr, tszPushAddr))
		{
			st_RTSPResponse.nCode = 404;
			RTSPProtocol_REPPacket_Response(tszSDBuffer, &nSDLen, &st_RTSPResponse);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("RTSP:%s,请求拉流的URL参数不正确:%s,错误:%lX"), lpszClientAddr, tszSMSAddr, ModuleSession_GetLastError());
			return false;
		}
		st_RTSPResponse.st_OPTion.bAnnounce = true;
		st_RTSPResponse.st_OPTion.bDescribe = true;
		st_RTSPResponse.st_OPTion.bOptions = true;
		st_RTSPResponse.st_OPTion.bPlay = true;
		st_RTSPResponse.st_OPTion.bSetup = true;
		st_RTSPResponse.st_OPTion.bTeardown = true;

		RTSPProtocol_REPPacket_Response(tszSDBuffer, &nSDLen, &st_RTSPResponse);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTSP:%s,请求OPTION选项成功,请求的拉流ID:%s"), lpszClientAddr, tszSMSAddr);
	}
	else if (ENUM_RTSPPROTOCOL_METHOD_TYPE_DESCRIBE == st_RTSPRequest.enMethod)
	{
		//DESCRIBE rtsp://10.0.1.89:554/480p.264 RTSP/1.0
		XCHAR tszKeyStr[XPATH_MAX];
		XCHAR tszPushAddr[XPATH_MAX];
		XCHAR tszSMSAddr[XPATH_MAX];
		XENGINE_PROTOCOL_AVINFO st_AVInfo;
		
		memset(tszPushAddr, '\0', sizeof(tszPushAddr));
		memset(tszSMSAddr, '\0', sizeof(tszSMSAddr));
		memset(&st_AVInfo, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));

		BaseLib_String_GetKeyValue((*ppptszParamList)[1], "=", tszKeyStr, tszSMSAddr);
		if (!ModuleSession_PushStream_FindStream(tszSMSAddr, tszPushAddr))
		{
			st_RTSPResponse.nCode = 404;
			RTSPProtocol_REPPacket_Response(tszSDBuffer, &nSDLen, &st_RTSPResponse);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("RTSP:%s,请求拉流的URL参数不正确:%s,错误:%lX"), lpszClientAddr, tszSMSAddr, ModuleSession_GetLastError());
			return false;
		}
		ModuleSession_PushStream_GetAVInfo(tszPushAddr, &st_AVInfo);

		int nSPSLen = 0;
		int nPPSLen = 0;
		XBYTE tszSPSBuffer[512] = {};
		XBYTE tszPPSBuffer[512] = {};
		AVHelp_Parse_VideoHdr(st_AVInfo.st_VideoInfo.tszVInfo, st_AVInfo.st_VideoInfo.nVLen, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264, NULL, tszSPSBuffer, tszPPSBuffer, NULL, NULL, &nSPSLen, &nPPSLen, NULL, NULL);
		XNETHANDLE xhSDPToken = 0;
		__int64x nSessionID = 0;
		SDPProtocol_Packet_Create(&xhSDPToken);
		SDPProtocol_Packet_Owner(xhSDPToken, _X("XEngine"), nSessionID, _X("0.0.0.0"));
		SDPProtocol_Packet_Session(xhSDPToken, tszSMSAddr);
		SDPProtocol_Packet_KeepTime(xhSDPToken);
		SDPProtocol_Packet_OptionalRange(xhSDPToken);
		SDPProtocol_Packet_Control(xhSDPToken, -1);
		//配置视频属性
		XCHAR** pptszAVList;
		BaseLib_Memory_Malloc((XPPPMEM)&pptszAVList, 1, 64);
		_tcsxcpy(pptszAVList[0], "96");

		SDPProtocol_Packet_AddMedia(xhSDPToken, _X("video"), _X("RTP/AVP"), &pptszAVList, 1);

		STREAMMEDIA_SDPPROTOCOL_MEDIAINFO st_SDPMediaVideo;
		memset(&st_SDPMediaVideo, '\0', sizeof(STREAMMEDIA_SDPPROTOCOL_MEDIAINFO));

		st_SDPMediaVideo.st_FmtpVideo.nFrameRate = st_AVInfo.st_VideoInfo.nFrameRate;
		st_SDPMediaVideo.st_FmtpVideo.nPacketMode = 1;
		st_SDPMediaVideo.st_FmtpVideo.tszLeaveId[0] = tszSPSBuffer[0];
		st_SDPMediaVideo.st_FmtpVideo.tszLeaveId[1] = tszSPSBuffer[1];
		st_SDPMediaVideo.st_FmtpVideo.tszLeaveId[2] = tszSPSBuffer[2];

		Cryption_Codec_Base64((LPCXSTR)tszSPSBuffer, st_SDPMediaVideo.st_FmtpVideo.tszSPSBase, &nSPSLen, true);
		Cryption_Codec_Base64((LPCXSTR)tszPPSBuffer, st_SDPMediaVideo.st_FmtpVideo.tszPPSBase, &nPPSLen, true);
		
		st_SDPMediaVideo.nTrackID = 0;
		st_SDPMediaVideo.st_RTPMap.nSampleRate = 90000;
		_tcsxcpy(st_SDPMediaVideo.st_RTPMap.tszCodecName, _X("H264"));
		SDPProtocol_Packet_VideoFmt(xhSDPToken, nRTPVIndex, &st_SDPMediaVideo);
		SDPProtocol_Packet_Control(xhSDPToken, 0);
		//配置音频属性
		_tcsxcpy(pptszAVList[0], _X("98"));
		SDPProtocol_Packet_AddMedia(xhSDPToken, _X("audio"), _X("RTP/AVP"), &pptszAVList, 1);

		STREAMMEDIA_SDPPROTOCOL_MEDIAINFO st_SDPMediaAudio;
		memset(&st_SDPMediaAudio, '\0', sizeof(STREAMMEDIA_SDPPROTOCOL_MEDIAINFO));

		AVHelp_Parse_AACInfo((const XBYTE*)st_AVInfo.st_AudioInfo.tszAInfo, st_AVInfo.st_AudioInfo.nALen, &st_SDPMediaAudio.st_RTPMap.nChannel, &st_SDPMediaAudio.st_RTPMap.nSampleRate, &st_SDPMediaAudio.st_FmtpAudio.nProfileID, &st_SDPMediaAudio.st_FmtpAudio.nConfig);

		st_SDPMediaAudio.nTrackID = 1;
		st_SDPMediaAudio.st_FmtpAudio.nDeltaLen = 3;
		st_SDPMediaAudio.st_FmtpAudio.nIndexLen = 3;
		st_SDPMediaAudio.st_FmtpAudio.nSizeLen = 13;

		st_SDPMediaAudio.st_RTPMap.nChannel = 2;
		st_SDPMediaAudio.st_RTPMap.nSampleRate = 90000;
		_tcsxcpy(st_SDPMediaAudio.st_RTPMap.tszCodecName, _X("mpeg4-generic"));

		_tcsxcpy(st_SDPMediaAudio.st_FmtpAudio.tszMode, "AAC-hbr");
		SDPProtocol_Packet_AudioFmt(xhSDPToken, nRTPAIndex, &st_SDPMediaAudio);
		SDPProtocol_Packet_Control(xhSDPToken, 1);

		SDPProtocol_Packet_GetPacket(xhSDPToken, tszRVBuffer, &nRVLen);
		SDPProtocol_Packet_Destory(xhSDPToken);
		ModuleHelp_Rtsp_CreateClient(lpszClientAddr, 0, 1);
		//创建SESSION
		BaseLib_Handle_CreateStr(st_RTSPResponse.tszSession, 12);
		ModuleHelp_Rtsp_SetSession(lpszClientAddr, st_RTSPResponse.tszSession);

		st_RTSPResponse.nPLen = nRVLen;
		_tcsxcpy(st_RTSPResponse.tszConBase, st_RTSPRequest.tszUrl);
		st_RTSPResponse.st_AdditionalOPtions.bDynamicRate = true;
		st_RTSPResponse.st_AdditionalOPtions.bRetransmit = true;

		RTSPProtocol_REPPacket_Response(tszSDBuffer, &nSDLen, &st_RTSPResponse);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		XEngine_Network_Send(lpszClientAddr, tszRVBuffer, nRVLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTSP:%s,请求DESCRIBE选项成功,请求的拉流ID:%s"), lpszClientAddr, tszSMSAddr);
	}
	else if (ENUM_RTSPPROTOCOL_METHOD_TYPE_SETUP == st_RTSPRequest.enMethod)
	{
		ModuleHelp_Rtsp_GetSession(lpszClientAddr, st_RTSPResponse.tszSession);
		ModuleHelp_Rtsp_SetClient(lpszClientAddr, st_RTSPRequest.st_TransportInfo.st_ClientPorts.nRTPPort, st_RTSPRequest.st_TransportInfo.st_ClientPorts.nRTCPPort, st_RTSPRequest.st_ChannelInfo.nChannelNumber);

		st_RTSPResponse.st_TransportInfo.st_TransFlags.bAVP = true;
		st_RTSPResponse.st_TransportInfo.st_TransFlags.bRTP = true;
		st_RTSPResponse.st_TransportInfo.st_TransFlags.bUDP = true;
		st_RTSPResponse.st_TransportInfo.st_TransTypes.bUnicast = true;

		st_RTSPResponse.st_TransportInfo.st_ClientPorts.nRTPPort = st_RTSPRequest.st_TransportInfo.st_ClientPorts.nRTPPort;
		st_RTSPResponse.st_TransportInfo.st_ClientPorts.nRTCPPort = st_RTSPRequest.st_TransportInfo.st_ClientPorts.nRTCPPort;

		bool bVideo = false;
		ModuleHelp_Rtsp_GetTrack(lpszClientAddr, st_RTSPRequest.st_ChannelInfo.nChannelNumber, &bVideo);
		if (bVideo)
		{
			st_RTSPResponse.st_TransportInfo.st_ServerPorts.nRTPPort = st_ServiceConfig.st_XPull.st_PullRtsp.nVRTPPort;
			st_RTSPResponse.st_TransportInfo.st_ServerPorts.nRTCPPort = st_ServiceConfig.st_XPull.st_PullRtsp.nVRTCPPort;
		}
		else
		{
			st_RTSPResponse.st_TransportInfo.st_ServerPorts.nRTPPort = st_ServiceConfig.st_XPull.st_PullRtsp.nARTPPort;
			st_RTSPResponse.st_TransportInfo.st_ServerPorts.nRTCPPort = st_ServiceConfig.st_XPull.st_PullRtsp.nARTCPPort;
		}
		BaseLib_Handle_CreateStr(st_RTSPResponse.st_TransportInfo.tszSSRCStr, 8, 1);
		ModuleHelp_Rtsp_SetSsrc(lpszClientAddr, st_RTSPResponse.st_TransportInfo.tszSSRCStr, bVideo);

		RTSPProtocol_REPPacket_Response(tszSDBuffer, &nSDLen, &st_RTSPResponse);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTSP:%s,请求SETUP选项成功,请求的设置的TRACKID:%d,客户端RTP端口:%d,客户端RTCP端口:%d"), lpszClientAddr, st_RTSPRequest.st_ChannelInfo.nChannelNumber, st_RTSPRequest.st_TransportInfo.st_ClientPorts.nRTPPort, st_RTSPRequest.st_TransportInfo.st_ClientPorts.nRTCPPort);
	}
	else if (ENUM_RTSPPROTOCOL_METHOD_TYPE_PLAY == st_RTSPRequest.enMethod)
	{
		XCHAR tszKeyStr[XPATH_MAX];
		XCHAR tszPushAddr[XPATH_MAX];
		XCHAR tszSMSAddr[XPATH_MAX];
		XENGINE_PROTOCOL_AVINFO st_AVInfo;

		memset(tszPushAddr, '\0', sizeof(tszPushAddr));
		memset(tszSMSAddr, '\0', sizeof(tszSMSAddr));
		memset(&st_AVInfo, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));

		BaseLib_String_GetKeyValue((*ppptszParamList)[1], "=", tszKeyStr, tszSMSAddr);

		if (!ModuleSession_PushStream_FindStream(tszSMSAddr, tszPushAddr))
		{
			st_RTSPResponse.nCode = 404;
			RTSPProtocol_REPPacket_Response(tszSDBuffer, &nSDLen, &st_RTSPResponse);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("RTSP:%s,请求拉流的URL参数不正确:%s,错误:%lX"), lpszClientAddr, tszSMSAddr, ModuleSession_GetLastError());
			return false;
		}
		ModuleSession_PushStream_GetAVInfo(tszPushAddr, &st_AVInfo);

		st_RTSPResponse.st_Range.bNPTTime = true;
		_tcsxcpy(st_RTSPResponse.tszSession, st_RTSPRequest.tszSession);

		st_RTSPResponse.nRTPCount = 2;
		BaseLib_Memory_Malloc((XPPPMEM)&st_RTSPResponse.ppSt_RTPInfo, 2, sizeof(RTSPPROTOCOL_RTPINFO));

		XCHAR tszSSRCVideo[128] = {};
		XCHAR tszSSRCAudio[128] = {};
		ModuleHelp_Rtsp_GetSsrc(lpszClientAddr, tszSSRCVideo, true);
		ModuleHelp_Rtsp_GetSsrc(lpszClientAddr, tszSSRCAudio, false);
		//创建RTP包管理器
		RTPProtocol_Packet_Insert(tszSSRCVideo);
		RTPProtocol_Packet_SetLink(tszSSRCVideo, nRTPVIndex, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_H264);
		RTPProtocol_Packet_Insert(tszSSRCAudio);
		RTPProtocol_Packet_SetLink(tszSSRCAudio, nRTPAIndex, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_AAC);

		if (st_AVInfo.st_VideoInfo.nFrameRate > 0)
		{
			RTPProtocol_Packet_SetTime(tszSSRCVideo, nRTPVIndex, st_AVInfo.st_VideoInfo.nFrameRate);
			RTPProtocol_Packet_GetTime(tszSSRCVideo, nRTPVIndex, &st_RTSPResponse.ppSt_RTPInfo[0]->nNTPTime);
			RTPProtocol_Packet_GetCSeq(tszSSRCVideo, nRTPVIndex, &st_RTSPResponse.ppSt_RTPInfo[0]->nCSeq);
			_xstprintf(st_RTSPResponse.ppSt_RTPInfo[0]->tszURLStr, _X("%s/trackID=0"), st_RTSPRequest.tszUrl);
		}
		if (st_AVInfo.st_AudioInfo.nSampleRate > 0)
		{
			RTPProtocol_Packet_SetTime(tszSSRCAudio, nRTPAIndex, st_AVInfo.st_AudioInfo.nSampleRate);
			RTPProtocol_Packet_GetTime(tszSSRCAudio, nRTPAIndex, &st_RTSPResponse.ppSt_RTPInfo[1]->nNTPTime);
			RTPProtocol_Packet_GetCSeq(tszSSRCAudio, nRTPAIndex, &st_RTSPResponse.ppSt_RTPInfo[1]->nCSeq);
			_xstprintf(st_RTSPResponse.ppSt_RTPInfo[1]->tszURLStr, _X("%s/trackID=1"), st_RTSPRequest.tszUrl);
		}
		RTSPProtocol_REPPacket_Response(tszSDBuffer, &nSDLen, &st_RTSPResponse);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);

		ModuleSession_PullStream_Insert(lpszClientAddr, tszSMSAddr, tszPushAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_RTSP);
		ModuleSession_PushStream_ClientInsert(tszPushAddr, lpszClientAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_RTSP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTSP:%s,请求PLAY选项成功,会话ID:%s,视频SSRC:%s,音频SSRC:%s"), lpszClientAddr, st_RTSPRequest.tszSession, tszSSRCVideo, tszSSRCAudio);
	}
	else if (ENUM_RTSPPROTOCOL_METHOD_TYPE_TEARDOWN == st_RTSPRequest.enMethod)
	{
		XCHAR tszSSRCVideo[128] = {};
		XCHAR tszSSRCAudio[128] = {};

		ModuleHelp_Rtsp_GetSsrc(lpszClientAddr, tszSSRCVideo, true);
		ModuleHelp_Rtsp_GetSsrc(lpszClientAddr, tszSSRCAudio, false);
		RTPProtocol_Packet_Delete(tszSSRCVideo);
		RTPProtocol_Packet_Delete(tszSSRCAudio);
		ModuleHelp_Rtsp_DeleteClient(lpszClientAddr);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTSP:%s,请求关闭选项成功,会话ID:%s,视频SSRC:%s,音频SSRC:%s"), lpszClientAddr, st_RTSPRequest.tszSession, tszSSRCVideo, tszSSRCAudio);
	}
	else
	{
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("RTSP:%s,请求的类型不支持:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpMethod);
		return false;
	}
	return true;
}