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
bool CALLBACK Network_Callback_CenterLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);                                  //用户连接
void CALLBACK Network_Callback_CenterRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam); //接受到数据
void CALLBACK Network_Callback_CenterLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);                                  //用户离开
void CALLBACK Network_Callback_CenterHeart(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam);                      //心跳超时
//关闭与发送
void XEngine_Network_Close(LPCXSTR lpszClientAddr, bool bHeart);
bool XEngine_Network_Send(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen);