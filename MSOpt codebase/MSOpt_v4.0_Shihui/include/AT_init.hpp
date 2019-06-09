// $Id: AAP_init.hpp,v 1.2 2003/12/03 01:52:30 magh Exp $

/* ------------------------------------------------------------------------
 Author: Matthew Galati (magh@lehigh.edu)

 (c) Copyright 2003 Lehigh University. All Rights Reserved.

 This software is licensed under the Common Public License. Please see 
 accompanying file for terms.    
---------------------------------------------------------------------------*/

#ifndef AT_INIT_H
#define AT_INIT_H

#include "BCP_USER.hpp"
//#include "BCP_buffer.hpp"

//#include <vector>
//using namespace std;

/*---------------------------------------------------*/
//Class for the user-defined USER interface to BCP which 
//initializes each process. In this case, just TM and LP. 
class AT_init : public USER_initialize {
public:
   BCP_tm_user * tm_init(BCP_tm_prob& p,const int argnum,const char * const * arglist);
   BCP_lp_user * lp_init(BCP_lp_prob& p);
};

#endif
