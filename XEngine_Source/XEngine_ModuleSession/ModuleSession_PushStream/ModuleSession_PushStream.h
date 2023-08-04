#pragma once
/********************************************************************
//    Created:     2023/06/04  20:17:43
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_PushStream\ModuleSession_PushStream.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_PushStream
//    File Base:   ModuleSession_PushStream
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     推流全局会话
//    History:
*********************************************************************/
typedef struct
{
	XCHAR tszMsgBuffer[2048];              //缓存的头
	int nMsgLen;                           //缓冲头大小
}AVPACKET_HDRBUFFER;

typedef struct
{
	XENGINE_PROTOCOL_AVINFO st_AVInfo;
	XCHAR tszSMSAddr[MAX_PATH];
	ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enStreamType;
	
	unique_ptr<mutex> st_MSGLocker;
	unique_ptr<mutex> st_ClientLocker;

	unique_ptr<list<STREAMMEDIA_SESSIONCLIENT>> pStl_ListClient;
	unique_ptr<unordered_map<int, AVPACKET_HDRBUFFER>> pStl_MapPushStream;
}PUSHSTREAM_PACKET;

class CModuleSession_PushStream
{
public:
	CModuleSession_PushStream();
	~CModuleSession_PushStream();
public:
	bool ModuleSession_PushStream_Create(LPCXSTR lpszClientAddr, LPCXSTR lpszSMSAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enStreamType);
	bool ModuleSession_PushStream_Destroy(LPCXSTR lpszClientAddr);
	bool ModuleSession_PushStream_GetAddrForAddr(LPCXSTR lpszClientAddr, XCHAR* ptszSMSAddr);
	bool ModuleSession_PushStream_SetHDRBuffer(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enStreamType);
	bool ModuleSession_PushStream_GetHDRBuffer(LPCXSTR lpszClientAddr, XCHAR* ptszMsgBuffer, int* pInt_MsgLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enStreamType);
	bool ModuleSession_PushStream_SetAVInfo(LPCXSTR lpszClientAddr, XENGINE_PROTOCOL_AVINFO *pSt_AVInfo);
	bool ModuleSession_PushStream_GetAVInfo(LPCXSTR lpszClientAddr, XENGINE_PROTOCOL_AVINFO* pSt_AVInfo);
	bool ModuleSession_PushStream_FindStream(LPCXSTR lpszSMSAddr, XCHAR* ptszClientAddr);
	bool ModuleSession_PushStream_GetInfo(STREAMMEDIA_PUBLISHINFO*** pppSt_ProtocolStream, int* pInt_ListCount);
public:
	bool ModuleSession_PushStream_ClientInsert(LPCXSTR lpszClientAddr, LPCXSTR lpszPullAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enStreamType);
	bool ModuleSession_PushStream_ClientDelete(LPCXSTR lpszClientAddr, LPCXSTR lpszPullAddr);
	bool ModuleSession_PushStream_ClientList(LPCXSTR lpszClientAddr, list<STREAMMEDIA_SESSIONCLIENT> *pStl_ListClient);
private:
	shared_mutex st_Locker;
private:
	unordered_map<xstring, PUSHSTREAM_PACKET *> stl_MapPushStream;
};
