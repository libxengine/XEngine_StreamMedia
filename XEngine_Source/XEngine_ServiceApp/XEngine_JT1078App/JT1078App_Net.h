#pragma once
/********************************************************************
//    Created:     2022/04/24  14:05:55
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_JT1078App\JT1078App_Net.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_JT1078App
//    File Base:   JT1078App_Net
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     网络处理
//    History:
*********************************************************************/
bool CALLBACK XEngine_Callback_StreamLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
void CALLBACK XEngine_Callback_StreamRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam);
void CALLBACK XEngine_Callback_StreamLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
void CALLBACK XEngine_Callback_StreamHBLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam);

bool CALLBACK XEngine_Callback_RecordLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
void CALLBACK XEngine_Callback_RecordRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam);
void CALLBACK XEngine_Callback_RecordLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
void CALLBACK XEngine_Callback_RecordHBLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam);

bool XEngine_Net_CloseClient(LPCXSTR lpszClientAddr, XSOCKET hSocket, ENUM_XENGINE_STREAMAPP_DEVICE_TYPE enDeviceType, bool bHBLeave);
bool XEngine_Net_SendMsg(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, ENUM_XENGINE_STREAMAPP_DEVICE_TYPE enDeviceType);
