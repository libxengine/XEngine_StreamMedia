CC = g++ -Wall -std=c++20
PLATFORM = linux
PLATVER = 
PLATDIR = 
RELEASE = 0
UNICODE = 0
LOADHDR = -I ./ 
LOADSO = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_Core -L /usr/local/lib/XEngine_Release/XEngine_HelpComponents -L /usr/local/lib/XEngine_Release/XEngine_RfcComponents -L /usr/local/lib/XEngine_Release/XEngine_StreamMedia  \
         -L ../../XEngine_ModuleConfigure -L ../../XEngine_ModuleProtocol -L ../../XEngine_ModuleSession  \
		 -L ../../XEngine_ThirdPart/jsoncpp
LIB = -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_Core -lXEngine_ManagePool -lHelpComponents_XLog -lRfcComponents_HttpProtocol -lStreamMedia_StreamClient \
      -lXEngine_ModuleConfigure -lXEngine_ModuleProtocol -lXEngine_ModuleSession  \
	  -ljsoncpp
LIBEX = 
OBJECTS = XEngine_Configure.o XEngine_Network.o XEngine_HttpTask.o XEngine_ForwardApp.o

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
	$(CC) $(DEBUG) $(OBJECTS) -o XEngine_ForwardApp $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)
  
XEngine_Configure.o:./XEngine_Configure.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XEngine_Configure.cpp
XEngine_Network.o:./XEngine_Network.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XEngine_Network.cpp 
XEngine_HttpTask.o:./XEngine_HttpTask.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XEngine_HttpTask.cpp

XEngine_ForwardApp.o:./XEngine_ForwardApp.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./XEngine_ForwardApp.cpp   


InstallAll:InstallApp
InstallApp:./XEngine_ForwardApp
	cp -f ./XEngine_ForwardApp ../../../XEngine_Release/XEngine_ForwardApp

CleanAll:CleanObj CleanMk
CleanObj:                                              
	rm *.o
CleanMk:                                  
	rm XEngine_ForwardApp     
	       
