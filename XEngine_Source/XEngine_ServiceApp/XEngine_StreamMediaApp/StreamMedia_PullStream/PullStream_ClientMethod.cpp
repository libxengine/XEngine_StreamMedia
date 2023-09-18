#include "../XEngine_Hdr.h"
/********************************************************************
//    Created:     2023/09/18  14:19:28
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PullStream\PullStream_ClientMethod.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PullStream
//    File Base:   PullStream_ClientMethod
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     其他方法处理
//    History:
*********************************************************************/
bool PullStream_ClientMethod_Handle(RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR*** ppptszListHdr, int nListCount)
{
	int nRVLen = 0;
	int nSDLen = 0;
	XCHAR tszRVBuffer[4096];
	XCHAR tszSDBuffer[4096];
	LPCXSTR lpszOPTIONStr = _X("OPTIONS");
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;

	memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));
	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

	st_HDRParam.nHttpCode = 200; //HTTP CODE码
	st_HDRParam.bIsClose = true; //收到回复后就关闭

	if (0 == _tcsxnicmp(pSt_HTTPParam->tszHttpMethod, lpszOPTIONStr, _tcsxlen(pSt_HTTPParam->tszHttpMethod)))
	{
		//OPTIONS rtsp://10.0.1.89:554/480p.264 RTSP/1.0
		XCHAR tszPushAddr[MAX_PATH];
		XCHAR tszSMSAddr[MAX_PATH];

		memset(tszPushAddr, '\0', sizeof(tszPushAddr));
		memset(tszSMSAddr, '\0', sizeof(tszSMSAddr));
		if (!ModuleSession_PushStream_FindStream(tszSMSAddr, tszPushAddr))
		{
			ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, NULL, 404, "not found");
			HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("RTSP:%s,请求拉流的URL参数不正确:%s,错误:%lX"), lpszClientAddr, tszSMSAddr, ModuleSession_GetLastError());
			return false;
		}
		RTSPPROTOCOL_REQUEST st_RTSPRequest;
		memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

		RTSPProtocol_CoreParse_Parse(&st_RTSPRequest, lpszMsgBuffer, nMsgLen);
		//rtsp://10.0.1.89:554/live/livestream
		XCHAR tszGMTBuffer[128];
		memset(tszGMTBuffer, '\0', sizeof(tszGMTBuffer));

		BaseLib_OperatorTime_GMTTime(tszGMTBuffer);
		nSDLen = _xstprintf(tszSDBuffer, _X("RTSP/1.0 200 OK\r\n"
			"CSeq: %d\r\n"
			"Date: %s\r\n"
			"Server: XEngine/%s\r\n"
			"Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE, GET_PARAMETER, SET_PARAMETER\r\n\r\n"), st_RTSPRequest.nCseq, tszGMTBuffer, BaseLib_OperatorVer_XNumberStr());

		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("RTSP:%s,请求OPTION选项成功,请求的拉流ID:%s"), lpszClientAddr, tszSMSAddr);
	}
	else
	{
		ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, NULL, 500, "not support");
		HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("拉流端:%s,请求的类型不支持:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpMethod);
		return false;
	}
	return true;
}