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
	int nChannel;
	bool bLive;
}MODULESESSION_SDKCLIENT;
typedef struct
{
	XCHAR tszAVUrl[MAX_PATH];
	XCHAR tszToken[MAX_PATH];
}MODULESESSION_FORWARDINFO;
//////////////////////////////////////////////////////////////////////////
//                       导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG ModuleSession_GetLastError(int *pInt_SysError = NULL);
/*********************************************************************************
*                          导出会话模块                                          *
*********************************************************************************/
/********************************************************************
函数名称：ModuleSession_Client_Create
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
extern "C" bool ModuleSession_Client_Create(XNETHANDLE xhClient);
/********************************************************************
函数名称：ModuleSession_Client_Get
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
extern "C" bool ModuleSession_Client_Get(XNETHANDLE * pxhClient);
/********************************************************************
函数名称：ModuleSession_Client_Exist
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
extern "C" bool ModuleSession_Client_Exist(XNETHANDLE * pxhClient, LPCXSTR lpszDeviceAddr, LPCXSTR lpszDeviceNumber, int nChannel, bool bLive);
/********************************************************************
函数名称：ModuleSession_Client_Insert
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
extern "C" bool ModuleSession_Client_Insert(XNETHANDLE xhClient, LPCXSTR lpszDeviceAddr, LPCXSTR lpszDeviceNumber, int nChannel, bool bLive);
/********************************************************************
函数名称：ModuleSession_Client_DeleteAddr
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
extern "C" bool ModuleSession_Client_DeleteAddr(LPCXSTR lpszDeviceAddr, XNETHANDLE * pxhClient = NULL, XCHAR* ptszDeviceNumber = NULL, int* pInt_Channel = NULL, bool* pbLive = NULL);
/********************************************************************
函数名称：ModuleSession_Client_DeleteNumber
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
extern "C" bool ModuleSession_Client_DeleteNumber(LPCXSTR lpszDeviceNumber, int nChannel, bool bLive);
/********************************************************************
函数名称：ModuleSession_Client_Destory
函数功能：销毁客户端会话管理器
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleSession_Client_Destory();
/********************************************************************
函数名称：ModuleSession_Server_Create
函数功能：创建一个服务器会话管理器
 参数.一：lpszDeviceNumber
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入设备编号
 参数.二：nChannel
  In/Out：In
  类型：nChannel
  可空：N
  意思：输入设备通道
 参数.三：bLive
  In/Out：In
  类型：逻辑型
  可空：N
  意思：输入直播还是录像
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleSession_Server_Create(LPCXSTR lpszDeviceNumber, int nChannel, bool bLive);
/********************************************************************
函数名称：ModuleSession_Server_Destroy
函数功能：销毁一个管理器
 参数.一：lpszDeviceNumber
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入设备编号
 参数.二：nChannel
  In/Out：In
  类型：nChannel
  可空：N
  意思：输入设备通道
 参数.三：bLive
  In/Out：In
  类型：逻辑型
  可空：N
  意思：输入直播还是录像
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleSession_Server_Destroy(LPCXSTR lpszDeviceNumber, int nChannel, bool bLive);
/********************************************************************
函数名称：ModuleSession_Server_SetPush
函数功能：设置推送句柄
 参数.一：lpszDeviceNumber
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入设备编号
 参数.二：nChannel
  In/Out：In
  类型：nChannel
  可空：N
  意思：输入设备通道
 参数.三：bLive
  In/Out：In
  类型：逻辑型
  可空：N
  意思：输入直播还是录像
 参数.四：xhToken
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要设置的推送句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleSession_Server_SetPush(LPCXSTR lpszDeviceNumber, int nChannel, bool bLive, XHANDLE xhToken);
/********************************************************************
函数名称：ModuleSession_Server_GetPush
函数功能：获取推送句柄
 参数.一：lpszDeviceNumber
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入设备编号
 参数.二：nChannel
  In/Out：In
  类型：nChannel
  可空：N
  意思：输入设备通道
 参数.三：bLive
  In/Out：In
  类型：逻辑型
  可空：N
  意思：输入直播还是录像
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" XHANDLE ModuleSession_Server_GetPush(LPCXSTR lpszDeviceNumber, int nChannel, bool bLive);
/*********************************************************************************
*                          流转发导出会话模块                                    *
*********************************************************************************/
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
extern "C" bool ModuleSession_Forward_Create(LPCXSTR lpszPlay, XHANDLE xhToken, LPCXSTR lpszSMSPlay);
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
extern "C" XHANDLE ModuleSession_Forward_Get(LPCXSTR lpszPlay);
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
extern "C" bool ModuleSession_Forward_Delete(LPCXSTR lpszPlay);
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
extern "C" bool ModuleSession_Forward_List(MODULESESSION_FORWARDINFO * **pppSt_Forward, int* pInt_ListCount);

extern "C" bool ModuleSession_PushStream_Create(LPCXSTR lpszSMSAddr, XNETHANDLE xhFLVStream);
extern "C" bool ModuleSession_PushStream_Destroy(LPCXSTR lpszSMSAddr);
extern "C" bool ModuleSession_PushStream_GetStreamForAddr(LPCXSTR lpszSMSAddr, XNETHANDLE* pxhFLVStream);
extern "C" bool ModuleSession_PushStream_Send(LPCXSTR lpszSMSAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nAVType, int nFrameType);
extern "C" bool ModuleSession_PushStream_Recv(LPCXSTR lpszSMSAddr, XCHAR** pptszMsgBuffer, int* pInt_MsgLen, int* pInt_AVType, int* pInt_FrameType);