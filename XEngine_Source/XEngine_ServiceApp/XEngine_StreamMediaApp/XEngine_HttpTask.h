#pragma once
/********************************************************************
//    Created:     2023/06/04  10:54:28
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\XEngine_HttpTask.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp
//    File Base:   XEngine_HttpTask
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     HTTP任务处理代码
//    History:
*********************************************************************/
//任务处理池,用来获取一个完整包
XHTHREAD XCALLBACK XEngine_HTTPTask_Thread(XPVOID lParam);
//任务处理相关函数,处理包的内容
bool XEngine_HTTPTask_Handle(RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR*** ppptszHDRList, int nHDRCount);