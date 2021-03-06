#pragma once
/********************************************************************
//    Created:     2021/12/03  14:16:27
//    File Name:   D:\XEngine_ServiceApp\XEngine_Source\XEngine_UserProtocol.h
//    File Path:   D:\XEngine_ServiceApp\XEngine_Source
//    File Base:   XEngine_UserProtocol
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     用户自定义协议头
//    History:
*********************************************************************/
/*********************************************************************
						操作类型定义
*********************************************************************/
typedef enum
{
	ENUM_XENGINE_STREAMMEDIA_JT1078_RTP_FRAME_I = 0x00,
	ENUM_XENGINE_STREAMMEDIA_JT1078_RTP_FRAME_P = 0x01,
	ENUM_XENGINE_STREAMMEDIA_JT1078_RTP_FRAME_B = 0x02,
	ENUM_XENGINE_STREAMMEDIA_JT1078_RTP_FRAME_A = 0x03,
	ENUM_XENGINE_STREAMMEDIA_JT1078_RTP_FRAME_T = 0x04
}ENUM_XENGINE_STREAMMEDIA_JT1078_RTP_FRAME_TYPE;
typedef enum
{
	ENUM_XENGINE_STREAMMEDIA_JT1078_DEVICE_TYPE_UNKNOW = 0,
	ENUM_XENGINE_STREAMMEDIA_JT1078_DEVICE_TYPE_2014 = 1,
	ENUM_XENGINE_STREAMMEDIA_JT1078_DEVICE_TYPE_2016 = 2
}ENUM_XENGINE_STREAMMEDIA_JT1078_DEVICE_TYPE;
//////////////////////////////////////////////////////////////////////////
//                          主协议定义;unOperatorType
//////////////////////////////////////////////////////////////////////////
typedef enum 
{
	//用户消息
	ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_MSG = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_USER + 1
}ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_CUSTOM;
///////////////////////////////////////////////////////////////////////////
//                          子协议定义:unOperatorCode
///////////////////////////////////////////////////////////////////////////
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REQCREATE 0x3001  //创建流
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REPCREATE 0x3002 
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REQPUSH 0x3003    //请求推送
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REPPUSH 0x3004   
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REQDESTROY 0x3005 //请求销毁流
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REPDESTROY 0x3006
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REQPULL 0x3007    //请求拉流
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REPPULL 0x3008      
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_STATPUSH 0x3009   //推流端状态推送协议
#define XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_STATPULL 0x300A   //拉流端状态推送协议
///////////////////////////////////////////////////////////////////////////
//                          导出的数据结构
///////////////////////////////////////////////////////////////////////////
#pragma pack(push)
#pragma pack(1)
//添加自己的消息数据结构
typedef struct
{
	XENGINE_PROTOCOL_AVINFO st_AVInfo;
	TCHAR tszDeviceNumber[64];
}XENGINE_PROTOCOLSTREAM;
typedef struct
{
	TCHAR tszDeviceNumber[64];
	int nChannel;
	BOOL bLive;
}XENGINE_PROTOCOLDEVICE;
//////////////////////////////////////////////////////////////////////////设备协议RTP
typedef struct
{
	BYTE byFlags[4];

	BYTE byCC : 4;
	BYTE byX : 1;
	BYTE byP : 1;
	BYTE byV : 2;

	BYTE byPT : 7;
	BYTE byM : 1;

	WORD wSerial;
	BYTE bySIMNumber[6];
	BYTE byChannel;

	BYTE byPacket : 4;
	BYTE byType : 4;
	__int64u ullTimestamp;
	WORD wLen;
}XENGINE_RTPPACKETHDR2016;
typedef struct
{
	BYTE byV : 2;
	BYTE byP : 1;
	BYTE byX : 1;
	BYTE byCC : 4;

	BYTE byM : 1;
	BYTE byPT : 7;

	WORD wSerial;
	BYTE bySIMNumber[6];
	BYTE byChannel;

	BYTE byType : 4;
	BYTE byPacket : 4;
	__int64u ullTimestamp;
	WORD wLen;
}XENGINE_RTPPACKETHDR2014;

typedef struct
{
	WORD wLastIFrame;
	WORD wLastPBFrame;
}XENGINE_RTPPACKETTAIL;
#pragma pack(pop)