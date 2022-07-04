CC = g++ -Wall -std=c++17 -fPIC
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./
LOADBIN = 
LIB = 
LIBEX = -static-libgcc -lrt -ldl -lpthread
LOADSO = -Wl,-rpath=./,--disable-new-dtags
LIBINCLUDE = ModuleSession_Client.o ModuleSession_Server.o ModuleSession_SDKDevice.o pch.o

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
	$(CC) $(DEBUG) $(LIBINCLUDE) -o libXEngine_ModuleSession.so -shared -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

ModuleSession_Client.o:./ModuleSession_Client/ModuleSession_Client.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./ModuleSession_Client/ModuleSession_Client.cpp 
ModuleSession_Server.o:./ModuleSession_Server/ModuleSession_Server.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./ModuleSession_Server/ModuleSession_Server.cpp 
ModuleSession_SDKDevice.o:./ModuleSession_SDKDevice/ModuleSession_SDKDevice.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./ModuleSession_SDKDevice/ModuleSession_SDKDevice.cpp 

pch.o:./pch.cpp                 		       	    
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./pch.cpp


InstallAll:InstallSo 
InstallSo:./libXEngine_ModuleSession.so
	cp ./libXEngine_ModuleSession.so ../../XEngine_Release/libXEngine_ModuleSession.so


CleanAll:CleanObj CleanMk
CleanObj:                                               
	rm *.o
CleanMk:                             
	rm *.so                        
