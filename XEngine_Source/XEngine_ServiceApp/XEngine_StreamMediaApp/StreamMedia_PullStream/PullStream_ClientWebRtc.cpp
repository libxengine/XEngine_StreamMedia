#include "../XEngine_Hdr.h"
/********************************************************************
//    Created:     2024/01/11  10:31:20
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PullStream\PullStream_ClientWebRtc.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PullStream
//    File Base:   PullStream_ClientWebRtc
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     WEBRTC拉流服务
//    History:
*********************************************************************/
bool PullStream_ClientProtocol_Dtls(LPCXSTR lpszMSGBuffer, int nMSGLen)
{
	// DTLS有可能以多种不同的记录层类型开头，这里检查它是否是handshake(0x16)
	return ((nMSGLen >= 13) && (lpszMSGBuffer[0] == 0x16));
}
bool PullStream_ClientProtocol_Stun(LPCXSTR lpszMSGBuffer, int nMSGLen)
{
	// STUN消息的类型字段（前两位为00）以及魔术cookie字段
	return (nMSGLen >= 20) && ((lpszMSGBuffer[0] & 0xC0) == 0x00) && (lpszMSGBuffer[4] == 0x21) && (lpszMSGBuffer[5] == 0x12) && ((XBYTE)lpszMSGBuffer[6] == 0xA4) && (lpszMSGBuffer[7] == 0x42);
}
bool PullStream_ClientProtocol_Handle(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nRVLen = 0;
	int nSDLen = 0;
	XCHAR tszRVBuffer[2048] = {};
	XCHAR tszSDBuffer[2048] = {};

	if (PullStream_ClientProtocol_Dtls(lpszMsgBuffer, nMsgLen))
	{
		nSDLen = 2048;
		bool bConnect = false;

		if (!ModuleSession_PullStream_RTCConnGet(lpszClientAddr, &bConnect))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("RTC客户端:%s,请求的DTLS协议处理失败,地址不存在"), lpszClientAddr);
			return false;
		}
		
		if (bConnect)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("RTC客户端:%s,请求的DTLS协议已经链接成功,但是发送了一段未知协议"), lpszClientAddr);
		}
		else
		{
			bool bRet = OPenSsl_Server_AcceptMemoryEx(xhRTCSsl, hSocket, lpszClientAddr, tszSDBuffer, &nSDLen, lpszMsgBuffer, nMsgLen);
			if (nSDLen > 0)
			{
				XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTC);
			}
			
			if (bRet)
			{
				XBYTE tszKEYBuffer[MAX_PATH] = {};
				OPenSsl_Server_GetKeyEx(xhRTCSsl, lpszClientAddr, tszKEYBuffer);
				ModuleHelp_SRTPCore_Create(tszKEYBuffer);

				XCHAR tszSMSName[128] = {};
				XCHAR tszSMSAddr[128] = {};
				if (!ModuleSession_PullStream_RTCSmsGet(lpszClientAddr, tszSMSName))
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("RTC客户端:%s,握手成功,处理SMS地址失败,诶有找到"), lpszClientAddr);
					return false;
				}
				if (!ModuleSession_PushStream_FindStream(tszSMSName, tszSMSAddr))
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("RTC客户端:%s,握手成功,处理SMS地址失败,诶有找到"), lpszClientAddr);
					return false;
				}
				ModuleSession_PullStream_RTCConnSet(lpszClientAddr, true);
				ModuleSession_PushStream_ClientInsert(tszSMSAddr, lpszClientAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_RTC);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTC客户端:%s,请求的DTLS握手协议处理成功,绑定的地址:%s,绑定的名称:%s"), lpszClientAddr, tszSMSAddr, tszSMSName);
			}
			else
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTC客户端:%s,请求的DTLS握手协议,还需要进一步处理,响应大小:%d"), lpszClientAddr, nSDLen);
			}
		}
	}
	else if (PullStream_ClientProtocol_Stun(lpszMsgBuffer, nMsgLen))
	{
		int nAttrCount = 0;
		RFCCOMPONENTS_NATATTR** ppSt_ListAttr;
		RFCCOMPONENTS_NATSTUN st_NatClient = {};

		if (!NatProtocol_StunNat_Parse(lpszMsgBuffer, nMsgLen, &st_NatClient, &ppSt_ListAttr, &nAttrCount))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("RTC客户端:%s,请求的STUN协议不正确,解析失败,错误:%lX"), lpszClientAddr, NatProtocol_GetLastError());
			return false;
		}
		XCHAR tszUserStr[128] = {};
		for (int i = 0; i < nAttrCount; i++)
		{
			if (RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_USERNAME == ppSt_ListAttr[i]->wAttr)
			{
				memcpy(tszUserStr, ppSt_ListAttr[i]->tszMsgBuffer, ppSt_ListAttr[i]->wLen);
				break;
			}
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListAttr, nAttrCount);

		int nPort = 0;
		XCHAR tszIPPort[128] = {};
		_tcsxcpy(tszIPPort, lpszClientAddr);
		BaseLib_OperatorIPAddr_SegAddr(tszIPPort, &nPort);

		NatProtocol_StunNat_BuildAttr(tszRVBuffer, &nRVLen, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_USERNAME, tszUserStr, _tcsxlen(tszUserStr));
		NatProtocol_StunNat_BuildMapAddress(tszRVBuffer + nRVLen, &nRVLen, tszIPPort, nPort, true);
		nSDLen = nRVLen;
		NatProtocol_StunNat_Packet(tszSDBuffer, &nSDLen, (LPCXSTR)st_NatClient.byTokenStr, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_CLASS_RESPONSE, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_MAPPED_ADDRESS, tszRVBuffer, true, st_ServiceConfig.st_XPull.st_PullWebRtc.tszICEPass, true);
		//更新绑定的地址
		ModuleSession_PullStream_RTCAddrSet(tszUserStr, lpszClientAddr);
		SocketOpt_HeartBeat_ActiveAddrEx(xhRTCHeart, tszUserStr);            //激活一次心跳
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTC);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTC客户端:%s,请求的STUN协议处理成功,请求的用户:%s"), lpszClientAddr, tszUserStr);
	}
	else if (((XBYTE)lpszMsgBuffer[0] >> 6) == 2)
	{
		if (((XBYTE)lpszMsgBuffer[1] >= 200) && ((XBYTE)lpszMsgBuffer[1] <= 207))
		{
			nRVLen = nMsgLen;
			memcpy(tszRVBuffer, lpszMsgBuffer, nMsgLen);

			if (!ModuleHelp_SRTPCore_RTCPUNProtect(tszRVBuffer, &nRVLen))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("RTC客户端:%s,RTCP协议解密失败,大小:%d,错误码:%lX"), lpszClientAddr, nMsgLen, ModuleHelp_GetLastError());
				return false;
			}
			//RTCP
			RTCPPROTOCOL_RTCPHDR st_RTCPHdr = {};
			if (!RTCPProtocol_Parse_Header(tszRVBuffer, nRVLen, &st_RTCPHdr))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("RTC客户端:%s,RTCP协议解析失败,大小:%d,错误码:%lX"), lpszClientAddr, nMsgLen, RTCPProtocol_GetLastError());
				return false;
			}
			int nPos = sizeof(RTCPPROTOCOL_RTCPHDR);

			int nListCount = 0;
			RTCPPROTOCOL_RTCPRECVER** ppSt_ListRecvInfo;
			RTCPProtocol_Parse_Recver(tszRVBuffer + nPos, nRVLen - nPos, &st_RTCPHdr, &ppSt_ListRecvInfo, &nListCount);
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListRecvInfo, nListCount);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTC客户端:%s,请求的RTCP协议处理成功,请求处理的协议:%d"), lpszClientAddr, st_RTCPHdr.byPT);
		}
		else
		{
			//RTP
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTC客户端:%s,请求的RTP协议处理成功"), lpszClientAddr);
		}
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("RTC客户端:%s,发送了不能识别的协议,大小:%d,首位值:%02X"), lpszClientAddr, nMsgLen, (XBYTE)lpszMsgBuffer[0]);
	}

	return true;
}
bool PullStream_ClientWebRtc_SDKPacket(XNETHANDLE xhPacket, LPCXSTR lpszClientID, bool bVideo, XENGINE_PROTOCOL_AVINFO* pSt_AVInfo)
{
	XCHAR** pptszAVList;
	BaseLib_OperatorMemory_Malloc((XPPPMEM)&pptszAVList, 1, MAX_PATH);

	if (bVideo)
	{
		_tcsxcpy(pptszAVList[0], _X("106"));
		SDPProtocol_Packet_AddMedia(xhPacket, _X("video"), _X("UDP/TLS/RTP/SAVPF"), &pptszAVList, 1, 9);
		SDPProtocol_Packet_ClientInet(xhPacket);
	}
	else
	{
		_tcsxcpy(pptszAVList[0], _X("111"));
		SDPProtocol_Packet_AddMedia(xhPacket, _X("audio"), _X("UDP/TLS/RTP/SAVPF"), &pptszAVList, 1, 9);
		SDPProtocol_Packet_ClientInet(xhPacket);
	}
	//生成用户和密码
	SDPProtocol_Packet_ICEUser(xhPacket, st_ServiceConfig.st_XPull.st_PullWebRtc.tszICEUser, st_ServiceConfig.st_XPull.st_PullWebRtc.tszICEPass);

	int nDLen = 0;
	XBYTE tszDigestStr[MAX_PATH] = {};
	XCHAR tszDigestHex[MAX_PATH] = {};
	int nPos = _xstprintf(tszDigestHex, _X("sha-256 "));
	OPenSsl_Api_Digest(st_ServiceConfig.st_XPull.st_PullWebRtc.tszDerStr, tszDigestStr, &nDLen, true, XENGINE_OPENSSL_API_DIGEST_SHA256);
	for (int i = 0; i < nDLen; i++)
	{
		int nRet = _xstprintf(tszDigestHex + nPos, _X("%02X"), tszDigestStr[i]);
		nPos += nRet;
		tszDigestHex[nPos] = ':';
		nPos++;
	}
	tszDigestHex[nPos - 1] = '\0';
	SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("fingerprint"), tszDigestHex);
	SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("setup"), _X("passive"));
	if (bVideo)
	{
		SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("mid"), _X("1"));
		SDPProtocol_Packet_OnlyRWFlag(xhPacket, true);
		SDPProtocol_Packet_RtcpComm(xhPacket, true, true);

		SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("rtpmap"), _X("106 H264/90000"));
		SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("rtcp-fb"), _X("106 transport-cc"));
		SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("rtcp-fb"), _X("106 nack"));
		SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("rtcp-fb"), _X("106 nack pli"));

		int nSPSLen = 0;
		XCHAR tszSPSBuffer[MAX_PATH] = {};
		STREAMMEDIA_SDPPROTOCOL_MEDIAINFO st_SDPMedia = {};

		AVHelp_Parse_VideoHdr(pSt_AVInfo->st_VideoInfo.tszVInfo, pSt_AVInfo->st_VideoInfo.nVLen, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264, NULL, (XBYTE *)tszSPSBuffer, NULL, NULL, NULL, &nSPSLen, NULL, NULL, NULL);

		st_SDPMedia.st_FmtpVideo.nPacketMode = 1;
		st_SDPMedia.st_FmtpVideo.tszLeaveId[0] = tszSPSBuffer[0];
		st_SDPMedia.st_FmtpVideo.tszLeaveId[1] = tszSPSBuffer[1];
		st_SDPMedia.st_FmtpVideo.tszLeaveId[2] = tszSPSBuffer[2];
		SDPProtocol_Packet_VideoFmt(xhPacket, 106, &st_SDPMedia, true);

		XCHAR tszSSrcStr[128] = {};
		BaseLib_OperatorHandle_CreateStr(tszSSrcStr, 8, 1);
		SDPProtocol_Packet_CName(xhPacket, _ttxoll(tszSSrcStr), _X("79a9722580589zr5"), _X("video-666q08to"));
		ModuleSession_PullStream_RTCSSrcSet(lpszClientID, tszSSrcStr, _X("79a9722580589zr5"), _X("video-666q08to"));
		RTPProtocol_Packet_Insert(tszSSrcStr, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_H264);
		RTPProtocol_Packet_SetPType(tszSSrcStr, 106);
	}
	else
	{
		SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("mid"), _X("0"));
		SDPProtocol_Packet_OnlyRWFlag(xhPacket, true);
		SDPProtocol_Packet_RtcpComm(xhPacket, true, true);

		SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("rtpmap"), _X("111 opus/48000/2"));
		SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("rtcp-fb"), _X("111 transport-cc"));

		XCHAR tszSSrcStr[128] = {};
		BaseLib_OperatorHandle_CreateStr(tszSSrcStr, 8, 1);
		SDPProtocol_Packet_CName(xhPacket, _ttxoll(tszSSrcStr), _X("79a9722580589zr5"), _X("audio-23z8fj2g"));
		ModuleSession_PullStream_RTCSSrcSet(lpszClientID, tszSSrcStr, _X("79a9722580589zr5"), _X("audio-23z8fj2g"), false);
		RTPProtocol_Packet_Insert(tszSSrcStr, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_AAC);
		RTPProtocol_Packet_SetPType(tszSSrcStr, 111);
	}
	SDPProtocol_Packet_OptionalCandidate(xhPacket, st_ServiceConfig.tszIPAddr, st_ServiceConfig.nRTCPort);
	BaseLib_OperatorMemory_Free((XPPPMEM)&pptszAVList, 1);
	return true;
}
bool PullStream_ClientWebRtc_Handle(RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nRVLen = 0;
	int nSDLen = 0;
	XNETHANDLE xhParse = 0;
	XNETHANDLE xhPacket = 0;
	XCHAR tszRVBuffer[4096] = {};
	XCHAR tszSDBuffer[4096] = {};
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam = {};

	st_HDRParam.nHttpCode = 200; //HTTP CODE码
	st_HDRParam.bIsClose = true; //收到回复后就关闭

	XCHAR tszSMSAddr[128] = {};
	BaseLib_OperatorString_GetStartEnd(pSt_HTTPParam->tszHttpUri, tszSMSAddr, _X("app="), _X("&"));
	_tcsxcat(tszSMSAddr, _X("/"));
	BaseLib_OperatorString_GetStartEnd(pSt_HTTPParam->tszHttpUri, tszSMSAddr + _tcsxlen(tszSMSAddr), _X("stream="), NULL);
	//查找流是否存在
	XCHAR tszPushAddr[128] = {};
	XENGINE_PROTOCOL_AVINFO st_AVInfo = {};
	if (!ModuleSession_PushStream_FindStream(tszSMSAddr, tszPushAddr))
	{
		ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, NULL, 404, "stream name not found");
		HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("WEBRTC:%s,请求拉流的地址:%s 不正确,没有找到,错误:%lX"), lpszClientAddr, tszSMSAddr, ModuleSession_GetLastError());
		return false;
	}
	ModuleSession_PushStream_GetAVInfo(tszPushAddr, &st_AVInfo);
	//解析SDP
	if (!SDPProtocol_Parse_Create(&xhParse, lpszMsgBuffer, nMsgLen))
	{
		ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, NULL, 400, "sdp is incorrent");
		HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("WEBRTC:%s,请求拉流的SDP不正确,错误:%lX"), lpszClientAddr, SDPProtocol_GetLastError());
		return false;
	}

	bool bAudio = false;
	bool bVideo = false;
	bool bRTCPMux = false;
	int nListCount = 0;
	XCHAR tszICEUser[MAX_PATH] = {};
	XCHAR tszICEPass[MAX_PATH] = {};
	XCHAR tszAlgType[MAX_PATH] = {};
	XCHAR tszHMacStr[MAX_PATH] = {};
	STREAMMEDIA_SDPPROTOCOL_ATTR** ppSt_ListAttr;
	SDPProtocol_Parse_GetAttr(xhParse, &ppSt_ListAttr, &nListCount);

	SDPProtocol_Parse_AttrBundle(&ppSt_ListAttr, nListCount, &bAudio, &bVideo, &bRTCPMux);
	SDPProtocol_Parse_AttrICEUser(&ppSt_ListAttr, nListCount, tszICEUser, tszICEPass);
	SDPProtocol_Parse_AttrFinger(&ppSt_ListAttr, nListCount, tszAlgType, tszHMacStr);
	SDPProtocol_Parse_Destory(xhParse);
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListAttr, nListCount);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("WEBRTC:%s,请求的SDP信息属性解析完毕,总共解析了:%d 个属性"), lpszClientAddr, nListCount);

	SDPProtocol_Packet_Create(&xhPacket);
	SDPProtocol_Packet_Owner(xhPacket, _X("rtc"), xhPacket, _X("0.0.0.0"));
	SDPProtocol_Packet_Session(xhPacket, _X("XEngine_Session"));
	SDPProtocol_Packet_KeepTime(xhPacket);
	SDPProtocol_Packet_Bundle(xhPacket);
	SDPProtocol_Packet_OptionalRange(xhPacket);
	SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("ice-lite"));
	SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("msid-semantic"), _X("WMS live/livestream"));

	XCHAR tszTokenStr[MAX_PATH] = {};
	XCHAR tszHDRStr[MAX_PATH] = {};
	XCHAR tszUserStr[MAX_PATH] = {};

	BaseLib_OperatorHandle_CreateStr(tszTokenStr, 10);
	_xstprintf(tszUserStr, _X("%s:%s"), st_ServiceConfig.st_XPull.st_PullWebRtc.tszICEUser, tszICEUser);
	_xstprintf(tszHDRStr, _X("Location: /rtc/v1/whip/?action=delete&token=%s&app=live&stream=livestream.flv&session=%s\r\n"), tszTokenStr, tszUserStr);

	ModuleSession_PullStream_Insert(tszUserStr, tszSMSAddr, tszPushAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_RTC);
	ModuleSession_PullStream_RTCSet(tszUserStr, tszTokenStr, tszICEUser, tszICEPass, tszHMacStr);
	SocketOpt_HeartBeat_InsertAddrEx(xhRTCHeart, tszUserStr);     //需要加入心跳,不然没法知道超时

	PullStream_ClientWebRtc_SDKPacket(xhPacket, tszUserStr, false, &st_AVInfo);
	PullStream_ClientWebRtc_SDKPacket(xhPacket, tszUserStr, true, &st_AVInfo);
	
	SDPProtocol_Packet_GetPacket(xhPacket, tszRVBuffer, &nRVLen);
	SDPProtocol_Packet_Destory(xhPacket);

	st_HDRParam.nHttpCode = 201;
	HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen, tszHDRStr);
	XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("WEBRTC:%s,WHEP协议拉流请求成功"), lpszClientAddr);
	return true;
}