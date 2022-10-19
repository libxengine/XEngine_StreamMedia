#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2022/10/17  10:59:58
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_ForwardApp\XEngine_HttpTask.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_ForwardApp
//    File Base:   XEngine_HttpTask
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     HTTP任务处理代码
//    History:
*********************************************************************/
XHTHREAD CALLBACK XEngine_HTTPTask_Thread(LPVOID lParam)
{
	//任务池是编号1开始的.
	int nThreadPos = *(int*)lParam;
	nThreadPos++;

	while (bIsRun)
	{
		//等待编号1的任务池触发一个组完包的事件
		if (!RfcComponents_HttpServer_EventWaitEx(xhHttpPacket, nThreadPos))
		{
			continue;
		}
		int nListCount = 0;
		RFCCOMPONENTS_HTTP_PKTCLIENT** ppSst_ListAddr;
		//获得编号1的所有待处理任务的客户端列表(也就是客户端发送过来的数据已经组好了一个包,需要我们处理)
		RfcComponents_HttpServer_GetPoolEx(xhHttpPacket, nThreadPos, &ppSst_ListAddr, &nListCount);
		//先循环客户端
		for (int i = 0; i < nListCount; i++)
		{
			//再循环客户端拥有的任务个数
			for (int j = 0; j < ppSst_ListAddr[i]->nPktCount; j++)
			{
				int nMsgLen = 0;                                    //客户端发送的数据大小,不包括头
				CHAR* ptszMsgBuffer = NULL;                         //客户端发送的数据
				RFCCOMPONENTS_HTTP_REQPARAM st_HTTPReqparam;        //客户端的请求参数

				memset(&st_HTTPReqparam, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));
				//得到一个指定客户端的完整数据包
				if (!RfcComponents_HttpServer_GetMemoryEx(xhHttpPacket, ppSst_ListAddr[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_HTTPReqparam))
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,获取数据包失败，错误：%lX"), ppSst_ListAddr[i]->tszClientAddr, HttpServer_GetLastError());
					continue;
				}
				//在另外一个函数里面处理数据
				XEngine_HTTPTask_Handle(&st_HTTPReqparam, ppSst_ListAddr[i]->tszClientAddr, ptszMsgBuffer, nMsgLen);
				//释放内存
				BaseLib_OperatorMemory_FreeCStyle((VOID**)&ptszMsgBuffer);
			}
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSst_ListAddr, nListCount);
	}
	return 0;
}
BOOL XEngine_HTTPTask_Handle(RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	int nSDLen = 4096;
	int nRVLen = 4096;
	LPCTSTR lpszMethodPost = _T("POST");
	LPCTSTR lpszMethodGet = _T("GET");
	TCHAR tszSDBuffer[4096];
	TCHAR tszRVBuffer[4096];
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;    //发送给客户端的参数

	memset(tszSDBuffer, '\0', sizeof(tszSDBuffer));
	memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));
	memset(&st_HDRParam, '\0', sizeof(RFCCOMPONENTS_HTTP_HDRPARAM));

	st_HDRParam.nHttpCode = 200; //HTTP CODE码
	st_HDRParam.bIsClose = TRUE; //收到回复后就关闭

	TCHAR** pptszList;
	TCHAR tszUrlName[128];
	int nListCount = 0;

	memset(tszUrlName, '\0', sizeof(tszUrlName));
	//得到URL参数个数
	RfcComponents_HttpHelp_GetParament(pSt_HTTPParam->tszHttpUri, &pptszList, &nListCount, tszUrlName);
	if (nListCount < 1)
	{
		ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, 400, "Bad Request,parament is incorrent");
		RfcComponents_HttpServer_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen);
		BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
		return FALSE;
	}
	TCHAR tszKey[128];
	TCHAR tszValue[128];
	LPCTSTR lpszFuncName = _T("api");
	LPCTSTR lpszParamFuncKey = _T("function");
	LPCTSTR lpszParamPlay = _T("play");
	LPCTSTR lpszParamStop = _T("stop");

	memset(tszKey, '\0', sizeof(tszKey));
	memset(tszValue, '\0', sizeof(tszValue));

	if (0 != _tcsnicmp(lpszFuncName, tszUrlName, _tcslen(lpszFuncName)))
	{
		ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, 400, "Bad Request,parament is incorrent");
		RfcComponents_HttpServer_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen);
		BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
		return FALSE;
	}
	//获得函数名
	BaseLib_OperatorString_GetKeyValue(pptszList[0], "=", tszKey, tszValue);
	if (0 != _tcsnicmp(lpszParamFuncKey, tszKey, _tcslen(lpszParamFuncKey)))
	{
		ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, 400, "Bad Request,parament is incorrent");
		RfcComponents_HttpServer_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
		XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen);
		BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
		return FALSE;
	}

	if (0 == _tcsnicmp(lpszMethodPost, pSt_HTTPParam->tszHttpMethod, _tcslen(lpszMethodPost)))
	{
		//播放:http://127.0.0.1:5601/api?function=play
		if (0 == _tcsnicmp(lpszParamPlay, tszValue, _tcslen(lpszParamPlay)))
		{
			TCHAR tszSMSUrl[1024];
			TCHAR tszAVUrl[MAX_PATH];
			TCHAR tszToken[MAX_PATH];

			memset(tszSMSUrl, '\0', sizeof(tszSMSUrl));
			memset(tszAVUrl, '\0', MAX_PATH);
			memset(tszToken, '\0', MAX_PATH);

			ModuleProtocol_Parse_HTTPForward(lpszMsgBuffer, nMsgLen, tszAVUrl, tszToken);
			if (_tcslen(tszToken) > 0)
			{
				if (NULL != ModuleSession_Forward_Get(tszToken))
				{
					_stprintf(tszSMSUrl, _T("%s/%s.flv"), st_ServiceConfig.tszSMSUrl, tszToken);
					ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, 0, tszSMSUrl, tszToken);
					RfcComponents_HttpServer_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
					XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen);
					BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求播放RTSP:%s,成功,流已经存在,播放地址:%s"), lpszClientAddr, tszAVUrl, tszSMSUrl);
					return TRUE;
				}
			}
			else
			{
				XNETHANDLE xhPlay = 0;
				BaseLib_OperatorHandle_Create(&xhPlay);
				_stprintf(tszToken, _T("%lld"), xhPlay);
			}

			int nStreamCount = 0;
			STREAMMEDIA_PULLSTREAM** ppSt_PullStream;
			XHANDLE xhToken = XClient_StreamPull_Init(tszAVUrl, &ppSt_PullStream, &nStreamCount, NULL);
			if (NULL == xhToken)
			{
				ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, -1, "not found stream");
				RfcComponents_HttpServer_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
				XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen);
				BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求播放RTSP:%s,失败"), lpszClientAddr, tszAVUrl);
				return FALSE;
			}
			_stprintf(tszSMSUrl, _T("%s/%s"), st_ServiceConfig.tszSMSUrl, tszToken);
			if (!XClient_StreamPull_PushStream(xhToken, tszSMSUrl, &ppSt_PullStream, nStreamCount))
			{
				ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, -2, "push stream is failed");
				RfcComponents_HttpServer_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
				XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen);
				BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
				XClient_StreamPull_Close(xhToken);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("HTTP客户端:%s,请求播放RTSP:%s,失败"), lpszClientAddr, tszAVUrl);
				return FALSE;
			}
			XClient_StreamPull_Start(xhToken);
			//返回数据
			_tcscat(tszSMSUrl, _T(".flv"));
			ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen, 0, tszSMSUrl, tszToken);
			RfcComponents_HttpServer_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求播放RTSP:%s 成功,设备流个数:%d,播放地址:%s"), lpszClientAddr, tszAVUrl, nStreamCount, tszSMSUrl);
		}
		else if (0 == _tcsnicmp(lpszParamStop, tszValue, _tcslen(lpszParamStop)))
		{
			//停止:http://127.0.0.1:5601/api?function=stop
			TCHAR tszToken[MAX_PATH];
			TCHAR tszSMSUrl[MAX_PATH];

			memset(tszToken, '\0', MAX_PATH);
			memset(tszSMSUrl, '\0', MAX_PATH);

			ModuleProtocol_Parse_HTTPForward(lpszMsgBuffer, nMsgLen, NULL, tszToken);
			XHANDLE xhToken = ModuleSession_Forward_Get(tszToken);
			if (NULL != xhToken)
			{
				XClient_StreamPull_Close(xhToken);
			}
			ModuleProtocol_Packet_Comm(tszRVBuffer, &nRVLen);
			RfcComponents_HttpServer_SendMsgEx(xhHttpPacket, tszSDBuffer, &nSDLen, &st_HDRParam, tszRVBuffer, nRVLen);
			XEngine_Network_Send(lpszClientAddr, tszSDBuffer, nSDLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求停止设备:%lld 成功"), lpszClientAddr, xhToken);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
	}
	else if (0 == _tcsnicmp(lpszMethodGet, pSt_HTTPParam->tszHttpMethod, _tcslen(lpszMethodGet)))
	{
		
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("HTTP客户端:%s,协议错误"), lpszClientAddr);
	}
	return TRUE;
}