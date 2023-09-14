#include "../XEngine_Hdr.h"
/********************************************************************
//    Created:     2023/09/14  17:15:33
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PullStream\PullStream_ClientPost.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PullStream
//    File Base:   PullStream_ClientPost
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     POST拉流
//    History:
*********************************************************************/
bool PullStream_ClientPost_Handle(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR*** ppptszListHdr, int nListCount)
{
	int nRVLen = 0;
	int nSDLen = 0;
	XCHAR tszRVBuffer[4096];
	XCHAR tszSDBuffer[4096];
	XCHAR tszKeyBuffer[MAX_PATH];
	XCHAR tszVluBuffer[MAX_PATH];
	LPCXSTR lpszStreamPlay = _X("play");
	LPCXSTR lpszStreamStop = _X("stop");
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;

	memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszKeyBuffer, '\0', sizeof(tszKeyBuffer));
	memset(tszVluBuffer, '\0', sizeof(tszVluBuffer));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

	st_HDRParam.nHttpCode = 200; //HTTP CODE码
	st_HDRParam.bIsClose = true; //收到回复后就关闭

	BaseLib_OperatorString_GetKeyValue((*ppptszListHdr)[0], "=", tszKeyBuffer, tszVluBuffer);
	
	if (0 == _tcsxnicmp(lpszStreamPlay, tszVluBuffer, _tcsxlen(lpszStreamPlay)))
	{
		//播放流:http://127.0.0.1:5600/api?stream=play&sms=live/qyt&type=flv
		XCHAR tszPushAddr[MAX_PATH];
		XCHAR tszSMSAddr[MAX_PATH];

		memset(tszPushAddr, '\0', sizeof(tszPushAddr));
		memset(tszSMSAddr, '\0', sizeof(tszSMSAddr));
		memset(tszKeyBuffer, '\0', sizeof(tszKeyBuffer));
		memset(tszVluBuffer, '\0', sizeof(tszVluBuffer));

		BaseLib_OperatorString_GetKeyValue((*ppptszListHdr)[1], "=", tszKeyBuffer, tszSMSAddr);

		if (!ModuleSession_PushStream_FindStream(tszSMSAddr, tszPushAddr))
		{
			ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, NULL, 404, "not found");
			HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("拉流端:%s,请求拉流的URL参数不正确:%s,错误:%lX"), lpszClientAddr, tszVluBuffer, ModuleSession_GetLastError());
			return false;
		}
		memset(tszVluBuffer, '\0', sizeof(tszVluBuffer));
		BaseLib_OperatorString_GetKeyValue((*ppptszListHdr)[2], "=", tszKeyBuffer, tszVluBuffer);

		ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enStreamType;
		if (0 == _tcsxnicmp(tszVluBuffer, "rtsp", 4))
		{
			enStreamType = ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_RTSP;
			XENGINE_PROTOCOL_AVINFO st_AVInfo;
			RTSPPROTOCOL_REQUEST st_RTSPRequest;
			memset(&st_AVInfo, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));
			memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

			ModuleSession_PushStream_GetAVInfo(tszPushAddr, &st_AVInfo);

			RTSPProtocol_CoreParse_Parse(&st_RTSPRequest, lpszMsgBuffer, nMsgLen);
			if (ENUM_RTSPPROTOCOL_METHOD_TYPE_SETUP == st_RTSPRequest.enMethod)
			{
				XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			}
			ModuleSession_PullStream_Insert(lpszClientAddr, tszSMSAddr, tszPushAddr, enStreamType);
			ModuleSession_PushStream_ClientInsert(tszPushAddr, lpszClientAddr, enStreamType);
		}
		else
		{
			ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, NULL, 500, "not support");
			HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("拉流端:%s,请求拉流的数据类型不支持:%s,错误:%lX"), lpszClientAddr, tszVluBuffer, ModuleSession_GetLastError());
			return false;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("拉流端:%s,请求拉流数据成功:%s"), lpszClientAddr, tszVluBuffer);
	}
	return true;
}