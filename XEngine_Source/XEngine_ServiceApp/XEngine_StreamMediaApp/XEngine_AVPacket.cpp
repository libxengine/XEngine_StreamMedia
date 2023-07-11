#include "XEngine_Hdr.h"
/********************************************************************
//    Created:     2023/07/10  16:58:28
//    File Name:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp\XEngine_AVPacket.cpp
//    File Path:   D:\XEngine_StreamMedia\XEngine_Source\XEngine_ServiceApp\XEngine_StreamMediaApp
//    File Base:   XEngine_AVPacket
//    File Ext:    cpp
//    Project:     XEngine(网络通信引擎)
//    Author:      qyt
//    Purpose:     bool XEngine_AVPacket_AVHdr()
//    History:
*********************************************************************/
bool XEngine_AVPacket_AVCreate(LPCXSTR lpszClientAddr)
{
	if (st_ServiceConfig.st_XPull.st_PullFlv.bEnable)
	{
		FLVProtocol_Packet_Insert(lpszClientAddr);
	}
	if (st_ServiceConfig.st_XPull.st_PullRtmp.bEnable)
	{
		RTMPProtocol_Packet_Insert(lpszClientAddr);
	}
	return true;
}
bool XEngine_AVPacket_AVDelete(LPCXSTR lpszClientAddr)
{
	if (st_ServiceConfig.st_XPull.st_PullFlv.bEnable)
	{
		FLVProtocol_Packet_Delete(lpszClientAddr);
	}
	if (st_ServiceConfig.st_XPull.st_PullRtmp.bEnable)
	{
		RTMPProtocol_Packet_Delete(lpszClientAddr);
	}
	return true;
}
bool XEngine_AVPacket_AVSetTime(LPCXSTR lpszClientAddr, int nVideoParament, int nAudioParament)
{
	if (st_ServiceConfig.st_XPull.st_PullFlv.bEnable)
	{
		FLVProtocol_Packet_SetTime(lpszClientAddr, nVideoParament, nAudioParament);
	}
	if (st_ServiceConfig.st_XPull.st_PullRtmp.bEnable)
	{
		RTMPProtocol_Packet_SetTime(lpszClientAddr, nVideoParament, nAudioParament);
	}
	return true;
}
bool XEngine_AVPacket_AVHdr(LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XBYTE byAVType)
{
	XENGINE_PROTOCOL_AVINFO st_AVInfo;
	ModuleSession_PushStream_GetAVInfo(lpszClientAddr, &st_AVInfo);

	if (st_ServiceConfig.st_XPull.st_PullFlv.bEnable)
	{
		if (0 == byAVType)
		{
			//视频参数信息是否存在
			if (0 == st_AVInfo.st_VideoInfo.nVLen)
			{
				st_AVInfo.st_VideoInfo.nVLen = nMsgLen;
				memcpy(st_AVInfo.st_VideoInfo.tszVInfo, lpszMsgBuffer, nMsgLen);
				ModuleSession_PushStream_SetAVInfo(lpszClientAddr, &st_AVInfo);
			}
			//是否全部设置完毕
			if (st_AVInfo.st_AudioInfo.nALen > 0 && st_AVInfo.st_VideoInfo.nVLen > 0)
			{
				//配置头
				int nHLen = 0;
				int nPLen = 0;
				XCHAR tszMsgBuffer[1024];
				memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

				FLVProtocol_Packet_FrameHdr(lpszClientAddr, tszMsgBuffer, &nPLen);
				nHLen += nPLen;
				FLVProtocol_Packet_FrameScript(lpszClientAddr, tszMsgBuffer + nHLen, &nPLen, &st_AVInfo);
				nHLen += nPLen;
				FLVProtocol_Packet_FrameCustom(lpszClientAddr, tszMsgBuffer + nHLen, &nPLen, st_AVInfo.st_VideoInfo.tszVInfo, st_AVInfo.st_VideoInfo.nVLen, 0, 9);
				nHLen += nPLen;
				FLVProtocol_Packet_FrameCustom(lpszClientAddr, tszMsgBuffer + nHLen, &nPLen, st_AVInfo.st_AudioInfo.tszAInfo, st_AVInfo.st_AudioInfo.nALen, 0, 8);
				nHLen += nPLen;

				ModuleSession_PushStream_SetHDRBuffer(lpszClientAddr, tszMsgBuffer, nHLen);
			}
		}
		else
		{
			//音频参数信息是否存在
			if (0 == st_AVInfo.st_AudioInfo.nALen)
			{
				st_AVInfo.st_AudioInfo.nALen = nMsgLen;
				memcpy(st_AVInfo.st_AudioInfo.tszAInfo, lpszMsgBuffer, nMsgLen);
				ModuleSession_PushStream_SetAVInfo(lpszClientAddr, &st_AVInfo);
			}
			//是否全部设置完毕
			if (st_AVInfo.st_AudioInfo.nALen > 0 && st_AVInfo.st_VideoInfo.nVLen > 0)
			{
				//配置头
				int nHLen = 0;
				int nPLen = 0;
				XCHAR tszMsgBuffer[1024];

				memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

				FLVProtocol_Packet_FrameHdr(lpszClientAddr, tszMsgBuffer, &nPLen);
				nHLen += nPLen;
				FLVProtocol_Packet_FrameScript(lpszClientAddr, tszMsgBuffer + nHLen, &nPLen, &st_AVInfo);
				nHLen += nPLen;
				FLVProtocol_Packet_FrameCustom(lpszClientAddr, tszMsgBuffer + nHLen, &nPLen, st_AVInfo.st_VideoInfo.tszVInfo, st_AVInfo.st_VideoInfo.nVLen, 0, 9);
				nHLen += nPLen;
				FLVProtocol_Packet_FrameCustom(lpszClientAddr, tszMsgBuffer + nHLen, &nPLen, st_AVInfo.st_AudioInfo.tszAInfo, st_AVInfo.st_AudioInfo.nALen, 0, 8);
				nHLen += nPLen;

				ModuleSession_PushStream_SetHDRBuffer(lpszClientAddr, tszMsgBuffer, nHLen);
			}
		}
	}
	if (st_ServiceConfig.st_XPull.st_PullRtmp.bEnable)
	{
		if (0 == byAVType)
		{
			//视频参数信息是否存在
			if (0 == st_AVInfo.st_VideoInfo.nVLen)
			{
				st_AVInfo.st_VideoInfo.nVLen = nMsgLen;
				memcpy(st_AVInfo.st_VideoInfo.tszVInfo, lpszMsgBuffer, nMsgLen);
				ModuleSession_PushStream_SetAVInfo(lpszClientAddr, &st_AVInfo);
			}
			//是否全部设置完毕
			if (st_AVInfo.st_AudioInfo.nALen > 0 && st_AVInfo.st_VideoInfo.nVLen > 0)
			{
				//配置头
				int nHLen = 0;
				int nPLen = 0;
				XCHAR tszMsgBuffer[1024];
				memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

				RTMPProtocol_Packet_FrameAVScript(tszMsgBuffer + nHLen, &nPLen, &st_AVInfo);
				nHLen += nPLen;
				RTMPProtocol_Packet_FrameAVCConfigure(tszMsgBuffer + nHLen, &nPLen, &st_AVInfo);
				nHLen += nPLen;
				RTMPProtocol_Packet_FrameAACConfigure(tszMsgBuffer + nHLen, &nPLen, &st_AVInfo);
				nHLen += nPLen;

				ModuleSession_PushStream_SetHDRBuffer(lpszClientAddr, tszMsgBuffer, nHLen);
			}
		}
		else
		{
			//音频参数信息是否存在
			if (0 == st_AVInfo.st_AudioInfo.nALen)
			{
				st_AVInfo.st_AudioInfo.nALen = nMsgLen;
				memcpy(st_AVInfo.st_AudioInfo.tszAInfo, lpszMsgBuffer, nMsgLen);
				ModuleSession_PushStream_SetAVInfo(lpszClientAddr, &st_AVInfo);
			}
			//是否全部设置完毕
			if (st_AVInfo.st_AudioInfo.nALen > 0 && st_AVInfo.st_VideoInfo.nVLen > 0)
			{
				//配置头
				int nHLen = 0;
				int nPLen = 0;
				XCHAR tszMsgBuffer[1024];

				memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

				RTMPProtocol_Packet_FrameAVScript(tszMsgBuffer + nHLen, &nPLen, &st_AVInfo);
				nHLen += nPLen;
				RTMPProtocol_Packet_FrameAVCConfigure(tszMsgBuffer + nHLen, &nPLen, &st_AVInfo);
				nHLen += nPLen;
				RTMPProtocol_Packet_FrameAACConfigure(tszMsgBuffer + nHLen, &nPLen, &st_AVInfo);
				nHLen += nPLen;

				ModuleSession_PushStream_SetHDRBuffer(lpszClientAddr, tszMsgBuffer, nHLen);
			}
		}
	}
	return true;
}
bool XEngine_AVPacket_AVFrame(XCHAR* ptszRVBuffer, int* pInt_RVLen, XCHAR* ptszSDBuffer, int* pInt_SDLen, LPCXSTR lpszClientAddr, LPCXSTR lpszMsgBuffer, int nMsgLen, XBYTE byAVType)
{
	if (st_ServiceConfig.st_XPull.st_PullFlv.bEnable)
	{
		if (0 == byAVType)
		{
			FLVProtocol_Packet_FrameCustom(lpszClientAddr, ptszRVBuffer, pInt_RVLen, lpszMsgBuffer, nMsgLen, -1, 9);
			*pInt_SDLen = _xstprintf(ptszSDBuffer, _X("%x\r\n"), *pInt_RVLen);
			memcpy(ptszSDBuffer + *pInt_SDLen, ptszRVBuffer, *pInt_RVLen);
			*pInt_SDLen += *pInt_RVLen;

			memcpy(ptszSDBuffer + *pInt_SDLen, _X("\r\n"), 2);
			*pInt_SDLen += 2;
		}
		else
		{
			FLVProtocol_Packet_FrameCustom(lpszClientAddr, ptszRVBuffer, pInt_RVLen, lpszMsgBuffer, nMsgLen, -1, 8);
			*pInt_SDLen = _xstprintf(ptszSDBuffer, _X("%x\r\n"), *pInt_RVLen);
			memcpy(ptszSDBuffer + *pInt_SDLen, ptszRVBuffer, *pInt_RVLen);
			*pInt_SDLen += *pInt_RVLen;

			memcpy(ptszSDBuffer + *pInt_SDLen, _X("\r\n"), 2);
			*pInt_SDLen += 2;
		}
	}
	if (st_ServiceConfig.st_XPull.st_PullRtmp.bEnable)
	{
		if (0 == byAVType)
		{
			RTMPProtocol_Packet_FrameCustom(lpszClientAddr, ptszRVBuffer, pInt_RVLen, lpszMsgBuffer, nMsgLen, -1, 9);
			*pInt_SDLen = _xstprintf(ptszSDBuffer, _X("%x\r\n"), *pInt_RVLen);
			memcpy(ptszSDBuffer + *pInt_SDLen, ptszRVBuffer, *pInt_RVLen);
			*pInt_SDLen += *pInt_RVLen;

			memcpy(ptszSDBuffer + *pInt_SDLen, _X("\r\n"), 2);
			*pInt_SDLen += 2;
		}
		else
		{
			RTMPProtocol_Packet_FrameCustom(lpszClientAddr, ptszRVBuffer, pInt_RVLen, lpszMsgBuffer, nMsgLen, -1, 8);
			*pInt_SDLen = _xstprintf(ptszSDBuffer, _X("%x\r\n"), *pInt_RVLen);
			memcpy(ptszSDBuffer + *pInt_SDLen, ptszRVBuffer, *pInt_RVLen);
			*pInt_SDLen += *pInt_RVLen;

			memcpy(ptszSDBuffer + *pInt_SDLen, _X("\r\n"), 2);
			*pInt_SDLen += 2;
		}
	}
	return true;
}