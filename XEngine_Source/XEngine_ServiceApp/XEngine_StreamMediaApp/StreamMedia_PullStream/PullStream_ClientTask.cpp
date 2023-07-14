#include "../XEngine_Hdr.h"
/********************************************************************
//    Created:     2023/06/08  12:03:26
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PullStream\PullStream_ClientTask.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PullStream
//    File Base:   PullStream_ClientTask
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     拉流任务处理函数
//    History:
*********************************************************************/
bool PullStream_ClientTask_Handle(LPCXSTR lpszClientAddr, XCHAR*** ppptszListHdr, int nListCount)
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

		memset(tszPushAddr, '\0', sizeof(tszPushAddr));
		memset(tszKeyBuffer, '\0', sizeof(tszKeyBuffer));
		memset(tszVluBuffer, '\0', sizeof(tszVluBuffer));

		BaseLib_OperatorString_GetKeyValue((*ppptszListHdr)[1], "=", tszKeyBuffer, tszVluBuffer);

		if (!ModuleSession_PushStream_FindStream(tszVluBuffer, tszPushAddr))
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
		if (0 == _tcsxnicmp(tszVluBuffer, "flv", 3))
		{
			enStreamType = ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_FLV;
		}
		else if (0 == _tcsxnicmp(tszVluBuffer, "rtmp", 4))
		{
			enStreamType = ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_RTMP;
		}
		else
		{
			ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, NULL, 500, "not support");
			HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("拉流端:%s,请求拉流的数据类型不支持:%s,错误:%lX"), lpszClientAddr, tszVluBuffer, ModuleSession_GetLastError());
			return false;
		}
		//返回数据,为HTTP CHUNKED
		ModuleSession_PushStream_GetHDRBuffer(tszPushAddr, tszSDBuffer, &nSDLen, enStreamType);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);

		ModuleSession_PullStream_Insert(lpszClientAddr, tszVluBuffer, tszPushAddr, enStreamType);
		ModuleSession_PushStream_ClientInsert(tszPushAddr, lpszClientAddr, enStreamType);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("拉流端:%s,请求拉流数据成功:%s"), lpszClientAddr, tszVluBuffer);
	}
	else if (0 == _tcsxnicmp(lpszStreamStop, tszVluBuffer, _tcsxlen(lpszStreamStop)))
	{
		//停止拉流,:http://127.0.0.1:5600/api?stream=stop
		XCHAR tszSMSAddr[MAX_PATH];
		XCHAR tszPushAddr[MAX_PATH];

		memset(tszSMSAddr, '\0', sizeof(tszSMSAddr));
		memset(tszPushAddr, '\0', sizeof(tszPushAddr));

		ModuleSession_PullStream_GetSMSAddr(lpszClientAddr, tszSMSAddr);
		if (!ModuleSession_PullStream_GetPushAddr(lpszClientAddr, tszPushAddr))
		{
			ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, NULL, 404, "not found");
			HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("拉流端:%s,请求停止拉流失败,获取绑定推流地址失败,错误:%lX"), lpszClientAddr, ModuleSession_GetLastError());
			return false;
		}
		ModuleSession_PullStream_Delete(lpszClientAddr);
		ModuleSession_PushStream_ClientDelete(tszPushAddr, lpszClientAddr);
		HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("拉流端:%s,请求停止拉流成功,停止的流:%s,绑定的推流地址:%s"), lpszClientAddr, tszSMSAddr, tszPushAddr);
	}
	return true;
}