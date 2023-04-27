#pragma once
/********************************************************************
//    Created:     2022/04/24  15:35:06
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_JT1078App\JT1078App_Stream.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_JT1078App
//    File Base:   JT1078App_Stream
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     直播流
//    History:
*********************************************************************/
XHTHREAD CALLBACK XEngine_Stream_Thread(XPVOID lParam);
bool XEngine_Stream_Handle(XSOCKET hSocket, LPCXSTR lpszMsgBuffer, int nMsgLen, LPCXSTR lpszHDRBuffer, int nHDRLen);
bool XEngine_Stream_Handle2014(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_RTPPACKETHDR2014* pSt_RTPHdr, XENGINE_RTPPACKETTAIL* pSt_RTPTail);
bool XEngine_Stream_Handle2016(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_RTPPACKETHDR2016* pSt_RTPHdr, XENGINE_RTPPACKETTAIL* pSt_RTPTail);