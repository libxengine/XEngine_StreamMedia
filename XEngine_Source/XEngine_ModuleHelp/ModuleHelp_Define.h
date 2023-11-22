#pragma once
/********************************************************************
//    Created:     2022/04/24  16:26:53
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_Define.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp
//    File Base:   ModuleHelp_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出帮助模块定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
///                        导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG ModuleHelp_GetLastError(int *pInt_SysError = NULL);
/************************************************************************/
/*                         导出的JT1078帮助函数                         */
/************************************************************************/
/********************************************************************
函数名称：ModuleHelp_IDCard_CheckBirth
函数功能：检查身份证号生日
 参数.一：pSt_IDInfo
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要检查的身份结构
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleHelp_JT1078_BCDToString(XBYTE bySIMNumber[6], XCHAR* ptszMsgBuffer);
/********************************************************************
函数名称：ModuleHelp_JT1078_StringToBCD
函数功能：字符串转BCD
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要转换的缓冲区
 参数.二：pbySIMNumber
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出转换后的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleHelp_JT1078_StringToBCD(LPCXSTR lpszMsgBuffer, XBYTE* pbySIMNumber);
/************************************************************************/
/*                         导出的SRT函数                                */
/************************************************************************/
/********************************************************************
函数名称：ModuleHelp_SrtCore_Start
函数功能：启动SRT
 参数.一：nPort
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要绑定的端口
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleHelp_SrtCore_Start(int nPort);
/********************************************************************
函数名称：ModuleHelp_SrtCore_SetCallback
函数功能：设置数据回调函数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleHelp_SrtCore_SetCallback(CALLBACK_NETCORE_SOCKET_NETEVENT_LOGIN fpCallePoll_Login, CALLBACK_NETCORE_SOCKET_NETEVENT_RECV fpCallePoll_Recv, CALLBACK_NETCORE_SOCKET_NETEVENT_LEAVE fpCallePoll_Leave, XPVOID lPLogin = NULL, XPVOID lPRecv = NULL, XPVOID lPLeave = NULL);
/********************************************************************
函数名称：ModuleHelp_SrtCore_Send
函数功能：发送指定数据给客户端
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：要操作的客户端
 参数.二：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：发送的数据
 参数.三：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：发送的大小
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleHelp_SrtCore_Send(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen);
/********************************************************************
函数名称：ModuleHelp_SrtCore_GetStreamID
函数功能：获取流信息
 参数.一：hSocket
  In/Out：In
  类型：句柄
  可空：N
  意思：输入要操作的客户端
 参数.二：ptszSMSAddr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出获取到的推流地址
 参数.三：pbPublish
  In/Out：Out
  类型：逻辑型指针
  可空：N
  意思：输出这个地址是推流还是拉流
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleHelp_SrtCore_GetStreamID(SRTSOCKET hSocket, XCHAR * ptszSMSAddr, bool* pbPublish);
/********************************************************************
函数名称：ModuleHelp_SrtCore_Close
函数功能：关闭一个指定的客户端
 参数.一：lpszClientAddr
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入关闭的客户端低脂
 参数.二：hSocket
  In/Out：In
  类型：句柄
  可空：Y
  意思：或者输入客户端句柄
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleHelp_SrtCore_Close(LPCXSTR lpszClientAddr = NULL, SRTSOCKET hSocket = 0);
/********************************************************************
函数名称：ModuleHelp_SrtCore_Destory
函数功能：销毁SRT服务
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleHelp_SrtCore_Destory();
/************************************************************************/
/*                         导出的RTSP函数                               */
/************************************************************************/
/********************************************************************
函数名称：ModuleHelp_Rtsp_GetSMSAddr
函数功能：获得流媒体地址
 参数.一：lpszURLStr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要处理的URL
 参数.二：ptszSMSAddr
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出获得的流媒体地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleHelp_Rtsp_GetSMSAddr(LPCXSTR lpszURLStr, XCHAR* ptszSMSAddr);