#pragma once
/********************************************************************
//    Created:     2022/06/15  13:48:29
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModulePlugin\ModulePlugin_Define.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModulePlugin
//    File Base:   ModulePlugin_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     插件导出定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                       导出回调函数
//////////////////////////////////////////////////////////////////////////
typedef void(CALLBACK* CALLBACK_STREAMMEIDA_MODULE_PLUGIN_SDKBUFFER)(XNETHANDLE xhToken, int nChannel, BOOL bLive, int nDType, LPCTSTR lpszMsgBuffer, int nMsgLen, LPVOID lParam);
//////////////////////////////////////////////////////////////////////////
//                       导出函数定义
//////////////////////////////////////////////////////////////////////////
extern "C" DWORD ModulePlugin_GetLastError(int* pInt_SysError = NULL);
/*********************************************************************************
*                        插件框架导出函数定义                                    *
*********************************************************************************/
/********************************************************************
函数名称：ModulePlugin_Core_Insert
函数功能：插入一个模块在管理器中
 参数.一：pxhToken
  In/Out：Out
  类型：句柄
  可空：N
  意思：输出插入成功的模块句柄
 参数.二：lpszPluginFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入插件模块路径
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModulePlugin_Core_Insert(XNETHANDLE* pxhToken, LPCTSTR lpszPluginFile);
/********************************************************************
函数名称：ModulePlugin_Core_Delete
函数功能：删除模块
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModulePlugin_Core_Delete(XNETHANDLE xhToken);
/********************************************************************
函数名称：ModulePlugin_Core_Destory
函数功能：销毁
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModulePlugin_Core_Destory();
/********************************************************************
函数名称：ModulePlugin_Core_Init
函数功能：初始化一个设备
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的设备
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
extern "C" BOOL ModulePlugin_Core_Init(XNETHANDLE xhToken, LPCTSTR lpszAddr, int nPort, LPCTSTR lpszUser, LPCTSTR lpszPass, BOOL bDebug = FALSE);
/********************************************************************
函数名称：ModulePlugin_Core_CBSet
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
extern "C" BOOL ModulePlugin_Core_CBSet(XNETHANDLE xhToken, CALLBACK_STREAMMEIDA_MODULE_PLUGIN_SDKBUFFER fpCall_SDKBuffer, LPVOID lParam = NULL);
/********************************************************************
函数名称：ModulePlugin_Core_UnInit
函数功能：卸载设备
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的设备
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModulePlugin_Core_UnInit(XNETHANDLE xhToken);
/********************************************************************
函数名称：ModulePlugin_Core_Play
函数功能：播放
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的设备
 参数.二：nChannel
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要操作的通道
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
extern "C" BOOL ModulePlugin_Core_Play(XNETHANDLE xhToken, int nChannel, BOOL bAudio = FALSE);
/********************************************************************
函数名称：ModulePlugin_Core_Stop
函数功能：停止播放
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的设备
 参数.二：nChannel
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要操作的通道
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" BOOL ModulePlugin_Core_Stop(XNETHANDLE xhToken, int nChannel);