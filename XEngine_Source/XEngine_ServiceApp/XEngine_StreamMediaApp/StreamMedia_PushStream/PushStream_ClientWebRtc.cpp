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
bool PushStream_ClientProtocol_Dtls(LPCXSTR lpszMSGBuffer, int nMSGLen)
{
	// DTLS有可能以多种不同的记录层类型开头，这里检查它是否是handshake(0x16)
	return ((nMSGLen >= 13) && (lpszMSGBuffer[0] == 0x16));
}
bool PushStream_ClientProtocol_Stun(LPCXSTR lpszMSGBuffer, int nMSGLen)
{
	// STUN消息的类型字段（前两位为00）以及魔术cookie字段
	return (nMSGLen >= 20) && ((lpszMSGBuffer[0] & 0xC0) == 0x00) && (lpszMSGBuffer[4] == 0x21) && (lpszMSGBuffer[5] == 0x12) && ((XBYTE)lpszMSGBuffer[6] == 0xA4) && (lpszMSGBuffer[7] == 0x42);
}
bool PushStream_ClientProtocol_Handle(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nRVLen = 0;
	int nSDLen = 0;
	XCHAR tszRVBuffer[2048] = {};
	XCHAR tszSDBuffer[2048] = {};

	if (PushStream_ClientProtocol_Dtls(lpszMsgBuffer, nMsgLen))
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
			bool bRet = Cryption_Server_AcceptMemoryEx(xhRTCWhipSsl, hSocket, lpszClientAddr, tszSDBuffer, &nSDLen, lpszMsgBuffer, nMsgLen);
			if (nSDLen > 0)
			{
				XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTC);
			}
			
			if (bRet)
			{
				XBYTE tszKEYBuffer[XPATH_MAX] = {};
				Cryption_Server_GetKeyEx(xhRTCWhipSsl, lpszClientAddr, tszKEYBuffer);
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
	else if (PushStream_ClientProtocol_Stun(lpszMsgBuffer, nMsgLen))
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
		BaseLib_Memory_Free((XPPPMEM)&ppSt_ListAttr, nAttrCount);

		int nPort = 0;
		XCHAR tszIPPort[128] = {};
		_tcsxcpy(tszIPPort, lpszClientAddr);
		APIAddr_IPAddr_SegAddr(tszIPPort, &nPort);

		NatProtocol_StunNat_BuildAttr(tszRVBuffer, &nRVLen, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_USERNAME, tszUserStr, _tcsxlen(tszUserStr));
		NatProtocol_StunNat_BuildMapAddress(tszRVBuffer + nRVLen, &nRVLen, tszIPPort, nPort, true);
		nSDLen = nRVLen;
		NatProtocol_StunNat_Packet(tszSDBuffer, &nSDLen, (LPCXSTR)st_NatClient.byTokenStr, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_CLASS_RESPONSE, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_MAPPED_ADDRESS, tszRVBuffer, true, st_ServiceConfig.st_XPull.st_PullWebRtc.tszICEPass, true);
		//更新绑定的地址
		ModuleSession_PullStream_RTCAddrSet(tszUserStr, lpszClientAddr);
		SocketOpt_HeartBeat_ActiveAddrEx(xhRTCWhipHeart, tszUserStr);            //激活一次心跳
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
			BaseLib_Memory_Free((XPPPMEM)&ppSt_ListRecvInfo, nListCount);
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
bool PushStream_ClientWebRtc_SDKPacket(XNETHANDLE xhPacket, LPCXSTR lpszClientID, bool bVideo, int nAVIndex, STREAMMEDIA_SDPPROTOCOL_MEDIAINFO* pSt_SDPMediaInfo)
{
	XCHAR** pptszAVList;
	BaseLib_Memory_Malloc((XPPPMEM)&pptszAVList, 1, XPATH_MAX);

	if (bVideo)
	{
		_xstprintf(pptszAVList[0], "%d", nAVIndex);
		SDPProtocol_Packet_AddMedia(xhPacket, _X("video"), _X("UDP/TLS/RTP/SAVPF"), &pptszAVList, 1, 1, 9);
		SDPProtocol_Packet_ClientInet(xhPacket);
	}
	else
	{
		_xstprintf(pptszAVList[0], "%d", nAVIndex);
		SDPProtocol_Packet_AddMedia(xhPacket, _X("audio"), _X("UDP/TLS/RTP/SAVPF"), &pptszAVList, 1, 0, 9);
		SDPProtocol_Packet_ClientInet(xhPacket);
	}
	//生成用户和密码
	SDPProtocol_Packet_ICEUser(xhPacket, st_ServiceConfig.st_XPull.st_PullWebRtc.tszICEUser, st_ServiceConfig.st_XPull.st_PullWebRtc.tszICEPass);

	int nDLen = 0;
	XBYTE tszDigestStr[XPATH_MAX] = {};
	XCHAR tszDigestHex[XPATH_MAX] = {};
	int nPos = _xstprintf(tszDigestHex, _X("sha-256 "));
	Cryption_Api_Digest(st_ServiceConfig.st_XPull.st_PullWebRtc.tszDerStr, tszDigestStr, &nDLen, true, ENUM_XENGINE_CRYPTION_DIGEST_SHA256);
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
		SDPProtocol_Packet_OnlyRWFlag(xhPacket);
		SDPProtocol_Packet_RtcpComm(xhPacket, true, true);
		SDPProtocol_Packet_VideoFmt(xhPacket, nAVIndex, pSt_SDPMediaInfo, true);
	}
	else
	{
		SDPProtocol_Packet_OnlyRWFlag(xhPacket);
		SDPProtocol_Packet_RtcpComm(xhPacket, true, true);
		SDPProtocol_Packet_AudioFmt(xhPacket, nAVIndex, pSt_SDPMediaInfo, true);
	}
	SDPProtocol_Packet_OptionalCandidate(xhPacket, st_ServiceConfig.tszIPAddr, st_ServiceConfig.nRTCWhipPort);
	BaseLib_Memory_Free((XPPPMEM)&pptszAVList, 1);
	return true;
}
bool PushStream_ClientWhip_Handle(RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nRVLen = 0;
	int nSDLen = 0;
	XNETHANDLE xhParse = 0;
	XNETHANDLE xhPacket = 0;
	XCHAR tszRVBuffer[4096] = {};
	XCHAR tszSDBuffer[4096] = {};
	XENGINE_PROTOCOL_AVINFO st_AVInfo = {};
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam = {};

	st_HDRParam.nHttpCode = 200; //HTTP CODE码
	st_HDRParam.bIsClose = true; //收到回复后就关闭

	XCHAR tszSMSAddr[128] = {};
	BaseLib_String_GetStartEnd(pSt_HTTPParam->tszHttpUri, tszSMSAddr, _X("app="), _X("&"));
	_tcsxcat(tszSMSAddr, _X("/"));
	BaseLib_String_GetStartEnd(pSt_HTTPParam->tszHttpUri, tszSMSAddr + _tcsxlen(tszSMSAddr), _X("stream="), NULL);
	//查找流是否存在
	XCHAR tszPushAddr[128] = {};
	if (ModuleSession_PushStream_FindStream(tszSMSAddr, tszPushAddr))
	{
		ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, NULL, 400, "stream is published");
		HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("WEBRTC:%s,请求推流的地址:%s 不正确,此流已经在推送"), lpszClientAddr, tszSMSAddr);
		return false;
	}
	//解析SDP
	if (!SDPProtocol_Parse_Create(&xhParse, lpszMsgBuffer, nMsgLen))
	{
		ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, NULL, 400, "sdp is incorrent");
		HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("WEBRTC:%s,请求拉流的SDP不正确,错误:%lX"), lpszClientAddr, SDPProtocol_GetLastError());
		return false;
	}

	int nIndex1 = -1;
	int nIndex2 = -1;
	int nListCount = 0;
	XCHAR tszICEUser[XPATH_MAX] = {};
	XCHAR tszICEPass[XPATH_MAX] = {};
	XCHAR tszAlgType[XPATH_MAX] = {};
	XCHAR tszHMacStr[XPATH_MAX] = {};
	STREAMMEDIA_SDPPROTOCOL_ATTR** ppSt_ListAttr;
	SDPProtocol_Parse_GetAttr(xhParse, &ppSt_ListAttr, &nListCount);

	SDPProtocol_Parse_AttrBundle(&ppSt_ListAttr, nListCount, &nIndex1, &nIndex2);
	SDPProtocol_Parse_AttrICEUser(&ppSt_ListAttr, nListCount, tszICEUser, tszICEPass);
	SDPProtocol_Parse_AttrFinger(&ppSt_ListAttr, nListCount, tszAlgType, tszHMacStr);
	//查找合适的视频和音频流索引信息
	STREAMMEDIA_SDPPROTOCOL_MEDIAINFO st_SDPAudioInfo = {};
	STREAMMEDIA_SDPPROTOCOL_MEDIAINFO st_SDPVideoInfo = {};

	int nVideoIndex = 0;
	int nAudioIndex = 0;
	int nAVCount = 0;
	STREAMMEDIA_SDPPROTOCOL_AVMEDIA** ppSt_AVMedia;
	SDPProtocol_Parse_GetAVMedia(xhParse, &ppSt_AVMedia, &nAVCount);
	for (int i = 0; i < nAVCount; i++)
	{
		LPCXSTR lpszAudioStr = _X("audio");
		LPCXSTR lpszVideoStr = _X("video");
		if (0 == _tcsxnicmp(lpszAudioStr, ppSt_AVMedia[i]->tszAVType, _tcsxlen(lpszAudioStr)))
		{
			//查找列表
			for (int j = 0; j < ppSt_AVMedia[i]->nListCount; j++)
			{
				STREAMMEDIA_SDPPROTOCOL_MEDIAINFO st_SDPMeida = {};
				SDPProtocol_Parse_RTPMapAudio(&ppSt_ListAttr, nListCount, _ttxoi(ppSt_AVMedia[i]->pptszAVList[j]), &st_SDPMeida);
				//推流一般只有一个
				if ((2 == st_SDPMeida.st_RTPMap.nChannel) && (0 == _tcsxnicmp(st_SDPMeida.st_RTPMap.tszCodecName, "opus", 4)))
				{
					nAudioIndex = _ttxoi(ppSt_AVMedia[i]->pptszAVList[j]);
					st_SDPAudioInfo = st_SDPMeida;

					st_AVInfo.st_AudioInfo.bEnable = true;
					st_AVInfo.st_AudioInfo.nChannel = st_SDPMeida.st_RTPMap.nChannel;
					st_AVInfo.st_AudioInfo.nSampleRate = st_SDPMeida.st_RTPMap.nSampleRate;
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("WEBRTC:%s,找到了合适的音频信息索引:%s"), lpszClientAddr, ppSt_AVMedia[i]->pptszAVList[j]);
					break;
				}
			}
		}
		else if (0 == _tcsxnicmp(lpszVideoStr, ppSt_AVMedia[i]->tszAVType, _tcsxlen(lpszVideoStr)))
		{
			//查找列表
			for (int j = 0; j < ppSt_AVMedia[i]->nListCount; j++)
			{
				STREAMMEDIA_SDPPROTOCOL_MEDIAINFO st_SDPMeida = {};
				SDPProtocol_Parse_RTPMapVideo(&ppSt_ListAttr, nListCount, _ttxoi(ppSt_AVMedia[i]->pptszAVList[j]), &st_SDPMeida);
				
				if ((1 == st_SDPMeida.st_FmtpVideo.nPacketMode) && (0x42 == st_SDPMeida.st_FmtpVideo.tszLeaveId[0]) && (0xe0 == st_SDPMeida.st_FmtpVideo.tszLeaveId[1]) && (0x1f == st_SDPMeida.st_FmtpVideo.tszLeaveId[2]) && (0 == _tcsxnicmp(st_SDPMeida.st_RTPMap.tszCodecName, "H264", 4)))
				{
					nVideoIndex = _ttxoi(ppSt_AVMedia[i]->pptszAVList[j]);
					st_SDPVideoInfo = st_SDPMeida;

					st_AVInfo.st_VideoInfo.bEnable = true;
					st_AVInfo.st_VideoInfo.nFrameNumber = st_SDPMeida.st_FmtpVideo.nFrameRate;
					st_AVInfo.st_VideoInfo.nBitRate = st_SDPMeida.st_RTPMap.nSampleRate;
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("WEBRTC:%s,找到了合适的视频信息索引:%s,帧率:%d"), lpszClientAddr, ppSt_AVMedia[i]->pptszAVList[j], st_AVInfo.st_VideoInfo.nFrameRate);
				}
			}
		}
	}

	SDPProtocol_Parse_Destory(xhParse);
	BaseLib_Memory_Free((XPPPMEM)&ppSt_ListAttr, nListCount);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("WEBRTC:%s,请求的SDP信息属性解析完毕,总共解析了:%d 个属性"), lpszClientAddr, nListCount);

	SDPProtocol_Packet_Create(&xhPacket);
	SDPProtocol_Packet_Owner(xhPacket, _X("rtc"), xhPacket, _X("0.0.0.0"));
	SDPProtocol_Packet_Session(xhPacket, _X("XEngine_Session"));
	SDPProtocol_Packet_KeepTime(xhPacket);
	if (nIndex1 >= 0 && nIndex2 >= 0)
	{
		SDPProtocol_Packet_Bundle(xhPacket);
	}
	else
	{
		SDPProtocol_Packet_Bundle(xhPacket, 0, -1);
	}
	SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("ice-lite"));
	SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("msid-semantic"), _X("WMS *"));

	XCHAR tszTokenStr[XPATH_MAX] = {};
	XCHAR tszHDRStr[XPATH_MAX] = {};
	XCHAR tszUserStr[XPATH_MAX] = {};

	BaseLib_Handle_CreateStr(tszTokenStr, 10);
	_xstprintf(tszUserStr, _X("%s:%s"), st_ServiceConfig.st_XPull.st_PullWebRtc.tszICEUser, tszICEUser);
	_xstprintf(tszHDRStr, _X("Location: /rtc/v1/whip/?action=delete&token=%s&app=live&stream=livestream.flv&session=%s\r\n"), tszTokenStr, tszUserStr);

	if (nIndex1 >= 0 && nIndex2 >= 0)
	{
		PushStream_ClientWebRtc_SDKPacket(xhPacket, tszUserStr, false, nAudioIndex, &st_SDPAudioInfo);
		PushStream_ClientWebRtc_SDKPacket(xhPacket, tszUserStr, true, nVideoIndex, &st_SDPVideoInfo);
	}
	else
	{
		PushStream_ClientWebRtc_SDKPacket(xhPacket, tszUserStr, true, nVideoIndex, &st_SDPVideoInfo);
	}

	SDPProtocol_Packet_GetPacket(xhPacket, tszRVBuffer, &nRVLen);
	SDPProtocol_Packet_Destory(xhPacket);

	ModuleSession_PushStream_Create(tszUserStr, tszSMSAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_RTC);
	ModuleSession_PushStream_SetAVInfo(tszUserStr, &st_AVInfo);
	ModuleSession_PullStream_RTCSet(tszUserStr, tszTokenStr, tszICEUser, tszICEPass, tszHMacStr);
	SocketOpt_HeartBeat_InsertAddrEx(xhRTCWhipHeart, tszUserStr);     //需要加入心跳,不然没法知道超时

	st_HDRParam.nHttpCode = 201;
	_xstprintf(st_HDRParam.tszMimeType, _X("sdp"));
	HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen, tszHDRStr);
	XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("WEBRTC:%s,Whip协议推流请求成功"), lpszClientAddr);
	return true;
}