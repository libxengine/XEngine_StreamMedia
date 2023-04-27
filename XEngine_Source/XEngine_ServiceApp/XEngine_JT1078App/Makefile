CC = g++ -Wall -std=c++20
PLATFORM = linux
PLATVER = 
PLATDIR = 
RELEASE = 0
UNICODE = 0
LOADHDR = -I ./ 
LOADSO = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_Core -L /usr/local/lib/XEngine_Release/XEngine_HelpComponents -L /usr/local/lib/XEngine_Release/XEngine_Client \
         -L ../../XEngine_ModuleConfigure -L ../../XEngine_ModuleProtocol -L ../../XEngine_ModuleHelp -L ../../XEngine_ModuleSession  \
		 -L ../../XEngine_ThirdPart/jsoncpp
LIB = -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_Core -lXEngine_ManagePool -lXEngine_NetXApi -lHelpComponents_XLog -lHelpComponents_Packets -lXClient_Socket \
      -lXEngine_ModuleConfigure -lXEngine_ModuleProtocol -lXEngine_ModuleHelp -lXEngine_ModuleSession  \
	  -ljsoncpp
LIBEX = 
OBJECTS = JT1078App_Client.o JT1078App_Config.o JT1078App_Net.o JT1078App_Record.o JT1078App_Stream.o XEngine_JT1078App.o

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
	$(CC) $(DEBUG) $(OBJECTS) -o XEngine_JT1078App $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)
  
JT1078App_Client.o:./JT1078App_Client.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./JT1078App_Client.cpp
JT1078App_Config.o:./JT1078App_Config.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./JT1078App_Config.cpp 
JT1078App_Net.o:./JT1078App_Net.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./JT1078App_Net.cpp
JT1078App_Record.o:./JT1078App_Record.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./JT1078App_Record.cpp
JT1078App_Stream.o:./JT1078App_Stream.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./JT1078App_Stream.cpp

XEngine_JT1078App.o:./XEngine_JT1078App.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XEngine_JT1078App.cpp   


InstallAll:InstallApp
InstallApp:./XEngine_JT1078App
	cp -f ./XEngine_JT1078App ../../../XEngine_Release/XEngine_JT1078App

CleanAll:CleanObj CleanMk
CleanObj:                                              
	rm *.o
CleanMk:                                  
	rm XEngine_JT1078App     
	       
