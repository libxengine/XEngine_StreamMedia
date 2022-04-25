CC = g++ -Wall -fPIC
RELEASE = 0
UNICODE = 0
LOADBIN = -L ../../XEngine_Lib/NetEngine_BaseLib -L ../../XEngine_Lib/NetEngine_Algorithm
LIB = -lNetEngine_BaseLib -lNetEngine_Algorithm
LIBEX = -static-libgcc -lrt -ldl -lpthread
LOADSO = -Wl,-rpath=../XEngine_BaseLib,--disable-new-dtags
LIBINCLUDE = Session_Stream.o Session_Record.o Session_Server.o Session_Talk.o Session_Gover.o Session_GoverEx.o Session_XPush.o Session_DllMain.o

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
	$(CC) $(DEBUG) $(LIBINCLUDE) -o libNetHelp_Session.so -shared -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

Session_Stream.o:./Session_Stream/Session_Stream.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./Session_Stream/Session_Stream.cpp 
Session_Record.o:./Session_Record/Session_Record.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./Session_Record/Session_Record.cpp
Session_Server.o:./Session_Server/Session_Server.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./Session_Server/Session_Server.cpp
Session_Talk.o:./Session_Talk/Session_Talk.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./Session_Talk/Session_Talk.cpp
Session_Gover.o:./Session_Gover/Session_Gover.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./Session_Gover/Session_Gover.cpp
Session_GoverEx.o:./Session_Gover/Session_GoverEx.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./Session_Gover/Session_GoverEx.cpp
Session_XPush.o:./Session_XPush/Session_XPush.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./Session_XPush/Session_XPush.cpp
				              
Session_DllMain.o:./Session_DllMain.cpp                 		       	    
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./Session_DllMain.cpp


InstallAll:InstallSo 
InstallSo:./libNetHelp_Session.so
	cp ./libNetHelp_Session.so ../../../XEngine_Release/XEngine_NetHelp/libNetHelp_Session.so


CleanAll:CleanObj CleanMk
CleanObj:                                               
	rm *.o
CleanMk:                             
	rm *.so                        
