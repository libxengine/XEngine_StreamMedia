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

ifeq ($(PLATFORM),linux)
	FILEEXT = so
else ifeq ($(PLATFORM),mac)
	FILEEXT = dylib
endif

XENGINE_MODULES = libjsoncpp.so libXEngine_InfoReport.so                           \
                  libXEngine_ModuleConfigure.so libXEngine_ModuleHelp.so libXEngine_ModuleProtocol.so libXEngine_ModuleSession.so libXEngine_ModuleQueue.so       \
				  XEngine_StreamMediaApp.exe

.PHONY:MakeAll 
MakeAll:$(XENGINE_MODULES)


libjsoncpp.so:
ifeq ($(FLAGS), InstallAll)
	cp $(THIRDPART_MODULE_JSONCPP)/libjsoncpp.$(FILEEXT) ../XEngine_Release/
else
	make -C $(THIRDPART_MODULE_JSONCPP) PLATFORM=$(PLATFORM) UNICODE=$(UNICODE) RELEASE=$(RELEASE) $(FLAGS)
endif
libXEngine_InfoReport.so:
ifeq ($(FLAGS), InstallAll)
	cp $(THIRDPART_MODULE_REPORT)/libXEngine_InfoReport.$(FILEEXT) ../XEngine_Release/
else
	make -C $(THIRDPART_MODULE_REPORT) PLATFORM=$(PLATFORM) UNICODE=$(UNICODE) RELEASE=$(RELEASE) $(FLAGS)
endif

libXEngine_ModuleConfigure.so:
	make -C $(MODULE_CONFIGURE_PATH) PLATFORM=$(PLATFORM) UNICODE=$(UNICODE) RELEASE=$(RELEASE) $(FLAGS)
libXEngine_ModuleHelp.so:
	make -C $(MODULE_HELP_PATH) PLATFORM=$(PLATFORM) UNICODE=$(UNICODE) RELEASE=$(RELEASE) $(FLAGS)
libXEngine_ModuleProtocol.so:
	make -C $(MODULE_PROTOCOL_PATH) PLATFORM=$(PLATFORM) UNICODE=$(UNICODE) RELEASE=$(RELEASE) $(FLAGS)
libXEngine_ModuleSession.so:
	make -C $(MODULE_SESSION_PATH) PLATFORM=$(PLATFORM) UNICODE=$(UNICODE) RELEASE=$(RELEASE) $(FLAGS)
libXEngine_ModuleQueue.so:
	make -C $(MODULE_QUEUE_PATH) PLATFORM=$(PLATFORM) UNICODE=$(UNICODE) RELEASE=$(RELEASE) $(FLAGS)

XEngine_StreamMediaApp.exe:
	make -C $(APP_STREAMMEDIA_PATH) PLATFORM=$(PLATFORM) UNICODE=$(UNICODE) RELEASE=$(RELEASE) $(FLAGS)
