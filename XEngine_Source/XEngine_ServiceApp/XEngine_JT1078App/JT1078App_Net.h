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
BOOL CALLBACK XEngine_Callback_StreamLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);
void CALLBACK XEngine_Callback_StreamRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam);
void CALLBACK XEngine_Callback_StreamLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);
void CALLBACK XEngine_Callback_StreamHBLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam);

BOOL CALLBACK XEngine_Callback_RecordLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);
void CALLBACK XEngine_Callback_RecordRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam);
void CALLBACK XEngine_Callback_RecordLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);
void CALLBACK XEngine_Callback_RecordHBLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam);

BOOL XEngine_Net_CloseClient(LPCTSTR lpszClientAddr, SOCKET hSocket, ENUM_XENGINE_STREAMAPP_DEVICE_TYPE enDeviceType, BOOL bHBLeave);
BOOL XEngine_Net_SendMsg(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen, ENUM_XENGINE_STREAMAPP_DEVICE_TYPE enDeviceType);
