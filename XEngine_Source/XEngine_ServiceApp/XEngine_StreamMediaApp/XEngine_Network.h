﻿#pragma once
/********************************************************************
//    Created:     2023/06/04  10:54:45
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\XEngine_Network.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp
//    File Base:   XEngine_Network
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     网络IO相关代码
//    History:
*********************************************************************/
//HTTP相关
bool CALLBACK Network_Callback_HttpLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);                                  //用户连接
void CALLBACK Network_Callback_HttpRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam); //接受到数据
void CALLBACK Network_Callback_HttpLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);                                  //用户离开
void CALLBACK Network_Callback_HttpHeart(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam);                      //心跳超时
//X推流相关
bool CALLBACK Network_Callback_CenterLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);                                  //用户连接
void CALLBACK Network_Callback_CenterRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam); //接受到数据
void CALLBACK Network_Callback_CenterLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);                                  //用户离开
void CALLBACK Network_Callback_CenterHeart(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam);                      //心跳超时
//JT1078
bool CALLBACK Network_Callback_JT1078Login(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
void CALLBACK Network_Callback_JT1078Recv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam);
void CALLBACK Network_Callback_JT1078Leave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
void CALLBACK Network_Callback_JT1078HBLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam);
//关闭与发送
void XEngine_Network_Close(LPCXSTR lpszClientAddr, bool bHeart, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enClientType);
bool XEngine_Network_Send(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enClientType);