#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2022/01/20  14:29:47
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp\XEngine_CenterApp\XEngine_CenterTask.cpp
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp\XEngine_CenterApp
//    File Base:   XEngine_CenterTask
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     业务任务处理代码
//    History:
*********************************************************************/
XHTHREAD CALLBACK XEngine_CenterTask_Thread(LPVOID lParam)
{
	//任务池是编号1开始的.
	int nThreadPos = *(int*)lParam;
	nThreadPos++;
	while (bIsRun)
	{
		//等待编号1的任务池触发一个组完包的事件
		if (!HelpComponents_Datas_WaitEventEx(xhCenterPacket, nThreadPos))
		{
			continue;
		}
		int nListCount = 0;
		HELPCOMPONENT_PACKET_CLIENT** ppSst_ListAddr;
		//获得编号1的所有待处理任务的客户端列表(也就是客户端发送过来的数据已经组好了一个包,需要我们处理)
		HelpComponents_Datas_GetPoolEx(xhCenterPacket, nThreadPos, &ppSst_ListAddr, &nListCount);
		//先循环客户端
		for (int i = 0; i < nListCount; i++)
		{
			//再循环客户端拥有的任务个数
			for (int j = 0; j < ppSst_ListAddr[i]->nPktCount; j++)
			{
				int nMsgLen = 0;                             //客户端发送的数据大小,不包括头
				TCHAR* ptszMsgBuffer = NULL;                 //客户端发送的数据
				XENGINE_PROTOCOLHDR st_ProtocolHdr;          //客户端发送的数据的协议头

				memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
				//得到一个指定客户端的完整数据包
				if (!HelpComponents_Datas_GetMemoryEx(xhCenterPacket, ppSst_ListAddr[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端:%s,获取数据包失败,错误：%lX"), ppSst_ListAddr[i]->tszClientAddr, Packets_GetLastError());
					continue;
				}
				//在另外一个函数里面处理数据
				XEngine_CenterTask_Handle(&st_ProtocolHdr, ppSst_ListAddr[i]->tszClientAddr, ptszMsgBuffer, nMsgLen);
				//释放内存
				BaseLib_OperatorMemory_FreeCStyle((VOID**)&ptszMsgBuffer);
			}
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSst_ListAddr, nListCount);
	}
	return 0;
}
BOOL XEngine_CenterTask_Handle(XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	//这里开始编写你的代码
	if (ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_MSG == pSt_ProtocolHdr->unOperatorType)
	{
		if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_MSG_TEXTREQ == pSt_ProtocolHdr->unOperatorCode)
		{
			//我们收到一个包可以对他进行回复
			TCHAR tszMsgBuffer[2048];
			memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
			//我们推荐你新建一个模块项目来处理协议组包和解包相关代码
			pSt_ProtocolHdr->byIsReply = FALSE;
			pSt_ProtocolHdr->unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_MSG_TEXTREP;

			memcpy(tszMsgBuffer, pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
			memcpy(tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), lpszMsgBuffer, nMsgLen);
			//发送业务包,对方发送的内容我们返回相同的内容给对方,所以不需要改负载大小
			XEngine_Network_Send(lpszClientAddr, tszMsgBuffer, sizeof(XENGINE_PROTOCOLHDR) + nMsgLen);
			//回复完毕打印客户端发送的数据
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端:%s,发送普通包,大小:%d,内容:%s"), lpszClientAddr, nMsgLen, lpszMsgBuffer);
		}
	}
	else if (ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_AUTH == pSt_ProtocolHdr->unOperatorType)
	{
		//比如你想进行用户验证,可以编写你的代码
		if (0 == pSt_ProtocolHdr->unOperatorCode)
		{

		}
	}
	else
	{
		//我们可以给客户端发送一条错误信息
		pSt_ProtocolHdr->wReserve = 0xFF;        //表示不支持的协议
		pSt_ProtocolHdr->unPacketSize = 0;       //设置没有后续数据包
		XEngine_Network_Send(lpszClientAddr, (LPCTSTR)pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("业务客户端:%s,主协议错误,协议：%x 不支持"), lpszClientAddr, pSt_ProtocolHdr->unOperatorType);
	}
	return TRUE;
}