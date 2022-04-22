CC = g++ -Wall -std=c++17 -fPIC
RELEASE = 0
UNICODE = 0
INCLUDE = -I ./ 
LOADBIN = -L /usr/local/lib/XEngine_Release/XEngine_HelpComponents
LIB = -lHelpComponents_DataBase
LIBEX = -static-libgcc -lrt -ldl -lpthread
LOADSO = -Wl,-rpath=./,--disable-new-dtags
LIBINCLUDE = ModuleDatabase_MySql.o ModuleDatabase_SQlite.o pch.o

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
	$(CC) $(DEBUG) $(LIBINCLUDE) -o libXEngine_ModuleDatabase.so -shared -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)

ModuleDatabase_MySql.o:./ModuleDatabase_MySql/ModuleDatabase_MySql.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./ModuleDatabase_MySql/ModuleDatabase_MySql.cpp 
ModuleDatabase_SQlite.o:./ModuleDatabase_SQlite/ModuleDatabase_SQlite.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./ModuleDatabase_SQlite/ModuleDatabase_SQlite.cpp 
						              
pch.o:./pch.cpp                 		       	    
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) $(INCLUDE) ./pch.cpp


InstallAll:InstallSo 
InstallSo:./libXEngine_ModuleDatabase.so
	cp ./libXEngine_ModuleDatabase.so ../../XEngine_Release/libXEngine_ModuleDatabase.so


CleanAll:CleanObj CleanMk
CleanObj:                                               
	rm *.o
CleanMk:                             
	rm *.so                        