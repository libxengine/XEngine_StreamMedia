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
	pSt_VFile = NULL;
	pSt_AFile = NULL;
	CLIENT_Init(PluginCore_CB_Disconnect, (LDWORD)this);
	CLIENT_SetAutoReconnect(PluginCore_CB_AutoConnect, (LDWORD)this);
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
 参数.六：bPacket
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：是否启用分包传递
 参数.七：bDebug
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：是否启用调试
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CPlugin_Dahua::PluginCore_Init(XNETHANDLE* pxhToken, LPCTSTR lpszAddr, int nPort, LPCTSTR lpszUser, LPCTSTR lpszPass, BOOL bPacket /* = TRUE */, BOOL bDebug /* = FALSE */)
{
	SDKPlugin_IsErrorOccur = FALSE;

	if ((NULL == pxhToken) || (NULL == lpszAddr) || (NULL == lpszUser) || (NULL == lpszPass))
	{
		SDKPlugin_IsErrorOccur = TRUE;
		SDKPlugin_dwErrorCode = ERROR_XENGINE_STREAMMEDIA_PLUGIN_MODULE_DH_PARAMENT;
		return FALSE;
	}
	PLUGIN_SDKDAHUA st_SDKDahua;
	
	if (bDebug)
	{
		st_SDKDahua.hSDKModule = rand();
	}
	else
	{
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
	}
	st_SDKDahua.st_Locker = make_shared<std::shared_mutex>();

	m_bDebug = bDebug;
	m_bPacket = bPacket;
	*pxhToken = st_SDKDahua.hSDKModule;
	st_LockerManage.lock();
	stl_MapManager.insert(make_pair(st_SDKDahua.hSDKModule, st_SDKDahua));
	st_LockerManage.unlock();
	return TRUE;
}
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
BOOL CPlugin_Dahua::PluginCore_CBSet(XNETHANDLE xhToken, CALLBACK_STREAMMEIDA_MODULE_PLUGIN_SDKBUFFER fpCall_SDKBuffer, LPVOID lParam)
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

	st_LockerManage.unlock();
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
	if (stl_MapIterator != stl_MapManager.end())
	{
		// 退出设备
		if (0 != stl_MapIterator->second.hSDKModule)
		{
			CLIENT_Logout(stl_MapIterator->second.hSDKModule);
		}
		if (NULL != pSt_VFile)
		{
			fclose(pSt_VFile);
		}
		if (NULL != pSt_AFile)
		{
			fclose(pSt_AFile);
		}
		stl_MapManager.erase(stl_MapIterator);
	}
	st_LockerManage.unlock();
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
BOOL CPlugin_Dahua::PluginCore_Play(XNETHANDLE xhToken, int nChannel, BOOL bAudio /* = FALSE */)
{
	SDKPlugin_IsErrorOccur = FALSE;

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
	pSt_SDKInfo->nChannel = nChannel;
	pSt_SDKInfo->lClass = this;
	pSt_SDKInfo->bAudio = bAudio;

	if (m_bDebug)
	{
		pSt_VFile = _tfopen("D:\\h264 file\\480p.264", "rb");
		if (bAudio)
		{
			pSt_AFile = _tfopen("D:\\h264 file\\test.aac", "rb");
		}
		pSt_SDKInfo->pSTDThread = make_shared<thread>(PluginCore_Thread, pSt_SDKInfo);
	}
	else
	{
		NET_IN_REALPLAY_BY_DATA_TYPE st_PlayIn = { sizeof(st_PlayIn) };
		NET_OUT_REALPLAY_BY_DATA_TYPE st_PlayOut = { sizeof(st_PlayOut) };
		st_PlayIn.cbRealData = PluginCore_CB_RealData;
		st_PlayIn.emDataType = EM_REAL_DATA_TYPE_H264;
		st_PlayIn.rType = DH_RType_Realplay;
		st_PlayIn.nChannelID = nChannel;
		if (bAudio)
		{
			st_PlayIn.emAudioType = EM_AUDIO_DATA_TYPE_AAC;
		}
		st_PlayIn.hWnd = NULL;
		st_PlayIn.dwUser = (LDWORD)pSt_SDKInfo;

		pSt_SDKInfo->xhPlay = CLIENT_RealPlayByDataType(stl_MapIterator->second.hSDKModule, &st_PlayIn, &st_PlayOut, 3000);
		if (0 == pSt_SDKInfo->xhPlay)
		{
			SDKPlugin_IsErrorOccur = TRUE;
			SDKPlugin_dwErrorCode = CLIENT_GetLastError();
			st_LockerManage.unlock_shared();
			return FALSE;
		}
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

	LLONG xhPlay = 0;
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
			xhPlay = (*stl_ListIterator)->xhPlay;
			delete (*stl_ListIterator);
			(*stl_ListIterator) = NULL;
			stl_MapIterator->second.stl_ListChannel.erase(stl_ListIterator);
			break;
		}
	}
	stl_MapIterator->second.st_Locker->unlock_shared();
	st_LockerManage.unlock();

	if (xhPlay > 0)
	{
		CLIENT_StopRealPlayEx(xhPlay);
	}
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
	//printf("AVType:%d = Chn:%d\n", dwDataType, pSt_SDKInfo->nChannel);
	if (dwDataType == (NET_DATA_CALL_BACK_VALUE + EM_REAL_DATA_TYPE_H264))
	{
		if (pClass_This->m_bPacket)
		{
			pSt_SDKInfo->lpCall_SDKBuffer(pSt_SDKInfo->xhToken, pSt_SDKInfo->nChannel, TRUE, 0, (LPCTSTR)pBuffer, dwBufSize, pSt_SDKInfo->lParam);
		}
		else
		{
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
				pSt_SDKInfo->lpCall_SDKBuffer(pSt_SDKInfo->xhToken, pSt_SDKInfo->nChannel, TRUE, 0, (LPCTSTR)pBuffer + nPosSize, nCpyCount, pSt_SDKInfo->lParam);
				nAllSize -= nCpyCount;
				nPosSize += nCpyCount;
			}
		}
	}
	else if ((dwDataType == (NET_DATA_CALL_BACK_VALUE + EM_AUDIO_DATA_TYPE_AAC)) && pSt_SDKInfo->bAudio)
	{
		if (pClass_This->m_bPacket)
		{
			pSt_SDKInfo->lpCall_SDKBuffer(pSt_SDKInfo->xhToken, pSt_SDKInfo->nChannel, TRUE, 1, (LPCTSTR)pBuffer, dwBufSize, pSt_SDKInfo->lParam);
		}
		else
		{
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
				pSt_SDKInfo->lpCall_SDKBuffer(pSt_SDKInfo->xhToken, pSt_SDKInfo->nChannel, TRUE, 1, (LPCTSTR)pBuffer + nPosSize, nCpyCount, pSt_SDKInfo->lParam);
				nAllSize -= nCpyCount;
				nPosSize += nCpyCount;
			}
		}
	}
}
DWORD CPlugin_Dahua::PluginCore_Thread(LPVOID lParam)
{
	PLUGIN_SDKINFO* pSt_SDKInfo = (PLUGIN_SDKINFO*)lParam;
	CPlugin_Dahua* pClass_This = (CPlugin_Dahua*)pSt_SDKInfo->lClass;

	while (TRUE)
	{
		if (NULL != pClass_This->pSt_VFile)
		{
			TCHAR tszMsgBuffer[8196];
			memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

			int nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pClass_This->pSt_VFile);
			if (nRet <= 0)
			{
				fseek(pClass_This->pSt_VFile, 0, SEEK_SET);
				nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pClass_This->pSt_VFile);
			}
			//分拆数据包
			int nCpyCount = 0;
			int nPosSize = 0;
			int nAllSize = nRet;
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
				pSt_SDKInfo->lpCall_SDKBuffer(pSt_SDKInfo->xhToken, pSt_SDKInfo->nChannel, TRUE, 0, tszMsgBuffer + nPosSize, nCpyCount, pSt_SDKInfo->lParam);
				nAllSize -= nCpyCount;
				nPosSize += nCpyCount;
			}
		}
		if (NULL != pClass_This->pSt_AFile)
		{
			TCHAR tszMsgBuffer[8196];
			memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

			int nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pClass_This->pSt_AFile);
			if (nRet <= 0)
			{
				fseek(pClass_This->pSt_AFile, 0, SEEK_SET);
				nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pClass_This->pSt_AFile);
			}
			//分拆数据包
			int nCpyCount = 0;
			int nPosSize = 0;
			int nAllSize = nRet;
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
				pSt_SDKInfo->lpCall_SDKBuffer(pSt_SDKInfo->xhToken, pSt_SDKInfo->nChannel, TRUE, 1, tszMsgBuffer + nPosSize, nCpyCount, pSt_SDKInfo->lParam);
				nAllSize -= nCpyCount;
				nPosSize += nCpyCount;
			}
		}
		//25帧每秒
		std::this_thread::sleep_for(std::chrono::milliseconds(40));
	}
}