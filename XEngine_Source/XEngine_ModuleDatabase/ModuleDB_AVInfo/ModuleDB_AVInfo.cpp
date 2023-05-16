#include "pch.h"
#include "ModuleDB_AVInfo.h"
/********************************************************************
//    Created:     2022/07/27  10:29:42
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleDatabase\ModuleDB_AVInfo\ModuleDB_AVInfo.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleDatabase\ModuleDB_AVInfo
//    File Base:   ModuleDB_AVInfo
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     音视频信息数据库表操作
//    History:
*********************************************************************/
CModuleDB_AVInfo::CModuleDB_AVInfo()
{
}
CModuleDB_AVInfo::~CModuleDB_AVInfo()
{

}
//////////////////////////////////////////////////////////////////////////
//                             公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：ModuleDatabase_MySql_Init
函数功能：初始化
 参数.一：lpszSQLFile
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入要操作的SQL文件
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleDB_AVInfo::ModuleDB_AVInfo_Init(DATABASE_MYSQL_CONNECTINFO* pSt_MySQLConnector)
{
	DBModule_IsErrorOccur = false;

	_tcsxcpy(pSt_MySQLConnector->tszDBName, _X("StreamMedia_AVInfo"));
	//打开数据库
	if (!DataBase_MySQL_Connect(&xhSQL, pSt_MySQLConnector))
	{
		DBModule_IsErrorOccur = true;
		DBModule_dwErrorCode = DataBase_GetLastError();
		return false;
	}
	return true;
}
/********************************************************************
函数名称：ModuleDatabase_MySql_Destory
函数功能：销毁MYSQL数据库资源链接
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleDB_AVInfo::ModuleDB_AVInfo_Destory()
{
	DBModule_IsErrorOccur = false;

	DataBase_MySQL_Close(xhSQL);
	return true;
}
/********************************************************************
函数名称：ModuleDB_AVInfo_InfoInsert
函数功能：音视频参数插入
 参数.一：pSt_ProtocolStream
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要插入的信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleDB_AVInfo::ModuleDB_AVInfo_InfoInsert(XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream)
{
	DBModule_IsErrorOccur = false;

	if (NULL == pSt_ProtocolStream)
	{
		DBModule_IsErrorOccur = true;
		DBModule_dwErrorCode = ERROR_MODULE_DATABASE_JT1078_PARAMENT;
		return false;
	}
	XCHAR tszSQLQuery[4096];
	memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));

	if (ModuleDB_AVInfo_InfoQuery(pSt_ProtocolStream))
	{
		return true;
	}

	_xstprintf(tszSQLQuery, _X("INSERT INTO `DeviceList_AVInfo`(tszDeviceNumber,bVideo,nVideoBit,enVCodec,nWidth,nHeight,nFrameRate,bAudio,nAudioBit,enACodec,nChannel,nSampleRate,nSampleFmt,nFrameSize,TimeUPdate,TimeCreate) VALUES('%s',%d,%lld,%d,%d,%d,%d,%d,%lld,%d,%d,%d,%d,%d,now(),now())"), pSt_ProtocolStream->tszDeviceNumber, pSt_ProtocolStream->st_AVInfo.st_VideoInfo.bEnable, pSt_ProtocolStream->st_AVInfo.st_VideoInfo.nBitRate, pSt_ProtocolStream->st_AVInfo.st_VideoInfo.enAVCodec, pSt_ProtocolStream->st_AVInfo.st_VideoInfo.nWidth, pSt_ProtocolStream->st_AVInfo.st_VideoInfo.nHeight, pSt_ProtocolStream->st_AVInfo.st_VideoInfo.nFrameRate, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.bEnable, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nBitRate, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.enAVCodec, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nChannel, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nSampleRate, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nSampleFmt, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nFrameSize);

	if (!DataBase_MySQL_Execute(xhSQL, tszSQLQuery))
	{
		DBModule_IsErrorOccur = true;
		DBModule_dwErrorCode = DataBase_GetLastError();
		return false;
	}
	return true;
}
/********************************************************************
函数名称：ModuleDB_AVInfo_InfoQuery
函数功能：音视频信息参数查询
 参数.一：pSt_ProtocolStream
  In/Out：Out
  类型：数据结构指针
  可空：N
  意思：输出查询到的内容
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleDB_AVInfo::ModuleDB_AVInfo_InfoQuery(XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream)
{
	DBModule_IsErrorOccur = false;

	if (NULL == pSt_ProtocolStream)
	{
		DBModule_IsErrorOccur = true;
		DBModule_dwErrorCode = ERROR_MODULE_DATABASE_JT1078_PARAMENT;
		return false;
	}
	__int64u dwLine = 0;
	__int64u dwField = 0;
	XNETHANDLE xhResult;

	XCHAR tszSQLQuery[4096];
	memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));

	_xstprintf(tszSQLQuery, _X("SELECT * FROM `DeviceList_AVInfo` WHERE tszDeviceNumber = '%s'"), pSt_ProtocolStream->tszDeviceNumber);
	if (!DataBase_MySQL_ExecuteQuery(xhSQL, &xhResult, tszSQLQuery, &dwLine, &dwField))
	{
		DBModule_IsErrorOccur = true;
		DBModule_dwErrorCode = DataBase_GetLastError();
		return false;
	}
	if (dwLine <= 0)
	{
		DBModule_IsErrorOccur = true;
		DBModule_dwErrorCode = ERROR_MODULE_DATABASE_JT1078_NODATA;
		return false;
	}

	XCHAR** pptszResult = DataBase_MySQL_GetResult(xhSQL, xhResult);
	if (NULL == pptszResult[0])
	{
		DBModule_IsErrorOccur = true;
		DBModule_dwErrorCode = ERROR_MODULE_DATABASE_JT1078_FAILED;
		return false;
	}

	if (NULL != pSt_ProtocolStream)
	{
		if (NULL != pptszResult[2])
		{
			pSt_ProtocolStream->st_AVInfo.st_VideoInfo.bEnable = _ttxoi(pptszResult[2]);
		}
		if (NULL != pptszResult[3])
		{
			pSt_ProtocolStream->st_AVInfo.st_VideoInfo.nBitRate = _ttxoi(pptszResult[3]);
		}
		if (NULL != pptszResult[4])
		{
			pSt_ProtocolStream->st_AVInfo.st_VideoInfo.enAVCodec = _ttxoi(pptszResult[4]);
		}
		if (NULL != pptszResult[5])
		{
			pSt_ProtocolStream->st_AVInfo.st_VideoInfo.nWidth = _ttxoi(pptszResult[5]);
		}
		if (NULL != pptszResult[6])
		{
			pSt_ProtocolStream->st_AVInfo.st_VideoInfo.nHeight = _ttxoi(pptszResult[6]);
		}
		if (NULL != pptszResult[7])
		{
			pSt_ProtocolStream->st_AVInfo.st_VideoInfo.nFrameRate = _ttxoi(pptszResult[7]);
		}
		if (NULL != pptszResult[8])
		{
			pSt_ProtocolStream->st_AVInfo.st_AudioInfo.bEnable = _ttxoi(pptszResult[8]);
		}
		if (NULL != pptszResult[9])
		{
			pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nBitRate = _ttxoi(pptszResult[9]);
		}
		if (NULL != pptszResult[10])
		{
			pSt_ProtocolStream->st_AVInfo.st_AudioInfo.enAVCodec = _ttxoi(pptszResult[10]);
		}
		if (NULL != pptszResult[11])
		{
			pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nChannel = _ttxoi(pptszResult[11]);
		}
		if (NULL != pptszResult[12])
		{
			pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nSampleRate = _ttxoi(pptszResult[12]);
		}
		if (NULL != pptszResult[13])
		{
			pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nSampleFmt = _ttxoi(pptszResult[13]);
		}
		if (NULL != pptszResult[14])
		{
			pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nFrameSize = _ttxoi(pptszResult[14]);
		}
	}
	DataBase_MySQL_FreeResult(xhSQL, xhResult);
	return true;
}
/********************************************************************
函数名称：ModuleDB_AVInfo_InfoUPDate
函数功能：更新音视频参数信息
 参数.一：pSt_ProtocolStream
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要更新的内容
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleDB_AVInfo::ModuleDB_AVInfo_InfoUPDate(XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream)
{
	DBModule_IsErrorOccur = false;

	if (NULL == pSt_ProtocolStream)
	{
		DBModule_IsErrorOccur = true;
		DBModule_dwErrorCode = ERROR_MODULE_DATABASE_JT1078_PARAMENT;
		return false;
	}
	XCHAR tszSQLQuery[4096];
	XCHAR tszSQLTime[MAX_PATH];

	memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));
	memset(tszSQLTime, '\0', MAX_PATH);

	_xstprintf(tszSQLQuery, _X("UPDATE DeviceList_AVInfo SET bVideo = %d,nVideoBit = %lld,enVCodec = %d,nWidth = %d,nHeight = %d,nFrameRate = %d,bAudio = %d,nAudioBit = %lld,enACodec = %d,nChannel = %d,nSampleRate = %d,nSampleFmt = %d,nFrameSize = %d,TimeUPdate = now() WHERE tszDeviceNumber = '%s'"), pSt_ProtocolStream->st_AVInfo.st_VideoInfo.bEnable, pSt_ProtocolStream->st_AVInfo.st_VideoInfo.nBitRate, pSt_ProtocolStream->st_AVInfo.st_VideoInfo.enAVCodec, pSt_ProtocolStream->st_AVInfo.st_VideoInfo.nWidth, pSt_ProtocolStream->st_AVInfo.st_VideoInfo.nHeight, pSt_ProtocolStream->st_AVInfo.st_VideoInfo.nFrameRate, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.bEnable, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nBitRate, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.enAVCodec, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nChannel, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nSampleRate, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nSampleFmt, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nFrameSize, pSt_ProtocolStream->tszDeviceNumber);

	if (!DataBase_MySQL_Execute(xhSQL, tszSQLQuery))
	{
		DBModule_IsErrorOccur = true;
		DBModule_dwErrorCode = DataBase_GetLastError();
		return false;
	}
	return true;
}