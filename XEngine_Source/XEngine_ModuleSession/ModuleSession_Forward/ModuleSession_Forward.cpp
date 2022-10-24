#include "pch.h"
#include "ModuleSession_Forward.h"
/********************************************************************
//    Created:     2022/10/17  13:59:02
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_Forward\ModuleSession_Forward.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_Forward
//    File Base:   ModuleSession_Forward
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     媒体转发服务
//    History:
*********************************************************************/
CModuleSession_Forward::CModuleSession_Forward()
{
}
CModuleSession_Forward::~CModuleSession_Forward()
{
}
//////////////////////////////////////////////////////////////////////////
//                    公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：ModuleSession_Forward_Create
函数功能：创建转发服务
 参数.一：xhPlay
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要创建的句柄
 参数.二：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入绑定的句柄
 参数.三：lpszSMSPlay
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入播放地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_Forward::ModuleSession_Forward_Create(LPCTSTR lpszPlay, XHANDLE xhToken, LPCTSTR lpszSMSPlay)
{
    Session_IsErrorOccur = FALSE;

    MODULESESSION_FORWARD st_Forward;
	memset(&st_Forward, '\0', sizeof(MODULESESSION_FORWARD));

    st_Forward.xhToken = xhToken;
	_tcscpy(st_Forward.tszPlayUrl, lpszSMSPlay);

    st_Locker.lock();
    stl_MapClient.insert(make_pair(lpszPlay, st_Forward));
    st_Locker.unlock();
    return TRUE;
}
/********************************************************************
函数名称：ModuleSession_Forward_Get
函数功能：获取句柄绑定信息
 参数.一：xhPlay
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
返回值
  类型：句柄型
  意思：成功返回句柄,失败返回NULL
备注：
*********************************************************************/
XHANDLE CModuleSession_Forward::ModuleSession_Forward_Get(LPCTSTR lpszPlay)
{
    Session_IsErrorOccur = FALSE;

	st_Locker.lock_shared();
	unordered_map<string, MODULESESSION_FORWARD>::const_iterator stl_MapIterator = stl_MapClient.find(lpszPlay);
	if (stl_MapIterator == stl_MapClient.end())
	{
		Session_IsErrorOccur = TRUE;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return NULL;
	}
    XHANDLE xhToken = stl_MapIterator->second.xhToken;
	st_Locker.unlock_shared();
    return xhToken;
}
/********************************************************************
函数名称：ModuleSession_Forward_Delete
函数功能：删除绑定句柄
 参数.一：xhPlay
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_Forward::ModuleSession_Forward_Delete(LPCTSTR lpszPlay)
{
    Session_IsErrorOccur = FALSE;

	st_Locker.lock();
	unordered_map<string, MODULESESSION_FORWARD>::const_iterator stl_MapIterator = stl_MapClient.find(lpszPlay);
	if (stl_MapIterator != stl_MapClient.end())
	{
        stl_MapClient.erase(stl_MapIterator);
	}
	st_Locker.unlock();
    return TRUE;
}
/********************************************************************
函数名称：ModuleSession_Forward_List
函数功能：枚举当前播放列表
 参数.一：pppSt_Forward
  In/Out：In/Out
  类型：三级指针
  可空：N
  意思：输出播放列表
 参数.二：pInt_ListCount
  In/Out：In/Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleSession_Forward::ModuleSession_Forward_List(MODULESESSION_FORWARDINFO*** pppSt_Forward, int* pInt_ListCount)
{
	Session_IsErrorOccur = FALSE;

	st_Locker.lock_shared();
	*pInt_ListCount = stl_MapClient.size();
	BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_Forward, *pInt_ListCount, sizeof(MODULESESSION_FORWARDINFO));

	unordered_map<string, MODULESESSION_FORWARD>::const_iterator stl_MapIterator = stl_MapClient.begin();
	for (int i = 0; stl_MapIterator != stl_MapClient.end(); stl_MapIterator++, i++)
	{
		_tcscpy((*pppSt_Forward)[i]->tszToken, stl_MapIterator->first.c_str());
		_tcscpy((*pppSt_Forward)[i]->tszAVUrl, stl_MapIterator->second.tszPlayUrl);
	}
	st_Locker.unlock_shared();
	
	return TRUE;
}