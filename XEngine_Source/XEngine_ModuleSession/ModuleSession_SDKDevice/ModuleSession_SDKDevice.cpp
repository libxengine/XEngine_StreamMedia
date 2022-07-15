#include "pch.h"
#include "ModuleSession_SDKDevice.h"
/********************************************************************
//    Created:     2022/07/05  10:08:44
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_SDKDevice\ModuleSession_SDKDevice.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_SDKDevice
//    File Base:   ModuleSession_SDKDevice
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     SDK会话管理
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
函数名称：ModuleSession_SDKDevice_Create
函数功能：创建一个设备
 参数.一：xhDevice
  In/Out：In
  类型：句柄
  可空：N
  意思：输入句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_SDKDevice::ModuleSession_SDKDevice_Create(XNETHANDLE xhDevice)
{
    Session_IsErrorOccur = FALSE;

    MODULESESSION_SDKLIST* pSt_SessionList = new MODULESESSION_SDKLIST;
    if (NULL == pSt_SessionList)
    {
        Session_IsErrorOccur = TRUE;
        Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_MALLOC;
        return FALSE;
    }
    st_Locker.lock();
    stl_MapClient.insert(make_pair(xhDevice, pSt_SessionList));
    st_Locker.unlock();
    return TRUE;
}
/********************************************************************
函数名称：ModuleSession_SDKDevice_InsertClient
函数功能：插入客户端到设备管理器中
 参数.一：xhDevice
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的设备
 参数.二：xhClient
  In/Out：In
  类型：句柄
  可空：N
  意思：输入绑定的客户端
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_SDKDevice::ModuleSession_SDKDevice_InsertClient(XNETHANDLE xhDevice, XNETHANDLE xhClient)
{
    Session_IsErrorOccur = FALSE;

    st_Locker.lock_shared();
    unordered_map<XNETHANDLE, MODULESESSION_SDKLIST*>::const_iterator stl_MapIterator = stl_MapClient.find(xhDevice);
    if (stl_MapIterator == stl_MapClient.end())
    {
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
        st_Locker.unlock_shared();
        return FALSE;
    }
    list<MODULESESSION_SDKCLIENT> stl_ListClient;

    stl_MapIterator->second->st_Locker.lock();
    stl_MapIterator->second->stl_MapClient.insert(make_pair(xhClient, stl_ListClient));
    stl_MapIterator->second->st_Locker.unlock();
    st_Locker.unlock_shared();
    return TRUE;
}
/********************************************************************
函数名称：ModuleSession_SDKDevice_Insert
函数功能：绑定插入一个客户端
 参数.一：xhDevice
  In/Out：In
  类型：句柄
  可空：N
  意思：输入设备句柄
 参数.二：xhClient
  In/Out：In
  类型：句柄
  可空：N
  意思：输入绑定的客户端句柄
 参数.三：nChannel
  In/Out：In
  类型：整数型
  可空：N
  意思：输入绑定的通道
 参数.四：bLive
  In/Out：In
  类型：逻辑型
  可空：N
  意思：输入是直播还是录像
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_SDKDevice::ModuleSession_SDKDevice_InsertDevice(XNETHANDLE xhDevice, XNETHANDLE xhClient, int nChannel, BOOL bLive)
{
    Session_IsErrorOccur = FALSE;

    //查找
    st_Locker.lock_shared();
    unordered_map<XNETHANDLE, MODULESESSION_SDKLIST*>::iterator stl_MapIterator = stl_MapClient.find(xhDevice);
    if (stl_MapIterator == stl_MapClient.end())
    {
        Session_IsErrorOccur = TRUE;
        Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
        st_Locker.unlock_shared();
        return FALSE;
    }
    //开始绑定,查找客户端
    stl_MapIterator->second->st_Locker.lock();
    unordered_map<XNETHANDLE, list<MODULESESSION_SDKCLIENT> >::iterator stl_MapClientIterator = stl_MapIterator->second->stl_MapClient.find(xhClient);
    if (stl_MapClientIterator == stl_MapIterator->second->stl_MapClient.end())
    {
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTCLIENT;
        stl_MapIterator->second->st_Locker.unlock();
		st_Locker.unlock_shared();
		return FALSE;
    }
    BOOL bFound = FALSE;
    //是否存在客户端
    for (auto stl_ListIterator = stl_MapClientIterator->second.begin(); stl_ListIterator != stl_MapClientIterator->second.end(); stl_ListIterator++)
    {
        if ((nChannel == stl_ListIterator->nChannel) && (bLive == stl_ListIterator->bLive))
        {
            bFound = TRUE;
            break;
        }
    }
    if (bFound)
    {
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_EXIST;
		stl_MapIterator->second->st_Locker.unlock();
		st_Locker.unlock_shared();
		return FALSE;
    }
    MODULESESSION_SDKCLIENT st_SessionClient;
    memset(&st_SessionClient, '\0', sizeof(MODULESESSION_SDKCLIENT));

    st_SessionClient.bLive = bLive;
    st_SessionClient.nChannel = nChannel;

    stl_MapClientIterator->second.push_back(st_SessionClient);
    stl_MapIterator->second->st_Locker.unlock();
    st_Locker.unlock_shared();
    return TRUE;
}
/********************************************************************
函数名称：ModuleSession_SDKDevice_GetIdleClient
函数功能：获得一个可以使用的客户端句柄
 参数.一：xhDevice
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的设备
 参数.二：pxhClient
  In/Out：Out
  类型：句柄
  可空：N
  意思：输出获得的句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_SDKDevice::ModuleSession_SDKDevice_GetIdleClient(XNETHANDLE xhDevice, XNETHANDLE* pxhClient)
{
    Session_IsErrorOccur = FALSE;

    if (NULL == pxhClient)
    {
        Session_IsErrorOccur = TRUE;
        Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_PARAMENT;
        return FALSE;
    }
	//查找
	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, MODULESESSION_SDKLIST*>::iterator stl_MapIterator = stl_MapClient.find(xhDevice);
	if (stl_MapIterator == stl_MapClient.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return FALSE;
	}
    unsigned int nListCount = 100000; //最大任务个数
    //查找最小
    stl_MapIterator->second->st_Locker.lock();
    for (auto stl_MapClientIterator = stl_MapIterator->second->stl_MapClient.begin(); stl_MapClientIterator != stl_MapIterator->second->stl_MapClient.end(); stl_MapClientIterator++)
    {
        if (stl_MapClientIterator->second.size() < nListCount)
        {
            nListCount = stl_MapClientIterator->second.size();
            *pxhClient = stl_MapClientIterator->first;
        }
    }
    stl_MapIterator->second->st_Locker.unlock();
    st_Locker.unlock_shared();
    return TRUE;
}
/********************************************************************
函数名称：ModuleSession_SDKDevice_GetClient
函数功能：获取设备绑定的客户端
 参数.一：xhDevice
  In/Out：In
  类型：句柄
  可空：N
  意思：设备句柄
 参数.二：nChannel
  In/Out：In
  类型：整数型
  可空：N
  意思：输入绑定的通道
 参数.三：bLive
  In/Out：In
  类型：逻辑型
  可空：N
  意思：输入是直播还是录像
 参数.四：pxhClient
  In/Out：Out
  类型：句柄
  可空：N
  意思：输出客户端句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_SDKDevice::ModuleSession_SDKDevice_GetClient(XNETHANDLE xhDevice, int nChannel, BOOL bLive, XNETHANDLE* pxhClient)
{
    Session_IsErrorOccur = FALSE;

	//查找
	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, MODULESESSION_SDKLIST*>::iterator stl_MapIterator = stl_MapClient.find(xhDevice);
	if (stl_MapIterator == stl_MapClient.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTCLIENT;
		st_Locker.unlock_shared();
		return FALSE;
	}
    BOOL bFound = FALSE;
    stl_MapIterator->second->st_Locker.lock_shared();
    for (auto stl_MapClientIterator = stl_MapIterator->second->stl_MapClient.begin(); stl_MapClientIterator != stl_MapIterator->second->stl_MapClient.end(); stl_MapClientIterator++)
    {
        //当前客户端下的设备
        for (auto stl_ListIterator = stl_MapClientIterator->second.begin(); stl_ListIterator != stl_MapClientIterator->second.end(); stl_ListIterator++)
        {
            //如果找到设备就退出
            if ((nChannel == stl_ListIterator->nChannel) && (bLive == stl_ListIterator->bLive))
            {
				bFound = TRUE; //直接退出
				*pxhClient = stl_MapClientIterator->first;
				break;
            }
        }
        if (bFound)
        {
            break;
        }
    }
    stl_MapIterator->second->st_Locker.unlock_shared();
    st_Locker.unlock_shared();

    if (!bFound)
    {
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
		return FALSE;
    }
    return TRUE;
}
/********************************************************************
函数名称：ModuleSession_SDKDevice_Delete
函数功能：删除绑定的设备
 参数.一：xhDevice
  In/Out：In
  类型：句柄
  可空：N
  意思：设备句柄
 参数.二：nChannel
  In/Out：Out
  类型：整数型
  可空：N
  意思：输入通道号
 参数.三：bLive
  In/Out：Out
  类型：整数型
  可空：N
  意思：直播还是录播
 参数.四：pxhClient
  In/Out：Out
  类型：句柄指针
  可空：Y
  意思：输出绑定的句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_SDKDevice::ModuleSession_SDKDevice_Delete(XNETHANDLE xhDevice, int nChannel, BOOL bLive, XNETHANDLE* pxhClient /* = NULL */)
{
    Session_IsErrorOccur = FALSE;

	//查找
	st_Locker.lock_shared();
	unordered_map<XNETHANDLE, MODULESESSION_SDKLIST*>::iterator stl_MapIterator = stl_MapClient.find(xhDevice);
	if (stl_MapIterator == stl_MapClient.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTCLIENT;
		st_Locker.unlock_shared();
		return FALSE;
	}
	BOOL bFound = FALSE;
	stl_MapIterator->second->st_Locker.lock();
	for (auto stl_MapClientIterator = stl_MapIterator->second->stl_MapClient.begin(); stl_MapClientIterator != stl_MapIterator->second->stl_MapClient.end(); stl_MapClientIterator++)
	{
		//当前客户端下的设备
		for (auto stl_ListIterator = stl_MapClientIterator->second.begin(); stl_ListIterator != stl_MapClientIterator->second.end(); stl_ListIterator++)
		{
			//如果找到设备就退出
			if ((nChannel == stl_ListIterator->nChannel) && (bLive == stl_ListIterator->bLive))
			{
				bFound = TRUE;
                if (NULL != pxhClient)
                {
                    *pxhClient = stl_MapClientIterator->first;
                }
                stl_MapClientIterator->second.erase(stl_ListIterator);
				break;
			}
		}
		if (bFound)
		{
			break;
		}
	}
	stl_MapIterator->second->st_Locker.unlock();
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
    for (auto stl_MapIterator = stl_MapClient.begin(); stl_MapIterator != stl_MapClient.end(); stl_MapIterator++)
    {
        stl_MapIterator->second->st_Locker.lock();
        stl_MapIterator->second->stl_MapClient.clear();
        stl_MapIterator->second->st_Locker.unlock();

        delete stl_MapIterator->second;
        stl_MapIterator->second = NULL;
    }
    stl_MapClient.clear();
    st_Locker.unlock();
    return TRUE;
}