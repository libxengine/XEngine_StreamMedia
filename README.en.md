[中文](README.md) ||  [English](README.en.md)  
# XEngine_StreamMedia
This repository has a development and master branch. If you want to use it, please use the master branch  
As long as the repository is not in suspended state, we will maintain and develop it all the time, please use it with confidence  

## Introduction
c c++ 流媒体服务器  
c c++ streammedia server  
This is a streaming media server that supports multiple streaming protocols. It supports push and pull stream, and supports streaming media servers of various standards protocols
It can also be used as a frame extraction service and transcoding service  
This is a cross-platform server, provides a complete demo code  

## Software feature
A set of concise and high-performance cross-platform streaming media analysis and push streaming services developed and implemented based on libXEngine  

feature:  
1. Support voice intercom(planning)
2. Support video transcoding(planning)
3. Support frame extraction(planning)
4. Support Re-push stream 
5. Support Transparent transmission(planning)
6. Support HTTP API Management interface
7. Support HTTP HOOK(planning)
8. Support for the transfer of all streaming media protocols
9. Support visual FILTER filter processing (planning)
10. Support video AI effect processing (planning)
11. Support text language transcription (planning)
12. Support recording (planning)
13. Support audio and video recoding (planning)
14. Support pre-pull stream

Publish Stream:

| ProtocolName | VideoCodec  | AudioCodec | Status |
| -------- | --------- | -------- | -------- |
| RTMP     | H264      | AAC      | Completed   |
| SRT      | H264      | AAC      | Completed   |
| WEBRTC   | H264      | OPUS     | Not Start   |
| GB28181  | H264      | AAC      | Not Start   |
| JT1078   | H264      | AAC      | Completed   |
| XStream  | H264      | AAC      | Completed   |
  
Play Stream:

| ProtocolName | VideoCodec  | AudioCodec | Status |
| -------- | --------- | -------- | -------- |
| RTMP     | H264      | AAC      | Completed   |
| RTSP     | H264      | AAC      | Completed   |
| M3U8     | H264      | AAC      | Completed   |
| SRT      | H264      | AAC      | Completed   |
| WEBRTC   | H264      | OPUS     | Processing  |
| HTTP-FLV | H264      | AAC      | Completed   |
| XStream  | H264      | AAC      | Completed   |
| TS       | H264      | AAC      | Completed   |

## install

#### XEngine environment
you have to install xengine before complie,You can refer to its Readme document  
GITEE:https://gitee.com/libxengine/libxengine  
GITHUB:https://github.com/libxengine/libxengine  

#### fast to deployment
git clone https://gitee.com/libxengine/libxengine.git or git clone https://github.com/libxengine/libxengine.git  
window Exection XEngine_WINEnv.bat  
Linux Exection:sudo ./XEngine_LINEnv.sh -i 3  
Macos Exection:./XEngine_LINEnv.sh -i 3  

#### sub module
Due to the dependent sub-modules, after you checkout the resprepository, execute the following command in the resprepository directory to pull the sub-modules  
git submodule init  
git submodule update  

#### Windows
use vs open and complie,you need to install vcpkg and install depend library by vcpkg  
vcpkg.exe install libsrt:x86-windows libsrt:x64-windows libsrtp:x86-windows libsrtp:x64-windows
#### Linux
install srt library after Configure Xengine to Completed  
ubuntu:sudo apt install libsrt-gnutls-dev libsrtp2-dev  

Linux use Makefile complie  
UBUNTU22.04 or above  

##### Compile Command
execute command in XEngine_Source path   
make  
make FLAGS=InstallAll  
make FLAGS=CleanAll  

#### MacOS
install srt library after Configure Xengine to Completed  
brew install srt srtp  
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
#### publish Stream
rtmp://127.0.0.1/live/qyt  
srt://127.0.0.1:5603?streamid=#!::h=live/qyt,m=publish  
xstream://127.0.0.1:5601

#### Pull Stream
http://127.0.0.1:5600/api?stream=play&sms=live/qyt&type=flv(http-flv)  
http://127.0.0.1:5600/api?stream=play&sms=live/qyt&type=ts  
rtsp://127.0.0.1:5600/api?stream=play&sms=live/qyt&type=rtsp  
rtmp://127.0.0.1/live/qyt  
srt://127.0.0.1:5603?streamid=#!::r=live/qyt,m=request  
http://127.0.0.1:5601?api?stream=play&sms=live/qyt&type=xstream(http-xstream)

#### Management HTTP API
http://127.0.0.1:5600/api?function=publish&method=get  
http://127.0.0.1:5600/api?function=pull&method=get

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

如果你觉得这个软件对你有帮助,请你给我们一个START吧  
也可以通过微信关注我们  
![qrcode](https://www.xyry.org/qrcode.jpg)

## post issues

if you have eny quest.post issues...