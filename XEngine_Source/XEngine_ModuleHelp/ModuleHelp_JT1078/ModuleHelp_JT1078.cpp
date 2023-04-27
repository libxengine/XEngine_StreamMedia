#include "pch.h"
#include "ModuleHelp_JT1078.h"
/********************************************************************
//    Created:     2022/04/24  16:22:19
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_JT1078\ModuleHelp_JT1078.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleHelp\ModuleHelp_JT1078
//    File Base:   ModuleHelp_JT1078
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     JT1078相关帮助代码
//    History:
*********************************************************************/
CModuleHelp_JT1078::CModuleHelp_JT1078()
{
}
CModuleHelp_JT1078::~CModuleHelp_JT1078()
{

}
//////////////////////////////////////////////////////////////////////////
//                             公有函数
//////////////////////////////////////////////////////////////////////////
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
bool CModuleHelp_JT1078::ModuleHelp_JT1078_BCDToString(BYTE bySIMNumber[6], TCHAR* ptszMsgBuffer)
{
	ModuleHelp_IsErrorOccur = false;

	if (NULL == ptszMsgBuffer)
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_JT1078_PARAMENT;
		return false;
	}
	for (int i = 0, nPos = 0; i < 6; i++)
	{
		OPenSsl_Codec_BCDTo2Bytes(bySIMNumber[i], &ptszMsgBuffer[nPos]);
		nPos += 2;
	}
	return true;
}
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
bool CModuleHelp_JT1078::ModuleHelp_JT1078_StringToBCD(LPCXSTR lpszMsgBuffer, BYTE* pbySIMNumber)
{
	ModuleHelp_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer) || (NULL == pbySIMNumber))
	{
		ModuleHelp_IsErrorOccur = true;
		ModuleHelp_dwErrorCode = ERROR_MODULE_HELP_JT1078_PARAMENT;
		return false;
	}
	for (int i = 0, nPos = 0; i < 6; i++)
	{
		OPenSsl_Codec_2BytesToBCD(&lpszMsgBuffer[i], pbySIMNumber[nPos]);
		nPos += 2;
	}
	return true;
}