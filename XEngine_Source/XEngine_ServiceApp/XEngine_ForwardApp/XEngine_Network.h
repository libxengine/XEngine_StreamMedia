#pragma once
/********************************************************************
//    Created:     2022/10/17  10:59:22
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_ForwardApp\XEngine_Network.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_ForwardApp
//    File Base:   XEngine_Network
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     网络IO相关代码
//    History:
*********************************************************************/
//HTTP相关
BOOL __stdcall Network_Callback_HttpLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);                                  //用户连接
void __stdcall Network_Callback_HttpRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam); //接受到数据
void __stdcall Network_Callback_HttpLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);                                  //用户离开
void __stdcall Network_Callback_HttpHeart(LPCSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam);                      //心跳超时
//关闭与发送
void XEngine_Network_Close(LPCTSTR lpszClientAddr, BOOL bHeart);
BOOL XEngine_Network_Send(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen);