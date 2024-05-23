#pragma once
/********************************************************************
//    Created:     2023/06/05  16:10:01
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_PullStream\ModuleSession_PullStream.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_PullStream
//    File Base:   ModuleSession_PullStream
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     拉流端会话管理器
//    History:
*********************************************************************/
class CModuleSession_PullStream
{
public:
	CModuleSession_PullStream();
	~CModuleSession_PullStream();
public:
	bool ModuleSession_PullStream_Insert(LPCXSTR lpszClientAddr, LPCXSTR lpszSMSAddr, LPCXSTR lpszPushAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enStreamType);
	bool ModuleSession_PullStream_GetSMSAddr(LPCXSTR lpszClientAddr, XCHAR* ptszSMSAddr);
	bool ModuleSession_PullStream_GetPushAddr(LPCXSTR lpszClientAddr, XCHAR* ptszPushAddr);
	bool ModuleSession_PullStream_GetStreamType(LPCXSTR lpszClientAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE* penStreamType);
	bool ModuleSession_PullStream_Delete(LPCXSTR lpszClientAddr);
	bool ModuleSession_PullStream_PublishDelete(LPCXSTR lpszClientAddr);
	bool ModuleSession_PullStream_GetList(STREAMMEDIA_PULLLISTINFO*** pppSt_PullList, int* pInt_ListCount);
public:
	bool ModuleSession_PullStream_FLVTagSet(LPCXSTR lpszClientAddr, int nTagSize);
	bool ModuleSession_PullStream_FLVTagGet(LPCXSTR lpszClientAddr, int* pInt_TagSize);
public:
	bool ModuleSession_PullStream_RTCSet(LPCXSTR lpszClientAddr, LPCXSTR lpszTokenStr, LPCXSTR lpszICEUser, LPCXSTR lpszICEPass, LPCXSTR lpszHMacStr);
	bool ModuleSession_PullStream_RTCGet(LPCXSTR lpszClientAddr, XCHAR* ptszTokenStr = NULL, XCHAR* ptszICEUser = NULL, XCHAR* ptszICEPass = NULL, XCHAR* ptszHMacStr = NULL);
private:
	shared_mutex st_Locker;
private:
	unordered_map<xstring, STREAMMEDIA_PULLLISTINFO*> stl_MapClient;
};