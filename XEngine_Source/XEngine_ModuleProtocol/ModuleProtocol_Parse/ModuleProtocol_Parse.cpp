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
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleProtocol_Parse::ModuleProtocol_Parse_HTTPForward(LPCXSTR lpszMsgBuffer, int nMsgLen, XCHAR* ptszAVUrl)
{
	ModuleProtocol_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer) || (0 == nMsgLen))
	{
		ModuleProtocol_IsErrorOccur = true;
		ModuleProtocol_dwErrorCode = ERROR_MODULE_PROTOCOL_PARSE_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_JsonBuilder;
	//开始解析配置文件
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		ModuleProtocol_IsErrorOccur = true;
		ModuleProtocol_dwErrorCode = ERROR_MODULE_PROTOCOL_PARSE_JSON;
		return false;
	}
	if (!st_JsonRoot["tszAVUrl"].isNull() && (NULL != ptszAVUrl))
	{
		_tcsxcpy(ptszAVUrl, st_JsonRoot["tszAVUrl"].asCString());
	}
	return true;
}
/********************************************************************
函数名称：ModuleProtocol_Parse_HTTPParseAuth
函数功能：用户验证解析协议
 参数.一：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要解析的缓冲区
 参数.二：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：输入要解析的大小
 参数.三：pSt_UserAuth
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出解析的数据
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleProtocol_Parse::ModuleProtocol_Parse_HTTPParseAuth(LPCXSTR lpszMsgBuffer, int nMsgLen, XENGINE_PROTOCOL_USERAUTHEX* pSt_UserAuth)
{
	ModuleProtocol_IsErrorOccur = false;

	if ((NULL == lpszMsgBuffer) || (NULL == pSt_UserAuth))
	{
		ModuleProtocol_IsErrorOccur = true;
		ModuleProtocol_dwErrorCode = ERROR_MODULE_PROTOCOL_PARSE_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_ReaderBuilder;

	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_ReaderBuilder.newCharReader());
	if (!pSt_JsonReader->parse(lpszMsgBuffer, lpszMsgBuffer + nMsgLen, &st_JsonRoot, &st_JsonError))
	{
		ModuleProtocol_IsErrorOccur = true;
		ModuleProtocol_dwErrorCode = ERROR_MODULE_PROTOCOL_PARSE_JSON;
		return false;
	}
	Json::Value st_JsonProtocol = st_JsonRoot["st_UserAuth"];

	if (!st_JsonProtocol["tszUserName"].isNull())
	{
		_tcsxcpy(pSt_UserAuth->tszUserName, st_JsonProtocol["tszUserName"].asCString());
	}
	if (!st_JsonProtocol["tszUserPass"].isNull())
	{
		_tcsxcpy(pSt_UserAuth->tszUserPass, st_JsonProtocol["tszUserPass"].asCString());
	}
	if (!st_JsonProtocol["enClientType"].isNull())
	{
		pSt_UserAuth->enClientType = (ENUM_PROTOCOLCLIENT_TYPE)st_JsonProtocol["enClientType"].asInt();
	}
	if (!st_JsonProtocol["enDeviceType"].isNull())
	{
		pSt_UserAuth->enDeviceType = (ENUM_PROTOCOLDEVICE_TYPE)st_JsonProtocol["enDeviceType"].asInt();
	}
	if (!st_JsonProtocol["tszDCode"].isNull())
	{
		_tcsxcpy(pSt_UserAuth->tszDCode, st_JsonProtocol["tszDCode"].asCString());
	}
	if (!st_JsonProtocol["tszHWCode"].isNull())
	{
		_tcsxcpy(pSt_UserAuth->tszHWCode, st_JsonProtocol["tszHWCode"].asCString());
	}
	return true;
}