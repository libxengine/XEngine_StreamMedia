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
			XNETHANDLE xhClient = 0;
			if (ModuleSession_SDKDevice_Get(xhToken, st_MQData.nChannel, st_MQData.bLive, &xhClient))
			{
				XClient_TCPSelect_SendEx(xhClient, st_MQData.ptszMsgBuffer, &st_MQData.nMsgLen);
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	return 0;
}
BOOL XEngine_PluginTask_Handle(XNETHANDLE xhToken, PLUGIN_MQDATA* pSt_MQData)
{
	return TRUE;
}