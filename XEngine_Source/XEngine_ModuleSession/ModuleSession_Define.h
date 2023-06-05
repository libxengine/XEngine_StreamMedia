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
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleSession_PullStream_Insert(LPCXSTR lpszClientAddr, LPCXSTR lpszSMSAddr);
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
extern "C" bool ModuleSession_PullStream_Get(LPCXSTR lpszClientAddr, XCHAR* ptszSMSAddr);
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
extern "C" bool ModuleSession_PullStream_Delete(LPCXSTR lpszClientAddr);
/*********************************************************************************
*                         推流端导出会话模块                                     *
*********************************************************************************/
/********************************************************************
函数名称：ModuleSession_PushStream_Create
函数功能：创建一个推流会话管理器
 参数.一：lpszSMSAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入推流地址
 参数.二：xhFLVStream
  In/Out：In
  类型：整数型
  可空：N
  意思：输入绑定的FLV打包句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleSession_PushStream_Create(LPCXSTR lpszSMSAddr, XNETHANDLE xhFLVStream);
/********************************************************************
函数名称：ModuleSession_PushStream_Destroy
函数功能：销毁一个管理器
 参数.一：lpszSMSAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入推流地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleSession_PushStream_Destroy(LPCXSTR lpszSMSAddr);
/********************************************************************
函数名称：ModuleSession_PushStream_GetStreamForAddr
函数功能：通过地址获取流句柄
 参数.一：lpszSMSAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入地址
 参数.二：pxhFLVStream
  In/Out：Out
  类型：句柄
  可空：N
  意思：输出获取到的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleSession_PushStream_GetStreamForAddr(LPCXSTR lpszSMSAddr, XNETHANDLE* pxhFLVStream);
/********************************************************************
函数名称：ModuleSession_PushStream_Send
函数功能：投递一段数据给会话管理器
 参数.一：lpszSMSAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的流
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
extern "C" bool ModuleSession_PushStream_Send(LPCXSTR lpszSMSAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nAVType, int nFrameType);
/********************************************************************
函数名称：ModuleSession_PushStream_Recv
函数功能：获取缓冲区队列数据
 参数.一：lpszSMSAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的流
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
extern "C" bool ModuleSession_PushStream_Recv(LPCXSTR lpszSMSAddr, XCHAR** pptszMsgBuffer, int* pInt_MsgLen, int* pInt_AVType, int* pInt_FrameType);
/********************************************************************
函数名称：ModuleSession_PushStream_ClientInsert
函数功能：客户端插入
 参数.一：lpszSMSAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的流
 参数.二：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入插入的客户端
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleSession_PushStream_ClientInsert(LPCXSTR lpszSMSAddr, LPCXSTR lpszClientAddr);
/********************************************************************
函数名称：ModuleSession_PushStream_ClientDelete
函数功能：客户端删除
 参数.一：lpszSMSAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的流
 参数.二：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入删除的客户端
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleSession_PushStream_ClientDelete(LPCXSTR lpszSMSAddr, LPCXSTR lpszClientAddr);
/********************************************************************
函数名称：ModuleSession_PushStream_ClientList
函数功能：客户端获取
 参数.一：lpszSMSAddr
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
extern "C" bool ModuleSession_PushStream_ClientList(LPCXSTR lpszSMSAddr, list<xstring>* pStl_ListClient);