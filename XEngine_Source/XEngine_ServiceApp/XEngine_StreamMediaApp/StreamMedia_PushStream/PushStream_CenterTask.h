#pragma once
/********************************************************************
//    Created:     2023/06/04  12:57:16
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PushStream\PushStream_CenterTask.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PushStream
//    File Base:   PushStream_CenterTask
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     X推流任务处理代码
//    History:
*********************************************************************/
//任务处理池,用来获取一个完整包
XHTHREAD CALLBACK PushStream_CenterTask_Thread(XPVOID lParam);
//任务处理相关函数,处理包的内容
bool PushStream_CenterTask_Handle(XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen);