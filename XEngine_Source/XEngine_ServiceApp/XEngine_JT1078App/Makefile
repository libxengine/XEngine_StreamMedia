CC = g++ -Wall -std=c++17
RELEASE = 0
UNICODE = 0  
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_Core -L /usr/local/lib/XEngine_Release/XEngine_HelpComponents -L /usr/local/lib/XEngine_Release/XEngine_Client \
          -L ../../XEngine_ThirdPart/jsoncpp \
		  -L ../../XEngine_ModuleConfigure -L ../../XEngine_ModuleDatabase -L ../../XEngine_ModuleProtocol -L ../../XEngine_ModuleHelp -L ../../XEngine_ModuleSession
LIB = -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_Core -lXEngine_ManagePool -lXEngine_NetXApi -lHelpComponents_XLog -lHelpComponents_Packets -lHelpComponents_DataBase -lXClient_Socket \
      -ljsoncpp \
	  -lXEngine_ModuleConfigure -lXEngine_ModuleDatabase -lXEngine_ModuleProtocol -lXEngine_ModuleHelp -lXEngine_ModuleSession
LIBEX = -ldl -lpthread
LOADSO = -Wl,-rpath=./,--disable-new-dtags
LIBINCLUDE = JT1078App_Client.o JT1078App_Config.o JT1078App_Net.o JT1078App_Record.o JT1078App_Stream.o XEngine_JT1078App.o

ifeq ($(OS),Windows_NT)
else
	ifeq ($(shell uname),Darwin)
		LOADSO = 
	else
	endif
endif

ifeq ($(RELEASE),1)
FLAGS = -c -O2 
DEBUG = 
else
FLAGS = -c
DEBUG = -g
endif

ifeq ($(UNICODE),1)
UNICODE = -D _UNICODE
else
UNICODE = 
endif

all:$(LIBINCLUDE)
	$(CC) $(DEBUG) $(LIBINCLUDE) -o XEngine_JT1078App $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)
  
JT1078App_Client.o:./JT1078App_Client.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./JT1078App_Client.cpp
JT1078App_Config.o:./JT1078App_Config.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./JT1078App_Config.cpp 
JT1078App_Net.o:./JT1078App_Net.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./JT1078App_Net.cpp
JT1078App_Record.o:./JT1078App_Record.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./JT1078App_Record.cpp
JT1078App_Stream.o:./JT1078App_Stream.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./JT1078App_Stream.cpp

XEngine_JT1078App.o:./XEngine_JT1078App.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./XEngine_JT1078App.cpp   


InstallAll:InstallApp
InstallApp:./XEngine_JT1078App
	cp -f ./XEngine_JT1078App ../../../XEngine_Release/XEngine_JT1078App

CleanAll:CleanObj CleanMk
CleanObj:                                              
	rm *.o
CleanMk:                                  
	rm XEngine_JT1078App     
	       
