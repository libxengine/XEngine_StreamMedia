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
	STREAMMEDIA_SDPPROTOCOL_ATTR** ppSt_ListAttr;
	SDPProtocol_Parse_GetAttr(xhParse, &ppSt_ListAttr, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		LPCXSTR lpszAttrGroup = _X("group");
		LPCXSTR lpszAttrMux = _X("rtcp-mux");
		if (0 == _tcsxnicmp(lpszAttrGroup, ppSt_ListAttr[i]->tszAttrKey, _tcsxlen(lpszAttrGroup)))
		{
			LPCXSTR lpszBundleStr = _X("BUNDLE");
			if (0 == _tcsxnicmp(lpszBundleStr, ppSt_ListAttr[i]->tszAttrValue, _tcsxlen(lpszBundleStr)))
			{
				//是否启用了端口一致绑定
				bBundle = true;
				break;
			}
		}
		else if (0 == _tcsxnicmp(lpszAttrMux, ppSt_ListAttr[i]->tszAttrKey, _tcsxlen(lpszAttrMux)))
		{
			bRTCPMux = true;  //复用端口检查
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("SDPProtocol_Parse_GetAttr:%s-%s"), ppSt_ListAttr[i]->tszAttrKey, ppSt_ListAttr[i]->tszAttrValue);
	}

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

	ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen);
	HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
	XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
	SDPProtocol_Parse_Destory(xhParse);
	SDPProtocol_Packet_Destory(xhPacket);
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("WEBRTC:%s,WHIP协议拉流请求成功"), lpszClientAddr);
	return true;
}