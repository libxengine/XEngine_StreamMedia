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
函数名称：ModuleProtocol_Packet_Comm
函数功能：通用打包回复函数
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
 参数.三：pSt_ProtocolHdr
  In/Out：In
  类型：数据结构指针
  可空：Y
  意思：输入要打包的协议,如果需要的话
 参数.四：nCode
  In/Out：In
  类型：整数型
  可空：Y
  意思：输入回复的CODE
 参数.五：lpszMsgBuffer
  In/Out：In
  类型：常量字符指针
  可空：Y
  意思：输入要处理的缓冲区
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleProtocol_Packet::ModuleProtocol_Packet_Comm(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLHDR* pSt_ProtocolHdr /* = NULL */, int nCode /* = 0 */, LPCXSTR lpszMsgBuffer /* = NULL */)
{
	ModuleProtocol_IsErrorOccur = false;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		ModuleProtocol_IsErrorOccur = true;
		ModuleProtocol_dwErrorCode = ERROR_MODULE_PROTOCOL_PACKET_PARAMENT;
		return false;
	}
	int nPos = 0;
	Json::Value st_JsonRoot;

	st_JsonRoot["code"] = nCode;
	if (NULL == lpszMsgBuffer)
	{
		st_JsonRoot["msg"] = "success";
	}
	else
	{
		st_JsonRoot["msg"] = lpszMsgBuffer;
	}

	if (NULL != pSt_ProtocolHdr)
	{
		pSt_ProtocolHdr->unPacketSize = st_JsonRoot.toStyledString().length();

		memcpy(ptszMsgBuffer, pSt_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
		nPos = sizeof(XENGINE_PROTOCOLHDR);

		*pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + pSt_ProtocolHdr->unPacketSize;
	}
	else
	{
		*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	}
	memcpy(ptszMsgBuffer + nPos, st_JsonRoot.toStyledString().c_str(), st_JsonRoot.toStyledString().length());
	return true;
}
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
 参数.三：pSt_ProtocolStream
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入音视频流信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleProtocol_Packet::ModuleProtocol_Packet_Create(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream)
{
	ModuleProtocol_IsErrorOccur = false;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		ModuleProtocol_IsErrorOccur = true;
		ModuleProtocol_dwErrorCode = ERROR_MODULE_PROTOCOL_PACKET_PARAMENT;
		return false;
	}
	XENGINE_PROTOCOLHDR st_ProcotolHdr;
	memset(&st_ProcotolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

	st_ProcotolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProcotolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_SMS;
	st_ProcotolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REQCREATE;
	st_ProcotolHdr.unPacketSize = sizeof(XENGINE_PROTOCOLSTREAM);
	st_ProcotolHdr.xhToken = 0;
	st_ProcotolHdr.wReserve = 0;
	st_ProcotolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	*pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + sizeof(XENGINE_PROTOCOLSTREAM);
	memcpy(ptszMsgBuffer, &st_ProcotolHdr, sizeof(XENGINE_PROTOCOLHDR));
	memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), pSt_ProtocolStream, sizeof(XENGINE_PROTOCOLSTREAM));
	return true;
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
 参数.三：pSt_ProtocolStream
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入音视频流信息
 参数.四：nMsgLen
  In/Out：In
  类型：整数型
  可空：N
  意思：推流数据大小
 参数.五：nMsgType
  In/Out：In
  类型：整数型
  可空：N
  意思：消息类型,0视频,1音频
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleProtocol_Packet::ModuleProtocol_Packet_Push(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream, int nMsgLen, int nMsgType)
{
	ModuleProtocol_IsErrorOccur = false;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		ModuleProtocol_IsErrorOccur = true;
		ModuleProtocol_dwErrorCode = ERROR_MODULE_PROTOCOL_PACKET_PARAMENT;
		return false;
	}
	XENGINE_PROTOCOLHDR st_ProcotolHdr;
	memset(&st_ProcotolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

	st_ProcotolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProcotolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_SMS;
	st_ProcotolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REQPUSH;
	st_ProcotolHdr.unPacketSize = sizeof(XENGINE_PROTOCOLSTREAM) + nMsgLen;
	st_ProcotolHdr.xhToken = 0;
	st_ProcotolHdr.wReserve = nMsgType;
	st_ProcotolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	*pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + sizeof(XENGINE_PROTOCOLSTREAM);
	memcpy(ptszMsgBuffer, &st_ProcotolHdr, sizeof(XENGINE_PROTOCOLHDR));
	memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), pSt_ProtocolStream, sizeof(XENGINE_PROTOCOLSTREAM));
	return true;
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
 参数.三：pSt_ProtocolStream
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入音视频流信息
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleProtocol_Packet::ModuleProtocol_Packet_Destroy(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, XENGINE_PROTOCOLSTREAM* pSt_ProtocolStream)
{
	ModuleProtocol_IsErrorOccur = false;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		ModuleProtocol_IsErrorOccur = true;
		ModuleProtocol_dwErrorCode = ERROR_MODULE_PROTOCOL_PACKET_PARAMENT;
		return false;
	}
	XENGINE_PROTOCOLHDR st_ProcotolHdr;
	memset(&st_ProcotolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

	st_ProcotolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProcotolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_SMS;
	st_ProcotolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REQDESTROY;
	st_ProcotolHdr.unPacketSize = sizeof(XENGINE_PROTOCOLSTREAM);
	st_ProcotolHdr.xhToken = 0;
	st_ProcotolHdr.wReserve = 0;
	st_ProcotolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	*pInt_MsgLen = sizeof(XENGINE_PROTOCOLHDR) + sizeof(XENGINE_PROTOCOLSTREAM);
	memcpy(ptszMsgBuffer, &st_ProcotolHdr, sizeof(XENGINE_PROTOCOLHDR));
	memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), pSt_ProtocolStream, sizeof(XENGINE_PROTOCOLSTREAM));
	return true;
}
/********************************************************************
函数名称：ModuleProtocol_Packet_HTTPPublishGet
函数功能：获取发布流的信息
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
 参数.三：pppSt_PublishInfo
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入发布流信息列表数据
 参数.四：nListCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入数据个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleProtocol_Packet::ModuleProtocol_Packet_HTTPPublishGet(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, STREAMMEDIA_PUBLISHINFO*** pppSt_PublishInfo, int nListCount)
{
	ModuleProtocol_IsErrorOccur = false;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		ModuleProtocol_IsErrorOccur = true;
		ModuleProtocol_dwErrorCode = ERROR_MODULE_PROTOCOL_PACKET_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonArray;

	st_JsonRoot["code"] = 0;
	st_JsonRoot["msg"] = "success";

	for (int i = 0; i < nListCount; i++)
	{
		Json::Value st_JsonObject;
		Json::Value st_JsonVideo;
		Json::Value st_JsonAudio;

		st_JsonObject["nClientCount"] = (*pppSt_PublishInfo)[i]->nClientCount;
		st_JsonObject["tszSMSAddr"] = (*pppSt_PublishInfo)[i]->tszSMSAddr;
		st_JsonObject["enStreamType"] = (*pppSt_PublishInfo)[i]->enStreamType;

		st_JsonVideo["bEnable"] = (*pppSt_PublishInfo)[i]->st_AVInfo.st_VideoInfo.bEnable;
		if ((*pppSt_PublishInfo)[i]->st_AVInfo.st_VideoInfo.bEnable)
		{
			st_JsonVideo["nBitRate"] = (Json::Value::UInt64)(*pppSt_PublishInfo)[i]->st_AVInfo.st_VideoInfo.nBitRate;
			st_JsonVideo["enAVCodec"] = (*pppSt_PublishInfo)[i]->st_AVInfo.st_VideoInfo.enAVCodec;
			st_JsonVideo["nWidth"] = (*pppSt_PublishInfo)[i]->st_AVInfo.st_VideoInfo.nWidth;
			st_JsonVideo["nHeight"] = (*pppSt_PublishInfo)[i]->st_AVInfo.st_VideoInfo.nHeight;
			st_JsonVideo["nFrameRate"] = (*pppSt_PublishInfo)[i]->st_AVInfo.st_VideoInfo.nFrameRate;
		}
		st_JsonAudio["bEnable"] = (*pppSt_PublishInfo)[i]->st_AVInfo.st_AudioInfo.bEnable;
		if ((*pppSt_PublishInfo)[i]->st_AVInfo.st_AudioInfo.bEnable)
		{
			st_JsonAudio["nBitRate"] = (Json::Value::UInt64)(*pppSt_PublishInfo)[i]->st_AVInfo.st_AudioInfo.nBitRate;
			st_JsonAudio["enAVCodec"] = (*pppSt_PublishInfo)[i]->st_AVInfo.st_AudioInfo.enAVCodec;
			st_JsonAudio["nChannel"] = (*pppSt_PublishInfo)[i]->st_AVInfo.st_AudioInfo.nChannel;
			st_JsonAudio["nSampleRate"] = (*pppSt_PublishInfo)[i]->st_AVInfo.st_AudioInfo.nSampleRate;
			st_JsonAudio["nSampleFmt"] = (*pppSt_PublishInfo)[i]->st_AVInfo.st_AudioInfo.nSampleFmt;
		}
		st_JsonObject["st_VideoInfo"] = st_JsonVideo;
		st_JsonObject["st_AudioInfo"] = st_JsonAudio;
		st_JsonArray.append(st_JsonObject);
	}
	st_JsonRoot["Array"] = st_JsonArray;

	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), st_JsonRoot.toStyledString().length());
	return true;
}
/********************************************************************
函数名称：ModuleProtocol_Packet_HTTPPullGet
函数功能：获取拉流的信息
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
 参数.三：pppSt_PublishInfo
  In/Out：In
  类型：数据结构指针
  可空：N
  意思：输入拉流客户端数据
 参数.四：nListCount
  In/Out：In
  类型：整数型
  可空：N
  意思：输入数据个数
返回值
  类型：逻辑型
  意思：是否成功
备注：
*********************************************************************/
bool CModuleProtocol_Packet::ModuleProtocol_Packet_HTTPPullGet(XCHAR* ptszMsgBuffer, int* pInt_MsgLen, STREAMMEDIA_PULLLISTINFO*** pppSt_PullInfo, int nListCount)
{
	ModuleProtocol_IsErrorOccur = false;

	if ((NULL == ptszMsgBuffer) || (NULL == pInt_MsgLen))
	{
		ModuleProtocol_IsErrorOccur = true;
		ModuleProtocol_dwErrorCode = ERROR_MODULE_PROTOCOL_PACKET_PARAMENT;
		return false;
	}
	Json::Value st_JsonRoot;
	Json::Value st_JsonArray;

	st_JsonRoot["code"] = 0;
	st_JsonRoot["msg"] = "success";

	for (int i = 0; i < nListCount; i++)
	{
		Json::Value st_JsonObject;

		st_JsonObject["tszPushAddr"] = (*pppSt_PullInfo)[i]->tszPushAddr;
		st_JsonObject["tszSMSAddr"] = (*pppSt_PullInfo)[i]->tszSMSAddr;
		st_JsonObject["enStreamType"] = (*pppSt_PullInfo)[i]->enStreamType;
		st_JsonArray.append(st_JsonObject);
	}
	st_JsonRoot["Array"] = st_JsonArray;

	*pInt_MsgLen = st_JsonRoot.toStyledString().length();
	memcpy(ptszMsgBuffer, st_JsonRoot.toStyledString().c_str(), st_JsonRoot.toStyledString().length());
	return true;
}