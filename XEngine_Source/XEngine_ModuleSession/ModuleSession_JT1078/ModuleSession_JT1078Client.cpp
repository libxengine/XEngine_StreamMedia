#include "pch.h"
#include "ModuleSession_JT1078Client.h"
/********************************************************************
//    Created:     2022/04/25  10:43:22
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_JT1078\ModuleSession_JT1078Client.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_JT1078
//    File Base:   ModuleSession_JT1078Client
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     客户端会话管理
//    History:
*********************************************************************/
CModuleSession_JT1078Client::CModuleSession_JT1078Client()
{
}
CModuleSession_JT1078Client::~CModuleSession_JT1078Client()
{
}
//////////////////////////////////////////////////////////////////////////
//                    公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：ModuleSession_JT1078Stream_InsertDevice
函数功能：创建一个客户端会话
 参数.一：xhClient
  In/Out：In
  类型：句柄
  可空：N
  意思：输入客户端句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_JT1078Client::ModuleSession_JT1078Client_Create(XNETHANDLE xhClient)
{
    Session_IsErrorOccur = FALSE;

	MODULESESSION_LIST *pSt_SessionList = new MODULESESSION_LIST;
	if (NULL == pSt_SessionList)
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_MODULE_SESSION_JT1078_MALLOC;
		return FALSE;
	}
    st_Locker.lock();
    stl_MapClient.insert(make_pair(xhClient, pSt_SessionList));
    st_Locker.unlock();
    return TRUE;
}
/********************************************************************
函数名称：ModuleSession_JT1078Client_Get
函数功能：获得一个可以使用的客户端句柄
 参数.一：pxhClient
  In/Out：Out
  类型：句柄指针
  可空：N
  意思：输出获得的句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_JT1078Client::ModuleSession_JT1078Client_Get(XNETHANDLE* pxhClient)
{
	Session_IsErrorOccur = FALSE;

	if (NULL == pxhClient)
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_MODULE_SESSION_JT1078_PARAMENT;
		return FALSE;
	}
    unsigned int nListCount = 100000; //最大任务个数
    XNETHANDLE xhClient = 0;          //选择的客户端
    st_Locker.lock_shared();
	//查找最小
    for (auto stl_MapIterator = stl_MapClient.begin(); stl_MapIterator != stl_MapClient.end(); stl_MapIterator++)
    {
        if (stl_MapIterator->second->stl_ListClient.size() < nListCount)
        {
            nListCount = stl_MapIterator->second->stl_ListClient.size();
            xhClient = stl_MapIterator->first;
        }
    }
	st_Locker.unlock_shared();
    *pxhClient = xhClient;
	return TRUE;
}
/********************************************************************
函数名称：ModuleSession_JT1078Client_Exist
函数功能：客户端是否存在
 参数.一：pxhClient
  In/Out：Out
  类型：句柄
  可空：N
  意思：输出客户端句柄
 参数.二：lpszDeviceAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入绑定的设备地址
 参数.三：lpszDeviceNumber
  In/Out：Out
  类型：常量字符指针
  可空：N
  意思：输入绑定的设备编号
 参数.四：nChannel
  In/Out：In
  类型：整数型
  可空：N
  意思：输入绑定的通道
 参数.五：bLive
  In/Out：In
  类型：逻辑型
  可空：N
  意思：输入是直播还是录像
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_JT1078Client::ModuleSession_JT1078Client_Exist(XNETHANDLE* pxhClient, LPCTSTR lpszDeviceAddr, LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive)
{
	Session_IsErrorOccur = FALSE;

	if ((NULL == lpszDeviceAddr) || (NULL == lpszDeviceNumber))
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_MODULE_SESSION_JT1078_PARAMENT;
		return FALSE;
	}
    BOOL bFound = FALSE;
	st_Locker.lock_shared();
    //编译所有
	for (auto stl_MapIterator = stl_MapClient.begin(); stl_MapIterator != stl_MapClient.end(); stl_MapIterator++)
	{
        //编译当前客户端下的设备
        stl_MapIterator->second->st_Locker.lock_shared();
        for (auto stl_ListIterator = stl_MapIterator->second->stl_ListClient.begin(); stl_ListIterator != stl_MapIterator->second->stl_ListClient.end(); stl_ListIterator++)
        {
            //如果找到设备就退出
			if ((0 == _tcsncmp(lpszDeviceNumber, stl_ListIterator->tszDeviceNumber, _tcslen(lpszDeviceNumber))) && (nChannel == stl_ListIterator->nChannel) && (bLive == stl_ListIterator->bLive))
			{
                //如果设备的IP和保存的IP匹配
                if (0 == _tcsncmp(lpszDeviceAddr, stl_ListIterator->tszDeviceAddr, _tcslen(lpszDeviceAddr)))
                {
                    bFound = TRUE; //直接退出
                    *pxhClient = stl_MapIterator->first;
                    break;
                }
                else
                {
                    //不匹配,返回错误
					Session_IsErrorOccur = TRUE;
					Session_dwErrorCode = ERROR_MODULE_SESSION_JT1078_ADDR;
					return FALSE;
                }
			}
        }
        stl_MapIterator->second->st_Locker.unlock_shared();
        if (bFound)
        {
            break;
        }
	}
	st_Locker.unlock_shared();
	return TRUE;
}
/********************************************************************
函数名称：ModuleSession_JT1078Client_Insert
函数功能：绑定插入一个客户端
 参数.一：xhClient
  In/Out：In
  类型：句柄
  可空：N
  意思：输入客户端句柄
 参数.二：lpszDeviceAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入绑定的设备地址
 参数.三：lpszDeviceNumber
  In/Out：Out
  类型：常量字符指针
  可空：N
  意思：输入绑定的设备编号
 参数.四：nChannel
  In/Out：In
  类型：整数型
  可空：N
  意思：输入绑定的通道
 参数.五：bLive
  In/Out：In
  类型：逻辑型
  可空：N
  意思：输入是直播还是录像
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_JT1078Client::ModuleSession_JT1078Client_Insert(XNETHANDLE xhClient, LPCTSTR lpszDeviceAddr, LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive)
{
    Session_IsErrorOccur = FALSE;

    if ((NULL == lpszDeviceAddr) || (NULL == lpszDeviceNumber))
    {
        Session_IsErrorOccur = TRUE;
        Session_dwErrorCode = ERROR_MODULE_SESSION_JT1078_PARAMENT;
        return FALSE;
    }
    //查找
    st_Locker.lock_shared();
    unordered_map<XNETHANDLE, MODULESESSION_LIST*>::iterator stl_MapIterator = stl_MapClient.find(xhClient);
    if (stl_MapIterator == stl_MapClient.end())
    {
        Session_IsErrorOccur = TRUE;
        Session_dwErrorCode = ERROR_MODULE_SESSION_JT1078_NOTCLIENT;
        st_Locker.unlock_shared();
        return FALSE;
    }
    MODULESESSION_CLIENT st_SessionClient;
    memset(&st_SessionClient, '\0', sizeof(MODULESESSION_CLIENT));

    st_SessionClient.bLive = bLive;
    st_SessionClient.nChannel = nChannel;
    _tcscpy(st_SessionClient.tszDeviceAddr, lpszDeviceAddr);
    _tcscpy(st_SessionClient.tszDeviceNumber, lpszDeviceNumber);
    
    stl_MapIterator->second->st_Locker.lock();
    stl_MapIterator->second->stl_ListClient.push_back(st_SessionClient);
    stl_MapIterator->second->st_Locker.unlock();
    st_Locker.unlock_shared();
    return TRUE;
}
/********************************************************************
函数名称：ModuleSession_JT1078Client_DeleteAddr
函数功能：通过IP地址删除绑定的设备
 参数.一：lpszDeviceAddr
  In/Out：In
  类型：句柄
  可空：N
  意思：输入设备IP地址
 参数.二：pxhClient
  In/Out：Out
  类型：句柄指针
  可空：Y
  意思：输出绑定的句柄
 参数.三：ptszDeviceNumber
  In/Out：Out
  类型：常量字符指针
  可空：Y
  意思：输出设备编号
 参数.四：pInt_Channel
  In/Out：Out
  类型：整数型
  可空：Y
  意思：输出通道号
 参数.五：pbLive
  In/Out：Out
  类型：逻辑型
  可空：Y
  意思：输出直播还是录像
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_JT1078Client::ModuleSession_JT1078Client_DeleteAddr(LPCTSTR lpszDeviceAddr, XNETHANDLE* pxhClient /* = NULL */, TCHAR* ptszDeviceNumber /* = NULL */, int* pInt_Channel /* = NULL */, BOOL* pbLive /* = NULL */)
{
    Session_IsErrorOccur = FALSE;

    BOOL bFound = FALSE;
    st_Locker.lock_shared();
    for (auto stl_MapIterator = stl_MapClient.begin(); stl_MapIterator != stl_MapClient.end(); stl_MapIterator++)
    {
		stl_MapIterator->second->st_Locker.lock();
		//循环查找
		for (auto stl_ListIterator = stl_MapIterator->second->stl_ListClient.begin(); stl_ListIterator != stl_MapIterator->second->stl_ListClient.end(); stl_ListIterator++)
		{
			if (0 == _tcsncmp(lpszDeviceAddr, stl_ListIterator->tszDeviceAddr, _tcslen(lpszDeviceAddr)))
			{
                //导出数据
                if (NULL != pxhClient)
                {
                    *pxhClient = stl_MapIterator->first;
                }
				if (NULL != ptszDeviceNumber)
				{
                    _tcscpy(ptszDeviceNumber, stl_ListIterator->tszDeviceNumber);
				}
                if (NULL != pInt_Channel)
                {
                    *pInt_Channel = stl_ListIterator->nChannel;
                }
                if (NULL != pbLive)
                {
                    *pbLive = stl_ListIterator->bLive;
                }
                bFound = TRUE;
				stl_MapIterator->second->stl_ListClient.erase(stl_ListIterator);
				break;
			}
		}
		stl_MapIterator->second->st_Locker.unlock();

        if (bFound)
        {
            break;
        }
    }
    st_Locker.unlock_shared();
    return TRUE;
}
/********************************************************************
函数名称：ModuleSession_JT1078Client_DeleteNumber
函数功能：通过设备信息删除绑定信息
 参数.一：lpszDeviceNumber
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入设备编号
 参数.二：nChannel
  In/Out：In
  类型：整数型
  可空：N
  意思：通道号
 参数.三：bLive
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：直播还是录像
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_JT1078Client::ModuleSession_JT1078Client_DeleteNumber(LPCTSTR lpszDeviceNumber, int nChannel, BOOL bLive)
{
	Session_IsErrorOccur = FALSE;

    BOOL bFound = FALSE;
	st_Locker.lock_shared();
	for (auto stl_MapIterator = stl_MapClient.begin(); stl_MapIterator != stl_MapClient.end(); stl_MapIterator++)
	{
		stl_MapIterator->second->st_Locker.lock();
		//循环查找
		for (auto stl_ListIterator = stl_MapIterator->second->stl_ListClient.begin(); stl_ListIterator != stl_MapIterator->second->stl_ListClient.end(); stl_ListIterator++)
		{
			if ((0 == _tcsncmp(lpszDeviceNumber, stl_ListIterator->tszDeviceNumber, _tcslen(lpszDeviceNumber))) && (nChannel == stl_ListIterator->nChannel) && (bLive == stl_ListIterator->bLive))
			{
				//找到后退出
				stl_MapIterator->second->stl_ListClient.erase(stl_ListIterator);
				break;
			}
		}
		stl_MapIterator->second->st_Locker.unlock();
		if (bFound)
		{
			break;
		}
	}
	st_Locker.unlock_shared();
	return TRUE;
}
/********************************************************************
函数名称：ModuleSession_JT1078Client_Destory
函数功能：销毁客户端会话管理器
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_JT1078Client::ModuleSession_JT1078Client_Destory()
{
	Session_IsErrorOccur = FALSE;

	st_Locker.lock();
    for (auto stl_MapIterator = stl_MapClient.begin(); stl_MapIterator != stl_MapClient.end(); stl_MapIterator++)
    {
        stl_MapIterator->second->st_Locker.lock();
        stl_MapIterator->second->stl_ListClient.clear();
        stl_MapIterator->second->st_Locker.unlock();

        delete stl_MapIterator->second;
        stl_MapIterator->second = NULL;
    }
    stl_MapClient.clear();
    st_Locker.unlock();
	return TRUE;
}