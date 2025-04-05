#pragma once
/********************************************************************
//    Created:     2023/07/02  18:00:02
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PushStream\PushStream_RTMPTask.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PushStream
//    File Base:   PushStream_RTMPTask
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     RTMP流处理协议
//    History:
*********************************************************************/
//任务处理池,用来获取一个完整包
XHTHREAD CALLBACK PushStream_RTMPTask_Thread(XPVOID lParam);
//预处理封包
bool PushStream_RTMPTask_Play(LPCXSTR lpszClientAddr, LPCXSTR lpszPushAddr, XCHAR* ptszSDBuffer);
//任务处理相关函数,处理包的内容
bool PushStream_RTMPTask_Handle(XENGINE_RTMPHDR* pSt_RTMPHdr, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen);