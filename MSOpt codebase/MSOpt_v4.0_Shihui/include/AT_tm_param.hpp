// $Id: AAP_tm_param.hpp,v 1.2 2003/12/03 01:52:30 magh Exp $

/* ------------------------------------------------------------------------
 Author: Matthew Galati (magh@lehigh.edu)

 (c) Copyright 2003 Lehigh University. All Rights Reserved.

 This software is licensed under the Common Public License. Please see 
 accompanying file for terms.    
---------------------------------------------------------------------------*/

#ifndef AT_TM_PARAM_H
#define AT_TM_PARAM_H

/*
  AAP_datafile  : data file
*/

/*-----------------------------------------------------------------*/
struct AT_tm_par {
  enum chr_params{
    char_dummy,
    end_of_chr_params
  };
  enum int_params{
    int_dummy,
    end_of_int_params
  };
  enum dbl_params{
    dbl_dummy,
    end_of_dbl_params
  };
  enum str_params{
    AT_input,
    AT_device,
    AT_key,
    AT_package,
    AT_lot,
    AT_machine_hours,
    AT_machine_mapping,
    AT_machines,
    AT_tooling,
    AT_tooling_mapping,
    AT_tooling_family,
    AT_wip,
    end_of_str_params
  };
  enum str_array_params{
    str_array_dummy,
    end_of_str_array_params
  };
};

#endif

