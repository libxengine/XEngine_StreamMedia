/*
 Navicat Premium Data Transfer

 Source Server         : Test
 Source Server Type    : MySQL
 Source Server Version : 80028
 Source Host           : 192.168.1.12:3306
 Source Schema         : StreamMedia_JT1078

 Target Server Type    : MySQL
 Target Server Version : 80028
 File Encoding         : 65001

 Date: 28/04/2022 16:23:17
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for DeviceList
-- ----------------------------
DROP TABLE IF EXISTS `DeviceList`;
CREATE TABLE `DeviceList`  (
  `ID` int NOT NULL AUTO_INCREMENT,
  `tszDeviceAddr` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '设备地址',
  `tszDeviceNumber` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '设备编号',
  `nChannel` int NOT NULL COMMENT '通道号',
  `xhClient` bigint NOT NULL COMMENT '客户端句柄',
  `bLive` tinyint NOT NULL COMMENT '直播还是录像',
  `tszVersion` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '设备版本2014,2016?',
  `CreateTime` datetime NULL DEFAULT NULL COMMENT '创建时间',
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Table structure for DeviceList_AVInfo
-- ----------------------------
DROP TABLE IF EXISTS `DeviceList_AVInfo`;
CREATE TABLE `DeviceList_AVInfo`  (
  `ID` int NOT NULL AUTO_INCREMENT,
  `tszDeviceNumber` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '设备编号',
  `bVideo` tinyint NOT NULL COMMENT '是否启用视频',
  `nVideoBit` bigint NULL DEFAULT NULL COMMENT '码率',
  `enVCodec` int NULL DEFAULT NULL COMMENT '音频编码器',
  `nWidth` int NULL DEFAULT NULL COMMENT '视频宽',
  `nHeight` int NULL DEFAULT NULL COMMENT '视频高',
  `nFrameRate` int NULL DEFAULT NULL COMMENT '帧率',
  `bAudio` tinyint NOT NULL COMMENT '是否启用音频',
  `nAudioBit` bigint NULL DEFAULT NULL COMMENT '码率',
  `enACodec` int NULL DEFAULT NULL COMMENT '音频编码器',
  `nChannel` int NULL DEFAULT NULL COMMENT '通道号',
  `nSampleRate` int NULL DEFAULT NULL COMMENT '采样率',
  `nStreamBitRate` int NULL DEFAULT NULL COMMENT '采样格式',
  `nFrameSize` int NULL DEFAULT NULL COMMENT '采样大小',
  `TimeUPdate` datetime NULL DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP COMMENT '更新时间',
  `TimeCreate` datetime NULL DEFAULT NULL COMMENT '插入时间',
  PRIMARY KEY (`ID`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = DYNAMIC;

SET FOREIGN_KEY_CHECKS = 1;
