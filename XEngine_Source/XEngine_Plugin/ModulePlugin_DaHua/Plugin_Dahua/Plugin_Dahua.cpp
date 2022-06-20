#include "pch.h"
#include "Plugin_Dahua.h"
/********************************************************************
//    Created:     2022/06/14  15:00:12
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_Plugin\ModulePlugin_DaHua\Plugin_Dahua\Plugin_Dahua.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_Plugin\ModulePlugin_DaHua\Plugin_Dahua
//    File Base:   Plugin_Dahua
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     大华SDK实现
//    History:
*********************************************************************/
CPlugin_Dahua::CPlugin_Dahua()
{
	CLIENT_Init(PluginCore_CB_Disconnect, (DWORD)this);
	CLIENT_SetAutoReconnect(PluginCore_CB_AutoConnect, (DWORD)this);
	int nWaitTime = 5000; // 登录请求响应超时时间设置为 5s
	int nTryTimes = 3;    // 登录时尝试建立链接 3 次
	CLIENT_SetConnectTime(nWaitTime, nTryTimes);
}
CPlugin_Dahua::~CPlugin_Dahua()
{
	CLIENT_Cleanup();
}
//////////////////////////////////////////////////////////////////////////
//                       公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：PluginCore_SetCall
函数功能：设置回调函数
 参数.一：fpCall_AVData
  In/Out：In/Out
  类型：回调函数
  可空：N
  意思：数据回调地址
 参数.二：lParam
  In/Out：In/Out
  类型：无类型指针
  可空：N
  意思：回调函数自定义参数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CPlugin_Dahua::PluginCore_SetCall(CALLBACK_STREAMMEDIA_PLUGIN_AVDATA fpCall_AVData, LPVOID lParam /* = NULL */)
{
	SDKPlugin_IsErrorOccur = FALSE;

	m_lParam = lParam;
	lpCall_AVData = fpCall_AVData;
	return TRUE;
}
/********************************************************************
函数名称：PluginCore_Init
函数功能：初始化插件模块
 参数.一：pxhToken
  In/Out：Out
  类型：句柄
  可空：N
  意思：导出初始化后的句柄
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
BOOL CPlugin_Dahua::PluginCore_Init(XNETHANDLE* pxhToken, LPCTSTR lpszAddr, int nPort, LPCTSTR lpszUser, LPCTSTR lpszPass)
{
	SDKPlugin_IsErrorOccur = FALSE;

	if ((NULL == pxhToken) || (NULL == lpszAddr) || (NULL == lpszUser) || (NULL == lpszPass))
	{
		SDKPlugin_IsErrorOccur = TRUE;
		SDKPlugin_dwErrorCode = ERROR_XENGINE_STREAMMEDIA_PLUGIN_MODULE_DH_PARAMENT;
		return FALSE;
	}
	PLUGIN_SDKDAHUA st_SDKDahua;
	memset(&st_SDKDahua, '\0', sizeof(PLUGIN_SDKDAHUA));

	st_SDKDahua.st_DevLoginInfo.dwSize = sizeof(NET_IN_LOGIN_WITH_HIGHLEVEL_SECURITY);
	strncpy(st_SDKDahua.st_DevLoginInfo.szIP, lpszAddr, sizeof(st_SDKDahua.st_DevLoginInfo.szIP) - 1);
	strncpy(st_SDKDahua.st_DevLoginInfo.szPassword, lpszPass, sizeof(st_SDKDahua.st_DevLoginInfo.szPassword) - 1);
	strncpy(st_SDKDahua.st_DevLoginInfo.szUserName, lpszUser, sizeof(st_SDKDahua.st_DevLoginInfo.szUserName) - 1);
	st_SDKDahua.st_DevLoginInfo.nPort = nPort;
	st_SDKDahua.st_DevLoginInfo.emSpecCap = EM_LOGIN_SPEC_CAP_TCP;
	// 登录设备
	st_SDKDahua.hSDKModule = CLIENT_LoginWithHighLevelSecurity(&st_SDKDahua.st_DevLoginInfo, &st_SDKDahua.st_DevOutInfo);
	if (0 == st_SDKDahua.hSDKModule)
	{
		// 根据错误码，可以在 dhnetsdk.h 中找到相应的解释，此处打印的是 16 进制，头文件中是十进制，其中的转换需注意
		// 例如：#define NET_NOT_SUPPORTED_EC(23) // 当前 SDK 未支持该功能，对应的错误码为 0x80000017, 23 对应的 16 进制为 0x17
		SDKPlugin_IsErrorOccur = TRUE;
		SDKPlugin_dwErrorCode = CLIENT_GetLastError();
		return FALSE;
	}

	st_Locker.lock();
	stl_MapManager.insert(make_pair(st_SDKDahua.hSDKModule, st_SDKDahua));
	st_Locker.unlock();
	return TRUE;
}
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
BOOL CPlugin_Dahua::PluginCore_UnInit(XNETHANDLE xhToken)
{
	SDKPlugin_IsErrorOccur = FALSE;

	st_Locker.lock();
	unordered_map<XNETHANDLE, PLUGIN_SDKDAHUA>::const_iterator stl_MapIterator = stl_MapManager.find(xhToken);
	if (stl_MapIterator == stl_MapManager.end())
	{
		SDKPlugin_IsErrorOccur = TRUE;
		SDKPlugin_dwErrorCode = ERROR_XENGINE_STREAMMEDIA_PLUGIN_MODULE_DH_NOTFOUND;
		st_Locker.unlock();
		return FALSE;
	}
	// 退出设备
	if (0 != stl_MapIterator->second.hSDKModule)
	{
		CLIENT_Logout(stl_MapIterator->second.hSDKModule);
	}
	stl_MapManager.erase(stl_MapIterator);
	st_Locker.unlock();
	return TRUE;
}
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
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CPlugin_Dahua::PluginCore_Play(XNETHANDLE xhToken, int nChannel)
{
	SDKPlugin_IsErrorOccur = FALSE;

	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, PLUGIN_SDKDAHUA>::const_iterator stl_MapIterator = stl_MapManager.find(xhToken);
	if (stl_MapIterator == stl_MapManager.end())
	{
		SDKPlugin_IsErrorOccur = TRUE;
		SDKPlugin_dwErrorCode = ERROR_XENGINE_STREAMMEDIA_PLUGIN_MODULE_DH_NOTFOUND;
		st_Locker.unlock_shared();
		return FALSE;
	}
	//通道是否打开
	for (auto stl_ListIterator = stl_MapIterator->second.pStl_ListChannel->begin(); stl_ListIterator != stl_MapIterator->second.pStl_ListChannel->end(); stl_ListIterator++)
	{
		if (nChannel == *stl_ListIterator)
		{
			SDKPlugin_IsErrorOccur = TRUE;
			SDKPlugin_dwErrorCode = ERROR_XENGINE_STREAMMEDIA_PLUGIN_MODULE_DH_OPENED;
			st_Locker.unlock_shared();
			return FALSE;
		}
	}
	//开始播放
	LLONG hPlay = CLIENT_RealPlayEx(stl_MapIterator->second.hSDKModule, nChannel, 0);
	if (0 == hPlay)
	{
		SDKPlugin_IsErrorOccur = TRUE;
		SDKPlugin_dwErrorCode = CLIENT_GetLastError();
		st_Locker.unlock_shared();
		return FALSE;
	}
	CLIENT_SetRealDataCallBackEx(hPlay, PluginCore_CB_RealData, (DWORD)this, REALDATA_FLAG_RAW_DATA);
	st_Locker.unlock_shared();
	return TRUE;
}
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
BOOL CPlugin_Dahua::PluginCore_Stop(XNETHANDLE xhToken, int nChannel)
{
	SDKPlugin_IsErrorOccur = FALSE;

	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, PLUGIN_SDKDAHUA>::const_iterator stl_MapIterator = stl_MapManager.find(xhToken);
	if (stl_MapIterator == stl_MapManager.end())
	{
		SDKPlugin_IsErrorOccur = TRUE;
		SDKPlugin_dwErrorCode = ERROR_XENGINE_STREAMMEDIA_PLUGIN_MODULE_DH_NOTFOUND;
		st_Locker.unlock_shared();
		return FALSE;
	}
	//查找通道
	for (auto stl_ListIterator = stl_MapIterator->second.pStl_ListChannel->begin(); stl_ListIterator != stl_MapIterator->second.pStl_ListChannel->end(); stl_ListIterator++)
	{
		if (nChannel == *stl_ListIterator)
		{
			CLIENT_StopRealPlayEx(nChannel);
			break;
		}
	}
	st_Locker.unlock_shared();
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//                         
//////////////////////////////////////////////////////////////////////////
void CALLBACK CPlugin_Dahua::PluginCore_CB_Disconnect(LLONG lLoginID, char* pchDVRIP, LONG nDVRPort, LDWORD dwUser)
{

}
void CALLBACK CPlugin_Dahua::PluginCore_CB_AutoConnect(LLONG lLoginID, char* pchDVRIP, LONG nDVRPort, LDWORD dwUser)
{

}
void CALLBACK CPlugin_Dahua::PluginCore_CB_RealData(LLONG lRealHandle, DWORD dwDataType, BYTE* pBuffer, DWORD dwBufSize, LONG param, LDWORD dwUser)
{
	CPlugin_Dahua* pClass_This = (CPlugin_Dahua*)dwUser;

	if (1 == dwDataType)
	{
		pClass_This->lpCall_AVData(lRealHandle, 1, pBuffer, dwBufSize, (LPVOID)param, (LPVOID)dwUser);
	}
	else if (3 == dwDataType)
	{
		pClass_This->lpCall_AVData(lRealHandle, 2, pBuffer, dwBufSize, (LPVOID)param, (LPVOID)dwUser);
	}
}