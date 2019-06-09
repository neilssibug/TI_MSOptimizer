// $Id: AAP_lp_param.hpp,v 1.2 2003/12/03 01:52:30 magh Exp $

/* ------------------------------------------------------------------------
 Author: Matthew Galati (magh@lehigh.edu)

 (c) Copyright 2003 Lehigh University. All Rights Reserved.

 This software is licensed under the Common Public License. Please see 
 accompanying file for terms.    
---------------------------------------------------------------------------*/

#ifndef AT_LP_PARAM_H
#define AT_LP_PARAM_H

/*
  AAP_StrongBranch_Can : number of candidates to select for strong branching
*/

/*-----------------------------------------------------------------*/
struct AT_lp_par {
  enum chr_params{
    char_dummy,
    end_of_chr_params
  };
  enum int_params{
    AT_StrongBranch_Can,
    end_of_int_params
  };
  enum dbl_params{
    dbl_dummy,
    end_of_dbl_params
  };
  enum str_params{
    str_dummy,
    end_of_str_params
  };
  enum str_array_params{
    str_array_dummy,
    end_of_str_array_params
  };
};

#endif
