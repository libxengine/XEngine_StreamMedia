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
				//得到一个指定客户端的完整数据包
				if (HelpComponents_Datas_GetMemoryEx(xhCenterPacket, ppSst_ListAddr[i]->tszClientAddr, &ptszMsgBuffer, &nMsgLen, &st_ProtocolHdr))
				{
					//在另外一个函数里面处理数据
					XEngine_CenterTask_Handle(&st_ProtocolHdr, ppSst_ListAddr[i]->tszClientAddr, ptszMsgBuffer, nMsgLen);
					//释放内存
					BaseLib_OperatorMemory_FreeCStyle((VOID**)&ptszMsgBuffer);
				}
			}
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSst_ListAddr, nListCount);
	}
	return 0;
}
BOOL XEngine_CenterTask_Handle(XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen)
{
	if (ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_HEARTBEAT == pSt_ProtocolHdr->unOperatorType)
	{
		if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_HB_SYN == pSt_ProtocolHdr->unOperatorCode)
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("业务客户端：%s,接受流媒体返回的心跳"), lpszClientAddr);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("业务客户端：%s,接受返回了无法处理的心跳协议类型"), lpszClientAddr, pSt_ProtocolHdr->unOperatorCode);
		}
	}
	else if (ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_SMS == pSt_ProtocolHdr->unOperatorType)
	{
		if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REQCREATE == pSt_ProtocolHdr->unOperatorCode)
		{
			XENGINE_PROTOCOLDEVICE* pSt_ProtocolDevice = new XENGINE_PROTOCOLDEVICE;
			XENGINE_PROTOCOLSTREAM* pSt_ProtocolAVAttr = new XENGINE_PROTOCOLSTREAM;

			memset(pSt_ProtocolDevice, '\0', sizeof(XENGINE_PROTOCOLDEVICE));
			memset(pSt_ProtocolAVAttr, '\0', sizeof(XENGINE_PROTOCOLSTREAM));

			memcpy(pSt_ProtocolDevice, lpszMsgBuffer, sizeof(XENGINE_PROTOCOLDEVICE));
			//创建会话
			if (!ModuleSession_Server_Create(pSt_ProtocolDevice->tszDeviceNumber, pSt_ProtocolDevice->nChannel, pSt_ProtocolDevice->bLive))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端：%s,创建会话失败,设备ID：%s,设备通道：%d,流类型：%d,错误：%X"), lpszClientAddr, pSt_ProtocolDevice->tszDeviceNumber, pSt_ProtocolDevice->nChannel, pSt_ProtocolDevice->bLive, ModuleSession_GetLastError());
				return FALSE;
			}
			//是否启用音视频数据信息
			if (st_ServiceConfig.st_XSql.bEnable)
			{
				//查询音视频数据,没有将无法使用特性
				if (ModuleDB_AVInfo_InfoQuery(pSt_ProtocolAVAttr))
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端：%s,设备ID：%s,通道：%d,从数据库缓存获取音视频属性成功！"), lpszClientAddr, pSt_ProtocolDevice->tszDeviceNumber, pSt_ProtocolDevice->nChannel);
				}
				else
				{
					XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("业务客户端：%s,设备ID：%s,通道：%d,音视频数据不存在数据库,请插入！"), lpszClientAddr, pSt_ProtocolDevice->tszDeviceNumber, pSt_ProtocolDevice->nChannel);
				}
			}
			else
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("业务客户端：%s,设备ID：%s,通道：%d,没有启用音视频数据库,可能无法处理音频数据"), lpszClientAddr, pSt_ProtocolDevice->tszDeviceNumber, pSt_ProtocolDevice->nChannel);
			}
			//开始创建音视频
			std::thread m_ThreadAV(XEngine_CenterPush_CreateAVThread, pSt_ProtocolDevice, pSt_ProtocolAVAttr);
			m_ThreadAV.detach();
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端：%s,处理创建流消息成功,设备ID：%s,设备通道：%d,流类型：%d"), lpszClientAddr, pSt_ProtocolDevice->tszDeviceNumber, pSt_ProtocolDevice->nChannel, pSt_ProtocolDevice->bLive);
		}
		else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REQDESTROY == pSt_ProtocolHdr->unOperatorCode)
		{
			XNETHANDLE xhToken;
			XENGINE_PROTOCOLDEVICE st_ProtocolDevice;
			memset(&st_ProtocolDevice, '\0', sizeof(XENGINE_PROTOCOLDEVICE));

			memcpy(&st_ProtocolDevice, lpszMsgBuffer, sizeof(XENGINE_PROTOCOLDEVICE));
			if (ModuleSession_Server_GetPush(st_ProtocolDevice.tszDeviceNumber, st_ProtocolDevice.nChannel, st_ProtocolDevice.bLive, &xhToken))
			{
				XClient_FilePush_Close(xhToken);
			}
			ModuleSession_Server_Destroy(st_ProtocolDevice.tszDeviceNumber, st_ProtocolDevice.nChannel, st_ProtocolDevice.bLive);
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("业务客户端：%s,接受请求了销毁流消息,设备ID：%s,设备通道：%d,流类型：%d"), lpszClientAddr, st_ProtocolDevice.tszDeviceNumber, st_ProtocolDevice.nChannel, st_ProtocolDevice.bLive);
		}
		else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REQPUSH == pSt_ProtocolHdr->unOperatorCode)
		{
			int nPos = 0;
			XENGINE_PROTOCOLDEVICE st_ProtocolDevice;
			memset(&st_ProtocolDevice, '\0', sizeof(XENGINE_PROTOCOLDEVICE));
			memcpy(&st_ProtocolDevice, lpszMsgBuffer, sizeof(XENGINE_PROTOCOLDEVICE));
			nPos = sizeof(XENGINE_PROTOCOLDEVICE);

			if (!ModuleSession_Server_Insert(st_ProtocolDevice.tszDeviceNumber, st_ProtocolDevice.nChannel, st_ProtocolDevice.bLive, lpszMsgBuffer + nPos, nMsgLen - nPos,pSt_ProtocolHdr->wReserve))
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("业务客户端：%s,插入流数据到视频队列失败,设备ID：%s,设备通道：%d,流类型：%d,错误：%X"), lpszClientAddr, st_ProtocolDevice.tszDeviceNumber, st_ProtocolDevice.nChannel, st_ProtocolDevice.bLive, ModuleSession_GetLastError());
				return FALSE;
			}
			if (NULL != pSt_FileVideo)
			{
				fwrite(lpszMsgBuffer + nPos, 1, nMsgLen - nPos, pSt_FileVideo);
			}
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("业务客户端：%s,接受推流数据,设备ID：%s,设备通道：%d,流类型：%d,大小：%d"), lpszClientAddr, st_ProtocolDevice.tszDeviceNumber, st_ProtocolDevice.nChannel, st_ProtocolDevice.bLive, nMsgLen);
		}
		else
		{
			XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_WARN, _T("业务客户端：%s,接受返回了无法处理的推流协议类型"), lpszClientAddr, pSt_ProtocolHdr->unOperatorCode);
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