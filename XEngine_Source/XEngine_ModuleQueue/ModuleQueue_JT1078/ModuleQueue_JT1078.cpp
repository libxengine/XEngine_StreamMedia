#include "pch.h"
#include "ModuleQueue_JT1078.h"
/********************************************************************
//    Created:     2023/09/19  11:12:01
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleQueue\ModuleQueue_JT1078\ModuleQueue_JT1078.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleQueue\ModuleQueue_JT1078
//    File Base:   ModuleQueue_JT1078
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     JT1078队列服务
//    History:
*********************************************************************/
CModuleQueue_JT1078::CModuleQueue_JT1078()
{
}
CModuleQueue_JT1078::~CModuleQueue_JT1078()
{
}
//////////////////////////////////////////////////////////////////////////
//                    公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：ModuleQueue_JT1078_Create
函数功能：创建一个JT1078队列
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
bool CModuleQueue_JT1078::ModuleQueue_JT1078_Create(LPCXSTR lpszClientAddr)
{
	Queue_IsErrorOccur = false;

	if ((NULL == lpszClientAddr) || (NULL == lpszClientAddr))
	{
		Queue_IsErrorOccur = true;
		Queue_dwErrorCode = ERROR_STREAMMEDIA_MODULE_QUEUE_PARAMENT;
		return false;
	}
	//申请内存
	MODULEQUEUE_PACKET st_Packet;
	memset(&st_Packet, '\0', sizeof(MODULEQUEUE_PACKET));

	st_Packet.nMsgLen = 0;
	st_Packet.ptszMsgBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);
	if (NULL == st_Packet.ptszMsgBuffer)
	{
		Queue_IsErrorOccur = true;
		Queue_dwErrorCode = ERROR_STREAMMEDIA_MODULE_QUEUE_MALLOC;
		return false;
	}
	//是否存在
	st_Locker.lock();
	unordered_map<xstring, MODULEQUEUE_PACKET>::iterator stl_MapIterator = stl_MapQueue.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapQueue.end())
	{
		//不存在就创建
		stl_MapQueue.insert(make_pair(lpszClientAddr, st_Packet));
	}
	else
	{
		//存在
		Queue_IsErrorOccur = true;
		Queue_dwErrorCode = ERROR_STREAMMEDIA_MODULE_QUEUE_EXIST;
		st_Locker.unlock();
		return false;
	}
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：ModuleQueue_JT1078_Destroy
函数功能：销毁一个队列
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
bool CModuleQueue_JT1078::ModuleQueue_JT1078_Destroy(LPCXSTR lpszClientAddr)
{
	Queue_IsErrorOccur = false;

	if (NULL == lpszClientAddr)
	{
		Queue_IsErrorOccur = true;
		Queue_dwErrorCode = ERROR_STREAMMEDIA_MODULE_QUEUE_PARAMENT;
		return false;
	}
	//设备编号是否存在
	st_Locker.lock();
	unordered_map<xstring, MODULEQUEUE_PACKET>::iterator stl_MapIterator = stl_MapQueue.find(lpszClientAddr);
	if (stl_MapIterator != stl_MapQueue.end())
	{
		free(stl_MapIterator->second.ptszMsgBuffer);
		stl_MapIterator->second.ptszMsgBuffer = NULL;
		stl_MapQueue.erase(stl_MapIterator);
	}
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：ModuleQueue_JT1078_Insert
函数功能：插入一段数据到队列
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端地址
 参数.二：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入插入的数据
 参数.三：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入插入大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleQueue_JT1078::ModuleQueue_JT1078_Insert(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen)
{
	Queue_IsErrorOccur = false;

	if (NULL == lpszClientAddr)
	{
		Queue_IsErrorOccur = true;
		Queue_dwErrorCode = ERROR_STREAMMEDIA_MODULE_QUEUE_PARAMENT;
		return false;
	}
	//设备编号是否存在
	st_Locker.lock_shared();
	unordered_map<xstring, MODULEQUEUE_PACKET>::iterator stl_MapIterator = stl_MapQueue.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapQueue.end())
	{
		Queue_IsErrorOccur = true;
		Queue_dwErrorCode = ERROR_STREAMMEDIA_MODULE_QUEUE_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	memcpy(stl_MapIterator->second.ptszMsgBuffer + stl_MapIterator->second.nMsgLen, lpszMsgBuffer, nMsgLen);
	stl_MapIterator->second.nMsgLen += nMsgLen;
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleQueue_JT1078_Get
函数功能：获取队列中的数据
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的推流端
 参数.二：pptszMsgBuffer
  In/Out：Out
  类型：字符指针的指针
  可空：N
  意思：输出一个完整的帧数据
 参数.三：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出数据大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleQueue_JT1078::ModuleQueue_JT1078_Get(LPCXSTR lpszClientAddr, XCHAR** pptszMsgBuffer, int* pInt_MsgLen)
{
	Queue_IsErrorOccur = false;

	if (NULL == lpszClientAddr)
	{
		Queue_IsErrorOccur = true;
		Queue_dwErrorCode = ERROR_STREAMMEDIA_MODULE_QUEUE_PARAMENT;
		return false;
	}
	//设备编号是否存在
	st_Locker.lock_shared();
	unordered_map<xstring, MODULEQUEUE_PACKET>::iterator stl_MapIterator = stl_MapQueue.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapQueue.end())
	{
		Queue_IsErrorOccur = true;
		Queue_dwErrorCode = ERROR_STREAMMEDIA_MODULE_QUEUE_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	*pInt_MsgLen = stl_MapIterator->second.nMsgLen;
	*pptszMsgBuffer = stl_MapIterator->second.ptszMsgBuffer;

	st_Locker.unlock_shared();
	return true;
}