CC = g++ -Wall -std=c++20 -fPIC
PLATVER = 
FILEEXT = 
LIBFLAG = 
LOADHDR = -I ./ 
LOADSO = 
LIB = -lXEngine_BaseLib -lXEngine_Cryption -lNetHelp_APIAddr
LIBEX = 
OBJECTS = ModuleHelp_JT1078.o ModuleHelp_SrtCore.o ModuleHelp_Rtsp.o ModuleHelp_Rtmp.o ModuleHelp_SRTPCore.o pch.o

MACRO_SRT_SUPPORT := $(shell gcc -E -dM ../XEngine_BuildSwitch.h | grep _XENGINE_STREAMMEDIA_BUILDSWITCH_SRT | cut -d ' ' -f 3)
MACRO_RTC_SUPPORT := $(shell gcc -E -dM ../XEngine_BuildSwitch.h | grep _XENGINE_STREAMMEDIA_BUILDSWITCH_RTC | cut -d ' ' -f 3)

ifeq ($(MACRO_SRT_SUPPORT),1)
	ifeq ($(PLATFORM),mac)
		LIB += -lsrt
	else
		ifeq (/etc/redhat-release,$(wildcard /etc/redhat-release))
			LIB += -lsrt
		else
			LIB += -lsrt-gnutls
		endif
		
	endif
endif
ifeq ($(MACRO_RTC_SUPPORT),1)
	LIB += -lsrtp2
endif

ifeq ($(RELEASE),1)
FLAGS = -c -D _RELEASE
DEBUG = -O2
else
	ifeq ($(PLATFORM),mac)
		FLAGS = -c
		DEBUG = -g -D _DEBUG
	else
		FLAGS = -c -lc_p
		DEBUG = -g -pg -D _DEBUG
	endif
endif

ifeq ($(UNICODE),1)
override UNICODE := -D __UNICODE__
else
override UNICODE := -D __ANSI__
endif

ifeq ($(PLATFORM),linux)
	ifeq (/etc/redhat-release,$(wildcard /etc/redhat-release))
		PLATVER = -D __CENTOS__
	else
		PLATVER = -D __UBUNTU__
	endif
	FILEEXT = so
	LIBFLAG = -shared
	LIBEX = -lpthread -lrt -ldl
	LOADBIN = -Wl,-rpath=./,--disable-new-dtags
else ifeq ($(PLATFORM),mac)
	CC += -Wno-deprecated-declarations
	PLATVER = -D __MACOS__
	FILEEXT = dylib
	LOADHDR += -I /opt/homebrew/opt/srt/include -I /opt/homebrew/opt/srtp/include
	LOADSO += -L /opt/homebrew/opt/srt/lib -L /opt/homebrew/opt/srtp/lib
	LIBFLAG = -dynamiclib
	LIBEX = -lpthread -ldl
	LOADBIN = -Wl,-rpath,@loader_path/./
endif


all:$(OBJECTS)
	$(CC) $(DEBUG) $(OBJECTS) -o libXEngine_ModuleHelp.$(FILEEXT) $(LIBFLAG) $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)

ModuleHelp_JT1078.o:./ModuleHelp_JT1078/ModuleHelp_JT1078.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./ModuleHelp_JT1078/ModuleHelp_JT1078.cpp 
ModuleHelp_SrtCore.o:./ModuleHelp_Srt/ModuleHelp_SrtCore.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./ModuleHelp_Srt/ModuleHelp_SrtCore.cpp 
ModuleHelp_Rtsp.o:./ModuleHelp_Rtsp/ModuleHelp_Rtsp.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./ModuleHelp_Rtsp/ModuleHelp_Rtsp.cpp 
ModuleHelp_Rtmp.o:./ModuleHelp_Rtmp/ModuleHelp_Rtmp.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./ModuleHelp_Rtmp/ModuleHelp_Rtmp.cpp
ModuleHelp_SRTPCore.o:./ModuleHelp_SRtp/ModuleHelp_SRTPCore.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./ModuleHelp_SRtp/ModuleHelp_SRTPCore.cpp

pch.o:./pch.cpp                 		       	    
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./pch.cpp


InstallAll:InstallSo 
InstallSo:./libXEngine_ModuleHelp.$(FILEEXT)
	cp ./libXEngine_ModuleHelp.$(FILEEXT) ../../XEngine_Release/libXEngine_ModuleHelp.$(FILEEXT)


CleanAll:CleanObj CleanMk
CleanObj:                                               
	rm *.o
CleanMk:                             
	rm *.$(FILEEXT)                        
