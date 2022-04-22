RELEASE = 0
UNICODE = 0
#编译的选项参数
FLAGS = RELEASE=$(RELEASE)

#要编译的模块
THIRDPART_MODULE_JSONCPP = ./XEngine_ThirdPart/jsoncpp

MODULE_CONFIGURE_PATH = ./XEngine_ModuleConfigure
MODULE_DATABASE_PATH = ./XEngine_ModuleDatabase
MODULE_PROTOCOL_PATH = ./XEngine_ModuleProtocol
APP_CENTER_PATH = ./XEngine_ServiceApp/XEngine_CenterApp
APP_HTTP_PATH = ./XEngine_ServiceApp/XEngine_HttpApp
APP_HTTP2_PATH = ./XEngine_ServiceApp/XEngine_HTTP2App
APP_WEBSOCKET_PATH = ./XEngine_ServiceApp/XEngine_WebSocketApp

XENGINE_MODULES = libjsoncpp.so                                   \
                  libXEngine_ModuleConfigure.so libXEngine_ModuleDatabase.so libXEngine_ModuleProtocol.so                 \
				  XEngine_CenterApp.exe XEngine_HttpApp.exe XEngine_Http2App.exe XEngine_WebSocketApp.exe

.PHONY:MakeAll 
MakeAll:$(XENGINE_MODULES)

libjsoncpp.so:
	make -C $(THIRDPART_MODULE_JSONCPP) $(FLAGS)

libXEngine_ModuleConfigure.so:
	make -C $(MODULE_CONFIGURE_PATH) $(FLAGS)
libXEngine_ModuleDatabase.so:
	make -C $(MODULE_DATABASE_PATH) $(FLAGS)
libXEngine_ModuleProtocol.so:
	make -C $(MODULE_PROTOCOL_PATH) $(FLAGS)

XEngine_CenterApp.exe:
	make -C $(APP_CENTER_PATH) $(FLAGS)
XEngine_HttpApp.exe:
	make -C $(APP_HTTP_PATH) $(FLAGS)
XEngine_Http2App.exe:
	make -C $(APP_HTTP2_PATH) $(FLAGS)
XEngine_WebSocketApp.exe:
	make -C $(APP_WEBSOCKET_PATH) $(FLAGS)

SetFlags_Install:
	export FLAGS=InstallAll
InstallAll:SetFlags_Install $(NETENGINE_LIB) 
 
SetFlags_Clean:
	export FLAGS=CleanAll
CleanAll:SetFlags_Clean $(NETENGINE_LIB) 
