CC = g++ -Wall
RELEASE = 0
UNICODE = 0  
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L /usr/local/lib/XEngine_Release/XEngine_Core -L /usr/local/lib/XEngine_Release/XEngine_HelpComponents -L /usr/local/lib/XEngine_Release/XEngine_StreamMedia -L /usr/local/lib/XEngine_Release/XEngine_AVCoder \
          -L ../../XEngine_ThirdPart/jsoncpp \
		  -L ../../XEngine_ModuleConfigure -L ../../XEngine_ModuleDatabase -L ../../XEngine_ModuleProtocol -L ../../XEngine_ModuleHelp -L ../../XEngine_ModuleSession
LIB = -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_Core -lXEngine_ManagePool -lHelpComponents_XLog -lHelpComponents_Packets -lHelpComponents_DataBase -lStreamMedia_XClient -lXEngine_AVHelp \
      -ljsoncpp \
	  -lXEngine_ModuleConfigure -lXEngine_ModuleDatabase -lXEngine_ModuleProtocol -lXEngine_ModuleHelp -lXEngine_ModuleSession
LIBEX = -static-libgcc -ldl -lrt -lpthread
LOADSO = -Wl,-rpath=./,--disable-new-dtags
LIBINCLUDE = XEngine_Configure.o XEngine_Network.o XEngine_HttpTask.o XEngine_PluginTask.o XEngine_SDKApp.o

ifeq ($(RELEASE),1)
FLAGS = -c -O2 
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

all:$(LIBINCLUDE)
	$(CC) $(DEBUG) $(LIBINCLUDE) -o XEngine_SDKApp $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)
  
XEngine_Configure.o:./XEngine_Configure.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./XEngine_Configure.cpp
XEngine_Network.o:./XEngine_Network.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./XEngine_Network.cpp 
XEngine_HttpTask.o:./XEngine_HttpTask.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./XEngine_HttpTask.cpp
XEngine_PluginTask.o:./XEngine_PluginTask.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./XEngine_PluginTask.cpp

XEngine_SDKApp.o:./XEngine_SDKApp.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./XEngine_SDKApp.cpp   


InstallAll:InstallApp
InstallApp:./XEngine_SDKApp
	cp -f ./XEngine_SDKApp ../../../XEngine_Release/XEngine_SDKApp

CleanAll:CleanObj CleanMk
CleanObj:                                              
	rm *.o
CleanMk:                                  
	rm XEngine_SDKApp     
	       
