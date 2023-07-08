#pragma once
/********************************************************************
//    Created:     2022/03/01  09:27:15
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_Define.h
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleProtocol
//    File Base:   ModuleProtocol_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出协议模块定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
///                        导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG ModuleProtocol_GetLastError(int *pInt_SysError = NULL);
/************************************************************************/
/*                         导出的协议封装函数                           */
/************************************************************************/
/********************************************************************
函数名称：ModuleProtocol_Packet_Comm
函数功能：通用打包回复函数
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出协议缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出缓冲区大小
 参数.三：pSt_ProtocolHdr
  In/Out：In
  类型：数据结构指针
  可空：Y
  意思：输入要打包的协议,如果需要的话
 参数.四：nCode
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入回复的CODE
 参数.五：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要处理的缓冲区
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleProtocol_Packet_Comm(XCHAR * ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR * pSt_ProtocolHdr = NULL, int nCode = 0, LPCXSTR lpszMsgBuffer = NULL);
/********************************************************************
函数名称：ModuleProtocol_Packet_Create
函数功能：流媒体创建
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出协议缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出缓冲区大小
 参数.三：pSt_ProtocolStream
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入音视频流信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleProtocol_Packet_Create(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLSTREAM * pSt_ProtocolStream);
/********************************************************************
函数名称：ModuleProtocol_Packet_Push
函数功能：推流协议
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出协议缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出缓冲区大小
 参数.三：pSt_ProtocolStream
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入音视频流信息
 参数.四：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：推流数据大小
 参数.五：nMsgType
  In/Out：In
  类型：整数型
  可空：N
  意思：消息类型,0视频,1音频
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleProtocol_Packet_Push(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLSTREAM * pSt_ProtocolStream, int nMsgLen, int nMsgType);
/********************************************************************
函数名称：ModuleProtocol_Packet_Destroy
函数功能：销毁流协议
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出协议缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出缓冲区大小
 参数.三：pSt_ProtocolStream
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入音视频流信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleProtocol_Packet_Destroy(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLSTREAM * pSt_ProtocolStream);
/************************************************************************/
/*                         导出的协议解析函数                           */
/************************************************************************/
/********************************************************************
函数名称：ModuleProtocol_Parse_HTTPForward
函数功能：转发解析
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的内容
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入解析大小
 参数.三：ptszAVUrl
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出解析到的URL
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
extern "C" bool ModuleProtocol_Parse_HTTPForward(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR * ptszAVUrl = NULL);