// $Id: AAP_tm_param.cpp,v 1.2 2003/12/03 01:52:30 magh Exp $

/*-------------------------------------------------------------------------
 Author: Matthew Galati (magh@lehigh.edu)

 (c) Copyright 2003 Lehigh University. All Rights Reserved.

 This software is licensed under the Common Public License. Please see 
 accompanying file for terms.    
---------------------------------------------------------------------------*/

#include "BCP_parameters.hpp"
#include "AT_tm_param.hpp"
using std::make_pair;

/*-----------------------------------------------------------------*/
template <>
void BCP_parameter_set<AT_tm_par>::create_keyword_list() {
  // String Parameter
  keys.push_back(make_pair(BCP_string("AT_input"),BCP_parameter(BCP_StringPar,AT_input)));
  keys.push_back(make_pair(BCP_string("AT_device"),BCP_parameter(BCP_StringPar,AT_device)));
  keys.push_back(make_pair(BCP_string("AT_key"),BCP_parameter(BCP_StringPar,AT_device)));
  keys.push_back(make_pair(BCP_string("AT_package"),BCP_parameter(BCP_StringPar,AT_package)));
  keys.push_back(make_pair(BCP_string("AT_lot"),BCP_parameter(BCP_StringPar,AT_lot)));
  keys.push_back(make_pair(BCP_string("AT_machine_hours"),BCP_parameter(BCP_StringPar,AT_machine_hours)));
  keys.push_back(make_pair(BCP_string("AT_machine_mapping"),BCP_parameter(BCP_StringPar,AT_machine_mapping)));
  keys.push_back(make_pair(BCP_string("AT_machines"),BCP_parameter(BCP_StringPar,AT_machines)));
  keys.push_back(make_pair(BCP_string("AT_tooling"),BCP_parameter(BCP_StringPar,AT_tooling)));
  keys.push_back(make_pair(BCP_string("AT_tooling_mapping"),BCP_parameter(BCP_StringPar,AT_tooling_mapping)));
  keys.push_back(make_pair(BCP_string("AT_tooling_family"),BCP_parameter(BCP_StringPar,AT_tooling_family)));
  keys.push_back(make_pair(BCP_string("AT_wip"),BCP_parameter(BCP_StringPar,AT_wip)));
}

/*-----------------------------------------------------------------*/
template <>
void BCP_parameter_set<AT_tm_par>::set_default_entries(){
  set_entry(AT_input,"Data/input.txt");  
  set_entry(AT_device,"Data/device_mapping.csv");
  set_entry(AT_key,"Data/keydevices.csv");
  set_entry(AT_package,"Data/package.csv");
  set_entry(AT_lot,"Data/lot_mapping.csv");
  set_entry(AT_machine_hours,"Data/machine_hours.csv");
  set_entry(AT_machine_mapping,"Data/machine_mapping.csv");
  set_entry(AT_machines,"Data/machines.csv");
  set_entry(AT_tooling,"Data/tooling.csv");
  set_entry(AT_tooling_mapping,"Data/tooling_mapping.csv");
  set_entry(AT_tooling_family,"Data/toolingfamily_temperature.csv");
  set_entry(AT_wip,"Data/wip.csv");  
}
