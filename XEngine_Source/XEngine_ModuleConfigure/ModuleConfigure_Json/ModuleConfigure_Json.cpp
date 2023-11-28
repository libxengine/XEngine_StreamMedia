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
bool CModuleConfigure_Json::ModuleConfigure_Json_File(LPCXSTR lpszConfigFile, XENGINE_SERVICECONFIG* pSt_ServerConfig)
{
	Config_IsErrorOccur = false;

	if ((NULL == lpszConfigFile) || (NULL == pSt_ServerConfig))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	JSONCPP_STRING st_JsonError;
	Json::CharReaderBuilder st_JsonBuilder;
	//读取配置文件所有内容到缓冲区
	FILE* pSt_File = _xtfopen(lpszConfigFile, _X("rb"));
	if (NULL == pSt_File)
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_OPENFILE;
		return false;
	}
	size_t nCount = 0;
	XCHAR tszMsgBuffer[4096];
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
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_PARSE;
		return false;
	}
	_tcsxcpy(pSt_ServerConfig->tszSMSUrl, st_JsonRoot["tszSMSUrl"].asCString());
	_tcsxcpy(pSt_ServerConfig->tszIPAddr, st_JsonRoot["tszIPAddr"].asCString());
	pSt_ServerConfig->bDeamon = st_JsonRoot["bDeamon"].asInt();

	pSt_ServerConfig->nRTMPPort = st_JsonRoot["nRTMPPort"].asInt();
	pSt_ServerConfig->nHttpPort = st_JsonRoot["nHttpPort"].asInt();
	pSt_ServerConfig->nXStreamPort = st_JsonRoot["nXStreamPort"].asInt();
	pSt_ServerConfig->nJT1078Port = st_JsonRoot["nJT1078Port"].asInt();
	pSt_ServerConfig->nSrtPort = st_JsonRoot["nSrtPort"].asInt();
	//最大配置
	if (st_JsonRoot["XMax"].empty() || (8 != st_JsonRoot["XMax"].size()))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_XMAX;
		return false;
	}
	Json::Value st_JsonXMax = st_JsonRoot["XMax"];
	pSt_ServerConfig->st_XMax.nMaxClient = st_JsonXMax["nMaxClient"].asInt();
	pSt_ServerConfig->st_XMax.nMaxQueue = st_JsonXMax["nMaxQueue"].asInt();
	pSt_ServerConfig->st_XMax.nIOThread = st_JsonXMax["nIOThread"].asInt();
	pSt_ServerConfig->st_XMax.nHTTPThread = st_JsonXMax["nHTTPThread"].asInt();
	pSt_ServerConfig->st_XMax.nXStreamThread = st_JsonXMax["nXStreamThread"].asInt();
	pSt_ServerConfig->st_XMax.nRTMPThread = st_JsonXMax["nRTMPThread"].asInt();
	pSt_ServerConfig->st_XMax.nJT1078Thread = st_JsonXMax["nJT1078Thread"].asInt();
	pSt_ServerConfig->st_XMax.nSRTThread = st_JsonXMax["nSRTThread"].asInt();
	//时间配置
	if (st_JsonRoot["XTime"].empty() || (5 != st_JsonRoot["XTime"].size()))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_XTIME;
		return false;
	}
	Json::Value st_JsonXTime = st_JsonRoot["XTime"];
	pSt_ServerConfig->st_XTime.nTimeCheck = st_JsonXTime["nTimeCheck"].asInt();
	pSt_ServerConfig->st_XTime.nHTTPTimeout = st_JsonXTime["nHTTPTimeout"].asInt();
	pSt_ServerConfig->st_XTime.nXStreamTimeout = st_JsonXTime["nXStreamTimeout"].asInt();
	pSt_ServerConfig->st_XTime.nRTMPTimeout = st_JsonXTime["nRTMPTimeout"].asInt();
	pSt_ServerConfig->st_XTime.nJT1078Timeout = st_JsonXTime["nJT1078Timeout"].asInt();
	//时间配置
	if (st_JsonRoot["XPull"].empty() || (5 != st_JsonRoot["XPull"].size()))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_XPULL;
		return false;
	}
	Json::Value st_Pull = st_JsonRoot["XPull"];
	Json::Value st_PullXStream = st_Pull["XStream"];
	Json::Value st_PullRtmp = st_Pull["RTMP"];
	Json::Value st_PullFlv = st_Pull["FLV"];
	Json::Value st_PullRtsp = st_Pull["RTSP"];
	Json::Value st_PullHls = st_Pull["HLS"];

	pSt_ServerConfig->st_XPull.st_PullXStream.bEnable = st_PullXStream["bEnable"].asBool();
	pSt_ServerConfig->st_XPull.st_PullRtmp.bEnable = st_PullRtmp["bEnable"].asBool();
	pSt_ServerConfig->st_XPull.st_PullFlv.bEnable = st_PullFlv["bEnable"].asBool();
	
	pSt_ServerConfig->st_XPull.st_PullRtsp.bEnable = st_PullRtsp["bEnable"].asBool();
	pSt_ServerConfig->st_XPull.st_PullRtsp.nRTPPort = st_PullRtsp["nRTPPort"].asInt();
	pSt_ServerConfig->st_XPull.st_PullRtsp.nRTCPPort = st_PullRtsp["nRTCPPort"].asInt();

	pSt_ServerConfig->st_XPull.st_PullHls.bEnable = st_PullHls["bEnable"].asBool();
	//日志配置
	if (st_JsonRoot["XLog"].empty() || (3 != st_JsonRoot["XLog"].size()))
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_XLOG;
		return false;
	}
	Json::Value st_JsonXLog = st_JsonRoot["XLog"];
	pSt_ServerConfig->st_XLog.nMaxSize = st_JsonXLog["MaxSize"].asInt();
	pSt_ServerConfig->st_XLog.nMaxCount = st_JsonXLog["MaxCount"].asInt();
	pSt_ServerConfig->st_XLog.nLogLeave = st_JsonXLog["LogLeave"].asInt();
	//版本列表
	if (st_JsonRoot["XVer"].empty())
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_XVER;
		return false;
	}
	Json::Value st_JsonXVer = st_JsonRoot["XVer"];
	pSt_ServerConfig->st_XVer.pStl_ListVer = new list<string>;
	if (NULL == pSt_ServerConfig->st_XVer.pStl_ListVer)
	{
		Config_IsErrorOccur = true;
		Config_dwErrorCode = ERROR_MODULE_CONFIGURE_JSON_MALLOC;
		return false;
	}
	for (unsigned int i = 0; i < st_JsonXVer.size(); i++)
	{
		pSt_ServerConfig->st_XVer.pStl_ListVer->push_back(st_JsonXVer[i].asCString());
	}
	return true;
}