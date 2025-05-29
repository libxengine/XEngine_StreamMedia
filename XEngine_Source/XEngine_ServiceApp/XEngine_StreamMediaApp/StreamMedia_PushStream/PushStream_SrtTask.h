#pragma once
/********************************************************************
//    Created:     2023/07/28  16:11:23
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PushStream\PushStream_SrtTask.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PushStream
//    File Base:   PushStream_SrtTask
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     SRT推拉流代码
//    History:
*********************************************************************/
bool PushStream_SrtTask_Connct(LPCXSTR lpszClientAddr, XSOCKET hSocket);
//任务处理相关函数,处理包的内容
bool PushStream_SrtTask_Handle(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszMsgBuffer, int nMsgLen);

XHTHREAD XCALLBACK PushStream_SRTTask_Thread(XPVOID lParam);
bool PushStream_SrtTask_ThreadProcess(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XBYTE byAVType);