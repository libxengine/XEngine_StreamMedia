#ifdef _MSC_BUILD
#include <windows.h>
#include <tchar.h>
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib")
#pragma comment(lib,"XEngine_Client/XClient_Socket")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVCollect")
#pragma comment(lib,"XEngine_AVCodec/XEngine_VideoCodec")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIClient")
#pragma comment(lib,"Ws2_32")
#endif
#include <list>
#include <thread>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Client/XClient_Define.h>
#include <XEngine_Include/XEngine_Client/XClient_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Error.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIClient_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIClient_Error.h>
#include "../../XEngine_Source/XEngine_UserProtocol.h"
using namespace std;

//需要优先配置XEngine
//WINDOWS使用VS2022 x86 或者 x64 debug 编译
//linux::g++ -std=c++17 -Wall -g APPClient_XStream.cpp -o APPClient_XStream.exe -I ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_NetHelp -L /usr/local/lib/XEngine_Release/XEngine_SystemSdk -L ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -lXEngine_BaseLib -lNetHelp_APIClient -lXEngine_SystemApi -ljsoncpp
//macos::g++ -std=c++17 -Wall -g APPClient_XStream.cpp -o APPClient_XStream.exe -I ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -L ../../XEngine_Source/XEngine_ThirdPart/jsoncpp -lXEngine_BaseLib -lNetHelp_APIClient -lXEngine_SystemApi -ljsoncpp
XSOCKET hSocket = 0;
XNETHANDLE xhVideo = 0;
__int64u nTimeVideo = 0;
FILE* pSt_File = NULL;

void CALLBACK XEngine_AVCollect_CBVideo(uint8_t* punStringY, int nYLen, uint8_t* punStringU, int nULen, uint8_t* punStringV, int nVLen, XPVOID lParam)
{
	XCHAR* ptszMsgBuffer = (XCHAR*)malloc(XENGINE_MEMORY_SIZE_MAX);
	XENGINE_PROTOCOLHDR st_ProtocolHdr;
	XENGINE_PROTOCOL_AVDATA st_ProtocolData;

	memset(ptszMsgBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
	memset(&st_ProtocolData, '\0', sizeof(XENGINE_PROTOCOL_AVDATA));

	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_SMS;
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REQPUSH;
	st_ProtocolHdr.byVersion = 1;
	st_ProtocolHdr.byIsReply = false;
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	int nListCount = 0;
	AVCODEC_VIDEO_MSGBUFFER** ppSt_MSGBuffer;
	VideoCodec_Stream_EnCodec(xhVideo, punStringY, punStringU, punStringV, nYLen, nULen, nVLen, &ppSt_MSGBuffer, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		nTimeVideo += (1000 / 24);
		st_ProtocolData.byAVType = 0;
		st_ProtocolData.nTimeStamp = nTimeVideo;
		st_ProtocolData.byFrameType = ppSt_MSGBuffer[i]->st_VideoInfo.nFrameType;
		st_ProtocolHdr.unPacketSize = sizeof(XENGINE_PROTOCOL_AVDATA) + ppSt_MSGBuffer[i]->nYLen;

		memcpy(ptszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
		memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), &st_ProtocolData, sizeof(XENGINE_PROTOCOL_AVDATA));
		memcpy(ptszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR) + sizeof(XENGINE_PROTOCOL_AVDATA), ppSt_MSGBuffer[i]->ptszYBuffer, ppSt_MSGBuffer[i]->nYLen);
		XClient_TCPSelect_SendMsg(hSocket, ptszMsgBuffer, sizeof(XENGINE_PROTOCOLHDR) + sizeof(XENGINE_PROTOCOL_AVDATA) + ppSt_MSGBuffer[i]->nYLen);
		BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ppSt_MSGBuffer[i]->ptszYBuffer);

		printf("Time:%llu Size:%d Type:%d\n", nTimeVideo, ppSt_MSGBuffer[i]->nYLen, ppSt_MSGBuffer[i]->st_VideoInfo.nFrameType);
	}
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_MSGBuffer, nListCount);
	free(ptszMsgBuffer);
	ptszMsgBuffer = NULL;
}
void CALLBACK XEngine_AVCollect_CBAudio(uint8_t* punStringAudio, int nVLen, XPVOID lParam)
{

}

int XStream_Push()
{
	LPCXSTR lpszServiceAddr = _X("127.0.0.1");
	if (!XClient_TCPSelect_Create(&hSocket, lpszServiceAddr, 5601))
	{
		_xtprintf("连接失败！%ld\n", XClient_GetLastError());
		return 0;
	}
	_xtprintf("连接成功！\n");
	int nLen = 0;
	XCHAR tszMsgBuffer[2048];
	XENGINE_PROTOCOLHDR st_ProtocolHdr;
	XENGINE_PROTOCOLSTREAM st_ProtocolStream;

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
	memset(&st_ProtocolStream, '\0', sizeof(XENGINE_PROTOCOLSTREAM));

	st_ProtocolHdr.wHeader = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_HEADER;
	st_ProtocolHdr.unOperatorType = ENUM_XENGINE_COMMUNICATION_PROTOCOL_TYPE_SMS;
	st_ProtocolHdr.unOperatorCode = XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_SMS_REQCREATE;
	st_ProtocolHdr.byVersion = 1;
	st_ProtocolHdr.byIsReply = true;           //获得处理返回结果
	st_ProtocolHdr.unPacketSize = sizeof(XENGINE_PROTOCOLSTREAM);
	st_ProtocolHdr.wTail = XENGIEN_COMMUNICATION_PACKET_PROTOCOL_TAIL;

	AVCOLLECT_SCREENINFO st_ScreenInfo;
	memset(&st_ScreenInfo, '\0', sizeof(AVCOLLECT_SCREENINFO));

	st_ScreenInfo.nFrameRate = 24;
	_tcsxcpy(st_ScreenInfo.tszVideoSize, "1920x1080");

	XHANDLE xhScreen = AVCollect_Video_Init("gdigrab", "desktop", &st_ScreenInfo, XEngine_AVCollect_CBVideo);
	AVCollect_Video_GetInfo(xhScreen, &st_ProtocolStream.st_AVInfo);
	printf("AVCollect_Screen_GetInfo:%d %d %lld\n", st_ProtocolStream.st_AVInfo.st_VideoInfo.nWidth, st_ProtocolStream.st_AVInfo.st_VideoInfo.nHeight, st_ProtocolStream.st_AVInfo.st_VideoInfo.nBitRate);

	XHANDLE xhAudio = AVCollect_Audio_Init("dshow", "audio=麦克风 (USB microphone)", XEngine_AVCollect_CBAudio);
	AVCollect_Audio_GetInfo(xhAudio, &st_ProtocolStream.st_AVInfo);
	printf("AVCollect_Audio_GetInfo:%d %d %d\n", st_ProtocolStream.st_AVInfo.st_AudioInfo.enAVCodec, st_ProtocolStream.st_AVInfo.st_AudioInfo.nChannel, st_ProtocolStream.st_AVInfo.st_AudioInfo.nSampleRate);

	st_ProtocolStream.st_AVInfo.st_VideoInfo.enAVCodec = ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264;
	if (!VideoCodec_Stream_EnInit(&xhVideo, &st_ProtocolStream.st_AVInfo.st_VideoInfo))
	{
		printf(_X("初始化编码器失败"));
		return -1;
	}
	_tcsxcpy(st_ProtocolStream.tszSMSAddr, _X("live/h265"));

	nLen = sizeof(XENGINE_PROTOCOLHDR) + st_ProtocolHdr.unPacketSize;
	memcpy(tszMsgBuffer, &st_ProtocolHdr, sizeof(XENGINE_PROTOCOLHDR));
	memcpy(tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), &st_ProtocolStream, sizeof(XENGINE_PROTOCOLSTREAM));
	if (!XClient_TCPSelect_SendMsg(hSocket, tszMsgBuffer, nLen))
	{
		_xtprintf("发送投递失败！\n");
		return 0;
	}
	nLen = 0;
	XCHAR* ptszMsgBuffer;
	memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));
	if (!XClient_TCPSelect_RecvPkt(hSocket, &ptszMsgBuffer, &nLen, &st_ProtocolHdr, 10))
	{
		_xtprintf("接受数据失败！\n");
		return 0;
	}
	_xtprintf("%d\n", st_ProtocolHdr.wReserve);
	if (nLen > 0)
	{
		BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	}

	AVCollect_Video_Start(xhScreen);

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}


static void CALLBACK APPClient_XStreamPull_Callback(XNETHANDLE xhToken, XPVOID lpszMsgBuffer, int nMsgLen, XPVOID lParam)
{
	static bool bFirst = true;

	if (bFirst)
	{
		XENGINE_PROTOCOL_AVINFO st_AVInfo;

		memset(&st_AVInfo, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));
		memcpy(&st_AVInfo, lpszMsgBuffer, sizeof(XENGINE_PROTOCOL_AVINFO));

		printf("%d,bEnable:%d,bEnable:%d\n", nMsgLen, st_AVInfo.st_VideoInfo.bEnable, st_AVInfo.st_AudioInfo.bEnable);
		bFirst = false;
	}
	else
	{
		XENGINE_PROTOCOL_AVDATA st_ProtocolAVData;
		memset(&st_ProtocolAVData, '\0', sizeof(XENGINE_PROTOCOL_AVDATA));

		memcpy(&st_ProtocolAVData, lpszMsgBuffer, sizeof(XENGINE_PROTOCOL_AVDATA));
		printf("%d,byAVType:%d,byFrameType:%d,nTimeStamp:%llu\n", nMsgLen, st_ProtocolAVData.byAVType, st_ProtocolAVData.byFrameType, st_ProtocolAVData.nTimeStamp);
		
		if (0 == st_ProtocolAVData.byAVType)
		{
			fwrite((LPCXSTR)lpszMsgBuffer + sizeof(XENGINE_PROTOCOL_AVDATA), 1, nMsgLen - sizeof(XENGINE_PROTOCOL_AVDATA), pSt_File);
		}
	}
}
void XStream_Pull()
{
	XNETHANDLE xhToken = 0;
	LPCXSTR lpszFLVUrl = _X("http://127.0.0.1:5600/api?stream=play&sms=live/qyt&type=xstream");

	pSt_File = _xtfopen("D:\\XEngine_StreamMedia\\XEngine_APPClient\\Debug\\1.h264", "wb");

	APIClient_Http_Create(&xhToken, APPClient_XStreamPull_Callback);
	APIClient_Http_SetUrl(xhToken, lpszFLVUrl, _X("GET"));
	APIClient_Http_Excute(xhToken);

	std::this_thread::sleep_for(std::chrono::seconds(20));

	APIClient_Http_Close(xhToken);
	fclose(pSt_File);
}
int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	XStream_Push();
	//XStream_Pull();

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}