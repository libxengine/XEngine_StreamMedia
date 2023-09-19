#pragma once
/********************************************************************
//    Created:     2023/09/19  11:09:11
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleQueue\ModuleQueue_Define.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleQueue
//    File Base:   ModuleQueue_Define
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     导出定义
//    History:
*********************************************************************/
//////////////////////////////////////////////////////////////////////////
//                       导出的函数
//////////////////////////////////////////////////////////////////////////
extern "C" XLONG ModuleQueue_GetLastError(int *pInt_SysError = NULL);
/*********************************************************************************
*                          JHT1078导出队列函数                                   *
*********************************************************************************/
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
extern "C" bool ModuleQueue_JT1078_Create(LPCXSTR lpszClientAddr);
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
extern "C" bool ModuleQueue_JT1078_Destroy(LPCXSTR lpszClientAddr);
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
extern "C" bool ModuleQueue_JT1078_Insert(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen);
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
extern "C" bool ModuleQueue_JT1078_Get(LPCXSTR lpszClientAddr, XCHAR** pptszMsgBuffer, int* pInt_MsgLen);