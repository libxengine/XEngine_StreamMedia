CC = g++ -Wall -std=c++17 -fPIC
SHAREFLAG = -shared
SHAREDLL = so
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./ 
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_HelpComponents
LIB = -lHelpComponents_DataBase
LIBEX = -ldl -lpthread
LOADSO = -Wl,-rpath=./,--disable-new-dtags
LIBINCLUDE = ModuleDB_JT1078.o pch.o

ifeq ($(OS),Windows_NT)
else
	ifeq ($(shell uname),Darwin)
		SHAREDLL = dylib
		SHAREFLAG = -dynamiclib
		LOADSO = 
	else
		SHAREDLL = so
		SHAREFLAG = -shared
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
	$(CC) $(DEBUG) $(LIBINCLUDE) -o libXEngine_ModuleDatabase.$(SHAREDLL) $(SHAREFLAG) -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

ModuleDB_JT1078.o:./ModuleDB_JT1078/ModuleDB_JT1078.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./ModuleDB_JT1078/ModuleDB_JT1078.cpp 
						              
pch.o:./pch.cpp                 		       	    
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./pch.cpp


InstallAll:InstallSo 
InstallSo:./libXEngine_ModuleDatabase.$(SHAREDLL)
	cp ./libXEngine_ModuleDatabase.$(SHAREDLL) ../../XEngine_Release/libXEngine_ModuleDatabase.$(SHAREDLL)


CleanAll:CleanObj CleanMk
CleanObj:                                               
	rm *.o
CleanMk:                             
	rm *.$(SHAREDLL)                        