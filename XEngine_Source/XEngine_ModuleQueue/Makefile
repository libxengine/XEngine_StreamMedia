CC = g++ -Wall -std=c++20 -fPIC
PLATVER = 
FILEEXT = 
LIBFLAG = 
LOADHDR = -I ./ 
LOADSO = 
LIB = 
LIBEX = 
OBJECTS = ModuleQueue_JT1078.o pch.o

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
	$(CC) $(DEBUG) $(OBJECTS) -o libXEngine_ModuleQueue.$(FILEEXT) $(LIBFLAG) $(LIB) $(LIBEX) $(LOADSO) $(LOADBIN)

ModuleQueue_JT1078.o:./ModuleQueue_JT1078/ModuleQueue_JT1078.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./ModuleQueue_JT1078/ModuleQueue_JT1078.cpp 

pch.o:./pch.cpp                 		       	    
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(PLATVER) $(LOADHDR) ./pch.cpp


InstallAll:InstallSo 
InstallSo:./libXEngine_ModuleQueue.$(FILEEXT)
	cp ./libXEngine_ModuleQueue.$(FILEEXT) ../../XEngine_Release/libXEngine_ModuleQueue.$(FILEEXT)


CleanAll:CleanObj CleanMk
CleanObj:                                               
	rm *.o
CleanMk:                             
	rm *.$(FILEEXT)                        
