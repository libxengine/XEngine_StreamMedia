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
	XCHAR tszRVBuffer[4096];
	XCHAR tszSDBuffer[4096];
	LPCXSTR lpszOPTIONStr = _X("OPTIONS");
	LPCXSTR lpszDESCRIBEStr = _X("DESCRIBE");
	LPCXSTR lpszSETUPStr = _X("SETUP");
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;

	memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

	st_HDRParam.nHttpCode = 200; //HTTP CODE码
	st_HDRParam.bIsClose = true; //收到回复后就关闭

	if (0 == _tcsxnicmp(pSt_HTTPParam->tszHttpMethod, lpszOPTIONStr, _tcsxlen(pSt_HTTPParam->tszHttpMethod)))
	{
		//OPTIONS rtsp://10.0.1.89:554/480p.264 RTSP/1.0
		XCHAR tszPushAddr[MAX_PATH];
		XCHAR tszSMSAddr[MAX_PATH];

		memset(tszPushAddr, '\0', sizeof(tszPushAddr));
		memset(tszSMSAddr, '\0', sizeof(tszSMSAddr));
		if (!ModuleSession_PushStream_FindStream(tszSMSAddr, tszPushAddr))
		{
			ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, NULL, 404, "not found");
			HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("RTSP:%s,请求拉流的URL参数不正确:%s,错误:%lX"), lpszClientAddr, tszSMSAddr, ModuleSession_GetLastError());
			return false;
		}
		RTSPPROTOCOL_REQUEST st_RTSPRequest;
		RTSPPROTOCOL_OPTIONS st_RTSPOPtion;

		memset(&st_RTSPRequest, '\0', sizeof(RTSPPROTOCOL_REQUEST));
		memset(&st_RTSPOPtion, 0x01, sizeof(RTSPPROTOCOL_OPTIONS));

		RTSPProtocol_CoreParse_Parse(&st_RTSPRequest, lpszMsgBuffer, nMsgLen);
		RTSPProtocol_CorePacket_Option(tszSDBuffer, &nSDLen, &st_RTSPOPtion, st_RTSPRequest.nCseq);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTSP:%s,请求OPTION选项成功,请求的拉流ID:%s"), lpszClientAddr, tszSMSAddr);
	}
	else if (0 == _tcsxnicmp(pSt_HTTPParam->tszHttpMethod, lpszDESCRIBEStr, _tcsxlen(pSt_HTTPParam->tszHttpMethod)))
	{
		//DESCRIBE rtsp://10.0.1.89:554/480p.264 RTSP/1.0
		XCHAR tszPushAddr[MAX_PATH];
		XCHAR tszSMSAddr[MAX_PATH];
		XENGINE_PROTOCOL_AVINFO st_AVInfo;
		
		memset(tszPushAddr, '\0', sizeof(tszPushAddr));
		memset(tszSMSAddr, '\0', sizeof(tszSMSAddr));
		memset(&st_AVInfo, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));

		if (!ModuleSession_PushStream_FindStream(tszSMSAddr, tszPushAddr))
		{
			ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, NULL, 404, "not found");
			HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("RTSP:%s,请求拉流的URL参数不正确:%s,错误:%lX"), lpszClientAddr, tszSMSAddr, ModuleSession_GetLastError());
			return false;
		}
		RTSPPROTOCOL_REQUEST st_RTSPRequest;
		memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

		RTSPProtocol_CoreParse_Parse(&st_RTSPRequest, lpszMsgBuffer, nMsgLen);
		ModuleSession_PushStream_GetAVInfo(tszPushAddr, &st_AVInfo);

		int nSPSLen = 0;
		int nPPSLen = 0;
		XBYTE tszSPSBuffer[512];
		XBYTE tszPPSBuffer[512];

		memset(tszSPSBuffer, '\0', sizeof(tszSPSBuffer));
		memset(tszPPSBuffer, '\0', sizeof(tszPPSBuffer));

		AVHelp_Parse_VideoHdr(st_AVInfo.st_VideoInfo.tszVInfo, st_AVInfo.st_VideoInfo.nVLen, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264, NULL, tszSPSBuffer, tszPPSBuffer, NULL, NULL, &nSPSLen, &nPPSLen, NULL, NULL);

		XNETHANDLE xhSDPToken = 0;
		__int64x nSessionID = 0;
		RfcComponents_SDPPacket_Create(&xhSDPToken);
		RfcComponents_SDPPacket_Owner(xhSDPToken, _X("-"), nSessionID, _X("0.0.0.0"));
		RfcComponents_SDPPacket_Session(xhSDPToken, tszSMSAddr);
		RfcComponents_SDPPacket_KeepTime(xhSDPToken);
		//配置视频属性
		RfcComponents_SDPPacket_AddMedia(xhSDPToken, _X("video"), 96);
		RFCCOMPONENTS_SDPPROTOCOL_MEDIAVIDEO st_SDPMediaVideo;
		memset(&st_SDPMediaVideo, '\0', sizeof(RFCCOMPONENTS_SDPPROTOCOL_MEDIAVIDEO));

		st_SDPMediaVideo.nTrackID = 1;
		st_SDPMediaVideo.st_RTPMap.nCodecNumber = 96;
		st_SDPMediaVideo.st_RTPMap.nSampleRate = 90000;
		strcpy(st_SDPMediaVideo.st_RTPMap.tszCodecName, _X("H264"));

		st_SDPMediaVideo.st_FmtpVideo.nFrameRate = st_AVInfo.st_VideoInfo.nFrameRate;
		st_SDPMediaVideo.st_FmtpVideo.nPacketMode = 1;
		st_SDPMediaVideo.st_FmtpVideo.tszLeaveId[0] = tszSPSBuffer[0];
		st_SDPMediaVideo.st_FmtpVideo.tszLeaveId[1] = tszSPSBuffer[1];
		st_SDPMediaVideo.st_FmtpVideo.tszLeaveId[2] = tszSPSBuffer[2];
		st_SDPMediaVideo.st_FmtpVideo.nFrameXSize = st_AVInfo.st_VideoInfo.nWidth;
		st_SDPMediaVideo.st_FmtpVideo.nFrameYSize = st_AVInfo.st_VideoInfo.nHeight;

		OPenSsl_Codec_Base64((LPCXSTR)tszSPSBuffer, st_SDPMediaVideo.st_FmtpVideo.tszSPSBase, &nSPSLen, true);
		OPenSsl_Codec_Base64((LPCXSTR)tszPPSBuffer, st_SDPMediaVideo.st_FmtpVideo.tszPPSBase, &nPPSLen, true);
		RfcComponents_SDPPacket_AttrVideo(xhSDPToken, &st_SDPMediaVideo);
		//配置音频属性
		RfcComponents_SDPPacket_AddMedia(xhSDPToken, _X("audio"), 97);

		RFCCOMPONENTS_SDPPROTOCOL_MEDIAAUDIO st_SDPMediaAudio;
		memset(&st_SDPMediaAudio, '\0', sizeof(RFCCOMPONENTS_SDPPROTOCOL_MEDIAAUDIO));
		st_SDPMediaAudio.nTrackID = 2;
		st_SDPMediaAudio.st_RTPMap.nCodecNumber = 97;
		strcpy(st_SDPMediaAudio.st_RTPMap.tszCodecName, _X("mpeg4-generic"));

		AVHelp_Parse_AACInfo((const XBYTE *)st_AVInfo.st_AudioInfo.tszAInfo, st_AVInfo.st_AudioInfo.nALen, &st_SDPMediaAudio.st_RTPMap.nChannel, &st_SDPMediaAudio.st_RTPMap.nSampleRate, &st_SDPMediaAudio.st_FmtpAudio.nProfileID, &st_SDPMediaAudio.st_FmtpAudio.nConfig);
		st_SDPMediaAudio.st_FmtpAudio.nDeltaLen = 3;
		st_SDPMediaAudio.st_FmtpAudio.nIndexLen = 3;
		st_SDPMediaAudio.st_FmtpAudio.nSizeLen = 13;
		strcpy(st_SDPMediaAudio.st_FmtpAudio.tszMode, "AAC-hbr");
		RfcComponents_SDPPacket_AttrAudio(xhSDPToken, &st_SDPMediaAudio);

		RfcComponents_SDPPacket_GetPacket(xhSDPToken, tszRVBuffer, &nRVLen);
		RfcComponents_SDPPacket_Destory(xhSDPToken);
		RTSPProtocol_CorePacket_Describe(tszSDBuffer, &nSDLen, pSt_HTTPParam->tszHttpUri, tszRVBuffer, st_RTSPRequest.nCseq);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTSP:%s,请求DESCRIBE选项成功,请求的拉流ID:%s"), lpszClientAddr, tszSMSAddr);
	}
	else if (0 == _tcsxnicmp(pSt_HTTPParam->tszHttpMethod, lpszSETUPStr, _tcsxlen(pSt_HTTPParam->tszHttpMethod)))
	{
		//rtsp://127.0.0.1:554/live/qyt/trackID=0 RTSP/1.0 only tcp
		RTSPPROTOCOL_REQUEST st_RTSPRequest;
		RTSPPROTOCOL_SETUP st_RTSPSetup;

		memset(&st_RTSPRequest, '\0', sizeof(RTSPPROTOCOL_REQUEST));
		memset(&st_RTSPSetup, '\0', sizeof(RTSPPROTOCOL_SETUP));

		st_RTSPSetup.nClientRTPPort = 0;
		st_RTSPSetup.nClientRTCPPort = 1;
		RTSPProtocol_CoreParse_Parse(&st_RTSPRequest, lpszMsgBuffer, nMsgLen);

		RTSPProtocol_CorePacket_Setup(tszSDBuffer, &nSDLen, _X(""), &st_RTSPSetup, st_RTSPRequest.nCseq);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTSP:%s,请求SETUP选项成功"), lpszClientAddr);
	}
	else
	{
		ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, NULL, 500, "not support");
		HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("拉流端:%s,请求的类型不支持:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpMethod);
		return false;
	}
	return true;
}