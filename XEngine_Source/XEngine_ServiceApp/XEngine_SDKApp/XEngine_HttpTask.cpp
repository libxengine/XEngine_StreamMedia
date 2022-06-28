#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2022/06/22  10:12:40
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_SDKApp\XEngine_HttpTask.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_SDKApp
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
BOOL XEngine_HTTPTask_Handle(RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, LPCTSTR lpszClientAddr, LPCTSTR lpszRVBuffer, int nRVLen)
{
	int nMsgLen = 4096;
	LPCTSTR lpszMethodPost = _T("POST");
	LPCTSTR lpszMethodGet = _T("GET");
	TCHAR tszMsgBuffer[4096];
	RFCCOMPONENTS_HTTP_HDRPARAM st_HDRParam;    //发送给客户端的参数

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
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
		st_HDRParam.nHttpCode = 404;
		RfcComponents_HttpServer_SendMsgEx(xhHttpPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
		XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
		BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
		return FALSE;
	}
	TCHAR tszKey[128];
	TCHAR tszValue[128];
	LPCTSTR lpszFuncName = _T("api");
	LPCTSTR lpszParamFuncKey = _T("function");
	LPCTSTR lpszParamName = _T("params1");
	LPCTSTR lpszParamPlay = _T("play");
	LPCTSTR lpszParamStop = _T("stop");

	memset(tszKey, '\0', sizeof(tszKey));
	memset(tszValue, '\0', sizeof(tszValue));

	if (0 != _tcsnicmp(lpszFuncName, tszUrlName, _tcslen(lpszFuncName)))
	{
		st_HDRParam.nHttpCode = 404;
		RfcComponents_HttpServer_SendMsgEx(xhHttpPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
		XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
		BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
		return FALSE;
	}
	//获得函数名
	BaseLib_OperatorString_GetKeyValue(pptszList[0], "=", tszKey, tszValue);
	if (0 != _tcsnicmp(lpszParamFuncKey, tszKey, _tcslen(lpszParamFuncKey)))
	{
		st_HDRParam.nHttpCode = 404;
		RfcComponents_HttpServer_SendMsgEx(xhHttpPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
		XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
		BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
		return FALSE;
	}

	if (0 == _tcsnicmp(lpszMethodPost, pSt_HTTPParam->tszHttpMethod, _tcslen(lpszMethodPost)))
	{
	}
	else if (0 == _tcsnicmp(lpszMethodGet, pSt_HTTPParam->tszHttpMethod, _tcslen(lpszMethodGet)))
	{
		//播放:http://127.0.0.1:5601/api?function=play&params1=10001001&params2=1
		if (0 == _tcsnicmp(lpszParamPlay, tszValue, _tcslen(lpszParamPlay)))
		{
			TCHAR tszChannel[64];
			XNETHANDLE xhClient = 0;

			memset(tszKey, '\0', sizeof(tszKey));
			memset(tszValue, '\0', sizeof(tszValue));
			memset(tszChannel, '\0', sizeof(tszChannel));
			BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszKey, tszValue);
			if (0 != _tcsnicmp(lpszParamName, tszKey, _tcslen(lpszParamName)))
			{
				st_HDRParam.nHttpCode = 404;
				RfcComponents_HttpServer_SendMsgEx(xhHttpPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
				XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
				BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求播放设备发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
				return FALSE;
			}
			BaseLib_OperatorString_GetKeyValue(pptszList[2], "=", tszKey, tszChannel);
			ModulePlugin_Core_Play(_ttoi64(tszValue), _ttoi(tszChannel));
			//通知推流服务
			if (!ModuleSession_SDKDevice_Get(_ttoi64(tszValue), _ttoi(tszChannel), TRUE, &xhClient))
			{
				st_HDRParam.nHttpCode = 500;
				ModulePlugin_Core_Stop(_ttoi64(tszValue), _ttoi(tszChannel));
				RfcComponents_HttpServer_SendMsgEx(xhHttpPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
				XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
				BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求播放设备失败,获取客户端失败:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
				return FALSE;
			}
			XENGINE_PROTOCOLDEVICE st_ProtocolDevice;
			memset(&st_ProtocolDevice, '\0', sizeof(XENGINE_PROTOCOLDEVICE));

			st_ProtocolDevice.bLive = TRUE;
			st_ProtocolDevice.nChannel = _ttoi(tszChannel);
			_tcscpy(st_ProtocolDevice.tszDeviceNumber, tszValue);
			ModuleProtocol_JT1078_StreamCreate(tszMsgBuffer, &nMsgLen, &st_ProtocolDevice);
			XClient_TCPSelect_SendEx(xhClient, tszMsgBuffer, &nMsgLen);

			RfcComponents_HttpServer_SendMsgEx(xhHttpPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
			XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求播放设备:%s 通道:%s 成功"), lpszClientAddr, tszValue, tszChannel);
		}
		else if (0 == _tcsnicmp(lpszParamStop, tszValue, _tcslen(lpszParamStop)))
		{
			//停止:http://127.0.0.1:5601/api?function=stop&params1=10001001&params2=1
			TCHAR tszChannel[64];
			XNETHANDLE xhClient = 0;

			memset(tszKey, '\0', sizeof(tszKey));
			memset(tszValue, '\0', sizeof(tszValue));
			memset(tszChannel, '\0', sizeof(tszChannel));
			BaseLib_OperatorString_GetKeyValue(pptszList[1], "=", tszKey, tszValue);
			if (0 != _tcsnicmp(lpszParamName, tszKey, _tcslen(lpszParamName)))
			{
				st_HDRParam.nHttpCode = 404;
				RfcComponents_HttpServer_SendMsgEx(xhHttpPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
				XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
				BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求停止设备发送的URL请求参数不正确:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
				return FALSE;
			}
			BaseLib_OperatorString_GetKeyValue(pptszList[2], "=", tszKey, tszChannel);
			ModulePlugin_Core_Stop(_ttoi64(tszValue), _ttoi(tszChannel));
			//通知推流服务
			if (!ModuleSession_SDKDevice_Get(_ttoi64(tszValue), _ttoi(tszChannel), TRUE, &xhClient))
			{
				st_HDRParam.nHttpCode = 500;
				RfcComponents_HttpServer_SendMsgEx(xhHttpPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
				XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
				BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求播放设备失败,获取客户端失败:%s"), lpszClientAddr, pSt_HTTPParam->tszHttpUri);
				return FALSE;
			}
			XENGINE_PROTOCOLDEVICE st_ProtocolDevice;
			memset(&st_ProtocolDevice, '\0', sizeof(XENGINE_PROTOCOLDEVICE));

			st_ProtocolDevice.bLive = TRUE;
			st_ProtocolDevice.nChannel = _ttoi(tszChannel);
			_tcscpy(st_ProtocolDevice.tszDeviceNumber, tszValue);
			ModuleProtocol_JT1078_StreamDestroy(tszMsgBuffer, &nMsgLen, &st_ProtocolDevice);
			XClient_TCPSelect_SendEx(xhClient, tszMsgBuffer, &nMsgLen);

			RfcComponents_HttpServer_SendMsgEx(xhHttpPacket, tszMsgBuffer, &nMsgLen, &st_HDRParam);
			XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, nMsgLen);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("HTTP客户端:%s,请求停止设备:%s 通道:%s 成功"), lpszClientAddr, tszValue, tszChannel);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&pptszList, nListCount);
	}
	else
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("HTTP客户端:%s,协议错误"), lpszClientAddr);
	}
	return TRUE;
}