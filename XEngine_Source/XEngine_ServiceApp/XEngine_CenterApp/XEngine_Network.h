#pragma once
/********************************************************************
//    Created:     2022/01/20  14:33:20
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp\XEngine_CenterApp\XEngine_Network.h
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp\XEngine_CenterApp
//    File Base:   XEngine_Network
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     网络IO相关代码
//    History:
*********************************************************************/
//业务相关
BOOL __stdcall Network_Callback_CenterLogin(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);                                  //用户连接
void __stdcall Network_Callback_CenterRecv(LPCTSTR lpszClientAddr, SOCKET hSocket, LPCTSTR lpszRecvMsg, int nMsgLen, LPVOID lParam); //接受到数据
void __stdcall Network_Callback_CenterLeave(LPCTSTR lpszClientAddr, SOCKET hSocket, LPVOID lParam);                                  //用户离开
void __stdcall Network_Callback_CenterHeart(LPCSTR lpszClientAddr, SOCKET hSocket, int nStatus, LPVOID lParam);                      //心跳超时
//关闭与发送
void XEngine_Network_Close(LPCTSTR lpszClientAddr, BOOL bHeart);
BOOL XEngine_Network_Send(LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen);