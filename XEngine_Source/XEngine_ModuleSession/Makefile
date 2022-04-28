CC = g++ -Wall -std=c++17 -fPIC
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./
LOADBIN = 
LIB = 
LIBEX = -static-libgcc -lrt -ldl -lpthread
LOADSO = -Wl,-rpath=./,--disable-new-dtags
LIBINCLUDE = ModuleSession_JT1078Client.o ModuleSession_JT1078Server.o pch.o

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

ModuleSession_JT1078Client.o:./ModuleSession_JT1078/ModuleSession_JT1078Client.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./ModuleSession_JT1078/ModuleSession_JT1078Client.cpp 
ModuleSession_JT1078Server.o:./ModuleSession_JT1078/ModuleSession_JT1078Server.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./ModuleSession_JT1078/ModuleSession_JT1078Server.cpp 

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
