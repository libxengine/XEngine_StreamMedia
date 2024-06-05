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
 参数.三：lpszPushAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要绑定的推流地址
 参数.四：lpszPushAddr
  In/Out：In
  类型：枚举型
  可空：N
  意思：输入客户端的拉流类型
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PullStream::ModuleSession_PullStream_Insert(LPCXSTR lpszClientAddr, LPCXSTR lpszSMSAddr, LPCXSTR lpszPushAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enStreamType)
{
    Session_IsErrorOccur = false;

	STREAMMEDIA_PULLLISTINFO* pSt_PullStream = new STREAMMEDIA_PULLLISTINFO;
	if (NULL == pSt_PullStream)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_MALLOC;
		return false;
	}
    memset(pSt_PullStream, '\0', sizeof(STREAMMEDIA_PULLLISTINFO));

	pSt_PullStream->enStreamType = enStreamType;
    _tcsxcpy(pSt_PullStream->tszSMSAddr, lpszSMSAddr);
	_tcsxcpy(pSt_PullStream->tszPushAddr, lpszPushAddr);

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
函数名称：ModuleSession_PullStream_Delete
函数功能：删除整个推流端关联的拉流地址
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
bool CModuleSession_PullStream::ModuleSession_PullStream_PublishDelete(LPCXSTR lpszClientAddr)
{
	Session_IsErrorOccur = false;

	if (NULL == lpszClientAddr)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_PARAMENT;
		return false;
	}
	st_Locker.lock();
	for (auto stl_MapIterator = stl_MapClient.begin(); stl_MapIterator != stl_MapClient.end(); )
	{
		if (0 == _tcsxnicmp(lpszClientAddr, stl_MapIterator->second->tszSMSAddr, _tcsxlen(lpszClientAddr)))
		{
			stl_MapIterator = stl_MapClient.erase(stl_MapIterator);
		}
		else
		{
			stl_MapIterator++;
		}
	}
	st_Locker.unlock();
	return true;
}
/********************************************************************
函数名称：ModuleSession_PullStream_GetSMSAddr
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
bool CModuleSession_PullStream::ModuleSession_PullStream_GetSMSAddr(LPCXSTR lpszClientAddr, XCHAR* ptszSMSAddr)
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
/********************************************************************
函数名称：ModuleSession_PullStream_GetPushAddr
函数功能：获取客户端绑定的推流地址
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的客户端
 参数.二：ptszSMSAddr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出推流地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PullStream::ModuleSession_PullStream_GetPushAddr(LPCXSTR lpszClientAddr, XCHAR* ptszPushAddr)
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
	_tcsxcpy(ptszPushAddr, stl_MapIterator->second->tszPushAddr);
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_PullStream_GetStreamType
函数功能：获取客户端流属性
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的客户端
 参数.二：penStreamType
  In/Out：Out
  类型：枚举型
  可空：N
  意思：输出流类型
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PullStream::ModuleSession_PullStream_GetStreamType(LPCXSTR lpszClientAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE* penStreamType)
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
	*penStreamType = stl_MapIterator->second->enStreamType;
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_PullStream_GetList
函数功能：获取用户列表
 参数.一：pppSt_PullList
  In/Out：Out
  类型：三级指针
  可空：N
  意思：输出用户列表数据
 参数.二：pInt_ListCount
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出列表个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PullStream::ModuleSession_PullStream_GetList(STREAMMEDIA_PULLLISTINFO*** pppSt_PullList, int* pInt_ListCount)
{
	Session_IsErrorOccur = false;

	st_Locker.lock_shared();

	*pInt_ListCount = stl_MapClient.size();
	BaseLib_OperatorMemory_Malloc((XPPPMEM)pppSt_PullList, stl_MapClient.size(), sizeof(STREAMMEDIA_PULLLISTINFO));

	auto stl_MapIterator = stl_MapClient.begin();
	for (int i = 0; stl_MapIterator != stl_MapClient.end(); stl_MapIterator++, i++)
	{
		(*pppSt_PullList)[i] = stl_MapIterator->second;
	}
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_PullStream_FLVTagSet
函数功能：设置会话的FLV标签大小
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
 参数.二：nTagSize
  In/Out：In
  类型：整数型
  可空：N
  意思：输入标签大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PullStream::ModuleSession_PullStream_FLVTagSet(LPCXSTR lpszClientAddr, int nTagSize)
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
	stl_MapIterator->second->nFLVTag = nTagSize;
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_PullStream_FLVTagGet
函数功能：获取会话的FLV标签大小
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的客户端
 参数.二：pInt_TagSize
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出标签大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PullStream::ModuleSession_PullStream_FLVTagGet(LPCXSTR lpszClientAddr, int* pInt_TagSize)
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
	*pInt_TagSize = stl_MapIterator->second->nFLVTag;
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_PullStream_RTCSet
函数功能：设置RTC流的信息
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的客户端
 参数.二：lpszTokenStr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入令牌字符串
 参数.三：lpszICEUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入ICE用户
 参数.四：lpszICEPass
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入ICE密码
 参数.五：lpszHMacStr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入HMAC的SHA值
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PullStream::ModuleSession_PullStream_RTCSet(LPCXSTR lpszClientAddr, LPCXSTR lpszTokenStr, LPCXSTR lpszICEUser, LPCXSTR lpszICEPass, LPCXSTR lpszHMacStr)
{
	Session_IsErrorOccur = false;

	st_Locker.lock_shared();
	auto stl_MapIterator = stl_MapClient.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapClient.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	_tcsxcpy(stl_MapIterator->second->st_WEBRtc.tszTokenStr, lpszTokenStr);
	_tcsxcpy(stl_MapIterator->second->st_WEBRtc.tszICEUser, lpszICEUser);
	_tcsxcpy(stl_MapIterator->second->st_WEBRtc.tszICEPass, lpszICEPass);
	_tcsxcpy(stl_MapIterator->second->st_WEBRtc.tszHMacStr, lpszHMacStr);
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_PullStream_RTCGet
函数功能：获取RTC流的信息
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的客户端
 参数.二：ptszTokenStr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出TOKEN
 参数.三：ptszICEUser
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出ICE用户
 参数.四：ptszICEPass
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出ICE密码
 参数.五：ptszHMacStr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出HMAC的SHA值
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PullStream::ModuleSession_PullStream_RTCGet(LPCXSTR lpszClientAddr, XCHAR* ptszTokenStr /* = NULL */, XCHAR* ptszICEUser /* = NULL */, XCHAR* ptszICEPass /* = NULL */, XCHAR* ptszHMacStr /* = NULL */)
{
	Session_IsErrorOccur = false;

	st_Locker.lock_shared();
	auto stl_MapIterator = stl_MapClient.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapClient.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}

	if (NULL != ptszTokenStr)
	{
		_tcsxcpy(ptszTokenStr, stl_MapIterator->second->st_WEBRtc.tszTokenStr);
	}
	if (NULL != ptszICEUser)
	{
		_tcsxcpy(ptszICEUser, stl_MapIterator->second->st_WEBRtc.tszICEUser);
	}
	if (NULL != ptszICEPass)
	{
		_tcsxcpy(ptszICEPass, stl_MapIterator->second->st_WEBRtc.tszICEPass);
	}
	if (NULL != ptszHMacStr)
	{
		_tcsxcpy(ptszHMacStr, stl_MapIterator->second->st_WEBRtc.tszHMacStr);
	}
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_PullStream_RTCSSrcSet
函数功能：设置SSRC
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的客户端
 参数.二：lpszSSRCStr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要设置的SSRC
 参数.三：lpszCNameStr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入SSRC别名
 参数.四：lpszLabelStr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入SSRC描述
 参数.五：bVideo
  In/Out：In
  类型：逻辑型
  可空：N
  意思：输入是否为视频
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PullStream::ModuleSession_PullStream_RTCSSrcSet(LPCXSTR lpszClientAddr, LPCXSTR lpszSSRCStr, LPCXSTR lpszCNameStr, LPCXSTR lpszLabelStr, bool bVideo)
{
	Session_IsErrorOccur = false;

	st_Locker.lock_shared();
	auto stl_MapIterator = stl_MapClient.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapClient.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}

	if (bVideo)
	{
		_tcsxcpy(stl_MapIterator->second->st_WEBRtc.tszVSSrcStr, lpszSSRCStr);
		_tcsxcpy(stl_MapIterator->second->st_WEBRtc.tszVideoCName, lpszCNameStr);
		_tcsxcpy(stl_MapIterator->second->st_WEBRtc.tszVideoLabel, lpszLabelStr);
	}
	else
	{
		_tcsxcpy(stl_MapIterator->second->st_WEBRtc.tszASSrcStr, lpszSSRCStr);
		_tcsxcpy(stl_MapIterator->second->st_WEBRtc.tszAudioCName, lpszCNameStr);
		_tcsxcpy(stl_MapIterator->second->st_WEBRtc.tszAudioLabel, lpszLabelStr);
	}
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_PullStream_RTCSSrcGet
函数功能：通过地址后去SSRC信息
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的客户端
 参数.二：ptszSSRCStr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出获取到的SSRC
 参数.三：bVideo
  In/Out：In
  类型：逻辑型
  可空：Y
  意思：视频还是音频的SSRC
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PullStream::ModuleSession_PullStream_RTCSSrcGet(LPCXSTR lpszClientAddr, XCHAR* ptszSSRCStr, bool bVideo)
{
	Session_IsErrorOccur = false;

	st_Locker.lock_shared();
	auto stl_MapIterator = stl_MapClient.find(lpszClientAddr);
	if (stl_MapIterator == stl_MapClient.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}

	if (bVideo)
	{
		_tcsxcpy(ptszSSRCStr, stl_MapIterator->second->st_WEBRtc.tszVSSrcStr);
	}
	else
	{
		_tcsxcpy(ptszSSRCStr, stl_MapIterator->second->st_WEBRtc.tszVSSrcStr);
	}
	
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_PullStream_RTCAddrSet
函数功能：设置RTC的数据通信地址
 参数.一：lpszClientUser
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的用户
 参数.二：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入绑定的地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PullStream::ModuleSession_PullStream_RTCAddrSet(LPCXSTR lpszClientUser, LPCXSTR lpszClientAddr)
{
	Session_IsErrorOccur = false;

	st_Locker.lock_shared();
	auto stl_MapIterator = stl_MapClient.find(lpszClientUser);
	if (stl_MapIterator == stl_MapClient.end())
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
		st_Locker.unlock_shared();
		return false;
	}
	_tcsxcpy(stl_MapIterator->second->st_WEBRtc.tszClientAddr, lpszClientAddr);
	st_Locker.unlock_shared();
	return true;
}
/********************************************************************
函数名称：ModuleSession_PullStream_RTCAddrGet
函数功能：获取地址绑定的用户
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的地址
 参数.二：ptszClientUser
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出绑定的用户
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PullStream::ModuleSession_PullStream_RTCAddrGet(LPCXSTR lpszClientAddr, XCHAR* ptszClientUser)
{
	Session_IsErrorOccur = false;

	bool bFound = false;

	st_Locker.lock_shared();
	for (auto stl_MapIterator = stl_MapClient.begin(); stl_MapIterator != stl_MapClient.end(); stl_MapIterator++)
	{
		if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_RTC == stl_MapIterator->second->enStreamType)
		{
			if (0 == _tcsxnicmp(lpszClientAddr, stl_MapIterator->second->st_WEBRtc.tszClientAddr, _tcsxlen(lpszClientAddr)))
			{
				bFound = true;
				break;
			}
		}
	}
	st_Locker.unlock_shared();

	if (!bFound)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
		return false;
	}
	return true;
}
/********************************************************************
函数名称：ModuleSession_PullStream_RTCConnSet
函数功能：设置RTC用户是否链接
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的地址
 参数.二：bConnect
  In/Out：In
  类型：逻辑型
  可空：N
  意思：是否链接
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PullStream::ModuleSession_PullStream_RTCConnSet(LPCXSTR lpszClientAddr, bool bConnect)
{
	Session_IsErrorOccur = false;

	bool bFound = false;
	st_Locker.lock_shared();
	for (auto stl_MapIterator = stl_MapClient.begin(); stl_MapIterator != stl_MapClient.end(); stl_MapIterator++)
	{
		if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_RTC == stl_MapIterator->second->enStreamType)
		{
			if (0 == _tcsxnicmp(lpszClientAddr, stl_MapIterator->second->st_WEBRtc.tszClientAddr, _tcsxlen(lpszClientAddr)))
			{
				bFound = true;
				stl_MapIterator->second->st_WEBRtc.bConnect = bConnect;
				break;
			}
		}
	}
	st_Locker.unlock_shared();

	if (!bFound)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
		return false;
	}
	return true;
}
/********************************************************************
函数名称：ModuleSession_PullStream_RTCConnGet
函数功能：获取RTC是否链接
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的地址
 参数.二：pbConnect
  In/Out：Out
  类型：逻辑型指针
  可空：N
  意思：输出是否链接
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PullStream::ModuleSession_PullStream_RTCConnGet(LPCXSTR lpszClientAddr, bool* pbConnect)
{
	Session_IsErrorOccur = false;

	bool bFound = false;
	st_Locker.lock_shared();
	for (auto stl_MapIterator = stl_MapClient.begin(); stl_MapIterator != stl_MapClient.end(); stl_MapIterator++)
	{
		if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_RTC == stl_MapIterator->second->enStreamType)
		{
			if (0 == _tcsxnicmp(lpszClientAddr, stl_MapIterator->second->st_WEBRtc.tszClientAddr, _tcsxlen(lpszClientAddr)))
			{
				bFound = true;
				*pbConnect = stl_MapIterator->second->st_WEBRtc.bConnect;
				break;
			}
		}
	}
	st_Locker.unlock_shared();

	if (!bFound)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
		return false;
	}
	return true;
}
/********************************************************************
函数名称：ModuleSession_PullStream_RTCSmsGet
函数功能：获取RTC对应的SMS名称
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的地址
 参数.二：ptszSMSAddr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出获取到的SMS地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleSession_PullStream::ModuleSession_PullStream_RTCSmsGet(LPCXSTR lpszClientAddr, XCHAR* ptszSMSAddr)
{
	Session_IsErrorOccur = false;

	bool bFound = false;
	st_Locker.lock_shared();
	for (auto stl_MapIterator = stl_MapClient.begin(); stl_MapIterator != stl_MapClient.end(); stl_MapIterator++)
	{
		if (ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_RTC == stl_MapIterator->second->enStreamType)
		{
			if (0 == _tcsxnicmp(lpszClientAddr, stl_MapIterator->second->st_WEBRtc.tszClientAddr, _tcsxlen(lpszClientAddr)))
			{
				bFound = true;
				_tcsxcpy(ptszSMSAddr, stl_MapIterator->second->tszSMSAddr);
				break;
			}
		}
	}
	st_Locker.unlock_shared();

	if (!bFound)
	{
		Session_IsErrorOccur = true;
		Session_dwErrorCode = ERROR_STREAMMEDIA_MODULE_SESSION_NOTFOUND;
		return false;
	}
	return true;
}