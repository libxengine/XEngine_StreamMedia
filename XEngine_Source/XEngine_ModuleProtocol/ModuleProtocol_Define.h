﻿#pragma once
/********************************************************************
//    Created:     2022/03/01  09:27:15
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_Define.h
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleProtocol
//    File Base:   ModuleProtocol_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出协议模块定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
///                        导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD ModuleProtocol_GetLastError(int *pInt_SysError = NULL);
/************************************************************************/
/*                         导出的协议封装函数                           */
/************************************************************************/
/********************************************************************
函数名称：ModuleProtocol_Packet_Comm
函数功能：通用打包回复函数
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出协议缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出缓冲区大小
 参数.三：nCode
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入回复的CODE
 参数.四：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要处理的缓冲区
 参数.五：lpszToken
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入打包的句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleProtocol_Packet_Comm(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, int nCode = 0, LPCTSTR lpszMsgBuffer = NULL, LPCTSTR lpszToken = NULL);
/********************************************************************
函数名称：ModuleProtocol_Packet_Create
函数功能：流媒体创建
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出协议缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出缓冲区大小
 参数.三：pSt_ProtocolDevice
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入设备信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleProtocol_Packet_Create(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLDEVICE* pSt_ProtocolDevice);
/********************************************************************
函数名称：ModuleProtocol_Packet_Push
函数功能：推流协议
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出协议缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出缓冲区大小
 参数.三：pSt_ProtocolDevice
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入设备信息
 参数.四：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：推流数据大小
 参数.五：nMsgType
  In/Out：In
  类型：整数型
  可空：N
  意思：消息类型,0视频,1音频
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleProtocol_Packet_Push(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLDEVICE * pSt_ProtocolDevice, int nMsgLen, int nMsgType);
/********************************************************************
函数名称：ModuleProtocol_Packet_Destroy
函数功能：销毁流协议
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出协议缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出缓冲区大小
 参数.三：pSt_ProtocolDev
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要销毁的流信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleProtocol_Packet_Destroy(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLDEVICE* pSt_ProtocolDev);
/************************************************************************/
/*                         导出的协议解析函数                           */
/************************************************************************/
/********************************************************************
函数名称：ModuleProtocol_Parse_HTTPForward
函数功能：转发解析
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的内容
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入解析大小
 参数.三：ptszAVUrl
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出解析到的URL
 参数.四：pxhToken
  In/Out：Out
  类型：字符指针
  可空：Y
  意思：输出解析的TOKEN
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModuleProtocol_Parse_HTTPForward(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR * ptszAVUrl, TCHAR * ptszToken = NULL);