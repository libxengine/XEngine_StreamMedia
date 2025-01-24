#include "../XEngine_Hdr.h"
/********************************************************************
//    Created:     2023/07/31  15:17:45
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_HTTPApi\StreamMedia_HTTPApi.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_HTTPApi
//    File Base:   StreamMedia_HTTPApi
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     HTTP管理接口
//    History:
*********************************************************************/
bool HTTPApi_Management_Task(LPCXSTR lpszClientAddr, XCHAR*** ppptszListHdr, int nListCount)
{
	int nRVLen = 0;
	int nSDLen = 0;
	XCHAR tszRVBuffer[4096];
	XCHAR tszSDBuffer[4096];
	XCHAR tszKeyBuffer[MAX_PATH];
	XCHAR tszVluBuffer[MAX_PATH];
	LPCXSTR lpszAPIPublish = _X("publish");
	LPCXSTR lpszAPIPull = _X("pull");
	LPCXSTR lpszAPIGet = _X("get");
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;

	memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszKeyBuffer, '\0', sizeof(tszKeyBuffer));
	memset(tszVluBuffer, '\0', sizeof(tszVluBuffer));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

	st_HDRParam.nHttpCode = 200; //HTTP CODE码
	st_HDRParam.bIsClose = true; //收到回复后就关闭

	BaseLib_String_GetKeyValue((*ppptszListHdr)[0], "=", tszKeyBuffer, tszVluBuffer);
	
	if (0 == _tcsxnicmp(lpszAPIPublish, tszVluBuffer, _tcsxlen(lpszAPIPublish)))
	{
		//获取发布流:http://127.0.0.1:5600?api?function=publish&method=get
		memset(tszKeyBuffer, '\0', sizeof(tszKeyBuffer));
		memset(tszVluBuffer, '\0', sizeof(tszVluBuffer));

		BaseLib_String_GetKeyValue((*ppptszListHdr)[1], "=", tszKeyBuffer, tszVluBuffer);
		if (0 == _tcsxnicmp(lpszAPIGet, tszVluBuffer, _tcsxlen(lpszAPIGet)))
		{
			int nListCount = 0;
			STREAMMEDIA_PUBLISHINFO** ppSt_ProtocolStream;
			ModuleSession_PushStream_GetInfo(&ppSt_ProtocolStream, &nListCount);
			ModuleProtocol_Packet_HTTPPublishGet(tszRVBuffer, &nRVLen, &ppSt_ProtocolStream, nListCount);
			HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP接口:%s,请求获取发布流信息成功,个数:%d"), lpszClientAddr, nListCount);
			BaseLib_Memory_Free((XPPPMEM)&ppSt_ProtocolStream, nListCount);
		}
	}
	else if (0 == _tcsxnicmp(lpszAPIPull, tszVluBuffer, _tcsxlen(lpszAPIPull)))
	{
		//获取订阅流:http://127.0.0.1:5600/api?function=pull&method=get
		memset(tszKeyBuffer, '\0', sizeof(tszKeyBuffer));
		memset(tszVluBuffer, '\0', sizeof(tszVluBuffer));

		BaseLib_String_GetKeyValue((*ppptszListHdr)[1], "=", tszKeyBuffer, tszVluBuffer);
		if (0 == _tcsxnicmp(lpszAPIGet, tszVluBuffer, _tcsxlen(lpszAPIGet)))
		{
			int nListCount = 0;
			STREAMMEDIA_PULLLISTINFO** ppSt_PullList;
			ModuleSession_PullStream_GetList(&ppSt_PullList, &nListCount);
			ModuleProtocol_Packet_HTTPPullGet(tszRVBuffer, &nRVLen, &ppSt_PullList, nListCount);
			HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP接口:%s,请求获取订阅流信息成功,个数:%d"), lpszClientAddr, nListCount);
			BaseLib_Memory_Free((XPPPMEM)&ppSt_PullList, nListCount);
		}
	}
	return true;
}