-- MySQL dump 10.13  Distrib 5.6.24, for Win32 (x86)
--
-- Host: 127.0.0.1    Database: toptimizer
-- ------------------------------------------------------
-- Server version	5.6.17

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `route`
--

DROP TABLE IF EXISTS `route`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `route` (
  `RouteName` varchar(20) DEFAULT NULL,
  `Logpoint` int(4) DEFAULT NULL,
  `Operation` int(1) DEFAULT NULL,
  `LPT_Desc` varchar(20) DEFAULT NULL,
  `Opn_Desc` varchar(20) DEFAULT NULL,
  `Device` varchar(20) DEFAULT NULL,
  `Package` varchar(20) DEFAULT NULL,
  `Pin` varchar(20) DEFAULT NULL,
  `alternate` varchar(3) DEFAULT NULL,
  `Setup` varchar(40) DEFAULT NULL,
  `PPH` int(4) DEFAULT NULL,
  `Machine_Family` varchar(30) DEFAULT NULL,
  `Tooling_Family` varchar(30) DEFAULT NULL,
  `ToolQTY` int(1) DEFAULT NULL,
  `Certification` int(1) DEFAULT NULL,
  `ID` bigint(20) NOT NULL AUTO_INCREMENT,
  `PKG_SIZE` varchar(20) DEFAULT NULL,
  `FLOW` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM AUTO_INCREMENT=1626 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `solution`
--

DROP TABLE IF EXISTS `solution`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `solution` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `PKG_SIZE` varchar(20) DEFAULT NULL,
  `FLOW` varchar(20) DEFAULT NULL,
  `Machine_Instance` varchar(45) DEFAULT NULL,
  `Machine_Family_Name` varchar(45) DEFAULT NULL,
  `Lot_Name` varchar(45) DEFAULT NULL,
  `Logpoint` varchar(45) DEFAULT NULL,
  `Quantity` varchar(45) DEFAULT NULL,
  `Lot_Weight` varchar(45) DEFAULT NULL,
  `Initial_Lot_Flag` varchar(45) DEFAULT NULL,
  `Device_Name` varchar(45) DEFAULT NULL,
  `Tooling_Family_Name` varchar(45) DEFAULT NULL,
  `Certification` varchar(45) DEFAULT NULL,
  `Setup_Time` varchar(45) DEFAULT NULL,
  `Start_Time` varchar(45) DEFAULT NULL,
  `Completion_Time` varchar(45) DEFAULT NULL,
  `Pass_no` varchar(45) DEFAULT NULL,
  `Finished` varchar(45) DEFAULT NULL,
  `package` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1214 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wip`
--

DROP TABLE IF EXISTS `wip`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wip` (
  `LOT` int(7) DEFAULT NULL,
  `Device` varchar(14) DEFAULT NULL,
  `Package` varchar(20) DEFAULT NULL,
  `Pin` varchar(20) DEFAULT NULL,
  `CUR_QTY` int(10) DEFAULT NULL,
  `Weight` decimal(10,2) DEFAULT NULL,
  `LOT_name` int(7) DEFAULT NULL,
  `LPT` int(4) DEFAULT NULL,
  `OPN` int(1) DEFAULT NULL,
  `TOTAL_PLAN_CT` varchar(20) DEFAULT NULL,
  `CUM_PLAN_CT` varchar(20) DEFAULT NULL,
  `LOT_AGE` varchar(20) DEFAULT NULL,
  `START_TIME` varchar(30) DEFAULT NULL,
  `SNAPSHOOT_TIME` varchar(30) DEFAULT NULL,
  `MACHINE_INSTANCE` varchar(20) DEFAULT NULL,
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `PKG_SIZE` varchar(20) DEFAULT NULL,
  `FLOW` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `index_UNIQUE` (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2961 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wip_cut`
--

DROP TABLE IF EXISTS `wip_cut`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wip_cut` (
  `LOT` int(7) DEFAULT NULL,
  `Device` varchar(14) DEFAULT NULL,
  `Package` varchar(20) DEFAULT NULL,
  `Pin` varchar(20) DEFAULT NULL,
  `CUR_QTY` int(10) DEFAULT NULL,
  `Weight` decimal(10,2) DEFAULT NULL,
  `LOT_name` int(7) DEFAULT NULL,
  `LPT` int(4) DEFAULT NULL,
  `OPN` int(1) DEFAULT NULL,
  `TOTAL_PLAN_CT` varchar(20) DEFAULT NULL,
  `CUM_PLAN_CT` varchar(20) DEFAULT NULL,
  `LOT_AGE` varchar(20) DEFAULT NULL,
  `START_TIME` varchar(20) DEFAULT NULL,
  `SNAPSHOOT_TIME` varchar(30) DEFAULT NULL,
  `MACHINE_INSTANCE` varchar(20) DEFAULT NULL,
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `PKG_SIZE` varchar(20) DEFAULT NULL,
  `FLOW` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `index_UNIQUE` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4096 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping events for database 'toptimizer'
--

--
-- Dumping routines for database 'toptimizer'
--
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-12-22 15:22:50
