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
	bH264Init = FALSE;
	
}
CPlugin_Dahua::~CPlugin_Dahua()
{
}
//////////////////////////////////////////////////////////////////////////
//                       公有函数
//////////////////////////////////////////////////////////////////////////
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
	CLIENT_Init(PluginCore_CB_Disconnect, (DWORD)this);
	CLIENT_SetAutoReconnect(PluginCore_CB_AutoConnect, (DWORD)this);
	int nWaitTime = 5000; // 登录请求响应超时时间设置为 5s
	int nTryTimes = 3;    // 登录时尝试建立链接 3 次
	CLIENT_SetConnectTime(nWaitTime, nTryTimes);

	PLUGIN_SDKDAHUA st_SDKDahua;
	memset(&st_SDKDahua, '\0', sizeof(PLUGIN_SDKDAHUA));

	st_SDKDahua.st_DevLoginInfo.dwSize = sizeof(NET_IN_LOGIN_WITH_HIGHLEVEL_SECURITY);
	strncpy(st_SDKDahua.st_DevLoginInfo.szIP, lpszAddr, sizeof(st_SDKDahua.st_DevLoginInfo.szIP) - 1);
	strncpy(st_SDKDahua.st_DevLoginInfo.szPassword, lpszPass, sizeof(st_SDKDahua.st_DevLoginInfo.szPassword) - 1);
	strncpy(st_SDKDahua.st_DevLoginInfo.szUserName, lpszUser, sizeof(st_SDKDahua.st_DevLoginInfo.szUserName) - 1);
	st_SDKDahua.st_DevLoginInfo.nPort = nPort;
	st_SDKDahua.st_DevLoginInfo.emSpecCap = EM_LOGIN_SPEC_CAP_TCP;

	st_SDKDahua.st_DevOutInfo.dwSize = sizeof(NET_OUT_LOGIN_WITH_HIGHLEVEL_SECURITY);
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
	st_SDKDahua.pStl_ListChannel = new list<PLUGIN_PLAYINFO>;
	if (NULL == st_SDKDahua.pStl_ListChannel)
	{
		SDKPlugin_IsErrorOccur = TRUE;
		SDKPlugin_dwErrorCode = ERROR_XENGINE_STREAMMEDIA_PLUGIN_MODULE_DH_MALLOC;
		return FALSE;
	}

	*pxhToken = st_SDKDahua.hSDKModule;
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
	unordered_map<XNETHANDLE, PLUGIN_SDKDAHUA>::iterator stl_MapIterator = stl_MapManager.find(xhToken);
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
	delete stl_MapIterator->second.pStl_ListChannel;
	stl_MapIterator->second.pStl_ListChannel = NULL;
	
	stl_MapManager.erase(stl_MapIterator);
	st_Locker.unlock();

	CLIENT_Cleanup();
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
		if (nChannel == stl_ListIterator->nChannle)
		{
			SDKPlugin_IsErrorOccur = TRUE;
			SDKPlugin_dwErrorCode = ERROR_XENGINE_STREAMMEDIA_PLUGIN_MODULE_DH_OPENED;
			st_Locker.unlock_shared();
			return FALSE;
		}
	}
	PLUGIN_PLAYINFO st_PlayInfo;
	memset(&st_PlayInfo, '\0', sizeof(PLUGIN_PLAYINFO));

	st_PlayInfo.nChannle = nChannel;
	//开始播放
	NET_IN_REALPLAY_BY_DATA_TYPE st_PlayIn = { sizeof(st_PlayIn) };
	NET_OUT_REALPLAY_BY_DATA_TYPE st_PlayOut = { sizeof(st_PlayOut) };

	st_PlayIn.cbRealData = PluginCore_CB_RealData;
	st_PlayIn.emDataType = EM_REAL_DATA_TYPE_H264;
	st_PlayIn.rType = DH_RType_Realplay;
	st_PlayIn.nChannelID = nChannel;
	//st_PlayIn.emAudioType = EM_AUDIO_DATA_TYPE_AAC;
	st_PlayIn.hWnd = NULL;
	st_PlayIn.dwUser = (DWORD)this;

	st_PlayInfo.xhPlay = CLIENT_RealPlayByDataType(stl_MapIterator->second.hSDKModule, &st_PlayIn, &st_PlayOut, 5000);
	if (0 == st_PlayInfo.xhPlay)
	{
		SDKPlugin_IsErrorOccur = TRUE;
		SDKPlugin_dwErrorCode = CLIENT_GetLastError();
		st_Locker.unlock_shared();
		return FALSE;
	}
	stl_MapIterator->second.pStl_ListChannel->push_back(st_PlayInfo);
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

	st_Locker.lock();
	unordered_map<XNETHANDLE, PLUGIN_SDKDAHUA>::const_iterator stl_MapIterator = stl_MapManager.find(xhToken);
	if (stl_MapIterator == stl_MapManager.end())
	{
		SDKPlugin_IsErrorOccur = TRUE;
		SDKPlugin_dwErrorCode = ERROR_XENGINE_STREAMMEDIA_PLUGIN_MODULE_DH_NOTFOUND;
		st_Locker.unlock();
		return FALSE;
	}
	//查找通道
	for (auto stl_ListIterator = stl_MapIterator->second.pStl_ListChannel->begin(); stl_ListIterator != stl_MapIterator->second.pStl_ListChannel->end(); stl_ListIterator++)
	{
		if (nChannel == stl_ListIterator->nChannle)
		{
			CLIENT_StopRealPlayEx(stl_ListIterator->xhPlay);
			break;
		}
	}
	st_Locker.unlock();
	return TRUE;
}
/********************************************************************
函数名称：PluginCore_GetData
函数功能：获取一个设备的数据
 参数.一：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
 参数.二：pSt_MQData
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出获取到的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CPlugin_Dahua::PluginCore_GetData(XNETHANDLE xhToken, PLUGIN_MQDATA* pSt_MQData)
{
	SDKPlugin_IsErrorOccur = FALSE;

	st_MQLocker.lock();
	if (stl_ListDatas.empty())
	{
		st_MQLocker.unlock();
		return FALSE;
	}
	*pSt_MQData = stl_ListDatas.front();
	stl_ListDatas.pop_front();
	st_MQLocker.unlock();
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

	if (dwDataType == (NET_DATA_CALL_BACK_VALUE + EM_REAL_DATA_TYPE_H264))
	{
		PLUGIN_MQDATA st_MQData;
		memset(&st_MQData, '\0', sizeof(PLUGIN_MQDATA));

		pClass_This->st_Locker.lock();
		unordered_map<XNETHANDLE, PLUGIN_SDKDAHUA>::const_iterator stl_MapIterator = pClass_This->stl_MapManager.begin();
		for (; stl_MapIterator != pClass_This->stl_MapManager.end(); stl_MapIterator++)
		{
			list<PLUGIN_PLAYINFO>::const_iterator stl_ListIterator = stl_MapIterator->second.pStl_ListChannel->begin();
			for (; stl_ListIterator != stl_MapIterator->second.pStl_ListChannel->end(); stl_ListIterator++)
			{
				if (lRealHandle == stl_ListIterator->xhPlay)
				{
					st_MQData.xhToken = stl_MapIterator->second.hSDKModule;
					st_MQData.nChannel = stl_ListIterator->nChannle;
					st_MQData.nDType = 1;
					//分拆数据包
					int nCpyCount = 0;
					int nPosSize = 0;
					int nAllSize = dwBufSize;
					while (nAllSize > 0)
					{
						if (nAllSize >= XENGINE_STREAMMEDIA_PLUGIN_DAHUA_PACKET_SIZE)
						{
							nCpyCount = XENGINE_STREAMMEDIA_PLUGIN_DAHUA_PACKET_SIZE;
						}
						else
						{
							nCpyCount = nAllSize;
						}
						st_MQData.nMsgLen = nCpyCount;
						memcpy(st_MQData.tszMsgBuffer, pBuffer + nPosSize, nCpyCount);

						pClass_This->st_MQLocker.lock();
						pClass_This->stl_ListDatas.push_back(st_MQData);
						pClass_This->st_MQLocker.unlock();
						nAllSize -= nCpyCount;
						nPosSize += nCpyCount;
					}
					break;
				}
			}
		}
		pClass_This->st_Locker.unlock();
	}
}