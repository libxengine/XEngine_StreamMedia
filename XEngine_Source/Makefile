RELEASE = 0
UNICODE = 0
#编译的选项参数
FLAGS = RELEASE=$(RELEASE)

#要编译的模块
THIRDPART_MODULE_JSONCPP = ./XEngine_ThirdPart/jsoncpp

MODULE_CONFIGURE_PATH = ./XEngine_ModuleConfigure
MODULE_DATABASE_PATH = ./XEngine_ModuleDatabase
MODULE_HELP_PATH = ./XEngine_ModuleHelp
MODULE_PLUGIN_PATH = ./XEngine_ModulePlugin
MODULE_PROTOCOL_PATH = ./XEngine_ModuleProtocol
MODULE_SESSION_PATH = ./XEngine_ModuleSession

PLUGIN_DAHUA_PATH = ./XEngine_Plugin/ModulePlugin_DaHua

APP_JT1078_PATH = ./XEngine_ServiceApp/XEngine_JT1078App
APP_SDKAPP_PATH = ./XEngine_ServiceApp/XEngine_SDKApp
APP_CENTER_PATH = ./XEngine_ServiceApp/XEngine_CenterApp

XENGINE_MODULES = libjsoncpp.so                                   \
                  libXEngine_ModuleConfigure.so libXEngine_ModuleDatabase.so libXEngine_ModuleHelp.so libXEngine_ModulePlugin.so libXEngine_ModuleProtocol.so libXEngine_ModuleSession.so        \
				  XEngine_JT1078App.exe XEngine_SDKApp.exe XEngine_CenterApp.exe
				  
ifeq ($(shell uname),Darwin)
else
XENGINE_MODULES += libModulePlugin_DaHua.so
endif

.PHONY:MakeAll 
MakeAll:$(XENGINE_MODULES)

libjsoncpp.so:
	make -C $(THIRDPART_MODULE_JSONCPP) $(FLAGS)

libXEngine_ModuleConfigure.so:
	make -C $(MODULE_CONFIGURE_PATH) $(FLAGS)
libXEngine_ModuleDatabase.so:
	make -C $(MODULE_DATABASE_PATH) $(FLAGS)
libXEngine_ModuleHelp.so:
	make -C $(MODULE_HELP_PATH) $(FLAGS)
libXEngine_ModulePlugin.so:
	make -C $(MODULE_PLUGIN_PATH) $(FLAGS)
libXEngine_ModuleProtocol.so:
	make -C $(MODULE_PROTOCOL_PATH) $(FLAGS)
libXEngine_ModuleSession.so:
	make -C $(MODULE_SESSION_PATH) $(FLAGS)

libModulePlugin_DaHua.so:
	make -C $(PLUGIN_DAHUA_PATH) $(FLAGS)	

XEngine_JT1078App.exe:
	make -C $(APP_JT1078_PATH) $(FLAGS)
XEngine_SDKApp.exe:
	make -C $(APP_SDKAPP_PATH) $(FLAGS)
XEngine_CenterApp.exe:
	make -C $(APP_CENTER_PATH) $(FLAGS)

SetFlags_Install:
	export FLAGS=InstallAll
InstallAll:SetFlags_Install $(NETENGINE_LIB) 
 
SetFlags_Clean:
	export FLAGS=CleanAll
CleanAll:SetFlags_Clean $(NETENGINE_LIB) 
