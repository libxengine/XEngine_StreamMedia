#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2022/04/28  15:15:37
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_CenterApp\XEngine_CenterPush.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_CenterApp
//    File Base:   XEngine_CenterPush
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     推流代码
//    History:
*********************************************************************/
XHTHREAD CALLBACK XEngine_CenterPush_CreateAVThread(XENGINE_PROTOCOLDEVICE* pSt_ProtocolDevice, XENGINE_PROTOCOLSTREAM* pSt_ProtocolAVAttr)
{
	XNETHANDLE xhToken = 0;
	TCHAR tszPushAddr[512];

	memset(tszPushAddr, '\0', sizeof(tszPushAddr));
	_stprintf(tszPushAddr, _T("%s/%s_%d_%d"), st_ServiceConfig.tszSMSUrl, pSt_ProtocolDevice->tszDeviceNumber, pSt_ProtocolDevice->nChannel, pSt_ProtocolDevice->bLive);

	if (!XClient_FilePush_Init(&xhToken))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("推流创建事件,处理创建流消息失败,设备ID：%s,设备通道：%d,流类型：%d,错误：%X"), pSt_ProtocolDevice->tszDeviceNumber, pSt_ProtocolDevice->nChannel, pSt_ProtocolDevice->bLive, StreamClient_GetLastError());
		return 0;
	}
	if (!XClient_FilePush_Input(xhToken, NULL, NULL, FramePush_Stream_CBVideo, NULL, pSt_ProtocolDevice))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("推流创建事件,处理创建输入流消息失败,设备ID：%s,设备通道：%d,流类型：%d,错误：%X"), pSt_ProtocolDevice->tszDeviceNumber, pSt_ProtocolDevice->nChannel, pSt_ProtocolDevice->bLive, StreamClient_GetLastError());
		return 0;
	}
	if (!XClient_FilePush_Output(xhToken, tszPushAddr))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("推流创建事件,处理创建输出流消息失败,设备ID：%s,设备通道：%d,流类型：%d,错误：%X"), pSt_ProtocolDevice->tszDeviceNumber, pSt_ProtocolDevice->nChannel, pSt_ProtocolDevice->bLive, StreamClient_GetLastError());
		return 0;
	}
	if (!XClient_FilePush_Start(xhToken))
	{
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("推流创建事件,处理创建启动流消息失败,设备ID：%s,设备通道：%d,流类型：%d,错误：%X"), pSt_ProtocolDevice->tszDeviceNumber, pSt_ProtocolDevice->nChannel, pSt_ProtocolDevice->bLive, StreamClient_GetLastError());
		return 0;
	}
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("推流创建事件,处理完毕,推流地址:%s,此流不带音频"), tszPushAddr);
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////
int FramePush_Stream_CBVideo(LPVOID lParam, uint8_t* puszMsgBuffer, int nSize)
{
	XENGINE_PROTOCOLDEVICE* pSt_ProtocolDevice = (XENGINE_PROTOCOLDEVICE*)lParam;

	int nMsgLen = 4096;
	time_t nTimeStart = time(NULL);
	TCHAR tszMsgBuffer[4096];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	while (1)
	{
		if (!ModuleSession_Server_Get(pSt_ProtocolDevice->tszDeviceNumber, pSt_ProtocolDevice->nChannel, pSt_ProtocolDevice->bLive, tszMsgBuffer, &nMsgLen))
		{
			if (-1 == nMsgLen)
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("视频流,设备ID：%s,设备通道：%d,流类型：%d,读取错误,设备通道已经被关闭,无法继续"), pSt_ProtocolDevice->tszDeviceNumber, pSt_ProtocolDevice->nChannel, pSt_ProtocolDevice->bLive);
				break;
			}
			if ((time(NULL) - nTimeStart) > 5)
			{
				nMsgLen = -1;
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("视频流,设备ID：%s,设备通道：%d,流类型：%d,读取错误,获取流失败,数据队列为空,无法继续"), pSt_ProtocolDevice->tszDeviceNumber, pSt_ProtocolDevice->nChannel, pSt_ProtocolDevice->bLive);
				break;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}
		nTimeStart = time(NULL);
		memcpy(puszMsgBuffer, tszMsgBuffer, nMsgLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("视频流,设备ID：%s,设备通道：%d,流类型：%d,推送到NGINX成功,大小：%d"), pSt_ProtocolDevice->tszDeviceNumber, pSt_ProtocolDevice->nChannel, pSt_ProtocolDevice->bLive, nMsgLen);
		break;
	}
	return nMsgLen;
}
int FramePush_Stream_CBAudio(LPVOID lParam, uint8_t* puszMsgBuffer, int nSize)
{
	XENGINE_PROTOCOLDEVICE* pSt_ProtocolDevice = (XENGINE_PROTOCOLDEVICE*)lParam;

	int nMsgLen = 4096;
	time_t nTimeStart = time(NULL);
	TCHAR tszMsgBuffer[4096];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	while (1)
	{
		if (!ModuleSession_Server_Get(pSt_ProtocolDevice->tszDeviceNumber, pSt_ProtocolDevice->nChannel, pSt_ProtocolDevice->bLive, tszMsgBuffer, &nMsgLen))
		{
			if (-1 == nMsgLen)
			{
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("音频流,设备ID：%s,设备通道：%d,流类型：%d,读取错误,设备通道已经被关闭,无法继续"), pSt_ProtocolDevice->tszDeviceNumber, pSt_ProtocolDevice->nChannel, pSt_ProtocolDevice->bLive);
				break;
			}
			if ((time(NULL) - nTimeStart) > 5)
			{
				nMsgLen = -1;
				XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_ERROR, _T("音频流,设备ID：%s,设备通道：%d,流类型：%d,读取错误,获取流失败,数据队列为空,无法继续"), pSt_ProtocolDevice->tszDeviceNumber, pSt_ProtocolDevice->nChannel, pSt_ProtocolDevice->bLive);
				break;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}
		nTimeStart = time(NULL);
		memcpy(puszMsgBuffer, tszMsgBuffer, nMsgLen);
		XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_DEBUG, _T("音频流,设备ID：%s,设备通道：%d,流类型：%d,推送到NGINX成功,大小：%d"), pSt_ProtocolDevice->tszDeviceNumber, pSt_ProtocolDevice->nChannel, pSt_ProtocolDevice->bLive, nMsgLen);
		break;
	}
	return nMsgLen;
}