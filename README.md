[中文](README.md) ||  [English](README.en.md)  
# XEngine_StreamMedia
本仓库有开发和主分支,如果要使用,请使用master分支下的代码  
只要仓库不是在暂停状态,那么就会有人一直维护和开发,请放心使用  

## 介绍
c c++ 流媒体服务器  
c c++ streammedia server  
这不是一个单纯的流媒体服务器,而是一个协议流解析推流服务,需要配合流媒体服务使用,他的主要作用是解析国标协议和其他协议的流媒体协议,然后打包推流到指定流媒体服务器上  
这个服务是流媒体服务和设备之间的一个补充服务.方便用户更好的管理推流  
可以支持JT1078,GB28181等协议流.解析这些协议流,然后通过RTMP协议推流到一个流媒体服务器上面.用户可以通过流媒体服务器拉流  
他是一个流媒体协议解析集合,可以接入任何协议的设备到我们的服务器,然后通过我们服务器解析打包成RTMP协议推流到指定服务器上面
这是一个跨平台简单高性能网络授权服务器,提供了完整的演示代码  

## 设计模式
通过XEngine_StreamMedia控制设备流到我们的服务器,然后通过我们服务器推流到第三方流媒体服务,客户端去第三方流媒体服务拉流  
此服务主要提供标准接口给用户方便的管理推拉流  
支持所有标准流媒体服务器,比如:nginx-rtmp AMS red5 等等  
device -> XEngine_StreamMedia -> stream media service -> client  

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
8. 支持RTSP,RTMP,HLS等转推流


## 安装教程

#### XEngine环境
在编译之前必须安装XEngine,可以参考其Readme文档  
GITEE:https://gitee.com/xyry/libxengine  
GITHUB:https://github.com/libxengine/xengine  

#### 快速部署
git clone https://gitee.com/xyry/libxengine.git 或者 git clone https://github.com/libxengine/xengine.git
window执行XEngine_WINEnv.bat 脚本.
Linux执行:sudo ./XEngine_LINEnv.sh -i 3
macos执行:./XEngine_LINEnv.sh -i 3

#### Windows
使用VS打开并且编译,你需要优先配置XEngine环境才能使用

#### Linux
Linux使用Makefile编译  
UBUNTU22.04 或者 RockyLinux 9 x64均可  

###### 编译命令
在XEngine_Source目录下执行命令  
make 编译  
make FLAGS=InstallAll 安装库程序  
make FLAGS=CleanAll 清理编译  

#### MacOS
使用makefile编译,控制台运行,需要mac 13以及以上版本

###### 编译命令
参考Linux


## 使用说明

1.  切换到MASTER分支
2.  下载代码
3.  编译
4.  安装
5.  运行XEngine_CenterApp.exe,XEngine_JT1078App.exe,linux可以使用 -d 1 命令在后台运行
7.  使用APPClient中的推流例子推流
8.  使用VLC等工具播放

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