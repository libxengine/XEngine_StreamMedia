[中文](README.md) ||  [English](README.en.md)  
# XEngine_StreamMedia
本仓库有开发和主分支,如果要使用,请使用master分支下的代码  
只要仓库不是在暂停状态,那么就会有人一直维护和开发,请放心使用  

## 介绍
c c++ 流媒体服务器  
c c++ streammedia server  
这不是一个单纯的流媒体服务器,而是一个协议流解析推流服务,需要配合流媒体服务使用,他的主要作用是解析国标协议和其他协议的流媒体协议,然后打包推流到指定流媒体服务器上  
可以支持JT1078,GB28181等协议流.解析这些协议流,然后通过RTMP协议推流到一个流媒体服务器上面.用户可以通过流媒体服务器拉流  
他是一个流媒体协议解析集合,可以接入任何协议的设备到我们的服务器,然后通过我们服务器解析打包成RTMP协议推流到指定服务器上面
这是一个跨平台简单高性能网络授权服务器,提供了完整的演示代码  

## 软件架构
基于libXEngine开发并实现的一套简洁高性能跨平台流媒体解析推流服务  
软件特性:  
1. 支持语音对讲
2. 支持多种音频格式转码
3. 支持录像
4. 支持JT1078协议解析和推流
5. 支持国标GB28181协议的解析和推流
6. 支持国际ONVIF协议
7. 支持SIP流协议
8. 支持设备厂商SDK(dahua,haikang,suobei等等)

## 安装教程

#### XEngine环境
在编译之前必须安装XEngine,可以参考其Readme文档  
GITEE:https://gitee.com/xyry/libxengine  
GITHUB:https://github.com/libxengine/xengine  

#### 版本需求
支持WINDOWS 7SP1和LINUX(UBUNT20.04,CENTOS8)以上系统  
XEngine版本需要V7.34或者以上版本  

#### Windows
使用VS打开并且编译,你需要优先配置XEngine环境才能使用

#### Linux
Linux使用Makefile编译  
UBUNTU20.04 x64或者CENTOS8 x64均可  

#### MacOS
使用makefile编译,控制台运行,需要mac 12以及以上版本

###### 编译命令
在XEngine_Source目录下执行命令  
make 编译  
make FLAGS=InstallAll 安装库程序  
make FLAGS=CleanAll 清理编译  

## 使用说明

1.  切换到MASTER分支
2.  下载代码
3.  编译
4.  安装
5.  运行XEngine_CenterApp.exe,在运行XEngine_SDKApp.exe或者XEngine_JT1078App.exe,linux可以使用 -d 1 命令在后台运行
7.  使用APPClient中的推流例子推流
8.  使用VLC等工具播放

#### Linux
如果在Linux下运行,你可以使用service脚本运行,需要先修改release目录中的.service脚本中的ExecStart和WorkingDirectory路径.  
接着拷贝service脚本到/usr/lib/systemd/system/目录,然后执行systemctl start 服务名,还可以设置自启动,systemctl enable 服务名  
需要先启动XEngine_CenterApp.exe(XStreamCenter.service服务),在启动对应的服务,比如:XEngine_SDKApp(XStreamSdk.service)  

## 目录结构
- XEngine_Docment 文档目录结构
- XEngine_Release 安装目录结构
- XEngine_Source 源代目录结构
- XEngine_APPClient 客户端演示代码
- XEngine_SQLFile 数据库脚本文件

## 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request

## 关注我们
如果你觉得这个软件对你有帮助,请你给我们一个START吧

## 提交问题
如果你有问题或者发现程序有BUG和意见,可以在issues中提交