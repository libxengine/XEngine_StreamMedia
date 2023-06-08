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
	int nAVType;                //包类型
	int nFrameType;             //帧类型
	int nMsgLen;                //大小
	XCHAR* ptszMsgBuffer;
}AVPACKET_MSGBUFFER;

typedef struct
{
	XCHAR tszMsgBuffer[2048];              //缓存的头
	XCHAR tszSMSAddr[MAX_PATH];
	XNETHANDLE xhFLVStream;
	int nMsgLen;                           //缓冲头大小

	unique_ptr<mutex> st_MSGLocker;
	unique_ptr<mutex> st_ClientLocker;

	unique_ptr<list<xstring>> pStl_ListClient;
	unique_ptr<list<AVPACKET_MSGBUFFER>> pStl_ListPacket;
}PUSHSTREAM_PACKET;

class CModuleSession_PushStream
{
public:
	CModuleSession_PushStream();
	~CModuleSession_PushStream();
public:
	bool ModuleSession_PushStream_Create(LPCXSTR lpszClientAddr, LPCXSTR lpszSMSAddr, XNETHANDLE xhFLVStream);
	bool ModuleSession_PushStream_Destroy(LPCXSTR lpszClientAddr);
	bool ModuleSession_PushStream_GetAddrForAddr(LPCXSTR lpszClientAddr, XCHAR* ptszSMSAddr);
	bool ModuleSession_PushStream_GetTokenForAddr(LPCXSTR lpszClientAddr, XNETHANDLE* pxhToken);
	bool ModuleSession_PushStream_SetHDRBuffer(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen);
	bool ModuleSession_PushStream_GetHDRBuffer(LPCXSTR lpszClientAddr, XCHAR* ptszMsgBuffer, int* pInt_MsgLen);
	bool ModuleSession_PushStream_FindStream(LPCXSTR lpszSMSAddr, XCHAR* ptszClientAddr);
	bool ModuleSession_PushStream_Send(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nAVType, int nFrameType);
	bool ModuleSession_PushStream_Recv(LPCXSTR lpszClientAddr, XCHAR** pptszMsgBuffer, int* pInt_MsgLen, int* pInt_AVType, int* pInt_FrameType);
public:
	bool ModuleSession_PushStream_ClientInsert(LPCXSTR lpszClientAddr, LPCXSTR lpszPullAddr);
	bool ModuleSession_PushStream_ClientDelete(LPCXSTR lpszClientAddr, LPCXSTR lpszPullAddr);
	bool ModuleSession_PushStream_ClientList(LPCXSTR lpszClientAddr, list<xstring> *pStl_ListClient);
private:
	shared_mutex st_Locker;
private:
	unordered_map<xstring, PUSHSTREAM_PACKET *> stl_MapPushStream;
};
