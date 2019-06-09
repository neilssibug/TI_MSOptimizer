// __START__

#ifndef __AT_GRASP_HPP__
#define __AT_GRASP_HPP__

#include <iostream>
#include <sstream>
#include <cstring>
#include <string>
#include <fstream>
#include <math.h>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <iomanip>
#include <cassert>
#include "combination.hpp"
#include "BCP_buffer.hpp"
#include "CoinPackedMatrix.hpp"
#include "CoinPackedVector.hpp"
#include "OsiSolverInterface.hpp"
#include <string>

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

/**
 * @brief record route for the data consistency checking
 */
struct route_check {
    char device[50];
    int logpoint;
    int operation;
};

/**
 * @brief 
 */
struct tooling_set {
    vector<int> tools;
};

/**
 * @brief 
 */
struct tooling_configure {
    struct tooling_set tf;
    int mg;
    int device;
    string package;
    int id;
    int temperature;
    int logpoint;
    int operation;
};

/**
 * @brief 
 */
struct Tooling {
    char name[100];
    int tf;
    int id;
    vector<int> temperature_vector;
};

/**
 * @brief 
 */
struct Machine {
    char name[100];
    int id;
    int mg;
    vector<int> temperature_vector;
};

/**
 * @brief 
 */
struct machine_set {
    vector<int> machines;
};

struct lot_set {
    vector<int> lots;
};

/**
 * @brief 
 */
struct device_item {
    int device;
    vector<int> device_lots;
};

/**
 * @brief 
 */
struct index_node {
    int index;
    int i;
    int l;
    int s;
    int Lambda_id;
};

/**
 * @brief 
 */
struct t_set {
    vector<int> set_elements;
};

/**
 * @brief 
 */
struct vec_int {
    vector<int> int_vector;
};

/**
 * @brief 
 */
struct vec_tc {
    vector<tooling_configure> tc_vector;
};

/**
 * @brief 
 */
struct equ_mach_set {
    int mg;
    int n;
    vector<int> equ_mach;
};

/**
 * @brief 
 */
struct ch_array {
    char char_arry[100];
    int id;
};

/**
 * @brief 
 */
struct Lambda_item {
    int id;
    vector<int> tooling_vec; //contains the tooling family
    vector<int> b; // contains the number of piece
    int temperature;
    string package;
};

/**
 * @brief 
 */
struct machine_heur_item {
    vector<int> tooling;
    string package;
    vector<int> b;
    vector<int> lot;
    double time_used;
    int temperature;
};

/**
 * @brief 
 */
struct lot_heur_item {
    int l;
    int i;
    int s;
};

/**
 * @brief 
 */
struct SIM_item {
    int simid;
    int represented_machine_id;
    int mg;
    char name[100];
    vector<int> temperature_vector;
    int num_machine;
};

/**
 * @brief 
 */
struct sim_lambda_item {
    vector<pair<int, int> > sim_lambda_pairs;
};

/**
 * @brief describes a i-lambda combination
 */
struct sub_soln_item {
    int i;
    int Lambda_id;
    double time_used;
    vec_int lots;
};

/**
 * @brief 
 */
struct soln_pool_item {
    double obj;
    vector<sub_soln_item> sub_soln;
    vector<pair<int, double> > shortage;
    vector<pair<pair<int, double>, double> > unassigned_lots;
};

/**
 * @brief 
 */
struct S_sim_l_lambda_item {
    int sim;
    int l;
    int Lambda_id;
    int s;
};

/**
 * @brief date and time format
 */
struct date_time {
    char str[50]; // this holds the information
    int MM;
    int DD;
    int YY;
    int hh;
    int mm;
    int ss;
};

/**
 * @brief 
 */
struct initial_running_lot_item {
    char lot_name[50];
    char dev_name[50];
    char PKG_name[50];
    int dev_id;
    int pkg_id;
    int PIN_num;
    int n_l_chips;
    double w_l;
    struct date_time Start_time;
    struct date_time Snapshot_time;
    char machine_name[50];
    int machine_id;
    double rate;
    double time_remain;
    int logpoint;
    int operation;
};

/**
 * @brief 
 */
struct tooling_trace_item {
    int id; //id of the tooling piece
    vector<pair<int, pair<double, double> > > schedule; //a vecetor keep track of the <machine_id, <start_time,end_time>> of the tooling piece
};

/**
 * @brief 
 */
class AT {
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

    //when clear MF,MI, need to delete char * inside
    vector<char *> MF; //to store machine family names
    vector<char *> MI; //to store machine instance names
    vector<char *> TF; //to store tooling family names
    vector<char *> TI; //to store tooling instance names
    vector<char *> LOT; //to store column A in wip.csv
    vector<char *> DEV; //to store column B in wip.csv
    vector<char *> PKG; //to store package name. This is a new package definition

    struct date_time General_Snapshot_time; //the starting of the planning horizon
    double Load_Unload_Time; //this is the time to load and unload a lot to a machine
    //double H[Max_Num_Machine];
    double * H;
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

    vector<tooling_configure> S;
    vector<tooling_set> Tooling_Families;
    vector<Tooling> Tooling_vector;
    vector<Machine> Machine_vector;
    vector<machine_set> Machine_Groups;
    vector<SIM_item> SIM;
    vector<int> Packages;
    vector<int> Key_Devices;
    vector<int> Key_PKG;
    vector<pair<pair<int, int>, int> > Key_PIN_PKG; //< <pin,package>,target>
    vector<device_item> Devices;
    vector<initial_running_lot_item> initial_running_lot;
    vector<pair<sub_soln_item, double> > enhanced_solution; //this soln stores the starting time
    vector<tooling_trace_item> tooling_trace; //this vector is used to trace the tooling usage
    //int lots_device[Max_Num_Lot];

    int * lots_device; //lots_device[lot_id]=dev_id
    //int machine_mg[Max_Num_Machine];
    int * machine_mg;
    //int machine_sim[Max_Num_Machine]; //initialize in get_SIM()
    int * machine_sim;
    //int tooling_tf[Max_Num_Tooling];
    int * tooling_tf;
    int x_limit; //in use
    int y_limit; //in use
    int x_limit_first_level; //not in use
    int y_limit_first_level; //not in use
    int x_limit_4; //not in use
    int y_limit_4; //not in use
    vector<index_node> index_vector;
    int * index_vector_flag; //this array is used to reduce the search time in index2_x() and index2_y()
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
    vector<Lambda_item> Lambda;
    //vec_int Lambda_i[Max_Num_Machine];
    vec_int * Lambda_i;
    //vec_int M_lambda[Max_Num_Tooling_Configure];
    vec_int * M_lambda;
    vector<pair<pair<int, int>, int> > tooling_sum_rhs; //<<tf,n>,sum>
    vector<pair<pair<int, int>, vec_int> > L_sim_lambda; //<<sim,lambda>,lots>
    vector<pair<int, sim_lambda_item> > sim_lambda_L; //<lot,<sim1,lambda1><sim2,lambda2><...> >
    vector<pair<pair<int, int>, int> > sil; //S(i,l)

    vector<soln_pool_item> elite_soln;

    vector<S_sim_l_lambda_item> S_sim_l_lambda;
    vector<int> sim_position_S_sim_l_lambda; // sim_position_S_sim_l_lambda[sim-1] is the position when the sim first appear in S_sim_l_lambda

    int * lot_logpoint; //lot_logpoint[lot_id]=logpoint
    int * lot_operation; //lot_operation[lot_id]=operation

    vector<pair<pair<int, int>, vec_int> > S_sim_l;

    double coef;
    double sum_weight;
    vector<int> LPD;

    //heuristics------------------------------------------------
    vector<int> lot_set_heur;
    vector<int> dev_ord_heur;
    vec_int * p_device;
    vector<machine_heur_item> Machine_Heur;
    vector<lot_heur_item> Lot_Assign_Heur;
    int ** tooling_used;
    int * package_qty;
    int * keydevice_qty;
    ofstream f_log;
    //----------------------------------------------
public:
    AT();
    AT(BCP_buffer & buf);
    void readFiles();
    void get_S(vector<tooling_configure> & TS, int i);
    void get_S(vector<tooling_configure> & TS, int i, int l);
    void get_S(vector<tooling_configure> & TS, int i, int l, int tf);
    void get_S_l(vector<tooling_configure> & TS, int l);
    void get_S_i_l_Lambda(vector<tooling_configure> &TS, int i, int l, int Lambda_id);
    void get_machine_mg();
    void get_tooling_tf();
    void get_L(vector<int> & LS, int i);
    void get_L(vector<int> & LS, int i, int device);
    void get_L_i_lambda(vector<int> & LS, int i, int lambda_id);
    void get_LPD();
    void get_Lambda();
    void get_Lambda(vector<int> & L, int i, int tf, int tau);
    int get_Lambda_id(vector<int> & tf, vector<int> & b, int & temperature, string & package);
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
    void show_PIN_PKG();
    void show_enhanced_solution(vector<pair<sub_soln_item, double> > & enhanced_solution);
    void show_initial_running_lot(vector<initial_running_lot_item> & initial_running_lot);
    int index_x(int i, int l, int s);
    int index_y(int i, int Lambda_id);
    void build_index_vector();
    void build_first_level_model_OSI(CoinPackedMatrix * M, double * cost, double * col_lb, double * col_ub, double * row_lb, double * row_ub);
    void first_level_heuristic();
    double second_level_heuristic(double * solution, vector<sub_soln_item> & sub_soln, vector<pair<int, double> > & shortage, vector<pair<pair<int, double>, double> > & unassigned_lots, int option);
    double second_level_greedy(int fix_y[], vector<sub_soln_item> & sub_soln, vector<pair<int, double> > & shortage, vector<pair<pair<int, double>, double> > & unassigned_lots);
    void build_update_benefit_model(CoinPackedMatrix * M, double * cost, double * col_lb, double * col_ub, double * row_lb, double * row_ub);
    void construct_CL(vector<pair<pair<int, int>, double> > & CL, int * tooling_used, int * sim_used, int * SIM_number, vector<pair<double, vec_int> > & sim_lambda_benefit);
    int construct_RCL(vector<pair<pair<int, int>, double> > & CL, double apha);
    int is_sim_lambda_feasible(int simid, int Lambda_id, int * tooling_used, int * sim_used, int * SIM_number);
    int pick(int RCL_length);
    int pick(vector<pair<pair<int, int>, double> > & candidate_list, int RCL_length, vector<double> & SL_avg, vector<double> & SL_best, double best_obj, double delta);
    void initialize_benefit(vector<pair<double, vec_int> > & sim_lambda_benefit, int * tooling_used, int * sim_used, int * SIM_number);
    void update_benefit(vector<pair<double, vec_int> > & sim_lambda_benefit, vector<int> & lot_remain, vector<int> & production, vector<int> & affected_dev, int * tooling_used, int * sim_used, int * SIM_number);
    void N1(vector<sub_soln_item> & sub_soln, vector<pair<int, double> > & shortage, vector<pair<pair<int, double>, double> > & unassigned_lots);
    void N2(vector<sub_soln_item> & sub_soln, vector<pair<int, double> > & shortage, vector<pair<pair<int, double>, double> > & unassigned_lots);
    void N3(vector<sub_soln_item> & sub_soln, vector<pair<int, double> > & shortage, vector<pair<pair<int, double>, double> > & unassigned_lots);
    int max_rate_s(int i, int l, int Lambda_id);
    double local_branch_LP(OsiXxxSolverInterface * si, struct soln_pool_item & soln_A, int K);
    bool make_feasible(int fix_y[]);
    double compute_obj(vector<sub_soln_item> & sub_soln, vector<pair<int, double> > & shortage);
    double compute_obj(vector<pair<sub_soln_item, double> > & enhanced_solution);
    void adjust_LP_solution(double * solution);
    int index_L_sim_lambda(int simid, int Lambda_id);
    int roundup(double f);
    int is_integer(double x);
    int select_alpha(double prob_list[], int length);
    void update_alpha_prob(double prob_list[], double avg_list[], int length, double best_so_far, double delta);
    int is_KD_PD(int dev);
    void temp_combination();
    void get_inter_set(struct t_set current_set, vector<t_set> & inter_set);
    int get_set_id(struct t_set current_set);
    int strtok2(char * pdes, char * porg, int start_id, char delim);
    int strtok2(char * pdes, char * porg, int start_id, char delim1, char delim2);
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
    void sort_lot_heur(int finished_dev_amount[]);
    void assign_heur(int ** tooling_used, int finished_dev_amount[]);
    void initialize_Machine_Heur();
    void show_Machine_Heur();
    void sort_TS(vector<tooling_configure> & TS);
    bool compare_tc(const tooling_configure & a, const tooling_configure & b);
    int AT_main();
    void pack(BCP_buffer & buf) const;
    void unpack(BCP_buffer & buf);
    void generate_reports(const double * solution);
    void soln_pool_item_to_solution(struct soln_pool_item & soln, double * solution);
    void minimize_makespan(struct soln_pool_item & soln);
    void show_targets();
    double get_PIN_PKG_penalty(int pkg_id, int pin_num);
    bool is_Key_PKG(int dev);
    bool is_Key_PIN_PKG(int dev);

    //------------------------------------------------------Functions for AT Enhancement

    void machine_resetup(struct soln_pool_item & GRASP_soln);
    void update_benefit_resetup(int current_machine, int current_Lambda, double current_time, vector<pair<double, vec_int> > & machine_lambda_benefit, vector<int> & lot_remain, vector<int> & production, vector<int> & affected_dev, int * tooling_used, int * sim_used, int * SIM_number, vector<int> & idle_machines);
    void construct_CL_resetup(vector<pair<pair<int, int>, double> > & CL, vector<pair<double, vec_int> > & machine_lambda_benefit);
    void extend_initial_lot(vector<initial_running_lot_item> & initial_running_lot);
    void process_initial_lot(vector<initial_running_lot_item> & initial_running_lot, double * H);
    void generate_reports(vector<initial_running_lot_item> & initial_running_lot, vector<pair<sub_soln_item, double> > & enhanced_solution);
    void str_to_date_time(char * ch, struct date_time & dt);
    double get_date_time_difference(const struct date_time & a, const struct date_time & b);
    struct date_time increase_date_time(const struct date_time & dt, double increment_hr);
    void output_date_time(struct date_time & dt);
    void initialize_tooling_trace(struct soln_pool_item & best_soln, vector<tooling_trace_item> & tooling_trace);
    void show_tooling_trace(vector<tooling_trace_item> & tooling_trace);

    //--------------------------------------------------Functions for logpoint

    int index_S_sim_l(int sim, int l);
    void initialize_S_sim_l();
    void show_S_sim_l();
    void initialize_S_sim_l_lambda();
    void show_S_sim_l_lambda();
    int index_S_sim_l_lambda(int sim, int l, int Lambda_id);

    ~AT();
};

/**
 * @brief 
 * 
 * @param v1
 * @param v2
 * 
 * @return 
 */
static bool compare_int_vec(vector<int> & v1, vector<int> & v2) {
    //the order doesn't matter
    //true: v1==v2; false o.w.
    if ((int) v1.size() != (int) v2.size()) {
        return false;
    } else {
        //test if v1 is a subset of v2
        int flag1 = 1;
        vii p_int = v1.begin();
        while (p_int != v1.end()) {
            vii p_find = find(v2.begin(), v2.end(), (*p_int));
            if (p_find == v2.end()) {
                flag1 = 0;
                break;
            }
            p_int++;
        }
        if (!flag1) {
            return flag1;
        } else {
            int flag2 = 1;
            vii p_int = v2.begin();
            while (p_int != v2.end()) {
                vii p_find = find(v1.begin(), v1.end(), (*p_int));
                if (p_find == v1.end()) {
                    flag2 = 0;
                    break;
                }
                p_int++;
            }
            return flag2;
        }
    }
}

/**
 * @brief 
 * 
 * @param can1
 * @param can2
 * 
 * @return 
 */
static bool compare_candidates(const pair<pair<int, int>, double> & can1,
        const pair<pair<int, int>, double> & can2) {
    double weight1 = can1.second;
    double weight2 = can2.second;
    if (weight1 > weight2) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief 
 * 
 * @param lot1
 * @param lot2
 * 
 * @return 
 */
static bool compare_lot(const pair<int, double> & lot1,
        const pair<int, double> & lot2) {
    double unit_price1 = lot1.second;
    double unit_price2 = lot2.second;
    if (unit_price1 > unit_price2) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief 
 * 
 * @param lot1
 * @param lot2
 * 
 * @return 
 */
static bool compare_2_3(const pair<pair<int, double>, double> & lot1,
        const pair<pair<int, double>, double> & lot2) {
    if (lot1.first.second > lot2.first.second) {
        return true;
    } else if (lot1.first.second < lot2.first.second) {
        return false;
    } else {
        if (lot1.second > lot2.second) {
            return true;
        } else {
            return false;
        }
    }
}

/**
 * @brief 
 * 
 * @param soln1
 * @param soln2
 * 
 * @return 
 */
static bool compare_soln(const soln_pool_item & soln1,
        const soln_pool_item & soln2) {
    if (soln1.obj < soln2.obj) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief 
 * 
 * @param v1
 * @param v2
 * 
 * @return 
 */
static bool compare_int_double(const pair<int, double> &v1,
        const pair<int, double> &v2) {
    if (v1.second < v2.second) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief 
 * 
 * @param v1
 * @param v2
 * 
 * @return 
 */
static bool compare_int_int_double(const pair<int, pair<int, double> > &v1,
        const pair<int, pair<int, double> > &v2) {
    if (v1.second.second < v2.second.second) {
        return true;
    } else {
        return false;
    }
}

#endif

// __END__
