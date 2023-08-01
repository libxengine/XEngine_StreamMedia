#pragma once
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
	XCHAR tszSMSAddr[MAX_PATH];
	XCHAR tszPushAddr[MAX_PATH];

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
 参数.四：enStreamType
  In/Out：In
  类型：枚举型
  可空：N
  意思：设置的缓冲区类型
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleSession_PushStream_SetHDRBuffer(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enStreamType);
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
 参数.四：enStreamType
  In/Out：In
  类型：枚举型
  可空：N
  意思：获取的缓冲区类型
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleSession_PushStream_GetHDRBuffer(LPCXSTR lpszClientAddr, XCHAR* ptszMsgBuffer, int* pInt_MsgLen, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enStreamType);
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