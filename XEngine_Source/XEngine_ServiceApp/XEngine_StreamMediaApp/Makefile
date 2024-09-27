CC = g++ -Wall -std=c++17 -Wno-format-overflow
PLATFORM = linux
PLATVER = 
PLATDIR = 
RELEASE = 0
UNICODE = 0
LOADHDR = -I ./ 
LOADSO = -L ../../XEngine_ModuleConfigure -L ../../XEngine_ModuleProtocol -L ../../XEngine_ModuleSession -L ../../XEngine_ModuleHelp  -L ../../XEngine_ModuleQueue \
		 -L ../../XEngine_Depend/XEngine_Module/jsoncpp -L ../../XEngine_Depend/XEngine_Module/XEngine_InfoReport
LIB = -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_Core -lXEngine_ManagePool -lXEngine_OPenSsl -lXClient_Stream -lHelpComponents_XLog -lHelpComponents_Packets -lNetHelp_APIHelp -lNetHelp_XSocket -lRfcComponents_HttpProtocol -lRfcComponents_NatProtocol -lXEngine_AVHelp -lStreamMedia_FLVProtocol -lStreamMedia_RTMPProtocol -lStreamMedia_SDPProtocol -lStreamMedia_HLSProtocol -lStreamMedia_RTSPProtocol -lStreamMedia_RTPProtocol -lStreamMedia_RTCPProtocol \
      -lXEngine_ModuleConfigure -lXEngine_ModuleProtocol -lXEngine_ModuleSession -lXEngine_ModuleHelp -lXEngine_ModuleQueue \
	  -ljsoncpp -lXEngine_InfoReport
LIBEX = 
OBJECTS = PullStream_ClientGet.o PullStream_ClientRtsp.o PullStream_ClientWebRtc.o \
          PushStream_JT1078Task.o PushStream_RTMPTask.o PushStream_XStreamTask.o PushStream_SrtTask.o \
		  StreamMedia_HTTPApi.o \
          XEngine_AVPacket.o XEngine_Configure.o XEngine_HttpTask.o XEngine_Network.o XEngine_StreamMediaApp.o

ifeq ($(RELEASE),1)
FLAGS = -c 
DEBUG = 
else
	ifeq ($(PLATFORM),mac)
		FLAGS = -c
		DEBUG = -g 
	else
		FLAGS = -c -lc_p
		DEBUG = -g -pg 
	endif
endif

ifeq ($(UNICODE),1)
UNICODE = -D _UNICODE
else
UNICODE = 
endif

ifeq ($(PLATFORM),linux)
	ifeq (/etc/redhat-release,$(wildcard /etc/redhat-release))
		PLATVER = -D __CENTOS__
		PLATDIR = XEngine_Linux/Centos
	else
		PLATVER = -D __UBUNTU__
		PLATDIR = XEngine_Linux/Ubuntu
	endif
	LIBEX = -lpthread -lrt -ldl
	LOADBIN = -Wl,-rpath=./,--disable-new-dtags
else ifeq ($(PLATFORM),mac)
	CC += -Wno-deprecated-declarations
	PLATVER = -D __MACOS__
	PLATDIR = XEngine_Mac
	LIBEX = -lpthread -ldl
	LOADBIN = -Wl,-rpath,@loader_path/./
endif

all:$(OBJECTS)
	$(CC) $(DEBUG) $(OBJECTS) -o XEngine_StreamMediaApp $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)

PullStream_ClientGet.o:./StreamMedia_PullStream/PullStream_ClientGet.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./StreamMedia_PullStream/PullStream_ClientGet.cpp
PullStream_ClientRtsp.o:./StreamMedia_PullStream/PullStream_ClientRtsp.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./StreamMedia_PullStream/PullStream_ClientRtsp.cpp
PullStream_ClientWebRtc.o:./StreamMedia_PullStream/PullStream_ClientWebRtc.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./StreamMedia_PullStream/PullStream_ClientWebRtc.cpp

PushStream_XStreamTask.o:./StreamMedia_PushStream/PushStream_XStreamTask.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./StreamMedia_PushStream/PushStream_XStreamTask.cpp
PushStream_JT1078Task.o:./StreamMedia_PushStream/PushStream_JT1078Task.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./StreamMedia_PushStream/PushStream_JT1078Task.cpp
PushStream_RTMPTask.o:./StreamMedia_PushStream/PushStream_RTMPTask.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./StreamMedia_PushStream/PushStream_RTMPTask.cpp
PushStream_SrtTask.o:./StreamMedia_PushStream/PushStream_SrtTask.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./StreamMedia_PushStream/PushStream_SrtTask.cpp

StreamMedia_HTTPApi.o:./StreamMedia_HTTPApi/StreamMedia_HTTPApi.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./StreamMedia_HTTPApi/StreamMedia_HTTPApi.cpp

XEngine_AVPacket.o:./XEngine_AVPacket.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XEngine_AVPacket.cpp 
XEngine_Configure.o:./XEngine_Configure.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XEngine_Configure.cpp
XEngine_HttpTask.o:./XEngine_HttpTask.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XEngine_HttpTask.cpp
XEngine_Network.o:./XEngine_Network.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XEngine_Network.cpp 

XEngine_StreamMediaApp.o:./XEngine_StreamMediaApp.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XEngine_StreamMediaApp.cpp   


InstallAll:InstallApp
InstallApp:./XEngine_StreamMediaApp
	cp -f ./XEngine_StreamMediaApp ../../../XEngine_Release/XEngine_StreamMediaApp

CleanAll:CleanObj CleanMk
CleanObj:                                              
	rm *.o
CleanMk:                                  
	rm XEngine_StreamMediaApp     
	       
