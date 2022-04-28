[中文](README.md) ||  [English](README.en.md)  
# XEngine_ServiceFramework

## Introduction
c c++网络后台服务器开发框架 
c c++ network backend server development framework  
It is a development framework code based on XEngine. This set of code uses the most basic code to demonstrate how to write a back-end server  
You can write your own backend server directly on the basis of this code  
The code has detailed comments, so you can easily understand how it works  

## Software feature
support feature list:  
1. use standard private protocol to handle network communication
2. Support http protocol
3. support http2 protocol
4. support websocket protocol
5. Support task pool
6. Support thread pool processing tasks
7. Support Log Management

## install

#### Requirements
support system above windows 7sp1 and linux(ubuntu20.04,centos8)  
XEngine need V7.25 or above  

## complie

#### XEngine environment
you have to install xengine before complie,You can refer to its Readme document  
GITEE:https://gitee.com/xyry/libxengine  
GITHUB:https://github.com/libxengine/xengine  

#### Windows
use vs open and complie  

#### Linux
Linux use Makefile complie  
UBUNTU20.04 x64 or CENTOS8 x64  

##### Screenshot of the program
execute command in XEngine_Source path   
make complie  
make FLAGS=InstallAll Install File  
make FLAGS=CleanAll Clear Complie  

## How to develop
XEngine_CenterApp service is mainly used for TCP protocol, you can write your TCP(Business) operation related processing code here  
XEngine_HttpApp service is mainly used for HTTP protocol, you can write your HTTP operation related processing code here  
XEngine_Http2App service is mainly used for HTTP2 protocol, you can write your HTTP2 operation related processing code here  
XEngine_WebSocketApp service is mainly used for Websocket protocol, you can write your Websocket operation related processing code here  

## Participate in contribution

1.  Fork this code
2.  Create new Feat_xxx branch
3.  Submit the code
4.  New Pull Request

## Follow us
If you think this software is helpful to you, please give us a START

## post issues

if you have eny quest.post issues...