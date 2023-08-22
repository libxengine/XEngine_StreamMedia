#include "../XEngine_Hdr.h"
/********************************************************************
//    Created:     2023/07/28  16:12:02
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PushStream\PushStream_SrtTask.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PushStream
//    File Base:   PushStream_SrtTask
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     SRT推拉流代码
//    History:
*********************************************************************/
bool PushStream_SrtTask_Connct(LPCXSTR lpszClientAddr, SRTSOCKET hSocket)
{
	bool bPublish = false;
	XCHAR tszSMSAddr[MAX_PATH];
	memset(tszSMSAddr, '\0', sizeof(tszSMSAddr));

	if (!ModuleHelp_SrtCore_GetStreamID(hSocket, tszSMSAddr, &bPublish))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("SRT客户端：获取流ID信息失败,错误码：%lX,错误信息:%s"), ModuleHelp_GetLastError(), srt_getlasterror_str());
		return false;
	}

	if (bPublish)
	{
		//创建会话
		ModuleSession_PushStream_Create(lpszClientAddr, tszSMSAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_SRT);
		//need to parse ts stream
		HLSProtocol_TSParse_Insert(lpszClientAddr);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("SRT客户端：%s,创建流成功,推流地址：%s,类型:推流端"), lpszClientAddr, tszSMSAddr);
	}
	else
	{
		XCHAR tszPushAddr[128];
		memset(tszPushAddr, '\0', sizeof(tszPushAddr));
		//得到推流地址
		if (!ModuleSession_PushStream_FindStream(tszSMSAddr, tszPushAddr))
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _X("SRT客户端：%s,请求拉流的参数不正确:%s,错误:%lX"), lpszClientAddr, tszSMSAddr, ModuleSession_GetLastError());
			return false;
		}
		ModuleSession_PullStream_Insert(lpszClientAddr, tszSMSAddr, tszPushAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_SRT);
		ModuleSession_PushStream_ClientInsert(tszPushAddr, lpszClientAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_SRT);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _X("SRT客户端：%s,拉取流成功,拉流地址：%s,类型:拉流端"), lpszClientAddr, tszSMSAddr);
	}
	return true;
}

bool PushStream_SrtTask_Handle(LPCXSTR lpszClientAddr, SRTSOCKET hSocket, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	HLSProtocol_TSParse_Send(lpszClientAddr, lpszMsgBuffer, nMsgLen);
	//SRT客户端就直接转发
	list<STREAMMEDIA_SESSIONCLIENT> stl_ListClient;
	ModuleSession_PushStream_ClientList(lpszClientAddr, &stl_ListClient);
	for (auto stl_ListIteratorClient = stl_ListClient.begin(); stl_ListIteratorClient != stl_ListClient.end(); ++stl_ListIteratorClient)
	{
		if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_SRT == stl_ListIteratorClient->enClientType)
		{
			XEngine_Network_Send(stl_ListIteratorClient->tszClientID, lpszMsgBuffer, nMsgLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_SRT);
			break;
		}
	}

	return true;
}

XHTHREAD CALLBACK PushStream_SRTTask_Thread(XPVOID lParam)
{
	//任务池是编号1开始的.
	int nThreadPos = *(int*)lParam;
	nThreadPos++;
	while (bIsRun)
	{
		//等待编号1的任务池触发一个组完包的事件
		if (!HLSProtocol_TSParse_WaitEvent(nThreadPos))
		{
			continue;
		}
		int nListCount = 0;
		XENGINE_MANAGEPOOL_TASKEVENT** ppSst_ListAddr;
		//获得编号1的所有待处理任务的客户端列表(也就是客户端发送过来的数据已经组好了一个包,需要我们处理)
		HLSProtocol_TSParse_GetPool(nThreadPos, &ppSst_ListAddr, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			//再循环客户端拥有的任务个数
			for (int j = 0; j < ppSst_ListAddr[i]->nPktCount; j++)
			{
				int nMsgLen = 0;                             //客户端发送的数据大小,不包括头
				XBYTE byAVType = 0;
				XCHAR* ptszMsgBuffer = NULL;                 //客户端发送的数据
				//得到一个指定客户端的完整数据包
				if (HLSProtocol_TSParse_Recv(ppSst_ListAddr[i]->tszClientAddr, ptszMsgBuffer, &nMsgLen, &byAVType))
				{
					//在另外一个函数里面处理数据
					PushStream_SrtTask_ThreadProcess(ppSst_ListAddr[i]->tszClientAddr, ptszMsgBuffer, nMsgLen);
					//释放内存
					BaseLib_OperatorMemory_FreeCStyle((VOID**)&ptszMsgBuffer);
				}
			}
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSst_ListAddr, nListCount);
	}
	return 0;
}
bool PushStream_SrtTask_ThreadProcess(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	
	return true;
}