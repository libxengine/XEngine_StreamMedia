#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2022/06/22  10:17:15
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_SDKApp\XEngine_PluginTask.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_SDKApp
//    File Base:   XEngine_PluginTask
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     插件处理代码
//    History:
*********************************************************************/
XHTHREAD CALLBACK XEngine_PluginTask_Thread(XNETHANDLE xhToken)
{
	while (bIsRun)
	{
		PLUGIN_MQDATA st_MQData;
		memset(&st_MQData, '\0', sizeof(PLUGIN_MQDATA));

		if (ModulePlugin_Core_GetData(xhToken, &st_MQData))
		{
			XEngine_PluginTask_Handle(xhToken, &st_MQData);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	return 0;
}
BOOL XEngine_PluginTask_Handle(XNETHANDLE xhToken, PLUGIN_MQDATA* pSt_MQData)
{
	XNETHANDLE xhClient = 0;
	if (ModuleSession_SDKDevice_Get(xhToken, pSt_MQData->nChannel, pSt_MQData->bLive, &xhClient))
	{
		int nMsgLen = 0;
		TCHAR tszMsgBuffer[1024];
		XENGINE_PROTOCOLDEVICE st_ProtocolDevice;

		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		memset(&st_ProtocolDevice, '\0', sizeof(XENGINE_PROTOCOLDEVICE));

		st_ProtocolDevice.bLive = TRUE;
		st_ProtocolDevice.nChannel = pSt_MQData->nChannel;
		_stprintf(st_ProtocolDevice.tszDeviceNumber, _T("%lld"), xhToken);

		ModuleProtocol_JT1078_StreamPush(tszMsgBuffer, &nMsgLen, &st_ProtocolDevice, pSt_MQData->ptszMsgBuffer, pSt_MQData->nMsgLen, 0);
		XClient_TCPSelect_SendEx(xhClient, tszMsgBuffer, &nMsgLen);
		BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&pSt_MQData->ptszMsgBuffer);
	}
	return TRUE;
}