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
	struct  
	{
		XCHAR tszFileName[XPATH_MAX];
		XNETHANDLE xhToken;
		__int64u nTime;
		FILE* pSt_File;
	}st_HLSFile;
	struct  
	{
		XCHAR tszClientAddr[XPATH_MAX];
		XCHAR tszRTPStr[128];
		int nIndexVideo;
		int nIndexAudio;
		bool bConnect;
	}st_RTCInfo;
	struct  
	{
		XNETHANDLE xhAudioDecodec;
		XNETHANDLE xhAudioEncodec;
	}st_AVCodec;
	XENGINE_PROTOCOL_AVINFO st_AVInfo;
	XCHAR tszSMSAddr[XPATH_MAX];
	ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enStreamType;

	unique_ptr<mutex> st_ClientLocker;
	unique_ptr<list<STREAMMEDIA_SESSIONCLIENT>> pStl_ListClient;
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
	bool ModuleSession_PushStream_SetAVInfo(LPCXSTR lpszClientAddr, XENGINE_PROTOCOL_AVINFO *pSt_AVInfo);
	bool ModuleSession_PushStream_GetAVInfo(LPCXSTR lpszClientAddr, XENGINE_PROTOCOL_AVINFO* pSt_AVInfo);
	bool ModuleSession_PushStream_FindStream(LPCXSTR lpszSMSAddr, XCHAR* ptszClientAddr);
	bool ModuleSession_PushStream_GetInfo(STREAMMEDIA_PUBLISHINFO*** pppSt_ProtocolStream, int* pInt_ListCount);
public:
	bool ModuleSession_PushStream_ClientInsert(LPCXSTR lpszClientAddr, LPCXSTR lpszPullAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enStreamType);
	bool ModuleSession_PushStream_ClientDelete(LPCXSTR lpszClientAddr, LPCXSTR lpszPullAddr);
	bool ModuleSession_PushStream_ClientList(LPCXSTR lpszClientAddr, list<STREAMMEDIA_SESSIONCLIENT> *pStl_ListClient);
public:
	bool ModuleSession_PushStream_HLSInsert(LPCXSTR lpszClientAddr, LPCXSTR lpszTSFile, XNETHANDLE xhToken);
	bool ModuleSession_PushStream_HLSGetFile(LPCXSTR lpszClientAddr, XCHAR* ptszFileName);
	bool ModuleSession_PushStream_HLSWrite(LPCXSTR lpszClientAddr, LPCXSTR lpszMSGBuffer, int nMSGLen);
	bool ModuleSession_PushStream_HLSClose(LPCXSTR lpszClientAddr, XNETHANDLE* pxhToken);
	bool ModuleSession_PushStream_HLSTimeSet(LPCXSTR lpszClientAddr, __int64u nTime);
	bool ModuleSession_PushStream_HLSTimeGet(LPCXSTR lpszClientAddr, __int64u *pInt_Time);
public:
	bool ModuleSession_PushStream_AudioCodecSet(LPCXSTR lpszClientAddr, XNETHANDLE xhAudioDecodec, XNETHANDLE xhAudioEncodec);
	bool ModuleSession_PushStream_AudioCodecGet(LPCXSTR lpszClientAddr, XNETHANDLE* pxhAudioDecodec, XNETHANDLE* pxhAudioEncodec);
public:
	bool ModuleSession_PushStream_RTCAddrSet(LPCXSTR lpszClientUser, LPCXSTR lpszClientAddr);
	bool ModuleSession_PushStream_RTCAddrGet(LPCXSTR lpszClientAddr, XCHAR* ptszClientUser);
	bool ModuleSession_PushStream_RTCConnSet(LPCXSTR lpszClientAddr, bool bConnect);
	bool ModuleSession_PushStream_RTCConnGet(LPCXSTR lpszClientAddr, bool* pbConnect);
	bool ModuleSession_PushStream_RTCIndexSet(LPCXSTR lpszClientUser, int nVideoIndex, int nAudioIndex);
	bool ModuleSession_PushStream_RTCIndexGet(LPCXSTR lpszClientAddr, int* pInt_VideoIndex, int* pInt_AudioIndex);
private:
	shared_mutex st_Locker;
private:
	unordered_map<xstring, PUSHSTREAM_PACKET *> stl_MapPushStream;
};
