#pragma once
#include <string>
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
typedef std::string xstring;
/*********************************************************************
					    客户端类型定义
*********************************************************************/
typedef enum
{
	ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_HTTP = 0x00,
	ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_XSTREAM = 0x01,
	ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_JT1078 = 0x02,
	ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_RTMP = 0x03,
	ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PUSH_SRT = 0x04,
	ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_XSTREAM = 0x10,
	ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_FLV = 0x11,
	ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_RTMP = 0x12,
	ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE_PULL_SRT = 0x13
}ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE;
/*********************************************************************
						操作类型定义
*********************************************************************/
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
	XCHAR tszSMSAddr[MAX_PATH];
}XENGINE_PROTOCOLSTREAM;
//////////////////////////////////////////////////////////////////////////设备协议RTP
typedef struct
{
	XBYTE byFlags[4];

	XBYTE byCC : 4;
	XBYTE byX : 1;
	XBYTE byP : 1;
	XBYTE byV : 2;

	XBYTE byPT : 7;
	XBYTE byM : 1;

	XSHOT wSerial;
	XBYTE bySIMNumber[6];
	XBYTE byChannel;

	XBYTE byPacket : 4;
	XBYTE byType : 4;
	__int64u ullTimestamp;
}XENGINE_RTPPACKETHDR2016;
typedef struct
{
	XBYTE byV : 2;
	XBYTE byP : 1;
	XBYTE byX : 1;
	XBYTE byCC : 4;

	XBYTE byM : 1;
	XBYTE byPT : 7;

	XSHOT wSerial;
	XBYTE bySIMNumber[6];
	XBYTE byChannel;

	XBYTE byType : 4;
	XBYTE byPacket : 4;
	__int64u ullTimestamp;
	XSHOT wLen;
}XENGINE_RTPPACKETHDR2014;

typedef struct
{
	XSHOT wLastIFrame;
	XSHOT wLastPBFrame;
}XENGINE_RTPPACKETTAIL;
typedef struct
{
	__int64u nTimeStamp;                                          //时间戳 
	int nFrameSize;                                               //帧大小
	XBYTE byAVType;                                               //音视频类型,0视频,1音频
	XBYTE byFrameType;                                            //帧类型,1关键帧,否则为其他
}XENGINE_PROTOCOL_AVDATA;
#pragma pack(pop)