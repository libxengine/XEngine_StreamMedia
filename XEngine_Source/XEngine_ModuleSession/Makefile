CC = g++ -Wall -std=c++20 -fPIC
PLATVER = 
FILEEXT = 
LIBFLAG = 
LOADHDR = -I ./ 
LOADSO = 
LIB = -lXEngine_BaseLib -lXEngine_SystemApi
LIBEX = 
OBJECTS = ModuleSession_PullStream.o ModuleSession_PushStream.o pch.o

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
	LIBFLAG = -dynamiclib
	LIBEX = -lpthread -ldl
	LOADBIN = -Wl,-rpath,@loader_path/./
endif


all:$(OBJECTS)
	$(CC) $(DEBUG) $(OBJECTS) -o libXEngine_ModuleSession.$(FILEEXT) $(LIBFLAG) $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)

ModuleSession_PullStream.o:./ModuleSession_PullStream/ModuleSession_PullStream.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./ModuleSession_PullStream/ModuleSession_PullStream.cpp 
ModuleSession_PushStream.o:./ModuleSession_PushStream/ModuleSession_PushStream.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./ModuleSession_PushStream/ModuleSession_PushStream.cpp 

pch.o:./pch.cpp                 		       	    
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./pch.cpp


InstallAll:InstallSo 
InstallSo:./libXEngine_ModuleSession.$(FILEEXT)
	cp ./libXEngine_ModuleSession.$(FILEEXT) ../../XEngine_Release/libXEngine_ModuleSession.$(FILEEXT)


CleanAll:CleanObj CleanMk
CleanObj:                                               
	rm *.o
CleanMk:                             
	rm *.$(FILEEXT)                        
