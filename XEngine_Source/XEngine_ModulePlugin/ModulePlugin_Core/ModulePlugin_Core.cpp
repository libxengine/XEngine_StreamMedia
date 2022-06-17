#include "pch.h"
#include "ModulePlugin_Core.h"
/********************************************************************
//    Created:     2022/06/14  13:41:24
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModulePlugin\ModulePlugin_Core\ModulePlugin_Core.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModulePlugin\ModulePlugin_Core
//    File Base:   ModulePlugin_Core
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     插件核心架构实现
//    History:
*********************************************************************/
CModulePlugin_Core::CModulePlugin_Core()
{
}
CModulePlugin_Core::~CModulePlugin_Core()
{
}
//////////////////////////////////////////////////////////////////////////
//                       公有函数
//////////////////////////////////////////////////////////////////////////
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
BOOL CModulePlugin_Core::ModulePlugin_Core_Insert(XNETHANDLE* pxhToken, LPCTSTR lpszPluginFile)
{
	ModulePlugin_IsErrorOccur = FALSE;

	if ((NULL == pxhToken) || (NULL == lpszPluginFile))
	{
		ModulePlugin_IsErrorOccur = TRUE;
		ModulePlugin_dwErrorCode = ERROR_XENGINE_APISERVICE_MODULE_PLUGIN_PARAMENT;
		return FALSE;
	}
	PLUGINCORE_FRAMEWORK st_FrameWork;
	st_FrameWork.mhFile = 0;
	memset(st_FrameWork.tszModuleFile, '\0', sizeof(st_FrameWork.tszModuleFile));
	//打开一个模块
#ifdef _MSC_BUILD
	st_FrameWork.mhFile = LoadLibrary(lpszPluginFile);
#else
	st_FrameWork.mhFile = dlopen(lpszPluginFile, RTLD_LAZY);
#endif
	if (NULL == st_FrameWork.mhFile)
	{
		ModulePlugin_IsErrorOccur = TRUE;
		ModulePlugin_dwErrorCode = ERROR_XENGINE_APISERVICE_MODULE_PLUGIN_OPENDL;
		return FALSE;
	}
	//开始查找模块中的函数
#ifdef _MSC_BUILD
	st_FrameWork.fpCall_PluginCore_Init = (FPCall_PluginCore_Init)GetProcAddress(st_FrameWork.mhFile, "PluginCore_Init");
#else
	* (void**)(&st_FrameWork.fpCall_PluginCore_Init) = dlsym(st_FrameWork.mhFile, _T("PluginCore_Init"));
#endif
	if (NULL == st_FrameWork.fpCall_PluginCore_Init)
	{
#ifdef _MSC_BUILD
		FreeLibrary(st_FrameWork.mhFile);
#else
		dlclose(st_FrameWork.mhFile);
#endif
		ModulePlugin_IsErrorOccur = TRUE;
		ModulePlugin_dwErrorCode = ERROR_XENGINE_APISERVICE_MODULE_PLUGIN_FPINIT;
		return FALSE;
	}
	//卸载
#ifdef _MSC_BUILD
	st_FrameWork.fpCall_PluginCore_UnInit = (FPCall_PluginCore_UnInit)GetProcAddress(st_FrameWork.mhFile, "PluginCore_UnInit");
#else
	* (void**)(&st_FrameWork.fpCall_PluginCore_UnInit) = dlsym(st_FrameWork.mhFile, _T("PluginCore_UnInit"));
#endif
	if (NULL == st_FrameWork.fpCall_PluginCore_UnInit)
	{
#ifdef _MSC_BUILD
		FreeLibrary(st_FrameWork.mhFile);
#else
		dlclose(st_FrameWork.mhFile);
#endif
		ModulePlugin_IsErrorOccur = TRUE;
		ModulePlugin_dwErrorCode = ERROR_XENGINE_APISERVICE_MODULE_PLUGIN_FPUNINIT;
		return FALSE;
	}
	//播放
#ifdef _MSC_BUILD
	st_FrameWork.fpCall_PluginCore_Play = (FPCall_PluginCore_Play)GetProcAddress(st_FrameWork.mhFile, "PluginCore_Play");
#else
	* (void**)(&st_FrameWork.fpCall_PluginCore_Play) = dlsym(st_FrameWork.mhFile, _T("PluginCore_Play"));
#endif
	if (NULL == st_FrameWork.fpCall_PluginCore_Play)
	{
#ifdef _MSC_BUILD
		FreeLibrary(st_FrameWork.mhFile);
#else
		dlclose(st_FrameWork.mhFile);
#endif
		ModulePlugin_IsErrorOccur = TRUE;
		ModulePlugin_dwErrorCode = ERROR_XENGINE_APISERVICE_MODULE_PLUGIN_FPPLAY;
		return FALSE;
	}
	//停止
#ifdef _MSC_BUILD
	st_FrameWork.fpCall_PluginCore_Stop = (FPCall_PluginCore_Stop)GetProcAddress(st_FrameWork.mhFile, "PluginCore_Stop");
#else
	* (void**)(&st_FrameWork.fpCall_PluginCore_Stop) = dlsym(st_FrameWork.mhFile, _T("PluginCore_Stop"));
#endif
	if (NULL == st_FrameWork.fpCall_PluginCore_Stop)
	{
#ifdef _MSC_BUILD
		FreeLibrary(st_FrameWork.mhFile);
#else
		dlclose(st_FrameWork.mhFile);
#endif
		ModulePlugin_IsErrorOccur = TRUE;
		ModulePlugin_dwErrorCode = ERROR_XENGINE_APISERVICE_MODULE_PLUGIN_FPSTOP;
		return FALSE;
	}
	//回调
#ifdef _MSC_BUILD
	st_FrameWork.fpCall_PluginCore_SetCall = (FPCall_PluginCore_SetCall)GetProcAddress(st_FrameWork.mhFile, "PluginCore_SetCall");
#else
	* (void**)(&st_FrameWork.fpCall_PluginCore_SetCall) = dlsym(st_FrameWork.mhFile, _T("PluginCore_SetCall"));
#endif
	if (NULL == st_FrameWork.fpCall_PluginCore_SetCall)
	{
#ifdef _MSC_BUILD
		FreeLibrary(st_FrameWork.mhFile);
#else
		dlclose(st_FrameWork.mhFile);
#endif
		ModulePlugin_IsErrorOccur = TRUE;
		ModulePlugin_dwErrorCode = ERROR_XENGINE_APISERVICE_MODULE_PLUGIN_FPCALL;
		return FALSE;
	}
	//错误
#ifdef _MSC_BUILD
	st_FrameWork.fpCall_PluginCore_GetLastError = (FPCall_PluginCore_GetLastError)GetProcAddress(st_FrameWork.mhFile, _T("PluginCore_GetLastError"));
#else
	* (void**)(&st_FrameWork.fpCall_PluginCore_GetLastError) = dlsym(st_FrameWork.mhFile, _T("PluginCore_GetLastError"));
#endif
	if (NULL == st_FrameWork.fpCall_PluginCore_GetLastError)
	{
#ifdef _MSC_BUILD
		FreeLibrary(st_FrameWork.mhFile);
#else
		dlclose(st_FrameWork.mhFile);
#endif
		ModulePlugin_IsErrorOccur = TRUE;
		ModulePlugin_dwErrorCode = ERROR_XENGINE_APISERVICE_MODULE_PLUGIN_FPERROR;
		return FALSE;
	}
	_tcscpy(st_FrameWork.tszModuleFile, lpszPluginFile);

	BaseLib_OperatorHandle_Create(pxhToken);
	//添加模块属性到STL函数中
	st_Locker.lock();
	stl_MapFrameWork.insert(make_pair(*pxhToken, st_FrameWork));
	st_Locker.unlock();
	return TRUE;
}
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
BOOL CModulePlugin_Core::ModulePlugin_Core_Delete(XNETHANDLE xhToken)
{
	ModulePlugin_IsErrorOccur = FALSE;

	//清理STL元素空间
	st_Locker.lock();
	unordered_map<XNETHANDLE, PLUGINCORE_FRAMEWORK>::iterator stl_MapIterator = stl_MapFrameWork.find(xhToken);
	if (stl_MapIterator != stl_MapFrameWork.end())
	{
#ifdef _MSC_BUILD
		FreeLibrary(stl_MapIterator->second.mhFile);
#else
		dlclose(stl_MapIterator->second.mhFile);
#endif
	}
	st_Locker.unlock();
	return TRUE;
}
/********************************************************************
函数名称：ModulePlugin_Core_SetCall
函数功能：设置回调函数
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要设置的句柄
 参数.二：fpCall_AVData
  In/Out：In/Out
  类型：回调函数
  可空：N
  意思：数据返回的内容
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
BOOL CModulePlugin_Core::ModulePlugin_Core_SetCall(XNETHANDLE xhToken, CALLBACK_STREAMMEDIA_PLUGIN_AVDATA fpCall_AVData, LPVOID lParam /* = NULL */)
{
	ModulePlugin_IsErrorOccur = FALSE;

	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, PLUGINCORE_FRAMEWORK>::const_iterator stl_MapIterator = stl_MapFrameWork.find(xhToken);
	if (stl_MapIterator == stl_MapFrameWork.end())
	{
		ModulePlugin_IsErrorOccur = TRUE;
		ModulePlugin_dwErrorCode = ERROR_XENGINE_APISERVICE_MODULE_PLUGIN_NOTFOUND;
		st_Locker.unlock_shared();
		return FALSE;
	}
	stl_MapIterator->second.fpCall_PluginCore_SetCall(fpCall_AVData, lParam);
	st_Locker.unlock_shared();
	return TRUE;
}
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
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModulePlugin_Core::ModulePlugin_Core_Init(XNETHANDLE xhToken, LPCTSTR lpszAddr, int nPort, LPCTSTR lpszUser, LPCTSTR lpszPass)
{
    ModulePlugin_IsErrorOccur = FALSE;

	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, PLUGINCORE_FRAMEWORK>::iterator stl_MapIterator = stl_MapFrameWork.find(xhToken);
	if (stl_MapIterator == stl_MapFrameWork.end())
	{
		ModulePlugin_IsErrorOccur = TRUE;
		ModulePlugin_dwErrorCode = ERROR_XENGINE_APISERVICE_MODULE_PLUGIN_NOTFOUND;
		st_Locker.unlock_shared();
		return FALSE;
	}
	BOOL bRet = stl_MapIterator->second.fpCall_PluginCore_Init(&stl_MapIterator->second.xhModule, lpszAddr, nPort, lpszUser, lpszPass);
	st_Locker.unlock_shared();
	return bRet;
}
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
BOOL CModulePlugin_Core::ModulePlugin_Core_UnInit(XNETHANDLE xhToken)
{
	ModulePlugin_IsErrorOccur = FALSE;

	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, PLUGINCORE_FRAMEWORK>::const_iterator stl_MapIterator = stl_MapFrameWork.find(xhToken);
	if (stl_MapIterator == stl_MapFrameWork.end())
	{
		ModulePlugin_IsErrorOccur = TRUE;
		ModulePlugin_dwErrorCode = ERROR_XENGINE_APISERVICE_MODULE_PLUGIN_NOTFOUND;
		st_Locker.unlock_shared();
		return FALSE;
	}
	BOOL bRet = stl_MapIterator->second.fpCall_PluginCore_UnInit(stl_MapIterator->second.xhModule);
	st_Locker.unlock_shared();
	return bRet;
}
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
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModulePlugin_Core::ModulePlugin_Core_Play(XNETHANDLE xhToken, int nChannel)
{
	ModulePlugin_IsErrorOccur = FALSE;

	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, PLUGINCORE_FRAMEWORK>::const_iterator stl_MapIterator = stl_MapFrameWork.find(xhToken);
	if (stl_MapIterator == stl_MapFrameWork.end())
	{
		ModulePlugin_IsErrorOccur = TRUE;
		ModulePlugin_dwErrorCode = ERROR_XENGINE_APISERVICE_MODULE_PLUGIN_NOTFOUND;
		st_Locker.unlock_shared();
		return FALSE;
	}
	BOOL bRet = stl_MapIterator->second.fpCall_PluginCore_Play(stl_MapIterator->second.xhModule, nChannel);
	st_Locker.unlock_shared();
	return bRet;
}
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
BOOL CModulePlugin_Core::ModulePlugin_Core_Stop(XNETHANDLE xhToken, int nChannel)
{
	ModulePlugin_IsErrorOccur = FALSE;

	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, PLUGINCORE_FRAMEWORK>::const_iterator stl_MapIterator = stl_MapFrameWork.find(xhToken);
	if (stl_MapIterator == stl_MapFrameWork.end())
	{
		ModulePlugin_IsErrorOccur = TRUE;
		ModulePlugin_dwErrorCode = ERROR_XENGINE_APISERVICE_MODULE_PLUGIN_NOTFOUND;
		st_Locker.unlock_shared();
		return FALSE;
	}
	BOOL bRet = stl_MapIterator->second.fpCall_PluginCore_Stop(stl_MapIterator->second.xhModule, nChannel);
	st_Locker.unlock_shared();
	return bRet;
}