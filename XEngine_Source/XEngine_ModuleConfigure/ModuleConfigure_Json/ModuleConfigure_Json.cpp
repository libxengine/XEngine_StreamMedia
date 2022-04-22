#include "pch.h"
#include "ModuleConfigure_Json.h"
/********************************************************************
//    Created:     2021/12/02  16:14:11
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleConfigure\ModuleConfigure_Json\ModuleConfigure_Json.cpp
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_ModuleConfigure\ModuleConfigure_Json
//    File Base:   ModuleConfigure_Json
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     JSON配置读写实现
//    History:
*********************************************************************/
CModuleConfigure_Json::CModuleConfigure_Json()
{

}
CModuleConfigure_Json::~CModuleConfigure_Json()
{

}
//////////////////////////////////////////////////////////////////////////
//                        公用函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：ModuleConfigure_Json_File
函数功能：读取JSON配置文件
 参数.一：lpszConfigFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要读取的配置文件
 参数.二：pSt_ServerConfig
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出服务配置信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleConfigure_Json::ModuleConfigure_Json_File(LPCTSTR lpszConfigFile, XENGINE_SERVICECONFIG* pSt_ServerConfig)
{
	Config_IsErrorOccur = FALSE;

	if ((NULL == lpszConfigFile) || (NULL == pSt_ServerConfig))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_PARAMENT;
		return FALSE;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_JsonBuilder;
	//读取配置文件所有内容到缓冲区
	FILE* pSt_File = _tfopen(lpszConfigFile, _T("rb"));
	if (NULL == pSt_File)
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_OPENFILE;
		return FALSE;
	}
	size_t nCount = 0;
	TCHAR tszMsgBuffer[4096];
	while (1)
	{
		size_t nRet = fread(tszMsgBuffer + nCount, 1, 2048, pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		nCount += nRet;
	}
	fclose(pSt_File);
	//开始解析配置文件
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(tszMsgBuffer, tszMsgBuffer + nCount, &st_JsonRoot, &st_JsonError))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_PARSE;
		return FALSE;
	}
	_tcscpy(pSt_ServerConfig->tszIPAddr, st_JsonRoot["tszIPAddr"].asCString());
	pSt_ServerConfig->bDeamon = st_JsonRoot["bDeamon"].asInt();
	pSt_ServerConfig->nCenterPort = st_JsonRoot["nCenterPort"].asInt();
	pSt_ServerConfig->nHttpPort = st_JsonRoot["nHttpPort"].asInt();
	pSt_ServerConfig->nHttp2Port = st_JsonRoot["nHttp2Port"].asInt();
	pSt_ServerConfig->nWSPort = st_JsonRoot["nWSPort"].asInt();
	//最大配置
	if (st_JsonRoot["XMax"].empty() || (7 != st_JsonRoot["XMax"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_XMAX;
		return FALSE;
	}
	Json::Value st_JsonXMax = st_JsonRoot["XMax"];
	pSt_ServerConfig->st_XMax.nMaxClient = st_JsonXMax["nMaxClient"].asInt();
	pSt_ServerConfig->st_XMax.nMaxQueue = st_JsonXMax["nMaxQueue"].asInt();
	pSt_ServerConfig->st_XMax.nIOThread = st_JsonXMax["nIOThread"].asInt();
	pSt_ServerConfig->st_XMax.nCenterThread = st_JsonXMax["nCenterThread"].asInt();
	pSt_ServerConfig->st_XMax.nHTTPThread = st_JsonXMax["nHttpThread"].asInt();
	pSt_ServerConfig->st_XMax.nHTTP2Thread = st_JsonXMax["nHttp2Thread"].asInt();
	pSt_ServerConfig->st_XMax.nWSThread = st_JsonXMax["nWSThread"].asInt();
	//时间配置
	if (st_JsonRoot["XTime"].empty() || (5 != st_JsonRoot["XTime"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_XTIME;
		return FALSE;
	}
	Json::Value st_JsonXTime = st_JsonRoot["XTime"];
	pSt_ServerConfig->st_XTime.nTimeCheck = st_JsonXTime["nTimeCheck"].asInt();
	pSt_ServerConfig->st_XTime.nCenterTimeOut = st_JsonXTime["nTCPTimeOut"].asInt();
	pSt_ServerConfig->st_XTime.nHTTPTimeOut = st_JsonXTime["nHttpTimeOut"].asInt();
	pSt_ServerConfig->st_XTime.nHTTP2TimeOut = st_JsonXTime["nHttp2TimeOut"].asInt();
	pSt_ServerConfig->st_XTime.nWSTimeOut = st_JsonXTime["nWSTimeOut"].asInt();
	//日志配置
	if (st_JsonRoot["XLog"].empty() || (3 != st_JsonRoot["XLog"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_XLOG;
		return FALSE;
	}
	Json::Value st_JsonXLog = st_JsonRoot["XLog"];
	pSt_ServerConfig->st_XLog.nMaxSize = st_JsonXLog["MaxSize"].asInt();
	pSt_ServerConfig->st_XLog.nMaxCount = st_JsonXLog["MaxCount"].asInt();
	pSt_ServerConfig->st_XLog.nLogLeave = st_JsonXLog["LogLeave"].asInt();
	//版本列表
	if (st_JsonRoot["XVer"].empty())
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_XVER;
		return FALSE;
	}
	Json::Value st_JsonXVer = st_JsonRoot["XVer"];
	pSt_ServerConfig->st_XVer.pStl_ListVer = new list<string>;
	if (NULL == pSt_ServerConfig->st_XVer.pStl_ListVer)
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_MALLOC;
		return FALSE;
	}
	for (unsigned int i = 0; i < st_JsonXVer.size(); i++)
	{
		pSt_ServerConfig->st_XVer.pStl_ListVer->push_back(st_JsonXVer[i].asCString());
	}
	return TRUE;
}