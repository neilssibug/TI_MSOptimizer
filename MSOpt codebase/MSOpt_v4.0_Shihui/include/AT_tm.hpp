// $Id: AAP_tm.hpp,v 1.2 2003/12/03 01:52:30 magh Exp $

/* ------------------------------------------------------------------------
Author: Matthew Galati (magh@lehigh.edu)

(c) Copyright 2003 Lehigh University. All Rights Reserved.

This software is licensed under the Common Public License. Please see 
accompanying file for terms.    
---------------------------------------------------------------------------*/

#ifndef AT_TM_H
#define AT_TM_H

#include "BCP_parameters.hpp"
#include "BCP_tm_user.hpp"
#include "AT_tm_param.hpp"
#include "AT_lp_param.hpp"
#include "AT_GRASP.h"

#include <vector>
using namespace std;

/*--------------------------------------------------------------------------*/
//Class for the user-defined TM interface to BCP

class AT_tm : public BCP_tm_user {
public:
	BCP_parameter_set<AT_tm_par> tm_par; //parameter list for TM
	BCP_parameter_set<AT_lp_par> lp_par; //parameter list for LP
	AT * at;                             //storage for instance of AT
	double * epsilon;
	double * delta;

public:
	AT_tm() : tm_par(), lp_par(), at(0), epsilon(0), delta(0) {}

	~AT_tm() { 
		delete at;
		delete epsilon;
		delete delta;
	};

public:
	virtual void pack_module_data(BCP_buffer& buf, BCP_process_t ptype);
	virtual void pack_var_algo(const BCP_var_algo* var, BCP_buffer& buf);
	virtual BCP_var_algo* unpack_var_algo(BCP_buffer& buf);
	virtual void pack_user_data(const BCP_user_data* ud, BCP_buffer& buf);
	virtual BCP_user_data* unpack_user_data(BCP_buffer& buf);

public:
	virtual void init_new_phase(int phase, BCP_column_generation& colgen);
	virtual void initialize_core(BCP_vec<BCP_var_core*>& vars,
		BCP_vec<BCP_cut_core*>& cuts,
		BCP_lp_relax*& matrix);
	virtual void create_root(BCP_vec<BCP_var*>& added_vars,
		BCP_vec<BCP_cut*>& added_cuts,
		BCP_user_data * & user_data,
		BCP_pricing_status& pricing_status);
	virtual void display_feasible_solution(const BCP_solution* sol);
};

#endif
