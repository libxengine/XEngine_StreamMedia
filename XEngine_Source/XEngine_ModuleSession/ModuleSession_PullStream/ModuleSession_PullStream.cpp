#include "pch.h"
#include "ModuleSession_PullStream.h"
/********************************************************************
//    Created:     2023/06/05  16:11:53
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_PullStream\ModuleSession_PullStream.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_PullStream
//    File Base:   ModuleSession_PullStream
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     拉流端会话管理器
//    History:
*********************************************************************/
CModuleSession_PullStream::CModuleSession_PullStream()
{
}
CModuleSession_PullStream::~CModuleSession_PullStream()
{
}
//////////////////////////////////////////////////////////////////////////
//                    公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：ModuleSession_PullStream_Insert
函数功能：插入一个拉流客户端到管理器
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的客户端
 参数.二：lpszSMSAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要绑定的流媒体ID
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PullStream::ModuleSession_PullStream_Insert(LPCXSTR lpszClientAddr, LPCXSTR lpszSMSAddr)
{
    Session_IsErrorOccur = false;

	PULLSTREAM_CLIENTINFO* pSt_PullStream = new PULLSTREAM_CLIENTINFO;
	if (NULL == pSt_PullStream)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_MALLOC;
		return false;
	}
    memset(pSt_PullStream, '\0', sizeof(PULLSTREAM_CLIENTINFO));

    _tcsxcpy(pSt_PullStream->tszSMSAddr, lpszSMSAddr);

    st_Locker.lock();
    stl_MapClient.insert(make_pair(lpszClientAddr, pSt_PullStream));
    st_Locker.unlock();
    return true;
}
/********************************************************************
函数名称：ModuleSession_PullStream_Delete
函数功能：删除一个拉流端
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的客户端
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PullStream::ModuleSession_PullStream_Delete(LPCXSTR lpszClientAddr)
{
	Session_IsErrorOccur = false;

	if (NULL == lpszClientAddr)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_PARAMENT;
		return false;
	}
	st_Locker.lock();
	auto stl_MapIterator = stl_MapClient.find(lpszClientAddr);
	if (stl_MapIterator != stl_MapClient.end())
	{
		stl_MapClient.erase(stl_MapIterator);
	}
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：ModuleSession_PullStream_Get
函数功能：获取客户端绑定的流ID
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的客户端
 参数.二：ptszSMSAddr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出流媒体ID
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PullStream::ModuleSession_PullStream_Get(LPCXSTR lpszClientAddr, XCHAR* ptszSMSAddr)
{
	Session_IsErrorOccur = false;

	if (NULL == lpszClientAddr)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_PARAMENT;
		return false;
	}
    st_Locker.lock_shared();
	//查找最小
    auto stl_MapIterator = stl_MapClient.find(lpszClientAddr);
    if (stl_MapIterator == stl_MapClient.end())
    {
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
        st_Locker.unlock_shared();
		return false;
    }
    _tcsxcpy(ptszSMSAddr, stl_MapIterator->second->tszSMSAddr);
	st_Locker.unlock_shared();
	return true;
}