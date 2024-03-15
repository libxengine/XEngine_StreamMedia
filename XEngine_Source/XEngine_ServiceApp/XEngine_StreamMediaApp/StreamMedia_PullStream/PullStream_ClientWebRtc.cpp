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
bool PullStream_ClientStun_Handle(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen)
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

	SDPProtocol_Packet_GetPacket(xhPacket, tszRVBuffer, &nRVLen);
	SDPProtocol_Packet_Destory(xhPacket);

	st_HDRParam.nHttpCode = 201;
	HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
	XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("WEBRTC:%s,WHEP协议拉流请求成功"), lpszClientAddr);
	return true;
}