#include "pch.h"
#include "ModuleProtocol_Parse.h"
/********************************************************************
//    Created:     2022/09/28  15:32:08
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_Parse\ModuleProtocol_Parse.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_Parse
//    File Base:   ModuleProtocol_Parse
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     协议解析工具
//    History:
*********************************************************************/
CModuleProtocol_Parse::CModuleProtocol_Parse()
{
}
CModuleProtocol_Parse::~CModuleProtocol_Parse()
{

}
//////////////////////////////////////////////////////////////////////////
//                             公有函数
//////////////////////////////////////////////////////////////////////////
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
 参数.四：pxhToken
  In/Out：Out
  类型：句柄
  可空：Y
  意思：输出解析到的URL
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleProtocol_Parse::ModuleProtocol_Parse_HTTPForward(LPCTSTR lpszMsgBuffer, int nMsgLen, TCHAR* ptszAVUrl, XNETHANDLE* pxhToken /* = NULL */)
{
	ModuleProtocol_IsErrorOccur = FALSE;

	if ((NULL == lpszMsgBuffer) || (0 == nMsgLen))
	{
		ModuleProtocol_IsErrorOccur = TRUE;
		ModuleProtocol_dwErrorCode = ERROR_MODULE_PROTOCOL_PARSE_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_JsonBuilder;
	//开始解析配置文件
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		ModuleProtocol_IsErrorOccur = TRUE;
		ModuleProtocol_dwErrorCode = ERROR_MODULE_PROTOCOL_PARSE_JSON;
		return FALSE;
	}
	if (st_JsonRoot["tszAVUrl"].isNull())
	{
		ModuleProtocol_IsErrorOccur = TRUE;
		ModuleProtocol_dwErrorCode = ERROR_MODULE_PROTOCOL_PARSE_JSON;
		return FALSE;
	}
	_tcscpy(ptszAVUrl, st_JsonRoot["tszAVUrl"].asCString());

	if (!st_JsonRoot["xhToken"].isNull() && (NULL != pxhToken))
	{
		*pxhToken = st_JsonRoot["xhToken"].asUInt64();
	}
	return TRUE;
}