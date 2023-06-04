#pragma once
/********************************************************************
//    Created:     2023/06/04  12:56:13
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PushStream\PushStream_JT1078Task.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PushStream
//    File Base:   PushStream_JT1078Task
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     JT1078流处理器
//    History:
*********************************************************************/
XHTHREAD CALLBACK PushStream_JT1078Task_Thread(XPVOID lParam);
bool PushStream_JT1078Task_Handle(XSOCKET hSocket, LPCXSTR lpszMsgBuffer, int nMsgLen, LPCXSTR lpszHDRBuffer, int nHDRLen);
bool PushStream_JT1078Task_Handle2014(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_RTPPACKETHDR2014* pSt_RTPHdr, XENGINE_RTPPACKETTAIL* pSt_RTPTail);
bool PushStream_JT1078Task_Handle2016(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_RTPPACKETHDR2016* pSt_RTPHdr, XENGINE_RTPPACKETTAIL* pSt_RTPTail);