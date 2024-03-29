﻿#include "../XEngine_Hdr.h"
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
int PullStream_ClientProtocol_Dtls(LPCXSTR lpszMSGBuffer, int nMSGLen)
{
	// DTLS有可能以多种不同的记录层类型开头，这里检查它是否是handshake(0x16)
	return nMSGLen >= 13 && lpszMSGBuffer[0] == 0x16;
}
int PullStream_ClientProtocol_Stun(LPCXSTR lpszMSGBuffer, int nMSGLen)
{
	// STUN消息的类型字段（前两位为00）以及魔术cookie字段
	return nMSGLen >= 20 && (lpszMSGBuffer[0] & 0xC0) == 0x00 && lpszMSGBuffer[4] == 0x21 && lpszMSGBuffer[5] == 0x12 && lpszMSGBuffer[6] == 0xA4 && lpszMSGBuffer[7] == 0x42;
}
bool PullStream_ClientProtocol_Handle(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	if (PullStream_ClientProtocol_Dtls(lpszMsgBuffer, nMsgLen))
	{
		int nSDLen = 2048;
		XCHAR tszSDBuffer[2048] = {};
		XBYTE tszSDKey[128] = {};
		XBYTE tszRVKey[128] = {};

		if (OPenSsl_Server_AcceptMemoryEx(xhRTCSsl, hSocket, lpszClientAddr, tszSDBuffer, &nSDLen, lpszMsgBuffer, nMsgLen))
		{
			OPenSsl_Server_GetKeyEx(xhRTCSsl, lpszClientAddr, tszSDKey, tszRVKey);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTC客户端:%s,请求的DTLS握手协议处理成功"), lpszClientAddr);
		}
		else
		{
			int nPort = 0;
			XCHAR tszIPPort[128] = {};
			_tcsxcpy(tszIPPort, lpszClientAddr);
			BaseLib_OperatorIPAddr_SegAddr(tszIPPort, &nPort);
			NetCore_UDPSelect_Send(xhRTCSocket, tszSDBuffer, nSDLen, tszIPPort, nPort);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTC客户端:%s,请求的DTLS握手协议,还需要进一步处理,响应大小:%d"), lpszClientAddr, nSDLen);
		}
	}
	else if (PullStream_ClientProtocol_Stun(lpszMsgBuffer, nMsgLen))
	{
		int nAttrCount = 0;
		RFCCOMPONENTS_NATATTR** ppSt_ListAttr;
		RFCCOMPONENTS_NATSTUN st_NatClient = {};

		if (!NatProtocol_StunNat_Parse(lpszMsgBuffer, nMsgLen, &st_NatClient, &ppSt_ListAttr, &nAttrCount))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("STUN客户端:%s,请求的STUN协议不正确,解析失败,错误:%lX"), lpszClientAddr, NatProtocol_GetLastError());
			return false;
		}
		XCHAR tszUserStr[128] = {};
		for (int i = 0; i < nAttrCount; i++)
		{
			if (RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_USERNAME == ppSt_ListAttr[i]->wAttr)
			{
				memcpy(tszUserStr, ppSt_ListAttr[i]->tszMsgBuffer, ppSt_ListAttr[i]->wLen);
			}
		}
		int nTMPLen = 0;
		int nMSGLen = 0;
		int nIPPort = 0;
		XCHAR tszTMPBuffer[1024] = {};
		XCHAR tszMSGBuffer[1024] = {};
		XCHAR tszIPAddr[128] = {};

		_tcsxcpy(tszIPAddr, lpszClientAddr);

		BaseLib_OperatorIPAddr_SegAddr(tszIPAddr, &nIPPort);

		NatProtocol_StunNat_BuildAttr(tszTMPBuffer, &nTMPLen, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_USERNAME, tszUserStr, _tcsxlen(tszUserStr));
		NatProtocol_StunNat_BuildMapAddress(tszTMPBuffer + nTMPLen, &nTMPLen, tszIPAddr, nIPPort, true);
		//NatProtocol_StunNat_BuildMSGIntegrity(tszMSGBuffer, &nMSGLen, tszTMPBuffer, nTMPLen, );
		NatProtocol_StunNat_Packet(tszMSGBuffer, &nMSGLen, (LPCXSTR)st_NatClient.byTokenStr, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_CLASS_FLAGS, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_MAPPED_ADDRESS);

		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListAttr, nAttrCount);
	}
	else 
	{
		
	}

	
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

	if (!SDPProtocol_Parse_Create(&xhParse, lpszMsgBuffer, nMsgLen))
	{
		ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, NULL, 400, "sdp is incorrent");
		HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("WEBRTC:%s,请求拉流的SDP不正确,错误:%lX"), lpszClientAddr, SDPProtocol_GetLastError());
		return false;
	}

	bool bBundle = false;
	bool bRTCPMux = false;
	int nListCount = 0;
	XCHAR tszICEUser[MAX_PATH] = {};
	XCHAR tszICEPass[MAX_PATH] = {};
	XCHAR tszHMacStr[MAX_PATH] = {};
	STREAMMEDIA_SDPPROTOCOL_ATTR** ppSt_ListAttr;
	SDPProtocol_Parse_GetAttr(xhParse, &ppSt_ListAttr, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		LPCXSTR lpszAttrGroup = _X("group");
		LPCXSTR lpszAttrMux = _X("rtcp-mux");
		LPCXSTR lpszICEUfrag = _X("ice-ufrag");
		LPCXSTR lpszICEPwd = _X("ice-pwd");
		LPCXSTR lpszFinger = _X("fingerprint");
		if (0 == _tcsxnicmp(lpszAttrGroup, ppSt_ListAttr[i]->tszAttrKey, _tcsxlen(lpszAttrGroup)))
		{
			LPCXSTR lpszBundleStr = _X("BUNDLE");
			if (0 == _tcsxnicmp(lpszBundleStr, ppSt_ListAttr[i]->tszAttrValue, _tcsxlen(lpszBundleStr)))
			{
				//是否启用了端口一致绑定
				bBundle = true;
			}
		}
		else if (0 == _tcsxnicmp(lpszAttrMux, ppSt_ListAttr[i]->tszAttrKey, _tcsxlen(lpszAttrMux)))
		{
			bRTCPMux = true;  //复用端口检查
		}
		else if (0 == _tcsxnicmp(lpszICEUfrag, ppSt_ListAttr[i]->tszAttrKey, _tcsxlen(lpszICEUfrag)))
		{
			_tcsxcpy(tszICEUser, ppSt_ListAttr[i]->tszAttrValue);
		}
		else if (0 == _tcsxnicmp(lpszICEPwd, ppSt_ListAttr[i]->tszAttrKey, _tcsxlen(lpszICEPwd)))
		{
			_tcsxcpy(tszICEPass, ppSt_ListAttr[i]->tszAttrValue);
		}
		else if (0 == _tcsxnicmp(lpszFinger, ppSt_ListAttr[i]->tszAttrKey, _tcsxlen(lpszFinger)))
		{
			XCHAR tszKeyStr[MAX_PATH] = {};
			BaseLib_OperatorString_GetKeyValue(ppSt_ListAttr[i]->tszAttrValue, _X(" "), tszKeyStr, tszHMacStr);
		}
	}
	SDPProtocol_Parse_Destory(xhParse);
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListAttr, nListCount);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("WEBRTC:%s,请求的SDP信息属性解析完毕,总共解析了:%d 个属性"), lpszClientAddr, nListCount);

	SDPProtocol_Packet_Create(&xhPacket);
	SDPProtocol_Packet_Owner(xhPacket, _X("rtc"), xhPacket, _X("0.0.0.0"));
	SDPProtocol_Packet_Session(xhPacket, _X("XEngine_Session"));
	SDPProtocol_Packet_OptionalRange(xhPacket);

	XCHAR** pptszAVList;
	BaseLib_OperatorMemory_Malloc((XPPPMEM)&pptszAVList, 1, MAX_PATH);
	_tcsxcpy(pptszAVList[0], _X("111"));

	SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("ice-lite"));
	SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("group"), _X("BUNDLE 0 1"));
	SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("msid-semantic"), _X("WMS live/livestream"));
	SDPProtocol_Packet_AddMedia(xhPacket, _X("audio"), _X("UDP/TLS/RTP/SAVPF"), &pptszAVList, 1);

	SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("ice-ufrag"), "j107le40");
	SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("ice-pwd"), "3321308h8i6vt3769r6638l1409d50jz");

	int nDLen = 0;
	XBYTE tszDigestStr[MAX_PATH] = {};
	XCHAR tszDigestHex[MAX_PATH] = {};
	int nPos = _xstprintf(tszDigestHex, _X("sha-256 "));
	OPenSsl_Api_Digest(st_ServiceConfig.st_XPull.st_PullWebRtc.tszCsrStr, tszDigestStr, &nDLen, true, XENGINE_OPENSSL_API_DIGEST_SHA256);
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
	SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("mid"), _X("0"));
	SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("extmap"), _X("3 http://www.ietf.org/id/draft-holmer-rmcat-transport-wide-cc-extensions-01"));
	SDPProtocol_Packet_OnlyRWFlag(xhPacket, true);
	SDPProtocol_Packet_RtcpComm(xhPacket, true, true);
	SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("rtpmap"), _X("111 opus/48000/2"));
	SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("rtcp-fb"), _X("111 transport-cc"));
	SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("ssrc"), _X("2124085006 cname:79a9722580589zr5"));
	SDPProtocol_Packet_OptionalAddAttr(xhPacket, _X("ssrc"), _X("2124085006 label:audio-23z8fj2g"));
	
	SDPProtocol_Packet_GetPacket(xhPacket, tszRVBuffer, &nRVLen);
	SDPProtocol_Packet_Destory(xhPacket);
	BaseLib_OperatorMemory_Free((XPPPMEM)&pptszAVList, 1);

	st_HDRParam.nHttpCode = 201;
	HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
	XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("WEBRTC:%s,WHEP协议拉流请求成功"), lpszClientAddr);
	return true;
}