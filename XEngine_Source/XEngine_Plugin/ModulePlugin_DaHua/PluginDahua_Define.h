#pragma once
/********************************************************************
//    Created:     2022/06/14  15:20:51
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_Plugin\ModulePlugin_DaHua\PluginDahua_Define.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_Plugin\ModulePlugin_DaHua
//    File Base:   PluginDahua_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     大华导出函数
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                       导出函数定义
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD PluginCore_GetLastError();
/*********************************************************************************
*                                导出函数定义                                    *
*********************************************************************************/
/********************************************************************
函数名称：PluginCore_Init
函数功能：初始化插件模块
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：初始化后的句柄
 参数.二：lpszAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入设备地址
 参数.三：nPort
  In/Out：In
  类型：整数型
  可空：N
  意思：输入端口号
 参数.四：lpszUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入用户名
 参数.五：lpszPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入密码
 参数.六：bDebug
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：是否启用调试
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL PluginCore_Init(XNETHANDLE xhToken, LPCTSTR lpszAddr, int nPort, LPCTSTR lpszUser, LPCTSTR lpszPass, BOOL bPacket = TRUE, BOOL bDebug = FALSE);
/********************************************************************
函数名称：PluginCore_CBSet
函数功能：设置数据回调
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的设备
 参数.二：fpCall_SDKBuffer
  In/Out：In/Out
  类型：回调函数
  可空：N
  意思：数据处理回调
 参数.三：lParam
  In/Out：In/Out
  类型：无类型指针
  可空：Y
  意思：回调函数自定义参数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL PluginCore_CBSet(XNETHANDLE xhToken, CALLBACK_STREAMMEIDA_MODULE_PLUGIN_SDKBUFFER fpCall_SDKBuffer, LPVOID lParam = NULL);
/********************************************************************
函数名称：PluginCore_UnInit
函数功能：卸载插件
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的插件句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL PluginCore_UnInit(XNETHANDLE xhToken);
/********************************************************************
函数名称：PluginCore_Play
函数功能：播放视频
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：要操作的设备
 参数.二：nChannel
  In/Out：In
  类型：整数型
  可空：N
  意思：要操作的通道号
 参数.三：bAudio
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：是否启用音频
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL PluginCore_Play(XNETHANDLE xhToken, int nChannel, BOOL bAudio = FALSE);
/********************************************************************
函数名称：PluginCore_Stop
函数功能：停止播放
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：要操作的设备
 参数.二：nChannel
  In/Out：In
  类型：整数型
  可空：N
  意思：要操作的通道号
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL PluginCore_Stop(XNETHANDLE xhToken, int nChannel);