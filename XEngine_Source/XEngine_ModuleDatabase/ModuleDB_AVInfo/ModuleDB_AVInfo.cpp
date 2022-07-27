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
BOOL CModuleDB_AVInfo::ModuleDB_AVInfo_Init(DATABASE_MYSQL_CONNECTINFO* pSt_MySQLConnector)
{
	DBModule_IsErrorOccur = FALSE;

	_tcscpy(pSt_MySQLConnector->tszDBName, _T("StreamMedia_AVInfo"));
	//打开数据库
	if (!DataBase_MySQL_Connect(&xhSQL, pSt_MySQLConnector))
	{
		DBModule_IsErrorOccur = TRUE;
		DBModule_dwErrorCode = DataBase_GetLastError();
		return FALSE;
	}
	return TRUE;
}
/********************************************************************
函数名称：ModuleDatabase_MySql_Destory
函数功能：销毁MYSQL数据库资源链接
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleDB_AVInfo::ModuleDB_AVInfo_Destory()
{
	DBModule_IsErrorOccur = FALSE;

	DataBase_MySQL_Close(xhSQL);
	return TRUE;
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
BOOL CModuleDB_AVInfo::ModuleDB_AVInfo_InfoInsert(XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream)
{
	DBModule_IsErrorOccur = FALSE;

	if (NULL == pSt_ProtocolStream)
	{
		DBModule_IsErrorOccur = TRUE;
		DBModule_dwErrorCode = ERROR_MODULE_DATABASE_JT1078_PARAMENT;
		return FALSE;
	}
	TCHAR tszSQLQuery[4096];
	memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));

	if (ModuleDB_AVInfo_InfoQuery(pSt_ProtocolStream))
	{
		return TRUE;
	}

	_stprintf_s(tszSQLQuery, _T("INSERT INTO `DeviceList_AVInfo`(tszDeviceNumber,bVideo,nVideoBit,enVCodec,nWidth,nHeight,nFrameRate,bAudio,nAudioBit,enACodec,nChannel,nSampleRate,nSampleFmt,nFrameSize,TimeUPdate,TimeCreate) VALUES('%s',%d,%lld,%d,%d,%d,%d,%d,%lld,%d,%d,%d,%d,%d,now(),now())"), pSt_ProtocolStream->tszDeviceNumber, pSt_ProtocolStream->st_AVInfo.st_VideoInfo.bEnable, pSt_ProtocolStream->st_AVInfo.st_VideoInfo.nBitRate, pSt_ProtocolStream->st_AVInfo.st_VideoInfo.enAVCodec, pSt_ProtocolStream->st_AVInfo.st_VideoInfo.nWidth, pSt_ProtocolStream->st_AVInfo.st_VideoInfo.nHeight, pSt_ProtocolStream->st_AVInfo.st_VideoInfo.nFrameRate, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.bEnable, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nBitRate, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.enAVCodec, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nChannel, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nSampleRate, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nSampleFmt, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nFrameSize);

	if (!DataBase_MySQL_Execute(xhSQL, tszSQLQuery))
	{
		DBModule_IsErrorOccur = TRUE;
		DBModule_dwErrorCode = DataBase_GetLastError();
		return FALSE;
	}
	return TRUE;
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
BOOL CModuleDB_AVInfo::ModuleDB_AVInfo_InfoQuery(XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream)
{
	DBModule_IsErrorOccur = FALSE;

	if (NULL == pSt_ProtocolStream)
	{
		DBModule_IsErrorOccur = TRUE;
		DBModule_dwErrorCode = ERROR_MODULE_DATABASE_JT1078_PARAMENT;
		return FALSE;
	}
	__int64u dwLine = 0;
	__int64u dwField = 0;
	XNETHANDLE xhResult;

	TCHAR tszSQLQuery[4096];
	memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));

	_stprintf_s(tszSQLQuery, _T("SELECT * FROM `DeviceList_AVInfo` WHERE tszDeviceNumber = '%s'"), pSt_ProtocolStream->tszDeviceNumber);
	if (!DataBase_MySQL_ExecuteQuery(xhSQL, &xhResult, tszSQLQuery, &dwLine, &dwField))
	{
		DBModule_IsErrorOccur = TRUE;
		DBModule_dwErrorCode = DataBase_GetLastError();
		return FALSE;
	}
	if (dwLine <= 0)
	{
		DBModule_IsErrorOccur = TRUE;
		DBModule_dwErrorCode = ERROR_MODULE_DATABASE_JT1078_NODATA;
		return FALSE;
	}

	TCHAR** pptszResult = DataBase_MySQL_GetResult(xhSQL, xhResult);
	if (NULL == pptszResult[0])
	{
		DBModule_IsErrorOccur = TRUE;
		DBModule_dwErrorCode = ERROR_MODULE_DATABASE_JT1078_FAILED;
		return FALSE;
	}

	if (NULL != pSt_ProtocolStream)
	{
		if (NULL != pptszResult[2])
		{
			pSt_ProtocolStream->st_AVInfo.st_VideoInfo.bEnable = _ttoi(pptszResult[2]);
		}
		if (NULL != pptszResult[3])
		{
			pSt_ProtocolStream->st_AVInfo.st_VideoInfo.nBitRate = _ttoi(pptszResult[3]);
		}
		if (NULL != pptszResult[4])
		{
			pSt_ProtocolStream->st_AVInfo.st_VideoInfo.enAVCodec = _ttoi(pptszResult[4]);
		}
		if (NULL != pptszResult[5])
		{
			pSt_ProtocolStream->st_AVInfo.st_VideoInfo.nWidth = _ttoi(pptszResult[5]);
		}
		if (NULL != pptszResult[6])
		{
			pSt_ProtocolStream->st_AVInfo.st_VideoInfo.nHeight = _ttoi(pptszResult[6]);
		}
		if (NULL != pptszResult[7])
		{
			pSt_ProtocolStream->st_AVInfo.st_VideoInfo.nFrameRate = _ttoi(pptszResult[7]);
		}
		if (NULL != pptszResult[8])
		{
			pSt_ProtocolStream->st_AVInfo.st_AudioInfo.bEnable = _ttoi(pptszResult[8]);
		}
		if (NULL != pptszResult[9])
		{
			pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nBitRate = _ttoi(pptszResult[9]);
		}
		if (NULL != pptszResult[10])
		{
			pSt_ProtocolStream->st_AVInfo.st_AudioInfo.enAVCodec = _ttoi(pptszResult[10]);
		}
		if (NULL != pptszResult[11])
		{
			pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nChannel = _ttoi(pptszResult[11]);
		}
		if (NULL != pptszResult[12])
		{
			pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nSampleRate = _ttoi(pptszResult[12]);
		}
		if (NULL != pptszResult[13])
		{
			pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nSampleFmt = _ttoi(pptszResult[13]);
		}
		if (NULL != pptszResult[14])
		{
			pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nFrameSize = _ttoi(pptszResult[14]);
		}
	}
	DataBase_MySQL_FreeResult(xhSQL, xhResult);
	return TRUE;
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
BOOL CModuleDB_AVInfo::ModuleDB_AVInfo_InfoUPDate(XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream)
{
	DBModule_IsErrorOccur = FALSE;

	if (NULL == pSt_ProtocolStream)
	{
		DBModule_IsErrorOccur = TRUE;
		DBModule_dwErrorCode = ERROR_MODULE_DATABASE_JT1078_PARAMENT;
		return FALSE;
	}
	TCHAR tszSQLQuery[4096];
	TCHAR tszSQLTime[MAX_PATH];

	memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));
	memset(tszSQLTime, '\0', MAX_PATH);

	_stprintf_s(tszSQLQuery, _T("UPDATE DeviceList_AVInfo SET bVideo = %d,nVideoBit = %lld,enVCodec = %d,nWidth = %d,nHeight = %d,nFrameRate = %d,bAudio = %d,nAudioBit = %lld,enACodec = %d,nChannel = %d,nSampleRate = %d,nSampleFmt = %d,nFrameSize = %d,TimeUPdate = now() WHERE tszDeviceNumber = '%s'"), pSt_ProtocolStream->st_AVInfo.st_VideoInfo.bEnable, pSt_ProtocolStream->st_AVInfo.st_VideoInfo.nBitRate, pSt_ProtocolStream->st_AVInfo.st_VideoInfo.enAVCodec, pSt_ProtocolStream->st_AVInfo.st_VideoInfo.nWidth, pSt_ProtocolStream->st_AVInfo.st_VideoInfo.nHeight, pSt_ProtocolStream->st_AVInfo.st_VideoInfo.nFrameRate, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.bEnable, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nBitRate, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.enAVCodec, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nChannel, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nSampleRate, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nSampleFmt, pSt_ProtocolStream->st_AVInfo.st_AudioInfo.nFrameSize, pSt_ProtocolStream->tszDeviceNumber);

	if (!DataBase_MySQL_Execute(xhSQL, tszSQLQuery))
	{
		DBModule_IsErrorOccur = TRUE;
		DBModule_dwErrorCode = DataBase_GetLastError();
		return FALSE;
	}
	return TRUE;
}