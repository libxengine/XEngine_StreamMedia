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
XHTHREAD CALLBACK XEngine_HTTPTask_Thread(XPVOID lParam)
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
		RFCCOMPONENTS_HTTP_PKTCLIENT** ppSst_ListAddr;
		//获得编号1的所有待处理任务的客户端列表(也就是客户端发送过来的数据已经组好了一个包,需要我们处理)
		HttpProtocol_Server_GetPoolEx(xhHttpPacket, nThreadPos, &ppSst_ListAddr, &nListCount);
		//先循环客户端
		for (int i = 0; i < nListCount; i++)
		{
			//再循环客户端拥有的任务个数
			for (int j = 0; j < ppSst_ListAddr[i]->nPktCount; j++)
			{
				int nMsgLen = 0;                                    //客户端发送的数据大小,不包括头
				XCHAR* ptszMsgBuffer = NULL;                         //客户端发送的数据
				RFCCOMPONENTS_HTTP_REQPARAM st_HTTPReqparam;        //客户端的请求参数

				memset(&st_HTTPReqparam, '\0', sizeof(RFCCOMPONENTS_HTTP_REQPARAM));
				//得到一个指定客户端的完整数据包
				if (HttpProtocol_Server_GetMemoryEx(xhHttpPacket, ppSst_ListAddr[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_HTTPReqparam))
				{
					//在另外一个函数里面处理数据
					XEngine_HTTPTask_Handle(&st_HTTPReqparam, ppSst_ListAddr[i]->tszClientAddr, ptszMsgBuffer, nMsgLen);
					//释放内存
					BaseLib_OperatorMemory_FreeCStyle((VOID**)&ptszMsgBuffer);
				}
			}
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSst_ListAddr, nListCount);
	}
	return 0;
}
bool XEngine_HTTPTask_Handle(RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	int nMLen = 4096;
	int nListCount = 0;

	XCHAR** pptszList;
	LPCXSTR lpszMethodPost = _X("POST");
	LPCXSTR lpszMethodGet = _X("GET");
	XCHAR tszMSGBuffer[4096];
	XCHAR tszUrlName[128];
	
	memset(tszMSGBuffer, '\0', sizeof(tszMSGBuffer));
	memset(tszUrlName, '\0', sizeof(tszUrlName));
	//得到URL参数个数
	HttpProtocol_ServerHelp_GetParament(pSt_HTTPParam->tszHttpUri, &pptszList, &nListCount, tszUrlName);
	if (nListCount < 1)
	{
		ModuleProtocol_Packet_Comm(tszMSGBuffer, &nMLen, 400, "Bad Request,parament is incorrent");
		XEngine_Network_Send(lpszClientAddr, tszMSGBuffer, nMLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
		return false;
	}
	XCHAR tszKey[128];
	XCHAR tszValue[128];
	LPCXSTR lpszFuncName = _X("api");
	LPCXSTR lpszParamFuncKey = _X("function");
	LPCXSTR lpszParamPlay = _X("play");
	LPCXSTR lpszParamStop = _X("stop");
	LPCXSTR lpszParamList = _X("list");

	memset(tszKey, '\0', sizeof(tszKey));
	memset(tszValue, '\0', sizeof(tszValue));

	if (0 != _tcsxnicmp(lpszFuncName, tszUrlName, _tcsxlen(lpszFuncName)))
	{
		ModuleProtocol_Packet_Comm(tszMSGBuffer, &nMLen, 400, "Bad Request,parament is incorrent");
		XEngine_Network_Send(lpszClientAddr, tszMSGBuffer, nMLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
		return false;
	}
	//获得函数名
	BaseLib_OperatorString_GetKeyValue(pptszList[0], "=", tszKey, tszValue);
	if (0 != _tcsxnicmp(lpszParamFuncKey, tszKey, _tcsxlen(lpszParamFuncKey)))
	{
		ModuleProtocol_Packet_Comm(tszMSGBuffer, &nMLen, 400, "Bad Request,parament is incorrent");
		XEngine_Network_Send(lpszClientAddr, tszMSGBuffer, nMLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
		BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
		return false;
	}

	if (0 == _tcsxnicmp(lpszMethodPost, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodPost)))
	{
		//播放:http://127.0.0.1:5602/api?function=play&token=100010101
		if (0 == _tcsxnicmp(lpszParamPlay, tszValue, _tcsxlen(lpszParamPlay)))
		{
			XCHAR tszSMSUrl[1024];
			XCHAR tszAVUrl[MAX_PATH];
			XCHAR tszToken[MAX_PATH];

			memset(tszSMSUrl, '\0', sizeof(tszSMSUrl));
			memset(tszAVUrl, '\0', MAX_PATH);
			memset(tszToken, '\0', MAX_PATH);

			int nStreamCount = 0;
			STREAMMEDIA_PULLSTREAM** ppSt_PullStream;
			
			ModuleProtocol_Parse_HTTPForward(lpszMsgBuffer, nMsgLen, tszAVUrl);
			if (2 == nListCount)
			{
				BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszKey, tszToken);
				if (NULL != ModuleSession_Forward_Get(tszToken))
				{
					ModuleProtocol_Packet_Comm(tszMSGBuffer, &nMLen, 0, NULL, tszToken);
					XEngine_Network_Send(lpszClientAddr, tszMSGBuffer, nMLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
					BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求播放RTSP:%s,成功,流已经存在"), lpszClientAddr, tszAVUrl);
					return true;
				}
			}
			else
			{
				XNETHANDLE xhPlay = 0;
				BaseLib_OperatorHandle_Create(&xhPlay);
				_xstprintf(tszToken, _X("%lld"), xhPlay);
			}
			
			XHANDLE xhToken = StreamClient_StreamPull_Init(tszAVUrl, &ppSt_PullStream, &nStreamCount);
			if (NULL == xhToken)
			{
				ModuleProtocol_Packet_Comm(tszMSGBuffer, &nMLen, -1, "not found stream");
				XEngine_Network_Send(lpszClientAddr, tszMSGBuffer, nMLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
				BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求播放RTSP:%s,失败"), lpszClientAddr, tszAVUrl);
				return false;
			}
			_xstprintf(tszSMSUrl, _X("%s/%s"), st_ServiceConfig.tszSMSUrl, tszToken);
			if (!StreamClient_StreamPull_PushStream(xhToken, tszSMSUrl, &ppSt_PullStream, nStreamCount))
			{
				ModuleProtocol_Packet_Comm(tszMSGBuffer, &nMLen, -2, "push stream is failed");
				XEngine_Network_Send(lpszClientAddr, tszMSGBuffer, nMLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
				BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
				StreamClient_StreamPull_Close(xhToken);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求播放RTSP:%s,失败"), lpszClientAddr, tszAVUrl);
				return false;
			}
			StreamClient_StreamPull_Start(xhToken);
			ModuleSession_Forward_Create(tszToken, xhToken, tszAVUrl);
			//返回数据
			ModuleProtocol_Packet_Comm(tszMSGBuffer, &nMLen, 0, NULL, tszToken);
			XEngine_Network_Send(lpszClientAddr, tszMSGBuffer, nMLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求播放RTSP:%s 成功,设备流个数:%d"), lpszClientAddr, tszAVUrl, nStreamCount);
		}
		else if (0 == _tcsxnicmp(lpszParamStop, tszValue, _tcsxlen(lpszParamStop)))
		{
			//停止:http://127.0.0.1:5602/api?function=stop&token=100010101
			XCHAR tszToken[MAX_PATH];
			memset(tszToken, '\0', MAX_PATH);

			if (2 != nListCount)
			{
				ModuleProtocol_Packet_Comm(tszMSGBuffer, &nMLen, -404, "url parament have no token");
				XEngine_Network_Send(lpszClientAddr, tszMSGBuffer, nMLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
				BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("HTTP客户端:%s,请求停止流失败失败,没有传输令牌信息"), lpszClientAddr);
				return false;
			}
			BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszKey, tszToken);
			XHANDLE xhToken = ModuleSession_Forward_Get(tszToken);
			if (NULL != xhToken)
			{
				StreamClient_StreamPull_Close(xhToken);
				ModuleSession_Forward_Delete(tszToken);
			}
			ModuleProtocol_Packet_Comm(tszMSGBuffer, &nMLen);
			XEngine_Network_Send(lpszClientAddr, tszMSGBuffer, nMLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求停止推流:%s 成功"), lpszClientAddr, tszToken);
		}
	}
	else if (0 == _tcsxnicmp(lpszMethodGet, pSt_HTTPParam->tszHttpMethod, _tcsxlen(lpszMethodGet)))
	{
		if (0 == _tcsxnicmp(lpszParamList, tszValue, _tcsxlen(lpszParamList)))
		{
			//枚举:http://127.0.0.1:5602/api?function=list
			int nListCount = 0;
			MODULESESSION_FORWARDINFO** ppSt_Forward;
			ModuleSession_Forward_List(&ppSt_Forward, &nListCount);

			ModuleProtocol_Packet_ForwardList(tszMSGBuffer, &nMLen, &ppSt_Forward, nListCount);
			XEngine_Network_Send(lpszClientAddr, tszMSGBuffer, nMLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP);
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_Forward, nListCount);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("HTTP客户端:%s,请求列举推流数成功,数量;%d"), lpszClientAddr, nListCount);
		}
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _X("HTTP客户端:%s,协议错误"), lpszClientAddr);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
	return true;
}