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
XHTHREAD CALLBACK XEngine_PluginTask_Thread(XNETHANDLE xhDevice, int nThread)
{
	int nMsgLen = 0;
	TCHAR tszMsgBuffer[4096];

	while (bIsRun)
	{
		PLUGIN_MQDATA st_MQData;
		memset(&st_MQData, '\0', sizeof(PLUGIN_MQDATA));

		if (ModulePlugin_Core_GetData(xhDevice, nThread, &st_MQData))
		{
			XNETHANDLE xhClient = 0;
			if (ModuleSession_SDKDevice_GetClient(xhDevice, st_MQData.nChannel, st_MQData.bLive, &xhClient))
			{
				XENGINE_PROTOCOLDEVICE st_ProtocolDevice;
				memset(&st_ProtocolDevice, '\0', sizeof(XENGINE_PROTOCOLDEVICE));

				st_ProtocolDevice.bLive = TRUE;
				st_ProtocolDevice.nChannel = st_MQData.nChannel;
				_stprintf(st_ProtocolDevice.tszDeviceNumber, _T("%lld"), xhDevice);

				ModuleProtocol_Stream_Push(tszMsgBuffer, &nMsgLen, &st_ProtocolDevice, st_MQData.tszMsgBuffer, st_MQData.nMsgLen, 0);
				XClient_TCPSelect_SendEx(xhClient, tszMsgBuffer, &nMsgLen);
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return 0;
}