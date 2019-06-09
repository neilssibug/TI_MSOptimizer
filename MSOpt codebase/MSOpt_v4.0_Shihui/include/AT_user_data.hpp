// $Id: AAP_user_data.hpp,v 1.3 2003/12/03 01:52:30 magh Exp $

/* ------------------------------------------------------------------------
 Author: Matthew Galati (magh@lehigh.edu)

 (c) Copyright 2003 Lehigh University. All Rights Reserved.

 This software is licensed under the Common Public License. Please see 
 accompanying file for terms.    
---------------------------------------------------------------------------*/

#ifndef AT_USER_DATA_H
#define AT_USER_DATA_H

#include "BCP_USER.hpp"
#include "BCP_buffer.hpp"

#include <vector>
using namespace std;
using std::make_pair;
/*---------------------------------------------------*/
//Class to store extra information to be sent between 
//nodes after branching. In this case, we just store 
//the indices of the (original) variables that were 
//branched on.

class AT_user_data : public BCP_user_data {
public:
	vector<pair<int,int> > ones;
	vector<pair<int,int> > zeros;

public:
	AT_user_data() : BCP_user_data() {}

	AT_user_data(vector<pair<int,int> > & ones_, vector<pair<int,int> > & zeros_)	: BCP_user_data(), ones(ones_), zeros(zeros_) {}

	AT_user_data(BCP_buffer & buf):BCP_user_data(){
		unpack(buf);
	}
	AT_user_data(const AT_user_data & x):BCP_user_data(), ones(x.ones), zeros(x.zeros) {}
	~AT_user_data() {
		ones.clear();
		zeros.clear();
	}

public:
  void pack(BCP_buffer & buf) const{
    //Pack the user data
    buf.pack(ones).pack(zeros);
  }

  void unpack(BCP_buffer & buf){
    //Unpack the user data
    buf.unpack(ones).unpack(zeros);
  }
};

#endif
