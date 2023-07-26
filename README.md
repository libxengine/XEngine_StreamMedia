[中文](README.md) ||  [English](README.en.md)  
# XEngine_StreamMedia
本仓库有开发和主分支,如果要使用,请使用master分支下的代码  
只要仓库不是在暂停状态,那么就会有人一直维护和开发,请放心使用  

## 介绍
c c++ 流媒体服务器  
c c++ streammedia server  
这是一个支持多种流协议的流媒体服务器,他支持推流和拉流,支持各种主流标准的流媒体服务器
他还可以作为抽帧服务和转码服务使用.
这是一个跨平台的流媒体服务器,提供了完整的演示代码   

## 软件架构
基于libXEngine开发并实现的一套简洁高性能跨平台流媒体解析推流服务  
推流支持:
1. RTMP(H264,H265)
2. XStream(H264,H265) 
3. WEBRTC(planning)
4. GB28181(planning)
5. JT1078(planning)
6. SRT(planning)  
拉流支持:
1. RTSP(planning)
2. RTMP(H264,H265)
3. FLV(H264,H265)
4. HLS(TS)(planning)
5. webrtc(planning)
6. srt(planning)  
特性:  
1. 支持语音对讲
2. 支持视频转码
3. 支持抽帧
4. 支持转推流
5. 支持透传数据

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
5.  运行XEngine_StreamMediaApp,linux可以使用 -d 1 命令在后台运行
7.  使用obs推流
8.  使用VLC等工具播放

## 示例
推流地址:rtmp://127.0.0.1/live/qyt  
播放地址:HTTP-FLV http://127.0.0.1:5600/api?stream=play&sms=live/qyt&type=flv
播放地址:RTMP     rtmp://127.0.0.1/live/qyt  
## 目录结构
- XEngine_Docment 文档目录结构
- XEngine_Release 安装目录结构
- XEngine_Source 源代目录结构
- XEngine_APPClient 客户端演示代码

## 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request

## 关注我们
如果你觉得这个软件对你有帮助,请你给我们一个START吧

## 提交问题
如果你有问题或者发现程序有BUG和意见,可以在issues中提交