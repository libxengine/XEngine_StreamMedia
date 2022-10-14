#pragma once
/********************************************************************
//    Created:     2022/06/22  10:12:15
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_SDKApp\XEngine_PluginTask.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_SDKApp
//    File Base:   XEngine_PluginTask
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     插件处理代码
//    History:
*********************************************************************/
void CALLBACK XEngine_PluginTask_CBRecv(XNETHANDLE xhToken, int nChannel, BOOL bLive, int nDType, LPCTSTR lpszMsgBuffer, int nMsgLen, LPVOID lParam);