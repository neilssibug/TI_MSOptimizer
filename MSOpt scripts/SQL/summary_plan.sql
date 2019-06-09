--summary_plan
--header
--id,Tester,TesterConfig,PackageSize,Initial_LoadBoard,Optimizer_LoadBoard,N_Lots,SUM_QTY,ChangeOver,Hours,PKGTYPE,PIN,CAP_PER_DAY,WIP_POST_WB,WIP_FT1,WIP_FT2,POST_WB_QUEUE,TEST_QUEUE_FT1,TEST_QUEUE_FT2,TOTAL_WIP

create table summary_plan
(
id 					int(11) NOT NULL AUTO_INCREMENT,
Tester				varchar(45) DEFAULT NULL,
TesterConfig		varchar(45) DEFAULT NULL,
PackageSize			varchar(45) DEFAULT NULL,
Initial_LoadBoard	varchar(45) DEFAULT NULL,
Optimizer_LoadBoard	varchar(45) DEFAULT NULL,
N_Lots				varchar(45) DEFAULT NULL,
SUM_QTY				varchar(45) DEFAULT NULL,
ChangeOver			varchar(45) DEFAULT NULL,
Hours				varchar(45) DEFAULT NULL,
PKGTYPE				varchar(45) DEFAULT NULL,
PIN					varchar(45) DEFAULT NULL,
CAP_PER_DAY			varchar(45) DEFAULT NULL,
WIP_POST_WB			varchar(45) DEFAULT NULL,
WIP_FT1				varchar(45) DEFAULT NULL,
WIP_FT2				varchar(45) DEFAULT NULL,
POST_WB_QUEUE		varchar(45) DEFAULT NULL,
TEST_QUEUE_FT1		varchar(45) DEFAULT NULL,
TEST_QUEUE_FT2		varchar(45) DEFAULT NULL,
TOTAL_WIP 			varchar(45) DEFAULT NULL,
PRIMARY KEY (id)
)ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1;