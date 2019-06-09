// $Id: AAP_lp.hpp,v 1.2 2003/12/03 01:52:30 magh Exp $

/* ------------------------------------------------------------------------
Author: Matthew Galati (magh@lehigh.edu)

(c) Copyright 2003 Lehigh University. All Rights Reserved.

This software is licensed under the Common Public License. Please see 
accompanying file for terms.    
---------------------------------------------------------------------------*/

#ifndef AT_LP_H
#define AT_LP_H

#include "BCP_lp_user.hpp"
#include "BCP_parameters.hpp"

#include "AT_lp_param.hpp"
#include "AT_var.hpp"
#include "AT_GRASP.h"

#include <vector>
#include <functional>
using namespace std;

const double tolerance = 0.0001;
const double bigM      = 100000;
using namespace std;
using std::make_pair;
/*--------------------------------------------------------------------------*/
class is_less_than{
public:
  bool operator()( const pair< pair<int,int>,double> & x,
                   const pair< pair<int,int>,double> & y)
  {
	  if(x.second > y.second + tolerance){
		  return false;
	  }else{
		  return true;
	  }
  }
};
/*--------------------------------------------------------------------------*/
//Class for the user-defined LP interface to BCP

class AT_lp : public BCP_lp_user{
public:
	BCP_parameter_set<AT_lp_par> lp_par; //parameter list for LP
	AT  * at;
	vector<AT_var*> generated_vars;      //variables generated in compute LB
	//stores the mapping from strong branch candidate to original master variable
	vector<pair<int,pair<int,int> > > cand_to_variable;       
	ofstream fdebug_vars;
	vector<double *> subprob_col_lb;
	vector<double *> subprob_col_ub;
	vector<double *> subprob_row_lb;
	vector<double *> subprob_row_ub;
	vector<CoinPackedMatrix *> subprob_M;

public:
	AT_lp() : lp_par(), at(0) { 
		fdebug_vars.open("debug_vars.txt");
		subprob_col_lb.clear();
		subprob_col_ub.clear();
		subprob_row_lb.clear();
		subprob_row_ub.clear();
		subprob_M.clear();
	}

	~AT_lp() {
		delete at;
		fdebug_vars.close();
		for(int i=0;i<=(int)subprob_col_lb.size()-1;i++){
			if(subprob_col_lb[i]){
				delete [] subprob_col_lb[i];
			}
		}
		for(int i=0;i<=(int)subprob_col_ub.size()-1;i++){
			if(subprob_col_ub[i]){
				delete [] subprob_col_ub[i];
			}
		}
		for(int i=0;i<=(int)subprob_row_lb.size()-1;i++){
			if(subprob_row_lb[i]){
				delete [] subprob_row_lb[i];
			}
		}
		for(int i=0;i<=(int)subprob_row_ub.size()-1;i++){
			if(subprob_row_ub[i]){
				delete [] subprob_row_ub[i];
			}
		}
		for(int i=0;i<=(int)subprob_M.size()-1;i++){
			if(subprob_M[i]){
				delete [] subprob_M[i];
			}
		}
		subprob_col_lb.clear();
		subprob_col_ub.clear();
		subprob_row_lb.clear();
		subprob_row_ub.clear();
		subprob_M.clear();
	};    

public:
	virtual void unpack_module_data(BCP_buffer& buf);
	virtual void pack_var_algo(const BCP_var_algo* var, BCP_buffer& buf);
	virtual BCP_var_algo* unpack_var_algo(BCP_buffer& buf);
	virtual void pack_user_data(const BCP_user_data* ud, BCP_buffer& buf);
	virtual BCP_user_data* unpack_user_data(BCP_buffer& buf);

public:
	virtual OsiSolverInterface* initialize_solver_interface();
	virtual void vars_to_cols(const BCP_vec<BCP_cut*>& cuts,
		BCP_vec<BCP_var*>& vars,       
		BCP_vec<BCP_col*>& cols,
		const BCP_lp_result& lpres,
		BCP_object_origin origin, bool allow_multiple);
	virtual void display_lp_solution(const BCP_lp_result& lpres,
		const BCP_vec<BCP_var*>& vars,
		const BCP_vec<BCP_cut*>& cuts,
		const bool final_lp_solution);
	virtual double compute_lower_bound(const double old_lower_bound,
		const BCP_lp_result& lpres,
		const BCP_vec<BCP_var*>& vars,
		const BCP_vec<BCP_cut*>& cuts);
	virtual void generate_vars_in_lp(const BCP_lp_result& lpres,
		const BCP_vec<BCP_var*>& vars,
		const BCP_vec<BCP_cut*>& cuts,
		const bool before_fathom,
		BCP_vec<BCP_var*>& new_vars,
		BCP_vec<BCP_col*>& new_cols);
	virtual void restore_feasibility(const BCP_lp_result& lpres,
		const std::vector<double*> dual_rays,
		const BCP_vec<BCP_var*>& vars,
		const BCP_vec<BCP_cut*>& cuts,
		BCP_vec<BCP_var*>& vars_to_add,
		BCP_vec<BCP_col*>& cols_to_add);

public:
	virtual BCP_branching_decision
		select_branching_candidates(const BCP_lp_result& lpres,
		const BCP_vec<BCP_var*>& vars,
		const BCP_vec<BCP_cut*>& cuts,
		const BCP_lp_var_pool& local_var_pool,
		const BCP_lp_cut_pool& local_cut_pool,
		BCP_vec<BCP_lp_branching_object*>& cans);
	virtual void set_user_data_for_children(BCP_presolved_lp_brobj* best,
		const int selected);

public:
	int solve_subproblem(int machine_id,double * costv,double * solution,double & objective_value);
	void construct_cost(const double * pi, const bool from_restore,int machine_id,double * costv);
	bool generate_vars(const BCP_vec<BCP_var*>& vars,const double * pi, const bool from_restore);
	void branch_close_to_half(const BCP_lp_result& lpres,const BCP_vec<BCP_var*>& vars,BCP_vec<BCP_lp_branching_object*>& cans);
	void append_branching_vars(const BCP_lp_result& lpres,const BCP_vec<BCP_var*>& vars
		,const vector< pair<pair<int,int>,double> > & var_candidates
		,BCP_vec<BCP_lp_branching_object*>& cans);
	void create_subprob_matrix();
	int solve_subproblem_sim(int sim,double * costv,double * solution,double & objective_value);
};

#endif
