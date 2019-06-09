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
#ifdef COIN_USE_CLP
#include "OsiClpSolverInterface.hpp"
typedef OsiClpSolverInterface OsiXxxSolverInterface;
#endif
#ifdef COIN_USE_OSL
#include "OsiOslSolverInterface.hpp"
typedef OsiOslSolverInterface OsiXxxSolverInterface;
#endif

struct tooling_set{
	vector<int> tools;
};
struct tooling_configure{
	struct tooling_set tf;
	int mg;
	int device;
	int id;
	int temperature;
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
	int Lamuda_id;
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
struct Lamuda_item{
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
struct sim_lamuda_item{
	vector< pair<int,int> > sim_lamuda_pairs;
};
struct sub_soln_item{//describe a i-lamuda combination
	int i;
	int Lamuda_id;
	double time_used;
	vec_int lots;
};
struct soln_pool_item{
	double obj;
	vector<sub_soln_item> sub_soln;
	vector< pair<int,double> > shortage;
	vector< pair< pair<int,double>,double> > unassigned_lots;
};
struct S_sim_d_lamuda_item{
	int sim;
	int d;
	int Lamuda_id;
	int s;
};
const int Max_Num_Tooling=1000;
const int Max_Num_Tooling_Family=500;
const int Max_Num_Machine=1000;
const int Max_Num_Lot=1000000;
const int Max_Num_Device=1000;
const int Max_Num_Keydevice=1000;
const int Max_Num_Package=1000;
const int Max_Num_Temperature=100;
const int Max_Num_Tooling_Configure=500;

class AT{
public:
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

	//when clear MF,MI, need to delete char * inside
	vector<char *> MF; //to store machine family names
	vector<char *> MI; //to store machine instance names
	vector<char *> TF; //to store tooling family names
	vector<char *> TI; //to store tooling instance names
	vector<char *> LOT; //to store column A in wip.csv
	vector<char *> DEV; //to store column B in wip.csv
	vector<char *> PKG; //to store package name. This is a new package definition

	double H[Max_Num_Machine];
	int n_t_tooling[Max_Num_Tooling];
	int n_t_n_tooling[Max_Num_Tooling_Family][Max_Num_Temperature];
	int n_l_chips[Max_Num_Lot];
	int n_k_minchips[Max_Num_Device];
	int n_p_minchips[Max_Num_Device];
	int n_kp_minchips[Max_Num_Device];
	int a_st[Max_Num_Tooling_Configure][Max_Num_Tooling_Family];
	double w_l[Max_Num_Lot];
	double rate[Max_Num_Tooling_Configure];
	double eps[Max_Num_Tooling_Configure];
	double penalty[Max_Num_Device];
	double PKG_penalty[Max_Num_Device];
	vector<int> PIN_order;
	double * PIN_PKG_penalty;
	int device_PKG[Max_Num_Device]; //given device d, find its associated package id
	int device_PIN[Max_Num_Device]; //given device d, find its number of pins

	vector<tooling_configure> S;
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
	int lots_device[Max_Num_Lot];
	int machine_mg[Max_Num_Machine];
	int machine_sim[Max_Num_Machine]; //initialize in get_SIM()
	int tooling_tf[Max_Num_Tooling];
	int x_limit;              //in use
	int y_limit;              //in use
	int x_limit_first_level;  //not in use
	int y_limit_first_level;  //not in use
	int x_limit_4;            //not in use
	int y_limit_4;            //not in use
	vector<index_node> index_vector;
	vector<index_node> first_level_index_vector;
	vector<index_node> index_vector_no_s;
	vector<t_set> Temperature_Set;
	vec_int L_i[Max_Num_Machine];
	vec_int L_i_d[Max_Num_Machine][Max_Num_Device];      //L(i,dev)
	vec_int M_l[Max_Num_Lot];
	vec_tc S_i[Max_Num_Machine];
	vector<equ_mach_set> Equivalent_Machine;
	vector<ch_array> device_name;
	vector<ch_array> lot_name;
	vector<Lamuda_item> Lamuda;
	vec_int Lamuda_i[Max_Num_Machine];
	vec_int M_lamuda[Max_Num_Tooling_Configure];
	vector< pair< pair<int,int>,int> > tooling_sum_rhs;   //<<tf,n>,sum>
	vector< pair< pair<int,int>,vec_int> > L_sim_lamuda;  //<<sim,lamuda>,lots>
	vector< pair<int,sim_lamuda_item> > sim_lamuda_L;     //<lot,<sim1,lamuda1><sim2,lamuda2><...> >
	vector< pair< pair<int,int>,int > > sil;              //S(i,l)
	vector< pair< pair<int,int>,vec_int > > S_sim_d;      //S(i,l)
	vector<S_sim_d_lamuda_item> S_sim_d_lamuda;           //S(i,l,lamuda)
	vector<soln_pool_item> elite_soln;

	double coef;
	double sum_weight;
	vector<int> LPD;

	//heurisitics------------------------------------------------
	vector<int> lot_set_heur;
	vector<int> dev_ord_heur;
	vec_int p_device[Max_Num_Device];
	vector<machine_heur_item> Machine_Heur;
	vector<lot_heur_item> Lot_Assign_Heur;
	int ** tooling_used;
	int package_qty[Max_Num_Device+1];
	int keydevice_qty[Max_Num_Device+1];
	ofstream f_log;
	//----------------------------------------------
public:
	AT();
	AT(BCP_buffer & buf);
	void readFiles();
	void get_S(vector<tooling_configure> & TS);
	void get_S(vector<tooling_configure> & TS,int i);
	void get_S_l(vector<tooling_configure> & TS,int l);
	void get_S(vector<tooling_configure> & TS,int i,int l);
	void get_S(vector<tooling_configure> & TS,int i,int l,int tf);
	void get_S(vector<tooling_configure> & TS,int i,int l,int tf,int tau);
	void get_S_i_l_Lamuda(vector<tooling_configure> &TS,int i,int l,int Lamuda_id);
	void get_M(vector<machine_set> & MS);
	void get_M(vector<int> & MS,int l);
	void get_machine_mg();
	void get_tooling_tf();
	void get_L(vector<int> & LS);
	void get_L(vector<int> & LS,int i);
	void get_L(vector<int> & LS,int i,int device);
	void get_L_i_lamuda(vector<int> & LS,int i,int lamuda_id);
	void get_LPD();
	void get_K(vector<int> & KS);
	void get_Lamuda();
	void get_Lamuda(vector<int> & L,int i,int tf,int tau);
	int  get_Lamuda_id(vector<int> & tf,vector<int> & b,int & temperature);
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
	void show_Lamuda();
	void show_Lamuda_i();
	void show_w_l();
	void show_eps();
	void show_SIM();
	void show_machine_sim();
	void show_L_sim_lamuda();
	void show_sim_lamuda_L();
	void show_sub_soln(vector<sub_soln_item> & sub_soln);
	void show_sil();
	void show_S_sim_d();
	void show_S_sim_d_lamuda();
	void show_PIN_PKG();
	void test();
	int index_x(int i,int l,int s);
	int index2_x(int i,int l,int s);
	int index3_x(int i,int l,int s);
	int index4_x(int i,int l);
	int index_y(int i,int l);
	int index2_y(int i,int Lamuda_id);
	int index3_y(int i,int Lamuda_id);
	int index4_y(int i,int Lamuda_id);
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
	void construct_CL(vector< pair< pair<int,int>,double> > & CL,int * tooling_used,int * sim_used,int * SIM_number,vector< pair<double,vec_int> > & sim_lamuda_benefit);
	int construct_RCL(vector< pair< pair<int,int>,double> > & CL,double apha);
	int is_sim_lamuda_feasible(int simid,int Lamuda_id,int * tooling_used,int * sim_used,int * SIM_number);
	int pick(int RCL_length);
	int pick(vector< pair< pair<int,int>,double> > & candidate_list,int RCL_length,vector<double> & SL_avg,vector<double> & SL_best,double best_obj,double delta);
	void greedy_assignment(int simid,int Lamuda_id,vector<int> & lot_set,vector<int> & lot_selected,vector<int> & production);
	void initialize_benefit(vector< pair<double,vec_int> > & sim_lamuda_benefit,int * tooling_used,int * sim_used,int * SIM_number);
	void update_benefit(vector< pair<double,vec_int> > & sim_lamuda_benefit,vector<int> & lot_remain,vector<int> & production,vector<int> & affected_dev,int * tooling_used,int * sim_used,int * SIM_number);
	void update_benefit2(vector< pair<double,vec_int> > & sim_lamuda_benefit,vector<int> & lot_remain,vector<int> & production
						 ,vector<int> & affected_dev,int * tooling_used,int * sim_used,int * SIM_number,OsiXxxSolverInterface & si_KP);
	void N1(vector<sub_soln_item> & sub_soln,vector< pair<int,double> > & shortage
			,vector< pair< pair<int,double>,double> > & unassigned_lots);
	void N2(vector<sub_soln_item> & sub_soln,vector< pair<int,double> > & shortage
			,vector< pair< pair<int,double>,double> > & unassigned_lots);
	void N3(vector<sub_soln_item> & sub_soln,vector< pair<int,double> > & shortage
			,vector< pair< pair<int,double>,double> > & unassigned_lots);
	int max_rate_s(int i,int l,int Lamuda_id);
	void local_search_on_y(OsiXxxSolverInterface * si,struct soln_pool_item & soln_A);
	double local_branch_AB(OsiXxxSolverInterface * si,struct soln_pool_item & soln_A,struct soln_pool_item & soln_B);
	double local_branch_AB2(OsiXxxSolverInterface * si,struct soln_pool_item & soln_A,struct soln_pool_item & soln_B);
	double local_branch_LP(OsiXxxSolverInterface * si, struct soln_pool_item & soln_A,int K);
	bool make_feasible(int fix_y[]);
	double compute_obj(vector<sub_soln_item> & sub_soln,vector< pair<int,double> > & shortage);
	double compute_obj(double * solution);
	void adjust_LP_solution(double * solution);
	int index_L_sim_lamuda(int simid,int Lamuda_id);
	int index_sil(int i,int l);
	int index_S_sim_d(int sim,int d);
	int index_S_sim_d_lamuda(int sim,int d,int Lamuda_id);
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
	void initialize_Lamuda_i();
	void initialize_M_lamuda();
	void initialize_L_sim_lamuda();
	void initialize_sim_lamuda_L();
	void initialize_sil();
	void initialize_S_sim_d();
	void initialize_S_sim_d_lamuda();
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
		,vec_int M_l_copy[],vec_tc S_i_copy[],vector<ch_array> & device_name_copy,vector<ch_array> & lot_name_copy,vector<Lamuda_item> & Lamuda_copy,vec_int Lamuda_i_copy[]
		,vec_int M_lamuda_copy[]);
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
	~AT();
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


#endif

