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
	_tcscpy(pSt_ServerConfig->tszSMSUrl, st_JsonRoot["tszSMSUrl"].asCString());
	_tcscpy(pSt_ServerConfig->tszIPAddr, st_JsonRoot["tszIPAddr"].asCString());
	pSt_ServerConfig->bDeamon = st_JsonRoot["bDeamon"].asInt();
	pSt_ServerConfig->nCenterPort = st_JsonRoot["nCenterPort"].asInt();
	//最大配置
	if (st_JsonRoot["XMax"].empty() || (4 != st_JsonRoot["XMax"].size()))
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
	//时间配置
	if (st_JsonRoot["XTime"].empty() || (2 != st_JsonRoot["XTime"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_XTIME;
		return FALSE;
	}
	Json::Value st_JsonXTime = st_JsonRoot["XTime"];
	pSt_ServerConfig->st_XTime.nTimeCheck = st_JsonXTime["nTimeCheck"].asInt();
	pSt_ServerConfig->st_XTime.nCenterTimeOut = st_JsonXTime["nTCPTimeOut"].asInt();
	//数据库配置
	if (st_JsonRoot["XSQL"].empty() || (4 != st_JsonRoot["XSQL"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_XSQL;
		return FALSE;
	}
	Json::Value st_JsonXSQL = st_JsonRoot["XSQL"];
	pSt_ServerConfig->st_XSql.nSQLPort = st_JsonXSQL["nSQLPort"].asInt();
	_tcscpy(pSt_ServerConfig->st_XSql.tszSQLAddr, st_JsonXSQL["tszSQLAddr"].asCString());
	_tcscpy(pSt_ServerConfig->st_XSql.tszSQLUser, st_JsonXSQL["tszSQLUser"].asCString());
	_tcscpy(pSt_ServerConfig->st_XSql.tszSQLPass, st_JsonXSQL["tszSQLPass"].asCString());
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
/********************************************************************
函数名称：ModuleConfigure_Json_JT1078
函数功能：读取1078配置文件
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
BOOL CModuleConfigure_Json::ModuleConfigure_Json_JT1078(LPCTSTR lpszConfigFile, XENGINE_JT1078CONFIG* pSt_ServerConfig)
{
	Config_IsErrorOccur = FALSE;

	if ((NULL == lpszConfigFile) || (NULL == pSt_ServerConfig))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_PARAMENT;
		return FALSE;
	}
	JSONCPP_STRING st_JsonError;
	Json::Value st_JsonRoot;
	Json::CharReaderBuilder st_JsonBuilder;

	FILE* pSt_File = _tfopen(lpszConfigFile, _T("rb"));
	if (NULL == pSt_File)
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_OPENFILE;
		return FALSE;
	}
	int nCount = 0;
	TCHAR tszMsgBuffer[4096];
	while (1)
	{
		int nRet = fread(tszMsgBuffer + nCount, 1, 2048, pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		nCount += nRet;
	}
	fclose(pSt_File);
	std::unique_ptr<Json::CharReader> const pSt_JsonReader(st_JsonBuilder.newCharReader());
	if (!pSt_JsonReader->parse(tszMsgBuffer, tszMsgBuffer + nCount, &st_JsonRoot, &st_JsonError))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_PARSE;
		return FALSE;
	}
	_tcscpy(pSt_ServerConfig->tszIPAddr, st_JsonRoot["tszIPAddr"].asCString());
	pSt_ServerConfig->bDeamon = st_JsonRoot["bDeamon"].asBool();
	pSt_ServerConfig->nAudio = st_JsonRoot["nAudio"].asBool();
	pSt_ServerConfig->nStreamPort = st_JsonRoot["nStreamPort"].asInt();
	pSt_ServerConfig->nRecordPort = st_JsonRoot["nRecordPort"].asInt();

	if (st_JsonRoot["XMax"].empty() || (5 != st_JsonRoot["XMax"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_XMAX;
		return FALSE;
	}
	Json::Value st_JsonXMax = st_JsonRoot["XMax"];
	pSt_ServerConfig->st_XMax.nMaxClient = st_JsonXMax["MaxClient"].asInt();
	pSt_ServerConfig->st_XMax.nMaxQueue = st_JsonXMax["MaxQueue"].asInt();
	pSt_ServerConfig->st_XMax.nIOThread = st_JsonXMax["IOThread"].asInt();
	pSt_ServerConfig->st_XMax.nStreamThread = st_JsonXMax["StreamThread"].asInt();
	pSt_ServerConfig->st_XMax.nRecordThread = st_JsonXMax["RecordThread"].asInt();

	if (st_JsonRoot["XTime"].empty() || (3 != st_JsonRoot["XTime"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_XTIME;
		return FALSE;
	}
	Json::Value st_JsonXTime = st_JsonRoot["XTime"];
	pSt_ServerConfig->st_XTime.nTimeCheck = st_JsonXTime["nTimeCheck"].asInt();
	pSt_ServerConfig->st_XTime.nStreamTimeout = st_JsonXTime["nStreamTimeout"].asInt();
	pSt_ServerConfig->st_XTime.nRecordTimeout = st_JsonXTime["nRecordTimeout"].asInt();

	if (st_JsonRoot["XClient"].empty() || (3 != st_JsonRoot["XClient"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_XCLIENT;
		return FALSE;
	}
	Json::Value st_JsonXClient = st_JsonRoot["XClient"];
	pSt_ServerConfig->st_XClient.nMaxConnect = st_JsonXClient["nMaxConnect"].asInt();
	pSt_ServerConfig->st_XClient.nPort = st_JsonXClient["nPort"].asInt();
	_tcscpy(pSt_ServerConfig->st_XClient.tszIPAddr, st_JsonXClient["tszIPAddr"].asCString());

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
/********************************************************************
函数名称：ModuleConfigure_Json_PluginFile
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
  意思：输出SDK服务配置信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleConfigure_Json::ModuleConfigure_Json_Sdk(LPCTSTR lpszConfigFile, XENGINE_SDKCONFIG* pSt_ServerConfig)
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
	pSt_ServerConfig->bDeamon = st_JsonRoot["bDeamon"].asBool();
	pSt_ServerConfig->nHttpPort = st_JsonRoot["nHttpPort"].asInt();

	if (st_JsonRoot["XMax"].empty() || (4 != st_JsonRoot["XMax"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_XMAX;
		return FALSE;
	}
	Json::Value st_JsonXMax = st_JsonRoot["XMax"];
	pSt_ServerConfig->st_XMax.nMaxClient = st_JsonXMax["MaxClient"].asInt();
	pSt_ServerConfig->st_XMax.nMaxQueue = st_JsonXMax["MaxQueue"].asInt();
	pSt_ServerConfig->st_XMax.nIOThread = st_JsonXMax["IOThread"].asInt();
	pSt_ServerConfig->st_XMax.nHTTPThread = st_JsonXMax["HttpThread"].asInt();

	if (st_JsonRoot["XTime"].empty() || (2 != st_JsonRoot["XTime"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_XTIME;
		return FALSE;
	}
	Json::Value st_JsonXTime = st_JsonRoot["XTime"];
	pSt_ServerConfig->st_XTime.nTimeCheck = st_JsonXTime["nTimeCheck"].asInt();
	pSt_ServerConfig->st_XTime.nHTTPTimeOut = st_JsonXTime["nHttpTimeout"].asInt();
	//客户端解析
	if (st_JsonRoot["XClient"].empty() || (3 != st_JsonRoot["XClient"].size()))
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_XCLIENT;
		return FALSE;
	}
	Json::Value st_JsonXClient = st_JsonRoot["XClient"];
	pSt_ServerConfig->st_XClient.nPort = st_JsonXClient["nPort"].asInt();
	pSt_ServerConfig->st_XClient.nMaxClient = st_JsonXClient["nMaxClient"].asInt();
	_tcscpy(pSt_ServerConfig->st_XClient.tszIPAddr, st_JsonXClient["tszIPAddr"].asCString());
	//解析插件
	pSt_ServerConfig->st_XPlugin.pStl_ListPlugin = new list<XENGINE_PLUGININFO>;
	if (NULL == pSt_ServerConfig->st_XPlugin.pStl_ListPlugin)
	{
		Config_IsErrorOccur = TRUE;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_MALLOC;
		return FALSE;
	}
	Json::Value st_JsonArray = st_JsonRoot["XPlugin"];
	for (unsigned int i = 0; i < st_JsonArray.size(); i++)
	{
		XENGINE_PLUGININFO st_PluginInfo;
		memset(&st_PluginInfo, '\0', sizeof(XENGINE_PLUGININFO));

		st_PluginInfo.bEnable = st_JsonArray[i]["PluginEnable"].asBool();
		st_PluginInfo.nPort = st_JsonArray[i]["PluginPort"].asInt();
		st_PluginInfo.xhToken = st_JsonArray[i]["PluginToken"].asInt64();
		_tcscpy(st_PluginInfo.tszPluginFile, st_JsonArray[i]["PluginFile"].asCString());
		_tcscpy(st_PluginInfo.tszPluginName, st_JsonArray[i]["PluginName"].asCString());
		_tcscpy(st_PluginInfo.tszPluginAddr, st_JsonArray[i]["PluginAddr"].asCString());
		_tcscpy(st_PluginInfo.tszPluginUser, st_JsonArray[i]["PluginUser"].asCString());
		_tcscpy(st_PluginInfo.tszPluginPass, st_JsonArray[i]["PluginPass"].asCString());
		pSt_ServerConfig->st_XPlugin.pStl_ListPlugin->push_back(st_PluginInfo);
	}
	//解析版本
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