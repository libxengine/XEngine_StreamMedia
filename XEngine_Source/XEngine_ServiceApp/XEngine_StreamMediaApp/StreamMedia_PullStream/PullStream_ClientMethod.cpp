#include "../XEngine_Hdr.h"
/********************************************************************
//    Created:     2023/09/18  14:19:28
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PullStream\PullStream_ClientMethod.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PullStream
//    File Base:   PullStream_ClientMethod
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     其他方法处理
//    History:
*********************************************************************/
bool PullStream_ClientMethod_Handle(RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR*** ppptszListHdr, int nListCount)
{
	int nRVLen = 0;
	int nSDLen = 0;
	XCHAR tszRVBuffer[4096] = {};
	XCHAR tszSDBuffer[4096] = {};
	RTSPPROTOCOL_REQUEST st_RTSPRequest = {};
	RTSPPROTOCOL_RESPONSE st_RTSPResponse = {};

	RTSPProtocol_REQParse_Request(&st_RTSPRequest, pSt_HTTPParam->tszHttpMethod, pSt_HTTPParam->tszHttpUri, pSt_HTTPParam->tszHttpVer, ppptszListHdr, nListCount);

	st_RTSPResponse.nCode = 200;
	st_RTSPResponse.nCSeq = st_RTSPRequest.nCseq;
	if (ENUM_RTSPPROTOCOL_METHOD_TYPE_OPTIONS == st_RTSPRequest.enMethod)
	{
		//OPTIONS rtsp://10.0.1.89:554/480p.264 RTSP/1.0
		XCHAR tszPushAddr[MAX_PATH];
		XCHAR tszSMSAddr[MAX_PATH];

		memset(tszPushAddr, '\0', sizeof(tszPushAddr));
		memset(tszSMSAddr, '\0', sizeof(tszSMSAddr));

		ModuleHelp_Rtsp_GetSMSAddr(st_RTSPRequest.tszUrl, tszSMSAddr);
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
		XCHAR tszPushAddr[MAX_PATH];
		XCHAR tszSMSAddr[MAX_PATH];
		XENGINE_PROTOCOL_AVINFO st_AVInfo;
		
		memset(tszPushAddr, '\0', sizeof(tszPushAddr));
		memset(tszSMSAddr, '\0', sizeof(tszSMSAddr));
		memset(&st_AVInfo, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));

		ModuleHelp_Rtsp_GetSMSAddr(st_RTSPRequest.tszUrl, tszSMSAddr);
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
		SDPProtocol_Packet_Control(xhSDPToken, -1);
		//配置视频属性
		int nListCount = 0;
		XCHAR** pptszAVList;
		BaseLib_OperatorMemory_Malloc((XPPPMEM)&pptszAVList, 1, 64);
		_tcsxcpy(pptszAVList[0], "96");

		SDPProtocol_Packet_AddMedia(xhSDPToken, _X("video"), _X("RTP/AVP"), &pptszAVList, 1);

		STREAMMEDIA_SDPPROTOCOL_MEDIAINFO st_SDPMediaVideo;
		memset(&st_SDPMediaVideo, '\0', sizeof(STREAMMEDIA_SDPPROTOCOL_MEDIAINFO));

		st_SDPMediaVideo.st_FmtpVideo.nFrameRate = st_AVInfo.st_VideoInfo.nFrameRate;
		st_SDPMediaVideo.st_FmtpVideo.nPacketMode = 1;
		st_SDPMediaVideo.st_FmtpVideo.tszLeaveId[0] = tszSPSBuffer[0];
		st_SDPMediaVideo.st_FmtpVideo.tszLeaveId[1] = tszSPSBuffer[1];
		st_SDPMediaVideo.st_FmtpVideo.tszLeaveId[2] = tszSPSBuffer[2];

		OPenSsl_Codec_Base64((LPCXSTR)tszSPSBuffer, st_SDPMediaVideo.st_FmtpVideo.tszSPSBase, &nSPSLen, true);
		OPenSsl_Codec_Base64((LPCXSTR)tszPPSBuffer, st_SDPMediaVideo.st_FmtpVideo.tszPPSBase, &nPPSLen, true);
		
		st_SDPMediaVideo.nTrackID = 0;
		st_SDPMediaVideo.st_RTPMap.nChannel = 2;
		st_SDPMediaVideo.st_RTPMap.nSampleRate = 90000;
		_tcsxcpy(st_SDPMediaVideo.st_RTPMap.tszCodecName, _X("H264"));
		SDPProtocol_Packet_VideoFmt(xhSDPToken, 96, &st_SDPMediaVideo);
		SDPProtocol_Packet_Control(xhSDPToken, 0);
		//配置音频属性
		_tcsxcpy(pptszAVList[0], _X("98"));
		SDPProtocol_Packet_AddMedia(xhSDPToken, _X("audio"), _X("RTP/AVP"), &pptszAVList, 1);

		STREAMMEDIA_SDPPROTOCOL_MEDIAINFO st_SDPMediaAudio;
		memset(&st_SDPMediaAudio, '\0', sizeof(STREAMMEDIA_SDPPROTOCOL_MEDIAINFO));

		AVHelp_Parse_AACInfo((const XBYTE*)st_AVInfo.st_AudioInfo.tszAInfo, st_AVInfo.st_AudioInfo.nALen, &st_SDPMediaAudio.st_RTPMap.nChannel, &st_SDPMediaAudio.st_RTPMap.nSampleRate, &st_SDPMediaAudio.st_FmtpAudio.nProfileID, &st_SDPMediaAudio.st_FmtpAudio.nConfig);

		st_SDPMediaAudio.nTrackID = 1;
		_tcsxcpy(st_SDPMediaAudio.st_RTPMap.tszCodecName, _X("mpeg4-generic"));

		st_SDPMediaAudio.st_FmtpAudio.nDeltaLen = 3;
		st_SDPMediaAudio.st_FmtpAudio.nIndexLen = 3;
		st_SDPMediaAudio.st_FmtpAudio.nSizeLen = 13;
		_tcsxcpy(st_SDPMediaAudio.st_FmtpAudio.tszMode, "AAC-hbr");
		SDPProtocol_Packet_AudioFmt(xhSDPToken, 98, &st_SDPMediaAudio);
		SDPProtocol_Packet_Control(xhSDPToken, 1);

		SDPProtocol_Packet_GetPacket(xhSDPToken, tszRVBuffer, &nRVLen);
		SDPProtocol_Packet_Destory(xhSDPToken);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTSP:%s,请求DESCRIBE选项成功,请求的拉流ID:%s"), lpszClientAddr, tszSMSAddr);
	}
	else
	{
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("拉流端:%s,请求的类型不支持:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpMethod);
		return false;
	}
	return true;
}