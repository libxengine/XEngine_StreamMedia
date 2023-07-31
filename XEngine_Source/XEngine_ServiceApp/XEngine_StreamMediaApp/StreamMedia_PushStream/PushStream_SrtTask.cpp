#include "../XEngine_Hdr.h"
/********************************************************************
//    Created:     2023/07/28  16:12:02
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PushStream\PushStream_SrtTask.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PushStream
//    File Base:   PushStream_SrtTask
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     SRT推拉流代码
//    History:
*********************************************************************/
bool PushStream_SrtTask_Connct(LPCXSTR lpszClientAddr, SRTSOCKET hSocket)
{
	bool bPublish = false;
	XCHAR tszSMSAddr[MAX_PATH];
	memset(tszSMSAddr, '\0', sizeof(tszSMSAddr));

	if (!ModuleHelp_SrtCore_GetStreamID(hSocket, tszSMSAddr, &bPublish))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("SRT客户端：获取流ID信息失败,错误码：%lX,错误信息:%s"), ModuleHelp_GetLastError(), srt_getlasterror_str());
		return false;
	}

	if (bPublish)
	{
		//创建会话
		ModuleSession_PushStream_Create(lpszClientAddr, tszSMSAddr);
		//need to parse ts stream
		ModuleSession_PushStream_SetHDRBuffer(lpszClientAddr, NULL, 0, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_SRT);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("SRT客户端：%s,创建流成功,推流地址：%s,类型:%s"), lpszClientAddr, tszSMSAddr, bPublish ? "publish" : "request");
	}
	else
	{
		XCHAR tszPushAddr[128];
		memset(tszPushAddr, '\0', sizeof(tszPushAddr));
		//得到推流地址
		if (!ModuleSession_PushStream_FindStream(tszSMSAddr, tszPushAddr))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("SRT客户端：%s,请求拉流的参数不正确:%s,错误:%lX"), lpszClientAddr, tszSMSAddr, ModuleSession_GetLastError());
			return false;
		}
		ModuleSession_PullStream_Insert(lpszClientAddr, tszSMSAddr, tszPushAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_SRT);
		ModuleSession_PushStream_ClientInsert(tszPushAddr, lpszClientAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_SRT);
	}
	return true;
}

bool PushStream_SrtTask_Handle(LPCXSTR lpszClientAddr, SRTSOCKET hSocket, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	list<STREAMMEDIA_SESSIONCLIENT> stl_ListClient;
	ModuleSession_PushStream_ClientList(lpszClientAddr, &stl_ListClient);
	for (auto stl_ListIteratorClient = stl_ListClient.begin(); stl_ListIteratorClient != stl_ListClient.end(); ++stl_ListIteratorClient)
	{
		if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_SRT == stl_ListIteratorClient->enClientType)
		{
			XEngine_Network_Send(stl_ListIteratorClient->tszClientID, lpszMsgBuffer, nMsgLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_SRT);
			break;
		}
	}

	return true;
}