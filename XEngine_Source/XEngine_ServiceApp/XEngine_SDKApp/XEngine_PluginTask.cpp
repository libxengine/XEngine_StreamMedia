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
void CALLBACK XEngine_PluginTask_CBRecv(XNETHANDLE xhToken, int nChannel, BOOL bLive, int nDType, LPCTSTR lpszMsgBuffer, int nMsgLen, LPVOID lParam)
{
	int nSDLen = 0;
	XNETHANDLE xhClient = 0;
	TCHAR tszMsgBuffer[1024];

	if (ModuleSession_SDKDevice_GetClient(xhToken, nChannel, bLive, &xhClient))
	{
		XENGINE_PROTOCOLDEVICE st_ProtocolDevice;
		memset(&st_ProtocolDevice, '\0', sizeof(XENGINE_PROTOCOLDEVICE));

		st_ProtocolDevice.bLive = TRUE;
		st_ProtocolDevice.nChannel = nChannel;
		_stprintf(st_ProtocolDevice.tszDeviceNumber, _T("%lld"), xhToken);

		ModuleProtocol_Packet_Push(tszMsgBuffer, &nSDLen, &st_ProtocolDevice, nMsgLen, nDType);
		XClient_TCPSelect_SendEx(xhClient, tszMsgBuffer, &nSDLen);
		XClient_TCPSelect_SendEx(xhClient, lpszMsgBuffer, &nMsgLen);
	}
}