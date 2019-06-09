// AT.cpp : Defines the entry point for the console application.
//----------------------------------------------
//#include<ilcplex/ilocplex.h>
//#include<ilconcert/ilolinear.h>
#include<iostream>
#include<sstream>
#include<cstring>
#include<string>
#include<fstream>
#include<math.h>
#include<algorithm>
#include<vector>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include <iomanip>
#include <cassert>
#include "combination.h"
#include "AT_CPLEX.h"

using namespace std;
using namespace stdcomb;

AT::AT(){
	Num_Tooling=0;
	Num_Tooling_Family=0;
	Num_Machine=0;
	Num_Machine_Group=0;
	Num_Lot=0;
	Num_Device=0;
	Num_Keydevice=0;
	Num_Package=0;
	Num_Temperature=0;
	x_limit=0;
	y_limit=0;
	Num_LP_solved=0;
	Num_Simulated=0;
	Num_Discarded=0;
	cout<<"----------------- Important parameters -----------------"<<endl;
	cout<<"Max_Num_Tooling="<<Max_Num_Tooling<<endl;
	cout<<"Max_Num_Tooling_Family="<<Max_Num_Tooling_Family<<endl;
	cout<<"Max_Num_Machine="<<Max_Num_Machine<<endl;
	cout<<"Max_Num_Lot="<<Max_Num_Lot<<endl;
	cout<<"Max_Num_Device="<<Max_Num_Device<<endl;
	cout<<"Max_Num_Keydevice="<<Max_Num_Keydevice<<endl;
	cout<<"Max_Num_Package="<<Max_Num_Package<<endl;
	cout<<"Max_Num_Temperature="<<Max_Num_Temperature<<endl;
	cout<<"Max_Num_Tooling_Configure="<<Max_Num_Tooling_Configure<<endl;
}

AT::~AT(){
	for(int id=1;id<=(int)MF.size();id++){
		delete [] MF[id-1];
	}
	MF.clear();
	for(int id=1;id<=(int)MI.size();id++){
		delete [] MI[id-1];
	}
	MI.clear();
	for(int id=1;id<=(int)TF.size();id++){
		delete [] TF[id-1];
	}
	TF.clear();
	for(int id=1;id<=(int)TI.size();id++){
		delete [] TI[id-1];
	}
	TI.clear();
		for(int id=1;id<=(int)LOT.size();id++){
		delete [] LOT[id-1];
	}
	LOT.clear();
	for(int id=1;id<=(int)DEV.size();id++){
		delete [] DEV[id-1];
	}
	DEV.clear();
	for(int id=1;id<=(int)PKG.size();id++){
		delete [] PKG[id-1];
	}
	PKG.clear();
	delete [] PIN_PKG_penalty;
}

int AT::AT_main()
{	
	time_t t_prog_start;
	time(&t_prog_start);
	cout<<"The starting time is "<<ctime(&t_prog_start)<<endl;
	//test();
	f_log.open("Debug/debug_AT.txt");
	char temp[1000],pdes[500];
	ifstream f_input("Data/input.txt");
	f_input.getline(temp,1000,'\n');
	int k=0,l=0;
	k=strtok2(pdes,temp,l,' ');
	Generate_Option=atoi(pdes);
	f_input.getline(temp,1000,'\n');
	l=0;
	k=strtok2(pdes,temp,l,' ');
	Run_Option=atoi(pdes);
	f_input.getline(temp,1000,'\n');
	l=0;
	k=strtok2(pdes,temp,l,' ');
	LP_IP=atoi(pdes);
	f_input.getline(temp,1000,'\n');
	l=0;
	k=strtok2(pdes,temp,l,' ');
	new_old_model=atoi(pdes);
	if(!Generate_Option){//generate the case randomly
		f_input.getline(temp,1000,'\n');//Seed
		k=0,l=0;
		k=strtok2(pdes,temp,l,' ');
		Seed=atoi(pdes);
		f_input.getline(temp,1000,'\n');//Max_Iter
		l=0;
		k=strtok2(pdes,temp,l,' ');
		Max_Iter=atoi(pdes);
		f_input.getline(temp,1000,'\n');//Num_Lot
		l=0;
		k=strtok2(pdes,temp,l,' ');
		Num_Lot=atoi(pdes);
		f_input.getline(temp,1000,'\n');//Num_Machine
		l=0;
		k=strtok2(pdes,temp,l,' ');
		Num_Machine=atoi(pdes);
		f_input.getline(temp,1000,'\n');//num_mg
		l=0;
		k=strtok2(pdes,temp,l,' ');
		num_mg=atoi(pdes);
		f_input.getline(temp,1000,'\n');//Num_Tooling
		l=0;
		k=strtok2(pdes,temp,l,' ');
		Num_Tooling=atoi(pdes);
		f_input.getline(temp,1000,'\n');//Num_Tooling_Family
		l=0;
		k=strtok2(pdes,temp,l,' ');
		Num_Tooling_Family=atoi(pdes);
		f_input.getline(temp,1000,'\n');//Num_Device
		l=0;
		k=strtok2(pdes,temp,l,' ');
		Num_Device=atoi(pdes);
		f_input.getline(temp,1000,'\n');//Num_Keydevice
		l=0;
		k=strtok2(pdes,temp,l,' ');
		Num_Keydevice=atoi(pdes);
		f_input.getline(temp,1000,'\n');//Num_Package
		l=0;
		k=strtok2(pdes,temp,l,' ');
		Num_Package=atoi(pdes);
		f_input.getline(temp,1000,'\n');//Num_Temperature
		l=0;
		k=strtok2(pdes,temp,l,' ');
		Num_Temperature=atoi(pdes);
		f_input.getline(temp,1000,'\n'); //SIM_option
		l=0;
		k=strtok2(pdes,temp,l,' ');
		SIM_option=atoi(pdes);
		f_input.getline(temp,1000,'\n'); //PhaseII_option
		l=0;
		k=strtok2(pdes,temp,l,' ');
		PhaseII_option=atoi(pdes);
		cout<<"Generate_Option="<<Generate_Option<<endl;
		cout<<"Num_Lot="<<Num_Lot<<endl;
		cout<<"SIM_option = "<<SIM_option<<endl;
		cout<<"PhaseII_option = "<<PhaseII_option<<endl;
		random_case();
	}else{
		f_input.getline(temp,1000,'\n');
		f_input.getline(temp,1000,'\n'); //Max_Iter
		l=0;
		k=strtok2(pdes,temp,l,' ');
		Max_Iter=atoi(pdes);
		f_input.getline(temp,1000,'\n'); //skip Num_Lot
		f_input.getline(temp,1000,'\n'); //skip Num_Machine
		f_input.getline(temp,1000,'\n'); //skip num_mg
		f_input.getline(temp,1000,'\n'); //skip Num_Tooling
		f_input.getline(temp,1000,'\n'); //skip Num_Tooling_Family
		f_input.getline(temp,1000,'\n'); //skip Num_Device
		f_input.getline(temp,1000,'\n'); //skip Num_Keydevice
		f_input.getline(temp,1000,'\n'); //skip Num_Package
		f_input.getline(temp,1000,'\n'); //skip Num_Temperature
		f_input.getline(temp,1000,'\n'); //SIM_option
		l=0;
		k=strtok2(pdes,temp,l,' ');
		SIM_option=atoi(pdes);
		f_input.getline(temp,1000,'\n'); //PhaseII_option
		l=0;
		k=strtok2(pdes,temp,l,' ');
		PhaseII_option=atoi(pdes);
		create_case_from_files();
	}
	f_input.close();
	f_log.close();

	time_t end_time;
	time(&end_time);
	cout<<"The end time is "<<ctime(&end_time)<<endl;
	double diff=difftime(end_time,t_prog_start);
	cout<<"It has been running for "<<diff<<" seconds."<<endl;

	return 0;
}

void AT::readFiles(){
	//read the files and process tha data
	char temp[1000];
	char pdes[100];
	//read machines.csv
	ifstream f_machines("Data/machines.csv");
	f_machines.getline(temp,1000,'\n');
	f_machines.getline(temp,1000,'\n');
	Num_Machine=0;
	Num_Machine_Group=0;
	Num_Temperature=0;
	while(!f_machines.eof()){
		int l=0,k=0;
		for(int i=1;i<=50;i++){ pdes[i]=' ';}
		k=strtok2(pdes,temp,l,',');
		trim(pdes,' ');
		char * mf_name=new char[50];
		strcpy(mf_name,pdes);
		char * mi_name=new char[50];
		l=k;
		k=strtok2(pdes,temp,l,',');
		trim(pdes,' ');
		strcpy(mi_name,pdes);
		bool MF_name_exist=false;
		bool MI_name_exist=false;
		for(int id=1;id<=(int)MF.size();id++){
			if(!strcmp(MF[id-1],mf_name)){//if name already exists in MF
				delete [] mf_name;
				MF_name_exist=true; break;
			}
		}
		for(int id=1;id<=(int)MI.size();id++){
			if(!strcmp(MI[id-1],mi_name)){//if name already exists in MI
				delete [] mi_name;
				MI_name_exist=true; break;
			}
		}
		if(!MF_name_exist){//name does not exist in MF
			MF.push_back(mf_name);
		}
		if(!MI_name_exist){//name does not exist in MI
			MI.push_back(mi_name);
		}
		f_machines.getline(temp,1000,'\n');
	}
	ofstream f_MF("Debug/debug_machine_family_name.txt");
	//cout<<"output the machine family (MF) names"<<endl;
	for(int id=1;id<=(int)MF.size();id++){
		f_MF<<"id="<<id<<",name="<<MF[id-1]<<endl;
	}
	f_MF.close();
	f_MF.clear();
	ofstream f_MI("Debug/debug_machine_name.txt");
	//cout<<"output the machine instance (MI) names"<<endl;
	for(int id=1;id<=(int)MI.size();id++){
		f_MI<<"id="<<id<<",name="<<MI[id-1]<<endl;
	}
	f_MI.close();
	f_MI.clear();
	Num_Machine_Group=(int)MF.size();
	//-----------------------------------------------------------------
	f_machines.close();
	f_machines.clear();
	f_machines.open("Data/machines.csv");
	f_machines.getline(temp,1000,'\n');
	f_machines.getline(temp,1000,'\n');
	while(!f_machines.eof()){
		int l=0,k=0;
		for(int i=1;i<=50;i++){ pdes[i]=' ';}
		k=strtok2(pdes,temp,l,',');
		trim(pdes,' ');
		int mg=0;
		for(int id=1;id<=(int)MF.size();id++){
			if(!strcmp(MF[id-1],pdes)){ mg=id;break;};
		}
		if(mg>0){
			struct Machine one_mc;
			one_mc.mg=mg;
			l=strtok2(pdes,temp,k,',');
			trim(pdes,' ');
			int mi=0;
			for(int id=1;id<=(int)MI.size();id++){
				if(!strcmp(MI[id-1],pdes)){ mi=id;break;};
			}
			if(mi<1){ cout<<"Wrong machine id"<<endl; abort();}
			one_mc.id=mi;
			k=strtok2(pdes,temp,l,',');//name
			trim(pdes,' ');
			strcpy(one_mc.name,MI[one_mc.id-1]);
			l=strtok2(pdes,temp,k,',');//certification
			trim(pdes,' ');
			int temperature=atoi(pdes);
			if(temperature>Num_Temperature){ Num_Temperature=temperature;}
			if(temperature>0){
				one_mc.temperature_vector.push_back(temperature);
			}
			Num_Machine++;
			Machine_vector.push_back(one_mc);
		}else{
			l=strtok2(pdes,temp,k,',');
			trim(pdes,' ');
			k=strtok2(pdes,temp,l,',');
			trim(pdes,' ');
			l=strtok2(pdes,temp,k,',');
			trim(pdes,' ');
			int temperature=atoi(pdes);
			if(temperature>Num_Temperature){ Num_Temperature=temperature;}
			if(temperature>0){
				vector<Machine>::iterator p_M=Machine_vector.end()-1;
				(*p_M).temperature_vector.push_back(temperature);
			}
		}
		f_machines.getline(temp,1000,'\n');
	}
	f_machines.close();
	show_Machine_vector();
	//read tooling.csv
	ifstream f_tooling("Data/tooling.csv");
	f_tooling.getline(temp,1000,'\n');
	f_tooling.getline(temp,1000,'\n');
	Num_Tooling=0;
	Num_Tooling_Family=0;
	while(!f_tooling.eof()){
		int l=0,k=0;
		for(int i=1;i<=50;i++){ pdes[i]=' ';}
		k=strtok2(pdes,temp,l,',');
		trim(pdes,' ');
		char * tf_name=new char[50];
		strcpy(tf_name,pdes);
		l=k;
		k=strtok2(pdes,temp,l,',');
		trim(pdes,' ');
		char * ti_name=new char[50];
		strcpy(ti_name,pdes);
		bool TF_name_exist=false;
		bool TI_name_exist=false;
		for(int id=1;id<=(int)TF.size();id++){
			if(!strcmp(TF[id-1],tf_name)){//the tf_name already exists in TF
				delete [] tf_name;
				TF_name_exist=true;break;
			}
		}
		for(int id=1;id<=(int)TI.size();id++){
			if(!strcmp(TI[id-1],ti_name)){//the tf_name already exists in TF
				delete [] ti_name;
				TI_name_exist=true;break;
			}
		}
		if(!TF_name_exist){
			TF.push_back(tf_name);
		}
		if(!TI_name_exist){
			TI.push_back(ti_name);
		}
		f_tooling.getline(temp,1000,'\n');
	}
	ofstream f_TF("Debug/debug_tooling_family_name.txt");
	//cout<<"output the tooling family (TF) names"<<endl;
	for(int id=1;id<=(int)TF.size();id++){
		f_TF<<"id="<<id<<",name="<<TF[id-1]<<endl;
	}
	f_TF.close();
	f_TF.clear();
	ofstream f_TI("Debug/debug_tooling_name.txt");
	//cout<<"output the tooling instance (TI) names"<<endl;
	for(int id=1;id<=(int)TI.size();id++){
		f_TI<<"id="<<id<<",name="<<TI[id-1]<<endl;
	}
	f_TI.close();
	f_TI.clear();
	Num_Tooling_Family=(int)TF.size();
	f_tooling.close();
	f_tooling.clear();
	f_tooling.open("Data/tooling.csv");
	//------------------------------------------------------------
	f_tooling.getline(temp,1000,'\n');
	f_tooling.getline(temp,1000,'\n');
	while(!f_tooling.eof()){
		int l=0,k=0;
		for(int i=1;i<=50;i++){ pdes[i]=' ';}
		k=strtok2(pdes,temp,l,',');
		trim(pdes,' ');
		int tf=0;
		for(int id=1;id<=(int)TF.size();id++){
			if(!strcmp(TF[id-1],pdes)){ tf=id;break;}
		}
		if(tf>0){
			struct Tooling one_tool;
			one_tool.tf=tf;
			l=strtok2(pdes,temp,k,',');
			trim(pdes,' ');
			int ti=0;
			for(int id=1;id<=(int)TI.size();id++){
				if(!strcmp(TI[id-1],pdes)){ ti=id;break;}
			}
			if(ti<1){ cout<<"Wrong tooling instance id"<<endl; abort(); }
			one_tool.id=ti;
			k=strtok2(pdes,temp,l,',');//name
			trim(pdes,' ');
			strcpy(one_tool.name,TI[one_tool.id-1]);
			l=strtok2(pdes,temp,k,',');//certification
			trim(pdes,' ');
			int temperature=atoi(pdes);
			if(temperature>Num_Temperature){ Num_Temperature=temperature;}
			if(temperature>0){
				one_tool.temperature_vector.push_back(temperature);
			}
			Num_Tooling++;
			Tooling_vector.push_back(one_tool);
		}else{
			l=strtok2(pdes,temp,k,',');
			trim(pdes,' ');
			k=strtok2(pdes,temp,l,',');
			trim(pdes,' ');
			l=strtok2(pdes,temp,k,',');
			trim(pdes,' ');
			int temperature=atoi(pdes);
			if(temperature>Num_Temperature){ Num_Temperature=temperature;}
			if(temperature>0){
				vector<Tooling>::iterator p_T=Tooling_vector.end()-1;
				(*p_T).temperature_vector.push_back(temperature);
			}
		}
		f_tooling.getline(temp,1000,'\n');
	}
	f_tooling.close();
	show_Tooling_vector();
	//----------------------------------------------------------------------------------get LOT
	//read wip.csv
	for(int l=1;l<=Max_Num_Lot;l++){ w_l[l]=0.0;}
	ifstream f_wip("Data/wip.csv");
	f_wip.getline(temp,1000,'\n');
	f_wip.getline(temp,1000,'\n');
	cout<<"get LOT"<<endl;
	Num_Lot=0;
	Num_Device=0;
	while(!f_wip.eof()){
		int l=0,k=0;
		k=strtok2(pdes,temp,l,',');
		trim(pdes,' ');
		bool lot_exist=false;
		for(int id=1;id<=(int)LOT.size();id++){
			if(!strcmp(LOT[id-1],pdes)){ lot_exist=true;break;}
		}
		if(!lot_exist){//lot does not exist
			char * name=new char[50];
			strcpy(name,pdes);
			LOT.push_back(name);
		}
		f_wip.getline(temp,1000,'\n');
	}
	//for(int id=1;id<=(int)LOT.size();id++){
	//	cout<<"id="<<id<<",lot="<<LOT[id-1]<<endl;
	//}
	Num_Lot=(int)LOT.size();
	f_wip.close();
	f_wip.clear();
	f_wip.open("Data/wip.csv");
	//---------------------------------------------------------------------------------get DEV
	f_wip.getline(temp,1000,'\n');
	f_wip.getline(temp,1000,'\n');
	cout<<"get DEV"<<endl;
	while(!f_wip.eof()){
		int l=0,k=0;
		k=strtok2(pdes,temp,l,',');
		l=k;
		k=strtok2(pdes,temp,l,',');
		trim(pdes,' ');
		bool dev_exist=false;
		for(int id=1;id<=(int)DEV.size();id++){
			if(!strcmp(DEV[id-1],pdes)){ dev_exist=true;break;}
		}
		if(!dev_exist){//dev does not exist
			char * name=new char[50];
			strcpy(name,pdes);
			DEV.push_back(name);
		}
		f_wip.getline(temp,1000,'\n');
	}
	//for(int id=1;id<=(int)DEV.size();id++){
	//	cout<<"id="<<id<<",dev="<<DEV[id-1]<<endl;
	//}
	Num_Device=(int)DEV.size();
	f_wip.close();
	f_wip.clear();
	//----------------------------------------------------------------------------------get PKG
	f_wip.open("Data/wip.csv");
	f_wip.getline(temp,1000,'\n');
	f_wip.getline(temp,1000,'\n');
	cout<<"get PKG"<<endl;
	PKG.clear();
	while(!f_wip.eof()){
		int l=0,k=0;
		k=strtok2(pdes,temp,l,',');
		l=k;
		k=strtok2(pdes,temp,l,',');
		l=k;
		k=strtok2(pdes,temp,l,',');
		trim(pdes,' ');
		bool pkg_exist=false;
		for(int id=1;id<=(int)PKG.size();id++){
			if(!strcmp(PKG[id-1],pdes)){ pkg_exist=true;break;}
		}
		if(!pkg_exist){//pkg does not exist
			char * name=new char[50];
			strcpy(name,pdes);
			PKG.push_back(name);
		}
		f_wip.getline(temp,1000,'\n');
	}
	f_wip.close();
	f_wip.clear();	
	//----------------------------------------------------------------------------------get device_PKG,device_PIN
	for(int id=0;id<=Max_Num_Device-1;id++){
		device_PKG[id]=-1;
		device_PIN[id]=-1;
	}
	cout<<"get device_PKG[],device_PIN"<<endl;
	f_wip.open("Data/wip.csv");
	f_wip.getline(temp,1000,'\n');
	f_wip.getline(temp,1000,'\n');
	while(!f_wip.eof()){
		int l=0,k=0;
		k=strtok2(pdes,temp,l,',');
		l=k;
		k=strtok2(pdes,temp,l,',');
		trim(pdes,' ');
		int dev_id=-1;
		for(int id=1;id<=(int)DEV.size();id++){
			if(!strcmp(DEV[id-1],pdes)){ dev_id=id;break;}
		}
		if(dev_id<1){
			cout<<"Wrong device id, abort"<<endl; abort();
		}
		l=k;
		k=strtok2(pdes,temp,l,',');
		trim(pdes,' ');
		int pkg_id=-1;
		for(int id=1;id<=(int)PKG.size();id++){
			if(!strcmp(PKG[id-1],pdes)){ pkg_id=id; break;}
		}
		if(pkg_id<1){
			cout<<"Wrong package id, abort"<<endl; abort();
		}
		device_PKG[dev_id]=pkg_id;
		l=k;
		k=strtok2(pdes,temp,l,',');
		trim(pdes,' ');
		int pin=atoi(pdes);
		if(device_PIN[dev_id]>0 && device_PIN[dev_id]!=pin){
			cout<<"same device "<<DEV[dev_id-1]<<" has different number of pins. abort"<<endl; abort();
		}
		device_PIN[dev_id]=pin;
		f_wip.getline(temp,1000,'\n');
	}
	//----------------------------------------------------------------------------------get n_l_chips and lot weight
	f_wip.close();
	f_wip.clear();	
	f_wip.open("Data/wip.csv");
	f_wip.getline(temp,1000,'\n');
	f_wip.getline(temp,1000,'\n');
	cout<<"get n_l_chips[] and w_l[]"<<endl;
	while(!f_wip.eof()){
		int l=0,k=0;
		k=strtok2(pdes,temp,l,',');//get Lot name
		trim(pdes,' ');
		//-----------
		int lot=0;
		for(int id=1;id<=(int)LOT.size();id++){
			if(!strcmp(LOT[id-1],pdes)){ lot=id;break;}
		}
		if(lot<1){ cout<<"Wrong lot id"<<endl; abort();}
		l=strtok2(pdes,temp,k,',');//get device name
		trim(pdes,' ');
		//--------------
		int dev=0;
		for(int id=1;id<=(int)DEV.size();id++){
			if(!strcmp(DEV[id-1],pdes)){ dev=id;break;}
		}
		if(dev<1){ cout<<"Wrong dev id"<<endl; abort();}
		//---------------
		k=strtok2(pdes,temp,l,',');//skip package
		l=strtok2(pdes,temp,k,',');//skip pin
		k=strtok2(pdes,temp,l,',');//get qty
		trim(pdes,' ');
		n_l_chips[lot]=atoi(pdes);
		lots_device[lot]=dev;
		l=strtok2(pdes,temp,k,',');//get lot weight
		trim(pdes,' ');
		w_l[lot]=(double)atof(pdes);
		k=strtok2(pdes,temp,l,',');//get lot name
		trim(pdes,' ');
		struct ch_array one_ch_array;
		one_ch_array.id=lot;
		strcpy(one_ch_array.char_arry,pdes);
		lot_name.push_back(one_ch_array);
		f_wip.getline(temp,1000,'\n');
	}
	f_wip.close();
	f_wip.clear();
	//-----------------------------------------------------------------
	//read the keydevices.csv
	ifstream p_key("Data/keydevices.csv");
	p_key.getline(temp,1000,'\n');
	p_key.getline(temp,1000,'\n');
	Num_Keydevice=0;
	while(!p_key.eof()){
		int l=0,k=0;
		k=strtok2(pdes,temp,l,',');
		trim(pdes,' ');
		int dev=0;
		for(int id=1;id<=(int)DEV.size();id++){
			if(!strcmp(DEV[id-1],pdes)){ dev=id;break;}
		}
		if(dev<1){ 
			cout<<"Add device "<<pdes<<" to the set of devices"<<endl; 
			dev=Num_Device+1; 
			Num_Device++;
			char * name=new char[50];
			strcpy(name,pdes);
			DEV.push_back(name);
		}
		Key_Devices.push_back(dev);
		l=strtok2(pdes,temp,k,',');
		trim(pdes,' ');
		n_k_minchips[dev]=atoi(pdes);
		Num_Keydevice++;
		p_key.getline(temp,1000,'\n');
	}
	p_key.close();
	//----------------------------------------this is originally for package.csv
	//however, since the definition for package is changed, we need to update this part for key_package
	//read the key_package.csv
	ifstream p_package("Data/key_package.csv");
	p_package.getline(temp,1000,'\n');
	p_package.getline(temp,1000,'\n');
	Num_Key_PKG=0;
	while(!p_package.eof()){
		int l=0,k=0;
		k=strtok2(pdes,temp,l,',');
		trim(pdes,' ');
		int pkg=0;
		for(int id=1;id<=(int)PKG.size();id++){
			if(!strcmp(PKG[id-1],pdes)){ pkg=id;break;}
		}
		if(pkg<1){ 
			cout<<"Add package "<<pdes<<" to the set of PKG in reading key_package.csv"<<endl; 
			pkg=(int)PKG.size()+1; 
			char * name=new char[50];
			strcpy(name,pdes);
			PKG.push_back(name);
		}
		Key_PKG.push_back(pkg);
		l=strtok2(pdes,temp,k,',');
		trim(pdes,' ');
		n_kp_minchips[pkg]=atoi(pdes);
		//cout<<"pkg="<<pkg<<",target="<<n_kp_minchips[pkg]<<endl;
		Num_Key_PKG++;
		p_package.getline(temp,1000,'\n');
	}
	p_package.close();
	//-------------------------------------------------------------read key_pin_package.csv
	ifstream f_kpp("Data/key_pin_package.csv");
	f_kpp.getline(temp,1000,'\n');
	f_kpp.getline(temp,1000,'\n');
	while(!f_kpp.eof()){
		//package,pin,target
		int l=0,k=0;
		k=strtok2(pdes,temp,l,',');
		trim(pdes);
		int pkg=0,pin=0;
		for(int id=1;id<=(int)PKG.size();id++){
			if(!strcmp(PKG[id-1],pdes)){ pkg=id;break;}
		}
		if(pkg<1){ 
			cout<<"Add package "<<pdes<<" to the set of PKG in reading key_pin_package.csv"<<endl; 
			pkg=(int)PKG.size()+1; 
			char * name=new char[50];
			strcpy(name,pdes);
			PKG.push_back(name);
		}
		l=k;
		k=strtok2(pdes,temp,l,',');
		trim(pdes);
		pin=atoi(pdes);
		l=k;
		k=strtok2(pdes,temp,l,',');
		trim(pdes);
		int target=atoi(pdes);
		Key_PIN_PKG.push_back(make_pair(make_pair(pin,pkg),target));
		f_kpp.getline(temp,1000,'\n');
	}
	f_kpp.close();
	//-------------------------------------------------------------device_name
	for(int i=1;i<=Num_Device;i++){
		struct ch_array one_dev;
		strcpy(one_dev.char_arry,DEV[i-1]);
		one_dev.id=i;
		device_name.push_back(one_dev);
	}
	//-------------------------------------------------------------
	cout<<"Num_Lot="<<Num_Lot<<",Num_Device="<<Num_Device<<endl;
	for(int d=1;d<=Num_Device;d++){
		struct device_item one_dev;
		one_dev.device=d;
		for(int l=1;l<=Num_Lot;l++){
			if(lots_device[l]==one_dev.device){
				one_dev.device_lots.push_back(l);
			}
		}
		Devices.push_back(one_dev);
	}
	show_Devices();
	//----------------------------------------------------------
	//---------------------------------------------------------------
	//read the route.csv
	double min_weight=1E+8;
	for(int l=1;l<=Num_Lot;l++){
		if(w_l[l]<min_weight && w_l[l]>0){ min_weight=w_l[l];}
	}
	ifstream f_route("Data/route.csv");
	f_route.getline(temp,1000,'\n');
	f_route.getline(temp,1000,'\n');
	int count=0;
	while(!f_route.eof()){
		int l=0,k=0;
		for(int i=1;i<=50;i++){ pdes[i]=' ';}
		for(int n=1;n<=3;n++){
			k=strtok2(pdes,temp,l,',');
			l=strtok2(pdes,temp,k,',');
		}
		trim(pdes,' ');
		int dev=-1;
		for(int id=1;id<=(int)DEV.size();id++){
			if(!strcmp(DEV[id-1],pdes)){ dev=id;break;}
		}
		if(strlen(pdes)>=1){
			count++;
			k=strtok2(pdes,temp,l,',');//skip the package
			l=k;
			k=strtok2(pdes,temp,l,',');//skip the pin
			l=k;
			trim(pdes,' ');
			struct tooling_configure one_tc;
			one_tc.device=dev; //it is possible that dev=-1 since the device in route.csv is not in wip.csv
			one_tc.id=count;
			k=strtok2(pdes,temp,l,',');//get alternative
			trim(pdes,' ');
			if(strlen(pdes)>0){ eps[one_tc.id]=(min_weight-0.1)/2.0;}
			l=strtok2(pdes,temp,k,',');//skip setup description
			k=strtok2(pdes,temp,l,',');//get PPH
			trim(pdes,' ');
			rate[one_tc.id]=(double)atof(pdes);// NOTE:when clean S data the rate[] should be updated accordingly
			l=strtok2(pdes,temp,k,',');//get machine family
			trim(pdes,' ');
			int mg=-1;
			for(int id=1;id<=(int)MF.size();id++){
				if(!strcmp(MF[id-1],pdes)){ mg=id;break;}
			}
			one_tc.mg=mg;//it is possible that mg=-1
			k=strtok2(pdes,temp,l,',');//get tooling family
			trim(pdes,' ');
			if(strlen(pdes)>=1){//non-empty
				int tf=-1;
				for(int id=1;id<=(int)TF.size();id++){
					if(!strcmp(TF[id-1],pdes)){ tf=id;break;}
				}
				if(tf>0){// with tooling requirement
					eps[one_tc.id]+=(min_weight-0.1)/4.0;
				}
				one_tc.tf.tools.push_back(tf);
				l=strtok2(pdes,temp,k,',');//get the number of tooling piece
				trim(pdes,' ');
				a_st[one_tc.id][tf]=atoi(pdes);//!!NOTE:when clean S data a_st[][] need to be updated accordingly
				k=strtok2(pdes,temp,l,',');//get certification
				trim(pdes,' ');
				one_tc.temperature=atoi(pdes);
				S.push_back(one_tc);
			}else{//empty
				int tf=0;//no tooling requirement
				one_tc.tf.tools.push_back(tf);
				l=strtok2(pdes,temp,k,',');
				trim(pdes,' ');
				a_st[one_tc.id][tf]=atoi(pdes);//!!NOTE:when clean S data ast[][] need to be updated accordingly
				k=strtok2(pdes,temp,l,',');
				trim(pdes,' ');
				one_tc.temperature=atoi(pdes);
				S.push_back(one_tc);
			}
		}else{//need to append tooling families and pieces
			for(int m=1;m<=3;m++){
				k=strtok2(pdes,temp,l,',');
				l=strtok2(pdes,temp,k,',');
			}
			k=strtok2(pdes,temp,l,',');//get additional tooling family
			trim(pdes,' ');
			//now there should be non-empty tf, since this is an additional line for tooling info
			int tf=-1;
			for(int id=1;id<=(int)TF.size();id++){
				if(!strcmp(TF[id-1],pdes)){ tf=id;break;}
			}
			//it is possible that tf=-1;
			vector<tooling_configure>::iterator p_TC=S.end()-1;
			(*p_TC).tf.tools.push_back(tf);
			l=strtok2(pdes,temp,k,',');//get number of pieces
			trim(pdes,' ');
			int id=(*p_TC).id;
			a_st[id][tf]=atoi(pdes);//!!Note: when clean S data a_st[][] need to be updated
		}
		f_route.getline(temp,1000,'\n');
	}
	f_route.close();
	//still need to clean the S data,rate[] and a_st[][]
	//since it is possible that some device,machine family,tooling family are -1
	vector<int> route_to_remove;
	vector<pair<int,int> > old_new;
	for(int id=1;id<=(int)S.size();id++){
		old_new.push_back(make_pair(id,id));
	}
	for(int id=1;id<=(int)S.size();id++){
		bool remove_flag=false;
		if(S[id-1].device<1 || S[id-1].mg<1 || S[id-1].temperature<1){ remove_flag=true;}
		for(int id2=1;id2<=(int)S[id-1].tf.tools.size();id2++){
			int tf=S[id-1].tf.tools[id2-1];
			if(tf<1){ remove_flag=true;break;}
		}
		if(remove_flag){//need to remove the route
			route_to_remove.push_back(id);
		}
	}
	cout<<"route_to_remove"<<endl;
	for(int id=1;id<=(int)route_to_remove.size();id++){
		cout<<route_to_remove[id-1]<<endl;
	}

	for(int id=1;id<=(int)S.size();id++){//update old_new
		vii pfind=find(route_to_remove.begin(),route_to_remove.end(),id);
		if(pfind!=route_to_remove.end()){//route id is going to be removed
			old_new[id-1].second=-1;
			for(int id2=id+1;id2<=(int)old_new.size();id2++){
				old_new[id2-1].second=old_new[id2-1].second-1;
			}
		}
	}
	//update a_st[][] and rate[]
	//save a copy of a_st[][] and rate[]
	int a_st_save[Max_Num_Tooling_Configure][Max_Num_Tooling_Family];
	double rate_save[Max_Num_Tooling_Configure];
	for(int id=1;id<=Max_Num_Tooling_Configure;id++){ rate_save[id-1]=rate[id-1];}
	for(int id=1;id<=Max_Num_Tooling_Configure;id++){
		for(int id2=1;id2<=Max_Num_Tooling_Family;id2++){
			a_st_save[id-1][id2-1]=a_st[id-1][id2-1];
		}
	}
	for(int id=1;id<=(int)old_new.size();id++){
		int old_id=old_new[id-1].first;
		int new_id=old_new[id-1].second;
		if(old_id!=new_id){
			rate[new_id]=rate_save[old_id];
			rate[old_id]=0;
			for(int id2=0;id2<=Max_Num_Tooling_Family-1;id2++){
				a_st[new_id][id2]=a_st_save[old_id][id2];
				a_st[old_id][id2]=0;
			}
		}
	}
	//remove the routes
	vector<tooling_configure>::iterator p_S=S.begin();
	while(p_S!=S.end()){
		bool remove_flag=false;
		if((*p_S).device<1 || (*p_S).mg<1 || (*p_S).temperature<1){ remove_flag=true;}
		for(int id2=1;id2<=(int)(*p_S).tf.tools.size();id2++){
			int tf=(*p_S).tf.tools[id2-1];
			if(tf<1){ remove_flag=true; break;}
		}
		if(remove_flag){//need to remove the route
			p_S=S.erase(p_S);
		}
		if(!remove_flag){
			p_S++;
		}
	}
	for(int id=1;id<=(int)S.size();id++){
		S[id-1].id=id;
	}
	//--------
	show_S();
	//-----------------------------------------------------------------------------
	//set up machine_mg[] and Machine_Groups
	vector<Machine>::iterator p_M=Machine_vector.begin();
	while(p_M!=Machine_vector.end()){
		machine_mg[(*p_M).id]=(*p_M).mg;
		p_M++;
	}
	for(int n=1;n<=Num_Machine_Group;n++){
		struct machine_set m_set;
		for(int m=1;m<=Num_Machine;m++){
			if(machine_mg[m]==n){
				m_set.machines.push_back(m);
			}
		}
		Machine_Groups.push_back(m_set);
	}
	show_mg();
	//set up tooling_tf[] and Tooling_Families
	vector<Tooling>::iterator p_T=Tooling_vector.begin();
	while(p_T!=Tooling_vector.end()){
		tooling_tf[(*p_T).id]=(*p_T).tf;
		p_T++;
	}
	for(int tf=1;tf<=Num_Tooling_Family;tf++){
		struct tooling_set t_set;
		for(int t=1;t<=Num_Tooling;t++){
			if(tooling_tf[t]==tf){
				t_set.tools.push_back(t);
			}
		}
		Tooling_Families.push_back(t_set);
	}
	show_tf();
	//------------------------------------------------------------------------------create the temperature combination
	cout<<"initialize Temperature_Set"<<endl;
	temp_combination();
	show_temp_comb(Temperature_Set);
	//------------------------------------------------------------------------------get n_t_n_tooling[][]
	cout<<"get n_t_n_tooling[][]"<<endl;
	cout<<"Num_Tooling_Family="<<Num_Tooling_Family<<",Temperature_Set.size()="<<(int)Temperature_Set.size()<<endl;
	for(int tf=1;tf<=Num_Tooling_Family;tf++){
		for(int id=1;id<=(int)Temperature_Set.size();id++){
			n_t_n_tooling[tf][id]=0;
			for(int id2=1;id2<=(int)Tooling_vector.size();id2++){
				//cout<<"id2="<<id2<<",tf="<<Tooling_vector[id2-1].tf<<",int_vec=";
				//for(int id3=1;id3<=Tooling_vector[id2-1].temperature_vector.size();id3++){ cout<<Tooling_vector[id2-1].temperature_vector[id3-1]<<",";}
				//cout<<endl;
				if(Tooling_vector[id2-1].tf==tf && compare_int_vec(Tooling_vector[id2-1].temperature_vector,Temperature_Set[id-1].set_elements)){
					n_t_n_tooling[tf][id]++;
				}
			}
		}
	}
	//------------------------------------------------------------------------------
	//read the toolingfamily_temperature.csv for n_t_n_tooling[][]
	/*
	ifstream p_tf_temperature("Data/toolingfamily_temperature.csv");
	p_tf_temperature.getline(temp,1000,'\n');
	p_tf_temperature.getline(temp,1000,'\n');
	while(!p_tf_temperature.eof()){
		int k=0,l=0;
		k=strtok2(pdes,temp,l,',');//get the tooling family
		trim(pdes,' ');
		int tf=0;
		for(int id=1;id<=(int)TF.size();id++){
			if(!strcmp(TF[id-1],pdes)){ tf=id;break;}
		}
		if(tf<1){
			cout<<"tooling family "<<pdes<<" is not included in tooling.csv, discard it"<<endl;
			p_tf_temperature.getline(temp,1000,'\n');
			continue;
		}
		if(tf>0){
			l=strtok2(pdes,temp,k,',');//get temperature
			trim(pdes,' ');
			char pdes2[100];
			int k2=0,l2=0;
			struct t_set one_set;
			do{
				k2=strtok2(pdes2,pdes,l2,';');//get the list of temperatures
				trim(pdes2,' ');
				int element=atoi(pdes2);
				if(element>0){
					one_set.set_elements.push_back(element);
				}
				l2=k2;
			}while(pdes[l2-1]!='\0');
			int id=get_set_id(one_set);
			k=strtok2(pdes,temp,l,',');// get the number of tooling pieces
			trim(pdes,' ');
			n_t_n_tooling[tf][id]=atoi(pdes);// this is the purpose to read the file
			//cout<<"tf="<<tf<<",temperature id="<<id<<","<<atoi(pdes)<<endl;
		}
		p_tf_temperature.getline(temp,1000,'\n');
	}
	p_tf_temperature.close();
	*/
	ofstream f_n_t_n("Debug/debug_n_t_n_tooling.txt");
	for(int tf=1;tf<=Num_Tooling_Family;tf++){
		for(int id=1;id<=(int)Temperature_Set.size();id++){
			f_n_t_n<<"tf="<<tf<<",id="<<id<<",n_t_n_tooling[tf][id]="<<n_t_n_tooling[tf][id]<<endl;
		}
	}
	f_n_t_n.close();
	//----------------------------------------------------------
	//initialize H[]
	for(int i=1;i<=Num_Machine;i++){ H[i]=0.0; }//if a machine i is in machine.csv but not in machine_hours.csv, then set its machine time to zero
	//read the machine_hours.csv
	ifstream f_mach_hr("Data/machine_hours.csv");
	f_mach_hr.getline(temp,1000,'\n');
	f_mach_hr.getline(temp,1000,'\n');
	while(!f_mach_hr.eof()){
		int k=0,l=0;
		k=strtok2(pdes,temp,l,',');
		trim(pdes,' ');
		int mach=0;
		for(int id=1;id<=(int)MI.size();id++){
			if(!strcmp(MI[id-1],pdes)){ mach=id;break;}
		}
		if(mach<1){ 
			cout<<"machine instance "<<pdes<<" is not included in machines.csv, discard it"<<endl; 
			f_mach_hr.getline(temp,1000,'\n');
			continue;
		}
		l=strtok2(pdes,temp,k,',');
		trim(pdes,' ');
		H[mach]=(double)atof(pdes);
		f_mach_hr.getline(temp,1000,'\n');
	}
	f_mach_hr.close();
	//--------------------------------------------------------
	//for(int id=1;id<=(int)Key_PKG.size();id++){
	//	int pkg_id=Key_PKG[id-1];
	//	cout<<"pkg_id="<<pkg_id<<",target="<<n_kp_minchips[pkg_id]<<endl;
	//}
	show_targets();
	show_statistics();
	//--------------------------------------------------------
}

void AT::get_S(vector<tooling_configure> & TS){
	//get the whole tooling_set
}

void AT::initialize_S_i(){
	for(int i=1;i<=Num_Machine;i++){
		int mg=machine_mg[i];
		vector<Machine>::iterator p_M=Machine_vector.begin()+i-1;
		vector<int>::iterator p_int=(*p_M).temperature_vector.begin();
		vector<tooling_configure>::iterator p_S=S.begin();
		while(p_S!=S.end()){
			p_int=find((*p_M).temperature_vector.begin(),(*p_M).temperature_vector.end(),(*p_S).temperature);
			if((*p_S).mg==mg && p_int!=(*p_M).temperature_vector.end()){
				S_i[i].tc_vector.push_back(*p_S);
			}
			p_S++;
		}
	}
}

void AT::show_S_i(){
	ofstream fout("Debug/debug_S_i.txt");
	fout<<"show S_i ------------------------------------"<<endl;
	for(int i=1;i<=Num_Machine;i++){
		fout<<"i="<<i<<",";
		vector<tooling_configure>::iterator p_S=S_i[i].tc_vector.begin();
		while(p_S!=S_i[i].tc_vector.end()){
			fout<<"id="<<(*p_S).id<<",device="<<(*p_S).device<<",mg="<<(*p_S).mg<<",temperature="<<(*p_S).temperature<<",tf:";
			vii p_int=(*p_S).tf.tools.begin();
			while(p_int!=(*p_S).tf.tools.end()){
				fout<<(*p_int)<<",";
				p_int++;
			}
			p_S++;
		}
		fout<<endl;
	}
	fout.close();
}

void AT::get_S(vector<tooling_configure> & TS,int i){
	//get the tooling set that can be installed to machine i
	
	int mg=machine_mg[i];
	vector<Machine>::iterator p_M=Machine_vector.begin()+i-1;
	vector<int>::iterator p_int=(*p_M).temperature_vector.begin();
	vector<tooling_configure>::iterator p_S=S.begin();
	while(p_S!=S.end()){
		p_int=find((*p_M).temperature_vector.begin(),(*p_M).temperature_vector.end(),(*p_S).temperature);
		if((*p_S).mg==mg && p_int!=(*p_M).temperature_vector.end()){
			TS.push_back(*p_S);
		}
		p_S++;
	}
	
	/*
	TS.clear();
	vector<tooling_configure>::iterator p_S=S_i[i].tc_vector.begin();
	while(p_S!=S_i[i].tc_vector.end()){
		TS.push_back(*p_S);
		p_S++;
	}
	*/
}

void AT::get_S_l(vector<tooling_configure> & TS,int l){
	//get the routes that can process lot l;
	int dev=lots_device[l];
	vector<tooling_configure>::iterator p_S=S.begin();
	while(p_S!=S.end()){
		if((*p_S).device==dev){
			TS.push_back((*p_S));
		}
		p_S++;
	}
}

void AT::get_S(vector<tooling_configure> & TS,int i,int l){
	//get the tooling set that can be installed to machine i to process lot l
	get_S(TS,i);
	int dev=lots_device[l];
	vector<tooling_configure>::iterator p_TS=TS.begin();
	while(p_TS!=TS.end()){
		if((*p_TS).device!=dev){
			p_TS=TS.erase(p_TS);
		}
		else{p_TS++;}
	}
}

void AT::get_S(vector<tooling_configure> & TS,int i,int l,int tf){
	//get the tooling set that can be installed to machine i for lot l and including tooling family tf
	get_S(TS,i,l);
	vector<tooling_configure>::iterator p_TS=TS.begin();
	while(p_TS!=TS.end()){
		vector<int>::iterator p_int=find((*p_TS).tf.tools.begin(),(*p_TS).tf.tools.end(),tf);
		if(p_int==(*p_TS).tf.tools.end()){
			p_TS=TS.erase(p_TS);
		}
		else{p_TS++;}
	}
}

void AT::get_S(vector<tooling_configure> & TS,int i,int l,int tf,int tau){
	get_S(TS,i,l,tf);
	vector<tooling_configure>::iterator p_TS=TS.begin();
	while(p_TS!=TS.end()){
		int tau_TS=(*p_TS).temperature;
		if(tau_TS!=tau){
			p_TS=TS.erase(p_TS);
		}
		else{ p_TS++;}
	}
}

void AT::get_S_i_l_Lamuda(vector<tooling_configure> &TS,int i,int l,int Lamuda_id){
	get_S(TS,i,l);
	vector<tooling_configure>::iterator p_TS=TS.begin();
	while(p_TS!=TS.end()){
		vector<int> a;
		int s=(*p_TS).id;
		vii pt=(*p_TS).tf.tools.begin();
		while(pt!=(*p_TS).tf.tools.end()){
			int tf=(*pt);
			//if(i==28 && l==13){ cout<<"s="<<s<<",tf="<<tf<<",a_st="<<a_st[s][tf]<<endl;}
			a.push_back(a_st[s][tf]);
			pt++;
		}
		/*
		if((i==28 && l==13)){
			vii p=(*p_TS).tf.tools.begin();
			cout<<"tools:";
			while(p!=(*p_TS).tf.tools.end()){
				cout<<(*p)<<",";
				p++;
			}
			cout<<endl;
			cout<<"quantity:";
			p=a.begin();
			while(p!=a.end()){
				cout<<(*p)<<",";
				p++;
			}
			cout<<endl;
			cout<<"temperature:"<<(*p_TS).temperature<<endl;
		}
		*/
		int La_id=get_Lamuda_id((*p_TS).tf.tools,a,(*p_TS).temperature);
		if(La_id!=Lamuda_id){
			p_TS=TS.erase(p_TS);
			//if(i==28 && l==13)
			//	cout<<"!!!,la_id="<<La_id<<",Lamuda_id="<<Lamuda_id<<endl;
		}else{
			p_TS++;
		}
	}
}

void AT::get_Lamuda(){
	//a Lamuda_item is a combination of toolings,number of pieces,and temperature
	vector<tooling_configure>::iterator p_S=S.begin();
	int count=1;
	while(p_S!=S.end()){
		vector<int> a;
		int s=(*p_S).id;
		vii pt=(*p_S).tf.tools.begin();
		while(pt!=(*p_S).tf.tools.end()){
			int tf=(*pt);
			a.push_back(a_st[s][tf]);
			pt++;
		}
		vector<Lamuda_item>::iterator p_Lamuda=Lamuda.begin();
		while(p_Lamuda!=Lamuda.end()){
			if(compare_int_vec((*p_S).tf.tools,(*p_Lamuda).tooling_vec) 
				&& compare_int_vec(a,(*p_Lamuda).b) 
				&& (*p_S).temperature==(*p_Lamuda).temperature){ break;}
			p_Lamuda++;
		}
		if(p_Lamuda==Lamuda.end()){
			struct Lamuda_item one_Lamuda;
			one_Lamuda.id=count;
			count++;
			one_Lamuda.tooling_vec.resize((int)(*p_S).tf.tools.size());
			copy((*p_S).tf.tools.begin(),(*p_S).tf.tools.end(),one_Lamuda.tooling_vec.begin());
			one_Lamuda.b.resize((int)a.size());
			copy(a.begin(),a.end(),one_Lamuda.b.begin());
			one_Lamuda.temperature=(*p_S).temperature;
			Lamuda.push_back(one_Lamuda);
		}
		p_S++;
	}
}

void AT::show_Lamuda(){
	ofstream fout("Debug/debug_Lamuda.txt");
	fout<<"show Lamuda vector -------------------------------------"<<endl;
	vector<Lamuda_item>::iterator p_Lamuda=Lamuda.begin();
	while(p_Lamuda!=Lamuda.end()){
		fout<<"id="<<(*p_Lamuda).id<<",tooling=";
		vii p_int=(*p_Lamuda).tooling_vec.begin();
		while(p_int!=(*p_Lamuda).tooling_vec.end()){
			int offset=p_int-(*p_Lamuda).tooling_vec.begin();
			vii pb=(*p_Lamuda).b.begin()+offset;
			fout<<(*p_int)<<"("<<(*pb)<<"),";
			p_int++;
		}
		fout<<endl;
		fout<<"temperature="<<(*p_Lamuda).temperature<<endl;
		p_Lamuda++;
	}
	fout.close();
}

int AT::get_Lamuda_id(vector<int> & tf,vector<int> & b,int & temperature){
	//for(int i=1;i<=(int)tf.size();i++){ cout<<"i="<<i<<",tf="<<tf[i-1]<<endl;}
	//for(int i=1;i<=(int)b.size();i++){ cout<<"i="<<i<<",b="<<b[i-1]<<endl;}
	//cout<<"temperature="<<temperature<<endl;

	vector<Lamuda_item>::iterator p_Lamuda=Lamuda.begin();
	while(p_Lamuda!=Lamuda.end()){
		if(compare_int_vec(tf,(*p_Lamuda).tooling_vec) && compare_int_vec(b,(*p_Lamuda).b) && temperature==(*p_Lamuda).temperature){
			return (*p_Lamuda).id;
		}
		p_Lamuda++;
	}
	//could not find the tooling setup in Lamuda
	cout<<"could not find the tooling setup in Lamuda"<<endl;
	return 0;
}

void AT::get_Lamuda(vector<int> & L,int i,int tf,int tau){
	//get Lamuda(i,tf,tau)
	L.resize((int)Lamuda_i[i].int_vector.size());
	copy(Lamuda_i[i].int_vector.begin(),Lamuda_i[i].int_vector.end(),L.begin());
	vii p_int=L.begin();
	while(p_int!=L.end()){
		vector<Lamuda_item>::iterator p_Lamuda=Lamuda.begin()+(*p_int)-1;
		vii p_find=find((*p_Lamuda).tooling_vec.begin(),(*p_Lamuda).tooling_vec.end(),tf);
		if(p_find==(*p_Lamuda).tooling_vec.end() || tau!=(*p_Lamuda).temperature){
			L.erase(p_int);
		}else{
			p_int++;
		}
	}
}

void AT::initialize_Lamuda_i(){
	for(int i=1;i<=Num_Machine;i++){
		vector<tooling_configure> TS;
		get_S(TS,i);
		vector<tooling_configure>::iterator p_TS=TS.begin();
		while(p_TS!=TS.end()){
			//----------------------------
			vector<int> a;
			int s=(*p_TS).id;
			vii pt=(*p_TS).tf.tools.begin();
			while(pt!=(*p_TS).tf.tools.end()){
				int tf=(*pt);
				a.push_back(a_st[s][tf]);
				pt++;
			}
			//----------------------------
			int id=get_Lamuda_id((*p_TS).tf.tools,a,(*p_TS).temperature);
			vii p_find=find(Lamuda_i[i].int_vector.begin(),Lamuda_i[i].int_vector.end(),id);
			if(p_find==Lamuda_i[i].int_vector.end()){
				Lamuda_i[i].int_vector.push_back(id);
			}
			p_TS++;
		}
		vector<int> v((int)Lamuda_i[i].int_vector.size(),0);
		vii p_int=Lamuda_i[i].int_vector.begin();
		copy(p_int,p_int+(int)Lamuda_i[i].int_vector.size(),v.begin());
		sort(v.begin(),v.end());
		swap(v,Lamuda_i[i].int_vector);
	}
}

void AT::initialize_M_lamuda(){
	//build M_lamuda vector, should be called after initialize_Lamuda_i
	vector<Lamuda_item>::iterator p_Lamuda=Lamuda.begin();
	while(p_Lamuda!=Lamuda.end()){
		int id=(*p_Lamuda).id;
		for(int i=1;i<=Num_Machine;i++){
			vector<int>::iterator p_find=find(Lamuda_i[i].int_vector.begin(),Lamuda_i[i].int_vector.end(),id);
			if(p_find!=Lamuda_i[i].int_vector.end()){
				M_lamuda[id].int_vector.push_back(i);
			}
		}
		p_Lamuda++;
	}
}

void AT::show_Lamuda_i(){
	ofstream fout("Debug/debug_Lamuda_i.txt");
	fout<<"show Lamuda_i ---------------------------------"<<endl;
	for(int i=1;i<=Num_Machine;i++){
		fout<<"i="<<i<<":";
		vii p_int=Lamuda_i[i].int_vector.begin();
		while(p_int!=Lamuda_i[i].int_vector.end()){
			fout<<(*p_int)<<",";
			p_int++;
		}
		fout<<endl;
	}
	fout.close();
}

void AT::get_M(vector<machine_set> & MS){
	//get the whole machine set
}

void AT::get_machine_mg(){
	//initialize the array machine_mg[]
	for(int i=1;i<=Num_Machine;i++){
		int i_mg=-1;
		vector<machine_set>::iterator pmg=Machine_Groups.begin();
		while(pmg!=Machine_Groups.end()){
			vector<int>::iterator pm=(*pmg).machines.begin();
			while(pm!=(*pmg).machines.end()){
				if((*pm)==i){//find the machine
					i_mg=pmg-Machine_Groups.begin()+1;
					break;
				}
				pm++;
			}
			if(i_mg>0){ break;}
			pmg++;
		}
		machine_mg[i]=i_mg;
	}
}

void AT::get_tooling_tf(){
	//initialize the array tooling_tf[]
	for(int t=1;t<=Num_Tooling;t++){
		int t_tf=-1;
		vector<tooling_set>::iterator ptf=Tooling_Families.begin();
		while(ptf!=Tooling_Families.end()){
			vector<int>::iterator pt=find((*ptf).tools.begin(),(*ptf).tools.end(),t);
			if(pt!=(*ptf).tools.end()){
				t_tf=ptf-Tooling_Families.begin()+1;
				break;
			}
			ptf++;
		}
		tooling_tf[t]=t_tf;
	}
}

void AT::initialize_M_l(){
	for(int l=1;l<=Num_Lot;l++){
		vector<tooling_configure>::iterator p_S=S.begin();
		while(p_S!=S.end()){
			int dev=(*p_S).device;
			vector<device_item>::iterator p_d=Devices.begin()+dev-1;
			vector<int>::iterator p_l=find((*p_d).device_lots.begin(),(*p_d).device_lots.end(),l);
			if(p_l!=(*p_d).device_lots.end()){
				int mg=(*p_S).mg;
				int temp=(*p_S).temperature;
				vector<machine_set>::iterator p_M=Machine_Groups.begin()+mg-1;
				vector<int>::iterator p_int=(*p_M).machines.begin();
				while(p_int!=(*p_M).machines.end()){
					vector<Machine>::iterator p_Machine=Machine_vector.begin()+(*p_int)-1;
					vector<int>::iterator p_temp=find((*p_Machine).temperature_vector.begin(),(*p_Machine).temperature_vector.end(),temp);
					if(p_temp!=(*p_Machine).temperature_vector.end()){
						int i=(*p_Machine).id;
						vector<int>::iterator p1=find(M_l[l].int_vector.begin(),M_l[l].int_vector.end(),i);
						if(p1==M_l[l].int_vector.end()){ M_l[l].int_vector.push_back(i);}
					}
					p_int++;
				}
			}
			p_S++;
		}
	}
}

void AT::show_M_l(){
	ofstream fout("Debug/debug_M_l.txt");
	fout<<"show M_l ---------------------------------"<<endl;
	for(int l=1;l<=Num_Lot;l++){
		fout<<"l="<<l<<":";
		vii p_int=M_l[l].int_vector.begin();
		while(p_int!=M_l[l].int_vector.end()){
			fout<<(*p_int)<<",";
			p_int++;
		}
		fout<<endl;
	}
	fout.close();
}

void AT::get_M(vector<int> & MS,int l){
	//get the machines that can process lot l
	vector<tooling_configure>::iterator p_S=S.begin();
	while(p_S!=S.end()){
		int dev=(*p_S).device;
		vector<device_item>::iterator p_d=Devices.begin()+dev-1;
		vector<int>::iterator p_l=find((*p_d).device_lots.begin(),(*p_d).device_lots.end(),l);
		if(p_l!=(*p_d).device_lots.end()){
			int mg=(*p_S).mg;
			int temp=(*p_S).temperature;
			vector<machine_set>::iterator p_M=Machine_Groups.begin()+mg-1;
			vector<int>::iterator p_int=(*p_M).machines.begin();
			while(p_int!=(*p_M).machines.end()){
				vector<Machine>::iterator p_Machine=Machine_vector.begin()+(*p_int)-1;
				vector<int>::iterator p_temp=find((*p_Machine).temperature_vector.begin(),(*p_Machine).temperature_vector.end(),temp);
				if(p_temp!=(*p_Machine).temperature_vector.end()){
					int i=(*p_Machine).id;
					vector<int>::iterator p1=find(MS.begin(),MS.end(),i);
					if(p1==MS.end()){ MS.push_back(i);}
				}
				p_int++;
			}
		}
		p_S++;
	}
}

void AT::get_L(vector<int> & LS){
	//get the whole lot set
}

void AT::initialize_L_i(){
	vector<Machine>::iterator p_M=Machine_vector.begin();
	while(p_M!=Machine_vector.end()){
		int i=p_M-Machine_vector.begin()+1;
		vector<tooling_configure>::iterator p_S=S.begin();
		while(p_S!=S.end()){
			int temp=(*p_S).temperature;
			vector<int>::iterator p_int=find((*p_M).temperature_vector.begin(),(*p_M).temperature_vector.end(),temp);
			if((*p_S).mg==machine_mg[i] && p_int!=(*p_M).temperature_vector.end()){
				int device_num=(*p_S).device;
				vector<device_item>::iterator p_d=Devices.begin()+device_num-1;
				vector<int>::iterator p_l=(*p_d).device_lots.begin();
				while(p_l!=(*p_d).device_lots.end()){
					vii p_int=find(L_i[i].int_vector.begin(),L_i[i].int_vector.end(),(*p_l));
					if(p_int==L_i[i].int_vector.end()){
						L_i[i].int_vector.push_back((*p_l));
					}
					p_l++;
				}
			}
			p_S++;
		}
		p_M++;
	}
}

void AT::show_L_i(){
	ofstream fout("Debug/debug_L_i.txt");
	fout<<"show L_i ------------------------------------"<<endl;
	for(int i=1;i<=Num_Machine;i++){
		fout<<"i="<<i<<" processes lots:";
		vii p_int=L_i[i].int_vector.begin();
		while(p_int!=L_i[i].int_vector.end()){
			fout<<(*p_int)<<",";
			p_int++;
		}
		fout<<endl;
	}
	fout.close();
}

void AT::initialize_L_i_d(){
	for(int i=1;i<=Num_Machine;i++){
		for(int d=1;d<=Num_Device;d++){
			vector<device_item>::iterator p_D=Devices.begin()+d-1;
			vii p_lot=(*p_D).device_lots.begin();
			while(p_lot!=(*p_D).device_lots.end()){
				vii p_int=find(L_i[i].int_vector.begin(),L_i[i].int_vector.end(),(*p_lot));
				if(p_int!=L_i[i].int_vector.end()){
					L_i_d[i][d].int_vector.push_back(*p_lot);
				}
				p_lot++;
			}
		}
	}
}

void AT::show_L_i_d(){
	ofstream fout("Debug/debug_L_i_d.txt");
	fout<<"show L_i_d ----------------------------------------"<<endl;
	for(int i=1;i<=Num_Machine;i++){
		for(int d=1;d<=Num_Device;d++){
			fout<<"i="<<i<<",d="<<d<<":";
			vii p_int=L_i_d[i][d].int_vector.begin();
			while(p_int!=L_i_d[i][d].int_vector.end()){
				fout<<(*p_int)<<",";
				p_int++;
			}
			fout<<endl;
		}
	}
	fout.close();
}

void AT::get_L(vector<int> & LS,int i){
	//get the lots that can be processed by machine i
	//cout<<"i="<<i<<",i_mg="<<machine_mg[i]<<endl;
	vector<Machine>::iterator p_M=Machine_vector.begin()+i-1;
	vector<tooling_configure>::iterator p_S=S.begin();
	while(p_S!=S.end()){
		int temp=(*p_S).temperature;
		vector<int>::iterator p_int=find((*p_M).temperature_vector.begin(),(*p_M).temperature_vector.end(),temp);
		if((*p_S).mg==machine_mg[i] && p_int!=(*p_M).temperature_vector.end()){
			int device_num=(*p_S).device;
			vector<device_item>::iterator p_d=Devices.begin()+device_num-1;
			vector<int>::iterator p_l=(*p_d).device_lots.begin();
			while(p_l!=(*p_d).device_lots.end()){
				vii p_int=find(LS.begin(),LS.end(),(*p_l));
				if(p_int==LS.end()){
					LS.push_back((*p_l));
				}
				p_l++;
			}
		}
		p_S++;
	}
}

void AT::get_L(vector<int> & LS,int i,int device){
	//get the lots that can be processed by machine i and contains the device
	get_L(LS,i);
	vector<int>::iterator p_LS=LS.begin();
	while(p_LS!=LS.end()){
		int lot=(*p_LS);
		int dev=lots_device[lot];
		if(dev!=device){
			p_LS=LS.erase(p_LS);
		}else{
			p_LS++;
		}
	}
}

void AT::get_L_i_lamuda(vector<int> & LS,int i,int lamuda_id){
	LS.clear();
	vii p_l=L_i[i].int_vector.begin();
	while(p_l!=L_i[i].int_vector.end()){
		int l=(*p_l);
		//cout<<"in get_L_i_lamuda,i="<<i<<",l="<<l<<",lamuda_id="<<lamuda_id<<endl;
		vector<tooling_configure> TS;
		get_S_i_l_Lamuda(TS,i,l,lamuda_id);
		//cout<<"size of S(i,l,lamuda)="<<TS.size()<<endl;
		if((int)TS.size()>0){
			LS.push_back(l);
		}
		p_l++;
	}
}

void AT::get_K(vector<int> & KS){
	//get the whole set of key devices
}

void AT::show_mg(){
	ofstream fout("Debug/debug_mg.txt");
	fout<<"show machine groups ---------------------"<<endl;
	vector<machine_set>::iterator pmg=Machine_Groups.begin();
	while(pmg!=Machine_Groups.end()){
		vector<int>::iterator pm=(*pmg).machines.begin();
		while(pm!=(*pmg).machines.end()){
			fout<<(*pm)<<",";
			pm++;
		}
		fout<<endl;
		pmg++;
	}
	fout.close();
}

void AT::show_tf(){
	ofstream fout("Debug/debug_tf.txt");
	fout<<"show tooling families ------------------"<<endl;
	vector<tooling_set>::iterator ptf=Tooling_Families.begin();
	while(ptf!=Tooling_Families.end()){
		vector<int>::iterator pt=(*ptf).tools.begin();
		while(pt!=(*ptf).tools.end()){
			fout<<(*pt)<<",";
			pt++;
		}
		fout<<endl;
		ptf++;
	}
	fout.close();
}

void AT::show_S(){
	ofstream fout("Debug/debug_S.txt");
	fout<<"show tooling configurations (S)-------------"<<endl;
	vector<tooling_configure>::iterator p_S=S.begin();
	while(p_S!=S.end()){
		fout<<"Tooling configuration ID = "<<(*p_S).id<<endl;
		fout<<"Device ="<<(*p_S).device<<endl;
		fout<<"Machine Group = "<<(*p_S).mg<<endl;
		fout<<"Temperature   = "<<(*p_S).temperature<<endl;
		fout<<"Tool Family =";
		vector<int>::iterator pt=(*p_S).tf.tools.begin();
		while(pt!=(*p_S).tf.tools.end()){
			fout<<(*pt)<<",";
			pt++;
		}
		fout<<endl;
		p_S++;
	}
	fout.close();
}

void AT::show_Devices(){
	ofstream fout("Debug/debug_Devices.txt");
	fout<<"show devices ----------------------------------"<<endl;
	vector<device_item>::iterator p_d=Devices.begin();
	while(p_d!=Devices.end()){
		fout<<"Device "<<(*p_d).device<<" consists Lots ";
		vector<int>::iterator p_lot=(*p_d).device_lots.begin();
		while(p_lot!=(*p_d).device_lots.end()){
			fout<<(*p_lot)<<",";
			p_lot++;
		}
		fout<<endl;
		p_d++;
	}
	fout.close();
}

void AT::show_index_vector(){
	ofstream fout("Debug/debug_index_vector.txt");
	fout<<"show index_vector -------------------------------"<<endl;
	vector<index_node>::iterator p_ind=index_vector.begin();
	while(p_ind!=index_vector.end()){
		fout<<"index="<<(*p_ind).index<<",i="<<(*p_ind).i<<",l="<<(*p_ind).l<<",s="<<(*p_ind).s<<",Lamuda_id="<<(*p_ind).Lamuda_id<<endl;
		p_ind++;
	}
	fout.close();
}

void AT::show_Machine_vector(){
	ofstream fout("Debug/debug_Machine_vector.txt");
	vector<Machine>::iterator p_M=Machine_vector.begin();
	while(p_M!=Machine_vector.end()){
		fout<<"ID="<<(*p_M).id<<endl;
		fout<<"Name="<<(*p_M).name<<endl;
		fout<<"Machine Group="<<(*p_M).mg<<endl;
		fout<<"Temperature: ";
		vector<int>::iterator p_temp=(*p_M).temperature_vector.begin();
		while(p_temp!=(*p_M).temperature_vector.end()){
			fout<<(*p_temp)<<",";
			p_temp++;
		}
		fout<<endl;
		p_M++;
	}
	fout.close();
}

void AT::show_Tooling_vector(){
	ofstream fout("Debug/debug_Tooling_vector.txt");
	vector<Tooling>::iterator p_T=Tooling_vector.begin();
	while(p_T!=Tooling_vector.end()){
		fout<<"ID="<<(*p_T).id<<endl;
		fout<<"Name="<<(*p_T).name<<endl;
		fout<<"Tooling Family="<<(*p_T).tf<<endl;
		fout<<"Temperature: ";
		vector<int>::iterator p_temp=(*p_T).temperature_vector.begin();
		while(p_temp!=(*p_T).temperature_vector.end()){
			fout<<(*p_temp)<<",";
			p_temp++;
		}
		fout<<endl;
		p_T++;
	}
	fout.close();
}

void AT::show_temp_comb(vector<t_set> & temp_set){
	ofstream fout("Debug/debug_temp_comb.txt");
	fout<<"Show temperature combinations -----------------"<<endl;
	vector<t_set>::iterator p_set=temp_set.begin();
	while(p_set!=temp_set.end()){
		vector<int>::iterator p_int=(*p_set).set_elements.begin();
		while(p_int!=(*p_set).set_elements.end()){
			fout<<(*p_int)<<",";
			p_int++;
		}
		fout<<endl;
		p_set++;
	}
	fout.close();
}

void AT::show_statistics(){
	cout<<"show the problem statistics --------------"<<endl;
	cout<<"Num_Lot = "<<Num_Lot<<endl;
	cout<<"Num_Device = "<<Num_Device<<endl;
	cout<<"Num_Keydevice = "<<Num_Keydevice<<endl;
	cout<<"Num_Package = "<<Num_Package<<endl;
	cout<<"Num_Temperature = "<<Num_Temperature<<endl;
	cout<<"Num_Machine = "<<Num_Machine<<endl;
	cout<<"Num_Machine_Group = "<<Num_Machine_Group<<endl;
	cout<<"Num_Tooling = "<<Num_Tooling<<endl;
	cout<<"Num_Tooling_Family = "<<Num_Tooling_Family<<endl;
	cout<<"Number of routes = "<<(int)S.size()<<endl;
}

void AT::show_device_name(){
	ofstream fout("Debug/debug_device_name.txt");
	fout<<"show the device_name ------------------------"<<endl;
	vector<ch_array>::iterator p_ch=device_name.begin();
	while(p_ch!=device_name.end()){
		fout<<"id="<<(*p_ch).id<<",name="<<(*p_ch).char_arry<<endl;
		p_ch++;
	}
	fout.close();
}

void AT::show_lot_name(){
	ofstream fout("Debug/debug_lot_name.txt");
	fout<<"show the lot_name --------------------------"<<endl;
	vector<ch_array>::iterator p_ch=lot_name.begin();
	while(p_ch!=lot_name.end()){
		fout<<"id="<<(*p_ch).id<<",name="<<(*p_ch).char_arry<<endl;
		p_ch++;
	}
	fout.close();
}

void AT::test(){
	Num_Machine=5;
	Num_Tooling=10;
	Num_Lot=2;
	Num_Device=4;
	Num_Keydevice=1;
	Num_Package=1;
	Num_Tooling_Family=2;

	for(int i=1;i<=Num_Machine;i++){ H[i]=8;}
	for(int t=1;t<=Num_Tooling;t++){ n_t_tooling[t]=2;}
	for(int l=1;l<=Num_Lot;l++)    { n_l_chips[l]=6000;}
	n_k_minchips[2]=1000;
	n_p_minchips[1]=1000;
	for(int l=1;l<=Num_Lot;l++)    { w_l[l]=1;}

	struct machine_set m_set1;
	m_set1.machines.push_back(1);
	m_set1.machines.push_back(2);
	Machine_Groups.push_back(m_set1);
	struct machine_set m_set2;
	m_set2.machines.push_back(3);
	m_set2.machines.push_back(4);
	m_set2.machines.push_back(5);
	Machine_Groups.push_back(m_set2);
	show_mg();
	get_machine_mg();

	struct tooling_set t_set1;
	t_set1.tools.push_back(1);
	t_set1.tools.push_back(2);
	t_set1.tools.push_back(3);
	t_set1.tools.push_back(4);
	t_set1.tools.push_back(5);
	Tooling_Families.push_back(t_set1);
	struct tooling_set t_set2;
	t_set2.tools.push_back(6);
	t_set2.tools.push_back(7);
	t_set2.tools.push_back(8);
	t_set2.tools.push_back(9);
	t_set2.tools.push_back(10);
	Tooling_Families.push_back(t_set2);
	show_tf();
	get_tooling_tf();

	struct tooling_configure t_con1;
	t_con1.id=1;
	t_con1.mg=1;
	t_con1.device=1;
	t_con1.tf.tools.push_back(1);
	t_con1.tf.tools.push_back(2);
	S.push_back(t_con1);
	struct tooling_configure t_con2;
	t_con2.id=2;
	t_con2.mg=2;
	t_con2.device=1;
	t_con2.tf.tools.push_back(1);
	t_con2.tf.tools.push_back(2);
	S.push_back(t_con2);
	struct tooling_configure t_con3;
	t_con3.id=3;
	t_con3.mg=1;
	t_con3.device=2;
	t_con3.tf.tools.push_back(1);
	t_con3.tf.tools.push_back(2);
	S.push_back(t_con3);
	show_S();

	Packages.push_back(1);
	Key_Devices.push_back(2);

	struct device_item dev1;
	dev1.device=1;
	dev1.device_lots.push_back(1);
	Devices.push_back(dev1);
	struct device_item dev2;
	dev2.device=2;
	dev2.device_lots.push_back(2);
	Devices.push_back(dev2);
	struct device_item dev3;
	dev3.device=3;
	dev3.device_lots.push_back(3);
	dev3.device_lots.push_back(4);
	Devices.push_back(dev3);
	show_Devices();

	lots_device[1]=1;
	lots_device[2]=2;
	lots_device[3]=3;
	lots_device[4]=3;

	//-----------------
	vector<int> L;
	get_L(L,Num_Machine);
	int last_lot=(*(L.end()-1));
	vector<tooling_configure> TS;
	get_S(TS,Num_Machine,last_lot);
	int last_configure=(*(TS.end()-1)).id;
	x_limit=index_x(Num_Machine,last_lot,last_configure);
	cout<<"x_limit="<<x_limit<<endl;
	cout<<index_x(1,1,1)<<endl;
	y_limit=index_y(Num_Machine,last_lot);
	cout<<"y_limit"<<y_limit<<endl;
	cout<<index_y(4,1)<<","<<index_y(5,1)<<endl;

	//build_model();
}

int AT::index_x(int i,int l,int s){
	int pos=-1;
	for(int i_id=1;i_id<=i-1;i_id++){
		vector<int>::iterator p_L=L_i[i_id].int_vector.begin();
		while(p_L!=L_i[i_id].int_vector.end()){
			int l_id=(*p_L);
			vector<tooling_configure> TS;
			get_S(TS,i_id,l_id);
			pos+=TS.size();
			p_L++;
		}
	}

	vector<int>::iterator p_L=L_i[i].int_vector.begin();
	while(p_L!=L_i[i].int_vector.end() && (*p_L)!=l){
		int l_id=(*p_L);
		vector<tooling_configure> TS;
		get_S(TS,i,l_id);
		pos+=TS.size();
		p_L++;
	}
	vector<tooling_configure> TS;
	get_S(TS,i,l);
	vector<tooling_configure>::iterator p_TS=TS.begin();
	while(p_TS!=TS.end()){
		if((*p_TS).id!=s){
			pos++;
		}else{
			pos++; break;
		}
		p_TS++;
	}
	return pos;
}

int AT::index4_x(int i,int l){
	int index=-1;
	vector<index_node>::iterator p_node=index_vector_no_s.begin();
	while(p_node!=index_vector_no_s.end()){
		if((*p_node).i==i&&(*p_node).l==l){
			index=(*p_node).index;
			break;
		}
		p_node++;
	}
	if(index<0){ cout<<"WRONG x index!!!, i="<<i<<",l="<<l<<endl; }
	return index;
}

int AT::index4_y(int i,int Lamuda_id){
	int index=-1;
	vector<index_node>::iterator p_node=index_vector_no_s.begin();
	while(p_node!=index_vector_no_s.end()){
		if((*p_node).i==i && (*p_node).Lamuda_id==Lamuda_id){
			index=(*p_node).index;
			break;
		}
		p_node++;
	}
	if(index<0){ cout<<"WRONG y index!!!"<<endl;}
	return index;
}

int AT::index3_x(int i,int l,int s){
	int index=-1;
	vector<index_node>::iterator p_node=first_level_index_vector.begin();
	while(p_node!=first_level_index_vector.end()){
		if((*p_node).i==i&&(*p_node).l==l&&(*p_node).s==s){
			index=(*p_node).index;
			break;
		}
		p_node++;
	}
	if(index<0){ 
		cout<<"WRONG x index!!!, i="<<i<<",l="<<l<<",s="<<s<<endl; 
		abort();
	}
	return index;
}

int AT::index3_y(int i,int Lamuda_id){
	int index=-1;
	vector<index_node>::iterator p_node=first_level_index_vector.begin();
	while(p_node!=first_level_index_vector.end()){
		if((*p_node).i==i && (*p_node).Lamuda_id==Lamuda_id){
			index=(*p_node).index;
			break;
		}
		p_node++;
	}
	if(index<0){ cout<<"WRONG y index!!!"<<endl; abort();}
	return index;
}

int AT::index2_x(int i,int l,int s){
	int index=-1;
	vector<index_node>::iterator p_node=index_vector.begin();
	while(p_node!=index_vector.end()){
		if((*p_node).i==i&&(*p_node).l==l&&(*p_node).s==s){
			index=(*p_node).index;
			break;
		}
		p_node++;
	}
	if(index<0){ cout<<"WRONG x index!!!, i="<<i<<",l="<<l<<",s="<<s<<endl; }
	return index;
}

int AT::index2_y(int i,int Lamuda_id){
	int index=-1;
	vector<index_node>::iterator p_node=index_vector.begin();
	while(p_node!=index_vector.end()){
		if((*p_node).i==i && (*p_node).Lamuda_id==Lamuda_id){
			index=(*p_node).index;
			break;
		}
		p_node++;
	}
	if(index<0){ cout<<"WRONG y index!!!"<<endl;}
	return index;
}

int AT::index_y(int i,int l){
	//this function should not be used
	int pos=x_limit;
	for(int i_id=1;i_id<=i-1;i_id++){
		vector<int> L;
		get_L(L,i_id);
		pos+=L.size();
	}
	vector<int> L;
	get_L(L,i);
	vector<int>::iterator p_L=L.begin();
	while(p_L!=L.end()){
		if((*p_L)!=l){ pos++;}
		else{ pos++; break;}
		p_L++;
	}
	return pos;
}

void AT::build_model(){
#if 0
	try{
	IloEnv env;
	IloModel model(env);
	IloRangeArray con(env);
	vector<IloNumVar> deltaKP_vector;
	vector<IloNumVar> deltaKPP_vector;
	vector<IloNumVar> deltaKD_vector;
	vector<IloNumVar> x_vector;
	vector<IloNumVar> y_vector;
	vector<IloNumVar> t_vector;
	IloNumVar t_max,t_min;
	int num_con=0;
	int num_var=0;
	
	//initialize x variables
	for(int i=1;i<=Num_Machine;i++){
		vii p_L=L_i[i].int_vector.begin();
		while(p_L!=L_i[i].int_vector.end()){
			int l=(*p_L);
			int sim=machine_sim[i];
			int d=lots_device[l];
			int index=index_S_sim_d(sim,d);
			vii p_s=S_sim_d[index].second.int_vector.begin();
			while(p_s!=S_sim_d[index].second.int_vector.end()){
				int s=(*p_s);
				string str="x";
				stringstream ss;
				ss<<"("<<i<<")("<<l<<")("<<s<<")";
				str+=ss.str();
				char name[20];
				strcpy(name,str.c_str());
				x_vector.push_back(IloNumVar(env,0.0,1.0,name));
				num_var++;
				p_s++;
			}
			p_L++;
		}
	}
	//initialize y variables
	for(int i=1;i<=Num_Machine;i++){
		vii p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			string str="y";
			stringstream ss;
			ss<<"("<<i<<")("<<Lamuda_id<<")";
			str+=ss.str();
			char name[20];
			strcpy(name,str.c_str());
			y_vector.push_back(IloNumVar(env,0.0,1.0,name));
			num_var++;
			p_Lamuda++;
		}
	}

	//initalize delta variables
	for(int k=1;k<=Num_Keydevice;k++){
		string str="deltaKD";
		stringstream ss;
		ss<<"("<<k<<")";
		str+=ss.str();
		char name[20];
		strcpy(name,str.c_str());
		deltaKD_vector.push_back(IloNumVar(env,0,IloInfinity,name));
		num_var++;
	}
	for(int k=1;k<=(int)Key_PKG.size();k++){
		string str="deltaKP";
		stringstream ss;
		ss<<"("<<k<<")";
		str+=ss.str();
		char name[20];
		strcpy(name,str.c_str());
		deltaKP_vector.push_back(IloNumVar(env,0,IloInfinity,name));
		num_var++;
	}
	for(int k=1;k<=(int)Key_PIN_PKG.size();k++){
		string str="deltaKPP";
		stringstream ss;
		ss<<"("<<k<<")";
		str+=ss.str();
		char name[20];
		strcpy(name,str.c_str());
		deltaKPP_vector.push_back(IloNumVar(env,0,IloInfinity,name));
		num_var++;
	}
	//initialize t variables
	for(int i=1;i<=Num_Machine;i++){
		vii p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			string str="t";
			stringstream ss;
			ss<<"("<<i<<")("<<Lamuda_id<<")";
			str+=ss.str();
			char name[20];
			strcpy(name,str.c_str());
			t_vector.push_back(IloNumVar(env,0,IloInfinity,name));
			num_var++;
			p_Lamuda++;
		}
	}
	string str="t_max";
	char name[20];
	strcpy(name,str.c_str());
	t_max=IloNumVar(env,0,IloInfinity,name);
	//str="t_min";
	//strcpy(name,str.c_str());
	//t_min=IloNumVar(env,0,IloInfinity,name);
	//str="";
	//-------------------------------------------------------------------
	double eps_M=1E+6,eps_W=0.0;
	for(int l=1;l<=Num_Lot;l++){
		if(w_l[l]>0 && w_l[l]<eps_M){ eps_M=w_l[l];}
	}
	double temp=0;
	for(int i=1;i<=Num_Machine;i++){
		if(H[i]>temp){ temp=H[i];}
	}
	eps_W=eps_M/temp;
	//-------------------------------------------------------------------
	cout<<"build obj"<<endl;
	IloExpr expr(env);
	for(int i=1;i<=Num_Machine;i++){
		vector<int>::iterator p_L=L_i[i].int_vector.begin();
		while(p_L!=L_i[i].int_vector.end()){
			int l=(*p_L);
			int sim=machine_sim[i];
			int d=lots_device[l];
			int index=index_S_sim_d(sim,d);
			vii p_s=S_sim_d[index].second.int_vector.begin();
			while(p_s!=S_sim_d[index].second.int_vector.end()){
				int s=(*p_s);
				vector<IloNumVar>::iterator p_x=x_vector.begin()+index2_x(i,l,s);
				expr+=(w_l[l]-eps[s])*(* p_x);
				p_s++;
			}
			p_L++;
		}
	}
	for(int k=1;k<=Num_Keydevice;k++){
		int d=Key_Devices[k-1];
		vector<IloNumVar>::iterator p_deltaKD=deltaKD_vector.begin()+k-1;
		expr-=penalty[d]*(* p_deltaKD);
	}
	for(int id=1;id<=(int)Key_PKG.size();id++){
		int pkg_id=Key_PKG[id-1];
		vector<IloNumVar>::iterator p_deltaKP=deltaKP_vector.begin()+id-1;
		expr-=PKG_penalty[pkg_id]*(* p_deltaKP);
	}
	for(int id=1;id<=(int)Key_PIN_PKG.size();id++){
		int pin_num=Key_PIN_PKG[id-1].first.first;
		int pkg_id=Key_PIN_PKG[id-1].first.second;
		vector<IloNumVar>::iterator p_deltaKPP=deltaKPP_vector.begin()+id-1;
		expr-=get_PIN_PKG_penalty(pkg_id,pin_num)*(* p_deltaKPP);
	}
	for(int i=1;i<=Num_Machine;i++){
		vii p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			vector<IloNumVar>::iterator p_y=y_vector.begin()+index2_y(i,Lamuda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1; //use the index of y
			expr-=eps_M*(*p_y);
			p_Lamuda++;
		}
	}
	//expr-=eps_W*t_max;
	//expr+=eps_W*t_min;
	IloObjective obj(env,expr,IloObjective::Maximize);
	model.add(obj);
	expr.clear();
	//-------------------------------------------------------------------
	cout<<"build constraints (1b)"<<endl;
	for(int l=1;l<=Num_Lot;l++){
		IloExpr expr(env);
		vector<int>::iterator p_M=M_l[l].int_vector.begin();
		while(p_M!=M_l[l].int_vector.end()){
			int i=(*p_M);
			int sim=machine_sim[i];
			int d=lots_device[l];
			int index=index_S_sim_d(sim,d);
			vii p_s=S_sim_d[index].second.int_vector.begin();
			while(p_s!=S_sim_d[index].second.int_vector.end()){
				int s=(*p_s);
				vector<IloNumVar>::iterator p_x= x_vector.begin()+index2_x(i,l,s);
				expr+=(*p_x);
				p_s++;
			}
			p_M++;
		}
		con.add(expr<=1.0);
		expr.end();
		num_con++;
		string str="(";
		stringstream ss;
		ss<<"l@"<<l<<")";
		char name[100];
		str+=ss.str();
		strcpy(name,str.c_str());
		con[num_con-1].setName(name);
		//cout<<"l="<<l<<endl;
	}

	if(new_old_model==2){
		cout<<"build constraints (1c)"<<endl;
		for(int i=1;i<=Num_Machine;i++){
			vector<int>::iterator p_Lamuda=Lamuda_i[i].int_vector.begin();
			while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
				int Lamuda_id=(*p_Lamuda);
				//int index=index_L_sim_lamuda(machine_sim[i],Lamuda_id);
				//vector<int>::iterator p_L=L_sim_lamuda[index].second.int_vector.begin();
				//while(p_L!=L_sim_lamuda[index].second.int_vector.end()){
				vector<int>::iterator p_L=L_i[i].int_vector.begin();
				while(p_L!=L_i[i].int_vector.end()){
					int l=(*p_L);
					vector<tooling_configure> TS;
					get_S_i_l_Lamuda(TS,i,l,Lamuda_id);
					vector<tooling_configure>::iterator p_TS=TS.begin();
					while(p_TS!=TS.end()){
						IloExpr expr(env);
						int s=(*p_TS).id;
						vector<IloNumVar>::iterator p_x=x_vector.begin()+index2_x(i,l,s);
						vector<IloNumVar>::iterator p_y=y_vector.begin()+index2_y(i,Lamuda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;
						expr=(*p_y)-(*p_x);
						con.add(expr>=0);
						expr.end();
						num_con++;
						string str="(";
						stringstream ss;
						ss<<"i@"<<i<<",Lamuda@"<<Lamuda_id<<",l@"<<l<<",s@"<<s<<")";
						str+=ss.str();
						char name[100];
						strcpy(name,str.c_str());
						con[num_con-1].setName(name);
						p_TS++;
					}
					p_L++;
				}
				p_Lamuda++;
			}
		}
	}

	/*
	for(int i=1;i<=Num_Machine;i++){
		vector<int>::iterator p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			int counter=0;
			IloExpr expr(env);
			vector<IloNumVar>::iterator p_y=y_vector.begin()+index2_y(i,Lamuda_id)-x_limit-Num_Keydevice-Num_Package-1;
			vector<int>::iterator p_L=L_i[i].int_vector.begin();
			while(p_L!=L_i[i].int_vector.end()){
				int l=(*p_L);
				vector<tooling_configure> TS;
				get_S_i_l_Lamuda(TS,i,l,Lamuda_id);
				vector<tooling_configure>::iterator p_TS=TS.begin();
				while(p_TS!=TS.end()){
					int s=(*p_TS).id;
					vector<IloNumVar>::iterator p_x=x_vector.begin()+index2_x(i,l,s);
					expr-=(*p_x);
					counter++;
					p_TS++;
				}
				p_L++;
			}
			expr+=(*p_y)*counter;
			con.add(expr>=0);
			expr.end();
			num_con++;
			string str="(";
			stringstream ss;
			ss<<"i@"<<i<<",Lamuda@"<<Lamuda_id<<")";
			str+=ss.str();
			char name[100];
			strcpy(name,str.c_str());
			con[num_con-1].setName(name);
			p_Lamuda++;
		}
	}
	*/

	cout<<"build constraints (1d)"<<endl;
	for(int i=1;i<=Num_Machine;i++){
		IloExpr expr(env);
		vector<int>::iterator p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(* p_Lamuda);
			vector<IloNumVar>::iterator p_y=y_vector.begin()+index2_y(i,Lamuda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;
			expr+=(*p_y);
			p_Lamuda++;
		}
		con.add(expr<=1);
		expr.end();
		num_con++;
		string str="(";
		stringstream ss;
		ss<<"(y)i@"<<i<<")";
		str+=ss.str();
		char name[100];
		strcpy(name,str.c_str());
		con[num_con-1].setName(name);
	}
	/*
	cout<<"build constraints (1e)"<<endl;
	for(int tf=1;tf<=Num_Tooling_Family;tf++){
		for(int n=1;n<=(int)Temperature_Set.size();n++){
			vector<t_set>::iterator p_set=Temperature_Set.begin()+n-1;
			IloExpr expr(env);
			for(int i=1;i<=Num_Machine;i++){
				//cout<<"i="<<i<<", L_i[i] size="<<L_i[i].int_vector.size()<<endl;
				vector<int>::iterator p_L=L_i[i].int_vector.begin();
				while(p_L!=L_i[i].int_vector.end()){
					int l=(*p_L);		
					vii p_int=(*p_set).set_elements.begin();
					while(p_int!=(*p_set).set_elements.end()){
						int tau=(*p_int);
						vector<tooling_configure> TS;
						get_S(TS,i,l,tf,tau);
						vector<tooling_configure>::iterator p_TS=TS.begin();
						while(p_TS!=TS.end()){
							int s=(*p_TS).id;
							//vector<int>::iterator p_int=find((*p_TS).tf.tools.begin(),(*p_TS).tf.tools.end(),tf);
							if(a_st[s][tf]>=1){
								vector<IloNumVar>::iterator p_x=x_vector.begin()+index2_x(i,l,s);
								expr+=a_st[s][tf]*(*p_x);
							}
							p_TS++;
						}
						p_int++;
					}
					p_L++;
				}
			}
			vector<t_set> inter_set;
			get_inter_set((*p_set),inter_set);
			vector<t_set>::iterator pm=inter_set.begin();
			int sum=0;
			while(pm!=inter_set.end()){
				int set_id=get_set_id(*pm);
				sum+=n_t_n_tooling[tf][set_id];
				pm++;
			}
			con.add(expr<=sum);
			num_con++;
			expr.end();
			string str="(";
			stringstream ss;
			ss<<"tf@"<<tf<<",n@"<<n<<")";
			str+=ss.str();
			char name[100];
			strcpy(name,str.c_str());
			con[num_con-1].setName(name);
			//cout<<"tf="<<tf<<",n="<<n<<endl;
		}
	}*/
	cout<<"build constraints (1e)"<<endl;
	for(int tf=1;tf<=Num_Tooling_Family;tf++){
		for(int n=1;n<=(int)Temperature_Set.size();n++){
			vector<t_set>::iterator p_set=Temperature_Set.begin()+n-1;
			IloExpr expr(env);
			for(int i=1;i<=Num_Machine;i++){
				vii p_int=(*p_set).set_elements.begin();
				while(p_int!=(*p_set).set_elements.end()){
					int tau=(*p_int);
					vector<int> Lamuda_i_tf_tau;
					get_Lamuda(Lamuda_i_tf_tau,i,tf,tau);
					vii p_Lamuda=Lamuda_i_tf_tau.begin();
					while(p_Lamuda!=Lamuda_i_tf_tau.end()){
						int Lamuda_id=(*p_Lamuda);
						vector<Lamuda_item>::iterator pL=Lamuda.begin()+Lamuda_id-1;
						vii p_find=find((*pL).tooling_vec.begin(),(*pL).tooling_vec.end(),tf);
						int offset=p_find-(*pL).tooling_vec.begin();
						vii pb=(*pL).b.begin()+offset;
						vector<IloNumVar>::iterator p_y=y_vector.begin()+index2_y(i,Lamuda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;
						expr+=(*pb)*(*p_y);
						p_Lamuda++;
					}//lamuda_iteration
					p_int++;
				}//tau_iteration
			}//i_iteration
			vector<t_set> inter_set;
			get_inter_set((*p_set),inter_set);
			vector<t_set>::iterator pm=inter_set.begin();
			int sum=0;
			while(pm!=inter_set.end()){
				int set_id=get_set_id(*pm);
				sum+=n_t_n_tooling[tf][set_id];
				pm++;
			}
			con.add(expr<=sum);
			num_con++;
			expr.end();
			string str="(";
			stringstream ss;
			ss<<"tf@"<<tf<<",n@"<<n<<")";
			str+=ss.str();
			char name[100];
			strcpy(name,str.c_str());
			con[num_con-1].setName(name);
			//cout<<"tf="<<tf<<",n="<<n<<endl;
		}
	}

	if(new_old_model==2){
	cout<<"build constraints (1f)"<<endl;
	for(int i=1;i<=Num_Machine;i++){
		IloExpr expr(env);
		vector<int>::iterator p_L=L_i[i].int_vector.begin();
		while(p_L!=L_i[i].int_vector.end()){
			int l=(*p_L);
			int sim=machine_sim[i];
			int d=lots_device[l];
			int index=index_S_sim_d(sim,d);
			vii p_s=S_sim_d[index].second.int_vector.begin();
			while(p_s!=S_sim_d[index].second.int_vector.end()){
				int s=(*p_s);
				vector<IloNumVar>::iterator p_x=x_vector.begin()+index2_x(i,l,s);
				expr+=n_l_chips[l]/rate[s]*(*p_x);
				p_s++;
			}
			p_L++;
		}
		con.add(expr<=H[i]);
		num_con++;
		expr.end();
		string str="(";
		stringstream ss;
		ss<<"i@"<<i<<")";
		str+=ss.str();
		char name[100];
		strcpy(name,str.c_str());
		con[num_con-1].setName(name);
		//cout<<"i="<<i<<endl;
	}
	}else{

	cout<<"build updated constraints (1f)"<<endl;
	for(int i=1;i<=Num_Machine;i++){
		vector<int>::iterator p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			IloExpr expr(env);
			int index=index_L_sim_lamuda(machine_sim[i],Lamuda_id);
			vector<int>::iterator p_L=L_sim_lamuda[index].second.int_vector.begin();
			while(p_L!=L_sim_lamuda[index].second.int_vector.end()){
				int l=(*p_L);
				vector<tooling_configure> TS;
				get_S_i_l_Lamuda(TS,i,l,Lamuda_id);
				vector<tooling_configure>::iterator p_TS=TS.begin();
				while(p_TS!=TS.end()){
					int s=(*p_TS).id;
					vector<IloNumVar>::iterator p_x=x_vector.begin()+index2_x(i,l,s);
					expr+=n_l_chips[l]/rate[s]*(*p_x);
					p_TS++;
				}
				p_L++;
			}
			vector<IloNumVar>::iterator p_t=t_vector.begin()+index2_y(i,Lamuda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;
			expr-=(*p_t);
			con.add(expr==0);
			num_con++;
			expr.end();
			string str="(";
			stringstream ss;
			ss<<"i@"<<i<<",Lamuda@"<<Lamuda_id<<")";
			str+=ss.str();
			char name[100];
			strcpy(name,str.c_str());
			con[num_con-1].setName(name);
			//cout<<"i="<<i<<endl;
			p_Lamuda++;
		}
	}

	for(int i=1;i<=Num_Machine;i++){
		vector<int>::iterator p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			IloExpr expr(env);	
			vector<IloNumVar>::iterator p_t=t_vector.begin()+index2_y(i,Lamuda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;
			vector<IloNumVar>::iterator p_y=y_vector.begin()+index2_y(i,Lamuda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;
			expr+=(*p_t)-H[i]*(*p_y);
			con.add(expr<=0);
			num_con++;
			expr.end();
			string str="t_y(";
			stringstream ss;
			ss<<"i@"<<i<<",Lamuda@"<<Lamuda_id<<")";
			str+=ss.str();
			char name[100];
			strcpy(name,str.c_str());
			con[num_con-1].setName(name);
			//cout<<"i="<<i<<endl;
			p_Lamuda++;
		}
	}
	for(int i=1;i<=Num_Machine;i++){
		vector<int>::iterator p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			IloExpr expr(env);	
			vector<IloNumVar>::iterator p_t=t_vector.begin()+index2_y(i,Lamuda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;
			expr+=t_max-(*p_t);
			con.add(expr>=0);
			num_con++;
			expr.end();
			string str="t_max_con(";
			stringstream ss;
			ss<<"i@"<<i<<",Lamuda@"<<Lamuda_id<<")";
			str+=ss.str();
			char name[100];
			strcpy(name,str.c_str());
			con[num_con-1].setName(name);
			//cout<<"i="<<i<<endl;
			p_Lamuda++;
		}
	}
	/*
	for(int i=1;i<=Num_Machine;i++){
		vector<int>::iterator p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			IloExpr expr(env);	
			vector<IloNumVar>::iterator p_t=t_vector.begin()+index2_y(i,Lamuda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;
			vector<IloNumVar>::iterator p_y=y_vector.begin()+index2_y(i,Lamuda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;
			expr+=t_min-(*p_t)-H[i]*(1-(*p_y));
			con.add(expr<=0);
			num_con++;
			expr.end();
			string str="t_min_con(";
			stringstream ss;
			ss<<"i@"<<i<<",Lamuda@"<<Lamuda_id<<")";
			str+=ss.str();
			char name[100];
			strcpy(name,str.c_str());
			con[num_con-1].setName(name);
			//cout<<"i="<<i<<endl;
			p_Lamuda++;
		}
	}
	*/
	}//end_if

	cout<<"build constraints (1g)"<<endl;
	for(int k=1;k<=Num_Keydevice;k++){
		vector<int>::iterator p_int=Key_Devices.begin()+k-1;
		int d=(*p_int);
		IloExpr expr(env);
		for(int i=1;i<=Num_Machine;i++){
			vector<int>::iterator p_L=L_i_d[i][d].int_vector.begin();
			while(p_L!=L_i_d[i][d].int_vector.end()){
				int l=(*p_L);
				int sim=machine_sim[i];
				int index=index_S_sim_d(sim,d);
				vii p_s=S_sim_d[index].second.int_vector.begin();
				while(p_s!=S_sim_d[index].second.int_vector.end()){
					int s=(*p_s);
					vector<IloNumVar>::iterator p_x=x_vector.begin()+index2_x(i,l,s);
					expr+=n_l_chips[l]*(*p_x);
					p_s++;
				}
				p_L++;
			}
		}
		vector<IloNumVar>::iterator p_KD=deltaKD_vector.begin()+k-1;
		expr+=coef*(*p_KD);
		con.add(expr>=n_k_minchips[d]);
		num_con++;
		expr.end();
		string str="(";
		stringstream ss;
		ss<<"k@"<<k<<",dev@"<<d<<")";
		str+=ss.str();
		char name[100];
		strcpy(name,str.c_str());
		con[num_con-1].setName(name);
		//cout<<"k="<<k<<endl;
	}

	cout<<"build constraints (1h)"<<endl;
	for(int id=1;id<=(int)Key_PKG.size();id++){
		int pkg_id=Key_PKG[id-1];
		vector<int> dev;
		for(int d=1;d<=Num_Device;d++){
			if(device_PKG[d]==pkg_id){ dev.push_back(d);}
		}
		IloExpr expr(env);
		for(int i=1;i<=Num_Machine;i++){
			for(int id2=1;id2<=(int)dev.size();id2++){
				int d=dev[id2-1];
				vector<int>::iterator p_L=L_i_d[i][d].int_vector.begin();
				while(p_L!=L_i_d[i][d].int_vector.end()){
					int l=(*p_L);
					int sim=machine_sim[i];
					int index=index_S_sim_d(sim,d);
					vii p_s=S_sim_d[index].second.int_vector.begin();
					while(p_s!=S_sim_d[index].second.int_vector.end()){
						int s=(*p_s);
						vector<IloNumVar>::iterator p_x=x_vector.begin()+index2_x(i,l,s);
						expr+=n_l_chips[l]*(*p_x);
						p_s++;
					}
					p_L++;
				}
			}
		}
		expr+=coef*deltaKP_vector[id-1];
		con.add(expr>=n_kp_minchips[pkg_id]);
		num_con++;
		expr.end();
		string str="(";
		stringstream ss;
		ss<<"id@"<<id<<",kp@"<<pkg_id<<")";
		str+=ss.str();
		char name[100];
		strcpy(name,str.c_str());
		con[num_con-1].setName(name);
	}

	cout<<"build constraints (1i)"<<endl;
	for(int id=1;id<=(int)Key_PIN_PKG.size();id++){
		int pin=Key_PIN_PKG[id-1].first.first;
		int pkg_id=Key_PIN_PKG[id-1].first.second;
		int target=Key_PIN_PKG[id-1].second;
		vector<int> dev;
		for(int d=1;d<=Num_Device;d++){
			if(device_PKG[d]==pkg_id && device_PIN[d]==pin){ dev.push_back(d);}
		}
		IloExpr expr(env);
		for(int i=1;i<=Num_Machine;i++){
			for(int id2=1;id2<=(int)dev.size();id2++){
				int d=dev[id2-1];
				vector<int>::iterator p_L=L_i_d[i][d].int_vector.begin();
				while(p_L!=L_i_d[i][d].int_vector.end()){
					int l=(*p_L);
					int sim=machine_sim[i];
					int index=index_S_sim_d(sim,d);
					vii p_s=S_sim_d[index].second.int_vector.begin();
					while(p_s!=S_sim_d[index].second.int_vector.end()){
						int s=(*p_s);
						vector<IloNumVar>::iterator p_x=x_vector.begin()+index2_x(i,l,s);
						expr+=n_l_chips[l]*(*p_x);
						p_s++;
					}
					p_L++;
				}
			}
		}
		expr+=coef*deltaKPP_vector[id-1];
		con.add(expr>=target);
		num_con++;
		expr.end();
		string str="(";
		stringstream ss;
		ss<<"id@"<<id<<",pkg@"<<pkg_id<<",pin@"<<pin<<")";
		str+=ss.str();
		char name[100];
		strcpy(name,str.c_str());
		con[num_con-1].setName(name);
	}
/*
	cout<<"build constriants (1i)"<<endl;
	for(int it=1;it<=(int)Lamuda.size();it++){
		vector<Lamuda_item>::iterator p_Lamuda=Lamuda.begin()+it-1;
		//-----------------
		int sum_b=0;
		vector<int>::iterator p_int=(*p_Lamuda).b.begin();
		while(p_int!=(*p_Lamuda).b.end()){
			sum_b+=(*p_int);
			p_int++;
		}
		//---------------------------
		if(sum_b>0){//when the tooling list is not empty
			int Lamuda_id=(*p_Lamuda).id;
			//-------------------------------------temperature sest
			int temperature=(*p_Lamuda).temperature;
			struct t_set one_set;
			one_set.set_elements.push_back(temperature);
			vector<t_set> inter_set;
			get_inter_set(one_set,inter_set);
			//---------------------------------------------------------------
			IloExpr expr(env);
			vector<int>::iterator p_M=M_lamuda[Lamuda_id].int_vector.begin();
			while(p_M!=M_lamuda[Lamuda_id].int_vector.end()){
				int i=(*p_M);
				//cout<<"i="<<i<<",Lamuda="<<Lamuda_id<<","<<index2_y(i,Lamuda_id)<<endl;
				vector<IloNumVar>::iterator p_y=y_vector.begin()+index2_y(i,Lamuda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;
				expr+=(*p_y);
				p_M++;
			}
			double min_num=1E+8;
			vector<int>::iterator p_tool=(*p_Lamuda).tooling_vec.begin();
			while(p_tool!=(*p_Lamuda).tooling_vec.end()){
				int offset=p_tool-(*p_Lamuda).tooling_vec.begin();
				vector<int>::iterator p_b=(*p_Lamuda).b.begin()+offset;
				int tf=(*p_tool);
				int require_num=(*p_b);
				int available_num=0;
				vector<t_set>::iterator pm=inter_set.begin();
				while(pm!=inter_set.end()){
					int set_id=get_set_id(*pm);
					available_num+=n_t_n_tooling[tf][set_id];
					pm++;
				}
				double ratio=available_num/(require_num*1.0);
				if(ratio<min_num){
					min_num=ratio;
				}
				p_tool++;
			}
			con.add(expr<=(int)min_num);
			num_con++;
			expr.end();
			string str="(";
			stringstream ss;
			ss<<"Lamuda@"<<Lamuda_id<<")";
			str+=ss.str();
			char name[100];
			strcpy(name,str.c_str());
			con[num_con-1].setName(name);
		}
	}
*/
	model.add(con);
	
	for(int i=x_limit+Num_Keydevice+Num_Package+(int)Key_PKG.size()+(int)Key_PIN_PKG.size()+1;i<=y_limit;i++){
		vector<IloNumVar>::iterator p_y=y_vector.begin()+i-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;
		model.add(IloConversion(env,(*p_y),ILOBOOL));
	}
	for(int i=0;i<=x_limit;i++){
		vector<IloNumVar>::iterator p_x=x_vector.begin()+i;
		model.add(IloConversion(env,(*p_x),ILOBOOL));
	}
	
	IloCplex cplex(model);
	cplex.setParam(IloCplex::RootAlg,1); //use primal method
	cplex.setParam(IloCplex::TiLim,3600);
	cplex.setParam(IloCplex::EpGap,0.01);
	cplex.exportModel("AT_CPLEX.lp");
	if(!cplex.solve()){
		cout<<"!!The model can not be solved by CPLEX!!"<<endl;
	}
	cplex.out() <<"Status="<<cplex.getStatus()<<","<<cplex.getObjValue()<<endl;
	cplex.out() <<"Maximum bound violation = "<<cplex.getQuality(IloCplex::MaxPrimalInfeas) << endl;	
	cplex.out() << "Iteration count = " << cplex.getNiterations() << endl;
	cplex.out() << "Num of nodes processed = "<<cplex.getNnodes()<<endl;
	cplex.out() << "Phase I Iter Num= " << cplex.getNphaseOneIterations() <<endl;
	cplex.printTime();
	//---------------------------------------------get the selected lots
	vector<int> lot_selected;
	for(int id=1;id<=(int)x_vector.size();id++){
		double value=cplex.getValue(x_vector[id-1]);
		if(value>0.99){
			vector<index_node>::iterator p_ind=index_vector.begin()+id-1;
			int l=(*p_ind).l;
			lot_selected.push_back(l);
		}
	}
	//---------------------------------------------get the y_value
	vector<double> y_value;
	for(int i=1;i<=Num_Machine;i++){
		vector<int>::iterator p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			vector<IloNumVar>::iterator p_y=y_vector.begin()+index2_y(i,Lamuda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;
			double value=cplex.getValue(*p_y);
			if(value>0.99){ value=1.0;}
			else{value=0.0;}
			y_value.push_back(value);
			p_Lamuda++;
		}
	}
	//---------------------------------------------modify the problem for balancing
	//modify the objective function
	expr.clear();
	//for(int id=1;id<=(int)t_vector.size();id++){
	//	expr+=t_vector[id-1]*t_vector[id-1];
	//}
	expr+=t_max;
	obj.setExpr(expr);
	obj.setSense(IloObjective::Minimize);
	expr.clear();
	//fix y_i lamuda
	for(int id=1;id<=(int)y_vector.size();id++){
		y_vector[id-1].setBounds(y_value[id-1],y_value[id-1]);
	}
	//fix lot constraints
	for(int l=1;l<=Num_Lot;l++){
		vii pfind=find(lot_selected.begin(),lot_selected.end(),l);
		if(pfind!=lot_selected.end()){
			con[l-1].setBounds(1.0,1.0);
		}else{
			con[l-1].setBounds(0.0,0.0);
		}
	}
	cplex.exportModel("AT_balancing.lp");
	if(!cplex.solve()){
		cout<<"!!The model can not be solved by CPLEX!!"<<endl;
	}
	cplex.out() <<"Status="<<cplex.getStatus()<<","<<cplex.getObjValue()<<endl;
	cplex.out() <<"Maximum bound violation = "<<cplex.getQuality(IloCplex::MaxPrimalInfeas) << endl;	
	cplex.out() << "Iteration count = " << cplex.getNiterations() << endl;
	cplex.out() << "Num of nodes processed = "<<cplex.getNnodes()<<endl;
	cplex.out() << "Phase I Iter Num= " << cplex.getNphaseOneIterations() <<endl;
	cplex.printTime();
	//---------------------------------------------
	double * solution = new double[num_var+1];
	double total_shortage=0.0;
	int id=0;
	vector<IloNumVar>::iterator p_x=x_vector.begin();
	while(p_x!=x_vector.end()){
		solution[id]=cplex.getValue(*p_x);
		id++;
		p_x++;
	}
	vector<IloNumVar>::iterator p_deltaKD=deltaKD_vector.begin();
	while(p_deltaKD!=deltaKD_vector.end()){
		solution[id]=cplex.getValue(*p_deltaKD);
		total_shortage+=solution[id];
		id++;
		p_deltaKD++;
	}
	vector<IloNumVar>::iterator p_deltaKP=deltaKP_vector.begin();
	while(p_deltaKP!=deltaKP_vector.end()){
		solution[id]=cplex.getValue(*p_deltaKP);
		total_shortage+=solution[id];
		id++;
		p_deltaKP++;
	}
	total_shortage*=coef;  //get the actual number

	vector<IloNumVar>::iterator p_y=y_vector.begin();
	while(p_y!=y_vector.end()){
		solution[id]=cplex.getValue(*p_y);
		id++;
		p_y++;
	}
	vector<IloNumVar>::iterator p_t=t_vector.begin();
	while(p_t!=t_vector.end()){
		solution[id]=cplex.getValue(*p_t);
		id++;
		p_t++;
	}
	//---------------------------------------------
	generate_reports(solution);
	env.end();
	delete [] solution;
	}catch(IloException & e){
		cout<<e.getMessage()<<endl;
	}
#endif
}

void AT::build_model_no_s(){
	//get rid of the s index and try to build a smaller size model
#if 0
	IloEnv env;
	IloModel model(env);
	IloRangeArray con(env);
	vector<IloNumVar> deltaPD_vector;
	vector<IloNumVar> deltaKD_vector;
	vector<IloNumVar> x_vector;
	vector<IloNumVar> y_vector;
	int num_con=0;
	int num_var=0;
	
	//initialize x variables
	for(int i=1;i<=Num_Machine;i++){
		vii p_L=L_i[i].int_vector.begin();
		while(p_L!=L_i[i].int_vector.end()){
			int l=(*p_L);
			string str="x";
			stringstream ss;
			ss<<"("<<i<<")("<<l<<")";
			str+=ss.str();
			char name[20];
			strcpy(name,str.c_str());
			x_vector.push_back(IloNumVar(env,0.0,1.0,name));
			num_var++;
			p_L++;
		}
	}
	//initialize y variables
	for(int i=1;i<=Num_Machine;i++){
		vii p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			string str="y";
			stringstream ss;
			ss<<"("<<i<<")("<<Lamuda_id<<")";
			str+=ss.str();
			char name[20];
			strcpy(name,str.c_str());
			y_vector.push_back(IloNumVar(env,0.0,1.0,name));
			num_var++;
			p_Lamuda++;
		}
	}

	//initalize delta variables
	for(int k=1;k<=Num_Keydevice;k++){
		string str="deltaKD";
		stringstream ss;
		ss<<"("<<k<<")";
		str+=ss.str();
		char name[20];
		strcpy(name,str.c_str());
		deltaKD_vector.push_back(IloNumVar(env,0,IloInfinity,name));
		num_var++;
	}
	for(int k=1;k<=Num_Package;k++){
		string str="deltaPD";
		stringstream ss;
		ss<<"("<<k<<")";
		str+=ss.str();
		char name[20];
		strcpy(name,str.c_str());
		deltaPD_vector.push_back(IloNumVar(env,0,IloInfinity,name));
		num_var++;
	}

	cout<<"build obj"<<endl;
	IloExpr expr(env);
	for(int i=1;i<=Num_Machine;i++){
		vector<int>::iterator p_L=L_i[i].int_vector.begin();
		while(p_L!=L_i[i].int_vector.end()){
			int l=(*p_L);
			vector<IloNumVar>::iterator p_x=x_vector.begin()+index4_x(i,l);
			expr+=w_l[l]*(* p_x);
			p_L++;
		}
	}
	for(int k=1;k<=Num_Keydevice;k++){
		int d=Key_Devices[k-1];
		vector<IloNumVar>::iterator p_deltaKD=deltaKD_vector.begin()+k-1;
		expr-=penalty[d]*(* p_deltaKD);
	}
	for(int p=1;p<=Num_Package;p++){
		int d=Packages[p-1];
		vector<IloNumVar>::iterator p_deltaPD=deltaPD_vector.begin()+p-1;
		expr-=penalty[d]*(* p_deltaPD);
	}
	IloObjective obj(env,expr,IloObjective::Maximize);
	model.add(obj);
	expr.end();

	cout<<"build constraints (1b)"<<endl;
	for(int l=1;l<=Num_Lot;l++){
		IloExpr expr(env);
		vector<int>::iterator p_M=M_l[l].int_vector.begin();
		while(p_M!=M_l[l].int_vector.end()){
			int i=(*p_M);
			vector<IloNumVar>::iterator p_x= x_vector.begin()+index4_x(i,l);
			expr+=(*p_x);
			p_M++;
		}
		con.add(expr<=1.0);
		expr.end();
		num_con++;
		string str="(";
		stringstream ss;
		ss<<"l@"<<l<<")";
		char name[100];
		str+=ss.str();
		strcpy(name,str.c_str());
		con[num_con-1].setName(name);
		//cout<<"l="<<l<<endl;
	}
/*
	cout<<"build constraints (1c)"<<endl;
	for(int i=1;i<=Num_Machine;i++){
		vector<int>::iterator p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			int index=index_L_sim_lamuda(machine_sim[i],Lamuda_id);
			vector<int>::iterator p_L=L_sim_lamuda[index].second.int_vector.begin();
			while(p_L!=L_sim_lamuda[index].second.int_vector.end()){
				int l=(*p_L);
				IloExpr expr(env);
				vector<IloNumVar>::iterator p_x=x_vector.begin()+index4_x(i,l);
				vector<IloNumVar>::iterator p_y=y_vector.begin()+index4_y(i,Lamuda_id)-x_limit_4-Num_Keydevice-Num_Package-1;
				expr=(*p_y)-(*p_x);
				con.add(expr>=0);
				expr.end();
				num_con++;
				string str="(";
				stringstream ss;
				ss<<"i@"<<i<<",Lamuda@"<<Lamuda_id<<",l@"<<l<<")";
				str+=ss.str();
				char name[100];
				strcpy(name,str.c_str());
				con[num_con-1].setName(name);
				p_L++;
			}
			p_Lamuda++;
		}
	}
*/
	cout<<"build constraints (1d)"<<endl;
	for(int i=1;i<=Num_Machine;i++){
		IloExpr expr(env);
		vector<int>::iterator p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(* p_Lamuda);
			vector<IloNumVar>::iterator p_y=y_vector.begin()+index4_y(i,Lamuda_id)-x_limit_4-Num_Keydevice-Num_Package-1;
			expr+=(*p_y);
			p_Lamuda++;
		}
		con.add(expr<=1);
		expr.end();
		num_con++;
		string str="(";
		stringstream ss;
		ss<<"(y)i@"<<i<<")";
		str+=ss.str();
		char name[100];
		strcpy(name,str.c_str());
		con[num_con-1].setName(name);
	}

	cout<<"build constraints (1e)"<<endl;
	for(int tf=1;tf<=Num_Tooling_Family;tf++){
		for(int n=1;n<=(int)Temperature_Set.size();n++){
			vector<t_set>::iterator p_set=Temperature_Set.begin()+n-1;
			IloExpr expr(env);
			for(int i=1;i<=Num_Machine;i++){
				vii p_int=(*p_set).set_elements.begin();
				while(p_int!=(*p_set).set_elements.end()){
					int tau=(*p_int);
					vector<int> Lamuda_i_tf_tau;
					get_Lamuda(Lamuda_i_tf_tau,i,tf,tau);
					vii p_Lamuda=Lamuda_i_tf_tau.begin();
					while(p_Lamuda!=Lamuda_i_tf_tau.end()){
						int Lamuda_id=(*p_Lamuda);
						vector<Lamuda_item>::iterator pL=Lamuda.begin()+Lamuda_id-1;
						vii p_find=find((*pL).tooling_vec.begin(),(*pL).tooling_vec.end(),tf);
						int offset=p_find-(*pL).tooling_vec.begin();
						vii pb=(*pL).b.begin()+offset;
						vector<IloNumVar>::iterator p_y=y_vector.begin()+index4_y(i,Lamuda_id)-x_limit_4-Num_Keydevice-Num_Package-1;
						expr+=(*pb)*(*p_y);
						p_Lamuda++;
					}//lamuda_iteration
					p_int++;
				}//tau_iteration
			}//i_iteration
			vector<t_set> inter_set;
			get_inter_set((*p_set),inter_set);
			vector<t_set>::iterator pm=inter_set.begin();
			int sum=0;
			while(pm!=inter_set.end()){
				int set_id=get_set_id(*pm);
				sum+=n_t_n_tooling[tf][set_id];
				pm++;
			}
			con.add(expr<=sum);
			num_con++;
			expr.end();
			string str="(";
			stringstream ss;
			ss<<"tf@"<<tf<<",n@"<<n<<")";
			str+=ss.str();
			char name[100];
			strcpy(name,str.c_str());
			con[num_con-1].setName(name);
			//cout<<"tf="<<tf<<",n="<<n<<endl;
		}
	}

	cout<<"build constraints (1f)"<<endl;
	for(int i=1;i<=Num_Machine;i++){
		vector<int>::iterator p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			IloExpr expr(env);
			int index=index_L_sim_lamuda(machine_sim[i],Lamuda_id);
			vector<int>::iterator p_L=L_sim_lamuda[index].second.int_vector.begin();
			while(p_L!=L_sim_lamuda[index].second.int_vector.end()){
				int l=(*p_L);
				int s=max_rate_s(i,l,Lamuda_id);
				vector<IloNumVar>::iterator p_x=x_vector.begin()+index4_x(i,l);
				expr+=n_l_chips[l]/rate[s]*(*p_x);
				p_L++;
			}
			vector<IloNumVar>::iterator p_y=y_vector.begin()+index4_y(i,Lamuda_id)-x_limit_4-Num_Keydevice-Num_Package-1;
			expr-=H[i]*(*p_y);
			con.add(expr<=0);
			num_con++;
			expr.end();
			string str="(";
			stringstream ss;
			ss<<"i@"<<i<<"Lamuda@"<<Lamuda_id<<")";
			str+=ss.str();
			char name[100];
			strcpy(name,str.c_str());
			con[num_con-1].setName(name);
			//cout<<"i="<<i<<",Lamuda_id="<<Lamuda_id<<endl;
			p_Lamuda++;
		}
	}

	cout<<"build constraints (1g)"<<endl;
	for(int k=1;k<=Num_Keydevice;k++){
		vector<int>::iterator p_int=Key_Devices.begin()+k-1;
		int d=(*p_int);
		IloExpr expr(env);
		for(int i=1;i<=Num_Machine;i++){
			vector<int>::iterator p_L=L_i_d[i][d].int_vector.begin();
			while(p_L!=L_i_d[i][d].int_vector.end()){
				int l=(*p_L);
				vector<IloNumVar>::iterator p_x=x_vector.begin()+index4_x(i,l);
				expr+=n_l_chips[l]*(*p_x);
				p_L++;
			}
		}
		vector<IloNumVar>::iterator p_KD=deltaKD_vector.begin()+k-1;
		expr+=coef*(*p_KD);
		con.add(expr>=n_k_minchips[d]);
		num_con++;
		expr.end();
		string str="(";
		stringstream ss;
		ss<<"k@"<<k<<",dev@"<<d<<")";
		str+=ss.str();
		char name[100];
		strcpy(name,str.c_str());
		con[num_con-1].setName(name);
		//cout<<"k="<<k<<endl;
	}

	cout<<"build constraints (1h)"<<endl;
	for(int p=1;p<=Num_Package;p++){
		vector<int>::iterator p_int=Packages.begin()+p-1;
		int d=(*p_int);
		IloExpr expr(env);
		for(int i=1;i<=Num_Machine;i++){
			vector<int>::iterator p_L=L_i_d[i][d].int_vector.begin();
			while(p_L!=L_i_d[i][d].int_vector.end()){
				int l=(*p_L);
				vector<IloNumVar>::iterator p_x=x_vector.begin()+index4_x(i,l);
				expr+=n_l_chips[l]*(*p_x);
				p_L++;
			}
		}
		vector<IloNumVar>::iterator p_PD=deltaPD_vector.begin()+p-1;
		expr+=coef*(*p_PD);
		con.add(expr>=n_p_minchips[d]);
		num_con++;
		expr.end();
		string str="(";
		stringstream ss;
		ss<<"p@"<<p<<",dev@"<<d<<")";
		str+=ss.str();
		char name[100];
		strcpy(name,str.c_str());
		con[num_con-1].setName(name);
		//cout<<"p="<<p<<endl;
	}

	cout<<"build constriants (1i)"<<endl;
	for(int it=1;it<=(int)Lamuda.size();it++){
		vector<Lamuda_item>::iterator p_Lamuda=Lamuda.begin()+it-1;
		//-----------------
		int sum_b=0;
		vector<int>::iterator p_int=(*p_Lamuda).b.begin();
		while(p_int!=(*p_Lamuda).b.end()){
			sum_b+=(*p_int);
			p_int++;
		}
		//---------------------------
		if(sum_b>0){//when the tooling list is not empty
			int Lamuda_id=(*p_Lamuda).id;
			//-------------------------------------temperature sest
			int temperature=(*p_Lamuda).temperature;
			struct t_set one_set;
			one_set.set_elements.push_back(temperature);
			vector<t_set> inter_set;
			get_inter_set(one_set,inter_set);
			//---------------------------------------------------------------
			IloExpr expr(env);
			vector<int>::iterator p_M=M_lamuda[Lamuda_id].int_vector.begin();
			while(p_M!=M_lamuda[Lamuda_id].int_vector.end()){
				int i=(*p_M);
				//cout<<"i="<<i<<",Lamuda="<<Lamuda_id<<","<<index4_y(i,Lamuda_id)<<endl;
				vector<IloNumVar>::iterator p_y=y_vector.begin()+index4_y(i,Lamuda_id)-x_limit_4-Num_Keydevice-Num_Package-1;
				expr+=(*p_y);
				p_M++;
			}
			double min_num=1E+8;
			vector<int>::iterator p_tool=(*p_Lamuda).tooling_vec.begin();
			while(p_tool!=(*p_Lamuda).tooling_vec.end()){
				int offset=p_tool-(*p_Lamuda).tooling_vec.begin();
				vector<int>::iterator p_b=(*p_Lamuda).b.begin()+offset;
				int tf=(*p_tool);
				int require_num=(*p_b);
				int available_num=0;
				vector<t_set>::iterator pm=inter_set.begin();
				while(pm!=inter_set.end()){
					int set_id=get_set_id(*pm);
					available_num+=n_t_n_tooling[tf][set_id];
					pm++;
				}
				double ratio=available_num/(require_num*1.0);
				if(ratio<min_num){
					min_num=ratio;
				}
				p_tool++;
			}
			con.add(expr<=(int)min_num);
			num_con++;
			expr.end();
			string str="(";
			stringstream ss;
			ss<<"Lamuda@"<<Lamuda_id<<")";
			str+=ss.str();
			char name[100];
			strcpy(name,str.c_str());
			con[num_con-1].setName(name);
		}
	}
	
	model.add(con);
	
	for(int i=x_limit_4+Num_Keydevice+Num_Package+1;i<=y_limit_4;i++){
		vector<IloNumVar>::iterator p_y=y_vector.begin()+i-x_limit_4-Num_Keydevice-Num_Package-1;
		model.add(IloConversion(env,(*p_y),ILOBOOL));
	}
	for(int i=0;i<=x_limit_4;i++){
		vector<IloNumVar>::iterator p_x=x_vector.begin()+i;
		model.add(IloConversion(env,(*p_x),ILOBOOL));
	}
	
	IloCplex cplex(model);
	cplex.setParam(IloCplex::RootAlg,1); //use primal method
	//cplex.setParam(IloCplex::TiLim,100);
	//cplex.setParam(IloCplex::EpGap,0.005);
	cplex.exportModel("AT_CPLEX_no_s.lp");
	if(!cplex.solve()){
		cout<<"!!The model can not be solved by CPLEX!!"<<endl;
	}
	cplex.out() <<"Status="<<cplex.getStatus()<<","<<cplex.getObjValue()<<endl;
	cplex.out() <<"Maximum bound violation = "<<cplex.getQuality(IloCplex::MaxPrimalInfeas) << endl;	
	cplex.out() << "Iteration count = " << cplex.getNiterations() << endl;
	cplex.out() << "Phase I Iter Num= " << cplex.getNphaseOneIterations() <<endl;
	cplex.printTime();

	env.end();
#endif
}

void AT::build_index_vector(){
	int counter=0;
	for(int i=1;i<=Num_Machine;i++){
		vector<int> L;
		get_L(L,i);
		vector<int>::iterator p_L=L.begin();
		while(p_L!=L.end()){
			int l=(*p_L);
			int sim=machine_sim[i];
			int d=lots_device[l];
			int index=index_S_sim_d(sim,d);
			vii p_s=S_sim_d[index].second.int_vector.begin();
			while(p_s!=S_sim_d[index].second.int_vector.end()){
				int s=(*p_s);
				struct index_node one_node;
				one_node.i=i;
				one_node.l=l;
				one_node.s=s;
				one_node.Lamuda_id=-1;
				one_node.index=counter;
				counter++;
				//cout<<"i="<<i<<",l="<<l<<",s="<<s<<",index="<<one_node.index<<endl;
				index_vector.push_back(one_node);

				p_s++;
			}
			p_L++;
		}
	}
	x_limit=counter-1;
	for(int k=1;k<=Num_Keydevice;k++){
		struct index_node one_node;
		one_node.index=counter;
		one_node.i=-1;
		one_node.l=-1;
		one_node.s=-1;
		one_node.Lamuda_id=-1;
		counter++;
		index_vector.push_back(one_node);
	}
	for(int p=1;p<=Num_Package;p++){
		struct index_node one_node;
		one_node.index=counter;
		one_node.i=-1;
		one_node.l=-1;
		one_node.s=-1;
		one_node.Lamuda_id=-1;
		counter++;
		index_vector.push_back(one_node);
	}
	for(int kp=1;kp<=(int)Key_PKG.size();kp++){
		struct index_node one_node;
		one_node.index=counter;
		one_node.i=-1;
		one_node.l=-1;
		one_node.s=-1;
		one_node.Lamuda_id=-1;
		counter++;
		index_vector.push_back(one_node);
	}
	for(int kpp=1;kpp<=(int)Key_PIN_PKG.size();kpp++){
		struct index_node one_node;
		one_node.index=counter;
		one_node.i=-1;
		one_node.l=-1;
		one_node.s=-1;
		one_node.Lamuda_id=-1;
		counter++;
		index_vector.push_back(one_node);
	}
	for(int i=1;i<=Num_Machine;i++){
		vii p_int=Lamuda_i[i].int_vector.begin();
		while(p_int!=Lamuda_i[i].int_vector.end()){
			struct index_node one_node;
			one_node.i=i;
			one_node.index=counter;
			one_node.l=-1;
			one_node.s=-1;
			one_node.Lamuda_id=(*p_int);
			index_vector.push_back(one_node);
			counter++;
			p_int++;
		}
	}
	y_limit=counter-1;
	cout<<"counter="<<counter<<endl;
	cout<<"x_limit="<<x_limit<<endl;
	cout<<"y_limit="<<y_limit<<endl;
}

void AT::build_first_level_index_vector(){
	ofstream fout("Debug/debug_first_level_index_vector.txt");
	int counter=0;
	for(int i=1;i<=Num_Machine;i++){
		vector<int>::iterator p_L=L_i[i].int_vector.begin();
		while(p_L!=L_i[i].int_vector.end()){
			vii pfind=find(LPD.begin(),LPD.end(),(*p_L));
			if(pfind!=LPD.end()){
				int l=(*p_L);
				int sim=machine_sim[i];
				int d=lots_device[l];
				int index=index_S_sim_d(sim,d);
				vii p_s=S_sim_d[index].second.int_vector.begin();
				while(p_s!=S_sim_d[index].second.int_vector.end()){
					int s=(*p_s);
					struct index_node one_node;
					one_node.i=i;
					one_node.l=l;
					one_node.s=s;
					one_node.Lamuda_id=-1;
					one_node.index=counter;
					counter++;
					fout<<"i="<<i<<",l="<<l<<",s="<<s<<",index="<<one_node.index<<endl;
					first_level_index_vector.push_back(one_node);

					p_s++;
				}
			}
			p_L++;
		}
	}
	x_limit_first_level=counter-1;
	for(int k=1;k<=Num_Keydevice;k++){
		struct index_node one_node;
		one_node.index=counter;
		one_node.i=-1;
		one_node.l=-1;
		one_node.s=-1;
		one_node.Lamuda_id=-1;
		counter++;
		fout<<"i="<<one_node.i<<",l="<<one_node.l<<",s="<<one_node.s<<",index="<<one_node.index<<endl;
		first_level_index_vector.push_back(one_node);
	}
	for(int p=1;p<=Num_Package;p++){
		struct index_node one_node;
		one_node.index=counter;
		one_node.i=-1;
		one_node.l=-1;
		one_node.s=-1;
		one_node.Lamuda_id=-1;
		counter++;
		fout<<"i="<<one_node.i<<",l="<<one_node.l<<",s="<<one_node.s<<",index="<<one_node.index<<endl;
		first_level_index_vector.push_back(one_node);
	}
	//add delta variables for KP and KPP
	for(int kp=1;kp<=(int)Key_PKG.size();kp++){
		struct index_node one_node;
		one_node.index=counter;
		one_node.i=-1;
		one_node.l=-1;
		one_node.s=-1;
		one_node.Lamuda_id=-1;
		counter++;
		fout<<"i="<<one_node.i<<",l="<<one_node.l<<",s="<<one_node.s<<",index="<<one_node.index<<endl;
		first_level_index_vector.push_back(one_node);
	}
	for(int kpp=1;kpp<=(int)Key_PIN_PKG.size();kpp++){
		struct index_node one_node;
		one_node.index=counter;
		one_node.i=-1;
		one_node.l=-1;
		one_node.s=-1;
		one_node.Lamuda_id=-1;
		counter++;
		fout<<"i="<<one_node.i<<",l="<<one_node.l<<",s="<<one_node.s<<",index="<<one_node.index<<endl;
		first_level_index_vector.push_back(one_node);
	}
	for(int i=1;i<=Num_Machine;i++){
		vii p_int=Lamuda_i[i].int_vector.begin();
		while(p_int!=Lamuda_i[i].int_vector.end()){
			struct index_node one_node;
			one_node.i=i;
			one_node.index=counter;
			one_node.l=-1;
			one_node.s=-1;
			one_node.Lamuda_id=(*p_int);
			first_level_index_vector.push_back(one_node);
			counter++;
			fout<<"i="<<one_node.i<<",l="<<one_node.l<<",s="<<one_node.s<<",index="<<one_node.index<<endl;
			p_int++;
		}
	}
	y_limit_first_level=counter-1;
	cout<<"counter (first level model) ="<<counter<<endl;
	cout<<"x_limit_first_level="<<x_limit_first_level<<endl;
	cout<<"y_limit_first_level="<<y_limit_first_level<<endl;
	fout.close();
}

void AT::build_index_vector_no_s(){
	ofstream fout("Debug/debug_index_vector_no_s.txt");
	int counter=0;
	for(int i=1;i<=Num_Machine;i++){
		vector<int>::iterator p_L=L_i[i].int_vector.begin();
		while(p_L!=L_i[i].int_vector.end()){
			int l=(*p_L);
			vector<tooling_configure> TS;
			struct index_node one_node;
			one_node.i=i;
			one_node.l=l;
			one_node.s=-1;
			one_node.Lamuda_id=-1;
			one_node.index=counter;
			counter++;
			fout<<"i="<<i<<",l="<<l<<",index="<<one_node.index<<endl;
			index_vector_no_s.push_back(one_node);
			p_L++;
		}
	}
	x_limit_4=counter-1;
	for(int k=1;k<=Num_Keydevice;k++){
		struct index_node one_node;
		one_node.index=counter;
		one_node.i=-1;
		one_node.l=-1;
		one_node.s=-1;
		one_node.Lamuda_id=-1;
		counter++;
		fout<<"i="<<one_node.i<<",l="<<one_node.l<<",s="<<one_node.s<<",index="<<one_node.index<<endl;
		index_vector_no_s.push_back(one_node);
	}
	for(int p=1;p<=Num_Package;p++){
		struct index_node one_node;
		one_node.index=counter;
		one_node.i=-1;
		one_node.l=-1;
		one_node.s=-1;
		one_node.Lamuda_id=-1;
		counter++;
		fout<<"i="<<one_node.i<<",l="<<one_node.l<<",s="<<one_node.s<<",index="<<one_node.index<<endl;
		index_vector_no_s.push_back(one_node);
	}
	for(int i=1;i<=Num_Machine;i++){
		vii p_int=Lamuda_i[i].int_vector.begin();
		while(p_int!=Lamuda_i[i].int_vector.end()){
			struct index_node one_node;
			one_node.i=i;
			one_node.index=counter;
			one_node.l=-1;
			one_node.s=-1;
			one_node.Lamuda_id=(*p_int);
			fout<<"i="<<one_node.i<<",l="<<one_node.l<<",s="<<one_node.s<<",index="<<one_node.index<<endl;
			index_vector_no_s.push_back(one_node);
			counter++;
			p_int++;
		}
	}
	cout<<"counter="<<counter<<endl;
	y_limit_4=counter-1;
	cout<<"counter (first level model) ="<<counter<<endl;
	cout<<"x_limit_4="<<x_limit_4<<endl;
	cout<<"y_limit_4="<<y_limit_4<<endl;
}

void AT::random_case(){
	//1st step,setup the constants
	srand(Seed);
	//2nd step,setup the parameters
	cout<<"setup H[i],n_l_chips[l],w_l[l]"<<endl;
	for(int i=1;i<=Num_Machine;i++){ H[i]=8;}
	ofstream fnl("Debug/debug_n_l_chips.txt");
	for(int l=1;l<=Num_Lot;l++)    { n_l_chips[l]=1+roundup(rand()*1.0/RAND_MAX*27500); fnl<<"l="<<l<<",n_l_chips="<<n_l_chips[l]<<endl;}
	fnl.close();

	for(int l=1;l<=Num_Lot;l++)    { w_l[l]=rand()*1.0/RAND_MAX*200.0;}
	//3rd step,setup the Machine_Groups vector and Machine_vector
	cout<<"setup Machine_Groups,Machine_vector"<<endl;
	Machine_Groups.clear();
	for(int i=1;i<=Num_Machine;i++){
		double rnd=rand()*1.0/RAND_MAX;
		machine_mg[i]=roundup(rnd*num_mg);
		//cout<<"i="<<i<<",mg="<<machine_mg[i]<<",rnd="<<rnd<<endl;
	}
	for(int mg=1;mg<=num_mg;mg++){
		struct machine_set ms;
		for(int i=1;i<=Num_Machine;i++){
			if(machine_mg[i]==mg){
				ms.machines.push_back(i);
			}
		}
		Machine_Groups.push_back(ms);
	}
	show_mg();

	Machine_vector.clear();
	for(int i=1;i<=Num_Machine;i++){
		struct Machine one_mc;
		one_mc.id=i;
		stringstream ss;
		ss<<"mach"<<i;
		ss.str().copy(one_mc.name,ss.str().size());
		one_mc.mg=machine_mg[i];
		double rnd=rand()*1.0/RAND_MAX;
		int temp_length=roundup(rnd*Num_Temperature);
		//cout<<"temp_length="<<temp_length<<endl;
		int selected[Max_Num_Temperature];
		int excluded[Max_Num_Temperature];
		random_select(Num_Temperature,temp_length,selected,excluded,0);
		for(int tem=0;tem<=temp_length-1;tem++){
			one_mc.temperature_vector.push_back(selected[tem]);
		}
		Machine_vector.push_back(one_mc);
	}
	show_Machine_vector();
		
	//4th step, set up the Tooling_vector
	cout<<"setup Tooling_Families,Tooling_vector"<<endl;
	Tooling_Families.clear();
	for(int i=1;i<=Num_Tooling;i++){
		double rnd=rand()*1.0/RAND_MAX;
		tooling_tf[i]=roundup(rnd*Num_Tooling_Family);
		//cout<<"i="<<i<<",tf="<<tooling_tf[i]<<",rnd="<<rnd<<endl;
	}
	for(int tf=1;tf<=Num_Tooling_Family;tf++){
		struct tooling_set ts;
		for(int i=1;i<=Num_Tooling;i++){
			if(tooling_tf[i]==tf){
				ts.tools.push_back(i);
			}
		}
		Tooling_Families.push_back(ts);
	}
	show_tf();

	Tooling_vector.clear();
	for(int i=1;i<=Num_Tooling;i++){
		struct Tooling one_tool;
		one_tool.id=i;
		stringstream ss;
		ss<<"tool"<<i;
		one_tool.name[0]='\0';
		ss.str().copy(one_tool.name,ss.str().size());
		one_tool.tf=tooling_tf[i];
		double rnd=rand()*1.0/RAND_MAX;
		int temp_length=roundup(rnd*Num_Temperature);
		//cout<<"temp_length="<<temp_length<<endl;
		int selected[Max_Num_Temperature];
		int excluded[Max_Num_Temperature];
		random_select(Num_Temperature,temp_length,selected,excluded,0);
		for(int tem=0;tem<=temp_length-1;tem++){
			one_tool.temperature_vector.push_back(selected[tem]);
		}
		Tooling_vector.push_back(one_tool);
	}
	show_Tooling_vector();

	//5th step,setup the routings
	cout<<"setup routings S,Lamuda"<<endl;
	S.clear();
	int id_couter=1;
	for(int d=1;d<=Num_Device;d++){
		int mg_couter=0;
		while(mg_couter==0){//eliminate the situation such that no machine is selected
			for(int mg=1;mg<=num_mg;mg++){
				double rnd=rand()*1.0/RAND_MAX;
				if(rnd>=0.5){//if the mg is activated for device d
					mg_couter++;
					struct tooling_configure tc;
					tc.device=d;
					tc.mg=mg;
					tc.id=id_couter; 
					id_couter++;
					rate[tc.id]=15000+roundup(rand()*1.0/RAND_MAX*135000);
					S.push_back(tc);
				}
			}
		}
	}

	double Lamuda_ratio=0.5;
	int num_Lamuda=roundup((int)S.size()*Lamuda_ratio);
	vector<Lamuda_item> temp_Lamuda;
	for(int Lamuda_id=1;Lamuda_id<=num_Lamuda;Lamuda_id++){
		struct Lamuda_item one_item;
		one_item.id=Lamuda_id;
		double rnd2=rand()*1.0/RAND_MAX;
		one_item.temperature=roundup(rnd2*Num_Temperature);
		int tf_couter=0;
		while(tf_couter==0){
			for(int tf=1;tf<=Num_Tooling_Family;tf++){
				double rnd=rand()*1.0/RAND_MAX;
				if(rnd<=(1.0/Num_Tooling_Family)){//if the tf is activated
					tf_couter++;
					one_item.tooling_vec.push_back(tf);
					int num=(int)(rand()*1.0/RAND_MAX*4);
					if(num<1){ num=1;}
					one_item.b.push_back(num);
				}
			}
		}
		temp_Lamuda.push_back(one_item);
	}

	vector<tooling_configure>::iterator p_S=S.begin();
	while(p_S!=S.end()){
		int Lamuda_selected=0;
		while(Lamuda_selected<1){ Lamuda_selected=roundup(rand()*1.0/RAND_MAX*num_Lamuda);}
		vector<Lamuda_item>::iterator p_Lamuda=temp_Lamuda.begin()+Lamuda_selected-1;
		(*p_S).tf.tools.resize((int)(*p_Lamuda).tooling_vec.size());
		copy((*p_Lamuda).tooling_vec.begin(),(*p_Lamuda).tooling_vec.end(),(*p_S).tf.tools.begin());
		//setup a_st[][]
		vii p_tf=(*p_S).tf.tools.begin();
		while(p_tf!=(*p_S).tf.tools.end()){
			int tf=(*p_tf);
			int s=(*p_S).id;
			int offset=p_tf-(*p_S).tf.tools.begin();
			vii pb=(*p_Lamuda).b.begin()+offset;
			a_st[s][tf]=(*pb);
			p_tf++;
		}
		(*p_S).temperature=(*p_Lamuda).temperature;
		p_S++;
	}
	show_S();

	//6th,setup key device and package device
	cout<<"setup Key device,Packages device"<<endl;
	
	Key_Devices.clear();
	Packages.clear();
	int * selected=new int[Num_Keydevice+Num_Package];
	int * excluded=new int[Num_Keydevice+Num_Package];
	int excluded_length=0;
	random_select(Num_Device,Num_Keydevice,selected,excluded,excluded_length);
	for(int i=0;i<=Num_Keydevice-1;i++){ 
		Key_Devices.push_back(selected[i]);
		excluded[i]=selected[i];
		excluded_length++;
		selected[i]=0;
	}
	random_select(Num_Device,Num_Keydevice,selected,excluded,excluded_length);
	for(int i=0;i<=Num_Package-1;i++){ 
		Packages.push_back(selected[i]);
	}
	cout<<"Key devices:";
	vector<int>::iterator p_int=Key_Devices.begin();
	while(p_int!=Key_Devices.end()){
		cout<<(*p_int)<<",";
		p_int++;
	}
	cout<<endl;
	cout<<"Package devices:";
	p_int=Packages.begin();
	while(p_int!=Packages.end()){
		cout<<(*p_int)<<",";
		p_int++;
	}
	delete [] selected;
	delete [] excluded;
	cout<<endl;
	//7th,setup Device,lots_device
	cout<<"setup Device,lots_device"<<endl;
	Devices.clear();
	int record_l[Max_Num_Lot];
	for(int l=0;l<=Num_Lot;l++){ record_l[l]=0;}
	for(int d=1;d<=Num_Device-1;d++){
		struct device_item dev;
		dev.device=d;
		int l_count=0;
		while(l_count==0){
			for(int l=1;l<=Num_Lot;l++){
				double rnd=rand()*1.0/RAND_MAX;
				if(rnd<=1.0/Num_Device && record_l[l]==0){//lot l is activated
					record_l[l]=1;
					lots_device[l]=d;
					l_count++;
					dev.device_lots.push_back(l);
				}
			}
		}
		Devices.push_back(dev);
	}
	struct device_item dev;
	dev.device=Num_Device;
	for(int l=1;l<=Num_Lot;l++){
		if(record_l[l]==0){
			record_l[l]=1;
			lots_device[l]=Num_Device;
			dev.device_lots.push_back(l);
		}
	}
	Devices.push_back(dev);
	show_Devices();
	//for(int l=1;l<=Num_Lot;l++){ cout<<"l="<<l<<",device="<<lots_device[l]<<endl;}

	//8th step
	p_int=Key_Devices.begin();
	while(p_int!=Key_Devices.end()){
		int temp=(*p_int);
		n_k_minchips[temp]=roundup(rand()*1.0/RAND_MAX*120000);
		p_int++;
	}
	p_int=Packages.begin();
	while(p_int!=Packages.end()){
		int temp=(*p_int);
		n_p_minchips[temp]=roundup(rand()*1.0/RAND_MAX*120000);
		p_int++;
	}
	
	//9th setup the temperature combination
	cout<<"setup temperature T and temperature combination"<<endl;
	temp_combination();
	show_temp_comb(Temperature_Set);
	for(int tf=1;tf<=(int)Tooling_Families.size();tf++){
		for(int n=1;n<=(int)Temperature_Set.size();n++){
			n_t_n_tooling[tf][n]=0;
			vector<int>::iterator pt=Tooling_Families[tf-1].tools.begin();
			while(pt!=Tooling_Families[tf-1].tools.end()){
				int tool=(*pt);
				bool com=compare_int_vec(Tooling_vector[tool-1].temperature_vector,Temperature_Set[n-1].set_elements);
				if(com){
					n_t_n_tooling[tf][n]++;
				}
				pt++;
			}
			//cout<<"tf="<<tf<<",n="<<n<<",n_t_n_tooling="<<n_t_n_tooling[tf][n]<<endl;
		}
	}
	//---------------------------------10th, intialization
	cout<<"initialization"<<endl;
	show_w_l();
	show_eps();
	initialize_L_i();
	show_L_i();
	initialize_L_i_d();
	show_L_i_d();
	initialize_M_l();
	show_M_l();
	initialize_S_i();
	show_S_i();
	initialize_equivalent_machines();
	show_equivalent_machines();
	show_device_name();
	show_lot_name();
	//----------------
	get_Lamuda();
	show_Lamuda();
	initialize_Lamuda_i();
	show_Lamuda_i();
	initialize_M_lamuda();
	//step,setup the model
	//cout<<"get SIM"<<endl;
	get_SIM();
	initialize_S_sim_d();
	get_tooling_sum_rhs();
	initialize_L_sim_lamuda();
	initialize_sim_lamuda_L();
	initialize_sil();
	initialize_S_sim_d_lamuda();
	index_vector.clear();
	build_index_vector();
	//build_index_vector_no_s();

	show_L_sim_lamuda();
	show_sim_lamuda_L();
	show_SIM();
	show_machine_sim();
	show_index_vector();
	show_sil();
	show_S_sim_d();
	show_S_sim_d_lamuda();
	get_LPD();
	get_sum_weight();
	if(Run_Option){
		build_model();
	}
	show_PIN_PKG();
	//----------------------------
	first_level_heuristic();
	cout<<"The run has been finished"<<endl;
	exit(0);
}

int AT::roundup(double f)
{	//roundup the fractional f to integer
	if(is_integer(f)) return (int)f;
	return (int)f+1;
}

int AT::is_integer(double x){
	//test if x is an integer, return 1 if it is integer
	int lb=(int)x;
	double eps=1e-06;
	if(x-lb>eps) return 0;
	return 1;
}

void AT::random_select(int N,int n,int selected[],int excluded[],int excluded_length){
	//randomly select n objects from N objects
	int * record=new int[N+1];
	for(int i=1;i<=N;i++){ record[i]=0;}
	for(int i=0;i<=excluded_length-1;i++){
		int temp=excluded[i];
		record[temp]=1;
	}
	int counter=0;
	for(int i=1;i<=n;i++){
		double rnd=rand()*1.0/RAND_MAX;
		rnd=rnd*N;
		int rndup=roundup(rnd);
		if(record[rndup]==0){
			record[rndup]=1;
			selected[counter]=rndup;
			counter++;
		}else{//the object is already selected
			i--;
		}
	}
	delete [] record;
}

int AT::not_in_int_array(int target,int array[],int array_length){
	//return 0 if target is not in this array
	//return 1 otherwise
	//index: 0,1,2,...,array_length-1
	int i,found=0;
	for(i=0;i<=array_length-1;i++){
		if(target==array[i]){
			found=1;
			break;
		}
	}
	//cout<<"found="<<found<<endl;
	return found;
}

void AT::display(vii begin,vii end){
	struct t_set one_set;
	for (vii it=begin;it!=end;++it){
		one_set.set_elements.push_back(*it);
	}
	Temperature_Set.push_back(one_set);
}

void AT::temp_combination(){
vector<int> G;
	for(int tau=1;tau<=Num_Temperature;tau++){
		G.push_back(tau);
	}
	vector<int> save;
	vector<int> H;
	for(int i=1;i<=Num_Temperature;i++){
		save.push_back(i);
		H.resize((int)save.size());
		copy(save.begin(),save.end(),H.begin());
		do{
			struct t_set one_set;
			vii p_int=H.begin();
			while(p_int!=H.end()){
				one_set.set_elements.push_back(*p_int);
				p_int++;
			}
			Temperature_Set.push_back(one_set);
		}while(next_combination(G.begin(),G.end(),H.begin(),H.end()));
	}
}

void AT::get_super_set(struct t_set current_set,vector<t_set> & super_set){
	vector<t_set>::iterator p_set=Temperature_Set.begin();
	while(p_set!=Temperature_Set.end()){
		int flag=0;
		vii p_int=current_set.set_elements.begin();
		while(p_int!=current_set.set_elements.end()){
			vii p_find=find((*p_set).set_elements.begin(),(*p_set).set_elements.end(),(*p_int));
			if(p_find!=(*p_set).set_elements.end()){
				flag++;
			}
			p_int++;
		}
		if(flag>=(int)current_set.set_elements.size()){//it is a super set
			struct t_set one_set=(*p_set);
			super_set.push_back(one_set);
		}
		p_set++;
	}
}

void AT::get_inter_set(struct t_set current_set,vector<t_set> & inter_set){
	vector<t_set>::iterator p_set=Temperature_Set.begin();
	while(p_set!=Temperature_Set.end()){
		vii p_int=current_set.set_elements.begin();
		while(p_int!=current_set.set_elements.end()){
			vii p_find=find((*p_set).set_elements.begin(),(*p_set).set_elements.end(),(*p_int));
			if(p_find!=(*p_set).set_elements.end()){//interset
				struct t_set one_set=(*p_set);
				inter_set.push_back(one_set);
				break;
			}
			p_int++;
		}
		p_set++;
	}
}

int AT::get_set_id(struct t_set current_set){
	int id=0;
	vector<t_set>::iterator p_set=Temperature_Set.begin();
	while(p_set!=Temperature_Set.end()){
		if((int)(*p_set).set_elements.size()==(int)current_set.set_elements.size()){		
			int flag=0;
			vii p_int=current_set.set_elements.begin();
			while(p_int!=current_set.set_elements.end()){
				vii p_find=find((*p_set).set_elements.begin(),(*p_set).set_elements.end(),(*p_int));
				if(p_find!=(*p_set).set_elements.end()){ flag++;}
				p_int++;
			}
			if(flag>=(int)current_set.set_elements.size()){
				id=p_set-Temperature_Set.begin()+1;
				break;
			}
		}
		p_set++;
		if(id>0){ break;}
	}
	return id;
}

int AT::strtok2(char * pdes, char * porg, int start_id, char delim){
	//the strtok() provided by C++ could not work for the case ',,,,,,,'
	//find the string starting from start_id until meeting the delim character or meeting the end of the porg
	//return the position in the orginal string after this operation
	//see the test_strtok2()
	char ch;
	ch=porg[start_id];
	if(ch=='\0' || ch==delim){//if the starting characther is the end or the delim
		pdes[0]='\0'; 
		return start_id+1;
	}else{
		int id=start_id;
		while(ch!='\0' && ch!=delim){
			pdes[id-start_id]=ch;
			id++;
			ch=porg[id];
		}
		pdes[id-start_id]='\0';
		return id+1;
	}
}

int AT::strtok2(char * pdes, char * porg, int start_id, char delim1, char delim2){
	//the strtok() provided by C++ could not work for the case ',,,,,,,'
	//find the string starting from start_id until meeting the delim character or meeting the end of the porg
	//return the position in the orginal string after this operation
	//see the test_strtok2()
	char ch;
	ch=porg[start_id];
	if(ch=='\0' || ch==delim1 || ch==delim2){//if the starting characther is the end of the delim
		pdes[0]='\0'; 
		return start_id+1;
	}else{
		int id=start_id;
		while(ch!='\0' && ch!=delim1 && ch!=delim2){
			pdes[id-start_id]=ch;
			id++;
			ch=porg[id];
		}
		pdes[id-start_id]='\0';
		return id+1;
	}
}

void AT::trim(char * ch){
	int length = strlen(ch);
	if(length==0) return;
	if(ch[0]!=' ' && ch[length-1]!=' ') return;
	int i;
	int pfirst=0,plast=length-1;
	for(i=1;i<=length-1;i++){
		if(ch[i-1]==' ' && ch[i]!=' '){
			pfirst = i;
			break;
		}
	}
	for(i=length-2; i>=0; i--){
		if(ch[i+1]==' ' && ch[i]!=' '){
			plast = i;
			break;
		}
	}
	char temp[500];
	for(i=pfirst; i<=plast; i++){ temp[i-pfirst] = ch[i]; }
	for(i=0; i<=plast-pfirst; i++){ ch[i] = temp[i]; }
	ch[i]='\0';
}

void AT::trim(char * ch, char delim){
	//trim the delim from both end of the string, like "O"->O
	int length = strlen(ch);
	if(length==0) return;
	if(ch[0]!=delim && ch[length-1]!=delim) return;
	int i;
	int pfirst=0,plast=length-1;
	for(i=1;i<=length-1;i++){
		if(ch[i-1]==delim && ch[i]!=delim){
			pfirst = i;
			break;
		}
	}
	for(i=length-2; i>=0; i--){
		if(ch[i+1]==delim && ch[i]!=delim){
			plast = i;
			break;
		}
	}
	char temp[500];
	for(i=pfirst; i<=plast; i++){ temp[i-pfirst] = ch[i]; }
	for(i=0; i<=plast-pfirst; i++){ ch[i] = temp[i]; }
	ch[i]='\0';
}

void AT::create_case_from_files(){
	readFiles();
	show_w_l();
	show_eps();
	initialize_L_i();
	//show_L_i();
	initialize_L_i_d();
	//show_L_i_d();
	initialize_M_l();
	//show_M_l();
	initialize_S_i();
	//show_S_i();
	initialize_equivalent_machines();
	show_equivalent_machines();
	show_device_name();
	show_lot_name();
	//----------------
	get_Lamuda();
	show_Lamuda();
	initialize_Lamuda_i();
	show_Lamuda_i();
	initialize_M_lamuda();
	//step,setup the model
	cout<<"get SIM"<<endl;
	get_SIM();
	initialize_S_sim_d();
	get_tooling_sum_rhs();
	initialize_L_sim_lamuda();
	initialize_sim_lamuda_L();
	initialize_sil();
	initialize_S_sim_d_lamuda();
	index_vector.clear();
	build_index_vector();
	//build_index_vector_no_s();

	show_L_sim_lamuda();
	show_sim_lamuda_L();
	show_sil();
	show_S_sim_d_lamuda();
	get_LPD();
	get_sum_weight();
	show_S_sim_d();
	show_PIN_PKG();
	//show_SIM();
	//show_machine_sim();
	//show_index_vector();
	//build_model_no_s();
	if(Run_Option){
		build_model();
	}
	//initial_soln();
	//----------------------------
	first_level_heuristic();
	cout<<"The run has been finished"<<endl;
	exit(0);
}

void AT::initialize_equivalent_machines(){
	for(int mg=1;mg<=Num_Machine_Group;mg++){
		for(int n=1;n<=(int)Temperature_Set.size();n++){
			struct equ_mach_set one_equ_mach_set;
			one_equ_mach_set.mg=mg;
			one_equ_mach_set.n=n;
			vector<t_set>::iterator p_set=Temperature_Set.begin()+n-1;
			vector<Machine>::iterator p_M=Machine_vector.begin();
			while(p_M!=Machine_vector.end()){
				if((*p_M).mg==mg){
					int flag1=1;
					vii p_temp=(*p_M).temperature_vector.begin();
					while(p_temp!=(*p_M).temperature_vector.end()){
						vii p_find=find((*p_set).set_elements.begin(),(*p_set).set_elements.end(),(*p_temp));
						if(p_find==(*p_set).set_elements.end()){
							flag1=0; break;
						}
						p_temp++;
					}
					int flag2=1;
					p_temp=(*p_set).set_elements.begin();
					while(p_temp!=(*p_set).set_elements.end()){
						vii p_find=find((*p_M).temperature_vector.begin(),(*p_M).temperature_vector.end(),(*p_temp));
						if(p_find==(*p_M).temperature_vector.end()){
							flag2=0; break;
						}
						p_temp++;
					}
					if(flag1==1&&flag2==1){
						one_equ_mach_set.equ_mach.push_back((*p_M).id);
					}
				}
				p_M++;
			}
			Equivalent_Machine.push_back(one_equ_mach_set);
		}
	}
}

void AT::show_equivalent_machines(){
	ofstream f_out("Debug/debug_equivalent_machines.txt");
	vector<equ_mach_set>::iterator p_equ_mach=Equivalent_Machine.begin();
	while(p_equ_mach!=Equivalent_Machine.end()){
		f_out<<"mg="<<(*p_equ_mach).mg<<",n="<<(*p_equ_mach).n<<endl;
		vii p_int=(*p_equ_mach).equ_mach.begin();
		while(p_int!=(*p_equ_mach).equ_mach.end()){
			f_out<<(*p_int)<<",";
			p_int++;
		}
		f_out<<endl;
		p_equ_mach++;
	}
}

void AT::show_lot_set_heur(){
	ofstream fout("Debug/debug_lot_set_heur.txt");
	vii p_int=lot_set_heur.begin();
	while(p_int!=lot_set_heur.end()){
		int d=lots_device[(*p_int)];
		fout<<"lot="<<(*p_int)<<",d="<<d<<",weight="<<w_l[(*p_int)]<<endl;
		p_int++;
	}
	fout.close();
}

void AT::lot_assignment_heur(){
	vii p_lot=lot_set_heur.begin();
	while(p_lot!=lot_set_heur.end()){
		int l=(*p_lot);
		vector<tooling_configure>  TS;
		get_S_l(TS,l);
		//sort(TS.begin(),TS.end(),(*this)->compare_tc);//sort according to eps[]
		sort_TS(TS);
		vector<tooling_configure>::iterator p_TS=TS.begin();
		while(p_TS!=TS.end()){//find the major route
			p_TS++;
		}
		p_lot++;
	}
}

void AT::sort_TS(vector<tooling_configure> & TS){
	//sort TS according to eps
	for(int pass=0;pass<=(int)TS.size()-2;pass++){
		for(int i=0;i<=(int)TS.size()-2;i++){
			vector<tooling_configure>::iterator p1=TS.begin()+i;
			vector<tooling_configure>::iterator p2=TS.begin()+i+1;
			if(!compare_tc((*p1),(*p2))){
				struct tooling_configure one_tc;
				one_tc=(*p1);
				(*p1)=(*p2);
				(*p2)=one_tc;
			}
		}
	}
}

void AT::show_w_l(){
	ofstream fout("Debug/debug_w_l.txt");
	fout<<"show w_l[] --------------------------"<<endl;
	for(int l=1;l<=Num_Lot;l++){
		fout<<"l="<<l<<",w_l="<<w_l[l]<<endl;
	}
	fout.close();
}

void AT::show_eps(){
	ofstream fout("Debug/debug_eps.txt");
	fout<<"show eps[] --------------------------"<<endl;
	vector<tooling_configure>::iterator p_S=S.begin();
	while(p_S!=S.end()){
		int s=(*p_S).id;
		fout<<"s="<<s<<",eps="<<eps[s]<<endl;
		p_S++;
	}
	fout.close();
}

void AT::test_feasibility(double value[]){
	double obj=0.0;
	for(int id=0;id<=x_limit;id++){
		if(value[id]>0){
			vector<index_node>::iterator p_x=index_vector.begin()+id;
			int s=(*p_x).s;
			int i=(*p_x).i;
			int l=(*p_x).l;
			obj+=(w_l[l]-eps[s])*value[id];
			//cout<<"id="<<id<<",i="<<i<<",l="<<l<<",s="<<s<<",eps="<<eps[s]<<",value="<<value[id]<<endl;
		}
	}
	cout<<"obj="<<obj<<endl;
	for(int id=x_limit+1;id<=x_limit+Num_Keydevice;id++){
		int d=Key_Devices[id-x_limit-1];
		obj-=penalty[d]*value[d];
	}
	for(int id=x_limit+Num_Keydevice+1;id<=x_limit+Num_Keydevice+Num_Package;id++){
		int d=Packages[id-x_limit-Num_Keydevice-1];
		obj-=penalty[d]*value[id];
	}
	cout<<"obj="<<obj<<endl;
	cout<<"test constraints (1b)";
	for(int l=1;l<=Num_Lot;l++){
		double sum=0;
		vector<int>::iterator p_M=M_l[l].int_vector.begin();
		while(p_M!=M_l[l].int_vector.end()){
			int i=(*p_M);
			int sim=machine_sim[i];
			int d=lots_device[l];
			int index=index_S_sim_d(sim,d);
			vii p_s=S_sim_d[index].second.int_vector.begin();
			while(p_s!=S_sim_d[index].second.int_vector.end()){
				int s=(*p_s);
				sum+=value[index2_x(i,l,s)];
				p_s++;
			}
			p_M++;
		}
		string str="(";
		stringstream ss;
		ss<<"l@"<<l<<")";
		char name[100];
		str+=ss.str();
		strcpy(name,str.c_str());
		if(sum>1.001){
			cout<<" fail:(LHS="<<sum<<")"<<name<<endl;
		}
	}
	cout<<endl;

	cout<<"test constraints (1c)";
	for(int i=1;i<=Num_Machine;i++){
		vector<int>::iterator p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			vector<int>::iterator p_L=L_i[i].int_vector.begin();
			while(p_L!=L_i[i].int_vector.end()){
				int l=(*p_L);
				vector<tooling_configure> TS;
				get_S_i_l_Lamuda(TS,i,l,Lamuda_id);
				vector<tooling_configure>::iterator p_TS=TS.begin();
				while(p_TS!=TS.end()){
					double sum=0;
					int s=(*p_TS).id;
					sum-=value[index2_x(i,l,s)];
					sum+=value[index2_y(i,Lamuda_id)];
					string str="(";
					stringstream ss;
					ss<<"i@"<<i<<",Lamuda@"<<Lamuda_id<<",l@"<<l<<",s@"<<s<<")";
					str+=ss.str();
					char name[100];
					strcpy(name,str.c_str());
					if(sum<-1E-05){
						cout<<" fail:"<<name<<endl;
					}
					p_TS++;
				}
				p_L++;
			}
			p_Lamuda++;
		}
	}
	cout<<endl;

	cout<<"test constraints (1d)";
	for(int i=1;i<=Num_Machine;i++){
		double sum=0;
		vector<int>::iterator p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(* p_Lamuda);
			sum+=value[index2_y(i,Lamuda_id)];
			p_Lamuda++;
		}
		string str="(";
		stringstream ss;
		ss<<"(y)i@"<<i<<")";
		str+=ss.str();
		char name[100];
		strcpy(name,str.c_str());
		if(sum>1.001){
			cout<<" fail:"<<name<<endl;
		}
	}
	cout<<endl;

	cout<<"test constraints (1e)";
	for(int tf=1;tf<=Num_Tooling_Family;tf++){
		for(int n=1;n<=(int)Temperature_Set.size();n++){
			vector<t_set>::iterator p_set=Temperature_Set.begin()+n-1;
			double sumx=0.0;
			for(int i=1;i<=Num_Machine;i++){
				vii p_int=(*p_set).set_elements.begin();
				while(p_int!=(*p_set).set_elements.end()){
					int tau=(*p_int);
					vector<int> Lamuda_i_tf_tau;
					get_Lamuda(Lamuda_i_tf_tau,i,tf,tau);
					vii p_Lamuda=Lamuda_i_tf_tau.begin();
					while(p_Lamuda!=Lamuda_i_tf_tau.end()){
						int Lamuda_id=(*p_Lamuda);
						vector<Lamuda_item>::iterator pL=Lamuda.begin()+Lamuda_id-1;
						vii p_find=find((*pL).tooling_vec.begin(),(*pL).tooling_vec.end(),tf);
						int offset=p_find-(*pL).tooling_vec.begin();
						vii pb=(*pL).b.begin()+offset;
						sumx+=(*pb)*value[index2_y(i,Lamuda_id)];
						p_Lamuda++;
					}//lamuda_iteration
					p_int++;
				}//tau_iteration
			}//i_iteration
			vector<t_set> inter_set;
			get_inter_set((*p_set),inter_set);
			vector<t_set>::iterator pm=inter_set.begin();
			int sum=0;
			while(pm!=inter_set.end()){
				int set_id=get_set_id(*pm);
				sum+=n_t_n_tooling[tf][set_id];
				pm++;
			}
			string str="(";
			stringstream ss;
			ss<<"tf@"<<tf<<",n@"<<n<<")";
			str+=ss.str();
			char name[100];
			strcpy(name,str.c_str());
			if(sumx>sum+0.001){
				cout<<" fail:"<<name<<","<<sumx<<">"<<sum<<endl;
			}
		}
	}
	cout<<endl;

	cout<<"test constraints (1f)";
	for(int i=1;i<=Num_Machine;i++){
		double sum=0;
		vector<int>::iterator p_L=L_i[i].int_vector.begin();
		while(p_L!=L_i[i].int_vector.end()){
			int l=(*p_L);
			int sim=machine_sim[i];
			int d=lots_device[l];
			int index=index_S_sim_d(sim,d);
			vii p_s=S_sim_d[index].second.int_vector.begin();
			while(p_s!=S_sim_d[index].second.int_vector.end()){
				int s=(*p_s);
				sum+=n_l_chips[l]/rate[s]*value[index2_x(i,l,s)];
				p_s++;
			}
			p_L++;
		}
		string str="(";
		stringstream ss;
		ss<<"i@"<<i<<")";
		str+=ss.str();
		char name[100];
		strcpy(name,str.c_str());
		if(sum>H[i]+0.01){
			cout<<" fail:"<<name<<endl;
		}
	}
	cout<<endl;

	cout<<"test constraints (1g)";
	for(int p=1;p<=Num_Package;p++){
		vector<int>::iterator p_int=Packages.begin()+p-1;
		int d=(*p_int);
		double sum=0.0;
		for(int i=1;i<=Num_Machine;i++){
			vector<int>::iterator p_L=L_i_d[i][d].int_vector.begin();
			while(p_L!=L_i_d[i][d].int_vector.end()){
				int l=(*p_L);
				int sim=machine_sim[i];
				int index=index_S_sim_d(sim,d);
				vii p_s=S_sim_d[index].second.int_vector.begin();
				while(p_s!=S_sim_d[index].second.int_vector.end()){
					int s=(*p_s);
					sum+=n_l_chips[l]*value[index2_x(i,l,s)];
					p_s++;
				}
				p_L++;
			}
		}
		sum+=value[x_limit+Num_Keydevice+p]*coef;
		string str="(";
		stringstream ss;
		ss<<"p@"<<p<<",dev@"<<d<<")";
		str+=ss.str();
		char name[100];
		strcpy(name,str.c_str());
		if(sum<n_p_minchips[d]-0.001){
			cout<<" fail:"<<name<<endl;
		}
	}
	cout<<endl;

	cout<<"test constraints (1h)";
	for(int k=1;k<=Num_Keydevice;k++){
		vector<int>::iterator p_int=Key_Devices.begin()+k-1;
		int d=(*p_int);
		double sum=0;
		for(int i=1;i<=Num_Machine;i++){
			vector<int>::iterator p_L=L_i_d[i][d].int_vector.begin();
			while(p_L!=L_i_d[i][d].int_vector.end()){
				int l=(*p_L);
				int sim=machine_sim[i];
				int index=index_S_sim_d(sim,d);
				vii p_s=S_sim_d[index].second.int_vector.begin();
				while(p_s!=S_sim_d[index].second.int_vector.end()){
					int s=(*p_s);
					sum+=value[index2_x(i,l,s)]*n_l_chips[l];
					p_s++;
				}
				p_L++;
			}
		}
		sum+=value[x_limit+k]*coef;
		string str="(";
		stringstream ss;
		ss<<"k@"<<k<<",dev@"<<d<<")";
		str+=ss.str();
		char name[100];
		strcpy(name,str.c_str());
		if(sum<n_k_minchips[d]-0.001){
			cout<<" fail:"<<name<<endl;
		}
	}
	cout<<endl;
}

void AT::sort_lot_heur(int finished_dev_amount[]){
	//sort the subset of lots
	//lot_set_heur -> p_device, dev_ord_heur
	//the function builds p_device, dev_ord_heur
	//ofstream fout("debug_sort_lot_heur.txt");
	f_log<<"In sort_lot_heur()"<<endl;
	//build p_device[] from lot_set_heur
	for(int i=0;i<=Max_Num_Device-1;i++){
		p_device[i].int_vector.clear();
	}
	vii p_lot=lot_set_heur.begin();
	while(p_lot!=lot_set_heur.end()){
		int lot=(*p_lot);
		int dev=lots_device[lot];
		p_device[dev].int_vector.push_back(lot);
		p_lot++;
	}
	//--------------------------------------------------
	double * total_weight=new double[Num_Device+1];
	double * total_qty = new double[Num_Device+1];
	double * unit_price= new double[Num_Device+1];
	int    * dev_seq   = new int[Num_Device+1];
	for(int d=1;d<=Num_Device;d++){
		total_weight[d]=0;
		total_qty[d]   =0;
	}
	for(int d=1;d<=Num_Device;d++){
		dev_seq[d]=d;
		vii p_l=p_device[d].int_vector.begin();
		while(p_l!=p_device[d].int_vector.end()){
			int l=(*p_l);
			total_weight[d]+=w_l[l];
			total_qty[d]+=n_l_chips[l];
			p_l++;
		}
		if(total_qty[d]>1){
			unit_price[d]=total_weight[d]/total_qty[d];
			vii p_find=find(Packages.begin(),Packages.end(),d);
			if(p_find==Packages.end()){
				p_find=find(Key_Devices.begin(),Key_Devices.end(),d);
				if(p_find!=Key_Devices.end()){//d is a keydevice
					double temp=n_k_minchips[d]-finished_dev_amount[d];
					if(temp>0){
						unit_price[d]+=temp;
					}
				}
			}else{//d is a package device
				double temp=n_p_minchips[d]-finished_dev_amount[d];
				if(temp>0){
					unit_price[d]+=temp;
				}
			}
		}else{
			unit_price[d]=0;
		}
		//cout<<"device="<<d<<",total_weight="<<total_weight[d]<<",total_qty="<<total_qty[d]<<",unit_price="<<unit_price[d]<<endl;
	}
	//sort the devices according to unit_price[],the order is stored in up[]
	int * up=new int[Num_Device+1];
	for(int d=1;d<=Num_Device;d++){ up[d]=0;}
	up[1]=1; 
	int up_length=1;
	for(int d=2;d<=Num_Device;d++){
		int index=0;
		for(int up_id=up_length;up_id>=1;up_id--){
			int i=up[up_id];
			if(unit_price[d]>unit_price[i]){
				index=up_id;
				up[up_id+1]=up[up_id];
			}else{
				index=up_id+1;
				break;
			}
		}
		up[index]=d;
		up_length++;
	}
	//store the devices' sequence in dev_ord_heur
	dev_ord_heur.clear();
	for(int d=1;d<=Num_Device;d++){
		f_log<<"d="<<up[d]<<",unit_price="<<unit_price[up[d]]<<endl;
		if(unit_price[up[d]]>0){
			dev_ord_heur.push_back(up[d]);
		}
	}
	//sort the lots within one device
	for(int d=1;d<=Num_Device;d++){
		if((int)p_device[d].int_vector.size()>0){
			vector<int> lot_vector;
			lot_vector.push_back(*p_device[d].int_vector.begin());
			vii p_l=p_device[d].int_vector.begin()+1;
			while(p_l!=p_device[d].int_vector.end()){
				int l=(*p_l);
				double unit_price=w_l[l]/n_l_chips[l];
				vii p_int=lot_vector.end()-1;
				while(p_int!=lot_vector.begin()-1){
					int lot=(*p_int);
					if(w_l[lot]/n_l_chips[lot]>unit_price){
						break;
					}
					p_int--;
				}
				p_int++;
				lot_vector.insert(p_int,l);
				p_l++;
			}
			swap(lot_vector,p_device[d].int_vector);
			f_log<<"d="<<d<<":";
			p_l=p_device[d].int_vector.begin();
			while(p_l!=p_device[d].int_vector.end()){
				f_log<<(*p_l)<<",";
				p_l++;
			}
			f_log<<endl;
		}
	}
	//fout.close();
	delete [] up;
	delete [] dev_seq;
	delete [] unit_price;
	delete [] total_qty;
	delete [] total_weight;
}

void AT::assign_heur(int ** tooling_used,int finished_dev_amount[]){
	//ofstream fout("debug_assign_heur.txt");
	f_log<<"In assign_heur()"<<endl;
	int lot_counter=0;
	int break_limit=0;
	vii p_dev=dev_ord_heur.begin();
	while(p_dev!=dev_ord_heur.end()){
		int dev=(*p_dev);
		//-----------------------------S(d)
		vector<tooling_configure> TS;
		get_S_l(TS,(*p_device[dev].int_vector.begin()));
		//sort(TS.begin(),TS.end(),compare_tc);//sort the routes, primative first according to eps[]
		sort_TS(TS);

		vii p_lot=p_device[dev].int_vector.begin();
		while(p_lot!=p_device[dev].int_vector.end()){//for each lot in the device 
			f_log<<"dev="<<dev<<",lot="<<(*p_lot)<<endl;

			int mach_selected=-1;
			int route_selected=-1;
			vector<int> mach_vect;
			vector<int> route_vect;
			vector<tooling_configure>::iterator p_TS=TS.begin();
			while(p_TS!=TS.end()){
				int mg=(*p_TS).mg;
				int s=(*p_TS).id;
				vector<int> a;
				vii pt=(*p_TS).tf.tools.begin();
				while(pt!=(*p_TS).tf.tools.end()){
					int tf=(*pt);
					a.push_back(a_st[s][tf]);
					pt++;
				}

				vii pm=(*(Machine_Groups.begin()+mg-1)).machines.begin();
				//f_log<<"mg="<<mg<<",temperature="<<temperature<<endl;
				while(pm!=(*(Machine_Groups.begin()+mg-1)).machines.end()){
					int i=(*pm);
					vector<machine_heur_item>::iterator p_mach=Machine_Heur.begin()+i-1;
					//is the mach running?
					if(compare_int_vec((*p_TS).tf.tools,(*p_mach).tooling)
						&& compare_int_vec(a,(*p_mach).b)
						&& (*p_mach).temperature==(*p_TS).temperature
						&& (*p_mach).time_used+n_l_chips[(*p_lot)]/rate[s]<=H[i])
					{//some toolings have been installed
						mach_vect.push_back(i);
						route_vect.push_back(s);
					}
					pm++;
				}
				p_TS++;
			}
			//cout<<"1"<<endl;
			if((int)(mach_vect.size())>0){//some running mach can be assigned to the device
				//cout<<"2"<<endl;
				double min_s=1E+8;
				int s_record=-1;
				vii p_route=route_vect.begin();
				while(p_route!=route_vect.end()){
					int s=(*p_route);
					if(eps[s]<min_s){
						min_s=eps[s];
						s_record=s;
					}
					p_route++;
				}

				vii p_mach=mach_vect.begin();
				double min_t=1E+8;
				int i_record=-1;
				while(p_mach!=mach_vect.end()){
					//select the machine with the least remaining time
					int n=p_mach-mach_vect.begin();
					vii p_r=route_vect.begin()+n;
					vector<machine_heur_item>::iterator pm=Machine_Heur.begin()+(*p_mach)-1;
					if((*pm).time_used<min_t && (*p_r)==s_record){
						min_t=(*pm).time_used;
						i_record=(*p_mach);
					}
					p_mach++;
				}
				mach_selected=i_record;
				route_selected=s_record;
			}else{//no related machine is running
				//cout<<"3"<<endl;
				p_TS=TS.begin();
				while(p_TS!=TS.end()){
					int tooling_selected_flag=0;
					int s=(*p_TS).id;
					int mg=(*p_TS).mg;
					int temperature=(*p_TS).temperature;
					struct t_set one_set;
					one_set.set_elements.push_back(temperature);
					vector<t_set> inter_set;
					get_inter_set(one_set,inter_set);
					vector<t_set>::iterator p_set=inter_set.begin();
					vector<int> n_vect;
					while(p_set!=inter_set.end()){
						int n=get_set_id((*p_set));
						n_vect.push_back(n);
						p_set++;
					}
					//select the tooling----------------------------------------------------------------------
					//cout<<"select tooling"<<endl;
					vii p_tool=(*p_TS).tf.tools.begin();//test if there is enough tooling
					vector<int> record_n;
					while(p_tool!=(*p_TS).tf.tools.end()){
						int tf=(*p_tool);
						vii p_n=n_vect.begin();							
						while(p_n!=n_vect.end()){
							int n=(*p_n);
							if(tooling_used[tf][n]+a_st[s][tf]<=n_t_n_tooling[tf][n]){
								record_n.push_back(n);
								break;
							}
							p_n++;
						}
						p_tool++;
					}
					if((int)record_n.size()==(int)(*p_TS).tf.tools.size()){
						tooling_selected_flag=1;
					}
					if((int)(*p_TS).tf.tools.size()==1 && (*(*p_TS).tf.tools.begin())==0){
						//cout<<"no tooling required"<<endl;
						tooling_selected_flag=1;
					}
					//select a machine to open----------------------------------------------------------------------
					//cout<<"select a machine to open"<<endl;
					vii pmg=(*(Machine_Groups.begin()+mg-1)).machines.begin();
					while(pmg!=(*(Machine_Groups.begin()+mg-1)).machines.end() && tooling_selected_flag){
						int i=(*pmg);
						vector<Machine>::iterator pM=Machine_vector.begin()+i-1;
						vii pfind=find((*pM).temperature_vector.begin(),(*pM).temperature_vector.end(),temperature);
						vector<machine_heur_item>::iterator p_mach=Machine_Heur.begin()+i-1;
						if(pfind==(*pM).temperature_vector.end()||(int)(*p_mach).tooling.size()>0)
						{//the machine i can not run under the temperature
							pmg++;
							continue;
						}else{
							mach_selected=i;
							route_selected=s;
							break;
						}
						//pmg++;
					}
					if(mach_selected>0){//some machine is selected
						for(int i=1;i<=(int)record_n.size();i++){//assgin the tooling
							vii p_int=record_n.begin()+i-1;
							int n=(*p_int);
							p_int=(*p_TS).tf.tools.begin()+i-1;
							int tf=(*p_int);
							tooling_used[tf][n]+=a_st[s][tf];
						}
						break;
					}
					p_TS++;
				}
			}//endif
			//cout<<"4"<<endl;
			f_log<<"mach_selected="<<mach_selected<<endl;
			f_log<<"route_selected="<<route_selected<<endl;
			//assign the lot to mach_selected
			if(mach_selected>0){
				vector<machine_heur_item>::iterator p_mach=Machine_Heur.begin()+mach_selected-1;
				(*p_mach).time_used+=n_l_chips[(*p_lot)]/rate[route_selected];
				vector<tooling_configure>::iterator p_S=S.begin()+route_selected-1;
				(*p_mach).temperature=(*p_S).temperature;
				(*p_mach).tooling.resize((int)(*p_S).tf.tools.size());
				copy((*p_S).tf.tools.begin(),(*p_S).tf.tools.end(),(*p_mach).tooling.begin());
				vector<int> a;
				int s=(*p_S).id;
				vii pt=(*p_S).tf.tools.begin();
				while(pt!=(*p_S).tf.tools.end()){
					int tf=(*pt);
					a.push_back(a_st[s][tf]);
					pt++;
				}
				(*p_mach).b.resize((int)a.size());
				copy(a.begin(),a.end(),(*p_mach).b.begin());
				(*p_mach).lot.push_back(*p_lot);
				finished_dev_amount[dev]+=n_l_chips[(*p_lot)];
				lot_counter++;
				vector<lot_heur_item>::iterator p_find=Lot_Assign_Heur.begin()+(*p_lot)-1;
				(*p_find).i=mach_selected;
				(*p_find).s=route_selected;
			}else{
				f_log<<"discard lot "<<(*p_lot)<<endl;
			}
			vii p_find=find(lot_set_heur.begin(),lot_set_heur.end(),(*p_lot));
			lot_set_heur.erase(p_find);
			//cout<<"5"<<endl;
			if(lot_counter>=(int)lot_set_heur.size()*0.01 && lot_counter>=5){
				break_limit=1;
				break;
			}
			p_find=find(Packages.begin(),Packages.end(),dev);
			if(p_find!=Packages.end() && finished_dev_amount[dev]>=n_p_minchips[dev]){
				//cout<<"more than requried"<<endl;
				break_limit=1;
				break;
			}
			p_find=find(Key_Devices.begin(),Key_Devices.end(),dev);
			if(p_find!=Key_Devices.end() && finished_dev_amount[dev]>=n_k_minchips[dev]){
				break_limit=1;
				break;
			}
			p_lot++;
		}
		if(break_limit){
			f_log<<"lot_counter="<<lot_counter<<endl;
			break;
		}
		p_dev++;
	}
	//fout.close();
}

void AT::initialize_Machine_Heur(){
	Machine_Heur.clear();
	for(int i=1;i<=Num_Machine;i++){
		struct machine_heur_item one_item;
		one_item.time_used=0;
		one_item.temperature=-1;
		Machine_Heur.push_back(one_item);
	}
}

void AT::show_Machine_Heur(){
	ofstream fout("Debug/debug_Machine_Heur.txt");
	double total_weight=0;
	for(int d=1;d<=Num_Device;d++){
		package_qty[d]=0; keydevice_qty[d]=0;
	}
	double obj=0.0;
	for(int i=1;i<=Num_Machine;i++){
		vector<machine_heur_item>::iterator pm=Machine_Heur.begin()+i-1;
		fout<<"i="<<i<<endl;
		vii p_lot=(*pm).lot.begin();
		fout<<"assigned lots:";
		while(p_lot!=(*pm).lot.end()){
			fout<<(*p_lot)<<",";
			total_weight+=w_l[(*p_lot)];
			vector<lot_heur_item>::iterator pp=Lot_Assign_Heur.begin()+(*p_lot)-1;
			obj+=(w_l[(*p_lot)]-eps[(*pp).s]);
			int d=lots_device[(*p_lot)];
			vii p_find=find(Packages.begin(),Packages.end(),d);
			if(p_find!=Packages.end()){
				package_qty[d]+=n_l_chips[(*p_lot)];
			}
			p_find=find(Key_Devices.begin(),Key_Devices.end(),d);
			if(p_find!=Key_Devices.end()){
				keydevice_qty[d]+=n_l_chips[(*p_lot)];
			}
			p_lot++;
		}
		fout<<endl<<"installed tooling:";
		vii p_tool=(*pm).tooling.begin();
		while(p_tool!=(*pm).tooling.end()){
			fout<<(*p_tool)<<",";
			p_tool++;
		}
		fout<<endl<<"number of toolings ";
		vii pb=(*pm).b.begin();
		while(pb!=(*pm).b.end()){
			fout<<(*pb)<<",";
			pb++;
		}
		fout<<"temperature:"<<(*pm).temperature<<endl;
		fout<<"time used="<<(*pm).time_used<<", H[i]="<<H[i]<<endl;
	}
	fout<<"------------------------------------------------"<<endl;
	fout<<"total_weight="<<total_weight<<endl;
	for(int k=1;k<=Num_Keydevice;k++){
		int d=(*(Key_Devices.begin()+k-1));
		double shortage=n_k_minchips[d]-keydevice_qty[d];
		fout<<"d="<<d<<",required qty="<<n_k_minchips[d]<<",finished qty="<<keydevice_qty[d]<<",shortage="<<shortage<<endl;
		if(shortage>0){
			obj-=shortage*penalty[d]/coef;
		}
	}
	for(int p=1;p<=Num_Package;p++){
		int d=(*(Packages.begin()+p-1));
		double shortage=n_p_minchips[d]-package_qty[d];
		fout<<"d="<<d<<",required qty="<<n_p_minchips[d]<<",finished qty="<<package_qty[d]<<",shortage="<<shortage<<endl;
		if(shortage>0){
			obj-=shortage*penalty[d]/coef;
		}
	}
	fout<<"obj="<<obj<<endl;
	fout.close();
}

void AT::initial_soln(){
	//---------------------------------------------------------
	//lot_set_heur is initialized in build_model(), which lots should be processed
	vector<int> save_lot_set_heur;
	save_lot_set_heur.resize((int)lot_set_heur.size());
	copy(lot_set_heur.begin(),lot_set_heur.end(),save_lot_set_heur.begin());
	int ** tooling_used=new int *[Num_Tooling_Family+1];
	for(int i=0;i<=Num_Tooling_Family;i++){
		tooling_used[i]=new int[(int)Temperature_Set.size()+1];
	}
	for(int tf=1;tf<=Num_Tooling_Family;tf++){
		for(int i=1;i<=(int)Temperature_Set.size();i++){ tooling_used[tf][i]=0;}
	}
	int finished_dev_amount[Max_Num_Device+1];
	for(int i=1;i<=Num_Device;i++){ finished_dev_amount[i]=0;}
	initialize_Machine_Heur();
	initialize_Lot_Assign_Heur();
	//---------------------------------------------------------
	int amount_decrease=1;
	int iter_count=1;
	while(amount_decrease>0){
		f_log<<"iter_count="<<iter_count<<endl;
		int save_lot_set_heur_size=(int)lot_set_heur.size();
		f_log<<"sort"<<endl;
		sort_lot_heur(finished_dev_amount);
		f_log<<"assign"<<endl;
		assign_heur(tooling_used,finished_dev_amount);
		amount_decrease=save_lot_set_heur_size-(int)lot_set_heur.size();
		f_log<<"finished amount"<<endl;
		for(int i=1;i<=Num_Device;i++){
			f_log<<"d="<<i<<",finished ="<<finished_dev_amount[i]<<endl;
		}
		iter_count++;
	}
	lot_set_heur.resize((int)save_lot_set_heur.size());
	copy(save_lot_set_heur.begin(),save_lot_set_heur.end(),lot_set_heur.begin());
	show_Machine_Heur();
	show_Lot_Assign_Heur();
	double * soln=new double[y_limit+1];
	f_log<<"soln:"<<endl;
	for(int i=0;i<=y_limit;i++){ soln[i]=0.0;}
	build_soln(soln);
	//test_feasibility(soln);
	delete [] soln;
	//---------------------------------------------------------
	for(int i=0;i<=Num_Tooling_Family;i++){
		delete [] tooling_used[i];
	}
	delete [] tooling_used;
}

void AT::initialize_Lot_Assign_Heur(){
	for(int l=1;l<=Num_Lot;l++){
		struct lot_heur_item one_item;
		one_item.l=l;
		one_item.i=-1;
		one_item.s=-1;
		Lot_Assign_Heur.push_back(one_item);
	}
}

void AT::show_Lot_Assign_Heur(){
	ofstream fout("Debug/debug_Lot_Assign_Heur.txt");
	for(int l=1;l<=Num_Lot;l++){
		vector<lot_heur_item>::iterator p_lot=Lot_Assign_Heur.begin()+l-1;
		fout<<"lot="<<(*p_lot).l<<",i="<<(*p_lot).i<<",s="<<(*p_lot).s<<",eps="<<eps[(*p_lot).s]<<endl;
	}
	fout.close();
}

void AT::build_soln(double soln[]){
	//build soln vector from (1) Lot_Assign_Heur, (2) keydevice_qty, (3) package_qty, (4) Machine_Heur
	//1,filling x
	for(int l=1;l<=Num_Lot;l++){
		vector<lot_heur_item>::iterator p_l=Lot_Assign_Heur.begin()+l-1;
		int i=(*p_l).i;
		int s=(*p_l).s;
		if(i>0 && s>0){//the lot is processed
			int index=index2_x(i,l,s);
			soln[index]=1;
		}
	}
	//2,fill ing delta
	vii p_int=Key_Devices.begin();
	while(p_int!=Key_Devices.end()){
		int d=(*p_int);
		double shortage=n_k_minchips[d]-keydevice_qty[d];
		if(shortage>0){
			int index=p_int-Key_Devices.begin()+x_limit+1;
			soln[index]=shortage/coef;
		}
		p_int++;
	}
	p_int=Packages.begin();
	while(p_int!=Packages.end()){
		int d=(*p_int);
		double shortage=n_p_minchips[d]-package_qty[d];
		if(shortage>0){
			int index=p_int-Packages.begin()+x_limit+Num_Keydevice+1;
			soln[index]=shortage/coef;
		}
		p_int++;
	}
	//3,fill in y
	for(int i=1;i<=Num_Machine;i++){
		vector<machine_heur_item>::iterator pm=Machine_Heur.begin()+i-1;
		if((int)(*pm).tooling.size()>0){//"0" is also a tooling setup
			int Lamuda_id=get_Lamuda_id((*pm).tooling,(*pm).b,(*pm).temperature);
			int index=index2_y(i,Lamuda_id);
			soln[index]=1;
		}
	}
}

void AT::get_AT_Nums(int & Num_Tooling_copy,int & Num_Tooling_Family_copy,int & Num_Machine_copy,int & Num_Machine_Group_copy
				 ,int & Num_Lot_copy,int & Num_Device_copy,int & Num_Keydevice_copy,int & Num_Package_copy,int & Num_Temperature_copy
				 ,int & x_limit_copy,int & y_limit_copy){
	cout<<"in get_AT_Nums,Num_Tooling="<<Num_Tooling<<endl;
	Num_Tooling_copy=Num_Tooling;
	Num_Tooling_Family_copy=Num_Tooling_Family;
	Num_Machine_copy=Num_Machine;
	Num_Machine_Group_copy=Num_Machine_Group;
	Num_Lot_copy=Num_Lot;
	Num_Device_copy=Num_Device;
	Num_Keydevice_copy=Num_Keydevice;
	Num_Package_copy=Num_Package;
	Num_Temperature_copy=Num_Temperature;
	x_limit_copy=x_limit;
	y_limit_copy=y_limit;
}

void AT::get_AT_data(double * & H_copy,int * n_t_n_tooling_copy,int n_l_chips_copy[],int n_k_minchips_copy[],int n_p_minchips_copy[],double w_l_copy[],double rate_copy[]
				,double eps_copy[],int lots_device_copy[],int machine_mg_copy[],int tooling_tf_copy[]){

	cout<<"get H"<<endl;
	H_copy[0]=0;
	for(int i=1;i<=Num_Machine;i++){
		H_copy[i]=H[i];
		machine_mg_copy[i]=machine_mg[i];
	}

	cout<<"get n_t_n_tooling"<<endl;
	for(int tf=1;tf<=Num_Tooling_Family;tf++){
		for(int n=1;n<=(int)Temperature_Set.size();n++){
			int id=tf*(int)Temperature_Set.size()+n;
			n_t_n_tooling_copy[id]=n_t_n_tooling[tf][n];
		}
	}

	cout<<"get n_l_chips and lots_device"<<endl;
	for(int l=1;l<=Num_Lot;l++){
		n_l_chips_copy[l]=n_l_chips[l];
		lots_device_copy[l]=lots_device[l];
	}

	cout<<"get n_k_minchips"<<endl;
	for(int k=1;k<=Num_Keydevice;k++){
		n_k_minchips_copy[k]=n_k_minchips[k];
	}

	cout<<"get n_p_minchips"<<endl;
	for(int p=1;p<=Num_Package;p++){
		n_p_minchips_copy[p]=n_p_minchips[p];
	}

	cout<<"get w_l"<<endl;
	for(int l=1;l<=Num_Lot;l++){
		w_l_copy[l]=w_l[l];
	}

	cout<<"get rate and eps"<<endl;
	for(int s=1;s<=(int)S.size();s++){
		rate_copy[s]=rate[s];
		eps_copy[s] =eps[s];
	}

	cout<<"get tooling_tf"<<endl;
	for(int t=1;t<=Num_Tooling;t++){
		tooling_tf_copy[t]=tooling_tf[t];
	}
}

void AT::get_AT_vector(vector<tooling_configure> & S_copy,vector<tooling_set> & Tooling_Families_copy
				,vector<Tooling> & Tooling_vector_copy,vector<Machine> & Machine_vector_copy,vector<machine_set> & Machine_Groups_copy,vector<int> & Packages_copy
				,vector<int> & Key_Devices_copy,vector<device_item> & Devices_copy,vector<index_node> & index_vector_copy,vector<t_set> & Temperature_Set_copy,vec_int L_i_copy[]
				,vec_int M_l_copy[],vec_tc S_i_copy[],vector<ch_array> & device_name_copy,vector<ch_array> & lot_name_copy,vector<Lamuda_item> & Lamuda_copy,vec_int Lamuda_i_copy[]
				,vec_int M_lamuda_copy[]){
	cout<<"get S"<<endl;
	S_copy.clear();
	vector<tooling_configure>::iterator p_S=S.begin();
	while(p_S!=S.end()){
		S_copy.push_back(*p_S);
		p_S++;
	}
	cout<<"in AT_CPLEX, S_copy size="<<S_copy.size()<<endl;

	cout<<"get Tooling_Families"<<endl;
	vector<tooling_set>::iterator p_TF=Tooling_Families.begin();
	while(p_TF!=Tooling_Families.end()){
		Tooling_Families_copy.push_back(*p_TF);
		p_TF++;
	}
	cout<<"get Tooling_vector"<<endl;
	vector<Tooling>::iterator p_T=Tooling_vector.begin();
	while(p_T!=Tooling_vector.end()){
		Tooling_vector_copy.push_back(*p_T);
		p_T++;
	}
	cout<<"get Machine_vector"<<endl;
	vector<Machine>::iterator p_M=Machine_vector.begin();
	while(p_M!=Machine_vector.end()){
		Machine_vector_copy.push_back(*p_M);
		p_M++;
	}
	cout<<"get Machine_Groups"<<endl;
	vector<machine_set>::iterator p_MG=Machine_Groups.begin();
	while(p_MG!=Machine_Groups.end()){
		Machine_Groups_copy.push_back(*p_MG);
		p_MG++;
	}
	cout<<"get Packages"<<endl;
	vii p_int=Packages.begin();
	while(p_int!=Packages.end()){
		Packages_copy.push_back(*p_int);
		p_int++;
	}
	cout<<"get Key_Devices"<<endl;
	p_int=Key_Devices.begin();
	while(p_int!=Key_Devices.end()){
		Key_Devices_copy.push_back(*p_int);
		p_int++;
	}
	cout<<"get Devices"<<endl;
	vector<device_item>::iterator p_D=Devices.begin();
	while(p_D!=Devices.end()){
		Devices_copy.push_back(*p_D);
		p_D++;
	}
	cout<<"get index_vector"<<endl;
	vector<index_node>::iterator p_index=index_vector.begin();
	while(p_index!=index_vector.end()){
		index_vector_copy.push_back(*p_index);
		p_index++;
	}
	cout<<"get Temperature_Set"<<endl;
	vector<t_set>::iterator p_set=Temperature_Set.begin();
	while(p_set!=Temperature_Set.end()){
		Temperature_Set_copy.push_back(*p_set);
		p_set++;
	}
	cout<<"get L_i"<<endl;
	for(int i=1;i<=Num_Machine;i++){
		vii p_int=L_i[i].int_vector.begin();
		while(p_int!=L_i[i].int_vector.end()){
			L_i_copy[i].int_vector.push_back(*p_int);
			p_int++;
		}
	}
	cout<<"get M_l"<<endl;
	for(int l=1;l<=Num_Lot;l++){
		vii p_int=M_l[l].int_vector.begin();
		while(p_int!=M_l[l].int_vector.end()){
			M_l_copy[l].int_vector.push_back(*p_int);
			p_int++;
		}
	}
	cout<<"get S_i"<<endl;
	for(int i=1;i<=Num_Machine;i++){
		vector<tooling_configure>::iterator p_tc=S_i[i].tc_vector.begin();
		while(p_tc!=S_i[i].tc_vector.end()){
			S_i_copy[i].tc_vector.push_back(*p_tc);
			p_tc++;
		}
	}
	cout<<"get device_name"<<endl;
	vector<ch_array>::iterator p_name=device_name.begin();
	while(p_name!=device_name.end()){
		device_name_copy.push_back(*p_name);
		p_name++;
	}
	cout<<"get lot_name"<<endl;
	p_name=lot_name.begin();
	while(p_name!=lot_name.end()){
		lot_name_copy.push_back(*p_name);
		p_name++;
	}
	cout<<"get Lamuda"<<endl;
	vector<Lamuda_item>::iterator p_L=Lamuda.begin();
	while(p_L!=Lamuda.end()){
		Lamuda_copy.push_back(*p_L);
		p_L++;
	}
	cout<<"get Lamuda_i"<<endl;
	for(int i=1;i<=Num_Machine;i++){
		vii p_int=Lamuda_i[i].int_vector.begin();
		while(p_int!=Lamuda_i[i].int_vector.end()){
			Lamuda_i_copy[i].int_vector.push_back(*p_int);
			p_int++;
		}
	}
	cout<<"get M_lamuda"<<endl;
	for(int la=1;la<=(int)Lamuda.size();la++){
		vii p_int=M_lamuda[la].int_vector.begin();
		while(p_int!=M_lamuda[la].int_vector.end()){
			M_lamuda_copy[la].int_vector.push_back(*p_int);
			p_int++;
		}
	}				
}

bool AT::compare_tc(const tooling_configure & a,const tooling_configure & b){
	return (eps[a.id]<eps[b.id]);
}

void AT::pack(BCP_buffer & buf) const{
	cout<<"int AT pack()"<<endl;
	buf.pack(Num_Tooling);
	buf.pack(Num_Tooling_Family);
	buf.pack(Num_Machine);
	buf.pack(Num_Machine_Group);
	buf.pack(Num_Lot);
	buf.pack(Num_Device);
	buf.pack(Num_Keydevice);
	buf.pack(Num_Package);
	buf.pack(Num_Temperature);
	buf.pack(x_limit);
	buf.pack(y_limit);
	buf.pack(SIM_option);

	buf.pack(H,Num_Machine+1);
	int * int_array=new int[Num_Tooling_Family*Max_Num_Temperature];
	for(int tf=1;tf<=Num_Tooling_Family;tf++){
		for(int n=1;n<=Max_Num_Temperature-1;n++){
			int id=(tf-1)*Max_Num_Temperature+n-1;
			int_array[id]=n_t_n_tooling[tf][n];
			//if(n==1){ cout<<"tf="<<tf<<",n="<<n<<",id="<<id<<",n_t_n_tooling="<<n_t_n_tooling[tf][n]<<endl;}
		}
	}
	buf.pack(int_array,Num_Tooling_Family*Max_Num_Temperature);
	delete [] int_array;
	buf.pack(n_l_chips,Num_Lot+1);

	buf.pack(Key_Devices);
	buf.pack(Packages);
	buf.pack(n_k_minchips,Max_Num_Device+1);
	buf.pack(n_p_minchips,Max_Num_Device+1);

	buf.pack(w_l,Num_Lot+1);
	buf.pack(rate,Max_Num_Tooling_Configure);
	buf.pack(eps,Max_Num_Tooling_Configure);
	buf.pack(lots_device,Num_Lot+1);
	buf.pack(machine_mg,Num_Machine+1);
	buf.pack(machine_sim,Num_Machine+1);
	buf.pack(tooling_tf,Num_Tooling+1);
	buf.pack(L_i,Num_Machine+1);
	buf.pack(M_l,Num_Lot+1);
	buf.pack(S_i,Num_Machine+1);
	buf.pack(Lamuda_i,Num_Machine+1);
	buf.pack(M_lamuda,Max_Num_Tooling_Configure);

	buf.pack(S);
	buf.pack(Tooling_Families);
	buf.pack(Tooling_vector);
	buf.pack(Machine_vector);
	buf.pack(Machine_Groups);
	buf.pack(Devices);
	buf.pack(index_vector);
	buf.pack(Temperature_Set);
	buf.pack(device_name);
	buf.pack(lot_name);
	buf.pack(Lamuda);

	int_array=new int[(int)S.size() * (Num_Tooling_Family+1)];
	for(int s=1;s<=(int)S.size();s++){
		for(int tf=0;tf<=Num_Tooling_Family;tf++){//need to start from 0 for a_st[][], tf could be zero !!!
			int id=(s-1)*(Num_Tooling_Family+1)+tf;
			int_array[id]=a_st[s][tf];
		}
	}
	buf.pack(int_array,(int)S.size() * (Num_Tooling_Family+1));
	delete [] int_array;

	buf.pack(SIM);
	buf.pack(Machine_Heur);
	buf.pack(Lot_Assign_Heur);

	buf.pack(tooling_sum_rhs);
	buf.pack(L_sim_lamuda);
	buf.pack(sim_lamuda_L);
	buf.pack(S_sim_d);         //S(i,l)
	buf.pack(S_sim_d_lamuda);  //S(i,l,lamuda)
	buf.pack(elite_soln);
	buf.pack(coef);
	buf.pack(sum_weight);
}

void AT::unpack(BCP_buffer & buf){
	cout<<"unpack numbers"<<endl;
	buf.unpack(Num_Tooling);
	cout<<"Num_Tooling="<<Num_Tooling<<endl;
	buf.unpack(Num_Tooling_Family);
	buf.unpack(Num_Machine);
	buf.unpack(Num_Machine_Group);
	buf.unpack(Num_Lot);
	buf.unpack(Num_Device);
	buf.unpack(Num_Keydevice);
	buf.unpack(Num_Package);
	buf.unpack(Num_Temperature);
	buf.unpack(x_limit);
	buf.unpack(y_limit);
	cout<<"y_limit="<<y_limit<<endl;
	buf.unpack(SIM_option);

	cout<<"unpack H[]"<<endl;
	int num_cols=Num_Machine+1;
	double * H_cp=new double[num_cols];
	buf.unpack(H_cp,num_cols,false);
	copy(H_cp,H_cp+num_cols,H);
	//for(int i=1;i<=Num_Machine;i++){ cout<<"i="<<i<<",H="<<H[i]<<endl;}
	delete [] H_cp;

	cout<<"unpack n_t_n_tooling[][]"<<endl;
	num_cols=Num_Tooling_Family*Max_Num_Temperature;
	int * int_array=new int[num_cols];
	buf.unpack(int_array,num_cols,false);
	for(int tf=1;tf<=Num_Tooling_Family;tf++){
		for(int n=1;n<=Max_Num_Temperature-1;n++){
			int id=(tf-1)*Max_Num_Temperature+n-1;
			n_t_n_tooling[tf][n]=int_array[id];
			//if(n==1){cout<<"tf="<<tf<<",n="<<n<<",id="<<id<<",n_t_n_tooling="<<n_t_n_tooling[tf][n]<<endl;}
		}
	}
	delete [] int_array;

	cout<<"unpack n_l_chips[]"<<endl;
	num_cols=Num_Lot+1;
	int_array=new int[num_cols];
	buf.unpack(int_array,num_cols,false);
	copy(int_array,int_array+num_cols,n_l_chips);
	//for(int i=1;i<=Num_Lot;i++){ cout<<"i="<<i<<",n_l_chips="<<n_l_chips[i]<<endl;}
	delete [] int_array;

	cout<<"unpack Key_Devices"<<endl;
	buf.unpack(Key_Devices);
	cout<<"size="<<Key_Devices.size()<<endl;
	cout<<"unpack Packages"<<endl;
	buf.unpack(Packages);
	cout<<"size="<<Packages.size()<<endl;

	cout<<"unpack n_k_minchips[]"<<endl;
	num_cols=Max_Num_Device+1;
	int_array=new int[num_cols];
	buf.unpack(int_array,num_cols,false);
	copy(int_array,int_array+num_cols,n_k_minchips);
	//for(int k=1;k<=Num_Keydevice;k++){ 
	//	vii pk=Key_Devices.begin()+k-1;
	//	int dev=(*pk);
	//	cout<<"k="<<k<<",dev="<<dev<<",n_k_minchips="<<n_k_minchips[dev]<<endl;
	//}
	delete [] int_array;

	cout<<"unpack n_p_minchips[]"<<endl;
	num_cols=Max_Num_Device+1;
	int_array=new int[num_cols];
	buf.unpack(int_array,num_cols,false);
	copy(int_array,int_array+num_cols,n_p_minchips);
	//for(int p=1;p<=Num_Package;p++){ 
	//	vii pp=Packages.begin()+p-1;
	//	int dev=(*pp);
	//	cout<<"p="<<p<<",dev="<<dev<<",n_p_minchips="<<n_p_minchips[dev]<<endl;
	//}
	delete [] int_array;

	num_cols=Num_Lot+1;
	double * w_l_cp=new double[num_cols];
	buf.unpack(w_l_cp,num_cols,false);
	copy(w_l_cp,w_l_cp+num_cols,w_l);
	//for(int l=1;l<=Num_Lot;l++){ cout<<"l="<<l<<",w_l="<<w_l[l]<<endl;}
	delete [] w_l_cp;

	cout<<"unpack rate[]"<<endl;
	num_cols=Max_Num_Tooling_Configure;
	double * rate_cp=new double[num_cols];
	buf.unpack(rate_cp,num_cols,false);
	copy(rate_cp,rate_cp+num_cols,rate);
	//for(int s=1;s<=120;s++){ cout<<"s="<<s<<",rate="<<rate[s]<<endl;}
	delete [] rate_cp;

	cout<<"unpack eps[]"<<endl;
	double * eps_cp=new double[num_cols];
	buf.unpack(eps_cp,num_cols,false);
	copy(eps_cp,eps_cp+num_cols,eps);
	//for(int s=1;s<=120;s++){ cout<<"s="<<s<<",eps="<<eps[s]<<endl;}
	delete [] eps_cp;

	cout<<"unpack lots_device[]"<<endl;
	num_cols=Num_Lot+1;
	int_array=new int[num_cols];
	buf.unpack(int_array,num_cols,false);
	copy(int_array,int_array+num_cols,lots_device);
	//for(int l=1;l<=Num_Lot;l++){ cout<<"l="<<l<<",lots_device="<<lots_device[l]<<endl;}
	delete [] int_array;

	cout<<"unpack machine_mg[]"<<endl;
	num_cols=Num_Machine+1;
	int_array=new int[num_cols];
	buf.unpack(int_array,num_cols,false);
	copy(int_array,int_array+num_cols,machine_mg);
	//for(int i=1;i<=Num_Machine;i++){ cout<<"i="<<i<<",machine_mg="<<machine_mg[i]<<endl;}
	delete [] int_array;

	cout<<"unpack machine_sim[]"<<endl;
	num_cols=Num_Machine+1;
	int_array=new int[num_cols];
	buf.unpack(int_array,num_cols,false);
	copy(int_array,int_array+num_cols,machine_sim);
	delete [] int_array;

	cout<<"unpack tooling_tf"<<endl;
	num_cols=Num_Tooling+1;
	int_array=new int[num_cols];
	buf.unpack(int_array,num_cols,false);
	copy(int_array,int_array+num_cols,tooling_tf);
	//for(int t=1;t<=Num_Tooling;t++){ cout<<"t="<<t<<",tooling_tf="<<tooling_tf[t]<<endl;}
	delete [] int_array;

	cout<<"unpack L_i"<<endl;
	num_cols=Num_Machine+1;
	vec_int * vec_array=new vec_int[num_cols];
	buf.unpack(vec_array,num_cols,false);
	copy(vec_array,vec_array+num_cols,L_i);
	//show_L_i();
	delete [] vec_array;

	cout<<"unpack M_l"<<endl;
	num_cols=Num_Lot+1;
	vec_array=new vec_int[num_cols];
	buf.unpack(vec_array,num_cols,false);
	copy(vec_array,vec_array+num_cols,M_l);
	//show_M_l();
	delete [] vec_array;

	cout<<"unpack S_i"<<endl;
	num_cols=Num_Machine+1;
	vec_tc *vec_tc_array=new vec_tc[num_cols];
	buf.unpack(vec_tc_array,num_cols,false);
	copy(vec_tc_array,vec_tc_array+num_cols,S_i);
	//show_S_i();
	delete [] vec_tc_array;

	cout<<"unpack Lamuda_i"<<endl;
	vec_array=new vec_int[num_cols];
	buf.unpack(vec_array,num_cols,false);
	copy(vec_array,vec_array+num_cols,Lamuda_i);
	//show_Lamuda_i();
	delete [] vec_array;

	cout<<"unpack M_lamuda"<<endl;
	num_cols=Max_Num_Tooling_Configure;
	vec_array=new vec_int[num_cols];
	buf.unpack(vec_array,num_cols,false);
	copy(vec_array,vec_array+num_cols,M_lamuda);
	delete [] vec_array;
	
	cout<<"unpack S"<<endl;
	buf.unpack(S);
	//show_S();

	cout<<"unpack Tooling_Families"<<endl;
	buf.unpack(Tooling_Families);

	cout<<"unpack Tooling_vector"<<endl;
	buf.unpack(Tooling_vector);
	//show_Tooling_vector();

	cout<<"unpack Machine_vector"<<endl;
	buf.unpack(Machine_vector);
	//show_Machine_vector();

	cout<<"unpack Machine_Groups"<<endl;
	buf.unpack(Machine_Groups);

	cout<<"unpack Devices"<<endl;
	buf.unpack(Devices);
	//show_Devices();

	cout<<"unpack index_vector"<<endl;
	buf.unpack(index_vector);
	//show_index_vector();

	cout<<"unpack Temperature_Set"<<endl;
	buf.unpack(Temperature_Set);

	cout<<"unpack device_name"<<endl;
	buf.unpack(device_name);
	//show_device_name();

	cout<<"unpack lot_name"<<endl;
	buf.unpack(lot_name);

	cout<<"unpack Lamuda vector"<<endl;
	buf.unpack(Lamuda);
	//show_Lamuda();

	cout<<"unpack a_st"<<endl;
	num_cols=(int)S.size() * (Num_Tooling_Family+1);
	int_array=new int[num_cols];
	buf.unpack(int_array,num_cols,false);
	for(int s=1;s<=(int)S.size();s++){
		for(int tf=0;tf<=Num_Tooling_Family;tf++){//need to start from 0 for a_st[][], tf could be zero !!!
			int id=(s-1)*(Num_Tooling_Family+1)+tf;
			a_st[s][tf]=int_array[id];
		}
	}
	//cout<<"s=62,tf=0,a_st="<<a_st[62][0]<<endl;
	delete [] int_array;

	cout<<"unpack SIM"<<endl;
	buf.unpack(SIM);
	cout<<"unpack Machine_Heur"<<endl;
	buf.unpack(Machine_Heur);
	cout<<"unpack Lot_Assign_Heur"<<endl;
	buf.unpack(Lot_Assign_Heur);

	cout<<"unpack tooling_sum_rhs"<<endl;
	buf.unpack(tooling_sum_rhs);
	cout<<"unpack L_sim_lamuda"<<endl;
	buf.unpack(L_sim_lamuda);
	cout<<"unpack sim_lamuda_L"<<endl;
	buf.unpack(sim_lamuda_L);
	cout<<"unpack S_sim_d"<<endl;
	buf.unpack(S_sim_d);         //S(i,l)
	cout<<"unpack S_sim_d_lamuda"<<endl;
	buf.unpack(S_sim_d_lamuda);  //S(i,l,lamuda)
	cout<<"unpack elite_soln"<<endl;
	buf.unpack(elite_soln);
	cout<<"unpack coef"<<endl;
	buf.unpack(coef);
	cout<<"unpack sum_wieght"<<endl;
	buf.unpack(sum_weight);
}

AT::AT(BCP_buffer &buf){
	unpack(buf);
}

void AT::get_SIM(){
	//this function is used to group the machines into SIM(set of identical machines)
	//identical machine: (1)in the same machine family (2)have the same list of operating temperatures
	vector<Machine>::iterator pM=Machine_vector.begin();
	while(pM!=Machine_vector.end()){
		int identical_flag=0; //not identical
		if((int)SIM.size()<1){//nothing in SIM yet
			struct SIM_item one_item;
			one_item.simid=1;
			one_item.represented_machine_id=(*pM).id;
			one_item.mg=(*pM).mg;
			copy((*pM).name,(*pM).name+50,one_item.name);
			one_item.temperature_vector.resize((int)(*pM).temperature_vector.size());
			copy((*pM).temperature_vector.begin(),(*pM).temperature_vector.end(),one_item.temperature_vector.begin());
			one_item.num_machine=1;
			machine_sim[(*pM).id]=one_item.simid;
			SIM.push_back(one_item);
		}
		else{
			vector<SIM_item>::iterator pSIM=SIM.begin();
			while(pSIM!=SIM.end()){
				if((*pSIM).mg==(*pM).mg && compare_int_vec((*pSIM).temperature_vector,(*pM).temperature_vector)){
					//(*pSIM) is identical to (*pM)
					(*pSIM).num_machine++;
					machine_sim[(*pM).id]=(*pSIM).simid;
					identical_flag=1;
					break;
				}
				pSIM++;
			}
			if(!identical_flag){
				struct SIM_item one_item;
				one_item.simid=(int)SIM.size()+1;
				one_item.represented_machine_id=(*pM).id;
				one_item.mg=(*pM).mg;
				copy((*pM).name,(*pM).name+50,one_item.name);
				one_item.temperature_vector.resize((int)(*pM).temperature_vector.size());
				copy((*pM).temperature_vector.begin(),(*pM).temperature_vector.end(),one_item.temperature_vector.begin());
				one_item.num_machine=1;
				machine_sim[(*pM).id]=one_item.simid;
				SIM.push_back(one_item);
			}
		}
		//cout<<"identical_flag="<<identical_flag<<endl;
		pM++;
	}
}

void AT::show_SIM(){
	ofstream fSIM("Debug/debugSIM.txt");
	vector<SIM_item>::iterator pSIM=SIM.begin();
	while(pSIM!=SIM.end()){
		fSIM<<"simid="<<(*pSIM).simid<<",mg="<<(*pSIM).mg<<",represented_id="<<(*pSIM).represented_machine_id
			<<",name="<<(*pSIM).name<<",num_machine="<<(*pSIM).num_machine<<endl;
		fSIM<<"operating temperatures: "<<endl;
		vii pt=(*pSIM).temperature_vector.begin();
		while(pt!=(*pSIM).temperature_vector.end()){
			fSIM<<(*pt)<<",";
			pt++;
		}
		fSIM<<endl;
		for(int i=1;i<=Num_Machine;i++){
			if(machine_sim[i]==(*pSIM).simid){ 
				fSIM<<i<<",";
			}
		}
		fSIM<<endl;
		pSIM++;
	}
	fSIM.close();
}

void AT::show_machine_sim(){
	ofstream fout("Debug/debug_machine_sim.txt");
	for(int i=1;i<=Num_Machine;i++){
		fout<<"i="<<i<<",sim="<<machine_sim[i]<<endl;
	}
	fout.close();
}

void AT::get_LPD(){
	LPD.clear();
	vii p_int=Key_Devices.begin();
	while(p_int!=Key_Devices.end()){
		int d=(*p_int);
		//cout<<"d="<<d<<endl;
		vector<device_item>::iterator p_dev=Devices.begin()+d-1;
		vii p_lot=(*p_dev).device_lots.begin();
		while(p_lot!=(*p_dev).device_lots.end()){
			LPD.push_back(*p_lot);
			p_lot++;
		}
		p_int++;
	}
	p_int=Packages.begin();
	while(p_int!=Packages.end()){
		int d=(*p_int);
		//cout<<"d="<<d<<endl;
		vector<device_item>::iterator p_dev=Devices.begin()+d-1;
		vii p_lot=(*p_dev).device_lots.begin();
		while(p_lot!=(*p_dev).device_lots.end()){
			LPD.push_back(*p_lot);
			p_lot++;
		}
		p_int++;
	}
	ofstream fout("Debug/debug_LPD.txt");
	p_int=LPD.begin();
	while(p_int!=LPD.end()){
		fout<<(*p_int)<<endl;
		p_int++;
	}
	fout.close();
}

#if 0
void AT::build_first_level_model(IloEnv & env,IloModel & model,vector<IloNumVar> & x_vector,vector<IloNumVar> & y_vector
								 ,vector<IloNumVar> & deltaPD_vector,vector<IloNumVar> & deltaKD_vector){
	IloRangeArray con(env);
	int num_con=0;
	int num_var=0;
	
	//initialize x variables
	for(int i=1;i<=Num_Machine;i++){
		vii p_L=L_i[i].int_vector.begin();
		while(p_L!=L_i[i].int_vector.end()){
			vii pfind=find(LPD.begin(),LPD.end(),(*p_L));
			if(pfind!=LPD.end()){
				int l=(*p_L);
				int sim=machine_sim[i];
				int d=lots_device[l];
				int index=index_S_sim_d(sim,d);
				vii p_s=S_sim_d[index].second.int_vector.begin();
				while(p_s!=S_sim_d[index].second.int_vector.end()){
					int s=(*p_s);
					string str="x";
					stringstream ss;
					ss<<"("<<i<<")("<<l<<")("<<s<<")";
					str+=ss.str();
					char name[20];
					strcpy(name,str.c_str());
					x_vector.push_back(IloNumVar(env,0.0,1.0,name));
					num_var++;
					p_s++;
				}
			}
			p_L++;
		}
	}
	//initialize y variables
	for(int i=1;i<=Num_Machine;i++){
		vii p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			string str="y";
			stringstream ss;
			ss<<"("<<i<<")("<<Lamuda_id<<")";
			str+=ss.str();
			char name[20];
			strcpy(name,str.c_str());
			y_vector.push_back(IloNumVar(env,0.0,1.0,name));
			num_var++;
			p_Lamuda++;
		}
	}
	//initalize delta variables
	for(int k=1;k<=Num_Keydevice;k++){
		string str="deltaKD";
		stringstream ss;
		ss<<"("<<k<<")";
		str+=ss.str();
		char name[20];
		strcpy(name,str.c_str());
		deltaKD_vector.push_back(IloNumVar(env,0,IloInfinity,name));
		num_var++;
	}
	for(int k=1;k<=Num_Package;k++){
		string str="deltaPD";
		stringstream ss;
		ss<<"("<<k<<")";
		str+=ss.str();
		char name[20];
		strcpy(name,str.c_str());
		deltaPD_vector.push_back(IloNumVar(env,0,IloInfinity,name));
		num_var++;
	}

	cout<<"build obj"<<endl;
	IloExpr expr(env);
	for(int i=1;i<=Num_Machine;i++){
		vector<int>::iterator p_L=L_i[i].int_vector.begin();
		while(p_L!=L_i[i].int_vector.end()){
			vii pfind=find(LPD.begin(),LPD.end(),(*p_L));
			if(pfind!=LPD.end()){
				int l=(*p_L);
				int sim=machine_sim[i];
				int d=lots_device[l];
				int index=index_S_sim_d(sim,d);
				vii p_s=S_sim_d[index].second.int_vector.begin();
				while(p_s!=S_sim_d[index].second.int_vector.end()){
					int s=(*p_s);
					vector<IloNumVar>::iterator p_x=x_vector.begin()+index3_x(i,l,s);
					expr+=(-w_l[l]+eps[s])*(* p_x);
					p_s++;
				}
			}
			p_L++;
		}
	}
	for(int k=1;k<=Num_Keydevice;k++){
		int d=Key_Devices[k-1];
		vector<IloNumVar>::iterator p_deltaKD=deltaKD_vector.begin()+k-1;
		expr+=penalty[d]*(* p_deltaKD);
	}
	for(int p=1;p<=Num_Package;p++){
		int d=Packages[p-1];
		vector<IloNumVar>::iterator p_deltaPD=deltaPD_vector.begin()+p-1;
		expr+=penalty[d]*(* p_deltaPD);
	}
	/*
	for(int i=1;i<=Num_Machine;i++){
		vii p_L=Lamuda_i[i].int_vector.begin();
		while(p_L!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(*p_L);
			vector<IloNumVar>::iterator p_y=y_vector.begin()+index3_y(i,Lamuda_id)-x_limit_first_level-Num_Keydevice-Num_Package-1;
			expr+=(*p_y);
			p_L++;
		}
	}
	*/
	IloObjective obj(env,expr,IloObjective::Minimize);
	model.add(obj);
	expr.end();

	cout<<"build constraints (1b)"<<endl;
	for(vii p_lot=LPD.begin();p_lot!=LPD.end();p_lot++){
		int l=(*p_lot);
		IloExpr expr(env);
		vector<int>::iterator p_M=M_l[l].int_vector.begin();
		while(p_M!=M_l[l].int_vector.end()){
			int i=(*p_M);
			int sim=machine_sim[i];
			int d=lots_device[l];
			int index=index_S_sim_d(sim,d);
			vii p_s=S_sim_d[index].second.int_vector.begin();
			while(p_s!=S_sim_d[index].second.int_vector.end()){
				int s=(*p_s);
				vector<IloNumVar>::iterator p_x= x_vector.begin()+index3_x(i,l,s);
				expr+=(*p_x);
				p_s++;
			}
			p_M++;
		}
		con.add(expr<=1.0);
		expr.end();
		num_con++;
		string str="(";
		stringstream ss;
		ss<<"l@"<<l<<")";
		char name[100];
		str+=ss.str();
		strcpy(name,str.c_str());
		con[num_con-1].setName(name);
		//cout<<"l="<<l<<endl;
	}

	cout<<"build constraints (1c)"<<endl;
	for(int i=1;i<=Num_Machine;i++){
		vector<int>::iterator p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			vector<int>::iterator p_L=L_i[i].int_vector.begin();
			while(p_L!=L_i[i].int_vector.end()){
				vii pfind=find(LPD.begin(),LPD.end(),(*p_L));
				if(pfind!=LPD.end()){
					int l=(*p_L);
					vector<tooling_configure> TS;
					get_S_i_l_Lamuda(TS,i,l,Lamuda_id);
					vector<tooling_configure>::iterator p_TS=TS.begin();
					while(p_TS!=TS.end()){
						IloExpr expr(env);
						int s=(*p_TS).id;
						vector<IloNumVar>::iterator p_x=x_vector.begin()+index3_x(i,l,s);
						vector<IloNumVar>::iterator p_y=y_vector.begin()+index3_y(i,Lamuda_id)-x_limit_first_level-Num_Keydevice-Num_Package-1;
						expr=(*p_y)-(*p_x);
						con.add(expr>=0);
						expr.end();
						num_con++;
						string str="(";
						stringstream ss;
						ss<<"i@"<<i<<",Lamuda@"<<Lamuda_id<<",l@"<<l<<",s@"<<s<<")";
						str+=ss.str();
						char name[100];
						strcpy(name,str.c_str());
						con[num_con-1].setName(name);
						p_TS++;
					}
				}
				p_L++;
			}
			p_Lamuda++;
		}
	}
	/*
	for(int i=1;i<=Num_Machine;i++){
		vector<int>::iterator p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			int counter=0;
			IloExpr expr(env);
			vector<IloNumVar>::iterator p_y=y_vector.begin()+index3_y(i,Lamuda_id)-x_limit_first_level-Num_Keydevice-Num_Package-1;
			vector<int>::iterator p_L=L_i[i].int_vector.begin();
			while(p_L!=L_i[i].int_vector.end()){
				int l=(*p_L);
				vector<tooling_configure> TS;
				get_S_i_l_Lamuda(TS,i,l,Lamuda_id);
				vector<tooling_configure>::iterator p_TS=TS.begin();
				while(p_TS!=TS.end()){
					int s=(*p_TS).id;
					vector<IloNumVar>::iterator p_x=x_vector.begin()+index3_x(i,l,s);
					expr-=(*p_x);
					counter++;
					p_TS++;
				}
				p_L++;
			}
			expr+=(*p_y)*counter;
			con.add(expr>=0);
			expr.end();
			num_con++;
			string str="(";
			stringstream ss;
			ss<<"i@"<<i<<",Lamuda@"<<Lamuda_id<<")";
			str+=ss.str();
			char name[100];
			strcpy(name,str.c_str());
			con[num_con-1].setName(name);
			p_Lamuda++;
		}
	}
	*/

	cout<<"build constraints (1d)"<<endl;
	for(int i=1;i<=Num_Machine;i++){
		IloExpr expr(env);
		vector<int>::iterator p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(* p_Lamuda);
			vector<IloNumVar>::iterator p_y=y_vector.begin()+index3_y(i,Lamuda_id)-x_limit_first_level-Num_Keydevice-Num_Package-1;
			expr+=(*p_y);
			p_Lamuda++;
		}
		con.add(expr<=1);
		expr.end();
		num_con++;
		string str="(";
		stringstream ss;
		ss<<"(y)i@"<<i<<")";
		str+=ss.str();
		char name[100];
		strcpy(name,str.c_str());
		con[num_con-1].setName(name);
	}
	/*
	cout<<"build constraints (1e)"<<endl;
	for(int tf=1;tf<=Num_Tooling_Family;tf++){
		for(int n=1;n<=(int)Temperature_Set.size();n++){
			vector<t_set>::iterator p_set=Temperature_Set.begin()+n-1;
			IloExpr expr(env);
			for(int i=1;i<=Num_Machine;i++){
				//cout<<"i="<<i<<", L_i[i] size="<<L_i[i].int_vector.size()<<endl;
				vector<int>::iterator p_L=L_i[i].int_vector.begin();
				while(p_L!=L_i[i].int_vector.end()){
					int l=(*p_L);		
					vii p_int=(*p_set).set_elements.begin();
					while(p_int!=(*p_set).set_elements.end()){
						int tau=(*p_int);
						vector<tooling_configure> TS;
						get_S(TS,i,l,tf,tau);
						vector<tooling_configure>::iterator p_TS=TS.begin();
						while(p_TS!=TS.end()){
							int s=(*p_TS).id;
							//vector<int>::iterator p_int=find((*p_TS).tf.tools.begin(),(*p_TS).tf.tools.end(),tf);
							if(a_st[s][tf]>=1){
								vector<IloNumVar>::iterator p_x=x_vector.begin()+index3_x(i,l,s);
								expr+=a_st[s][tf]*(*p_x);
							}
							p_TS++;
						}
						p_int++;
					}
					p_L++;
				}
			}
			vector<t_set> inter_set;
			get_inter_set((*p_set),inter_set);
			vector<t_set>::iterator pm=inter_set.begin();
			int sum=0;
			while(pm!=inter_set.end()){
				int set_id=get_set_id(*pm);
				sum+=n_t_n_tooling[tf][set_id];
				pm++;
			}
			con.add(expr<=sum);
			num_con++;
			expr.end();
			string str="(";
			stringstream ss;
			ss<<"tf@"<<tf<<",n@"<<n<<")";
			str+=ss.str();
			char name[100];
			strcpy(name,str.c_str());
			con[num_con-1].setName(name);
			//cout<<"tf="<<tf<<",n="<<n<<endl;
		}
	}*/
	cout<<"build constraints (1e)"<<endl;
	for(int tf=1;tf<=Num_Tooling_Family;tf++){
		for(int n=1;n<=(int)Temperature_Set.size();n++){
			vector<t_set>::iterator p_set=Temperature_Set.begin()+n-1;
			IloExpr expr(env);
			for(int i=1;i<=Num_Machine;i++){
				vii p_int=(*p_set).set_elements.begin();
				while(p_int!=(*p_set).set_elements.end()){
					int tau=(*p_int);
					vector<int> Lamuda_i_tf_tau;
					get_Lamuda(Lamuda_i_tf_tau,i,tf,tau);
					vii p_Lamuda=Lamuda_i_tf_tau.begin();
					while(p_Lamuda!=Lamuda_i_tf_tau.end()){
						int Lamuda_id=(*p_Lamuda);
						vector<Lamuda_item>::iterator pL=Lamuda.begin()+Lamuda_id-1;
						vii p_find=find((*pL).tooling_vec.begin(),(*pL).tooling_vec.end(),tf);
						int offset=p_find-(*pL).tooling_vec.begin();
						vii pb=(*pL).b.begin()+offset;
						vector<IloNumVar>::iterator p_y=y_vector.begin()+index3_y(i,Lamuda_id)-x_limit_first_level-Num_Keydevice-Num_Package-1;
						expr+=(*pb)*(*p_y);
						p_Lamuda++;
					}//lamuda_iteration
					p_int++;
				}//tau_iteration
			}//i_iteration
			vector<t_set> inter_set;
			get_inter_set((*p_set),inter_set);
			vector<t_set>::iterator pm=inter_set.begin();
			int sum=0;
			while(pm!=inter_set.end()){
				int set_id=get_set_id(*pm);
				sum+=n_t_n_tooling[tf][set_id];
				pm++;
			}
			con.add(expr<=sum);
			num_con++;
			expr.end();
			string str="(";
			stringstream ss;
			ss<<"tf@"<<tf<<",n@"<<n<<")";
			str+=ss.str();
			char name[100];
			strcpy(name,str.c_str());
			con[num_con-1].setName(name);
			//cout<<"tf="<<tf<<",n="<<n<<endl;
		}
	}

	cout<<"build constraints (1f)"<<endl;
	for(int i=1;i<=Num_Machine;i++){
		IloExpr expr(env);
		vector<int>::iterator p_L=L_i[i].int_vector.begin();
		while(p_L!=L_i[i].int_vector.end()){
			vii pfind=find(LPD.begin(),LPD.end(),(*p_L));
			if(pfind!=LPD.end()){
				int l=(*p_L);
				int sim=machine_sim[i];
				int d=lots_device[l];
				int index=index_S_sim_d(sim,d);
				vii p_s=S_sim_d[index].second.int_vector.begin();
				while(p_s!=S_sim_d[index].second.int_vector.end()){
					int s=(*p_s);
					vector<IloNumVar>::iterator p_x=x_vector.begin()+index3_x(i,l,s);
					expr+=n_l_chips[l]/rate[s]*(*p_x);
					p_s++;
				}
			}
			p_L++;
		}
		con.add(expr<=H[i]);
		num_con++;
		expr.end();
		string str="(";
		stringstream ss;
		ss<<"i@"<<i<<")";
		str+=ss.str();
		char name[100];
		strcpy(name,str.c_str());
		con[num_con-1].setName(name);
		//cout<<"i="<<i<<endl;
	}

	cout<<"build constraints (1g)"<<endl;
	for(int k=1;k<=Num_Keydevice;k++){
		vector<int>::iterator p_int=Key_Devices.begin()+k-1;
		int d=(*p_int);
		IloExpr expr(env);
		for(int i=1;i<=Num_Machine;i++){
			vector<int>::iterator p_L=L_i_d[i][d].int_vector.begin();
			while(p_L!=L_i_d[i][d].int_vector.end()){
				vii pfind=find(LPD.begin(),LPD.end(),(*p_L));
				if(pfind!=LPD.end()){
					int l=(*p_L);
					int sim=machine_sim[i];
					int index=index_S_sim_d(sim,d);
					vii p_s=S_sim_d[index].second.int_vector.begin();
					while(p_s!=S_sim_d[index].second.int_vector.end()){
						int s=(*p_s);
						vector<IloNumVar>::iterator p_x=x_vector.begin()+index3_x(i,l,s);
						expr+=n_l_chips[l]*(*p_x);
						p_s++;
					}
				}
				p_L++;
			}
		}
		vector<IloNumVar>::iterator p_KD=deltaKD_vector.begin()+k-1;
		expr+=coef*(*p_KD);
		con.add(expr>=n_k_minchips[d]);
		num_con++;
		expr.end();
		string str="(";
		stringstream ss;
		ss<<"k@"<<k<<",dev@"<<d<<")";
		str+=ss.str();
		char name[100];
		strcpy(name,str.c_str());
		con[num_con-1].setName(name);
		//cout<<"k="<<k<<endl;
	}

	cout<<"build constraints (1h)"<<endl;
	for(int p=1;p<=Num_Package;p++){
		vector<int>::iterator p_int=Packages.begin()+p-1;
		int d=(*p_int);
		IloExpr expr(env);
		for(int i=1;i<=Num_Machine;i++){
			vector<int>::iterator p_L=L_i_d[i][d].int_vector.begin();
			while(p_L!=L_i_d[i][d].int_vector.end()){
				vii pfind=find(LPD.begin(),LPD.end(),(*p_L));
				if(pfind!=LPD.end()){
					int l=(*p_L);
					int sim=machine_sim[i];
					int index=index_S_sim_d(sim,d);
					vii p_s=S_sim_d[index].second.int_vector.begin();
					while(p_s!=S_sim_d[index].second.int_vector.end()){
						int s=(*p_s);
						vector<IloNumVar>::iterator p_x=x_vector.begin()+index3_x(i,l,s);
						expr+=n_l_chips[l]*(*p_x);
						p_s++;
					}
				}
				p_L++;
			}
		}
		vector<IloNumVar>::iterator p_PD=deltaPD_vector.begin()+p-1;
		expr+=coef*(*p_PD);
		con.add(expr>=n_p_minchips[d]);
		num_con++;
		expr.end();
		string str="(";
		stringstream ss;
		ss<<"p@"<<p<<",dev@"<<d<<")";
		str+=ss.str();
		char name[100];
		strcpy(name,str.c_str());
		con[num_con-1].setName(name);
		//cout<<"p="<<p<<endl;
	}
/*
	cout<<"build constriants (1i)"<<endl;
	for(int it=1;it<=(int)Lamuda.size();it++){
		vector<Lamuda_item>::iterator p_Lamuda=Lamuda.begin()+it-1;
		//-----------------
		int sum_b=0;
		vector<int>::iterator p_int=(*p_Lamuda).b.begin();
		while(p_int!=(*p_Lamuda).b.end()){
			sum_b+=(*p_int);
			p_int++;
		}
		//---------------------------
		if(sum_b>0){//when the tooling list is not empty
			int Lamuda_id=(*p_Lamuda).id;
			//-------------------------------------temperature sest
			int temperature=(*p_Lamuda).temperature;
			struct t_set one_set;
			one_set.set_elements.push_back(temperature);
			vector<t_set> inter_set;
			get_inter_set(one_set,inter_set);
			//---------------------------------------------------------------
			IloExpr expr(env);
			vector<int>::iterator p_M=M_lamuda[Lamuda_id].int_vector.begin();
			while(p_M!=M_lamuda[Lamuda_id].int_vector.end()){
				int i=(*p_M);
				//cout<<"i="<<i<<",Lamuda="<<Lamuda_id<<","<<index3_y(i,Lamuda_id)<<endl;
				vector<IloNumVar>::iterator p_y=y_vector.begin()+index3_y(i,Lamuda_id)-x_limit_first_level-Num_Keydevice-Num_Package-1;
				expr+=(*p_y);
				p_M++;
			}
			double min_num=1E+8;
			vector<int>::iterator p_tool=(*p_Lamuda).tooling_vec.begin();
			while(p_tool!=(*p_Lamuda).tooling_vec.end()){
				int offset=p_tool-(*p_Lamuda).tooling_vec.begin();
				vector<int>::iterator p_b=(*p_Lamuda).b.begin()+offset;
				int tf=(*p_tool);
				int require_num=(*p_b);
				int available_num=0;
				vector<t_set>::iterator pm=inter_set.begin();
				while(pm!=inter_set.end()){
					int set_id=get_set_id(*pm);
					available_num+=n_t_n_tooling[tf][set_id];
					pm++;
				}
				double ratio=available_num/(require_num*1.0);
				if(ratio<min_num){
					min_num=ratio;
				}
				p_tool++;
			}
			con.add(expr<=(int)min_num);
			num_con++;
			expr.end();
			string str="(";
			stringstream ss;
			ss<<"Lamuda@"<<Lamuda_id<<")";
			str+=ss.str();
			char name[100];
			strcpy(name,str.c_str());
			con[num_con-1].setName(name);
		}
	}
	*/
	model.add(con);
	
	for(int i=x_limit_first_level+Num_Keydevice+Num_Package+1;i<=y_limit_first_level;i++){
		vector<IloNumVar>::iterator p_y=y_vector.begin()+i-x_limit_first_level-Num_Keydevice-Num_Package-1;
		model.add(IloConversion(env,(*p_y),ILOBOOL));
	}
	for(int i=0;i<=x_limit_first_level;i++){
		vector<IloNumVar>::iterator p_x=x_vector.begin()+i;
		model.add(IloConversion(env,(*p_x),ILOBOOL));
	}
}
#endif	

#if 0
void AT::solve_first_level_model(IloModel & model,vector<IloNumVar> & x_vector,vector<IloNumVar> & y_vector
							 ,vector<IloNumVar> & deltaPD_vector,vector<IloNumVar> & deltaKD_vector,double * solution){
	IloCplex cplex(model);
	cplex.setParam(IloCplex::RootAlg,1); //use primal method
	cplex.setParam(IloCplex::TiLim,100000000);
	cplex.setParam(IloCplex::EpGap,0.001);	//reset the tolerance
	cplex.exportModel("AT_CPLEX_first_level.lp");
	if(!cplex.solve()){
		cout<<"!!The model can not be solved by CPLEX!!"<<endl;
	}
	cplex.out() <<"Status="<<cplex.getStatus()<<","<<cplex.getObjValue()<<endl;
	cplex.out() <<"Maximum bound violation = "<<cplex.getQuality(IloCplex::MaxPrimalInfeas) << endl;	
	cplex.out() << "Iteration count = " << cplex.getNiterations() << endl;
	cplex.out() << "Phase I Iter Num= " << cplex.getNphaseOneIterations() <<endl;
	cplex.printTime();

	int id=0;
	vector<IloNumVar>::iterator p_x=x_vector.begin();
	while(p_x!=x_vector.end()){
		solution[id]=cplex.getValue(*p_x);
		id++;
		p_x++;
	}
	vector<IloNumVar>::iterator p_deltaKD=deltaKD_vector.begin();
	while(p_deltaKD!=deltaKD_vector.end()){
		solution[id]=cplex.getValue(*p_deltaKD);
		id++;
		p_deltaKD++;
	}
	vector<IloNumVar>::iterator p_deltaPD=deltaPD_vector.begin();
	while(p_deltaPD!=deltaPD_vector.end()){
		solution[id]=cplex.getValue(*p_deltaPD);
		id++;
		p_deltaPD++;
	}
	vector<IloNumVar>::iterator p_y=y_vector.begin();
	//ofstream fout("ddd.txt");
	while(p_y!=y_vector.end()){
		solution[id]=cplex.getValue(*p_y);
		//fout<<"id="<<id<<",solution="<<solution[id]<<","<<(*p_y)<<endl;
		id++;
		p_y++;
	}
	//fout.close();
}
#endif

void AT::first_level_heuristic(){
	//given the LP solution, apply the heuristic for the first level problem
	//this routine should be called after the LP solver
	//build_first_level_index_vector();
	double * solution = new double[y_limit+1];
	int * tooling_used=new int[Num_Tooling_Family*Num_Temperature];
	int * sim_used = new int[(int)SIM.size()+1];
	int * SIM_number=new int[(int)SIM.size()+1];
	int fix_y[Max_Num_Machine+1];
	//clear the arrays
	for(int id=0;id<=y_limit;id++){ solution[id]=0.0;}
	for(int id=0;id<=Num_Tooling_Family*Num_Temperature-1;id++){ tooling_used[id]=0;}
	for(int id=0;id<=(int)SIM.size();id++){ sim_used[id]=0; SIM_number[id]=0;}

	int second_level_option=1; //1: apply math model 2:apply greedy method
	//============================================================================================================{
    //--------------------------------------------------------setup the model through OSI
	CoinPackedMatrix M;
	double * cost   = new double[y_limit+1];
	double * col_lb = new double[y_limit+1];
	double * col_ub = new double[y_limit+1];

	int num_con=0;
	if(new_old_model==1){
		//updated model
		num_con=Num_Lot+Num_Machine+Num_Tooling_Family*(int)Temperature_Set.size();
		for(int i=1;i<=Num_Machine;i++){
			num_con+=(int)Lamuda_i[i].int_vector.size();
		}
		num_con+=Num_Keydevice+Num_Package;
		//add Key PKG and Key PIN PKG constraints
		num_con+=(int)Key_PKG.size()+(int)Key_PIN_PKG.size();
	}else{
		num_con=Num_Lot;
		for(int i=1;i<=Num_Machine;i++){
			vector<int>::iterator p_Lamuda=Lamuda_i[i].int_vector.begin();
			while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
				int Lamuda_id=(*p_Lamuda);
				vector<int>::iterator p_L=L_i[i].int_vector.begin();
				while(p_L!=L_i[i].int_vector.end()){
					int l=(*p_L);
					vector<tooling_configure> TS;
					get_S_i_l_Lamuda(TS,i,l,Lamuda_id);
					num_con+=(int)TS.size();
					p_L++;
				}
				p_Lamuda++;
			}
		}
		num_con+=Num_Machine+Num_Tooling_Family*(int)Temperature_Set.size()+Num_Machine+Num_Keydevice+Num_Package;
		//add Key PKG and Key PIN PKG constraints
		num_con+=(int)Key_PKG.size()+(int)Key_PIN_PKG.size();
	}
	cout<<"in first_level_heuristic,num_var="<<y_limit+1<<",num_con="<<num_con<<endl;
	double * row_lb=new double[num_con];
	double * row_ub=new double[num_con];
	//--------------------------------------------clear the arrays
	time_t t1,t2;

	for(int id=0;id<=y_limit;id++){
		cost[id]  =0.0;
		col_lb[id]=-DBL_MAX;
		col_ub[id]= DBL_MAX;
	}
	for(int id=0;id<=num_con-1;id++){
		row_lb[id]=-DBL_MAX;
		row_ub[id]= DBL_MAX;
	}
	//============================================================================================================}
	if(second_level_option==1){
		build_first_level_model_OSI(&M,cost,col_lb,col_ub,row_lb,row_ub);
		OsiXxxSolverInterface si;
		time(&t1);
		si.loadProblem(M,col_lb,col_ub,cost,row_lb,row_ub);
		time(&t2);
		cout<<"it costs "<<difftime(t2,t1)<<" seconds to load the problem"<<endl;
		//set the integer variables		
		time(&t1);
		for(int id=0;id<=x_limit;id++){ si.setInteger(id);}
		for(int id=x_limit+Num_Keydevice+Num_Package+(int)Key_PKG.size()+(int)Key_PIN_PKG.size()+1;id<=y_limit;id++){ si.setInteger(id);}
		time(&t2);
		cout<<"it costs "<<difftime(t2,t1)<<" seconds to set the integrity"<<endl;
		si.setHintParam(OsiDoDualInResolve,false,OsiHintDo);
		//time(&t1);
		si.writeLp("Debug/AT_OSI");
		//si.resolve();
		//time(&t2);
		//cout<<"solving LP costs "<<difftime(t2,t1)<<" seconds"<<endl;
		//si.branchAndBound();
	}
	//============================================================================================================{build the model for update_benefit
	CoinPackedMatrix M2;
	double * cost2   = new double[Num_Lot+Num_Keydevice+Num_Package];
	double * col_lb2 = new double[Num_Lot+Num_Keydevice+Num_Package];
	double * col_ub2 = new double[Num_Lot+Num_Keydevice+Num_Package];
	double * row_lb2 = new double[1+Num_Keydevice+Num_Package];
	double * row_ub2 = new double[1+Num_Keydevice+Num_Package];
	build_update_benefit_model(&M2,cost2,col_lb2,col_ub2,row_lb2,row_ub2);
	OsiXxxSolverInterface si_KP;
	si_KP.loadProblem(M2,col_lb2,col_ub2,cost2,row_lb2,row_ub2);
	//============================================================================================================}
	//-----------------------------------------------------------------------------------
	vector<double> SL_avg((int)L_sim_lamuda.size(),0);
	vector<int> SL_num((int)SL_avg.size(),0);
	vector<double> SL_best((int)L_sim_lamuda.size(),0);
	//-----------------------------------------------------------------------------------
	time(&t1);
	int alpha_list[]={1,2,3,4,5,6,7,8,9,10};
	double prob_list[] ={0.10,0.10,0.10,0.10,0.10,0.10,0.10,0.10,0.10,0.10};
	double avg_list[]  ={0,0,0,0,0,0,0,0,0,0};
	int    num_list[]  ={0,0,0,0,0,0,0,0,0,0};
	double delta=50;
	vector<soln_pool_item> soln_pool;
	int soln_pool_size=20;
	double best_obj=1E+10;
	int best_iter=-1;
	//----------------------------------------------------------initialize the sim_lamuda_benefit
	for(int tf=1;tf<=Num_Tooling_Family;tf++){
		for(int n=1;n<=Num_Temperature;n++){
			int id=(tf-1)*Num_Temperature+n-1;
			tooling_used[id]=0;
		}
	}
	for(int i=1;i<=(int)SIM.size();i++){ sim_used[i]=0;}
	for(int i=1;i<=(int)SIM.size();i++){ SIM_number[i]=0;}
	for(int i=1;i<=Num_Machine;i++){
		int simid=machine_sim[i];
		SIM_number[simid]++;
	}
	vector< pair<double,vec_int> > sim_lamuda_benefit_save;
	initialize_benefit(sim_lamuda_benefit_save,tooling_used,sim_used,SIM_number);
	//-----------------------------------------------------------
	OsiXxxSolverInterface si;
	si.loadProblem(M,col_lb,col_ub,cost,row_lb,row_ub);
	//set the integer variables		
	for(int id=0;id<=x_limit;id++){ si.setInteger(id);}
	for(int id=x_limit+Num_Keydevice+Num_Package+(int)Key_PKG.size()+(int)Key_PIN_PKG.size()+1;id<=y_limit;id++){ si.setInteger(id); si.setColBounds(id,0,0);}

	for(int iter=1;iter<=Max_Iter;iter++){
		cout<<"Outer iter="<<iter<<endl;
		//----------------------------------------------------initialization
		for(int id=x_limit+Num_Keydevice+Num_Package+(int)Key_PKG.size()+(int)Key_PIN_PKG.size()+1;id<=y_limit;id++){ si.setColBounds(id,0,0);}
		for(int tf=1;tf<=Num_Tooling_Family;tf++){
			for(int n=1;n<=Num_Temperature;n++){
				int id=(tf-1)*Num_Temperature+n-1;
				tooling_used[id]=0;
			}
		}
		for(int i=1;i<=(int)SIM.size();i++){ sim_used[i]=0;}

		for(int i=1;i<=Num_Machine;i++){ fix_y[i]=0;}  //clear fix_y[]
		vector<int> lot_remain;

		for(int l=1;l<=Num_Lot;l++){ lot_remain.push_back(l);}
		vector< pair<double,vec_int> > sim_lamuda_benefit;
		sim_lamuda_benefit.resize((int)sim_lamuda_benefit_save.size());
		copy(sim_lamuda_benefit_save.begin(),sim_lamuda_benefit_save.end(),sim_lamuda_benefit.begin());
		//--------------------------------------------------------------------self-adjusted RCL
		int select_alpha_id=-1;
		double alpha=0;
		if(iter<=10){//warm up
			select_alpha_id=iter-1;
			alpha=alpha_list[select_alpha_id];
		}else{
			select_alpha_id=select_alpha(prob_list,10);
			alpha=alpha_list[select_alpha_id];
		}
		//cout<<"alpha="<<alpha<<endl;
		//cout<<"prob_list=";
		//for(int i=0;i<=9;i++){ cout<<prob_list[i]<<","; } 
		//cout<<endl;
		//--------------------------------------------------------------------start the loop
		int counter=0;
		vector<int> lot_assigned_so_far;
		vector<int> production(Num_Device,0);
		while(true){
			//cout<<"counter="<<counter<<endl;
			//--------------------------------------------------------------------------construct the CL and RCL
			//cout<<"construct CL"<<endl;
			vector< pair< pair<int,int>,double> > candidate_list;
			construct_CL(candidate_list,tooling_used,sim_used,SIM_number,sim_lamuda_benefit);
			//cout<<"size of CL="<<(int)candidate_list.size()<<endl;
			if((int)candidate_list.size()<1){ break; }//stop the loop here

			int RCL_length=construct_RCL(candidate_list,alpha);
			//if(iter>500){
			//	RCL_length=RCL_length*(1+(roundup(iter*1.0/100)-5)*0.5);
			//	if(RCL_length>(int)candidate_list.size()){
			//		RCL_length=(int)candidate_list.size();
			//	}
			//}
			//cout<<"RCL_length="<<RCL_length<<endl;

			int selected_id;
			if(iter<=20){
				selected_id=pick(RCL_length)+1;
			}else{
				selected_id=pick(candidate_list,RCL_length,SL_avg,SL_best,best_obj,50)+1;
			}
			//cout<<"selected_id="<<selected_id<<endl;
			//----------------------------------------------------------------------------randomly select one
			vector<pair< pair<int,int>,double > >::iterator p_selected=candidate_list.begin()+selected_id-1;
			int sim_selected=(*p_selected).first.first;
			int Lamuda_selected=(*p_selected).first.second;
			vector<Lamuda_item>::iterator pL=Lamuda.begin()+Lamuda_selected-1;
			//cout<<"sim_selected="<<sim_selected<<",Lamuda_selected="<<Lamuda_selected<<endl;
			//----------------------------------------------------------------------------update machine and tooling usage
			sim_used[sim_selected]++;
			//---------------------------------------------------
			vii p_tf=(*pL).tooling_vec.begin();
			while(p_tf!=(*pL).tooling_vec.end()){
				int tf=(*p_tf);
				int offset=p_tf-(*pL).tooling_vec.begin();
				vii p_b=(*pL).b.begin()+offset;
				int id=(tf-1)*Num_Temperature+(*pL).temperature-1;
				tooling_used[id]+=(*p_b);
				p_tf++;
			}
			//---------------------------------------------------fix y
			//cout<<"fix y"<<endl;
			for(int i=1;i<=Num_Machine;i++){
				if(machine_sim[i]==sim_selected && fix_y[i]<1){
					fix_y[i]=Lamuda_selected;
					break;
				}
			}
			//-------------------------------------------------------------------------------------assign lots to the selected sim_lamuda combination
			vector<int> lot_selected;
			//cout<<"greedy assignment"<<endl;
			//greedy_assignment(sim_selected,Lamuda_selected,lot_remain,lot_selected,production);
			int id_select=index_L_sim_lamuda(sim_selected,Lamuda_selected);
			lot_selected.resize((int)sim_lamuda_benefit[id_select].second.int_vector.size());
			copy(sim_lamuda_benefit[id_select].second.int_vector.begin(),sim_lamuda_benefit[id_select].second.int_vector.end(),lot_selected.begin());
			vector<int> affected_dev;
			for(int l1=1;l1<=(int)lot_selected.size();l1++){
				int l=lot_selected[l1-1];
				int d=lots_device[l];
				lot_assigned_so_far.push_back(l);
				production[d-1]+=n_l_chips[l];
				vii pfind=find(lot_remain.begin(),lot_remain.end(),l);
				if(pfind!=lot_remain.end()){
					lot_remain.erase(pfind);
				}
				vii p2=find(affected_dev.begin(),affected_dev.end(),d);
				if(p2==affected_dev.end()){
					affected_dev.push_back(d);
				}
				si_KP.setColBounds(l-1,0.0,0.0);
			}
			//--------------------------------------------------
			//cout<<"size of lot_remain  ="<<lot_remain.size()<<endl;
			//cout<<"size of lot_selected="<<lot_selected.size()<<endl;
			//-------------------------------------------------
			//cout<<"updates"<<endl;
			update_benefit(sim_lamuda_benefit,lot_remain,production,affected_dev,tooling_used,sim_used,SIM_number);
			//update_benefit2(sim_lamuda_benefit,lot_remain,production,affected_dev,tooling_used,sim_used,SIM_number,si_KP);
			counter++;
		}//end_while
		//for(int i=1;i<=Num_Machine;i++){ cout<<"i="<<i<<",fix_y="<<fix_y[i]<<endl;}
		//-------------------------------------------------------------------------------
		double second_level_obj=1E+10;
		vector<sub_soln_item> sub_soln;
		vector< pair<int,double> > shortage;
		vector< pair< pair<int,double>,double> > unassigned_lots;
		if(second_level_option==1){
			//---------------------------------------------------------------------------solve the problem with fix_y[] as LP
			for(int i=1;i<=Num_Machine;i++){
				if(fix_y[i]>0){
					int y_id=index2_y(i,fix_y[i]);
					//cout<<"set i="<<i<<",Lamuda_id="<<fix_y[i]<<",id="<<y_id<<endl;
					si.setColBounds(y_id,1,1);
				}
			}

			si.setHintParam(OsiDoDualInResolve,false,OsiHintDo);

			if(LP_IP==1){
				si.writeLp("Debug/first_level_heur");
				si.resolve();
				Num_LP_solved++;
			}else{
				si.branchAndBound();
			}
			if(si.isProvenOptimal()){
				CoinDisjointCopyN(si.getColSolution(), y_limit+1, solution);
				//---------------------------------------------------------------------------adjust solution[]
				adjust_LP_solution(solution);
				//-----------------------------------------------------------------------solve this subproblem heuristically
				second_level_obj=second_level_heuristic(solution,sub_soln,shortage,unassigned_lots,2);
				//show_sub_soln(sub_soln);
			}else{
				continue;
			}
		}else if(second_level_option==2){
			second_level_obj=second_level_greedy(fix_y,sub_soln,shortage,unassigned_lots);
		}
		cout<<"second_level_obj="<<second_level_obj<<endl;
		//------------------------------------------------------------------
		bool insert_flag=true;
		for(int j=1;j<=(int)soln_pool.size();j++){
			if(fabs(second_level_obj-soln_pool[j-1].obj)<1E-06){ insert_flag=false;}
		}
		if(insert_flag){
			soln_pool_item one_item;
			one_item.obj=second_level_obj;
			one_item.sub_soln.resize((int)sub_soln.size());
			copy(sub_soln.begin(),sub_soln.end(),one_item.sub_soln.begin());
			one_item.shortage.resize((int)shortage.size());
			copy(shortage.begin(),shortage.end(),one_item.shortage.begin());
			one_item.unassigned_lots.resize((int)unassigned_lots.size());
			copy(unassigned_lots.begin(),unassigned_lots.end(),one_item.unassigned_lots.begin());
			soln_pool.push_back(one_item);
		}
		//------------------------------------------------------------------do update staff here
		if(second_level_obj<best_obj){
			best_obj=second_level_obj; 
			best_iter=iter;
		}
		//update the alpha probabilit list
		if(iter<=10){
			avg_list[select_alpha_id]=second_level_obj;
			num_list[select_alpha_id]++;
		}else{
			avg_list[select_alpha_id]=((avg_list[select_alpha_id]*num_list[select_alpha_id])+second_level_obj)/(num_list[select_alpha_id]+1);
			num_list[select_alpha_id]++;
			update_alpha_prob(prob_list,avg_list,10,best_obj,delta);
		}
		//update SL
		for(int i=1;i<=Num_Machine;i++){
			int sim=machine_sim[i];
			int Lamuda_id=fix_y[i];
			if(Lamuda_id>0){
				int index=index_L_sim_lamuda(sim,Lamuda_id);
				SL_avg[index]=(SL_avg[index]*SL_num[index]+second_level_obj)/(SL_num[index]+1);
				if(SL_num[index]<1 || second_level_obj<SL_best[index]){ SL_best[index]=second_level_obj;}
				SL_num[index]++;
			}
		}		
		cout<<"insert_flag="<<insert_flag<<endl;
	}//end_for_iter
	time(&t2);
	cout<<"it costs "<<difftime(t2,t1)<<" seconds to run the Phase I iterations"<<endl;
	cout<<"best iter="<<best_iter<<",best_obj="<<best_obj<<endl;
	sort(soln_pool.begin(),soln_pool.end(),compare_soln);
	cout<<"Phase I statistics:"<<endl;
	cout<<"Min ="<<soln_pool[0].obj<<",Max="<<soln_pool[(int)soln_pool.size()-1].obj<<endl;
	double PhaseI_avg=0.0;
	for(int id=1;id<=(int)soln_pool.size();id++){
		PhaseI_avg+=soln_pool[id-1].obj;
	}
	PhaseI_avg=PhaseI_avg/(int)soln_pool.size();
	cout<<"ave obj="<<PhaseI_avg<<endl;

	if((int)soln_pool.size()>soln_pool_size){
		soln_pool.erase(soln_pool.begin()+soln_pool_size,soln_pool.end());
	}

	for(int i=1;i<=(int)soln_pool.size();i++){
		cout<<soln_pool[i-1].obj<<endl;
	}
	//---------------------------------------------------------------------------copy the solutions to elite_soln (data member of AT class)
	elite_soln.clear();
	elite_soln.resize((int)soln_pool.size());
	copy(soln_pool.begin(),soln_pool.end(),elite_soln.begin());
	//---------------------------------------------------------------------------output Phase I solution
	cout<<"elite soln after Phase I"<<endl;
	for(int i=1;i<=(int)soln_pool.size();i++){
		double total_weight=0.0;
		for(int j=1;j<=Num_Machine;j++){
			vii p_lot=soln_pool[i-1].sub_soln[j-1].lots.int_vector.begin();
			while(p_lot!=soln_pool[i-1].sub_soln[j-1].lots.int_vector.end()){
				int l=(*p_lot);
				total_weight+=w_l[l];
				p_lot++;
			}
		}
		double total_shortage=0.0;
		for(int d=1;d<=Num_Device;d++){
			if(is_KD_PD(d) && soln_pool[i-1].shortage[d-1].second>0){
				total_shortage+=soln_pool[i-1].shortage[d-1].second;
			}
		}
		cout<<"obj="<<soln_pool[i-1].obj<<",total_weight="<<total_weight<<",total_shortage="<<total_shortage<<endl;
	}
	//---------------------------------------------------------------------------local search for the elite soln
	if(PhaseII_option){
		time_t local_search_start;
		time(&local_search_start);
		/*
		for(int id=1;id<=(int)soln_pool.size();id++){
			OsiXxxSolverInterface sii;
			sii.loadProblem(M,col_lb,col_ub,cost,row_lb,row_ub);

			cout<<"apply local search to elite soln "<<id<<endl;
			local_search_on_y(&sii,soln_pool[id-1]);
			soln_pool[id-1].obj=compute_obj(soln_pool[id-1].sub_soln,soln_pool[id-1].shortage);
		}

		cout<<"elite soln after local search"<<endl;
		sort(soln_pool.begin(),soln_pool.end(),compare_soln);
		for(int i=1;i<=(int)soln_pool.size();i++){
			double total_weight=0.0;
			for(int j=1;j<=Num_Machine;j++){
				vii p_lot=soln_pool[i-1].sub_soln[j-1].lots.int_vector.begin();
				while(p_lot!=soln_pool[i-1].sub_soln[j-1].lots.int_vector.end()){
					int l=(*p_lot);
					total_weight+=w_l[l];
					p_lot++;
				}
			}
			double total_shortage=0.0;
			for(int d=1;d<=Num_Device;d++){
				if(is_KD_PD(d) && soln_pool[i-1].shortage[d-1].second>0){
				total_shortage+=soln_pool[i-1].shortage[d-1].second;
			}
		}
		cout<<"obj="<<soln_pool[i-1].obj<<",total_weight="<<total_weight<<",total_shortage="<<total_shortage<<endl;
		}
		time_t local_search_end;
		time(&local_search_end);
		cout<<"time elapsed for local search ="<<difftime(local_search_end,local_search_start)<<" seconds"<<endl;
		*/
		//--------------------------------------------------------------------------------------------------------------
		/*
		for(int id=1;id<=(int)soln_pool.size();id++){
		cout<<soln_pool[id-1].obj<<endl;
		show_sub_soln(soln_pool[id-1].sub_soln);
		for(int i=1;i<=Num_Machine;i++){
		double benefit=0.0;
		vector<int> dev;
		vector<int> amount;
		vii p_lot=soln_pool[id-1].sub_soln[i-1].lots.int_vector.begin();
		while(p_lot!=soln_pool[id-1].sub_soln[i-1].lots.int_vector.end()){
		int l=(*p_lot);
		benefit+=w_l[l];
		int d=lots_device[l];
		vii pfind=find(dev.begin(),dev.end(),d);
		if(pfind!=dev.end()){
		int offset=pfind-dev.begin();
		vii p=amount.begin()+offset;
		(*p)+=n_l_chips[l];
		}else{
		dev.push_back(d);
		amount.push_back(n_l_chips[l]);
		}
		p_lot++;
		}
		for(int j=1;j<=(int)dev.size();j++){
			int d=dev[j-1];
			int num=amount[j-1];
			if(is_KD_PD(d)){
			if(soln_pool[id-1].shortage[d-1].second>0){
				benefit+=penalty[d]/coef*num;
			}else if(soln_pool[id-1].shortage[d-1].second+num>0){
				benefit+=penalty[d]/coef*(soln_pool[id-1].shortage[d-1].second+num);
			}
		}
		}
		cout<<"i="<<i<<",benefit="<<benefit<<endl;
		}
		}
		*/
		//-------------------------------------------------------------------------------------------------------------------------
		//------------------------------------- try the local branch idea
		/*	
		vector<double> obj_AB;
		for(int id=1;id<=(int)soln_pool.size();id++){
		for(int id2=id+1;id2<=(int)soln_pool.size();id2++){
		OsiXxxSolverInterface sii;
		sii.loadProblem(M,col_lb,col_ub,cost,row_lb,row_ub);
		for(int i=0;i<=x_limit;i++){ sii.setInteger(i);}
		for(int i=x_limit+Num_Keydevice+Num_Package+1;i<=y_limit;i++){ sii.setInteger(i);}
		//double obj=local_branch_AB2(&sii,soln_pool[id-1],soln_pool[id2-1]);
		//obj_AB.push_back(obj);
		}
		}
		//---------------------------------------
		sort(obj_AB.begin(),obj_AB.end());
		for(int i=1;i<=(int)obj_AB.size();i++){
		cout<<obj_AB[i-1]<<endl;
		}	
		*/
		//-------------------------------------------------------------------------------try the LP local branch idea
		cout<<"Apply LP based Local Branch with K=1"<<endl;
		time(&t1);
		double best_local_branch_obj=1E+10;
		int best_id=-1;
		for(int id=1;id<=(int)soln_pool.size();id++){
			OsiXxxSolverInterface ssii;
			ssii.loadProblem(M,col_lb,col_ub,cost,row_lb,row_ub);
			int counter=1;
			//apply local branch 1
			int K=1;
			double best_for_this_id=soln_pool[id-1].obj;
			double local_branch_obj=local_branch_LP(&ssii,soln_pool[id-1],K);
			cout<<"id="<<id<<",local_branch_obj_1="<<local_branch_obj<<",best_for_this_id="<<best_for_this_id<<endl;
			while(local_branch_obj<best_for_this_id-1E-06 && counter<=2){
				best_for_this_id=local_branch_obj;
				local_branch_obj=local_branch_LP(&ssii,soln_pool[id-1],K);
				cout<<"id="<<id<<",local_branch_obj_1="<<local_branch_obj<<endl;
				counter++;
			}	
			if(best_for_this_id<best_local_branch_obj){
				best_local_branch_obj=best_for_this_id;
				best_id=id;
			}
		}
		soln_pool[best_id-1].obj=compute_obj(soln_pool[best_id-1].sub_soln,soln_pool[best_id-1].shortage);
		//--------------------------------------------------------------------------------compute total weight and total shortage
		double tw_lb=0.0;
		for(int j=1;j<=Num_Machine;j++){
			vii p_lot=soln_pool[best_id-1].sub_soln[j-1].lots.int_vector.begin();
			while(p_lot!=soln_pool[best_id-1].sub_soln[j-1].lots.int_vector.end()){
				int l=(*p_lot);
				tw_lb+=w_l[l];
				p_lot++;
			}
		}
		double ts_lb=0.0;
		for(int d=1;d<=Num_Device;d++){
			if(is_KD_PD(d) && soln_pool[best_id-1].shortage[d-1].second>0){
				ts_lb+=soln_pool[best_id-1].shortage[d-1].second;
			}
		}
		cout<<"obj="<<soln_pool[best_id-1].obj<<",total_weight="<<tw_lb<<",total_shortage="<<ts_lb<<endl;
		time(&t2);
		cout<<"it costs "<<difftime(t2,t1)<<" seconds to run the local branch"<<endl;
		cout<<"Num_Simulated="<<Num_Simulated<<",Num_Discarded="<<Num_Discarded<<endl;
		//------------------------------------------------------------------------------------------------------------
		cout<<"Apply LP based Local Branch with K=2"<<endl;
		time(&t1);
		best_local_branch_obj=1E+10;
		best_id=-1;
		for(int id=1;id<=(int)soln_pool.size();id++){
			OsiXxxSolverInterface ssii;
			ssii.loadProblem(M,col_lb,col_ub,cost,row_lb,row_ub);
			int counter=1;
			//apply local branch 2

			double best_for_this_id=soln_pool[id-1].obj;
			double local_branch_obj=local_branch_LP(&ssii,soln_pool[id-1],2);
			cout<<"id="<<id<<",local_branch_obj_2="<<local_branch_obj<<",best_for_this_id="<<best_for_this_id<<endl;
			while(local_branch_obj<best_for_this_id-1E-06 && counter<=2){
				best_for_this_id=local_branch_obj;
				local_branch_obj=local_branch_LP(&ssii,soln_pool[id-1],2);
				cout<<"id="<<id<<",local_branch_obj_2="<<local_branch_obj<<endl;
			}
			if(best_for_this_id<best_local_branch_obj){
				best_local_branch_obj=best_for_this_id;
				best_id=id;
			}
		}
		soln_pool[best_id-1].obj=compute_obj(soln_pool[best_id-1].sub_soln,soln_pool[best_id-1].shortage);
		cout<<"obj="<<soln_pool[best_id-1].obj<<endl;
		time(&t2);
		cout<<"it costs "<<difftime(t2,t1)<<" seconds to run the local branch"<<endl;
		cout<<"Total Num_Simulated="<<Num_Simulated<<",Total Num_Discarded="<<Num_Discarded<<endl;
		cout<<"Num_LP_solved="<<Num_LP_solved<<endl;
		//-----------------------------------------------------------
		//----------------------------------------------------------try the gene idea
	}
	//---------------------------------------------------------------------------output the best solution
	sort(soln_pool.begin(),soln_pool.end(),compare_soln);
	minimize_makespan(soln_pool[0]);
	double * solution_output = new double[y_limit+1+(y_limit-x_limit+1)];
	soln_pool_item_to_solution(soln_pool[0],solution_output);
	generate_reports(solution_output);
	delete [] solution_output;
	//---------------------------------------------------------------------------
	delete [] SIM_number;
	delete [] sim_used;
	delete [] tooling_used;
	delete [] solution;
	delete [] cost;
	delete [] col_lb;
	delete [] col_ub;
	delete [] row_lb;
	delete [] row_ub;
	delete [] cost2;
	delete [] col_lb2;
	delete [] col_ub2;
	delete [] row_lb2;
	delete [] row_ub2;
}

void AT::construct_CL(vector< pair< pair<int,int>,double> > & CL,int * tooling_used,int * sim_used,int * SIM_number,vector< pair<double,vec_int> > & sim_lamuda_benefit){
	//given tooling and SIM usage, construct CL according to the (sim,lamuda) benefit
	CL.clear();
	int index=-1;
	for(int simid=1;simid<=(int)SIM.size();simid++){
		//cout<<"simid="<<simid<<endl;
		vector<SIM_item>::iterator pSIM=SIM.begin()+simid-1;
		int machine_id=(*pSIM).represented_machine_id;
		vector<int>::iterator p_Lamuda=Lamuda_i[machine_id].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[machine_id].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			index++;

			if(sim_lamuda_benefit[index].first>1E-04){
				//append the CL
				pair< pair<int,int>,double > one_pair;
				one_pair.first.first=simid;
				one_pair.first.second=Lamuda_id;
				one_pair.second=sim_lamuda_benefit[index].first;
				CL.push_back(one_pair);
			}//end_if
			p_Lamuda++;
		}//end_while
	}//end_for_sim
	//sort the candidate_list according to the benefit
	//----------------------------------------------------------------------output the CL
	sort(CL.begin(),CL.end(),compare_candidates);
	ofstream fdebug_CL("Debug/debug_CL.txt");
	for(int i=1;i<=(int)CL.size();i++){
		vector< pair< pair<int,int>,double> >::iterator p_CL=CL.begin()+i-1;
		fdebug_CL<<"id="<<i<<",sim="<<(*p_CL).first.first<<",Lamuda_id="<<(*p_CL).first.second<<",benefit="<<(*p_CL).second<<endl;
	}
	fdebug_CL.close();
}

int AT::construct_RCL(vector< pair< pair<int,int>,double> > & CL,double alpha){
	int temp=roundup(alpha);
	if((int)CL.size()<temp){ temp=(int)CL.size();}
	return temp;
}

void AT::get_tooling_sum_rhs(){
	tooling_sum_rhs.clear();
	for(int tf=1;tf<=Num_Tooling_Family;tf++){
		for(int n=1;n<=(int)Temperature_Set.size();n++){
			vector<t_set>::iterator p_set=Temperature_Set.begin()+n-1;
			vector<t_set> inter_set;
			get_inter_set((*p_set),inter_set);
			vector<t_set>::iterator pm=inter_set.begin();
			int sum=0;
			while(pm!=inter_set.end()){
				int set_id=get_set_id(*pm);
				sum+=n_t_n_tooling[tf][set_id];
				pm++;
			}
			tooling_sum_rhs.push_back(make_pair(make_pair(tf,n),sum));
		}
	}
}

void AT::greedy_assignment(int simid,int Lamuda_id,vector<int> & lot_set,vector<int> & lot_selected,vector<int> & production){
	vector<int> prod;
	prod.resize((int)production.size());
	copy(production.begin(),production.end(),prod.begin());
	lot_selected.clear();
	//-----------------------------------------------------------------------------------------for LL
	vector< pair<int,double> > LL;
	int i=SIM[simid-1].represented_machine_id;
	int index=index_L_sim_lamuda(simid,Lamuda_id);
	for(int id=1;id<=(int)L_sim_lamuda[index].second.int_vector.size();id++){
		int l=L_sim_lamuda[index].second.int_vector[id-1];
		vii pfind=find(lot_set.begin(),lot_set.end(),l);
		if(pfind!=lot_set.end()){
			int s=max_rate_s(i,l,Lamuda_id);
			double time_required=n_l_chips[l]/rate[s];
			int d=lots_device[l];
			int type=is_KD_PD(d);
			if(type==1 && prod[d-1]<n_k_minchips[d]){
				int min=n_k_minchips[d]-prod[d-1];
				if(min>n_l_chips[l]){ min=n_l_chips[l];}
				LL.push_back(make_pair(l,(w_l[l]+penalty[d]/coef*min)/time_required));
			}else if(type==2 && prod[d-1]<n_p_minchips[d]){//it is a package device
				int min=n_p_minchips[d]-prod[d-1];
				if(min>n_l_chips[l]){ min=n_l_chips[l];}
				LL.push_back(make_pair(l,(w_l[l]+penalty[d]/coef*min)/time_required));
			}else{
				LL.push_back(make_pair(l,w_l[l]/time_required));
			}
		}//end_if
	}
	sort(LL.begin(),LL.end(),compare_lot);
	//-----------------------------------------------------------------------------------------Assign
	double time_used=0.0;
	for(int id=1;id<=(int)LL.size();id++){
		int l=LL[id-1].first;
		int s=max_rate_s(i,l,Lamuda_id);
		double time_required=n_l_chips[l]/rate[s];
		if(time_used+time_required<=H[i]){
			lot_selected.push_back(l);
			time_used+=time_required;
			int d=lots_device[l];
			prod[d-1]+=n_l_chips[l];
			//------------------------------------------------------------------------------updated LL
			int type=is_KD_PD(d);
			if(type==1){
				bool flag=false;
				int shortage=n_k_minchips[d]-prod[d-1];
				for(int id2=id+1;id2<=(int)LL.size();id2++){
					int l2=LL[id2-1].first;
					int d2=lots_device[l2];
					if(d2==d && shortage<n_l_chips[l2]){
						int s2=max_rate_s(i,l2,Lamuda_id);
						double time2=n_l_chips[l2]/rate[s2];
						if(shortage>0){
							LL[id2-1].second=(w_l[l2]+penalty[d2]/coef*shortage)/time2;
							flag=true;
						}else if(shortage<0 && shortage+n_l_chips[l]>0){
							LL[id2-1].second=w_l[l2]/time2;
							flag=true;
						}
					}
				}//end_for
				if(flag){
					sort(LL.begin()+id,LL.end(),compare_lot);
				}
			}else if(type==2){
				bool flag=false;
				int shortage=n_p_minchips[d]-prod[d-1];
				for(int id2=id+1;id2<=(int)LL.size();id2++){
					int l2=LL[id2-1].first;
					int d2=lots_device[l2];
					if(d2==d && shortage<n_l_chips[l2]){
						int s2=max_rate_s(i,l2,Lamuda_id);
						double time2=n_l_chips[l2]/rate[s2];
						if(shortage>0){
							LL[id2-1].second=(w_l[l2]+penalty[d2]/coef*shortage)/time2;
							flag=true;
						}else if(shortage<0 && shortage+n_l_chips[l]>0){
							LL[id2-1].second=w_l[l2]/time2;
							flag=true;
						}
					}
				}//end_for
				if(flag){
					sort(LL.begin()+id,LL.end(),compare_lot);
				}
			}//end_if
			//--------------------------------------------------------------------------------------
		}
	}		
	cout<<"time_use="<<time_used<<endl;
}

void AT::initialize_benefit(vector< pair<double,vec_int> > & sim_lamuda_benefit,int * tooling_used,int * sim_used,int * SIM_number){
	cout<<"in initialize_benefit"<<endl;
	//---------------------------------------------------------------------------------------------------------for LL
	vector< pair<int,double> > LL;
	for(int l=1;l<=Num_Lot;l++){
		int d=lots_device[l];
		int type=is_KD_PD(d);
		bool is_kp=is_Key_PKG(d);
		bool is_kpp=is_Key_PIN_PKG(d);
		pair<int,double> one_pair;
		one_pair.first=l;
		one_pair.second=w_l[l];
		if(type==1){
			double min=n_l_chips[l];
			if(min>n_k_minchips[d]){ min=n_k_minchips[d];}
			one_pair.second+=penalty[d]/coef*min;
			//LL.push_back(make_pair(l,w_l[l]+penalty[d]/coef*min));
		}
		if(is_kp){
			double min=n_l_chips[l];
			int pkg_id=device_PKG[d];
			if(min>n_kp_minchips[pkg_id]){ min=n_kp_minchips[pkg_id];}
			one_pair.second+=PKG_penalty[pkg_id]/coef*min;
		}
		if(is_kpp){
			double min=n_l_chips[l];
			int pkg_id=device_PKG[d];
			int pin_num=device_PIN[d];
			int target=0;
			for(int id=1;id<=(int)Key_PIN_PKG.size();id++){
				if(Key_PIN_PKG[id-1].first.first==pin_num && Key_PIN_PKG[id-1].first.second==pkg_id){
					target=Key_PIN_PKG[id-1].second;
					break;
				}
			}
			if(min>target){ min=target;}
			one_pair.second+=get_PIN_PKG_penalty(pkg_id,pin_num)/coef*min;
		}

		LL.push_back(one_pair);
	}
	sort(LL.begin(),LL.end(),compare_lot);
	//for(int j=1;j<=(int)LL.size();j++){
	//	cout<<"l="<<LL[j-1].first<<",initial ben="<<LL[j-1].second<<endl;
	//}
	//------------------------------------------------------------------------------------------------
	sim_lamuda_benefit.clear();
	int index=-1;
	for(int sim=1;sim<=(int)SIM.size();sim++){
		int machine_id=SIM[sim-1].represented_machine_id;
		vector<int>::iterator p_Lamuda=Lamuda_i[machine_id].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[machine_id].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			index++;
			pair<double,vec_int> one_pair;

			//---------------------------------------------------------------------------------------------------------test if sim_lamuda is feasible
			int test=is_sim_lamuda_feasible(sim,Lamuda_id,tooling_used,sim_used,SIM_number);
			if(!test){ 
				one_pair.first=-1E+6;
				one_pair.second.int_vector.clear();
				sim_lamuda_benefit.push_back(one_pair);
				p_Lamuda++; 
				continue;
			}
			//---------------------------------------------------------------------------------------------------------for LLL
			vector< pair<int,double> > LLL;
			for(int id=1;id<=(int)LL.size();id++){
				int l=LL[id-1].first;
				vii pfind=find(L_sim_lamuda[index].second.int_vector.begin(),L_sim_lamuda[index].second.int_vector.end(),l);
				if(pfind!=L_sim_lamuda[index].second.int_vector.end()){
					int s=max_rate_s(machine_id,l,Lamuda_id);
					double time_required=n_l_chips[l]/rate[s];
					double ben=LL[id-1].second/time_required;
					LLL.push_back(make_pair(l,ben));
				}
			}
			sort(LLL.begin(),LLL.end(),compare_lot);
			//---------------------------------------------------------------------------------------------------------for LLL
			vector<int> production(Num_Device,0);
			vector<int> PKG_production((int)PKG.size(),0);
			vector<int> PIN_PKG_production((int)PKG.size()*(int)PIN_order.size()+1,0);
			double time_used=0;
			double sum=0;
			for(int id=1;id<=(int)LLL.size();id++){
				int l=LLL[id-1].first;
				int d=lots_device[l];
				int s=max_rate_s(machine_id,l,Lamuda_id);
				double time_required=n_l_chips[l]/rate[s];
				if(time_used+time_required<=H[machine_id]){
					time_used+=time_required;
					production[d-1]+=n_l_chips[l];
					int pkg_id=device_PKG[d];
					int pin_num=device_PIN[d];
					PKG_production[pkg_id-1]+=n_l_chips[l];
					vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);
					int pin_order=pfind-PIN_order.begin()+1;
					int pin_pkg_index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;
					PIN_PKG_production[pin_pkg_index]+=n_l_chips[l];
					int target=0;
					for(int id2=1;id2<=(int)Key_PIN_PKG.size();id2++){
						if(Key_PIN_PKG[id2-1].first.first==pin_num && Key_PIN_PKG[id2-1].first.second==pkg_id){
							target=Key_PIN_PKG[id2-1].second;
							break;
						}
					}

					sum+=LLL[id-1].second*time_required;
					one_pair.second.int_vector.push_back(l);
					//------------------------------------------------update and sort
					int type=is_KD_PD(d);
					bool is_kp=is_Key_PKG(d);
					bool is_kpp=is_Key_PIN_PKG(d);
					/*
					if(type==1){
						bool flag=false;
						for(int id2=id+1;id2<=(int)LLL.size();id2++){
							int l2=LLL[id2-1].first;
							int d2=lots_device[l2];
							if(d2==d){
								int s2=max_rate_s(machine_id,l2,Lamuda_id);
								double time2=n_l_chips[l2]/rate[s2];
								if(production[d-1]<n_k_minchips[d]){
									int diff=n_k_minchips[d]-production[d-1];
									if(diff<n_l_chips[l2]){
										LLL[id2-1].second=(w_l[l2]+penalty[d2]/coef*diff)/time2;
									}else{
										LLL[id2-1].second=(w_l[l2]+penalty[d2]/coef*n_l_chips[l2])/time2;
									}
								}else{
									LLL[id2-1].second=w_l[l2]/time2;
								}
								flag=true;
							}
						}//end_for
						if(flag){
							sort(LLL.begin()+id,LLL.end(),compare_lot);
						}
					}else if(type==2){
						bool flag=false;
						for(int id2=id+1;id2<=(int)LLL.size();id2++){
							int l2=LLL[id2-1].first;
							int d2=lots_device[l2];
							if(d2==d){
								int s2=max_rate_s(machine_id,l2,Lamuda_id);
								double time2=n_l_chips[l2]/rate[s2];
								if(production[d-1]<n_p_minchips[d]){
									int diff=n_p_minchips[d]-production[d-1];
									if(diff<n_l_chips[l2]){
										LLL[id2-1].second=(w_l[l2]+penalty[d2]/coef*diff)/time2;
									}else{
										LLL[id2-1].second=(w_l[l2]+penalty[d2]/coef*n_l_chips[l2])/time2;
									}
								}else{
									LLL[id2-1].second=w_l[l2]/time2;
								}
								flag=true;
							}
						}//end_for
						if(flag){
							sort(LLL.begin()+id,LLL.end(),compare_lot);
						}
					}//end_if
					*/

					if(type==1||is_kp||is_kpp){
						bool flag=false;
						for(int id2=id+1;id2<=(int)LLL.size();id2++){
							int l2=LLL[id2-1].first;
							int d2=lots_device[l2];
							int s2=max_rate_s(machine_id,l2,Lamuda_id);
							double time2=n_l_chips[l2]/rate[s2];
							LLL[id2-1].second=w_l[l2]/time2;
							if(type==1&&d2==d){
								if(production[d-1]<n_k_minchips[d]){
									int diff=n_k_minchips[d]-production[d-1];
									if(diff<n_l_chips[l2]){
										LLL[id2-1].second+=(penalty[d2]/coef*diff)/time2;
									}else{
										LLL[id2-1].second+=(penalty[d2]/coef*n_l_chips[l2])/time2;
									}
									flag=true;
								}
							}
							if(is_kp && pkg_id==device_PKG[d2]){
								if(PKG_production[pkg_id-1]<n_kp_minchips[pkg_id]){
									int diff=n_kp_minchips[pkg_id]-PKG_production[pkg_id-1];
									if(diff<n_l_chips[l2]){
										LLL[id2-1].second+=(PKG_penalty[pkg_id]/coef*diff)/time2;
									}else{
										LLL[id2-1].second+=(PKG_penalty[pkg_id]/coef*n_l_chips[l2])/time2;
									}
									flag=true;
								}
							}
							if(is_kpp && pkg_id==device_PKG[d2] && pin_num==device_PIN[d2]){
								if(PIN_PKG_production[pin_pkg_index]<target){
									int diff=target-PIN_PKG_production[pin_pkg_index];
									if(diff<n_l_chips[l2]){
										LLL[id2-1].second+=(get_PIN_PKG_penalty(pkg_id,pin_num)/coef*diff)/time2;
									}else{
										LLL[id2-1].second+=(get_PIN_PKG_penalty(pkg_id,pin_num)/coef*n_l_chips[l2])/time2;
									}
									flag=true;
								}
							}
						}
						if(flag){
							sort(LLL.begin()+id,LLL.end(),compare_lot);
						}
					}
				}//end_if
			}
			one_pair.first=sum;
			sim_lamuda_benefit.push_back(one_pair);
			p_Lamuda++;
		}
	}
}

void AT::update_benefit(vector< pair<double,vec_int> > & sim_lamuda_benefit,vector<int> & lot_remain,vector<int> & production,vector<int> & affected_dev,int * tooling_used,int * sim_used,int * SIM_number){
	//cout<<"in update_benefit()"<<endl;
	int index=-1;
	/*
	cout<<"original benefit"<<endl;
	for(int sim=1;sim<=(int)SIM.size();sim++){
		int machine_id=SIM[sim-1].represented_machine_id;
		vector<int>::iterator p_Lamuda=Lamuda_i[machine_id].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[machine_id].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			index++;
			cout<<"index="<<index<<",sim="<<sim<<",Lamuda_id="<<Lamuda_id<<",benefit="<<sim_lamuda_benefit[index].first<<endl;
			p_Lamuda++;
		}
	}
	*/
	//------------copy production
	vector<int> prod((int)production.size(),0);
	copy(production.begin(),production.end(),prod.begin());
	vector<int> PKG_production_save((int)PKG.size(),0);
	vector<int> PIN_PKG_production_save((int)PKG.size()*(int)PIN_order.size()+1,0);
	for(int d=1;d<=Num_Device;d++){
		int pkg_id=device_PKG[d];
		int pin_num=device_PIN[d];
		vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);
		int pin_order=pfind-PIN_order.begin()+1;
		int index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;
		PKG_production_save[pkg_id-1]+=prod[d-1];
		PIN_PKG_production_save[index]+=prod[d-1];
	}
	//---------------------------------------------------------------------------------------------------------for LL
	vector< pair<int,double> > LL;
	for(int id=1;id<=(int)lot_remain.size();id++){
		int l=lot_remain[id-1];
		int d=lots_device[l];
		int type=is_KD_PD(d);
		int pkg_id=device_PKG[d];
		int pin_num=device_PIN[d];
		bool is_kp=is_Key_PKG(d);
		bool is_kpp=is_Key_PIN_PKG(d);
		pair<int,double> one_pair;
		one_pair.first=l;
		one_pair.second=w_l[l];
		if(type==1 && prod[d-1]<n_k_minchips[d]){
			int min=n_l_chips[l];
			if(min>n_k_minchips[d]-prod[d-1]){ min=n_k_minchips[d]-prod[d-1];}
			one_pair.second+=penalty[d]/coef*min;
		}else if(type==2 && prod[d-1]<n_p_minchips[d]){
			int min=n_l_chips[l];
			if(min>n_p_minchips[d]-prod[d-1]){ min=n_p_minchips[d]-prod[d-1];}
			one_pair.second+=penalty[d]/coef*min;
		}
		if(is_kp && PKG_production_save[pkg_id-1]<n_kp_minchips[pkg_id]){
			int diff=n_kp_minchips[pkg_id]-PKG_production_save[pkg_id-1];
			int min=n_l_chips[l];
			if(min>diff){ min=diff;}
			one_pair.second+=PKG_penalty[pkg_id]/coef*min;
		}
		int target=0;
		for(int id2=1;id2<=(int)Key_PIN_PKG.size();id2++){
			if(Key_PIN_PKG[id2-1].first.first==pin_num && Key_PIN_PKG[id2-1].first.second==pkg_id){
				target=Key_PIN_PKG[id2-1].second;
				break;
			}
		}
		vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);
		int pin_order=pfind-PIN_order.begin()+1;
		int index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;
		if(is_kpp && PIN_PKG_production_save[index]<target){
			int diff=target-PIN_PKG_production_save[index];
			int min=n_l_chips[l];
			if(min>diff){ min=diff;}
			one_pair.second+=get_PIN_PKG_penalty(pkg_id,pin_num)/coef*min;
		}
		LL.push_back(one_pair);
	}
	sort(LL.begin(),LL.end(),compare_lot);
	//------------------------------------------------------------------------------------------------
	int count=0;
	int count_limit=1E+6;
	index=-1;
	for(int sim=1;sim<=(int)SIM.size();sim++){
		int machine_id=SIM[sim-1].represented_machine_id;
		vector<int>::iterator p_Lamuda=Lamuda_i[machine_id].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[machine_id].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			index++;
			count++;
			//--------------------------------------------------------------------------------------------
			if(sim_lamuda_benefit[index].first<0){//previosly infeasible or non-benefitial setup
				p_Lamuda++; continue;
			}
			//---------------------------------------------------------------------------------------------------------test if sim_lamuda is feasible
			int test=is_sim_lamuda_feasible(sim,Lamuda_id,tooling_used,sim_used,SIM_number);
			if(!test){ 
				sim_lamuda_benefit[index].first=-1E+6;
				sim_lamuda_benefit[index].second.int_vector.clear();
				p_Lamuda++; 
				continue;
			}

			copy(production.begin(),production.end(),prod.begin());
			vector<int> PKG_production((int)PKG.size(),0);
			vector<int> PIN_PKG_production((int)PKG.size()*(int)PIN_order.size()+1,0);
			copy(PKG_production_save.begin(),PKG_production_save.end(),PKG_production.begin());
			copy(PIN_PKG_production_save.begin(),PIN_PKG_production_save.end(),PIN_PKG_production.begin());
			//---------------------------------------------------------------------------------------------------------check if affected dev
			/*
			bool affected=false;
			for(int id=1;id<=(int)sim_lamuda_benefit[index].second.int_vector.size();id++){
				int l=sim_lamuda_benefit[index].second.int_vector[id-1];
				int d=lots_device[l];
				vii pfind=find(affected_dev.begin(),affected_dev.end(),d);
				if(pfind!=affected_dev.end()){
					affected=true;
					break;
				}
			}
			if(!affected){ p_Lamuda++; continue;}
			*/
			//--------------------------------------------------------if affected, clear
			sim_lamuda_benefit[index].second.int_vector.clear();
			sim_lamuda_benefit[index].first=0;
			//---------------------------------------------------------------------------------------------------------for LLL
			vector< pair<int,double> > LLL;
			for(int id=1;id<=(int)LL.size();id++){
				int l=LL[id-1].first;
				vii pfind=find(L_sim_lamuda[index].second.int_vector.begin(),L_sim_lamuda[index].second.int_vector.end(),l);
				if(pfind!=L_sim_lamuda[index].second.int_vector.end()){
					int s=max_rate_s(machine_id,l,Lamuda_id);
					double time_required=n_l_chips[l]/rate[s];
					double ben=LL[id-1].second/time_required;
					LLL.push_back(make_pair(l,ben));
				}
			}
			sort(LLL.begin(),LLL.end(),compare_lot);
			//---------------------------------------------------------------------------------------------------------for LLL
			double sum=0;
			double time_used=0;
			for(int id=1;id<=(int)LLL.size();id++){
				int l=LLL[id-1].first;
				int d=lots_device[l];
				int s=max_rate_s(machine_id,l,Lamuda_id);
				double time_required=n_l_chips[l]/rate[s];
				if(time_used+time_required<=H[machine_id]){
					time_used+=time_required;
					sum+=LLL[id-1].second*time_required;
					sim_lamuda_benefit[index].second.int_vector.push_back(l);
					prod[d-1]+=n_l_chips[l];
					int pkg_id=device_PKG[d];
					int pin_num=device_PIN[d];
					PKG_production[pkg_id-1]+=n_l_chips[l];
					vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);
					int pin_order=pfind-PIN_order.begin()+1;
					int pin_pkg_index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;
					PIN_PKG_production[pin_pkg_index]+=n_l_chips[l];
					int target=0;
					for(int id2=1;id2<=(int)Key_PIN_PKG.size();id2++){
						if(Key_PIN_PKG[id2-1].first.first==pin_num && Key_PIN_PKG[id2-1].first.second==pkg_id){
							target=Key_PIN_PKG[id2-1].second;
							break;
						}
					}
					//------------------------------------------------update and sort
					int type=is_KD_PD(d);
					bool is_kp=is_Key_PKG(d);
					bool is_kpp=is_Key_PIN_PKG(d);
					/*
					if(type==1){
						bool flag=false;
						int shortage=n_k_minchips[d]-prod[d-1];
						for(int id2=id+1;id2<=(int)LLL.size();id2++){
							int l2=LLL[id2-1].first;
							int d2=lots_device[l2];
							if(d2==d && shortage<n_l_chips[l2]){
								int s2=max_rate_s(machine_id,l2,Lamuda_id);
								double time2=n_l_chips[l2]/rate[s2];
								if(shortage>0){
									LLL[id2-1].second=(w_l[l2]+penalty[d2]/coef*shortage)/time2;
									flag=true;
								}else if(shortage<0 && shortage+n_l_chips[l]>0){
									LLL[id2-1].second=w_l[l2]/time2;
									flag=true;
								}
							}
						}//end_for
						if(flag){
							sort(LLL.begin()+id,LLL.end(),compare_lot);
						}
					}else if(type==2){
						bool flag=false;
						int shortage=n_p_minchips[d]-prod[d-1];
						for(int id2=id+1;id2<=(int)LLL.size();id2++){
							int l2=LLL[id2-1].first;
							int d2=lots_device[l2];
							if(d2==d && shortage<n_l_chips[l2]){
								int s2=max_rate_s(machine_id,l2,Lamuda_id);
								double time2=n_l_chips[l2]/rate[s2];
								if(shortage>0){
									LLL[id2-1].second=(w_l[l2]+penalty[d2]/coef*shortage)/time2;
									flag=true;
								}else if(shortage<0 && shortage+n_l_chips[l]>0){
									LLL[id2-1].second=w_l[l2]/time2;
									flag=true;
								}
							}
						}//end_for
						if(flag){
							sort(LLL.begin()+id,LLL.end(),compare_lot);
						}
					}//end_if
					*/
					if(type==1||is_kp||is_kpp){
						bool flag=false;
						for(int id2=id+1;id2<=(int)LLL.size();id2++){
							int l2=LLL[id2-1].first;
							int d2=lots_device[l2];
							int s2=max_rate_s(machine_id,l2,Lamuda_id);
							double time2=n_l_chips[l2]/rate[s2];
							LLL[id2-1].second=w_l[l2]/time2;
							if(type==1&&d2==d){
								if(production[d-1]<n_k_minchips[d]){
									int diff=n_k_minchips[d]-production[d-1];
									if(diff<n_l_chips[l2]){
										LLL[id2-1].second+=(penalty[d2]/coef*diff)/time2;
									}else{
										LLL[id2-1].second+=(penalty[d2]/coef*n_l_chips[l2])/time2;
									}
									flag=true;
								}
							}
							if(is_kp && pkg_id==device_PKG[d2]){
								if(PKG_production[pkg_id-1]<n_kp_minchips[pkg_id]){
									int diff=n_kp_minchips[pkg_id]-PKG_production[pkg_id-1];
									if(diff<n_l_chips[l2]){
										LLL[id2-1].second+=(PKG_penalty[pkg_id]/coef*diff)/time2;
									}else{
										LLL[id2-1].second+=(PKG_penalty[pkg_id]/coef*n_l_chips[l2])/time2;
									}
									flag=true;
								}
							}
							if(is_kpp && pkg_id==device_PKG[d2] && pin_num==device_PIN[d2]){
								if(PIN_PKG_production[pin_pkg_index]<target){
									int diff=target-PIN_PKG_production[pin_pkg_index];
									if(diff<n_l_chips[l2]){
										LLL[id2-1].second+=(get_PIN_PKG_penalty(pkg_id,pin_num)/coef*diff)/time2;
									}else{
										LLL[id2-1].second+=(get_PIN_PKG_penalty(pkg_id,pin_num)/coef*n_l_chips[l2])/time2;
									}
									flag=true;
								}
							}
						}
						if(flag){
							sort(LLL.begin()+id,LLL.end(),compare_lot);
						}		
					//------------------------------------------------------------------------------------
					}//end_if
				}
			}//end_for
			sim_lamuda_benefit[index].first=sum;
			p_Lamuda++;
			if(count>count_limit){ break;}
		}
		if(count>count_limit){ break;}
	}
	//cout<<"count="<<count<<endl;

	/*
	cout<<"updated benefit"<<endl;
	index=-1;
	for(int sim=1;sim<=(int)SIM.size();sim++){
		int machine_id=SIM[sim-1].represented_machine_id;
		vector<int>::iterator p_Lamuda=Lamuda_i[machine_id].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[machine_id].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			index++;
			cout<<"index="<<index<<",sim="<<sim<<",Lamuda_id="<<Lamuda_id<<",benefit="<<sim_lamuda_benefit[index].first<<endl;
			//for(int id=1;id<=(int)sim_lamuda_benefit[index].second.int_vector.size();id++){
			//	cout<<sim_lamuda_benefit[index].second.int_vector[id-1]<<",";
			//}
			//cout<<endl;
			p_Lamuda++;
		}
	}
	*/
	//cout<<"end of update_benefit()"<<endl;
}

void AT::update_benefit2(vector< pair<double,vec_int> > & sim_lamuda_benefit,vector<int> & lot_remain,vector<int> & production
						 ,vector<int> & affected_dev,int * tooling_used,int * sim_used,int * SIM_number,OsiXxxSolverInterface & si_KP){
	//cout<<"in update_benefit2()"<<endl;
	int index=-1;
	/*
	cout<<"original benefit"<<endl;
	for(int sim=1;sim<=(int)SIM.size();sim++){
		int machine_id=SIM[sim-1].represented_machine_id;
		vector<int>::iterator p_Lamuda=Lamuda_i[machine_id].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[machine_id].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			index++;
			cout<<"index="<<index<<",sim="<<sim<<",Lamuda_id="<<Lamuda_id<<",benefit="<<sim_lamuda_benefit[index].first<<endl;
			p_Lamuda++;
		}
	}
	*/
	//------------------------------------------------------------------------------------------------
	double * solution = new double[Num_Lot+Num_Keydevice+Num_Package];
	index=-1;
	for(int sim=1;sim<=(int)SIM.size();sim++){
		int machine_id=SIM[sim-1].represented_machine_id;
		vector<int>::iterator p_Lamuda=Lamuda_i[machine_id].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[machine_id].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			index++;
			//--------------------------------------------------------------------------------------------
			if(sim_lamuda_benefit[index].first<0){//previosly infeasible or non-benefitial setup
				p_Lamuda++; continue;
			}
			//---------------------------------------------------------------------------------------------------------test if sim_lamuda is feasible
			int test=is_sim_lamuda_feasible(sim,Lamuda_id,tooling_used,sim_used,SIM_number);
			if(!test){ 
				sim_lamuda_benefit[index].first=-1E+6;
				sim_lamuda_benefit[index].second.int_vector.clear();
				p_Lamuda++; 
				continue;
			}
			//---------------------------------------------------------------------------------------------------------check if affected dev
			bool affected=false;
			for(int id=1;id<=(int)sim_lamuda_benefit[index].second.int_vector.size();id++){
				int l=sim_lamuda_benefit[index].second.int_vector[id-1];
				int d=lots_device[l];
				vii pfind=find(affected_dev.begin(),affected_dev.end(),d);
				if(pfind!=affected_dev.end()){
					affected=true;
					break;
				}
			}
			if(!affected){ p_Lamuda++; continue;}
			//--------------------------------------------------------if affected, clear
			sim_lamuda_benefit[index].second.int_vector.clear();
			sim_lamuda_benefit[index].first=0;
			//---------------------------------------------------------------------------------------------------------Apply LP to solve the KP problem
			for(int k=1;k<=Num_Keydevice;k++){
				vector<int>::iterator p_int=Key_Devices.begin()+k-1;
				int d=(*p_int);
				int sh=n_k_minchips[d]-production[d-1];
				si_KP.setRowBounds(k-1,sh,DBL_MAX);
			}
			for(int p=1;p<=Num_Package;p++){
				vector<int>::iterator p_int=Packages.begin()+p-1;
				int d=(*p_int);
				int sh=n_p_minchips[d]-production[d-1];
				si_KP.setRowBounds(Num_Keydevice+p-1,sh,DBL_MAX);
			}
			CoinPackedVector row;
			vector<int> id_vec;
			for(int id=1;id<=(int)lot_remain.size();id++){
				int l=lot_remain[id-1];
				vii pfind=find(L_sim_lamuda[index].second.int_vector.begin(),L_sim_lamuda[index].second.int_vector.end(),l);
				if(pfind==L_sim_lamuda[index].second.int_vector.end()){//lot l cannot be processed by (sim,Lamuda_id) combination
					si_KP.setColBounds(l-1,0.0,0.0);
					id_vec.push_back(l-1);
				}else{//lot l can be processed
					int s=max_rate_s(machine_id,l,Lamuda_id);
					double time_required=n_l_chips[l]/rate[s];
					row.insert(l-1,time_required);
				}
			}
			si_KP.addRow(row,-DBL_MAX,H[machine_id]);
			si_KP.setHintParam(OsiDoDualInResolve,false,OsiHintDo);
			si_KP.writeLp("update_model");
			si_KP.resolve();
			if(si_KP.isProvenOptimal()){
				vector<int> prod(Num_Device,0);
				double benefit=0.0;
				CoinDisjointCopyN(si_KP.getColSolution(), Num_Lot+Num_Keydevice+Num_Package, solution);
				for(int l=1;l<=Num_Lot;l++){
					if(solution[l-1]>1.0-1E-06){//truncate the fractional solutions
						int d=lots_device[l];
						//cout<<"sim="<<sim<<",Lamuda_id="<<Lamuda_id<<",lot="<<l<<",w_l="<<w_l[l]<<"d,="<<d<<",type="<<is_KD_PD(d)<<endl;
						int s=max_rate_s(machine_id,l,Lamuda_id);
						benefit+=w_l[l]-eps[s];
						prod[d-1]+=n_l_chips[l];
						sim_lamuda_benefit[index].second.int_vector.push_back(l);
					}
				}
				for(int d=1;d<=Num_Device;d++){
					if(prod[d-1]>0){
						int type=is_KD_PD(d);
						if(type==1){
							int sh=n_k_minchips[d]-production[d-1];
							if(prod[d-1]<sh){ benefit+=penalty[d]/coef*prod[d-1];}
							else if(sh>0){ benefit+=penalty[d]/coef*sh;}
						}else if(type==2){
							int sh=n_p_minchips[d]-production[d-1];
							if(prod[d-1]<sh){ benefit+=penalty[d]/coef*prod[d-1];}
							else if(sh>0){ benefit+=penalty[d]/coef*sh;}
						}
					}
				}
				//double KP_obj=si_KP.getObjValue();
				//cout<<"sim="<<sim<<",Lamuda_id="<<Lamuda_id<<",KP_obj="<<KP_obj<<",benefit="<<benefit<<endl;
				sim_lamuda_benefit[index].first=benefit;
			}else{
				cout<<"Warning!!! the KP cannot be solved optiamlly by CLP"<<endl;
				abort();
			}
			//-----------------------------------------------------------------------------------------------------------recover the bounds and delete the constriant
			for(int id=1;id<=(int)id_vec.size();id++){
				int ind=id_vec[id-1];
				si_KP.setColBounds(ind,0.0,1.0);
			}
			int ind=si_KP.getNumRows()-1;
			si_KP.deleteRows(1,&ind);
			//-----------------------------------------------------------------------------------------------------------
			p_Lamuda++;
		}
	}
	/*
	cout<<"updated benefit2"<<endl;
	index=-1;
	for(int sim=1;sim<=(int)SIM.size();sim++){
		int machine_id=SIM[sim-1].represented_machine_id;
		vector<int>::iterator p_Lamuda=Lamuda_i[machine_id].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[machine_id].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			index++;
			cout<<"index="<<index<<",sim="<<sim<<",Lamuda_id="<<Lamuda_id<<",benefit="<<sim_lamuda_benefit[index].first<<endl;
			p_Lamuda++;
		}
	}
	*/
	//cout<<"end of update_benefit2()"<<endl;
	delete [] solution;
}

void AT::initialize_L_sim_lamuda(){
	L_sim_lamuda.clear();
	for(int sim=1;sim<=(int)SIM.size();sim++){
		int i=SIM[sim-1].represented_machine_id;
		vector<int>::iterator p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			vector<int> LS;
			get_L_i_lamuda(LS,i,Lamuda_id);
			pair< pair<int,int>,vec_int> one_pair;
			one_pair.first.first=sim;
			one_pair.first.second=Lamuda_id;
			one_pair.second.int_vector.resize((int)LS.size());
			copy(LS.begin(),LS.end(),one_pair.second.int_vector.begin());
			L_sim_lamuda.push_back(one_pair);
			p_Lamuda++;
		}
	}
}

void AT::show_L_sim_lamuda(){
	ofstream fdebug_L_i_lamuda("Debug/debug_L_i_lamuda.txt");
	vector< pair< pair<int,int>,vec_int> >::iterator pLS=L_sim_lamuda.begin();
	while(pLS!=L_sim_lamuda.end()){
		fdebug_L_i_lamuda<<"sim="<<(*pLS).first.first<<",Lamuda_id="<<(*pLS).first.second<<",lots:";
		vii p_lot=(*pLS).second.int_vector.begin();
		while(p_lot!=(*pLS).second.int_vector.end()){
			fdebug_L_i_lamuda<<(*p_lot)<<",";
			p_lot++;
		}
		fdebug_L_i_lamuda<<endl;
		pLS++;
	}
	fdebug_L_i_lamuda.close();
}

double AT::second_level_heuristic(double * solution,vector<sub_soln_item> & sub_soln,vector< pair<int,double> > & shortage
								  ,vector< pair< pair<int,double>,double> > & unassigned_lots,int option){
	//given the LP solution
	//-----------------------------debug the LP solution (second level)
	/*
	ofstream fdebugLP("debugLP.txt");
	for(int id=0;id<=x_limit_first_level;id++){
		vector<index_node>::iterator p_node=first_level_index_vector.begin()+id;
		int i=(*p_node).i;
		int l=(*p_node).l;
		int s=(*p_node).s;
		fdebugLP<<"i="<<i<<",l="<<l<<",s="<<s<<",x="<<solution[id]<<endl;
	}
	for(int id=x_limit_first_level+Num_Keydevice+Num_Package+1;id<=y_limit_first_level;id++){
		vector<index_node>::iterator p_node=first_level_index_vector.begin()+id;
		int i=(*p_node).i;
		int l=(*p_node).l;
		int Lamuda_id=(*p_node).Lamuda_id;
		fdebugLP<<"i="<<i<<",l="<<l<<",Lamuda_id="<<Lamuda_id<<",y="<<solution[id]<<endl;
	}
	fdebugLP.close();
	*/
	//initialize sub_soln
	//< < <i,Lamuda_id>,time_used>,lot1,lot2,...>
	for(int i=1;i<=Num_Machine;i++){
		sub_soln_item one_item;
		one_item.i=i;
		one_item.Lamuda_id=-1;
		one_item.time_used=0.0;
		sub_soln.push_back(one_item);
	}
	//initialize assigned_lots and shortage
	vector<int> assigned_lots;
	for(int d=1;d<=Num_Device;d++){
		vii pfind=find(Key_Devices.begin(),Key_Devices.end(),d);
		if(pfind!=Key_Devices.end()){//d is a key device
			shortage.push_back(make_pair(d,n_k_minchips[d]));
		}else{
			vii pfind=find(Packages.begin(),Packages.end(),d);
			if(pfind!=Packages.end()){//d is a package device
				shortage.push_back(make_pair(d,n_p_minchips[d]));
			}else{//d is neither a key nor package device
				shortage.push_back(make_pair(d,-1E+10));
			}
		}
	}
	//construct sub_soln,assigned_lots,and shortage
	for(int id=0;id<=x_limit;id++){
		if(solution[id]>1.0-1E-06){
			vector<index_node>::iterator p_node=index_vector.begin()+id;
			int l=(*p_node).l;
			int i=(*p_node).i;
			int s=(*p_node).s;
			sub_soln[i-1].lots.int_vector.push_back(l);
			sub_soln[i-1].time_used+=n_l_chips[l]/rate[s];
			assigned_lots.push_back(l);
			int dev=lots_device[l];
			shortage[dev-1].second-=n_l_chips[l];
		}
	}
/*
	cout<<"assgined_lots:"<<endl;
	for(int id=1;id<=assigned_lots.size();id++){
		cout<<assigned_lots[id-1]<<endl;
	}
*/
	for(int id=x_limit+Num_Keydevice+Num_Package+(int)Key_PKG.size()+(int)Key_PIN_PKG.size()+1;id<=y_limit;id++){
		if(solution[id]>1.0-1E-06){
			vector<index_node>::iterator p_node=index_vector.begin()+id;
			int i=(*p_node).i;
			int Lamuda_id=(*p_node).Lamuda_id;
			sub_soln[i-1].Lamuda_id=Lamuda_id;
		}
	}
	//get the KP and KPP production
	vector<int> PKG_production((int)PKG.size(),0);
	vector<int> PIN_PKG_production((int)PKG.size()*(int)PIN_order.size()+1,0);
	for(int id=1;id<=(int)assigned_lots.size();id++){
		int l=assigned_lots[id-1];
		int d=lots_device[l];
		int pkg_id=device_PKG[d];
		int pin_num=device_PIN[d];
		vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);
		int pin_order=pfind-PIN_order.begin()+1;
		int index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;
		PKG_production[pkg_id-1]+=n_l_chips[l];
		PIN_PKG_production[index]+=n_l_chips[l];
	}
	//initialze and construct unassigned_lots
	//<<lot,contribution>,unit_price>
	unassigned_lots.resize(Num_Lot);
	for(int l=1;l<=Num_Lot;l++){
		unassigned_lots[l-1].first.first=l;
		unassigned_lots[l-1].first.second=w_l[l];
		unassigned_lots[l-1].second=w_l[l]/n_l_chips[l];
	}
	vector< pair< pair<int,double>,double> >::iterator p_lot=unassigned_lots.begin();
	while(p_lot!=unassigned_lots.end()){
		int l=(*p_lot).first.first;
		vii pfind=find(assigned_lots.begin(),assigned_lots.end(),l);
		if(pfind!=assigned_lots.end()){
			p_lot=unassigned_lots.erase(p_lot);
		}else{
			int dev=lots_device[l];
			bool is_kp=is_Key_PKG(dev);
			bool is_kpp=is_Key_PIN_PKG(dev);
			int pkg_id=device_PKG[dev];
			int pin_num=device_PIN[dev];
			vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);
			int pin_order=pfind-PIN_order.begin()+1;
			int index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;

			int min=shortage[dev-1].second;
			if(min>n_l_chips[l]){ min=n_l_chips[l];}
			if(min>0){ 
				(*p_lot).first.second+=penalty[dev]/coef*min;
				(*p_lot).second+=penalty[dev]/coef*min/n_l_chips[l];
			}
			if(is_kp){
				int diff=n_kp_minchips[pkg_id]-PKG_production[pkg_id-1];
				int min=diff;
				if(min>n_l_chips[l]){ min=n_l_chips[l];}
				if(min>0){ 
					(*p_lot).first.second+=PKG_penalty[pkg_id]/coef*min;
					(*p_lot).second+=PKG_penalty[pkg_id]/coef*min/n_l_chips[l];
				}
			}
			if(is_kpp){
				int target=0;
				for(int id=1;id<=(int)Key_PIN_PKG.size();id++){
					if(Key_PIN_PKG[id-1].first.first==pin_num && Key_PIN_PKG[id-1].first.second==pkg_id){ target=Key_PIN_PKG[id-1].second; break;}
				}
				int diff=target-PIN_PKG_production[index];
				int min=diff;
				if(min>n_l_chips[l]){ min=n_l_chips[l];}
				if(min>0){ 
					(*p_lot).first.second+=get_PIN_PKG_penalty(pkg_id,pin_num)/coef*min;
					(*p_lot).second+=get_PIN_PKG_penalty(pkg_id,pin_num)/coef*min/n_l_chips[l];
				}
			}
			p_lot++;
		}
	}
	sort(unassigned_lots.begin(),unassigned_lots.end(),compare_2_3);
	//dispay section
	cout<<"Number of unassigned lots="<<(int)unassigned_lots.size()<<endl;
/*
	p_lot=unassigned_lots.begin();
	while(p_lot!=unassigned_lots.end()){
		cout<<"("<<(*p_lot).first.first<<","<<(*p_lot).first.second<<","<<(*p_lot).second<<") "<<endl;
		p_lot++;
	}
*/
	//cout<<endl;
	//cout<<"Number of assigned lots="<<(int)assigned_lots.size()<<endl;
	//cout<<"obj before N1="<<compute_obj(sub_soln,shortage)<<endl;
/*
	cout<<"before any local search"<<endl;
	show_sub_soln(sub_soln);
	cout<<"show shortage:"<<endl;
	vector< pair<int,double> >::iterator p_shortage=shortage.begin();
	while(p_shortage!=shortage.end()){
		cout<<"dev="<<(*p_shortage).first<<",shortage="<<(*p_shortage).second<<endl;
		p_shortage++;
	}
*/
	//-------------------------------------------------------------------------------------------
	if(LP_IP==1){
		N1(sub_soln,shortage,unassigned_lots);
	}
/*
	cout<<"after N1"<<endl;
	cout<<"show shortage:"<<endl;
	p_shortage=shortage.begin();
	while(p_shortage!=shortage.end()){
		cout<<"dev="<<(*p_shortage).first<<",shortage="<<(*p_shortage).second<<endl;
		p_shortage++;
	}
	cout<<"obj="<<compute_obj(sub_soln,shortage)<<endl;
	show_sub_soln(sub_soln);
*/
	//cout<<"obj after N1="<<compute_obj(sub_soln,shortage)<<endl;
	if(option==2 && LP_IP==1){
		N2(sub_soln,shortage,unassigned_lots);
	}
	//cout<<"obj after N2="<<compute_obj(sub_soln,shortage)<<endl;

	if(option==3 && LP_IP==1){
		N2(sub_soln,shortage,unassigned_lots);
		N3(sub_soln,shortage,unassigned_lots);	
	}
/*
	cout<<"after N2"<<endl;
	cout<<"show shortage:"<<endl;
	p_shortage=shortage.begin();
	while(p_shortage!=shortage.end()){
		cout<<"dev="<<(*p_shortage).first<<",shortage="<<(*p_shortage).second<<endl;
		p_shortage++;
	}
	cout<<"obj="<<compute_obj(sub_soln,shortage)<<endl;
	show_sub_soln(sub_soln);
*/
	return compute_obj(sub_soln,shortage);
}

void AT::N1(vector<sub_soln_item> & sub_soln,vector< pair<int,double> > & shortage,vector< pair< pair<int,double>,double> > & unassigned_lots){
	cout<<"in local search N1()"<<endl;
	vector<int> PKG_production((int)PKG.size(),0);
	vector<int> PIN_PKG_production((int)PKG.size()*(int)PIN_order.size()+1,0);
	for(int id=1;id<=(int)sub_soln.size();id++){
		for(int id2=1;id2<=(int)sub_soln[id-1].lots.int_vector.size();id2++){
			int l=sub_soln[id-1].lots.int_vector[id2-1];
			int d=lots_device[l];
			int pkg_id=device_PKG[d];
			int pin_num=device_PIN[d];
			vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);
			int pin_order=pfind-PIN_order.begin()+1;
			int index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;

			PKG_production[pkg_id-1]+=n_l_chips[l];
			PIN_PKG_production[index]+=n_l_chips[l];
		}
	}
	vector< pair< pair<int,double>,double> >::iterator p_lot=unassigned_lots.begin();
	while(p_lot!=unassigned_lots.end()){
		int flag=0;
		int l=(*p_lot).first.first;
		//cout<<"observing lot "<<l<<endl;
		vector< pair<int,int> >::iterator p_pair=sim_lamuda_L[l-1].second.sim_lamuda_pairs.begin();
		while(p_pair!=sim_lamuda_L[l-1].second.sim_lamuda_pairs.end()){
			int sim=(*p_pair).first;
			int Lamuda_id=(*p_pair).second;
			int s=max_rate_s(SIM[sim-1].represented_machine_id,l,Lamuda_id);
			double time_required=n_l_chips[l]/rate[s];
			for(int id2=1;id2<=(int)sub_soln.size();id2++){
				int i=sub_soln[id2-1].i;
				if(sim==machine_sim[i] && Lamuda_id==sub_soln[id2-1].Lamuda_id 
					&& sub_soln[id2-1].time_used+time_required<=H[i]){
					//lot l can be assigned to this machine-lamuda combination
					//cout<<"insert lot "<<l<<" to machine "<<i<<endl;
					sub_soln[id2-1].lots.int_vector.push_back(l);
					sub_soln[id2-1].time_used+=time_required;
					int dev=lots_device[l];
					shortage[dev-1].second-=n_l_chips[l];
					p_lot=unassigned_lots.erase(p_lot);
					//cout<<"!!!"<<(*p_lot).first.first<<endl;
					//---------------------------------------------do update stuff
					/*
					int pkg_id=device_PKG[dev];
					int pin_num=device_PIN[dev];
					vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);
					int pin_order=pfind-PIN_order.begin()+1;
					int index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;
					PKG_production[pkg_id-1]+=n_l_chips[l];
					PIN_PKG_production[index]+=n_l_chips[l];
					cout<<"is_KD_PD="<<is_KD_PD(dev)<<",is_Key_PKG="<<is_Key_PKG(dev)<<",is_Key_PIN_PKG="<<is_Key_PIN_PKG(dev)<<endl;
					if(is_KD_PD(dev)==1 || is_Key_PKG(dev) || is_Key_PIN_PKG(dev)){
						int start=p_lot-unassigned_lots.begin()+1;
						for(int id3=start;id3<=(int)unassigned_lots.size();id3++){
							int l=unassigned_lots[id3-1].first.first;
							int d=lots_device[l];
							bool is_kp=is_Key_PKG(d);
							bool is_kpp=is_Key_PIN_PKG(d);
							int type=is_KD_PD(d);
							if(type!=1 && !is_kp && !is_kpp){ break;}
							unassigned_lots[id3-1].first.second=w_l[l]/n_l_chips[l];
							if(type==1 && dev==d){
								int min=n_l_chips[l];
								if(min>shortage[d-1].second){ min=shortage[d-1].second;}
								if(min>0){
									unassigned_lots[id3-1].first.second+=penalty[d]*min/coef/n_l_chips[l];
								}
							}
							if(is_kp && pkg_id==device_PKG[d]){
								int min=n_l_chips[l];
								int diff=n_kp_minchips[pkg_id]-PKG_production[pkg_id-1];
								if(min>diff){ min=diff;}
								if(min>0){
									unassigned_lots[id3-1].first.second+=PKG_penalty[pkg_id]*min/coef/n_l_chips[l];
								}
							}
							if(is_kpp && pkg_id==device_PKG[d] && pin_num==device_PIN[d]){
								int min=n_l_chips[l];
								int target=0;
								for(int id4=1;id4<=(int)Key_PIN_PKG.size();id4++){
									if(Key_PIN_PKG[id4-1].first.first==pin_num && Key_PIN_PKG[id4-1].first.second==pkg_id){
										target=Key_PIN_PKG[id4-1].second; break;
									}
								}
								int diff=target-PIN_PKG_production[index];
								if(min>diff){ min=diff;}
								cout<<"min="<<min<<endl;
								if(min>0){
									unassigned_lots[id3-1].first.second+=get_PIN_PKG_penalty(pkg_id,pin_num)*min/coef/n_l_chips[l];
								}
							}
							sort(unassigned_lots.begin()+start-1,unassigned_lots.end(),compare_2_3);
						}
					}
					*/
					//---------------------------------------------
					flag=1;
					break;
				}//end_if
			}//end_for
			if(flag){ break;}
			p_pair++;
		}//end_while
		if(!flag){
			p_lot++;
		}
	}//end_while
}

void AT::N2(vector<sub_soln_item> & sub_soln,vector< pair<int,double> > & shortage,vector< pair< pair<int,double>,double> > & unassigned_lots){
	cout<<"in local search N2()"<<endl;
	//compute PKG and PIN_PKG production
	vector<int> PKG_production((int)PKG.size(),0);
	vector<int> PIN_PKG_production((int)PKG.size()*(int)PIN_order.size()+1,0);
	for(int id=1;id<=(int)sub_soln.size();id++){
		for(int id2=1;id2<=(int)sub_soln[id-1].lots.int_vector.size();id2++){
			int l=sub_soln[id-1].lots.int_vector[id2-1];
			int d=lots_device[l];
			int pkg_id=device_PKG[d];
			int pin_num=device_PIN[d];
			vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);
			int pin_order=pfind-PIN_order.begin()+1;
			int index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;

			PKG_production[pkg_id-1]+=n_l_chips[l];
			PIN_PKG_production[index]+=n_l_chips[l];
		}
	}
	bool iter_flag=true;
	int count=0;
	while(iter_flag && count<=2){
		iter_flag=false;
		vector< pair< pair<int,double>,double> >::iterator p_lot=unassigned_lots.begin();
		while(p_lot!=unassigned_lots.end()){
			int l_from=(*p_lot).first.first;
			int d_from=lots_device[l_from];
			//cout<<"l_from="<<l_from<<endl;
			int sim_best=-1;
			int i_best=-1;
			int Lamuda_id_best=-1;
			int l_to_best=-1;
			double ben_best=1E+10;
			vector< pair<int,int> >::iterator p_pair=sim_lamuda_L[l_from-1].second.sim_lamuda_pairs.begin();
			while(p_pair!=sim_lamuda_L[l_from-1].second.sim_lamuda_pairs.end()){
				int sim=(*p_pair).first;
				int Lamuda_id=(*p_pair).second;
				//cout<<"1, Lamuda_id="<<Lamuda_id<<endl;
				int s_from=max_rate_s(SIM[sim-1].represented_machine_id,l_from,Lamuda_id);
				//cout<<"sim="<<sim<<",Lamuda_id="<<Lamuda_id<<",s_from="<<s_from<<endl;
				double time_required=n_l_chips[l_from]/rate[s_from];
				for(int id2=1;id2<=(int)sub_soln.size();id2++){
					int i=sub_soln[id2-1].i;
					if(sim==machine_sim[i] && Lamuda_id==sub_soln[id2-1].Lamuda_id){
						//lot l_from can be assigned to this machine-lamuda combination
						vii pint=sub_soln[id2-1].lots.int_vector.begin();
						pint=sub_soln[id2-1].lots.int_vector.begin();
						while(pint!=sub_soln[id2-1].lots.int_vector.end()){
							int l_to=(*pint);  //this is the lot to be exchange
							int d_to=lots_device[l_to];
							//cout<<"2, Lamuda_id="<<Lamuda_id<<endl;
							int s_to=max_rate_s(i,l_to,Lamuda_id);
							if(sub_soln[id2-1].time_used-n_l_chips[l_to]/rate[s_to]+time_required<=H[i]){
								double benefit=0.0;
								//cout<<"---------------"<<endl;
								//--------------
								vector< pair<int,double> > shortage_2((int)shortage.size());
								vector<int> PKG_production_2((int)PKG.size(),0);
								vector<int> PIN_PKG_production_2((int)PKG.size()*(int)PIN_order.size()+1,0);
								copy(shortage.begin(),shortage.end(),shortage_2.begin());
								copy(PKG_production.begin(),PKG_production.end(),PKG_production_2.begin());
								copy(PIN_PKG_production.begin(),PIN_PKG_production.end(),PIN_PKG_production_2.begin());
								//--------------
								//this is the benefit to loss
								benefit+=w_l[l_to]-eps[s_to];
								shortage_2[d_to-1].second+=n_l_chips[l_to];
								if(is_KD_PD(d_to)==1 && shortage_2[d_to-1].second>0){
									if(shortage_2[d_to-1].second-n_l_chips[l_to]<0){
										benefit+=penalty[d_to]*shortage_2[d_to-1].second/coef;
									}else{
										benefit+=penalty[d_to]*n_l_chips[l_to]/coef;
									}
								}
								//cout<<"benefit 1="<<benefit<<endl;
								bool is_kp_to=is_Key_PKG(d_to);
								bool is_kpp_to=is_Key_PIN_PKG(d_to);
								int pkg_id_to=device_PKG[d_to];
								int pin_num_to=device_PIN[d_to];
								vii pfind_to=find(PIN_order.begin(),PIN_order.end(),pin_num_to);
								int pin_order_to=pfind_to-PIN_order.begin()+1;

								if(is_kp_to){
									PKG_production_2[pkg_id_to-1]-=n_l_chips[l_to];
									int diff=n_kp_minchips[pkg_id_to]-PKG_production_2[pkg_id_to-1];
									int min=diff;
									if(min>n_l_chips[l_to]){ min=n_l_chips[l_to];}
									if(min>0){
										benefit+=PKG_penalty[pkg_id_to]*min/coef;
									}
								}
								//cout<<"benefit 2="<<benefit<<endl;
								if(is_kpp_to){
									int index=(pkg_id_to-1)*(int)PIN_order.size()+pin_order_to-1;
									PIN_PKG_production_2[index]-=n_l_chips[l_to];
									int target=0;
									for(int id3=1;id3<=(int)Key_PIN_PKG.size();id3++){
										if(Key_PIN_PKG[id3-1].first.first==pin_num_to && Key_PIN_PKG[id3-1].first.second==pkg_id_to){ 
											target=Key_PIN_PKG[id3-1].second; break;
										}
									}
									int diff=target-PIN_PKG_production_2[index];
									int min=diff;
									if(min>n_l_chips[l_to]){ min=n_l_chips[l_to];}
									if(min>0){
										benefit+=get_PIN_PKG_penalty(pkg_id_to,pin_num_to)*min/coef;
									}
								}
								//cout<<"benefit 3="<<benefit<<endl;
								//------------------------------------------------------------------------
								//this is the benefit to gain
								bool is_kp_from=is_Key_PKG(d_from);
								bool is_kpp_from=is_Key_PIN_PKG(d_from);
								int pkg_id_from=device_PKG[d_from];
								int pin_num_from=device_PIN[d_from];
								vii pfind_from=find(PIN_order.begin(),PIN_order.end(),pin_num_from);
								int pin_order_from=pfind_from-PIN_order.begin()+1;

								benefit-=(w_l[l_from]-eps[s_from]);
								if(is_KD_PD(d_from)==1){
									int min=shortage_2[d_from-1].second;
									if(min>n_l_chips[l_from]){ min=n_l_chips[l_from];}
									if(min>0){
										benefit-=penalty[d_from]*min/coef;
									}
								}
								shortage_2[d_from-1].second-=n_l_chips[l_from];
								//cout<<"benefit 4="<<benefit<<endl;

								if(is_kp_from){
									int temp=PKG_production_2[pkg_id_from-1]+n_l_chips[l_from];
									int diff=n_kp_minchips[pkg_id_from]-temp;
									int min=diff;
									if(min>n_l_chips[l_from]){ min=n_l_chips[l_from];}
									if(min>0){
										//cout<<"benefit="<<benefit<<endl;
										//cout<<PKG_penalty[pkg_id_from]<<","<<min<<","<<coef<<endl;
										//double temp=PKG_penalty[pkg_id_from]*min/coef;
										//cout<<"temp="<<temp<<endl;
										benefit-=PKG_penalty[pkg_id_from]*min/coef;
										//cout<<"benefit="<<benefit<<endl;
									}
									PKG_production_2[pkg_id_from-1]+=n_l_chips[l_from];
								}
								//cout<<"benefit 5="<<benefit<<endl;

								if(is_kpp_from){
									int index=(pkg_id_from-1)*(int)PIN_order.size()+pin_order_from-1;
									int temp=PIN_PKG_production_2[index]+n_l_chips[l_from];
									int target=0;
									for(int id3=1;id3<=(int)Key_PIN_PKG.size();id3++){
										if(Key_PIN_PKG[id3-1].first.first==pin_num_from && Key_PIN_PKG[id3-1].first.second==pkg_id_from){ 
											target=Key_PIN_PKG[id3-1].second; break;
										}
									}
									int diff=target-temp;
									int min=diff;
									if(min>n_l_chips[l_from]){ min=n_l_chips[l_from];}
									if(min>0){
										benefit-=(get_PIN_PKG_penalty(pkg_id_from,pin_num_from)*min/coef);
									}
									PIN_PKG_production_2[index]+=n_l_chips[l_from];
								}

								//cout<<"benefit 6="<<benefit<<endl;
								//cout<<"benefit="<<benefit<<endl;
								if(benefit<0 && benefit<ben_best){
									ben_best=benefit;
									sim_best=sim;
									i_best=i;
									Lamuda_id_best=Lamuda_id;
									l_to_best=l_to;
								}
								//cout<<"---------------"<<endl;
							}//end_if
							pint++;
						}
					}//end_if
				}//end_for
				p_pair++;
			}//end_while
			if(ben_best<-1){
				//cout<<"ben_best="<<ben_best<<endl;
				//cout<<"l_to_best="<<l_to_best<<endl;
				int d_to_best=lots_device[l_to_best];
				//---------------------------------------------------------------------
				shortage[d_from-1].second-=n_l_chips[l_from];

				int pkg_from=device_PKG[d_from];
				int pin_from=device_PIN[d_from];
				vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_from);
				int pin_from_order=pfind-PIN_order.begin()+1;
				int index_from=(pkg_from-1)*(int)PIN_order.size()+pin_from_order-1;
				PKG_production[pkg_from-1]+=n_l_chips[l_from];
				PIN_PKG_production[index_from]+=n_l_chips[l_from];

				int pkg_to=device_PKG[d_to_best];
				int pin_to=device_PIN[d_to_best];
				pfind=find(PIN_order.begin(),PIN_order.end(),pin_to);
				int pin_to_order=pfind-PIN_order.begin()+1;
				int index_to=(pkg_to-1)*(int)PIN_order.size()+pin_to_order-1;
				//-----------------------------------------------------------------------
				//update unassigned_lots
				iter_flag=true;
				count++;
				p_lot=unassigned_lots.erase(p_lot);
				pair< pair<int,double>,double> one_pair;
				one_pair.first.first=l_to_best;
				one_pair.first.second=w_l[l_to_best];
				if(is_KD_PD(d_to_best) && shortage[d_to_best-1].second+n_l_chips[l_to_best]>0){
					if(shortage[d_to_best-1].second>0){
						one_pair.first.second+=penalty[d_to_best]/coef*n_l_chips[l_to_best];
					}else{
						one_pair.first.second+=penalty[d_to_best]/coef*(shortage[d_to_best-1].second+n_l_chips[l_to_best]);
					}
				}
				if(is_Key_PKG(d_to_best) && PKG_production[pkg_to-1]-n_l_chips[l_to_best]<n_kp_minchips[pkg_to]){
					int diff=n_kp_minchips[pkg_to]-(PKG_production[pkg_to-1]-n_l_chips[l_to_best]);
					int min=diff;
					if(min>n_l_chips[l_to_best]){ min=n_l_chips[l_to_best];}
					one_pair.first.second+=PKG_penalty[pkg_to]*min/coef;
				}
				int target=0;
				for(int id=1;id<=(int)Key_PIN_PKG.size();id++){
					if(Key_PIN_PKG[id-1].first.first==pin_to && Key_PIN_PKG[id-1].first.second==pkg_to){
						target=Key_PIN_PKG[id-1].second; break;
					}
				}
				if(is_Key_PIN_PKG(d_to_best) && PIN_PKG_production[index_to]-n_l_chips[l_to_best]<target){
					int diff=target-(PIN_PKG_production[index_to]-n_l_chips[l_to_best]);
					int min=diff;
					if(min>n_l_chips[l_to_best]){ min=n_l_chips[l_to_best];}
					one_pair.first.second+=get_PIN_PKG_penalty(pkg_to,pin_to)*min/coef;
				}
				one_pair.second=one_pair.first.second/n_l_chips[l_to_best];
				unassigned_lots.push_back(one_pair);
				//update sub_soln
				vii pint=find(sub_soln[i_best-1].lots.int_vector.begin(),sub_soln[i_best-1].lots.int_vector.end(),l_to_best);
				if(pint!=sub_soln[i_best-1].lots.int_vector.end()){
					(*pint)=l_from;
				}else{
					cout<<"!!! error here"<<endl;
				}
				//cout<<"3,Lamuda_id_best="<<Lamuda_id_best<<endl;
				int s_to_best=max_rate_s(i_best,l_to_best,Lamuda_id_best);
				//cout<<"4,Lamuda_id_best="<<Lamuda_id_best<<endl;
				int s_from=max_rate_s(SIM[sim_best-1].represented_machine_id,l_from,Lamuda_id_best);
				sub_soln[i_best-1].time_used+=-n_l_chips[l_to_best]/rate[s_to_best]+n_l_chips[l_from]/rate[s_from];
				//update shortage
				shortage[d_to_best-1].second+=n_l_chips[l_to_best];

				PKG_production[pkg_to-1]-=n_l_chips[l_to_best];
				PIN_PKG_production[index_to]-=n_l_chips[l_to_best];
			}else{
				p_lot++;
			}
		}//end_while
	}//end_while
	sort(unassigned_lots.begin(),unassigned_lots.end(),compare_2_3);
	cout<<"N2 finished"<<endl;
}

void AT::N3(vector<sub_soln_item> & sub_soln,vector< pair<int,double> > & shortage,vector< pair< pair<int,double>,double> > & unassigned_lots){
	//extended insertion
	//cout<<"unassgined_lots";
	//for(int j=1;j<=(int)unassigned_lots.size();j++){ cout<<unassigned_lots[j-1].first.first<<",";}
	//cout<<endl;

	vector< pair< pair<int,double>,double> >::iterator p_lot=unassigned_lots.begin();
	while(p_lot!=unassigned_lots.end()){
		bool erase_flag=false;
		int l_from=(*p_lot).first.first;
		int d_from=lots_device[l_from];
		for(int i=1;i<=Num_Machine;i++){
			int Lamuda_id=sub_soln[i-1].Lamuda_id;
			//cout<<"l_from="<<l_from<<",d_from="<<d_from<<",i="<<i<<",Lamuda_id="<<Lamuda_id<<endl;

			if(Lamuda_id>0){
				int sim=machine_sim[i];
				int index=index_L_sim_lamuda(sim,Lamuda_id);
				vii pfind=find(L_sim_lamuda[index].second.int_vector.begin(),L_sim_lamuda[index].second.int_vector.end(),l_from);
				if(pfind==L_sim_lamuda[index].second.int_vector.end()){ continue;} 
				//-----------------------------------------------------------------------------------------
				int s_from=max_rate_s(i,l_from,Lamuda_id);
				double time_from=n_l_chips[l_from]/rate[s_from];
				//cout<<"s_from="<<s_from<<",time_from="<<time_from<<endl;
				if(time_from+sub_soln[i-1].time_used>H[i]){
					//-----------------------------------------------------------------------------------------
					vector< pair<int,double> > lots;
					lots.resize((int)sub_soln[i-1].lots.int_vector.size());
					for(int j=1;j<=(int)sub_soln[i-1].lots.int_vector.size();j++){
						lots[j-1].first=sub_soln[i-1].lots.int_vector[j-1];
						lots[j-1].second=0;
					}
					lots.push_back(make_pair(l_from,0));
					//-----------------------------------------------------------------------------------------
					vector<double> sh((int)shortage.size(),0);
					for(int j=1;j<=(int)shortage.size();j++){ 
						sh[j-1]=shortage[j-1].second;
					}
					//-----------------------------------------------------------------------------------------
					double ben_get=w_l[l_from]-eps[s_from];
					if(is_KD_PD(d_from) && sh[d_from-1]>0){
						if(sh[d_from-1]>n_l_chips[l_from]){
							ben_get+=penalty[d_from]/coef*n_l_chips[l_from];
						}else{
							ben_get+=penalty[d_from]/coef*sh[d_from-1];
						}
					}
					//cout<<"ben_get="<<ben_get<<endl;
					sh[d_from-1]-=n_l_chips[l_from];
					//-----------------------------------------------------------------------------------------
					for(int j=1;j<=(int)lots.size();j++){
						int l=lots[j-1].first;
						int s=max_rate_s(i,l,Lamuda_id);
						lots[j-1].second=w_l[l]-eps[s];
						int d=lots_device[l];
						if(is_KD_PD(d) && sh[d-1]+n_l_chips[l]>0){
							double min=n_l_chips[l];
							if(min>sh[d-1]+n_l_chips[l]){ min=sh[d-1]+n_l_chips[l];}
							lots[j-1].second+=penalty[d]/coef*min;
						}
					}
					sort(lots.begin(),lots.end(),compare_lot);
					//------------------------------------------------------------------------------------------
					double time_required=time_from+sub_soln[i-1].time_used;
					double ben_loss=lots[(int)lots.size()-1].second;
					vector<int> lot_removed;
					while(time_required>H[i] && ben_loss<ben_get){
						int l=lots[(int)lots.size()-1].first;
						int d=lots_device[l];
						int s=max_rate_s(i,l,Lamuda_id);
						lots.erase(lots.end()-1);
						lot_removed.push_back(l);
						//cout<<"l="<<l<<",d="<<d<<",sh="<<sh[d-1]<<",time_required="<<time_required<<",ben_loss="<<ben_loss<<endl;
						//--------------------------------------------------------------------------------------update the remove_ben
						sh[d-1]+=n_l_chips[l];
						if(is_KD_PD(d)){
							for(int j=1;j<=(int)lots.size();j++){
								int ll=lots[j-1].first;
								int dd=lots_device[ll];
								int ss=max_rate_s(i,ll,Lamuda_id);
								if(dd==d && sh[dd-1]+n_l_chips[ll]>0){
									lots[j-1].second=w_l[ll]-eps[ss];
									double min=n_l_chips[ll];
									if(min>sh[dd-1]+n_l_chips[ll]){ min=sh[dd-1]+n_l_chips[ll];}
									lots[j-1].second+=penalty[dd]/coef*min;
								}
							}
							sort(lots.begin(),lots.end(),compare_lot);
						}
						//--------------------------------------------------------------------------------------
						time_required-=n_l_chips[l]/rate[s];
						ben_loss+=lots[(int)lots.size()-1].second;
					}//end_while
					if(time_required<H[i]){
						sub_soln[i-1].lots.int_vector.clear();
						sub_soln[i-1].lots.int_vector.resize((int)lots.size());
						double time=0;
						for(int j=1;j<=(int)lots.size();j++){
							int l=lots[j-1].first;
							int s=max_rate_s(i,l,Lamuda_id);
							time+=n_l_chips[l]/rate[s];
							sub_soln[i-1].lots.int_vector[j-1]=l;
						}
						sub_soln[i-1].time_used=time;
						//cout<<"lot_removed=";
						for(int j=1;j<=(int)lot_removed.size();j++){
							pair< pair<int,double>,double> one_pair;
							int l2=lot_removed[j-1];
							//cout<<l2<<",";
							int d2=lots_device[l2];
							one_pair.first.first=l2;
							if(is_KD_PD(d2) && sh[d2-1]>0){
								double min=n_l_chips[l2];
								if(min>sh[d2-1]){ min=sh[d2-1];}
								one_pair.first.second=w_l[l2]+penalty[d2]/coef*min;
							}else{
								one_pair.first.second=w_l[l2];
							}
							one_pair.second=one_pair.first.second/n_l_chips[l2];
							unassigned_lots.push_back(one_pair);
						}
						//cout<<endl;
						for(int j=1;j<=(int)sh.size();j++){
							shortage[j-1].second=sh[j-1];
						}
						p_lot=unassigned_lots.erase(p_lot);
						erase_flag=true;
						break;
					}else{//it is not benefitial to assign to machine i
						continue;
					}
				}else{
					//simply insert lot l_from to machine i
					sub_soln[i-1].lots.int_vector.push_back(l_from);
					sub_soln[i-1].time_used+=time_from;
					shortage[d_from-1].second-=n_l_chips[l_from];
					p_lot=unassigned_lots.erase(p_lot);
					erase_flag=true;
					break;
				}//end_if
			}//end_if
		}//end_for_i
		//cout<<"erase_flag="<<erase_flag<<endl;
		if(!erase_flag){ p_lot++;}
	}
}

/*
void AT::N3(vector<sub_soln_item> & sub_soln,vector< pair<int,double> > & shortage,vector< pair< pair<int,double>,double> > & unassigned_lots){
	for(int i=1;i<=Num_Machine;i++){
		if(sub_soln[i-1].Lamuda_id>0){//the machine is used
			//remove the lots and place them into unassigned_lots,destruction
			double penalty=0.0;
			double org_contribution=0.0;
			vii p_lot=sub_soln[i-1].lots.int_vector.begin();
			while(p_lot!=sub_soln[i-1].lots.int_vector.end()){
				int l=(*p_lot);
				pair< pair<int,double>,double> one_pair;
				one_pair.first.first=l;
				//!!!one_pair.first.second+=n_l_chips[l];
				one_pair.second=w_l[l]/n_l_chips[l];
				unassigned_lots.push_back(one_pair);
				int d=lots_device[l];
				shortage[d-1].second+=n_l_chips[l];
				//--------------------------------------compute lost in obj
				org_contribution+=w_l[l];
				double org_shortage=shortage[d-1].second-n_l_chips[l];
				double new_shortage=shortage[d-1].second;
				if(new_shortage>0){
					if(org_shortage<0){
						penalty=new_shortage*penalty[d]/coef;
					}else{
						penalty=(new_shortage-org_shortage)*penalty[d]/coef;
					}
				}
				org_contribution-=penalty;
				p_lot++
			}
			sub_soln[i-1].clear();
			int Lamuda_save=sub_soln[i-1].Lamuda_id;
			sub_soln[i-1].Lamuda_id=-1;
			//----------------------------------------------------------construction
			int simid=machine_sim[i];
			vii p_Lamuda=Lamuda_i[i].int_vector.begin();
			while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
				int Lamuda_id=(*p_Lamuda);
				if(Lamuda_id==Lamuda_save){ continue;}
				vector< pair<int,double> > lot_to_assigne;
				int index=index_L_sim_lamuda(simid,Lamuda_id);
				pair< pair<int,double>,double> p_unassigned=unassigned_lots.begin();
				while(p_unassigned!=unassigned_lots.end()){
					int l=(*p_unassigned).first.first;
					vii pfind=find(L_sim_lamuda[index].second.int_vector.begin(),L_sim_lamuda[index].second.int_vector.end(),l);
					if(pfind!=L_sim_lamuda[index].second.int_vector.end()){
						//push lot l into lot_to_assign
						pair<int,double> one_pair;
						one_pair.first=l;
						int d=lots_device[l];
						vii p1=find(LPD.begin(),LPD.end(),l);
						if(p1!=LPD.end()){//it is a key device or package device
							if(shortage[d-1]<0){
								one_pair.second=w_l[l];
							}else{
								one_pair.second=w_l[l]+penalty[d]/coef*n_l_chips[l];
							}
						}else{//it is a normal device
							one_pair.second=w_l[l];
						}
						lot_to_assign.push_back(one_pair);
					}
					p_unassigned++;
				}
				sort(lot_to_assign.begin(),lot_to_assign.end(),compare_lot);
				//-------------------------------------------------------------------greedy assignment

				p_Lamuda++;
			}
		}
	}
}
*/

void AT::initialize_sim_lamuda_L(){
	//given a lot, find all the (sim,lamuda) combinations
	for(int l=1;l<=Num_Lot;l++){
		pair<int,sim_lamuda_item> one_pair;
		one_pair.first=l;
		sim_lamuda_L.push_back(one_pair);
	}
	for(int l=1;l<=Num_Lot;l++){
		int index=-1;
		for(int sim=1;sim<=(int)SIM.size();sim++){
			int i=SIM[sim-1].represented_machine_id;
			vii p_Lamuda=Lamuda_i[i].int_vector.begin();
			while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
				int Lamuda_id=(*p_Lamuda);
				index++;
				vii pfind=find(L_sim_lamuda[index].second.int_vector.begin(),L_sim_lamuda[index].second.int_vector.end(),l);
				if(pfind!=L_sim_lamuda[index].second.int_vector.end()){
					sim_lamuda_L[l-1].second.sim_lamuda_pairs.push_back(make_pair(sim,Lamuda_id));
				}
				p_Lamuda++;
			}
		}
	}
}

void AT::show_sim_lamuda_L(){
	ofstream fout("Debug/debug_sim_lamuda_L.txt");
	for(int l=1;l<=Num_Lot;l++){
		fout<<"lot="<<l<<endl;
		vector< pair<int,int> >::iterator p_pair=sim_lamuda_L[l-1].second.sim_lamuda_pairs.begin();
		while(p_pair!=sim_lamuda_L[l-1].second.sim_lamuda_pairs.end()){
			fout<<"("<<(*p_pair).first<<","<<(*p_pair).second<<")"<<",";
			p_pair++;
		}
		fout<<endl;
	}
	fout.close();
}

int AT::max_rate_s(int i,int l,int Lamuda_id){
	int sim=machine_sim[i];
	int d=lots_device[l];
	int index=index_S_sim_d_lamuda(sim,d,Lamuda_id);
	return S_sim_d_lamuda[index].s;
}

void AT::initialize_S_sim_d_lamuda(){
	//find S(i,l,lamuda)->s with highest rate
	for(int sim=1;sim<=(int)SIM.size();sim++){
		int i=SIM[sim-1].represented_machine_id;
		vii p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			int index=index_L_sim_lamuda(sim,Lamuda_id);
			vector<int>::iterator p_L=L_sim_lamuda[index].second.int_vector.begin();
			while(p_L!=L_sim_lamuda[index].second.int_vector.end()){
				int l=(*p_L);
				int d=lots_device[l];
				int flag=0;
				for(int id=1;id<=(int)S_sim_d_lamuda.size();id++){
					if(sim==S_sim_d_lamuda[id-1].sim && Lamuda_id==S_sim_d_lamuda[id-1].Lamuda_id && d==S_sim_d_lamuda[id-1].d){
						flag=1;
						break;
					}
				}
				if(flag){
					p_L++;
					continue;
				}
				int max_rate_s=-1;
				double max_rate=-1E+6;
				vector<tooling_configure> TS;
				get_S_i_l_Lamuda(TS,i,l,Lamuda_id);

				vector<tooling_configure>::iterator pTS=TS.begin();
				while(pTS!=TS.end()){
					int s=(*pTS).id;
					if(rate[s]>max_rate){
						max_rate=rate[s];
						max_rate_s=s;
					}
					pTS++;
				}
				struct S_sim_d_lamuda_item one_item;
				one_item.sim=sim;
				one_item.d=d;
				one_item.Lamuda_id=Lamuda_id;
				one_item.s=max_rate_s;
				S_sim_d_lamuda.push_back(one_item);
				p_L++;
			}
			p_Lamuda++;
		}
	}
}

void AT::show_S_sim_d_lamuda(){
	ofstream fout("Debug/debug_S_sim_d_lamuda.txt");
	for(int id=1;id<=(int)S_sim_d_lamuda.size();id++){
		fout<<"sim="<<S_sim_d_lamuda[id-1].sim<<","
			<<"d="<<S_sim_d_lamuda[id-1].d<<","
			<<"lamuda="<<S_sim_d_lamuda[id-1].Lamuda_id<<","
			<<"s="<<S_sim_d_lamuda[id-1].s<<endl;
	}
	fout.close();
}

int AT::index_S_sim_d_lamuda(int sim,int d,int Lamuda_id){
	int index=-1;
	for(int id=1;id<=(int)S_sim_d_lamuda.size();id++){
		if(sim==S_sim_d_lamuda[id-1].sim && d==S_sim_d_lamuda[id-1].d && Lamuda_id==S_sim_d_lamuda[id-1].Lamuda_id){
			index = id-1;
			break;
		}
	}
	if(index<0){
		cout<<"Warning!! Wrong S_sim_l_lamuda index!!"<<endl;
		cout<<"sim="<<sim<<",d="<<d<<",Lamuda_id="<<Lamuda_id<<endl;
	}
	return index;
}

int AT::is_KD_PD(int dev){
	//return 0 if dev is neither KD nor PD
	//return 1 if dev is KD
	//return 2 if dev is PD
	//return 3 if dev is a key package
	//return 4 if dev is a key pin-package
	vii pfind=find(Key_Devices.begin(),Key_Devices.end(),dev);
	if(pfind!=Key_Devices.end()){
		return 1;
	}
	pfind=Packages.begin();
	pfind=find(Packages.begin(),Packages.end(),dev);
	if(pfind!=Packages.end()){
		return 2;
	}
}

bool AT::is_Key_PKG(int dev){
	bool flag=false;
	int pkg=device_PKG[dev];
	vii pfind=find(Key_PKG.begin(),Key_PKG.end(),pkg);
	if(pfind!=Key_PKG.end()){
		flag=true;
	}
	return flag;
}

bool AT::is_Key_PIN_PKG(int dev){
	bool flag=false;
	int pkg_id=device_PKG[dev];
	int pin_num=device_PIN[dev];
	for(int id=1;id<=(int)Key_PIN_PKG.size();id++){
		if(Key_PIN_PKG[id-1].first.first==pin_num && Key_PIN_PKG[id-1].first.second==pkg_id){
			flag=true;
		}
	}
	return flag;
}

double AT::compute_obj(vector<sub_soln_item> & sub_soln,vector< pair<int,double> > & shortage){
	//compute the obj for the first level problem, given the sub_soln and shortage
	double obj=0.0;
	int counter=0;
	for(int i=1;i<=Num_Machine;i++){
		int Lamuda_id=sub_soln[i-1].Lamuda_id;
		//cout<<"Lamuda_id="<<Lamuda_id<<endl;
		//cout<<"size ="<<(int)sub_soln[i-1].lots.int_vector.size()<<endl;
		vii p_lot=sub_soln[i-1].lots.int_vector.begin();
		double time_used=0.0;
		while(p_lot!=sub_soln[i-1].lots.int_vector.end()){
			int l=(*p_lot);
			int s=max_rate_s(i,l,Lamuda_id);
			obj+=-w_l[l]+eps[s];
			time_used+=n_l_chips[l]/rate[s];
			counter++;
			p_lot++;
		}
		//cout<<"i="<<i<<",time_used="<<sub_soln[i-1].time_used<<",time_computed="<<time_used<<endl;
	}
	//cout<<"counter="<<counter<<endl;
	//compute the penalty of deviation for Key Device
	for(int d=1;d<=Num_Device;d++){
		if(shortage[d-1].second>0.0){
			obj+=shortage[d-1].second/coef*penalty[d];
		}
	}
	//cout<<"obj="<<obj<<endl;
	//compute the penalty of deviation for KP and KPP
	vector<int> PKG_production((int)PKG.size(),0);
	vector<int> PIN_PKG_production((int)PKG.size()*(int)PIN_order.size()+1,0);
	for(int i=1;i<=Num_Machine;i++){
		int Lamuda_id=sub_soln[i-1].Lamuda_id;
		vii p_lot=sub_soln[i-1].lots.int_vector.begin();
		while(p_lot!=sub_soln[i-1].lots.int_vector.end()){
			int l=(*p_lot);
			int d=lots_device[l];
			int pkg_id=device_PKG[d];
			int pin_num=device_PIN[d];
			vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);
			int pin_order=pfind-PIN_order.begin()+1;
			PKG_production[pkg_id-1]+=n_l_chips[l];
			int index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;
			PIN_PKG_production[index]+=n_l_chips[l];
			p_lot++;
		}
	}
	for(int kp=1;kp<=(int)Key_PKG.size();kp++){
		int pkg_id=Key_PKG[kp-1];
		int diff=n_kp_minchips[pkg_id]-PKG_production[pkg_id-1];
		//cout<<"kp="<<kp<<",diff="<<diff<<",deltaKP="<<diff/coef<<endl;
		if(diff>0){
			obj+=diff/coef*PKG_penalty[pkg_id];
		}
	}
	for(int kpp=1;kpp<=(int)Key_PIN_PKG.size();kpp++){
		int pkg_id=Key_PIN_PKG[kpp-1].first.second;
		int pin_num=Key_PIN_PKG[kpp-1].first.first;
		int target=Key_PIN_PKG[kpp-1].second;
		vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);
		int pin_order=pfind-PIN_order.begin()+1;
		int index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;
		int diff=target-PIN_PKG_production[index];
		//cout<<"kpp="<<kpp<<",diff="<<diff<<",deltaKPP="<<diff/coef<<endl;
		if(diff>0){
			obj+=diff/coef*get_PIN_PKG_penalty(pkg_id,pin_num);
		}
	}
	//cout<<"obj="<<obj<<endl;
	return obj;
}

double AT::compute_obj(double * solution){
	double obj=0.0;
	for(int i=1;i<=Num_Machine;i++){
		vector<int>::iterator p_L=L_i[i].int_vector.begin();
		while(p_L!=L_i[i].int_vector.end()){
			int l=(*p_L);
			int sim=machine_sim[i];
			int d=lots_device[l];
			int index=index_S_sim_d(sim,d);
			vii p_s=S_sim_d[index].second.int_vector.begin();
			while(p_s!=S_sim_d[index].second.int_vector.end()){
				int s=(*p_s);
				int id=index2_x(i,l,s);
				obj+=(-w_l[l]+eps[s])*solution[id];
				p_s++;
			}
			p_L++;
		}
	}
	for(int k=1;k<=Num_Keydevice;k++){
		int id=x_limit+k;
		int d=Key_Devices[k-1];
		obj+=penalty[d]*solution[id];
	}
	for(int p=1;p<=Num_Package;p++){
		int id=x_limit+Num_Keydevice+p;
		int d=Packages[p-1];
		obj+=penalty[d]*solution[id];
	}
	for(int kp=1;kp<=(int)Key_PKG.size();kp++){
		int id=x_limit+Num_Keydevice+Num_Package+kp;
		int pkg_id=Key_PKG[kp-1];
		obj+=PKG_penalty[pkg_id]*solution[id];
	}
	for(int kpp=1;kpp<=(int)Key_PIN_PKG.size();kpp++){
		int id=x_limit+Num_Keydevice+Num_Package+(int)Key_PKG.size()+kpp;
		int pkg_id=Key_PIN_PKG[kpp-1].first.second;
		int pin_num=Key_PIN_PKG[kpp-1].first.first;
		vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);
		int pin_order=pfind-PIN_order.begin()+1;
		obj+=get_PIN_PKG_penalty(pkg_id,pin_num)*solution[id];
	}
	return obj;
}

void AT::show_sub_soln(vector<sub_soln_item> & sub_soln){
	cout<<"show sub_soln"<<endl;
	for(int i=1;i<=Num_Machine;i++){
		cout<<"i="<<i<<",Lamdua_id="<<sub_soln[i-1].Lamuda_id<<",time_used="<<sub_soln[i-1].time_used<<",assign lots=";
		vii p_lot=sub_soln[i-1].lots.int_vector.begin();
		while(p_lot!=sub_soln[i-1].lots.int_vector.end()){
			cout<<(*p_lot)<<",";
			p_lot++;
		}
		cout<<endl;
	}
}

void AT::build_first_level_model_OSI(CoinPackedMatrix * M,double * cost,double * col_lb,double * col_ub,double * row_lb,double * row_ub){
	//the arrays are filled in in this routine
	int num_con=0;
	int num_var=0;
	
	//initialize x variables
	for(int i=1;i<=Num_Machine;i++){
		vii p_L=L_i[i].int_vector.begin();
		while(p_L!=L_i[i].int_vector.end()){
			int l=(*p_L);
			int sim=machine_sim[i];
			int d=lots_device[l];
			int index=index_S_sim_d(sim,d);
			vii p_s=S_sim_d[index].second.int_vector.begin();
			while(p_s!=S_sim_d[index].second.int_vector.end()){
				int s=(*p_s);
				string str="x";
				stringstream ss;
				ss<<"("<<i<<")("<<l<<")("<<s<<")";
				str+=ss.str();
				char name[20];
				strcpy(name,str.c_str());
				int x_id=index2_x(i,l,s);
				col_lb[x_id]=0.0;
				col_ub[x_id]=1.0;
				num_var++;
				p_s++;
			}
			p_L++;
		}
	}
	//initialize y variables
	for(int i=1;i<=Num_Machine;i++){
		vii p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			string str="y";
			stringstream ss;
			ss<<"("<<i<<")("<<Lamuda_id<<")";
			str+=ss.str();
			char name[20];
			strcpy(name,str.c_str());
			int y_id=index2_y(i,Lamuda_id);
			col_lb[y_id]=0.0;
			col_ub[y_id]=1.0;
			num_var++;
			p_Lamuda++;
		}
	}
	//initalize delta variables
	for(int k=1;k<=Num_Keydevice;k++){
		string str="deltaKD";
		stringstream ss;
		ss<<"("<<k<<")";
		str+=ss.str();
		char name[20];
		strcpy(name,str.c_str());
		int id=x_limit+k;
		col_lb[id]=0.0;
		col_ub[id]=DBL_MAX;
		num_var++;
	}
	for(int k=1;k<=Num_Package;k++){
		string str="deltaPD";
		stringstream ss;
		ss<<"("<<k<<")";
		str+=ss.str();
		char name[20];
		strcpy(name,str.c_str());
		int id=x_limit+Num_Keydevice+k;
		col_lb[id]=0.0;
		col_ub[id]=DBL_MAX;
		num_var++;
	}
	for(int kp=1;kp<=(int)Key_PKG.size();kp++){
		int id=x_limit+Num_Keydevice+Num_Package+kp;
		col_lb[id]=0.0;
		col_ub[id]=DBL_MAX;
		num_var++;
	}
	for(int kpp=1;kpp<=(int)Key_PIN_PKG.size();kpp++){
		int id=x_limit+Num_Keydevice+Num_Package+(int)Key_PKG.size()+kpp;
		col_lb[id]=0.0;
		col_ub[id]=DBL_MAX;
		num_var++;
	}
	cout<<"num_var="<<num_var<<endl;
	M->setDimensions(0, num_var);

	cout<<"build obj"<<endl;
	for(int i=1;i<=Num_Machine;i++){
		vector<int>::iterator p_L=L_i[i].int_vector.begin();
		while(p_L!=L_i[i].int_vector.end()){
			int l=(*p_L);
			int sim=machine_sim[i];
			int d=lots_device[l];
			int index=index_S_sim_d(sim,d);
			vii p_s=S_sim_d[index].second.int_vector.begin();
			while(p_s!=S_sim_d[index].second.int_vector.end()){
				int s=(*p_s);
				int x_id=index2_x(i,l,s);
				cost[x_id]=-w_l[l]+eps[s];
				p_s++;
			}
			p_L++;
		}
	}
	for(int k=1;k<=Num_Keydevice;k++){
		int id=x_limit+k;
		int d=Key_Devices[k-1];
		cost[id]=penalty[d];
	}
	for(int p=1;p<=Num_Package;p++){
		int id=x_limit+Num_Keydevice+p;
		int d=Packages[p-1];
		cost[id]=penalty[d];
	}
	for(int kp=1;kp<=(int)Key_PKG.size();kp++){
		int pkg=Key_PKG[kp-1];
		int id=x_limit+Num_Keydevice+Num_Package+kp;
		cost[id]=PKG_penalty[pkg];
	}
	for(int kpp=1;kpp<=(int)Key_PIN_PKG.size();kpp++){
		int pin_num=Key_PIN_PKG[kpp-1].first.first;
		int pkg_id =Key_PIN_PKG[kpp-1].first.second;
		int id=x_limit+Num_Keydevice+Num_Package+(int)Key_PKG.size()+kpp;
		cost[id]=get_PIN_PKG_penalty(pkg_id,pin_num);
	}
	cout<<"build constraints (1b)"<<endl;
	for(int l=1;l<=Num_Lot;l++){
		CoinPackedVector row;
		vector<int>::iterator p_M=M_l[l].int_vector.begin();
		while(p_M!=M_l[l].int_vector.end()){
			int i=(*p_M);
			int sim=machine_sim[i];
			int d=lots_device[l];
			int index=index_S_sim_d(sim,d);
			vii p_s=S_sim_d[index].second.int_vector.begin();
			while(p_s!=S_sim_d[index].second.int_vector.end()){
				int s=(*p_s);
				int x_id=index2_x(i,l,s);
				row.insert(x_id,1.0);
				p_s++;
			}
			p_M++;
		}
		row_lb[num_con]=-DBL_MAX;
		row_ub[num_con]=1.0;
		M->appendRow(row);
		num_con++;
		string str="(";
		stringstream ss;
		ss<<"l@"<<l<<")";
		char name[100];
		str+=ss.str();
		strcpy(name,str.c_str());
	}

	if(new_old_model==2){
	cout<<"build constraints (1c)"<<endl;
	for(int i=1;i<=Num_Machine;i++){
		vector<int>::iterator p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			int simid=machine_sim[i];
			int index=index_L_sim_lamuda(simid,Lamuda_id);
			vector<int>::iterator p_L=L_sim_lamuda[index].second.int_vector.begin();
			while(p_L!=L_sim_lamuda[index].second.int_vector.end()){
				int l=(*p_L);
				vector<tooling_configure> TS;
				get_S_i_l_Lamuda(TS,i,l,Lamuda_id);
				vector<tooling_configure>::iterator p_TS=TS.begin();
				while(p_TS!=TS.end()){
					CoinPackedVector row;
					int s=(*p_TS).id;
					int x_id=index2_x(i,l,s);
					int y_id=index2_y(i,Lamuda_id);
					row.insert(x_id,-1.0);
					row.insert(y_id,1.0);
					row_lb[num_con]=0.0;
					row_ub[num_con]=DBL_MAX;
					M->appendRow(row);
					num_con++;
					string str="(";
					stringstream ss;
					ss<<"i@"<<i<<",Lamuda@"<<Lamuda_id<<",l@"<<l<<",s@"<<s<<")";
					str+=ss.str();
					char name[100];
					strcpy(name,str.c_str());
					p_TS++;
				}
				p_L++;
			}
			p_Lamuda++;
		}
	}
	}

	cout<<"build constraints (1d)"<<endl;
	for(int i=1;i<=Num_Machine;i++){
		CoinPackedVector row;
		vector<int>::iterator p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(* p_Lamuda);
			int y_id=index2_y(i,Lamuda_id);
			row.insert(y_id,1.0);
			p_Lamuda++;
		}
		row_lb[num_con]=-DBL_MAX;
		row_ub[num_con]=1.0;
		M->appendRow(row);
		num_con++;
		string str="(";
		stringstream ss;
		ss<<"(y)i@"<<i<<")";
		str+=ss.str();
		char name[100];
		strcpy(name,str.c_str());
	}

	cout<<"build constraints (1e)"<<endl;
	for(int tf=1;tf<=Num_Tooling_Family;tf++){
		for(int n=1;n<=(int)Temperature_Set.size();n++){
			vector<t_set>::iterator p_set=Temperature_Set.begin()+n-1;
			CoinPackedVector row;
			for(int i=1;i<=Num_Machine;i++){
				vii p_int=(*p_set).set_elements.begin();
				while(p_int!=(*p_set).set_elements.end()){
					int tau=(*p_int);
					vector<int> Lamuda_i_tf_tau;
					get_Lamuda(Lamuda_i_tf_tau,i,tf,tau);
					vii p_Lamuda=Lamuda_i_tf_tau.begin();
					while(p_Lamuda!=Lamuda_i_tf_tau.end()){
						int Lamuda_id=(*p_Lamuda);
						vector<Lamuda_item>::iterator pL=Lamuda.begin()+Lamuda_id-1;
						vii p_find=find((*pL).tooling_vec.begin(),(*pL).tooling_vec.end(),tf);
						int offset=p_find-(*pL).tooling_vec.begin();
						vii pb=(*pL).b.begin()+offset;
						int y_id=index2_y(i,Lamuda_id);
						row.insert(y_id,(*pb));
						p_Lamuda++;
					}//lamuda_iteration
					p_int++;
				}//tau_iteration
			}//i_iteration
			vector<t_set> inter_set;
			get_inter_set((*p_set),inter_set);
			vector<t_set>::iterator pm=inter_set.begin();
			int sum=0;
			while(pm!=inter_set.end()){
				int set_id=get_set_id(*pm);
				sum+=n_t_n_tooling[tf][set_id];
				pm++;
			}
			row_lb[num_con]=-DBL_MAX;
			row_ub[num_con]=sum;
			M->appendRow(row);
			num_con++;
			string str="(";
			stringstream ss;
			ss<<"tf@"<<tf<<",n@"<<n<<")";
			str+=ss.str();
			char name[100];
			strcpy(name,str.c_str());
		}
	}

	if(new_old_model==1){
	cout<<"build updated constraints (1f)"<<endl;
	for(int i=1;i<=Num_Machine;i++){
		vector<int>::iterator p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			CoinPackedVector row;
			int index=index_L_sim_lamuda(machine_sim[i],Lamuda_id);
			vector<int>::iterator p_L=L_sim_lamuda[index].second.int_vector.begin();
			while(p_L!=L_sim_lamuda[index].second.int_vector.end()){
				int l=(*p_L);
				vector<tooling_configure> TS;
				get_S_i_l_Lamuda(TS,i,l,Lamuda_id);
				vector<tooling_configure>::iterator p_TS=TS.begin();
				while(p_TS!=TS.end()){
					int s=(*p_TS).id;
					int x_id=index2_x(i,l,s);
					row.insert(x_id,n_l_chips[l]/rate[s]);
					p_TS++;
				}
				p_L++;
			}
			int y_id=index2_y(i,Lamuda_id);
			row.insert(y_id,-H[i]);
			row_lb[num_con]=-DBL_MAX;
			row_ub[num_con]=0;
			M->appendRow(row);
			num_con++;
			string str="(";
			stringstream ss;
			ss<<"i@"<<i<<",Lamuda@"<<Lamuda_id<<")";
			str+=ss.str();
			char name[100];
			strcpy(name,str.c_str());
			p_Lamuda++;
		}
	}
	}

	if(new_old_model==2){
	cout<<"build constraints (1f)"<<endl;
	for(int i=1;i<=Num_Machine;i++){
		CoinPackedVector row;
		vector<int>::iterator p_L=L_i[i].int_vector.begin();
		while(p_L!=L_i[i].int_vector.end()){
			int l=(*p_L);
			int sim=machine_sim[i];
			int d=lots_device[l];
			int index=index_S_sim_d(sim,d);
			vii p_s=S_sim_d[index].second.int_vector.begin();
			while(p_s!=S_sim_d[index].second.int_vector.end()){
				int s=(*p_s);
				int x_id=index2_x(i,l,s);
				row.insert(x_id,n_l_chips[l]/rate[s]);
				p_s++;
			}
			p_L++;
		}
		row_lb[num_con]=-DBL_MAX;
		row_ub[num_con]=H[i];
		M->appendRow(row);
		num_con++;
		string str="(";
		stringstream ss;
		//ss<<"i@"<<i<<",Lamuda@"<<Lamuda_id<<")";
		str+=ss.str();
		char name[100];
		strcpy(name,str.c_str());
	}
	}

	cout<<"build constraints (1g)"<<endl;
	for(int k=1;k<=Num_Keydevice;k++){
		vector<int>::iterator p_int=Key_Devices.begin()+k-1;
		int d=(*p_int);
		CoinPackedVector row;
		for(int i=1;i<=Num_Machine;i++){
			vector<int>::iterator p_L=L_i_d[i][d].int_vector.begin();
			while(p_L!=L_i_d[i][d].int_vector.end()){
				int l=(*p_L);
				int sim=machine_sim[i];
				int index=index_S_sim_d(sim,d);
				vii p_s=S_sim_d[index].second.int_vector.begin();
				while(p_s!=S_sim_d[index].second.int_vector.end()){
					int s=(*p_s);
					int x_id=index2_x(i,l,s);
					row.insert(x_id,n_l_chips[l]);
					p_s++;
				}
				p_L++;
			}
		}
		int delta_id=x_limit+k;
		row.insert(delta_id,coef);
		row_lb[num_con]=n_k_minchips[d];
		row_ub[num_con]=DBL_MAX;
		M->appendRow(row);
		num_con++;
		string str="(";
		stringstream ss;
		ss<<"k@"<<k<<",dev@"<<d<<")";
		str+=ss.str();
		char name[100];
		strcpy(name,str.c_str());
	}

	cout<<"build constraints (1h)"<<endl;
	for(int p=1;p<=Num_Package;p++){
		vector<int>::iterator p_int=Packages.begin()+p-1;
		int d=(*p_int);
		CoinPackedVector row;
		for(int i=1;i<=Num_Machine;i++){
			vector<int>::iterator p_L=L_i_d[i][d].int_vector.begin();
			while(p_L!=L_i_d[i][d].int_vector.end()){
				int l=(*p_L);
				int sim=machine_sim[i];
				int index=index_S_sim_d(sim,d);
				vii p_s=S_sim_d[index].second.int_vector.begin();
				while(p_s!=S_sim_d[index].second.int_vector.end()){
					int s=(*p_s);
					int x_id=index2_x(i,l,s);
					row.insert(x_id,n_l_chips[l]);
					p_s++;
				}
				p_L++;
			}
		}
		int delta_id=x_limit+Num_Keydevice+p;
		row.insert(delta_id,coef);
		row_lb[num_con]=n_p_minchips[d];
		row_ub[num_con]=DBL_MAX;
		M->appendRow(row);
		num_con++;
		string str="(";
		stringstream ss;
		ss<<"p@"<<p<<",dev@"<<d<<")";
		str+=ss.str();
		char name[100];
		strcpy(name,str.c_str());
	}

	//constriants for deviation of KP
	cout<<"build constraints (1i)"<<endl;
	for(int id=1;id<=(int)Key_PKG.size();id++){
		int pkg_id=Key_PKG[id-1];
		vector<int> dev;
		for(int d=1;d<=Num_Device;d++){
			if(device_PKG[d]==pkg_id){ dev.push_back(d);}
		}
		CoinPackedVector row;
		for(int i=1;i<=Num_Machine;i++){
			for(int id2=1;id2<=(int)dev.size();id2++){
				int d=dev[id2-1];
				vector<int>::iterator p_L=L_i_d[i][d].int_vector.begin();
				while(p_L!=L_i_d[i][d].int_vector.end()){
					int l=(*p_L);
					int sim=machine_sim[i];
					int index=index_S_sim_d(sim,d);
					vii p_s=S_sim_d[index].second.int_vector.begin();
					while(p_s!=S_sim_d[index].second.int_vector.end()){
						int s=(*p_s);
						int x_id=index2_x(i,l,s);
						row.insert(x_id,n_l_chips[l]);
						p_s++;
					}
					p_L++;
				}
			}
		}
		int delta_id=x_limit+Num_Keydevice+Num_Package+id;
		row.insert(delta_id,coef);
		row_lb[num_con]=n_kp_minchips[pkg_id];
		row_ub[num_con]=DBL_MAX;
		M->appendRow(row);
		num_con++;
		string str="(";
		stringstream ss;
		ss<<"id@"<<id<<",kp@"<<pkg_id<<")";
		str+=ss.str();
		char name[100];
		strcpy(name,str.c_str());
	}

	//constriants for deviation of KPP
	cout<<"build constraints (1j)"<<endl;
	for(int id=1;id<=(int)Key_PIN_PKG.size();id++){
		int pin=Key_PIN_PKG[id-1].first.first;
		int pkg_id=Key_PIN_PKG[id-1].first.second;
		int target=Key_PIN_PKG[id-1].second;
		vector<int> dev;
		for(int d=1;d<=Num_Device;d++){
			if(device_PKG[d]==pkg_id && device_PIN[d]==pin){ dev.push_back(d);}
		}
		CoinPackedVector row;
		for(int i=1;i<=Num_Machine;i++){
			for(int id2=1;id2<=(int)dev.size();id2++){
				int d=dev[id2-1];
				vector<int>::iterator p_L=L_i_d[i][d].int_vector.begin();
				while(p_L!=L_i_d[i][d].int_vector.end()){
					int l=(*p_L);
					int sim=machine_sim[i];
					int index=index_S_sim_d(sim,d);
					vii p_s=S_sim_d[index].second.int_vector.begin();
					while(p_s!=S_sim_d[index].second.int_vector.end()){
						int s=(*p_s);
						int x_id=index2_x(i,l,s);
						row.insert(x_id,n_l_chips[l]);
						p_s++;
					}
					p_L++;
				}
			}
		}
		int delta_id=x_limit+Num_Keydevice+Num_Package+(int)Key_PKG.size()+id;
		row.insert(delta_id,coef);
		row_lb[num_con]=target;
		row_ub[num_con]=DBL_MAX;
		M->appendRow(row);
		num_con++;
		string str="(";
		stringstream ss;
		ss<<"id@"<<id<<",pkg@"<<pkg_id<<",pin@"<<pin<<")";
		str+=ss.str();
		char name[100];
		strcpy(name,str.c_str());
	}

	cout<<"num_con="<<num_con<<endl;
}

void AT::solve_first_level_model_OSI(OsiXxxSolverInterface * si,double * solution){
	//si->writeLp("AT_CPLEX_first_level");
	cout<<"num_var="<<si->getNumCols()<<",num_con="<<si->getNumRows()<<endl;
#ifdef COIN_USE_CPX
	//CPXsetintparam(si->getEnvironmentPtr(),CPX_PARAM_PREIND, 0);
	CPXsetdblparam(si->getEnvironmentPtr(),CPX_PARAM_EPGAP, 0.001);
#endif
	//use primal simplex to solve the master LP
	si->setHintParam(OsiDoDualInResolve,false,OsiHintDo);
	if(LP_IP==1){
		si->resolve();
	}else{
		si->branchAndBound();
	}
	if(si->isProvenOptimal()){
		CoinDisjointCopyN(si->getColSolution(), y_limit+1, solution);
	}else{
		cout<<"not optimal!!!"<<endl;
	}
	/*
	cout<<"obj="<<si->getObjValue()<<endl;
	cout<<"check soln:"<<endl;
	for(int id=x_limit+Num_Keydevice+Num_Package+1;id<=y_limit;id++){
		vector<index_node>::iterator p_node=index_vector.begin()+id;
		int i=(*p_node).i;
		int Lamuda_id=(*p_node).Lamuda_id;
		cout<<"i="<<i<<",Lamuda_id="<<Lamuda_id<<",y="<<si->getColSolution()[id]<<",LB="<<si->getColLower()[id]<<",UB="<<si->getColUpper()[id]<<",id="<<id<<endl;
	}
	*/
}

void AT::build_update_benefit_model(CoinPackedMatrix * M,double * cost,double * col_lb,double * col_ub,double * row_lb,double * row_ub){
	int num_con=0;
	int num_var=0;
	
	//initialize x variables
	for(int l=0;l<=Num_Lot-1;l++){
		col_lb[l]=0.0;
		col_ub[l]=1.0;
	}
	//initialize delta variables
	for(int k=Num_Lot;k<=Num_Lot+Num_Keydevice+Num_Package-1;k++){
		col_lb[k]=0.0;
		col_ub[k]=DBL_MAX;
	}
	M->setDimensions(0, Num_Lot+Num_Keydevice+Num_Package);

	//cout<<"build obj"<<endl;
	for(int l=0;l<=Num_Lot-1;l++){
		cost[l]=-w_l[l];
	}
	for(int k=Num_Lot;k<=Num_Lot+Num_Keydevice-1;k++){
		int d=Key_Devices[k-Num_Lot];
		cost[k]=penalty[d];
	}
	for(int k=Num_Lot+Num_Keydevice;k<=Num_Lot+Num_Keydevice+Num_Package-1;k++){
		int d=Packages[k-Num_Lot-Num_Keydevice];
		cost[k]=penalty[d];
	}
	
	//cout<<"build constraints (1c)"<<endl;
	for(int k=1;k<=Num_Keydevice;k++){
		vector<int>::iterator p_int=Key_Devices.begin()+k-1;
		int d=(*p_int);
		CoinPackedVector row;
		vii p_lot=Devices[d-1].device_lots.begin();
		while(p_lot!=Devices[d-1].device_lots.end()){
			int l=(*p_lot);
			row.insert(l-1,n_l_chips[l]);
			p_lot++;
		}
		row.insert(Num_Lot+k-1,coef);
		row_lb[num_con]=n_k_minchips[d];
		row_ub[num_con]=DBL_MAX;
		M->appendRow(row);
		num_con++;
	}

	//cout<<"build constraints (1d)"<<endl;
	for(int p=1;p<=Num_Package;p++){
		vector<int>::iterator p_int=Packages.begin()+p-1;
		int d=(*p_int);
		CoinPackedVector row;
		vii p_lot=Devices[d-1].device_lots.begin();
		while(p_lot!=Devices[d-1].device_lots.end()){
			int l=(*p_lot);
			row.insert(l-1,n_l_chips[l]);
			p_lot++;
		}
		row.insert(Num_Lot+Num_Keydevice+p-1,coef);
		row_lb[num_con]=n_p_minchips[d];
		row_ub[num_con]=DBL_MAX;
		M->appendRow(row);
		num_con++;
	}
}

int AT::index_L_sim_lamuda(int simid,int Lamuda_id){
	//get the index of element of vector L_sim_lamuda given simid and Lamdua_id
	int index=-1;
	vector< pair< pair<int,int>,vec_int> >::iterator p=L_sim_lamuda.begin();
	while(p!=L_sim_lamuda.end()){
		if((*p).first.first==simid && (*p).first.second==Lamuda_id){
			index=p-L_sim_lamuda.begin();
		}
		p++;
	}
	//index=-1,cannot find the combination
	return index;
}

double AT::second_level_greedy(int fix_y[],vector<sub_soln_item> & sub_soln,vector< pair<int,double> > & shortage
								  ,vector< pair< pair<int,double>,double> > & unassigned_lots){
	//greed assignment to solve the second level IP, in order to replace the LP part
	//unassigned_lots   < <lot,shortage>,unit_price>, shortage is the shortage of the device the lot contains
						  
	//initialize sub_soln
	//< < <i,Lamuda_id>,time_used>,lot1,lot2,...>
	sub_soln.clear();
	for(int i=1;i<=Num_Machine;i++){
		sub_soln_item one_item;
		one_item.i=i;
		if(fix_y[i]>0){
			one_item.Lamuda_id=fix_y[i];
		}else{
			one_item.Lamuda_id=-1;
		}
		one_item.time_used=0.0;
		sub_soln.push_back(one_item);
	}
	//initialize shortage
	shortage.clear();
	for(int d=1;d<=Num_Device;d++){
		vii pfind=find(Key_Devices.begin(),Key_Devices.end(),d);
		if(pfind!=Key_Devices.end()){//d is a key device
			shortage.push_back(make_pair(d,n_k_minchips[d]));
		}else{
			vii pfind=find(Packages.begin(),Packages.end(),d);
			if(pfind!=Packages.end()){//d is a package device
				shortage.push_back(make_pair(d,n_p_minchips[d]));
			}else{//d is neither a key nor package device
				shortage.push_back(make_pair(d,-1E+10));
			}
		}
	}
	//initialize unassigned_lots
	unassigned_lots.clear();
	for(int l=1;l<=Num_Lot;l++){
		pair< pair<int,double>,double> one_pair;
		one_pair.first.first=l;
		int dev=lots_device[l];
		if(shortage[dev-1].second>0){
			one_pair.first.second=w_l[l]+penalty[dev]/coef*n_l_chips[l];
		}else{
			one_pair.first.second=w_l[l];
		}
		one_pair.second=w_l[l]/n_l_chips[l];
		unassigned_lots.push_back(one_pair);
	}
	//construct benefit of machine i that already setup
	double * benefit = new double[Num_Machine+1];
	for(int i=0;i<=Num_Machine;i++){ benefit[i]=0.0;}
	//------------------------------------------------------------------------------------
	vector< pair< pair<int,double>,double> >::iterator p_lot=unassigned_lots.begin();
	while(p_lot!=unassigned_lots.end()){
		int l=(*p_lot).first.first;
		for(int i=1;i<=Num_Machine;i++){
			vector< pair<int,int> >::iterator p_pair=sim_lamuda_L[l-1].second.sim_lamuda_pairs.begin();
			while(p_pair!=sim_lamuda_L[l-1].second.sim_lamuda_pairs.end()){
				if(machine_sim[i]==(*p_pair).first && sub_soln[i-1].Lamuda_id==(*p_pair).second){//the machine can be used to processed the lot
					benefit[i]+=n_l_chips[l];
				}
				p_pair++;
			}
		}
		p_lot++;
	}
	//------------------------------------------------------------------------------------assign in a greedy way
	p_lot=unassigned_lots.begin();
	while(p_lot!=unassigned_lots.end()){
		int erase_flag=0;
		int l=(*p_lot).first.first;
		int i_selected=-1;
		double best_ben=-1E+6;
		double time_required_save=0.0;
		for(int i=1;i<=Num_Machine;i++){
			int s=max_rate_s(i,l,sub_soln[i-1].Lamuda_id);
			double time_required=n_l_chips[l]/rate[s];
			vector< pair<int,int> >::iterator p_pair=sim_lamuda_L[l-1].second.sim_lamuda_pairs.begin();
			while(p_pair!=sim_lamuda_L[l-1].second.sim_lamuda_pairs.end()){
				if(machine_sim[i]==(*p_pair).first && sub_soln[i-1].Lamuda_id==(*p_pair).second
					&& sub_soln[i-1].time_used + time_required<=H[i]){//the machine can be used to processed the lot
					if(benefit[i]>best_ben){
						best_ben=benefit[i]; i_selected=i; time_required_save=time_required;
					}
				}
				p_pair++;
			}
		}
		if(i_selected>0){//a machine is selected finally to process the lot
			sub_soln[i_selected-1].lots.int_vector.push_back(l);
			sub_soln[i_selected-1].time_used+=time_required_save;
			int dev=lots_device[l];
			shortage[dev-1].second-=n_l_chips[l];
			p_lot=unassigned_lots.erase(p_lot);
			erase_flag=1;
			benefit[i_selected]-=n_l_chips[l];
		}
		if(!erase_flag){
			p_lot++;
		}
	}
	//------------------------------------------------------------------------------------
	double obj_greedy=compute_obj(sub_soln,shortage);
	cout<<"obj_greedy="<<obj_greedy<<endl;
	return obj_greedy;
	delete [] benefit;
}

void AT::local_search_on_y(OsiXxxSolverInterface * si,struct soln_pool_item & soln_A){
	//diversified
    double * solution=new double[y_limit+1];
	int * fix_y=new int[Num_Machine+1];
	for(int id=0;id<=y_limit;id++){ solution[id]=0.0;}
	for(int id=x_limit+Num_Keydevice+Num_Package+1;id<=y_limit;id++){ si->setColBounds(id,0,0);}
	fix_y[0]=-1;
	for(int i=1;i<=Num_Machine;i++){ 
		if(soln_A.sub_soln[i-1].Lamuda_id>0){
			fix_y[i]=soln_A.sub_soln[i-1].Lamuda_id;
			int y_id=index2_y(i,fix_y[i]);
			si->setColBounds(y_id,1,1);
		}else{
			fix_y[i]=-1;
		}
	}

    double cur_obj=compute_obj(soln_A.sub_soln,soln_A.shortage);
	cout<<"cur_obj="<<cur_obj<<endl;
	bool primal_dual=false;
	bool iter_flag=true;
	while(iter_flag){
		iter_flag=false;
		double best_obj=1E+10;
		int best_i=-1;
		int best_Lamuda=-1;
		vector<sub_soln_item> best_sub_soln;
		vector< pair<int,double> > best_shortage;
		vector< pair< pair<int,double>,double> > best_unassigned_lots;
		best_sub_soln.resize(Num_Machine);
		best_shortage.resize(Num_Device);

		for(int i=1;i<=Num_Machine;i++){
			if(i==1){ primal_dual=true;}
			int Lamuda_id_cur=soln_A.sub_soln[i-1].Lamuda_id;
			//cout<<"Lamuda_id_cur="<<Lamuda_id_cur<<endl;
			int y_id=-1000;
			if(Lamuda_id_cur>0){
				y_id=index2_y(i,Lamuda_id_cur);
				si->setColBounds(y_id,0,0);
			}
			//----------------------------------------------------------compute the benefit lost
			double benefit_lost=0.0;
			vector<int> dev;
			vector<int> amount;
			vii p_lot=soln_A.sub_soln[i-1].lots.int_vector.begin();
			while(p_lot!=soln_A.sub_soln[i-1].lots.int_vector.end()){
				int l=(*p_lot);
				benefit_lost+=w_l[l];
				int d=lots_device[l];
				vii pfind=find(dev.begin(),dev.end(),d);
				if(pfind!=dev.end()){
					int offset=pfind-dev.begin();
					vii p=amount.begin()+offset;
					(*p)+=n_l_chips[l];
				}else{
					dev.push_back(d);
					amount.push_back(n_l_chips[l]);
				}
				p_lot++;
			}
			for(int j=1;j<=(int)dev.size();j++){
				int d=dev[j-1];
				int num=amount[j-1];
				if(is_KD_PD(d)){
					if(soln_A.shortage[d-1].second>0){
						benefit_lost+=penalty[d]/coef*num;
					}else if(soln_A.shortage[d-1].second+num>0){
						benefit_lost+=penalty[d]/coef*(soln_A.shortage[d-1].second+num);
					}
				}
			}
			vector< pair< pair<int,double>,double> > all_unassigned_lots;
			all_unassigned_lots.resize((int)soln_A.unassigned_lots.size()+(int)soln_A.sub_soln[i-1].lots.int_vector.size());
			copy(soln_A.unassigned_lots.begin(),soln_A.unassigned_lots.end(),all_unassigned_lots.begin());

			cout<<"i="<<i<<",benefit_lost="<<benefit_lost<<endl;
			//----------------------------------------------------------------------------------------
			vii p_Lamuda=Lamuda_i[i].int_vector.begin();
			while(p_Lamuda!=Lamuda_i[i].int_vector.end()){//loop over the lamuda(i)
				int Lamuda_id=(*p_Lamuda);
				if(Lamuda_id!=Lamuda_id_cur){
					int y_id_temp=index2_y(i,Lamuda_id);
					si->setColBounds(y_id_temp,1,1);
					fix_y[i]=Lamuda_id;

					//cout<<"Lamuda_id="<<Lamuda_id<<endl;
					if(make_feasible(fix_y)){
						if(primal_dual){
							primal_dual=false;
							si->setHintParam(OsiDoDualInResolve,false,OsiHintDo);
						}else{
							si->setHintParam(OsiDoDualInResolve,true,OsiHintDo);
						}
						si->resolve();
						if(si->isProvenOptimal()){//it is possible that the run is infeasible due to the tooling constriants
							CoinDisjointCopyN(si->getColSolution(), y_limit+1, solution);
						}else{
							cout<<"!!!"<<endl;
							si->setColBounds(y_id_temp,0,0);
							p_Lamuda++;
							continue;
						}
						//---------------------------------------------------------------------------adjust solution[]
						adjust_LP_solution(solution);

						vector<sub_soln_item> sub_soln2;
						vector< pair<int,double> > shortage2;
						vector< pair< pair<int,double>,double> > unassigned_lots2;
						//-----------------------------------------------------------------------solve this subproblem heuristically
						double second_level_obj=second_level_heuristic(solution,sub_soln2,shortage2,unassigned_lots2,2);
						if(second_level_obj<best_obj){
							best_obj=second_level_obj;
							best_i=i;
							best_Lamuda=sub_soln2[i-1].Lamuda_id;
							//cout<<"best_Lamuda="<<best_Lamuda<<endl;
							copy(sub_soln2.begin(),sub_soln2.end(),best_sub_soln.begin());
							copy(shortage2.begin(),shortage2.end(),best_shortage.begin());
							best_unassigned_lots.resize((int)unassigned_lots2.size());
							copy(unassigned_lots2.begin(),unassigned_lots2.end(),best_unassigned_lots.begin());
						}
					}
					si->setColBounds(y_id_temp,0,0);
				}
				p_Lamuda++;
			}
			if(Lamuda_id_cur>0){
				si->setColBounds(y_id,1,1);
			}
			fix_y[i]=Lamuda_id_cur;
		}
		if(best_obj<cur_obj){
			cout<<"improved obj="<<best_obj<<endl;
			//iter_flag=true;
			cur_obj=best_obj;
			soln_A.sub_soln.clear();
			soln_A.shortage.clear();
			soln_A.unassigned_lots.clear();
			soln_A.sub_soln.resize(Num_Machine);
			soln_A.shortage.resize(Num_Device);
			soln_A.unassigned_lots.resize((int)best_unassigned_lots.size());
			copy(best_sub_soln.begin(),best_sub_soln.end(),soln_A.sub_soln.begin());
			copy(best_shortage.begin(),best_shortage.end(),soln_A.shortage.begin());
			copy(best_unassigned_lots.begin(),best_unassigned_lots.end(),soln_A.unassigned_lots.begin());
		}
	}
	delete [] solution;
	delete [] fix_y;
}


void AT::get_sum_weight(){
	sum_weight=0.0;
	double max_weight=-1000.0;
	for(int l=1;l<=Num_Lot;l++){
		sum_weight+=w_l[l];
		if(w_l[l]>max_weight){
			max_weight=w_l[l];
		}
	}
	double max=-1E+6;
	for(int l=1;l<=Num_Lot;l++){
		double temp=w_l[l]/n_l_chips[l];
		if(temp>max){
			max=temp;
		}
	}
	coef=sum_weight/(10*max);
	//initialize the penalty for key and package devices, these will be coefficients in the obj function
	for(int d=1;d<=Num_Device;d++){
		penalty[d]=0.0;
		int type=is_KD_PD(d);
		if(type>0){//d is a key or package device
			penalty[d]=sum_weight;
			for(int id=1;id<=(int)Devices[d-1].device_lots.size();id++){//for all the lots containing the device
				int l=Devices[d-1].device_lots[id-1];
				penalty[d]+=w_l[l];
			}
		}
	}
	//initialize the penalty for the key packages
	for(int pkg=1;pkg<=(int)PKG.size();pkg++){
		PKG_penalty[pkg]=0.0;
		double temp1=0.0,temp2=0.0;
		for(int d=1;d<=Num_Device;d++){
			if(device_PKG[d]==pkg){
				int qty=0;
				for(int l=1;l<=Num_Lot;l++){
					if(lots_device[l]==d){ qty+=n_l_chips[l];}
				}
				temp1+=penalty[d]*qty;
				temp2+=qty;
			}
		}
		if(temp2>1){
			PKG_penalty[pkg]=temp1/temp2;
		}else{
			PKG_penalty[pkg]=0.0;
		}
	}
	//initialize the penalty for the key pin packages
	//!! PIN_PKG_penalty [pkg_id][pin_id], NOTE!! pin_id!=pin number, it is the order id
	for(int d=1;d<=Num_Device;d++){
		int temp=device_PIN[d];
		vii pfind=find(PIN_order.begin(),PIN_order.end(),temp);
		if(pfind==PIN_order.end()){ PIN_order.push_back(temp);}
	}
	PIN_PKG_penalty=new double[(int)PKG.size()*(int)PIN_order.size()+1];
	for(int pkg=1;pkg<=(int)PKG.size();pkg++){
		for(int id=1;id<=(int)PIN_order.size();id++){
			int index=(pkg-1)*(int)PIN_order.size()+id-1;
			PIN_PKG_penalty[index]=0.0;
			double temp1=0.0,temp2=0.0;
			for(int d=1;d<=Num_Device;d++){
				if(device_PKG[d]==pkg && device_PIN[d]==PIN_order[id-1]){
					int qty=0;
					for(int l=1;l<=Num_Lot;l++){
						if(lots_device[l]==d){ qty+=n_l_chips[l];}
					}
					temp1+=penalty[d]*qty;
					temp2+=qty;
				}
			}
			if(temp2>1){
				PIN_PKG_penalty[index]=temp1/temp2;
			}else{
				PIN_PKG_penalty[index]=0.0;
			}
		}
	}
}

double AT::get_PIN_PKG_penalty(int pkg_id,int pin_num){
	//first parameter: package order in PKG (start from 1)
	//second parameter: number of pins (not the order in PIN_order)
	int pin_id=0;
	for(int id=1;id<=(int)PIN_order.size();id++){
		pin_id++;
		if(PIN_order[id-1]==pin_num){ break;}
	}
	int index=(pkg_id-1)*(int)PIN_order.size()+pin_id-1;
	return PIN_PKG_penalty[index];
}

double AT::local_branch_AB(OsiXxxSolverInterface * si,struct soln_pool_item & soln_A,struct soln_pool_item & soln_B){
	int * selected=new int[Num_Machine];
	int * excluded=new int[1];
	//random_select(Num_Machine,rnd_num,selected,excluded,0);
	for(int i=1;i<=Num_Machine;i++){
		int Lamuda_A=soln_A.sub_soln[i-1].Lamuda_id;
		int Lamuda_B=soln_B.sub_soln[i-1].Lamuda_id;
		vii p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			if(Lamuda_id!=Lamuda_A && Lamuda_id!=Lamuda_B){
				int id=index2_y(i,Lamuda_id);
				si->setColBounds(id,0,0);
			}
			p_Lamuda++;
		}
	}
	si->writeLp("AT_CPLEX_post");
	si->branchAndBound();
	double obj=1E+10;
	if(si->isProvenOptimal()){
		obj=si->getObjValue();
	}
	delete [] selected;
	delete [] excluded;
	return obj;
}

double AT::local_branch_AB2(OsiXxxSolverInterface * si,struct soln_pool_item & soln_A,struct soln_pool_item & soln_B){
	CoinPackedVector row;
	int num=0;
	for(int i=1;i<=Num_Machine;i++){
		int Lamuda_A=soln_A.sub_soln[i-1].Lamuda_id;
		int Lamuda_B=soln_B.sub_soln[i-1].Lamuda_id;
		if(Lamuda_A>0){
			int id_A=index2_y(i,Lamuda_A);
			row.insert(id_A,-1);
			num++;
		}else if(Lamuda_B>0 && Lamuda_B!=Lamuda_A){
			int id_B=index2_y(i,Lamuda_B);
			row.insert(id_B,-1);
			num++;
		}
	}
	si->addRow(row,5-num,20-num);
	si->writeLp("AT_CPLEX_post");
	si->branchAndBound();
	double obj=1E+10;
	if(si->isProvenOptimal()){
		obj=si->getObjValue();
	}
	return obj;
}

void AT::adjust_LP_solution(double * solution){
	//truncate the LP solution
	for(int id=0;id<=x_limit;id++){
		if(solution[id]>1E-06 && solution[id]<1-1E-06){
			double portion=solution[id];
			solution[id]=0.0;  //discard the lot
/*
			vector<index_node>::iterator p_node=index_vector.begin()+id;
			int l=(*p_node).l;
			int dev=lots_device[l];
			double production=portion*n_l_chips[l];
			int flag=0;
			int pos=-1;
			vii pfind=find(Key_Devices.begin(),Key_Devices.end(),dev);
			if(pfind!=Key_Devices.end()){
				flag=1; //dev is a key device
				pos =pfind-Key_Devices.begin()+1;
			}else{
				vii pfind=find(Packages.begin(),Packages.end(),dev);
				if(pfind!=Packages.end()){ 
					flag=2; //dev is a package device
					pos=pfind-Packages.begin()+1;
				} 
			}
			if(flag==1){
				int index=x_limit+pos;
				solution[index]+=portion*n_k_minchips[dev]/coef;
			}else if(flag==2){
				int index=x_limit+Num_Keydevice+pos;
				solution[index]+=portion*n_p_minchips[dev]/coef;
			}
			int pkg_id=device_PKG[dev];
			int pin_num=device_PIN[dev];
			bool is_kp=is_Key_PKG(dev);
			bool is_kpp=is_Key_PIN_PKG(dev);
*/
		}
	}
}

double AT::local_branch_LP(OsiXxxSolverInterface * si, struct soln_pool_item & soln_A,int K){
	CoinPackedVector row;
	int num=0;
	for(int i=1;i<=Num_Machine;i++){
		int Lamuda_A=soln_A.sub_soln[i-1].Lamuda_id;
		if(Lamuda_A>0){
			int id_A=index2_y(i,Lamuda_A);
			row.insert(id_A,-1);
			num++;
		}
	}
	//add the local branch cuts
	si->addRow(row,-DBL_MAX,K-num);
	si->setHintParam(OsiDoDualInResolve,false,OsiHintDo);
	si->resolve();
	Num_LP_solved++;
	double * solution=new double[y_limit+1];
	if(si->isProvenOptimal()){
		//get the LP solution
		CoinDisjointCopyN(si->getColSolution(), y_limit+1, solution);
	}
	int index=si->getNumRows()-1;
	si->deleteRows(1,&index);
	for(int id=x_limit+Num_Keydevice+Num_Package+1;id<=y_limit;id++){ si->setColBounds(id,0,0);}

	int * fix_y=new int[Num_Machine+1];
	for(int i=1;i<=Num_Machine;i++){ fix_y[i]=-1;}

	vector< vector<int> >  Lamuda_list; //<i,<Lamuda_id> >
	Lamuda_list.resize(Num_Machine);
	vector< vector<double> > Prob_list; //<i,<probability> >
	Prob_list.resize(Num_Machine);

	bool int_flag=true;
	for(int id=x_limit+Num_Keydevice+Num_Package+1;id<=y_limit;id++){
		if(solution[id]>1E-06){
			vector<index_node>::iterator p_node=index_vector.begin()+id;
			int i=(*p_node).i;
			int Lamuda_id=(*p_node).Lamuda_id;
			Lamuda_list[i-1].push_back(Lamuda_id);
			Prob_list[i-1].push_back(solution[id]);
			//cout<<"i="<<i<<",Lamuda_id="<<Lamuda_id<<",prob="<<solution[id]<<endl;
		}
		if(solution[id]>1E-06 && solution[id]<1-1E-06){
			int_flag=false;
		}
	}
	//cout<<"in local_branch_LP, int_flag="<<int_flag<<endl;

	bool primal_dual=false;
	int iter_counter=0;
	double best_obj=soln_A.obj;
	while(iter_counter<=10){
		if(iter_counter==0){ primal_dual=true; }
		//-----------------------------------------------------randomly selected fix_y[]
		for(int i=1;i<=Num_Machine;i++){
			double rnd=rand()*1.0/RAND_MAX;
			double sum_prob=0.0;
			for(int j=1;j<=(int)Lamuda_list[i-1].size();j++){
				vector<double>::iterator pp=Prob_list[i-1].begin()+j-1;
				if(rnd>=sum_prob && rnd<sum_prob+(*pp)){
					fix_y[i]=(*(Lamuda_list[i-1].begin()+j-1));
					break;
				}
				sum_prob+=(*pp);
			}
		}
		Num_Simulated++;

		//cout<<"randomly selected fix_y[]"<<endl;
		//for(int i=1;i<=Num_Machine;i++){
		//	cout<<"i="<<i<<",fix_y="<<fix_y[i]<<endl;
		//}
		for(int i=0;i<=y_limit;i++){ solution[i]=0.0;}
		//----------------------------------------------------
		if(make_feasible(fix_y)){
			//---------------------------------------------------------------------------solve the proble with fix_y[] as LP
			//set the fix_y[] values
			vector<int> index_record;
			for(int i=1;i<=Num_Machine;i++){
				if(fix_y[i]>0){
					int y_id=index2_y(i,fix_y[i]);
					si->setColBounds(y_id,1,1);
					index_record.push_back(y_id);
				}
			}
			if(primal_dual){
				primal_dual=false;
				si->setHintParam(OsiDoDualInResolve,false,OsiHintDo);
			}else{
				si->setHintParam(OsiDoDualInResolve,true,OsiHintDo);
			}
			if(LP_IP==1){
				si->resolve();
				Num_LP_solved++;
			}else{
				si->branchAndBound();
			}
			if(si->isProvenOptimal() && si->getObjValue()<soln_A.obj){//only if the LP relaxation soln is better 
				//get the LP solution
				CoinDisjointCopyN(si->getColSolution(), y_limit+1, solution);
				//---------------------------------------------------------------------------adjust solution[]
				adjust_LP_solution(solution);
				vector<sub_soln_item> sub_soln;
				vector< pair<int,double> > shortage;
				vector< pair< pair<int,double>,double> > unassigned_lots;
				//-----------------------------------------------------------------------solve this subproblem heuristically
				double local_branch_obj=second_level_heuristic(solution,sub_soln,shortage,unassigned_lots,2);
				if(local_branch_obj<best_obj){
					best_obj=local_branch_obj;
					copy(sub_soln.begin(),sub_soln.end(),soln_A.sub_soln.begin());
					copy(shortage.begin(),shortage.end(),soln_A.shortage.begin());
					soln_A.unassigned_lots.clear();
					soln_A.unassigned_lots.resize((int)unassigned_lots.size());
					copy(unassigned_lots.begin(),unassigned_lots.end(),soln_A.unassigned_lots.begin());
					soln_A.obj=best_obj;
				}
			}//end_if
			//--------------------------------------------------------------------------set back the fix_y values
			for(int i=1;i<=(int)index_record.size();i++){
				int y_id=index_record[i-1];
				si->setColBounds(y_id,0,0);
			}
		}else{
			cout<<"infeasible setup"<<endl;
			Num_Discarded++;
			
		}
		if(int_flag){ break;}
		iter_counter++;
	}//end_while;

	delete [] solution;
	delete [] fix_y;

	return best_obj;
}

bool AT::make_feasible(int fix_y[]){
	////---------------------------------------------------------------tooling_used[tf][tau]
	int * tooling_used=new int[Num_Tooling_Family*Num_Temperature];
	for(int id=0;id<=Num_Tooling_Family*Num_Temperature-1;id++){ tooling_used[id]=0;}

	for(int i=1;i<=Num_Machine;i++){
		int Lamuda_id=fix_y[i];
		//cout<<"Lamuda_id="<<Lamuda_id<<endl;
		if(Lamuda_id>=1){
			vector<Lamuda_item>::iterator pL=Lamuda.begin()+Lamuda_id-1;
			int tau=(*pL).temperature;
			//-----------------------------------------------
			vii p_tf=(*pL).tooling_vec.begin();
			//cout<<(int)(*pL).tooling_vec.size()<<endl;
			if((*p_tf)>0){
				while(p_tf!=(*pL).tooling_vec.end()){
					int tf=(*p_tf);
					int offset=p_tf-(*pL).tooling_vec.begin();
					vii p_b=(*pL).b.begin()+offset;
					int id=(tf-1)*Num_Temperature+tau-1;
					tooling_used[id]+=(*p_b);
					p_tf++;
				}
			}
		}
	}
	bool feasible_flag=true;
	for(int tf=1;tf<=Num_Tooling_Family;tf++){
		for(int n=1;n<=(int)Temperature_Set.size();n++){
			int lhs=0;
			vector<t_set>::iterator p_set=Temperature_Set.begin()+n-1;
			vii p_int=(*p_set).set_elements.begin();
			while(p_int!=(*p_set).set_elements.end()){
				int tau=(*p_int);
				int id=(tf-1)*Num_Temperature+tau-1;
				lhs+=tooling_used[id];
				p_int++;
			}
			int id2=(tf-1)*(int)Temperature_Set.size()+n-1;
			//cout<<"tf="<<tf<<",n="<<n<<",lhs="<<lhs<<",rhs="<<tooling_sum_rhs[id2].second<<endl;
			if(lhs>tooling_sum_rhs[id2].second){
				feasible_flag=false;
				break;
			}
		}
	}
	//cout<<"feasible_flag="<<feasible_flag<<endl;
	//------------------------------------------------------------------------------------
	//if(!feasible_flag){
	//
	//}
	delete [] tooling_used;
	return feasible_flag;
}

void AT::initialize_sil(){
	for(int i=1;i<=Num_Machine;i++){
		vector<int>::iterator p_lot=L_i[i].int_vector.begin();
		while(p_lot!=L_i[i].int_vector.end()){
			int l=(*p_lot);
			int sim=machine_sim[i];
			int d=lots_device[l];
			int index=index_S_sim_d(sim,d);
			double max_rate=-1E+6;
			int max_rate_s=-1;
			vii p_s=S_sim_d[index].second.int_vector.begin();
			while(p_s!=S_sim_d[index].second.int_vector.end()){
				int s=(*p_s);
				if(rate[s]>max_rate){
					max_rate=rate[s];
					max_rate_s=s;
				}
				p_s++;
			}
			pair< pair<int,int>,int > one_pair;
			one_pair.first.first=i;
			one_pair.first.second=l;
			one_pair.second=max_rate_s;
			sil.push_back(one_pair);
			p_lot++;
		}
	}
}

int AT::index_sil(int i,int l){
	int index=-1;
	for(int id=0;id<=(int)sil.size()-1;id++){
		if(sil[id].first.first==i && sil[id].first.second==l){
			index=id;
			break;
		}
	}
	if(index<0){
		cout<<"WRONG index!!! in index_sil()"<<endl;
	}
	return index;
}

void AT::show_sil(){
	ofstream fout("Debug/debug_sil.txt");
	for(int id=0;id<=(int)sil.size()-1;id++){
		fout<<"id="<<id<<",i="<<sil[id].first.first<<",l="<<sil[id].first.second<<",sil="<<sil[id].second<<endl;
	}
	fout.close();
}

void AT::initialize_S_sim_d(){
	for(int sim=1;sim<=(int)SIM.size();sim++){
		int i=SIM[sim-1].represented_machine_id;
		for(int d=1;d<=Num_Device;d++){
			if((int)Devices[d-1].device_lots.size()>0){
				int l=*(Devices[d-1].device_lots.begin());
				//cout<<"i="<<i<<",l="<<l<<endl;
				vector<tooling_configure> TS;
				get_S(TS,i,l);
				if((int)TS.size()>0){
					pair< pair<int,int>,vec_int > one_pair;
					one_pair.first.first=sim;
					one_pair.first.second=d;
					vector<tooling_configure>::iterator p_TS=TS.begin();
					while(p_TS!=TS.end()){
						int s=(*p_TS).id;
						one_pair.second.int_vector.push_back(s);
						p_TS++;
					}
					S_sim_d.push_back(one_pair);
				}
			}
		}
	}
}

int AT::index_S_sim_d(int sim,int d){
	int index=-1;
	for(int id=1;id<=(int)S_sim_d.size();id++){
		if(S_sim_d[id-1].first.first==sim && S_sim_d[id-1].first.second==d){
			index=id-1;
			break;
		}
	}
	if(index<0){
		cout<<"WRONG index!!! in index_S_sim_d()"<<endl;
	}
	return index;
}

void AT::show_S_sim_d(){
	ofstream fout("Debug/debug_S_sim_d.txt");
	for(int id=1;id<=(int)S_sim_d.size();id++){
		fout<<"id="<<id-1<<",sim="<<S_sim_d[id-1].first.first<<",d="<<S_sim_d[id-1].first.second<<",s(sim,d)=";
		vii p=S_sim_d[id-1].second.int_vector.begin();
		while(p!=S_sim_d[id-1].second.int_vector.end()){
			fout<<(*p)<<",";
			p++;
		}
		fout<<endl;
	}
	fout.close();
}

int AT::select_alpha(double prob_list[],int length){
	int selected;
	double rnd=rand()*1.0/RAND_MAX;
	double sum=0;
	for(int i=0;i<=length-1;i++){
		if(sum<rnd && rnd<=sum+prob_list[i]){
			selected=i; break;
		}
		sum+=prob_list[i];
	}
}

void AT::update_alpha_prob(double prob_list[],double avg_list[],int length,double best_so_far,double delta){
	double * q=new double[length];
	double sum=0;
	for(int i=0;i<=length-1;i++){
		q[i]=(best_so_far/avg_list[i]);
		q[i]=pow(q[i],delta);
		sum+=q[i];
	}
	for(int i=0;i<=length-1;i++){
		prob_list[i]=q[i]/sum;
	}
	delete [] q;
}


/*
void AT::LP_to_IP(OsiXxxSolver & si,struct soln_pool_item & IP_soln, vector<soln_pool_item> & pool){
	//approach from LP soln to the given IP soln
	double * LP_soln=new double[y_limit+1];
	for(int i=0;i<=y_limit;i++){ LP_soln[i]=0;}
	int * fix_y=new double[Num_Machine+1];
	for(int i=0;i<=Num_Machine;i++){ fix_y[i]=-1;}

	si.setHintParam(OsiDoDualInResolve,false,OsiHintDo);
	for(int i=1;i<=Num_Machine;i++){
		int Lamuda_id=IP_soln.sub_soln[i-1].Lamuda_id;
		int y_id=index2_y(i,Lamuda_id);
		si.setColBounds(y_id,1,1);
		si.resolve();
		CoinDisjointCopyN(si.getColSolution(), y_limit+1, LP_soln);

		vector< vector<int> >  Lamuda_list; //<i,<Lamuda_id> >
		Lamuda_list.resize(Num_Machine);
		vector< vector<double> > Prob_list; //<i,<probability> >
		Prob_list.resize(Num_Machine);

		for(int id=x_limit+Num_Keydevice+Num_Package+1;id<=y_limit;id++){
			if(LP_soln[id]>1E-06){
				vector<index_node>::iterator p_node=index_vector.begin()+id;
				int i2=(*p_node).i;
				int Lamuda_id2=(*p_node).Lamuda_id;
				Lamuda_list[i2-1].push_back(Lamuda_id2);
				Prob_list[i2-1].push_back(LP_soln[id]);
				//cout<<"i2="<<i2<<",Lamuda_id2="<<Lamuda_id2<<",prob="<<LP_soln[id]<<endl;
			}
		}

		for(int iter=1;iter<=10;iter++){
			for(int ii=1;ii<=Num_Machine;ii++){
				double rnd=rand()*1.0/RAND_MAX;
				double sum_prob=0.0;
				for(int j=1;j<=(int)Lamuda_list[ii-1].size();j++){
					vector<double>::iterator pp=Prob_list[ii-1].begin()+j-1;
					if(rnd>=sum_prob && rnd<sum_prob+(*pp)){
						fix_y[ii]=(*(Lamuda_list[ii-1].begin()+j-1));
						break;
					}
					sum_prob+=(*pp);
				}
			}
		}

		//cout<<"randomly selected fix_y[]"<<endl;
		//for(int i=1;i<=Num_Machine;i++){
		//	cout<<"i="<<i<<",fix_y="<<fix_y[i]<<endl;
		//}
		for(int i=0;i<=y_limit;i++){ solution[i]=0.0;}

	}
	delete [] LP_soln;
	delete [] fix_y;
}
*/

int AT::pick(vector< pair< pair<int,int>,double> > & candidate_list,int RCL_length,vector<double> & SL_avg,vector<double> & SL_best,double best_obj,double delta){
	//cout<<"SL_avg length="<<SL_avg.size()<<endl;
	vector<double> q;
	double sum=0;
	for(int j=1;j<=RCL_length;j++){
		int sim=candidate_list[j-1].first.first;
		int Lamuda_id=candidate_list[j-1].first.second;
		//cout<<"sim="<<sim<<",Lamuda_id="<<Lamuda_id<<endl;
		int index=index_L_sim_lamuda(sim,Lamuda_id);
		//cout<<"index="<<index<<endl;
		double temp;
		if(SL_avg[index]<1){//this sim_lamuda combination never be used before
			temp=1;
		}else{			
			temp=pow((best_obj/SL_avg[index])*pow(best_obj/SL_best[index],2.0),delta);
		}
		q.push_back(temp);
		sum+=temp;
	}
	//cout<<"sum="<<sum<<endl;
	//cout<<"probability:";
	vector<double> p((int)q.size(),0);
	for(int j=1;j<=(int)q.size();j++){
		p[j-1]=q[j-1]/sum;
	}
	//adjust the probability
	double sum_p=0;
	vector<int> indicator((int)p.size(),0);
	for(int j=1;j<=(int)p.size();j++){
		if(p[j-1]<0.01){
			p[j-1]=0.01;
			indicator[j-1]=1;
			sum_p+=p[j-1];
		}
	}
	double min_p=100;
	for(int j=1;j<=(int)p.size();j++){
		if(p[j-1]<min_p && indicator[j-1]<1){
			min_p=p[j-1];
		}
	}
	double sum2=0;
	for(int j=1;j<=(int)p.size();j++){
		if(indicator[j-1]<1){
			p[j-1]=p[j-1]/min_p;
			sum2+=p[j-1];
		}
	}
	for(int j=1;j<=(int)p.size();j++){
		if(indicator[j-1]<1){
			p[j-1]=p[j-1]/sum2*(1-sum_p);
		}
		//cout<<p[j-1]<<",";
	}
	//cout<<endl;
	//now we have probability
	double rnd=rand()*1.0/RAND_MAX;
	sum=0;
	int id=-1;
	for(int j=1;j<=(int)p.size();j++){
		if(sum<=rnd && rnd<sum+p[j-1]){
			id=j-1;
			break;
		}
		sum+=p[j-1];
	}
	return id;
}


int AT::pick(int RCL_length){
	//pick one randomly from RCL
	//cout<<"RAND_MAX="<<RAND_MAX<<endl;
	double delta=1.0/RCL_length;
//	cout<<"delta="<<delta<<endl;
	double rnd=rand()*1.0/RAND_MAX;
//	cout<<"rnd="<<rnd<<endl;
	int id;
	int found=-1;
	for(id=0;id<=RCL_length-1;id++){
		if(delta*id<=rnd && rnd<delta*(id+1)){
			found=id;
			break;
		}
	}
//	cout<<"found="<<found<<endl;
	return found;
}

int AT::is_sim_lamuda_feasible(int simid,int Lamuda_id,int * tooling_used,int * sim_used,int * SIM_number){
	//return 0 if infeasible; 1 if feasible
	//cout<<"simid="<<simid<<",Lamuda_id="<<Lamuda_id<<endl;
	int flag_machine=0,flag_tooling=0;
	vector<Lamuda_item>::iterator pL=Lamuda.begin()+Lamuda_id-1;
	//can we find the machine?
	if(sim_used[simid]+1<=SIM_number[simid]){ flag_machine=1;}
	else{ 
		return 0;
	}
	//can we find the tooling?
	//---------------------------------------------------
	//---------------------------------------------------
	vii p_tf=(*pL).tooling_vec.begin();
	if((*p_tf)==0 && (int)(*pL).tooling_vec.size()==1){//not require any tooling
		flag_tooling=1;
	}else{
		while(p_tf!=(*pL).tooling_vec.end()){
			int tf=(*p_tf);
			int offset=p_tf-(*pL).tooling_vec.begin();
			vii p_b=(*pL).b.begin()+offset;
			flag_tooling=1;

			for(int n=1;n<=(int)Temperature_Set.size();n++){
				vii pfind=find(Temperature_Set[n-1].set_elements.begin(),Temperature_Set[n-1].set_elements.end(),(*pL).temperature);
				if(pfind!=Temperature_Set[n-1].set_elements.end()){
					int lhs=0;
					for(int j=1;j<=(int)Temperature_Set[n-1].set_elements.size();j++){
						int tau=Temperature_Set[n-1].set_elements[j-1];
						int id=(tf-1)*Num_Temperature+tau-1;
						lhs+=tooling_used[id];
					}
					lhs+=(*p_b);
					int id2=(tf-1)*(int)Temperature_Set.size()+n-1;
					if(lhs>tooling_sum_rhs[id2].second){
						//cout<<"tf="<<tf<<",n="<<n<<",required="<<(*p_b)<<endl;
						//cout<<"lhs="<<lhs<<",rhs="<<tooling_sum_rhs[id2].second<<endl;
						//cout<<"flag_tooling=0"<<endl;
						flag_tooling=0;
						break;
					}
				}
			}
			if(!flag_tooling){
				break;
			}
			p_tf++;
		}
	}

	//------------------------only for test
	/*
	if(!flag_tooling){
		for(int tf=1;tf<=Num_Tooling_Family;tf++){
			for(int tau=1;tau<=Num_Temperature;tau++){
				int id=(tf-1)*Num_Temperature+tau-1;
				cout<<"tf="<<tf<<",tau="<<tau<<",used="<<tooling_used[id]<<",";
			}
			cout<<endl;
		}
		abort();
	}
	*/
	if(flag_machine && flag_tooling){
		return 1;
	}else{
		return 0;
	}
}

void AT::generate_reports(const double * solution){
	double total_shortage=0.0;
	for(int id=x_limit+1;id<=x_limit+Num_Keydevice+Num_Package;id++){
		total_shortage+=solution[id];
	}
	total_shortage=total_shortage*coef;

	double total_weight=0.0;
	ofstream f_soln("Debug/debug_solution.csv");
	f_soln<<"Machine_Instance_ID,Machine_Family_ID,Machine_Instance_Name,Lot_ID,Lot_Name,Quantity,Lot_Weight,Device ID,Device_Name,Route_ID,Tooling_Family_ID,Tooling_Family_Name,Certification,Value"<<endl;
	int count=0;
	for (int iColumn=0;iColumn<=x_limit;iColumn++) {
		double value=solution[iColumn];
		if(value>0.001){
			count++;
			vector<index_node>::iterator p_ind=index_vector.begin()+iColumn;
			int i=(*p_ind).i;
			int l=(*p_ind).l;
			int s=(*p_ind).s;
			total_weight+=w_l[l]*value;
			f_soln<<i<<","<<machine_mg[i]<<",";
			f_soln<<MI[i-1]<<","<<l<<",";
			f_soln<<LOT[l-1]<<","<<n_l_chips[l]<<","<<w_l[l]<<",";
			int d=lots_device[l];
			f_soln<<d<<",";
			f_soln<<DEV[d-1]<<","<<s<<",";
			vector<tooling_configure>::iterator p_S=S.begin()+s-1;
			vii p_int=(*p_S).tf.tools.begin();
			int tf=(*p_int);
			f_soln<<tf<<",";
			if(tf>0){
				f_soln<<TF[tf-1]<<","<<(*p_S).temperature<<","<<value<<endl;
			}else{//no tooling requirement
				f_soln<<","<<(*p_S).temperature<<","<<value<<endl;
			}
			p_int++;
			while(p_int!=(*p_S).tf.tools.end()){//append other tooling 
				for(int m=1;m<=9;m++){ f_soln<<",";}
				int tf=(*p_int);
				f_soln<<tf<<",";
				if(tf>0){
					f_soln<<TF[tf-1]<<","<<(*p_S).temperature<<endl;
				}else{
					f_soln<<","<<(*p_S).temperature<<","<<value<<endl;
				}
				p_int++;
			}
			if(value>0){//initialize lot_set_heur vector
				vii p_find=find(lot_set_heur.begin(),lot_set_heur.end(),l);
				if(p_find==lot_set_heur.end()){
					lot_set_heur.push_back(l);
				}
			}
		}
	}
	f_soln.close();
	f_soln.clear();
	//-------------------------------------------------
	total_weight=0.0;
	f_soln.open("Output/solution.csv");
	f_soln<<"Machine_Instance,Machine_Family_Name,Lot_Name,Quantity,Lot_Weight,Device_Name,Tooling_Family_Name,Certification"<<endl;
	count=0;
	for (int iColumn=0;iColumn<=x_limit;iColumn++) {
		double value=solution[iColumn];
		if(value>0.001){
			count++;
			vector<index_node>::iterator p_ind=index_vector.begin()+iColumn;
			int i=(*p_ind).i;
			int l=(*p_ind).l;
			int s=(*p_ind).s;
			total_weight+=w_l[l]*value;
			int mach=0;
			for(int id=1;id<=(int)Machine_vector.size();id++){
				if(Machine_vector[id-1].id==i){ mach=id;}
			}
			int mg=machine_mg[i];
			f_soln<<Machine_vector[mach-1].name<<","<<MF[mg-1]<<",";
			vector<Machine>::iterator p_M=Machine_vector.begin();
			vector<ch_array>::iterator p_ch=lot_name.begin();
			while(p_ch!=lot_name.end()){
				if((*p_ch).id==l){ break;}
				p_ch++;
			}
			f_soln<<(*p_ch).char_arry<<","<<n_l_chips[l]<<","<<w_l[l]<<",";
			int d=lots_device[l];
			p_ch=device_name.begin();
			while(p_ch!=device_name.end()){
				if((*p_ch).id==d){ break;}
				p_ch++;
			}
			f_soln<<(*p_ch).char_arry<<",";
			vector<tooling_configure>::iterator p_S=S.begin()+s-1;
			vii p_int=(*p_S).tf.tools.begin();
			int tf=(*p_int);
			if(tf>0){
				f_soln<<TF[tf-1]<<","<<(*p_S).temperature<<endl;
			}else{
				f_soln<<","<<(*p_S).temperature<<endl;
			}
			p_int++;
			while(p_int!=(*p_S).tf.tools.end()){
				for(int m=1;m<=6;m++){ f_soln<<",";}
				int tf=(*p_int);
				if(tf>0){
					f_soln<<TF[tf-1]<<","<<(*p_S).temperature<<endl;
				}else{
					f_soln<<","<<(*p_S).temperature<<endl;
				}
				p_int++;
			}
			if(value>0){//initialize lot_set_heur vector
				vii p_find=find(lot_set_heur.begin(),lot_set_heur.end(),l);
				if(p_find==lot_set_heur.end()){
					lot_set_heur.push_back(l);
				}
			}
		}
	}
	f_soln.close();
	//----------------------------------------------------------------------
	show_lot_set_heur();
	/*	
	ofstream f_shortage("Output/shortage.csv");
	f_shortage<<"Device,Require amount,Shorage"<<endl;
	for(int i=x_limit+1;i<=x_limit+Num_Keydevice+Num_Package;i++){
		if(i<=x_limit+Num_Keydevice){//keydevice
			vii p_int=Key_Devices.begin()+i-x_limit-1;
			int d=(*p_int); 
			f_shortage<<DEV[d-1]<<","<<n_k_minchips[d]<<","<<solution[i]*coef<<endl;
		}else{//package
			vii p_int=Packages.begin()+i-x_limit-Num_Keydevice-1;
			int d=(*p_int);
			f_shortage<<DEV[d-1]<<","<<n_p_minchips[d]<<","<<solution[i]*coef<<endl;
		}

	}
	f_shortage.close();
	*/	
	cout<<"Num_Key_device="<<Num_Keydevice<<endl;
	cout<<"Num_Package_device="<<Num_Package<<endl;
	cout<<"Num_Key_Package="<<(int)Key_PKG.size()<<endl;
	cout<<"Num_Key_PIN_Package="<<(int)Key_PIN_PKG.size()<<endl;

	cout<<"total_weight="<<total_weight<<endl;
	cout<<"total_shortage="<<total_shortage<<endl;
	//---------------------------------------------------------------------------
	/*
	ofstream f_package("Output/package_production.csv");
	f_package<<"Device,Required amount,Finished amount,Shortage"<<endl;
	vii p_package=Packages.begin();
	while(p_package!=Packages.end()){
		int d=(*p_package);
		double production=0.0;
		f_package<<DEV[d-1]<<","<<n_p_minchips[d]<<",";
		vii p_lot=(*(Devices.begin()+d-1)).device_lots.begin();
		while(p_lot!=(*(Devices.begin()+d-1)).device_lots.end()){
			int l=(*p_lot);
			vii p_m=M_l[l].int_vector.begin();
			while(p_m!=M_l[l].int_vector.end()){
				int i=(*p_m);
				int sim=machine_sim[i];
				int index=index_S_sim_d(sim,d);
				vii p_s=S_sim_d[index].second.int_vector.begin();
				while(p_s!=S_sim_d[index].second.int_vector.end()){
					int s=(*p_s);
					int index=index2_x(i,l,s);
					double value=solution[index];
					production+=value*n_l_chips[l];
					p_s++;
				}
				p_m++;
			}
			p_lot++;
		}
		f_package<<production<<","<<n_p_minchips[d]-production<<endl;
		p_package++;
	}
	f_package.close();
	*/	
	//--------------------------------------------------------------------------
	ofstream f_keydevice("Output/keydevice_production.csv");
	f_keydevice<<"Device,Target amount,Finished amount,Shortage"<<endl;
	vii p_keydevice=Key_Devices.begin();
	while(p_keydevice!=Key_Devices.end()){
		int d=(*p_keydevice);
		double production=0.0;
		f_keydevice<<DEV[d-1]<<","<<n_k_minchips[d]<<",";
		vii p_lot=(*(Devices.begin()+d-1)).device_lots.begin();
		while(p_lot!=(*(Devices.begin()+d-1)).device_lots.end()){
			int l=(*p_lot);
			vii p_m=M_l[l].int_vector.begin();
			while(p_m!=M_l[l].int_vector.end()){
				int i=(*p_m);
				int sim=machine_sim[i];
				int index=index_S_sim_d(sim,d);
				vii p_s=S_sim_d[index].second.int_vector.begin();
				while(p_s!=S_sim_d[index].second.int_vector.end()){
					int s=(*p_s);
					int index=index2_x(i,l,s);
					double value=solution[index];
					production+=value*n_l_chips[l];
					p_s++;
				}
				p_m++;
			}
			p_lot++;
		}
		f_keydevice<<production<<","<<n_k_minchips[d]-production<<endl;
		p_keydevice++;
	}
	f_keydevice.close();
	//--------------------------------------------------------------------------
	ofstream f_time("Output/time.csv");
	f_time<<"Machine,Time"<<endl;
	for(int i=1;i<=Num_Machine;i++){
		vector<int>::iterator p_Lamuda=Lamuda_i[i].int_vector.begin();
		while(p_Lamuda!=Lamuda_i[i].int_vector.end()){
			int Lamuda_id=(*p_Lamuda);
			int offset=index2_y(i,Lamuda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;
			int index=y_limit+1+offset;
			if(solution[index]>1E-06){
				f_time<<MI[i-1]<<","<<solution[index]<<endl;
			}
			p_Lamuda++;
		}
	}
	f_time.close();
	//--------------------------------------------------------------------------
	ofstream f_kp("Output/key_package_production.csv");
	f_kp<<"Key_Package_Name,Target_amount,Finished_amount,Shortage"<<endl;	
	for(int kp=1;kp<=(int)Key_PKG.size();kp++){
		int pkg_id=Key_PKG[kp-1];
		int production=0;	
		for(int l=1;l<=Num_Lot;l++){
			int d=lots_device[l];
			if(device_PKG[d]==pkg_id){	
				for(int id=1;id<=(int)M_l[l].int_vector.size();id++){
					int i=M_l[l].int_vector[id-1];
					int sim=machine_sim[i];
					int index=index_S_sim_d(sim,d);
					for(int id2=1;id2<=(int)S_sim_d[index].second.int_vector.size();id2++){
						int s=S_sim_d[index].second.int_vector[id2-1];
						int x_id=index2_x(i,l,s);
						production+=solution[x_id]*n_l_chips[l];	
					}	
				}	
			}//end_if	
		}	
		f_kp<<PKG[pkg_id-1]<<","<<n_kp_minchips[pkg_id]<<","<<production<<","<<n_kp_minchips[pkg_id]-production<<endl;	
	}
	f_kp.close();	
	//--------------------------------------------------------------------------
	ofstream f_kpp("Output/key_pin_package_production.csv");
	f_kpp<<"Package_Name,Pin,Target_amount,Finished_amount,Shortage"<<endl;	
	for(int kpp=1;kpp<=(int)Key_PIN_PKG.size();kpp++){
		int pkg_id=Key_PIN_PKG[kpp-1].first.second;
		int pin=Key_PIN_PKG[kpp-1].first.first;
		int target=Key_PIN_PKG[kpp-1].second;	
		int production=0;
		for(int l=1;l<=Num_Lot;l++){
			int d=lots_device[l];
			if(device_PKG[d]==pkg_id && device_PIN[d]==pin){
				for(int id=1;id<=(int)M_l[l].int_vector.size();id++){
					int i=M_l[l].int_vector[id-1];
					int sim=machine_sim[i];
					int index=index_S_sim_d(sim,d);
					for(int id2=1;id2<=(int)S_sim_d[index].second.int_vector.size();id2++){
						int s=S_sim_d[index].second.int_vector[id2-1];
						int x_id=index2_x(i,l,s);
						production+=solution[x_id]*n_l_chips[l];	
					}		
				}		
			}//end_if	
		}	
		f_kpp<<PKG[pkg_id-1]<<","<<pin<<","<<target<<","<<production<<","<<target-production<<endl;
	}
	f_kpp.close();
}

void AT::soln_pool_item_to_solution(struct soln_pool_item & soln,double * solution){
	//convert a solution_pool_item to a solution
	for(int id=0;id<=y_limit+(y_limit-x_limit);id++){ solution[id]=0.0;}
	vector< pair<int,int> > production;
	for(int d=1;d<=Num_Device;d++){ production.push_back(make_pair(d,0));}
	//file x_ils and y_i Lamuda
	for(int id=1;id<=(int)soln.sub_soln.size();id++){
		int i=soln.sub_soln[id-1].i;
		int Lamuda_id=soln.sub_soln[id-1].Lamuda_id;
		if(Lamuda_id>0){//the machine is in used
			for(int id2=1;id2<=(int)soln.sub_soln[id-1].lots.int_vector.size();id2++){
				int l=soln.sub_soln[id-1].lots.int_vector[id2-1];
				int s=max_rate_s(i,l,Lamuda_id);
				
				int index=index2_x(i,l,s);
				solution[index]=1.0;
				
				index=index2_y(i,Lamuda_id);
				solution[index]=1.0;
				
				index=y_limit+index2_y(i,Lamuda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size();
				solution[index]=soln.sub_soln[id-1].time_used;

				int d=lots_device[l];
				production[d-1].second+=n_l_chips[l];
			}
		}
	}
	//fill delta
	int solution_id=x_limit;
	for(int id=1;id<=(int)Key_Devices.size();id++){
		int d=Key_Devices[id-1];
		solution_id++;
		if(production[d-1].second>=n_k_minchips[d]){
			solution[solution_id]=0.0;
		}else{
			solution[solution_id]=n_k_minchips[d]-production[d-1].second;
			solution[solution_id]=solution[solution_id]/coef;
		}
	}
	for(int id=1;id<=(int)Packages.size();id++){
		int d=Packages[id-1];
		solution_id++;
		if(production[d-1].second>=n_p_minchips[d]){
			solution[solution_id]=0.0;
		}else{
			solution[solution_id]=n_p_minchips[d]-production[d-1].second;
			solution[solution_id]=solution[solution_id]/coef;
		}
	}
	//get the production for KP and KPP
	vector<int> PKG_production((int)PKG.size(),0);
	vector<int> PIN_PKG_production((int)PKG.size()*(int)PIN_order.size()+1,0);
	for(int i=1;i<=Num_Machine;i++){
		int Lamuda_id=soln.sub_soln[i-1].Lamuda_id;
		vii p_lot=soln.sub_soln[i-1].lots.int_vector.begin();
		while(p_lot!=soln.sub_soln[i-1].lots.int_vector.end()){
			int l=(*p_lot);
			int d=lots_device[l];
			int pkg_id=device_PKG[d];
			int pin_num=device_PIN[d];
			vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);
			int pin_order=pfind-PIN_order.begin()+1;
			PKG_production[pkg_id-1]+=n_l_chips[l];
			int index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;
			PIN_PKG_production[index]+=n_l_chips[l];
			p_lot++;
		}
	}
	for(int kp=1;kp<=(int)Key_PKG.size();kp++){
		solution_id++;
		int pkg_id=Key_PKG[kp-1];
		int diff=n_kp_minchips[pkg_id]-PKG_production[pkg_id-1];
		if(diff>0){
			solution[solution_id]=diff/coef;
		}else{
			solution[solution_id]=0.0;
		}
	}
	for(int kpp=1;kpp<=(int)Key_PIN_PKG.size();kpp++){
		solution_id++;
		int pkg_id=Key_PIN_PKG[kpp-1].first.second;
		int pin_num=Key_PIN_PKG[kpp-1].first.first;
		vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);
		int pin_order=pfind-PIN_order.begin()+1;
		int index=(pkg_id-1)*PIN_order.size()+pin_order-1;
		int diff=Key_PIN_PKG[kpp-1].second-PIN_PKG_production[index];
		if(diff>0){
			solution[solution_id]=diff/coef;
		}else{
			solution[solution_id]=0.0;
		}
	}
}

void AT::minimize_makespan(struct soln_pool_item & soln){
	bool exit_flag=false;
	while(!exit_flag){
		//get the makespan
		double makespan=0.0;
		int makespan_id=-1;
		for(int id=1;id<=(int)soln.sub_soln.size();id++){
			int i=soln.sub_soln[id-1].i;
			int Lamuda_id=soln.sub_soln[id-1].Lamuda_id;
			if(Lamuda_id>0){//the machine is in used
				double time_used=soln.sub_soln[id-1].time_used;
				if(time_used>makespan){
					makespan=time_used;
					makespan_id=id;
				}
			}
		}
		cout<<"current makespan="<<makespan<<endl;
		//
		if(makespan_id>0){
			exit_flag=true;
			for(int id=1;id<=(int)soln.sub_soln[makespan_id-1].lots.int_vector.size();id++){
				int l=soln.sub_soln[makespan_id-1].lots.int_vector[id-1];
				//try to assign lot l to some other machine
				vector<int> assignable;
				for(int id2=1;id2<=(int)soln.sub_soln.size();id2++){
					int i=soln.sub_soln[id2-1].i;
					int sim=machine_sim[i];
					int Lamuda_id=soln.sub_soln[id2-1].Lamuda_id;
					if(Lamuda_id>0){
						vector< pair<int,int> >::iterator p_pair=sim_lamuda_L[l-1].second.sim_lamuda_pairs.begin();
						while(p_pair!=sim_lamuda_L[l-1].second.sim_lamuda_pairs.end()){
							if((*p_pair).first==sim && (*p_pair).second==Lamuda_id){
								assignable.push_back(id2); break;
							}
							p_pair++;
						}
					}
				}
				if((int)assignable.size()>=1){//lot l can be assgined to the vector
					int s_from=max_rate_s(soln.sub_soln[makespan_id-1].i,l,soln.sub_soln[makespan_id-1].Lamuda_id);
					double time_from=n_l_chips[l]/rate[s_from];
					double best_time=1E+10;
					double best_ID=-1;
					for(int id3=1;id3<=(int)assignable.size();id3++){
						int ID=assignable[id3-1];
						int s_to=max_rate_s(soln.sub_soln[ID-1].i,l,soln.sub_soln[ID-1].Lamuda_id);
						double time_to=n_l_chips[l]/rate[s_to];
						if(soln.sub_soln[ID-1].time_used+time_to<=H[soln.sub_soln[ID-1].i] &&
							soln.sub_soln[ID-1].time_used+time_to<makespan-0.0001){
								double new_time=soln.sub_soln[ID-1].time_used+time_to;
								if(new_time<soln.sub_soln[makespan_id-1].time_used-time_from){
									new_time=soln.sub_soln[makespan_id-1].time_used-time_from;
								}
								if(new_time<best_time){
									best_time=new_time; best_ID=ID;
								}
						}
					}//end_for
					if(best_ID>0){
						vii pfind=find(soln.sub_soln[makespan_id-1].lots.int_vector.begin(),soln.sub_soln[makespan_id-1].lots.int_vector.end(),l);
						soln.sub_soln[makespan_id-1].lots.int_vector.erase(pfind);
						soln.sub_soln[makespan_id-1].time_used-=time_from;
						soln.sub_soln[best_ID-1].lots.int_vector.push_back(l);
						int s_to_best=max_rate_s(soln.sub_soln[best_ID-1].i,l,soln.sub_soln[best_ID-1].Lamuda_id);
						double time_to_best=n_l_chips[l]/rate[s_to_best];
						soln.sub_soln[best_ID-1].time_used+=time_to_best;
						exit_flag=false;
						break;
					}
				}//end_if
			}
		}
		else{
			cout<<"Wrong makespan id,probably no machine assignment"<<endl;
			abort();
		}
	}
}

void AT::show_targets(){
	ofstream f_target("Debug/debug_Targets.txt");
	f_target<<"Target for key devices"<<endl;
	for(int id=1;id<=(int)Key_Devices.size();id++){
		int dev=Key_Devices[id-1];
		f_target<<DEV[dev-1]<<","<<n_k_minchips[dev]<<endl;
	}
	f_target<<"Target for key packages"<<endl;
	for(int id=1;id<=(int)Key_PKG.size();id++){
		int pkg=Key_PKG[id-1];
		f_target<<PKG[pkg-1]<<","<<n_kp_minchips[pkg]<<endl;
	}
	f_target<<"Target for key pin packages"<<endl;
	for(int id=1;id<=(int)Key_PIN_PKG.size();id++){
		int pin=Key_PIN_PKG[id-1].first.first;
		int pkg=Key_PIN_PKG[id-1].first.second;
		int target=Key_PIN_PKG[id-1].second;
		f_target<<PKG[pkg-1]<<","<<pin<<","<<target<<endl;
	}
	f_target.close();
}

void AT::show_PIN_PKG(){
	ofstream fpkg("Debug/debug_PIN_PKG.txt");
	fpkg<<"ID,PKG Name,PKG_penalty"<<endl;
	for(int id=1;id<=(int)PKG.size();id++){
		fpkg<<id<<","<<PKG[id-1]<<","<<PKG_penalty[id]<<endl;
	}
	fpkg<<endl;
	for(int id=1;id<=(int)PIN_order.size();id++){
		fpkg<<PIN_order[id-1]<<",";
	}
	fpkg<<endl;
	fpkg<<"Device ID,Device Name,PKG Name,PIN"<<endl;
	for(int d=1;d<=Num_Device;d++){
		fpkg<<d<<","<<DEV[d-1]<<","<<
		  (device_PKG[d] < 1 ? "__no pkg__" : PKG[device_PKG[d]-1])
		    <<","<<device_PIN[d]<<endl;
	}
	fpkg.close();
}

