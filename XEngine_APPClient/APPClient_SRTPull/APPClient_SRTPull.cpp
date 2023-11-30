#ifdef _MSC_BUILD
#pragma comment(lib,"Ws2_32")
#pragma comment(lib,"srt")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <srt/srt.h>
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
//需要优先配置XEngine
//WINDOWS使用VS2022 x86 或者 x64 debug 编译
//linux::g++ -std=c++17 -Wall -g APPClient_SRTPull.cpp -o APPClient_SRTPull.exe -lsrt
//macos::g++ -std=c++17 -Wall -g APPClient_SRTPull.cpp -o APPClient_SRTPull.exe -lsrt
FILE* pSt_File;

int nPort = 10080;
LPCXSTR lpszServiceAddr = _X("10.0.3.155");

int main()
{
	srt_startup();
	srt_setloglevel(srt_logging::LogLevel::error);

	struct sockaddr_in st_SockAddr;

	st_SockAddr.sin_family = AF_INET;
	st_SockAddr.sin_port = htons(nPort);
	if (inet_pton(AF_INET, lpszServiceAddr, &st_SockAddr.sin_addr) != 1)
	{
		return 1;
	}
	pSt_File = fopen("D:\\XEngine_StreamMedia\\XEngine_APPClient\\Debug\\1.ts", "wb");

	SRTSOCKET hSocket = srt_create_socket();
	if (SRT_INVALID_SOCK == hSocket)
	{
		printf("srt_socket:%s\n", srt_getlasterror_str());
		return 0;
	}
	SRT_TRANSTYPE nType = SRTT_LIVE;
	srt_setsockopt(hSocket, 0, SRTO_TRANSTYPE, &nType, sizeof(nType));

	const char* lpszMsgBuffer = "#!::r=live/livestream,m=request";
	if (SRT_ERROR == srt_setsockflag(hSocket, SRTO_STREAMID, lpszMsgBuffer, strlen(lpszMsgBuffer)))
	{
		printf("srt_setsockflag:%s\n", srt_getlasterror_str());
		return 0;
	}

	int nLatency = 0;
	if (SRT_ERROR == srt_setsockflag(hSocket, SRTO_LATENCY, &nLatency, sizeof(int)))
	{
		printf("srt_setsockflag:%s\n", srt_getlasterror_str());
		return 0;
	}
	if (SRT_ERROR == srt_connect(hSocket, (struct sockaddr*)&st_SockAddr, sizeof st_SockAddr))
	{
		printf("srt_connect:%s\n", srt_getlasterror_str());
		return 0;
	}

	int nTimeStart = time(NULL);
	while (true)
	{
		int nTimeEnd = time(NULL);
		char tszMsgBuffer[2048];
		
		int ret = srt_recvmsg(hSocket, tszMsgBuffer, sizeof(tszMsgBuffer));
		if (SRT_ERROR != ret)
		{
			fwrite(tszMsgBuffer, 1, ret, pSt_File);
		}

		if (nTimeEnd - nTimeStart > 20)
		{
			break;
		}
	}
	srt_close(hSocket);
	srt_cleanup();
	return 0;
}