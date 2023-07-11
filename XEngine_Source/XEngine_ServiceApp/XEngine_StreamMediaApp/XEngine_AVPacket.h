#pragma once
/********************************************************************
//    Created:     2023/07/10  16:58:04
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\XEngine_AVPacket.h
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp
//    File Base:   XEngine_AVPacket
//    File Ext:    h
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     AV数据包打包
//    History:
*********************************************************************/
typedef struct
{
	ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enMSGType;
	int nMsgLen;
	XCHAR* ptszMsgBuffer;
}AVPACKET_MSGFRAME;

bool XEngine_AVPacket_AVCreate(LPCXSTR lpszClientAddr);
bool XEngine_AVPacket_AVDelete(LPCXSTR lpszClientAddr);
bool XEngine_AVPacket_AVSetTime(LPCXSTR lpszClientAddr, int nVideoParament, int nAudioParament);
bool XEngine_AVPacket_AVHdr(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XBYTE byAVType, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enClientType);
bool XEngine_AVPacket_AVFrame(list<AVPACKET_MSGFRAME>* pStl_ListFrame, XCHAR* ptszRVBuffer, int* pInt_RVLen, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, int nTimeStamp, XBYTE byAVType, ENUM_XENGINE_STREAMMEDIA_CLIENT_TYPE enClientType);