#pragma once
/********************************************************************
//    Created:     2022/01/20  14:29:55
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp\XEngine_CenterApp\XEngine_CenterTask.h
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ServiceApp\XEngine_CenterApp
//    File Base:   XEngine_CenterTask
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     业务任务处理代码
//    History:
*********************************************************************/
//任务处理池,用来获取一个完整包
XHTHREAD CALLBACK XEngine_CenterTask_Thread(XPVOID lParam);
//任务处理相关函数,处理包的内容
bool XEngine_CenterTask_Handle(XENGINE_PROTOCOLHDR* pSt_ProtocolHdr, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen);