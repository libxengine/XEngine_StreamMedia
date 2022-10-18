#pragma once
/********************************************************************
//    Created:     2022/10/17  10:59:16
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_ForwardApp\XEngine_HttpTask.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_ForwardApp
//    File Base:   XEngine_HttpTask
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     HTTP任务处理代码
//    History:
*********************************************************************/
//任务处理池,用来获取一个完整包
XHTHREAD CALLBACK XEngine_HTTPTask_Thread(LPVOID lParam);
//任务处理相关函数,处理包的内容
BOOL XEngine_HTTPTask_Handle(RFCCOMPONENTS_HTTP_REQPARAM* pSt_HTTPParam, LPCTSTR lpszClientAddr, LPCTSTR lpszMsgBuffer, int nMsgLen);