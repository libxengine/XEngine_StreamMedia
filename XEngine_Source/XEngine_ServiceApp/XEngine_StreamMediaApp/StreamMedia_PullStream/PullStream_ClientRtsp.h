#pragma once
/********************************************************************
//    Created:     2023/12/20  14:59:27
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PullStream\PullStream_ClientRtsp.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PullStream
//    File Base:   PullStream_ClientRtsp
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     RTSP推流服务
//    History:
*********************************************************************/
//任务处理相关函数,处理包的内容
bool PullStream_ClientRtsp_RTCPProcess(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszMsgBuffer, int nMsgLen);
bool PullStream_ClientRtsp_Handle(RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR*** ppptszParamList, int nParamCount, XCHAR*** ppptszHDRList, int nHDRList);