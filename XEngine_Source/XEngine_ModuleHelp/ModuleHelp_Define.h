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
extern "C" DWORD ModuleHelp_GetLastError(int *pInt_SysError = NULL);
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
extern "C" BOOL ModuleHelp_JT1078_BCDToString(BYTE bySIMNumber[6], TCHAR* ptszMsgBuffer);
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
extern "C" BOOL ModuleHelp_JT1078_StringToBCD(LPCTSTR lpszMsgBuffer, BYTE* pbySIMNumber);