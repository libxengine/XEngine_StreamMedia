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
XHTHREAD CALLBACK XEngine_CenterPush_CreateAVThread(XHANDLE xhToken, XENGINE_PROTOCOLDEVICE* pSt_ProtocolDevice, XENGINE_PROTOCOLSTREAM* pSt_ProtocolAVAttr)
{
	TCHAR tszPushAddr[512];

	memset(tszPushAddr, '\0', sizeof(tszPushAddr));
	_stprintf(tszPushAddr, _T("%s/%s_%d_%d"), st_ServiceConfig.tszSMSUrl, pSt_ProtocolDevice->tszDeviceNumber, pSt_ProtocolDevice->nChannel, pSt_ProtocolDevice->bLive);

	if (!XClient_FilePush_Input(xhToken))
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
	XLOG_PRINT(xhLog, XENGINE_HELPCOMPONENTS_XLOG_IN_LOGLEVEL_INFO, _T("推流创建事件,处理完毕,推流地址:%s"), tszPushAddr);
	return 0;
}