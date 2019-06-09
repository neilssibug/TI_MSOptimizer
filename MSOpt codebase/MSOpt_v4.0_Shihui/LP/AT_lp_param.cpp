// $Id: AAP_lp_param.cpp,v 1.2 2003/12/03 01:52:30 magh Exp $

/* ------------------------------------------------------------------------
 Author: Matthew Galati (magh@lehigh.edu)

 (c) Copyright 2003 Lehigh University. All Rights Reserved.

 This software is licensed under the Common Public License. Please see 
 accompanying file for terms.    
 ---------------------------------------------------------------------------*/

#include "BCP_parameters.hpp"
#include "AT_lp_param.hpp"
using std::make_pair;

/*-----------------------------------------------------------------*/
template <>
void BCP_parameter_set<AT_lp_par>::create_keyword_list() {
	// Int Parameter
	keys.push_back(make_pair(BCP_string("AT_StrongBranch_Can"),BCP_parameter(BCP_IntPar,AT_StrongBranch_Can )));
}

/*-----------------------------------------------------------------*/
template <>
void BCP_parameter_set<AT_lp_par>::set_default_entries(){
	set_entry(AT_StrongBranch_Can, 5);
}
