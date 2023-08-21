#include "pch.h"
#include "ModuleSession_PushStream.h"
/********************************************************************
//    Created:     2023/06/04  20:19:13
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_PushStream\ModuleSession_PushStream.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_PushStream
//    File Base:   ModuleSession_PushStream
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     推流全局会话
//    History:
*********************************************************************/
CModuleSession_PushStream::CModuleSession_PushStream()
{
}
CModuleSession_PushStream::~CModuleSession_PushStream()
{
}
//////////////////////////////////////////////////////////////////////////
//                    公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：ModuleSession_PushStream_Create
函数功能：创建一个推流会话管理器
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端地址
 参数.二：lpszSMSAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入流媒体ID
 参数.三：enStreamType
  In/Out：In
  类型：枚举型
  可空：N
  意思：输入推流类型
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PushStream::ModuleSession_PushStream_Create(LPCXSTR lpszClientAddr, LPCXSTR lpszSMSAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enStreamType)
{
	Session_IsErrorOccur = false;

	if ((NULL == lpszClientAddr) || (NULL == lpszClientAddr))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_PARAMENT;
		return false;
	}
	//申请内存
	PUSHSTREAM_PACKET* pSt_Packet = new PUSHSTREAM_PACKET;
	if (NULL == pSt_Packet)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_MALLOC;
		return false;
	}
	memset(pSt_Packet, '\0', sizeof(PUSHSTREAM_PACKET));

	pSt_Packet->st_ClientLocker = make_unique<mutex>();
	pSt_Packet->pStl_ListClient = make_unique<list<STREAMMEDIA_SESSIONCLIENT>>();
	
	if ((NULL == pSt_Packet->pStl_ListClient))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_MALLOC;
		return false;
	}
	pSt_Packet->enStreamType = enStreamType;
	_tcsxcpy(pSt_Packet->tszSMSAddr, lpszSMSAddr);
	//是否存在
	st_Locker.lock();
	unordered_map<xstring, PUSHSTREAM_PACKET *>::iterator stl_MapIterator = stl_MapPushStream.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapPushStream.end())
	{
		//不存在就创建
		stl_MapPushStream.insert(make_pair(lpszClientAddr, pSt_Packet));
	}
	else
	{
		//存在
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_EXIST;
		st_Locker.unlock();
		return false;
	}
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：ModuleSession_PushStream_Destroy
函数功能：销毁一个管理器
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PushStream::ModuleSession_PushStream_Destroy(LPCXSTR lpszClientAddr)
{
	Session_IsErrorOccur = false;

	if (NULL == lpszClientAddr)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_PARAMENT;
		return false;
	}
	//设备编号是否存在
	st_Locker.lock();
	unordered_map<xstring, PUSHSTREAM_PACKET *>::iterator stl_MapIterator = stl_MapPushStream.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapPushStream.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock();
		return false;
	}
	delete stl_MapIterator->second;
	stl_MapIterator->second = NULL;
	stl_MapPushStream.erase(stl_MapIterator);
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：ModuleSession_PushStream_GetAddrForAddr
函数功能：通过地址获取流地址
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端地址
 参数.二：ptszSMSAddr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出获取到的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PushStream::ModuleSession_PushStream_GetAddrForAddr(LPCXSTR lpszClientAddr, XCHAR* ptszSMSAddr)
{
	Session_IsErrorOccur = false;

	if (NULL == lpszClientAddr)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_PARAMENT;
		return false;
	}
	//设备编号是否存在
	st_Locker.lock_shared();
	unordered_map<xstring, PUSHSTREAM_PACKET*>::iterator stl_MapIterator = stl_MapPushStream.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapPushStream.end())
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
/********************************************************************
函数名称：ModuleSession_PushStream_SetAVInfo
函数功能：设置推流的音视频信息
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的推流端
 参数.二：pSt_AVInfo
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要保存的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PushStream::ModuleSession_PushStream_SetAVInfo(LPCXSTR lpszClientAddr, XENGINE_PROTOCOL_AVINFO* pSt_AVInfo)
{
	Session_IsErrorOccur = false;

	if (NULL == lpszClientAddr)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_PARAMENT;
		return false;
	}
	//设备编号是否存在
	st_Locker.lock_shared();
	unordered_map<xstring, PUSHSTREAM_PACKET*>::iterator stl_MapIterator = stl_MapPushStream.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapPushStream.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	stl_MapIterator->second->st_AVInfo = *pSt_AVInfo;
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_PushStream_GetAVInfo
函数功能：获取推流端音视频信息
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的推流端
 参数.二：pSt_AVInfo
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出保存的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PushStream::ModuleSession_PushStream_GetAVInfo(LPCXSTR lpszClientAddr, XENGINE_PROTOCOL_AVINFO* pSt_AVInfo)
{
	Session_IsErrorOccur = false;

	if (NULL == lpszClientAddr)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_PARAMENT;
		return false;
	}
	//设备编号是否存在
	st_Locker.lock_shared();
	unordered_map<xstring, PUSHSTREAM_PACKET*>::iterator stl_MapIterator = stl_MapPushStream.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapPushStream.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	*pSt_AVInfo = stl_MapIterator->second->st_AVInfo;
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_PushStream_FindStream
函数功能：查找流对应地址
 参数.一：lpszSMSAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要查找的流
 参数.二：ptszClientAddr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输入要查找的地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PushStream::ModuleSession_PushStream_FindStream(LPCXSTR lpszSMSAddr, XCHAR* ptszClientAddr)
{
	Session_IsErrorOccur = false;

	if (NULL == lpszSMSAddr)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_PARAMENT;
		return false;
	}
	bool bFound = false;

	st_Locker.lock_shared();
	unordered_map<xstring, PUSHSTREAM_PACKET*>::iterator stl_MapIterator = stl_MapPushStream.begin();
	for (; stl_MapIterator != stl_MapPushStream.end(); stl_MapIterator++)
	{
		if (0 == _tcsxnicmp(lpszSMSAddr, stl_MapIterator->second->tszSMSAddr, _tcsxlen(lpszSMSAddr)))
		{
			bFound = true;
			break;
		}
	}
	if (!bFound)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	_tcsxcpy(ptszClientAddr, stl_MapIterator->first.c_str());
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_PushStream_GetInfo
函数功能：获取推流信息
 参数.一：pppSt_ProtocolStream
  In/Out：In/Out
  类型：三级指针
  可空：N
  意思：输出推流统计信息
 参数.二：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出获取到的个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PushStream::ModuleSession_PushStream_GetInfo(STREAMMEDIA_PUBLISHINFO*** pppSt_ProtocolStream, int* pInt_ListCount)
{
	Session_IsErrorOccur = false;

	st_Locker.lock_shared();

	*pInt_ListCount = stl_MapPushStream.size();
	BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_ProtocolStream, stl_MapPushStream.size(), sizeof(STREAMMEDIA_PUBLISHINFO));

	unordered_map<xstring, PUSHSTREAM_PACKET*>::iterator stl_MapIterator = stl_MapPushStream.begin();
	for (int i = 0; stl_MapIterator != stl_MapPushStream.end(); stl_MapIterator++, i++)
	{
		(*pppSt_ProtocolStream)[i]->nClientCount = stl_MapIterator->second->pStl_ListClient->size();
		(*pppSt_ProtocolStream)[i]->enStreamType = stl_MapIterator->second->enStreamType;
		(*pppSt_ProtocolStream)[i]->st_AVInfo = stl_MapIterator->second->st_AVInfo;
		_tcsxcpy((*pppSt_ProtocolStream)[i]->tszSMSAddr, stl_MapIterator->second->tszSMSAddr);
	}
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_PushStream_ClientInsert
函数功能：客户端插入
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端地址
 参数.二：lpszPullAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入插入的客户端
 参数.三：enStreamType
  In/Out：In
  类型：枚举型
  可空：N
  意思：输入客户端拉流类型
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PushStream::ModuleSession_PushStream_ClientInsert(LPCXSTR lpszClientAddr, LPCXSTR lpszPullAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enStreamType)
{
	Session_IsErrorOccur = false;

	if (NULL == lpszClientAddr)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_PARAMENT;
		return false;
	}
	//是否存在
	st_Locker.lock_shared();
	unordered_map<xstring, PUSHSTREAM_PACKET*>::iterator stl_MapIterator = stl_MapPushStream.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapPushStream.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	STREAMMEDIA_SESSIONCLIENT st_Client;
	memset(&st_Client, '\0', sizeof(STREAMMEDIA_SESSIONCLIENT));

	st_Client.enClientType = enStreamType;
	_tcsxcpy(st_Client.tszClientID, lpszPullAddr);

	stl_MapIterator->second->st_ClientLocker->lock();
	stl_MapIterator->second->pStl_ListClient->push_back(st_Client);
	stl_MapIterator->second->st_ClientLocker->unlock();
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_PushStream_ClientDelete
函数功能：客户端删除
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端地址
 参数.二：lpszPullAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入删除的客户端
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PushStream::ModuleSession_PushStream_ClientDelete(LPCXSTR lpszClientAddr, LPCXSTR lpszPullAddr)
{
	Session_IsErrorOccur = false;

	if (NULL == lpszClientAddr)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_PARAMENT;
		return false;
	}
	//是否存在
	st_Locker.lock_shared();
	unordered_map<xstring, PUSHSTREAM_PACKET*>::iterator stl_MapIterator = stl_MapPushStream.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapPushStream.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	//查找并且释放客户端
	stl_MapIterator->second->st_ClientLocker->lock();
	for (auto stl_ListIterator = stl_MapIterator->second->pStl_ListClient->begin(); stl_ListIterator != stl_MapIterator->second->pStl_ListClient->end(); stl_ListIterator++)
	{
		if (0 == _tcsxnicmp(lpszPullAddr, stl_ListIterator->tszClientID, _tcsxlen(lpszPullAddr)))
		{
			stl_MapIterator->second->pStl_ListClient->erase(stl_ListIterator);
			break;
		}
	}
	stl_MapIterator->second->st_ClientLocker->unlock();
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_PushStream_ClientList
函数功能：客户端获取
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端地址
 参数.二：pStl_ListClient
  In/Out：Out
  类型：容器指针
  可空：N
  意思：输出获取到的列表
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PushStream::ModuleSession_PushStream_ClientList(LPCXSTR lpszClientAddr, list<STREAMMEDIA_SESSIONCLIENT> *pStl_ListClient)
{
	Session_IsErrorOccur = false;

	if (NULL == lpszClientAddr)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_PARAMENT;
		return false;
	}
	//是否存在
	st_Locker.lock_shared();
	unordered_map<xstring, PUSHSTREAM_PACKET*>::iterator stl_MapIterator = stl_MapPushStream.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapPushStream.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	//查找并且释放客户端
	stl_MapIterator->second->st_ClientLocker->lock();
	*pStl_ListClient = *stl_MapIterator->second->pStl_ListClient;
	stl_MapIterator->second->st_ClientLocker->unlock();
	st_Locker.unlock_shared();
	return true;
}