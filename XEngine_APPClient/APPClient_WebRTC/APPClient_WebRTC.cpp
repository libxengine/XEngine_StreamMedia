#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _MSC_BUILD
#include <windows.h>
#include <tchar.h>
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_Client/XClient_Socket")
#pragma comment(lib,"XEngine_Client/XClient_OPenSsl")
#pragma comment(lib,"XEngine_Client/XClient_APIHelp")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_SDPProtocol")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_RTPProtocol")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVHelp")
#pragma comment(lib,"XEngine_RfcComponents/RfcComponents_NatProtocol")
#pragma comment(lib,"Ws2_32")
#endif
#include <list>
#include <thread>
#include <memory>
#include <srtp2/srtp.h>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Define.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_Client/APIClient_Define.h>
#include <XEngine_Include/XEngine_Client/APIClient_Error.h>
#include <XEngine_Include/XEngine_Client/SslClient_Define.h>
#include <XEngine_Include/XEngine_Client/SslClient_Error.h>
#include <XEngine_Include/XEngine_StreamMedia/SDPProtocol_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/SDPProtocol_Error.h>
#include <XEngine_Include/XEngine_StreamMedia/RTPProtocol_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/RTPProtocol_Error.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Error.h>
#include <XEngine_Include/XEngine_RfcComponents/NatProtocol_Define.h>
#include <XEngine_Include/XEngine_RfcComponents/NatProtocol_Error.h>
#include "../../XEngine_Source/XEngine_UserProtocol.h"
using namespace std;

typedef struct
{
	srtp_t st_SRTPSendCtx;
	srtp_t st_SRTPRecvCtx;
}SRTPCORE_CLIENTINFO;

std::string m_ClientKey;
std::string m_ServerKey;

bool APPClient_WEBRTC_SDPPacket(LPCXSTR lpszAPIUrl, LPCXSTR lpszFileCert, XCHAR* ptszSDPPacket, int* pInt_SDPLen)
{
	XNETHANDLE xhToken = 0;

	SDPProtocol_Packet_Create(&xhToken);
	SDPProtocol_Packet_Owner(xhToken, _X("-"), 123456789, _X("127.0.0.1"));
	SDPProtocol_Packet_Session(xhToken, _X("live/stream"));
	SDPProtocol_Packet_KeepTime(xhToken);
	SDPProtocol_Packet_Bundle(xhToken, 0, -1);

	SDPProtocol_Packet_OptionalAddAttr(xhToken, _X("extmap-allow-mixed"));
	SDPProtocol_Packet_OptionalAddAttr(xhToken, _X("msid-semantic"), _X(" WMS"));
	STREAMMEDIA_SDPPROTOCOL_MEDIAINFO st_SDPMedia = {};
	//AUDIO
	
	int nAVCount = 1;
	XCHAR** pptszAVList;
	BaseLib_OperatorMemory_Malloc((XPPPMEM)&pptszAVList, nAVCount, 64);/*
	_xstprintf(pptszAVList[0], _X("111"));

	SDPProtocol_Packet_AddMedia(xhToken, _X("audio"), _X("UDP/TLS/RTP/SAVPF"), &pptszAVList, nAVCount, 0, 9);
	SDPProtocol_Packet_ClientInet(xhToken, _X("0.0.0.0"));
	SDPProtocol_Packet_RtcpInet(xhToken, 9);
	SDPProtocol_Packet_ICEUser(xhToken, _X("nzWE"), _X("xk/FvO+TXrJy6739etI/y0Kc"), true);
	SDPProtocol_Packet_OptionalAddAttr(xhToken, _X("fingerprint"), _X("sha-256 E6:29:09:F9:C5:C7:A0:EA:0B:71:AC:F9:4C:D7:78:79:CC:35:DE:C3:90:45:25:EB:79:8F:14:0A:95:BC:30:B5"));
	SDPProtocol_Packet_OptionalAddAttr(xhToken, _X("setup"), _X("actpass"));
	SDPProtocol_Packet_OnlyRWFlag(xhToken);
	SDPProtocol_Packet_RtcpComm(xhToken, true, false);

	st_SDPMedia.st_RTPMap.nChannel = 2;
	st_SDPMedia.st_RTPMap.nSampleRate = 48000;
	st_SDPMedia.st_FmtpAudio.nMinPTime = 10;
	st_SDPMedia.st_FmtpAudio.nUseINBandfec = 1;
	_xstprintf(st_SDPMedia.st_RTPMap.tszCodecName, _X("opus"));
	st_SDPMedia.bTransportCC = true;
	SDPProtocol_Packet_AudioFmt(xhToken, 111, &st_SDPMedia, true);
	*/
	//VIDEO
	_xstprintf(pptszAVList[0], _X("106"));
	SDPProtocol_Packet_AddMedia(xhToken, _X("video"), _X("UDP/TLS/RTP/SAVPF"), &pptszAVList, nAVCount, 0, 9);
	SDPProtocol_Packet_ClientInet(xhToken, _X("0.0.0.0"));
	SDPProtocol_Packet_RtcpInet(xhToken, 9);
	SDPProtocol_Packet_ICEUser(xhToken, _X("nzWE"), _X("xk/FvO+TXrJy6739etI/y0Kc"), true);
	SDPProtocol_Packet_OptionalAddAttr(xhToken, _X("fingerprint"), _X("sha-256 E6:29:09:F9:C5:C7:A0:EA:0B:71:AC:F9:4C:D7:78:79:CC:35:DE:C3:90:45:25:EB:79:8F:14:0A:95:BC:30:B5"));
	SDPProtocol_Packet_OptionalAddAttr(xhToken, _X("setup"), _X("actpass"));
	SDPProtocol_Packet_OnlyRWFlag(xhToken);
	SDPProtocol_Packet_RtcpComm(xhToken, true, true);

	memset(&st_SDPMedia, '\0', sizeof(STREAMMEDIA_SDPPROTOCOL_MEDIAINFO));
	st_SDPMedia.st_RTPMap.nSampleRate = 90000;
	_xstprintf(st_SDPMedia.st_RTPMap.tszCodecName, _X("H264"));
	st_SDPMedia.bGoogRemb = true;
	st_SDPMedia.bTransportCC = true;
	st_SDPMedia.bCcmFir = true;
	st_SDPMedia.bNAck = true;
	st_SDPMedia.bNAckPli = true;
	st_SDPMedia.st_FmtpVideo.bLevelAllow = true;
	st_SDPMedia.st_FmtpVideo.nPacketMode = 1;
	st_SDPMedia.st_FmtpVideo.tszLeaveId[0] = 0x42;
	st_SDPMedia.st_FmtpVideo.tszLeaveId[1] = 0xe0;
	st_SDPMedia.st_FmtpVideo.tszLeaveId[2] = 0x1f;
	SDPProtocol_Packet_VideoFmt(xhToken, 106, &st_SDPMedia, true);

	SDPProtocol_Packet_GetPacket(xhToken, ptszSDPPacket, pInt_SDPLen);
	SDPProtocol_Packet_Destory(xhToken);
	return true;
}
bool APPClient_WEBRTC_SDPParse(LPCXSTR lpszMSGBuffer, int nMSGLen, XCHAR* ptszICEUser, XCHAR* ptszICEPass)
{
	XNETHANDLE xhParse = 0;
	
	if (!SDPProtocol_Parse_Create(&xhParse, lpszMSGBuffer, nMSGLen))
	{
		return false;
	}
	int nAttrCount = 0;
	STREAMMEDIA_SDPPROTOCOL_ATTR** ppSt_ListAttr;
	SDPProtocol_Parse_GetAttr(xhParse, &ppSt_ListAttr, &nAttrCount);

	int nListCount = 0;
	STREAMMEDIA_SDPPROTOCOL_AVMEDIA** ppSt_AVMedia;
	SDPProtocol_Parse_GetAVMedia(xhParse, &ppSt_AVMedia, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		STREAMMEDIA_SDPPROTOCOL_MEDIAINFO st_SDPMeida = {};
		if (0 == _tcsxnicmp(ppSt_AVMedia[i]->tszAVType, "audio", 5))
		{
			SDPProtocol_Parse_RTPMapAudio(&ppSt_ListAttr, nAttrCount, ppSt_AVMedia[i]->nCodecNumber, &st_SDPMeida);
		}
		else
		{
			SDPProtocol_Parse_RTPMapVideo(&ppSt_ListAttr, nAttrCount, ppSt_AVMedia[i]->nCodecNumber, &st_SDPMeida);
		}
		printf("%s:\n", ppSt_AVMedia[i]->tszAVType);
	}
	int nAVIndex1 = 0;
	int nAVIndex2 = 0;
	SDPProtocol_Parse_AttrBundle(&ppSt_ListAttr, nAttrCount, &nAVIndex1, &nAVIndex2);
	SDPProtocol_Parse_AttrICEUser(&ppSt_ListAttr, nAttrCount, ptszICEUser, ptszICEPass);

	XCHAR tszAlgorithmKey[MAX_PATH] = {};
	XCHAR tszAlgorithmVlu[MAX_PATH] = {};
	SDPProtocol_Parse_AttrFinger(&ppSt_ListAttr, nAttrCount, tszAlgorithmKey, tszAlgorithmVlu);

	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_AVMedia, nListCount);
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListAttr, nAttrCount);
	SDPProtocol_Parse_Destory(xhParse);
	return true;
}
bool APPClient_WEBRTC_StunSend(XSOCKET hSocket, LPCXSTR lpszICEUser, LPCXSTR lpszICEPass)
{
	int nMSGLen = 0;
	XCHAR tszMSGBuffer[1024] = {};
	XCHAR tszICEUser[128] = {};
	XCHAR tszTmpBuffer[128] = {};
	XCHAR tszTokenStr[64] = {};
	XCHAR tszRandomStr[10] = {};

	_xstprintf(tszICEUser, _X("%s:nzWE"), lpszICEUser);
	BaseLib_OperatorHandle_CreateStr(tszRandomStr, 8, 0, 2);
	NatProtocol_StunNat_BuildAttr(tszTmpBuffer, &nMSGLen, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_USERNAME, tszICEUser, 13);
	NatProtocol_StunNat_BuildAttr(tszTmpBuffer + nMSGLen, &nMSGLen, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_ICE_CONTROLLING, tszRandomStr, 8);
	NatProtocol_StunNat_BuildPriority(tszTmpBuffer + nMSGLen, &nMSGLen);

	BaseLib_OperatorHandle_CreateStr(tszTokenStr, 12, 0, 2);
	NatProtocol_StunNat_Packet(tszMSGBuffer, &nMSGLen, tszTokenStr, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_CLASS_REQUEST, RFCCOMPONENTS_NATCLIENT_PROTOCOL_STUN_ATTR_MAPPED_ADDRESS, tszTmpBuffer, true, lpszICEPass, true);

	while (true)
	{
		if (!XClient_UDPSelect_SendMsg(hSocket, tszMSGBuffer, nMSGLen))
		{
			return false;
		}
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
	
	return true;
}

bool PullStream_ClientProtocol_Dtls(LPCXSTR lpszMSGBuffer, int nMSGLen)
{
	// DTLS有可能以多种不同的记录层类型开头，这里检查它是否是handshake(0x16)
	return ((nMSGLen >= 13) && (lpszMSGBuffer[0] == 0x16));
}
bool PullStream_ClientProtocol_Stun(LPCXSTR lpszMSGBuffer, int nMSGLen)
{
	// STUN消息的类型字段（前两位为00）以及魔术cookie字段
	return (nMSGLen >= 20) && ((lpszMSGBuffer[0] & 0xC0) == 0x00) && (lpszMSGBuffer[4] == 0x21) && (lpszMSGBuffer[5] == 0x12) && ((XBYTE)lpszMSGBuffer[6] == 0xA4) && (lpszMSGBuffer[7] == 0x42);
}
bool APPClient_WEBRTC_SRTPCreate(LPCXBTR lpszKEYBuffer, SRTPCORE_CLIENTINFO* pSt_SRTPCore)
{
	int nPos = 0;
	const int SRTP_MASTER_KEY_KEY_LEN = 16;
	const int SRTP_MASTER_KEY_SALT_LEN = 14;
	srtp_policy_t st_SRTPPolicy = {};

	std::string m_StrClientKey(reinterpret_cast<LPCXSTR>(lpszKEYBuffer), SRTP_MASTER_KEY_KEY_LEN);
	nPos += SRTP_MASTER_KEY_KEY_LEN;
	std::string m_StrServerKey(reinterpret_cast<LPCXSTR>(lpszKEYBuffer + nPos), SRTP_MASTER_KEY_KEY_LEN);
	nPos += SRTP_MASTER_KEY_KEY_LEN;
	std::string m_StrClientSalt(reinterpret_cast<LPCXSTR>(lpszKEYBuffer + nPos), SRTP_MASTER_KEY_SALT_LEN);
	nPos += SRTP_MASTER_KEY_SALT_LEN;
	std::string m_StrServerSalt(reinterpret_cast<LPCXSTR>(lpszKEYBuffer + nPos), SRTP_MASTER_KEY_SALT_LEN);

	m_ClientKey = m_StrClientKey + m_StrClientSalt;
	m_ServerKey = m_StrServerKey + m_StrServerSalt;

	srtp_crypto_policy_set_aes_cm_128_hmac_sha1_80(&st_SRTPPolicy.rtp);
	srtp_crypto_policy_set_aes_cm_128_hmac_sha1_80(&st_SRTPPolicy.rtcp);

	st_SRTPPolicy.ssrc.type = ssrc_any_inbound;
	st_SRTPPolicy.key = (unsigned char*)m_ServerKey.c_str();

	srtp_err_status_t nRet = srtp_err_status_ok;
	if (srtp_err_status_ok != (nRet = srtp_create(&pSt_SRTPCore->st_SRTPRecvCtx, &st_SRTPPolicy)))
	{
		return false;
	}
	return true;
}
bool APPClient_WEBRTC_Dlts(XSOCKET hSocket)
{
	LPCXSTR lpszRTPClient = _X("client");
	LPCXSTR lpszCertFile = _X("D:\\XEngine_StreamMedia\\XEngine_APPClient\\Debug\\certificate.crt");
	LPCXSTR lpszPrivateFile = _X("D:\\XEngine_StreamMedia\\XEngine_APPClient\\Debug\\private.key");

	XHANDLE xhSsl = XClient_OPenSsl_InitEx(ENUM_XCLIENT_SSL_TYPE_DTL_VERSION, false, lpszCertFile, NULL, lpszPrivateFile);
	if (NULL == xhSsl)
	{
		return false;
	}
	RTPProtocol_Parse_Init(1);
	RTPProtocol_Parse_Insert(lpszRTPClient);

	XClient_OPenSsl_ConfigEx(xhSsl);

	XCLIENT_SSLCERT_SRVINFO st_SslInfo = {};
	SRTPCORE_CLIENTINFO st_SRTPInfo = {};
	XClient_OPenSsl_ConnectEx(xhSsl, hSocket, &st_SslInfo);

	XBYTE byKEYBuffer[128] = {};
	XClient_OPenSsl_GetKeyEx(xhSsl, byKEYBuffer);
	for (int i = 0; i < 60; i++)
	{
		printf("0x%02X, ", byKEYBuffer[i]);
	}
	printf("\n");

	XCHAR tszRTPFile[MAX_PATH] = {};
	XCHAR tsz264File[MAX_PATH] = {};
	XCHAR tszFSizeFile[MAX_PATH] = {};

	_xstprintf(tsz264File, _X("D:\\XEngine_StreamMedia\\XEngine_APPClient\\Debug\\recv.h264"));
	_xstprintf(tszRTPFile, _X("D:\\XEngine_StreamMedia\\XEngine_APPClient\\Debug\\recv.rtp"));
	_xstprintf(tszFSizeFile, _X("D:\\XEngine_StreamMedia\\XEngine_APPClient\\Debug\\1.txt"));
	FILE* pSt_264File = _xtfopen(tsz264File, "wb");
	FILE* pSt_RTPFile = _xtfopen(tszRTPFile, "wb");
	FILE* pSt_FSFile = _xtfopen(tszFSizeFile, "wb");

	bool bKEYFrame = false;
	XCHAR* ptszRTPBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);
	APPClient_WEBRTC_SRTPCreate(byKEYBuffer, &st_SRTPInfo);
	while (true)
	{
		int nMSGLen = 2048;
		XCHAR tszMSGBuffer[2048] = {};
		if (XClient_UDPSelect_RecvMsg(hSocket, tszMSGBuffer, &nMSGLen))
		{
			if (PullStream_ClientProtocol_Dtls(tszMSGBuffer, nMSGLen))
			{
				printf("dtls protcol recved\n");
			}
			else if (PullStream_ClientProtocol_Stun(tszMSGBuffer, nMSGLen))
			{
				printf("stun protocol recved\n");
			}
			else
			{
				int nRet = srtp_unprotect(st_SRTPInfo.st_SRTPRecvCtx, tszMSGBuffer, &nMSGLen);
				if (srtp_err_status_ok == nRet)
				{
					//char tszFSizeBuffer[64] = {};
					//int nFSize = sprintf(tszFSizeBuffer, "%d\r\n", nMSGLen);
					//fwrite(tszFSizeBuffer, 1, nFSize, pSt_FSFile);
					//fwrite(tszMSGBuffer, 1, nMSGLen, pSt_RTPFile);

					RTPProtocol_Parse_Send(lpszRTPClient, tszMSGBuffer, nMSGLen);
					while (true)
					{
						int nRTPLen = XENGINE_MEMORY_SIZE_MAX;
						STREAMMEDIA_RTPPROTOCOL_HDR st_RTPHdr = {};
						if (!RTPProtocol_Parse_Recv(lpszRTPClient, ptszRTPBuffer, &nRTPLen, &st_RTPHdr))
						{
							break;
						}
						if (106 == st_RTPHdr.enPayload)
						{

						}

						if (!bKEYFrame)
						{
							XENGINE_AVCODEC_VIDEOFRAMETYPE enFrameType;
							AVHelp_Parse_NaluType(ptszRTPBuffer, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264, &enFrameType);
							if ((ENUM_XENGINE_AVCODEC_VIDEO_FRAMETYPE_SPS == enFrameType) || (ENUM_XENGINE_AVCODEC_VIDEO_FRAMETYPE_PPS == enFrameType))
							{
								bKEYFrame = true;
								printf("ok\n");
							}
						}
						
						if (bKEYFrame)
						{
							fwrite(ptszRTPBuffer, 1, nRTPLen, pSt_264File);
						}
					}
				}
				else
				{
					printf("srtp protcol recved unprotocol failed:%d\n", nRet);
				}
			}
		}
		else
		{
			printf("Recv error:%lX\n", XClient_GetLastError());
		}
	}
	free(ptszRTPBuffer);
	ptszRTPBuffer = NULL;
	fclose(pSt_264File);
	XClient_OPenSsl_CloseEx(xhSsl);
	RTPProtocol_Parse_Destory();
	return true;
}

int main()
{
	srtp_init();
	int nMSGLen = 0;
	XSOCKET hSocket;
	XCHAR tszMSGBuffer[2048] = {};
	LPCXSTR lpszAPIUrl = _X("http://127.0.0.1:5600/rtc/v1/whep/?app=live&stream=livestream");
	//LPCXSTR lpszAPIUrl = _X("http://app.xyry.org:1985/rtc/v1/whep/?app=live&stream=livestream");
	//LPCXSTR lpszAPIUrl = _X("http://10.0.3.154:1985/rtc/v1/whep/?app=live&stream=livestream");
	LPCXSTR lpszFileCert = _X("");

	APPClient_WEBRTC_SDPPacket(lpszAPIUrl, lpszFileCert, tszMSGBuffer, &nMSGLen);

	int nHTTPCode = 0;
	XCHAR* ptszMSGBuffer = NULL;
	if (!APIClient_Http_Request(_X("POST"), lpszAPIUrl, tszMSGBuffer, &nHTTPCode, &ptszMSGBuffer, &nMSGLen))
	{
		printf("APIClient_Http_Request:%lX\n", APIClient_GetLastError());
		return -1;
	}
	XCHAR tszICEUser[128] = {};
	XCHAR tszICEPass[128] = {};
	APPClient_WEBRTC_SDPParse(ptszMSGBuffer, nMSGLen, tszICEUser, tszICEPass);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMSGBuffer);

	XClient_UDPSelect_Create(&hSocket);
	//XClient_UDPSelect_Connect(hSocket, "10.0.3.154", 8000);
	//XClient_UDPSelect_Connect(hSocket, "43.139.170.67", 8000);
	XClient_UDPSelect_Connect(hSocket, "127.0.0.1", 5604);

	std::thread pSTDThread_Stun(APPClient_WEBRTC_StunSend, hSocket, tszICEUser, tszICEPass);
	pSTDThread_Stun.detach();

	std::thread pSTDThread_Dtls(APPClient_WEBRTC_Dlts, hSocket);
	pSTDThread_Dtls.detach();

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	XClient_UDPSelect_Close(hSocket);
	srtp_shutdown();
	return 0;
}