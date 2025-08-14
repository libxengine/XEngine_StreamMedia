[中文](README.md) ||  [English](README.en.md)  

# XEngine_StreamMedia

本仓库有开发和主分支,如果要使用,请使用master分支下的代码  
只要仓库不是在暂停状态,那么就会有人一直维护和开发,请放心使用

## 介绍

c c++ 流媒体服务器  
c c++ streammedia server  
这是一个支持多种流协议的流媒体服务器,他支持推流和拉流,支持各种主流标准协议的流媒体服务器  
他还可以作为抽帧服务和转码服务使用  
这是一个跨平台的流媒体服务器,提供了完整的演示代码

## 软件架构
基于libXEngine开发并实现的一套简洁高性能跨平台流媒体解析推流服务  

特性:
1. 支持语音对讲(planning)
2. 支持视频转码(planning)
3. 支持抽帧(planning)
4. 支持转推流
5. 支持透传数据(planning)
6. 支持HTTP API管理接口
7. 支持HTTP HOOK(planning)
8. 支持全流媒体协议互转
9. 支持视FILTER滤镜处理(planning)
10. 支持视频AI效果处理(planning)
11. 支持文本语言转录(planning)
12. 支持录像(planning)
13. 支持音视频重编解码(planning)
14. 支持预拉流

推流协议:

| 协议名称 | 视频格式  | 音频格式 | 当前状态 |
| -------- | --------- | -------- | -------- |
| RTMP     | H264      | AAC      | 已完成   |
| SRT      | H264      | AAC      | 已完成   |
| WEBRTC   | H264      | OPUS     | 进行中   |
| GB28181  | H264      | AAC      | 未开始   |
| JT1078   | H264      | AAC      | 已完成   |
| XStream  | H264      | AAC      | 已完成   |
  
拉流协议:

| 协议名称 | 视频格式  | 音频格式 | 当前状态 |
| -------- | --------- | -------- | -------- |
| RTMP     | H264      | AAC      | 已完成   |
| RTSP     | H264      | AAC      | 已完成   |
| M3U8     | H264      | AAC      | 已完成   |
| SRT      | H264      | AAC      | 已完成   |
| WEBRTC   | H264      | OPUS     | 进行中   |
| HTTP-FLV | H264      | AAC      | 已完成   |
| XStream  | H264      | AAC      | 已完成   |
| TS       | H264      | AAC      | 已完成   |

## 安装教程

#### XEngine环境

在编译之前必须安装XEngine,可以参考其Readme文档  
GITEE:https://gitee.com/libxengine/libxengine  
GITHUB:https://github.com/libxengine/libxengine

#### 快速部署

git clone https://gitee.com/libxengine/libxengine.git 或者 git clone https://github.com/libxengine/libxengine.git  
window执行XEngine_WINEnv.bat 脚本  
Linux执行:sudo ./XEngine_LINEnv.sh -i 3  
macos执行:./XEngine_LINEnv.sh -i 3  

#### sub module
由于依赖的子模块,在你checkout仓库后,在仓库目录下执行下面的命令拉取子模块  
git submodule init  
git submodule update  
如果github访问失败,你也可以clone该项目,在主目录下使用命令:git clone https://gitee.com/xengine/XEngine_OPenSource.git XEngine_Source/XEngine_DependLibrary

#### Windows

使用VS打开并且编译,你需要优先配置XEngine环境才能使用  
还需要libsrt库,通过vcpkg安装  
vcpkg.exe install libsrt:x86-windows libsrt:x64-windows libsrtp:x86-windows libsrtp:x64-windows

#### Linux
配置完XEngine后需要安装srt库  
ubuntu:sudo apt install libsrt-gnutls-dev libsrtp2-dev  

Linux使用Makefile编译  
UBUNTU22.04或者以上版本  

###### 编译命令

在XEngine_Source目录下执行命令  
make 编译  
make FLAGS=InstallAll 安装库程序  
make FLAGS=CleanAll 清理编译

#### MacOS
配置完XEngine后需要安装srt库  
brew install srt srtp  
使用makefile编译,控制台运行,需要mac 13以及以上版本

###### 编译命令

参考Linux

## 视频教程
你也可以观看视频教程来了解我们的代码框架情况:https://www.bilibili.com/video/BV17F4m1M7Ph

## 使用说明

1. 切换到MASTER分支
2. 下载代码
3. 编译
4. 安装
5. 运行XEngine_StreamMediaApp,linux可以使用 -d 1 命令在后台运行
6. 使用obs推流
7. 使用VLC等工具播放

## 示例

#### 推流示例
rtmp://127.0.0.1/live/qyt  
srt://127.0.0.1:5603?streamid=#!::h=live/qyt,m=publish  
xstream://127.0.0.1:5601

#### 拉流示例
http://127.0.0.1:5600/api?stream=play&sms=live/qyt&type=flv(http-flv)  
http://127.0.0.1:5600/api?stream=play&sms=live/qyt&type=ts  
rtsp://127.0.0.1:5600/api?stream=play&sms=live/qyt&type=rtsp  
rtmp://127.0.0.1/live/qyt  
srt://127.0.0.1:5603?streamid=#!::r=live/qyt,m=request  
http://127.0.0.1:5601?api?stream=play&sms=live/qyt&type=xstream(http-xstream)

#### 管理接口示例
http://127.0.0.1:5600/api?function=publish&method=get  
http://127.0.0.1:5600/api?function=pull&method=get

## 目录结构

- XEngine_Docment 文档目录结构
- XEngine_Release 安装目录结构
- XEngine_Source 源代目录结构
- XEngine_APPClient 客户端演示代码

## 参与贡献

1. Fork 本仓库
2. 新建 Feat_xxx 分支
3. 提交代码
4. 新建 Pull Request

## 关注我们

如果你觉得这个软件对你有帮助,请你给我们一个START吧  
也可以通过微信关注我们  
![qrcode](https://www.xyry.org/qrcode.jpg)

## 提交问题

如果你有问题或者发现程序有BUG和意见,可以在issues中提交