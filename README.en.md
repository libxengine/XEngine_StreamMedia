[中文](README.md) ||  [English](README.en.md)  
# XEngine_StreamMedia
This repository has a development and master branch. If you want to use it, please use the master branch  
As long as the repository is not in suspended state, we will maintain and develop it all the time, please use it with confidence  

## Introduction
c c++ 流媒体服务器  
c c++ streammedia server  
This is not a simple streaming media server, but a protocol stream parsing push streaming service, need to be used with the streaming service, his main role is to parse the national standard protocol and other protocols of the streaming media protocol, and then packaged to push the stream to the specified streaming media server  
This service is a supplementary service between streaming media services and devices. It is convenient for users to better manage streaming  
can support JT1078, GB28181 and other protocol streams. Parsing these protocol streams, and then pushing them to a streaming server through RTMP protocol. Users can pull streams through the streaming server.  
It is a streaming protocol parsing collection, which can access any protocol devices to our server, and then packaged into RTMP protocol by our server to push the stream to the specified server.  
This is a cross-platform simple high-performance network licensing server, provides a complete demo code  

## Design Patterns
Control the device to stream to our server through XEngine_StreamMedia, push the stream to the third-party streaming service through our server, and the client to the third-party streaming service to pull the stream  
This service mainly provides standard interfaces for users to conveniently manage push-pull streams  
Support all standard streaming media servers, such as: nginx-rtmp AMS red5 etc  
device -> XEngine_StreamMedia -> stream media service -> client  

## Software feature
A set of concise and high-performance cross-platform streaming media analysis and push streaming services developed and implemented based on libXEngine  
1. Support voice intercom
2. Support multiple audio format transcoding
3. Support video recording
4. Support JT1078 protocol parsing and streaming
5. Support the parsing and streaming of the national standard GB28181 protocol
6. Support international ONVIF protocol
7. Support SIP streaming protocol
8. support rtsp,rtmp,hls convert push stream

## install

#### XEngine environment
you have to install xengine before complie,You can refer to its Readme document  
GITEE:https://gitee.com/xyry/libxengine  
GITHUB:https://github.com/libxengine/xengine  

#### Version requirements
Support WINDOWS 7SP1 and LINUX (UBUNT20.04, CENTOS8) above system  

#### Windows
use vs open and complie  

#### Linux
Linux use Makefile complie  
UBUNTU20.04 x64 or CENTOS8 x64  

#### MacOS
Linux use Makefile to complie,terminal exection,Requires mac 12 and above 

##### Compile Command
execute command in XEngine_Source path   
make complie  
make FLAGS=InstallAll Install File  
make FLAGS=CleanAll Clear Complie  

## Use

1.  Switch to the MASTER branch
2.  download code
3.  complie
4.  install
5.  run XEngine_CenterApp.exe,and running XEngine_SDKApp.exe or XEngine_JT1078App.exe,linux can be set parament -d 1 command
6.  use APPClient push stream
7.  use vlc play


## Dir Struct
- XEngine_Docment Docment Dir Struct
- XEngine_Release Install Dir Struct
- XEngine_Source Source Dir
- XEngine_APPClient Client Example Code
- XEngine_SQLFile Sql File

## Participate in contribution

1.  Fork this code
2.  Create new Feat_xxx branch
3.  Submit the code
4.  New Pull Request

## Follow us
If you think this software is helpful to you, please give us a START

## post issues

if you have eny quest.post issues...