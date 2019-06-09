#ifndef _AT_H

#define _AT_H



//#include<ilcplex/ilocplex.h>

//#include<ilconcert/ilolinear.h>

#include<iostream>

#include<sstream>

#include<cstring>

#include<string>

#include<fstream>

#include<math.h>

#include<algorithm>

#include<vector>

#include<map>
#include<stdlib.h>

#include<stdio.h>

#include<time.h>

#include <iomanip>

#include <cassert>

#include "combination.h"

#include "BCP_buffer.hpp"

#include "CoinPackedMatrix.hpp"

#include "CoinPackedVector.hpp"

#include "OsiSolverInterface.hpp"



using namespace std;



typedef vector<int>::iterator vii;



#ifdef COIN_USE_CPX

#include "OsiCpxSolverInterface.hpp"

#include "cplex.h"

typedef OsiCpxSolverInterface OsiXxxSolverInterface;

#endif

//#ifdef COIN_USE_CLP

#include "OsiClpSolverInterface.hpp"

typedef OsiClpSolverInterface OsiXxxSolverInterface;

//#endif

#ifdef COIN_USE_OSL

#include "OsiOslSolverInterface.hpp"

typedef OsiOslSolverInterface OsiXxxSolverInterface;

#endif



//Comment by Zhufeng Sep 05 ,2011. Special for yufen's cdoe. The route_check has been replaced by route

////added by Zhufeng for logpoint, Aug 30, 2011

//struct route_check //record route for the data consistency checking

//{

//	char device[50];

//    int logpoint;

//	int operation;

//};

//

////end by Zhufeng for logpoint

//end by Zhufeng Sep 05, 2011



//Added by Zhufeng, Dec 2013
struct multi_device_pass{
  //int dev_id; 
  vector<int> logpoint_id_vector;
  //vector<int> route_id_vector;
  int num_step;

};
//end by Zhufeng, Dec 2013
//Added by Zhufeng

struct route{

	//this is used to record the route after reading mahcines.csv, tooling.csv, when 

	//doing data consistency checking for "Data/route.csv".

	//It is designed for single line not multiple lines.

    int row_no;

	int logpoint;

	//added by Zhufeng for logpoint, Sep 04, 2011

	int operation;

	//end by Zhufeng for logpoint, Sep 04, 2011

	int mf_id;

	int tf_id;

	int certification;

	char device_name[50];

};

struct setup{

	//this is used to record the setup from initialsetup.csv

	//do data consistency checking for "Data/initialsetup.csv",

	//designed for single line

	int row_no;

	int mi_id;

	int mf_id;

	int tf_id;

	int certification;

};



struct tooling_set{

	vector<int> tools;

};

struct tooling_configure{

	struct tooling_set tf;

	int mg;

	int device;

	int id;

	int temperature;



	// --- added by yufen ---

	double PPH;

	// --- end yufen ---



	//added by Zhufeng for logppoint, Aug 30, 2011

	int logpoint;

	int operation;

	//end by Zhufeng

};





struct Tooling{

	char name[100];

	int tf;

	int id;

	vector<int> temperature_vector;

};

struct Machine{

	char name[100];

	int id;

	int mg;

	vector<int> temperature_vector;



	//------------- added by yufen ------------

	bool hasInitialLot;

	bool hasInitialSetup;	

	bool hasInitialToolingConfigure;



	int initial_lot;  //the id in the vector of initial_running_lot.note:initial_running_lot[initial_lot] is the initial lot on the machine. 

	int initial_tool; // toolFamilyId. Just think the initail tool is a single line

	int initial_temperature;

	int initial_tooling_setup;		//lambda_id. Lambda[lambda_id-1] is the tooling setup.

	int initial_tooling_configure;	//s_id, S[s_id-1] is the s

	//------------- end yufen -----------------

};

struct machine_set{

	vector<int> machines;

};

struct lot_set{

	vector<int> lots;

};

struct device_item{

	int device;

	vector<int> device_lots;

};

struct index_node{

	int index;

	int i;

	int l;

	int s;

	int Lambda_id;

};

struct t_set{

	vector<int> set_elements;

};

struct vec_int{

	vector<int> int_vector;

};

struct vec_tc{

	vector<tooling_configure> tc_vector;

};

struct equ_mach_set{

	int mg;

	int n;

	vector<int> equ_mach;

};

struct ch_array{

	char char_arry[100];

	int id;

};

struct Lambda_item{

	int id;

	vector<int> tooling_vec; //contains the tooling family

	vector<int> b;			// contains the number of piece

	int temperature;

};

struct machine_heur_item{

	vector<int> tooling;

	vector<int> b;

	vector<int> lot;

	double time_used;

	int temperature;

};

struct lot_heur_item{

	int l;

	int i;

	int s;

};

struct SIM_item{

	int simid;

	int represented_machine_id;

	int mg;

	char name[100];

	vector<int> temperature_vector;

	int num_machine;

};

struct sim_lambda_item{

	vector< pair<int,int> > sim_lambda_pairs;

};

struct sub_soln_item{//describe a i-lambda combination

	int i;

	int Lambda_id;

	// --- added by yufen

	int s;

	// --- end yufen ---

	double time_used;

	vec_int lots;

	//added by Zhufeng for multipass, Jan 1, 2012

	int which_pass;

	vec_int logpoints;

	//end by Zhufeng Jan 1, 2012



};

struct soln_pool_item{

	double obj;

	vector<sub_soln_item> sub_soln;

	vector< pair<int,double> > shortage;

	vector< pair< pair<int,double>,double> > unassigned_lots;

};





//Comment by Zhufeng for logpoint, Sep 01 ,2011



//struct S_sim_d_lambda_item{

//	int sim;

//	int d;

//	int Lambda_id;

//	int s;

//};



//end by Zhufeng for logpoint



//Added by Zhufeng for logpoint in Aug 31, 2011

struct S_sim_l_lambda_item{

	int sim;

	int l;

	int Lambda_id;

	int s;

};



//end Zhufeng

struct date_time{//this is a date and time format

	char str[50]; //this hold the information

	int MM;

	int DD;

	int YY;

	int hh;

	int mm;

	int ss;

};



struct initial_running_lot_item{

	char lot_name[50];

	char dev_name[50];

	char PKG_name[50];

	int  dev_id;

	int  pkg_id;

	int  PIN_num;

	int  n_l_chips;

	double w_l;

	struct date_time Start_time;

	struct date_time Snapshoot_time;

	char machine_name[50];

	int machine_id;

	double rate;

	double time_remain; //time_remain means how long this lot would be finished from the start of the time horizon



	//Added by Zhufeng for logpoint, Aug 17, 2011

	int logpoint;

	int operation;

	//end by Zhufeng

	//Added by Zhufeng, Dec 20, 2013
	//int dev_id;
  //end by Zhufeng, Dec 20, 2013
};



struct tooling_trace_item{

	int id;	//id of the tooling piece

	vector< pair<int,pair<double,double> > > schedule; //a vecetor keep track of the <machine_id, <start_time,end_time>> of the tooling piece

};



//added by Zhufeng for multipass,Dec 22, 2011

struct multipass{

	bool whether_multi;

	vector<int> logpoint_list;

};

//end Zhufeng, Dec 22, 2011.



//Added by Zhufeng for multipass,Heuristics 3, Jan 20, 2012

struct solution{

	int id;       //id in Machine2 vectors. id-1= postion in the vector.

	int mi_id;    //machine_instance_id

	int mf_id;    //machine_family_id

	int tf_id;    //tooling family id

	int temperature; //certification

	int lots; //the lot (only one)processed in this machine under such setup

	int lot_id;

	char time[50];    // completion time of the lot



	bool Initial_Lot_Flag; // Whether this lot is the initial lot on the machine

	//int initial_running_lot_id;



	double weight;

	int dev_id;  //the id in the vector<char *> DEV. The id-1 is the position



	vector<int> tf_id_vector;   // for multiple line tooling setup

	//vector<int> temperature_set; // According to the method to generate solution.csv, there is only one temperature for each route, with possible multiple tf and different tooling pieces

	                              // the number of tooling pieces of each tf is not output. 



	char setuptime[50];			// the setup time of the lot. 

	double from_begin_to_setup; //

	double from_begin_to_start_time;

	struct date_time completion_time;

	double from_setup_to_completion; //the length from settuping the machine to finish the lot.Note Not from the time setup the lot but setup the Machine



	int logpoint;  //store logpoint

	int pass_no;



	int quantity;

};



//Jan 24, 2012





//Jan 25

struct multi_lot{  //struct for initial lots and regular lots in the multipass problem

	int lot_id;  //dev_id=lots_device[lot_id]

	int logpoint;

	int pass_no;

	double from_begin_to_completion;

	bool whether_initial_lot;  //Note if whether_initial_lot==true, lot_id=-1, cannot output lot_name by LOT[lot_id-1];

	int initial_running_lot_id;//initial_running_lot[(* p_l).initial_running_lot_id-1].



	int quantity; //number of chips in a lot

	double weight; //weight of a lot



	double start_time; //from beginging of the horzion to starting time of the lots processed. Note the start_time for firs pass of initial lot will be negative. 

						//So when output the start_time of the first-pass initial lot, directly use the start_time in initial_running_lot but not this.

	int s_id; //for route s, S[s_id -1] is the route;



	double benefit;//the value of a lot contributed to the objective value including weight of a lot and shortage of key device. 

					//Update to the shortage of key device for the step, not the pass



	//double benefit_step;// calculate the shortage by step_id not pass_no.



	bool operator<(const struct multi_lot &a) const

	{

		return benefit<a.benefit;

	}



	//added by Zhufeng, Dec 2013
	int num_step_left; //denote num of steps left to go through including the upcoming step. 

  //end by Zhufeng, Dec 2013

	int dev_id;  //This was just used for statistics on percentage of lot containing key device now. 

	             //can be used later to reduce running time

	

};



//March 25. 

struct multi_production{ // Record the production of a device realtime for multi-pass problem

	//May 22,add step_no. From now on, the production of a device for the step is multi_production((pass_no-1)*Num_Device+dev_id-1) 

	int dev_id;

	int pass_no;

	int production;

};



struct multi_production_step{

	//May 22,add step_no. From now on, the production of a device for the step is multi_production((step_no-1)*Num_Device+dev_id-1) 

	int dev_id;

	int production;

	int step_id;  //denote which step is this for the device

	int logpoint; //denote which logpoint this step is corresponding to

	bool whether_exist;//some device has fewwer step than the other. So some device only has a production for first steps not the last.

	bool whether_final_step;//whether this step is the final step.

};

//end March 25



struct multi_mach{

	int mf_id;

	int mi_id;                //mi_id is also the id for vector machine_list.

	bool whether_initial_lot; //whether there is a initial lot on this machine

							 //Note:If the initial lot is finished before the start of the time horizon, still counted as no initial lot. whether_initial_lot will be false.



	int initial_running_lot_id; //Note:initial_running_lot[id-1] is the corresponding initial lot. The reason adding this is  that LOT doesn't include initial lots.

	                            //This is different from the Machine_vector.initial_lot. The "id=initial_lot+1".

	bool whether_setup; //whether setup in the solution.csv. Note if no setup in the solution, it means neither initial lot nor regular lot. then tf_id_vector would be empty. certification would be -1.

						//Two reasons not setup in the solution.csv. The first is that it is not setup at all. The second is that it cannot finish its initial lots within time horizon.

						//Note: if it finish its initial lot before the start of the horizon, and no regular lot, also counted as false,





	//Added by Zhufeng,Feb 11, 2012

	bool whether_finish_initial;//if there is initial lot on this machine, whether it can finish it within time horizon.If not, it cannot be setup.

								//If there is no initial lot, then the value of this will also be false.

	//End Feb 11, 2012



	vector<int> tf_id_vector;

	int certification;



	vector<int> lot_id_list; //regulor first-pass lot id list

	vector<int> logpoint_list; //logpoint list for regular first-pass lots

	vector<double> from_begin_to_start_time_list;  //start_time of regular first-pass lots on machine

	vector<double> from_begin_to_completion_time_list; //completion time for regular first-pass lots on the machine

	vector<int> pass_no_list;

	

	vector<int> first_pass_solution_id_list;



	double from_begin_to_setup;//If reset as the original setup, this is the time of original setup.

							// Note this is the time when the machine finish its setup. We are assuming that machines can be setup at the start of the planning horizon.

	                        // Yumin didn't add the Tooling_Setup_Time[tf_id] to the first-pass solution.Tooling_Setup_Time[tf_id] is just used to  

							//if this machine is an initial machine and just has initial lot, then this is 0 because we don't know when it was setup

	double from_begin_to_completion; //If this machine just has an initial lot during the first-pass solution, then this is the completion time of the initial lot

	double from_begin_to_start;//to compute the operation time. The start time of this setup. Even reset with the same tool, this record the start time of reset.

	                           //if this machine just has an initial lot assigned during the first-pass solution, then this should be when the initial machine finishes its initial lot. 

	double horizon; //the length of the whole time horizon for a machine

	bool whether_need_check_for_new_lot;



	vector<multi_lot> assigned_lot_list;//assigned second-pass , third-pass or more lots 



	bool whether_resetup;

};

//end Jan 24, 2012



//Feb 26

struct multi_output{

	vector<int> production_dev;//production_dev[dev_id-1] is the number of device produced for the pass_no th pass

	int pass_no;

};

//Feb 26



//Feb 12,2012

struct tool_setup{    



	int id;

	vector<int> tf_id_vector;   //contains the tooling family id

	vector<int> num_tool_vector;			// contains the number of piece

	int certification;

	vector<int> s_id_vector; //it means which routes this tooling setup can correspond to.

};

//end Feb 12, 2012





//Feb 13, 2012

struct mf_tool_setup{

	//tool_setup_list[(tf_id-1)*sizeof()+tool_setup_list_id-1] is the onefor the mf and tool_setup

	int mf_id;

	int tool_setup_list_id;

	bool whether_s; //whether_s=false, if s_id_vector is empty

	vector<int> s_id_vector;

};

//Feb 13, 2012.



//Feb 15

struct tf_cert_ti{

	//tf_ti_vector[(tf_id-1)*Num_Temperature+certification-1] is the number of ti available under certification



	int id; //basically the id =(tf_id-1)*Num_Temperature+certification-1

	int tf_id;   //tooling family name id.

	int certification; //

	int num_ti;// this is the number of tooling instance for the corresponding certification in this tooling family



};



//Added by Zhufeng for fixed initial setup, April 06, 2013

struct initial_mi_sim_lambda{

	int mi_id;

	int sim_id;  //this is the machine family combined with remaining time. If two machines belong to the same MF and have the same remaining time, then they are the sim

	int lambda_id; //the lambda_id. Lambda[lambda_id-1] is the lambda

};

//Feb 13, 2012

//struct mf_tool_setup_begin_end{

//	// mf_tool_setup_list[begin_id-1] is the begin of the setup_list for mf_id

//	// mf_tool_setup_list[end_id-1] is the end of the setup_list for mf_id

//	int begin_id;

//	int end_id;

//}

//Feb 13, 2012

//end by Zhufeng, Jan 20, 2012



class AT{

public:

	int Max_Num_Tooling;

	int Max_Num_Tooling_Family;

	int Max_Num_Machine;

	int Max_Num_Lot;

	int Max_Num_Device;

	int Max_Num_Temperature;

	int Max_Num_Tooling_Configure;



	int Num_Tooling;

	int Num_Tooling_Family;

	int Num_Machine;

	int Num_Machine_Group;

	int Num_Lot;

	int Num_Device;

	int Num_Keydevice;

	int Num_Package;

	int Num_Key_PKG;

	int Num_Temperature;

	int num_mg;

	int Generate_Option;

	int Run_Option;

	int LP_IP;

	int new_old_model;

	int Seed;

	int SIM_option; //1:use SIM; 0 ow

	int PhaseII_option; //1 run Phase II;0 do not run Phase II

	int Num_LP_solved;

	int Num_Simulated;

	int Num_Discarded;

	int Max_Iter;



	//--------- added by yufen ---------

	int INF;                            //comment by Zhufeng. This value is not used anymore. fix_intial_setup can decide whether fix initial setup or not before changeover

	//--------- end yufen --------------



	//Added by Zhufeng for multi-pass production, March 25, 2012

	//record how many passes of lots are assigned for the program

	int Num_Pass; 

	//end by Zhufeng, March 25, 2012

	//May 22 for maximal number of steps

	int Max_Num_Step;

	//May 22

	//Added by Zhufeng, Oct 19, 2012

	int Multipass_Option;

	int fix_initial_setup;

	int grasp_times;

	int resetup_option;

	int beyond_horizon; //Feb 4,2014

	//end by Zhufeng, April 09, 2013



	//when clear MF,MI, need to delete char * inside

	vector<char *> MF; //to store machine family names

	vector<char *> MI; //to store machine instance names

	vector<char *> TF; //to store tooling family names

	vector<char *> TI; //to store tooling instance names

	vector<char *> LOT; //to store column A in wip.csv;  Zhufeng: LOT[lot_id-1] is the name; only store regular lot's name not initial lot

	vector<char *> DEV; //to store column B in wip.csv

	vector<char *> PKG; //to store package name. This is a new package definition



	struct date_time General_Snapshoot_time;   //the starting of the planning horizon

	double Load_Unload_Time; //this is the time to load and unload a lot to a machine

	//double H[Max_Num_Machine];

	double * H;      //Note:H[mi_id]is the time horizon for MI[mi_id-1], added by Zhufeng on Jan 27, 2012

	double * effective_starting_time; //due to the initial lots, the starting time is not necessary from time zero	

	double * Tooling_Setup_Time; //the setup time for a tooling pieces

	//int n_t_n_tooling[Max_Num_Tooling_Family][Max_Num_Temperature];

	int * n_t_n_tooling;

	//int n_l_chips[Max_Num_Lot];

	int * n_l_chips;

	//int n_k_minchips[Max_Num_Device];

	int * n_k_minchips;

	//int n_p_minchips[Max_Num_Device];

	int * n_p_minchips;

	//int n_kp_minchips[Max_Num_Device];

	int * n_kp_minchips;

	//int a_st[Max_Num_Tooling_Configure][Max_Num_Tooling_Family];

	int * a_st;

	//double w_l[Max_Num_Lot];

	double * w_l;

	//double rate[Max_Num_Tooling_Configure];

	double * rate;

	//double eps[Max_Num_Tooling_Configure];

	double * eps;

	//double penalty[Max_Num_Device];

	double * penalty;

	//double PKG_penalty[Max_Num_Device];

	double * PKG_penalty;

	vector<int> PIN_order;

	double * PIN_PKG_penalty;

	//int device_PKG[Max_Num_Device]; //given device d, find its associated package id

	int * device_PKG;

	//int device_PIN[Max_Num_Device]; //given device d, find its number of pins

	int * device_PIN;



	vector<tooling_configure> S;  //S[s-1] is the route

	vector<tooling_set> Tooling_Families;

	vector<Tooling> Tooling_vector;

	vector<Machine> Machine_vector;

	vector<machine_set> Machine_Groups;

	vector<SIM_item> SIM;

	vector<int> Packages;

	vector<int> Key_Devices;

	vector<int> Key_PKG;

	vector<pair<pair<int,int>, int> > Key_PIN_PKG; //< <pin,package>,target>

	vector<device_item> Devices;

	vector<initial_running_lot_item>initial_running_lot;

	vector< pair<sub_soln_item,double> > enhanced_solution; //this soln stores the starting time

	vector<tooling_trace_item> tooling_trace; //this vector is used to trace the tooling usage	

	//int lots_device[Max_Num_Lot];

	int * lots_device;  //lots_device[lot_id]is the device for LOT[lot_id-1]

	//int machine_mg[Max_Num_Machine];

	int * machine_mg;

	//int machine_sim[Max_Num_Machine]; //initialize in get_SIM()

	int * machine_sim;

	//int tooling_tf[Max_Num_Tooling];

	int * tooling_tf;

	int x_limit;              //in use

	int y_limit;              //in use

	int x_limit_first_level;  //not in use

	int y_limit_first_level;  //not in use

	int x_limit_4;            //not in use

	int y_limit_4;            //not in use

	vector<index_node> index_vector;

	int * index_vector_flag;  //this array is used to reduce the search time in index2_x() and index2_y()

	vector<index_node> first_level_index_vector;

	vector<index_node> index_vector_no_s;

	vector<t_set> Temperature_Set;

	//vec_int L_i[Max_Num_Machine];

	vec_int * L_i;

	//vec_int L_i_d[Max_Num_Machine][Max_Num_Device];      //L(i,dev)

	vec_int * L_i_d;

	//vec_int M_l[Max_Num_Lot];

	vec_int * M_l;

	//vec_tc S_i[Max_Num_Machine];

	vec_tc * S_i;

	vector<equ_mach_set> Equivalent_Machine;

	vector<ch_array> device_name;

	vector<ch_array> lot_name;

	vector<Lambda_item> Lambda;// Added by Zhufeng. Lamda contains all the possible tooling setup

	                           //Each tooling setup is comprised by some tooling families, the number of machine instances in each TF,

	                           //and the temprature.



	//vec_int Lambda_i[Max_Num_Machine];

	vec_int * Lambda_i;  // Lambda_i[machine_id] is the possible Lambd_id on this machine

						//Added by Zhufeng, now exclude initial setups for the machines that don't take initial setup in the beginning of the time horizon. Feb 26 



	//added by Zhufeng Feb 26

	//vec_int * Lambda_i2; //S_sim_l_lambda initialize by Lambda_i, so need to get original Lambda_i to initialize S_sim_l_lambda initialize .

	//end by Zhufeng



	//vec_int M_lambda[Max_Num_Tooling_Configure];

	vec_int * M_lambda;

	vector< pair< pair<int,int>,int> > tooling_sum_rhs;   //<<tf,n>,sum>

	vector< pair< pair<int,int>,vec_int> > L_sim_lambda;  //<<sim,lambda>,lots>

	



	vector< pair<int,sim_lambda_item> > sim_lambda_L;     //<lot,<sim1,lambda1><sim2,lambda2><...> >

	vector< pair< pair<int,int>,int > > sil;              //S(i,l)



	//comment by Zhufeng for logpoint, Aug 31, 2011

	//vector< pair< pair<int,int>,vec_int > > S_sim_d;      //S(i,l)

    //end by Zhufeng



	//Added by Zhufeng for logpoint, Aug 31 ,2011

	vector< pair< pair<int,int>,vec_int > > S_sim_l; 

	//end by Zhufeng



	//comment by Zhufeng for logpoint, Sep 02, 2011

	//vector<S_sim_d_lambda_item> S_sim_d_lambda;           //S(i,l,lambda)

    //end by Zhufeng

	vector<soln_pool_item> elite_soln;



	double coef;

	double sum_weight;

	vector<int> LPD;



	//heurisitics------------------------------------------------

	vector<int> lot_set_heur;

	vector<int> dev_ord_heur;

	//vec_int p_device[Max_Num_Device];

	vec_int * p_device;

	vector<machine_heur_item> Machine_Heur;

	vector<lot_heur_item> Lot_Assign_Heur;

	int ** tooling_used;

	//int package_qty[Max_Num_Device+1];

	int * package_qty;

	//int keydevice_qty[Max_Num_Device+1];

	int * keydevice_qty;

	ofstream f_log;

	//----------------------------------------------



	//Added by Zhufeng for logpoint

    int * lot_logpoint;   //lot_logpoint[lot_id]=logpoint of the lot with lot_id

	int * lot_operation;  //lot_operation[lot_id]=operation of the lot with lot_id



	vector<S_sim_l_lambda_item> S_sim_l_lambda; 

	vector<int> sim_position_S_sim_l_lambda;// sim_position_S_sim_l_lambda[sim-1] is the position when the sim first appear in S_sim_l_lambda

	//end by Zhufeng



	//added by Zhufeng for multipass, Dec 22, 2011

	vector<multipass> multipass_vector; //This is required in the new version of Heuristics.



	vec_int * L_second_pass_i;

	vector< pair< pair<int,int>,vec_int > > S_sim_l_second_pass; //end by Zhufeng for multipass, Dec 31, 2011

	vector<S_sim_l_lambda_item> S_sim_l_second_pass_lambda; 

	vector<int> sim_position_S_sim_l_second_pass_lambda;



	vector< pair< pair<int,int>,vec_int> > L_second_pass_sim_lambda;  //<<sim,lambda>,lots>

	vector<int> logpoint_enhanced_solution;    //logpoint_enhanced_solution[id-1] is the logpoint of the corresponding route in the enhanced_solution

	vector<int> which_pass;  //which_pass[id-1]= 1or 2. Recording which pass is this for the solution in the enhanced_solution

	

	//end by Zhufeng for multipass, Dec 31, 2011

	



	//Added by Zhufeng for multipass, Heuristics 3, Jan 20, 2012

	vector<solution> first_pass_solution;



	//March 25,

	vector<multi_production> multi_device_production;

	//May 27,

	vector<multi_production_step> multi_device_production_step;

	//may 27

	//Dec,2013
	  map<int,multi_device_pass> device_pass_map;
	//Dec,2013


	

	//end by Zhufeng, Jan 20, 2012





//Feb 13, 2012



public:

	AT();

	AT(BCP_buffer & buf);

	void readFiles();

	// --- added by yufen ----

	void readInitialSetups();

	// --- end yufen ----

	void get_S(vector<tooling_configure> & TS);

	void get_S(vector<tooling_configure> & TS,int i);

	void get_S_l(vector<tooling_configure> & TS,int l);

	void get_S(vector<tooling_configure> & TS,int i,int l);

	void get_S(vector<tooling_configure> & TS,int i,int l,int tf);

	void get_S(vector<tooling_configure> & TS,int i,int l,int tf,int tau);

	void get_S_i_l_Lambda(vector<tooling_configure> &TS,int i,int l,int Lambda_id);

	void get_M(vector<machine_set> & MS);

	void get_M(vector<int> & MS,int l);

	void get_machine_mg();

	void get_tooling_tf();

	void get_L(vector<int> & LS);

	void get_L(vector<int> & LS,int i);

	void get_L(vector<int> & LS,int i,int device);

	void get_L_i_lambda(vector<int> & LS,int i,int lambda_id);

	void get_LPD();

	void get_K(vector<int> & KS);

	void get_Lambda();

	void get_Lambda(vector<int> & L,int i,int tf,int tau);

	int  get_Lambda_id(vector<int> & tf,vector<int> & b,int & temperature);

	void get_SIM();

	void get_tooling_sum_rhs();

	void get_sum_weight();

	void show_mg();

	void show_tf();

	void show_S();

	void show_Devices();

	void show_index_vector();

	void show_Machine_vector();

	void show_Tooling_vector();

	void show_temp_comb(vector<t_set> & temp_set);

	void show_statistics();

	void show_L_i();

	void show_L_i_d();

	void show_M_l();

	void show_S_i();

	void show_equivalent_machines();

	void show_device_name();

	void show_lot_name();

	void show_lot_set_heur();

	void show_Lambda();

	void show_Lambda_i();

	void show_w_l();

	void show_eps();

	void show_SIM();

	void show_machine_sim();

	void show_L_sim_lambda();

	void show_sim_lambda_L();

	void show_sub_soln(vector<sub_soln_item> & sub_soln);

	void show_sil();

    //Comment by Zhufeng for logpoint, Aug 31, 2011

	//void show_S_sim_d();

	//end by Zhufeng



	void show_S_sim_d_lambda();

	void show_PIN_PKG();

	void show_enhanced_solution(vector< pair<sub_soln_item,double> > & enhanced_solution);

	void show_initial_running_lot(vector<initial_running_lot_item> & initial_running_lot);

	void test();

	int index_x(int i,int l,int s);

	int index2_x(int i,int l,int s);

	int index3_x(int i,int l,int s);

	int index4_x(int i,int l);

	int index_y(int i,int l);

	int index2_y(int i,int Lambda_id);

	int index3_y(int i,int Lambda_id);

	int index4_y(int i,int Lambda_id);

	void build_index_vector();

	void build_first_level_index_vector();

	void build_index_vector_no_s();

	//void build_first_level_model(IloEnv & env,IloModel & model,vector<IloNumVar> & x_vector,vector<IloNumVar> & y_vector

	//							 ,vector<IloNumVar> & deltaPD_vector,vector<IloNumVar> & deltaKD_vector);

	void build_first_level_model_OSI(CoinPackedMatrix * M,double * cost,double * col_lb,double * col_ub,double * row_lb,double * row_ub);

	//void solve_first_level_model(IloModel & model,vector<IloNumVar> & x_vector,vector<IloNumVar> & y_vector

	//						 ,vector<IloNumVar> & deltaPD_vector,vector<IloNumVar> & deltaKD_vector,double * solution);

	void solve_first_level_model_OSI(OsiXxxSolverInterface * si,double * solution);

	void first_level_heuristic();

	double second_level_heuristic(double * solution,vector<sub_soln_item> & sub_soln,vector< pair<int,double> > & shortage

								  ,vector< pair< pair<int,double>,double> > & unassigned_lots,int option);

	double second_level_greedy(int fix_y[],vector<sub_soln_item> & sub_soln,vector< pair<int,double> > & shortage

								  ,vector< pair< pair<int,double>,double> > & unassigned_lots);

	void build_update_benefit_model(CoinPackedMatrix * M,double * cost,double * col_lb,double * col_ub,double * row_lb,double * row_ub);

	void construct_CL(vector< pair< pair<int,int>,double> > & CL,const int * tooling_used,const int * sim_used,const int * SIM_number,const vector< pair<double,vec_int> > & sim_lambda_benefit);



	//addded by Zhufeng for fixed initial setup, April 09, 2012 

	void initialize_machine_setup_vector( vector<initial_mi_sim_lambda> & );

	void construct_CL_fixed_initial_setup(vector< pair< pair<int,int>,double> > & CL,vector<initial_mi_sim_lambda> & ,int & ); //get the initial mi_id to construct CL from initial_mi_setup,and remove it from  initial_mi_setup 

	//end by Zhufeng for fixed initial setup, April 09, 2012



	int construct_RCL(vector< pair< pair<int,int>,double> > & CL,double apha);

	int is_sim_lambda_feasible(int simid,int Lambda_id,int * tooling_used,int * sim_used,int * SIM_number);

	int pick(int RCL_length);

	int pick(vector< pair< pair<int,int>,double> > & candidate_list,int RCL_length,vector<double> & SL_avg,vector<double> & SL_best,double best_obj,double delta);

	void greedy_assignment(int simid,int Lambda_id,vector<int> & lot_set,vector<int> & lot_selected,vector<int> & production);

	void initialize_benefit(vector< pair<double,vec_int> > & sim_lambda_benefit,int * tooling_used,int * sim_used,int * SIM_number);

	void update_benefit(vector< pair<double,vec_int> > & sim_lambda_benefit,vector<int> & lot_remain,vector<int> & production,vector<int> & affected_dev,int * tooling_used,int * sim_used,int * SIM_number);

	void update_benefit2(vector< pair<double,vec_int> > & sim_lambda_benefit,vector<int> & lot_remain,vector<int> & production

						 ,vector<int> & affected_dev,int * tooling_used,int * sim_used,int * SIM_number,OsiXxxSolverInterface & si_KP);

	void N1(vector<sub_soln_item> & sub_soln,vector< pair<int,double> > & shortage

			,vector< pair< pair<int,double>,double> > & unassigned_lots);

	void N2(vector<sub_soln_item> & sub_soln,vector< pair<int,double> > & shortage

			,vector< pair< pair<int,double>,double> > & unassigned_lots);

	void N3(vector<sub_soln_item> & sub_soln,vector< pair<int,double> > & shortage

			,vector< pair< pair<int,double>,double> > & unassigned_lots);

	int max_rate_s(int i,int l,int Lambda_id);

	void local_search_on_y(OsiXxxSolverInterface * si,struct soln_pool_item & soln_A);

	double local_branch_AB(OsiXxxSolverInterface * si,struct soln_pool_item & soln_A,struct soln_pool_item & soln_B);

	double local_branch_AB2(OsiXxxSolverInterface * si,struct soln_pool_item & soln_A,struct soln_pool_item & soln_B);

	double local_branch_LP(OsiXxxSolverInterface * si, struct soln_pool_item & soln_A,int K);

	bool make_feasible(int fix_y[]);

	double compute_obj(vector<sub_soln_item> & sub_soln,vector< pair<int,double> > & shortage);

	double compute_obj(double * solution);

	double compute_obj(vector< pair<sub_soln_item,double> > & enhanced_solution);

	void adjust_LP_solution(double * solution);

	int index_L_sim_lambda(int simid,int Lambda_id);

	int index_sil(int i,int l);

    

	//comment by Zhufeng for logpoint, Aug 31, 2011

	//int index_S_sim_d(int sim,int d);

	//end by Zhufeng

	int index_S_sim_d_lambda(int sim,int d,int Lambda_id);

	void build_model();

	void build_model_no_s();

	void random_case();

	int roundup(double f);

	int is_integer(double x);

	int select_alpha(double prob_list[],int length);

	void update_alpha_prob(double prob_list[],double avg_list[],int length,double best_so_far,double delta);

	int is_KD_PD(int dev);

	void random_select(int N,int n,int selected[],int excluded[],int excluded_length);

	int not_in_int_array(int target,int array[],int array_length);

	void display(vii begin,vii end);

	void temp_combination();

	void get_super_set(struct t_set current_set,vector<t_set> & super_set);

	void get_inter_set(struct t_set current_set,vector<t_set> & inter_set);

	int get_set_id(struct t_set current_set);

	int strtok2(char * pdes, char * porg, int start_id, char delim);

	int strtok2(char * pdes, char * porg, int start_id, char delim1, char delim2);

	void trim(char * ch, char delim);

	void trim(char * ch);

	void create_case_from_files();

	void initialize_L_i();

	void initialize_L_i_d();

	void initialize_M_l();

	void initialize_S_i();

	void initialize_equivalent_machines();

	void initialize_Lambda_i();

	void initialize_M_lambda();

	void initialize_L_sim_lambda();

	void initialize_sim_lambda_L();

	void initialize_sil();

    //comment by Zhufeng for logpoint

	//void initialize_S_sim_d();

    // end by Zhufeng for logpoint

	void initialize_S_sim_d_lambda();



	void test_feasibility(double value[]);

	void sort_lot_heur(int finished_dev_amount[]);

	void assign_heur(int ** tooling_used,int finished_dev_amount[]);

	void initialize_Machine_Heur();

	void show_Machine_Heur();

	void initial_soln();

	void initialize_Lot_Assign_Heur();

	void show_Lot_Assign_Heur();

	void lot_assignment_heur();

	void build_soln(double soln[]);

	void sort_TS(vector<tooling_configure> & TS);

	bool compare_tc(const tooling_configure & a,const tooling_configure & b);

	void get_AT_Nums(int & Num_Tooling_copy,int & Num_Tooling_Family_copy,int & Num_Machine_copy,int & Num_Machine_Group_copy

		,int & Num_Lot_copy,int & Num_Device_copy,int & Num_Keydevice_copy,int & Num_Package_copy,int & Num_Temperature_copy

		,int & x_limit_copy,int & y_limit_copy);

	void get_AT_data(double * & H_copy,int * n_t_n_tooling_copy,int n_l_chips_copy[],int n_k_minchips_copy[],int n_p_minchips_copy[],double w_l_copy[],double rate_copy[]

		,double eps_copy[],int lots_device_copy[],int machine_mg_copy[],int tooling_tf_copy[]);

	void get_AT_vector(vector<tooling_configure> & S_copy,vector<tooling_set> & Tooling_Families_copy

		,vector<Tooling> & Tooling_vector_copy,vector<Machine> & Machine_vector_copy,vector<machine_set> & Machine_Groups_copy,vector<int> & Packages_copy

		,vector<int> & Key_Devices_copy,vector<device_item> & Devices_copy,vector<index_node> & index_vector_copy,vector<t_set> & Temperature_Set_copy,vec_int L_i_copy[]

		,vec_int M_l_copy[],vec_tc S_i_copy[],vector<ch_array> & device_name_copy,vector<ch_array> & lot_name_copy,vector<Lambda_item> & Lambda_copy,vec_int Lambda_i_copy[]

		,vec_int M_lambda_copy[]);

	int AT_main();

	void pack(BCP_buffer & buf) const;

	void unpack(BCP_buffer & buf);  

	void generate_reports(const double * solution);

	void soln_pool_item_to_solution(struct soln_pool_item & soln,double * solution);

	void minimize_makespan(struct soln_pool_item & soln);

	void show_targets();

	double get_PIN_PKG_penalty(int pkg_id,int pin_num);

	bool is_Key_PKG(int dev);

	bool is_Key_PIN_PKG(int dev);

	//------------------------------------------------------Functions for AT Enhancement

	//Added by Zhufeng

	//void initialize_Lambda_i2();//This is designed for Yufen's code.The initialize_Lambda_i() give the machines with initial lots only one Lambda candidate which is just the initial setup

	                            //When considering Enhancement, this need to be modified to Yumin's original initialize_Lambda_i().

	//void show_Lambda_i2();

	//End by Zhufeng

	void machine_resetup(struct soln_pool_item & GRASP_soln);

	void update_benefit_resetup(int current_machine,int current_Lambda,double current_time,vector< pair<double,vec_int> > & machine_lambda_benefit,vector<int> & lot_remain,vector<int> & production,vector<int

> & affected_dev,int * tooling_used,int * sim_used,int * SIM_number,vector<int> & idle_machines);	

	void construct_CL_resetup(vector< pair< pair<int,int>,double> > & CL,vector< pair<double,vec_int> > & machine_lambda_benefit);	

	void extend_initial_lot(vector<initial_running_lot_item> & initial_running_lot);

	void process_initial_lot(vector<initial_running_lot_item> & initial_running_lot,double * H);

	void generate_reports(vector<initial_running_lot_item> & initial_running_lot,vector< pair<sub_soln_item,double> > & enhanced_solution);

	void str_to_date_time(char * ch,struct date_time & dt);

	double get_date_time_difference(const struct date_time & a,const struct date_time & b);

	struct date_time increase_date_time(const struct date_time & dt,double increment_hr);

	void output_date_time(struct date_time & dt);

	void initialize_tooling_trace(struct soln_pool_item & best_soln,vector<tooling_trace_item> & tooling_trace);

	void show_tooling_trace(vector<tooling_trace_item> & tooling_trace);	



	//--------------------------------------------------Fuctions for logpoint Added by Zhufeng
	

	int index_S_sim_l(int sim, int l);

	void initialize_S_sim_l();

    void show_S_sim_l();



	void initialize_S_sim_l_lambda();

	void show_S_sim_l_lambda();

	int index_S_sim_l_lambda(int sim,int l,int Lambda_id);

	//end by Zhufeng for logpoint



	//Added by Zhufeng for multipass, Dec 22, 2011

	void initialize_multipass_vector( int & Max_Num_Step);  //multipass_vector[device_id-1] denote the corresponding logpoint list

	void show_multipass_vector();//Dec 22, 2011

	//void second_pass_setup(struct soln_pool_item & best_soln);//Dec 29, 2011. not used anymore. Nov 11, 2012

	void update_benefit_second_pass_setup(int current_machine,int current_Lambda,double current_time,vector< pair<double,vec_int> > & machine_lambda_benefit,vector<int> & lot_remain,vector<int> & lot_remain_second_pass_logpoint,vector<int> & production,vector<int> & affected_dev,int * tooling_used,int * sim_used,int * SIM_number,vector<int> & idle_machines);

	int get_second_logpoint(int lot_id);

	void show_second_logpoint();

	void initialize_L_second_pass_sim_lambda();

	void show_L_second_pass_sim_lambda();

	void get_L_second_pass_i_lambda(vector<int> & LS,int i,int lambda_id);

	void initialize_L_second_pass_i();



	void show_L_second_pass_i();



	void get_S_i_l_second_pass_Lambda(vector<tooling_configure> &TS,int i,int l,int Lambda_id);



	void get_S_second_pass(vector<tooling_configure> & TS,int i,int l);



	int max_rate_s_second_pass(int i,int l,int Lambda_id);



	int index_L_second_pass_sim_lambda(int simid,int Lambda_id);



	void initialize_S_sim_l_second_pass();



	void show_S_sim_l_second_pass();



	int index_S_sim_l_second_pass(int sim,int l);



	void initialize_S_sim_l_second_pass_lambda();



	void show_S_sim_l_second_pass_lambda();



	int index_S_sim_l_second_pass_lambda(int sim,int l,int Lambda_id);

		

		//end by Zhufeng, Dec 31, 2011

	

	//Added by Zhufeng for multipass, Heuristics 3, Jan 20, 2012

	void initialize_enhanced_solution( struct soln_pool_item & best_soln );

	void readsolution(vector<solution> & first_pass_solution);

	void show_first_pass_solution( vector<solution> & first_pass_solution);



	//Jan 24, 2012

	void initialize_machine_list( vector<multi_mach> & machine_list,vector<solution> & first_pass_solution, const double * H);

	//Jan 24, 2012



	//Jan 25, 2012

	void show_machine_list( vector<multi_mach> & machine_list);

	void Heuristics_three(vector<multi_mach> & machine_list,const vector<multi_lot> & first_pass_lot_vector,const vector<tool_setup> & tool_setup_list,const vector<mf_tool_setup> & mf_tool_setup_list, vector<tf_cert_ti> & tf_cert_ti_list,vector<multi_lot> & finished_lot_list);



	void resetup(vector<multi_mach> & reset_machine_list,const vector<multi_lot> & first_pass_lot_vector,const vector<tool_setup> & tool_setup_list,const vector<mf_tool_setup> & mf_tool_setup_list, vector<tf_cert_ti> & tf_cert_ti_list,const vector<multi_lot> & finished_lot_list,

				 vector<multi_lot> & finished_lot_reset_list,const vector<multi_lot> & unassigned_lot_first_pass_vector);

	//Feb 24



	//Jan 25, 2012



	//Jan 26

	void initialize_first_pass_lot_vector( vector<multi_lot> & first_pass_lot_vector,vector<solution> & first_pass_solution);

	void show_first_pass_lot_vector(vector<multi_lot> & first_pass_lot_vector);

	bool get_candidate_lot_list( vector<multi_lot> & candidate_lot_list, const vector<multi_lot> & first_pass_lot_vector,const vector<multi_lot> & finshied_lot_list,double current_time,double last_time, int & Num_Pass);

	//Jan 26

    //Feb 23

	bool get_candidate_lot_list( vector<multi_lot> & candidate_lot_list, const vector<multi_lot> & first_pass_lot_vector,const vector<multi_lot> & finshied_lot_reset_list,double current_time,double last_time,const vector<multi_lot> finished_lot_list,int & Num_Pass);

	//Feb 23



	//Jan 27

	void show_candidate_lot_list( vector<multi_lot> & candidate_lot_list, double last_time, double current_time);

	void show_candidate_lot_list( vector<multi_lot> & candidate_lot_list);

	int get_next_pass( struct multi_lot one_lot); //retunr -1 if only single pass for this device;0 if already in the last logpointlogpoint;logpoint, if normal.

	void assign_lot( vector<multi_mach> & machine_list,vector<multi_lot> & candidate_lot_list,vector<multi_lot> & finished_lot_list,double current_time,const vector<tool_setup> tool_setup_list,const vector<mf_tool_setup> & mf_tool_setup_list, vector<tf_cert_ti> & tf_cert_ti_list);

	void assign_lot( vector<multi_mach> & machine_list,vector<multi_lot> & candidate_lot_list,vector<multi_lot> & finished_lot_list,double current_time, vector<multi_mach> & min_candidate_list,const vector<tool_setup> tool_setup_list,const vector<mf_tool_setup> & mf_tool_setup_list, vector<tf_cert_ti> & tf_cert_ti_list);

	//Jan 27



	//Jan 28

	void show_H();

	void rank_candidate_lot( vector<multi_lot> & candidate_lot);

	void output_multi(const vector<multi_mach> & machine_list,const vector<solution> & first_pass_solution);

	//



	////Feb 12

	void intialize_tool_setup_list(vector<tool_setup> & tool_setup_list,const vector<tooling_configure> &S);

	void show_tool_setup_list(const vector<tool_setup> & tool_setup_list);

	////Feb 12



	//Feb 13

	void initialize_mf_tool_setup_list(vector<mf_tool_setup> & mf_tool_setup_list,const vector<tool_setup> & tool_setup_list);

	//Feb 13

	//Feb 14

	void show_mf_tool_setup_list( const vector<mf_tool_setup> & mf_tool_setup_list);

	//Feb 14

	//Feb 15

	void initialize_tf_cert_ti_list( vector<tf_cert_ti> & tf_cert_ti_list,const int Num_Temperature, const vector<Tooling>Tooling_Vector);

	void show_tf_cert_ti_list( const vector<tf_cert_ti> & tf_cert_ti_list,const int Num_Temperature);

	//Feb 15

	//Feb 16

	void update_tf_cert_ti_list_begin(vector<tf_cert_ti> & tf_cert_ti_list,const int Num_Temperature,const vector<multi_mach> & machine_list);

	//Feb 16

	//Feb 19

	bool compute_benefit_resetup_empty( const struct multi_mach & one_mach,

						 vector<multi_lot> & candidate_lot_list,

						 vector<multi_lot> & finished_lot_list,

						 const double current_time,

						 double & best_ts_weight,

						 int & best_tool_setup_id,

						 vector<multi_lot> & assigned_lot_list_best_ts,

						 //vector<double> & from_begin_to_completion_list_best_ts,

						 const vector<tool_setup> tool_setup_list,

						 const vector<mf_tool_setup> mf_tool_setup_list,

						 const vector<tf_cert_ti> & tf_cert_ti_list,

						 double & from_begin_to_completion,

						 double & from_begin_to_setup,

						 vector<multi_production> & multi_device_production,

						 int & Num_Pass,

						 vector<multi_production_step> & multi_device_production_step

						 );

	//Feb 20

	//compute the benefit for reset machines that are empty 



	//Feb 23

	void show_finished_lot_list( const vector<multi_lot> finished_lot_list);

	//Feb 23

	//Feb 24

	void initialize_unassigned_lot_first_pass_vector(vector<multi_lot> & unassigned_lot_first_pass_vector,const vector<multi_lot> & first_pass_lot_vector);//initialize unassigned lots  by reading wip.csv and first_pass_lot_vector

	
	//changed by SJ, from void to int
	int assign_lot_reset( vector<multi_mach> & machine_list,    // assign lots to all machines  earlier than current_time

		             vector<multi_lot> & candidate_lot_list,

					 vector<multi_lot> & finished_lot_reset_list,

					 double current_time,

					 const vector<tool_setup> tool_setup_list,

					 const vector<mf_tool_setup> & mf_tool_setup_list, 

					 vector<tf_cert_ti> & tf_cert_ti_list,

					 int type

					 );



	void assign_lot_reset( vector<multi_mach> & machine_list,

					vector<multi_lot> & candidate_lot_list,

					vector<multi_lot> & finished_lot_list,

					double current_time, 

					const vector<int> & min_candidate_mach_list_id_vector,

					const vector<tool_setup> tool_setup_list,

					const vector<mf_tool_setup> & mf_tool_setup_list, 

					vector<tf_cert_ti> & tf_cert_ti_list

					);





	//Feb 24

	//Feb 26

	double compute_obj(vector< pair<sub_soln_item,double> > & enhanced_solution,vector<int> & production_first_pass,double & weighted_item, double & weighted_sum_lot_first_pass);

	double compute_obj_multi( const vector<multi_mach> & machine_list,const vector<int> & production_first_pass, const vector<multi_lot> & finished_lot_list,const double & weighted_item,const double & weighted_sum_lot_first_pass,const vector<multi_production_step> multi_device_production_step,const int Max_Num_Step,const bool);

	//Feb 26



	//May 23, for the obj value for steps

	double compute_obj_step( const vector<solution> & first_pass_solution, const vector<multi_production_step> multi_device_production_step,const double weighted_item,const int Max_Num_Step);

	//May 23



	//March 25

	void initialize_multi_device_production( const vector<int> & production_first_pass,vector<multi_production> & multi_device_production);

	void update_multi_device_production( vector<multi_production> & multi_device_production, const struct multi_lot & one_lot,int & Num_Pass);

	void show_multi_device_production( const vector<multi_production> & multi_device_production);

	//March 25



	//May 22,2012

	void initialize_multi_device_production_step( vector<multi_production_step> & multi_device_production_step,const vector<multi_lot> & unassigned_lot_first_pass_vector,const vector<multi_lot> & first_pass_lot_vector,const int Max_Num_Step);

	void show_multi_device_production_step( vector<multi_production_step> & multi_device_production_step,const int Max_Num_Step);

	//May 23,2012

	int get_step_id( const int dev_id, const int logpoint);

	void update_multi_device_production_step( vector<multi_production_step> & multi_device_production, const struct multi_lot & one_lot);

	//end May 23

	//end May 22, 2012



	//March 27

	//void compute_lot_benefit( vector<multi_lot> & candidate_lot_list, const vector<multi_production> & multi_device_production);

	//void update_lot_benefit( vector<multi_lot> & candidate_lot_list, const vector<multi_production> & multi_device_production,vector<multi_lot>::iterator p_l);



	//March 27



	//May 23, 2012

	void compute_lot_benefit( vector<multi_lot> & candidate_lot_list, const vector<multi_production_step> & multi_device_production_step);

	void update_lot_benefit( vector<multi_lot> & candidate_lot_list, const vector<multi_production_step> & multi_device_production_step,vector<multi_lot>::iterator p_l);

	void update_lot_benefit( vector<multi_lot> & candidate_lot_list, const vector<multi_production_step> & multi_device_production_step);

	//May 23, 2012



	//April 28, 2012 

	//void update_lot_benefit( vector<multi_lot> & candidate_lot_list, const vector<multi_production> & multi_device_production);

	//April 28,2012



	//end by Zhufeng, Jan 20, 2012



	  //Dec 20, 2013
	  void build_device_pass_from_S();
	  void show_device_pass_map()const;
	  int get_num_step_left(const int & dev_id, const int & logpoint)const;
	  //Dec 20, 2013
	virtual ~AT();

};



static bool compare_int_vec(vector<int> & v1,vector<int> & v2){

	//the order doesn't matter

	//true: v1==v2; false o.w.

	if((int)v1.size()!=(int)v2.size()){ return false;}

	else{

		//test if v1 is a subset of v2

		int flag1=1;

		vii p_int=v1.begin();

		while(p_int!=v1.end()){

			vii p_find=find(v2.begin(),v2.end(),(*p_int));

			if(p_find==v2.end()){

				flag1=0;break;

			}

			p_int++;

		}

		if(!flag1){

			return flag1;

		}else{

			int flag2=1;

			vii p_int=v2.begin();

			while(p_int!=v2.end()){

				vii p_find=find(v1.begin(),v1.end(),(*p_int));

				if(p_find==v1.end()){

					flag2=0;break;

				}

				p_int++;

			}

			return flag2;

		}

	}

}



static bool compare_candidates(const pair< pair<int,int>,double> & can1,const pair< pair<int,int>,double> & can2){

	double weight1=can1.second;

	double weight2=can2.second;

	if(weight1>weight2){

		return true;

	}else{

		return false;

	}

}



static bool compare_lot(const pair<int,double> & lot1,const pair<int,double> & lot2){

	double unit_price1=lot1.second;

	double unit_price2=lot2.second;

	if(unit_price1>unit_price2){

		return true;

	}else{

		return false;

	}

}



static bool compare_2_3(const pair< pair<int,double>,double> & lot1,const pair< pair<int,double>,double> & lot2){

	if(lot1.first.second>lot2.first.second){

		return true;

	}else if(lot1.first.second<lot2.first.second){

		return false;

	}else {

		if(lot1.second>lot2.second){

			return true;

		}

		else{

			return false;

		}

	}

}



static bool compare_soln(const soln_pool_item & soln1,const soln_pool_item & soln2){

	if(soln1.obj<soln2.obj){

		return true;

	}else{

		return false;

	}

}



static bool compare_int_double(const pair<int,double> &v1,const pair<int,double> &v2){

	if(v1.second<v2.second){

		return true;

	}else{

		return false;

	}

}



static bool compare_int_int_double(const pair<int,pair<int,double> > &v1,const pair<int,pair<int,double> > &v2){

        if(v1.second.second<v2.second.second){

                return true;

        }else{

                return false;

        }

}



#endif



