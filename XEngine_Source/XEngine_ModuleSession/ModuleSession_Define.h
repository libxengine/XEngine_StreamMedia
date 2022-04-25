﻿#pragma once
/********************************************************************
//    Created:     2022/04/25  10:17:21
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_Define.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession
//    File Base:   ModuleSession_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出会话模块定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                       导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD ModuleSession_GetLastError(int *pInt_SysError = NULL);
/*********************************************************************************
*                          JT1078导出会话模块                                    *
*********************************************************************************/
/********************************************************************
函数名称：ModuleSession_JT1078Stream_InsertDevice
函数功能：创建一个客户端会话
 参数.一：xhClient
  In/Out：In
  类型：句柄
  可空：N
  意思：输入客户端句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleSession_JT1078Client_Create(XNETHANDLE xhClient);
/********************************************************************
函数名称：ModuleSession_JT1078Client_Get
函数功能：获得一个可以使用的客户端句柄
 参数.一：pxhClient
  In/Out：Out
  类型：句柄指针
  可空：N
  意思：输出获得的句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleSession_JT1078Client_Get(XNETHANDLE* pxhClient);
/********************************************************************
函数名称：ModuleSession_JT1078Client_Exist
函数功能：客户端是否存在
 参数.一：pxhClient
  In/Out：Out
  类型：句柄
  可空：N
  意思：输出客户端句柄
 参数.二：lpszDeviceAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入绑定的设备地址
 参数.三：lpszDeviceNumber
  In/Out：Out
  类型：常量字符指针
  可空：N
  意思：输入绑定的设备编号
 参数.四：nChannel
  In/Out：In
  类型：整数型
  可空：N
  意思：输入绑定的通道
 参数.五：bLive
  In/Out：In
  类型：逻辑型
  可空：N
  意思：输入是直播还是录像
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleSession_JT1078Client_Exist(XNETHANDLE* pxhClient, LPCTSTR lpszDeviceAddr, LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive);
/********************************************************************
函数名称：ModuleSession_JT1078Client_Insert
函数功能：绑定插入一个客户端
 参数.一：xhClient
  In/Out：In
  类型：句柄
  可空：N
  意思：输入客户端句柄
 参数.二：lpszDeviceAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入绑定的设备地址
 参数.三：lpszDeviceNumber
  In/Out：Out
  类型：常量字符指针
  可空：N
  意思：输入绑定的设备编号
 参数.四：nChannel
  In/Out：In
  类型：整数型
  可空：N
  意思：输入绑定的通道
 参数.五：bLive
  In/Out：In
  类型：逻辑型
  可空：N
  意思：输入是直播还是录像
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleSession_JT1078Client_Insert(XNETHANDLE xhClient, LPCTSTR lpszDeviceAddr, LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive);
/********************************************************************
函数名称：ModuleSession_JT1078Client_DeleteAddr
函数功能：通过IP地址删除绑定的设备
 参数.一：lpszDeviceAddr
  In/Out：In
  类型：句柄
  可空：N
  意思：输入设备IP地址
 参数.二：pxhClient
  In/Out：Out
  类型：句柄指针
  可空：Y
  意思：输出绑定的句柄
 参数.三：ptszDeviceNumber
  In/Out：Out
  类型：常量字符指针
  可空：Y
  意思：输出设备编号
 参数.四：pInt_Channel
  In/Out：Out
  类型：整数型
  可空：Y
  意思：输出通道号
 参数.五：pbLive
  In/Out：Out
  类型：逻辑型
  可空：Y
  意思：输出直播还是录像
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleSession_JT1078Client_DeleteAddr(LPCTSTR lpszDeviceAddr, XNETHANDLE* pxhClient = NULL, TCHAR* ptszDeviceNumber = NULL, int* pInt_Channel = NULL, BOOL* pbLive = NULL);
/********************************************************************
函数名称：ModuleSession_JT1078Client_DeleteNumber
函数功能：通过设备信息删除绑定信息
 参数.一：lpszDeviceNumber
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入设备编号
 参数.二：nChannel
  In/Out：In
  类型：整数型
  可空：N
  意思：通道号
 参数.三：bLive
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：直播还是录像
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleSession_JT1078Client_DeleteNumber(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive);
/********************************************************************
函数名称：ModuleSession_JT1078Client_Destory
函数功能：销毁客户端会话管理器
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleSession_JT1078Client_Destory();