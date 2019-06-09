-- Fix Route Issue

use optimizer;

DROP TABLE IF EXISTS `route_temp`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `route_temp` (
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
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM AUTO_INCREMENT=1626 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


TRUNCATE route_temp;

INSERT INTO route_temp (RouteName, logpoint, operation,
                lpt_desc, opn_desc, device, package,
    pin, alternate, setup, pph, Machine_Family,
    tooling_family, toolqty,Certification, id)
SELECT r.RouteName, r.logpoint, r.operation,
                r.lpt_desc, r.opn_desc, r.device, r.package,
    r.pin, r.alternate, r.setup, r.pph, r.Machine_Family,
    r.tooling_family, r.toolqty,
    CASE
                                WHEN ISNULL(temp_cert) then r.certification
        ELSE temp_cert
    END AS certification,
    r.id
FROM
                (select @final_cert:=0) t,
    route r 
    LEFT JOIN
                (
    SELECT distinct tmp_cnt, temp_cert, machine_family, Initial_Tool_Family, logpoint, id
                FROM (SELECT @prev_inst as prev_inst,  
                                                CASE
                                                                WHEN @prev_inst <> init.machine_instance THEN @temp_cnt:=r.cnt
                                                                WHEN @prev_inst = init.Machine_Instance THEN
                                                                CASE
                                                                                WHEN @temp_cnt <= r.cnt THEN @temp_cnt:=r.cnt
                                                                                WHEN @temp_cnt > r.cnt THEN @temp_cnt
                                                                END
                                                END as tmp_cnt,
                                                CASE
                                                                WHEN @prev_inst <> init.machine_instance THEN @temp_cert:=init.initial_certification
                                                                WHEN @prev_inst = init.machine_instance THEN
                                                                CASE
                                                                                WHEN @temp_cnt <= r.cnt THEN @temp_cert:=init.Initial_Certification
                                                                                WHEN @temp_cnt > r.cnt THEN @temp_cert
                                                                END
                                                END AS temp_cert,
                                                CASE
                                                                WHEN @prev_inst = init.Machine_Instance THEN 'T'
                                                                ELSE 'F'
                                                END AS has_issue,
                                                @prev_inst:=init.machine_instance as machine_instance, init.machine_family,
                                                init.initial_tool_family, r.certification, r.logpoint, r.cnt, r.id
                                FROM
                                                (SELECT @prev_inst:='', @temp_cert:=0) t,
                                                (SELECT 
                                                                init.machine_family, init.machine_instance AS machine_instance,
                                                                                init.machine_family_name,
                                                                                init.Initial_Tool_Family,
                                                                                init.Initial_Certification
                                                FROM initialsetup init
                                                ORDER BY machine_instance) init
                                                                JOIN
                                                (SELECT *, @temp_cnt:=r.cnt FROM
                                                                (
                                                                SELECT machine_family, tooling_family, certification, logpoint, COUNT(*) as cnt, id
                                                                FROM route
                                                                WHERE logpoint = 4410
                                                                GROUP BY machine_family , tooling_family , certification, logpoint
                                                                ) r
                                                ) r ON init.machine_family = r.machine_family
                                                                AND init.initial_tool_family = r.tooling_family
                                ) init_r
                WHERE has_issue = 'T') init_r
    on r.machine_family = init_r.machine_family
                                AND r.tooling_family = init_r.Initial_Tool_Family
        AND r.logpoint = init_r.logpoint
    ORDER BY id;


select * from route_temp;


-- insert into route from route_temp
TRUNCATE route;
INSERT INTO route (RouteName, logpoint, operation,
                lpt_desc, opn_desc, device, package,
    pin, alternate, setup, pph, Machine_Family,
    tooling_family, toolqty,Certification, id)
    ;
SELECT * FROM route_temp;
