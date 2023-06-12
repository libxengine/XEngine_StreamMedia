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
 参数.三：xhFLVStream
  In/Out：In
  类型：整数型
  可空：N
  意思：输入绑定的FLV打包句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PushStream::ModuleSession_PushStream_Create(LPCXSTR lpszClientAddr, LPCXSTR lpszSMSAddr, XNETHANDLE xhFLVStream)
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
	pSt_Packet->st_MSGLocker = make_unique<mutex>();
	pSt_Packet->pStl_ListClient = make_unique<list<xstring>>();
	pSt_Packet->pStl_ListPacket = make_unique<list<AVPACKET_MSGBUFFER>>();
	
	if ((NULL == pSt_Packet->pStl_ListPacket) || (NULL == pSt_Packet->pStl_ListClient))
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_MALLOC;
		return false;
	}
	pSt_Packet->xhFLVStream = xhFLVStream;
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
函数名称：ModuleSession_PushStream_GetTokenForAddr
函数功能：通过地址获取流句柄
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端地址
 参数.二：pxhToken
  In/Out：Out
  类型：句柄
  可空：N
  意思：输出获取到的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PushStream::ModuleSession_PushStream_GetTokenForAddr(LPCXSTR lpszClientAddr, XNETHANDLE* pxhToken)
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
	*pxhToken = stl_MapIterator->second->xhFLVStream;
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_PushStream_SetHDRBuffer
函数功能：设置流ID的缓存头
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端地址
 参数.二：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要缓存的数据
 参数.三：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入缓存大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PushStream::ModuleSession_PushStream_SetHDRBuffer(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen)
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
	//准备头
	stl_MapIterator->second->nMsgLen = _xstprintf(stl_MapIterator->second->tszMsgBuffer, _X("HTTP/1.1 200 OK\r\n"
		"Connection: Close\r\n"
		"Content-Type: video/x-flv\r\n"
		"Server: XEngine/8.13.0.1001\r\n"
		"Transfer-Encoding: chunked\r\n\r\n"
		"%x\r\n"), nMsgLen);
	//拷贝数据
	memcpy(stl_MapIterator->second->tszMsgBuffer + stl_MapIterator->second->nMsgLen, lpszMsgBuffer, nMsgLen);
	stl_MapIterator->second->nMsgLen += nMsgLen;
	//拷贝结尾
	memcpy(stl_MapIterator->second->tszMsgBuffer + stl_MapIterator->second->nMsgLen, _X("\r\n"), 2);
	stl_MapIterator->second->nMsgLen += 2;
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_PushStream_GetHDRBuffer
函数功能：获取流ID的缓存头
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端地址
 参数.二：ptszMsgBuffer
  In/Out：In
  类型：字符指针
  可空：N
  意思：输出获取到的数据
 参数.三：pInt_MsgLen
  In/Out：In
  类型：整数型指针
  可空：N
  意思：输出数据大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PushStream::ModuleSession_PushStream_GetHDRBuffer(LPCXSTR lpszClientAddr, XCHAR* ptszMsgBuffer, int* pInt_MsgLen)
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
	*pInt_MsgLen = stl_MapIterator->second->nMsgLen;
	memcpy(ptszMsgBuffer, stl_MapIterator->second->tszMsgBuffer, stl_MapIterator->second->nMsgLen);
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
函数名称：ModuleSession_PushStream_Send
函数功能：投递一段数据给会话管理器
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端地址
 参数.二：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要投递的数据缓冲区
 参数.三：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入缓冲区大小
 参数.四：nAVType
  In/Out：In
  类型：整数型
  可空：N
  意思：输入缓冲区类型.0视频1音频
 参数.五：nFrameType
  In/Out：In
  类型：整数型
  可空：N
  意思：输入帧类型,视频的关键帧还是PB
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PushStream::ModuleSession_PushStream_Send(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nAVType, int nFrameType)
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

	AVPACKET_MSGBUFFER st_MSGBuffer;
	memset(&st_MSGBuffer, '\0', sizeof(AVPACKET_MSGBUFFER));

	st_MSGBuffer.nAVType = nAVType;
	st_MSGBuffer.nFrameType = nFrameType;
	st_MSGBuffer.nMsgLen = nMsgLen;
	st_MSGBuffer.ptszMsgBuffer = (XCHAR*)malloc(nMsgLen);
	if (NULL == st_MSGBuffer.ptszMsgBuffer)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_MALLOC;
		st_Locker.unlock_shared();
		return false;
	}
	memcpy(st_MSGBuffer.ptszMsgBuffer, lpszMsgBuffer, nMsgLen);

	stl_MapIterator->second->st_MSGLocker->lock();
	//视频?
	if (0 == nAVType)
	{
		//遇到关键帧
		if (1 == nFrameType)
		{
			//释放所有包
			for (auto stl_ListIterator = stl_MapIterator->second->pStl_ListPacket->begin(); stl_ListIterator != stl_MapIterator->second->pStl_ListPacket->end(); stl_ListIterator++)
			{
				free(stl_ListIterator->ptszMsgBuffer);
			}
			stl_MapIterator->second->pStl_ListPacket->clear();
		}
	}
	stl_MapIterator->second->pStl_ListPacket->push_back(st_MSGBuffer);
	stl_MapIterator->second->st_MSGLocker->unlock();

	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_PushStream_Recv
函数功能：获取缓冲区队列数据
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端地址
 参数.二：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要投递的数据缓冲区
 参数.三：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入缓冲区大小
 参数.四：nAVType
  In/Out：In
  类型：整数型
  可空：N
  意思：输入缓冲区类型.0视频1音频
 参数.五：nFrameType
  In/Out：In
  类型：整数型
  可空：N
  意思：输入帧类型,视频的关键帧还是PB
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PushStream::ModuleSession_PushStream_Recv(LPCXSTR lpszClientAddr, XCHAR** pptszMsgBuffer, int* pInt_MsgLen, int* pInt_AVType, int* pInt_FrameType)
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
	//输出内容
	stl_MapIterator->second->st_MSGLocker->lock();
	AVPACKET_MSGBUFFER st_MSGBuffer = stl_MapIterator->second->pStl_ListPacket->front();
	stl_MapIterator->second->pStl_ListPacket->pop_front();

	*pInt_AVType = st_MSGBuffer.nAVType;
	*pInt_FrameType = st_MSGBuffer.nFrameType;
	*pInt_MsgLen = st_MSGBuffer.nMsgLen;
	*pptszMsgBuffer = st_MSGBuffer.ptszMsgBuffer;
	stl_MapIterator->second->st_MSGLocker->unlock();

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
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PushStream::ModuleSession_PushStream_ClientInsert(LPCXSTR lpszClientAddr, LPCXSTR lpszPullAddr)
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
	stl_MapIterator->second->st_ClientLocker->lock();
	stl_MapIterator->second->pStl_ListClient->push_back(lpszPullAddr);
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
		if (0 == _tcsxnicmp(lpszPullAddr, stl_ListIterator->c_str(), _tcsxlen(lpszPullAddr)))
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
bool CModuleSession_PushStream::ModuleSession_PushStream_ClientList(LPCXSTR lpszClientAddr, list<xstring> *pStl_ListClient)
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