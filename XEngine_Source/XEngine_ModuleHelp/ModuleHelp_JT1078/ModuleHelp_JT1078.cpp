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
bool CModuleHelp_JT1078::ModuleHelp_JT1078_BCDToString(XBYTE bySIMNumber[6], XCHAR* ptszMsgBuffer)
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
		ModuleHelp_JT1078_BCDTo2Bytes(bySIMNumber[i], &ptszMsgBuffer[nPos]);
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
bool CModuleHelp_JT1078::ModuleHelp_JT1078_StringToBCD(LPCXSTR lpszMsgBuffer, XBYTE* pbySIMNumber)
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
		ModuleHelp_JT1078_2BytesToBCD(&lpszMsgBuffer[i], pbySIMNumber[nPos]);
		nPos += 2;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
//                             保护函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：ModuleHelp_JT1078_2BytesToBCD
函数功能：两个字符转BCD编码
 参数.一：lpszSource
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入一段要编码的数据
 参数.二：chBCD
  In/Out：Out
  类型：无符号字符
  可空：N
  意思：输出编码好的数据
返回值
  类型：无
  意思：
备注：
*********************************************************************/
void CModuleHelp_JT1078::ModuleHelp_JT1078_2BytesToBCD(LPCXSTR lpszSource, XBYTE& chBCD)
{
    ModuleHelp_IsErrorOccur = false;

    XBYTE uszBsd;
    uszBsd = lpszSource[0] - '0';
    uszBsd = uszBsd << 4;
    chBCD |= uszBsd;
    uszBsd = lpszSource[1] - '0';
    uszBsd = uszBsd & 0x0f;
    chBCD |= uszBsd;
}
/********************************************************************
函数名称：ModuleHelp_JT1078_BCDTo2Bytes
函数功能：BCD编码转字符
 参数.一：chBCD
  In/Out：In
  类型：无符号字符
  可空：N
  意思：输入要解码的BCD字符
 参数.二：ptszDest
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出解码后的字符串
返回值
  类型：无
  意思：
备注：
*********************************************************************/
void CModuleHelp_JT1078::ModuleHelp_JT1078_BCDTo2Bytes(XBYTE chBCD, XCHAR* ptszDest)
{
    ModuleHelp_IsErrorOccur = false;

    XBYTE uszBsd;

    uszBsd = chBCD & 0x0f;
    ptszDest[1] = uszBsd + '0';
    uszBsd = chBCD & 0xf0;
    uszBsd = uszBsd >> 4;
    ptszDest[0] = uszBsd + '0';
}
/********************************************************************
函数名称：ModuleHelp_JT1078_BCDToInt
函数功能：BCD编码转整数型
 参数.一：chBCD
  In/Out：In
  类型：无符号字符
  可空：N
  意思：输入要解码的BCD字符
返回值
  类型：整数型
  意思：输出解码数据
备注：
*********************************************************************/
int CModuleHelp_JT1078::ModuleHelp_JT1078_BCDToInt(XBYTE chBCD)
{
    ModuleHelp_IsErrorOccur = false;

    XBYTE uszBsd1, uszBsd2, uszBsd3;

    uszBsd1 = chBCD & 0x0f;
    uszBsd2 = chBCD & 0xf0;
    uszBsd2 = uszBsd2 >> 4;
    uszBsd3 = uszBsd2 * 10 + uszBsd1;
    return uszBsd3;
}
/********************************************************************
函数名称：ModuleHelp_JT1078_IntToBCD
函数功能：将00-99的整数存放成1个字节的BCD
 参数.一：uszInt
  In/Out：In
  类型：无符号字符
  可空：N
  意思：输入要编码的整数
返回值
  类型：无符号字符
  意思：输出编码数据
备注：
*********************************************************************/
XBYTE CModuleHelp_JT1078::ModuleHelp_JT1078_IntToBCD(XBYTE uszInt)
{
    ModuleHelp_IsErrorOccur = false;

    XBYTE usBCD;
    XBYTE nRet;

    nRet = 0;
    usBCD = uszInt / 10;
    usBCD = usBCD << 4;
    nRet |= usBCD;
    usBCD = uszInt % 10;
    nRet |= usBCD;
    return nRet;
}