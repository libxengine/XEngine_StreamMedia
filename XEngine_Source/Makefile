RELEASE = 0
UNICODE = 0
PLATFORM=linux
FLAGS= 
#要编译的模块
THIRDPART_MODULE_JSONCPP = ./XEngine_Depend/XEngine_Module/jsoncpp
THIRDPART_MODULE_REPORT = ./XEngine_Depend/XEngine_Module/XEngine_InfoReport

MODULE_CONFIGURE_PATH = ./XEngine_ModuleConfigure
MODULE_HELP_PATH = ./XEngine_ModuleHelp
MODULE_PROTOCOL_PATH = ./XEngine_ModuleProtocol
MODULE_SESSION_PATH = ./XEngine_ModuleSession
MODULE_QUEUE_PATH = ./XEngine_ModuleQueue

APP_STREAMMEDIA_PATH = ./XEngine_ServiceApp/XEngine_StreamMediaApp

XENGINE_MODULES = libjsoncpp.so libXEngine_InfoReport.so                           \
                  libXEngine_ModuleConfigure.so libXEngine_ModuleHelp.so libXEngine_ModuleProtocol.so libXEngine_ModuleSession.so libXEngine_ModuleQueue.so       \
				  XEngine_StreamMediaApp.exe

.PHONY:MakeAll 
MakeAll:$(XENGINE_MODULES)

libjsoncpp.so:
	make -C $(THIRDPART_MODULE_JSONCPP) PLATFORM=$(PLATFORM) $(FLAGS)
libXEngine_InfoReport.so:
	make -C $(THIRDPART_MODULE_REPORT) PLATFORM=$(PLATFORM) $(FLAGS)

libXEngine_ModuleConfigure.so:
	make -C $(MODULE_CONFIGURE_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
libXEngine_ModuleHelp.so:
	make -C $(MODULE_HELP_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
libXEngine_ModuleProtocol.so:
	make -C $(MODULE_PROTOCOL_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
libXEngine_ModuleSession.so:
	make -C $(MODULE_SESSION_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
libXEngine_ModuleQueue.so:
	make -C $(MODULE_QUEUE_PATH) PLATFORM=$(PLATFORM) $(FLAGS)

XEngine_StreamMediaApp.exe:
	make -C $(APP_STREAMMEDIA_PATH) PLATFORM=$(PLATFORM) $(FLAGS)
