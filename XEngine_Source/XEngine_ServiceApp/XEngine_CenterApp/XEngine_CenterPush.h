#pragma once
/********************************************************************
//    Created:     2022/04/28  15:14:17
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_CenterApp\XEngine_CenterPush.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_CenterApp
//    File Base:   XEngine_CenterPush
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     推流代码
//    History:
*********************************************************************/
XHTHREAD CALLBACK XEngine_CenterPush_CreateAVThread(XENGINE_PROTOCOLDEVICE* pSt_ProtocolDevice, XENGINE_PROTOCOLSTREAM* pSt_ProtocolAVAttr);
//////////////////////////////////////////////////////////////////////////
int FramePush_Stream_CBVideo(LPVOID lParam, uint8_t* puszMsgBuffer, int nSize);
int FramePush_Stream_CBAudio(LPVOID lParam, uint8_t* puszMsgBuffer, int nSize);