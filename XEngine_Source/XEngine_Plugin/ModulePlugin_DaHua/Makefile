CC = g++ -Wall -std=c++17 -fPIC
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_BaseLib -L ../../../XEngine_Release/XEngine_Plugin
LIB = -lXEngine_BaseLib -lavnetsdk -ldhconfigsdk -ldhnetsdk -lStreamConvertor
LIBEX = -static-libgcc -lrt -ldl -lpthread
LOADSO = -Wl,-rpath=../:./,--disable-new-dtags
LIBINCLUDE = Plugin_Dahua.o pch.o

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
	$(CC) $(DEBUG) $(LIBINCLUDE) -o libModulePlugin_DaHua.so -shared -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

Plugin_Dahua.o:./Plugin_Dahua/Plugin_Dahua.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./Plugin_Dahua/Plugin_Dahua.cpp 

pch.o:./pch.cpp                 		       	    
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./pch.cpp


InstallAll:InstallSo 
InstallSo:./libModulePlugin_DaHua.so
	cp ./libModulePlugin_DaHua.so ../../../XEngine_Release/XEngine_Plugin/libModulePlugin_DaHua.so

CleanAll:CleanObj CleanMk
CleanObj:                                               
	rm *.o
CleanMk:                             
	rm *.so                        