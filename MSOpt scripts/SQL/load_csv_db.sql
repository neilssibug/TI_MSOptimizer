LOAD DATA INFILE 'C:/wamp/bin/mysql/mysql5.6.17/data/test_opt/wip.csv' 
INTO TABLE wip 
FIELDS TERMINATED BY ',' 
LINES TERMINATED BY '\n'
IGNORE 1 ROWS
(lot, device, package, pin, cur_qty, weight, lot_name, lpt, opn, total_plan_ct, cum_plan_ct, lot_age, @start_time, @snapshoot_time, machine_instance)
SET start_time = CASE WHEN STR_TO_DATE(@start_time, '%m/%d/%Y %h:%i:%s') < STR_TO_DATE('01/01/1999', '%m/%d/%Y %h:%i:%s') then null else STR_TO_DATE(@start_time, '%m/%d/%Y %h:%i:%s') end, 
snapshot_time = STR_TO_DATE(@snapshoot_time, '%m/%d/%Y %h:%i:%s');