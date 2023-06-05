#pragma once
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
	bool ModuleProtocol_Packet_Comm(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, int nCode = 0, LPCXSTR lpszMsgBuffer = NULL, LPCXSTR lpszToken = NULL);
public:
	bool ModuleProtocol_Packet_Create(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream);
	bool ModuleProtocol_Packet_Push(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream, int nMsgLen, int nMsgType);
	bool ModuleProtocol_Packet_Destroy(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream);
private:
};