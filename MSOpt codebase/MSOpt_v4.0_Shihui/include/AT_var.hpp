// $Id: AAP_var.hpp,v 1.2 2003/12/03 01:52:30 magh Exp $

/* ------------------------------------------------------------------------
 Author: Matthew Galati (magh@lehigh.edu)

 (c) Copyright 2003 Lehigh University. All Rights Reserved.

 This software is licensed under the Common Public License. Please see 
 accompanying file for terms.    
---------------------------------------------------------------------------*/

#ifndef AT_VAR_H
#define AT_VAR_H

#include "BCP_var.hpp"
#include "BCP_buffer.hpp"
#include "AT_GRASP.h"
#include "CoinHelperFunctions.hpp"

#include <iostream>
#include <vector>
#include <algorithm>
#include<stdlib.h>
#include<stdio.h>

using std::make_pair;
using namespace std;

/*--------------------------------------------------------------------------*/
/* Class for an AAP algorithmic variable (derived from a BCP_var_algo).
   A variable in this context is an point that satisfies the assignment
   constraints over J x K.
*/
class AT_var : public BCP_var_algo{
public:
	int i;
	int lambda_id;
	vector< pair<int,int> > L_S;
public:
	AT_var() : BCP_var_algo(BCP_IntegerVar, 0.0, 0.0, DBL_MAX){
		cout<<"construct AT_var"<<endl;
		i=0;
		lambda_id=0;
		L_S.clear();
	}

	AT_var(int i_input,int lambda_id_input,const vector< pair<int,int> > & LS_input):BCP_var_algo(BCP_IntegerVar,0.0,0.0,DBL_MAX){
		i=i_input;
		lambda_id=lambda_id_input;
		L_S.resize((int)LS_input.size());
		copy(LS_input.begin(),LS_input.end(),L_S.begin());
	}

	AT_var(const AT_var & x) :
	BCP_var_algo(BCP_IntegerVar,0.0, 0.0, DBL_MAX), i(x.i), lambda_id(x.lambda_id), L_S(x.L_S) {} 

	AT_var(BCP_buffer & buf):BCP_var_algo(BCP_IntegerVar, 0.0, 0.0, DBL_MAX){
		i=0;
		lambda_id=0;
		//Constructor given a buffer stream - unpacks the buffer stream 
		//into this's data members
		unpack(buf);
	}
	~AT_var() {
		i=0;
		lambda_id=0;
		L_S.clear();
	};

public:
	void pack(BCP_buffer & buf) const {
		//Pack the variable
		buf.pack(i);
		buf.pack(lambda_id);
		buf.pack(L_S);
	}

	void unpack(BCP_buffer & buf){
		//Unpack the variable
		buf.unpack(i);
		buf.unpack(lambda_id);
		buf.unpack(L_S);
	} 

	void print(ostream * os = &cout) const {
		(*os)<<"i="<<i<<",lambda_id="<<lambda_id<<endl;
		vector< pair<int,int> >::const_iterator pLS=L_S.begin();
		while(pLS!=L_S.end()){
			(*os)<<"l="<<(*pLS).first<<",s="<<(*pLS).second<<endl;
			pLS++;
		}
	}
};

#endif
