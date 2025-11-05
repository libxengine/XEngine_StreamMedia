#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2023/06/04  10:54:37
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\XEngine_HttpTask.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp
//    File Base:   XEngine_HttpTask
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     HTTP任务处理代码
//    History:
*********************************************************************/
XHTHREAD XCALLBACK XEngine_HTTPTask_Thread(XPVOID lParam)
{
	//任务池是编号1开始的.
	int nThreadPos = *(int*)lParam;
	nThreadPos++;

	while (bIsRun)
	{
		//等待编号1的任务池触发一个组完包的事件
		if (!HttpProtocol_Server_EventWaitEx(xhHttpPacket, nThreadPos))
		{
			continue;
		}
		int nListCount = 0;
		XENGINE_MANAGEPOOL_TASKEVENT** ppSst_ListAddr;
		//获得编号1的所有待处理任务的客户端列表(也就是客户端发送过来的数据已经组好了一个包,需要我们处理)
		HttpProtocol_Server_GetPoolEx(xhHttpPacket, nThreadPos, &ppSst_ListAddr, &nListCount);
		//先循环客户端
		for (int i = 0; i < nListCount; i++)
		{
			//再循环客户端拥有的任务个数
			for (int j = 0; j < ppSst_ListAddr[i]->nPktCount; j++)
			{
				int nMsgLen = 0;                                    //客户端发送的数据大小,不包括头
				int nHCount = 0;
				XCHAR* ptszMsgBuffer = NULL;                         //客户端发送的数据
				XCHAR** pptszListHdr;
				RFCCOMPONENTS_HTTP_REQPARAM st_HTTPReqparam;        //客户端的请求参数

				memset(&st_HTTPReqparam, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));
				//得到一个指定客户端的完整数据包
				if (HttpProtocol_Server_GetMemoryEx(xhHttpPacket, ppSst_ListAddr[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_HTTPReqparam, &pptszListHdr, &nHCount))
				{
					//在另外一个函数里面处理数据
					XEngine_HTTPTask_Handle(&st_HTTPReqparam, ppSst_ListAddr[i]->tszClientAddr, ptszMsgBuffer, nMsgLen, &pptszListHdr, nHCount);
					//释放内存
					BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
					BaseLib_Memory_Free((XPPPMEM)&pptszListHdr, nHCount);
				}
			}
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSst_ListAddr, nListCount);
	}
	return 0;
}
bool XEngine_HTTPTask_Handle(RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR*** ppptszHDRList, int nHDRCount)
{
	int nRVLen = 0;
	int nSDLen = 0;
	int nListCount = 0;
	XCHAR** pptszList;
	LPCXSTR lpszMethodPost = _X("POST");
	LPCXSTR lpszMethodGet = _X("GET");
	LPCXSTR lpszMethodOPtion = _X("OPTION");
	XCHAR tszRVBuffer[4096] = {};
	XCHAR tszSDBuffer[4096] = {};
	XCHAR tszUrlName[128] = {};
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam = {};

	st_HDRParam.nHttpCode = 200; //HTTP CODE码
	st_HDRParam.bIsClose = true; //收到回复后就关闭
	//HTTP验证
	if (st_ServiceConfig.st_XVerification.bEnable)
	{
		int nVType = 0;
		RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam = {};

		st_HDRParam.nHttpCode = 401;
		st_HDRParam.bIsClose = true;
		st_HDRParam.bAuth = true;
		//打包验证信息
		int nHDRLen = 0;
		XCHAR tszHDRBuffer[XPATH_MAX] = {};
		if (1 == st_ServiceConfig.st_XVerification.nVType)
		{
			Verification_HTTP_BasicServerPacket(tszHDRBuffer, &nHDRLen);
		}
		else
		{
			XCHAR tszNonceStr[64] = {};
			XCHAR tszOpaqueStr[64] = {};
			Verification_HTTP_DigestServerPacket(tszHDRBuffer, &nHDRLen, tszNonceStr, tszOpaqueStr);
		}
		//后去验证方法
		if (!Verification_HTTP_GetType(*ppptszHDRList, nHDRCount, &nVType))
		{
			HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, tszHDRBuffer);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,用户验证失败,验证方式:%d,错误:%lX"), lpszClientAddr, st_ServiceConfig.st_XVerification.nVType, Verification_GetLastError());
			return false;
		}
		//验证方式是否一致
		if (st_ServiceConfig.st_XVerification.nVType != nVType)
		{
			HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, tszHDRBuffer);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,用户验证失败,验证方式错误,请求:%d,需求:%d"), lpszClientAddr, nVType, st_ServiceConfig.st_XVerification.nVType);
			return false;
		}
		bool bRet = false;
		//通过什么方法获得用户密码
		if (_tcsxlen(st_ServiceConfig.st_XVerification.tszAPIUrl) > 0)
		{
			int nHTTPCode = 0;
			int nMSGLen = 0;
			XCLIENT_APIHTTP st_APIHttp = {};
			XCHAR* ptszMSGBuffer = NULL;
			if (!APIClient_Http_Request(_X("GET"), st_ServiceConfig.st_XVerification.tszAPIUrl, NULL, &nHTTPCode, &ptszMSGBuffer, &nMSGLen, NULL, NULL, &st_APIHttp))
			{
				st_HDRParam.nHttpCode = 500;
				ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, NULL, ERROR_SMS_PROTOCOL_UNAUTHORIZE, "api server is down,cant verification");
				HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
				XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,用户验证失败,GET请求验证服务:%s 失败,错误码:%lX"), lpszClientAddr, st_ServiceConfig.st_XVerification.tszAPIUrl, APIClient_GetLastError());
				return false;
			}
			if (200 != nHTTPCode)
			{
				st_HDRParam.nHttpCode = 500;
				ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, NULL, ERROR_SMS_PROTOCOL_UNAUTHORIZE, "api server is down,cant verification");
				HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
				XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,用户验证失败,GET请求验证服务:%s 失败,错误:%d"), lpszClientAddr, st_ServiceConfig.st_XVerification.tszAPIUrl, nHTTPCode);
				return false;
			}
			XENGINE_PROTOCOL_USERAUTHEX st_UserAuth = {};
			if (!ModuleProtocol_Parse_HTTPParseAuth(ptszMSGBuffer, nMsgLen, &st_UserAuth))
			{
				st_HDRParam.nHttpCode = 500;
				ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, NULL, ERROR_SMS_PROTOCOL_UNAUTHORIZE, "api server reply failure,cant verification");
				HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
				XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,用户验证失败,返回内容:%s 错误,无法继续"), lpszClientAddr, ptszMSGBuffer);
				BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMSGBuffer);
				return false;
			}
			BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMSGBuffer);

			if (1 == nVType)
			{
				bRet = Verification_HTTP_Basic(st_UserAuth.tszUserName, st_UserAuth.tszUserPass, *ppptszHDRList, nHDRCount);
			}
			else if (2 == nVType)
			{
				bRet = Verification_HTTP_Digest(st_UserAuth.tszUserName, st_UserAuth.tszUserPass, pSt_HTTPParam->tszHttpMethod, *ppptszHDRList, nHDRCount);
			}
		}
		else
		{
			if (1 == nVType)
			{
				bRet = Verification_HTTP_Basic(st_ServiceConfig.st_XVerification.tszUserName, st_ServiceConfig.st_XVerification.tszUserPass, *ppptszHDRList, nHDRCount);
			}
			else if (2 == nVType)
			{
				bRet = Verification_HTTP_Digest(st_ServiceConfig.st_XVerification.tszUserName, st_ServiceConfig.st_XVerification.tszUserPass, pSt_HTTPParam->tszHttpMethod, *ppptszHDRList, nHDRCount);
			}
		}

		if (!bRet)
		{
			HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, NULL, 0, tszHDRBuffer);
			NetCore_TCPXCore_SendEx(xhHttpSocket, lpszClientAddr, tszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,用户验证失败,验证处理错误,可能用户密码登信息不匹配,类型:%d"), lpszClientAddr, nVType);
			return false;
		}
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,HTTP验证类型:%d 通过"), lpszClientAddr, nVType);
	}
	XCHAR tszAPIType[128] = {};
	XCHAR tszAPIVer[128] = {};
	XCHAR tszAPIName[128] = {};
	HttpProtocol_ServerHelp_GetUrlApi(pSt_HTTPParam->tszHttpUri, tszAPIType, tszAPIVer, tszAPIName);
	//得到URL参数个数
	HttpProtocol_ServerHelp_GetParament(pSt_HTTPParam->tszHttpUri, &pptszList, &nListCount, tszUrlName);
	if (nListCount < 1)
	{
		ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, NULL, ERROR_SMS_PROTOCOL_REQUEST_PARAMETER, "Bad Request,parament is incorrent");
		HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		BaseLib_Memory_Free((XPPPMEM)&pptszList, nListCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
		return false;
	}
	XCHAR tszKey[128];
	XCHAR tszValue[128];
	LPCXSTR lpszFuncName = _X("api");
	LPCXSTR lpszFunctionStr = _X("function");
	LPCXSTR lpszStreamStr = _X("stream");
	LPCXSTR lpszFunRtc = _X("rtc");

	memset(tszKey, '\0', sizeof(tszKey));
	memset(tszValue, '\0', sizeof(tszValue));

	if (0 != _tcsxnicmp(lpszFuncName, tszUrlName, _tcsxlen(lpszFuncName)) && 0 != _tcsxnicmp(lpszFunRtc, tszAPIType, _tcsxlen(lpszFunRtc)))
	{
		ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, NULL, ERROR_SMS_PROTOCOL_REQUEST_PARAMETER, "Bad Request,parament is incorrent");
		HttpProtocol_Server_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		BaseLib_Memory_Free((XPPPMEM)&pptszList, nListCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
		return false;
	}
	//获得方法
	if (0 == _tcsxnicmp(lpszMethodPost, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodPost)))
	{
		if (0 == _tcsxnicmp(lpszFunRtc, tszAPIType, _tcsxlen(lpszFunRtc)))
		{
			if (0 == _tcsxnicmp(tszAPIName, "whip", 4))
			{
				PushStream_ClientWhip_Handle(pSt_HTTPParam, lpszClientAddr, lpszMsgBuffer, nMsgLen);
			}
			else
			{
				PullStream_ClientWhep_Handle(pSt_HTTPParam, lpszClientAddr, lpszMsgBuffer, nMsgLen);
			}
		}
	}
	else if (0 == _tcsxnicmp(lpszMethodGet, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodGet)))
	{
		//获得函数名
		BaseLib_String_GetKeyValue(pptszList[0], "=", tszKey, tszValue);

		//获得函数名
		//http://app.xyry.org:5501/api?function=forward&url=http://app.xyry.org
		if (0 == _tcsxnicmp(lpszFunctionStr, tszKey, _tcsxlen(lpszFunctionStr)))
		{
			HTTPApi_Management_Task(lpszClientAddr, &pptszList, nListCount);
		}
		else if (0 == _tcsxnicmp(lpszStreamStr, tszKey, _tcsxlen(lpszStreamStr)))
		{
			//如果是拉流请求
			PullStream_ClientGet_Handle(lpszClientAddr, &pptszList, nListCount);
		}
	}
	else if (0 == _tcsxnicmp(lpszMethodOPtion, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodOPtion)))
	{
		nSDLen = _xstprintf(tszSDBuffer, _X("HTTP/1.1 200 OK\r\n"
			"Connection: Close\r\n"
			"Access-Control-Allow-Origin: *\r\n"
			"Access-Control-Allow-Headers: *\r\n"
			"Access-Control-Allow-Methods: *\r\n"
			"Access-Control-Expose-Headers: *\r\n"
			"Access-Control-Allow-Credentials: false\r\n"
			"Access-Control-Request-Private-Network: true\r\n"
			"Content-Length: 0\r\n"
			"Server: %s/V%s\r\n\r\n"), XENGINE_NAME_STR, BaseLib_Version_XNumberStr());

		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP接口:%s,请求OPTION方法成功"), lpszClientAddr);
	}
	else
	{
		//可能是RTSP
		PullStream_ClientRtsp_Handle(pSt_HTTPParam, lpszClientAddr, lpszMsgBuffer, nMsgLen, &pptszList, nListCount, ppptszHDRList, nHDRCount);
	}
	BaseLib_Memory_Free((XPPPMEM)&pptszList, nListCount);
	return true;
}