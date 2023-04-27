#pragma once
/********************************************************************
//    Created:     2022/04/24  14:40:09
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_JT1078App\JT1078App_Record.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_JT1078App
//    File Base:   JT1078App_Record
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     录像操作
//    History:
*********************************************************************/
XHTHREAD CALLBACK XEngine_Record_Thread(XPVOID lParam);
bool XEngine_Record_Handle(XSOCKET hSocket, LPCXSTR lpszMsgBuffer, int nMsgLen, LPCXSTR lpszHDRBuffer, int nHDRLen);
bool XEngine_Record_Handle2014(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_RTPPACKETHDR2014* pSt_RTPHdr, XENGINE_RTPPACKETTAIL *pSt_RTPTail);
bool XEngine_Record_Handle2016(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_RTPPACKETHDR2016* pSt_RTPHdr, XENGINE_RTPPACKETTAIL* pSt_RTPTail);