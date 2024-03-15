CC = g++ -Wall -std=c++17 -fPIC
PLATFORM = linux
PLATVER = 
PLATDIR = 
FILEEXT = 
LIBFLAG = 
RELEASE = 0
UNICODE = 0
LOADHDR = -I ./ 
LOADSO = 
LIB = -lXEngine_OPenSsl
LIBEX = 
OBJECTS = ModuleHelp_JT1078.o ModuleHelp_SrtCore.o ModuleHelp_Rtsp.o ModuleHelp_Rtmp.o pch.o

MACRO_SRT_SUPPORT := $(shell gcc -E -dM ../XStorage_BuildSwitch.h | grep _XENGINE_STREAMMEDIA_BUILDSWITCH_SRT | cut -d ' ' -f 3)
MACRO_RTC_SUPPORT := $(shell gcc -E -dM ../XStorage_BuildSwitch.h | grep _XENGINE_STREAMMEDIA_BUILDSWITCH_RTC | cut -d ' ' -f 3)

ifeq ($(MACRO_SRT_SUPPORT),1)
    LIB += -lsrt-gnutls
endif

ifeq ($(RELEASE),1)
FLAGS = -c 
DEBUG = 
else
FLAGS = -c -lc_p
DEBUG = -g -pg 
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
	FILEEXT = so
	LIBFLAG = -shared
	LIBEX = -lpthread -lrt -ldl
	LOADBIN = -Wl,-rpath=./,--disable-new-dtags
else ifeq ($(PLATFORM),mac)
	CC += -Wno-deprecated-declarations
	PLATVER = -D __MACOS__
	PLATDIR = XEngine_Mac
	FILEEXT = dylib
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