// $Id: AAP_init.cpp,v 1.2 2003/12/03 01:52:30 magh Exp $

/*-------------------------------------------------------------------------
Author: Matthew Galati (magh@lehigh.edu)

(c) Copyright 2003 Lehigh University. All Rights Reserved.

This software is licensed under the Common Public License. Please see 
accompanying file for terms.    
---------------------------------------------------------------------------*/

#include "AT_init.hpp"
#include "AT_tm.hpp"
#include "AT_lp.hpp"
#include "AT_GRASP.h"

/*---------------------------------------------------*/
USER_initialize * BCP_user_init(){
	return new AT_init;
}
/*---------------------------------------------------*/
int main(int argc, char* argv[])
{
	//	AAP_init aap_init;
	return bcp_main(argc, argv, BCP_user_init());
}
/*---------------------------------------------------*/
BCP_lp_user * AT_init::lp_init(BCP_lp_prob& p){
	//Initialize the user-defined LP interface to BCP
	std::cout<<"in lp_init()"<<endl;

	return new AT_lp;
}

/*---------------------------------------------------*/
BCP_tm_user * AT_init::tm_init(BCP_tm_prob& p,const int argnum, const char * const * arglist){

	//Initialize the user-defined TM interface to BCP
	std::cout<<"in tm_init()"<<endl;
	AT_tm* tm = new AT_tm;   

	//Read in the TM and LP parameters from the arg ParamFile: parameter file
	std::cout<<"read in the TM and LP parameters"<<endl;
	tm->tm_par.read_from_arglist(argnum,arglist);
	tm->lp_par.read_from_arglist(argnum,arglist);
	std::cout<<"end reading"<<endl;

	//Construct the AT data object
	tm->at = new AT();
	(*(tm->at)).AT_main();  
	tm->epsilon = new double[(*(tm->at)).Num_Lot+1];
	tm->delta   = new double[(*(tm->at)).Num_Lot+1];
	for(int l=1;l<=(*(tm->at)).Num_Lot;l++){
		//(*tm).epsilon[l]=0.1*l;
		//(*tm).delta[l]=-1000;
		(*tm).epsilon[l]=0.0;
		(*tm).delta[l]=0.0;
		//cout<<"l="<<l<<",epsilon="<<(*tm).epsilon[l]<<",delta="<<(*tm).delta[l]<<endl;
	}
	return tm;
}
