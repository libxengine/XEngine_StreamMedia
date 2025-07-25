#pragma once
/********************************************************************
//    Created:     2025/07/25  10:24:09
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PushStream\PushStream_ClientWebRtc.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PushStream
//    File Base:   PushStream_ClientWebRtc
//    File Ext:    h
//    Project:     XEngine
//    Author:      qyt
//    Purpose:     WEBRTC推流服务
//    History:
*********************************************************************/
bool PushStream_ClientProtocol_Handle(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszMsgBuffer, int nMsgLen);
bool PushStream_ClientWebRtc_SDKPacket(XNETHANDLE xhPacket, LPCXSTR lpszClientID, bool bVideo, int nAVIndex, STREAMMEDIA_SDPPROTOCOL_MEDIAINFO* pSt_SDPMediaInfo);
bool PushStream_ClientWhip_Handle(RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen);