#include "pch.h"
#include "ModuleDB_JT1078.h"
/********************************************************************
//    Created:     2022/04/24  14:45:01
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleDatabase\ModuleDB_JT1078\ModuleDB_JT1078.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleDatabase\ModuleDB_JT1078
//    File Base:   ModuleDB_JT1078
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     JT1078数据库表操作
//    History:
*********************************************************************/
CModuleDB_JT1078::CModuleDB_JT1078()
{
}
CModuleDB_JT1078::~CModuleDB_JT1078()
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
BOOL CModuleDB_JT1078::ModuleDB_JT1078_Init(DATABASE_MYSQL_CONNECTINFO* pSt_MySQLConnector)
{
	DBModule_IsErrorOccur = FALSE;

	_tcscpy(pSt_MySQLConnector->tszDBName, _T("StreamMedia_JT1078"));
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
BOOL CModuleDB_JT1078::ModuleDB_JT1078_Destory()
{
	DBModule_IsErrorOccur = FALSE;

	DataBase_MySQL_Close(xhSQL);
	return TRUE;
}
/********************************************************************
函数名称：ModuleDB_JT1078_DeviceInsert
函数功能：设备插入
 参数.一：lpszDeviceAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入设备IP地址
 参数.二：lpszDeviceNumber
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入设备编号
 参数.三：nChannel
  In/Out：In
  类型：整数型
  可空：N
  意思：输入通道号
 参数.四：xhClient
  In/Out：In
  类型：句柄
  可空：N
  意思：输入绑定的客户端
 参数.五：bLive
  In/Out：In
  类型：逻辑型
  可空：N
  意思：直播还是录像
 参数.六：lpszDeviceVer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：设备版本
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleDB_JT1078::ModuleDB_JT1078_DeviceInsert(LPCTSTR lpszDeviceAddr, LPCTSTR lpszDeviceNumber, int nChannel, XNETHANDLE xhClient, BOOL bLive, LPCTSTR lpszDeviceVer)
{
	DBModule_IsErrorOccur = FALSE;

	if ((NULL == lpszDeviceAddr) || (NULL == lpszDeviceNumber) || (NULL == lpszDeviceVer))
	{
		DBModule_IsErrorOccur = TRUE;
		DBModule_dwErrorCode = ERROR_MODULE_DATABASE_JT1078_PARAMENT;
		return FALSE;
	}
	TCHAR tszSQLQuery[4096];
	memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));

	_stprintf_s(tszSQLQuery, _T("INSERT INTO `DeviceList` (tszDeviceAddr,tszDeviceNumber,nChannel,xhClient,bLive,tszVersion,CreateTime) VALUES('%s','%s',%d,%lld,%d,'%s',now())"), lpszDeviceAddr, lpszDeviceNumber, nChannel, xhClient, bLive, lpszDeviceVer);

	if (!DataBase_MySQL_Execute(xhSQL, tszSQLQuery))
	{
		DBModule_IsErrorOccur = TRUE;
		DBModule_dwErrorCode = DataBase_GetLastError();
		return FALSE;
	}
	return TRUE;
}
/********************************************************************
函数名称：ModuleDB_JT1078_DeviceDelete
函数功能：设备删除
 参数.一：lpszDeviceAddr
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入设备地址
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleDB_JT1078::ModuleDB_JT1078_DeviceDelete(LPCTSTR lpszDeviceAddr)
{
	DBModule_IsErrorOccur = FALSE;

	if (NULL == lpszDeviceAddr)
	{
		DBModule_IsErrorOccur = TRUE;
		DBModule_dwErrorCode = ERROR_MODULE_DATABASE_JT1078_PARAMENT;
		return FALSE;
	}
	TCHAR tszSQLQuery[4096];
	memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));

	_stprintf_s(tszSQLQuery, _T("DELETE FROM `DeviceList` WHERE tszDeviceAddr = '%s'"), lpszDeviceAddr);
	if (!DataBase_MySQL_Execute(xhSQL, tszSQLQuery))
	{
		DBModule_IsErrorOccur = TRUE;
		DBModule_dwErrorCode = DataBase_GetLastError();
		return FALSE;
	}
	return TRUE;
}
/********************************************************************
函数名称：ModuleDB_JT1078_DeviceClear
函数功能：清空设备表
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleDB_JT1078::ModuleDB_JT1078_DeviceClear()
{
	DBModule_IsErrorOccur = FALSE;

	TCHAR tszSQLQuery[4096];
	memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));

	_stprintf_s(tszSQLQuery, _T("TRUNCATE TABLE `DeviceList`"));
	if (!DataBase_MySQL_Execute(xhSQL, tszSQLQuery))
	{
		DBModule_IsErrorOccur = TRUE;
		DBModule_dwErrorCode = DataBase_GetLastError();
		return FALSE;
	}
	return TRUE;
}
/********************************************************************
函数名称：ModuleDB_JT1078_InfoInsert
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
BOOL CModuleDB_JT1078::ModuleDB_JT1078_InfoInsert(XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream)
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

	if (ModuleDB_JT1078_InfoQuery(pSt_ProtocolStream))
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
函数名称：ModuleDB_JT1078_InfoQuery
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
BOOL CModuleDB_JT1078::ModuleDB_JT1078_InfoQuery(XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream)
{
	DBModule_IsErrorOccur = FALSE;

	if (NULL == pSt_ProtocolStream)
	{
		DBModule_IsErrorOccur = TRUE;
		DBModule_dwErrorCode = ERROR_MODULE_DATABASE_JT1078_PARAMENT;
		return FALSE;
	}
	DWORD64 dwLine = 0;
	DWORD64 dwField = 0;
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
函数名称：ModuleDB_JT1078_InfoUPDate
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
BOOL CModuleDB_JT1078::ModuleDB_JT1078_InfoUPDate(XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream)
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