#pragma once
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
bool XCALLBACK Network_Callback_HttpLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);                                  //用户连接
void XCALLBACK Network_Callback_HttpRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam); //接受到数据
void XCALLBACK Network_Callback_HttpLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);                                  //用户离开
void XCALLBACK Network_Callback_HttpHeart(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam);                      //心跳超时
//X推流相关
bool XCALLBACK Network_Callback_XStreamLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);                                  //用户连接
void XCALLBACK Network_Callback_XStreamRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam); //接受到数据
void XCALLBACK Network_Callback_XStreamLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);                                  //用户离开
void XCALLBACK Network_Callback_XStreamHeart(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam);                      //心跳超时
//RTMP推流相关
bool XCALLBACK Network_Callback_RTMPLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);                             
void XCALLBACK Network_Callback_RTMPRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam);
void XCALLBACK Network_Callback_RTMPLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);                          
void XCALLBACK Network_Callback_RTMPHeart(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam);             
//JT1078
bool XCALLBACK Network_Callback_JT1078Login(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
void XCALLBACK Network_Callback_JT1078Recv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam);
void XCALLBACK Network_Callback_JT1078Leave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
void XCALLBACK Network_Callback_JT1078HBLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam);
//SRT
bool XCALLBACK Network_Callback_SRTLogin(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
void XCALLBACK Network_Callback_SRTRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam);
void XCALLBACK Network_Callback_SRTLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, XPVOID lParam);
//UDP
void XCALLBACK Network_Callback_VideoRTPRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam);
void XCALLBACK Network_Callback_VideoRTCPRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam);
void XCALLBACK Network_Callback_AudioRTPRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam);
void XCALLBACK Network_Callback_AudioRTCPRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam);
//WEBRTC
void XCALLBACK Network_Callback_RTCWhepRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam);
void XCALLBACK Network_Callback_RTCWhipRecv(LPCXSTR lpszClientAddr, XSOCKET hSocket, LPCXSTR lpszRecvMsg, int nMsgLen, XPVOID lParam);
void XCALLBACK Network_Callback_RTCWhepLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam);
void XCALLBACK Network_Callback_RTCWhipLeave(LPCXSTR lpszClientAddr, XSOCKET hSocket, int nStatus, XPVOID lParam);
//关闭与发送
void XEngine_Network_Close(LPCXSTR lpszClientAddr, XSOCKET hSocket, bool bHeart, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enClientType);
bool XEngine_Network_Send(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enClientType);