#pragma once
/********************************************************************
//    Created:     2023/07/31  15:17:01
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_HTTPApi\StreamMedia_HTTPApi.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_HTTPApi
//    File Base:   StreamMedia_HTTPApi
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     HTTP管理接口
//    History:
*********************************************************************/
//任务处理相关函数,处理包的内容
bool HTTPApi_Management_Task(LPCXSTR lpszClientAddr, XCHAR*** ppptszListHdr, int nListCount);