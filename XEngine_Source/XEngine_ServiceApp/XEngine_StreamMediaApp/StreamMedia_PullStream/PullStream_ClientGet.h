#pragma once
/********************************************************************
//    Created:     2023/06/08  11:37:23
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PullStream\PullStream_ClientTask.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\StreamMedia_PullStream
//    File Base:   PullStream_ClientTask
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     拉流任务处理函数
//    History:
*********************************************************************/
bool PullStream_ClientGet_FLVPlay(LPCXSTR lpszClientAddr, LPCXSTR lpszPushAddr, XCHAR* ptszSDBuffer, XCHAR* ptszRVBuffer);
bool PullStream_ClientGet_XStreamPlay(LPCXSTR lpszClientAddr, LPCXSTR lpszPushAddr, XCHAR* ptszSDBuffer, XCHAR* ptszRVBuffer);
//任务处理相关函数,处理包的内容
bool PullStream_ClientGet_Handle(LPCXSTR lpszClientAddr, XCHAR*** ppptszListHdr, int nListCount);