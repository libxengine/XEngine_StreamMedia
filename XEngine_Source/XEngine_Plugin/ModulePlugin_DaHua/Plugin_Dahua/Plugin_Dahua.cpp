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
 参数.六：nMaxPool
  In/Out：In
  类型：整数型
  可空：N
  意思：输入最大线程个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CPlugin_Dahua::PluginCore_Init(XNETHANDLE* pxhToken, LPCTSTR lpszAddr, int nPort, LPCTSTR lpszUser, LPCTSTR lpszPass, int nMaxPool)
{
	SDKPlugin_IsErrorOccur = FALSE;

	if ((NULL == pxhToken) || (NULL == lpszAddr) || (NULL == lpszUser) || (NULL == lpszPass))
	{
		SDKPlugin_IsErrorOccur = TRUE;
		SDKPlugin_dwErrorCode = ERROR_XENGINE_STREAMMEDIA_PLUGIN_MODULE_DH_PARAMENT;
		return FALSE;
	}
	CLIENT_Init(PluginCore_CB_Disconnect, (LDWORD)this);
	CLIENT_SetAutoReconnect(PluginCore_CB_AutoConnect, (LDWORD)this);

	NET_PARAM st_NetParam;
	memset(&st_NetParam, '\0', sizeof(NET_PARAM));
	st_NetParam.nWaittime = 8000;
	st_NetParam.nConnectTime = 3000;
	st_NetParam.nConnectTryNum = 3;
	st_NetParam.nSubConnectSpaceTime = 100;
	st_NetParam.nGetConnInfoTime = 3000;
	CLIENT_SetNetworkParam(&st_NetParam);

	PLUGIN_SDKDAHUA st_SDKDahua;
	// 登录设备
	int nErrcode = 0;
	st_SDKDahua.hSDKModule = CLIENT_LoginEx2(lpszAddr, nPort, lpszUser, lpszPass, EM_LOGIN_SPEC_CAP_TCP, NULL, &st_SDKDahua.st_DevInfo, &nErrcode);
	if (0 == st_SDKDahua.hSDKModule)
	{
		// 根据错误码，可以在 dhnetsdk.h 中找到相应的解释，此处打印的是 16 进制，头文件中是十进制，其中的转换需注意
		// 例如：#define NET_NOT_SUPPORTED_EC(23) // 当前 SDK 未支持该功能，对应的错误码为 0x80000017, 23 对应的 16 进制为 0x17
		SDKPlugin_IsErrorOccur = TRUE;
		SDKPlugin_dwErrorCode = CLIENT_GetLastError();
		return FALSE;
	}
	st_SDKDahua.st_Locker = make_shared<std::shared_mutex>();

	*pxhToken = st_SDKDahua.hSDKModule;
	st_LockerManage.lock();
	stl_MapManager.insert(make_pair(st_SDKDahua.hSDKModule, st_SDKDahua));
	st_LockerManage.unlock();

	st_LockerData.lock();
	unordered_map<int, PLUGIN_SDKMQLSIT> stl_MapData;
	for (int i = 0; i < nMaxPool; i++)
	{
		PLUGIN_SDKMQLSIT st_SDKList;
		st_SDKList.st_Locker = make_shared<std::shared_mutex>();

		stl_MapData.insert(make_pair(i, st_SDKList));
	}
	stl_MapSDKData.insert(make_pair(*pxhToken, stl_MapData));
	st_LockerData.unlock();
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

	st_LockerManage.lock();
	unordered_map<XNETHANDLE, PLUGIN_SDKDAHUA>::iterator stl_MapIterator = stl_MapManager.find(xhToken);
	if (stl_MapIterator == stl_MapManager.end())
	{
		SDKPlugin_IsErrorOccur = TRUE;
		SDKPlugin_dwErrorCode = ERROR_XENGINE_STREAMMEDIA_PLUGIN_MODULE_DH_NOTFOUND;
		st_LockerManage.unlock();
		return FALSE;
	}
	// 退出设备
	if (0 != stl_MapIterator->second.hSDKModule)
	{
		CLIENT_Logout(stl_MapIterator->second.hSDKModule);
	}
	stl_MapManager.erase(stl_MapIterator);
	st_LockerManage.unlock();
	
	st_LockerData.lock();
	stl_MapSDKData.clear();
	st_LockerData.unlock();
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

	//先找到最小队列
	st_LockerData.lock_shared();
	unordered_map<XNETHANDLE, unordered_map<int, PLUGIN_SDKMQLSIT> >::iterator stl_MapMQIterator = stl_MapSDKData.find(xhToken);
	if (stl_MapMQIterator == stl_MapSDKData.end())
	{
		SDKPlugin_IsErrorOccur = TRUE;
		SDKPlugin_dwErrorCode = ERROR_XENGINE_STREAMMEDIA_PLUGIN_MODULE_DH_NOTFOUND;
		st_LockerData.unlock_shared();
		return FALSE;
	}
	int nCount = 10000;
	int nIndex = 0;
	for (auto stl_MapMQIndexIterator = stl_MapMQIterator->second.begin(); stl_MapMQIndexIterator != stl_MapMQIterator->second.end(); stl_MapMQIndexIterator++)
	{  
		if ((int)stl_MapMQIndexIterator->second.stl_ListMQData.size() < nCount)
		{
			nCount = stl_MapMQIndexIterator->second.stl_ListMQData.size();
			nIndex = stl_MapMQIndexIterator->first;
		}
	}
	st_LockerData.unlock_shared();

	st_LockerManage.lock_shared();
	unordered_map<XNETHANDLE, PLUGIN_SDKDAHUA>::iterator stl_MapIterator = stl_MapManager.find(xhToken);
	if (stl_MapIterator == stl_MapManager.end())
	{
		SDKPlugin_IsErrorOccur = TRUE;
		SDKPlugin_dwErrorCode = ERROR_XENGINE_STREAMMEDIA_PLUGIN_MODULE_DH_NOTFOUND;
		st_LockerManage.unlock_shared();
		return FALSE;
	}
	BOOL bFound = FALSE;
	//通道是否打开
	list<PLUGIN_SDKINFO*>::iterator stl_ListIterator = stl_MapIterator->second.stl_ListChannel.begin();
	for (; stl_ListIterator != stl_MapIterator->second.stl_ListChannel.end(); stl_ListIterator++)
	{
		if (nChannel == (*stl_ListIterator)->nChannel)
		{
			bFound = TRUE;
			break;
		}
	}
	if (bFound)
	{
		SDKPlugin_IsErrorOccur = TRUE;
		SDKPlugin_dwErrorCode = ERROR_XENGINE_STREAMMEDIA_PLUGIN_MODULE_DH_OPENED;
		st_LockerManage.unlock_shared();
		return FALSE;
	}
	//开始播放
	PLUGIN_SDKINFO *pSt_SDKInfo = new PLUGIN_SDKINFO;
	if (NULL == pSt_SDKInfo)
	{
		SDKPlugin_IsErrorOccur = TRUE;
		SDKPlugin_dwErrorCode = ERROR_XENGINE_STREAMMEDIA_PLUGIN_MODULE_DH_MALLOC;
		st_LockerManage.unlock_shared();
		return FALSE;
	}
	pSt_SDKInfo->xhToken = xhToken;
	pSt_SDKInfo->nIndex = nIndex;
	pSt_SDKInfo->nChannel = nChannel;
	pSt_SDKInfo->lClass = this;

	NET_IN_REALPLAY_BY_DATA_TYPE st_PlayIn = { sizeof(st_PlayIn) };
	NET_OUT_REALPLAY_BY_DATA_TYPE st_PlayOut = { sizeof(st_PlayOut) };
	st_PlayIn.cbRealData = PluginCore_CB_RealData;
	st_PlayIn.emDataType = EM_REAL_DATA_TYPE_H264;
	st_PlayIn.rType = DH_RType_Realplay;
	st_PlayIn.nChannelID = nChannel;
	//st_PlayIn.emAudioType = EM_AUDIO_DATA_TYPE_AAC;
	st_PlayIn.hWnd = NULL;
	//st_PlayIn.dwUser = (LDWORD)this;
	st_PlayIn.dwUser = (LDWORD)pSt_SDKInfo;

	pSt_SDKInfo->xhPlay = CLIENT_RealPlayByDataType(stl_MapIterator->second.hSDKModule, &st_PlayIn, &st_PlayOut, 3000);
	if (0 == pSt_SDKInfo->xhPlay)
	{
		SDKPlugin_IsErrorOccur = TRUE;
		SDKPlugin_dwErrorCode = CLIENT_GetLastError();
		st_LockerManage.unlock_shared();
		return FALSE;
	}
	stl_MapIterator->second.st_Locker->lock();
	stl_MapIterator->second.stl_ListChannel.push_back(pSt_SDKInfo);
	stl_MapIterator->second.st_Locker->unlock();
	st_LockerManage.unlock_shared();
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

	st_LockerManage.lock();
	unordered_map<XNETHANDLE, PLUGIN_SDKDAHUA>::iterator stl_MapIterator = stl_MapManager.find(xhToken);
	if (stl_MapIterator == stl_MapManager.end())
	{
		SDKPlugin_IsErrorOccur = TRUE;
		SDKPlugin_dwErrorCode = ERROR_XENGINE_STREAMMEDIA_PLUGIN_MODULE_DH_NOTFOUND;
		st_LockerManage.unlock();
		return FALSE;
	}
	//查找通道
	stl_MapIterator->second.st_Locker->lock();
	list<PLUGIN_SDKINFO*>::iterator stl_ListIterator = stl_MapIterator->second.stl_ListChannel.begin();
	for (; stl_ListIterator != stl_MapIterator->second.stl_ListChannel.end(); stl_ListIterator++)
	{
		if (nChannel == (*stl_ListIterator)->nChannel)
		{
			CLIENT_StopRealPlayEx((*stl_ListIterator)->xhPlay);

			delete (*stl_ListIterator);
			(*stl_ListIterator) = NULL;
			stl_MapIterator->second.stl_ListChannel.erase(stl_ListIterator);
			break;
		}
	}
	stl_MapIterator->second.st_Locker->unlock();
	st_LockerManage.unlock();
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
 参数.二：nIndex
  In/Out：In
  类型：句柄
  可空：N
  意思：输入线程索引
 参数.三：pSt_MQData
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出获取到的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CPlugin_Dahua::PluginCore_GetData(XNETHANDLE xhToken, int nIndex, PLUGIN_MQDATA* pSt_MQData)
{
	SDKPlugin_IsErrorOccur = FALSE;

	st_LockerData.lock_shared();
	unordered_map<XNETHANDLE, unordered_map<int, PLUGIN_SDKMQLSIT> >::iterator stl_MapIterator = stl_MapSDKData.find(xhToken);
	if (stl_MapIterator == stl_MapSDKData.end())
	{
		SDKPlugin_IsErrorOccur = TRUE;
		SDKPlugin_dwErrorCode = ERROR_XENGINE_STREAMMEDIA_PLUGIN_MODULE_DH_NOTFOUND;
		st_LockerData.unlock_shared();
		return FALSE;
	}
	unordered_map<int, PLUGIN_SDKMQLSIT>::iterator stl_MapIndexIterator = stl_MapIterator->second.find(nIndex);
	if (stl_MapIndexIterator == stl_MapIterator->second.end())
	{
		SDKPlugin_IsErrorOccur = TRUE;
		SDKPlugin_dwErrorCode = ERROR_XENGINE_STREAMMEDIA_PLUGIN_MODULE_DH_NOTFOUND;
		st_LockerData.unlock_shared();
		return FALSE;
	}
	stl_MapIndexIterator->second.st_Locker->lock();
	if (stl_MapIndexIterator->second.stl_ListMQData.empty())
	{
		SDKPlugin_IsErrorOccur = TRUE;
		SDKPlugin_dwErrorCode = ERROR_XENGINE_STREAMMEDIA_PLUGIN_MODULE_DH_EMPTY;
		stl_MapIndexIterator->second.st_Locker->unlock();
		st_LockerData.unlock_shared();
		return FALSE;
	}
	*pSt_MQData = stl_MapIndexIterator->second.stl_ListMQData.front();
	stl_MapIndexIterator->second.stl_ListMQData.pop_front();
	stl_MapIndexIterator->second.st_Locker->unlock();
	st_LockerData.unlock_shared();
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
	PLUGIN_SDKINFO* pSt_SDKInfo = (PLUGIN_SDKINFO*)dwUser;
	CPlugin_Dahua* pClass_This = (CPlugin_Dahua*)pSt_SDKInfo->lClass;

	if (dwDataType == (NET_DATA_CALL_BACK_VALUE + EM_REAL_DATA_TYPE_H264))
	{
		PLUGIN_MQDATA st_MQData;
		st_MQData.xhToken = pSt_SDKInfo->xhToken;
		st_MQData.nChannel = pSt_SDKInfo->nChannel;
		st_MQData.bLive = TRUE;
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

			pClass_This->st_LockerData.lock_shared();
			unordered_map<XNETHANDLE, unordered_map<int, PLUGIN_SDKMQLSIT> >::iterator stl_MapMQIterator = pClass_This->stl_MapSDKData.find(pSt_SDKInfo->xhToken);
			if (stl_MapMQIterator != pClass_This->stl_MapSDKData.end())
			{
				unordered_map<int, PLUGIN_SDKMQLSIT>::iterator stl_MapMQIndexIterator = stl_MapMQIterator->second.find(pSt_SDKInfo->nIndex);
				if (stl_MapMQIndexIterator != stl_MapMQIterator->second.end())
				{
					stl_MapMQIndexIterator->second.st_Locker->lock();
					stl_MapMQIndexIterator->second.stl_ListMQData.push_back(st_MQData);
					stl_MapMQIndexIterator->second.st_Locker->unlock();
				}
			}
			pClass_This->st_LockerData.unlock_shared();
			nAllSize -= nCpyCount;
			nPosSize += nCpyCount;
		}
	}
}