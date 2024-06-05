﻿#pragma once
/********************************************************************
//    Created:     2022/04/25  10:17:21
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession\ModuleSession_Define.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleSession
//    File Base:   ModuleSession_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出会话模块定义
//    History:
*********************************************************************/
typedef struct  
{
	XCHAR tszClientID[128];
	ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enClientType;
}STREAMMEDIA_SESSIONCLIENT;
typedef struct
{
	XENGINE_PROTOCOL_AVINFO st_AVInfo;
	XCHAR tszSMSAddr[MAX_PATH];
	ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enStreamType;
	int nClientCount;
}STREAMMEDIA_PUBLISHINFO;
typedef struct
{
	struct  
	{
		XCHAR tszTokenStr[MAX_PATH];
		XCHAR tszICEUser[MAX_PATH];
		XCHAR tszICEPass[MAX_PATH];
		XCHAR tszHMacStr[MAX_PATH];
		XCHAR tszClientAddr[128];
		bool bConnect;

		XCHAR tszVSSrcStr[128];
		XCHAR tszVideoCName[MAX_PATH];
		XCHAR tszVideoLabel[MAX_PATH];
		
		XCHAR tszASSrcStr[128];
		XCHAR tszAudioCName[MAX_PATH];
		XCHAR tszAudioLabel[MAX_PATH];
	}st_WEBRtc;
	XCHAR tszSMSAddr[MAX_PATH];
	XCHAR tszPushAddr[MAX_PATH];
	int nFLVTag;

	ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enStreamType;
}STREAMMEDIA_PULLLISTINFO;
//////////////////////////////////////////////////////////////////////////
//                       导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG ModuleSession_GetLastError(int *pInt_SysError = NULL);
/*********************************************************************************
*                          拉流端导出会话模块                                    *
*********************************************************************************/
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
extern "C" bool ModuleSession_PullStream_Insert(LPCXSTR lpszClientAddr, LPCXSTR lpszSMSAddr, LPCXSTR lpszPushAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enStreamType);
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
extern "C" bool ModuleSession_PullStream_GetSMSAddr(LPCXSTR lpszClientAddr, XCHAR* ptszSMSAddr);
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
extern "C" bool ModuleSession_PullStream_GetPushAddr(LPCXSTR lpszClientAddr, XCHAR* ptszPushAddr);
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
extern "C" bool ModuleSession_PullStream_GetStreamType(LPCXSTR lpszClientAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE* penStreamType);
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
extern "C" bool ModuleSession_PullStream_Delete(LPCXSTR lpszClientAddr);
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
extern "C" bool ModuleSession_PullStream_PublishDelete(LPCXSTR lpszClientAddr);
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
extern "C" bool ModuleSession_PullStream_GetList(STREAMMEDIA_PULLLISTINFO*** pppSt_PullList, int* pInt_ListCount);
//////////////////////////////////////////////////////////////////////////
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
extern "C" bool ModuleSession_PullStream_FLVTagSet(LPCXSTR lpszClientAddr, int nTagSize);
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
extern "C" bool ModuleSession_PullStream_FLVTagGet(LPCXSTR lpszClientAddr, int* pInt_TagSize);
//////////////////////////////////////////////////////////////////////////
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
extern "C" bool ModuleSession_PullStream_RTCSet(LPCXSTR lpszClientAddr, LPCXSTR lpszTokenStr, LPCXSTR lpszICEUser, LPCXSTR lpszICEPass, LPCXSTR lpszHMacStr);
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
extern "C" bool ModuleSession_PullStream_RTCGet(LPCXSTR lpszClientAddr, XCHAR * ptszTokenStr = NULL, XCHAR * ptszICEUser = NULL, XCHAR * ptszICEPass = NULL, XCHAR * ptszHMacStr = NULL);
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
extern "C" bool ModuleSession_PullStream_RTCSSrcSet(LPCXSTR lpszClientAddr, LPCXSTR lpszSSRCStr, LPCXSTR lpszCNameStr, LPCXSTR lpszLabelStr, bool bVideo = true);
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
extern "C" bool ModuleSession_PullStream_RTCSSrcGet(LPCXSTR lpszClientAddr, XCHAR* ptszSSRCStr, bool bVideo = true);
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
extern "C" bool ModuleSession_PullStream_RTCAddrSet(LPCXSTR lpszClientUser, LPCXSTR lpszClientAddr);
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
extern "C" bool ModuleSession_PullStream_RTCAddrGet(LPCXSTR lpszClientAddr, XCHAR* ptszClientUser);
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
extern "C" bool ModuleSession_PullStream_RTCConnSet(LPCXSTR lpszClientAddr, bool bConnect);
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
extern "C" bool ModuleSession_PullStream_RTCConnGet(LPCXSTR lpszClientAddr, bool* pbConnect);
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
extern "C" bool ModuleSession_PullStream_RTCSmsGet(LPCXSTR lpszClientAddr, XCHAR* ptszSMSAddr);
/*********************************************************************************
*                         推流端导出会话模块                                     *
*********************************************************************************/
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
extern "C" bool ModuleSession_PushStream_Create(LPCXSTR lpszClientAddr, LPCXSTR lpszSMSAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enStreamType);
/********************************************************************
函数名称：ModuleSession_PushStream_Destroy
函数功能：销毁一个管理器
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入推流地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleSession_PushStream_Destroy(LPCXSTR lpszClientAddr);
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
extern "C" bool ModuleSession_PushStream_GetAddrForAddr(LPCXSTR lpszClientAddr, XCHAR * ptszSMSAddr);
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
extern "C" bool ModuleSession_PushStream_FindStream(LPCXSTR lpszSMSAddr, XCHAR* ptszClientAddr);
/********************************************************************
函数名称：ModuleSession_PushStream_ClientInsert
函数功能：客户端插入
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的流
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
extern "C" bool ModuleSession_PushStream_ClientInsert(LPCXSTR lpszClientAddr, LPCXSTR lpszPullAddr, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enStreamType);
/********************************************************************
函数名称：ModuleSession_PushStream_ClientDelete
函数功能：客户端删除
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的流
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
extern "C" bool ModuleSession_PushStream_ClientDelete(LPCXSTR lpszClientAddr, LPCXSTR lpszPullAddr);
/********************************************************************
函数名称：ModuleSession_PushStream_ClientList
函数功能：客户端获取
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的流
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
extern "C" bool ModuleSession_PushStream_ClientList(LPCXSTR lpszClientAddr, list<STREAMMEDIA_SESSIONCLIENT>* pStl_ListClient);
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
extern "C" bool ModuleSession_PushStream_SetAVInfo(LPCXSTR lpszClientAddr, XENGINE_PROTOCOL_AVINFO* pSt_AVInfo);
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
extern "C" bool ModuleSession_PushStream_GetAVInfo(LPCXSTR lpszClientAddr, XENGINE_PROTOCOL_AVINFO* pSt_AVInfo);
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
extern "C" bool ModuleSession_PushStream_GetInfo(STREAMMEDIA_PUBLISHINFO*** pppSt_ProtocolStream, int* pInt_ListCount);
/********************************************************************
函数名称：ModuleSession_PushStream_HLSInsert
函数功能：插入创建一个HLS文件
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端地址
 参数.二：lpszTSFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入路径
 参数.三：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入HLS文件句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleSession_PushStream_HLSInsert(LPCXSTR lpszClientAddr, LPCXSTR lpszTSFile, XNETHANDLE xhToken);
/********************************************************************
函数名称：ModuleSession_PushStream_HLSInsert
函数功能：插入创建一个HLS文件
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端地址
 参数.二：ptszFileName
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出文件保存路径
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleSession_PushStream_HLSGetFile(LPCXSTR lpszClientAddr, XCHAR* ptszFileName);
/********************************************************************
函数名称：ModuleSession_PushStream_HLSWrite
函数功能：HLS写入数据
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端地址
 参数.二：lpszMSGBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要写入的缓冲区数据
 参数.三：nMSGLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入缓冲区大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleSession_PushStream_HLSWrite(LPCXSTR lpszClientAddr, LPCXSTR lpszMSGBuffer, int nMSGLen);
/********************************************************************
函数名称：ModuleSession_PushStream_HLSClose
函数功能：关闭一个HLS文件
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端地址
 参数.二：pxhToken
  In/Out：Out
  类型：句柄
  可空：N
  意思：输出HLS文件句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleSession_PushStream_HLSClose(LPCXSTR lpszClientAddr, XNETHANDLE* pxhToken);
/********************************************************************
函数名称：ModuleSession_PushStream_HLSTimeSet
函数功能：设置一个时间戳
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端地址
 参数.二：nTime
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要设置的时间戳
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleSession_PushStream_HLSTimeSet(LPCXSTR lpszClientAddr, __int64u nTime);
/********************************************************************
函数名称：ModuleSession_PushStream_HLSTimeGet
函数功能：获取一个时间戳
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入客户端地址
 参数.二：pInt_Time
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出获取到的时间戳
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleSession_PushStream_HLSTimeGet(LPCXSTR lpszClientAddr, __int64u* pInt_Time);