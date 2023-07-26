[中文](README.md) ||  [English](README.en.md)  
# XEngine_StreamMedia
This repository has a development and master branch. If you want to use it, please use the master branch  
As long as the repository is not in suspended state, we will maintain and develop it all the time, please use it with confidence  

## Introduction
c c++ 流媒体服务器  
c c++ streammedia server  
This is a streaming media server that supports multiple streaming protocols. It supports push and pull stream, and supports streaming media servers of various standards  
It can also be used as a frame extraction service and transcoding service  
This is a cross-platform server, provides a complete demo code  

## Software feature
A set of concise and high-performance cross-platform streaming media analysis and push streaming services developed and implemented based on libXEngine  
Push stream Protocol:
1. RTMP
2. XStream(H264,H265)
3. WEBRTC(planning)
4. GB28181(planning)
5. JT1078(planning)
6. SRT(planning)
Pull Stream Protocols:  
1. RTSP(planning)
2. RTMP(H264,H265)
3. FLV(H264,H265)
4. HLS(TS)(planning)
5. webrtc(planning)
6. srt(planning)  
feature  
1. Support voice intercom
2. Support video transcoding
3. Support frame extraction
4. Support Re-push stream 
5. Support Transparent transmission
## install

#### XEngine environment
you have to install xengine before complie,You can refer to its Readme document  
GITEE:https://gitee.com/xyry/libxengine  
GITHUB:https://github.com/libxengine/xengine  

#### fast to deployment
git clone https://gitee.com/xyry/libxengine.git or git clone https://github.com/libxengine/xengine.git
window Exection XEngine_WINEnv.bat
Linux Exection:sudo ./XEngine_LINEnv.sh -i 3
Macos Exection:./XEngine_LINEnv.sh -i 3

#### Windows
use vs open and complie  

#### Linux
Linux use Makefile complie  
UBUNTU22.04 x64 or RockyLinux 9 x64  

##### Compile Command
execute command in XEngine_Source path   
make complie  
make FLAGS=InstallAll Install File  
make FLAGS=CleanAll Clear Complie  

#### MacOS
Linux use Makefile to complie,terminal exection,Requires mac 13 and above 

##### Compile Command
refer to linux

## Use

1.  Switch to the MASTER branch
2.  download code
3.  complie
4.  install
5.  run XEngine_StreamMediaApp,linux can be set parament -d 1 command
6.  use obs push stream
7.  use vlc play

## example 
Publish Stream Address:rtmp://127.0.0.1/live/stream  
Pull Stream Address-HTTPFLV: http://127.0.0.1:5600/api?stream=play&sms=live/qyt&type=flv
Pull Stream Address-RTMP:    rtmp://127.0.0.1/live/qyt  
## Dir Struct
- XEngine_Docment Docment Dir Struct
- XEngine_Release Install Dir Struct
- XEngine_Source Source Dir
- XEngine_APPClient Client Example Code

## Participate in contribution

1.  Fork this code
2.  Create new Feat_xxx branch
3.  Submit the code
4.  New Pull Request

## Follow us
If you think this software is helpful to you, please give us a START

## post issues

if you have eny quest.post issues...