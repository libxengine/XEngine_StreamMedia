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
bool PullStream_ClientGet_FLVPlay(LPCXSTR lpszClientAddr, LPCXSTR lpszPushAddr, XCHAR* ptszSDBuffer, XCHAR* ptszRVBuffer)
{
	int nRVLen = 0;
	int nSDLen = 0;
	XENGINE_PROTOCOL_AVINFO st_AVInfo = {};
	//拷贝头
	FLVProtocol_Packet_FrameHdr(lpszPushAddr, ptszRVBuffer, &nRVLen);
	nSDLen = _xstprintf(ptszSDBuffer, _X("%x\r\n"), nRVLen);
	memcpy(ptszSDBuffer + nSDLen, ptszRVBuffer, nRVLen);
	nSDLen += nRVLen;
	memcpy(ptszSDBuffer + nSDLen, _X("\r\n"), 2);
	nSDLen += 2;
	XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
	//标签信息
	int nTagSize = 0;
	ModuleSession_PushStream_GetAVInfo(lpszPushAddr, &st_AVInfo);
	FLVProtocol_Packet_FrameScript(lpszPushAddr, ptszRVBuffer, &nRVLen, &st_AVInfo, &nTagSize);
	nSDLen = _xstprintf(ptszSDBuffer, _X("%x\r\n"), nRVLen);
	memcpy(ptszSDBuffer + nSDLen, ptszRVBuffer, nRVLen);
	nSDLen += nRVLen;
	memcpy(ptszSDBuffer + nSDLen, _X("\r\n"), 2);
	nSDLen += 2;
	XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
	//发送音视频信息
	if (st_AVInfo.st_VideoInfo.bEnable)
	{
		FLVProtocol_Packet_FrameAVCConfigure(lpszPushAddr, ptszRVBuffer, &nRVLen, &st_AVInfo, &nTagSize);
		nSDLen = _xstprintf(ptszSDBuffer, _X("%x\r\n"), nRVLen);
		memcpy(ptszSDBuffer + nSDLen, ptszRVBuffer, nRVLen);
		nSDLen += nRVLen;
		memcpy(ptszSDBuffer + nSDLen, _X("\r\n"), 2);
		nSDLen += 2;
		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
	}

	if (st_AVInfo.st_AudioInfo.bEnable)
	{
		FLVProtocol_Packet_FrameAACConfigure(lpszPushAddr, ptszRVBuffer, &nRVLen, &st_AVInfo, &nTagSize);
		nSDLen = _xstprintf(ptszSDBuffer, _X("%x\r\n"), nRVLen);
		memcpy(ptszSDBuffer + nSDLen, ptszRVBuffer, nRVLen);
		nSDLen += nRVLen;
		memcpy(ptszSDBuffer + nSDLen, _X("\r\n"), 2);
		nSDLen += 2;
		XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
	}
	ModuleSession_PullStream_FLVTagSet(lpszClientAddr, nTagSize);
	ModuleSession_PushStream_ClientInsert(lpszPushAddr, lpszClientAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_FLV);
	return true;
}
bool PullStream_ClientGet_XStreamPlay(LPCXSTR lpszClientAddr, LPCXSTR lpszPushAddr, XCHAR* ptszSDBuffer, XCHAR* ptszRVBuffer)
{
	int nRVLen = 0;
	int nSDLen = 0;
	XENGINE_PROTOCOL_AVINFO st_AVInfo = {};
	//拷贝头
	ModuleSession_PushStream_GetAVInfo(lpszPushAddr, &st_AVInfo);
	nRVLen = sizeof(XENGINE_PROTOCOL_AVINFO);
	nSDLen = _xstprintf(ptszSDBuffer, _X("%x\r\n"), nRVLen);
	memcpy(ptszSDBuffer + nSDLen, ptszRVBuffer, nRVLen);
	nSDLen += nRVLen;
	memcpy(ptszSDBuffer + nSDLen, _X("\r\n"), 2);
	nSDLen += 2;
	XEngine_Network_Send(lpszClientAddr, ptszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
	ModuleSession_PushStream_ClientInsert(lpszPushAddr, lpszClientAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_XSTREAM);
	return true;
}
bool PullStream_ClientGet_Handle(LPCXSTR lpszClientAddr, XCHAR*** ppptszListHdr, int nListCount)
{
	int nRVLen = 0;
	int nSDLen = 0;
	XCHAR tszRVBuffer[4096];
	XCHAR tszSDBuffer[4096];
	XCHAR tszKeyBuffer[XPATH_MAX];
	XCHAR tszVluBuffer[XPATH_MAX];
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

	BaseLib_String_GetKeyValue((*ppptszListHdr)[0], "=", tszKeyBuffer, tszVluBuffer);
	
	if (0 == _tcsxnicmp(lpszStreamPlay, tszVluBuffer, _tcsxlen(lpszStreamPlay)))
	{
		//播放流:http://127.0.0.1:5600/api?stream=play&sms=live/qyt&type=flv
		XCHAR tszPushAddr[XPATH_MAX];
		XCHAR tszSMSAddr[XPATH_MAX];

		memset(tszPushAddr, '\0', sizeof(tszPushAddr));
		memset(tszSMSAddr, '\0', sizeof(tszSMSAddr));
		memset(tszKeyBuffer, '\0', sizeof(tszKeyBuffer));
		memset(tszVluBuffer, '\0', sizeof(tszVluBuffer));

		BaseLib_String_GetKeyValue((*ppptszListHdr)[1], "=", tszKeyBuffer, tszSMSAddr);
		memset(tszVluBuffer, '\0', sizeof(tszVluBuffer));
		BaseLib_String_GetKeyValue((*ppptszListHdr)[2], "=", tszKeyBuffer, tszVluBuffer);

		ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enStreamType;
		if (0 == _tcsxnicmp(tszVluBuffer, "flv", 3))
		{
			bool bSMSFound = false;
			if (ModuleSession_PushStream_FindStream(tszSMSAddr, tszPushAddr))
			{
				bSMSFound = true;
			}
			else
			{
				if (!st_ServiceConfig.st_XPull.st_PullFlv.bPrePull)
				{
					ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, NULL, 404, "not found");
					HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
					XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("拉流端:%s,请求拉流的URL参数不正确:%s,可能流不存在,错误:%lX"), lpszClientAddr, tszVluBuffer, ModuleSession_GetLastError());
					return false;
				}
				bSMSFound = false;
			}
			enStreamType = ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_FLV;
			
			//返回数据,为HTTP CHUNKED
			nSDLen = _xstprintf(tszSDBuffer, _X("HTTP/1.1 200 OK\r\n"
				"Connection: Close\r\n"
				"Content-Type: video/x-flv\r\n"
				"Server: XEngine/%s\r\n"
				"Access-Control-Allow-Origin: *\r\n"
				"Access-Control-Allow-Credentials: true\r\n"
				"Transfer-Encoding: chunked\r\n\r\n"), BaseLib_Version_XTypeStr());
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			
			ModuleSession_PullStream_Insert(lpszClientAddr, tszSMSAddr, tszPushAddr, enStreamType);
			if (bSMSFound)
			{
				PullStream_ClientGet_FLVPlay(lpszClientAddr, tszPushAddr, tszSDBuffer, tszRVBuffer);
			}
		}
		else if (0 == _tcsxnicmp(tszVluBuffer, "xstream", 7))
		{
			bool bSMSFound = false;
			if (st_ServiceConfig.st_XPull.st_PullXStream.bPrePull)
			{
				bSMSFound = true;
			}
			else
			{
				if (!ModuleSession_PushStream_FindStream(tszSMSAddr, tszPushAddr))
				{
					ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, NULL, 404, "not found");
					HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
					XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("拉流端:%s,请求拉流的URL参数不正确:%s,可能流不存在,错误:%lX"), lpszClientAddr, tszVluBuffer, ModuleSession_GetLastError());
					return false;
				}
				bSMSFound = false;
			}
			enStreamType = ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_XSTREAM;
			
			memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
			//返回数据,为HTTP CHUNKED
			nSDLen = _xstprintf(tszSDBuffer, _X("HTTP/1.1 200 OK\r\n"
				"Connection: Close\r\n"
				"Content-Type: video/x-stream\r\n"
				"Server: XEngine/%s\r\n"
				"Access-Control-Allow-Origin: *\r\n"
				"Access-Control-Allow-Credentials: true\r\n"
				"Transfer-Encoding: chunked\r\n\r\n"), BaseLib_Version_XTypeStr());
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		
			ModuleSession_PullStream_Insert(lpszClientAddr, tszSMSAddr, tszPushAddr, enStreamType);
			if (bSMSFound)
			{
				PullStream_ClientGet_XStreamPlay(lpszClientAddr, tszPushAddr, tszSDBuffer, tszRVBuffer);
			}
		}
		else if (0 == _tcsxnicmp(tszVluBuffer, "ts", 2))
		{
			bool bSMSFound = false;

			if (ModuleSession_PushStream_FindStream(tszSMSAddr, tszPushAddr))
			{
				bSMSFound = true;
			}
			else
			{
				if (!st_ServiceConfig.st_XPull.st_PullTs.bPrePull)
				{
					ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, NULL, 404, "not found");
					HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
					XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("拉流端:%s,请求拉流的URL参数不正确:%s,可能流不存在,错误:%lX"), lpszClientAddr, tszVluBuffer, ModuleSession_GetLastError());
					return false;
				}
				bSMSFound = false;
			}
			
			enStreamType = ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_TS;
			//返回数据,为HTTP CHUNKED
			nSDLen = _xstprintf(tszSDBuffer, _X("HTTP/1.1 200 OK\r\n"
				"Connection: Close\r\n"
				"Content-Type: video/x-stream\r\n"
				"Server: XEngine/%s\r\n"
				"Access-Control-Allow-Origin: *\r\n"
				"Access-Control-Allow-Credentials: true\r\n"
				"Transfer-Encoding: chunked\r\n\r\n"), BaseLib_Version_XTypeStr());
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);

			ModuleSession_PullStream_Insert(lpszClientAddr, tszSMSAddr, tszPushAddr, enStreamType);
			if (bSMSFound)
			{
				ModuleSession_PushStream_ClientInsert(tszPushAddr, lpszClientAddr, enStreamType);
			}
		}
		else
		{
			ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, NULL, 500, "not support");
			HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("拉流端:%s,请求拉流的数据类型不支持:%s,错误:%lX"), lpszClientAddr, tszVluBuffer, ModuleSession_GetLastError());
			return false;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("拉流端:%s,请求拉流数据类型:%s,地址:%s 成功"), lpszClientAddr, tszVluBuffer, tszSMSAddr);
	}
	else if (0 == _tcsxnicmp(lpszStreamStop, tszVluBuffer, _tcsxlen(lpszStreamStop)))
	{
		//停止拉流,:http://127.0.0.1:5600/api?stream=stop
		XCHAR tszSMSAddr[XPATH_MAX];
		XCHAR tszPushAddr[XPATH_MAX];

		memset(tszSMSAddr, '\0', sizeof(tszSMSAddr));
		memset(tszPushAddr, '\0', sizeof(tszPushAddr));

		if (!ModuleSession_PullStream_GetPushAddr(lpszClientAddr, tszPushAddr))
		{
			ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, NULL, 404, "not found");
			HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("拉流端:%s,请求停止拉流失败,获取绑定推流地址失败,错误:%lX"), lpszClientAddr, ModuleSession_GetLastError());
			return false;
		}
		ModuleSession_PullStream_GetSMSAddr(lpszClientAddr, tszSMSAddr);

		ModuleSession_PullStream_Delete(lpszClientAddr);
		ModuleSession_PushStream_ClientDelete(tszPushAddr, lpszClientAddr);
		HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("拉流端:%s,请求停止拉流成功,停止的流:%s,绑定的推流地址:%s"), lpszClientAddr, tszSMSAddr, tszPushAddr);
	}
	return true;
}