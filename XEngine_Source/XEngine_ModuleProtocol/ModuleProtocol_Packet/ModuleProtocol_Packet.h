﻿#pragma once
/********************************************************************
//    Created:     2022/09/28  15:29:48
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_Packet\ModuleProtocol_Packet.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_Packet
//    File Base:   ModuleProtocol_Packet
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     协议打包工具
//    History:
*********************************************************************/

class CModuleProtocol_Packet
{
public:
	CModuleProtocol_Packet();
	~CModuleProtocol_Packet();
public:
	bool ModuleProtocol_Packet_Comm(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr = NULL, int nCode = 0, LPCXSTR lpszMsgBuffer = NULL);
public:
	bool ModuleProtocol_Packet_Create(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream);
	bool ModuleProtocol_Packet_Push(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream, int nMsgLen, int nMsgType);
	bool ModuleProtocol_Packet_Destroy(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream);
public:
	bool ModuleProtocol_Packet_HTTPPublishGet(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, STREAMMEDIA_PUBLISHINFO*** pppSt_PublishInfo, int nListCount);
	bool ModuleProtocol_Packet_HTTPPullGet(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, STREAMMEDIA_PULLLISTINFO*** pppSt_PullInfo, int nListCount);
private:
};