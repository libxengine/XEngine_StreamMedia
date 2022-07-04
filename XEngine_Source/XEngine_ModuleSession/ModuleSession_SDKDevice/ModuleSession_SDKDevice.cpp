#include "pch.h"
#include "ModuleSession_SDKDevice.h"
/********************************************************************
//    Created:     2022/07/04  14:40:08
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_SDKDevice\ModuleSession_SDKDevice.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_SDKDevice
//    File Base:   ModuleSession_SDKDevice
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     SDK客户端会话
//    History:
*********************************************************************/
CModuleSession_SDKDevice::CModuleSession_SDKDevice()
{
}
CModuleSession_SDKDevice::~CModuleSession_SDKDevice()
{
}
//////////////////////////////////////////////////////////////////////////
//                    公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：ModuleSession_JT1078Stream_InsertDevice
函数功能：创建一个客户端会话
 参数.一：xhToken
  In/Out：In
  类型：设备句柄
  可空：N
  意思：输入设备句柄
 参数.二：xhClient
  In/Out：In
  类型：句柄
  可空：N
  意思：输入客户端句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_SDKDevice::ModuleSession_SDKDevice_Create(XNETHANDLE xhToken, XNETHANDLE xhClient)
{
    Session_IsErrorOccur = FALSE;

	MODULESESSION_LISTCLIENT st_SDKClient;
    memset(&st_SDKClient, '\0', sizeof(MODULESESSION_LISTCLIENT));

	st_SDKClient.xhToken = xhToken;
    st_SDKClient.xhClient = xhClient;
	st_SDKClient.st_Locker = make_shared<shared_mutex>();
	st_SDKClient.pStl_ListClient = new list<MODULESESSION_SDKCLIENT>;
	if (NULL == st_SDKClient.pStl_ListClient)
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_MALLOC;
		return FALSE;
	}
	//查找句柄,一个设备对应一个客户端
	st_Locker.lock();
	unordered_map<XNETHANDLE, MODULESESSION_LISTCLIENT>::iterator stl_MapIterator = stl_MapClient.find(xhToken);
	if (stl_MapIterator != stl_MapClient.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_EXIST;
		st_Locker.unlock();
		return FALSE;
	}
	stl_MapClient.insert(make_pair(xhToken, st_SDKClient));
	st_Locker.unlock();
    return TRUE;
}
/********************************************************************
函数名称：ModuleSession_SDKDevice_Insert
函数功能：插入一条播放记录
 参数.一：xhToken
  In/Out：In
  类型：设备句柄
  可空：N
  意思：输入设备句柄
 参数.二：nChannel
  In/Out：In
  类型：整数型
  可空：N
  意思：输入通道
 参数.三：bLive
  In/Out：In
  类型：逻辑型
  可空：N
  意思：输入是直播还是录像
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_SDKDevice::ModuleSession_SDKDevice_Insert(XNETHANDLE xhToken, int nChannel, BOOL bLive)
{
	Session_IsErrorOccur = FALSE;

	MODULESESSION_SDKCLIENT st_SDKClient;
	st_SDKClient.nChannel = nChannel;
	st_SDKClient.bLive = bLive;

	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, MODULESESSION_LISTCLIENT>::iterator stl_MapIterator = stl_MapClient.find(xhToken);
	if (stl_MapIterator == stl_MapClient.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return FALSE;
	}
	//是否存在
	BOOL bFound = FALSE;
	stl_MapIterator->second.st_Locker->lock();
	for (auto stl_ListIterator = stl_MapIterator->second.pStl_ListClient->begin(); stl_ListIterator != stl_MapIterator->second.pStl_ListClient->end(); stl_ListIterator++)
	{
		if ((nChannel == stl_ListIterator->nChannel) && (bLive == stl_ListIterator->bLive))
		{
			bFound = TRUE;
			break;
		}
	}
	if (!bFound)
	{
		stl_MapIterator->second.pStl_ListClient->push_back(st_SDKClient);
	}
	stl_MapIterator->second.st_Locker->unlock();
	st_Locker.unlock_shared();

	if (bFound)
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_EXIST;
		return FALSE;
	}
	return TRUE;
}
/********************************************************************
函数名称：ModuleSession_SDKDevice_Delete
函数功能：删除一条播放记录
 参数.一：xhToken
  In/Out：In
  类型：设备句柄
  可空：N
  意思：输入设备句柄
 参数.二：nChannel
  In/Out：In
  类型：整数型
  可空：N
  意思：输入通道
 参数.三：bLive
  In/Out：In
  类型：逻辑型
  可空：N
  意思：输入是直播还是录像
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_SDKDevice::ModuleSession_SDKDevice_Delete(XNETHANDLE xhToken, int nChannel, BOOL bLive)
{
	Session_IsErrorOccur = FALSE;

	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, MODULESESSION_LISTCLIENT>::iterator stl_MapIterator = stl_MapClient.find(xhToken);
	if (stl_MapIterator == stl_MapClient.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return FALSE;
	}
	//是否存在
	stl_MapIterator->second.st_Locker->lock();
	for (auto stl_ListIterator = stl_MapIterator->second.pStl_ListClient->begin(); stl_ListIterator != stl_MapIterator->second.pStl_ListClient->end(); stl_ListIterator++)
	{
		if ((nChannel == stl_ListIterator->nChannel) && (bLive == stl_ListIterator->bLive))
		{
			stl_MapIterator->second.pStl_ListClient->erase(stl_ListIterator);
			break;
		}
	}
	stl_MapIterator->second.st_Locker->unlock();
	st_Locker.unlock_shared();
	return TRUE;
}
/********************************************************************
函数名称：ModuleSession_SDKDevice_Get
函数功能：获得一个可以使用的客户端句柄
 参数.一：xhToken
  In/Out：In
  类型：设备句柄
  可空：N
  意思：输入设备句柄
 参数.二：nChannel
  In/Out：In
  类型：整数型
  可空：N
  意思：输入通道
 参数.三：bLive
  In/Out：In
  类型：逻辑型
  可空：N
  意思：输入是直播还是录像
 参数.四：pxhClient
  In/Out：Out
  类型：句柄指针
  可空：N
  意思：输出获得的句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_SDKDevice::ModuleSession_SDKDevice_Get(XNETHANDLE xhToken, int nChannel, BOOL bLive, XNETHANDLE *pxhClient)
{
	Session_IsErrorOccur = FALSE;

	if (NULL == pxhClient)
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_PARAMENT;
		return FALSE;
	}
	st_Locker.lock_shared();
	//查找句柄
	unordered_map<XNETHANDLE, MODULESESSION_LISTCLIENT>::const_iterator stl_MapIterator = stl_MapClient.find(xhToken);
	if (stl_MapIterator == stl_MapClient.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
        st_Locker.unlock_shared();
        return FALSE;
	}
    *pxhClient = stl_MapIterator->second.xhClient;
	st_Locker.unlock_shared();
	return TRUE;
}
/********************************************************************
函数名称：ModuleSession_SDKDevice_Destory
函数功能：销毁客户端会话管理器
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_SDKDevice::ModuleSession_SDKDevice_Destory()
{
	Session_IsErrorOccur = FALSE;

	st_Locker.lock();
	unordered_map<XNETHANDLE, MODULESESSION_LISTCLIENT>::iterator stl_MapIterator = stl_MapClient.begin();
	for (; stl_MapIterator != stl_MapClient.end(); stl_MapIterator++)
	{
		stl_MapIterator->second.st_Locker->lock();
		stl_MapIterator->second.pStl_ListClient->clear();
		delete stl_MapIterator->second.pStl_ListClient;
		stl_MapIterator->second.pStl_ListClient = NULL;
		stl_MapIterator->second.st_Locker->unlock();
	}
	stl_MapClient.clear();
    st_Locker.unlock();
	return TRUE;
}