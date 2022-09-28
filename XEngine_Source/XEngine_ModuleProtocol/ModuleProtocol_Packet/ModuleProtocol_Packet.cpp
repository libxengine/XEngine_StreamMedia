#include "pch.h"
#include "ModuleProtocol_Packet.h"
/********************************************************************
//    Created:     2022/09/28  15:30:22
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_Packet\ModuleProtocol_Packet.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ModuleProtocol\ModuleProtocol_Packet
//    File Base:   ModuleProtocol_Packet
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     协议打包工具
//    History:
*********************************************************************/
CModuleProtocol_Packet::CModuleProtocol_Packet()
{
}
CModuleProtocol_Packet::~CModuleProtocol_Packet()
{

}
//////////////////////////////////////////////////////////////////////////
//                             公有函数
//////////////////////////////////////////////////////////////////////////
/********************************************************************
函数名称：ModuleProtocol_Packet_Create
函数功能：流媒体创建
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出协议缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出缓冲区大小
 参数.三：pSt_ProtocolDevice
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入设备信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleProtocol_Packet::ModuleProtocol_Packet_Create(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLDEVICE* pSt_ProtocolDevice)
{
	ModuleProtocol_IsErrorOccur = FALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		ModuleProtocol_IsErrorOccur = TRUE;
		ModuleProtocol_dwErrorCode = ERROR_MODULE_PROTOCOL_JT1078_PARAMENT;
		return FALSE;
	}
	XENGINE_PROTOCOLHDR st_ProcotolHdr;
	memset(&st_ProcotolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

	st_ProcotolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProcotolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_SMS;
	st_ProcotolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REQCREATE;
	st_ProcotolHdr.unPacketSize = sizeof(XENGINE_PROTOCOLDEVICE);
	st_ProcotolHdr.xhToken = 0;
	st_ProcotolHdr.wReserve = 0;
	st_ProcotolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	*pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + sizeof(XENGINE_PROTOCOLDEVICE);
	memcpy(ptszMsgBuffer, &st_ProcotolHdr, sizeof(XENGINE_PROTOCOLHDR));
	memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), pSt_ProtocolDevice, sizeof(XENGINE_PROTOCOLDEVICE));
	return TRUE;
}
/********************************************************************
函数名称：ModuleProtocol_Packet_Push
函数功能：推流协议
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出协议缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出缓冲区大小
 参数.三：pSt_ProtocolDevice
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入设备信息
 参数.四：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：N
  意思：输入推流数据
 参数.五：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：推流数据大小
 参数.六：nMsgType
  In/Out：In
  类型：整数型
  可空：N
  意思：消息类型,0视频,1音频
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleProtocol_Packet::ModuleProtocol_Packet_Push(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLDEVICE* pSt_ProtocolDevice, LPCTSTR lpszMsgBuffer, int nMsgLen, int nMsgType)
{
	ModuleProtocol_IsErrorOccur = FALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		ModuleProtocol_IsErrorOccur = TRUE;
		ModuleProtocol_dwErrorCode = ERROR_MODULE_PROTOCOL_JT1078_PARAMENT;
		return FALSE;
	}
	XENGINE_PROTOCOLHDR st_ProcotolHdr;
	memset(&st_ProcotolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

	st_ProcotolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProcotolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_SMS;
	st_ProcotolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REQPUSH;
	st_ProcotolHdr.unPacketSize = sizeof(XENGINE_PROTOCOLDEVICE) + nMsgLen;
	st_ProcotolHdr.xhToken = 0;
	st_ProcotolHdr.wReserve = nMsgType;
	st_ProcotolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	*pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + st_ProcotolHdr.unPacketSize;
	memcpy(ptszMsgBuffer, &st_ProcotolHdr, sizeof(XENGINE_PROTOCOLHDR));
	memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), pSt_ProtocolDevice, sizeof(XENGINE_PROTOCOLDEVICE));
	memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR) + sizeof(XENGINE_PROTOCOLDEVICE), lpszMsgBuffer, nMsgLen);
	return TRUE;
}
/********************************************************************
函数名称：ModuleProtocol_Packet_Destroy
函数功能：销毁流协议
 参数.一：ptszMsgBuffer
  In/Out：Out
  类型：字符指针
  可空：N
  意思：输出协议缓冲区
 参数.二：pInt_MsgLen
  In/Out：Out
  类型：整数型指针
  可空：N
  意思：输出缓冲区大小
 参数.三：pSt_ProtocolDev
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入要销毁的流信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
BOOL CModuleProtocol_Packet::ModuleProtocol_Packet_Destroy(TCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLDEVICE* pSt_ProtocolDev)
{
	ModuleProtocol_IsErrorOccur = FALSE;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		ModuleProtocol_IsErrorOccur = TRUE;
		ModuleProtocol_dwErrorCode = ERROR_MODULE_PROTOCOL_JT1078_PARAMENT;
		return FALSE;
	}
	XENGINE_PROTOCOLHDR st_ProcotolHdr;
	memset(&st_ProcotolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

	st_ProcotolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProcotolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_SMS;
	st_ProcotolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REQDESTROY;
	st_ProcotolHdr.unPacketSize = sizeof(XENGINE_PROTOCOLDEVICE);
	st_ProcotolHdr.xhToken = 0;
	st_ProcotolHdr.wReserve = 0;
	st_ProcotolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	*pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + sizeof(XENGINE_PROTOCOLDEVICE);
	memcpy(ptszMsgBuffer, &st_ProcotolHdr, sizeof(XENGINE_PROTOCOLHDR));
	memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), pSt_ProtocolDev, sizeof(XENGINE_PROTOCOLDEVICE));
	return TRUE;
}