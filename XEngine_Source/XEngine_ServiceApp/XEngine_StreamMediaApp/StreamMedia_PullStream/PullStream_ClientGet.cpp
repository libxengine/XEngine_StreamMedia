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
bool PullStream_ClientGet_Handle(LPCXSTR lpszClientAddr, XCHAR*** ppptszListHdr, int nListCount)
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
		if (0 == _tcsxnicmp(tszVluBuffer, "flv", 3))
		{
			int nTagSize = 0;
			enStreamType = ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_FLV;
			//拷贝数据
			FLVProtocol_Packet_FrameHdr(tszPushAddr, tszRVBuffer, &nRVLen);
			//返回数据,为HTTP CHUNKED
			nSDLen = _xstprintf(tszSDBuffer, _X("HTTP/1.1 200 OK\r\n"
				"Connection: Close\r\n"
				"Content-Type: video/x-flv\r\n"
				"Server: XEngine/%s\r\n"
				"Access-Control-Allow-Origin: *\r\n"
				"Access-Control-Allow-Credentials: true\r\n"
				"Transfer-Encoding: chunked\r\n\r\n"
				"%x\r\n"), BaseLib_OperatorVer_XTypeStr(), nRVLen);
			memcpy(tszSDBuffer + nSDLen, tszRVBuffer, nRVLen);
			nSDLen += nRVLen;
			memcpy(tszSDBuffer + nSDLen, _X("\r\n"), 2);
			nSDLen += 2;
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			//发送脚本信息
			XENGINE_PROTOCOL_AVINFO st_AVInfo;

			memset(&st_AVInfo, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));
			memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));
			memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
			
			ModuleSession_PushStream_GetAVInfo(tszPushAddr, &st_AVInfo);
			FLVProtocol_Packet_FrameScript(tszPushAddr, tszRVBuffer, &nRVLen, &st_AVInfo, &nTagSize);
			nSDLen = _xstprintf(tszSDBuffer, _X("%x\r\n"), nRVLen);
			memcpy(tszSDBuffer + nSDLen, tszRVBuffer, nRVLen);
			nSDLen += nRVLen;
			memcpy(tszSDBuffer + nSDLen, _X("\r\n"), 2);
			nSDLen += 2;
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			//发送音视频信息
			if (st_AVInfo.st_VideoInfo.bEnable)
			{
				FLVProtocol_Packet_FrameAVCConfigure(tszPushAddr, tszRVBuffer, &nRVLen, &st_AVInfo, &nTagSize);
				nSDLen = _xstprintf(tszSDBuffer, _X("%x\r\n"), nRVLen);
				memcpy(tszSDBuffer + nSDLen, tszRVBuffer, nRVLen);
				nSDLen += nRVLen;
				memcpy(tszSDBuffer + nSDLen, _X("\r\n"), 2);
				nSDLen += 2;
				XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			}

			if (st_AVInfo.st_AudioInfo.bEnable)
			{
				FLVProtocol_Packet_FrameAACConfigure(tszPushAddr, tszRVBuffer, &nRVLen, &st_AVInfo, &nTagSize);
				nSDLen = _xstprintf(tszSDBuffer, _X("%x\r\n"), nRVLen);
				memcpy(tszSDBuffer + nSDLen, tszRVBuffer, nRVLen);
				nSDLen += nRVLen;
				memcpy(tszSDBuffer + nSDLen, _X("\r\n"), 2);
				nSDLen += 2;
				XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			}

			ModuleSession_PullStream_Insert(lpszClientAddr, tszSMSAddr, tszPushAddr, enStreamType);
			ModuleSession_PushStream_ClientInsert(tszPushAddr, lpszClientAddr, enStreamType);
			ModuleSession_PullStream_FLVTagSet(lpszClientAddr, nTagSize);
		}
		else if (0 == _tcsxnicmp(tszVluBuffer, "xstream", 7))
		{
			enStreamType = ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_XSTREAM;
			XENGINE_PROTOCOL_AVINFO st_AVInfo;

			memset(&st_AVInfo, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));
			memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

			ModuleSession_PushStream_GetAVInfo(tszPushAddr, &st_AVInfo);

			nRVLen = sizeof(XENGINE_PROTOCOL_AVINFO);
			//返回数据,为HTTP CHUNKED
			nSDLen = _xstprintf(tszSDBuffer, _X("HTTP/1.1 200 OK\r\n"
				"Connection: Close\r\n"
				"Content-Type: video/x-stream\r\n"
				"Server: XEngine/%s\r\n"
				"Access-Control-Allow-Origin: *\r\n"
				"Access-Control-Allow-Credentials: true\r\n"
				"Transfer-Encoding: chunked\r\n\r\n"
				"%x\r\n"), BaseLib_OperatorVer_XTypeStr(), nRVLen);
			memcpy(tszSDBuffer + nSDLen, &st_AVInfo, nRVLen);
			nSDLen += nRVLen;
			memcpy(tszSDBuffer + nSDLen, _X("\r\n"), 2);
			nSDLen += 2;

			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);

			ModuleSession_PullStream_Insert(lpszClientAddr, tszSMSAddr, tszPushAddr, enStreamType);
			ModuleSession_PushStream_ClientInsert(tszPushAddr, lpszClientAddr, enStreamType);
		}
		else if (0 == _tcsxnicmp(tszVluBuffer, "ts", 2))
		{
			int nPATLen = 0;
			int nPMTLen = 0;
			enStreamType = ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_TS;

			memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));
			memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));

			HLSProtocol_TSPacket_PATInfo(tszPushAddr, (XBYTE*)tszRVBuffer, &nPATLen);
			HLSProtocol_TSPacket_PMTInfo(tszPushAddr, (XBYTE*)tszRVBuffer + nPATLen, &nPMTLen);
			nRVLen = nPATLen + nPMTLen;
			//返回数据,为HTTP CHUNKED
			nSDLen = _xstprintf(tszSDBuffer, _X("HTTP/1.1 200 OK\r\n"
				"Connection: Close\r\n"
				"Content-Type: video/x-stream\r\n"
				"Server: XEngine/%s\r\n"
				"Access-Control-Allow-Origin: *\r\n"
				"Access-Control-Allow-Credentials: true\r\n"
				"Transfer-Encoding: chunked\r\n\r\n"
				"%x\r\n"), BaseLib_OperatorVer_XTypeStr(), nRVLen);
			memcpy(tszSDBuffer + nSDLen, tszRVBuffer, nRVLen);
			nSDLen += nRVLen;
			memcpy(tszSDBuffer + nSDLen, _X("\r\n"), 2);
			nSDLen += 2;

			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);

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
	else if (0 == _tcsxnicmp(lpszStreamStop, tszVluBuffer, _tcsxlen(lpszStreamStop)))
	{
		//停止拉流,:http://127.0.0.1:5600/api?stream=stop
		XCHAR tszSMSAddr[MAX_PATH];
		XCHAR tszPushAddr[MAX_PATH];

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