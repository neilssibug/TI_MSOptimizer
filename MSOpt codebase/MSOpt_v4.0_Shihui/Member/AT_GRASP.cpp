// AT.cpp : Defines the entry point for the console application.

//----------------------------------------------

//#include<ilcplex/ilocplex>

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

#include "AT_GRASP.h"



using namespace std;

using namespace stdcomb;



AT::AT(){

	//constructor of the AT class

	//initialize the parameters

	Max_Num_Tooling=0;

	Max_Num_Tooling_Family=0;

	Max_Num_Machine=0;

	Max_Num_Lot=0;

	Max_Num_Device=0;

	Max_Num_Temperature=0;

	Max_Num_Tooling_Configure=0;



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



	//comment by Zhufeng,Feb 16, 2012

	//This is not used anymore. Just keep INF = 0;

	INF = 0;

	////--- added by yufen -----

	//INF = 100000000;

	////--- end yufen ----------

	//end by Zhufeng, Feb 16, 2012







   //comment by Zhufeng

	//strcpy(General_Snapshoot_time.str,"");

	//end by Zhufeng

	strcpy(General_Snapshoot_time.str,"00\00\00\00:00:00");



}



AT::~AT(){

	//destructor of the AT class

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



	delete [] H;

	delete [] effective_starting_time;	

	delete [] Tooling_Setup_Time;

	delete [] n_l_chips;

	delete [] n_k_minchips;

	delete [] n_p_minchips;

	delete [] n_kp_minchips;

	delete [] w_l;

	delete [] rate;

	delete [] eps;

	delete [] penalty;

	delete [] PKG_penalty;

	delete [] device_PKG;

	delete [] device_PIN;

	delete [] lots_device;

	delete [] machine_mg;

	delete [] machine_sim;

	delete [] tooling_tf;

	delete [] L_i;

	delete [] L_second_pass_i;

	delete [] M_l;

	delete [] S_i;

	delete [] Lambda_i;

	delete [] M_lambda;

	delete [] p_device;

	delete [] package_qty;

	delete [] keydevice_qty;

	delete [] n_t_n_tooling;

	delete [] a_st;

	delete [] L_i_d;

	delete [] index_vector_flag;



	//Added by Zhufeng, Feb 26, 2012

	//delete[] Lambda_i2;

	//end by Zhufeng, Feb 26, 2012

}



int AT::AT_main()

{	

	//the main entry point of the GRASP algorithm	

	time_t t_prog_start; //keep a record of the program starting time

	time(&t_prog_start);

	clock_t start_cpu_time;

	start_cpu_time = clock();



	cout<<"The starting time is "<<ctime(&t_prog_start)<<endl;

	//test();

	//read the configuration file "input.txt"

	char temp[1000],pdes[500];

	ifstream f_input("Data/input.txt");

	f_input.getline(temp,1000,'\n');

	int k=0,l=0;

	k=strtok2(pdes,temp,l,' ');//get Max_Num_Tooling

	Max_Num_Tooling=atoi(pdes);



	f_input.getline(temp,1000,'\n');//get Max_Num_Tooling_Family

	l=0;

	k=strtok2(pdes,temp,l,' ');

	Max_Num_Tooling_Family=atoi(pdes);



	f_input.getline(temp,1000,'\n');//get Max_Num_Machine

	l=0;

	k=strtok2(pdes,temp,l,' ');

	Max_Num_Machine=atoi(pdes);



	f_input.getline(temp,1000,'\n');//get Max_Num_Lot

	l=0;

	k=strtok2(pdes,temp,l,' ');

	Max_Num_Lot=atoi(pdes);



	f_input.getline(temp,1000,'\n');//get Max_Num_Device

	l=0;

	k=strtok2(pdes,temp,l,' ');

	Max_Num_Device=atoi(pdes);



	f_input.getline(temp,1000,'\n');//get Max_Num_Temperature

	l=0;

	k=strtok2(pdes,temp,l,' ');

	Max_Num_Temperature=atoi(pdes);



	f_input.getline(temp,1000,'\n');//get Max_Num_Tooling_Configure

	l=0;

	k=strtok2(pdes,temp,l,' ');

	Max_Num_Tooling_Configure=atoi(pdes);



	f_input.getline(temp,1000,'\n');// get Generate_Option

	l=0;

	k=strtok2(pdes,temp,l,' ');

	Generate_Option=atoi(pdes);



	f_input.getline(temp,1000,'\n');// get Load_Unload_Time

	l=0;

	k=strtok2(pdes,temp,l,' ');

	Load_Unload_Time=(double)atof(pdes);



	f_input.getline(temp,1000,'\n');// get Run_Option

	l=0;

	k=strtok2(pdes,temp,l,' ');

	Run_Option=atoi(pdes);



	f_input.getline(temp,1000,'\n');//get LP_IP

	l=0;

	k=strtok2(pdes,temp,l,' ');

	LP_IP=atoi(pdes);



	f_input.getline(temp,1000,'\n');//get new_old_model

	l=0;

	k=strtok2(pdes,temp,l,' ');

	new_old_model=atoi(pdes);



	f_input.getline(temp,1000,'\n');//get multi-pass option

	l=0;

	k=strtok2(pdes,temp,l,' ');

	

	Multipass_Option=atoi(pdes);



	if(Multipass_Option==1)

	{

		cout<<"use multi-pass!"<<endl;

	}



	f_input.getline(temp,1000,'\n');//get fixed initial setup option

	l=0;

	k=strtok2(pdes,temp,l,' ');



	fix_initial_setup = atoi(pdes);



	if(fix_initial_setup==1){

		cout<<"fix initial setup before changeover!"<<endl;

	}else{

		cout<<"Don't fix initial setup before changeover!"<<endl;

	}



    f_input.getline(temp,1000,'\n');//get resetup option

	l=0;

	k=strtok2(pdes,temp,l,' ');



	resetup_option = atoi(pdes);


	//Changed by Shihui Jia, Setp 8,2014 from resetup_option==1 to resetup_option>=1
	if(resetup_option>=1){

		cout<<"Consider resetup, i.e. runnning changeover procedure "<<endl;

	}else{

		cout<<"No changeover or resetup"<<endl;

	}



	f_input.getline(temp,1000,'\n');//get the number of iterations for heuristics after the first GRASP period

	l=0;

	k=strtok2(pdes,temp,l,' ');

    grasp_times = atoi(pdes);



	cout<<"It run "<<grasp_times<<" iterations of multi-pass heuristics after GRASP of single-pass solution"<<endl;



	f_input.getline(temp,1000,'\n');//get the option that whether the last lot could be beyond the time horizon

	l=0;

	k=strtok2(pdes,temp,l,' ');

    beyond_horizon = atoi(pdes);



	if(beyond_horizon ==1)

	{

		cout<<"The last lot assigned to a machine could be beyond the time horizon."<<endl;

	}

	else

	{

		cout<<"The last lot assigned to a machine couldn't be beyond the time horizon."<<endl;

	}

	

	

	//--------------------------------------------------------------------------

	cout<<"-----------------k Important parameters -----------------"<<endl;

	cout<<"Max_Num_Tooling="<<Max_Num_Tooling<<endl;

	cout<<"Max_Num_Tooling_Family="<<Max_Num_Tooling_Family<<endl;

	cout<<"Max_Num_Machine="<<Max_Num_Machine<<endl;

	cout<<"Max_Num_Lot="<<Max_Num_Lot<<endl;

	cout<<"Max_Num_Device="<<Max_Num_Device<<endl;

	cout<<"Max_Num_Temperature="<<Max_Num_Temperature<<endl;

	cout<<"Max_Num_Tooling_Configure="<<Max_Num_Tooling_Configure<<endl;

	//-------------------------------------------------initialize the data member

	H=new double [Max_Num_Machine];

	effective_starting_time=new double [Max_Num_Machine];	

	for(int j=0;j<=Max_Num_Machine-1;j++){ H[j]=0.0; effective_starting_time[j]=0.0;}	

	Tooling_Setup_Time=new double [Max_Num_Tooling_Family];

	for(int j=0;j<=Max_Num_Tooling_Family-1;j++){ Tooling_Setup_Time[j]=0;}//initilaize the Tooling_Setup_Time[]

	n_l_chips=new int [Max_Num_Lot];

	n_k_minchips=new int [Max_Num_Device];

	n_p_minchips=new int [Max_Num_Device];

	n_kp_minchips=new int [Max_Num_Device];

	w_l=new double [Max_Num_Lot];

	rate=new double [Max_Num_Tooling_Configure];

	eps=new double [Max_Num_Tooling_Configure];

	penalty=new double [Max_Num_Device];

	PKG_penalty=new double [Max_Num_Device+1];

	device_PKG=new int [Max_Num_Device];

	device_PIN=new int [Max_Num_Device];

	lots_device=new int [Max_Num_Lot];

	machine_mg=new int [Max_Num_Machine];

	machine_sim=new int [Max_Num_Machine];

	tooling_tf=new int [Max_Num_Tooling];

	L_i=new vec_int [Max_Num_Machine];

	L_second_pass_i=new vec_int[Max_Num_Machine];

	M_l=new vec_int [Max_Num_Lot];

	S_i=new vec_tc [Max_Num_Machine];

	Lambda_i=new vec_int [Max_Num_Machine];

	M_lambda=new vec_int [Max_Num_Tooling_Configure];

	p_device=new vec_int [Max_Num_Device];

	package_qty=new int [Max_Num_Device];

	keydevice_qty=new int [Max_Num_Device];

	n_t_n_tooling=new int [Max_Num_Tooling_Family*Max_Num_Temperature];

	a_st=new int [Max_Num_Tooling_Configure*Max_Num_Tooling_Family];

	L_i_d=new vec_int [Max_Num_Machine*Max_Num_Device];

	index_vector_flag=new int [Max_Num_Machine];



	//added by Zhufeng for logpoint, Aug30, 2011

	lot_logpoint=new int [Max_Num_Lot];

	lot_operation=new int [Max_Num_Lot];

    //end by Zhufeng 



	//Added by Zhufeng, for not overtake initial setup and relaxible initial setups, Feb 26, 2012

	//Lambda_i2=new vec_int [Max_Num_Machine];

	//end by Zhufeng, Feb 26, 2012





	//--------------------------------------------------------------------------



	if(!Generate_Option){//generate the case randomly

		//this part is not used any more

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

		//generate the case from input files

		f_input.getline(temp,1000,'\n');

		f_input.getline(temp,1000,'\n'); //Max_Iter

		l=0;

		k=strtok2(pdes,temp,l,' ');

		Max_Iter=atoi(pdes);

		cout<<"It will run "<< Max_Iter <<" iterations for the GRASP of single-pass solution"<<endl;

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







	time_t end_time; //keep a record the end time

	time(&end_time);

	cout<<"The end time is "<<ctime(&end_time)<<endl;

	double diff=difftime(end_time,t_prog_start);

	cout<<"It has been running for "<<diff<<" seconds."<<endl;



	clock_t end_cpu_time;

	end_cpu_time = clock();

	double cpuTime = (double)difftime(end_cpu_time,start_cpu_time)/CLOCKS_PER_SEC;



	//added by Zhufeng
	fstream fcout("Output/resultsummery.txt", ios::out|ios::app);
	fcout<<"The end time is "<<ctime(&end_time)<<endl;
	fcout<<"The code has been running for "<<diff<<" seconds (real time)totally."<<endl;
	fcout<<"Total cpu time = "<<cpuTime<<" seconds"<<endl;
	fcout.close();
	//end by Zhufeng


    //added by Zhufeng 

	exit(0);

	//end Zhufeng

	//Commented by Zhufeng

	//return 0;

	//end Zhufeng

}



void AT::readFiles(){

	//this is the major function to read the files and process tha data

	char temp[1000];

	char pdes[100];

	//Added by Zhufeng. For exception process

	fstream f_log("Output/log_error.txt", ios::out|ios::app);

	//end by Zhufeng

	//read machines.csv

	ifstream f_machines("Data/machines.csv");

	f_machines.getline(temp,1000,'\n');

	f_machines.getline(temp,1000,'\n');

	Num_Machine=0;

	Num_Machine_Group=0;

	Num_Temperature=0;

	trim(temp);

	//The whole reading machines.csv has been Modified by Zhufeng

	while(!f_machines.eof() && strlen(temp)>0){

		

		int l=0,k=0;

		for(int i=1;i<=50;i++){ pdes[i]=' ';}

		k=strtok2(pdes,temp,l,',');//get machine family name

		trim(pdes);

		

		// --- added by yufen ---

		if(strlen(pdes)>0)

		{

			

		// --- end by yufen ---

			char * mf_name=new char[50];

		    strcpy(mf_name,pdes);

			l=k;

		    k=strtok2(pdes,temp,l,',');//get machine instance name

		    trim(pdes);

			char * mi_name=new char[50];

			

			strcpy(mi_name,pdes);

	

			l=strtok2(pdes,temp,k,',');

		

            k=strtok2(pdes,temp,l,',');//certification

			

			trim(pdes);

		

			int certification=atoi(pdes);

			

			

			if(strlen(mi_name)>0 && strlen(pdes)>0 && certification>0)

			{

				

			

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

						//added by Zhufeng

						f_log<<"In the machines.csv, "<<mi_name<<" appeared more than once. Only the last one would be used,and skip others."<<endl;

					    //end by Zhufeng

						delete [] mi_name;

						MI_name_exist=true; break;

					}

				}

				if(!MF_name_exist){//name does not exist in MF

					MF.push_back(mf_name);

					//NOTE: don't need to delete [] mf_name;

				}

				if(!MI_name_exist){//name does not exist in MI

					MI.push_back(mi_name);

					//NOTE: don't need to delete [] mi_name;

				}

				//added by Zhufeg

			}else

			{

				

				delete [] mf_name;

				delete [] mi_name;

				

			}

			//end Zhufeng

		// -- added by yufen ---

		}

		// --- end yufen ---

		f_machines.getline(temp,1000,'\n');

		trim(temp);	

	}//end of while(!f_machines.eof() && strlen(temp)>0){



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



	//---- added by yufen -----------------

	Machine_vector.resize((int)MI.size());

	//----- end yufen ---------------------

	

	



	//-----------------------------------------------------------------

	f_machines.close();

	f_machines.clear();

	f_machines.open("Data/machines.csv");

	f_machines.getline(temp,1000,'\n');

	f_machines.getline(temp,1000,'\n');

	trim(temp);



	



	// --- added by yufen --- modified by Zhufeng

	//memset(pdes,0,sizeof(pdes));

	int l=0,k=0;

	//k=strtok2(pdes,temp,l,',');//put mf in pdes

 	//trim(pdes);

    int row_no=2;

	while(!f_machines.eof() && strlen(temp)>0)

	{

		memset(pdes,0,sizeof(pdes));

		l=0;k=0;

		k=strtok2(pdes,temp,l,',');//put mf in pdes

		trim(pdes);

		struct Machine one_mc;

		if(strlen(pdes)>0)// if mf is not empty

		{

			int mg=0;

			for(int id=1;id<=(int)MF.size();id++){

				if(!strcmp(MF[id-1],pdes)){ mg=id;break;};

			}

			if(mg==0)

			{

				
				f_log<<"The "<<row_no<<"th row in machines.csv:Information incomplete. This machine instance is skipped."<<endl;

				memset(temp,0,sizeof(temp));

			    f_machines.getline(temp,1000,'\n');

				row_no++;

				



			}else

			{

				

				one_mc.mg=mg;

				//cout<<"	mg= "<<MF[one_mc.mg-1];

				l=strtok2(pdes,temp,k,',');//mi name

				trim(pdes);

				

				if(strlen(pdes)<=0) //if mi is  empty

				{

					//fstream f_log("Output/log_error.txt", ios::out|ios::app);

					f_log<<"The "<<row_no<<"th row in machines.csv:Machine_Instance is empty.The machine instance is skipped."<<endl;

					memset(temp,0,sizeof(temp));

			        f_machines.getline(temp,1000,'\n');

					row_no++;

					//f_log.close();

				}else

				{

					

					int mi=0;

					for(int id=1;id<=(int)MI.size();id++){

						if(!strcmp(MI[id-1],pdes)){ mi=id;break;};

					}

					//cout<<"	"<<mi<<"	"<<pdes<<endl;

					if(mi<1)
					{ 
						//fstream f_log("Output/log_error.txt", ios::out|ios::app);
						f_log<<"The "<<row_no<<"th row in machines.csv:Information incomplete.This machine instance is skipped."<<endl;

						memset(temp,0,sizeof(temp));

			            f_machines.getline(temp,1000,'\n');

						row_no++;



						//abort();

					}else

					{

						one_mc.id=mi;

						k=strtok2(pdes,temp,l,',');//mf_name

						trim(pdes);

						strcpy(one_mc.name,MI[one_mc.id-1]);

						//cout<<"	mi= "<<one_mc.name;

						memset(pdes,0,sizeof(pdes));

						l=strtok2(pdes,temp,k,',');//certification

						trim(pdes);

						//cout<<"strlen(pdes)="<<strlen(pdes)<<endl;

						if(strlen(pdes)<=0) //if the cerrification is empty

						{

							f_log<<"The "<<row_no<<"th row in machines.csv: the certification is empty.This machine instance is skipped."<<endl;

						    memset(temp,0,sizeof(temp));

			                f_machines.getline(temp,1000,'\n');

						    row_no++;



						}else

						{

							int temperature=atoi(pdes);

							if(temperature>Num_Temperature)

							{ Num_Temperature=temperature;}

							if(temperature<=0)

							{

								f_log<<"The "<<row_no<<"th row in machines.csv: the certification is non-positive.This machine instance is skipped."<<endl;

						        memset(temp,0,sizeof(temp));

			                    f_machines.getline(temp,1000,'\n');

						        row_no++;

							}else

							{

								one_mc.temperature_vector.push_back(temperature);

							

								if(!f_machines.eof())

								{

									char mf_name[50];

									char mi_name[50];

									char mf_name2[50];

						

									do

									{

										memset(temp,0,sizeof(temp));

										f_machines.getline(temp,1000,'\n');

										row_no++;

										if(!f_machines.eof())

										{

											memset(pdes,0,sizeof(pdes));

											l=0;

											k=0;

											k=strtok2(pdes,temp,l,',');//mf

											trim(pdes);

											strcpy(mf_name,pdes);

											l=strtok2(pdes,temp,k,',');//mi

                                            trim(pdes);

											strcpy(mi_name,pdes);

											k=strtok2(pdes,temp,l,',');//mf_name

											trim(pdes);

											strcpy(mf_name2,pdes);

                                           

											if(mg>0 && strlen(mf_name)<=0 && strlen(mi_name)<=0 && strlen(mf_name2)<=0)

											{

                                                //memset(pdes,0,sizeof(pdes));

												l=0;k=0;

												l=strtok2(pdes,temp,k,',');

												k=strtok2(pdes,temp,l,',');

												l=strtok2(pdes,temp,k,',');

                                                k=strtok2(pdes,temp,l,',');

												trim(pdes);

                                  									

												if(strlen(pdes)<=0) //if the cerrification is empty

												{

													f_log<<"The "<<row_no<<"th row in machines.csv: the certification is empty.This row was supposed to be one of multiple certifications for machine instance. This row (not the machine instance)is skipped."<<endl;

													//memset(temp,0,sizeof(temp));

													//f_machines.getline(temp,1000,'\n');

													//l=0;

													//k=0;

													//k=strtok2(pdes,temp,l,',');//mf

													//trim(pdes);

													//strcpy(mf_name,pdes);

													//l=strtok2(pdes,temp,k,',');//mi

             //                                       trim(pdes);

											  //      strcpy(mi_name,pdes);

													//k=strtok2(pdes,temp,l,',');//mf_name

											  //      trim(pdes);

											  //      strcpy(mf_name2,pdes);

													//row_no++;

												}else

												{

													int temperature=atoi(pdes);

													if(temperature>Num_Temperature){ Num_Temperature=temperature;}

													if(temperature<=0)

													{

														f_log<<"The "<<row_no<<"th row in machines.csv: the certification is non-positive.This row was supposed to be one of multiple certifications for machine instance.This row (not the machine instance)is skipped."<<endl;

														



													}else

													{

														one_mc.temperature_vector.push_back(temperature);

													}

												}

											}//end of if(mg>0 && strlen(mf_name)<=0)

										}//end of if(!f_machines.eof())

									}//end of do

									while(!f_machines.eof() && strlen(temp)>0 && strlen(mf_name)<=0 && strlen(mi_name)<=0 && strlen(mf_name2)<=0);

								}//end of if(!f_machines.eof())



                                //comment by Zhufeng. Since Num_Machine should be equal to number of MI, just let Num_Machine=(int)MI.size();

								//Num_Machine++;

								//end Zhufeng

								Machine_vector[one_mc.id-1] = one_mc;

							}//end of if(temperature<=0) else

						

						}//end of if(strlen(pdes)<=0) //if the cerrification is empty else

					}//end of if(mi<1) else

				}//end of if(strlen(pdes)<=0) //if mi is  empty else

				

				

				//end Zhufeng





			}//end of if(mg==0) else



			

		}//if(strlen(pdes)>0)// if mf is not empty

		else

		{

			//fstream f_log("Output/log_error.txt", ios::out|ios::app);
			f_log<<"The "<<row_no<<"th row in machines.csv: Machine_Family is empty.The machine instance is skipped."<<endl;

			memset(temp,0,sizeof(temp));

			f_machines.getline(temp,1000,'\n');

			row_no++;



		}//end of if(strlen(pdes)>0)// if mf is not empty else

	}//end of while(!f_machines.eof() && strlen(temp)>0)



	//added by Zhufeng

    Num_Machine=(int)MI.size();

	// ---  end yufen ---



	f_machines.close();

	//Added by Zhufeng

    //initialize the initial lot and initial setup part of Machine_vector after finish reading machines.csv.

	for(int i=1;i<=(int)Machine_vector.size();i++)

	{

		Machine_vector[i-1].hasInitialLot=false;

	    Machine_vector[i-1].hasInitialSetup=false;	

	    Machine_vector[i-1].hasInitialToolingConfigure=false;



	    Machine_vector[i-1].initial_lot=-1;  //the id in the vector of initial_running_lot

	    Machine_vector[i-1].initial_tool=-1;

	    Machine_vector[i-1].initial_temperature=-1;

	    Machine_vector[i-1].initial_tooling_setup=-1;		//lambda

	    Machine_vector[i-1].initial_tooling_configure=-1;	    //s

	}

	//end Zhufeng

	show_Machine_vector();



	//read tooling.csv

	

	cout<<"read file Data/tooling.csv"<<endl;	

	ifstream f_tooling("Data/tooling.csv");

	f_tooling.getline(temp,1000,'\n');

	f_tooling.getline(temp,1000,'\n');

	//added by Zhufeng

	row_no=2;

	//end by Zhufeng

	Num_Tooling=0;

	Num_Tooling_Family=0;

	trim(temp);	

	while(!f_tooling.eof() && strlen(temp)>0){

		int l=0,k=0;

		for(int i=1;i<=50;i++){ pdes[i]=' ';}

		k=strtok2(pdes,temp,l,',');//get tooling family name

		trim(pdes);

		//comment by Zhufeng

		/*char * tf_name=new char[50];

		strcpy(tf_name,pdes);

		l=k;

		k=strtok2(pdes,temp,l,',');//get tooling instance name

		trim(pdes);

		*/

        //end Zhufeng



		// --- added by yufen ---modified by Zhufeng

		if(strlen(pdes)<=0)

		{

			f_tooling.getline(temp,1000,'\n');

		    trim(temp);	

			row_no++;



		}else

		{

		// --- end yufen ---

			//MOdified by Zhufeng

			

			char * tf_name=new char[50];

		    strcpy(tf_name,pdes);

	

		    l=k;

		    k=strtok2(pdes,temp,l,',');//get tooling instance 

		    trim(pdes);

			char * ti_name=new char[50];

			strcpy(ti_name,pdes);

			

            l=strtok2(pdes,temp,k,',');//Tooling_Family_Name

			k=strtok2(pdes,temp,l,',');//Certification

            trim(pdes);

			int certification=atoi(pdes);



			if(strlen(ti_name)>0 && certification > 0)

			{



				bool TF_name_exist=false;

				bool TI_name_exist=false;

			

				for(int id=1;id<=(int)TF.size();id++){

					if(!strcmp(TF[id-1],tf_name)){//the tf_name already exists in TF

						delete [] tf_name;

						TF_name_exist=true;

			

						break;

						

					}

				}

				for(int id=1;id<=(int)TI.size();id++){

					if(!strcmp(TI[id-1],ti_name)){//the tf_name already exists in TF

						//added by Zhufeng

						f_log<<"In the tooling.csv, "<<ti_name<<" appeared more than once. Only the last one would be used,and skip others."<<endl;

					    //end by Zhufeng

						delete [] ti_name;

						TI_name_exist=true;

			

						break;

						

					}

				}

				if(!TF_name_exist){

					TF.push_back(tf_name);

					//Note:Cannot delete [] tf_name.TF is the vector of pointers, not the arrays. So the memory space with the pointer can not be deleted until TF[] is not used any more 

				}

				if(!TI_name_exist){

					TI.push_back(ti_name);

					//Note:Cannot delete [] ti_name;

				}

				

			}else

			{

				delete [] tf_name;

				delete [] ti_name;

			}//end of if(strlen(ti_name)>0 && certification > 0) else



            f_tooling.getline(temp,1000,'\n');

		    trim(temp);	

			row_no++;

			//end Zhufeng

		// --- added by yufen ---

		}

		// --- end yufen ---

		

	}//end of while (!f_tooling.eof() && strlen(temp)>0){

	//The following statement is used to deal with empty tooling in the route.csv file

	if(1){	

		char * name=new char[50];

		strcpy(name,"No_Tooling");	

		TF.push_back(name);	

		for(int i=1;i<=Num_Machine;i++){

			stringstream ss;

			ss<<name<<"-"<<i;	

			char * name2=new char[50];

			strcpy(name2,(ss.str()).c_str());

			TI.push_back(name2);

			//added by Zhufeng

			//delete [] name2;

			//end Zhufeng

		}

		//added by Zhufeng

		//Note: can not delete [] name here;

		

		//end by Zhufeng

	}	

	//////

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

	// --- commented by yufen ---

	/*f_tooling.getline(temp,1000,'\n');

	f_tooling.getline(temp,1000,'\n');

	trim(temp);	

	while(!f_tooling.eof() && strlen(temp)>0){

		memset(pdes,0,sizeof(pdes));

		int l=0,k=0;

		for(int i=1;i<=50;i++){ pdes[i]=' ';}

		k=strtok2(pdes,temp,l,',');//get tooling family name

		trim(pdes);

		int tf=0;

		for(int id=1;id<=(int)TF.size();id++){

			if(!strcmp(TF[id-1],pdes)){ tf=id;break;}

		}

		if(tf>0){//This is a new tooling family

			struct Tooling one_tool;

			one_tool.tf=tf;

			l=strtok2(pdes,temp,k,',');//get tooling instance

			trim(pdes);

			int ti=0;

			for(int id=1;id<=(int)TI.size();id++){

				if(!strcmp(TI[id-1],pdes)){ ti=id;break;}

			}

			if(ti<1){ cout<<"Wrong tooling instance id"<<endl; abort(); }

			one_tool.id=ti;

			k=strtok2(pdes,temp,l,',');//get tooling family name

			trim(pdes);

			strcpy(one_tool.name,TI[one_tool.id-1]);

			l=strtok2(pdes,temp,k,',');//certification

			trim(pdes);



			int temperature=atoi(pdes);

			if(temperature>Num_Temperature){ Num_Temperature=temperature;}

			if(temperature>0){

				one_tool.temperature_vector.push_back(temperature);

			}

			Num_Tooling++;

			Tooling_vector.push_back(one_tool);

		}

	

		else{//This is a continue tooling family from the previous line

			l=strtok2(pdes,temp,k,',');

			trim(pdes);

			k=strtok2(pdes,temp,l,',');

			trim(pdes);

			l=strtok2(pdes,temp,k,',');

			trim(pdes);

			int temperature=atoi(pdes);

			if(temperature>Num_Temperature){ Num_Temperature=temperature;}

			if(temperature>0){

				vector<Tooling>::iterator p_T=Tooling_vector.end()-1;

				(*p_T).temperature_vector.push_back(temperature);

			}

		}



		memset(temp,0,sizeof(temp));

		f_tooling.getline(temp,1000,'\n');

		trim(temp);	

	}*/

	// --- end yufen ---





	// --- added by yufen ---

	

	f_tooling.getline(temp,1000,'\n');

	f_tooling.getline(temp,1000,'\n');

	trim(temp);

    //Added by Zhufeng

	row_no=2;

	Tooling_vector.resize((int)TI.size());

	Num_Tooling=(int)TI.size();

	//end by Zhufeng


	//end by Zhufeng

	// comment by Zhufeng

	/*memset(pdes,0,sizeof(pdes));

	l=0;

	k=0;

	k=strtok2(pdes,temp,l,','); //TF

	trim(pdes);

   */

	//end by Zhufeng

	while(!f_tooling.eof() && strlen(temp)>0)

	{

		//Added by Zhufeng

		//memset(pdes,0,sizeof(pdes));

		memset(pdes,0,sizeof(pdes));

		l=0;

	    k=0;

	    k=strtok2(pdes,temp,l,','); //TF

	    trim(pdes);

        //end Zhufeng

		struct Tooling one_tool;

		//comment by Zhufeng

		//if(strlen(pdes)>0)

		//end Zhufeng

        

		//Added by Zhufeng

		if(strlen(pdes)<=0)

		{

			f_log<<"The "<<row_no<<"th row in tooling.csv: Tool_Family is empty.The tooling instance is skipped."<<endl;

			memset(temp,0,sizeof(temp));

			f_tooling.getline(temp,1000,'\n');

			//memset(pdes,0,sizeof(pdes));

			row_no++;

		}else

		{

			//end Zhufeng

			int tf=0;

			for(int id=1;id<=(int)TF.size();id++){

				if(!strcmp(TF[id-1],pdes)){ tf=id;break;}

			}

			//comment by Zhufeng

			//if(tf>0)

			//end by Zhufeng



			//Added by Zhufeng

			if(tf==0)

			{

				f_log<<"The "<<row_no<<"th row in tooling.csv: Information incomplete.The tooling instance is skipped."<<endl;

				memset(temp,0,sizeof(temp));

				f_tooling.getline(temp,1000,'\n');

				//memset(pdes,0,sizeof(pdes));

				row_no++;



			}else

				//end by Zhufeng

			{

				one_tool.tf=tf;

				l=strtok2(pdes,temp,k,',');//get tooling instance

				trim(pdes);

				//Added by Zhufeng

				if(strlen(pdes)<=0) //ti

				{

					f_log<<"The "<<row_no<<"th row in tooling.csv: Tooling_Instance is empty.The tooling instance is skipped."<<endl;

					memset(temp,0,sizeof(temp));

					f_tooling.getline(temp,1000,'\n');

					//memset(pdes,0,sizeof(pdes));

					row_no++;



				}else

				{

					int ti=0;

					for(int id=1;id<=(int)TI.size();id++){

						if(!strcmp(TI[id-1],pdes)){ ti=id;break;}

					}

					//comment by Zhufeng

					//if(ti<1){ cout<<"Wrong tooling instance id"<<endl; abort(); }

					//end Zhufeng



					//added by Zhufeng

					if(ti==0)

					{

						f_log<<"The "<<row_no<<"th row in tooling.csv: Information incomplete. The tooling instance is skipped."<<endl;

						memset(temp,0,sizeof(temp));

						f_tooling.getline(temp,1000,'\n');

						//memset(pdes,0,sizeof(pdes));

						row_no++;

					}else

					{

						//end by Zhufeng

						one_tool.id=ti;

						k=strtok2(pdes,temp,l,',');//get tooling family name

						trim(pdes);

						strcpy(one_tool.name,TI[one_tool.id-1]);

						l=strtok2(pdes,temp,k,',');//certification

						trim(pdes);

						//added by Zhufeng

						if(strlen(pdes)<=0) //for certification

						{

							f_log<<"The "<<row_no<<"th row in tooling.csv: Certification is empty. The tooling instance is skipped."<<endl;

							memset(temp,0,sizeof(temp));

							f_tooling.getline(temp,1000,'\n');

							//memset(pdes,0,sizeof(pdes));

							row_no++;

						}else

						{

							int temperature=atoi(pdes);

							if(temperature>Num_Temperature){ Num_Temperature=temperature;}

							//comment by Zhufeng

							//if(temperature>0){

							//	one_tool.temperature_vector.push_back(temperature);

							//}

							//end by Zhufeng

							if(temperature<=0)

							{

								f_log<<"The "<<row_no<<"th row in tooling.csv: Certification is non-positive. The tooling instance is skipped."<<endl;

								memset(temp,0,sizeof(temp));

								f_tooling.getline(temp,1000,'\n');

								//memset(pdes,0,sizeof(pdes));

								row_no++;

							}else

							{

								one_tool.temperature_vector.push_back(temperature);



								//comment by Zhufeng

								//}

								//end by Zhufeng

								if(!f_tooling.eof())

								{

									char tf_name[50];

									char ti_name[50];

									char tf_name2[50];

									do{

										memset(temp,0,sizeof(temp));

										f_tooling.getline(temp,1000,'\n');

										row_no++;

										if(!f_tooling.eof())

										{

											//memset(pdes,0,sizeof(pdes));

											l=0;

											k=0;

											k=strtok2(pdes,temp,l,',');//tf

											trim(pdes);

											strcpy(tf_name,pdes);

											//added by Zhufeng

											l=strtok2(pdes,temp,k,',');//ti

											trim(pdes);

											strcpy(ti_name,pdes);

											k=strtok2(pdes,temp,l,',');//tf_name

											trim(pdes);

											strcpy(tf_name2,pdes);

											l=strtok2(pdes,temp,k,',');//certification

											trim(pdes);



											//end Zhufeng

											if(tf>0 && strlen(tf_name)<=0 && strlen(ti_name)<=0 && strlen(tf_name2)<=0)

											{

												if(strlen(pdes)<=0)

												{

													f_log<<"The "<<row_no<<"th row in tooling.csv: Certification is empty.It may be one of multiple certifications of one tooling. The row is skipped but not the tooling instance."<<endl;



												}else

												{

													//comment by Zhufeng

													//l=strtok2(pdes,temp,k,',');

													//k=strtok2(pdes,temp,l,',');

													//l=strtok2(pdes,temp,k,',');

													//trim(pdes);

													//end Zhufeng

													int temperature=atoi(pdes);

													if(temperature>Num_Temperature){ Num_Temperature=temperature;}

													if(temperature>0)

													{

														one_tool.temperature_vector.push_back(temperature);

													}else

													{

														f_log<<"The "<<row_no<<"th row in tooling.csv: Certification is non-positive.It may be one of multiple certifications of one tooling. The row is skipped but not the tooling instance."<<endl;

													}

												}

											}

										}//end of if(!f_tooling.eof())

									}//end of do

									while(!f_tooling.eof() && strlen(temp)>0 && strlen(tf_name)<=0 && strlen(ti_name)<=0 && strlen(tf_name2)<=0);

								}//end of if(!f_tooling.eof())



								if(tf>0)

								{

									//comment  by Zhufeng

									//Num_Tooling++;

									//comment end

									cout<<"	"<<Num_Tooling<<"	"<<TI[one_tool.id-1]<<endl;



                                    //comment  by Zhufeng

									//Tooling_vector.push_back(one_tool);

									//comment end



									Tooling_vector[one_tool.id-1] = one_tool;

								}

							}//end of if(temperature<=0) else

						}//end of if(strlen(pdes)<=0) else for certification

					}//end of if(ti==0)

				}//end of if(strlen(pdes)<=0) else for ti

			}//end of if(tf==0) else 

		}//end of if(strlen(pdes)<=0) else

	}//end of while(!f_tooling.eof() && strlen(temp)>0)

	// ---  end yufen ---



	f_tooling.close();

	//the following statement is to deal with the empty tooling requirement

	for(int i=1;i<=Num_Machine;i++){

		struct Tooling one_tool;

		int tf=-100;

                for(int id=1;id<=(int)TF.size();id++){

                        if(!strcmp(TF[id-1],"No_Tooling")){ tf=id;break;}

                }

		one_tool.tf=tf;			

		int ti=0;

               	stringstream ss;

		ss<<"No_Tooling-"<<i;

		char name[50];

		strcpy(name,(ss.str()).c_str());	 

		for(int id=1;id<=(int)TI.size();id++){

                        if(!strcmp(TI[id-1],name)){ ti=id;break;}

                }

		one_tool.id=ti;

		strcpy(one_tool.name,TI[one_tool.id-1]);

		for(int tau=1;tau<=Num_Temperature;tau++){ one_tool.temperature_vector.push_back(tau);}



	 	//Num_Tooling++;

        //Tooling_vector.push_back(one_tool);

		  //added by Zhufeng
		Tooling_vector[one_tool.id-1] = one_tool;
		//end Zhufeng

	}

	/////	

	show_Tooling_vector();



	//----------------------------------------------------------------------------------get Tooling_Setup_Time[]



	//Added by Zhufeng

    vector<bool> whether_exist_vector; //record whether a TF exist in toolingfamily_setuptime.csv
	whether_exist_vector.resize((int)TF.size());

    for(int i=1;i<=(int)whether_exist_vector.size();i++)
	{
		whether_exist_vector[i-1]=false;
	}
	

	//end Zhufeng

	ifstream f_setup("Data/toolingfamily_setuptime.csv");

	cout<<"get Tooling_Setup_Time[]"<<endl;

	f_setup.getline(temp,1000,'\n');

	f_setup.getline(temp,1000,'\n');

	row_no=2;

	trim(temp);	

	while(!f_setup.eof() && strlen(temp)>0){

		int l=0,k=0;

		k=strtok2(pdes,temp,l,',');//get the tf name

		trim(pdes);

		//added by Zhufeng

		if(strlen(pdes)<=0)

		{

			f_log<<"The "<<row_no<<"th row in toolingfamily_setuptime.csv: Tooling_Family is empty. The row is skipped."<<endl;

		}else

		{ //end Zhufeng

			int tf_id=-100;

			for(int j=1;j<=(int)TF.size();j++){//get the tf id in TF[]

				if(!strcmp(TF[j-1],pdes)){

					tf_id=j;break;

				}

			}

			if(tf_id>0){//if the tf exists in the tooling.csv file as well

				whether_exist_vector[tf_id-1]=true;

				l=k;

				k=strtok2(pdes,temp,l,',');//get the setup time

				trim(pdes);

				Tooling_Setup_Time[tf_id]=(double)atof(pdes);

				//added by Zhufeng

				double setup_time=(double)atof(pdes);

				if(setup_time<=0)

				{

					f_log<<"The "<<row_no<<"th row in toolingfamily_setuptime.csv: Setup_time is non-positive.It is still used."<<endl;

				}



			}

			//It is possible that tf_id<0, that is, the tf cannot be found in TF. In that case, just skip the current record.

			//Added by Zhufeng

			else

			{

				f_log<<"The "<<row_no<<"th row in toolingfamily_setuptime.csv: No corresponding tool family. The row is skipped."<<endl;



			}

			//added by Zhufeng

		}//end of if(strlen(pdes)<=0) else

		//end Zhufeng

		f_setup.getline(temp,1000,'\n');

		trim(temp);	

		row_no++;

	}

	f_setup.close();

	f_setup.clear();

	//the following statement is to deal with empty tooling requirement

	if(1){

		int tf_id=-100;

                for(int j=1;j<=(int)TF.size();j++){//get the tf id in TF[]

                        if(!strcmp(TF[j-1],"No_Tooling")){

                                tf_id=j;break;

                        }

                }

		Tooling_Setup_Time[tf_id]=0.0; //there is no setup time for empty tooling

		//added by Zhufeng
		whether_exist_vector[tf_id-1]=true;
		//end by Zhufeng

	}

	///	

	//added by Zhufeng
	for(int i=1;i<=(int)whether_exist_vector.size();i++)
	{
		if(whether_exist_vector[i-1]==false)
		{
			f_log<<"The tooling family "<<TF[i-1]<<" exsits in tooling.csv but not in toolingfamily_setuptime.csv. The setuptime for this tooling family is taken as "
				<<Tooling_Setup_Time[i]<<endl;
		}
	}
	//end by Zhufeng



	ofstream f_debugsetup("Debug/debug_tf_setuptime.txt");

	for(int j=1;j<=(int)TF.size();j++){

		f_debugsetup<<TF[j-1]<<","<<Tooling_Setup_Time[j]<<endl;

	}

	f_debugsetup.close();

	f_debugsetup.clear();



    //added by Zhufeng

	//------------------data consistency checking for route.csv

	ofstream f_debugroute("Debug/route.csv");

	ifstream f_dataroute("Data/route.csv");



	//Updated for logpoint by Zhufeng, Aug 30, 2011

	//The following is not used anymore

	//vector<string> device_vector;//used to check whether wip has corresponding route.

	//end by Zhufeng, Aug 30, 2011



	//comment by Zhufeng, Sep 05, 2011. The  route_check_vector has been replaced by route_vector. Special for Yufen's codes

	//vector<route_check> route_check_vector;

	//end by Zhufeng



	char one_device[50];

	//struct route one_route;      //used to record a route

	vector<route> route_vector;  //record all the route kept in Debug/route.csv. Used to do data consisitency for initialsetup.csv and initial lots in wip.csv

	                             //This is special for Yufen's code. Do data consistency also for all lots in wip.csv. Comment of Zhufeng, Sep 05, 2011. 

	f_dataroute.getline(temp,1000,'\n');

	trim(temp);

	f_debugroute<<temp<<endl;

	f_dataroute.getline(temp,1000,'\n');

	trim(temp);

	row_no=2;

	while(!f_dataroute.eof() && strlen(temp) > 0)

	{

		l=0;k=0;

		k=strtok2(pdes,temp,l,',');//Route_name

		l=k;

		trim(pdes);



		if(strlen(pdes)<=0)

		{

			f_log<<"The "<<row_no<<"th row in route.csv: RouteName is empty. The row is still used."<<endl;

		}

		k=strtok2(pdes,temp,l,',');//logpoint

		l=k;

		trim(pdes);

		//cout<<pdes;

		int logpoint=atoi(pdes);

		//cout<<"log_point="<<log_point<<endl;

		if(strlen(pdes)<=0)

		{

			f_log<<"The "<<row_no<<"th row in route.csv: Logpoint is empty. The row is skipped."<<endl;

			f_dataroute.getline(temp,1000,'\n');

			trim(temp);

			row_no++;

		}else

		{

			k=strtok2(pdes,temp,l,',');//operation

			l=k;

			trim(pdes);



			int operation=atoi(pdes);



			if(strlen(pdes)<=0)

			{

				f_log<<"The "<<row_no<<"th row in route.csv: Operation is empty. The row is skipped."<<endl;

				f_dataroute.getline(temp,1000,'\n');

				trim(temp);

				row_no++;

			}else

			{



				l=0;k=0;

				for(int i=1;i<=6;i++)        //Device

				{

					k=strtok2(pdes,temp,l,',');

					l=k;

				}

				trim(pdes);

				char device_name[50];

				strcpy(device_name,pdes);

				if(strlen(pdes)<=0)

				{

					f_log<<"The "<<row_no<<"th row in route.csv: Device is empty. The row is skipped."<<endl;

					f_dataroute.getline(temp,1000,'\n');

					trim(temp);

					row_no++;



				}else

				{

					memset(one_device,0,sizeof(one_device));

					strcpy(one_device,pdes);//get device name;



					l=0;k=0;

					for(int i=1;i<=11;i++)        //PPH;

					{

						k=strtok2(pdes,temp,l,',');

						l=k;

					}

					trim(pdes);

					double pph=atof(pdes);

					if(pph <= 0)

					{

						f_log<<"The "<<row_no<<"th row in route.csv: PPH is non-positive. The row is skipped."<<endl;

						f_dataroute.getline(temp,1000,'\n');

						trim(temp);

						row_no++;



					}else

					{

						l=0;k=0;

						for(int i=1;i<=12;i++)        //Machine_Family; NOT allow for empty machine_family now

						{

							k=strtok2(pdes,temp,l,',');

							l=k;

						}

						trim(pdes);

						char machine_family[50];

						if(strlen(pdes)<=0)

						{

							f_log<<"The "<<row_no<<"th row in route.csv: Machine_Family is empty. The row is skipped."<<endl;

							//strcpy(machine_family,MF[0]);

							f_dataroute.getline(temp,1000,'\n');

							trim(temp);

							row_no++;

						}else

						{

							//char machine_family[50];

							//else

							//{

							strcpy(machine_family,pdes);

							//}

							int mf_id=-1;

							for(int i=1;i<=(int)MF.size();i++)

							{

								if(strcmp(machine_family,MF[i-1])==0)

								{

									mf_id=i;

								}

							}

							if(mf_id<=0)

							{

								f_log<<"The "<<row_no<<"th row in route.csv: Machine_Family is not found in machines.csv. The row is skipped."<<endl;

								f_dataroute.getline(temp,1000,'\n');

								trim(temp);

								row_no++;



							}else

							{

								l=0;k=0;

								for(int i=1;i<=13;i++)        //Tooling_Family;Note:the Tooling_Family can be empty.

								{

									k=strtok2(pdes,temp,l,',');

									l=k;

								}

								trim(pdes);



								char tool_family[50];

								if(strlen(pdes)<=0)

								{

									f_log<<"The "<<row_no<<"th row in route.csv: Tooling_Family is empty. The row is still used."<<endl;

									//cout<<"OK no_tooling "<<row_no<<endl;

									strcpy(tool_family,"No_Tooling");

								}else

								{

									strcpy(tool_family,pdes);

								}

								int tf_id=-1;

								for(int i=1;i<=(int)TF.size();i++)

								{

									if(strcmp(tool_family,TF[i-1])==0)

									{

										tf_id=i;

									}

								}

								if(tf_id<=0)

								{

									f_log<<"The "<<row_no<<"th row in route.csv: Tooling_Family is not found in tooling.csv. The row is skipped."<<endl;

									f_dataroute.getline(temp,1000,'\n');

									trim(temp);

									row_no++;



								}else

								{

									l=0;k=0;

									for(int i=1;i<=15;i++)        //Certification

									{

										k=strtok2(pdes,temp,l,',');

										l=k;

									}

									trim(pdes);

									int certification=atoi(pdes);

									if(certification<=0)

									{

										f_log<<"The "<<row_no<<"th row in route.csv: Certification is non-positive. The row is skipped."<<endl;

										f_dataroute.getline(temp,1000,'\n');

										trim(temp);

										row_no++;



									}else

									{

										//Comment by Zhufeng for logpoint, Aug 30, 2011

										/*vector<string>::iterator p_find=find(device_vector.begin(),device_vector.end(),one_device);

										if(p_find==device_vector.end())

										{

										device_vector.push_back(one_device);

										}*/

										//end by Zhufeng





                                        //Comment by Zhufeng, Sep 05, 2011. route_check_vector has been replaced by route_vector. Special for Yufen's code

										/*struct route_check one_route_check;

										one_route_check.logpoint=logpoint;

										one_route_check.operation=operation;

										strcpy(one_route_check.device,one_device);



										route_check_vector.push_back(one_route_check);*/

										//end by Zhufeng, Sep 05, 2011.





										//Zhufeng:Note this part is special for Yufen's code,Sep 04, 2011.

										struct route one_route;

										one_route.row_no=row_no;

										one_route.logpoint=logpoint;



										//added by Zhufeng for logpoint, Sep 04, 2011

										one_route.operation=operation;

										//end by Zhufeng for logpoint, Sep 04, 2011



										one_route.mf_id=mf_id;

										one_route.tf_id=tf_id;

										one_route.certification=certification;

										strcpy(one_route.device_name,device_name);

										route_vector.push_back(one_route);

										//end by Zhufeng,Sep 04, 2011



										f_debugroute<<temp<<endl;

										f_dataroute.getline(temp,1000,'\n');

										trim(temp);

										row_no++;



									}



								}



							}



						}



					}



				}

			}



		}



	}

	f_debugroute.close();

	f_debugroute.clear();

	f_dataroute.close();

    f_dataroute.clear();





    //comment by Zhufeng for logpoint, Aug 31, 2011

	////show device_vector

	//ofstream f_debug_route("Debug/route.txt");

	//f_debug_route<<"route_vector from route.csv:"<<endl;

	//for(int i=1;i<=(int)route_vector.size();i++)

	//{

	//	f_debug_route<<"row_no="<<route_vector[i-1].row_no

	//                 <<",log_point"<<route_vector[i-1].log_point

	//                 <<",mf="       <<MF[route_vector[i-1].mf_id-1]

	//				 <<",tf="	<<TF[route_vector[i-1].tf_id-1]

    //                    <<",certification="<<route_vector[i-1].certification

	//				<<",device="<<route_vector[i-1].device_name<<endl;

	//}

	//f_debug_route.close();

	//f_debug_route.clear();

	////end Zhufeng



	//end by Zhufeng



	//added by Zhufeng

	//Do data checking on initialsetup.csv.

	

	vector<bool> mi_exist_vector;

	mi_exist_vector.resize((int)MI.size()); //mi_exist_vector[mi_id-1] denote whether MI[mi_id] exist in initialsetup.csv

	for(int i=1;i<=(int)mi_exist_vector.size();i++)

	{

		mi_exist_vector[i-1]=false;

	}



	ofstream f_debuginset("Debug/initialsetup.csv");

	ifstream f_datainset("Data/initialsetup.csv");

	f_datainset.getline(temp,1000,'\n');

	f_debuginset<<temp<<endl;

	f_datainset.getline(temp,1000,'\n');

	trim(temp);

	row_no=2;

	vector<setup> initialsetup_vector;

	

	while(!f_datainset.eof() && strlen(temp)>0)

	{

		l=0;k=0;

		k=strtok2(pdes,temp,l,',');//Machine_Family

		trim(pdes);

	

		if(strlen(pdes)<=0)

		{

			f_datainset.getline(temp,1000,'\n');

			f_log<<"The "<<row_no<<"th row in initialsetup.csv: Machine_Family is empty. The row is skipped."<<endl;

			row_no++;

			continue;

		}

		

		char mf_name[50];

		int mf_id=-1;

		strcpy(mf_name, pdes);

		for(int i=1;i<=(int)MF.size();i++) //mf_id

		{

			if(strcmp(mf_name,MF[i-1])==0)

			{

				mf_id=i;

			}

		}

		if(mf_id==-1)

		{

			f_datainset.getline(temp,1000,'\n');

			f_log<<"The "<<row_no<<"th row in initialsetup.csv: Machine_Family is not found in machines.csv. The row is skipped."<<endl;

			row_no++;

			continue;

		}

        

		l=0;k=0;

		for(int i=1;i<=2;i++)             //Machine_Instance

		{

			k=strtok2(pdes,temp,l,',');

			l=k;

		}

		trim(pdes);

        if(strlen(pdes)<=0)

		{

			f_datainset.getline(temp,1000,'\n');

			f_log<<"The "<<row_no<<"th row in initialsetup.csv: Machine_Instance is empty. The row is skipped."<<endl;

			row_no++;

			continue;

		}

        

		char mi_name[50];

		int mi_id=-1;

		strcpy(mi_name, pdes);

		for(int i=1;i<=(int)MI.size();i++) //mi_id

		{

			if(strcmp(mi_name,MI[i-1])==0)

			{

				mi_id=i;

			}

		}

		



		if(mi_id==-1)

		{

			f_datainset.getline(temp,1000,'\n');

			f_log<<"The "<<row_no<<"th row in initialsetup.csv: Machine_Instance is not found in machines.csv. The row is skipped."<<endl;

			row_no++;

			continue;

		}

       



		l=0;k=0;

		for(int i=1;i<=4;i++)             //Initial_Tool_Family

		{

			k=strtok2(pdes,temp,l,',');   

			l=k;

		}

		trim(pdes);

		if(strlen(pdes)<=0)        //if empty initial tf. It may be regular machine or error data of initial machine

		{

			l=0;k=0;

			for(int i=1;i<=5;i++)             //Initial_Certification

			{

				k=strtok2(pdes,temp,l,',');   

				l=k;

			}

			trim(pdes);

			int ini_cert=atoi(pdes);

			if(ini_cert >0)

			{

				f_log<<"The "<<row_no<<"th row in initialsetup.csv: Initial_Tool_Family is empty while the initial certification is positive . The row is skipped."<<endl;

			}else

			{

				//a regular machine

				

				mi_exist_vector[mi_id-1]=true;

				f_debuginset<<temp<<endl; 



			}

			f_datainset.getline(temp,1000,'\n');			

			row_no++;

			continue;

		}

        

		char tf_name[50];

		int tf_id=-1;

		strcpy(tf_name, pdes);

		for(int i=1;i<=(int)TF.size();i++) //TF_id

		{

			if(strcmp(tf_name,TF[i-1])==0)

			{

				tf_id=i;

			}

		}

		if(tf_id==-1)

		{

			f_datainset.getline(temp,1000,'\n');

			f_log<<"The "<<row_no<<"th row in initialsetup.csv: Initial_Tool_Family is not found in tooling.csv. The row is skipped."<<endl;

			row_no++;

			continue;

		}



		l=0;k=0;

		for(int i=1;i<=5;i++)             //Initial_Certification

		{

			k=strtok2(pdes,temp,l,',');   

			l=k;

		}

		trim(pdes);

		if(strlen(pdes)<=0)

		{

			f_datainset.getline(temp,1000,'\n');

			f_log<<"The "<<row_no<<"th row in initialsetup.csv: Initial_Certification is empty. The row is skipped."<<endl;

			row_no++;

			continue;

		}

        

		int ini_cert=atoi(pdes);

		if(ini_cert<=0)

		{

			f_datainset.getline(temp,1000,'\n');

			f_log<<"The "<<row_no<<"th row in initialsetup.csv: Initial_Certification is non-positive. The row is skipped."<<endl;

			row_no++;

			continue;

		}

        

		int route_id=-1;

		//cout<<"InitialSetup, MF="<<MF[mf_id-1]<<", TF="<<TF[tf_id-1]<<", cert="<<ini_cert<<endl;

		for(int i=1;i<=(int)route_vector.size();i++)

		{

			if(mf_id==route_vector[i-1].mf_id &&

				tf_id==route_vector[i-1].tf_id &&

				ini_cert==route_vector[i-1].certification)

			{

				route_id=i;

			}

		}

		if(route_id<0)

		{

			f_log<<"The "<<row_no<<"th row in initialsetup.csv: The combination of machine family, tooling family and certification is not found in route.csv. The row is skipped."<<endl;

			f_datainset.getline(temp,1000,'\n');

			row_no++;

			continue;

		}



		struct setup one_setup;

		one_setup.row_no=row_no;

		one_setup.mf_id=mf_id;

		one_setup.tf_id=tf_id;

		one_setup.certification=ini_cert;

		one_setup.mi_id=mi_id;



		initialsetup_vector.push_back(one_setup);



		 mi_exist_vector[mi_id-1]=true;

        f_debuginset<<temp<<endl; 

		

		f_datainset.getline(temp,1000,'\n');

		row_no++;



	}//end of while(!f_datainset.end() && strlen(temp)>0)



	for(int i=1;i<=(int)mi_exist_vector.size();i++)

	{

		if(mi_exist_vector[i-1]==false)

		{

			f_log<<"The machine instance "<<MI[i-1]<<" doesn't exist (or skipped) in initialsetup.csv.This machine is taken as no initial setup"<<endl;

		}

	}

    

	ofstream f_debug_i("Debug/initialsetup.txt");

    f_debug_i<<"correct initialsetups"<<endl;

	for(int i=1;i<=(int)initialsetup_vector.size();i++)

	{

		f_debug_i<<"row_no="<<initialsetup_vector[i-1].row_no

			<<",MI="<<MI[initialsetup_vector[i-1].mi_id-1]

			 <<",mf="<<MF[initialsetup_vector[i-1].mf_id-1]

             <<",tf="<<TF[initialsetup_vector[i-1].tf_id-1]

			 <<",certification="<<initialsetup_vector[i-1].certification<<endl;



	}

    f_debug_i.close();

	f_debug_i.clear();

	f_datainset.close();

	f_datainset.clear();

	f_debuginset.close();

	f_debuginset.clear();



    //end Zhufeng



	//----------------------------------------------------------------------------------get LOT

	//Added by Zhufeng.

	//Do data checking first. If any critical field is blank, skip the lot.

    ofstream f_debugwip("Debug/wip.csv");

	ifstream f_wip("Data/wip.csv");

	//f_wip.getline(temp,1000,'\n');

	f_wip.getline(temp,1000,'\n');

	trim(temp);

	f_debugwip<<temp<<endl;

    f_wip.getline(temp,1000,'\n');

	trim(temp);

	row_no=2;



	//first check whether the snapshoot time is in good format

	if(!f_wip.eof() && strlen(temp)>0)

	{

		

		l=0;k=0;

		for(int i=1;i<=14;i++)

		{

			k=strtok2(pdes,temp,l,',');

			l=k;

		}

		trim(pdes); //get the Genearl Snapshoot time

		int snapshoot_time_length=strlen(pdes);

		

		if(strlen(pdes)<=0)

		{

			f_log<<"The "<<row_no<<"th row in wip.csv: SNAPSHOOT_TIME is empty. This may couse some errors."<<endl;

		}else

		{

			k=0;l=0;

			char pdes2[50];

			memset(pdes2,0,sizeof(pdes2));

	        k=strtok2(pdes2,pdes,l,'/');//get MM

			l=k;

			if(strlen(pdes2)<=0)

			{

				f_log<<"The "<<row_no<<"th row in wip.csv: the month in SNAPSHOOT_TIME is empty. This may couse some errors."<<endl;

			}else

			{

				

				int mm;

				mm = atoi(pdes2);

				if(mm<=0 || mm >12 )

				{



					f_log<<"The "<<row_no<<"th row in wip.csv: the month in SNAPSHOOT_TIME is wrong. This may couse some errors."<<endl;

					

				}

				if(l>=snapshoot_time_length)

				{

					f_log<<"The "<<row_no<<"th row in wip.csv: the SNAPSHOOT_TIME is in wrong format or only have information about month. This may couse some errors."<<endl;

				}else

				{

					l=0;k=0;

					for(int i=1;i<=2;i++)

					{

						k=strtok2(pdes2,pdes,l,'/');//get day

						l=k;

					}

					

					if(strlen(pdes2)<=0)

					{

						f_log<<"The "<<row_no<<"th row in wip.csv: the day in SNAPSHOOT_TIME is empty. This may couse some errors."<<endl;

					}else

					{

						

						int dd=atoi(pdes2);

						if(dd< 0 || dd >31)

						{



							f_log<<"The "<<row_no<<"th row in wip.csv: the day in SNAPSHOOT_TIME is wrong. This may couse some errors."<<endl;



						}

						if(l>=snapshoot_time_length)

						{

							f_log<<"The "<<row_no<<"th row in wip.csv: the SNAPSHOOT_TIME is in wrong format or only has onformation about month and day. This may couse some errors."<<endl;

						}else

						{

							l=0;k=0;

							for(int i=1;i<=2;i++)

							{

								k=strtok2(pdes2,pdes,l,'/');

								l=k;

							}

							k=strtok2(pdes2,pdes,l,' ','/');//get YY

							l=k;

							trim(pdes2);

							if(strlen(pdes2)<=0)

							{

								f_log<<"The "<<row_no<<"th row in wip.csv: the year in SNAPSHOOT_TIME is empty. This may couse some errors."<<endl;

							}else

							{

								

								if(l>=snapshoot_time_length)

								{

									f_log<<"The "<<row_no<<"th row in wip.csv: the SNAPSHOOT_TIME is in wrong format or lack information about hh:mm:ss. This may couse some errors."<<endl;

								}else

								{

									k=strtok2(pdes2,pdes,l,':');//get hh

									trim(pdes2);

									int hh_length=strlen(pdes2);

									int hh=atoi(pdes2);;

									l=k;

									k=strtok2(pdes2,pdes,l,':');//get mm

									trim(pdes2);

									//cout<<"pdes="<<pdes<<endl;

									int mm_length=strlen(pdes2);

									int mm=atoi(pdes2);

									l=k;

									if(l>=snapshoot_time_length)

									{

										f_log<<"The "<<row_no<<"th row in wip.csv: the hh:mm:ss in SNAPSHOOT_TIME is wrong or incomplete. This may couse some errors.Stop running the code."<<endl;



									}else

									{



										k=strtok2(pdes2,pdes,l,' ');//get ss

										trim(pdes2);

										int ss_length=strlen(pdes2);

										int ss=atoi(pdes2);

										if(hh_length <= 0 || hh <0 || hh>24 || mm_length <= 0 || mm <0 || mm>=60 || ss_length <=0 || ss<0 || ss>=60)

										{

											f_log<<"The "<<row_no<<"th row in wip.csv: the hh:mm:ss in SNAPSHOOT_TIME is wrong. This may couse some errors."<<endl;

								

										}

									}

								}//end of if(l>=snapshoot_time_length)



							}//end of if(strlen(pdes2)<=0)



						}//end of if(dd< 0 || dd >31)

					}//end of if(strlen(pdes2)<=0)



				}//end of if(mm<=0 || mm >12)

			}//end of if(strlen(pdes2)<=0)

		}//end of if(strlen(pdes)<=0)

	}//end of if(!f_wip.eof() && strlen(temp)>0)









	while(!f_wip.eof() && strlen(temp)>0)

	{

		l=0;k=0;

		k=strtok2(pdes,temp,l,',');//LOT 

		trim(pdes);

		l=k;

		if(strlen(pdes)<=0)

		{

			f_wip.getline(temp,1000,'\n');

			f_log<<"The "<<row_no<<"th row in wip.csv: LOT is empty. The row is skipped."<<endl;

			row_no++;

		}else

		{

			k=strtok2(pdes,temp,l,',');//DEVICE

			trim(pdes);



			l=k;

			if(strlen(pdes)<=0)

			{

				f_wip.getline(temp,1000,'\n');

				f_log<<"The "<<row_no<<"th row in wip.csv: DEVICE is empty. The row is skipped."<<endl;

				row_no++;

			}else

			{

				strcpy(one_device,pdes);

				//comment by Zhufeng for logpoint, Aug 30, 2011

				////vector<string>::iterator p_find=find(device_vector.begin(),device_vector.end(),one_device);

				//if(p_find==device_vector.end()) //Does the device have a route? 

				//{

				//	f_wip.getline(temp,1000,'\n');

				//    f_log<<"The "<<row_no<<"th row in wip.csv: There is no route for DEVICE "<<one_device<<" from route.csv. The row is skipped."<<endl;

				//    row_no++;



				//}else

				//{

				//end by Zhufeng for logpoint



				k=strtok2(pdes,temp,l,',');//package

				l=k;

				trim(pdes);

				if(strlen(pdes)<=0)

				{

					f_wip.getline(temp,1000,'\n');

					f_log<<"The "<<row_no<<"th row in wip.csv: Package is empty. The row is skipped."<<endl;

					row_no++;

				}else

				{

					k=strtok2(pdes,temp,l,',');//Pin

					l=k;

					trim(pdes);

					if(strlen(pdes)<=0)

					{

						f_wip.getline(temp,1000,'\n');

						f_log<<"The "<<row_no<<"th row in wip.csv: Pin is empty. The row is skipped."<<endl;

						row_no++;

					}else

					{

						k=strtok2(pdes,temp,l,',');//CUR_QTY

						l=k;

						trim(pdes);

						if(strlen(pdes)<=0)

						{

							f_wip.getline(temp,1000,'\n');

							f_log<<"The "<<row_no<<"th row in wip.csv: CUR_QTY is empty. The row is skipped."<<endl;

							row_no++;

						}else

						{

							k=strtok2(pdes,temp,l,',');//weight

							l=k;

							trim(pdes);

							if(strlen(pdes)<=0)

							{

								f_wip.getline(temp,1000,'\n');

								f_log<<"The "<<row_no<<"th row in wip.csv: Weight is empty. The row is skipped."<<endl;

								row_no++;

							}else

							{

								k=strtok2(pdes,temp,l,',');//Lot_Name

								l=k;

								k=strtok2(pdes,temp,l,',');//LPT

								l=k;

								trim(pdes);



								//Added for logpoint by Zhufeng, Sep 04, 2011

								int logpoint=atoi(pdes);



								if(strlen(pdes)<=0)

								{

									f_wip.getline(temp,1000,'\n');

									f_log<<"The "<<row_no<<"th row in wip.csv: LPT is empty. The row is skipped."<<endl;

									row_no++;

								}else

								{

									k=strtok2(pdes,temp,l,',');//OPN

									l=k;

									trim(pdes);

									int operation=atoi(pdes);

									if(strlen(pdes)<=0)

									{

										f_wip.getline(temp,1000,'\n');

										f_log<<"The "<<row_no<<"th row in wip.csv: OPN is empty. The row is skipped."<<endl;

										row_no++;

									}else

									{

										//Comment by Zhufeng for replacing route_check_vector by route_vector. Sep 05, 2011



										//vector<route_check>::iterator p_route=route_check_vector.begin();

										//while( p_route != route_check_vector.end())

										//{

										//	if((* p_route).logpoint!=logpoint)

										//	{

										//		p_route++;

										//	}else

										//	{

										//		if((* p_route).operation != operation)

										//		{

										//			p_route++;

										//		}else

										//		{

										//			if(strcmp(one_device,(* p_route).device))

										//			{

										//				p_route++;

										//			}else

										//			{

										//				break;

										//			}

										//		}

										//	}//end of if((* p_route).logpoint!=logpoint)



										//}//end of while( p_route != route_check_vector.end())



										//if(p_route == route_check_vector.end() ) //does the wip have a route?

										//{

										//	f_wip.getline(temp,1000,'\n');

										//	f_log<<"The "<<row_no<<"th row in wip.csv: The lot with Device, LPT,and OPN doesn't have a route in route.csv. The row is skipped."<<endl;

										//	row_no++;



										//end by Zhufeng on Sep 05, 2011



										//Added by Zhufeng for replacing route_check_vector by route_vector. Sep 05, 2011

										vector<route>::iterator p_route=route_vector.begin();

										while( p_route != route_vector.end())

										{

											if((* p_route).logpoint!=logpoint)

											{

												p_route++;

											}else

											{

												if((* p_route).operation != operation)

												{

													p_route++;

												}else

												{

													if(strcmp(one_device,(* p_route).device_name))

													{

														p_route++;

													}else

													{

														break;

													}

												}

											}//end of if((* p_route).logpoint!=logpoint)



										}//end of while( p_route != route_vector.end())



										if(p_route == route_vector.end() ) //does the wip have a route?

										{

											f_wip.getline(temp,1000,'\n');

											f_log<<"The "<<row_no<<"th row in wip.csv: The lot with Device, LPT,and OPN doesn't have a route in route.csv. The row is skipped."<<endl;

											row_no++;



											//end  by Zhufeng Sep 05, 2011



										}else

										{



											l=0;k=0;

											for(int i=1;i<=14;i++)

											{

												k=strtok2(pdes,temp,l,',');//SNAPSHOOT_TIME

												l=k;

											}

											trim(pdes);

											if(strlen(pdes)<=0)

											{

												f_wip.getline(temp,1000,'\n');

												f_log<<"The "<<row_no<<"th row in wip.csv: SNAPSHOOT_TIME is empty. The row is skipped."<<endl;

												row_no++;

											}else

											{

												char start_time[50];

												char mi_initial[50];

												l=0;k=0;

												for(int i=1;i<=13;i++)

												{

													k=strtok2(pdes,temp,l,',');//START_TIME

													l=k;	

												}

												trim(pdes);

												strcpy(start_time,pdes);

												l=0;k=0;

												for(int i=1;i<=15;i++)

												{

													k=strtok2(pdes,temp,l,',');//Machine_Instance

													l=k;	

												}

												trim(pdes);

												strcpy(mi_initial,pdes);

												if( (strlen(start_time)<=0 && strlen(mi_initial) > 0)

													|| (strlen(start_time)> 0 && strlen(mi_initial) <= 0))

												{

													f_wip.getline(temp,1000,'\n');

													f_log<<"The "<<row_no<<"th row in wip.csv: This is an initial lot but the star_time or machine_instance is empty. The row is skipped."<<endl;

													row_no++;

												}else

												{

													if(strlen(mi_initial)>0)

													{

														//initial lot



														int machine_id=-1;

														for(int j=1;j<=(int)MI.size();j++){

															if(!strcmp(MI[j-1],mi_initial)){

																machine_id=j; break;

															}

														}



														if(machine_id<0){//if the machine_id is not included in <MI>,then simply discard the info of this line. Continue to the next line



															f_log<<"The "<<row_no<<"th row in wip.csv: This is an initial lot but the machine_instance is not found in machines.csv. The row is skipped."<<endl;

															row_no++;

															f_wip.getline(temp,1000,'\n');



														}else

														{

															if(mi_exist_vector[machine_id-1]==false)

															{

																f_log<<"The "<<row_no<<"th row in wip.csv: This is an initial lot but the machine_instance "<<MI[machine_id-1]<<" is not found (or skipped) in initialsetup.csv. The lot is skipped."<<endl;

																row_no++;

																f_wip.getline(temp,1000,'\n');



															}else

															{

																//check "initialsetup.csv" whether the machine has initial setup?

																int initial_id=-1;

																for(int i=1;i<=initialsetup_vector.size();i++)

																{

																	if(initialsetup_vector[i-1].mi_id==machine_id)

																	{

																		initial_id=i;

																	}



																}

																if(initial_id==-1)

																{

																	f_log<<"The "<<row_no<<"th row in wip.csv: This is an initial lot but the machine_instance"<<MI[machine_id-1]<<" doesn't have setups in initialsetup.csv. The lot is skipped."<<endl;

																	row_no++;

																	f_wip.getline(temp,1000,'\n');



																}else

																{

																	//check whether the initial setup and the device is a route in route.csv?

																	

																	int mf_id=initialsetup_vector[initial_id-1].mf_id;

																	int tf_id=initialsetup_vector[initial_id-1].tf_id;

																	int certification=initialsetup_vector[initial_id-1].certification;

																	int route_id=-1;

																	for(int i=1;i<=(int)route_vector.size();i++)

																	{

																		if( mf_id==route_vector[i-1].mf_id &&

																			tf_id==route_vector[i-1].tf_id &&

																			certification==route_vector[i-1].certification &&

																			logpoint==route_vector[i-1].logpoint &&

																			strcmp(one_device,route_vector[i-1].device_name)==0

																			//Added by Zhufeng for adding logpoint and operation into consideration. Zhufeng, Sep 05, 2011.

																			&& operation==route_vector[i-1].operation

																			//end Zhufeng, Sep 05, 2011

																			)

																		{

																			route_id=i;

																		}

																	}

																	if(route_id==-1)

																	{

																		f_log<<"The "<<row_no<<"th row in wip.csv: This is an initial lot. But the initial machine_instance"<<MI[machine_id-1]<<"with tooling setups ("<<TF[tf_id-1]

																		<<" and certification "<<certification<<") in initialsetup.csv cannot process device"<<one_device <<" with logpoint equal to"<<logpoint

																			<<" because there is no corresponding route in route.csv.The lot is skipped."<<endl;

																		f_log<<"The machine instance"<<MI[machine_id-1]<<"is taken as a machine with initial setup but without initial lot."<<endl;

																		row_no++;

																		f_wip.getline(temp,1000,'\n');

																	}else

																	{

																		f_debugwip<<temp<<endl;

																		f_wip.getline(temp,1000,'\n');

																		row_no++;

																	}

																}//end of if(initial_id==-1)

															}







														}

													}else

													{

														//regular lot

														f_debugwip<<temp<<endl;

														f_wip.getline(temp,1000,'\n');

														row_no++;

													}



												}//end of if( (strlen(start_time)<=0 && strlen(mi_initial) > 0)



											}//end of SNAPSHOOT_TIME

										}//end of does the wip have a route?



									}//end of OPN



								}//end of LPT





							}//end of weight



						}//end of CUR_QTY



					}//end of Pin



				}//end of Package



			}//end of device in wip?



		}//end of for DEVICE



		//end of if(strlen(pdes)<=0) else for LOT



	}//end of while (!f_wip.eof() && strlen(temp)>0)

    f_debugwip.close();

	f_debugwip.clear();

	f_wip.close();

	//end by Zhufeng

	//read wip.csv

	for(int l=0;l<Max_Num_Lot;l++){ w_l[l]=0.0;}

	//ifstream f_wip("Data/wip.csv");

	//added by Zhufeng

	f_wip.open("Debug/wip.csv");  //from now on, all the Data/wip.csv has been replaced by Debug/wip.csv .

	//end Zhufeng

	//comment by Zhufeng

	//f_wip.open("Data/wip.csv");

	//end Zhufeng

	f_wip.getline(temp,1000,'\n');

	f_wip.getline(temp,1000,'\n');

	//add by Zhufeng

	//row_no=2;

	//end Zhufeng

	trim(temp);	

	cout<<"get LOT"<<endl;

	Num_Lot=0;

	Num_Device=0;

	while(!f_wip.eof() && strlen(temp)>0){

		int l=0,k=0;

		//------------------------------------------test if the lot is an initial lot

		for(int j=1;j<=6;j++){

			k=strtok2(pdes,temp,l,',');

			l=strtok2(pdes,temp,k,',');

		}

		k=strtok2(pdes,temp,l,','); //get "Start_time"

		trim(pdes);

		//---------------------------------------------------------------------------

		if(strlen(pdes)<=0){//if this field is blank, then it is a regular lot

			l=0;

			k=strtok2(pdes,temp,l,',');//lot

			//added by Zhufeng

			//if(strlen(pdes)<=0)

			//{

			//	f_log<<"The "<<row_no<<"th row in wip.csv: Lot is empty. The row is skipped."<<endl;



			//}else

			//{

			trim(pdes);

			bool lot_exist=false;

			for(int id=1;id<=(int)LOT.size();id++){

				if(!strcmp(LOT[id-1],pdes)){ lot_exist=true;break;}

			}

			if(!lot_exist){//lot does not exist

				char * name=new char[50];

				strcpy(name,pdes);

				LOT.push_back(name);

			}

			//}

		}

		f_wip.getline(temp,1000,'\n');

		trim(temp);	

	}

	//for(int id=1;id<=(int)LOT.size();id++){

	//	cout<<"id="<<id<<",lot="<<LOT[id-1]<<endl;

	//}

	Num_Lot=(int)LOT.size();

	f_wip.close();

	f_wip.clear();

	f_wip.open("Debug/wip.csv");

	//---------------------------------------------------------------------------------get DEV

	//Zhufeng:Only get regular lot's dev but add initial lot's dev later in extend_initial_lot()



	//Zhufeng, Dec 23, 2011:add device later when reading keydevice_production.csv. Note: the device in keydevice_production may not have a route in the route.csv





	f_wip.getline(temp,1000,'\n');

	f_wip.getline(temp,1000,'\n');

	trim(temp);

	cout<<"get DEV"<<endl;

	while(!f_wip.eof() && strlen(temp)>0){

		int l=0,k=0;

		//------------------------------------------test if the lot is an initial lot

		for(int j=1;j<=6;j++){

			k=strtok2(pdes,temp,l,',');

			l=strtok2(pdes,temp,k,',');

		}

		k=strtok2(pdes,temp,l,','); //get "Start_time"

		trim(pdes);

		//---------------------------------------------------------------------------

		if(strlen(pdes)<=0){//if this field is blank, then it is a regular lot

			l=0;

			k=strtok2(pdes,temp,l,',');

			l=k;

			k=strtok2(pdes,temp,l,',');//get the device info

			trim(pdes);

			bool dev_exist=false;

			for(int id=1;id<=(int)DEV.size();id++){

				if(!strcmp(DEV[id-1],pdes)){ dev_exist=true;break;}

			}

			if(!dev_exist){//dev does not exist

				char * name=new char[50];

				strcpy(name,pdes);

				DEV.push_back(name);

			}

		}

		f_wip.getline(temp,1000,'\n');

		trim(temp);	

	}

	

	Num_Device=(int)DEV.size();

	f_wip.close();

	f_wip.clear();

	//----------------------------------------------------------------------------------get PKG

	int dummy_pkg_pos=-1;

	f_wip.open("Debug/wip.csv");

	f_wip.getline(temp,1000,'\n');

	f_wip.getline(temp,1000,'\n');

	trim(temp);	

	cout<<"get PKG"<<endl;

	PKG.clear();

	while(!f_wip.eof() && strlen(temp)>0){

		int l=0,k=0;

		//------------------------------------------test if the lot is an initial lot

		for(int j=1;j<=6;j++){

			k=strtok2(pdes,temp,l,',');

			l=strtok2(pdes,temp,k,',');

		}

		k=strtok2(pdes,temp,l,','); //get "Start_time"

		trim(pdes);

		//---------------------------------------------------------------------------

		if(strlen(pdes)<=0){//if this field is blank, then it is a regular lot

			l=0;

			k=strtok2(pdes,temp,l,',');

			l=k;

			k=strtok2(pdes,temp,l,',');

			l=k;

			k=strtok2(pdes,temp,l,',');//get package info

			trim(pdes);

			bool pkg_exist=false;

			for(int id=1;id<=(int)PKG.size();id++){

				if(!strcmp(PKG[id-1],pdes)){ pkg_exist=true;break;}

			}

			if(!pkg_exist){//pkg does not exist

				char * name=new char[50];

				strcpy(name,pdes);

				PKG.push_back(name);

			}

		}

		f_wip.getline(temp,1000,'\n');

		trim(temp);	

	}

	f_wip.close();

	f_wip.clear();	

	//it is likely that some other devices will be introduced later (mainly come from other files)

	//however, there is no pkg and pin information for these devices

	//thus, append dummy pkg info for these devices here

    //comment by Zhufeng

	//char dummyname[50]="dummy_pkg";

	//end by Zhufeng

	//added by Zhufeng. Note:the pointer to "dummy_pkg" must be valid throught the whole program. So use new[] but not use delete[]

	//

	char * dummyname=new char[50];

	strcpy(dummyname,"dummy_pkg");

	//end by Zhufeng

	PKG.push_back(dummyname);

	//PKG will be extended later, thus remember of position of the dummy_pkg

	dummy_pkg_pos=(int)PKG.size();

	//----------------------------------------------------------------------------------get device_PKG,device_PIN

	for(int id=0;id<=Max_Num_Device-1;id++){

		device_PKG[id]=-100;

		device_PIN[id]=-100;

	}

	cout<<"get device_PKG[],device_PIN"<<endl;

	f_wip.open("Debug/wip.csv");

	f_wip.getline(temp,1000,'\n');

	f_wip.getline(temp,1000,'\n');

	trim(temp);	

	while(!f_wip.eof() && strlen(temp)>0){

		int l=0,k=0;

		//------------------------------------------test if the lot is an initial lot

		for(int j=1;j<=6;j++){

			k=strtok2(pdes,temp,l,',');

			l=strtok2(pdes,temp,k,',');

		}

		k=strtok2(pdes,temp,l,','); //get "Start_time"

		trim(pdes);

		//---------------------------------------------------------------------------

		if(strlen(pdes)<=0){//if this field is blank, then it is a regular lot

			l=0;

			k=strtok2(pdes,temp,l,',');

			l=k;

			k=strtok2(pdes,temp,l,',');//get device info

			trim(pdes);

			int dev_id=-1;

			for(int id=1;id<=(int)DEV.size();id++){

				if(!strcmp(DEV[id-1],pdes)){ dev_id=id;break;}

			}

			if(dev_id<1){

				cout<<"Wrong device id, abort"<<endl; abort();

			}

			l=k;

			k=strtok2(pdes,temp,l,',');//get package info

			trim(pdes);

			int pkg_id=-1;

			for(int id=1;id<=(int)PKG.size();id++){

				if(!strcmp(PKG[id-1],pdes)){ pkg_id=id; break;}

			}

			if(pkg_id<1){

				//cout<<"Wrong package id, abort"<<endl; abort();

				//no pkg info for such device (it is possible this field is empty)

				//assign the current device to package dummy_pkg

				pkg_id=dummy_pkg_pos;

			}

			device_PKG[dev_id]=pkg_id;

			l=k;

			k=strtok2(pdes,temp,l,',');//get PIN info

			trim(pdes);

			int pin=atoi(pdes);

			if(device_PIN[dev_id]>0 && device_PIN[dev_id]!=pin){

				cout<<"same device "<<DEV[dev_id-1]<<" has different number of pins. abort"<<endl; abort();

			}

			device_PIN[dev_id]=pin; //it is possible that pin=0 when the field is empty

		}

		f_wip.getline(temp,1000,'\n');

		trim(temp);	

	}

	//----------------------------------------------------------------------------------get n_l_chips and lot weight

	f_wip.close();

	f_wip.clear();	

	f_wip.open("Debug/wip.csv");

	f_wip.getline(temp,1000,'\n');

	f_wip.getline(temp,1000,'\n');

	trim(temp);	

	cout<<"get n_l_chips[] and w_l[]"<<endl;

	while(!f_wip.eof() && strlen(temp)>0){

		int l=0,k=0;





		//------------------------------------------test if the lot is an initial lot

		for(int j=1;j<=6;j++){

			k=strtok2(pdes,temp,l,',');

			l=strtok2(pdes,temp,k,',');

		}

		k=strtok2(pdes,temp,l,','); //get "Start_time"

		trim(pdes);

		//---------------------------------------------------------------------------

		if(strlen(pdes)<=0){//if this field is blank, then it is a regular lot

			l=0;

			k=strtok2(pdes,temp,l,',');//get Lot name

			trim(pdes);

			//-----------

			int lot=0;

			for(int id=1;id<=(int)LOT.size();id++){

				if(!strcmp(LOT[id-1],pdes)){ lot=id;break;}

			}

			if(lot<1){ cout<<"Wrong lot id"<<endl; abort();}

			l=strtok2(pdes,temp,k,',');//get device name

			trim(pdes);

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

			trim(pdes);

			n_l_chips[lot]=atoi(pdes);

			lots_device[lot]=dev;

			l=strtok2(pdes,temp,k,',');//get lot weight

			trim(pdes);

			w_l[lot]=(double)atof(pdes);

			k=strtok2(pdes,temp,l,',');//get lot name

			trim(pdes);

			struct ch_array one_ch_array;

			one_ch_array.id=lot;

			strcpy(one_ch_array.char_arry,pdes);

			lot_name.push_back(one_ch_array);



			//added by Zhufeng for logpoint, Aug 30, 2011
            l=strtok2(pdes,temp,k,',');//get log point
			trim(pdes);
			int logpoint=atoi(pdes);
			lot_logpoint[lot]=logpoint;

			k=strtok2(pdes,temp,l,',');//get operation
            trim(pdes);
			int operation=atoi(pdes);
			lot_operation[lot]=operation;
			//end by Zhufeng

		}

		f_wip.getline(temp,1000,'\n');

		trim(temp);	

	}

	f_wip.close();

	f_wip.clear();

	//------------------------------------------------------------------------------get General_Snapshoot_time

	f_wip.open("Debug/wip.csv");

	f_wip.getline(temp,1000,'\n');

	f_wip.getline(temp,1000,'\n');

	trim(temp);	

	if(1){

		int l=0,k=0;

		for(int j=1;j<=7;j++){

			k=strtok2(pdes,temp,l,',');

			l=strtok2(pdes,temp,k,',');

		}

		trim(pdes);

		str_to_date_time(pdes,General_Snapshoot_time);

	}

	f_wip.close();

	f_wip.clear();

	output_date_time(General_Snapshoot_time);

	//------------------------------------------------------------------------------get the initial_running_lot

	//added by Zhufeng. Initialize Machine_vector.hasInitialLot

	for(int i=1;i<=(int)Machine_vector.size();i++)

	{

		Machine_vector[i-1].hasInitialLot=false;

	}



	//ended by Zhufeng

	f_wip.open("Debug/wip.csv");

	f_wip.getline(temp,1000,'\n');

	f_wip.getline(temp,1000,'\n');

	trim(temp);	

	cout<<"get the initial_running_lot"<<endl;

	int line_num=2;

	while(!f_wip.eof() && strlen(temp)>0){

		int l=0,k=0;

		//------------------------------------------test if the lot is an initial lot

		for(int j=1;j<=6;j++){

			k=strtok2(pdes,temp,l,',');

			l=strtok2(pdes,temp,k,',');

		}

		k=strtok2(pdes,temp,l,','); //get "Start_time"

		trim(pdes);

		//---------------------------------------------------------------------------

		if(strlen(pdes)>0){//this field is not empty,thus the lot is running initially

			cout<<temp<<endl;	

			struct initial_running_lot_item one_item;

			l=0;

			k=strtok2(pdes,temp,l,',');//get lot name

			trim(pdes);

			strcpy(one_item.lot_name,pdes);

			l=k;

			k=strtok2(pdes,temp,l,',');//get device name

			trim(pdes);

			strcpy(one_item.dev_name,pdes);

			l=k;

			k=strtok2(pdes,temp,l,',');//get pkg name

			trim(pdes);

			strcpy(one_item.PKG_name,pdes);

			l=k;

			k=strtok2(pdes,temp,l,',');//get pin number

			trim(pdes);

			one_item.PIN_num=atoi(pdes);

			l=k;

			k=strtok2(pdes,temp,l,',');//get quantity

			trim(pdes);

			one_item.n_l_chips=atoi(pdes);

			l=k;

			k=strtok2(pdes,temp,l,',');//get lot weight

			trim(pdes);

			one_item.w_l=(double)atof(pdes);



			for(int j=1;j<=7;j++){

				l=k;

				k=strtok2(pdes,temp,l,',');//get "Start_time"

			}

			trim(pdes);

			str_to_date_time(pdes,one_item.Start_time);

			l=k;

			k=strtok2(pdes,temp,l,',');//get "Snapshoot_time"

			trim(pdes);

			str_to_date_time(pdes,one_item.Snapshoot_time);

			l=k;

			k=strtok2(pdes,temp,l,',');//get machine name

			trim(pdes);

			strcpy(one_item.machine_name,pdes);

			int machine_id=-1;

			for(int j=1;j<=(int)MI.size();j++){

				if(!strcmp(MI[j-1],one_item.machine_name)){

					machine_id=j; break;

				}

			}



			if(machine_id<0){//if the machine_id is not included in <MI>,then simply discard the info of this line. Continue to the next line

				cout<<"Warnning message: in file wip.csv, the machine_instance "<<one_item.machine_name<<" corresponds to lot "<<one_item.lot_name<<" in line "<<line_num

					<<" does not exist in file machines.csv"<<endl;

				//added by Zhufeng

				f_log<<"In wip.csv: This is an initial lot but the machine_instance"<<one_item.machine_name<<" corresponds to lot"<<one_item.lot_name<<" is not found in machines.csv. The row is skipped."<<endl;

				//end Zhufeng

				f_wip.getline(temp,1000,'\n');

				line_num++;

				continue;

			}



			// ----- added by yufen ---------

			else{

				Machine_vector[machine_id-1].hasInitialLot = true;

				Machine_vector[machine_id-1].initial_lot = (int)initial_running_lot.size();	

			}

			// ----- end yufen --------------



			one_item.machine_id=machine_id;



			//Added by Zhufeng for logpoint, Sep 02, 2011
			k=0;l=0;
			for(int i=1;i<=8;i++)
			{
				k=strtok2(pdes,temp,l,',');
				l=k;
			}
			trim(pdes);
			one_item.logpoint=atoi(pdes);

			k=0;l=0;
			for(int i=1;i<=9;i++)
			{
				k=strtok2(pdes,temp,l,',');
				l=k;
			}
			trim(pdes);
			one_item.operation=atoi(pdes);
			//end by Zhufeng



			initial_running_lot.push_back(one_item);

		}//end_if

		f_wip.getline(temp,1000,'\n');

		trim(temp);	

		line_num++;

	}

	f_wip.close();

	f_wip.clear();

	extend_initial_lot(initial_running_lot); //this fuctin is called immedaite after <initial_running_lot> is ready to extend the arrays

	//-------------------------------------------------------------------This is part extend <DEV> <PKG> device_PKG and device_PIN

	//-----------------------------------------------------------------------------------------------------------

	//read the keydevices.csv

	ifstream p_key("Data/keydevices.csv");

	p_key.getline(temp,1000,'\n');

	p_key.getline(temp,1000,'\n');

	//adde by Zhufeng

	row_no=2;

	trim(temp);	

	Num_Keydevice=0;

	while(!p_key.eof() && strlen(temp)>0){

		int l=0,k=0;

		k=strtok2(pdes,temp,l,',');

		trim(pdes);

		//added by Zhufeng

		if(strlen(pdes)<=0)

		{

			f_log<<"The "<<row_no<<"th row in keydevices.csv: The Key_Device is empty. Skip the row."<<endl;

			p_key.getline(temp,1000,'\n');

			trim(temp);

			row_no++;



		}else

		{ //end Zhufeng

			int dev=0;

			for(int id=1;id<=(int)DEV.size();id++){

				if(!strcmp(DEV[id-1],pdes)){ dev=id;break;}

			}

			if(dev<1){ 

				cout<<"Add device "<<pdes<<" to the set of devices DEV"<<endl; 

				//added by Zhufeng

				f_log<<"The "<<row_no<<"th row in keydevices.csv: The Key_Device is not found (or skipped )in wip.csv. Add device "<<pdes<<" to the set of devices DEV. It is still used."<<endl;

				//end Zhufeng

				dev=Num_Device+1; 

				Num_Device++;

				char * name=new char[50];

				strcpy(name,pdes);

				DEV.push_back(name);

				//link the device just added to the dummy_pkg since there is no package info for the device

				device_PKG[Num_Device]=dummy_pkg_pos;

			}

			Key_Devices.push_back(dev);

			l=strtok2(pdes,temp,k,',');

			trim(pdes);

			//added by Zhufeng

			int target=atoi(pdes);

			if(target<=0)

			{

				f_log<<"The "<<row_no<<"th row in keydevices.csv: The Target is non-positive.It is still used."<<endl;



			}

			//end by Zhufeng

			n_k_minchips[dev]=atoi(pdes);

			Num_Keydevice++;

			p_key.getline(temp,1000,'\n');

			trim(temp);

			//added by Zhufeng

			row_no++;

			

		}

		//end Zhufeng

	}

	p_key.close();

	//----------------------------------------this is originally for package.csv

	//however, since the definition for package is changed, we need to update this part for key_package

	//read the key_package.csv

	ifstream p_package("Data/key_package.csv");

	p_package.getline(temp,1000,'\n');

	p_package.getline(temp,1000,'\n');

	trim(temp);

	row_no=2;

	Num_Key_PKG=0;

	while(!p_package.eof() && strlen(temp)>0){

		int l=0,k=0;

		k=strtok2(pdes,temp,l,',');

		trim(pdes);

		//added by Zhufeng

		if(strlen(pdes)<=0)

		{

			f_log<<"The "<<row_no<<"th row in key_package.csv: The Key_Package is empty. Skip the row."<<endl;

			p_key.getline(temp,1000,'\n');

			trim(temp);

			row_no++;

		}else

		{//end Zhufeng

			int pkg=0;

			for(int id=1;id<=(int)PKG.size();id++){

				if(!strcmp(PKG[id-1],pdes)){ pkg=id;break;}

			}

			if(pkg<1){ 

				cout<<"Add package "<<pdes<<" to the set of PKG in reading key_package.csv"<<endl; 

				//added by Zhufeng

				f_log<<"The "<<row_no<<"th row in key_package.csv: The Key_Package is not found in wip.csv. Add package "<<pdes<<" to the set of PKG. It is still used."<<endl;

				//end Zhufeng

				pkg=(int)PKG.size()+1; 

				char * name=new char[50];

				strcpy(name,pdes);

				PKG.push_back(name);

			}

			Key_PKG.push_back(pkg);

			l=strtok2(pdes,temp,k,',');

			trim(pdes);

			//added by Zhufeng

			int target=atoi(pdes);

			if(target<=0)

			{

				f_log<<"The "<<row_no<<"th row in key_package.csv: The Key_Package is non-positive.It is still used."<<endl;



			}

			//end by Zhufeng



			n_kp_minchips[pkg]=atoi(pdes);

			//cout<<"pkg="<<pkg<<",target="<<n_kp_minchips[pkg]<<endl;

			Num_Key_PKG++;

			p_package.getline(temp,1000,'\n');

			trim(temp);	

			row_no++;

			//added by Zhufeng

		}//end of if(strlen(pdes)<=0) else

		//end Zhufeng

	}

	p_package.close();

	//-------------------------------------------------------------read key_pin_package.csv

	ifstream f_kpp("Data/key_pin_package.csv");

	f_kpp.getline(temp,1000,'\n');

	f_kpp.getline(temp,1000,'\n');

	trim(temp);

	row_no=2;

	while(!f_kpp.eof() && strlen(temp)>0){

		//package,pin,target

		int l=0,k=0;

		k=strtok2(pdes,temp,l,','); //package

		trim(pdes);

		if(strlen(pdes)<=0)

		{

			f_log<<"The "<<row_no<<"th row in key_pin_package.csv: The Key_Package is empty. Skip the row."<<endl;

			p_key.getline(temp,1000,'\n');

			trim(temp);

			row_no++;



		}else

		{

			int pkg=0,pin=0;

			for(int id=1;id<=(int)PKG.size();id++){

				if(!strcmp(PKG[id-1],pdes)){ pkg=id;break;}

			}

			if(pkg<1){ 

				cout<<"Add package "<<pdes<<" to the set of PKG in reading key_pin_package.csv"<<endl; 

				//added by Zhufeng

				f_log<<"The "<<row_no<<"th row in key_pin_package.csv: The Key_Package is not found in wip.csv. Add package "<<pdes<<" to the set of PKG. It is still used."<<endl;

				//end Zhufeng

				pkg=(int)PKG.size()+1; 

				char * name=new char[50];

				strcpy(name,pdes);

				PKG.push_back(name);

			}

			l=k;

			k=strtok2(pdes,temp,l,','); //pin

			trim(pdes);

			pin=atoi(pdes);

			l=k;

			k=strtok2(pdes,temp,l,','); //target

			trim(pdes);

			int target=atoi(pdes);

			if(pin<=0 || target<=0)

			{

				f_log<<"The "<<row_no<<"th row in key_pin_package.csv: The Pin or the Target is non-positive.It is still used."<<endl;



			}

			Key_PIN_PKG.push_back(make_pair(make_pair(pin,pkg),target));

			f_kpp.getline(temp,1000,'\n');

			trim(temp);	

			//added by Zhufeng

			row_no++;

		}//end of /ifif(strlen(pdes)<=0)//package

		//end Zhufeng

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



	//added by Zhufeng. Initialize eps 
	
	for(int i=1;i<=Max_Num_Tooling_Configure;i++)
	{
		eps[i-1]=0;
	}
	//end Zhufeng



	//comment by Zhufeng

	//ifstream f_route("Data/route.csv");

	//end Zhufeng

    //Added by Zhufeng

	ifstream f_route("Debug/route.csv");

	//end Zhufeng

	f_route.getline(temp,1000,'\n');

	f_route.getline(temp,1000,'\n');

	trim(temp);

	int count=0;

	while(!f_route.eof() && strlen(temp)>0){

		int l=0,k=0;

		for(int i=1;i<=50;i++){ pdes[i]=' ';}



		//added by Zhufeng for logpoint, Aug 31, 2011

		//get logpoint
		for(int i=1;i<=2;i++)
		{
			k=strtok2(pdes,temp,l,',');
			l=k;
		}
        
		trim(pdes);

		int logpoint=atoi(pdes);

		l=0;k=0;
		for(int i=1;i<=3;i++)
		{
			k=strtok2(pdes,temp,l,',');
			l=k;
		}
        
		trim(pdes);

		int operation=atoi(pdes);

		l=0;k=0;

		//end by Zhufeng



		//get the 6th element of the line, which contains the device info

		for(int n=1;n<=3;n++){

			k=strtok2(pdes,temp,l,',');

			l=strtok2(pdes,temp,k,',');

		}

		trim(pdes);

		int dev=-1;

		for(int id=1;id<=(int)DEV.size();id++){//get the device id in DEV vector

			if(!strcmp(DEV[id-1],pdes)){ dev=id;break;}

		}

		if(dev<0){

			cout<<"device "<<pdes<<" cannot be found in vector DEV"<<endl;

		}	

		if(strlen(pdes)>=1){//this is a new line of the route table

			count++;

			k=strtok2(pdes,temp,l,',');//skip the package

			l=k;

			k=strtok2(pdes,temp,l,',');//skip the pin

			l=k;

			trim(pdes);

			struct tooling_configure one_tc;



			//added by Zhufeng
			one_tc.logpoint=logpoint;
			one_tc.operation=operation;
			//end by Zhufeng



			one_tc.device=dev; //it is possible that dev=-1 since the device in route.csv is not in wip.csv

			one_tc.id=count;

			k=strtok2(pdes,temp,l,',');//get alternative

			trim(pdes);

			if(strlen(pdes)>0){ eps[one_tc.id]=(min_weight-0.1)/2.0;} //this is an alternative route; o.w. eps=0;

			l=strtok2(pdes,temp,k,',');//skip setup description

			k=strtok2(pdes,temp,l,',');//get PPH

			trim(pdes);



			// ---- added by yufen -------

			one_tc.PPH = (double)atof(pdes);

			// ---- end yufen ------------



			rate[one_tc.id]=(double)atof(pdes);// NOTE:when clean S data the rate[] should be updated accordingly

			l=strtok2(pdes,temp,k,',');//get machine family

			trim(pdes);

			int mg=-1;

			for(int id=1;id<=(int)MF.size();id++){//get the machine family id in the MF vector

				if(!strcmp(MF[id-1],pdes)){ mg=id;break;}

			}

			one_tc.mg=mg;//it is possible that mg=-1

			k=strtok2(pdes,temp,l,',');//get tooling family

			trim(pdes);

			//add the following statement such that pdes will never to empty

			//the empty field in the line is replaced by "No_Tooling"

			if(strlen(pdes)<1){ strcpy(pdes,"No_Tooling");}	

			if(strlen(pdes)>=1){//non-empty

				int tf=-1;

				for(int id=1;id<=(int)TF.size();id++){//get the tooling family id in the TF vector

					if(!strcmp(TF[id-1],pdes)){ tf=id;break;}

				}

				if(tf>0){// with tooling requirement

					eps[one_tc.id]+=(min_weight-0.1)/4.0;

				}

				one_tc.tf.tools.push_back(tf);

				if(tf>0){

					l=strtok2(pdes,temp,k,',');//get the number of tooling piece

					trim(pdes);

					int pos=one_tc.id*Max_Num_Tooling_Family+tf; //if tf=-1, then the pos will not be right

					//a_st[one_tc.id][tf]=atoi(pdes);//!!NOTE:when clean S data a_st[][] need to be updated accordingly

					a_st[pos]=atoi(pdes);

				}else{

					l=strtok2(pdes,temp,k,',');//skip the number of tooling piece

				}

				k=strtok2(pdes,temp,l,',');//get certification

				trim(pdes);

				one_tc.temperature=atoi(pdes);

				S.push_back(one_tc);

			}else{//empty

				int tf=0;//no tooling requirement

				one_tc.tf.tools.push_back(tf);

				l=strtok2(pdes,temp,k,',');

				trim(pdes);

				int pos=one_tc.id*Max_Num_Tooling_Family+tf;

				//a_st[one_tc.id][tf]=atoi(pdes);//!!NOTE:when clean S data ast[][] need to be updated accordingly

				a_st[pos]=atoi(pdes);

				k=strtok2(pdes,temp,l,',');

				trim(pdes);

				one_tc.temperature=atoi(pdes);

				S.push_back(one_tc);

			}

		}else{//need to append tooling families and pieces

			for(int m=1;m<=3;m++){

				k=strtok2(pdes,temp,l,',');

				l=strtok2(pdes,temp,k,',');

			}

			k=strtok2(pdes,temp,l,',');//get additional tooling family

			trim(pdes);

			//now there should be non-empty tf, since this is an additional line for tooling info

			int tf=-1;

			for(int id=1;id<=(int)TF.size();id++){

				if(!strcmp(TF[id-1],pdes)){ tf=id;break;}

			}

			//it is possible that tf=-1;

			vector<tooling_configure>::iterator p_TC=S.end()-1;

			(*p_TC).tf.tools.push_back(tf);

			if(tf>0){

				l=strtok2(pdes,temp,k,',');//get number of pieces

				trim(pdes);

				int id=(*p_TC).id;

				int pos=id*Max_Num_Tooling_Family+tf;//if tf=-1, then the pos will not be right

				//a_st[id][tf]=atoi(pdes);//!!Note: when clean S data a_st[][] need to be updated

				a_st[pos]=atoi(pdes);

			}

		}

		f_route.getline(temp,1000,'\n');

		trim(temp);	

	}

	f_route.close();



	//---------------------------------------------------------------------------------------The following part is used to clean <S>,rate[] and a_st[][]

	//still need to clean the S data,rate[] and a_st[][]

	//since it is possible that some device,machine family,tooling family are -1

	vector<int> route_to_remove;  //this vector stores the routes to remove

	vector<int> reason_to_remove; //this vector stores the reason why the routes are remvoed 1->negative device; 2->negative mg; 3->negative temperature; 4->negative tf

	vector<pair<int,int> > old_new;

	for(int id=1;id<=(int)S.size();id++){

		old_new.push_back(make_pair(id,id));

	}

	for(int id=1;id<=(int)S.size();id++){

		bool remove_flag=false;

		int reason=0;

		//Zhufeng:since device of initial lots have been added into DEV, why yufen comment this?

		//Note this part(1) must be modified with the following part(2) together

		// --- commented by yufen ---

		//if(S[id-1].device<1){ reason=1;remove_flag=true;}

		// --- end yufen ------------

		//part(1)added by Zhufeng

		if(S[id-1].device<1){ reason=1;remove_flag=true;}

		//end by Zhufeng



		if(S[id-1].mg<1)    { reason=2;remove_flag=true;}

		if(S[id-1].temperature<1){ reason=3;remove_flag=true;}

		for(int id2=1;id2<=(int)S[id-1].tf.tools.size();id2++){

			int tf=S[id-1].tf.tools[id2-1];

			if(tf<1){ reason=4;remove_flag=true;break;}

		}

		if(remove_flag){//need to remove the route

			route_to_remove.push_back(id);

			reason_to_remove.push_back(reason);

		}

	}

	cout<<"route_to_remove"<<endl;

	for(int id=1;id<=(int)route_to_remove.size();id++){

		cout<<route_to_remove[id-1]<<",Reason code="<<reason_to_remove[id-1]<<":";

		switch(reason_to_remove[id-1]){

			case 1:

				cout<<"negative device"<<endl;

				break;

			case 2:

				cout<<"netagive machine family"<<endl;

				break;

			case 3:

				cout<<"negative temperature"<<endl;

				break;

			case 4:

				cout<<"negative tooling family"<<endl;

				break;

			default:

				cout<<"Wrong reason to remove the route,need to check this part"<<endl;

				abort();

		}

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



	//added by Zhufeng
	//update eps[]
	double * eps_save=new double [Max_Num_Tooling_Configure];
	for(int id=1;id<=Max_Num_Tooling_Configure;id++){ eps_save[id-1]=eps[id-1];}

	//end Zhufeng



	//update a_st[][] and rate[]

	//save a copy of a_st[][] and rate[]

	//int a_st_save[Max_Num_Tooling_Configure][Max_Num_Tooling_Family];

	int * a_st_save=new int [Max_Num_Tooling_Configure*Max_Num_Tooling_Family];

	//double rate_save[Max_Num_Tooling_Configure];

	int * rate_save=new int [Max_Num_Tooling_Configure];



	for(int id=1;id<=Max_Num_Tooling_Configure;id++){ rate_save[id-1]=rate[id-1];}



	for(int id=1;id<=Max_Num_Tooling_Configure;id++){

		for(int id2=1;id2<=Max_Num_Tooling_Family;id2++){

			int pos=(id-1)*Max_Num_Tooling_Family+id2-1;

			//a_st_save[id-1][id2-1]=a_st[id-1][id2-1];

			a_st_save[pos]=a_st[pos];

		}

	}

	//cout << "test 1" <<endl;

	for(int id=1;id<=(int)old_new.size();id++){

		int old_id=old_new[id-1].first;

		int new_id=old_new[id-1].second;

		//cout << "old " << old_id << " new " << new_id <<endl;

		if(old_id!=new_id && new_id>0){

			rate[new_id]=rate_save[old_id];

			rate[old_id]=0;



			//added by Zhufeng

			eps[new_id]=eps_save[old_id];
			eps[old_id]=0;

			//end Zhufeng



			//cout << "test 1a" <<endl;

			for(int id2=0;id2<=Max_Num_Tooling_Family-1;id2++){

			  	//cout << "test 1a2 " << id2 <<endl;

				//a_st[new_id][id2]=a_st_save[old_id][id2];

				a_st[new_id*Max_Num_Tooling_Family+id2]=a_st_save[old_id*Max_Num_Tooling_Family+id2];

				//a_st[old_id][id2]=0;

				a_st[old_id*Max_Num_Tooling_Family+id2]=0;

			}

			//cout << "test 1b" <<endl;

		}

	}

	//cout << "test 2" <<endl;

	delete [] a_st_save;

	delete [] rate_save;

	delete [] eps_save;

    

	//remove the routes

	vector<tooling_configure>::iterator p_S=S.begin();

	while(p_S!=S.end()){

		bool remove_flag=false;

		// --- commented by yufen ---

		//if((*p_S).device<1 || (*p_S).mg<1 || (*p_S).temperature<1){ remove_flag=true;}

		// --- end yufen ----



		//comment by Zhufeng. Why yufen don't want delete the route with negative dev_id 

		// --- added by yufen ---

		//if((*p_S).mg<1 || (*p_S).temperature<1){ remove_flag=true;}

		// --- end yufen ---

		//end Zhufeng



		//part(2)added by Zhufeng.This Part(2) must be modified with the part(1) 

		

		if((*p_S).device<1 || (*p_S).mg<1 || (*p_S).temperature<1){ remove_flag=true;}

		//end Zhufeng

		

		for(int id2=1;id2<=(int)(*p_S).tf.tools.size();id2++){

			int tf=(*p_S).tf.tools[id2-1];

			if(tf<1){ remove_flag=true; break;}

		}

      

		if(remove_flag){//need to remove the route

			

			//comment by Zhufeng. By doing this yufen's modify to route_remove is not needed any more!

			//cout<<"The removed route is associated to device "<<DEV[(*p_S).device-1]<<endl;	

			//end Zhufeng

			

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

			int pos=tf*Max_Num_Temperature+id;

			//n_t_n_tooling[tf][id]=0;

			n_t_n_tooling[pos]=0;

			for(int id2=1;id2<=(int)Tooling_vector.size();id2++){

				//cout<<"id2="<<id2<<",tf="<<Tooling_vector[id2-1].tf<<",int_vec=";

				//for(int id3=1;id3<=Tooling_vector[id2-1].temperature_vector.size();id3++){ cout<<Tooling_vector[id2-1].temperature_vector[id3-1]<<",";}

				//cout<<endl;

				if(Tooling_vector[id2-1].tf==tf && compare_int_vec(Tooling_vector[id2-1].temperature_vector,Temperature_Set[id-1].set_elements)){

					//n_t_n_tooling[tf][id]++;

					n_t_n_tooling[pos]++;

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

	trim(temp);	

	while(!p_tf_temperature.eof() && strlen(temp)>0){

		int k=0,l=0;

		k=strtok2(pdes,temp,l,',');//get the tooling family

		trim(pdes);

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

			trim(pdes);

			char pdes2[100];

			int k2=0,l2=0;

			struct t_set one_set;

			do{

				k2=strtok2(pdes2,pdes,l2,';');//get the list of temperatures

				trim(pdes2);

				int element=atoi(pdes2);

				if(element>0){

					one_set.set_elements.push_back(element);

				}

				l2=k2;

			}while(pdes[l2-1]!='\0');

			int id=get_set_id(one_set);

			k=strtok2(pdes,temp,l,',');// get the number of tooling pieces

			trim(pdes);

			n_t_n_tooling[tf][id]=atoi(pdes);// this is the purpose to read the file

			//cout<<"tf="<<tf<<",temperature id="<<id<<","<<atoi(pdes)<<endl;

		}

		p_tf_temperature.getline(temp,1000,'\n');

		trim(temp);	

	}

	p_tf_temperature.close();

	*/



	ofstream f_n_t_n("Debug/debug_n_t_n_tooling.txt");

	for(int tf=1;tf<=Num_Tooling_Family;tf++){

		for(int id=1;id<=(int)Temperature_Set.size();id++){

			int pos=tf*Max_Num_Temperature+id;

			f_n_t_n<<"tf="<<tf<<",id="<<id<<",n_t_n_tooling[tf][id]="<<n_t_n_tooling[pos]<<endl;

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

	row_no=2;

	trim(temp);	

	while(!f_mach_hr.eof() && strlen(temp)>0){

		int k=0,l=0;

		k=strtok2(pdes,temp,l,',');

		trim(pdes);

		int mach=0;

		for(int id=1;id<=(int)MI.size();id++){

			if(!strcmp(MI[id-1],pdes)){ mach=id;break;}

		}

		if(mach<1){ 

			cout<<"machine instance "<<pdes<<" is not included in machines.csv, discard it"<<endl; 

			//added by Zhufeng

			f_log<<"The "<<row_no<<"th row in machine_hours.csv: machine instance "<<pdes<<" is not included in machines.csv. The row is skipped."<<endl;

			row_no++;

			//end Zhufeng

			f_mach_hr.getline(temp,1000,'\n');

			continue;

		}

		l=strtok2(pdes,temp,k,',');

		trim(pdes);

		H[mach]=(double)atof(pdes);

		f_mach_hr.getline(temp,1000,'\n');

		//added by Zhufeng

		row_no++;

		//end by Zhufeng

		trim(temp);	

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



	//added by Zhufeng

	f_log.close();

	//f_log.clear();

}



void AT::readInitialSetups(){



		//=======================================================================================================

	//------ added by yufen ---------------------------------------------------

	//------ to get initial setup information for each machine instance -------

	cout<<" ====================================== read initialsetup.csv ===================================== "<<endl;

	char temp[1000],pdes[500];



	//Added by Zhufeng. For exception process

	fstream f_log("Output/log_error.txt", ios::out|ios::app);



	int row_no;

	ifstream f_machines("Debug/initialsetup.csv");

	//end by Zhufeng

	//comment by Zhufeng

	//ifstream f_machines("Data/initialsetup.csv");

    //end by Zhufeng

 	

	f_machines.getline(temp,1000,'\n');

	f_machines.getline(temp,1000,'\n');

	trim(temp);	

	//row_no=2;



	while(!f_machines.eof() && strlen(temp)>0)

	{

		char machineName[50];

		char toolFamilyName[50];

		int temperature;



		int l=0,k=0;

		for(int i=1;i<=50;i++){ pdes[i]=' ';}

		k=strtok2(pdes,temp,l,',');		//Machine_Family

		cout<<pdes<<"	";

		l=strtok2(pdes,temp,k,',');		//Machine_Instance

		cout<<pdes<<"	";

		trim(pdes);

		strcpy(machineName,pdes);

		k=strtok2(pdes,temp,l,',');		//Machine_Family_Name

		cout<<pdes<<"	";

		l=strtok2(pdes,temp,k,',');		//Initial_Tool_Family

		cout<<pdes<<"	";

		trim(pdes);

		strcpy(toolFamilyName,pdes);

		k=strtok2(pdes,temp,l,',');		//Initial_Certification

		cout<<pdes<<"	";

		trim(pdes);

		temperature = atoi(pdes);

		cout<<endl;



		int machineIdx=0;

		for(int id=1;id<=(int)MI.size();id++)

		{			

			if(strcmp(MI[id-1],machineName)==0)

			{

				machineIdx=id;

				break;

			}			

		}



		if(machineIdx<1)

		{

			//added by Zhufeng

			f_log<<"In initialsetup.csv, Machine "<< machineName<<" is not found in machines.csv."<<endl;

			//end Zhufeng

			cout<<"Machine "<< machineName<<" is not considered in this time period!!!"<<endl;

		}else{



			if(strlen(toolFamilyName)<=0)

			{

				Machine_vector[machineIdx-1].hasInitialSetup = false;

				Machine_vector[machineIdx-1].hasInitialToolingConfigure = false;

			}else{

				int toolFamilyId = 0;

				for(int id=1;id<=(int)TF.size();id++)

				{

					if(strcmp(TF[id-1],toolFamilyName)==0)

					{

						toolFamilyId=id;

						break;

					}

				}

				

				if(toolFamilyId<1)

				{

					//added by Zhufeng

			        f_log<<"In initialsetup.csv, Tool "<< toolFamilyName<<" is not found in tooling.csv."<<endl;

			        //end Zhufeng

					cout<<"Error: Tool "<< toolFamilyName <<" does not exisit!!!"<<endl;



				}else{



                    //comment by Zhufeng

					/*Machine_vector[machineIdx-1].hasInitialSetup = true;

					Machine_vector[machineIdx-1].initial_tool = toolFamilyId;

					Machine_vector[machineIdx-1].initial_temperature = temperature;

                    */ 

					//end Zhufeng

					int setupIdx=0;

					for(int id=1;id<=(int)Lambda.size();id++)

					{

						if(		find(Lambda[id-1].tooling_vec.begin(),Lambda[id-1].tooling_vec.end(),toolFamilyId)!=Lambda[id-1].tooling_vec.end()

							&&	(Lambda[id-1].temperature				== temperature)	)

						{

							//Added by Zhufeng

                            setupIdx=id;

							//end by Zhufeng

							Machine_vector[machineIdx-1].initial_tooling_setup = id;

							cout<<"			lambda="<<id<<"	"<<endl;

							break;

						}

						

					}

					//Added by Zhufeng

					int initial_lot=Machine_vector[machineIdx-1].initial_lot;



					//Added by Zhufeng for logpoint, Sep 03, 2011

					int logpoint=initial_running_lot[initial_lot].logpoint;

					int operation=initial_running_lot[initial_lot].operation;

					//end by Zhufeng for logpoint, Sep 03, 2011

					if(setupIdx<1)

					{

						cout<<"Error: Machine"<<Machine_vector[machineIdx-1].name<<"'s initial setup: tooling family(" <<TF[toolFamilyId-1]<<")and temperatue("<<temperature

							<<")is not found in (Lambda)Route.csv."<<endl<<" This error may cause segmentation fault"<<endl;

						f_log<<"In initialsetup.csv, Machine"<<Machine_vector[machineIdx-1].name<<"'s initial setup: the combination of tooling family(" <<TF[toolFamilyId-1]<<")and temperatue("<<temperature

							<<")is not found in route.csv. This machine would be taken as no initial setup"<<endl;

						

						

						if(initial_lot>=0)

						{

							//This is impossible to happen since the lots have been removed if the initial setup of machine is not found in route.csv 

							//and the corresponding machine also has been removed from initialsetup.csv when we do data consistency checking.

							//just in case.

							f_log<<"Skip the initial lot: ";

                            f_log<<initial_running_lot[initial_lot].lot_name<<"in the wip.csv."<<endl;

	                        initial_running_lot.erase(initial_running_lot.begin()+initial_lot);	

							Machine_vector[machineIdx-1].hasInitialLot=false;

                            Machine_vector[machineIdx-1].initial_lot=-1;

							//since the initial_running_lot changed, the initial_lot of some machine_vector should be modified.

							for (int i=initial_lot;i<=((int)initial_running_lot.size()-1);i++)

							{

								int machine_vector_id=initial_running_lot[i].machine_id;

								Machine_vector[machine_vector_id-1].initial_lot--;

							}

						}else

						{

							f_log<<"There is no initial lot on the machine "<<Machine_vector[machineIdx-1].name<<".So no lot skipped.";



						}

    

					}else

					{

						Machine_vector[machineIdx-1].hasInitialSetup = true;

						Machine_vector[machineIdx-1].initial_tool = toolFamilyId;

						Machine_vector[machineIdx-1].initial_temperature = temperature;



						//int initial_lot=Machine_vector[machineIdx-1].initial_lot;

						//cout<<Machine_vector[machineIdx-1].name<<":"<<initial_running_lot[initial_lot].lot_name<<endl;

						//End by Zhufeng

						if(!Machine_vector[machineIdx-1].hasInitialLot)

						{

							f_log<<"In initialsetup.csv machine "<<Machine_vector[machineIdx-1].name<<"has initial tooling setups.But there is no initial lots in wip.csv on the machine.The machine is taken as a machine with initial setup but without initial lot."<<endl;



						}else

						{

							int dev_Idx = 0;

							for(int dev_id=1; dev_id<=DEV.size(); dev_id++)

							{

								if(strcmp(DEV[dev_id-1], initial_running_lot[Machine_vector[machineIdx-1].initial_lot].dev_name) == 0)//it is possible that some machine has initial setup but not initial lot.

									//then initial_lot=-1.This would result in wrong initial tooling configure.

								{

									dev_Idx = dev_id;

									break;

								}

							}

							//Added by Zhufeng

							if(dev_Idx<1)

							{

								//This looks like impossible to happen,but just in case.

								f_log<<Machine_vector[machineIdx-1].name<<"'s initial lot's device("<<initial_running_lot[Machine_vector[machineIdx-1].initial_lot].dev_name<<") is not found in wip.csv."<<endl;

								f_log<<"Skip the initial lot: ";

                                f_log<<initial_running_lot[initial_lot].lot_name<<"in the wip.csv."<<endl;

                                Machine_vector[machineIdx-1].hasInitialLot=false;

                                Machine_vector[machineIdx-1].initial_lot=-1;

	                            initial_running_lot.erase(initial_running_lot.begin()+initial_lot);	

								for (int i=initial_lot;i<=((int)initial_running_lot.size()-1);i++)

							{

								int machine_vector_id=initial_running_lot[i].machine_id;

								Machine_vector[machine_vector_id-1].initial_lot--;

							}



							}else

							{

								//End by Zhufeng

								int toolingConfigureIdx=0;

								for(int id=1;id<=(int)S.size();id++)

								{

									vector<int>::iterator tool_Idx=find(S[id-1].tf.tools.begin(),S[id-1].tf.tools.end(),toolFamilyId);						

									if(		(tool_Idx				!= S[id-1].tf.tools.end())

										&&	(dev_Idx				> 0)

										&&	(S[id-1].mg				== Machine_vector[machineIdx-1].mg)

										&&	(S[id-1].device			== dev_Idx)

										&&	(S[id-1].temperature	== temperature)	

										//added by Zhufeng for logpoint, Sep 03, 2011

                                        &&  (S[id-1].logpoint == logpoint)

										&&  (S[id-1].operation == operation)

										//end by Zhufeng for logpoint, Sep 03, 2011

										)

									{

										//comment by Zhufeng

										//Machine_vector[machineIdx-1].hasInitialToolingConfigure = true;

										//end Zhufeng

										

										toolingConfigureIdx=id;

										cout<<"			s ="<<id<<"	"<<endl;

										break;

									}

								}//end of for(int id=1;id<=(int)S.size();id++)

								//Added by Zhufeng

								if(toolingConfigureIdx<1)

								{

									//it is impossible to happen since the lot has been removed if the initial setup cannot process the lot because of no route when doing data consistency checking for wip.csv

									//just in case.

									f_log<<"Machine"<<Machine_vector[machineIdx-1].name<<"with its initial setup (tooling family " <<TF[toolFamilyId-1]<<" and certification "<<temperature

										<<") cannot process the initial lots"<<initial_running_lot[initial_lot].lot_name<<" because of no route is not found in (S) Route.csv."<<endl

										<<" But the tooling setup (tooling family " <<TF[toolFamilyId-1]<<" and certification "<<temperature<<") is found in route.csv.This machine is taken as a machine with initial setup but without initial lot"

									<<"skip the lot in the wip.csv but keep the initial setup on the machine in the initialsetup.csv."<<endl;

									Machine_vector[machineIdx-1].hasInitialLot=false;

                                    Machine_vector[machineIdx-1].initial_lot=-1;

	                                initial_running_lot.erase(initial_running_lot.begin()+initial_lot);	

									for (int i=initial_lot;i<=((int)initial_running_lot.size()-1);i++)

									{

										int machine_vector_id=initial_running_lot[i].machine_id;

										Machine_vector[machine_vector_id-1].initial_lot--;

									}



								}else

								{

									Machine_vector[machineIdx-1].hasInitialToolingConfigure = true;

									Machine_vector[machineIdx-1].initial_tooling_configure = toolingConfigureIdx;

								}

							}

						}//end of if(Machine_vector[machineIdx-1].hasInitialLot)

						//End by Zhufeng

					}//end of if(setupIdx<1) else



				}

			}

		}

		



		f_machines.getline(temp,1000,'\n');

		trim(temp);

	}

	f_machines.close();

	show_Machine_vector();

	//---- end yufen -----------------------------------------------------

	//======================================================================================================

	cout<<" ====================================== end reading initialsetup.csv ===================================== "<<endl;

	f_log.close();

}



void AT::get_S(vector<tooling_configure> & TS){

	//get all the routes

}



void AT::initialize_S_i(){



	//initialize the data structure S_i

	for(int i=1;i<=Num_Machine;i++){

		int mg=machine_mg[i];

		vector<Machine>::iterator p_M=Machine_vector.begin()+i-1;

		vector<int>::iterator p_int=(*p_M).temperature_vector.begin();

		vector<tooling_configure>::iterator p_S=S.begin();

		while(p_S!=S.end()){

			p_int=find((*p_M).temperature_vector.begin(),(*p_M).temperature_vector.end(),(*p_S).temperature);

			if((*p_S).mg==mg && p_int!=(*p_M).temperature_vector.end()){

				struct tooling_configure one_tc;

   							one_tc.mg=(*p_S).mg;

           					one_tc.device=(*p_S).device;

               				one_tc.id=(*p_S).id;

               				one_tc.temperature=(*p_S).temperature;

							one_tc.tf.tools.resize((int)(*p_S).tf.tools.size());

  							copy((*p_S).tf.tools.begin(),(*p_S).tf.tools.end(),one_tc.tf.tools.begin());

	

				S_i[i].tc_vector.push_back(one_tc);

			}

			p_S++;

		}

	}

}



void AT::show_S_i(){

	//output the data structure S_i

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

	//get the routes that can use machine i	

	int mg=machine_mg[i];

	vector<Machine>::iterator p_M=Machine_vector.begin()+i-1;

	vector<int>::iterator p_int=(*p_M).temperature_vector.begin();

	vector<tooling_configure>::iterator p_S=S.begin();

	while(p_S!=S.end()){

		p_int=find((*p_M).temperature_vector.begin(),(*p_M).temperature_vector.end(),(*p_S).temperature);

		if((*p_S).mg==mg && p_int!=(*p_M).temperature_vector.end()){

			struct tooling_configure one_tc;

			one_tc.mg=(*p_S).mg;

			one_tc.device=(*p_S).device;

			one_tc.id=(*p_S).id;

			one_tc.temperature=(*p_S).temperature;

			one_tc.tf.tools.resize((int)(*p_S).tf.tools.size());

			copy((*p_S).tf.tools.begin(),(*p_S).tf.tools.end(),one_tc.tf.tools.begin());

			//added by Zhufeng for logpoint, Sep 01, 2011
			one_tc.logpoint=(*p_S).logpoint;
			one_tc.operation=(*p_S).operation;
			//end by Zhufeng for logpoint



			TS.push_back(one_tc);

		}

		p_S++;

	}

}



void AT::get_S_l(vector<tooling_configure> & TS,int l){

	//get the routes that can process lot l

	int dev=lots_device[l];



	//added by Zhufeng for logpoint, Sep 1, 2011
	int logpoint=lot_logpoint[l];



	int operation=lot_operation[l];

	//end by Zhufeng

	vector<tooling_configure>::iterator p_S=S.begin();

	while(p_S!=S.end()){

		if((*p_S).device==dev

			//added by Zhufeng
			&& (*p_S).logpoint==logpoint && (*p_S).operation==operation
			//end by Zhufeng

			){

				struct tooling_configure one_tc;

				one_tc.mg=(*p_S).mg;

				one_tc.device=(*p_S).device;

				one_tc.id=(*p_S).id;

				one_tc.temperature=(*p_S).temperature;



				//added by Zhufeng for logpoint,Sep 01, 2011
				one_tc.logpoint=logpoint;
				one_tc.operation=operation;
				//end by Zhufeng

				one_tc.tf.tools.resize((int)(*p_S).tf.tools.size());

				copy((*p_S).tf.tools.begin(),(*p_S).tf.tools.end(),one_tc.tf.tools.begin());

				TS.push_back(one_tc);

		}

		p_S++;

	}

}



void AT::get_S(vector<tooling_configure> & TS,int i,int l){

	//get the routes that can use machine i to process lot l

	//----------------------------------

        int mg=machine_mg[i];

        vector<Machine>::iterator p_M=Machine_vector.begin()+i-1;

        vector<tooling_configure>::iterator p_S=S.begin();

       	int dev=lots_device[l]; 



		//added by Zhufeng for logpoint, AUg31, 2011
		int logpoint=lot_logpoint[l];

		

	    int operation=lot_operation[l];

		//end by Zhufeng for logpoint



	while(p_S!=S.end()){

                vii p_int=find((*p_M).temperature_vector.begin(),(*p_M).temperature_vector.end(),(*p_S).temperature);

                if((*p_S).mg==mg && p_int!=(*p_M).temperature_vector.end() && (*p_S).device==dev

					//added by Zhufeng for logpoint, Aug 31, 2011
					&& (*p_S).logpoint==logpoint && (*p_S).operation==operation
					//end by Zhufeng					

					){

                        struct tooling_configure one_tc;

        		one_tc.mg=(*p_S).mg;

			one_tc.device=(*p_S).device;

			one_tc.id=(*p_S).id;

			one_tc.temperature=(*p_S).temperature;



			//added by Zhufeng for logpiont, Aug 31, 2011
			one_tc.logpoint=logpoint;
			one_tc.operation=operation;
			//end by Zhufeng for logpoint



			//cout<<"size_of_S_tools="<<(int)(*p_S).tf.tools.size();	

			//if((int)(*p_S).tf.tools.size()<1 || (int)(*p_S).tf.tools.size()>2){ cout<<"!!!";}

			//cout<<endl;	

			one_tc.tf.tools.resize((int)(*p_S).tf.tools.size());

			copy((*p_S).tf.tools.begin(),(*p_S).tf.tools.end(),one_tc.tf.tools.begin()); 

         	        TS.push_back(one_tc);

                }

                p_S++;

        }

}



void AT::get_S(vector<tooling_configure> & TS,int i,int l,int tf){

	//get the routes that can use machine i to process lot l with tooling family tf

	get_S(TS,i,l);

	vector<tooling_configure>::iterator p_TS=TS.begin();

	while(p_TS!=TS.end()){

		vector<int>::iterator p_int=find((*p_TS).tf.tools.begin(),(*p_TS).tf.tools.end(),tf);

		if(p_int==(*p_TS).tf.tools.end()){

			vector<tooling_configure>::iterator p_erase=p_TS;	

			p_TS=TS.erase(p_erase);

		}

		else{p_TS++;}

	}

}



void AT::get_S(vector<tooling_configure> & TS,int i,int l,int tf,int tau){

	//get the routes that can use machine i to process lot l with tooling family tf under operating temperature tau

	get_S(TS,i,l,tf);

	vector<tooling_configure>::iterator p_TS=TS.begin();

	while(p_TS!=TS.end()){

		int tau_TS=(*p_TS).temperature;

		if(tau_TS!=tau){

			vector<tooling_configure>::iterator p_erase=p_TS;	

			p_TS=TS.erase(p_erase);

		}

		else{ p_TS++;}

	}

}



void AT::get_S_i_l_Lambda(vector<tooling_configure> &TS,int i,int l,int Lambda_id){

	//get the routes that can use machine i to process lot l with setup Lambda_id

	vector<tooling_configure> TS2;	

	get_S(TS2,i,l);

	vector<tooling_configure>::iterator p_TS2=TS2.begin();

	vector<int> a;	

	while(p_TS2!=TS2.end()){

		a.clear();	

		int s=(*p_TS2).id;

		vii pt=(*p_TS2).tf.tools.begin();

		while(pt!=(*p_TS2).tf.tools.end()){

			int tf=(*pt);

			int pos=s*Max_Num_Tooling_Family+tf;

			//a.push_back(a_st[s][tf]);

			a.push_back(a_st[pos]);

			pt++;

		}

		int La_id=get_Lambda_id((*p_TS2).tf.tools,a,(*p_TS2).temperature);

		if(La_id==Lambda_id){

			struct tooling_configure one_tc;

			one_tc.mg=(*p_TS2).mg;

			one_tc.device=(*p_TS2).device;

			one_tc.id=(*p_TS2).id;

			one_tc.temperature=(*p_TS2).temperature;

			one_tc.tf.tools.resize((int)(*p_TS2).tf.tools.size());

			copy((*p_TS2).tf.tools.begin(),(*p_TS2).tf.tools.end(),one_tc.tf.tools.begin());

			//added by Zhufeng for logpoint
			one_tc.logpoint=(*p_TS2).logpoint;
			one_tc.operation=(*p_TS2).operation;
			//end by Zhufeng

			TS.push_back(one_tc);

		}

		p_TS2++;	

	}

}



void AT::get_Lambda(){

	//a Lambda_item is a combination of toolings,number of pieces,and temperature

	//get all the tooling setups

	vector<tooling_configure>::iterator p_S=S.begin();

	int count=1;

	while(p_S!=S.end()){

		vector<int> a;

		int s=(*p_S).id;

		vii pt=(*p_S).tf.tools.begin();

		while(pt!=(*p_S).tf.tools.end()){

			int tf=(*pt);

			int pos=s*Max_Num_Tooling_Family+tf;

			//a.push_back(a_st[s][tf]);

			a.push_back(a_st[pos]);

			pt++;

		}

		vector<Lambda_item>::iterator p_Lambda=Lambda.begin();

		while(p_Lambda!=Lambda.end()){

			if(compare_int_vec((*p_S).tf.tools,(*p_Lambda).tooling_vec) 

				&& compare_int_vec(a,(*p_Lambda).b) 

				&& (*p_S).temperature==(*p_Lambda).temperature){ break;}

			p_Lambda++;

		}

		if(p_Lambda==Lambda.end()){

			struct Lambda_item one_Lambda;

			one_Lambda.id=count;

			count++;

			one_Lambda.tooling_vec.resize((int)(*p_S).tf.tools.size());

			copy((*p_S).tf.tools.begin(),(*p_S).tf.tools.end(),one_Lambda.tooling_vec.begin());

			one_Lambda.b.resize((int)a.size());

			copy(a.begin(),a.end(),one_Lambda.b.begin());

			one_Lambda.temperature=(*p_S).temperature;

			Lambda.push_back(one_Lambda);

		}

		p_S++;

	}

}



void AT::show_Lambda(){

	//output the data structure Lambda to a debug file

	ofstream fout("Debug/debug_Lambda.txt");

	fout<<"show Lambda vector -------------------------------------"<<endl;

	vector<Lambda_item>::iterator p_Lambda=Lambda.begin();

	while(p_Lambda!=Lambda.end()){

		fout<<"id="<<(*p_Lambda).id<<",tooling=";

		vii p_int=(*p_Lambda).tooling_vec.begin();

		while(p_int!=(*p_Lambda).tooling_vec.end()){

			int offset=p_int-(*p_Lambda).tooling_vec.begin();

			vii pb=(*p_Lambda).b.begin()+offset;

			fout<<(*p_int)<<"("<<(*pb)<<"),";

			//Added by Zhufeng

			fout<<","<<TF[(*p_int)-1]<<",";

			//end by Zhufeng

			p_int++;

		}

		fout<<endl;

		fout<<"temperature="<<(*p_Lambda).temperature<<endl;

		p_Lambda++;

	}

	fout.close();

}



int AT::get_Lambda_id(vector<int> & tf,vector<int> & b,int & temperature){

	//given the tooling family tf, the associated number of pieces b and the temperature

	//return the index of the corresponding tooling setup



	//for(int i=1;i<=(int)tf.size();i++){ cout<<"i="<<i<<",tf="<<tf[i-1]<<endl;}

	//for(int i=1;i<=(int)b.size();i++){ cout<<"i="<<i<<",b="<<b[i-1]<<endl;}

	//cout<<"temperature="<<temperature<<endl;



	vector<Lambda_item>::iterator p_Lambda=Lambda.begin();

	while(p_Lambda!=Lambda.end()){

		if(compare_int_vec(tf,(*p_Lambda).tooling_vec) && compare_int_vec(b,(*p_Lambda).b) && temperature==(*p_Lambda).temperature){

			return (*p_Lambda).id;

		}

		p_Lambda++;

	}

	//could not find the tooling setup in Lambda

	cout<<"could not find the tooling setup in Lambda"<<endl;

	return 0;

}



void AT::get_Lambda(vector<int> & L,int i,int tf,int tau){

	//get the tooling setups that that can use machine i and tooling family tf under operating temperature tau

	//the results are stored in L



	//get Lambda(i,tf,tau)

	L.resize((int)Lambda_i[i].int_vector.size());

	copy(Lambda_i[i].int_vector.begin(),Lambda_i[i].int_vector.end(),L.begin());

	vii p_int=L.begin();

	while(p_int!=L.end()){

		vector<Lambda_item>::iterator p_Lambda=Lambda.begin()+(*p_int)-1;

		vii p_find=find((*p_Lambda).tooling_vec.begin(),(*p_Lambda).tooling_vec.end(),tf);

		if(p_find==(*p_Lambda).tooling_vec.end() || tau!=(*p_Lambda).temperature){

			p_int=L.erase(p_int);

		}else{

			p_int++;

		}

	}

}



void AT::initialize_Lambda_i(){

	//initialize the data structure Lamuda_i

	cout<<" ====================================  show lambda ====================================== "<<endl;

	

	////Added by Zhufeng for flexible setups of initial machines while don't allow other machines take initial setups that are not available at the beginning.Feb 26,2012

	//vector<int> s_id_taken_vector;

	//for(int i=1;i<=Num_Machine;i++)

	//{

	//	if (Machine_vector[i-1].hasInitialSetup)

	//	{

	//		s_id_taken_vector.push_back(Machine_vector[i-1].initial_tooling_setup);

	//	}



	//}

	////end by Zhufeng, Feb 26, 2012



	for(int i=1;i<=Num_Machine;i++){

		

      

		//// -------- added by yufen ----------



		//if (Machine_vector[i-1].hasInitialSetup)

		//{

		//	Lambda_i[i].int_vector.push_back(Machine_vector[i-1].initial_tooling_setup);



		//}else{



		//// -------- end yufen ---------------

		// //end Zhufeng



			vector<tooling_configure> TS;

			get_S(TS,i);

			vector<tooling_configure>::iterator p_TS=TS.begin();

			while(p_TS!=TS.end()){

				//----------------------------

				vector<int> a;

				int s=(*p_TS).id;



				////Added by Zhufeng for flexible setups of initial machines while don't allow other machines take initial setups that are not available at the beginning.Feb 26,2012

				//vii p_s_find=find(s_id_taken_vector.begin(),s_id_taken_vector.end(),s);

				//if(s==Machine_vector[i-1].initial_tooling_setup || p_s_find==s_id_taken_vector.end() )

				//{//When this is the current machine's initial setup or not taken by other machines as initial setups



				////end by Zhufeng, Feb 26, 2012



					vii pt=(*p_TS).tf.tools.begin();

					while(pt!=(*p_TS).tf.tools.end()){

						int tf=(*pt);

						int pos=s*Max_Num_Tooling_Family+tf;

						//a.push_back(a_st[s][tf]);

						a.push_back(a_st[pos]);

						pt++;

					}

					//----------------------------

					int id=get_Lambda_id((*p_TS).tf.tools,a,(*p_TS).temperature);

					vii p_find=find(Lambda_i[i].int_vector.begin(),Lambda_i[i].int_vector.end(),id);

					if(p_find==Lambda_i[i].int_vector.end()){

						Lambda_i[i].int_vector.push_back(id);

					}



				////Added by Zhufeng, for flexible setups of initial machines while don't allow other machines take initial setups that are not available at the beginning. Feb 26, 2012

				//}else

				//{

				//	//doing nothing

				//	cout<<"route s "<<s <<" cannot be used by machine"<<Machine_vector[i-1].name<<",since this route is other machine's initial setup"<<endl;

				//}

				////end by Zhufeng, Feb 26, 2012

				p_TS++;

			}

			vector<int> v((int)Lambda_i[i].int_vector.size(),0);

			vii p_int=Lambda_i[i].int_vector.begin();

			copy(p_int,p_int+(int)Lambda_i[i].int_vector.size(),v.begin());

			sort(v.begin(),v.end());

			swap(v,Lambda_i[i].int_vector);

		

		//comment by ZHufeng

		////-------- added by yufen ------------

		//}

		////--------- end yufen --------------------

        //end Zhufeng

		cout<<i<<"	machine="<<Machine_vector[i-1].name<<"	hasInitialSetup?="<<Machine_vector[i-1].hasInitialSetup<<"	numLambdas="<<Lambda_i[i].int_vector.size()<<"	1stLambda="<<*(Lambda_i[i].int_vector.begin())<<endl;

	}

	cout<<" ====================================  end showing lambda ====================================== "<<endl;

}



void AT::initialize_M_lambda(){

	//build M_lambda vector, should be called after initialize_Lambda_i

	vector<Lambda_item>::iterator p_Lambda=Lambda.begin();

	while(p_Lambda!=Lambda.end()){

		int id=(*p_Lambda).id;

		for(int i=1;i<=Num_Machine;i++){

			vector<int>::iterator p_find=find(Lambda_i[i].int_vector.begin(),Lambda_i[i].int_vector.end(),id);

			if(p_find!=Lambda_i[i].int_vector.end()){

				M_lambda[id].int_vector.push_back(i);

			}

		}

		p_Lambda++;

	}

}



void AT::show_Lambda_i(){

	//output the data structure Lambda_i to a debug file

	ofstream fout("Debug/debug_Lambda_i.txt");

	fout<<"show Lambda_i ---------------------------------"<<endl;

	for(int i=1;i<=Num_Machine;i++){

		fout<<"i	"<<i<<"	"<<Machine_vector[i-1].name<<"	";

		vii p_int=Lambda_i[i].int_vector.begin();

		while(p_int!=Lambda_i[i].int_vector.end()){

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

	//initialize the data structure M_l

	for(int l=1;l<=Num_Lot;l++){

		vector<tooling_configure>::iterator p_S=S.begin();

		while(p_S!=S.end()){

			int dev=(*p_S).device;

			vector<device_item>::iterator p_d=Devices.begin()+dev-1;

			vector<int>::iterator p_l=find((*p_d).device_lots.begin(),(*p_d).device_lots.end(),l);



			//added by Zhufeng for logpoint, Sep 01, 2011
			int logpoint=(*p_S).logpoint;
			int operation=(*p_S).operation;
			if(lot_logpoint[l]==logpoint && lot_operation[l]==operation)
			{
				//end by Zhufeng

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

				//added by Zhufeng for logpoint
			}//end of if(lot_logpoint[l]==logpoint && lot_operation[l]==operation)
			//end by Zhufeng

			p_S++;

		}

	}

}



void AT::show_M_l(){

	//output the data structure M_l to a debug file

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

	//get the machines that can process lot l, the results are stored in MS

	vector<tooling_configure>::iterator p_S=S.begin();

	while(p_S!=S.end()){

		int dev=(*p_S).device;

		vector<device_item>::iterator p_d=Devices.begin()+dev-1;

		vector<int>::iterator p_l=find((*p_d).device_lots.begin(),(*p_d).device_lots.end(),l);



		//added by Zhufeng for logpoint, Sep 01, 2011
			int logpoint=(*p_S).logpoint;
			int operation=(*p_S).operation;
			if(lot_logpoint[l]==logpoint && lot_operation[l]==operation)
			{
				//end by Zhufeng



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



				//added by Zhufeng
			}//end of if(lot_logpoint[l]==logpoint && lot_operation[l]==operation)
			//end by Zhufeng

		p_S++;

	}

}



void AT::get_L(vector<int> & LS){

	//get the whole lot set

}



void AT::initialize_L_i(){

	//initialize the data structure L_i

	vector<Machine>::iterator p_M=Machine_vector.begin();

	while(p_M!=Machine_vector.end()){

	

		int i=p_M-Machine_vector.begin()+1;

		;

		vector<tooling_configure>::iterator p_S=S.begin();

		while(p_S!=S.end()){

			int temp=(*p_S).temperature;

		

			vector<int>::iterator p_int=find((*p_M).temperature_vector.begin(),(*p_M).temperature_vector.end(),temp);

			if((*p_S).mg==machine_mg[i] && p_int!=(*p_M).temperature_vector.end()){

				

				int device_num=(*p_S).device;

				

				vector<device_item>::iterator p_d=Devices.begin()+device_num-1;

				

				vector<int>::iterator p_l=(*p_d).device_lots.begin();

				while(p_l!=(*p_d).device_lots.end()){



					//Added by Zhufeng for logpoint
                    int l=(*p_l);
					int logpoint=lot_logpoint[l];
					int operation=lot_operation[l];
					if(logpoint==(*p_S).logpoint && operation==(*p_S).operation)
					{
						//end by Zhufeng

						vii p_int=find(L_i[i].int_vector.begin(),L_i[i].int_vector.end(),(*p_l));



						if(p_int==L_i[i].int_vector.end()){



							L_i[i].int_vector.push_back((*p_l));



						}



						//added by Zhufeng
					}
					//end by Zhufeng

					p_l++;

					

				}

				

			}

			p_S++;

		}

		p_M++;

	}

}



void AT::show_L_i(){

	//output the data structure L_i to a debug file

	ofstream fout("Debug/L_i.txt");

	fout<<"show L_i ------------------------------------"<<endl;

	for(int i=1;i<=Num_Machine;i++){

		fout<<"i="<<i<<" processes lots:";

		vii p_int=L_i[i].int_vector.begin();

		while(p_int!=L_i[i].int_vector.end()){

			fout<<LOT[(*p_int)-1]<<",";

			p_int++;

		}

		fout<<endl;

	}

	fout.close();

}



void AT::initialize_L_i_d(){

	//initialize the data structure L_i_d

	for(int i=1;i<=Num_Machine;i++){

		for(int d=1;d<=Num_Device;d++){

			vector<device_item>::iterator p_D=Devices.begin()+d-1;

			vii p_lot=(*p_D).device_lots.begin();

			while(p_lot!=(*p_D).device_lots.end()){

				vii p_int=find(L_i[i].int_vector.begin(),L_i[i].int_vector.end(),(*p_lot));

				if(p_int!=L_i[i].int_vector.end()){

					int pos=i*Max_Num_Device+d;

					//L_i_d[i][d].int_vector.push_back(*p_lot);

					L_i_d[pos].int_vector.push_back(*p_lot);

				}

				p_lot++;

			}

		}

	}

}



void AT::show_L_i_d(){

	//output the data structure L_i_d to a debug file

	ofstream fout("Debug/debug_L_i_d.txt");

	fout<<"show L_i_d ----------------------------------------"<<endl;

	for(int i=1;i<=Num_Machine;i++){

		for(int d=1;d<=Num_Device;d++){

			fout<<"i="<<i<<",d="<<d<<":";

			int pos=i*Max_Num_Device+d;

			vii p_int=L_i_d[pos].int_vector.begin();

			while(p_int!=L_i_d[pos].int_vector.end()){

				fout<<(*p_int)<<",";

				p_int++;

			}

			fout<<endl;

		}

	}

	fout.close();

}



void AT::get_L(vector<int> & LS,int i){

	//get the lots that can be processed by machine i. The results are stored in LS

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



				//Added by Zhufeng for logpoint

				int l=(*p_l);

				int logpoint=lot_logpoint[l];

				int operation=lot_operation[l];

				//cout<<"logpoint="<<logpoint<<",operation="<<operation<<",S.logpoint="<<(*p_S).logpoint<<",S.OPN="<<(*p_S).operation<<endl;



				if(logpoint==(*p_S).logpoint && operation==(*p_S).operation)

				{

					//cout<<"l="<<l<<",to be push back"<<endl;

					//end by Zhufeng

					vii p_int=find(LS.begin(),LS.end(),(*p_l));

					if(p_int==LS.end()){

						LS.push_back((*p_l));

					}

					//added by Zhufeng
				}
				//end by Zhufeng

				p_l++;

			}

		}

		p_S++;

	}

}



void AT::get_L(vector<int> & LS,int i,int device){

	//get the lots that can be processed by machine i and contains the device

	//the results are stored in LS

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



void AT::get_L_i_lambda(vector<int> & LS,int i,int lambda_id){

	//get the lots that can be processed by machine i with setup lambda_id. The results are stored in LS

	LS.clear();

	vii p_l=L_i[i].int_vector.begin();

	while(p_l!=L_i[i].int_vector.end()){

		int l=(*p_l);

		//cout<<"in get_L_i_lambda,i="<<i<<",l="<<l<<",lambda_id="<<lambda_id<<endl;

		vector<tooling_configure> TS;

		get_S_i_l_Lambda(TS,i,l,lambda_id);

		//cout<<"size of S(i,l,lambda)="<<TS.size()<<endl;

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

	//output the data structure Machine_Group to a debug file

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

	//output the data structure Tooling_Families to a debug file

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

	//output the data structure S to a debug file

	ofstream fout("Debug/debug_S.txt");

	fout<<"show tooling configurations (S)-------------"<<endl;

	vector<tooling_configure>::iterator p_S=S.begin();

	while(p_S!=S.end()){

		fout<<"Tooling configuration ID = "<<(*p_S).id<<endl;



		//added by Zhufeng for logpiont, Aug 31, 2011
		fout<<"Logpoint="<<(*p_S).logpoint<<endl;
		fout<<"operation="<<(*p_S).operation<<endl;
		fout<<"PPH="<<(*p_S).PPH<<endl;
		//end by Zhufeng for logpoint



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

	//output the data structure Devices to a debug file

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

	//output the data structure index_vector to a debug file

	ofstream fout("Debug/debug_index_vector.txt");

	fout<<"show index_vector -------------------------------"<<endl;

	vector<index_node>::iterator p_ind=index_vector.begin();

	while(p_ind!=index_vector.end()){

		fout<<"index="<<(*p_ind).index<<",i="<<(*p_ind).i<<",l="<<(*p_ind).l<<",s="<<(*p_ind).s<<",Lambda_id="<<(*p_ind).Lambda_id<<endl;

		p_ind++;

	}

	fout.close();

}



void AT::show_Machine_vector(){

	//output the data structure Machine_vector to a debug file

	ofstream fout("Debug/debug_Machine_vector.txt");

	vector<Machine>::iterator p_M=Machine_vector.begin();

	while(p_M!=Machine_vector.end()){

		fout<<"ID= "<<(*p_M).id<<endl;

		fout<<"	Name= "<<(*p_M).name<<endl;

		fout<<"	Machine Group= "<<(*p_M).mg<<endl;

		//add by zhufeng
		if((*p_M).hasInitialLot)
		{
			fout<<"hasInitial lot, ";
		}else
		{
			fout<<"NoInitiallot, ";
		}

        if((*p_M).hasInitialSetup)
		{
			fout<<"hasInitial setup, ";
		}else
		{
			fout<<"NoInitial setup, ";
		}

		

			if((*p_M).hasInitialToolingConfigure)
		{
			fout<<"hasInitial route, ";
		}else
		{
			fout<<"NoInitial route, ";
		}

        

        fout<<endl;

		//end by Zhufeng

		fout<<"Initial lot_id="<< (*p_M).initial_lot<<" initial_tooling_setup id="<< (*p_M).initial_tooling_setup <<"initial_route_id="<< (*p_M).initial_tooling_configure<<" Temperature: ";

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

	//output the data structure Tooling_vector to a debug file

	ofstream fout("Debug/debug_Tooling_vector.txt");

	vector<Tooling>::iterator p_T=Tooling_vector.begin();

	while(p_T!=Tooling_vector.end()){

		fout<<"	ID= "<<(*p_T).id<<endl;

		fout<<"	Name= "<<(*p_T).name<<endl;

		fout<<"	Tooling Family= "<<(*p_T).tf<<endl;

		fout<<"	Temperature: ";

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

	//output the temperature combination temp_set to a debug file

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

	//output the statistics of the problem to the screen

	cout<<"show the problem statistics --------------"<<endl;

	cout<<"Num_Lot = "<<Num_Lot<<endl;

	//added by Zhufeng
	cout<<"Num_Initial_lots="<<(int)initial_running_lot.size()<<endl;
	//ended by Zhufeng

	cout<<"Num_Device = "<<Num_Device<<endl;

	cout<<"Num_Keydevice = "<<Num_Keydevice<<endl;

	cout<<"Num_Package = "<<Num_Package<<endl;

	cout<<"Num_Temperature = "<<Num_Temperature<<endl;

	cout<<"Num_Machine = "<<Num_Machine<<endl;

	cout<<"Num_Machine_Group = "<<Num_Machine_Group<<endl;

	cout<<"Num_Tooling = "<<Num_Tooling<<endl;

	cout<<"Num_Tooling_Family = "<<Num_Tooling_Family<<endl;

	cout<<"Number of routes = "<<(int)S.size()<<endl;

	//added by Zhufeng
	//output the statistics of the problem to the file statistics.txt in Output
	ofstream fcout("Output/statistics.txt");
	fcout<<"Num_Lot = "<<Num_Lot<<endl;
	fcout<<"Num_Initial_lots="<<(int)initial_running_lot.size()<<endl;
	fcout<<"Num_Device = "<<Num_Device<<endl;
	fcout<<"Num_Keydevice = "<<Num_Keydevice<<endl;
	fcout<<"Num_Package = "<<Num_Package<<endl;
	fcout<<"Num_Temperature = "<<Num_Temperature<<endl;
	fcout<<"Num_Machine = "<<Num_Machine<<endl;
	fcout<<"Num_Machine_Group = "<<Num_Machine_Group<<endl;
	fcout<<"Num_Tooling = "<<Num_Tooling<<endl;
	fcout<<"Num_Tooling_Family = "<<Num_Tooling_Family<<endl;
	fcout<<"Number of routes = "<<(int)S.size()<<endl;
	fcout.close();
	//ended by Zhufeng

}



void AT::show_device_name(){

	//output the data structure device_name to a debug file

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

	//output the data structure lot_name to a debug file

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

	//this is a function to test purpose. It is NOT valid anymore and should not be used anymore!!!

	Num_Machine=5;

	Num_Tooling=10;

	Num_Lot=2;

	Num_Device=4;

	Num_Keydevice=1;

	Num_Package=1;

	Num_Tooling_Family=2;



	for(int i=1;i<=Num_Machine;i++){ H[i]=8;}

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

	//this function is used to get the index of the decision variable x_ils in the list of decision variables

	//However,this function is NOT correct anymore since it is design for previous version

	//DO NOT use this function!!!

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

	//this function is used to get the index of the decision variable x_il in the list of index_vector_no_s

	//However,this function is NOT correct anymore since it is design for test purpose.

	//DO NOT use this function!!!

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



int AT::index4_y(int i,int Lambda_id){

	//this function is used to get the index of the decision variable y_(i,lambda) in the list of index_vector_no_s

	//However,this function is NOT correct anymore since it is design for test purpose.

	//DO NOT use this function!!!

	int index=-1;

	vector<index_node>::iterator p_node=index_vector_no_s.begin();

	while(p_node!=index_vector_no_s.end()){

		if((*p_node).i==i && (*p_node).Lambda_id==Lambda_id){

			index=(*p_node).index;

			break;

		}

		p_node++;

	}

	if(index<0){ cout<<"WRONG y index!!!"<<endl;}

	return index;

}



int AT::index3_x(int i,int l,int s){

	//this function is used to get the index of the decision variable x_ils in the list of first_level_index_vector

	//However,this function is NOT correct anymore since it is design for test purpose.

	//DO NOT use this function!!!

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



int AT::index3_y(int i,int Lambda_id){

	//this function is used to get the index of the decision variable y_(i,lambda) in the list of first_level_index_vector

	//However,this function is NOT correct anymore since it is design for test purpose.

	//DO NOT use this function!!!

	int index=-1;

	vector<index_node>::iterator p_node=first_level_index_vector.begin();

	while(p_node!=first_level_index_vector.end()){

		if((*p_node).i==i && (*p_node).Lambda_id==Lambda_id){

			index=(*p_node).index;

			break;

		}

		p_node++;

	}

	if(index<0){ cout<<"WRONG y index!!!"<<endl; abort();}

	return index;

}



int AT::index2_x(int i,int l,int s){

	//This function is used to get the index of decision variable x_ils in the list of index_vector

	//Please USE this function other than any other index?_x version

	int index=-1;

	int start_pos=index_vector_flag[i];

	vector<index_node>::iterator p_node=index_vector.begin()+start_pos;

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



int AT::index2_y(int i,int Lambda_id){

	//This function is used to get the index of the y_(i,lambda) decision variable in the list of index_vector

	//Please USE this function instead of any other index?_y version

	int index=-1;

	vector<index_node>::iterator p_node=index_vector.begin()+x_limit;

	while(p_node!=index_vector.end()){

		if((*p_node).i==i && (*p_node).Lambda_id==Lambda_id){

			index=(*p_node).index;

			break;

		}

		p_node++;

	}

	if(index<0){ cout<<"WRONG y index!!!"<<endl;}

	return index;

}



int AT::index_y(int i,int l){

	//this function is used to get the index of the decision variable y_il in the list of decision variables

	//However,this function is NOT correct anymore since it is design for previous version

	//DO NOT use this function!!!

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

	//this function is used to build the AT model by CPLEX

	//NOTE; this model involves

	//1: Key devices, key packages and key pin packages

	//2: t_min, t_max, t_(i,lambda)

	//3: number of machines sum(y) in the objective function

	//Thus this model is the latest model,corresponding to the model in file "update AT model6_112109.docx"

	//At this point of time, the body of this function is actually comment out such that no CPLEX data types and functions will be involved during compilation.

	//To activate the part of the code

	//1: remove the "#if #endif" in this function

	//2: uncomment //#include<ilcplex/ilocplex.h>^M

				   //#include<ilconcert/ilolinear.h>^M

	//	 in the file ./include/AT_GRASP.h



	//NOTE: Due to the initial lots, the time horizon H[] has been modified, such that it is possible that some H[i] is NEGATIVE for machine i. This will lead to infeasibility of the model

	//Thus the constraints including H[] should be taken good care of.

#if 0

	cout<<"output the horizon H[]"<<endl;

	for(int i=1;i<=Num_Machine;i++){

		cout<<"i="<<i<<",H="<<H[i]<<endl;

	}

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

            

            //Updated for logpoint by Zhufeng, Aug 31, 2011

			/*int index=index_S_sim_d(sim,d);

			vii p_s=S_sim_d[index].second.int_vector.begin();

			while(p_s!=S_sim_d[index].second.int_vector.end()){*/



				int index=index_S_sim_l(sim,l);

				vii p_s=S_sim_l[index].second.int_vector.begin();

				while(p_s!=S_sim_l[index].second.int_vector.end()){

					//end by Zhufeng for logpiont, Aug 31, 2011



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

		vii p_Lambda=Lambda_i[i].int_vector.begin();

		while(p_Lambda!=Lambda_i[i].int_vector.end()){

			int Lambda_id=(*p_Lambda);

			string str="y";

			stringstream ss;

			ss<<"("<<i<<")("<<Lambda_id<<")";

			str+=ss.str();

			char name[20];

			strcpy(name,str.c_str());

			y_vector.push_back(IloNumVar(env,0.0,1.0,name));

			num_var++;

			p_Lambda++;

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

		vii p_Lambda=Lambda_i[i].int_vector.begin();

		while(p_Lambda!=Lambda_i[i].int_vector.end()){

			int Lambda_id=(*p_Lambda);

			string str="t";

			stringstream ss;

			ss<<"("<<i<<")("<<Lambda_id<<")";

			str+=ss.str();

			char name[20];

			strcpy(name,str.c_str());

			t_vector.push_back(IloNumVar(env,0,IloInfinity,name));

			num_var++;

			p_Lambda++;

		}

	}

	string str="t_max";

	char name[20];

	strcpy(name,str.c_str());

	t_max=IloNumVar(env,0,IloInfinity,name);

	str="t_min";

	strcpy(name,str.c_str());

	t_min=IloNumVar(env,0,IloInfinity,name);

	//-------------------------------------------------------------------

	double eps_M=1E+6,eps_W=0.0;

	for(int l=1;l<=Num_Lot;l++){

		if(w_l[l]>0 && w_l[l]<eps_M){ eps_M=w_l[l];} //get the minimum positive lot weight

	}

	double temp=1E-06;

	for(int i=1;i<=Num_Machine;i++){

		if(H[i]>temp){ temp=H[i];} //get the maximum time horizon

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



			//Updated by Zhufeng for logpint, Aug 31, 2011

			/*int index=index_S_sim_d(sim,d);

			vii p_s=S_sim_d[index].second.int_vector.begin();

			while(p_s!=S_sim_d[index].second.int_vector.end()){*/



			int index=index_S_sim_l(sim,l);

			vii p_s=S_sim_l[index].second.int_vector.begin();

			while(p_s!=S_sim_l[index].second.int_vector.end()){

			//end by Zhufeng



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

		vii p_Lambda=Lambda_i[i].int_vector.begin();

		while(p_Lambda!=Lambda_i[i].int_vector.end()){

			int Lambda_id=(*p_Lambda);

			vector<IloNumVar>::iterator p_y=y_vector.begin()+index2_y(i,Lambda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1; //use the index of y

			expr-=eps_M*(*p_y);

			p_Lambda++;

		}

	}

	expr-=eps_W*t_max;

	expr+=eps_W*t_min;

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



			//Updated by Zhufeng for logpoint, Aug 31, 2011

			/*int index=index_S_sim_d(sim,d);

			vii p_s=S_sim_d[index].second.int_vector.begin();

			while(p_s!=S_sim_d[index].second.int_vector.end()){*/



			int index=index_S_sim_l(sim,l);

			vii p_s=S_sim_l[index].second.int_vector.begin();

			while(p_s!=S_sim_l[index].second.int_vector.end()){

			//end by Zhufeng for logpoint



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

			vector<int>::iterator p_Lambda=Lambda_i[i].int_vector.begin();

			while(p_Lambda!=Lambda_i[i].int_vector.end()){

				int Lambda_id=(*p_Lambda);

				//int index=index_L_sim_lambda(machine_sim[i],Lambda_id);

				//vector<int>::iterator p_L=L_sim_lambda[index].second.int_vector.begin();

				//while(p_L!=L_sim_lambda[index].second.int_vector.end()){

				vector<int>::iterator p_L=L_i[i].int_vector.begin();

				while(p_L!=L_i[i].int_vector.end()){

					int l=(*p_L);

					vector<tooling_configure> TS;

					get_S_i_l_Lambda(TS,i,l,Lambda_id);

					vector<tooling_configure>::iterator p_TS=TS.begin();

					while(p_TS!=TS.end()){

						IloExpr expr(env);

						int s=(*p_TS).id;

						vector<IloNumVar>::iterator p_x=x_vector.begin()+index2_x(i,l,s);

						vector<IloNumVar>::iterator p_y=y_vector.begin()+index2_y(i,Lambda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;

						expr=(*p_y)-(*p_x);

						con.add(expr>=0);

						expr.end();

						num_con++;

						string str="(";

						stringstream ss;

						ss<<"i@"<<i<<",Lambda@"<<Lambda_id<<",l@"<<l<<",s@"<<s<<")";

						str+=ss.str();

						char name[100];

						strcpy(name,str.c_str());

						con[num_con-1].setName(name);

						p_TS++;

					}

					p_L++;

				}

				p_Lambda++;

			}

		}

	}



	/*

	for(int i=1;i<=Num_Machine;i++){

		vector<int>::iterator p_Lambda=Lambda_i[i].int_vector.begin();

		while(p_Lambda!=Lambda_i[i].int_vector.end()){

			int Lambda_id=(*p_Lambda);

			int counter=0;

			IloExpr expr(env);

			vector<IloNumVar>::iterator p_y=y_vector.begin()+index2_y(i,Lambda_id)-x_limit-Num_Keydevice-Num_Package-1;

			vector<int>::iterator p_L=L_i[i].int_vector.begin();

			while(p_L!=L_i[i].int_vector.end()){

				int l=(*p_L);

				vector<tooling_configure> TS;

				get_S_i_l_Lambda(TS,i,l,Lambda_id);

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

			ss<<"i@"<<i<<",Lambda@"<<Lambda_id<<")";

			str+=ss.str();

			char name[100];

			strcpy(name,str.c_str());

			con[num_con-1].setName(name);

			p_Lambda++;

		}

	}

	*/



	cout<<"build constraints (1d)"<<endl;

	for(int i=1;i<=Num_Machine;i++){

		IloExpr expr(env);

		vector<int>::iterator p_Lambda=Lambda_i[i].int_vector.begin();

		while(p_Lambda!=Lambda_i[i].int_vector.end()){

			int Lambda_id=(* p_Lambda);

			vector<IloNumVar>::iterator p_y=y_vector.begin()+index2_y(i,Lambda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;

			expr+=(*p_y);

			p_Lambda++;

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

					vector<int> Lambda_i_tf_tau;

					get_Lambda(Lambda_i_tf_tau,i,tf,tau);

					vii p_Lambda=Lambda_i_tf_tau.begin();

					while(p_Lambda!=Lambda_i_tf_tau.end()){

						int Lambda_id=(*p_Lambda);

						vector<Lambda_item>::iterator pL=Lambda.begin()+Lambda_id-1;

						vii p_find=find((*pL).tooling_vec.begin(),(*pL).tooling_vec.end(),tf);

						int offset=p_find-(*pL).tooling_vec.begin();

						vii pb=(*pL).b.begin()+offset;

						vector<IloNumVar>::iterator p_y=y_vector.begin()+index2_y(i,Lambda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;

						expr+=(*pb)*(*p_y);

						p_Lambda++;

					}//lambda_iteration

					p_int++;

				}//tau_iteration

			}//i_iteration

			vector<t_set> inter_set;

			get_inter_set((*p_set),inter_set);

			vector<t_set>::iterator pm=inter_set.begin();

			int sum=0;

			while(pm!=inter_set.end()){

				int set_id=get_set_id(*pm);

				int pos=tf*Max_Num_Temperature+set_id;

				//sum+=n_t_n_tooling[tf][set_id];

				sum+=n_t_n_tooling[pos];

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



	if(new_old_model==2){//use the old model

		cout<<"build constraints (1f)"<<endl;

		for(int i=1;i<=Num_Machine;i++){

			IloExpr expr(env);

			vector<int>::iterator p_L=L_i[i].int_vector.begin();

			while(p_L!=L_i[i].int_vector.end()){

				int l=(*p_L);

				int sim=machine_sim[i];

				int d=lots_device[l];



				//Updated by Zhufeng for logpoint, Aug 31, 2011



				/*int index=index_S_sim_d(sim,d);

				vii p_s=S_sim_d[index].second.int_vector.begin();

				while(p_s!=S_sim_d[index].second.int_vector.end()){*/



				int index=index_S_sim_l(sim,l);

				vii p_s=S_sim_l[index].second.int_vector.begin();

				while(p_s!=S_sim_l[index].second.int_vector.end()){

				//end by Zhufeng for logpoint



					int s=(*p_s);

					vector<IloNumVar>::iterator p_x=x_vector.begin()+index2_x(i,l,s);

					expr+=(n_l_chips[l]/rate[s]+Load_Unload_Time)*(*p_x);

					p_s++;

				}

				p_L++;

			}

			if(H[i]>1E-06){

				con.add(expr<=H[i]);

			}else{//consider H[i] to be negative

				con.add(expr<=0.0);

			}

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

		//use the new model

		cout<<"build updated constraints (1f)"<<endl;

		for(int i=1;i<=Num_Machine;i++){

			vector<int>::iterator p_Lambda=Lambda_i[i].int_vector.begin();

			while(p_Lambda!=Lambda_i[i].int_vector.end()){

				int Lambda_id=(*p_Lambda);

				IloExpr expr(env);

				int index=index_L_sim_lambda(machine_sim[i],Lambda_id);

				vector<int>::iterator p_L=L_sim_lambda[index].second.int_vector.begin();

				while(p_L!=L_sim_lambda[index].second.int_vector.end()){

					int l=(*p_L);

					vector<tooling_configure> TS;

					get_S_i_l_Lambda(TS,i,l,Lambda_id);

					vector<tooling_configure>::iterator p_TS=TS.begin();

					while(p_TS!=TS.end()){

						int s=(*p_TS).id;

						vector<IloNumVar>::iterator p_x=x_vector.begin()+index2_x(i,l,s);

						expr+=(n_l_chips[l]/rate[s]+Load_Unload_Time)*(*p_x);

						p_TS++;

					}

					p_L++;

				}

				vector<IloNumVar>::iterator p_t=t_vector.begin()+index2_y(i,Lambda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;

				expr-=(*p_t);

				con.add(expr==0);

				num_con++;

				expr.end();

				string str="(";

				stringstream ss;

				ss<<"i@"<<i<<",Lambda@"<<Lambda_id<<")";

				str+=ss.str();

				char name[100];

				strcpy(name,str.c_str());

				con[num_con-1].setName(name);

				//cout<<"i="<<i<<endl;

				p_Lambda++;

			}

		}

		for(int i=1;i<=Num_Machine;i++){

			vector<int>::iterator p_Lambda=Lambda_i[i].int_vector.begin();

			while(p_Lambda!=Lambda_i[i].int_vector.end()){

				int Lambda_id=(*p_Lambda);

				IloExpr expr(env);	

				vector<IloNumVar>::iterator p_t=t_vector.begin()+index2_y(i,Lambda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;

				vector<IloNumVar>::iterator p_y=y_vector.begin()+index2_y(i,Lambda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;

				if(H[i]>1E-06){

					expr+=(*p_t)-H[i]*(*p_y);

				}else{//negative H[i] is considered to be 0

					expr+=(*p_t);

				}

				con.add(expr<=0);

				num_con++;

				expr.end();

				string str="t_y(";

				stringstream ss;

				ss<<"i@"<<i<<",Lambda@"<<Lambda_id<<")";

				str+=ss.str();

				char name[100];

				strcpy(name,str.c_str());

				con[num_con-1].setName(name);

				//cout<<"i="<<i<<endl;

				p_Lambda++;

			}

		}

		for(int i=1;i<=Num_Machine;i++){

			vector<int>::iterator p_Lambda=Lambda_i[i].int_vector.begin();

			while(p_Lambda!=Lambda_i[i].int_vector.end()){

				int Lambda_id=(*p_Lambda);

				IloExpr expr(env);	

				vector<IloNumVar>::iterator p_t=t_vector.begin()+index2_y(i,Lambda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;

				expr+=t_max-(*p_t);

				con.add(expr>=0);

				num_con++;

				expr.end();

				string str="t_max_con(";

				stringstream ss;

				ss<<"i@"<<i<<",Lambda@"<<Lambda_id<<")";

				str+=ss.str();

				char name[100];

				strcpy(name,str.c_str());

				con[num_con-1].setName(name);

				//cout<<"i="<<i<<endl;

				p_Lambda++;

			}

		}



		for(int i=1;i<=Num_Machine;i++){

			vector<int>::iterator p_Lambda=Lambda_i[i].int_vector.begin();

			while(p_Lambda!=Lambda_i[i].int_vector.end()){

				int Lambda_id=(*p_Lambda);

				IloExpr expr(env);	

				vector<IloNumVar>::iterator p_t=t_vector.begin()+index2_y(i,Lambda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;

				vector<IloNumVar>::iterator p_y=y_vector.begin()+index2_y(i,Lambda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;

				if(H[i]>1E-06){

					expr+=t_min-(*p_t)-H[i]*(1-(*p_y));

				}else{//negative H[i] is considered to be 0

					expr+=t_min-(*p_t);

				}

				con.add(expr<=0);

				num_con++;

				expr.end();

				string str="t_min_con(";

				stringstream ss;

				ss<<"i@"<<i<<",Lambda@"<<Lambda_id<<")";

				str+=ss.str();

				char name[100];

				strcpy(name,str.c_str());

				con[num_con-1].setName(name);

				//cout<<"i="<<i<<endl;

				p_Lambda++;

			}

		}

	}//end_if



	cout<<"build constraints (1g)"<<endl;

	for(int k=1;k<=Num_Keydevice;k++){

		vector<int>::iterator p_int=Key_Devices.begin()+k-1;

		int d=(*p_int);

		IloExpr expr(env);

		for(int i=1;i<=Num_Machine;i++){

			int pos=i*Max_Num_Device+d;

			vector<int>::iterator p_L=L_i_d[pos].int_vector.begin();

			while(p_L!=L_i_d[pos].int_vector.end()){

				int l=(*p_L);

				int sim=machine_sim[i];



				//Updated by Zhufeng for logpoint, Aug 31, 2011



				/*int index=index_S_sim_d(sim,d);

				vii p_s=S_sim_d[index].second.int_vector.begin();

				while(p_s!=S_sim_d[index].second.int_vector.end()){*/



				int index=index_S_sim_l(sim,l);

				vii p_s=S_sim_l[index].second.int_vector.begin();

				while(p_s!=S_sim_l[index].second.int_vector.end()){

					//end by Zhufeng for logpiont



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

				int pos=i*Max_Num_Device+d;

				vector<int>::iterator p_L=L_i_d[pos].int_vector.begin();

				while(p_L!=L_i_d[pos].int_vector.end()){

					int l=(*p_L);

					int sim=machine_sim[i];



					//Updated by Zhufeng for logpoint, Aug 31, 2011



					/*int index=index_S_sim_d(sim,d);

					vii p_s=S_sim_d[index].second.int_vector.begin();

					while(p_s!=S_sim_d[index].second.int_vector.end()){*/



					int index=index_S_sim_l(sim,l);

					vii p_s=S_sim_l[index].second.int_vector.begin();

					while(p_s!=S_sim_l[index].second.int_vector.end()){

						//end by Zhufeng for logpoint




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

				int pos=i*Max_Num_Device+d;

				vector<int>::iterator p_L=L_i_d[pos].int_vector.begin();

				while(p_L!=L_i_d[pos].int_vector.end()){

					int l=(*p_L);

					int sim=machine_sim[i];



					//Updated by Zhufeng for logpiont, Aug 31, 2011



					/*int index=index_S_sim_d(sim,d);

					vii p_s=S_sim_d[index].second.int_vector.begin();

					while(p_s!=S_sim_d[index].second.int_vector.end()){*/



					int index=index_S_sim_l(sim,l);

					vii p_s=S_sim_l[index].second.int_vector.begin();

					while(p_s!=S_sim_l[index].second.int_vector.end()){

						//end by Zhufeng for logpoint




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

	for(int it=1;it<=(int)Lambda.size();it++){

		vector<Lambda_item>::iterator p_Lambda=Lambda.begin()+it-1;

		//-----------------

		int sum_b=0;

		vector<int>::iterator p_int=(*p_Lambda).b.begin();

		while(p_int!=(*p_Lambda).b.end()){

			sum_b+=(*p_int);

			p_int++;

		}

		//---------------------------

		if(sum_b>0){//when the tooling list is not empty

			int Lambda_id=(*p_Lambda).id;

			//-------------------------------------temperature sest

			int temperature=(*p_Lambda).temperature;

			struct t_set one_set;

			one_set.set_elements.push_back(temperature);

			vector<t_set> inter_set;

			get_inter_set(one_set,inter_set);

			//---------------------------------------------------------------

			IloExpr expr(env);

			vector<int>::iterator p_M=M_lambda[Lambda_id].int_vector.begin();

			while(p_M!=M_lambda[Lambda_id].int_vector.end()){

				int i=(*p_M);

				//cout<<"i="<<i<<",Lambda="<<Lambda_id<<","<<index2_y(i,Lambda_id)<<endl;

				vector<IloNumVar>::iterator p_y=y_vector.begin()+index2_y(i,Lambda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;

				expr+=(*p_y);

				p_M++;

			}

			double min_num=1E+8;

			vector<int>::iterator p_tool=(*p_Lambda).tooling_vec.begin();

			while(p_tool!=(*p_Lambda).tooling_vec.end()){

				int offset=p_tool-(*p_Lambda).tooling_vec.begin();

				vector<int>::iterator p_b=(*p_Lambda).b.begin()+offset;

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

			ss<<"Lambda@"<<Lambda_id<<")";

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

	cplex.exportModel("AT_GRASP.lp"); //export the model

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

		vector<int>::iterator p_Lambda=Lambda_i[i].int_vector.begin();

		while(p_Lambda!=Lambda_i[i].int_vector.end()){

			int Lambda_id=(*p_Lambda);

			vector<IloNumVar>::iterator p_y=y_vector.begin()+index2_y(i,Lambda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;

			double value=cplex.getValue(*p_y);

			if(value>0.99){ value=1.0;}

			else{value=0.0;}

			y_value.push_back(value);

			p_Lambda++;

		}

	}

	//---------------------------------------------modify the problem for balancing

	bool AT_balancing=false;

	if(AT_balancing){

		//modify the objective function

		expr.clear();

		//for(int id=1;id<=(int)t_vector.size();id++){

		//	expr+=t_vector[id-1]*t_vector[id-1];

		//}

		expr+=t_max;

		obj.setExpr(expr);

		obj.setSense(IloObjective::Minimize);

		expr.clear();

		//fix y_i lambda

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

	}

	//---------------------------------------------prepare the results

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

	//This is a function for test purpose

	//DO NOT use this function any more. It is out of date.

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

		vii p_Lambda=Lambda_i[i].int_vector.begin();

		while(p_Lambda!=Lambda_i[i].int_vector.end()){

			int Lambda_id=(*p_Lambda);

			string str="y";

			stringstream ss;

			ss<<"("<<i<<")("<<Lambda_id<<")";

			str+=ss.str();

			char name[20];

			strcpy(name,str.c_str());

			y_vector.push_back(IloNumVar(env,0.0,1.0,name));

			num_var++;

			p_Lambda++;

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

		vector<int>::iterator p_Lambda=Lambda_i[i].int_vector.begin();

		while(p_Lambda!=Lambda_i[i].int_vector.end()){

			int Lambda_id=(*p_Lambda);

			int index=index_L_sim_lambda(machine_sim[i],Lambda_id);

			vector<int>::iterator p_L=L_sim_lambda[index].second.int_vector.begin();

			while(p_L!=L_sim_lambda[index].second.int_vector.end()){

				int l=(*p_L);

				IloExpr expr(env);

				vector<IloNumVar>::iterator p_x=x_vector.begin()+index4_x(i,l);

				vector<IloNumVar>::iterator p_y=y_vector.begin()+index4_y(i,Lambda_id)-x_limit_4-Num_Keydevice-Num_Package-1;

				expr=(*p_y)-(*p_x);

				con.add(expr>=0);

				expr.end();

				num_con++;

				string str="(";

				stringstream ss;

				ss<<"i@"<<i<<",Lambda@"<<Lambda_id<<",l@"<<l<<")";

				str+=ss.str();

				char name[100];

				strcpy(name,str.c_str());

				con[num_con-1].setName(name);

				p_L++;

			}

			p_Lambda++;

		}

	}

*/

	cout<<"build constraints (1d)"<<endl;

	for(int i=1;i<=Num_Machine;i++){

		IloExpr expr(env);

		vector<int>::iterator p_Lambda=Lambda_i[i].int_vector.begin();

		while(p_Lambda!=Lambda_i[i].int_vector.end()){

			int Lambda_id=(* p_Lambda);

			vector<IloNumVar>::iterator p_y=y_vector.begin()+index4_y(i,Lambda_id)-x_limit_4-Num_Keydevice-Num_Package-1;

			expr+=(*p_y);

			p_Lambda++;

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

					vector<int> Lambda_i_tf_tau;

					get_Lambda(Lambda_i_tf_tau,i,tf,tau);

					vii p_Lambda=Lambda_i_tf_tau.begin();

					while(p_Lambda!=Lambda_i_tf_tau.end()){

						int Lambda_id=(*p_Lambda);

						vector<Lambda_item>::iterator pL=Lambda.begin()+Lambda_id-1;

						vii p_find=find((*pL).tooling_vec.begin(),(*pL).tooling_vec.end(),tf);

						int offset=p_find-(*pL).tooling_vec.begin();

						vii pb=(*pL).b.begin()+offset;

						vector<IloNumVar>::iterator p_y=y_vector.begin()+index4_y(i,Lambda_id)-x_limit_4-Num_Keydevice-Num_Package-1;

						expr+=(*pb)*(*p_y);

						p_Lambda++;

					}//lambda_iteration

					p_int++;

				}//tau_iteration

			}//i_iteration

			vector<t_set> inter_set;

			get_inter_set((*p_set),inter_set);

			vector<t_set>::iterator pm=inter_set.begin();

			int sum=0;

			while(pm!=inter_set.end()){

				int set_id=get_set_id(*pm);

				int pos=tf*Max_Num_Temperature+set_id;

				//sum+=n_t_n_tooling[tf][set_id];

				sum+=n_t_n_tooling[pos];

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

		vector<int>::iterator p_Lambda=Lambda_i[i].int_vector.begin();

		while(p_Lambda!=Lambda_i[i].int_vector.end()){

			int Lambda_id=(*p_Lambda);

			IloExpr expr(env);

			int index=index_L_sim_lambda(machine_sim[i],Lambda_id);

			vector<int>::iterator p_L=L_sim_lambda[index].second.int_vector.begin();

			while(p_L!=L_sim_lambda[index].second.int_vector.end()){

				int l=(*p_L);

				int s=max_rate_s(i,l,Lambda_id);

				vector<IloNumVar>::iterator p_x=x_vector.begin()+index4_x(i,l);

				expr+=(n_l_chips[l]/rate[s]+Load_Unload_Time)*(*p_x);

				p_L++;

			}

			vector<IloNumVar>::iterator p_y=y_vector.begin()+index4_y(i,Lambda_id)-x_limit_4-Num_Keydevice-Num_Package-1;

			expr-=H[i]*(*p_y);

			con.add(expr<=0);

			num_con++;

			expr.end();

			string str="(";

			stringstream ss;

			ss<<"i@"<<i<<"Lambda@"<<Lambda_id<<")";

			str+=ss.str();

			char name[100];

			strcpy(name,str.c_str());

			con[num_con-1].setName(name);

			//cout<<"i="<<i<<",Lambda_id="<<Lambda_id<<endl;

			p_Lambda++;

		}

	}



	cout<<"build constraints (1g)"<<endl;

	for(int k=1;k<=Num_Keydevice;k++){

		vector<int>::iterator p_int=Key_Devices.begin()+k-1;

		int d=(*p_int);

		IloExpr expr(env);

		for(int i=1;i<=Num_Machine;i++){

			int pos=i*Max_Num_Device+d;

			vector<int>::iterator p_L=L_i_d[pos].int_vector.begin();

			while(p_L!=L_i_d[pos].int_vector.end()){

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

			int pos=i*Max_Num_Device+d;

			vector<int>::iterator p_L=L_i_d[pos].int_vector.begin();

			while(p_L!=L_i_d[pos].int_vector.end()){

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

	for(int it=1;it<=(int)Lambda.size();it++){

		vector<Lambda_item>::iterator p_Lambda=Lambda.begin()+it-1;

		//-----------------

		int sum_b=0;

		vector<int>::iterator p_int=(*p_Lambda).b.begin();

		while(p_int!=(*p_Lambda).b.end()){

			sum_b+=(*p_int);

			p_int++;

		}

		//---------------------------

		if(sum_b>0){//when the tooling list is not empty

			int Lambda_id=(*p_Lambda).id;

			//-------------------------------------temperature sest

			int temperature=(*p_Lambda).temperature;

			struct t_set one_set;

			one_set.set_elements.push_back(temperature);

			vector<t_set> inter_set;

			get_inter_set(one_set,inter_set);

			//---------------------------------------------------------------

			IloExpr expr(env);

			vector<int>::iterator p_M=M_lambda[Lambda_id].int_vector.begin();

			while(p_M!=M_lambda[Lambda_id].int_vector.end()){

				int i=(*p_M);

				//cout<<"i="<<i<<",Lambda="<<Lambda_id<<","<<index4_y(i,Lambda_id)<<endl;

				vector<IloNumVar>::iterator p_y=y_vector.begin()+index4_y(i,Lambda_id)-x_limit_4-Num_Keydevice-Num_Package-1;

				expr+=(*p_y);

				p_M++;

			}

			double min_num=1E+8;

			vector<int>::iterator p_tool=(*p_Lambda).tooling_vec.begin();

			while(p_tool!=(*p_Lambda).tooling_vec.end()){

				int offset=p_tool-(*p_Lambda).tooling_vec.begin();

				vector<int>::iterator p_b=(*p_Lambda).b.begin()+offset;

				int tf=(*p_tool);

				int require_num=(*p_b);

				int available_num=0;

				vector<t_set>::iterator pm=inter_set.begin();

				while(pm!=inter_set.end()){

					int set_id=get_set_id(*pm);

					int pos=tf*Max_Num_Temperature+set_id;

					//available_num+=n_t_n_tooling[tf][set_id];

					available_num+=n_t_n_tooling[pos];

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

			ss<<"Lambda@"<<Lambda_id<<")";

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

	cplex.exportModel("AT_GRASP_no_s.lp");

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

	//this function is used to build the data structure index_vector

	//this vector is going to be used in function index2_x() and index2_y

	int counter=0;

	for(int i=1;i<=Num_Machine;i++){

		index_vector_flag[i]=counter; //keep a record of the flags where i starts

		vector<int> L;

		get_L(L,i);

		vector<int>::iterator p_L=L.begin();

		while(p_L!=L.end()){

			int l=(*p_L);

			int sim=machine_sim[i];

			int d=lots_device[l];



			//Updated by Zhufeng for logpoint, Aug 31, 2011

			/*int index=index_S_sim_d(sim,d);

			vii p_s=S_sim_d[index].second.int_vector.begin();

			while(p_s!=S_sim_d[index].second.int_vector.end()){*/



			int index=index_S_sim_l(sim,l);

			vii p_s=S_sim_l[index].second.int_vector.begin();

			while(p_s!=S_sim_l[index].second.int_vector.end()){

				//end by Zhufeng for logpoint



				int s=(*p_s);

				struct index_node one_node;

				one_node.i=i;

				one_node.l=l;

				one_node.s=s;

				one_node.Lambda_id=-1;

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

		one_node.Lambda_id=-1;

		counter++;

		index_vector.push_back(one_node);

	}

	for(int p=1;p<=Num_Package;p++){

		struct index_node one_node;

		one_node.index=counter;

		one_node.i=-1;

		one_node.l=-1;

		one_node.s=-1;

		one_node.Lambda_id=-1;

		counter++;

		index_vector.push_back(one_node);

	}

	for(int kp=1;kp<=(int)Key_PKG.size();kp++){

		struct index_node one_node;

		one_node.index=counter;

		one_node.i=-1;

		one_node.l=-1;

		one_node.s=-1;

		one_node.Lambda_id=-1;

		counter++;

		index_vector.push_back(one_node);

	}

	for(int kpp=1;kpp<=(int)Key_PIN_PKG.size();kpp++){

		struct index_node one_node;

		one_node.index=counter;

		one_node.i=-1;

		one_node.l=-1;

		one_node.s=-1;

		one_node.Lambda_id=-1;

		counter++;

		index_vector.push_back(one_node);

	}

	for(int i=1;i<=Num_Machine;i++){

		vii p_int=Lambda_i[i].int_vector.begin();

		while(p_int!=Lambda_i[i].int_vector.end()){

			struct index_node one_node;

			one_node.i=i;

			one_node.index=counter;

			one_node.l=-1;

			one_node.s=-1;

			one_node.Lambda_id=(*p_int);

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

	//This function is used to build the data structure first_level_index_vector

	//However, it is NOT USED anymore

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



				//Updated by Zhufeng for logpint, Aug 31, 2011



				/*int index=index_S_sim_d(sim,d);

				vii p_s=S_sim_d[index].second.int_vector.begin();

				while(p_s!=S_sim_d[index].second.int_vector.end()){*/



				int index=index_S_sim_l(sim,l);

				vii p_s=S_sim_l[index].second.int_vector.begin();

				while(p_s!=S_sim_l[index].second.int_vector.end()){

				//end by Zhufeng



					int s=(*p_s);

					struct index_node one_node;

					one_node.i=i;

					one_node.l=l;

					one_node.s=s;

					one_node.Lambda_id=-1;

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

		one_node.Lambda_id=-1;

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

		one_node.Lambda_id=-1;

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

		one_node.Lambda_id=-1;

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

		one_node.Lambda_id=-1;

		counter++;

		fout<<"i="<<one_node.i<<",l="<<one_node.l<<",s="<<one_node.s<<",index="<<one_node.index<<endl;

		first_level_index_vector.push_back(one_node);

	}

	for(int i=1;i<=Num_Machine;i++){

		vii p_int=Lambda_i[i].int_vector.begin();

		while(p_int!=Lambda_i[i].int_vector.end()){

			struct index_node one_node;

			one_node.i=i;

			one_node.index=counter;

			one_node.l=-1;

			one_node.s=-1;

			one_node.Lambda_id=(*p_int);

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

	//this function is used to build the data structure index_vector_no_s

	//it is for test purpose

	//This function is NOT USED anymore

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

			one_node.Lambda_id=-1;

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

		one_node.Lambda_id=-1;

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

		one_node.Lambda_id=-1;

		counter++;

		fout<<"i="<<one_node.i<<",l="<<one_node.l<<",s="<<one_node.s<<",index="<<one_node.index<<endl;

		index_vector_no_s.push_back(one_node);

	}

	for(int i=1;i<=Num_Machine;i++){

		vii p_int=Lambda_i[i].int_vector.begin();

		while(p_int!=Lambda_i[i].int_vector.end()){

			struct index_node one_node;

			one_node.i=i;

			one_node.index=counter;

			one_node.l=-1;

			one_node.s=-1;

			one_node.Lambda_id=(*p_int);

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

	//this function is used to generate random test cases

	//it was used before to write the paper

	//However, this function is NOT VALID anymore since the AT model has been updated

	//DO NOT used this function anymore

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

	cout<<"setup routings S,Lambda"<<endl;

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



	double Lambda_ratio=0.5;

	int num_Lambda=roundup((int)S.size()*Lambda_ratio);

	vector<Lambda_item> temp_Lambda;

	for(int Lambda_id=1;Lambda_id<=num_Lambda;Lambda_id++){

		struct Lambda_item one_item;

		one_item.id=Lambda_id;

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

		temp_Lambda.push_back(one_item);

	}



	vector<tooling_configure>::iterator p_S=S.begin();

	while(p_S!=S.end()){

		int Lambda_selected=0;

		while(Lambda_selected<1){ Lambda_selected=roundup(rand()*1.0/RAND_MAX*num_Lambda);}

		vector<Lambda_item>::iterator p_Lambda=temp_Lambda.begin()+Lambda_selected-1;

		(*p_S).tf.tools.resize((int)(*p_Lambda).tooling_vec.size());

		copy((*p_Lambda).tooling_vec.begin(),(*p_Lambda).tooling_vec.end(),(*p_S).tf.tools.begin());

		//setup a_st[][]

		vii p_tf=(*p_S).tf.tools.begin();

		while(p_tf!=(*p_S).tf.tools.end()){

			int tf=(*p_tf);

			int s=(*p_S).id;

			int offset=p_tf-(*p_S).tf.tools.begin();

			vii pb=(*p_Lambda).b.begin()+offset;

			int pos=s*Max_Num_Tooling_Family+tf;

			//a_st[s][tf]=(*pb);

			a_st[pos]=(*pb);

			p_tf++;

		}

		(*p_S).temperature=(*p_Lambda).temperature;

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

			int pos=tf*Max_Num_Temperature+n;

			//n_t_n_tooling[tf][n]=0;

			n_t_n_tooling[pos]=0;

			vector<int>::iterator pt=Tooling_Families[tf-1].tools.begin();

			while(pt!=Tooling_Families[tf-1].tools.end()){

				int tool=(*pt);

				bool com=compare_int_vec(Tooling_vector[tool-1].temperature_vector,Temperature_Set[n-1].set_elements);

				if(com){

					//n_t_n_tooling[tf][n]++;

					n_t_n_tooling[pos]++;

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

	get_Lambda();

	show_Lambda();

	initialize_Lambda_i();

	show_Lambda_i();



	//Added by Zhufeng, for not overtake initial setup and flexible initial setup.Feb 26, 2012

	//initialize_Lambda_i2();

	//show_Lambda_i2();

	//end by Zhufeng, Feb 26, 2012

	initialize_M_lambda();

	//step,setup the model

	//cout<<"get SIM"<<endl;

	get_SIM();

    //comment by Zhufeng for logoint, Aug 31, 2011

	//initialize_S_sim_d();

	//end by Zhufeng

	get_tooling_sum_rhs();

	initialize_L_sim_lambda();

	initialize_sim_lambda_L();

	initialize_sil();



	//comment by Zhufeng for logoint, Aug 31, 2011

	//initialize_S_sim_d_lambda();

	//end by Zhufeng for logpoint



	index_vector.clear();

	build_index_vector();

	//build_index_vector_no_s();



	show_L_sim_lambda();

	show_sim_lambda_L();

	show_SIM();

	show_machine_sim();

	show_index_vector();

	show_sil();

    

	//comment by Zhufeng for logoint, Aug 31, 2011

	//show_S_sim_d();

	//show_S_sim_d_lambda();

	//end by Zhufeng

    

	//Added by Zhufeng for logpoint in Sep 01,2011. Note it has to be after initialize_L_sim_lambda();
	initialize_S_sim_l_lambda();
	show_S_sim_l_lambda();
	//end by Zhufeng

	

	get_LPD();

	get_sum_weight();

	if(Run_Option){

		build_model();

	}

	show_PIN_PKG();

	//----------------------------



	first_level_heuristic();

	cout<<"Random case:The run has been finished"<<endl;

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

	//randomly select n objects from N objects. However, the objects listed in excluded[] should not be selected

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



void AT::display(vii begin,vii end){//this is a helping function to generate temperature combinations

	struct t_set one_set;

	for (vii it=begin;it!=end;++it){

		one_set.set_elements.push_back(*it);

	}

	Temperature_Set.push_back(one_set);

}



void AT::temp_combination(){//this is a function to generate temperature combinations

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

	//given the current set,generate the supper sets that cover the current set

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

	//given the current set,generate the inter sets that overlap the current set

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

	//given the current set, return the id of the set

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

	//added by Zhufeng

	int length=strlen(porg);

    if (start_id>=length)

	{

		cout<<"length of char[] ="<<length<<", start_id="<<start_id;

		cout<<", wrong start id in the strtok2()!"<<endl;

		pdes[0]='\0';

		return start_id;

	}

	//end by Zhufeng

	

	ch=porg[start_id];

	if(ch=='\0' || ch==delim
	   //Added by Zhufeng, Aug 12, 2012
           ||ch=='\r'
	   //end by Zhufeng,Aug 12, 2012
           ){//if the starting characther is the end or the delim

		pdes[0]='\0'; 

		return start_id+1;

	}else{

		int id=start_id;

		while(ch!='\0' && ch!=delim
		      //Added by Zhufeng, Aug 12, 2012
                      && ch!='\r'
		      //end by Zhufeng, Aug 12, 2012
                     ){

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

	//trim the spaces both from the head and the tail of the given string ch[]

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

	char temp[1000];

	for(i=pfirst; i<=plast; i++){ temp[i-pfirst] = ch[i]; }

	for(i=0; i<=plast-pfirst; i++){ ch[i] = temp[i]; }

	ch[i]='\0';

}



void AT::trim(char * ch, char delim){

	//trim the given character delim from both the head and tail of the given string ch[]

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

	char temp[1000];

	for(i=pfirst; i<=plast; i++){ temp[i-pfirst] = ch[i]; }

	for(i=0; i<=plast-pfirst; i++){ ch[i] = temp[i]; }

	ch[i]='\0';

}



void AT::create_case_from_files(){

	//create the case from input files

	//this is an entry function of the GRASP algorithm

	ofstream f_log("Output/log_error.txt");

	f_log<<"################################################################################################################"<<endl

		<<"# The file recorded all inconsistent input data."<<endl

		<<"# It pointed out where the incomplete data is, guessed what information may be wrong and what reaction was taken."<<endl

		<<"##################################################################################################################"<<endl<<endl;

	f_log.close();

	readFiles();



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

	get_Lambda();

	show_Lambda();



	//------ added by yufen ----

	readInitialSetups();

	//------ end yufen ---------



	initialize_Lambda_i();

	show_Lambda_i();



	initialize_M_lambda();

	

	//step,setup the model

	cout<<"process_initial_lot()"<<endl;

    process_initial_lot(initial_running_lot,H);	

	cout<<"get SIM"<<endl;

	get_SIM();



	//Comment by Zhufeng for logpoint, Aug 31, 2011



 	/*cout<<"initialize_S_sim_d()"<<endl;	

	initialize_S_sim_d();

	show_S_sim_d();*/

	
	//end by Zhufeng for logpoint

	//Added by Zhufeng for logpoint, Aug 31, 2011
	cout<<"initialize_S_sim_l()"<<endl;	
	initialize_S_sim_l();
	show_S_sim_l();
	//end by Zhufeng for logpoint



	cout<<"get_tooling_sum_rhs()"<<endl;	

	get_tooling_sum_rhs();

    cout<<"initialize_L_sim_lambda()"<<endl;

	initialize_L_sim_lambda();

	cout<<"initialize_sim_lambda_L()"<<endl;

	initialize_sim_lambda_L();

	cout<<"initialize_sil()"<<endl;

	initialize_sil();



	/*cout<<"initialize_S_sim_d_lambda()"<<endl;	

	initialize_S_sim_d_lambda();*/



	cout<<"General_Snapshoot_time = "<<General_Snapshoot_time.str<<endl;

	show_initial_running_lot(initial_running_lot);

	index_vector.clear();



	cout<<"build_index_vector"<<endl;

	build_index_vector();

	//build_index_vector_no_s();



	//Added by Zhufeng for logpoint in Spe 01,2011. Note it has to be after initialize_L_sim_lambda();
	cout<<"initialize_S_sim_l_lambda()"<<endl;	
	initialize_S_sim_l_lambda();
	show_S_sim_l_lambda();
	//end by Zhufeng



	//Added by Zhufeng for multipass, Dec 22, 2011

	//May 17, 2012, initialize Max_Num_Step at the same time

	initialize_multipass_vector(Max_Num_Step);

	//May 17, 2012

	

	show_multipass_vector(); // Dec 23,2011



	show_second_logpoint(); // Dec 30,2011



	

	

	cout<<"initialize_L_second_pass_i()"<<endl;

	initialize_L_second_pass_i();//Dec 31, 2011

	show_L_second_pass_i();//Dec 31, 2011



	cout<<"initialize_L_second_pass_sim_lambda()"<<endl;

	initialize_L_second_pass_sim_lambda();//Dec 31, 2011

	show_L_second_pass_sim_lambda();//Dec 31, 2011



	cout<<"initialize_S_sim_l_second_pass()"<<endl;

	initialize_S_sim_l_second_pass();

	show_S_sim_l_second_pass();



	cout<<"initialize_S_sim_l_second_pass_lambda()"<<endl;

	initialize_S_sim_l_second_pass_lambda();

	show_S_sim_l_second_pass_lambda();



    //end by Zhufeng for mulitpass, Dec 31,2011

    

	show_L_sim_lambda();

	

	show_sim_lambda_L();

	

	show_sil();

	

	//show_S_sim_d_lambda();

	

	get_LPD();

	

	get_sum_weight();

	

	

    

	show_PIN_PKG();

	

	show_SIM();

	show_machine_sim();

	

	show_index_vector();



	  //Added by Zhufeng, Dec 20, 2013
	  build_device_pass_from_S();
	  show_device_pass_map();
	  //end by Zhufeng, Dec 20, 2013

	//build_model_no_s();

	if(Run_Option){

		build_model();

	}



	//exit(0);

    

	first_level_heuristic();

    

	cout<<"The run has been finished"<<endl;

	//commented by Zhufeng. in order to show the total running time 

	//exit(0);

	//end by Zhufeng

}



//Added by Zhufeng for multipass, Dec 22, 2011

void AT::initialize_multipass_vector( int & Max_Num_Step)

{

	//This way initilizing mulitpass_vector implies that the logpoint list for a device is listed in route.csv in a ascending order.

	//e.g row 10:7110 for device abc,then 7111 for device abc must be in a row with row number bigger than 10.

	//

	//For a device with id "device_id", multipass_vector[device_id-1] denote the corresponding logpoint list

	//May 22, 2012

	//Initialize Max_Num_step at the same time

	Max_Num_Step = 0;



	for(int i=1;i<=(int)DEV.size();i++)

	{

		struct multipass one_multipass;

		one_multipass.whether_multi=false;

		int num_logpoint=0;  //denote the number of logpoint in the logpoint list for the device

		int dev=i;

		vector<tooling_configure>::iterator p_S=S.begin();

		while(p_S!=S.end()){

			if((*p_S).device==dev)

			{

				int logpoint=(*p_S).logpoint;

				if(num_logpoint==0)

				{

					//The first time add a logpoint

					num_logpoint++;

					one_multipass.logpoint_list.push_back(logpoint);



				}else

				{

					vector<int>::iterator p=find(one_multipass.logpoint_list.begin(),one_multipass.logpoint_list.end(),logpoint);

					if(p==one_multipass.logpoint_list.end())

					{

						num_logpoint++;

						one_multipass.logpoint_list.push_back(logpoint);

					}//end of if(p==one_multipass.logpoint_list.end())

				}

			}//end of if((*p_S).device==dev)

		p_S++;

		}//end of while(p_S!=S.end()){

		if(num_logpoint>1)

		{

			one_multipass.whether_multi=true;

		}

		//May 22. 2012

		if( num_logpoint > Max_Num_Step)

		{

			Max_Num_Step = num_logpoint;

		}



		multipass_vector.push_back(one_multipass);



	}//end of for(int i=1;i<=(int)DEV.size();i++)



	cout<<"max_num_step = "<<Max_Num_Step<<endl;



}



void AT::show_multipass_vector()

{

	ofstream f_multipass("Debug/multipass_vector.txt");

	vector<multipass>::iterator p_m=multipass_vector.begin();

	while(p_m!=multipass_vector.end())

	{

		int dev_id= p_m-multipass_vector.begin()+1;

		f_multipass<<DEV[dev_id-1]<<":";

		if((* p_m).whether_multi)

		{

			f_multipass<<",multipass,";

		}else

		{

			f_multipass<<",only single pass,";

		}



		vii p_log=(* p_m).logpoint_list.begin();

		while(p_log!=(* p_m).logpoint_list.end())

		{

			f_multipass<< (* p_log)<<",";

			p_log++;

		}

		f_multipass<<endl;

		p_m++;

	}

	f_multipass.close();



}

//end by Zhufeng for multipass, Dec 23, 2011





void AT::initialize_equivalent_machines(){

	//initialize the data structure equivalent machines

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

	//output the data structure Equivalent_Machines to a debug file

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

	//output the data structure lot_set_heur to a debug file

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



			//Updated by Zhufeng for logpoint, Aug 31, 2011



			/*int index=index_S_sim_d(sim,d);

			vii p_s=S_sim_d[index].second.int_vector.begin();

			while(p_s!=S_sim_d[index].second.int_vector.end()){*/



				int index=index_S_sim_l(sim,l);

				vii p_s=S_sim_l[index].second.int_vector.begin();

				while(p_s!=S_sim_l[index].second.int_vector.end()){

				//end by Zhufeng for logpoint



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

		vector<int>::iterator p_Lambda=Lambda_i[i].int_vector.begin();

		while(p_Lambda!=Lambda_i[i].int_vector.end()){

			int Lambda_id=(*p_Lambda);

			vector<int>::iterator p_L=L_i[i].int_vector.begin();

			while(p_L!=L_i[i].int_vector.end()){

				int l=(*p_L);

				vector<tooling_configure> TS;

				get_S_i_l_Lambda(TS,i,l,Lambda_id);

				vector<tooling_configure>::iterator p_TS=TS.begin();

				while(p_TS!=TS.end()){

					double sum=0;

					int s=(*p_TS).id;

					sum-=value[index2_x(i,l,s)];

					sum+=value[index2_y(i,Lambda_id)];

					string str="(";

					stringstream ss;

					ss<<"i@"<<i<<",Lambda@"<<Lambda_id<<",l@"<<l<<",s@"<<s<<")";

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

			p_Lambda++;

		}

	}

	cout<<endl;



	cout<<"test constraints (1d)";

	for(int i=1;i<=Num_Machine;i++){

		double sum=0;

		vector<int>::iterator p_Lambda=Lambda_i[i].int_vector.begin();

		while(p_Lambda!=Lambda_i[i].int_vector.end()){

			int Lambda_id=(* p_Lambda);

			sum+=value[index2_y(i,Lambda_id)];

			p_Lambda++;

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

					vector<int> Lambda_i_tf_tau;

					get_Lambda(Lambda_i_tf_tau,i,tf,tau);

					vii p_Lambda=Lambda_i_tf_tau.begin();

					while(p_Lambda!=Lambda_i_tf_tau.end()){

						int Lambda_id=(*p_Lambda);

						vector<Lambda_item>::iterator pL=Lambda.begin()+Lambda_id-1;

						vii p_find=find((*pL).tooling_vec.begin(),(*pL).tooling_vec.end(),tf);

						int offset=p_find-(*pL).tooling_vec.begin();

						vii pb=(*pL).b.begin()+offset;

						sumx+=(*pb)*value[index2_y(i,Lambda_id)];

						p_Lambda++;

					}//lambda_iteration

					p_int++;

				}//tau_iteration

			}//i_iteration

			vector<t_set> inter_set;

			get_inter_set((*p_set),inter_set);

			vector<t_set>::iterator pm=inter_set.begin();

			int sum=0;

			while(pm!=inter_set.end()){

				int set_id=get_set_id(*pm);

				int pos=tf*Max_Num_Temperature+set_id;

				//sum+=n_t_n_tooling[tf][set_id];

				sum+=n_t_n_tooling[pos];

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



			//Updated by Zhufeng, Aug 31, 2011



			/*int index=index_S_sim_d(sim,d);

			vii p_s=S_sim_d[index].second.int_vector.begin();

			while(p_s!=S_sim_d[index].second.int_vector.end()){*/



				int index=index_S_sim_l(sim,l);

				vii p_s=S_sim_l[index].second.int_vector.begin();

				while(p_s!=S_sim_l[index].second.int_vector.end()){



				//end by Zhufeng for logpoint



				int s=(*p_s);

				sum+=(n_l_chips[l]/rate[s]+Load_Unload_Time)*value[index2_x(i,l,s)];

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

			int pos=i*Max_Num_Device+d;

			vector<int>::iterator p_L=L_i_d[pos].int_vector.begin();

			while(p_L!=L_i_d[pos].int_vector.end()){

				int l=(*p_L);

				int sim=machine_sim[i];



				//Updated by Zhufeng for logpint, Aug 31, 2011



				/*int index=index_S_sim_d(sim,d);

				vii p_s=S_sim_d[index].second.int_vector.begin();

				while(p_s!=S_sim_d[index].second.int_vector.end()){*/



				int index=index_S_sim_l(sim,l);

				vii p_s=S_sim_l[index].second.int_vector.begin();

				while(p_s!=S_sim_l[index].second.int_vector.end()){



				//end by Zhufeng for logpoint



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

			int pos=i*Max_Num_Device+d;

			vector<int>::iterator p_L=L_i_d[pos].int_vector.begin();

			while(p_L!=L_i_d[pos].int_vector.end()){

				int l=(*p_L);

				int sim=machine_sim[i];



				//Updated by Zhufeng for logpint, Aug 31, 2011



				/*int index=index_S_sim_d(sim,d);

				vii p_s=S_sim_d[index].second.int_vector.begin();

				while(p_s!=S_sim_d[index].second.int_vector.end()){*/



				int index=index_S_sim_l(sim,l);

				vii p_s=S_sim_l[index].second.int_vector.begin();

				while(p_s!=S_sim_l[index].second.int_vector.end()){

				//end by Zhufeng for logpoint





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



//Commnet by Zhufeng, Feb 2014,not used anymroe

//void AT::assign_heur(int ** tooling_used,int finished_dev_amount[]){

//	//ofstream fout("debug_assign_heur.txt");

//	f_log<<"In assign_heur()"<<endl;

//	int lot_counter=0;

//	int break_limit=0;

//	vii p_dev=dev_ord_heur.begin();

//	while(p_dev!=dev_ord_heur.end()){

//		int dev=(*p_dev);

//		//-----------------------------S(d)

//		vector<tooling_configure> TS;

//		get_S_l(TS,(*p_device[dev].int_vector.begin()));

//		//sort(TS.begin(),TS.end(),compare_tc);//sort the routes, primative first according to eps[]

//		sort_TS(TS);

//

//		vii p_lot=p_device[dev].int_vector.begin();

//		while(p_lot!=p_device[dev].int_vector.end()){//for each lot in the device 

//			f_log<<"dev="<<dev<<",lot="<<(*p_lot)<<endl;

//

//			int mach_selected=-1;

//			int route_selected=-1;

//			vector<int> mach_vect;

//			vector<int> route_vect;

//			vector<tooling_configure>::iterator p_TS=TS.begin();

//			while(p_TS!=TS.end()){

//				int mg=(*p_TS).mg;

//				int s=(*p_TS).id;

//				vector<int> a;

//				vii pt=(*p_TS).tf.tools.begin();

//				while(pt!=(*p_TS).tf.tools.end()){

//					int tf=(*pt);

//					int pos=s*Max_Num_Tooling_Family+tf;

//					//a.push_back(a_st[s][tf]);

//					a.push_back(a_st[pos]);

//					pt++;

//				}

//

//				vii pm=(*(Machine_Groups.begin()+mg-1)).machines.begin();

//				//f_log<<"mg="<<mg<<",temperature="<<temperature<<endl;

//				while(pm!=(*(Machine_Groups.begin()+mg-1)).machines.end()){

//					int i=(*pm);

//					vector<machine_heur_item>::iterator p_mach=Machine_Heur.begin()+i-1;

//					//is the mach running?

//					if(compare_int_vec((*p_TS).tf.tools,(*p_mach).tooling)

//						&& compare_int_vec(a,(*p_mach).b)

//						&& (*p_mach).temperature==(*p_TS).temperature

//						&& (*p_mach).time_used+n_l_chips[(*p_lot)]/rate[s]+Load_Unload_Time<=H[i])

//					{//some toolings have been installed

//						mach_vect.push_back(i);

//						route_vect.push_back(s);

//					}

//					pm++;

//				}

//				p_TS++;

//			}

//			//cout<<"1"<<endl;

//			if((int)(mach_vect.size())>0){//some running mach can be assigned to the device

//				//cout<<"2"<<endl;

//				double min_s=1E+8;

//				int s_record=-1;

//				vii p_route=route_vect.begin();

//				while(p_route!=route_vect.end()){

//					int s=(*p_route);

//					if(eps[s]<min_s){

//						min_s=eps[s];

//						s_record=s;

//					}

//					p_route++;

//				}

//

//				vii p_mach=mach_vect.begin();

//				double min_t=1E+8;

//				int i_record=-1;

//				while(p_mach!=mach_vect.end()){

//					//select the machine with the least remaining time

//					int n=p_mach-mach_vect.begin();

//					vii p_r=route_vect.begin()+n;

//					vector<machine_heur_item>::iterator pm=Machine_Heur.begin()+(*p_mach)-1;

//					if((*pm).time_used<min_t && (*p_r)==s_record){

//						min_t=(*pm).time_used;

//						i_record=(*p_mach);

//					}

//					p_mach++;

//				}

//				mach_selected=i_record;

//				route_selected=s_record;

//			}else{//no related machine is running

//				//cout<<"3"<<endl;

//				p_TS=TS.begin();

//				while(p_TS!=TS.end()){

//					int tooling_selected_flag=0;

//					int s=(*p_TS).id;

//					int mg=(*p_TS).mg;

//					int temperature=(*p_TS).temperature;

//					struct t_set one_set;

//					one_set.set_elements.push_back(temperature);

//					vector<t_set> inter_set;

//					get_inter_set(one_set,inter_set);

//					vector<t_set>::iterator p_set=inter_set.begin();

//					vector<int> n_vect;

//					while(p_set!=inter_set.end()){

//						int n=get_set_id((*p_set));

//						n_vect.push_back(n);

//						p_set++;

//					}

//					//select the tooling----------------------------------------------------------------------

//					//cout<<"select tooling"<<endl;

//					vii p_tool=(*p_TS).tf.tools.begin();//test if there is enough tooling

//					vector<int> record_n;

//					while(p_tool!=(*p_TS).tf.tools.end()){

//						int tf=(*p_tool);

//						vii p_n=n_vect.begin();							

//						while(p_n!=n_vect.end()){

//							int n=(*p_n);

//							int pos=tf*Max_Num_Temperature+n;

//							int pos2=s*Max_Num_Tooling_Family+tf;

//							if(tooling_used[tf][n]+a_st[pos2]<=n_t_n_tooling[pos]){

//								record_n.push_back(n);

//								break;

//							}

//							p_n++;

//						}

//						p_tool++;

//					}

//					if((int)record_n.size()==(int)(*p_TS).tf.tools.size()){

//						tooling_selected_flag=1;

//					}

//					if((int)(*p_TS).tf.tools.size()==1 && (*(*p_TS).tf.tools.begin())==0){

//						//cout<<"no tooling required"<<endl;

//						tooling_selected_flag=1;

//					}

//					//select a machine to open----------------------------------------------------------------------

//					//cout<<"select a machine to open"<<endl;

//					vii pmg=(*(Machine_Groups.begin()+mg-1)).machines.begin();

//					while(pmg!=(*(Machine_Groups.begin()+mg-1)).machines.end() && tooling_selected_flag){

//						int i=(*pmg);

//						vector<Machine>::iterator pM=Machine_vector.begin()+i-1;

//						vii pfind=find((*pM).temperature_vector.begin(),(*pM).temperature_vector.end(),temperature);

//						vector<machine_heur_item>::iterator p_mach=Machine_Heur.begin()+i-1;

//						if(pfind==(*pM).temperature_vector.end()||(int)(*p_mach).tooling.size()>0)

//						{//the machine i can not run under the temperature

//							pmg++;

//							continue;

//						}else{

//							mach_selected=i;

//							route_selected=s;

//							break;

//						}

//						//pmg++;

//					}

//					if(mach_selected>0){//some machine is selected

//						for(int i=1;i<=(int)record_n.size();i++){//assgin the tooling

//							vii p_int=record_n.begin()+i-1;

//							int n=(*p_int);

//							p_int=(*p_TS).tf.tools.begin()+i-1;

//							int tf=(*p_int);

//							int pos=s*Max_Num_Tooling_Family+tf;

//							//tooling_used[tf][n]+=a_st[s][tf];

//							tooling_used[tf][n]+=a_st[pos];

//						}

//						break;

//					}

//					p_TS++;

//				}

//			}//endif

//			//cout<<"4"<<endl;

//			f_log<<"mach_selected="<<mach_selected<<endl;

//			f_log<<"route_selected="<<route_selected<<endl;

//			//assign the lot to mach_selected

//			if(mach_selected>0){

//				vector<machine_heur_item>::iterator p_mach=Machine_Heur.begin()+mach_selected-1;

//				(*p_mach).time_used+=n_l_chips[(*p_lot)]/rate[route_selected]+Load_Unload_Time;

//				vector<tooling_configure>::iterator p_S=S.begin()+route_selected-1;

//				(*p_mach).temperature=(*p_S).temperature;

//				(*p_mach).tooling.resize((int)(*p_S).tf.tools.size());

//				copy((*p_S).tf.tools.begin(),(*p_S).tf.tools.end(),(*p_mach).tooling.begin());

//				vector<int> a;

//				int s=(*p_S).id;

//				vii pt=(*p_S).tf.tools.begin();

//				while(pt!=(*p_S).tf.tools.end()){

//					int tf=(*pt);

//					int pos=s*Max_Num_Tooling_Family+tf;

//					//a.push_back(a_st[s][tf]);

//					a.push_back(a_st[pos]);

//					pt++;

//				}

//				(*p_mach).b.resize((int)a.size());

//				copy(a.begin(),a.end(),(*p_mach).b.begin());

//				(*p_mach).lot.push_back(*p_lot);

//				finished_dev_amount[dev]+=n_l_chips[(*p_lot)];

//				lot_counter++;

//				vector<lot_heur_item>::iterator p_find=Lot_Assign_Heur.begin()+(*p_lot)-1;

//				(*p_find).i=mach_selected;

//				(*p_find).s=route_selected;

//			}else{

//				f_log<<"discard lot "<<(*p_lot)<<endl;

//			}

//			vii p_find=find(lot_set_heur.begin(),lot_set_heur.end(),(*p_lot));

//			lot_set_heur.erase(p_find);

//			//cout<<"5"<<endl;

//			if(lot_counter>=(int)lot_set_heur.size()*0.01 && lot_counter>=5){

//				break_limit=1;

//				break;

//			}

//			p_find=find(Packages.begin(),Packages.end(),dev);

//			if(p_find!=Packages.end() && finished_dev_amount[dev]>=n_p_minchips[dev]){

//				//cout<<"more than requried"<<endl;

//				break_limit=1;

//				break;

//			}

//			p_find=find(Key_Devices.begin(),Key_Devices.end(),dev);

//			if(p_find!=Key_Devices.end() && finished_dev_amount[dev]>=n_k_minchips[dev]){

//				break_limit=1;

//				break;

//			}

//			p_lot++;

//		}

//		if(break_limit){

//			f_log<<"lot_counter="<<lot_counter<<endl;

//			break;

//		}

//		p_dev++;

//	}

//	//fout.close();

//}

//

//End by Zhufeng, Feb 2014.

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



//Comment by Zhufeng,Feb 4, 2014.Not used anymore



//void AT::initial_soln(){

//	//---------------------------------------------------------

//	//lot_set_heur is initialized in build_model(), which lots should be processed

//	vector<int> save_lot_set_heur;

//	save_lot_set_heur.resize((int)lot_set_heur.size());

//	copy(lot_set_heur.begin(),lot_set_heur.end(),save_lot_set_heur.begin());

//	int ** tooling_used=new int *[Num_Tooling_Family+1];

//	for(int i=0;i<=Num_Tooling_Family;i++){

//		tooling_used[i]=new int[(int)Temperature_Set.size()+1];

//	}

//	for(int tf=1;tf<=Num_Tooling_Family;tf++){

//		for(int i=1;i<=(int)Temperature_Set.size();i++){ tooling_used[tf][i]=0;}

//	}

//	int finished_dev_amount[Max_Num_Device+1];

//	for(int i=1;i<=Num_Device;i++){ finished_dev_amount[i]=0;}

//	initialize_Machine_Heur();

//	initialize_Lot_Assign_Heur();

//	//---------------------------------------------------------

//	int amount_decrease=1;

//	int iter_count=1;

//	while(amount_decrease>0){

//		f_log<<"iter_count="<<iter_count<<endl;

//		int save_lot_set_heur_size=(int)lot_set_heur.size();

//		f_log<<"sort"<<endl;

//		sort_lot_heur(finished_dev_amount);

//		f_log<<"assign"<<endl;

//		assign_heur(tooling_used,finished_dev_amount);

//		amount_decrease=save_lot_set_heur_size-(int)lot_set_heur.size();

//		f_log<<"finished amount"<<endl;

//		for(int i=1;i<=Num_Device;i++){

//			f_log<<"d="<<i<<",finished ="<<finished_dev_amount[i]<<endl;

//		}

//		iter_count++;

//	}

//	lot_set_heur.resize((int)save_lot_set_heur.size());

//	copy(save_lot_set_heur.begin(),save_lot_set_heur.end(),lot_set_heur.begin());

//	show_Machine_Heur();

//	show_Lot_Assign_Heur();

//	double * soln=new double[y_limit+1];

//	f_log<<"soln:"<<endl;

//	for(int i=0;i<=y_limit;i++){ soln[i]=0.0;}

//	build_soln(soln);

//	//test_feasibility(soln);

//	delete [] soln;

//	//---------------------------------------------------------

//	for(int i=0;i<=Num_Tooling_Family;i++){

//		delete [] tooling_used[i];

//	}

//	delete [] tooling_used;

//}



//end by Zhufeng, Feb 4, 2014



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

			int Lambda_id=get_Lambda_id((*pm).tooling,(*pm).b,(*pm).temperature);

			int index=index2_y(i,Lambda_id);

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

			int pos=tf*Max_Num_Temperature+n;

			//n_t_n_tooling_copy[id]=n_t_n_tooling[tf][n];

			n_t_n_tooling_copy[id]=n_t_n_tooling[pos];

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

				,vec_int M_l_copy[],vec_tc S_i_copy[],vector<ch_array> & device_name_copy,vector<ch_array> & lot_name_copy,vector<Lambda_item> & Lambda_copy,vec_int Lambda_i_copy[]

				,vec_int M_lambda_copy[]){

	cout<<"get S"<<endl;

	S_copy.clear();

	vector<tooling_configure>::iterator p_S=S.begin();

	while(p_S!=S.end()){

		S_copy.push_back(*p_S);

		p_S++;

	}

	cout<<"in AT_GRASP, S_copy size="<<S_copy.size()<<endl;



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

	cout<<"get Lambda"<<endl;

	vector<Lambda_item>::iterator p_L=Lambda.begin();

	while(p_L!=Lambda.end()){

		Lambda_copy.push_back(*p_L);

		p_L++;

	}

	cout<<"get Lambda_i"<<endl;

	for(int i=1;i<=Num_Machine;i++){

		vii p_int=Lambda_i[i].int_vector.begin();

		while(p_int!=Lambda_i[i].int_vector.end()){

			Lambda_i_copy[i].int_vector.push_back(*p_int);

			p_int++;

		}

	}

	cout<<"get M_lambda"<<endl;

	for(int la=1;la<=(int)Lambda.size();la++){

		vii p_int=M_lambda[la].int_vector.begin();

		while(p_int!=M_lambda[la].int_vector.end()){

			M_lambda_copy[la].int_vector.push_back(*p_int);

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

			int pos=(tf-1)*Max_Num_Temperature+n-1;

			//int_array[id]=n_t_n_tooling[tf][n];

			int_array[id]=n_t_n_tooling[pos];

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

	buf.pack(Lambda_i,Num_Machine+1);

	buf.pack(M_lambda,Max_Num_Tooling_Configure);



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

	buf.pack(Lambda);



	int_array=new int[(int)S.size() * (Num_Tooling_Family+1)];

	for(int s=1;s<=(int)S.size();s++){

		for(int tf=0;tf<=Num_Tooling_Family;tf++){//need to start from 0 for a_st[][], tf could be zero !!!

			int id=(s-1)*(Num_Tooling_Family+1)+tf;

			int pos=(s-1)*Max_Num_Tooling_Family+tf;

			//int_array[id]=a_st[s][tf];

			int_array[id]=a_st[pos];

		}

	}

	buf.pack(int_array,(int)S.size() * (Num_Tooling_Family+1));

	delete [] int_array;



	buf.pack(SIM);

	buf.pack(Machine_Heur);

	buf.pack(Lot_Assign_Heur);



	buf.pack(tooling_sum_rhs);

	buf.pack(L_sim_lambda);

	buf.pack(sim_lambda_L);



	//Comment by Zhufeng for logpoint, Aug 31, 2011

	//buf.pack(S_sim_d);         //S(i,l)

	//end by Zhufeng



	//Added by Zhufeng for logpoint, Aug 31,2011
    buf.pack(S_sim_l);         //S(i,l)

	//end by Zhufeng 



    //Comment by Zhufeng for logopint, Sep 02, 2011

	//buf.pack(S_sim_d_lambda);  //S(i,l,lambda)

	//end by Zhufeng

    

	//Added by Zhufeng for logpoint, Sep 02, 2011

	buf.pack(S_sim_l_lambda);

    //end by Zhufeng, Seo 02, 2011



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

			int pos=(tf-1)*Max_Num_Temperature+n-1;

			//n_t_n_tooling[tf][n]=int_array[id];

			n_t_n_tooling[pos]=int_array[id];

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



	cout<<"unpack Lambda_i"<<endl;

	vec_array=new vec_int[num_cols];

	buf.unpack(vec_array,num_cols,false);

	copy(vec_array,vec_array+num_cols,Lambda_i);

	//show_Lambda_i();

	delete [] vec_array;



	cout<<"unpack M_lambda"<<endl;

	num_cols=Max_Num_Tooling_Configure;

	vec_array=new vec_int[num_cols];

	buf.unpack(vec_array,num_cols,false);

	copy(vec_array,vec_array+num_cols,M_lambda);

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



	cout<<"unpack Lambda vector"<<endl;

	buf.unpack(Lambda);

	//show_Lambda();



	cout<<"unpack a_st"<<endl;

	num_cols=(int)S.size() * (Num_Tooling_Family+1);

	int_array=new int[num_cols];

	buf.unpack(int_array,num_cols,false);

	for(int s=1;s<=(int)S.size();s++){

		for(int tf=0;tf<=Num_Tooling_Family;tf++){//need to start from 0 for a_st[][], tf could be zero !!!

			int id=(s-1)*(Num_Tooling_Family+1)+tf;

			int pos=(s-1)*Max_Num_Tooling_Family+tf;

			//a_st[s][tf]=int_array[id];

			a_st[pos]=int_array[id];

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

	cout<<"unpack L_sim_lambda"<<endl;

	buf.unpack(L_sim_lambda);

	cout<<"unpack sim_lambda_L"<<endl;

	buf.unpack(sim_lambda_L);



	//Comment by Zhufeng for logpoint, Aug 31, 2011

	//cout<<"unpack S_sim_d"<<endl;

	//buf.unpack(S_sim_d);         //S(i,l)

    //end by Zhufeng



	//Added by Zhufeng for logpoint, Aug 31, 2011
	cout<<"unpack S_sim_l"<<endl;
	buf.unpack(S_sim_l);         //S(i,l)

	//end by Zhufeng



	//comment by Zhufeng for logopint, Sep 02, 2011

	//cout<<"unpack S_sim_d_lambda"<<endl;

	//buf.unpack(S_sim_d_lambda);  //S(i,l,lambda)



	//Added by Zhufeng for logpoint, Sep 02, 2011

	cout<<"unpack S_sim_l_lambda"<<endl;

	buf.unpack(S_sim_l_lambda);  //S(i,l,lambda)

	//end by Zhufeng for logpoint



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

	//identical machine: (1)in the same machine family (2)have the same list of operating temperatures (3)have the same H[]

	

	// -------------- added by yufen -----------------------

	// Those machine instances which have initial setup are treated with one sigle group of identical machine

	//													and this group only include this one machine instance



	vector<Machine>::iterator pM=Machine_vector.begin();

	while(pM!=Machine_vector.end()){

		int identical_flag=0; //not identical

		if((int)SIM.size()<1//nothing in SIM yet



			//------added by yufen -----

			|| (*pM).hasInitialSetup

			//-------end yufen ----------

			

			){

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

		else{

			vector<SIM_item>::iterator pSIM=SIM.begin();

			while(pSIM!=SIM.end()){

				if((*pSIM).mg==(*pM).mg && compare_int_vec((*pSIM).temperature_vector,(*pM).temperature_vector)

					&& fabs(H[(*pSIM).represented_machine_id]-H[(*pM).id])<1E-03

					

					//---------added by yufen ---------

					&& (!Machine_vector[(*pSIM).represented_machine_id-1].hasInitialSetup)

					//-------------end yufen ----------

					

					){

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

	//-------------- end yufen -----------------------------





	// ------------ commented by yufen ---------------------

	/*

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

				if((*pSIM).mg==(*pM).mg && compare_int_vec((*pSIM).temperature_vector,(*pM).temperature_vector)

					&& fabs(H[(*pSIM).represented_machine_id]-H[(*pM).id])<1E-03){

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

	*/

	// ---------------------- end yufen ------------------------

}



void AT::show_SIM(){

	ofstream fSIM("Debug/debugSIM.txt");

	vector<SIM_item>::iterator pSIM=SIM.begin();

	while(pSIM!=SIM.end()){

		fSIM<<"simid="<<(*pSIM).simid<<",mg="<<(*pSIM).mg<<",represented_id="<<(*pSIM).represented_machine_id

			<<",name="<<(*pSIM).name<<",num_machine="<<(*pSIM).num_machine<<endl;

		fSIM<<"operating temperatures: ";

		vii pt=(*pSIM).temperature_vector.begin();

		while(pt!=(*pSIM).temperature_vector.end()){

			fSIM<<(*pt)<<",";

			pt++;

		}

		fSIM<<endl;

		fSIM<<"machines in this SIM: ";

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



				//Updated by Zhufeng for logpoint, Aug 31, 2011



				/*int index=index_S_sim_d(sim,d);

				vii p_s=S_sim_d[index].second.int_vector.begin();

				while(p_s!=S_sim_d[index].second.int_vector.end()){*/



				int index=index_S_sim_l(sim,l);

				vii p_s=S_sim_l[index].second.int_vector.begin();

				while(p_s!=S_sim_l[index].second.int_vector.end()){



				//end by Zhufeng for logpoint



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

		vii p_Lambda=Lambda_i[i].int_vector.begin();

		while(p_Lambda!=Lambda_i[i].int_vector.end()){

			int Lambda_id=(*p_Lambda);

			string str="y";

			stringstream ss;

			ss<<"("<<i<<")("<<Lambda_id<<")";

			str+=ss.str();

			char name[20];

			strcpy(name,str.c_str());

			y_vector.push_back(IloNumVar(env,0.0,1.0,name));

			num_var++;

			p_Lambda++;

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



				//Updated by Zhufeng for logpoint, Aug 31, 2011



				/*int index=index_S_sim_d(sim,d);

				vii p_s=S_sim_d[index].second.int_vector.begin();

				while(p_s!=S_sim_d[index].second.int_vector.end()){*/



				int index=index_S_sim_l(sim,l);

				vii p_s=S_sim_l[index].second.int_vector.begin();

				while(p_s!=S_sim_l[index].second.int_vector.end()){

					//end by Zhufeng for logpoint



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

		vii p_L=Lambda_i[i].int_vector.begin();

		while(p_L!=Lambda_i[i].int_vector.end()){

			int Lambda_id=(*p_L);

			vector<IloNumVar>::iterator p_y=y_vector.begin()+index3_y(i,Lambda_id)-x_limit_first_level-Num_Keydevice-Num_Package-1;

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



			//Updated by Zhufeng for logpoint, Aug 31, 2011



			/*int index=index_S_sim_d(sim,d);

			vii p_s=S_sim_d[index].second.int_vector.begin();

			while(p_s!=S_sim_d[index].second.int_vector.end()){*/



				int index=index_S_sim_l(sim,l);

				vii p_s=S_sim_l[index].second.int_vector.begin();

				while(p_s!=S_sim_l[index].second.int_vector.end()){

					//end by Zhufeng for logpoint



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

		vector<int>::iterator p_Lambda=Lambda_i[i].int_vector.begin();

		while(p_Lambda!=Lambda_i[i].int_vector.end()){

			int Lambda_id=(*p_Lambda);

			vector<int>::iterator p_L=L_i[i].int_vector.begin();

			while(p_L!=L_i[i].int_vector.end()){

				vii pfind=find(LPD.begin(),LPD.end(),(*p_L));

				if(pfind!=LPD.end()){

					int l=(*p_L);

					vector<tooling_configure> TS;

					get_S_i_l_Lambda(TS,i,l,Lambda_id);

					vector<tooling_configure>::iterator p_TS=TS.begin();

					while(p_TS!=TS.end()){

						IloExpr expr(env);

						int s=(*p_TS).id;

						vector<IloNumVar>::iterator p_x=x_vector.begin()+index3_x(i,l,s);

						vector<IloNumVar>::iterator p_y=y_vector.begin()+index3_y(i,Lambda_id)-x_limit_first_level-Num_Keydevice-Num_Package-1;

						expr=(*p_y)-(*p_x);

						con.add(expr>=0);

						expr.end();

						num_con++;

						string str="(";

						stringstream ss;

						ss<<"i@"<<i<<",Lambda@"<<Lambda_id<<",l@"<<l<<",s@"<<s<<")";

						str+=ss.str();

						char name[100];

						strcpy(name,str.c_str());

						con[num_con-1].setName(name);

						p_TS++;

					}

				}

				p_L++;

			}

			p_Lambda++;

		}

	}

	/*

	for(int i=1;i<=Num_Machine;i++){

		vector<int>::iterator p_Lambda=Lambda_i[i].int_vector.begin();

		while(p_Lambda!=Lambda_i[i].int_vector.end()){

			int Lambda_id=(*p_Lambda);

			int counter=0;

			IloExpr expr(env);

			vector<IloNumVar>::iterator p_y=y_vector.begin()+index3_y(i,Lambda_id)-x_limit_first_level-Num_Keydevice-Num_Package-1;

			vector<int>::iterator p_L=L_i[i].int_vector.begin();

			while(p_L!=L_i[i].int_vector.end()){

				int l=(*p_L);

				vector<tooling_configure> TS;

				get_S_i_l_Lambda(TS,i,l,Lambda_id);

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

			ss<<"i@"<<i<<",Lambda@"<<Lambda_id<<")";

			str+=ss.str();

			char name[100];

			strcpy(name,str.c_str());

			con[num_con-1].setName(name);

			p_Lambda++;

		}

	}

	*/



	cout<<"build constraints (1d)"<<endl;

	for(int i=1;i<=Num_Machine;i++){

		IloExpr expr(env);

		vector<int>::iterator p_Lambda=Lambda_i[i].int_vector.begin();

		while(p_Lambda!=Lambda_i[i].int_vector.end()){

			int Lambda_id=(* p_Lambda);

			vector<IloNumVar>::iterator p_y=y_vector.begin()+index3_y(i,Lambda_id)-x_limit_first_level-Num_Keydevice-Num_Package-1;

			expr+=(*p_y);

			p_Lambda++;

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

					vector<int> Lambda_i_tf_tau;

					get_Lambda(Lambda_i_tf_tau,i,tf,tau);

					vii p_Lambda=Lambda_i_tf_tau.begin();

					while(p_Lambda!=Lambda_i_tf_tau.end()){

						int Lambda_id=(*p_Lambda);

						vector<Lambda_item>::iterator pL=Lambda.begin()+Lambda_id-1;

						vii p_find=find((*pL).tooling_vec.begin(),(*pL).tooling_vec.end(),tf);

						int offset=p_find-(*pL).tooling_vec.begin();

						vii pb=(*pL).b.begin()+offset;

						vector<IloNumVar>::iterator p_y=y_vector.begin()+index3_y(i,Lambda_id)-x_limit_first_level-Num_Keydevice-Num_Package-1;

						expr+=(*pb)*(*p_y);

						p_Lambda++;

					}//lambda_iteration

					p_int++;

				}//tau_iteration

			}//i_iteration

			vector<t_set> inter_set;

			get_inter_set((*p_set),inter_set);

			vector<t_set>::iterator pm=inter_set.begin();

			int sum=0;

			while(pm!=inter_set.end()){

				int set_id=get_set_id(*pm);

				int pos=tf*Max_Num_Temperature+set_id;

				//sum+=n_t_n_tooling[tf][set_id];

				sum+=n_t_n_tooling[pos];

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



				//Updated by Zhufeng for logpoint, Aug 31, 2011



				/*int index=index_S_sim_d(sim,d);

				vii p_s=S_sim_d[index].second.int_vector.begin();

				while(p_s!=S_sim_d[index].second.int_vector.end()){*/



				int index=index_S_sim_l(sim,l);

				vii p_s=S_sim_l[index].second.int_vector.begin();

				while(p_s!=S_sim_l[index].second.int_vector.end()){

					//end by Zhufeng



					int s=(*p_s);

					vector<IloNumVar>::iterator p_x=x_vector.begin()+index3_x(i,l,s);

					expr+=(n_l_chips[l]/rate[s]+Load_Unload_Time)*(*p_x);

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

			int pos=i*Max_Num_Device+d;

			vector<int>::iterator p_L=L_i_d[pos].int_vector.begin();

			while(p_L!=L_i_d[pos].int_vector.end()){

				vii pfind=find(LPD.begin(),LPD.end(),(*p_L));

				if(pfind!=LPD.end()){

					int l=(*p_L);

					int sim=machine_sim[i];



					//Updated by Zhufeng for logpoint, Aug 31, 2011



					//int index=index_S_sim_d(sim,d);

					//vii p_s=S_sim_d[index].second.int_vector.begin();

					//while(p_s!=S_sim_d[index].second.int_vector.end()){



				int index=index_S_sim_l(sim,l);

				vii p_s=S_sim_l[index].second.int_vector.begin();

				while(p_s!=S_sim_l[index].second.int_vector.end()){



					//end by Zhufeng for logpoint



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

			int pos=i*Max_Num_Device+d;

			vector<int>::iterator p_L=L_i_d[pos].int_vector.begin();

			while(p_L!=L_i_d[pos].int_vector.end()){

				vii pfind=find(LPD.begin(),LPD.end(),(*p_L));

				if(pfind!=LPD.end()){

					int l=(*p_L);

					int sim=machine_sim[i];



					//Updated by Zhufeng for logpoint, Aug 31, 2011



					/*int index=index_S_sim_d(sim,d);

					vii p_s=S_sim_d[index].second.int_vector.begin();

					while(p_s!=S_sim_d[index].second.int_vector.end()){*/



				int index=index_S_sim_l(sim,l);

				vii p_s=S_sim_l[index].second.int_vector.begin();

				while(p_s!=S_sim_l[index].second.int_vector.end()){



					//end by Zhufeng for logpoint



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

	for(int it=1;it<=(int)Lambda.size();it++){

		vector<Lambda_item>::iterator p_Lambda=Lambda.begin()+it-1;

		//-----------------

		int sum_b=0;

		vector<int>::iterator p_int=(*p_Lambda).b.begin();

		while(p_int!=(*p_Lambda).b.end()){

			sum_b+=(*p_int);

			p_int++;

		}

		//---------------------------

		if(sum_b>0){//when the tooling list is not empty

			int Lambda_id=(*p_Lambda).id;

			//-------------------------------------temperature sest

			int temperature=(*p_Lambda).temperature;

			struct t_set one_set;

			one_set.set_elements.push_back(temperature);

			vector<t_set> inter_set;

			get_inter_set(one_set,inter_set);

			//---------------------------------------------------------------

			IloExpr expr(env);

			vector<int>::iterator p_M=M_lambda[Lambda_id].int_vector.begin();

			while(p_M!=M_lambda[Lambda_id].int_vector.end()){

				int i=(*p_M);

				//cout<<"i="<<i<<",Lambda="<<Lambda_id<<","<<index3_y(i,Lambda_id)<<endl;

				vector<IloNumVar>::iterator p_y=y_vector.begin()+index3_y(i,Lambda_id)-x_limit_first_level-Num_Keydevice-Num_Package-1;

				expr+=(*p_y);

				p_M++;

			}

			double min_num=1E+8;

			vector<int>::iterator p_tool=(*p_Lambda).tooling_vec.begin();

			while(p_tool!=(*p_Lambda).tooling_vec.end()){

				int offset=p_tool-(*p_Lambda).tooling_vec.begin();

				vector<int>::iterator p_b=(*p_Lambda).b.begin()+offset;

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

			ss<<"Lambda@"<<Lambda_id<<")";

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

	cplex.exportModel("AT_GRASP_first_level.lp");

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

			num_con+=(int)Lambda_i[i].int_vector.size();

		}

		num_con+=Num_Keydevice+Num_Package;

		//add Key PKG and Key PIN PKG constraints

		num_con+=(int)Key_PKG.size()+(int)Key_PIN_PKG.size();

	}else{

		num_con=Num_Lot;

		for(int i=1;i<=Num_Machine;i++){

			vector<int>::iterator p_Lambda=Lambda_i[i].int_vector.begin();

			while(p_Lambda!=Lambda_i[i].int_vector.end()){

				int Lambda_id=(*p_Lambda);

				vector<int>::iterator p_L=L_i[i].int_vector.begin();

				while(p_L!=L_i[i].int_vector.end()){

					int l=(*p_L);

					vector<tooling_configure> TS;

					get_S_i_l_Lambda(TS,i,l,Lambda_id);

					num_con+=(int)TS.size();

					p_L++;

				}

				p_Lambda++;

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

		si.writeLp("Debug/AT_OSI"); //THIS IS THE WHOLE MODEL (This is the problem we are going to solve)

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

	vector<double> SL_avg((int)L_sim_lambda.size(),0);

	vector<int> SL_num((int)SL_avg.size(),0);

	vector<double> SL_best((int)L_sim_lambda.size(),0);

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

	//----------------------------------------------------------initialize the sim_lambda_benefit

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

	vector< pair<double,vec_int> > sim_lambda_benefit_save;

	initialize_benefit(sim_lambda_benefit_save,tooling_used,sim_used,SIM_number);

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

		vector< pair<double,vec_int> > sim_lambda_benefit;

		sim_lambda_benefit.resize((int)sim_lambda_benefit_save.size());

		copy(sim_lambda_benefit_save.begin(),sim_lambda_benefit_save.end(),sim_lambda_benefit.begin());

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

		cout<<"alpha="<<alpha<<endl;

		//cout<<"prob_list=";

		//for(int i=0;i<=9;i++){ cout<<prob_list[i]<<","; } 

		//cout<<endl;

		//--------------------------------------------------------------------start the loop

		int counter=0;

		vector<int> lot_assigned_so_far;

		vector<int> production(Num_Device,0);



		//Added by Zhufeng for fixed initial setup, April 06, 2013

		vector<initial_mi_sim_lambda> initial_machine_setup_vector;

		if(fix_initial_setup==1){

			initialize_machine_setup_vector(initial_machine_setup_vector);

		}

		//end by Zhufeng, April 06, 2013



		while(true){

			cout<<"counter="<<counter<<endl;

			//--------------------------------------------------------------------------construct the CL and RCL

			//cout<<"construct CL"<<endl;

			vector< pair< pair<int,int>,double> > candidate_list;



		    //Added by Zhufeng for fixed initial setup, April 06, 2013

			bool initial_flag = false;  //denote whether this couter is to construct CL for an initial machine

			int initial_mi_id = 0;

			if(fix_initial_setup==1 && (int)initial_machine_setup_vector.size()>0){

				cout<<"construct a candidate list for an initial machine setup"<<endl;

				initial_flag =true;

				//construct_CL_fixed_initial_setup(vector< pair< pair<int,int>,double> > & CL,vector<initial_mi_sim_lambda> & initial_mi_setup,int & initial_mi_id)

				construct_CL_fixed_initial_setup(candidate_list,initial_machine_setup_vector,initial_mi_id);

				//construct_CL_fixed_initial_setup(CL,initial_machine_setup_vector,initial_mi_id);

			}

			//end by Zhufeng, April 06, 2013

			else{

				cout<<"construct a candidate list for regular machine setups"<<endl;

				construct_CL(candidate_list,tooling_used,sim_used,SIM_number,sim_lambda_benefit);

			}

			cout<<"size of CL="<<(int)candidate_list.size()<<endl;

			

			if((int)candidate_list.size()<1){ break; }//stop the loop here

			

            

			int RCL_length=construct_RCL(candidate_list,alpha);

		

			//if(iter>500){

			//	RCL_length=RCL_length*(1+(roundup(iter*1.0/100)-5)*0.5);

			//	if(RCL_length>(int)candidate_list.size()){

			//		RCL_length=(int)candidate_list.size();

			//	}

			//}

			cout<<"RCL_length="<<RCL_length<<endl;



			int selected_id;

			if(iter<=20){

				selected_id=pick(RCL_length)+1;

			}else{

				selected_id=pick(candidate_list,RCL_length,SL_avg,SL_best,best_obj,50)+1;

			}

			cout<<"selected_id="<<selected_id<<endl;

			//----------------------------------------------------------------------------randomly select one

			vector<pair< pair<int,int>,double > >::iterator p_selected=candidate_list.begin()+selected_id-1;

			int sim_selected=(*p_selected).first.first;

			int Lambda_selected=(*p_selected).first.second;

			vector<Lambda_item>::iterator pL=Lambda.begin()+Lambda_selected-1;

			cout<<"sim_selected="<<sim_selected<<",Lambda_selected="<<Lambda_selected<<endl;

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

			cout<<"fix y"<<endl;

			//Added by Zhufeng for fixed initial setup, April 06, 2013

			if(initial_flag){

				cout<<"initial_mi_id = "<<initial_mi_id<<endl;

				fix_y[initial_mi_id]=Lambda_selected;

			}

			//end by Zhufeng, April 06, 2013

			else{

				for(int i=1;i<=Num_Machine;i++){

					if(machine_sim[i]==sim_selected && fix_y[i]<1){

						fix_y[i]=Lambda_selected;

						break;

					}

				}

			}

			//-------------------------------------------------------------------------------------assign lots to the selected sim_lambda combination

			vector<int> lot_selected;

			cout<<"greedy assignment"<<endl;

			//greedy_assignment(sim_selected,Lambda_selected,lot_remain,lot_selected,production);

			int id_select=index_L_sim_lambda(sim_selected,Lambda_selected);

	

			lot_selected.resize((int)sim_lambda_benefit[id_select].second.int_vector.size());

	

			copy(sim_lambda_benefit[id_select].second.int_vector.begin(),sim_lambda_benefit[id_select].second.int_vector.end(),lot_selected.begin());



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

			cout<<"size of lot_remain  ="<<lot_remain.size()<<endl;

			cout<<"size of lot_selected="<<lot_selected.size()<<endl;

			//-------------------------------------------------

			cout<<"updates"<<endl;

			update_benefit(sim_lambda_benefit,lot_remain,production,affected_dev,tooling_used,sim_used,SIM_number);

			//update_benefit2(sim_lambda_benefit,lot_remain,production,affected_dev,tooling_used,sim_used,SIM_number,si_KP);

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

					cout<<"set i="<<i<<",Lambda_id="<<fix_y[i]<<",id="<<y_id<<endl;

					si.setColBounds(y_id,1,1);

				}

			}



			si.setHintParam(OsiDoDualInResolve,false,OsiHintDo);



			if(LP_IP==1){

				si.writeLp("Debug/first_level_heur"); //This is a subproblem when y is fixed

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

			int Lambda_id=fix_y[i];

			if(Lambda_id>0){

				int index=index_L_sim_lambda(sim,Lambda_id);

				SL_avg[index]=(SL_avg[index]*SL_num[index]+second_level_obj)/(SL_num[index]+1);

				if(SL_num[index]<1 || second_level_obj<SL_best[index]){ SL_best[index]=second_level_obj;}

				SL_num[index]++;

				if(SL_best[index]>best_obj){ best_obj=SL_best[index];}	

			}

		}		

		cout<<"insert_flag="<<insert_flag<<endl;

	}//end_for_iter

	time(&t2);

	cout<<"it costs "<<difftime(t2,t1)<<" seconds to run the Phase I iterations"<<endl;

	cout<<"best iter="<<best_iter<<",best_obj="<<best_obj<<endl;

	sort(soln_pool.begin(),soln_pool.end(),compare_soln);

	cout<<"Phase I statistics:"<<endl;

	cout<<"Min ="<<soln_pool[0].obj;

	cout<<",Max="<<soln_pool[(int)soln_pool.size()-1].obj<<endl;

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



	//Added by Zhufeng
	ofstream fcout("Output/resultsummery.txt");
	//fcout<<"it costs "<<difftime(t2,t1)<<" seconds to run the Phase I iterations"<<endl;
	//fcout<<"best iter="<<best_iter<<",best_obj="<<best_obj<<endl;
    fcout<<"Phase I statistics:"<<endl;
	//fcout<<"Min ="<<soln_pool[0].obj<<",Max="<<soln_pool[(int)soln_pool.size()-1].obj<<endl;
	//fcout<<"ave obj="<<PhaseI_avg<<endl;
	fcout<<"solution pool:"<<endl;
	/*for(int i=1;i<=(int)soln_pool.size();i++){
		fcout<<soln_pool[i-1].obj<<endl;
	}*/
	fcout.close();
    //end by Zhufeng

	cout<<"  ==================================================== successfully solved ============================= "<<endl<<endl;

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

		//Added by Zhufeng, recording weighted shortage,March 16,

		double total_weighted_short=0;

		//end by Zhufeng

		for(int d=1;d<=Num_Device;d++){

			if(is_KD_PD(d) && soln_pool[i-1].shortage[d-1].second>0){

				total_shortage+=soln_pool[i-1].shortage[d-1].second;

				total_weighted_short+=soln_pool[i-1].shortage[d-1].second/(double)coef*penalty[d];

			}

		}

		cout<<"obj="<<soln_pool[i-1].obj<<",total_weight="<<total_weight<<",total_shortage="<<total_shortage<<endl;
		if(i==1)
		{
		fcout.open("Output/resultsummery.txt",ios::out|ios::app);
		fcout<<"obj="<<soln_pool[i-1].obj<<",weighted sum lots(no initial lots)="<<total_weight<<endl
			<<"total_shortage="<<total_shortage<<",total_weighted_shortage="<<total_weighted_short<<endl;
		fcout.close();
		}
		

	}

	//---------------------------------------------------------------------------local search for the elite soln

	if(PhaseII_option){

		time_t local_search_start;

		time(&local_search_start);

	

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



	//------------ commented by yufen -----------------

	

	minimize_makespan(soln_pool[0]);

	show_sub_soln(soln_pool[0].sub_soln);



	//---------------------------------------------------------------------------here comes the enhancement

	//Not used here anymore. For not overtaken initial setups and flexible initial setups, Feb 26, 2012







//Added by Zhufeng, Nov 13, 2012

	if(Multipass_Option ==1){

	//Added by Zhufeng for multipass,Heuristics 3, Jan 20, 2012

	initialize_enhanced_solution(soln_pool[0]);

	//end by Zhufeng, Jan 20, 2012

		}

//end by Zhufeng, Nov 13, 2012

	//Feb 26, 2012

	initialize_tooling_trace(soln_pool[0],tooling_trace);

	//show_tooling_trace(tooling_trace);	

	

//Added by Zhufeng, Oct 19,2012, for multi-pass option

   if(Multipass_Option !=1){

//end by Zhufeng, Oct 19, 2012

	machine_resetup(soln_pool[0]);

//Added by Zhufeng, Oct 19 ,2012

   }

//end by Zhufneg, Oct 19, 2012

	//end by Zhufeng, Dec 29th, 2011



	//Comment by Zhufeng for multipass,Heuristics 3, Jan 20, 2012

	//Added by Zhufeng for multipass, Dec 29th, 2011

	//second_pass_setup(soln_pool[0]);

	//end by Zhufeng, Dec 29th, 2011

	//end by Zhufeng, Jan 20, 2012

	

	



	//Added by Zhufeng, for multipass objective value. Feb 27, 2012\

	//First compute first-pass objective value and production

	//double obj_first_pass_solution=soln_pool[0].obj;
	vector<int> production_first_pass;
	production_first_pass.reserve(Num_Device);
	double weighted_item=0;// Just record the value of w_l[l]+eps[s] for first pass
	//Added by Zhufeng, March 15, 2012

	double weighted_sum_lot_first_pass=0; //recorded weighted sum of lots by Yumin's code(no initial lots). 

	//end by Zhufeng, March 15, 2012
	fstream f_result("Output/resultsummery.txt", ios::out|ios::app);

	//Added by Zhufeng, Nov 12, 2012
	if(Multipass_Option ==1){
    //end by Zhufeng, Nov12, 2012
	double obj_first_pass_solution=compute_obj(enhanced_solution, production_first_pass,weighted_item,weighted_sum_lot_first_pass);

	

	//Comment by Zhufeng, March 15, 2012, don't need output these
	//f_result<<"Without resetup,first_pass_solution obj = "<<obj_first_pass_solution<<endl;
	f_result<<endl<<"first_pass_solution weighted sum of lots = "<<weighted_sum_lot_first_pass<<endl;
	f_result.close();

	

	//end by Zhufeng, March 15.

	//Added by Zhufeng, Nov 12, 2012

	}

	//end by Zhufeng, Nov 12, 2012

	

	//end by Zhufeng, Feb 27

	



	// ---------- end yufen ----------------



	//---------------------------------------------------------------------------this part generates reports for the GRASP solution

	double * solution_output = new double[y_limit+1+(y_limit-x_limit+1)];

	soln_pool_item_to_solution(soln_pool[0],solution_output);//transform the best GRASP solution to a double array for output

	generate_reports(solution_output);

	delete [] solution_output;





	//------------ commented by yufen -----------------

	

	//---------------------------------------------------------------------------this part generates reports for the enhanced actions

	generate_reports(initial_running_lot,enhanced_solution);

	show_tooling_trace(tooling_trace);

	

	// ---------- end yufen ----------------



	//Added by Zhufeng,Oct 19, 2012, for Multipass_Option

	if(Multipass_Option ==1){

	//end by Zhufeng, Oct 19, 2012



	//Added by Zhufeng for multipass,Heuristics 3, Jan 20, 2012

	//Jan 28

	//show_H();

	//Jan 28

	

	readsolution(first_pass_solution);

	show_first_pass_solution(first_pass_solution);



	//Jan 24, 2012

	vector<multi_mach> machine_list;

	initialize_machine_list(machine_list,first_pass_solution,H); //



	//Jan 25. 2012

	show_machine_list( machine_list );



	vector<multi_lot> first_pass_lot_vector; //including initial lot finished before start of the plan and finished lots in solution

	vector<multi_lot> unassigned_lot_first_pass_vector; // all the unassigned first_pass lot after regular solution and heuristics three



	initialize_first_pass_lot_vector(first_pass_lot_vector,first_pass_solution);

	show_first_pass_lot_vector(first_pass_lot_vector);



	//Feb 24 //Updated by Zhufeng, March 27,2012, calculate benefit

	initialize_unassigned_lot_first_pass_vector(unassigned_lot_first_pass_vector,first_pass_lot_vector);//initialize unassigned lots and Num_Pass  by reading wip.csv and first_pass_lot_vector

	//cout<<"size of unassigned_lot_first_pass_vector"<<unassigned_lot_first_pass_vector.size()<<endl;

	//also showed in Debug/unassigned_lot_first_pass_vector.csv



	

	//May 22, Initialize Num_Pass

	initialize_multi_device_production_step(multi_device_production_step,unassigned_lot_first_pass_vector,first_pass_lot_vector,Max_Num_Step);

	show_multi_device_production_step( multi_device_production_step,Max_Num_Step);

	Num_Pass = Max_Num_Step;

	//May 22



	//May 23, 2012,this must be after initialize_multi_device_production_step()

	//compute the objective value for single-pass solution and output the result to result_summery.txt

	double obj_single_pass = compute_obj_step(first_pass_solution,multi_device_production_step,weighted_item, Max_Num_Step);

	//May 23, 2012,



	//March 25, 2012;

	initialize_multi_device_production( production_first_pass,multi_device_production);

	//March 25, 2012



	//exit(0);

	//Feb 24





   //Feb 13

	vector<tool_setup> tool_setup_list;

	intialize_tool_setup_list(tool_setup_list,S);

	show_tool_setup_list(tool_setup_list);



	vector<mf_tool_setup> mf_tool_setup_list; //mach_tool_setup_list[mf_id-1] is the possible tool_setup_list_id for machine family mf.

	initialize_mf_tool_setup_list(mf_tool_setup_list,tool_setup_list);

	show_mf_tool_setup_list(mf_tool_setup_list);



	//Feb 15

	vector<tf_cert_ti> tf_cert_ti_list;

	initialize_tf_cert_ti_list(tf_cert_ti_list,Num_Temperature, Tooling_vector);

	show_tf_cert_ti_list( tf_cert_ti_list,Num_Temperature);

	//Feb 15







	//Feb 16

	update_tf_cert_ti_list_begin(tf_cert_ti_list,Num_Temperature,machine_list);//update the number of ti in the beginning of the time horizon

																				//This must be implemented before Heuristics_three( machine_list,first_pass_lot_vector);

	show_tf_cert_ti_list( tf_cert_ti_list,Num_Temperature);



	//end by Zhufeng



	

	//Feb 23

	vector<multi_lot> finished_lot_list;



	//May 20, 2012, for GRASP

	vector<multi_mach> machine_list_temp = machine_list;

	vector<multi_mach> machine_list_best;



	vector<multi_lot> finished_lot_list_temp;

	//vector<multi_lot> finished_lot_list_best;



	vector<tf_cert_ti> tf_cert_ti_list_temp = tf_cert_ti_list;

	//vector<tf_cert_ti> tf_cert_ti_list_best;



	double obj_value_best = 1E13;



	//end May 20



	//May 23

	vector<multi_production> multi_device_production_best;

	vector<multi_production_step> multi_device_production_step_best;

	//May 23

	//May 24, 2012, for output purpose

	vector<multi_lot> finished_lot_total_list_best;

	//May24, 2012

	time(&t1);

	//int grasp_times = 100;

	for(int i =1;i<=grasp_times;i++)

	{

		Heuristics_three( machine_list_temp,first_pass_lot_vector,tool_setup_list,mf_tool_setup_list, tf_cert_ti_list_temp,finished_lot_list_temp);

		//Feb 23



		//Feb 27

		f_result.open("Output/resultsummery.txt", ios::out|ios::app);

		//Feb 27


		vector<multi_mach> reset_machine_list=machine_list_temp;



		machine_list_temp.clear();



		for(int index=1;index<=(int)reset_machine_list.size();index++)

		{

			reset_machine_list[index-1].whether_need_check_for_new_lot=true;

		}



		  vector<multi_lot> finished_lot_reset_list;
		  //Changed by Shihui Jia from resetup_option==1 as resetup_option >=1

		if(resetup_option >= 1){
			resetup(reset_machine_list,first_pass_lot_vector,tool_setup_list,mf_tool_setup_list, tf_cert_ti_list_temp,finished_lot_list_temp,finished_lot_reset_list,unassigned_lot_first_pass_vector);

		}


		vector<multi_lot> finished_lot_total_list=finished_lot_list_temp;

		finished_lot_list_temp.clear();



		for(int i=1;i<=(int)finished_lot_reset_list.size();i++)

		{

			finished_lot_total_list.push_back(finished_lot_reset_list[i-1]);

		}



		finished_lot_reset_list.clear();



		double obj_value_temp = compute_obj_multi(reset_machine_list,production_first_pass,finished_lot_total_list,weighted_item, weighted_sum_lot_first_pass,multi_device_production_step,Max_Num_Step,false);


		f_result.close();



		if(obj_value_temp < obj_value_best)

		{

			machine_list_best = reset_machine_list;

			obj_value_best=obj_value_temp;

			//May 23

			multi_device_production_best = multi_device_production;

     		multi_device_production_step_best = multi_device_production_step;

			//May 23

			//May 24, 2012, for output purpose

			finished_lot_total_list_best = finished_lot_total_list;

			//May 24, 2012

		}



		machine_list_temp = machine_list;

		tf_cert_ti_list_temp = tf_cert_ti_list;

		//May 23

		initialize_multi_device_production_step(multi_device_production_step,unassigned_lot_first_pass_vector,first_pass_lot_vector,Max_Num_Step);

		initialize_multi_device_production( production_first_pass,multi_device_production);

		//May 23



	}//end of for(int i =1;i<=grasp_times;i++)

	time(&t2);

	double time_multi = difftime(t2,t1);



	f_result.open("Output/resultsummery.txt", ios::out|ios::app);

	f_result<<endl<<"best multi_pass_solution obj = "<<obj_value_best<<endl;

	double obj_value_test = compute_obj_multi(machine_list_best,production_first_pass,finished_lot_total_list_best,weighted_item, weighted_sum_lot_first_pass,multi_device_production_step_best,Max_Num_Step,true);

	f_result<<endl<<"running time (real time) for multi-pass grasp = "<<time_multi<<endl;

	f_result.close();

	//Feb 25

	output_multi(machine_list_best, first_pass_solution);

	//Feb 25

	//March 25

	show_multi_device_production( multi_device_production_best);

	//March 25



	show_multi_device_production_step( multi_device_production_step_best,Max_Num_Step);





//Added by Zhufeng, Aug 19, 2012, for multipass poption

	}//end of if(Multipass_Option = 1)

//end by Zhufeng, Aug 19, 2012



	//output_multi(machine_list, first_pass_solution);

	//Jan 25, 2012



	//end Jan 24, 2012

	//end by Zhufeng, Jan 20, 2012



	//-------------------------------------------------------------------------------------------------------

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

	//cout<<"OK2"<<endl;

}



void AT::construct_CL(vector< pair< pair<int,int>,double> > & CL,const int * tooling_used,const int * sim_used,const int * SIM_number,const vector< pair<double,vec_int> > & sim_lambda_benefit){

	//given tooling and SIM usage, construct CL according to the (sim,lambda) benefit

	CL.clear();

	int index=-1;

	for(int simid=1;simid<=(int)SIM.size();simid++){

		//cout<<"simid="<<simid<<endl;

		vector<SIM_item>::iterator pSIM=SIM.begin()+simid-1;

		int machine_id=(*pSIM).represented_machine_id;

		vector<int>::iterator p_Lambda=Lambda_i[machine_id].int_vector.begin();

		while(p_Lambda!=Lambda_i[machine_id].int_vector.end()){

			int Lambda_id=(*p_Lambda);

			index++;



			if(sim_lambda_benefit[index].first>1E-04){

				//append the CL

				pair< pair<int,int>,double > one_pair;

				one_pair.first.first=simid;

				one_pair.first.second=Lambda_id;

				one_pair.second=sim_lambda_benefit[index].first;

				CL.push_back(one_pair);

			}//end_if

			p_Lambda++;

		}//end_while

	}//end_for_sim

	//sort the candidate_list according to the benefit

	//----------------------------------------------------------------------output the CL

	sort(CL.begin(),CL.end(),compare_candidates);

	//ofstream fdebug_CL("Debug/debug_CL.txt");

	//for(int i=1;i<=(int)CL.size();i++){

	//	vector< pair< pair<int,int>,double> >::iterator p_CL=CL.begin()+i-1;

	//	fdebug_CL<<"id="<<i<<",sim="<<(*p_CL).first.first<<",Lambda_id="<<(*p_CL).first.second<<",benefit="<<(*p_CL).second<<endl;

	//}

	//fdebug_CL.close();

}



//Added by Zhufeng, for fixed initial setup, April 06, 2013

void AT::initialize_machine_setup_vector( vector<initial_mi_sim_lambda> & mi_setup_vector ){



	for(vector<Machine>::const_iterator p = Machine_vector.begin();p!=Machine_vector.end();++p){

		if(p->hasInitialSetup){

			int mi_id = p->id;

			int sim_id = machine_sim[mi_id];

			int initial_lambda_id = p->initial_tooling_setup;

			struct initial_mi_sim_lambda one_case;

			one_case.mi_id = mi_id;

			one_case.sim_id = sim_id;

			one_case.lambda_id = initial_lambda_id;

			mi_setup_vector.push_back(one_case);

		}

	}

	//cout<<"initial_mi_setup_vector:"<<endl;

	//for(int i=0;i<(int)mi_setup_vector.size();++i){

	//	cout<<"mi_id = "<<mi_setup_vector[i].mi_id<<", sim_id = "<<mi_setup_vector[i].sim_id<<", lambda = "<<mi_setup_vector[i].lambda_id<<endl;

	//}

	//exit(0);

}

//end by Zhufeng, April 06, 2013



void AT::construct_CL_fixed_initial_setup(vector< pair< pair<int,int>,double> > & CL,vector<initial_mi_sim_lambda> & initial_mi_setup,int & initial_mi_id){

//construct the CL ={<sim,lambda>, benefit} with single element, which is the initial lot if this machine mi_id is a initial machine

//get the initial mi_id to construct CL from initial_mi_setup,and remove it from  initial_mi_setup 

	



	//clear CL first

	CL.clear();

	pair< pair<int,int>,double > one_pair;

//get the last element in initial_mi_setup

	vector<initial_mi_sim_lambda>::const_iterator p = initial_mi_setup.end()-1;

	if((p+1)==initial_mi_setup.begin()){

		cout<<"error in construct_CL_fixed_initial_setup! the size of initial_mi_setup is empty"<<endl;

		throw 1;

	}

	one_pair.first.first = p->sim_id;

	one_pair.first.second = p->lambda_id;

	one_pair.second = 10000;

	CL.push_back(one_pair);



	initial_mi_id = p->mi_id;



	initial_mi_setup.pop_back();

//	one_pair.first.first=simid;

//	one_pair.first.second=initial_lambda_id;

}



//End by Zhufeng, for fixed initial setup, April 06, 2013



void AT::construct_CL_resetup(vector< pair< pair<int,int>,double> > & CL,vector< pair<double,vec_int> > & machine_lambda_benefit){

	//construct a CL with respect to the machine-lambda combinations	

	CL.clear();

        int index=-1;

        for(int machine_id=1;machine_id<=(int)Machine_vector.size();machine_id++){

                vector<int>::iterator p_Lambda=Lambda_i[machine_id].int_vector.begin();

                while(p_Lambda!=Lambda_i[machine_id].int_vector.end()){

                        int Lambda_id=(*p_Lambda);

                        index++;



                        if(machine_lambda_benefit[index].first>1E-04){

                                //append the CL

                                pair< pair<int,int>,double > one_pair;

                                one_pair.first.first=machine_id;

                                one_pair.first.second=Lambda_id;

                                one_pair.second=machine_lambda_benefit[index].first;

                                CL.push_back(one_pair);

                        }//end_if

                        p_Lambda++;

                }//end_while

        }//end_for_sim

        //sort the candidate_list according to the benefit

        //----------------------------------------------------------------------output the CL

        sort(CL.begin(),CL.end(),compare_candidates);

}



int AT::construct_RCL(vector< pair< pair<int,int>,double> > & CL,double alpha){

	// --------- commented by yufen -------------

	/*

	int temp=roundup(alpha);

	if((int)CL.size()<temp){ temp=(int)CL.size();}

	return temp;

	*/

	// --------- end yufen ----------------------

  

	// --------- added by yufen -----------------

	int temp=roundup(alpha);



	//Added by Zhufeng. CL[0].first.first is the "sim_id" not the position."sim_id-1" is the position in the vector.

	//So it should be SIM[CL[0].first.first-1] that is the sim in the CL[0].

	if(Machine_vector[SIM[CL[0].first.first-1].represented_machine_id-1].hasInitialSetup

	//Added by Zhufeng

		&& Machine_vector[SIM[CL[0].first.first-1].represented_machine_id-1].initial_tooling_setup==CL[0].first.second

		//end by Zhufeng

		)

	//comment by Zhufeng

	//if(Machine_vector[SIM[CL[0].first.first].represented_machine_id-1].hasInitialSetup)

	//end Zhufeng

	{    //Added by Zhufeng

		 //cout<<"size of SIM="<<(int)SIM.size()<<endl;

	     //end Zhufeng

        

		int num_initial_cl = 0;

		for(int cl_id=0; cl_id<(int)CL.size(); cl_id++)

		{

			

			//Added Zhufeng

			int machine_id = SIM[CL[cl_id].first.first-1].represented_machine_id;

			int Lambda_id=CL[cl_id].first.second;

			//End Zhufeng

			

			//Comment Zhufeng

			//int machine_id = SIM[CL[cl_id].first.first].represented_machine_id;

			//End Zhufeng

			

			if(Machine_vector[machine_id-1].hasInitialSetup

				//Added by Zhufeng

		&& Machine_vector[machine_id-1].initial_tooling_setup==Lambda_id

		//end by Zhufeng

		     )

			{

				num_initial_cl ++;

			}//end of if(Machine_vector[machine_id-1].hasInitialSetup			

		}

	

		if(num_initial_cl<temp){temp=num_initial_cl;}



	}else{

	

		if((int)CL.size()<temp){ temp=(int)CL.size();}



	}



	return temp;

	// --------- end yufen ----------------------

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

				int pos=tf*Max_Num_Temperature+set_id;

				//sum+=n_t_n_tooling[tf][set_id];

				sum+=n_t_n_tooling[pos];

				pm++;

			}

			tooling_sum_rhs.push_back(make_pair(make_pair(tf,n),sum));

		}

	}

}



//Comment by Zhufeng, Feb 2014,not used anymore

//void AT::greedy_assignment(int simid,int Lambda_id,vector<int> & lot_set,vector<int> & lot_selected,vector<int> & production){

//	vector<int> prod;

//	prod.resize((int)production.size());

//	copy(production.begin(),production.end(),prod.begin());

//	lot_selected.clear();

//	//-----------------------------------------------------------------------------------------for LL

//	vector< pair<int,double> > LL;

//	int i=SIM[simid-1].represented_machine_id;

//	int index=index_L_sim_lambda(simid,Lambda_id);

//	for(int id=1;id<=(int)L_sim_lambda[index].second.int_vector.size();id++){

//		int l=L_sim_lambda[index].second.int_vector[id-1];

//		vii pfind=find(lot_set.begin(),lot_set.end(),l);

//		if(pfind!=lot_set.end()){

//			int s=max_rate_s(i,l,Lambda_id);

//			double time_required=n_l_chips[l]/rate[s]+Load_Unload_Time;

//			int d=lots_device[l];

//			int type=is_KD_PD(d);

//			if(type==1 && prod[d-1]<n_k_minchips[d]){

//				int min=n_k_minchips[d]-prod[d-1];

//				if(min>n_l_chips[l]){ min=n_l_chips[l];}

//				LL.push_back(make_pair(l,(w_l[l]+penalty[d]/coef*min)/time_required));

//			}else if(type==2 && prod[d-1]<n_p_minchips[d]){//it is a package device

//				int min=n_p_minchips[d]-prod[d-1];

//				if(min>n_l_chips[l]){ min=n_l_chips[l];}

//				LL.push_back(make_pair(l,(w_l[l]+penalty[d]/coef*min)/time_required));

//			}else{

//				LL.push_back(make_pair(l,w_l[l]/time_required));

//			}

//		}//end_if

//	}

//	sort(LL.begin(),LL.end(),compare_lot);

//	//-----------------------------------------------------------------------------------------Assign

//	double time_used=0.0;

//	for(int id=1;id<=(int)LL.size();id++){

//		int l=LL[id-1].first;

//		int s=max_rate_s(i,l,Lambda_id);

//		double time_required=n_l_chips[l]/rate[s]+Load_Unload_Time;

//		if(time_used+time_required<=H[i]){

//			lot_selected.push_back(l);

//			time_used+=time_required;

//			int d=lots_device[l];

//			prod[d-1]+=n_l_chips[l];

//			//------------------------------------------------------------------------------updated LL

//			int type=is_KD_PD(d);

//			if(type==1){

//				bool flag=false;

//				int shortage=n_k_minchips[d]-prod[d-1];

//				for(int id2=id+1;id2<=(int)LL.size();id2++){

//					int l2=LL[id2-1].first;

//					int d2=lots_device[l2];

//					if(d2==d && shortage<n_l_chips[l2]){

//						int s2=max_rate_s(i,l2,Lambda_id);

//						double time2=n_l_chips[l2]/rate[s2]+Load_Unload_Time;

//						if(shortage>0){

//							LL[id2-1].second=(w_l[l2]+penalty[d2]/coef*shortage)/time2;

//							flag=true;

//						}else if(shortage<0 && shortage+n_l_chips[l]>0){

//							LL[id2-1].second=w_l[l2]/time2;

//							flag=true;

//						}

//					}

//				}//end_for

//				if(flag){

//					sort(LL.begin()+id,LL.end(),compare_lot);

//				}

//			}else if(type==2){

//				bool flag=false;

//				int shortage=n_p_minchips[d]-prod[d-1];

//				for(int id2=id+1;id2<=(int)LL.size();id2++){

//					int l2=LL[id2-1].first;

//					int d2=lots_device[l2];

//					if(d2==d && shortage<n_l_chips[l2]){

//						int s2=max_rate_s(i,l2,Lambda_id);

//						double time2=n_l_chips[l2]/rate[s2]+Load_Unload_Time;

//						if(shortage>0){

//							LL[id2-1].second=(w_l[l2]+penalty[d2]/coef*shortage)/time2;

//							flag=true;

//						}else if(shortage<0 && shortage+n_l_chips[l]>0){

//							LL[id2-1].second=w_l[l2]/time2;

//							flag=true;

//						}

//					}

//				}//end_for

//				if(flag){

//					sort(LL.begin()+id,LL.end(),compare_lot);

//				}

//			}//end_if

//			//--------------------------------------------------------------------------------------

//		}

//	}		

//	cout<<"time_use="<<time_used<<endl;

//}



//end by Zhufeng, Feb 2014

void AT::initialize_benefit(vector< pair<double,vec_int> > & sim_lambda_benefit,int * tooling_used,int * sim_used,int * SIM_number){

	cout<<"in initialize_benefit"<<endl;

	//---------------------------------------------------------------------------------------------------------for LL (sort all the lots according to their improtance)

	//Added by Zhufeng. Debug benefit

	ofstream f_benefit("Debug/sim_lambda_benefit_initialization.csv");

	f_benefit<<"simid,Represented Machine_id,Lambda_id,benefit,time_used"<<endl;

	//End Zhufeng

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

	sim_lambda_benefit.clear();

	int index=-1;

	

	for(int sim=1;sim<=(int)SIM.size();sim++){

		

		int machine_id=SIM[sim-1].represented_machine_id;

		vector<int>::iterator p_Lambda=Lambda_i[machine_id].int_vector.begin();

		while(p_Lambda!=Lambda_i[machine_id].int_vector.end()){



			int Lambda_id=(*p_Lambda);

			index++;

			pair<double,vec_int> one_pair;



			//---------------------------------------------------------------------------------------------------------test if sim_lambda is feasible

			int test=is_sim_lambda_feasible(sim,Lambda_id,tooling_used,sim_used,SIM_number);

			

			if(!test){ 

				one_pair.first=-1E+6;

				one_pair.second.int_vector.clear();

				sim_lambda_benefit.push_back(one_pair);

				p_Lambda++; 

				continue;

			}

		

			//---------------------------------------------------------------------------------------------------------for LLL

			vector< pair<int,double> > LLL;

			for(int id=1;id<=(int)LL.size();id++){

				int l=LL[id-1].first;

				vii pfind=find(L_sim_lambda[index].second.int_vector.begin(),L_sim_lambda[index].second.int_vector.end(),l);

				

				if(pfind!=L_sim_lambda[index].second.int_vector.end()){

					int s=max_rate_s(machine_id,l,Lambda_id);

					double time_required=n_l_chips[l]/rate[s]+Load_Unload_Time;

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

			

			//Added by Zhufeng, Feb 14, 2014

			bool already_beyond_horizon = false;

			//end by Zhufeng, Feb 14, 2014



			for(int id=1;id<=(int)LLL.size();id++){

				

				int l=LLL[id-1].first;

				int d=lots_device[l];

				int s=max_rate_s(machine_id,l,Lambda_id);

				double time_required=n_l_chips[l]/rate[s]+Load_Unload_Time;

				//Modified by Zhufeng, Feb 2014,

				if( (time_used+time_required) <=H[machine_id] ||

					((time_used+time_required) > H[machine_id] && !already_beyond_horizon && beyond_horizon ==1)){



					//end by Zhufeng, Feb 2014



					time_used+=time_required;



					//Added by Zhufeng, Feb 2014

					if(time_used > H[machine_id])

					{

						already_beyond_horizon = true;

					}

					//End by Zhufeng, Feb 2014

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

					



					if(type==1||is_kp||is_kpp){

						bool flag=false;

						for(int id2=id+1;id2<=(int)LLL.size();id2++){

							int l2=LLL[id2-1].first;

							int d2=lots_device[l2];

							int s2=max_rate_s(machine_id,l2,Lambda_id);

							double time2=n_l_chips[l2]/rate[s2]+Load_Unload_Time;

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

            



			// ------------ added by yufen -------------------

			if (Machine_vector[machine_id-1].hasInitialSetup)

			{

				//Added by Zhufeng

				if( Lambda_id==Machine_vector[machine_id-1].initial_tooling_setup)

				

			       {//end Zhufeng)

					   //modified by Zhufeng, to allow machine changing setups, when INF=0, not always fixed as initial setup. Feb 16.

				    one_pair.first = sum + INF;

					//end by Zhufeng,to allow machine changing setups, when INF=0, not always fixed as initial setup. Feb 16. 

					//Added by Zhufeng

			       }else

				   {

					   //comment by Zhufeng, to allow machine changing setups, when INF=0, not always fixed as initial setup. Feb 16.

					 one_pair.first=sum;

					//end by Zhufeng, Feb 26



				   }//end by Zhufeng

			}



			// ------------ end yufen -----------------------

			//Added by Zhufeng

			f_benefit<<sim<<","<<machine_id<<","<<Lambda_id<<","<<one_pair.first<<","<<time_used<<endl;

            //end Zhufeng

			sim_lambda_benefit.push_back(one_pair);

			p_Lambda++;

		}

	}

	//Added by Zhufeng

	f_benefit.close();

    //End Zhufeng	  



}



void AT::update_benefit(vector< pair<double,vec_int> > & sim_lambda_benefit,vector<int> & lot_remain,vector<int> & production,vector<int> & affected_dev,int * tooling_used,int * sim_used,int * SIM_number){

	//cout<<"in update_benefit()"<<endl;

	//the vector <sim_lambda_benefit> was built by initialize_benefit(). Note that infeasible setup is also included in the vector. The vector is of the same size as <L_sim_lambda>

	//the values in <sim_lambda_benefit> are never used in the right hand side of =. That is, it is fine to use a <sim_lambda_benefit> with all 0 benefits (This statement is for AT enhancement.)

	int index=-1;

	/*

	cout<<"original benefit"<<endl;

	for(int sim=1;sim<=(int)SIM.size();sim++){

		int machine_id=SIM[sim-1].represented_machine_id;

		vector<int>::iterator p_Lambda=Lambda_i[machine_id].int_vector.begin();

		while(p_Lambda!=Lambda_i[machine_id].int_vector.end()){

			int Lambda_id=(*p_Lambda);

			index++;

			cout<<"index="<<index<<",sim="<<sim<<",Lambda_id="<<Lambda_id<<",benefit="<<sim_lambda_benefit[index].first<<endl;

			p_Lambda++;

		}

	}

	*/

	//------------copy production

	cout<<"copy production"<<endl;	

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

	cout<<"initialize LL"<<endl;	

	vector< pair<int,double> > LL; //<lot_id, benefit>

	for(int id=1;id<=(int)lot_remain.size();id++){

		//for each remaining lot,compute the corresponding benefit based on current production	

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

		if(type==1 && prod[d-1]<n_k_minchips[d]){//if the device is a key device,need to compute the reduction of penalty

			int min=n_l_chips[l];

			if(min>n_k_minchips[d]-prod[d-1]){ min=n_k_minchips[d]-prod[d-1];}

			one_pair.second+=penalty[d]/coef*min;

		}else if(type==2 && prod[d-1]<n_p_minchips[d]){//if the device is a package device (this is not used anymore)

			int min=n_l_chips[l];

			if(min>n_p_minchips[d]-prod[d-1]){ min=n_p_minchips[d]-prod[d-1];}

			one_pair.second+=penalty[d]/coef*min;

		}

		if(is_kp && PKG_production_save[pkg_id-1]<n_kp_minchips[pkg_id]){//if the device contains a key package

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

	sort(LL.begin(),LL.end(),compare_lot); //the remaining lots are sorted according to the benefits

	//------------------------------------------------------------------------------------------------

	cout<<"update the sim_lambda_benefit"<<endl;	

	int count=0;

	int count_limit=1E+6;

	index=-1;

	for(int sim=1;sim<=(int)SIM.size();sim++){

		int machine_id=SIM[sim-1].represented_machine_id;

		vector<int>::iterator p_Lambda=Lambda_i[machine_id].int_vector.begin();

		while(p_Lambda!=Lambda_i[machine_id].int_vector.end()){

			//for each SIM-Lambda combination,update the corresponding benefit can be obtained	

			int Lambda_id=(*p_Lambda);

			index++;

			count++;

			//some initial test to save effeor

			//--------------------------------------------------------------------------------------------

			if(sim_lambda_benefit[index].first<0){//previosly infeasible or non-benefitial setup

				p_Lambda++; continue;

			}

			//---------------------------------------------------------------------------------------------------------test if sim_lambda is feasible

			int test=is_sim_lambda_feasible(sim,Lambda_id,tooling_used,sim_used,SIM_number);

			if(!test){ 

				sim_lambda_benefit[index].first=-1E+6;

				sim_lambda_benefit[index].second.int_vector.clear();

				p_Lambda++; 

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

			for(int id=1;id<=(int)sim_lambda_benefit[index].second.int_vector.size();id++){

				int l=sim_lambda_benefit[index].second.int_vector[id-1];

				int d=lots_device[l];

				vii pfind=find(affected_dev.begin(),affected_dev.end(),d);

				if(pfind!=affected_dev.end()){

					affected=true;

					break;

				}

			}

			if(!affected){ p_Lambda++; continue;}

			*/

			//--------------------------------------------------------if affected, clear

			sim_lambda_benefit[index].second.int_vector.clear();

			sim_lambda_benefit[index].first=0;

			//---------------------------------------------------------------------------------------------------------for LLL

			vector< pair<int,double> > LLL;

			for(int id=1;id<=(int)LL.size();id++){

				int l=LL[id-1].first;

				vii pfind=find(L_sim_lambda[index].second.int_vector.begin(),L_sim_lambda[index].second.int_vector.end(),l); //index also can be used in L_sim_lambda[]

				if(pfind!=L_sim_lambda[index].second.int_vector.end()){

					//only consider the lots that can be processed by the given SIM-Lambda combination	

					int s=max_rate_s(machine_id,l,Lambda_id);

					double time_required=n_l_chips[l]/rate[s]+Load_Unload_Time;

					double ben=LL[id-1].second/time_required; //compute the benefit gained in each time unit

					LLL.push_back(make_pair(l,ben));

				}

			}

			sort(LLL.begin(),LLL.end(),compare_lot);

			//---------------------------------------------------------------------------------------------------------for LLL

			double sum=0;

			double time_used=0;



			//Added by Zhufeng, Feb 14, 2014

			bool already_beyond_horizon = false;

			//end by Zhufeng, Feb 14, 2014



			for(int id=1;id<=(int)LLL.size();id++){

				int l=LLL[id-1].first;

				int d=lots_device[l];

				int s=max_rate_s(machine_id,l,Lambda_id);

				double time_required=n_l_chips[l]/rate[s]+Load_Unload_Time;

				

				//Modified by Zhufeng, Feb 2014

				//if(time_used+time_required<=H[machine_id]){

				if( (time_used+time_required) <=H[machine_id] ||

					((time_used+time_required) > H[machine_id] && !already_beyond_horizon && beyond_horizon ==1)){



					//end by Zhufeng, Feb 2014



					time_used+=time_required;



					//Added by Zhufeng, Feb 2014

					if(time_used > H[machine_id])

					{

						already_beyond_horizon = true;

					}

					//End by Zhufeng, Feb 2014



					sum+=LLL[id-1].second*time_required;

					sim_lambda_benefit[index].second.int_vector.push_back(l);

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

					//------------------------------------------------update and sort, since the first lot is selected, the rest of the lots need to be sorted again!!!

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

								int s2=max_rate_s(machine_id,l2,Lambda_id);

								double time2=n_l_chips[l2]/rate[s2]+Load_Unload_Time;

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

								int s2=max_rate_s(machine_id,l2,Lambda_id);

								double time2=n_l_chips[l2]/rate[s2]+Load_Unload_Time;

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

							int s2=max_rate_s(machine_id,l2,Lambda_id);

							double time2=n_l_chips[l2]/rate[s2]+Load_Unload_Time;

							LLL[id2-1].second=w_l[l2]/time2;

							if(type==1&&d2==d){

								//if the lot under investigation contains the same key device

								//then the original benefit should be updated b/c a lot has been selected	

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

			sim_lambda_benefit[index].first=sum;

			

           // ------------ added by yufen -------------------

			if (Machine_vector[machine_id-1].hasInitialSetup)

			{

				//Added by Zhufeng

				if( Lambda_id==Machine_vector[machine_id-1].initial_tooling_setup)

				

			       {//end Zhufeng)

				   sim_lambda_benefit[index].first = sum + INF;

					//Added by Zhufeng

			       }else

				   {

					   //comment by Zhufeng, Feb 16, allow machines changing its setups,if INF==0, not fixed as initia lsetup all the time

					   sim_lambda_benefit[index].first=sum;

					   //end by Zhufeng, Feb 16.

				   }//end by Zhufeng

			}



			// ------------ end yufen -----------------------

			p_Lambda++;

			if(count>count_limit){ break;}

		}//end of while(p_Lambda!=Lambda_i[machine_id].int_vector.end())

		if(count>count_limit){ break;}

	}

	//cout<<"count="<<count<<endl;



	/*

	cout<<"updated benefit"<<endl;

	index=-1;

	for(int sim=1;sim<=(int)SIM.size();sim++){

		int machine_id=SIM[sim-1].represented_machine_id;

		vector<int>::iterator p_Lambda=Lambda_i[machine_id].int_vector.begin();

		while(p_Lambda!=Lambda_i[machine_id].int_vector.end()){

			int Lambda_id=(*p_Lambda);

			index++;

			cout<<"index="<<index<<",sim="<<sim<<",Lambda_id="<<Lambda_id<<",benefit="<<sim_lambda_benefit[index].first<<endl;

			//for(int id=1;id<=(int)sim_lambda_benefit[index].second.int_vector.size();id++){

			//	cout<<sim_lambda_benefit[index].second.int_vector[id-1]<<",";

			//}

			//cout<<endl;

			p_Lambda++;

		}

	}

	*/

	//cout<<"end of update_benefit()"<<endl;

}



void AT::update_benefit2(vector< pair<double,vec_int> > & sim_lambda_benefit,vector<int> & lot_remain,vector<int> & production

						 ,vector<int> & affected_dev,int * tooling_used,int * sim_used,int * SIM_number,OsiXxxSolverInterface & si_KP){

	//cout<<"in update_benefit2()"<<endl;

	int index=-1;

	/*

	cout<<"original benefit"<<endl;

	for(int sim=1;sim<=(int)SIM.size();sim++){

		int machine_id=SIM[sim-1].represented_machine_id;

		vector<int>::iterator p_Lambda=Lambda_i[machine_id].int_vector.begin();

		while(p_Lambda!=Lambda_i[machine_id].int_vector.end()){

			int Lambda_id=(*p_Lambda);

			index++;

			cout<<"index="<<index<<",sim="<<sim<<",Lambda_id="<<Lambda_id<<",benefit="<<sim_lambda_benefit[index].first<<endl;

			p_Lambda++;

		}

	}

	*/

	//------------------------------------------------------------------------------------------------

	double * solution = new double[Num_Lot+Num_Keydevice+Num_Package];

	index=-1;

	for(int sim=1;sim<=(int)SIM.size();sim++){

		int machine_id=SIM[sim-1].represented_machine_id;

		vector<int>::iterator p_Lambda=Lambda_i[machine_id].int_vector.begin();

		while(p_Lambda!=Lambda_i[machine_id].int_vector.end()){

			int Lambda_id=(*p_Lambda);

			index++;

			//--------------------------------------------------------------------------------------------

			if(sim_lambda_benefit[index].first<0){//previosly infeasible or non-benefitial setup

				p_Lambda++; continue;

			}

			//---------------------------------------------------------------------------------------------------------test if sim_lambda is feasible

			int test=is_sim_lambda_feasible(sim,Lambda_id,tooling_used,sim_used,SIM_number);

			if(!test){ 

				sim_lambda_benefit[index].first=-1E+6;

				sim_lambda_benefit[index].second.int_vector.clear();

				p_Lambda++; 

				continue;

			}

			//---------------------------------------------------------------------------------------------------------check if affected dev

			bool affected=false;

			for(int id=1;id<=(int)sim_lambda_benefit[index].second.int_vector.size();id++){

				int l=sim_lambda_benefit[index].second.int_vector[id-1];

				int d=lots_device[l];

				vii pfind=find(affected_dev.begin(),affected_dev.end(),d);

				if(pfind!=affected_dev.end()){

					affected=true;

					break;

				}

			}

			if(!affected){ p_Lambda++; continue;}

			//--------------------------------------------------------if affected, clear

			sim_lambda_benefit[index].second.int_vector.clear();

			sim_lambda_benefit[index].first=0;

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

				vii pfind=find(L_sim_lambda[index].second.int_vector.begin(),L_sim_lambda[index].second.int_vector.end(),l);

				if(pfind==L_sim_lambda[index].second.int_vector.end()){//lot l cannot be processed by (sim,Lambda_id) combination

					si_KP.setColBounds(l-1,0.0,0.0);

					id_vec.push_back(l-1);

				}else{//lot l can be processed

					int s=max_rate_s(machine_id,l,Lambda_id);

					double time_required=n_l_chips[l]/rate[s]+Load_Unload_Time;

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

						//cout<<"sim="<<sim<<",Lambda_id="<<Lambda_id<<",lot="<<l<<",w_l="<<w_l[l]<<"d,="<<d<<",type="<<is_KD_PD(d)<<endl;

						int s=max_rate_s(machine_id,l,Lambda_id);

						benefit+=w_l[l]-eps[s];

						prod[d-1]+=n_l_chips[l];

						sim_lambda_benefit[index].second.int_vector.push_back(l);

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

				//cout<<"sim="<<sim<<",Lambda_id="<<Lambda_id<<",KP_obj="<<KP_obj<<",benefit="<<benefit<<endl;

				sim_lambda_benefit[index].first=benefit;

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

			p_Lambda++;

		}

	}

	/*

	cout<<"updated benefit2"<<endl;

	index=-1;

	for(int sim=1;sim<=(int)SIM.size();sim++){

		int machine_id=SIM[sim-1].represented_machine_id;

		vector<int>::iterator p_Lambda=Lambda_i[machine_id].int_vector.begin();

		while(p_Lambda!=Lambda_i[machine_id].int_vector.end()){

			int Lambda_id=(*p_Lambda);

			index++;

			cout<<"index="<<index<<",sim="<<sim<<",Lambda_id="<<Lambda_id<<",benefit="<<sim_lambda_benefit[index].first<<endl;

			p_Lambda++;

		}

	}

	*/

	//cout<<"end of update_benefit2()"<<endl;

	delete [] solution;

}





void AT::update_benefit_resetup(int current_machine,int current_Lambda,double current_time,vector< pair<double,vec_int> > & machine_lambda_benefit,vector<int> & lot_remain,vector<int> & production,vector<int> & affected_dev,int * tooling_used,int * sim_used,int * SIM_number,vector<int> & idle_machines){

	//instead of update the benefit according to sim-lambda combination, it is necessary to use machine-lambda combination because at this point

	//machines in the sim are not identical anymore due the different H[]	

	//cout<<"in update_benefit_resetup()"<<endl;

	//the values in <machine_lambda_benefit> are never used in the right hand side of =. Thus the <machine_lambda_benefit> will not affect the values of other variables. That is, it is fine to use a <machine_lambda_benefit> with all 0 benefits (This statement is for AT enhancement.)

	int index=-1;

	//------------copy production for the devices

	vector<int> prod((int)production.size(),0);

	copy(production.begin(),production.end(),prod.begin());

	//--------------------------------------------------------compute the production for package and pin package

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

	//This segment of code sorts the remaining lots according to their benefit

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

	sort(LL.begin(),LL.end(),compare_lot); //the remaining lots are sorted according to the benefits

	//------------------------------------------------------------------------------------------------

	//The following segment of code iterats over all machine-lambda combinations. For each iteration,lots are picked from the sorted LL and assigned to the

	//selected machine-lambda combination. Once a lot is selcted and assigned,the benefit of the remaining lots need to be updated accordingly.Thus an updating

	//procedure need to be applied each time a lot is assigned. This reupdate costs a lot of computational time. 

	//In the case of resetup machines,change over setup time will be caused for each resetup, except the situation that the machine-lambda under investigation

	//is the current machine-lambda combination. That is, it is not necessary to change the tooling setup for the current machine.

	//Once change over time is incured,the available machine time H[] is reduced by the same amount of time.

	int count=0;

	int count_limit=1E+6;

	index=-1;

	for(int machine_id=1;machine_id<=(int)Machine_vector.size();machine_id++){

		//test if the machine_id is idle (available or not)	

		vii pfind=find(idle_machines.begin(),idle_machines.end(),machine_id);

		bool machine_idle_flag=true;	

		if(pfind==idle_machines.end()){//machine_id is NOT idle 

			machine_idle_flag=false;	

		}	

		int sim=machine_sim[machine_id];

		vector<int>::iterator p_Lambda=Lambda_i[machine_id].int_vector.begin();

		while(p_Lambda!=Lambda_i[machine_id].int_vector.end()){

			//the inner loop for each machine-lambda combination

			int Lambda_id=(*p_Lambda);

			//---------------------------------------------------compute the resetup time

			double resetup_time=0;

			if(machine_id!=current_machine || Lambda_id!=current_Lambda){//unmatched machine or unmatch Lambda_id

				vector<Lambda_item>::iterator pL=Lambda.begin()+Lambda_id-1;

				for(int j=1;j<=(int)(*pL).tooling_vec.size();j++){

					int tf=(*pL).tooling_vec[j-1];

					resetup_time+=Tooling_Setup_Time[tf];

				}

			}

			index++;

			count++;

			//--------------------------------------------------------------------------------------------

			if(machine_lambda_benefit[index].first<0){//previosly infeasible or non-benefitial setup

				p_Lambda++; continue;

			}

			//-----------------------------------------------------------------------------------------------test if machine_lambda is feasible

			int test=is_sim_lambda_feasible(sim,Lambda_id,tooling_used,sim_used,SIM_number);

			if(!test || !machine_idle_flag){//if the machine-lambda is infeasible

				machine_lambda_benefit[index].first=-1E+6;

				machine_lambda_benefit[index].second.int_vector.clear();

				p_Lambda++; 

				continue;

			}

			//-------------------------------------------------------------------------------if the current_time plus setup time exceed the horizon, then it is not interesed to resetup

			if(current_time+resetup_time>H[machine_id]){

				//cout<<"current_time="<<current_time<<",resetup_time="<<resetup_time<<",H="<<H[machine_id]<<endl;

				p_Lambda++;continue;

			}

			//--------------------------------------------------------------------------

			//upto this point,the machine-lambda is feasible and should be investigated

			//---------------------------------------------------------------make a copy of the production such that it will not be destroyed

			copy(production.begin(),production.end(),prod.begin());

			vector<int> PKG_production((int)PKG.size(),0);

			vector<int> PIN_PKG_production((int)PKG.size()*(int)PIN_order.size()+1,0);

			copy(PKG_production_save.begin(),PKG_production_save.end(),PKG_production.begin());

			copy(PIN_PKG_production_save.begin(),PIN_PKG_production_save.end(),PIN_PKG_production.begin());

			//---------------------------------------------------------------------------------------------------------check if affected dev

			/*

			bool affected=false;

			for(int id=1;id<=(int)machine_lambda_benefit[index].second.int_vector.size();id++){

				int l=machine_lambda_benefit[index].second.int_vector[id-1];

				int d=lots_device[l];

				vii pfind=find(affected_dev.begin(),affected_dev.end(),d);

				if(pfind!=affected_dev.end()){

					affected=true;

					break;

				}

			}

			if(!affected){ p_Lambda++; continue;}

			*/

			//--------------------------------------------------------if affected, clear

			machine_lambda_benefit[index].second.int_vector.clear();

			machine_lambda_benefit[index].first=0;

			//---------------------------------------------------------------------------------------------------------for LLL

			vector< pair<int,double> > LLL;

			for(int id=1;id<=(int)LL.size();id++){

				int l=LL[id-1].first;

				int pos=index_L_sim_lambda(sim,Lambda_id);	

				vii pfind=find(L_sim_lambda[pos].second.int_vector.begin(),L_sim_lambda[pos].second.int_vector.end(),l); 

				if(pfind!=L_sim_lambda[pos].second.int_vector.end()){

					int s=max_rate_s(machine_id,l,Lambda_id);

					double time_required=n_l_chips[l]/rate[s]+Load_Unload_Time;

					double ben=LL[id-1].second/time_required;

					LLL.push_back(make_pair(l,ben));

				}

			}

			sort(LLL.begin(),LLL.end(),compare_lot);

			//---------------------------------------------------------------------------------------------------------for LLL

			double sum=0;

			double time_used=current_time+resetup_time; //if the resetup costs time, it should be taken into accout in the time used

			//cout<<"sim="<<sim<<",machine_id="<<machine_id<<",Lambda_id="<<Lambda_id<<",time_used="<<time_used<<",H="<<H[machine_id]<<endl;	



			//Added by Zhufeng, Feb 14, 2014

			bool already_beyond_horizon = false;

			//end by Zhufeng, Feb 14, 2014



			for(int id=1;id<=(int)LLL.size();id++){

				int l=LLL[id-1].first;

				int d=lots_device[l];

				int s=max_rate_s(machine_id,l,Lambda_id);

				double time_required=n_l_chips[l]/rate[s]+Load_Unload_Time;

				//cout<<"l="<<l<<",time_required="<<time_required<<endl;	



				//Modified by Zhufeng, Feb 2014

				//if(time_used+time_required<=H[machine_id]){

				if( (time_used+time_required) <=H[machine_id] ||

					((time_used+time_required) > H[machine_id] && !already_beyond_horizon && beyond_horizon ==1)){



					//end by Zhufeng, Feb 2014



					time_used+=time_required;



					//Added by Zhufeng, Feb 2014

					if(time_used > H[machine_id])

					{

						already_beyond_horizon = true;

					}

					//End by Zhufeng, Feb 2014



					//cout<<"true,time_used="<<time_used<<endl;	

					sum+=LLL[id-1].second*time_required;

					machine_lambda_benefit[index].second.int_vector.push_back(l);

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

					//------------------------------------------------update and sort, since the first lot is selected, the rest of the lots need to be sorted again!!!

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

								int s2=max_rate_s(machine_id,l2,Lambda_id);

								double time2=n_l_chips[l2]/rate[s2]+Load_Unload_Time;

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

								int s2=max_rate_s(machine_id,l2,Lambda_id);

								double time2=n_l_chips[l2]/rate[s2]+Load_Unload_Time;

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

							int s2=max_rate_s(machine_id,l2,Lambda_id);

							double time2=n_l_chips[l2]/rate[s2]+Load_Unload_Time;

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

			machine_lambda_benefit[index].first=sum;

			p_Lambda++;

			if(count>count_limit){ break;}

		}

		if(count>count_limit){ break;}

	}

	//cout<<"count="<<count<<endl;

	//cout<<"end of update_benefit()"<<endl;

}



void AT::initialize_L_sim_lambda(){

	L_sim_lambda.clear();

	for(int sim=1;sim<=(int)SIM.size();sim++){

		int i=SIM[sim-1].represented_machine_id;



		//Comment by Zhufeng, Feb 26, 2012

		//vector<int>::iterator p_Lambda=Lambda_i[i].int_vector.begin();

		//while(p_Lambda!=Lambda_i[i].int_vector.end()){

		//end by Zhufeng, Feb 26, 2012



		//Added by Zhufeng, Feb 26, 2012

		vector<int>::iterator p_Lambda=Lambda_i[i].int_vector.begin();

		while(p_Lambda!=Lambda_i[i].int_vector.end()){

			//end by Zhufeng, Feb 26, 2012



			int Lambda_id=(*p_Lambda);

			vector<int> LS;

			get_L_i_lambda(LS,i,Lambda_id);

			pair< pair<int,int>,vec_int> one_pair;

			one_pair.first.first=sim;

			one_pair.first.second=Lambda_id;

			one_pair.second.int_vector.resize((int)LS.size());

			copy(LS.begin(),LS.end(),one_pair.second.int_vector.begin());

			L_sim_lambda.push_back(one_pair);

			p_Lambda++;

		}

	}

}



void AT::show_L_sim_lambda(){

	ofstream fdebug_L_i_lambda("Debug/debug_L_i_lambda.txt");

	vector< pair< pair<int,int>,vec_int> >::iterator pLS=L_sim_lambda.begin();

	while(pLS!=L_sim_lambda.end()){

		fdebug_L_i_lambda<<"sim="<<(*pLS).first.first<<",Lambda_id="<<(*pLS).first.second<<",lots:";

		vii p_lot=(*pLS).second.int_vector.begin();

		while(p_lot!=(*pLS).second.int_vector.end()){

			fdebug_L_i_lambda<<(*p_lot)<<",";

			p_lot++;

		}

		fdebug_L_i_lambda<<endl;

		pLS++;

	}

	fdebug_L_i_lambda.close();

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

		int Lambda_id=(*p_node).Lambda_id;

		fdebugLP<<"i="<<i<<",l="<<l<<",Lambda_id="<<Lambda_id<<",y="<<solution[id]<<endl;

	}

	fdebugLP.close();

	*/

	//initialize sub_soln

	//< < <i,Lambda_id>,time_used>,lot1,lot2,...>

	for(int i=1;i<=Num_Machine;i++){

		sub_soln_item one_item;

		one_item.i=i;

		one_item.Lambda_id=-1;

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

			// --- added by yufen --

			sub_soln[i-1].s = s;

			// --- end yufen ---

			sub_soln[i-1].lots.int_vector.push_back(l);

			sub_soln[i-1].time_used+=n_l_chips[l]/rate[s]+Load_Unload_Time;

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

			int Lambda_id=(*p_node).Lambda_id;

			sub_soln[i-1].Lambda_id=Lambda_id;

			// --- added by yufen --

			int s=(*p_node).s;

			sub_soln[i-1].s = s;

			// --- end yufen ---

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

	cout<<"Number of lots="<<Num_Lot<<endl;

	cout<<"Number of assigned lots="<<(int)assigned_lots.size()<<endl;

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

		vector< pair<int,int> >::iterator p_pair=sim_lambda_L[l-1].second.sim_lambda_pairs.begin();

		while(p_pair!=sim_lambda_L[l-1].second.sim_lambda_pairs.end()){

			int sim=(*p_pair).first;

			int Lambda_id=(*p_pair).second;

			int s=max_rate_s(SIM[sim-1].represented_machine_id,l,Lambda_id);

			double time_required=n_l_chips[l]/rate[s]+Load_Unload_Time;

			for(int id2=1;id2<=(int)sub_soln.size();id2++){

				int i=sub_soln[id2-1].i;

				if(sim==machine_sim[i] && Lambda_id==sub_soln[id2-1].Lambda_id 

					&& sub_soln[id2-1].time_used+time_required<=H[i]){

					//lot l can be assigned to this machine-lambda combination

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

	//cout<<"	number of pkg = "<<PKG_production.size()<<endl;

	//cout<<"	number of PIN_PKG = "<<PIN_PKG_production.size()<<endl;

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

			int Lambda_id_best=-1;

			int l_to_best=-1;

			double ben_best=1E+10;

			vector< pair<int,int> >::iterator p_pair=sim_lambda_L[l_from-1].second.sim_lambda_pairs.begin();

			while(p_pair!=sim_lambda_L[l_from-1].second.sim_lambda_pairs.end()){

				int sim=(*p_pair).first;

				int Lambda_id=(*p_pair).second;

				//cout<<"--------------------------------------------------"<<endl;

				//cout<<"sim="<<sim<<"	";

				//cout<<"machine="<<SIM[sim-1].represented_machine_id<<"	"<<Machine_vector[SIM[sim-1].represented_machine_id-1].name<<"	";

				//cout<<"lot="<<l_from<<"	"<<lot_name[l_from-1].char_arry<<"	";

				//cout<<"device="<<lots_device[l_from]<<"	"<<device_name[lots_device[l_from]-1].char_arry<<"	";

				//cout<<Lambda_id<<endl;



				int s_from=max_rate_s(SIM[sim-1].represented_machine_id,l_from,Lambda_id);



				double time_required=n_l_chips[l_from]/rate[s_from]+Load_Unload_Time;

				for(int id2=1;id2<=(int)sub_soln.size();id2++){

					int i=sub_soln[id2-1].i;

					// ---- commented by yufen -------

					//if(sim==machine_sim[i] && Lambda_id==sub_soln[id2-1].Lambda_id){

					// ----- end yufen



					//---- added by yufen ---

					if(sub_soln[id2-1].lots.int_vector.size()>0 && sim==machine_sim[i] && Lambda_id==sub_soln[id2-1].Lambda_id){

					//---- end yufen -----						

						//cout<<"	machine= "<<Machine_vector[i-1].name<<"	sim= "<<machine_sim[i]<<"	lambda_id="<<sub_soln[id2-1].Lambda_id<<" == "<< Machine_vector[i-1].initial_tooling_setup<<endl;

						

						//lot l_from can be assigned to this machine-lambda combination

						vii pint=sub_soln[id2-1].lots.int_vector.begin();

						//cout<<"	lot= "<<(*pint)<<"	"<<lot_name[(*pint)-1].char_arry<<endl;



						while(pint!=sub_soln[id2-1].lots.int_vector.end()){

							int l_to=(*pint);  //this is the lot to be exchange

							int d_to=lots_device[l_to];

							//cout<<"2, Lambda_id="<<Lambda_id<<"	";

							int s_to=max_rate_s(i,l_to,Lambda_id);

							//cout<<s_to<<endl;

							if(sub_soln[id2-1].time_used-(n_l_chips[l_to]/rate[s_to]+Load_Unload_Time)+time_required<=H[i]){

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

									Lambda_id_best=Lambda_id;

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

				//cout<<"i_best="<<i_best<<endl;	

				vii pint=find(sub_soln[i_best-1].lots.int_vector.begin(),sub_soln[i_best-1].lots.int_vector.end(),l_to_best);

				if(pint!=sub_soln[i_best-1].lots.int_vector.end()){

					(*pint)=l_from;

				}else{

					cout<<"!!! error here"<<endl;

				}

				//cout<<"3,Lambda_id_best="<<Lambda_id_best;

				int s_to_best=max_rate_s(i_best,l_to_best,Lambda_id_best);

				//cout<<s_to_best<<endl;

				//cout<<"4,Lambda_id_best="<<Lambda_id_best;

				int s_from_best=max_rate_s(SIM[sim_best-1].represented_machine_id,l_from,Lambda_id_best);

				//cout<<s_from_best<<endl;

				sub_soln[i_best-1].time_used+=-(n_l_chips[l_to_best]/rate[s_to_best]+Load_Unload_Time)+(n_l_chips[l_from]/rate[s_from_best]+Load_Unload_Time);

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

			int Lambda_id=sub_soln[i-1].Lambda_id;

			//cout<<"l_from="<<l_from<<",d_from="<<d_from<<",i="<<i<<",Lambda_id="<<Lambda_id<<endl;



			if(Lambda_id>0){

				int sim=machine_sim[i];

				int index=index_L_sim_lambda(sim,Lambda_id);

				vii pfind=find(L_sim_lambda[index].second.int_vector.begin(),L_sim_lambda[index].second.int_vector.end(),l_from);

				if(pfind==L_sim_lambda[index].second.int_vector.end()){ continue;} 

				//-----------------------------------------------------------------------------------------

				int s_from=max_rate_s(i,l_from,Lambda_id);

				double time_from=n_l_chips[l_from]/rate[s_from]+Load_Unload_Time;

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

						int s=max_rate_s(i,l,Lambda_id);

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

						int s=max_rate_s(i,l,Lambda_id);

						lots.erase(lots.end()-1);

						lot_removed.push_back(l);

						//cout<<"l="<<l<<",d="<<d<<",sh="<<sh[d-1]<<",time_required="<<time_required<<",ben_loss="<<ben_loss<<endl;

						//--------------------------------------------------------------------------------------update the remove_ben

						sh[d-1]+=n_l_chips[l];

						if(is_KD_PD(d)){

							for(int j=1;j<=(int)lots.size();j++){

								int ll=lots[j-1].first;

								int dd=lots_device[ll];

								int ss=max_rate_s(i,ll,Lambda_id);

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

						time_required-=(n_l_chips[l]/rate[s]+Load_Unload_Time);

						ben_loss+=lots[(int)lots.size()-1].second;

					}//end_while

					if(time_required<H[i]){

						sub_soln[i-1].lots.int_vector.clear();

						sub_soln[i-1].lots.int_vector.resize((int)lots.size());

						double time=0;

						for(int j=1;j<=(int)lots.size();j++){

							int l=lots[j-1].first;

							int s=max_rate_s(i,l,Lambda_id);

							time+=n_l_chips[l]/rate[s]+Load_Unload_Time;

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

		if(sub_soln[i-1].Lambda_id>0){//the machine is used

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

			int Lambda_save=sub_soln[i-1].Lambda_id;

			sub_soln[i-1].Lambda_id=-1;

			//----------------------------------------------------------construction

			int simid=machine_sim[i];

			vii p_Lambda=Lambda_i[i].int_vector.begin();

			while(p_Lambda!=Lambda_i[i].int_vector.end()){

				int Lambda_id=(*p_Lambda);

				if(Lambda_id==Lambda_save){ continue;}

				vector< pair<int,double> > lot_to_assigne;

				int index=index_L_sim_lambda(simid,Lambda_id);

				pair< pair<int,double>,double> p_unassigned=unassigned_lots.begin();

				while(p_unassigned!=unassigned_lots.end()){

					int l=(*p_unassigned).first.first;

					vii pfind=find(L_sim_lambda[index].second.int_vector.begin(),L_sim_lambda[index].second.int_vector.end(),l);

					if(pfind!=L_sim_lambda[index].second.int_vector.end()){

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



				p_Lambda++;

			}

		}

	}

}

*/



void AT::initialize_sim_lambda_L(){

	//given a lot, find all the (sim,lambda) combinations

	for(int l=1;l<=Num_Lot;l++){

		pair<int,sim_lambda_item> one_pair;

		one_pair.first=l;

		sim_lambda_L.push_back(one_pair);

	}

	for(int l=1;l<=Num_Lot;l++){

		int index=-1;

		for(int sim=1;sim<=(int)SIM.size();sim++){

			int i=SIM[sim-1].represented_machine_id;



			//comment by Zhufeng, Feb 26, 2012

			//vii p_Lambda=Lambda_i[i].int_vector.begin();

			//while(p_Lambda!=Lambda_i[i].int_vector.end()){

			//end by Zhufeng, Feb 26, 2012



			vii p_Lambda=Lambda_i[i].int_vector.begin();

			while(p_Lambda!=Lambda_i[i].int_vector.end()){





				int Lambda_id=(*p_Lambda);

				index++;

				vii pfind=find(L_sim_lambda[index].second.int_vector.begin(),L_sim_lambda[index].second.int_vector.end(),l);

				if(pfind!=L_sim_lambda[index].second.int_vector.end()){

					sim_lambda_L[l-1].second.sim_lambda_pairs.push_back(make_pair(sim,Lambda_id));

				}

				p_Lambda++;

			}

		}

	}

}



void AT::show_sim_lambda_L(){

	ofstream fout("Debug/debug_sim_lambda_L.txt");

	for(int l=1;l<=Num_Lot;l++){

		fout<<"lot="<<l<<endl;

		vector< pair<int,int> >::iterator p_pair=sim_lambda_L[l-1].second.sim_lambda_pairs.begin();

		while(p_pair!=sim_lambda_L[l-1].second.sim_lambda_pairs.end()){

			fout<<"("<<(*p_pair).first<<","<<(*p_pair).second<<")"<<",";

			p_pair++;

		}

		fout<<endl;

	}

	fout.close();

}





// Comment by Zhufeng for logpoint, Sep 01, 2011

//int AT::max_rate_s(int i,int l,int Lambda_id){

//	int sim=machine_sim[i];

//	int d=lots_device[l];

//	int index=index_S_sim_d_lambda(sim,d,Lambda_id);

//	return S_sim_d_lambda[index].s;

//}

// end by Zhufeng for logpoint





//Added by Zhufeng for logpoint in Sep 01, 2011

int AT::max_rate_s(int i,int l,int Lambda_id){
	
	int sim=machine_sim[i];
	
	int index=index_S_sim_l_lambda(sim,l,Lambda_id);
	return S_sim_l_lambda[index].s;

}

void AT::initialize_S_sim_l_lambda(){
	int count=1; // count is to record the id of the element in the S_sim_l_lambda
	sim_position_S_sim_l_lambda.push_back(count);
	for(int sim=1;sim<=(int)SIM.size();sim++){
		int i=SIM[sim-1].represented_machine_id;
		//cout<<"i="<<i<<endl;

		//comment  by Zhufeng, Feb 26,2012

		//vii p_Lambda=Lambda_i[i].int_vector.begin();
		//while(p_Lambda!=Lambda_i[i].int_vector.end()){

		//end by Zhufeng, Feb 26,2012



		//Added by Zhufeng, Feb 26, 2012
		vii p_Lambda=Lambda_i[i].int_vector.begin();
		while(p_Lambda!=Lambda_i[i].int_vector.end()){
		//end by Zhufeng, Feb 26, 2012

			int Lambda_id=(*p_Lambda);
			//cout<<"Lambda_id="<<Lambda_id<<endl;
			int index=index_L_sim_lambda(sim,Lambda_id);
			//cout<<"index="<<index<<endl;
			vector<int>::iterator p_L=L_sim_lambda[index].second.int_vector.begin();
			while(p_L!=L_sim_lambda[index].second.int_vector.end()){
				int l=(*p_L);
				//cout<<"l="<<l<<endl;
				//int d=lots_device[l];
				int flag=0;
				for(int id=1;id<=(int)S_sim_l_lambda.size();id++){
					if(sim==S_sim_l_lambda[id-1].sim && Lambda_id==S_sim_l_lambda[id-1].Lambda_id && l==S_sim_l_lambda[id-1].l){
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
				
				get_S_i_l_Lambda(TS,i,l,Lambda_id);
                
				vector<tooling_configure>::iterator pTS=TS.begin();
				while(pTS!=TS.end()){
					int s=(*pTS).id;
					if(rate[s]>max_rate){
						max_rate=rate[s];
						max_rate_s=s;
					}
					pTS++;
				}
				
				struct S_sim_l_lambda_item one_item;
				one_item.sim=sim;
				one_item.l=l;
				one_item.Lambda_id=Lambda_id;
				one_item.s=max_rate_s;
				S_sim_l_lambda.push_back(one_item);
				count++;
				p_L++;
			}
			p_Lambda++;
		}//end of while(p_Lambda!=Lambda_i[i].int_vector.end()){
         sim_position_S_sim_l_lambda.push_back(count);
	}//end of for(int sim=1;sim<=(int)SIM.size();sim++){
}

void AT::show_S_sim_l_lambda(){
	ofstream fout("Debug/debug_S_sim_l_lambda.txt");
	int position=1;
	for(int id=1;id<=(int)S_sim_l_lambda.size();id++){
		fout<<"position="<<position
			<<", sim="<<S_sim_l_lambda[id-1].sim<<","
			<<"l="<<S_sim_l_lambda[id-1].l<<","
			<<"lambda="<<S_sim_l_lambda[id-1].Lambda_id<<","
			<<"s="<<S_sim_l_lambda[id-1].s<<endl;
		position++;
	}
	vii p_sim=sim_position_S_sim_l_lambda.begin();
	int sim=1;
	while(p_sim!=sim_position_S_sim_l_lambda.end())
	{
		fout<<"sim="<<sim<<", position="<<(* p_sim)<<endl;
		sim++;
		p_sim++;
	}

	fout.close();
}

int AT::index_S_sim_l_lambda(int sim,int l,int Lambda_id){
	int index=-1;
	if(sim<=0)
	{
		cout<<"WRONG sim in index_S_sim_l_lambda() "<<endl;
		return index;
	}
	int start_id=sim_position_S_sim_l_lambda[sim-1];
	for(int id=start_id;id<=(int)S_sim_l_lambda.size();id++){
		if(sim==S_sim_l_lambda[id-1].sim && l==S_sim_l_lambda[id-1].l && Lambda_id==S_sim_l_lambda[id-1].Lambda_id){
			index = id-1;
			break;
		}
	}
	if(index<0){
		cout<<"Warning!! Wrong S_sim_l_lambda index!!"<<endl;
		cout<<"sim="<<sim<<",l="<<l<<",Lambda_id="<<Lambda_id<<endl;
	}
	return index;
}
//end Zhufeng for logpoint



//Comment by Zhufeng for logpoint, Sep 02, 2011



//void AT::initialize_S_sim_d_lambda(){

//	//find S(i,l,lambda)->s with highest rate

//	for(int sim=1;sim<=(int)SIM.size();sim++){

//		int i=SIM[sim-1].represented_machine_id;

//		vii p_Lambda=Lambda_i[i].int_vector.begin();

//		while(p_Lambda!=Lambda_i[i].int_vector.end()){

//			int Lambda_id=(*p_Lambda);

//			int index=index_L_sim_lambda(sim,Lambda_id);

//			vector<int>::iterator p_L=L_sim_lambda[index].second.int_vector.begin();

//			while(p_L!=L_sim_lambda[index].second.int_vector.end()){

//				int l=(*p_L);

//				int d=lots_device[l];

//				int flag=0;

//				for(int id=1;id<=(int)S_sim_d_lambda.size();id++){

//					if(sim==S_sim_d_lambda[id-1].sim && Lambda_id==S_sim_d_lambda[id-1].Lambda_id && d==S_sim_d_lambda[id-1].d){

//						flag=1;

//						break;

//					}

//				}

//				if(flag){

//					p_L++;

//					continue;

//				}

//				int max_rate_s=-1;

//				double max_rate=-1E+6;

//				vector<tooling_configure> TS;

//				get_S_i_l_Lambda(TS,i,l,Lambda_id);

//

//				vector<tooling_configure>::iterator pTS=TS.begin();

//				while(pTS!=TS.end()){

//					int s=(*pTS).id;

//					if(rate[s]>max_rate){

//						max_rate=rate[s];

//						max_rate_s=s;

//					}

//					pTS++;

//				}

//				struct S_sim_d_lambda_item one_item;

//				one_item.sim=sim;

//				one_item.d=d;

//				one_item.Lambda_id=Lambda_id;

//				one_item.s=max_rate_s;

//				S_sim_d_lambda.push_back(one_item);

//				p_L++;

//			}

//			p_Lambda++;

//		}

//	}

//}

//

//void AT::show_S_sim_d_lambda(){

//	ofstream fout("Debug/debug_S_sim_d_lambda.txt");

//	for(int id=1;id<=(int)S_sim_d_lambda.size();id++){

//		fout<<"sim="<<S_sim_d_lambda[id-1].sim<<","

//			<<"d="<<S_sim_d_lambda[id-1].d<<","

//			<<"lambda="<<S_sim_d_lambda[id-1].Lambda_id<<","

//			<<"s="<<S_sim_d_lambda[id-1].s<<endl;

//	}

//	fout.close();

//}

//

//int AT::index_S_sim_d_lambda(int sim,int d,int Lambda_id){

//	int index=-1;

//	for(int id=1;id<=(int)S_sim_d_lambda.size();id++){

//		if(sim==S_sim_d_lambda[id-1].sim && d==S_sim_d_lambda[id-1].d && Lambda_id==S_sim_d_lambda[id-1].Lambda_id){

//			index = id-1;

//			break;

//		}

//	}

//	if(index<0){

//		cout<<"Warning!! Wrong S_sim_l_lambda index!!"<<endl;

//		cout<<"sim="<<sim<<",device="<<d<<",Lambda_id="<<Lambda_id<<endl;

//	}

//	return index;

//}



//end by Zhufeng for logpoint



int AT::is_KD_PD(int dev){

	//return 0 if dev is neither KD nor PD

	//return 1 if dev is KD

	//return 2 if dev is PD

	int value=0;	

	vii pfind=find(Key_Devices.begin(),Key_Devices.end(),dev);

	if(pfind!=Key_Devices.end()){

		value=1;

	}

	pfind=Packages.begin();

	pfind=find(Packages.begin(),Packages.end(),dev);

	if(pfind!=Packages.end()){

		value=2;

	}

	return value;

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

	vector<int> production(Num_Device,0);

	int counter=0;

	for(int i=1;i<=Num_Machine;i++){

		int Lambda_id=sub_soln[i-1].Lambda_id;

		//cout<<"Lambda_id="<<Lambda_id<<endl;

		//cout<<"size ="<<(int)sub_soln[i-1].lots.int_vector.size()<<endl;

		vii p_lot=sub_soln[i-1].lots.int_vector.begin();

		double time_used=0.0;

		while(p_lot!=sub_soln[i-1].lots.int_vector.end()){

			int l=(*p_lot);

			int s=max_rate_s(i,l,Lambda_id);

			obj+=-w_l[l]+eps[s];

			time_used+=n_l_chips[l]/rate[s]+Load_Unload_Time;

			int d=lots_device[l];

			production[d-1]+=n_l_chips[l];

			counter++;

			p_lot++;

		}

		//cout<<"i="<<i<<",time_used="<<sub_soln[i-1].time_used<<",time_computed="<<time_used<<endl;

	}

	//cout<<"counter="<<counter<<endl;

	//---------------------------------------Still need to add the initial lot to the production

	for(int id=1;id<=(int)initial_running_lot.size();id++){

		//if(initial_running_lot[id-1].time_remain>0 && H[initial_running_lot[id-1].machine_id]>0){

		//if(initial_running_lot[id-1].time_remain>0){	

		if(1){//include every initial lots even though they might be finished already	

			//NOTE That H[] has been updated in process_initial_lot() but changed back to the original H[] in the beginning of machine_resetup()

			//It doesn't matter if the initial lot can be finished or not. (it is possible that some LONG lot cannot be done within a horizon)

			//This also excludes the lots that are setup as initial lot but has already benn finished before Snapshoot_time (pretended to be an initial lot)

			obj+=-initial_running_lot[id-1].w_l; //at this point,forget the small amount eps[s]

			int d=initial_running_lot[id-1].dev_id;

			production[d-1]+=initial_running_lot[id-1].n_l_chips;

        	}

	}

	//compute the penalty of deviation for Key Device

	//for(int d=1;d<=Num_Device;d++){

	//	if(shortage[d-1].second>0.0){

	//		obj+=shortage[d-1].second/coef*penalty[d];

	//	}

	//}

	//-------------------------------------------------Another way to compute the penalty for KD

	for(int id=1;id<=(int)Key_Devices.size();id++){

		int d=Key_Devices[id-1];

		int diff=n_k_minchips[d]-production[d-1];

		if(diff>0){

			obj+=diff/coef*penalty[d];

		}

	}

	//------------------------------------------------------------------------------------------

	//cout<<"obj="<<obj<<endl;

	//compute the penalty of deviation for KP and KPP

	vector<int> PKG_production((int)PKG.size(),0);

	vector<int> PIN_PKG_production((int)PKG.size()*(int)PIN_order.size()+1,0);

	for(int i=1;i<=Num_Machine;i++){

		int Lambda_id=sub_soln[i-1].Lambda_id;

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

	//-----------------------------------------------------------------Also need to consider the contribution from the initial lots

	for(int id=1;id<=(int)initial_running_lot.size();id++){

		//if(initial_running_lot[id-1].time_remain>0 && H[initial_running_lot[id-1].machine_id]>0){

		//if(initial_running_lot[id-1].time_remain>0){	

		if(1){//include every initial lots even though they might be finished already	

			//NOTE That H[] has been updated in process_initial_lot() but is changed back to the original H[] in the beginning of machine_resetup()

			//It doesn't matter if the initial lot can be finished or not. (it is possible that some LONG lot cannot be done within a horizon)

			//This also excludes the lots that are setup as initial lot but has already finished before Snapshoot_time (pretended to be an initial lot)

			int d=initial_running_lot[id-1].dev_id;

			int pkg_id=initial_running_lot[id-1].pkg_id;

			int pin_num=initial_running_lot[id-1].PIN_num;

			vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);

			int pin_order=pfind-PIN_order.begin()+1;

			PKG_production[pkg_id-1]+=initial_running_lot[id-1].n_l_chips;

			int index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;

			PIN_PKG_production[index]+=initial_running_lot[id-1].n_l_chips;

		}

	}

	//-----------------------------------------------------------------------------------

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



			// Updated by Zhufeng



			/*int index=index_S_sim_d(sim,d);

			vii p_s=S_sim_d[index].second.int_vector.begin();

			while(p_s!=S_sim_d[index].second.int_vector.end()){*/



				int index=index_S_sim_l(sim,l);

				vii p_s=S_sim_l[index].second.int_vector.begin();

				while(p_s!=S_sim_l[index].second.int_vector.end()){

					//end by Zhufeng



				int s=(*p_s);

				int id=index2_x(i,l,s);

				obj+=(-w_l[l]+eps[s])*solution[id];

				p_s++;

			}

			p_L++;

		}

	}

	//---------------------------------------Still need to add the initial lot to the production

	vector<int> additional_production(Num_Device,0);

	for(int id=1;id<=(int)initial_running_lot.size();id++){

		//if(initial_running_lot[id-1].time_remain>0 && H[initial_running_lot[id-1].machine_id]>0){

		//if(initial_running_lot[id-1].time_remain>0){	

		if(1){//include every initial lots even though they might be finished already	

			//NOTE That H[] has been updated in process_initial_lot() but is changed back to the original H[] at the beginning of machine_resetup()

			//It doesn't matter if the lot can be finished or not. (it is possible that some LONG lot can not be done withn a horizon)

			//This also excludes the lots that are setup as initial lot but has already finished before Snapshoot_time (pretended to be an initial lot)

			obj+=-initial_running_lot[id-1].w_l; //at this point,forget the small amount eps[s]

			int d=initial_running_lot[id-1].dev_id;

			additional_production[d-1]+=initial_running_lot[id-1].n_l_chips;

        }

	}

	//-----------------------------------------------------------------------------------------------------

	//NOTE solution[id]->Delta, Delta*coef->shortage. That is solution[id]*coef->shortage

	for(int k=1;k<=Num_Keydevice;k++){//compute the penalty for key devices

		int id=x_limit+k;

		int d=Key_Devices[k-1];

		double shortage_from_solution=solution[id]*coef;

		double shortage=shortage_from_solution-additional_production[d-1];

		if(shortage>0){

			obj+=shortage/coef*penalty[d];

		}

	}

	for(int p=1;p<=Num_Package;p++){

		int id=x_limit+Num_Keydevice+p;

		int d=Packages[p-1];

		double shortage_from_solution=solution[id]*coef;

		double shortage=shortage_from_solution-additional_production[d-1];

		if(shortage>0){

			obj+=shortage/coef*penalty[d];

		}

	}

	for(int kp=1;kp<=(int)Key_PKG.size();kp++){

		int id=x_limit+Num_Keydevice+Num_Package+kp;

		int pkg_id=Key_PKG[kp-1];

		double shortage_from_solution=solution[id]*coef;

		int total=0;

		for(int d=1;d<=Num_Device;d++){

			if(device_PKG[d]==pkg_id){

				total+=additional_production[d-1];

			}

		}

		double shortage=shortage_from_solution-total;

		if(shortage>0){

			//obj+=PKG_penalty[pkg_id]*solution[id];

			obj+=PKG_penalty[pkg_id]*shortage/coef;

		}

	}

	for(int kpp=1;kpp<=(int)Key_PIN_PKG.size();kpp++){

		int id=x_limit+Num_Keydevice+Num_Package+(int)Key_PKG.size()+kpp;

		int pkg_id=Key_PIN_PKG[kpp-1].first.second;

		int pin_num=Key_PIN_PKG[kpp-1].first.first;

		double shortage_from_solution=solution[id]*coef;

		int total=0;

		for(int d=1;d<=Num_Device;d++){

			if(device_PKG[d]==pkg_id && device_PIN[d]==pin_num){

				total+=additional_production[d-1];

			}

		}

		double shortage=shortage_from_solution-total;

		if(shortage>0){

			//obj+=get_PIN_PKG_penalty(pkg_id,pin_num)*solution[id];

			obj+=get_PIN_PKG_penalty(pkg_id,pin_num)*shortage/coef;

		}

	}

	return obj;

}



double AT::compute_obj(vector< pair<sub_soln_item,double> > & enhanced_solution){

	//------------------------------------------compute the obj for the enhanced solution after resetting the idle machines

	//NOTE: the lots have been sorted in update_benefit() already

	//NOTE: ACTION 2 is taken care of here

	double obj=0.0;

	vector<int> production(Num_Device,0);

	for(int j=1;j<=(int)enhanced_solution.size();j++){//compute the total lot weights

		double current_time=enhanced_solution[j-1].second;

		int i=enhanced_solution[j-1].first.i;

		int Lambda_id=enhanced_solution[j-1].first.Lambda_id;

		for(int id=1;id<=(int)enhanced_solution[j-1].first.lots.int_vector.size();id++){

			int l=enhanced_solution[j-1].first.lots.int_vector[id-1]; //lot

			int s=max_rate_s(i,l,Lambda_id);

			double time_required=n_l_chips[l]/rate[s]+Load_Unload_Time;

			current_time+=time_required;

			if(current_time<=H[i]){//only count the lots that are finished within the horizon (ACTION 2)

				obj+=-w_l[l]+eps[s];

				int d=lots_device[l];

				production[d-1]+=n_l_chips[l]; //update the device production

			}else{

				break; //the next lots also exceed the horizon

			}

		}

	}

	//---------------------------------------Still need to add the initial lot to the production

	for(int id=1;id<=(int)initial_running_lot.size();id++){

		//if(initial_running_lot[id-1].time_remain>0 && H[initial_running_lot[id-1].machine_id]>0){

		//if(initial_running_lot[id-1].time_remain>0){

		if(1){//include every initial lots even though they might be finished already	

			//NOTE That H[] has been updated in process_initial_lot() but is changed back to the original H[] in the beginning of machine_resetup()

			//It doesn't matter if the lot can be finished or not. (it is possible that some LONG lot can not be done withn a horizon)

			//This also excludes the lots that are setup as initial lot but has already finished before Snapshoot_time (pretended to be an initial lot)

			obj+=-initial_running_lot[id-1].w_l; //at this point,forget the small amount eps[s]

			int d=initial_running_lot[id-1].dev_id;

			production[d-1]+=initial_running_lot[id-1].n_l_chips;

        	}

	}

	//----------------------------------------------compute penalties for key devices

	for(int id=1;id<=(int)Key_Devices.size();id++){

		int d=Key_Devices[id-1];

		int shortage=n_k_minchips[d]-production[d-1];

		if(shortage>0){

			obj+=shortage/coef*penalty[d];

		}

	}

	//----------------------------------------------compute the penalties for key packages and key pin pacakges

	vector<int> PKG_production((int)PKG.size(),0);

	vector<int> PIN_PKG_production((int)PKG.size()*(int)PIN_order.size()+1,0);

	for(int j=1;j<=(int)enhanced_solution.size();j++){//--------------get the production of KP and KPP

		double current_time=enhanced_solution[j-1].second; //the starting time

		int i=enhanced_solution[j-1].first.i;

		int Lambda_id=enhanced_solution[j-1].first.Lambda_id;

		vii p_lot=enhanced_solution[j-1].first.lots.int_vector.begin();

		while(p_lot!=enhanced_solution[j-1].first.lots.int_vector.end()){

			int l=(*p_lot);

			int s=max_rate_s(i,l,Lambda_id);

			double time_required=n_l_chips[l]/rate[s]+Load_Unload_Time;

			current_time+=time_required;

			if(current_time<=H[i]){

				int d=lots_device[l];

				int pkg_id=device_PKG[d];

				int pin_num=device_PIN[d];

				vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);

				int pin_order=pfind-PIN_order.begin()+1;

				PKG_production[pkg_id-1]+=n_l_chips[l];

				int index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;

				PIN_PKG_production[index]+=n_l_chips[l];

			}else{

				break;

			}

			p_lot++;

		}

	}

	//-----------------------------------------------------------------Also need to consider the contribution from the initial lots

	for(int id=1;id<=(int)initial_running_lot.size();id++){

		//if(initial_running_lot[id-1].time_remain>0 && H[initial_running_lot[id-1].machine_id]>0){

		//if(initial_running_lot[id-1].time_remain>0){

		if(1){//include every initial lots even though they might be finished already	

			//NOTE That H[] has been updated in process_initial_lot() but is changed back to the original H[] at the beginning of machine_resetup()

			//It doesn't matter if the lot can be finished in the time horizon or not. (it is possible that some LONG lot can not be done withn a horizon)

			//This also excludes the lots that are setup as initial lot but has already finished before Snapshoot_time (pretended to be an initial lot)

			int d=initial_running_lot[id-1].dev_id;

			int pkg_id=initial_running_lot[id-1].pkg_id;

			int pin_num=initial_running_lot[id-1].PIN_num;

			vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);

			int pin_order=pfind-PIN_order.begin()+1;

			PKG_production[pkg_id-1]+=initial_running_lot[id-1].n_l_chips;

			int index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;

			PIN_PKG_production[index]+=initial_running_lot[id-1].n_l_chips;

		}

	}

	//-----------------------------------------------------------------------------------

	//---------------------------------------------------------now compute the penalties

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





//Added by Zhufeng to compute multi obj, Feb 26

double AT::compute_obj(vector< pair<sub_soln_item,double> > & enhanced_solution,

					   vector<int> & production_first_pass,

					   double & weighted_item,

					   double & weighted_sum_lot_first_pass){

	//Get produciton for the first-pass back. Zhufeng



	//------------------------------------------compute the obj for the enhanced solution after resetting the idle machines

	//NOTE: the lots have been sorted in update_benefit() already

	//NOTE: ACTION 2 is taken care of here

	double obj=0.0;

	//Added by Zhufeng for recording the weighted sum of lot processed in the Yumin's codes, March 15

	weighted_sum_lot_first_pass = 0;

	//End by Zhufeng, March 15, 2012



	vector<int> production(Num_Device,0);

	for(int j=1;j<=(int)enhanced_solution.size();j++){//compute the total lot weights

		double current_time=enhanced_solution[j-1].second;

		int i=enhanced_solution[j-1].first.i;

		int Lambda_id=enhanced_solution[j-1].first.Lambda_id;

		for(int id=1;id<=(int)enhanced_solution[j-1].first.lots.int_vector.size();id++){

			int l=enhanced_solution[j-1].first.lots.int_vector[id-1]; //lot

			int s=max_rate_s(i,l,Lambda_id);

			double time_required=n_l_chips[l]/rate[s]+Load_Unload_Time;

			current_time+=time_required;

			if(current_time<=H[i]){//only count the lots that are finished within the horizon (ACTION 2)

				obj+=-w_l[l]+eps[s];

				int d=lots_device[l];

				production[d-1]+=n_l_chips[l]; //update the device production

				//Added by Zhufeng for recording the weighted sum of lot processed in the Yumin's codes, March 15

				weighted_sum_lot_first_pass+= w_l[l];

				//End by Zhufeng, March 15, 2012



			}else{

				break; //the next lots also exceed the horizon

			}

		}

	}





	//---------------------------------------Still need to add the initial lot to the production

	for(int id=1;id<=(int)initial_running_lot.size();id++){

		//if(initial_running_lot[id-1].time_remain>0 && H[initial_running_lot[id-1].machine_id]>0){

		//if(initial_running_lot[id-1].time_remain>0){

		if(1){//include every initial lots even though they might be finished already	

			//NOTE That H[] has been updated in process_initial_lot() but is changed back to the original H[] in the beginning of machine_resetup()

			//It doesn't matter if the lot can be finished or not. (it is possible that some LONG lot can not be done withn a horizon)

			//This also excludes the lots that are setup as initial lot but has already finished before Snapshoot_time (pretended to be an initial lot)

			obj+=-initial_running_lot[id-1].w_l; //at this point,forget the small amount eps[s]

			int d=initial_running_lot[id-1].dev_id;

			production[d-1]+=initial_running_lot[id-1].n_l_chips;



			//Added by Zhufeng for recording the weighted sum of lot processed in the Yumin's codes, March 15

			//weighted_sum_lot_first_pass+= initial_running_lot[id-1].w_l;

			//End by Zhufeng, March 15, 2012

        	}

	}



	//Added by Zhufeng, Feb 27, 2012, to compute multi objective value

	weighted_item=obj;

	//end by Zhufeng, Feb 27, 2012



	//----------------------------------------------compute penalties for key devices

	for(int id=1;id<=(int)Key_Devices.size();id++){

		int d=Key_Devices[id-1];

		int shortage=n_k_minchips[d]-production[d-1];

		if(shortage>0){

			obj+=shortage/coef*penalty[d];

		}

	}

	//----------------------------------------------compute the penalties for key packages and key pin pacakges

	vector<int> PKG_production((int)PKG.size(),0);

	vector<int> PIN_PKG_production((int)PKG.size()*(int)PIN_order.size()+1,0);

	for(int j=1;j<=(int)enhanced_solution.size();j++){//--------------get the production of KP and KPP

		double current_time=enhanced_solution[j-1].second; //the starting time

		int i=enhanced_solution[j-1].first.i;

		int Lambda_id=enhanced_solution[j-1].first.Lambda_id;

		vii p_lot=enhanced_solution[j-1].first.lots.int_vector.begin();

		while(p_lot!=enhanced_solution[j-1].first.lots.int_vector.end()){

			int l=(*p_lot);

			int s=max_rate_s(i,l,Lambda_id);

			double time_required=n_l_chips[l]/rate[s]+Load_Unload_Time;

			current_time+=time_required;

			if(current_time<=H[i]){

				int d=lots_device[l];

				int pkg_id=device_PKG[d];

				int pin_num=device_PIN[d];

				vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);

				int pin_order=pfind-PIN_order.begin()+1;

				PKG_production[pkg_id-1]+=n_l_chips[l];

				int index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;

				PIN_PKG_production[index]+=n_l_chips[l];

			}else{

				break;

			}

			p_lot++;

		}

	}

	//-----------------------------------------------------------------Also need to consider the contribution from the initial lots

	for(int id=1;id<=(int)initial_running_lot.size();id++){

		//if(initial_running_lot[id-1].time_remain>0 && H[initial_running_lot[id-1].machine_id]>0){

		//if(initial_running_lot[id-1].time_remain>0){

		if(1){//include every initial lots even though they might be finished already	

			//NOTE That H[] has been updated in process_initial_lot() but is changed back to the original H[] at the beginning of machine_resetup()

			//It doesn't matter if the lot can be finished in the time horizon or not. (it is possible that some LONG lot can not be done withn a horizon)

			//This also excludes the lots that are setup as initial lot but has already finished before Snapshoot_time (pretended to be an initial lot)

			int d=initial_running_lot[id-1].dev_id;

			int pkg_id=initial_running_lot[id-1].pkg_id;

			int pin_num=initial_running_lot[id-1].PIN_num;

			vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);

			int pin_order=pfind-PIN_order.begin()+1;

			PKG_production[pkg_id-1]+=initial_running_lot[id-1].n_l_chips;

			int index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;

			PIN_PKG_production[index]+=initial_running_lot[id-1].n_l_chips;

		}

	}

	//-----------------------------------------------------------------------------------

	//---------------------------------------------------------now compute the penalties

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



	production_first_pass=production;



	return obj;

}



//end by Zhufeng, Feb 26



//May 23, 2012

//Aug 12, 2012, Zhufeng, Add KP and KPP 

double AT::compute_obj_step( const vector<solution> & first_pass_solution, 

						const vector<multi_production_step> multi_device_production_step, 

						const double weighted_item,

						const int Max_Num_Step)

{

	cout<<"compute obj_step for single-pass solution"<<endl;

	double obj=0.0;

	obj+=weighted_item;

	int total_shortage = 0;

	double total_weighted_shortage =0.0;

    int shortage_for_final_step = 0;

	double weighted_shortage_for_final_step = 0;

	

	//Added by Zhufeng, Aug 12, 2012, KP and KPP

	//Only if the last step of a device is done, it is counted as finished package, 

	//and as production the KP when it is KP.

	//The same to KPP

	int total_shortage_KP = 0;

	double total_weighted_shortage_KP = 0.0;





	int total_shortage_KPP = 0;

	double total_weighted_shortage_KPP = 0.0;



	int Num_PKG = (int)PKG.size();

	int Num_PIN_PKG = (int)PKG.size()*(int)PIN_order.size()+1;

	

	vector<int> PKG_production( Num_PKG,0);

	vector<int> PIN_PKG_production( Num_PIN_PKG,0);



    //end by Zhufeng, Aug 12, 2012



	for( int step_id=1;step_id<= Max_Num_Step;step_id++)

	{

		for(int id=1;id<=(int)Key_Devices.size();id++){

			int d=Key_Devices[id-1];

			//Added by Zhufeng, for shortage only for existing steps,May 29,2012,

			if(multi_device_production_step[(step_id-1)*Num_Device+d-1].whether_exist)

			{

				//end by May 29,2012

				int shortage=n_k_minchips[d]-multi_device_production_step[(step_id-1)*Num_Device+d-1].production;

				if(shortage>0)

				{

					obj+=(double)shortage/coef*penalty[d];

					total_weighted_shortage+=(double)shortage/coef*penalty[d];

					total_shortage +=shortage;

					if(multi_device_production_step[(step_id-1)*Num_Device+d-1].whether_final_step)

					{

						shortage_for_final_step+=shortage;

						weighted_shortage_for_final_step+=(double)shortage/coef*penalty[d];;

					}

				}

			//Added by May 29, 2012

			}

			//end by May 29, 2012

		}

	

	}//end of for( int step_id=1;step_id<= Max_Num_Step;step_id++)



	//Added by Zhufeng, Aug 12, 2012, KP and KPP.	

	//Only if the last step of a device is done, it is counted as finished package, 

	//and as production the KP when it is KP.

	//The same to KPP

	for( int step_id=1;step_id<= Max_Num_Step;step_id++)

	{

		for(int d=1;d<=Num_Device;d++){

			if(multi_device_production_step[(step_id-1)*Num_Device+d-1].whether_exist && multi_device_production_step[(step_id-1)*Num_Device+d-1].whether_final_step){

				int pkg_id=device_PKG[d];

				int pin_num=device_PIN[d];

				vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);

				int pin_order=pfind-PIN_order.begin()+1;

				int index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;

				PKG_production[pkg_id-1]+=multi_device_production_step[(step_id-1)*Num_Device+d-1].production;

				PIN_PKG_production[index]+=multi_device_production_step[(step_id-1)*Num_Device+d-1].production;

			}

		}

	}//end of for( int step_id=1;step_id<= Max_Num_Step;step_id++)



		//now compute the penalties

		for(int kp=1;kp<=(int)Key_PKG.size();kp++){

			int pkg_id=Key_PKG[kp-1];

			int diff=n_kp_minchips[pkg_id]-PKG_production[pkg_id-1];

			

			if(diff>0){

				obj+=diff/coef*PKG_penalty[pkg_id];

				total_shortage_KP+=diff;

				total_weighted_shortage_KP+=diff/coef*PKG_penalty[pkg_id];

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

			if(diff>0){

				obj+=diff/coef*get_PIN_PKG_penalty(pkg_id,pin_num);

				total_shortage_KPP+=diff;

				total_weighted_shortage_KPP+=diff/coef*get_PIN_PKG_penalty(pkg_id,pin_num);

			}

		}



	//end by Zhufeng, Aug 12, 2012





	fstream fout("Output/resultsummery.txt", ios::out|ios::app);

	fout<<"Based on steps not passes,single-pass solution,"<<endl

		<<"objective value = "<<obj<<endl

		<<"total shortage for all key device and steps = "<<total_shortage<<endl

		<<"total weighted for all key device and steps = "<<total_weighted_shortage<<endl

		<<"total shortage of finished device = "<<shortage_for_final_step<<endl

		<<"total weighted shortage of finished device = "<<weighted_shortage_for_final_step<<endl	 

//Added by Zhufeng, Aug 12, 2012, KP and KPP.	

		<<"total shortage of all key package = "<<total_shortage_KP<<endl

		<<"total weighted shortage of all key pin package = "<<total_weighted_shortage_KP<<endl	

		<<"total shortage of all key pin package = "<<total_shortage_KPP<<endl

		<<"total weighted shortage of all key pin package = "<<total_weighted_shortage_KPP<<endl;	

//end by Zhufeng, Aug 12, 2012

	fout.close();

}



void AT::show_sub_soln(vector<sub_soln_item> & sub_soln){

	cout<<"show sub_soln"<<endl;

	for(int i=1;i<=Num_Machine;i++){

		cout<<"i="<<i<<",Lamdua_id="<<sub_soln[i-1].Lambda_id<<",time_used="<<sub_soln[i-1].time_used<<",assign lots=";

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

	cout<<"initialize x variables"<<endl;

	for(int i=1;i<=Num_Machine;i++){

		//cout<<"i="<<i<<endl;



		vii p_L=L_i[i].int_vector.begin();

		while(p_L!=L_i[i].int_vector.end()){

			int l=(*p_L);

			//cout<<"l="<<l<<endl;

			int sim=machine_sim[i];

			int d=lots_device[l];



			//Updated by Zhufeng for logpoint, Aug 31, 2011



			/*int index=index_S_sim_d(sim,d);

			vii p_s=S_sim_d[index].second.int_vector.begin();

			while(p_s!=S_sim_d[index].second.int_vector.end()){*/



				int index=index_S_sim_l(sim,l);

				vii p_s=S_sim_l[index].second.int_vector.begin();

				while(p_s!=S_sim_l[index].second.int_vector.end()){

					//end by Zhufeng for logpoint



				int s=(*p_s);

				//cout<<"s="<<s<<endl;

				//string str="x";

				//stringstream ss;

				//ss<<"("<<i<<")("<<l<<")("<<s<<")";

				//str+=ss.str();

				//char name[20];

				//strcpy(name,str.c_str());

				int x_id=index2_x(i,l,s);

				col_lb[x_id]=0.0;

				col_ub[x_id]=1.0;



				// --- added by yufen ---

				if(Machine_vector[i-1].hasInitialSetup)

				{

					if(S[s-1].mg!=Machine_vector[i-1].mg

						|| S[s-1].temperature!=Machine_vector[i-1].initial_temperature

						|| find(S[s-1].tf.tools.begin(), S[s-1].tf.tools.end(), Machine_vector[i-1].initial_tool) == S[s-1].tf.tools.end())

					{

						col_ub[x_id]=0.0;

					}

				}

				// --- end yufen --------



				num_var++;

				p_s++;

			}

			p_L++;

		}

	}

	//initialize y variables

	cout<<"initialize y variables"<<endl;

	for(int i=1;i<=Num_Machine;i++){

		vii p_Lambda=Lambda_i[i].int_vector.begin();

		while(p_Lambda!=Lambda_i[i].int_vector.end()){

			int Lambda_id=(*p_Lambda);

			//string str="y";

			//stringstream ss;

			//ss<<"("<<i<<")("<<Lambda_id<<")";

			//str+=ss.str();

			//char name[20];

			//strcpy(name,str.c_str());

			int y_id=index2_y(i,Lambda_id);

			col_lb[y_id]=0.0;

			col_ub[y_id]=1.0;

			num_var++;

			p_Lambda++;

		}

	}

	//initalize delta variables

	cout<<"initialize delta variables"<<endl;

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



			//Updated by Zhufeng for logpoint



			/*int index=index_S_sim_d(sim,d);

			vii p_s=S_sim_d[index].second.int_vector.begin();

			while(p_s!=S_sim_d[index].second.int_vector.end()){*/



				int index=index_S_sim_l(sim,l);

				vii p_s=S_sim_l[index].second.int_vector.begin();

				while(p_s!=S_sim_l[index].second.int_vector.end()){

					//end by Zhufeng for logpoint



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

	//This is the lot assignment constraint. Each lot can be assigned to a machine-tooling combination at most once.

	for(int l=1;l<=Num_Lot;l++){

		CoinPackedVector row;

		vector<int>::iterator p_M=M_l[l].int_vector.begin();

		while(p_M!=M_l[l].int_vector.end()){

			int i=(*p_M);

			int sim=machine_sim[i];

			int d=lots_device[l];



			//Updated by Zhufeng for logpoint, Aug 31, 2011



			/*int index=index_S_sim_d(sim,d);

			vii p_s=S_sim_d[index].second.int_vector.begin();

			while(p_s!=S_sim_d[index].second.int_vector.end()){*/



				int index=index_S_sim_l(sim,l);

				vii p_s=S_sim_l[index].second.int_vector.begin();

				while(p_s!=S_sim_l[index].second.int_vector.end()){

					//end by Zhufeng for logpoint



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

	//build the constraint x(i,l,s)<=y(i,lambda),This set of constraint contains a large number of constraints

	for(int i=1;i<=Num_Machine;i++){

		vector<int>::iterator p_Lambda=Lambda_i[i].int_vector.begin();

		while(p_Lambda!=Lambda_i[i].int_vector.end()){

			int Lambda_id=(*p_Lambda);

			int simid=machine_sim[i];

			int index=index_L_sim_lambda(simid,Lambda_id);

			vector<int>::iterator p_L=L_sim_lambda[index].second.int_vector.begin();

			while(p_L!=L_sim_lambda[index].second.int_vector.end()){

				int l=(*p_L);

				vector<tooling_configure> TS;

				get_S_i_l_Lambda(TS,i,l,Lambda_id);

				vector<tooling_configure>::iterator p_TS=TS.begin();

				while(p_TS!=TS.end()){

					CoinPackedVector row;

					int s=(*p_TS).id;

					int x_id=index2_x(i,l,s);

					int y_id=index2_y(i,Lambda_id);

					row.insert(x_id,-1.0);

					row.insert(y_id,1.0);

					row_lb[num_con]=0.0;

					row_ub[num_con]=DBL_MAX;

					M->appendRow(row);

					num_con++;

					string str="(";

					stringstream ss;

					ss<<"i@"<<i<<",Lambda@"<<Lambda_id<<",l@"<<l<<",s@"<<s<<")";

					str+=ss.str();

					char name[100];

					strcpy(name,str.c_str());

					p_TS++;

				}

				p_L++;

			}

			p_Lambda++;

		}

	}

	}



	cout<<"build constraints (1d)"<<endl;

	//This is the machine setup constraint. Each machine can setup a lambda at most.

	for(int i=1;i<=Num_Machine;i++){

		CoinPackedVector row;

		vector<int>::iterator p_Lambda=Lambda_i[i].int_vector.begin();

		while(p_Lambda!=Lambda_i[i].int_vector.end()){

			int Lambda_id=(* p_Lambda);

			int y_id=index2_y(i,Lambda_id);

			row.insert(y_id,1.0);

			p_Lambda++;

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

	//This is the tooling capacity constraint.

	for(int tf=1;tf<=Num_Tooling_Family;tf++){

		for(int n=1;n<=(int)Temperature_Set.size();n++){

			vector<t_set>::iterator p_set=Temperature_Set.begin()+n-1;

			CoinPackedVector row;

			for(int i=1;i<=Num_Machine;i++){

				vii p_int=(*p_set).set_elements.begin();

				while(p_int!=(*p_set).set_elements.end()){

					int tau=(*p_int);

					vector<int> Lambda_i_tf_tau;

					get_Lambda(Lambda_i_tf_tau,i,tf,tau);

					vii p_Lambda=Lambda_i_tf_tau.begin();

					while(p_Lambda!=Lambda_i_tf_tau.end()){

						int Lambda_id=(*p_Lambda);

						vector<Lambda_item>::iterator pL=Lambda.begin()+Lambda_id-1;

						vii p_find=find((*pL).tooling_vec.begin(),(*pL).tooling_vec.end(),tf);

						int offset=p_find-(*pL).tooling_vec.begin();

						vii pb=(*pL).b.begin()+offset;

						int y_id=index2_y(i,Lambda_id);

						row.insert(y_id,(*pb));

						p_Lambda++;

					}//lambda_iteration

					p_int++;

				}//tau_iteration

			}//i_iteration

			vector<t_set> inter_set;

			get_inter_set((*p_set),inter_set);

			vector<t_set>::iterator pm=inter_set.begin();

			int sum=0;

			while(pm!=inter_set.end()){

				int set_id=get_set_id(*pm);

				int pos=tf*Max_Num_Temperature+set_id;

				//sum+=n_t_n_tooling[tf][set_id];

				sum+=n_t_n_tooling[pos];

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

	//This is the machine time constraint. The x<->y relationship is also taken into account in this constraint. 

	//Though this constraint is not as tight as the pure x<->y constraint, it helps to reduce the number of constraints a lot

	//In ACTION 3 of the AT enhancement,the Load_Unload_Time need to be considered in this constraint

	for(int i=1;i<=Num_Machine;i++){

		vector<int>::iterator p_Lambda=Lambda_i[i].int_vector.begin();

		while(p_Lambda!=Lambda_i[i].int_vector.end()){

			int Lambda_id=(*p_Lambda);

			CoinPackedVector row;

			int index=index_L_sim_lambda(machine_sim[i],Lambda_id);

			vector<int>::iterator p_L=L_sim_lambda[index].second.int_vector.begin();

			while(p_L!=L_sim_lambda[index].second.int_vector.end()){

				int l=(*p_L);

				vector<tooling_configure> TS;

				get_S_i_l_Lambda(TS,i,l,Lambda_id);

				vector<tooling_configure>::iterator p_TS=TS.begin();

				while(p_TS!=TS.end()){

					int s=(*p_TS).id;

					int x_id=index2_x(i,l,s);

					//row.insert(x_id,n_l_chips[l]/rate[s]);

					//modify this constraint to take into account the loading and unloading time

					row.insert(x_id,n_l_chips[l]/rate[s]+Load_Unload_Time);

					p_TS++;

				}

				p_L++;

			}

			int y_id=index2_y(i,Lambda_id);

			row.insert(y_id,-H[i]);

			row_lb[num_con]=-DBL_MAX;

			row_ub[num_con]=0;

			M->appendRow(row);

			num_con++;

			string str="(";

			stringstream ss;

			ss<<"i@"<<i<<",Lambda@"<<Lambda_id<<")";

			str+=ss.str();

			char name[100];

			strcpy(name,str.c_str());

			p_Lambda++;

		}

	}

	}



	if(new_old_model==2){

	cout<<"build constraints (1f)"<<endl;

	//This is the original machine time constraint

	//In ACTION 3 of the AT enhancement,the Load_Unload_Time need to be considered in this constraint

	for(int i=1;i<=Num_Machine;i++){

		CoinPackedVector row;

		vector<int>::iterator p_L=L_i[i].int_vector.begin();

		while(p_L!=L_i[i].int_vector.end()){

			int l=(*p_L);

			int sim=machine_sim[i];

			int d=lots_device[l];



			//Updated by Zhufeng for logpoint, Aug 31, 2011



			/*int index=index_S_sim_d(sim,d);

			vii p_s=S_sim_d[index].second.int_vector.begin();

			while(p_s!=S_sim_d[index].second.int_vector.end()){*/



				int index=index_S_sim_l(sim,l);

				vii p_s=S_sim_l[index].second.int_vector.begin();

				while(p_s!=S_sim_l[index].second.int_vector.end()){

					//end by Zhufeng



				int s=(*p_s);

				int x_id=index2_x(i,l,s);

				//row.insert(x_id,n_l_chips[l]/rate[s]);

				//modify this constraint to take into account the loading and unloading time

				row.insert(x_id,n_l_chips[l]/rate[s]+Load_Unload_Time);

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

		//ss<<"i@"<<i<<",Lambda@"<<Lambda_id<<")";

		str+=ss.str();

		char name[100];

		strcpy(name,str.c_str());

	}

	}



	cout<<"build constraints (1g)"<<endl;

	//this is the deviation constraint for key device

	for(int k=1;k<=Num_Keydevice;k++){

	

		//cout<<"k="<<k<<endl;

		vector<int>::iterator p_int=Key_Devices.begin()+k-1;

		int d=(*p_int);

		CoinPackedVector row;

		for(int i=1;i<=Num_Machine;i++){



			//cout<<"i="<<i<<endl;

			int pos=i*Max_Num_Device+d;

			//cout<<"pos="<<pos<<endl;	

			vector<int>::iterator p_L=L_i_d[pos].int_vector.begin();

			while(p_L!=L_i_d[pos].int_vector.end()){

				int l=(*p_L);

				int sim=machine_sim[i];



				//Updated by Zhufeng for logpoint, Aug 31, 2011



				/*int index=index_S_sim_d(sim,d);

				vii p_s=S_sim_d[index].second.int_vector.begin();

				while(p_s!=S_sim_d[index].second.int_vector.end()){*/



				int index=index_S_sim_l(sim,l);

				vii p_s=S_sim_l[index].second.int_vector.begin();

				while(p_s!=S_sim_l[index].second.int_vector.end()){

					//end by Zhufeng for logpoint



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

		//cout<<"name="<<name<<endl;

	}



	cout<<"build constraints (1h)"<<endl;

	//this is the deviation constraint for package device. NOTE it is not activated any more since Num_Package=0

	for(int p=1;p<=Num_Package;p++){

		vector<int>::iterator p_int=Packages.begin()+p-1;

		int d=(*p_int);

		CoinPackedVector row;

		for(int i=1;i<=Num_Machine;i++){

			int pos=i*Max_Num_Device+d;

			vector<int>::iterator p_L=L_i_d[pos].int_vector.begin();

			while(p_L!=L_i_d[pos].int_vector.end()){

				int l=(*p_L);

				int sim=machine_sim[i];



				//Updated by Zhufeng for logpoint. Aug 31, 2011



				/*int index=index_S_sim_d(sim,d);

				vii p_s=S_sim_d[index].second.int_vector.begin();

				while(p_s!=S_sim_d[index].second.int_vector.end()){*/



				int index=index_S_sim_l(sim,l);

				vii p_s=S_sim_l[index].second.int_vector.begin();

				while(p_s!=S_sim_l[index].second.int_vector.end()){

					//end by Zhufeng for logopint



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

	//This is the deviation constraint for key package

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

				int pos=i*Max_Num_Device+d;

				vector<int>::iterator p_L=L_i_d[pos].int_vector.begin();

				while(p_L!=L_i_d[pos].int_vector.end()){

					int l=(*p_L);

					int sim=machine_sim[i];



					//Updated by Zhufeng for logpoint, Aug 31, 2011



					/*int index=index_S_sim_d(sim,d);

					vii p_s=S_sim_d[index].second.int_vector.begin();

					while(p_s!=S_sim_d[index].second.int_vector.end()){*/



				int index=index_S_sim_l(sim,l);

				vii p_s=S_sim_l[index].second.int_vector.begin();

				while(p_s!=S_sim_l[index].second.int_vector.end()){

					//end by Zhufeng for logpoint



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

	//This is the deviation constraint for key pin package

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

				int pos=i*Max_Num_Device+d;

				vector<int>::iterator p_L=L_i_d[pos].int_vector.begin();

				while(p_L!=L_i_d[pos].int_vector.end()){

					int l=(*p_L);

					int sim=machine_sim[i];





                    // Updated by Zhufeng for logpoint



					/*int index=index_S_sim_d(sim,d);

					vii p_s=S_sim_d[index].second.int_vector.begin();

					while(p_s!=S_sim_d[index].second.int_vector.end()){*/



				int index=index_S_sim_l(sim,l);

				vii p_s=S_sim_l[index].second.int_vector.begin();

				while(p_s!=S_sim_l[index].second.int_vector.end()){

					//end by Zhufeng



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

	//si->writeLp("AT_GRASP_first_level");

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

		int Lambda_id=(*p_node).Lambda_id;

		cout<<"i="<<i<<",Lambda_id="<<Lambda_id<<",y="<<si->getColSolution()[id]<<",LB="<<si->getColLower()[id]<<",UB="<<si->getColUpper()[id]<<",id="<<id<<endl;

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



int AT::index_L_sim_lambda(int simid,int Lambda_id){

	//get the index of element of vector L_sim_lambda given simid and Lamdua_id

	int index=-1;

	vector< pair< pair<int,int>,vec_int> >::iterator p=L_sim_lambda.begin();

	while(p!=L_sim_lambda.end()){

		if((*p).first.first==simid && (*p).first.second==Lambda_id){

			index=p-L_sim_lambda.begin();

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

	//< < <i,Lambda_id>,time_used>,lot1,lot2,...>

	sub_soln.clear();

	for(int i=1;i<=Num_Machine;i++){

		sub_soln_item one_item;

		one_item.i=i;

		if(fix_y[i]>0){

			one_item.Lambda_id=fix_y[i];

		}else{

			one_item.Lambda_id=-1;

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

			vector< pair<int,int> >::iterator p_pair=sim_lambda_L[l-1].second.sim_lambda_pairs.begin();

			while(p_pair!=sim_lambda_L[l-1].second.sim_lambda_pairs.end()){

				if(machine_sim[i]==(*p_pair).first && sub_soln[i-1].Lambda_id==(*p_pair).second){//the machine can be used to processed the lot

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

			int s=max_rate_s(i,l,sub_soln[i-1].Lambda_id);

			double time_required=n_l_chips[l]/rate[s]+Load_Unload_Time;

			vector< pair<int,int> >::iterator p_pair=sim_lambda_L[l-1].second.sim_lambda_pairs.begin();

			while(p_pair!=sim_lambda_L[l-1].second.sim_lambda_pairs.end()){

				if(machine_sim[i]==(*p_pair).first && sub_soln[i-1].Lambda_id==(*p_pair).second

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



//Not used any more. Comment by Zhufeng, Feb 2014

//void AT::local_search_on_y(OsiXxxSolverInterface * si,struct soln_pool_item & soln_A){

//	//diversified

//    double * solution=new double[y_limit+1];

//	int * fix_y=new int[Num_Machine+1];

//	for(int id=0;id<=y_limit;id++){ solution[id]=0.0;}

//	for(int id=x_limit+Num_Keydevice+Num_Package+1;id<=y_limit;id++){ si->setColBounds(id,0,0);}

//	fix_y[0]=-1;

//	for(int i=1;i<=Num_Machine;i++){ 

//		if(soln_A.sub_soln[i-1].Lambda_id>0){

//			fix_y[i]=soln_A.sub_soln[i-1].Lambda_id;

//			int y_id=index2_y(i,fix_y[i]);

//			si->setColBounds(y_id,1,1);

//		}else{

//			fix_y[i]=-1;

//		}

//	}

//

//    double cur_obj=compute_obj(soln_A.sub_soln,soln_A.shortage);

//	cout<<"cur_obj="<<cur_obj<<endl;

//	bool primal_dual=false;

//	bool iter_flag=true;

//	while(iter_flag){

//		iter_flag=false;

//		double best_obj=1E+10;

//		int best_i=-1;

//		int best_Lambda=-1;

//		vector<sub_soln_item> best_sub_soln;

//		vector< pair<int,double> > best_shortage;

//		vector< pair< pair<int,double>,double> > best_unassigned_lots;

//		best_sub_soln.resize(Num_Machine);

//		best_shortage.resize(Num_Device);

//

//		for(int i=1;i<=Num_Machine;i++){

//			if(i==1){ primal_dual=true;}

//			int Lambda_id_cur=soln_A.sub_soln[i-1].Lambda_id;

//			//cout<<"Lambda_id_cur="<<Lambda_id_cur<<endl;

//			int y_id=-1000;

//			if(Lambda_id_cur>0){

//				y_id=index2_y(i,Lambda_id_cur);

//				si->setColBounds(y_id,0,0);

//			}

//			//----------------------------------------------------------compute the benefit lost

//			double benefit_lost=0.0;

//			vector<int> dev;

//			vector<int> amount;

//			vii p_lot=soln_A.sub_soln[i-1].lots.int_vector.begin();

//			while(p_lot!=soln_A.sub_soln[i-1].lots.int_vector.end()){

//				int l=(*p_lot);

//				benefit_lost+=w_l[l];

//				int d=lots_device[l];

//				vii pfind=find(dev.begin(),dev.end(),d);

//				if(pfind!=dev.end()){

//					int offset=pfind-dev.begin();

//					vii p=amount.begin()+offset;

//					(*p)+=n_l_chips[l];

//				}else{

//					dev.push_back(d);

//					amount.push_back(n_l_chips[l]);

//				}

//				p_lot++;

//			}

//			for(int j=1;j<=(int)dev.size();j++){

//				int d=dev[j-1];

//				int num=amount[j-1];

//				if(is_KD_PD(d)){

//					if(soln_A.shortage[d-1].second>0){

//						benefit_lost+=penalty[d]/coef*num;

//					}else if(soln_A.shortage[d-1].second+num>0){

//						benefit_lost+=penalty[d]/coef*(soln_A.shortage[d-1].second+num);

//					}

//				}

//			}

//			vector< pair< pair<int,double>,double> > all_unassigned_lots;

//			all_unassigned_lots.resize((int)soln_A.unassigned_lots.size()+(int)soln_A.sub_soln[i-1].lots.int_vector.size());

//			copy(soln_A.unassigned_lots.begin(),soln_A.unassigned_lots.end(),all_unassigned_lots.begin());

//

//			cout<<"i="<<i<<",benefit_lost="<<benefit_lost<<endl;

//			//----------------------------------------------------------------------------------------

//			vii p_Lambda=Lambda_i[i].int_vector.begin();

//			while(p_Lambda!=Lambda_i[i].int_vector.end()){//loop over the lambda(i)

//				int Lambda_id=(*p_Lambda);

//				if(Lambda_id!=Lambda_id_cur){

//					int y_id_temp=index2_y(i,Lambda_id);

//					si->setColBounds(y_id_temp,1,1);

//					fix_y[i]=Lambda_id;

//

//					//cout<<"Lambda_id="<<Lambda_id<<endl;

//					if(make_feasible(fix_y)){

//						if(primal_dual){

//							primal_dual=false;

//							si->setHintParam(OsiDoDualInResolve,false,OsiHintDo);

//						}else{

//							si->setHintParam(OsiDoDualInResolve,true,OsiHintDo);

//						}

//						si->resolve();

//						if(si->isProvenOptimal()){//it is possible that the run is infeasible due to the tooling constriants

//							CoinDisjointCopyN(si->getColSolution(), y_limit+1, solution);

//						}else{

//							cout<<"!!!"<<endl;

//							si->setColBounds(y_id_temp,0,0);

//							p_Lambda++;

//							continue;

//						}

//						//---------------------------------------------------------------------------adjust solution[]

//						adjust_LP_solution(solution);

//

//						vector<sub_soln_item> sub_soln2;

//						vector< pair<int,double> > shortage2;

//						vector< pair< pair<int,double>,double> > unassigned_lots2;

//						//-----------------------------------------------------------------------solve this subproblem heuristically

//						double second_level_obj=second_level_heuristic(solution,sub_soln2,shortage2,unassigned_lots2,2);

//						if(second_level_obj<best_obj){

//							best_obj=second_level_obj;

//							best_i=i;

//							best_Lambda=sub_soln2[i-1].Lambda_id;

//							//cout<<"best_Lambda="<<best_Lambda<<endl;

//							copy(sub_soln2.begin(),sub_soln2.end(),best_sub_soln.begin());

//							copy(shortage2.begin(),shortage2.end(),best_shortage.begin());

//							best_unassigned_lots.resize((int)unassigned_lots2.size());

//							copy(unassigned_lots2.begin(),unassigned_lots2.end(),best_unassigned_lots.begin());

//						}

//					}

//					si->setColBounds(y_id_temp,0,0);

//				}

//				p_Lambda++;

//			}

//			if(Lambda_id_cur>0){

//				si->setColBounds(y_id,1,1);

//			}

//			fix_y[i]=Lambda_id_cur;

//		}

//		if(best_obj<cur_obj){

//			cout<<"improved obj="<<best_obj<<endl;

//			//iter_flag=true;

//			cur_obj=best_obj;

//			soln_A.sub_soln.clear();

//			soln_A.shortage.clear();

//			soln_A.unassigned_lots.clear();

//			soln_A.sub_soln.resize(Num_Machine);

//			soln_A.shortage.resize(Num_Device);

//			soln_A.unassigned_lots.resize((int)best_unassigned_lots.size());

//			copy(best_sub_soln.begin(),best_sub_soln.end(),soln_A.sub_soln.begin());

//			copy(best_shortage.begin(),best_shortage.end(),soln_A.shortage.begin());

//			copy(best_unassigned_lots.begin(),best_unassigned_lots.end(),soln_A.unassigned_lots.begin());

//		}

//	}

//	delete [] solution;

//	delete [] fix_y;

//}

//



//end by Zhufeng, Feb, 2014

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

	ofstream fout("Debug/penalty_key_device.csv");

	fout<<"coef, penalty,penalty/coef,whether_key_device"<<endl;

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



	for(int d=1;d<=Num_Device;d++)

	{

		double multiplier = penalty[d]/coef;

		fout <<coef<<","<<penalty[d]<<","<<multiplier<<",";

		int type=is_KD_PD(d);

		if(type == 1)

		{

			fout<<"Y"<<endl;

		}else

		{

			fout<<"N"<<endl;

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

		int Lambda_A=soln_A.sub_soln[i-1].Lambda_id;

		int Lambda_B=soln_B.sub_soln[i-1].Lambda_id;

		vii p_Lambda=Lambda_i[i].int_vector.begin();

		while(p_Lambda!=Lambda_i[i].int_vector.end()){

			int Lambda_id=(*p_Lambda);

			if(Lambda_id!=Lambda_A && Lambda_id!=Lambda_B){

				int id=index2_y(i,Lambda_id);

				si->setColBounds(id,0,0);

			}

			p_Lambda++;

		}

	}

	si->writeLp("AT_GRASP_post");

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

		int Lambda_A=soln_A.sub_soln[i-1].Lambda_id;

		int Lambda_B=soln_B.sub_soln[i-1].Lambda_id;

		if(Lambda_A>0){

			int id_A=index2_y(i,Lambda_A);

			row.insert(id_A,-1);

			num++;

		}else if(Lambda_B>0 && Lambda_B!=Lambda_A){

			int id_B=index2_y(i,Lambda_B);

			row.insert(id_B,-1);

			num++;

		}

	}

	si->addRow(row,5-num,20-num);

	si->writeLp("AT_GRASP_post");

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

		int Lambda_A=soln_A.sub_soln[i-1].Lambda_id;

		if(Lambda_A>0){

			int id_A=index2_y(i,Lambda_A);

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



	vector< vector<int> >  Lambda_list; //<i,<Lambda_id> >

	Lambda_list.resize(Num_Machine);

	vector< vector<double> > Prob_list; //<i,<probability> >

	Prob_list.resize(Num_Machine);



	bool int_flag=true;

	for(int id=x_limit+Num_Keydevice+Num_Package+1;id<=y_limit;id++){

		if(solution[id]>1E-06){

			vector<index_node>::iterator p_node=index_vector.begin()+id;

			int i=(*p_node).i;

			int Lambda_id=(*p_node).Lambda_id;

			Lambda_list[i-1].push_back(Lambda_id);

			Prob_list[i-1].push_back(solution[id]);

			//cout<<"i="<<i<<",Lambda_id="<<Lambda_id<<",prob="<<solution[id]<<endl;

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

			for(int j=1;j<=(int)Lambda_list[i-1].size();j++){

				vector<double>::iterator pp=Prob_list[i-1].begin()+j-1;

				if(rnd>=sum_prob && rnd<sum_prob+(*pp)){

					fix_y[i]=(*(Lambda_list[i-1].begin()+j-1));

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

		int Lambda_id=fix_y[i];

		//cout<<"Lambda_id="<<Lambda_id<<endl;

		if(Lambda_id>=1){

			vector<Lambda_item>::iterator pL=Lambda.begin()+Lambda_id-1;

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

            //Comment by Zhufeng for logpoint, Aug 31, 2011

			//int index=index_S_sim_d(sim,d);

			//end by Zhufeng



			double max_rate=-1E+6;

			int max_rate_s=-1;

			//Comment by Zhufeng for logpoint, Aug 31, 2011

			//vii p_s=S_sim_d[index].second.int_vector.begin();

			//while(p_s!=S_sim_d[index].second.int_vector.end()){

			//end by Zhufeng



            //Added by Zhufeng for logpoint, Aug 31, 2011

			int index=index_S_sim_l(sim,l);

			vii p_s=S_sim_l[index].second.int_vector.begin();

			while(p_s!=S_sim_l[index].second.int_vector.end()){

			//end by Zhufeng





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



//Comment by Zhufeng for logpoint, Aug 31, 2011



//void AT::initialize_S_sim_d(){

//	for(int sim=1;sim<=(int)SIM.size();sim++){

//		int i=SIM[sim-1].represented_machine_id;

//		for(int d=1;d<=Num_Device;d++){

//			if((int)Devices[d-1].device_lots.size()>0){

//				int l=*(Devices[d-1].device_lots.begin());

//				//cout<<"i="<<i<<",l="<<l<<endl;

//				vector<tooling_configure> TS;

//				get_S(TS,i,l);

//				if((int)TS.size()>0){

//					pair< pair<int,int>,vec_int > one_pair;

//					one_pair.first.first=sim;

//					one_pair.first.second=d;

//					vector<tooling_configure>::iterator p_TS=TS.begin();

//					while(p_TS!=TS.end()){

//						int s=(*p_TS).id;

//						one_pair.second.int_vector.push_back(s);

//						p_TS++;

//					}

//					S_sim_d.push_back(one_pair);

//				}

//			}

//		}

//	}

//}

//

//int AT::index_S_sim_d(int sim,int d){

//	int index=-1;

//	for(int id=1;id<=(int)S_sim_d.size();id++){

//		if(S_sim_d[id-1].first.first==sim && S_sim_d[id-1].first.second==d){

//			index=id-1;

//			break;

//		}

//	}

//	if(index<0){

//		cout<<"WRONG index!!! in index_S_sim_d()"<<endl;

//	}

//	return index;

//}

//

//void AT::show_S_sim_d(){

//	ofstream fout("Debug/debug_S_sim_d.txt");

//	for(int id=1;id<=(int)S_sim_d.size();id++){

//		fout<<"id="<<id-1<<",sim="<<S_sim_d[id-1].first.first<<",d="<<S_sim_d[id-1].first.second<<",s(sim,d)=";

//		vii p=S_sim_d[id-1].second.int_vector.begin();

//		while(p!=S_sim_d[id-1].second.int_vector.end()){

//			fout<<(*p)<<",";

//			p++;

//		}

//		fout<<endl;

//	}

//	fout.close();

//}



//end by Zhufeng



//Added by Zhufeng for logpoint, Aug 31, 2011
void AT::initialize_S_sim_l(){
	for(int sim=1;sim<=(int)SIM.size();sim++){
		int i=SIM[sim-1].represented_machine_id;
		for(int l=1;l<=Num_Lot;l++){
				//cout<<"i="<<i<<",l="<<l<<endl;
				vector<tooling_configure> TS;
				get_S(TS,i,l);
				if((int)TS.size()>0){
					pair< pair<int,int>,vec_int > one_pair;
					one_pair.first.first=sim;
					one_pair.first.second=l;
					vector<tooling_configure>::iterator p_TS=TS.begin();
					while(p_TS!=TS.end()){
						int s=(*p_TS).id;
						one_pair.second.int_vector.push_back(s);
						p_TS++;
					}
					S_sim_l.push_back(one_pair);
				}//end of if((int)TS.size()>0){
		}//end of for(int l=1;l<=Num_Lot;l++){
	}//end of for(int sim=1;sim<=(int)SIM.size();sim++){
}

int AT::index_S_sim_l(int sim,int l){
	int index=-1;
	for(int id=1;id<=(int)S_sim_l.size();id++){
		if(S_sim_l[id-1].first.first==sim && S_sim_l[id-1].first.second==l){
			index=id-1;
			break;
		}
	}
	if(index<0){
		cout<<"sim="<<sim<<",l="<<l<<",WRONG index!!! in index_S_sim_l()"<<endl;
	}
	return index;
}

void AT::show_S_sim_l(){
	ofstream fout("Debug/debug_S_sim_l.txt");
	for(int id=1;id<=(int)S_sim_l.size();id++){
		fout<<"id="<<id-1<<",sim="<<S_sim_l[id-1].first.first<<",l="<<S_sim_l[id-1].first.second<<",s(sim,l)=";
		vii p=S_sim_l[id-1].second.int_vector.begin();
		while(p!=S_sim_l[id-1].second.int_vector.end()){
			fout<<(*p)<<",";
			p++;
		}
		fout<<endl;
	}
	fout.close();
}
//end by Zhufeng for logpoint



int AT::select_alpha(double prob_list[],int length){

	int selected=-100;

	double rnd=rand()*1.0/RAND_MAX;

	double sum=0;

	for(int i=0;i<=length-1;i++){

		if(sum-1E-06<rnd && rnd<=sum+prob_list[i]+1E-06){

			selected=i; break;

		}

		sum+=prob_list[i];

	}

	return selected;

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

		int Lambda_id=IP_soln.sub_soln[i-1].Lambda_id;

		int y_id=index2_y(i,Lambda_id);

		si.setColBounds(y_id,1,1);

		si.resolve();

		CoinDisjointCopyN(si.getColSolution(), y_limit+1, LP_soln);



		vector< vector<int> >  Lambda_list; //<i,<Lambda_id> >

		Lambda_list.resize(Num_Machine);

		vector< vector<double> > Prob_list; //<i,<probability> >

		Prob_list.resize(Num_Machine);



		for(int id=x_limit+Num_Keydevice+Num_Package+1;id<=y_limit;id++){

			if(LP_soln[id]>1E-06){

				vector<index_node>::iterator p_node=index_vector.begin()+id;

				int i2=(*p_node).i;

				int Lambda_id2=(*p_node).Lambda_id;

				Lambda_list[i2-1].push_back(Lambda_id2);

				Prob_list[i2-1].push_back(LP_soln[id]);

				//cout<<"i2="<<i2<<",Lambda_id2="<<Lambda_id2<<",prob="<<LP_soln[id]<<endl;

			}

		}



		for(int iter=1;iter<=10;iter++){

			for(int ii=1;ii<=Num_Machine;ii++){

				double rnd=rand()*1.0/RAND_MAX;

				double sum_prob=0.0;

				for(int j=1;j<=(int)Lambda_list[ii-1].size();j++){

					vector<double>::iterator pp=Prob_list[ii-1].begin()+j-1;

					if(rnd>=sum_prob && rnd<sum_prob+(*pp)){

						fix_y[ii]=(*(Lambda_list[ii-1].begin()+j-1));

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

	cout<<"SL_avg length="<<SL_avg.size()<<endl;

	//cout<<"best_obj="<<best_obj<<",delta="<<delta<<endl;	

	vector<double> q;

	double sum=0;

	for(int j=1;j<=RCL_length;j++){

		int sim=candidate_list[j-1].first.first;

		int Lambda_id=candidate_list[j-1].first.second;

		//cout<<"sim="<<sim<<",Lambda_id="<<Lambda_id<<endl;

		int index=index_L_sim_lambda(sim,Lambda_id);

		//cout<<"index="<<index<<endl;

		double temp;

		if(SL_avg[index]<1){//this sim_lambda combination never be used before

			temp=1;

		}else{			

			//cout<<"SL_avg[]="<<SL_avg[index]<<",SL_best[]="<<SL_best[index]<<endl;	

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

  	//cout<<"rnd="<<rnd<<endl;	

	sum=0;

	int id=-100;

	for(int j=1;j<=(int)p.size();j++){

		if(sum-(1e-06)<=rnd && rnd<sum+p[j-1]+(1e-06)){

			//cout<<"!!!"<<endl;

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



int AT::is_sim_lambda_feasible(int simid,int Lambda_id,int * tooling_used,int * sim_used,int * SIM_number){

	//based on the current simiid and tooling_used[] (NOTE: these are feasible)

	//see if applying Lambda_id(tf,num,temperature) is feasible or not

	//return 0 if infeasible; 1 if feasible

	

	int flag_machine=0,flag_tooling=0;

	vector<Lambda_item>::iterator pL=Lambda.begin()+Lambda_id-1;

	//can we find the machine?

	if(sim_used[simid]+1<=SIM_number[simid]){ flag_machine=1;}

	else{ 

		return 0;

	}

	//can we find the tooling?

	//---------------------------------------------------

	//---------------------------------------------------

	vii p_tf=(*pL).tooling_vec.begin();

	if((*pL).tooling_vec.empty())

		cout<<"empty"<<endl;



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

	f_soln<<"Machine_Instance_ID,Machine_Family_ID,Machine_Instance_Name,Lot_ID,Lot_Name,Quantity,Lot_Weight,Lot_logpoint,Device ID,Device_Name,Route_ID,route_logpoint,Tooling_Family_ID,Tooling_Family_Name,Certification,Value"<<endl;

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

			f_soln<<LOT[l-1]<<","<<n_l_chips[l]<<","<<w_l[l]<<","<<lot_logpoint[l]<<",";

			int d=lots_device[l];

			f_soln<<d<<",";

			f_soln<<DEV[d-1]<<","<<s<<",";

			vector<tooling_configure>::iterator p_S=S.begin()+s-1;

			vii p_int=(*p_S).tf.tools.begin();

			int tf=(*p_int);

			f_soln<<(*p_S).logpoint<<","<<tf<<",";

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

	f_soln.open("Debug/solution.csv");

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

			if(tf>0){//It is possible that no tooling is required for the operation

				f_soln<<TF[tf-1]<<","<<(*p_S).temperature<<endl;

			}else{

				f_soln<<","<<(*p_S).temperature<<endl;

			}

			p_int++;

			while(p_int!=(*p_S).tf.tools.end()){

				for(int m=1;m<=6;m++){ f_soln<<",";}

				int tf=(*p_int);

				if(tf>0){//it is possible that no tooling is requrired for the operation

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

	ofstream f_shortage("Debug/shortage.csv");

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

	//cout<<"Num_Key_device="<<Num_Keydevice<<endl;

	//cout<<"Num_Package_device="<<Num_Package<<endl;

	//cout<<"Num_Key_Package="<<(int)Key_PKG.size()<<endl;

	//cout<<"Num_Key_PIN_Package="<<(int)Key_PIN_PKG.size()<<endl;



	//cout<<"total_weight="<<total_weight<<endl;

	//cout<<"total_shortage="<<total_shortage<<endl;

	//---------------------------------------------------------------------------

	/*

	ofstream f_package("Debug/package_production.csv");

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

	ofstream f_keydevice("Debug/keydevice_production.csv");

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

                

				//Updated by Zhufeng for logpoint, Aug 31, 2011



				/*int index=index_S_sim_d(sim,d);

				vii p_s=S_sim_d[index].second.int_vector.begin();

				while(p_s!=S_sim_d[index].second.int_vector.end()){*/



				int index=index_S_sim_l(sim,l);
				vii p_s=S_sim_l[index].second.int_vector.begin();			
				while(p_s!=S_sim_l[index].second.int_vector.end()){



					//end by Zhufeng for logpoint, Aug 31, 2011



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

	ofstream f_time("Debug/time.csv");

	f_time<<"Machine,Time"<<endl;

	for(int i=1;i<=Num_Machine;i++){

		vector<int>::iterator p_Lambda=Lambda_i[i].int_vector.begin();

		while(p_Lambda!=Lambda_i[i].int_vector.end()){

			int Lambda_id=(*p_Lambda);

			int offset=index2_y(i,Lambda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size()-1;

			int index=y_limit+1+offset;

			if(solution[index]>1E-06){

				f_time<<MI[i-1]<<","<<solution[index]<<endl;

			}

			p_Lambda++;

		}

	}

	f_time.close();

	//--------------------------------------------------------------------------

	ofstream f_kp("Debug/key_package_production.csv");

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



					//Updated by Zhufeng for logpoint, Aug 31, 2011



					/*int index=index_S_sim_d(sim,d);

					for(int id2=1;id2<=(int)S_sim_d[index].second.int_vector.size();id2++){

						int s=S_sim_d[index].second.int_vector[id2-1];*/

           
                    int index=index_S_sim_l(sim,l);

					for(int id2=1;id2<=(int)S_sim_l[index].second.int_vector.size();id2++){
						int s=S_sim_l[index].second.int_vector[id2-1];

						//end by Zhufeng for logpoint

                    

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

	ofstream f_kpp("Debug/key_pin_package_production.csv");

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



					//Updated by Zhufeng for logpoint, Aug 31, 2011



					/*int index=index_S_sim_d(sim,d);

					for(int id2=1;id2<=(int)S_sim_d[index].second.int_vector.size();id2++){

						int s=S_sim_d[index].second.int_vector[id2-1];*/



						 int index=index_S_sim_l(sim,l);

						 for(int id2=1;id2<=(int)S_sim_l[index].second.int_vector.size();id2++){
						int s=S_sim_l[index].second.int_vector[id2-1];



						//end by Zhufeng for logpoint



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

	//file x_ils and y_i Lambda

	for(int id=1;id<=(int)soln.sub_soln.size();id++){

		int i=soln.sub_soln[id-1].i;

		int Lambda_id=soln.sub_soln[id-1].Lambda_id;

		if(Lambda_id>0){//the machine is in used

			for(int id2=1;id2<=(int)soln.sub_soln[id-1].lots.int_vector.size();id2++){

				int l=soln.sub_soln[id-1].lots.int_vector[id2-1];

				int s=max_rate_s(i,l,Lambda_id);

				

				int index=index2_x(i,l,s);

				solution[index]=1.0;

				

				index=index2_y(i,Lambda_id);

				solution[index]=1.0;

				

				index=y_limit+index2_y(i,Lambda_id)-x_limit-Num_Keydevice-Num_Package-(int)Key_PKG.size()-(int)Key_PIN_PKG.size();

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

		int Lambda_id=soln.sub_soln[i-1].Lambda_id;

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

			int Lambda_id=soln.sub_soln[id-1].Lambda_id;

			if(Lambda_id>0){//the machine is in used

				double time_used=soln.sub_soln[id-1].time_used;



				cout<<"In makespan time_used="<<time_used<<endl;



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

					int Lambda_id=soln.sub_soln[id2-1].Lambda_id;

					if(Lambda_id>0){

						vector< pair<int,int> >::iterator p_pair=sim_lambda_L[l-1].second.sim_lambda_pairs.begin();

						while(p_pair!=sim_lambda_L[l-1].second.sim_lambda_pairs.end()){

							if((*p_pair).first==sim && (*p_pair).second==Lambda_id){

								assignable.push_back(id2); break;

							}

							p_pair++;

						}

					}

				}

				if((int)assignable.size()>=1){//lot l can be assgined to the vector

					int s_from=max_rate_s(soln.sub_soln[makespan_id-1].i,l,soln.sub_soln[makespan_id-1].Lambda_id);

					double time_from=n_l_chips[l]/rate[s_from]+Load_Unload_Time;

					double best_time=1E+10;

					double best_ID=-1;

					for(int id3=1;id3<=(int)assignable.size();id3++){

						int ID=assignable[id3-1];

						int s_to=max_rate_s(soln.sub_soln[ID-1].i,l,soln.sub_soln[ID-1].Lambda_id);

						double time_to=n_l_chips[l]/rate[s_to]+Load_Unload_Time;

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

						int s_to_best=max_rate_s(soln.sub_soln[best_ID-1].i,l,soln.sub_soln[best_ID-1].Lambda_id);

						double time_to_best=n_l_chips[l]/rate[s_to_best]+Load_Unload_Time;

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



void AT::machine_resetup(struct soln_pool_item & best_soln){

	//Enhancement Action 1

	//If a setup (unique machine + tooling combination) runs out of WIP, the model will re-setup that specific machine(s)

	//by choosing the NEXT BEST SETUP to run from the WIP that has not been allocated yet

	//---------------------------------------------------------------Need to recover the time horizone H[] due the change for initial lots

	for(int j=1;j<=Num_Machine;j++){

		if(effective_starting_time[j]<=10000){//if the initial lot can be processed by the machine

			H[j]+=effective_starting_time[j];

		}

		else{

			H[j]=0.0;

		}	

	}	

	cout<<"---------------------------------------------------------- Enhancement to resetup the machines when they are idle"<<endl;

	double eps=1E-06;



	vector<int> mc_no_GRASP_initial; //this vector stores the machines that are not used in GRASP solution but used to process initial lots

	struct soln_pool_item GRASP_soln=best_soln; //don't want to modify the original best solution

	for(int j=1;j<=(int)GRASP_soln.sub_soln.size();j++){

		if(GRASP_soln.sub_soln[j-1].Lambda_id>0){

			pair<sub_soln_item,double> one_pair;//the second double component stores the info of starting time

			one_pair.first=GRASP_soln.sub_soln[j-1];

			int i=one_pair.first.i;

			if(effective_starting_time[i]<10000){//if the initial lot can be processed by the machine	

				one_pair.second=effective_starting_time[i];//this is the starting time of the solution except the initial lots

			}else{

				one_pair.second=H[i];	

			}

			

			//Added by Zhufeng for multipass, Dec 31, 2011

			one_pair.first.which_pass=1;

			vii p_l=one_pair.first.lots.int_vector.begin();

		    while(p_l!=one_pair.first.lots.int_vector.end())

			{

				int lot_id=(* p_l);

				int logpoint=lot_logpoint[lot_id];

				one_pair.first.logpoints.int_vector.push_back(logpoint);

				p_l++;

			}



			//end by Zhufeng  Dec 31, 2011



			enhanced_solution.push_back(one_pair);

		}else{//for the machines that not used in the GRASP solution

			int i=GRASP_soln.sub_soln[j-1].i;

			for(int id=1;id<=(int)initial_running_lot.size();id++){

				if(i==initial_running_lot[id-1].machine_id){//if the machine is used to process initial lots 

					mc_no_GRASP_initial.push_back(i);

					break;

				}	

			}	

		}

	}



	cout<<"output mc_no_GRASP_initial"<<endl;

	for(int j=1;j<=(int)mc_no_GRASP_initial.size();j++){ cout<<mc_no_GRASP_initial[j-1]<<",";}

	cout<<endl;	

	//malloc the following arrays for spare machine/tooling pieces

	int * tooling_used=new int[Num_Tooling_Family*Num_Temperature];

	int * sim_used = new int[(int)SIM.size()+1];

	int * SIM_number=new int[(int)SIM.size()+1];

	//clear the arrays

	for(int id=0;id<=Num_Tooling_Family*Num_Temperature-1;id++){ tooling_used[id]=0;}

	for(int id=0;id<=(int)SIM.size();id++){ sim_used[id]=0; SIM_number[id]=0;}

	//modify the arrrays according to the current soln GRASP_soln

	vector<int> idle_machines;

	for(int j=1;j<=(int)GRASP_soln.sub_soln.size();j++){//loop over all the machines

		int machine_id=GRASP_soln.sub_soln[j-1].i;

		if(GRASP_soln.sub_soln[j-1].time_used<eps && GRASP_soln.sub_soln[j-1].Lambda_id<1 && effective_starting_time[machine_id]<eps){//the machine is idle

			//The reason to add the condition "effective_starting_time[machine_id]<eps" is b/c it is possible that a machine is processing an initial lot.

			//However,it is not activated in the GRASP solution. Such machine is actually NOT idle.	

			idle_machines.push_back(machine_id);

		}else{//the machine is running

			//-----------------------------------------------------------------update tooling_used[]

			int Lambda_id=GRASP_soln.sub_soln[j-1].Lambda_id;

			if(Lambda_id>=1){//If a machine is used to process initial lots and it is not used in the GRASP solution, then there is no tooling info	

				vector<Lambda_item>::iterator pL=Lambda.begin()+Lambda_id-1;

				int temperature=(*pL).temperature;

				for(int id=1;id<=(int)(*pL).tooling_vec.size();id++){

					int tf=(*pL).tooling_vec[id-1];

					int b =(*pL).b[id-1];

					int pos=(tf-1)*Num_Temperature+temperature-1;

					tooling_used[pos]+=b;

				}	

			}

			//------------------------------------------------------------------update sim_used

			int simid=machine_sim[machine_id];

			sim_used[simid]++;

		}

	}

	//uncommented by Zhufeng

	cout<<"idle_machines:"<<endl;

	for(int j=1;j<=(int)idle_machines.size();j++){ cout<<idle_machines[j-1]<<",";}

	cout<<endl;

	//end Zhufeng



	////------------------------------------------------------------------------initialize SIM_number[];

	for(int i=1;i<=Num_Machine;i++){

		int simid=machine_sim[i];

		SIM_number[simid]++;

	}

	//------------------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------------------

	//generate lot_remain---------------------------------------------

	vector<int> lot_remain;

	vector<int> assigned_lots;	

	for(int j=1;j<=(int)GRASP_soln.sub_soln.size();j++){

		for(int id=1;id<=(int)GRASP_soln.sub_soln[j-1].lots.int_vector.size();id++){

			int l=GRASP_soln.sub_soln[j-1].lots.int_vector[id-1];

			assigned_lots.push_back(l);	

		}	

	}

	//cout<<"assigned_lots:"<<endl;

	//for(int j=1;j<=(int)assigned_lots.size();j++){

	//	cout<<assigned_lots[j-1]<<",";

	//}

	//cout<<endl;	

	for(int j=1;j<=Num_Lot;j++){

		vii pfind=find(assigned_lots.begin(),assigned_lots.end(),j);

		if(pfind==assigned_lots.end()){

			lot_remain.push_back(j);

		}	

	}	

	//generate production for each device-----------------------------

	vector<int> production(Num_Device,0);

	for(int j=1;j<=(int)GRASP_soln.sub_soln.size();j++){

		for(int id=1;id<=(int)GRASP_soln.sub_soln[j-1].lots.int_vector.size();id++){

			int l=GRASP_soln.sub_soln[j-1].lots.int_vector[id-1];

			int d=lots_device[l];

			production[d-1]+=n_l_chips[l];

		}

	}

	vector< pair<double,vec_int> > machine_lambda_benefit; //it is used for machine-lambda combination 

	for(int machine_id=1;machine_id<=(int)Machine_vector.size();machine_id++){

		for(int id=1;id<=(int)Lambda_i[machine_id].int_vector.size();id++){	

			pair<double,vec_int> one_pair;

			one_pair.first=0;

			one_pair.second.int_vector.clear();

			machine_lambda_benefit.push_back(one_pair);

		}		

	}

	//--------------------

	vector< pair<int, pair<int,double> > > uptime; //this vector keep a records of the current uptime

	//Type 1: the machine is used in the GRASP solution and thus in enhanced_solution (it doesn't matter if the machine is used to process initial lot or not)

	//Type 2: the machine is used to process initial lot but not used in the GRASP solution. In other words, the machine can be available sometime later.	

	//added by Zhufeng 

	// <int <int,double>>. 

	// The first int denote the type. The second int denote id, the first double denote time when the machine will be available



	for(int id=1;id<=(int)enhanced_solution.size();id++){					 //NOTE: it is the position in the vector <enhanced_solution>

		double time=enhanced_solution[id-1].first.time_used+enhanced_solution[id-1].second;//time->when the machine will be available	

		uptime.push_back(make_pair(1,make_pair(id,time))); //record the id-th used time in the vector <enhanced_solution>

	}



	for(int id=1;id<=(int)mc_no_GRASP_initial.size();id++){

		int i=mc_no_GRASP_initial[id-1];

		uptime.push_back(make_pair(2,make_pair(i,effective_starting_time[i])));

	}	



	sort(uptime.begin(),uptime.end(),compare_int_int_double);//sort the vector <uptime> according to the uptime.second.second (double) in aescending order

	//cout<<"output uptime"<<endl;

	//for(int j=1;j<=(int)uptime.size();j++){

	//	cout<<"Type="<<uptime[j-1].first<<",id="<<uptime[j-1].second.first<<",time available="<<uptime[j-1].second.second<<endl;

	//}

	//----------------------

	//added by Zhufeng

	//cout<<"begin while"<<endl;

	//end Zhufeng



	while((int)uptime.size()>=1){

		//commented by Zhufeng

		//vector< pair< int, pair<int,double> > > next_uptime; //prepare for the next run

        //end Zhufeng



		//added by Zhufeng

		

		pair< int, pair<int,double> > next_uptime; //prepare for the resetup machine' next uptime

        //end Zhufeng

       

		//------------------------------------------------------------------------

		//Modified by Zhufeng,cancel the "for" iteration, only keep while iteration

		//

		//for(int i=1;i<=(int)uptime.size();i++){

		//	double time_used=uptime[i-1].second.second;

		//	if(time_used>eps){//if time_used=0, then the machine is idle

		//		//for the machines that are being run and will be the first to finish the job

		//		int type=uptime[i-1].first; //get the type of the element	

		//end Zhufeng

		

			double time_used=uptime[0].second.second;

			

			cout<<"time_used:"<<time_used<<endl;

			//commended by Zhufeng

            //double time_used=uptime[i-1].second.second;

			//end by Zhufeng

			if(time_used>eps){//if time_used=0, then the machine is idle

				//for the machines that are being run and will be the first to finish the job

				////Added by Zhufeng

				//	cout<<"OK-5,";

				//	//end by Zhufeng

				int type=uptime[0].first; //get the type of the element

				////Added by Zhufeng

				//	cout<<"OK-4,";

				//	//end by Zhufeng

                //commended by Zhufeng

				//int type=uptime[i-1].first;

				//end by Zhufeng

				int machine_id=-100;

				int Lambda_id=-100;	

				if(type==1){

					//added by Zhufeng

				 //   //Added by Zhufeng

					//cout<<"OK-3,";

					////end by Zhufeng

					int id=uptime[0].second.first;

			

					//get the position in the vector <enhanced_solution>

					//end Zhufeng



					//commented by Zhufeng

					//int id=uptime[i-1].second.first;

					//end Zhufeng

     //                //Added by Zhufeng

					//cout<<"OK-2.5,";

					////end by Zhufeng

					//need to detach the tooling from the machine, reduce tooling_used[] and sim_used[]

					machine_id=enhanced_solution[id-1].first.i;

					idle_machines.push_back(machine_id);//append machine_id as a new idle machine

     //                //Added by Zhufeng

					//cout<<"OK-2.1,";

					////end by Zhufeng

					Lambda_id=enhanced_solution[id-1].first.Lambda_id;

					// //Added by Zhufeng

					//cout<<"OK-2,";

					////end by Zhufeng

					struct date_time one_dt=increase_date_time(General_Snapshoot_time,time_used);

					

					cout<<"Try to reset the machine "<<machine_id<<" at time "<<one_dt.str<<"("<<time_used<<"hr) which is"

						<<" (simid="<<machine_sim[machine_id]<<" and Lambda_id="<<Lambda_id<<") before resetup."<<endl;

					

     //               //Added by Zhufeng

					//cout<<"OK-1,";

					////end by Zhufeng

					//cout<<"output sim_used and SIM_number before resetup the machine"<<endl;

					//for(int j=1;j<=(int)SIM.size();j++){

					//	cout<<"simid="<<j<<",sim_used="<<sim_used[j]<<",SIM_number="<<SIM_number[j]<<endl;

					//}

     //              //Added by Zhufeng

					//cout<<"OK0,";

					////end by Zhufeng

					vector<Lambda_item>::iterator pL=Lambda.begin()+Lambda_id-1;

					////Added by Zhufeng

					//cout<<"OK01,";

					////end by Zhufeng

					int temperature=(*pL).temperature;

					////Added by Zhufeng

					//cout<<"OK1,";

					////end by Zhufeng

					for(int j=1;j<=(int)(*pL).tooling_vec.size();j++){

						int tf=(*pL).tooling_vec[j-1];

						int b=(*pL).b[j-1];

						int pos=(tf-1)*Num_Temperature+temperature-1;

						tooling_used[pos]-=b;

					}

     //               //Added by Zhufeng

					//cout<<"OK2,";

					////end by Zhufeng

					int simid=machine_sim[machine_id];

     //                //Added by Zhufeng

					//cout<<"OK3,";

					////end by Zhufeng

					sim_used[simid]--;

					//added by Zhufeng. the process of type 2 is wrong now because we have known the initial setup in Yufen's code.

					//So the tooling info of type 2 machine has been known. 

					//actauly we don't need to consider type 2 anymore. we need to change all type 2 back to type 1.

				}else{//type==2

					//the next available machine will be a machine processing initial lots

					// //Added by Zhufeng

					//cout<<"OK,type==2";

					////end by Zhufeng

					machine_id=uptime[0].second.first;

					

					//commended by Zhufeng

					//machine_id=uptime[i-1].second.first;

					//end by Zhufeng

					struct date_time one_dt=increase_date_time(General_Snapshoot_time,time_used);

					cout<<"Try to reset the machine "<<machine_id<<" at time "<<one_dt.str<<"("<<time_used<<"hr) with"

                                                <<" (simid="<<machine_sim[machine_id]<<" and no tooling requirement)"<<endl;	

					//cout<<"output sim_used and SIM_number before resetup the machine"<<endl;

                                        //for(int j=1;j<=(int)SIM.size();j++){

                                        //        cout<<"simid="<<j<<",sim_used="<<sim_used[j]<<",SIM_number="<<SIM_number[j]<<endl;

                                        //}

					//NOTE: don't need to adjust the tooling info since no tooling is used to process initial lots	

					//Lambda_id will be -100 in this case	

					int simid=machine_sim[machine_id];

                                        sim_used[simid]--;

				     

                   //end Zhufeng

				}

				

				///---------------------------------------------------------------------------------Now try to build a list according to sim_used[],tooling_used[] and the unassigned lots

				//clear the vector <machine_lambda_benefit>



				for(int j=1;j<=(int)machine_lambda_benefit.size();j++){

					machine_lambda_benefit[j-1].first=0.0;

					machine_lambda_benefit[j-1].second.int_vector.clear();

				}



				vector<int> affected_dev;



				update_benefit_resetup(machine_id,Lambda_id,time_used,machine_lambda_benefit,lot_remain,production,affected_dev,tooling_used,sim_used,SIM_number,idle_machines);



				//---------------------------------------------------------------------------------build the CL

				vector< pair< pair<int,int>,double> > CL;

				construct_CL_resetup(CL,machine_lambda_benefit);



				//cout<<"output the CL"<<endl;

				//for(int j=1;j<=(int)CL.size();j++){

				//	vector< pair< pair<int,int>,double> >::iterator p_CL=CL.begin()+j-1;

				//	cout<<"id="<<j<<",machine="<<(*p_CL).first.first<<",Lambda_id="<<(*p_CL).first.second<<",benefit="<<(*p_CL).second<<endl;

				//}



				sort(CL.begin(),CL.end(),compare_candidates); //sort the candidate of CL



				//investigate the top element of CL

				bool recover_flag=false;

				if((int)CL.size()==0){ recover_flag=true;

				//added by Zhufeng

				cout<<"keep original setup 1"<<endl;

				//end Zhufeng

				}

				else{ 

					if(CL[0].second<eps){ 

						recover_flag=true;

					//added by Zhufeng

					    cout<<"keep original setup 2"<<endl;

					//end Zhufeng

					}

				}

				

				if(recover_flag){//the setup should be recover (there is no benefitial setup)

					//----------------------------------------------------------------------reover to the original setup

					//NOTE that in this case not change over time will be induced

					//idle_machines.pop_back();  //remove the last element, which is the machine just added in the current iteration

					//comment out this part, since the machine and tooling will be detached no matter it is benefitial or not	

					//vector<Lambda_item>::iterator pL=Lambda.begin()+Lambda_id-1;

					//int temperature=(*pL).temperature;

					//for(int j=1;j<=(int)(*pL).tooling_vec.size();j++){

					//	int tf=(*pL).tooling_vec[j-1];

					//	int b=(*pL).b[j-1];

					//	int pos=(tf-1)*Num_Temperature+temperature-1;

					//	tooling_used[pos]+=b;

					//}

					//int simid=machine_sim[machine_id];

					//cout<<"simid="<<simid<<endl;

					//sim_used[simid]++;

				}else{//in this case, resetting the machine will be benefitial

					//NOTE: It is possible that the current (machine_id,Lamdua_id) still can be used to process lots. In that case, keep the original setup

					bool keep_org_setup=false;

					if(type==1){//only consider type 1 machine when detecting keep_org_setup since type 2 machine donesn't have setup	

						int pos_org_setup=0;

						for(int i=1;i<=(int)Machine_vector.size();i++){

							bool found_flag=false;	

							for(int j=1;j<=(int)Lambda_i[i].int_vector.size();j++){

								if(i==machine_id && Lambda_i[i].int_vector[j-1]==Lambda_id){//found the machine-lambda combination

									found_flag=true; break;

								}else{

									pos_org_setup++;

								}

							}

							if(found_flag){break;}	

						}	

						if(machine_lambda_benefit[pos_org_setup].first>eps){ 

							keep_org_setup=true;

							cout<<"keep the original setup"<<endl;

						}

					}	

					//--------------------------------------------------------------------------------

					double resetup_time=0; //NOTE that it is time to consider the change over time. If the the machine keep the original setup

					//there is no change over time. OW,taken into account the change over time

					int machine_selected,Lambda_id_selected;

					if(keep_org_setup){

						machine_selected=machine_id; 

						Lambda_id_selected=Lambda_id;

						resetup_time=0;

					}else{

						//added by Zhufeng

						cout<<"choose from CL"<<endl;

						//end Zhufeng

						machine_selected=CL[0].first.first;

						Lambda_id_selected=CL[0].first.second;

						resetup_time=0;

						vector<Lambda_item>::iterator pp=Lambda.begin()+Lambda_id_selected-1;

						for(int j=1;j<=(int)(*pp).tooling_vec.size();j++){//compute the change over time

							int tf=(*pp).tooling_vec[j-1];

							resetup_time+=Tooling_Setup_Time[tf];

						}

					}

					cout<<"machine_selected="<<machine_selected<<",Lambda_id_selected="<<Lambda_id_selected<<",change over time="<<resetup_time<<endl;

					//------------------------------------------------------need to locate the position in <machine_lambda_benefit>

					int pos=0;

					for(int i=1;i<=(int)Machine_vector.size();i++){

						bool found_flag=false; 

						for(int j=1;j<=(int)Lambda_i[i].int_vector.size();j++){

							if(i==machine_selected && Lambda_i[i].int_vector[j-1]==Lambda_id_selected){//found the machine-lambda combination

								found_flag=true; 

								break;

							}else{

								pos++;

							}

						}

						if(found_flag){break;}        

					}	

					

					cout<<"the selected lots are:";

					for(int j=1;j<=(int)machine_lambda_benefit[pos].second.int_vector.size();j++){ cout<<machine_lambda_benefit[pos].second.int_vector[j-1]<<",";}

					cout<<endl;

					//---------------------------------------------------------Now we know which lots to be assigned

					pair<sub_soln_item,double> one_pair;

					one_pair.second=time_used;  //This is the starting time for the new setup (NOT include the resetup time)

					cout<<"the starting time is time_used="<<time_used<<endl;	

					one_pair.first.i=machine_selected;	

					one_pair.first.Lambda_id=Lambda_id_selected;

					vii pfind=find(idle_machines.begin(),idle_machines.end(),machine_selected);

					if(pfind==idle_machines.end()){

						cout<<"Warnning message: the selected machine "<<machine_selected<<" is NOT an idle machine!!!";

					}else{

						idle_machines.erase(pfind);

					}	

					//the lots in machine_lambda_benefit[pos].second.int_vector[] has been sorted according to the benefit in update_benefit()

					one_pair.first.lots.int_vector.resize((int)machine_lambda_benefit[pos].second.int_vector.size());

					copy(machine_lambda_benefit[pos].second.int_vector.begin(),machine_lambda_benefit[pos].second.int_vector.end(),one_pair.first.lots.int_vector.begin());

					//----------------------------------------------------------------time used for the new setup

					double duration=resetup_time; //The change over time should be considered as part of the duration

					for(int j=1;j<=(int)one_pair.first.lots.int_vector.size();j++){

						int l=one_pair.first.lots.int_vector[j-1];

						int s=max_rate_s(one_pair.first.i,l,one_pair.first.Lambda_id);

						double time_required=n_l_chips[l]/rate[s]+Load_Unload_Time;	

						//cout<<"time_required="<<time_required<<endl;	

						duration+=time_required;

					}

					cout<<"duration="<<duration<<endl;	

					one_pair.first.time_used=duration;

					double new_uptime=one_pair.second+duration; //This is also the completion time



					//Comment by Zhufeng, Feb 2014, 

					//if(new_uptime<H[one_pair.first.i]){//if there still remainning time when the new lot-assignment is finished

					//end by Zhufeng, Feb 2014

					//Added by Zhufeng, for the option of the last lot beyond the horizon. Feb 2014

					if(new_uptime<H[one_pair.first.i] ||

					   (one_pair.second < H[one_pair.first.i] && new_uptime >= H[one_pair.first.i] && beyond_horizon ==1)){//if there still remainning time when the new lot-assignment is finished

					//End by Zhufeng. Feb 2014

						//commented by Zhufeng

						//next_uptime.push_back(make_pair(1,make_pair((int)enhanced_solution.size()+1,new_uptime)));

						//end by Zhufeng

						

						//added by Zhufeng

                        next_uptime=make_pair(1,make_pair((int)enhanced_solution.size()+1,new_uptime));

						//end zhufeng

						//added by Zhufeng. To fix a bug

	                   uptime.push_back(next_uptime);

		               sort(uptime.begin(),uptime.end(),compare_int_int_double);

					   cout<<"the resetup machine has been added to the right position "<<endl;

		               //end Zhufeng

                        //end by Zhufeng



						//this uptime corresponds to the new element to be add to <enhanced_solution>

						//this uptime is a type 1 machine	

					}



					

					//Added by Zhufeng for multipass, Dec 31, 2011

					one_pair.first.which_pass=1;

					vii p_l=one_pair.first.lots.int_vector.begin();

					 while(p_l!=one_pair.first.lots.int_vector.end())

					{

						int lot_id=(* p_l);

						int logpoint=lot_logpoint[lot_id];

						one_pair.first.logpoints.int_vector.push_back(logpoint);

						p_l++;

					}

					//end by Zhufeng  Dec 31, 2011



					enhanced_solution.push_back(one_pair);

					//----------------------------------------------------------------need to removed the assigned lots from <lot_remain>

					vector<int> lot_remain_copy;

					for(int j=1;j<=(int)lot_remain.size();j++){

						int l=lot_remain[j-1];

						vii pfind=find(one_pair.first.lots.int_vector.begin(),one_pair.first.lots.int_vector.end(),l);

						if(pfind==one_pair.first.lots.int_vector.end()){//the lot is not assigned and should be kept in <lot_remain>

							lot_remain_copy.push_back(l);

						}

					}

					lot_remain.clear();

					lot_remain.resize((int)lot_remain_copy.size());

					copy(lot_remain_copy.begin(),lot_remain_copy.end(),lot_remain.begin());



					cout<<"lot_remain size = "<<lot_remain.size();



					//---------------------------------------------------------Also need to update the SIM and tooling info

					vector<Lambda_item>::iterator pL=Lambda.begin()+Lambda_id_selected-1;

					int temperature=(*pL).temperature;

					for(int j=1;j<=(int)(*pL).tooling_vec.size();j++){

						int tf=(*pL).tooling_vec[j-1];

						int b=(*pL).b[j-1];

						int pos=(tf-1)*Num_Temperature+temperature-1;

						tooling_used[pos]+=b;

					}

					sim_used[machine_sim[machine_selected]]++;

					//----------------------------------------------------------------need to update the tooling_trace

					double start_time=time_used;	

					double end_time=new_uptime;	

					for(int j=1;j<=(int)(*pL).tooling_vec.size();j++){

						int tf=(*pL).tooling_vec[j-1];

                                                int b=(*pL).b[j-1];

						int count=0;

						for(int k=1;k<=(int)tooling_trace.size();k++){

                                        		//now pick the b tooling pieces in the family tf. Note that the tooling pieces should be able to work under the temperature

                                        		int tooling_id=tooling_trace[k-1].id; //this is the id for the tooling piece

                                        		if(tooling_tf[tooling_id]==tf){

                                                		vector<Tooling>::iterator pT=Tooling_vector.begin()+tooling_id-1; //point to the Tooling item

                                                		vii pfind=find((*pT).temperature_vector.begin(),(*pT).temperature_vector.end(),temperature); //look for the temperature

                                                		if(pfind!=(*pT).temperature_vector.end()){

                                                        		bool flag1=false,flag2=false;

                                                        		if((int)tooling_trace[k-1].schedule.size()<1){ flag1=true;} //there is no assignment for the tooling piece

                                                        		else{

                                                                		int size=(int)tooling_trace[k-1].schedule.size();

                                                                		double available_time=tooling_trace[k-1].schedule[size-1].second.second; //this is the earliest time the tooling is available

                                                                		if(start_time>available_time){ flag2=true;}

                                                        		}

                                                        		//--now we check if the current tooling piece is assignable

                                                        		if(flag1 || flag2){

                                                                		tooling_trace[k-1].schedule.push_back(make_pair(machine_selected,make_pair(start_time,end_time)));

                                                                		count++;

                                                       			}

                                               			 }

                                       			}//end_if

                                        		if(count>=b){ break;}

                                		}//end_for

	

					}//end_for	

				}

				//test whether this is the end of "if(time_used>esp)"

				

			}

             //added by Zhufeng

				//delete the first element in the vector "uptime"

				vector< pair<int, pair<int,double> > >:: iterator zhufeng=uptime.begin();

				uptime.erase(zhufeng);

				cout<<"delete the first element"<<endl;

				//end Zhufeng



			//commended by Zhufeng, since no "for" iteration

            

		//}//end_for each <uptime>

			//end Zhufeng



		//cout<<"output the next_uptime"<<endl;

		//for(int j=1;j<=(int)next_uptime.size();j++){

		//	cout<<"type="<<next_uptime[j-1].first<<",id="<<next_uptime[j-1].second.first<<",uptime="<<next_uptime[j-1].second.second<<endl;

		//}



		//commended by Zhufeng

		//uptime.clear();

		//uptime.resize((int)next_uptime.size());

		//copy(next_uptime.begin(),next_uptime.end(),uptime.begin());

        //end Zhufeng



		

	}//end_while

	show_enhanced_solution(enhanced_solution);

	cout<<"obj = "<<compute_obj(enhanced_solution)<<endl;



	//added by Zhufeng
	fstream fcout("Output/resultsummery.txt", ios::out|ios::app);
	fcout<<"After resetup"<<endl;
	fcout<<"obj = "<<compute_obj(enhanced_solution)<<endl;
	fcout.close();
	//end by Zhufeng



	//cout<<"final lot_remain"<<endl;

	//for(int id=1;id<=(int)lot_remain.size();id++){ cout<<lot_remain[id-1]<<",";}

	//cout<<endl;

	//free the arrays

	delete [] tooling_used;

	delete [] sim_used;

	delete [] SIM_number;

}



void AT::show_enhanced_solution(vector< pair<sub_soln_item,double> > & enhanced_solution){

	cout<<"--------------------------------------------------------------show solution after resetup the idle machines"<<endl;

	for(int id=1;id<=(int)enhanced_solution.size();id++){

		struct date_time start=increase_date_time(General_Snapshoot_time,enhanced_solution[id-1].second);

		struct date_time end  =increase_date_time(General_Snapshoot_time,enhanced_solution[id-1].second+enhanced_solution[id-1].first.time_used);

		cout<<"Machine "<<enhanced_solution[id-1].first.i<<" is setup with Lambda "<<enhanced_solution[id-1].first.Lambda_id

			<<". The starting time is "<<start.str<<". The duration is "<<enhanced_solution[id-1].first.time_used

			<<". The finished time is "<<end.str<<endl;

	}

}



void AT::show_initial_running_lot(vector<initial_running_lot_item> & initial_running_lot){

	ofstream fcout("Debug/initial_running_lot.txt");

	fcout<<"--------------------------------------------------------------show the initial_running_lot"<<endl;

	for(int id=1;id<=(int)initial_running_lot.size();id++){

		fcout<<"Lot "<<initial_running_lot[id-1].lot_name<<" is initially running on machine "<<initial_running_lot[id-1].machine_name

			<<".The device is "<<initial_running_lot[id-1].dev_name

			<<". The starting time is "<<initial_running_lot[id-1].Start_time.str

			<<". The current snapshoot time is "<<initial_running_lot[id-1].Snapshoot_time.str

			<<". Thus the lot has been running for "<<get_date_time_difference(initial_running_lot[id-1].Snapshoot_time,initial_running_lot[id-1].Start_time)<<" hours.";

		if(initial_running_lot[id-1].time_remain > 0)

		{

		    struct date_time completion_time=increase_date_time(initial_running_lot[id-1].Snapshoot_time,initial_running_lot[id-1].time_remain);

			output_date_time(completion_time);

		fcout<<"It can be finished at "<<completion_time.str;

		}else

		{

			fcout<<" it can be finished "<<(-initial_running_lot[id-1].time_remain)<<"hours earlier before the Snapshoot time";



		}

			fcout<<" The processing rate is "<<initial_running_lot[id-1].rate<<" PPH."

		    <<"  logpoint = "<<initial_running_lot[id-1].logpoint
            <<"  operation = "<<initial_running_lot[id-1].operation<<endl;

	}

}



void AT::extend_initial_lot(vector<initial_running_lot_item> & initial_running_lot){

	//This function is called immediately after <initial_running_lot> is ready

	//-------------------------------------------------------------------This is part extend <DEV> <PKG> device_PKG and device_PIN

	for(int id=1;id<=(int)initial_running_lot.size();id++){

		//-----------------------------figure out the device id

		int d=-100;

		for(int j=1;j<=(int)DEV.size();j++){

			if(!strcmp(DEV[j-1],initial_running_lot[id-1].dev_name)){

				d=j; break;

			}

		}

		if(d<0){//the vector <DEV> does not contain such a device

			cout<<"Add device "<<initial_running_lot[id-1].dev_name<<" to the set of devices DEV"<<endl;

			char * name=new char[50];

			strcpy(name,initial_running_lot[id-1].dev_name);

			DEV.push_back(name); //then put the device as a new device into <DEV>

			d=(int)DEV.size();

			Num_Device=(int)DEV.size(); //update the number of devices

		}

		initial_running_lot[id-1].dev_id=d;

		//-------------------------------figure out the package id

		int pkg=-100;

		for(int j=1;j<=(int)PKG.size();j++){

			if(!strcmp(PKG[j-1],initial_running_lot[id-1].PKG_name)){

				pkg=j; break;

			}

		}

		if(pkg<0){//the package name is not contained in <PKG>

			cout<<" Add package "<<initial_running_lot[id-1].PKG_name<<" to the set of packages PKG"<<endl;

			char * name=new char[50];

			strcmp(name,initial_running_lot[id-1].PKG_name);

			PKG.push_back(name);

			pkg=(int)PKG.size();

		}

		initial_running_lot[id-1].pkg_id=pkg;

		device_PKG[d]=pkg;

		device_PIN[d]=initial_running_lot[id-1].PIN_num;

		//-------------------------------------------------------------------------------------------------------------------------------------

	}

}



void AT::process_initial_lot(vector<initial_running_lot_item> & initial_running_lot,double * H){

	

//=====================================================================================================================



	//------------- added by yufen ---------------------

	for(int id=1;id<=(int)initial_running_lot.size();id++)

	{

		int machineId = initial_running_lot[id-1].machine_id;

		



		if (!Machine_vector[machineId-1].hasInitialToolingConfigure)//Zhufeng:if the initial setup doesn't have corresponding route, we skip the initial lot and initial setup. take the machine as non-initial machine

			                                                        // So this part won't make effect any more.

		{

			cout<<"Warnning Message:"<<endl;

            //comment by Zhufeng. This has a bug. It takes machineId as MF_id.

			//cout<<"There is no route associated to device "<<initial_running_lot[id-1].dev_name<<" and machine group "<<MF[machineId-1]<<endl;

			//end by Zhufeng

			//Added by Zhufeng

            cout<<"There is no route associated to device "<<initial_running_lot[id-1].dev_name<<" and machine group "<<MF[Machine_vector[machineId-1].mg-1]<<endl;

		

			//end by Zhufeng

			cout<<"The initial lot will stay in the machine and never be processed!"<<endl;

			cout<<"Set the machine available time H to zero such that this machine is not used at all."<<endl;

			H[machineId]=0;

			effective_starting_time[machineId]=1E+06;	

		

		}else{

			int routeId = Machine_vector[machineId-1].initial_tooling_configure;

			initial_running_lot[id-1].rate=S[routeId-1].PPH;

			double time_required=(double)initial_running_lot[id-1].n_l_chips/initial_running_lot[id-1].rate; //compute the total time that requried to process the initial lot

			double time_consumed=get_date_time_difference(initial_running_lot[id-1].Snapshoot_time,initial_running_lot[id-1].Start_time); //how much time has been consumed

			double time_remain=time_required-time_consumed;      //compute how much more time need to be used

			initial_running_lot[id-1].time_remain=time_remain;

			if(time_remain>0){

				H[machineId]-=time_remain; //reduce the available machien time for the machine

				effective_starting_time[machineId]=time_remain;	

				if(H[machineId]<0){

					cout<<"Message: the lot "<<initial_running_lot[id-1].lot_name<<" will take another "<<time_remain<<" hours to finish, which is longer than the time horizon."<<endl;

					effective_starting_time[machineId]=H[machineId]+time_remain;	// set the end of the horizon as the starting time of the machine

					H[machineId]=0;													// set 0 as the machine time

					

				}

			}else{//else_if time_remain<0, the initial lot is actually finished already. Output this message

				cout<<"Warnning Message: the lot "<<initial_running_lot[id-1].lot_name<<" has been actually finished on machine "

					<<initial_running_lot[id-1].machine_id<<" and should not be counted as initila lot."<<endl;

				effective_starting_time[machineId]=0.0;	

			}

		}

	}



	ofstream fdebug("Debug/debug_updated_H.txt");	

	fdebug<<"The updated time horizon for the machines is (subtract the time to process the initial lots)"<<endl;

	for(int i=1;i<=(int)Machine_vector.size();i++){

		fdebug<<"i="<<i<<",H="<<H[i]<<endl;	

	}	

	fdebug.close();

	fdebug.clear();

//------------- end yufen -----------------------------

//==============================================================================================

	

	//------------- commented by yufen -------------------------

	/*	

	//This fucntion should be called after <initial_running_lot> is ready but should be called before building models



    	//This function process the intial lots that are running on some machines at the snapshoot time

	//The available machine time H[] is reduced for the machines that running the initial lots

	//However,when computing the total obj,e.g.compute_obj(),the devices contained in the initial lots should be taken into account

	for(int id=1;id<=(int)initial_running_lot.size();id++){

		double time_consumed=get_date_time_difference(initial_running_lot[id-1].Snapshoot_time,initial_running_lot[id-1].Start_time); //how much time has been consumed

		int mg=machine_mg[initial_running_lot[id-1].machine_id]; //get machine group

		//---------------------------------------------------------------figure out the device id

		int d=-1;

		for(int j=1;j<=(int)DEV.size();j++){

			if(!strcmp(DEV[j-1],initial_running_lot[id-1].dev_name)){

				d=j; break;

			}

		}

		//-----------------------------------------------------------------------------------figure out which route is used for the initial lot

		//---------------------------------------------since the <S> has been clean right after reading the rout.csv file. It is possible that the device has been cleaned.

		//---------------------------------------------we need to reopen the route.csv file to read the info

		vector<double> PPH;

		char temp[2000],pdes[1000];

		ifstream f_route("Data/route.csv");

		f_route.getline(temp,1000,'\n');

		f_route.getline(temp,1000,'\n');

		int count=0;

		while(!f_route.eof()){

			int l=0,k=0;

			for(int j=1;j<=6;j++){//get the device name

				k=strtok2(pdes,temp,l,',');

				l=k;

			}

			trim(pdes);

			if(!strcmp(initial_running_lot[id-1].dev_name,pdes)){//if this match processes the device contained in the initial lot

				for(int j=1;j<=6;j++){//get the machine group

					k=strtok2(pdes,temp,l,',');

					l=k;

				}

				trim(pdes);

				if(!strcmp(pdes,MF[mg-1])){//if we got a match for the machine group too

					l=0;

					for(int j=1;j<=11;j++){//get the PPH

						k=strtok2(pdes,temp,l,',');

						l=k;

					}		

					trim(pdes);

					//cout<<"rate = "<<pdes<<endl;

					PPH.push_back((double)atof(pdes));

				}

			}

			f_route.getline(temp,1000,'\n');

		}

		f_route.close();

		f_route.clear();

		//--------------------------------------------------------------------------------------------------------

		if((int)PPH.size()==0){ 

			cout<<"Warnning Message:"<<endl;

			cout<<"There is no route associated to device "<<initial_running_lot[id-1].dev_name<<" and machine group "<<MF[mg-1]<<endl;

			cout<<"The initial lot will stay in the machine and never be processed!"<<endl;

			//abort(); //instead of aborting the process,set H[] to 0 such that the machine is not used at all

			cout<<"Set the machine available time H to zero such that this machine is not used at all."<<endl;

			H[initial_running_lot[id-1].machine_id]=0;

			effective_starting_time[initial_running_lot[id-1].machine_id]=1E+06;	

		}else{

			sort(PPH.begin(),PPH.end()); //This is in ascending order

			double max_rate=PPH[(int)PPH.size()-1];

			initial_running_lot[id-1].rate=max_rate;  //get the maximum processing rate

			double time_required=initial_running_lot[id-1].n_l_chips/max_rate+Load_Unload_Time; //compute the total time that requried to process the initial lot

			double time_remain=time_required-time_consumed;      //compute how much more time need to be used

			initial_running_lot[id-1].time_remain=time_remain;

			if(time_remain>0){

				H[initial_running_lot[id-1].machine_id]-=time_remain; //reduce the available machien time for the machine

				effective_starting_time[initial_running_lot[id-1].machine_id]=time_remain;	

				if(H[initial_running_lot[id-1].machine_id]<0){

					cout<<"Message: the lot "<<initial_running_lot[id-1].lot_name<<" will take another "<<time_remain<<" hours to finish, which is longer than the time horizon."<<endl;

				}

			}else{//else_if time_remain<0, the initial lot is actually finished already. Output this message

				cout<<"Warnning Message: the lot "<<initial_running_lot[id-1].lot_name<<" has been actually finished on machine "

					<<initial_running_lot[id-1].machine_id<<" and should not be counted as initila lot."<<endl;

				effective_starting_time[initial_running_lot[id-1].machine_id]=0.0;	

			}

		}

		//--------------------------------------------------------------------------------------

	}

	ofstream fdebug("Debug/debug_updated_H.txt");	

	fdebug<<"The updated time horizon for the machines is (subtract the time to process the initial lots)"<<endl;

	for(int i=1;i<=(int)Machine_vector.size();i++){

		fdebug<<"i="<<i<<",H="<<H[i]<<endl;	

	}	

	fdebug.close();

	fdebug.clear();

	*/

	//--------------------- end yufen --------------------------------------

}



void AT::generate_reports(vector<initial_running_lot_item> & initial_running_lot,vector< pair<sub_soln_item,double> > & enhanced_solution){

	//This function creats the report files for the whole results after enhancement

	ofstream f_soln("Output/solution.csv");



	//Added by Zhufeng

	//add completion time of each lot.

	f_soln<<"Machine_Instance,Machine_Family_Name,Lot_Name,Logpoint,Quantity,Lot_Weight,Initial_Lot_Flag,Device_Name,Tooling_Family_Name,Certification,Setup_Time,Start_Time,Completion_Time,Pass_No,Finished"<<endl;

	//first output the initial lots

	for(int j=1;j<=(int)initial_running_lot.size();j++){

		if(initial_running_lot[j-1].time_remain>0){//Only the valid initial lot is output

			//It is okay that the lot will go across the whole horizon. It will still be output but not be counted in the total throughput

			int i=initial_running_lot[j-1].machine_id;

			int mg=machine_mg[i];

			struct date_time dt=increase_date_time(General_Snapshoot_time,initial_running_lot[j-1].time_remain);

						output_date_time(dt);

			//Feb 18, 2013, for lot start time

			struct date_time start_time=initial_running_lot[j-1].Start_time;

			output_date_time(start_time);



			//Note that there is neither tooling info nor certification info for the initial lots,so leave it blank

			f_soln<<initial_running_lot[j-1].machine_name<<","<<MF[mg-1]<<","<<initial_running_lot[j-1].lot_name<<","<<initial_running_lot[j-1].logpoint<<","<<initial_running_lot[j-1].n_l_chips

				<<","<<initial_running_lot[j-1].w_l<<",Y,"<<initial_running_lot[j-1].dev_name<<",,,"<<General_Snapshoot_time.str<<","<<start_time.str<<","<<dt.str<<","<<1<<endl;

		}

	}

	//secondary,output the <enhanced_solution>

	for(int j=1;j<=(int)enhanced_solution.size();j++){

		int i=enhanced_solution[j-1].first.i;

		int mg=machine_mg[i];

		int Lambda_id=enhanced_solution[j-1].first.Lambda_id;

		double current_time=enhanced_solution[j-1].second;

		

		int s=enhanced_solution[j-1].first.s;

	



		//Added by Zhufeng for multipass, Jan 01, 2012

		int pass=enhanced_solution[j-1].first.which_pass;

        //end by Zhufeng, Jan 01 

		

		

		//Added by Zhufeng

        //To test whether there is resetup time.If the end_time of the machine is later than sum of time for all lots in the machine,

		//there must be resetup time. The resetup_time is just the difference of end_time and completion_time of all lots.

		double end_time=current_time+enhanced_solution[j-1].first.time_used;

		

		double current_time2=current_time; //current_time2 is to store the completion time of all lots not including resetup time



			//Feb18,2013,Zhufeng, for the starting time of the lot differs than the starting time of the machine

			double starting_time_lot = current_time;

			//end Feb18,2013

	

		for(int id=1;id<=(int)enhanced_solution[j-1].first.lots.int_vector.size();id++){

			int l=enhanced_solution[j-1].first.lots.int_vector[id-1];

				int d=lots_device[l];

				//MOdified by Zhufeng for mulitpass, Dec 31, 2011



				//Comment by Zhufeng, Feb 10, 2012, not used any more. all first pass

				//if(pass==1)

				//{

				//end by Zhufeng



					s=max_rate_s(i,l,Lambda_id);



				//Comment by Zhufeng, Feb 10, 2012, not used any more. all first pass

				//}

				

				/*else

				{

					s=max_rate_s_second_pass(i,l,Lambda_id);

				}*/

				//end by Zhufeng, Feb 10, 2012





				//end by Zhufeng, jan 01, 2012

				



				double time_required=n_l_chips[l]/rate[s]+Load_Unload_Time;

				current_time2+=time_required;

		}

		

		double resetup_time=0;

		if(current_time2<end_time){

			resetup_time=end_time-current_time2;

			

		}

        //end Zhufeng

        cout<<MI[i-1]<<"current_time= "<<current_time<<endl;



		if(current_time<=H[i]){//if the starting time is within the planning horizon

			//double time_start=enhanced_solution[j-1].second;





			for(int id=1;id<=(int)enhanced_solution[j-1].first.lots.int_vector.size();id++){

				int l=enhanced_solution[j-1].first.lots.int_vector[id-1];

				int d=lots_device[l];





				//Added by Zhufeng for multipass, Jan 01, 2012

				int logpoint=enhanced_solution[j-1].first.logpoints.int_vector[id-1];



				//Uncomment by Zhufeng, Feb 10, 2011. This cause a big mistake in the output of machine time!!

				//Comment by Zhufeng for multipass, Dec 31, 2011

				int s=max_rate_s(i,l,Lambda_id);

				//end Feb 10 ,2011



				double time_required=n_l_chips[l]/rate[s]+Load_Unload_Time;

			

				current_time+=time_required;

				//end by Zhufeng, Dec 31, 2011



				//Added by Zhufeng

				if(id==1){

					current_time+=resetup_time;

				}

                //end Zhufeng



				//cout<<MI[i-1]<<",current_time="<<current_time<<endl;

				



				//Comment by Zhufeng, Feb, 2014, for the last lot beyong the horizon

				//if(current_time<=H[i]){

				//end by Zhufeng, Feb, 2014

				//Added by Zhufeng,Feb,2014

				if(current_time<=H[i] ||

					(current_time>H[i]&&beyond_horizon ==1)){

				//end by Zhufeng, Feb, 2014

					f_soln<<MI[i-1]<<","<<MF[mg-1]<<","<<LOT[l-1]<<","<<logpoint<<","<<n_l_chips[l]<<","<<w_l[l]<<",N,"<<DEV[d-1]<<",";

					vector<Lambda_item>::iterator pL=Lambda.begin()+Lambda_id-1;

					int tf=(*pL).tooling_vec[0];

					if(tf>0){

						//Zhufeng: dt is the setup time of the machine

						//dt3 is the starting time of the lot. Feb 18, 2013

						double tool_setup_time = 0;

						if(id == 1){

							for(int i=1;i<= (*pL).tooling_vec.size();i++)

							{

								tool_setup_time+=Tooling_Setup_Time[(*pL).tooling_vec[i-1]];

							}

						}

						starting_time_lot += tool_setup_time;

						

						struct date_time dt3=increase_date_time(General_Snapshoot_time,starting_time_lot);

                        //end by Feb 18, 2013

						struct date_time dt=increase_date_time(General_Snapshoot_time,enhanced_solution[j-1].second);

						output_date_time(dt);

						//Zhufeng dt2 is the completion time of the lot

						struct date_time dt2=increase_date_time(General_Snapshoot_time,current_time);

						output_date_time(dt2);



						starting_time_lot = current_time;



						f_soln<<TF[tf-1]<<","<<(*pL).temperature<<","<<dt.str<<","<<dt3.str<<","<<dt2.str<<","<<pass<<",";

					}else{

						//Feb 18 ,2013

						struct date_time dt3=increase_date_time(General_Snapshoot_time,starting_time_lot);

						//End

						struct date_time dt=increase_date_time(General_Snapshoot_time,enhanced_solution[j-1].second);

						output_date_time(dt);

						struct date_time dt2=increase_date_time(General_Snapshoot_time,current_time);

						output_date_time(dt2);

						f_soln<<",,"<<(*pL).temperature<<","<<dt.str<<","<<dt3.str<<","<<dt2.str<<","<<pass<<",";

					}

					if((*pL).tooling_vec.size()>=2){

												//dt3 is the starting time of the lot. Feb 18, 2013



						for(int k=2;k<=(int)(*pL).tooling_vec.size();k++){//for each tooling family

							int tf=(*pL).tooling_vec[k-1];

							//Feb 18 ,2013

							struct date_time dt3=increase_date_time(General_Snapshoot_time,starting_time_lot);

							//End

							struct date_time dt=increase_date_time(General_Snapshoot_time,enhanced_solution[j-1].second);

                            output_date_time(dt);

                            struct date_time dt2=increase_date_time(General_Snapshoot_time,current_time);

						    output_date_time(dt2);

							f_soln<<",,,,,,,"<<TF[tf-1]<<","<<(*pL).temperature<<","<<dt.str<<","<<dt3.str<<","<<dt2.str<<","<<pass<<",";

						}

					}

					//Added by Zhufeng,Feb 2014, for the last lot beyond the horizon

					if(current_time<=H[i])

					{

						f_soln<<"Y"<<endl;

					}else

					{

						f_soln<<"N"<<endl;

					}



				}else{

					//Question:is it correct just comment this? Zhufeng, Jan 01, 2012

					//Comment by Zhufeng for multipass, Jan 01, 2012

					//f_soln<<"l="<<l<<",time_required="<<time_required<<",current_time="<<current_time<<",machine="<<i<<",H="<<H[i]<<endl;	

					//end by Zhufeng,Jan 01, 2012

					//break;

					cout<<"l="<<l<<",time_required="<<time_required<<",current_time="<<current_time<<",machine="<<i<<",H="<<H[i]<<endl;	

				}

			}//end_for

		}//end_if

	}

	//end Zhufeng



	//comment by Zhufeng

	//f_soln<<"Machine_Instance,Machine_Family_Name,Lot_Name,Quantity,Lot_Weight,Initial_Lot_Flag,Device_Name,Tooling_Family_Name,Certification,Setup_Time"<<endl;

	////first output the initial lots

	//for(int j=1;j<=(int)initial_running_lot.size();j++){

	//	if(initial_running_lot[j-1].time_remain>0){//Only the valid initial lot is output

	//		//It is okay that the lot will go across the whole horizon. It will still be output but not be counted in the total throughput

	//		int i=initial_running_lot[j-1].machine_id;

	//		int mg=machine_mg[i];

	//		//Note that there is neither tooling info nor certification info for the initial lots,so leave it blank

	//		f_soln<<initial_running_lot[j-1].machine_name<<","<<MF[mg-1]<<","<<initial_running_lot[j-1].lot_name<<","<<initial_running_lot[j-1].n_l_chips

	//			<<","<<initial_running_lot[j-1].w_l<<",Y,"<<initial_running_lot[j-1].dev_name<<",,,"<<General_Snapshoot_time.str<<endl;

	//	}

	//}

	////secondary,output the <enhanced_solution>

	//for(int j=1;j<=(int)enhanced_solution.size();j++){

	//	int i=enhanced_solution[j-1].first.i;

	//	int mg=machine_mg[i];

	//	int Lambda_id=enhanced_solution[j-1].first.Lambda_id;

	//	double current_time=enhanced_solution[j-1].second;

	//	if(current_time<H[i]){//if the starting time is within the planning horizon

	//		double time_start=enhanced_solution[j-1].second;

	//		for(int id=1;id<=(int)enhanced_solution[j-1].first.lots.int_vector.size();id++){

	//			int l=enhanced_solution[j-1].first.lots.int_vector[id-1];

	//			int d=lots_device[l];

	//			int s=max_rate_s(i,l,Lambda_id);

	//			double time_required=n_l_chips[l]/rate[s]+Load_Unload_Time;

	//			current_time+=time_required;

	//			if(current_time<=H[i]){

	//				f_soln<<MI[i-1]<<","<<MF[mg-1]<<","<<LOT[l-1]<<","<<n_l_chips[l]<<","<<w_l[l]<<",N,"<<DEV[d-1]<<",";

	//				vector<Lambda_item>::iterator pL=Lambda.begin()+Lambda_id-1;

	//				int tf=(*pL).tooling_vec[0];

	//				if(tf>0){

	//					struct date_time dt=increase_date_time(General_Snapshoot_time,enhanced_solution[j-1].second);

	//					output_date_time(dt);	

	//					f_soln<<TF[tf-1]<<","<<(*pL).temperature<<","<<dt.str<<endl;

	//				}else{

	//					struct date_time dt=increase_date_time(General_Snapshoot_time,enhanced_solution[j-1].second);

	//					output_date_time(dt);	

	//					f_soln<<",,"<<(*pL).temperature<<","<<dt.str<<endl;

	//				}

	//				if((*pL).tooling_vec.size()>=2){

	//					for(int k=2;k<=(int)(*pL).tooling_vec.size();k++){//for each tooling family

	//						int tf=(*pL).tooling_vec[k-1];

	//						struct date_time dt=increase_date_time(General_Snapshoot_time,enhanced_solution[j-1].second);

	//						f_soln<<",,,,,,,"<<TF[tf-1]<<","<<(*pL).temperature<<","<<dt.str<<endl;

	//					}

	//				}

	//			}else{

	//				f_soln<<"l="<<l<<",time_required="<<time_required<<",current_time="<<current_time<<",machine="<<i<<",H="<<H[i]<<endl;	

	//				//break;

	//			}

	//		}//end_for

	//	}//end_if

	//}

	//end Zhufeng

	f_soln.close();

	//--------------------------------------------------------------output the reports for key devices

	//---------------------------------------------------------------------get the production for the devices

	vector<int> production(Num_Device,0);

	for(int j=1;j<=(int)enhanced_solution.size();j++){//compute the total lot weights

		double current_time=enhanced_solution[j-1].second;

		int i=enhanced_solution[j-1].first.i;

		int Lambda_id=enhanced_solution[j-1].first.Lambda_id;

		if(current_time<H[i]){

			for(int id=1;id<=(int)enhanced_solution[j-1].first.lots.int_vector.size();id++){

				int l=enhanced_solution[j-1].first.lots.int_vector[id-1]; //lot



				//Added by Zhufeng for multipass, Jan 01, 2012

				int s;



				//Comment by Zhufeng, Feb 10, 2012. This is not used anymore. All is about the first pass

				//int pass=enhanced_solution[j-1].first.which_pass;

				/*if(pass==1)

				{*/

				//end Feb 10





				//end by Zhufeng, Jan 01 



					s=max_rate_s(i,l,Lambda_id);



				//Comment by Zhufeng, Feb 10, 2012. This is not used anymore. All is about the first pass



				////Added by Zhufeng for multipass, Jan 01, 2012



				//}else

				//{

				//	s=max_rate_s_second_pass(i,l,Lambda_id);

				//}

				//end by Zhufeng, Jan 01 



				//end by Zhufeng, Feb 10.







				double time_required=n_l_chips[l]/rate[s]+Load_Unload_Time;

				current_time+=time_required;

				if(current_time<=H[i]){//only count the lots that are finished within the horizon (ACTION 2)

					int d=lots_device[l];

					production[d-1]+=n_l_chips[l]; //update the device production

				}else{

					break; //the next lots also exceed the horizon

				}

			}//end_for

		}//end_if

	}

	//---------------------------------------Still need to add the initial lot to the production

	for(int id=1;id<=(int)initial_running_lot.size();id++){

		if(initial_running_lot[id-1].time_remain>0 && H[initial_running_lot[id-1].machine_id]>0){//NOTE That H[] has been updated in process_initial_lot()

			//This means that the initial lot can be finished within the horizon (it is possible that some LONG lot can not be done withn a horizon)

			//This also excludes the lots that are setup as initial lot but has already finished before Snapshoot_time (pretended to be an initial lot)

			int d=initial_running_lot[id-1].dev_id;

			production[d-1]+=initial_running_lot[id-1].n_l_chips;

        }

	}

	//------------------------------------------------------------------------------

	ofstream f_keydevice("Output/keydevice_production.csv");

	f_keydevice<<"Device,Target amount,Finished amount,Shortage"<<endl;

	for(int j=1;j<=(int)Key_Devices.size();j++){

		int d=Key_Devices[j-1];

		f_keydevice<<DEV[d-1]<<","<<n_k_minchips[d]<<","<<production[d-1]<<","<<n_k_minchips[d]-production[d-1]<<endl;

	}

	f_keydevice.close();

	//----------------------------------------------------compute the production of PKG and PIN_PKG

	vector<int> PKG_production((int)PKG.size(),0);

	vector<int> PIN_PKG_production((int)PKG.size()*(int)PIN_order.size()+1,0);

	for(int j=1;j<=(int)enhanced_solution.size();j++){//--------------get the production of KP and KPP

		double current_time=enhanced_solution[j-1].second; //the starting time

		int i=enhanced_solution[j-1].first.i;

		int Lambda_id=enhanced_solution[j-1].first.Lambda_id;

		vii p_lot=enhanced_solution[j-1].first.lots.int_vector.begin();

		if(current_time<H[i]){

			while(p_lot!=enhanced_solution[j-1].first.lots.int_vector.end()){

				int l=(*p_lot);



				int s=max_rate_s(i,l,Lambda_id);





				double time_required=n_l_chips[l]/rate[s]+Load_Unload_Time;

				current_time+=time_required;

				if(current_time<=H[i]){

					int d=lots_device[l];

					int pkg_id=device_PKG[d];

					int pin_num=device_PIN[d];

					vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);

					int pin_order=pfind-PIN_order.begin()+1;

					PKG_production[pkg_id-1]+=n_l_chips[l];

					int index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;

					PIN_PKG_production[index]+=n_l_chips[l];

				}else{

					break;

				}

				p_lot++;

			}//end_while

		}//end_if

	}

	//-----------------------------------------------------------------------------

	//--------------------------------------------------------------------------------

	ofstream f_kp("Output/key_package_production.csv");

	f_kp<<"Key_Package_Name,Target_amount,Finished_amount,Shortage"<<endl;	

	for(int kp=1;kp<=(int)Key_PKG.size();kp++){

		int pkg_id=Key_PKG[kp-1];

		int diff=n_kp_minchips[pkg_id]-PKG_production[pkg_id-1];

		f_kp<<PKG[pkg_id-1]<<","<<n_kp_minchips[pkg_id]<<","<<PKG_production[pkg_id-1]<<","<<diff<<endl;

	}

	f_kp.close();	

	//--------------------------------------------------------------------------

	ofstream f_kpp("Output/key_pin_package_production.csv");

	f_kpp<<"Package_Name,Pin,Target_amount,Finished_amount,Shortage"<<endl;	

	for(int kpp=1;kpp<=(int)Key_PIN_PKG.size();kpp++){

		int pkg_id=Key_PIN_PKG[kpp-1].first.second;

		int pin_num=Key_PIN_PKG[kpp-1].first.first;

		int target=Key_PIN_PKG[kpp-1].second;

		vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);

		int pin_order=pfind-PIN_order.begin()+1;

		int index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;

		int diff=target-PIN_PKG_production[index];

		f_kpp<<PKG[pkg_id-1]<<","<<pin_num<<","<<target<<","<<PIN_PKG_production[index]<<","<<diff<<endl;

	}

	f_kpp.close();

	//--------------------------------------------------------------------

	vector<pair<int,double> > mach_time; //this vector is used to store the makespan for each machine within a horizon

	for(int j=1;j<=(int)enhanced_solution.size();j++){

		int i=enhanced_solution[j-1].first.i;

		int Lambda_id=enhanced_solution[j-1].first.Lambda_id;



		//cout<<MI[i-1]<<".second="<<enhanced_solution[j-1].second<<"time_used"<<enhanced_solution[j-1].first.time_used<<endl;

		

		double time=enhanced_solution[j-1].second+enhanced_solution[j-1].first.time_used;//time is the total completion time



		//cout<<MI[i-1]<<",time="<<time<<endl;

		bool exist_flag=false;

		//It is likely that there are duplicate machines in the <enhanced_solution> b/c of the resetup

		//Thus need to check if the machine is already there in <mach_time>	

		for(int id=1;id<=(int)mach_time.size();id++){

			if(mach_time[id-1].first==i){

				exist_flag=true;

				if(time>mach_time[id-1].second && time<=H[i]){//the machine must be resetup and the completion time is within horizon

					mach_time[id-1].second=time;



				}else if(enhanced_solution[j-1].second<H[i] && time>H[i]){//some lots run across the horizon

					//the completion time will go across the time horizon if all the lots are processed

					//thus only pick up the lots that can be processed within the time horizon

					

					double completion_time=enhanced_solution[j-1].second;//the starting time



					//cout<<"completion_time="<<completion_time<<endl;



					//Added by Zhufeng, Feb 14, 2014

					bool already_beyond_horizon = false;

					//end by Zhufeng, Feb 14, 2014



					for(int k=1;k<=(int)enhanced_solution[j-1].first.lots.int_vector.size();k++){

						int l=enhanced_solution[j-1].first.lots.int_vector[k-1];

						int d=lots_device[l];



						int s=max_rate_s(i,l,Lambda_id);



						//modified by Zhufeng, Feb 10, 2012, add Load_Unload_time

						double time_required=n_l_chips[l]/rate[s]+Load_Unload_Time;



						//cout<<"time_required = "<<time_required<<endl;

						//end by Zhufeng, Feb 10, 2012



						//Comment by Zhufeng, Feb, 2014

						//if(completion_time+time_required<=H[i]){

						//end by Zhufeng, Feb 2014

						//Added by Zhufeng, Feb 2014

						if((completion_time+time_required<=H[i])||

							((completion_time+time_required>H[i])&&!already_beyond_horizon&&beyond_horizon ==1)

							){

							completion_time+=time_required;

							//Added by Zhufeng, Feb 2014

							if(completion_time > H[i])

							{

								already_beyond_horizon = true;

							}

							//end by Zhufeng, Feb 2014

						}else{

							break;

						}

					}



					//cout<<"now, completion_time="<<completion_time<<endl;

					//now we get the completion time

					mach_time[id-1].second=completion_time;





				}

				break;

			}

		}

		//Comment by Zhufeng, Feb 2014

		//if(!exist_flag && time<=H[i]){

		//end by Zhufeng, Feb 2014

		//Added by Zhufeng, Feb 2014

		if((!exist_flag && time<=H[i]) ||

			(!exist_flag && beyond_horizon ==1)

		){

	    //end by Zhufeng, Feb 2014

			mach_time.push_back(make_pair(i,time));

		}

	}

	//there still something missing here. We only consider the <enhanced_solution> so far.

	//there are some machine that is type 2. That is, not used in <enhanced_solution> but used to process initial lots.

	//these machine should also be counted here.

	for(int j=1;j<=(int)initial_running_lot.size();j++){

		int i=initial_running_lot[j-1].machine_id;

		bool found_flag=false;	

		for(int id=1;id<=(int)mach_time.size();id++){

			if(mach_time[id-1].first==i){//if we found a match of machine,then the machine is already used in <enhanced_solution>

				//however, still want to check it

				if(effective_starting_time[i]<=H[i]&& effective_starting_time[i]>mach_time[id-1].second){

					//the machine is used before finishing the process of initial lot

					//if effective_starting_time>H (for example 1E+06),then the machine is not used in <enhanced_solution>	

					cout<<"Eerror message: the machine "<<i<<" is used before finishing the process of initial lot"

					    <<". effective_starting_time="<<effective_starting_time[i]

					    <<", mach_time="<<mach_time[id-1].second<<endl;

				}	

				found_flag=true; break;

			}

		}

		if(!found_flag){

			//the machine is not used in <enhanced_solution> but used to process the initial lot

			mach_time.push_back(make_pair(i,effective_starting_time[i]));

			//it is possible effective_starting_time=1E+06 b/c the initial lot cannot be processed by the machine

		}

	}

	//it is still possible some machine is never used

	for(int j=1;j<=Num_Machine;j++){

		bool found_flag=false;

		for(int id=1;id<=(int)mach_time.size();id++){

			if(mach_time[id-1].first==j){

				found_flag=true; break;

			}

		}

		if(!found_flag){

			mach_time.push_back(make_pair(j,0));

		}

	}

	//at this point, all the machines should appear in <mach_time>	

	for(int j=1;j<=(int)mach_time.size();j++){

		//cout<<"j="<<j<<",mach="<<mach_time[j-1].first<<",time="<<mach_time[j-1].second<<endl;

	}

	ofstream f_time("Output/time.csv");

	f_time<<"Machine,Time"<<endl;

	for(int i=1;i<=Num_Machine;i++){

		int pos=-1;

		for(int j=1;j<=(int)mach_time.size();j++){

			if(mach_time[j-1].first==i){

				pos=j; break;

			}

		}

		if(pos>0){

			if(mach_time[pos-1].second<10000){	

				//it is possible some initial lot cannot be tested by the given machine and results in 1E+06 running time	

				f_time<<MI[i-1]<<","<<mach_time[pos-1].second<<endl;

			}else{

				f_time<<MI[i-1]<<","<<"N/A"<<endl;

			}	

		}else{

			cout<<"Warnning message: some machine is not include in the file time.csv"<<endl;

		}	

	}

	f_time.close();

	//------------------------------------------------------------------------

	ofstream f_longlot("Output/long_lot.csv");

	f_longlot<<"Lot Name,Device,Key Device,Key Package,Key Pin Package,Num of Device,Estimated Time Required"<<endl;

	for(int l=1;l<=Num_Lot;l++){//Num_Lot is the number of regular lots (not initial lot)

		int d=lots_device[l];	

		//------------------------------------------------get the highest processing rate for the lot	

		double max_rate=-1000;	

		vii pM=M_l[l].int_vector.begin();

		int count=0;	

		while(pM!=M_l[l].int_vector.end()){

			int i=(*pM);

			int sim=machine_sim[i];



			//Updated by Zhufeng for logpoint, Aug 31, 2011



			/*int index=index_S_sim_d(sim,d);	

			vii p_s=S_sim_d[index].second.int_vector.begin();

                        while(p_s!=S_sim_d[index].second.int_vector.end()){*/


			int index=index_S_sim_l(sim,l);	
			vii p_s=S_sim_l[index].second.int_vector.begin();
                        while(p_s!=S_sim_l[index].second.int_vector.end()){

            //end Zhufeng for logpoint




                                int s=(*p_s);

                               	if(rate[s]>max_rate){ max_rate=rate[s];} 

				p_s++;

                        }

			pM++;

			count++;	

		}

		//cout<<"l="<<l<<","<<LOT[l-1]<<",max_rate="<<max_rate<<endl;	

		//------------------------------------------------

		if(count<1){

			//This means count=0. There are two possible reasons.

			//(1) there is no available machine associated to the device in the lot.

			//(2) there is no available tooling associated to the device in the lot.

			int type=is_KD_PD(d);

                        bool is_kp=is_Key_PKG(d);

                        bool is_kpp=is_Key_PIN_PKG(d);

                        f_longlot<<LOT[l-1]<<","<<DEV[d-1]<<",";

                        if(type){ f_longlot<<"Y,";}else{ f_longlot<<"N,";}

                        if(is_kp){ f_longlot<<"Y,";}else{ f_longlot<<"N,";}

                        if(is_kpp){ f_longlot<<"Y,";}else{ f_longlot<<"N,";}

                        f_longlot<<n_l_chips[l]<<","<<"cannot be processed due to two possible reasons"

				<<"(1) lack of machine or tooling"

				<<"(2) the device is not contained in route.csv"<<endl;

		}	

		if(max_rate>1E-06){	

			double time_required=n_l_chips[l]/max_rate;

			//cout<<"l="<<l<<",time_required="<<time_required<<",max_rate="<<max_rate<<endl;	

			if(time_required>H[M_l[l].int_vector[0]]){//Assumption: all the machines have the same time horizon

				//this is a long lot

				int type=is_KD_PD(d);

       	   			bool is_kp=is_Key_PKG(d);

               		 	bool is_kpp=is_Key_PIN_PKG(d);

				f_longlot<<LOT[l-1]<<","<<DEV[d-1]<<",";

				if(type){ f_longlot<<"Y,";}else{ f_longlot<<"N,";}

				if(is_kp){ f_longlot<<"Y,";}else{ f_longlot<<"N,";}

				if(is_kpp){ f_longlot<<"Y,";}else{ f_longlot<<"N,";}

				f_longlot<<n_l_chips[l]<<","<<time_required<<endl;	

			}	

		}	

	}

	for(int id=1;id<=(int)initial_running_lot.size();id++){

		int d=initial_running_lot[id-1].dev_id;

		int type=is_KD_PD(d);

                bool is_kp=is_Key_PKG(d);

                bool is_kpp=is_Key_PIN_PKG(d);

		if(initial_running_lot[id-1].rate>1E-06){

			double time_required=initial_running_lot[id-1].n_l_chips/initial_running_lot[id-1].rate;

			if(time_required>H[initial_running_lot[id-1].machine_id]){	

				f_longlot<<initial_running_lot[id-1].lot_name<<","<<initial_running_lot[id-1].dev_name<<",";

       			        if(type){ f_longlot<<"Y,";}else{ f_longlot<<"N,";}

                		if(is_kp){ f_longlot<<"Y,";}else{ f_longlot<<"N,";}

                		if(is_kpp){ f_longlot<<"Y,";}else{ f_longlot<<"N,";}

				f_longlot<<initial_running_lot[id-1].n_l_chips<<","<<time_required<<endl;

			}	

		}else{

			f_longlot<<initial_running_lot[id-1].lot_name<<","<<initial_running_lot[id-1].dev_name<<",";

                        if(type){ f_longlot<<"Y,";}else{ f_longlot<<"N,";}

                        if(is_kp){ f_longlot<<"Y,";}else{ f_longlot<<"N,";}

                        if(is_kpp){ f_longlot<<"Y,";}else{ f_longlot<<"N,";}

			f_longlot<<initial_running_lot[id-1].n_l_chips<<","<<"cannot be processed due to two possible reasons"

                                <<"(1) lack of machine or tooling"

                                <<"(2) the device is not contained in route.csv"<<endl;

		}	

	}

	f_longlot.close();

}



void AT::str_to_date_time(char * ch,struct date_time & dt){



	//cout<<"in str_to_date_time()"<<endl;	

	strcpy(dt.str,ch); //copy the string

	char pdes[50];

	int k=0,l=0;

	trim(dt.str);

	k=strtok2(pdes,dt.str,l,'/');//get MM

	trim(pdes);

	//cout<<"pdes="<<pdes<<endl;



	dt.MM=atoi(pdes);

	l=k;

	k=strtok2(pdes,dt.str,l,'/');//get DD

	trim(pdes);

	//cout<<"pdes="<<pdes<<endl;

	dt.DD=atoi(pdes);

	l=k;

	k=strtok2(pdes,dt.str,l,' ','/');//get YY

	trim(pdes);

	//cout<<"pdes="<<pdes<<endl;

	dt.YY=atoi(pdes);

	l=k;

	

	k=strtok2(pdes,dt.str,l,':');//get hh

	trim(pdes);

	//cout<<"pdes="<<pdes<<endl;

	dt.hh=atoi(pdes);

	l=k;

	k=strtok2(pdes,dt.str,l,':');//get mm

	trim(pdes);

	dt.mm=atoi(pdes);

	l=k;

	k=strtok2(pdes,dt.str,l,' ');//get ss

	

	trim(pdes);

	

	dt.ss=atoi(pdes);

	

}



double AT::get_date_time_difference(const struct date_time & a,const struct date_time & b){

	//return a-b in hours

	//cout<<"a:MM="<<a.MM<<",DD="<<a.DD<<",YY="<<a.YY<<",hh="<<a.hh<<",mm="<<a.mm<<",ss="<<a.ss<<endl;

	//cout<<"b:MM="<<b.MM<<",DD="<<b.DD<<",YY="<<b.YY<<",hh="<<b.hh<<",mm="<<b.mm<<",ss="<<b.ss<<endl;

	int dMM=a.MM-b.MM;

	int dDD=a.DD-b.DD;

	int dYY=a.YY-b.YY;

	int dhh=a.hh-b.hh;

	int dmm=a.mm-b.mm;

	int dss=a.ss-b.ss;

	double diff=dYY*365*24+dDD*24+dMM*30*24+dhh+dmm/60.0+dss/3600.0;

	return diff;

}



struct date_time AT::increase_date_time(const struct date_time & org_dt,double increment_hr){

	//update the dt by increse "increment_hr" hours

	struct date_time dt=org_dt;

	int ss=increment_hr*3600; //transform to seconds

	ss+=dt.ss;

	int mm=(int)ss/60.0;

	dt.ss=ss-mm*60;

	mm+=dt.mm;

	int hh=(int)mm/60.0;

	dt.mm=mm-hh*60;

	hh+=dt.hh;

	int DD=(int)hh/24.0;

	dt.hh=hh-DD*24;

	DD+=dt.DD;

	int num_date[12]={31,28,31,30,31,30,31,31,30,31,30,31};	

	int MM=dt.MM;	

	if(DD>num_date[MM]){

		DD=DD-num_date[MM];

		MM++;	

	}	

	dt.DD=DD;

	int YY=(int)MM/12.0;

	dt.MM=MM-YY*12;

	YY+=dt.YY;

	dt.YY=YY;

	stringstream ssss;

	ssss<<dt.MM<<"/"<<dt.DD<<"/"<<dt.YY<<" "<<dt.hh<<":"<<dt.mm<<":"<<dt.ss;

	strcpy(dt.str,(ssss.str()).c_str());

	return dt;

}



void AT::output_date_time(struct date_time & dt){

	stringstream ss;

	if(dt.MM<=9){ ss<<"0";}

	ss<<dt.MM<<"/";

	if(dt.DD<=9){ ss<<"0";}

	ss<<dt.DD<<"/";

	if(dt.YY<=9){ ss<<"0";}

	ss<<dt.YY<<" ";

	if(dt.hh<=9){ ss<<"0";}

	ss<<dt.hh<<":";

	if(dt.mm<=9){ ss<<"0";}

	ss<<dt.mm<<":";

	if(dt.ss<=9){ ss<<"0";}

	ss<<dt.ss;

	strcpy(dt.str,(ss.str()).c_str());

}



void AT::initialize_tooling_trace(struct soln_pool_item & best_soln,vector<tooling_trace_item> & tooling_trace){

	//this function initialize the vector <tooling_trace> according to the GRASP best solution

	//initially, the vector <tooling_trace> is empty

	tooling_trace.resize((int)Tooling_vector.size());

	for(int j=1;j<=(int)tooling_trace.size();j++){ tooling_trace[j-1].id=j;}

	

	//Now we are going to investigate the tooling usage in the GRASP solution

	for(int j=1;j<=(int)best_soln.sub_soln.size();j++){

		int i=best_soln.sub_soln[j-1].i;

		int Lambda_id=best_soln.sub_soln[j-1].Lambda_id;

		double time_used=best_soln.sub_soln[j-1].time_used;

		double start_time=effective_starting_time[i];

		double end_time  =start_time+time_used;

	

		if(Lambda_id>=1 && time_used>0){

			//only consider the machines that are actually running	

			vector<Lambda_item>::iterator pL=Lambda.begin()+Lambda_id-1;			

			int temperature=(*pL).temperature;

                        for(int id=1;id<=(int)(*pL).tooling_vec.size();id++){

                                int tf=(*pL).tooling_vec[id-1];

                                int b =(*pL).b[id-1];

       				int count=0;	

				for(int k=1;k<=(int)tooling_trace.size();k++){

					//now pick the b tooling pieces in the family tf. Note that the tooling pieces should be able to work under the temperature

                       			int tooling_id=tooling_trace[k-1].id; //this is the id for the tooling piece

					if(tooling_tf[tooling_id]==tf){

						vector<Tooling>::iterator pT=Tooling_vector.begin()+tooling_id-1; //point to the Tooling item

						vii pfind=find((*pT).temperature_vector.begin(),(*pT).temperature_vector.end(),temperature); //look for the temperature

						if(pfind!=(*pT).temperature_vector.end()){

							bool flag1=false,flag2=false;

							if((int)tooling_trace[k-1].schedule.size()<1){ flag1=true;} //there is no assignment for the tooling piece

							else{

								int size=(int)tooling_trace[k-1].schedule.size();	

								double available_time=tooling_trace[k-1].schedule[size-1].second.second; //this is the earliest time the tooling is available

								if(start_time>available_time){ flag2=true;}

							}

							//--now we check if the current tooling piece is assignable

							if(flag1 || flag2){

								tooling_trace[k-1].schedule.push_back(make_pair(i,make_pair(start_time,end_time)));

								count++;

							}

						}	

					}

					if(count>=b){ break;}	

				}  

			}

		}

	}

}



void AT::show_tooling_trace(vector<tooling_trace_item> & tooling_trace){

	ofstream fdebug("Debug/debug_tooling_trace.txt");	

	for(int k=1;k<=(int)tooling_trace.size();k++){

		vector<Tooling>::iterator pT=Tooling_vector.begin()+tooling_trace[k-1].id-1;	

		fdebug<<"id="<<tooling_trace[k-1].id

			<<",name="<<(*pT).name

			<<",tf="<<(*pT).tf

			<<",family="<<TF[(*pT).tf-1]	

			<<",schedule=";

		for(int id=1;id<=(int)tooling_trace[k-1].schedule.size();id++){

			double start_time=tooling_trace[k-1].schedule[id-1].second.first;

			double end_time  =tooling_trace[k-1].schedule[id-1].second.second;

			double machine_id=tooling_trace[k-1].schedule[id-1].first;

	

			fdebug<<"("<<start_time<<","<<end_time<<")";

			fdebug<<"to machine "<<machine_id<<",name="<<MI[machine_id-1];	

		}

		fdebug<<endl;

	}	

	fdebug.close();

	fdebug.clear();

}







//Comment by Zhufeng, Feb 26, 2012 not used any more

////Added by Zhufeng

//void AT::initialize_Lambda_i2()

//{

//	

//	/*cout<<"Clear Lambda_i[]"<<endl;

//	for(int i=1;i<=Num_Machine;i++)

//	{

//	  Lambda_i[i].int_vector.clear();

//	

//	if(Lambda_i[i].int_vector.empty())

//	{

//		cout<<"It's already empty."<<endl;

//	}

//	}*/

//	

//

//	//Comment by Zhufeng, for not overtake initial setups and flexible initial setups, Feb 26, 2012

//

//	//for(int i=1;i<=Num_Machine;i++){

//	//	if (Machine_vector[i-1].hasInitialSetup)

//	//	{

//	//		cout<<"Reinitialize Lambda_i[] now. This machine is an initial machine and it needn't keep its initial setup when considering resetup."<<endl;

//	//	    

//	//		vector<tooling_configure> TS;

//	//		get_S(TS,i);

//	//		vector<tooling_configure>::iterator p_TS=TS.begin();

//	//		while(p_TS!=TS.end()){

//	//			//----------------------------

//	//			vector<int> a;

//	//			int s=(*p_TS).id;

//	//			vii pt=(*p_TS).tf.tools.begin();

//	//			while(pt!=(*p_TS).tf.tools.end()){

//	//				int tf=(*pt);

//	//				int pos=s*Max_Num_Tooling_Family+tf;

//	//				//a.push_back(a_st[s][tf]);

//	//				a.push_back(a_st[pos]);

//	//				pt++;

//	//			}

//	//			//----------------------------

//	//			int id=get_Lambda_id((*p_TS).tf.tools,a,(*p_TS).temperature);

//	//			vii p_find=find(Lambda_i[i].int_vector.begin(),Lambda_i[i].int_vector.end(),id);

//	//			if(p_find==Lambda_i[i].int_vector.end()){

//	//				Lambda_i[i].int_vector.push_back(id);

//	//			}

//	//			p_TS++;

//	//		}

//	//		vector<int> v((int)Lambda_i[i].int_vector.size(),0);

//	//		vii p_int=Lambda_i[i].int_vector.begin();

//	//		copy(p_int,p_int+(int)Lambda_i[i].int_vector.size(),v.begin());

//	//		sort(v.begin()+1,v.end());

//	//		swap(v,Lambda_i[i].int_vector);

//	//	}//end of if (Machine_vector[i-1].hasInitialSetup)

//	//	

//	//}

//	//cout<<"The reinitializatio of Lambda_i[] has been complete now."<<endl;

//

//	//end by Zhufeng, for not overtake initial setups and flexible initial setups, Feb 26, 2012 

//

//	for(int i=1;i<=Num_Machine;i++){

//		

//			vector<tooling_configure> TS;

//			get_S(TS,i);

//			vector<tooling_configure>::iterator p_TS=TS.begin();

//			while(p_TS!=TS.end()){

//				//----------------------------

//				vector<int> a;

//				int s=(*p_TS).id;

//				vii pt=(*p_TS).tf.tools.begin();

//				while(pt!=(*p_TS).tf.tools.end()){

//					int tf=(*pt);

//					int pos=s*Max_Num_Tooling_Family+tf;

//					//a.push_back(a_st[s][tf]);

//					a.push_back(a_st[pos]);

//					pt++;

//				}

//				//----------------------------

//				int id=get_Lambda_id((*p_TS).tf.tools,a,(*p_TS).temperature);

//				vii p_find=find(Lambda_i2[i].int_vector.begin(),Lambda_i2[i].int_vector.end(),id);

//				if(p_find==Lambda_i2[i].int_vector.end()){

//					Lambda_i2[i].int_vector.push_back(id);

//				}

//				p_TS++;

//			}

//			vector<int> v((int)Lambda_i2[i].int_vector.size(),0);

//			vii p_int=Lambda_i2[i].int_vector.begin();

//			copy(p_int,p_int+(int)Lambda_i2[i].int_vector.size(),v.begin());

//			sort(v.begin(),v.end());

//			swap(v,Lambda_i2[i].int_vector);

//		

//		//comment by ZHufeng

//		////-------- added by yufen ------------

//		//}

//		////--------- end yufen --------------------

//        //end Zhufeng

//		//cout<<i<<"	machine="<<Machine_vector[i-1].name<<"	hasInitialSetup?="<<Machine_vector[i-1].hasInitialSetup<<"	numLambdas="<<Lambda_i[i].int_vector.size()<<"	1stLambda="<<*(Lambda_i[i].int_vector.begin())<<endl;

//	}

//	//cout<<" ====================================  end showing lambda ====================================== "<<endl;

//	

//}//end of void AT::initialize_Lambda_i2()

//void AT::show_Lambda_i2()

//{

////output the data structure Lambda_i to a debug file

//	ofstream fout("Debug/debug_Lambda_i2.txt");

//	fout<<"show Lambda_i ---------------------------------"<<endl;

//	for(int i=1;i<=Num_Machine;i++){

//		fout<<"i	"<<i<<"	"<<Machine_vector[i-1].name<<"	";

//		vii p_int=Lambda_i2[i].int_vector.begin();

//		while(p_int!=Lambda_i2[i].int_vector.end()){

//			fout<<(*p_int)<<",";

//			p_int++;

//		}

//		fout<<endl;

//	}

//	fout.close();

//}//end of void AT::show_Lambda_i2()

////End Zhufeng

//End Zhufeng, Feb 26, 2012



//Added by Zhufeng for multipass,Heuristics 3, Jan 20, 2012

void AT::initialize_enhanced_solution(struct soln_pool_item & best_soln)

{

	for(int j=1;j<=Num_Machine;j++){

		if(effective_starting_time[j]<=10000){//if the initial lot can be processed by the machine

			H[j]+=effective_starting_time[j];

		}

		else{

			H[j]=0.0;

		}	

	}	

	cout<<"---------------------------------------------------------- Enhancement to resetup the machines when they are idle"<<endl;

	double eps=1E-06;



	vector<int> mc_no_GRASP_initial; //this vector stores the machines that are not used in GRASP solution but used to process initial lots

	struct soln_pool_item GRASP_soln=best_soln; //don't want to modify the original best solution

	for(int j=1;j<=(int)GRASP_soln.sub_soln.size();j++){

		if(GRASP_soln.sub_soln[j-1].Lambda_id>0){

			pair<sub_soln_item,double> one_pair;//the second double component stores the info of starting time

			one_pair.first=GRASP_soln.sub_soln[j-1];

			int i=one_pair.first.i;

			if(effective_starting_time[i]<10000){//if the initial lot can be processed by the machine	

				//cout<<MI[i-1]<<",one_pair.second="<<effective_starting_time[i]<<endl;

				one_pair.second=effective_starting_time[i];//this is the starting time of the solution except the initial lots

			}else{

				//cout<<MI[i-1]<<",one_pair.second="<<H[i]<<endl;

				one_pair.second=H[i];	

			}



			//Added by Zhufeng for multipass, Dec 31, 2011

			one_pair.first.which_pass=1;

			vii p_l=one_pair.first.lots.int_vector.begin();

		    while(p_l!=one_pair.first.lots.int_vector.end())

			{

				int lot_id=(* p_l);

				int logpoint=lot_logpoint[lot_id];;

				one_pair.first.logpoints.int_vector.push_back(logpoint);

				p_l++;

			}

			//end by Zhufeng  jan 1, 2012





			enhanced_solution.push_back(one_pair);

		}else{//for the machines that not used in the GRASP solution

			int i=GRASP_soln.sub_soln[j-1].i;

			for(int id=1;id<=(int)initial_running_lot.size();id++){

				if(i==initial_running_lot[id-1].machine_id){//if the machine is used to process initial lots 

					mc_no_GRASP_initial.push_back(i);

					break;

				}	

			}	

		}

	}



}

//end by Zhufeng, Jan 20, 2012



//Comment by Zhufeng, Feb, 2014

//void AT::update_benefit_second_pass_setup(int current_machine,int current_Lambda,double current_time,vector< pair<double,vec_int> > & machine_lambda_benefit,vector<int> & lot_remain,vector<int> & lot_remain_second_pass_logpoint,vector<int> & production,vector<int> & affected_dev,int * tooling_used,int * sim_used,int * SIM_number,vector<int> & idle_machines){

//	//instead of update the benefit according to sim-lambda combination, it is necessary to use machine-lambda combination because at this point

//	//machines in the sim are not identical anymore due the different H[]	

//	//cout<<"in update_benefit_resetup()"<<endl;

//	//the values in <machine_lambda_benefit> are never used in the right hand side of =. Thus the <machine_lambda_benefit> will not affect the values of other variables. That is, it is fine to use a <machine_lambda_benefit> with all 0 benefits (This statement is for AT enhancement.)

//	int index=-1;

//	//------------copy production for the devices

//	vector<int> prod((int)production.size(),0);

//	copy(production.begin(),production.end(),prod.begin());

//	//--------------------------------------------------------compute the production for package and pin package

//

//	//Question:how to compute production for devices, package, and pin package for second pass lot? Asked by Zhufeng, Dec 30, 2011

//	//

//	vector<int> PKG_production_save((int)PKG.size(),0);

//	vector<int> PIN_PKG_production_save((int)PKG.size()*(int)PIN_order.size()+1,0);

//	for(int d=1;d<=Num_Device;d++){

//		int pkg_id=device_PKG[d];

//		int pin_num=device_PIN[d];

//		vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);

//		int pin_order=pfind-PIN_order.begin()+1;

//		int index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;

//		PKG_production_save[pkg_id-1]+=prod[d-1];

//		PIN_PKG_production_save[index]+=prod[d-1];

//	}

//	//---------------------------------------------------------------------------------------------------------for LL

//	//This segment of code sorts the remaining lots according to their benefit

//	vector< pair<int,double> > LL;

//	for(int id=1;id<=(int)lot_remain.size();id++){

//		int l=lot_remain[id-1];

//		int d=lots_device[l];

//		int type=is_KD_PD(d);

//		int pkg_id=device_PKG[d];

//		int pin_num=device_PIN[d];

//		bool is_kp=is_Key_PKG(d);

//		bool is_kpp=is_Key_PIN_PKG(d);

//		pair<int,double> one_pair;

//		one_pair.first=l;

//		one_pair.second=w_l[l];

//		if(type==1 && prod[d-1]<n_k_minchips[d]){

//			int min=n_l_chips[l];

//			if(min>n_k_minchips[d]-prod[d-1]){ min=n_k_minchips[d]-prod[d-1];}

//			one_pair.second+=penalty[d]/coef*min;

//		}else if(type==2 && prod[d-1]<n_p_minchips[d]){

//			int min=n_l_chips[l];

//			if(min>n_p_minchips[d]-prod[d-1]){ min=n_p_minchips[d]-prod[d-1];}

//			one_pair.second+=penalty[d]/coef*min;

//		}

//		if(is_kp && PKG_production_save[pkg_id-1]<n_kp_minchips[pkg_id]){

//			int diff=n_kp_minchips[pkg_id]-PKG_production_save[pkg_id-1];

//			int min=n_l_chips[l];

//			if(min>diff){ min=diff;}

//			one_pair.second+=PKG_penalty[pkg_id]/coef*min;

//		}

//		int target=0;

//		for(int id2=1;id2<=(int)Key_PIN_PKG.size();id2++){

//			if(Key_PIN_PKG[id2-1].first.first==pin_num && Key_PIN_PKG[id2-1].first.second==pkg_id){

//				target=Key_PIN_PKG[id2-1].second;

//				break;

//			}

//		}

//		vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);

//		int pin_order=pfind-PIN_order.begin()+1;

//		int index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;

//		if(is_kpp && PIN_PKG_production_save[index]<target){

//			int diff=target-PIN_PKG_production_save[index];

//			int min=n_l_chips[l];

//			if(min>diff){ min=diff;}

//			one_pair.second+=get_PIN_PKG_penalty(pkg_id,pin_num)/coef*min;

//		}

//		LL.push_back(one_pair);

//	}

//	sort(LL.begin(),LL.end(),compare_lot); //the remaining lots are sorted according to the benefits

//	//------------------------------------------------------------------------------------------------

//	//The following segment of code iterats over all machine-lambda combinations. For each iteration,lots are picked from the sorted LL and assigned to the

//	//selected machine-lambda combination. Once a lot is selcted and assigned,the benefit of the remaining lots need to be updated accordingly.Thus an updating

//	//procedure need to be applied each time a lot is assigned. This reupdate costs a lot of computational time. 

//	//In the case of resetup machines,change over setup time will be caused for each resetup, except the situation that the machine-lambda under investigation

//	//is the current machine-lambda combination. That is, it is not necessary to change the tooling setup for the current machine.

//	//Once change over time is incured,the available machine time H[] is reduced by the same amount of time.

//	int count=0;

//	int count_limit=1E+6;

//	index=-1;

//	for(int machine_id=1;machine_id<=(int)Machine_vector.size();machine_id++){

//		//test if the machine_id is idle (available or not)	

//		vii pfind=find(idle_machines.begin(),idle_machines.end(),machine_id);

//		bool machine_idle_flag=true;	

//		if(pfind==idle_machines.end()){//machine_id is NOT idle 

//			machine_idle_flag=false;	

//		}	

//		int sim=machine_sim[machine_id];

//		vector<int>::iterator p_Lambda=Lambda_i[machine_id].int_vector.begin();

//		while(p_Lambda!=Lambda_i[machine_id].int_vector.end()){

//			//the inner loop for each machine-lambda combination

//			int Lambda_id=(*p_Lambda);

//			//---------------------------------------------------compute the resetup time

//			double resetup_time=0;

//			if(machine_id!=current_machine || Lambda_id!=current_Lambda){//unmatched machine or unmatch Lambda_id

//				vector<Lambda_item>::iterator pL=Lambda.begin()+Lambda_id-1;

//				for(int j=1;j<=(int)(*pL).tooling_vec.size();j++){

//					int tf=(*pL).tooling_vec[j-1];

//					resetup_time+=Tooling_Setup_Time[tf];

//				}

//			}

//			index++;

//			count++;

//			//--------------------------------------------------------------------------------------------

//			if(machine_lambda_benefit[index].first<0){//previosly infeasible or non-benefitial setup

//				p_Lambda++; continue;

//			}

//			//-----------------------------------------------------------------------------------------------test if machine_lambda is feasible

//			int test=is_sim_lambda_feasible(sim,Lambda_id,tooling_used,sim_used,SIM_number);

//			if(!test || !machine_idle_flag){//if the machine-lambda is infeasible

//				machine_lambda_benefit[index].first=-1E+6;

//				machine_lambda_benefit[index].second.int_vector.clear();

//				p_Lambda++; 

//				continue;

//			}

//			//-------------------------------------------------------------------------------if the current_time plus setup time exceed the horizon, then it is not interesed to resetup

//			if(current_time+resetup_time>H[machine_id]){

//				//cout<<"current_time="<<current_time<<",resetup_time="<<resetup_time<<",H="<<H[machine_id]<<endl;

//				p_Lambda++;continue;

//			}

//			//--------------------------------------------------------------------------

//			//upto this point,the machine-lambda is feasible and should be investigated

//			//---------------------------------------------------------------make a copy of the production such that it will not be destroyed

//			copy(production.begin(),production.end(),prod.begin());

//			vector<int> PKG_production((int)PKG.size(),0);

//			vector<int> PIN_PKG_production((int)PKG.size()*(int)PIN_order.size()+1,0);

//			copy(PKG_production_save.begin(),PKG_production_save.end(),PKG_production.begin());

//			copy(PIN_PKG_production_save.begin(),PIN_PKG_production_save.end(),PIN_PKG_production.begin());

//			//---------------------------------------------------------------------------------------------------------check if affected dev

//			/*

//			bool affected=false;

//			for(int id=1;id<=(int)machine_lambda_benefit[index].second.int_vector.size();id++){

//				int l=machine_lambda_benefit[index].second.int_vector[id-1];

//				int d=lots_device[l];

//				vii pfind=find(affected_dev.begin(),affected_dev.end(),d);

//				if(pfind!=affected_dev.end()){

//					affected=true;

//					break;

//				}

//			}

//			if(!affected){ p_Lambda++; continue;}

//			*/

//			//--------------------------------------------------------if affected, clear

//			machine_lambda_benefit[index].second.int_vector.clear();

//			machine_lambda_benefit[index].first=0;

//			//---------------------------------------------------------------------------------------------------------for LLL

//			vector< pair<int,double> > LLL;

//			for(int id=1;id<=(int)LL.size();id++){

//				int l=LL[id-1].first;

//

//				//commet for multipass, Zhufeng. Dec 31, 2011

//				//int pos=index_L_sim_lambda(sim,Lambda_id);	

//				//vii pfind=find(L_sim_lambda[pos].second.int_vector.begin(),L_sim_lambda[pos].second.int_vector.end(),l); 

//

//				//Add for multipass,Zhufeng, Dec 31, 2011 

//				int pos=index_L_second_pass_sim_lambda(sim,Lambda_id);	

//				vii pfind=find(L_second_pass_sim_lambda[pos].second.int_vector.begin(),L_second_pass_sim_lambda[pos].second.int_vector.end(),l); 

//				//end by Zhufeng, Dec 31, 2011

//

//				if(pfind!=L_second_pass_sim_lambda[pos].second.int_vector.end()){

//					//NOte:max_rate_s()need to be modified for second pass. 

//					int s=max_rate_s_second_pass(machine_id,l,Lambda_id);

//

//					double time_required=n_l_chips[l]/rate[s]+Load_Unload_Time;

//					double ben=LL[id-1].second/time_required;

//					LLL.push_back(make_pair(l,ben));

//				}

//			}

//			sort(LLL.begin(),LLL.end(),compare_lot);

//			//---------------------------------------------------------------------------------------------------------for LLL

//			double sum=0;

//			double time_used=current_time+resetup_time; //if the resetup costs time, it should be taken into accout in the time used

//			//cout<<"sim="<<sim<<",machine_id="<<machine_id<<",Lambda_id="<<Lambda_id<<",time_used="<<time_used<<",H="<<H[machine_id]<<endl;	

//			for(int id=1;id<=(int)LLL.size();id++){

//				int l=LLL[id-1].first;

//				int d=lots_device[l];

//

//				//int s=max_rate_s(machine_id,l,Lambda_id);

//

//				int s=max_rate_s_second_pass(machine_id,l,Lambda_id);

//

//				double time_required=n_l_chips[l]/rate[s]+Load_Unload_Time;

//				//cout<<"l="<<l<<",time_required="<<time_required<<endl;	

//				if(time_used+time_required<=H[machine_id]){

//					time_used+=time_required;

//					//cout<<"true,time_used="<<time_used<<endl;	

//					sum+=LLL[id-1].second*time_required;

//					machine_lambda_benefit[index].second.int_vector.push_back(l);

//					prod[d-1]+=n_l_chips[l];

//					int pkg_id=device_PKG[d];

//					int pin_num=device_PIN[d];

//					PKG_production[pkg_id-1]+=n_l_chips[l];

//					vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);

//					int pin_order=pfind-PIN_order.begin()+1;

//					int pin_pkg_index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;

//					PIN_PKG_production[pin_pkg_index]+=n_l_chips[l];

//					int target=0;

//					for(int id2=1;id2<=(int)Key_PIN_PKG.size();id2++){

//						if(Key_PIN_PKG[id2-1].first.first==pin_num && Key_PIN_PKG[id2-1].first.second==pkg_id){

//							target=Key_PIN_PKG[id2-1].second;

//							break;

//						}

//					}

//					//------------------------------------------------update and sort, since the first lot is selected, the rest of the lots need to be sorted again!!!

//					int type=is_KD_PD(d);

//					bool is_kp=is_Key_PKG(d);

//					bool is_kpp=is_Key_PIN_PKG(d);

//					/*

//					if(type==1){

//						bool flag=false;

//						int shortage=n_k_minchips[d]-prod[d-1];

//						for(int id2=id+1;id2<=(int)LLL.size();id2++){

//							int l2=LLL[id2-1].first;

//							int d2=lots_device[l2];

//							if(d2==d && shortage<n_l_chips[l2]){

//								int s2=max_rate_s(machine_id,l2,Lambda_id);

//								double time2=n_l_chips[l2]/rate[s2]+Load_Unload_Time;

//								if(shortage>0){

//									LLL[id2-1].second=(w_l[l2]+penalty[d2]/coef*shortage)/time2;

//									flag=true;

//								}else if(shortage<0 && shortage+n_l_chips[l]>0){

//									LLL[id2-1].second=w_l[l2]/time2;

//									flag=true;

//								}

//							}

//						}//end_for

//						if(flag){

//							sort(LLL.begin()+id,LLL.end(),compare_lot);

//						}

//					}else if(type==2){

//						bool flag=false;

//						int shortage=n_p_minchips[d]-prod[d-1];

//						for(int id2=id+1;id2<=(int)LLL.size();id2++){

//							int l2=LLL[id2-1].first;

//							int d2=lots_device[l2];

//							if(d2==d && shortage<n_l_chips[l2]){

//								int s2=max_rate_s(machine_id,l2,Lambda_id);

//								double time2=n_l_chips[l2]/rate[s2]+Load_Unload_Time;

//								if(shortage>0){

//									LLL[id2-1].second=(w_l[l2]+penalty[d2]/coef*shortage)/time2;

//									flag=true;

//								}else if(shortage<0 && shortage+n_l_chips[l]>0){

//									LLL[id2-1].second=w_l[l2]/time2;

//									flag=true;

//								}

//							}

//						}//end_for

//						if(flag){

//							sort(LLL.begin()+id,LLL.end(),compare_lot);

//						}

//					}//end_if

//					*/

//					if(type==1||is_kp||is_kpp){

//						bool flag=false;

//						for(int id2=id+1;id2<=(int)LLL.size();id2++){

//							int l2=LLL[id2-1].first;

//							int d2=lots_device[l2];

//

//							//Comment by Zhufeng, Dec 31, 2011

//							//int s2=max_rate_s(machine_id,l2,Lambda_id); 

//							//end by Zhufeng, Dec 31, 2011

//							//Added by Zhufeng for multipass, Dec 31, 2011

//							//cout<<"s2=max_rate_s_second_pass"<<endl;

//							int s2=max_rate_s_second_pass(machine_id,l2,Lambda_id);

//							//end by Zhufeng, Dec 31, 2011

//

//

//							double time2=n_l_chips[l2]/rate[s2]+Load_Unload_Time;

//							LLL[id2-1].second=w_l[l2]/time2;

//							if(type==1&&d2==d){

//								if(production[d-1]<n_k_minchips[d]){

//									int diff=n_k_minchips[d]-production[d-1];

//									if(diff<n_l_chips[l2]){

//										LLL[id2-1].second+=(penalty[d2]/coef*diff)/time2;

//									}else{

//										LLL[id2-1].second+=(penalty[d2]/coef*n_l_chips[l2])/time2;

//									}

//									flag=true;

//								}

//							}

//							if(is_kp && pkg_id==device_PKG[d2]){

//								if(PKG_production[pkg_id-1]<n_kp_minchips[pkg_id]){

//									int diff=n_kp_minchips[pkg_id]-PKG_production[pkg_id-1];

//									if(diff<n_l_chips[l2]){

//										LLL[id2-1].second+=(PKG_penalty[pkg_id]/coef*diff)/time2;

//									}else{

//										LLL[id2-1].second+=(PKG_penalty[pkg_id]/coef*n_l_chips[l2])/time2;

//									}

//									flag=true;

//								}

//							}

//							if(is_kpp && pkg_id==device_PKG[d2] && pin_num==device_PIN[d2]){

//								if(PIN_PKG_production[pin_pkg_index]<target){

//									int diff=target-PIN_PKG_production[pin_pkg_index];

//									if(diff<n_l_chips[l2]){

//										LLL[id2-1].second+=(get_PIN_PKG_penalty(pkg_id,pin_num)/coef*diff)/time2;

//									}else{

//										LLL[id2-1].second+=(get_PIN_PKG_penalty(pkg_id,pin_num)/coef*n_l_chips[l2])/time2;

//									}

//									flag=true;

//								}

//							}

//						}

//						if(flag){

//							sort(LLL.begin()+id,LLL.end(),compare_lot);

//						}		

//					//------------------------------------------------------------------------------------

//					}//end_if

//				}

//			}//end_for

//			machine_lambda_benefit[index].first=sum;

//			p_Lambda++;

//			if(count>count_limit){ break;}

//		}

//		if(count>count_limit){ break;}

//	}

//	//cout<<"count="<<count<<endl;

//	//cout<<"end of update_benefit()"<<endl;

//}



//End by Zhufeng, Feb, 2014

int AT::get_second_logpoint( int lot_id)

{

	//This function is to get the logpoint of second pass of lot lot_id

	//Dec 30, 2011, Zhufeng

	int dev_id=lots_device[lot_id];

	int logpoint=lot_logpoint[lot_id];

	if(multipass_vector[dev_id-1].whether_multi)

	{

		vii p_logpoint=find(multipass_vector[dev_id-1].logpoint_list.begin(),multipass_vector[dev_id-1].logpoint_list.end(),logpoint);

		if(p_logpoint==multipass_vector[dev_id-1].logpoint_list.end() || p_logpoint==(multipass_vector[dev_id-1].logpoint_list.end()-1))

		{

			//cannot find the logpoint or the logpoint is already the last logpoint

			return -1;

		}else

		{

			++p_logpoint;

			return * p_logpoint;

		}



	}else

	{

		return -1;

	}

}



void AT::show_second_logpoint()

{

	ofstream f_out("Debug/second_logpoint.txt");

	vector<char *>::iterator p_l=LOT.begin();

	while(p_l!=LOT.end())

	{

		int lot_id=p_l-LOT.begin()+1;

		int second_logpoint=get_second_logpoint(lot_id);

		f_out<<*p_l<<",current logpoint="<<lot_logpoint[lot_id]<<",second pass logpoint="<<second_logpoint<<endl;

		p_l++;

	}

}



void AT::initialize_L_second_pass_sim_lambda(){

	L_second_pass_sim_lambda.clear();

	for(int sim=1;sim<=(int)SIM.size();sim++){

		int i=SIM[sim-1].represented_machine_id;

		vector<int>::iterator p_Lambda=Lambda_i[i].int_vector.begin();

		while(p_Lambda!=Lambda_i[i].int_vector.end()){

			int Lambda_id=(*p_Lambda);

			vector<int> LS;

			//get_L_i_lambda(LS,i,Lambda_id);



			get_L_second_pass_i_lambda(LS,i,Lambda_id);

			



			pair< pair<int,int>,vec_int> one_pair;

			one_pair.first.first=sim;

			one_pair.first.second=Lambda_id;

			one_pair.second.int_vector.resize((int)LS.size());

			copy(LS.begin(),LS.end(),one_pair.second.int_vector.begin());

			L_second_pass_sim_lambda.push_back(one_pair);

			p_Lambda++;

		}

	}

}



void AT::show_L_second_pass_sim_lambda(){

	ofstream fdebug_L_i_lambda("Debug/L_second_pass_i_lambda.txt");

	fdebug_L_i_lambda<<"show the second pass of lots for sim_lambda:"<<endl;

	vector< pair< pair<int,int>,vec_int> >::iterator pLS=L_second_pass_sim_lambda.begin();

	while(pLS!=L_second_pass_sim_lambda.end()){

		fdebug_L_i_lambda<<"sim="<<(*pLS).first.first<<",Lambda_id="<<(*pLS).first.second<<",lots:";

		vii p_lot=(*pLS).second.int_vector.begin();

		while(p_lot!=(*pLS).second.int_vector.end()){

			fdebug_L_i_lambda<<(*p_lot)<<",";

			p_lot++;

		}

		fdebug_L_i_lambda<<endl;

		pLS++;

	}

	fdebug_L_i_lambda.close();

}



int AT::index_L_second_pass_sim_lambda(int simid,int Lambda_id){

	//get the index of element of vector L_sim_lambda given simid and Lamdua_id

	int index=-1;

	vector< pair< pair<int,int>,vec_int> >::iterator p=L_second_pass_sim_lambda.begin();

	while(p!=L_second_pass_sim_lambda.end()){

		if((*p).first.first==simid && (*p).first.second==Lambda_id){

			index=p-L_second_pass_sim_lambda.begin();

		}

		p++;

	}

	//index=-1,cannot find the combination

	if(index==-1)

	{

		cout<<"WRONG in index_L_second_pass_sim_lambda(int simid,int Lambda_id)!"<<endl;

	}

	return index;

}



void AT::get_L_second_pass_i_lambda(vector<int> & LS,int i,int lambda_id){

	//get the lots that can be processed by machine i with setup lambda_id. The results are stored in LS

	LS.clear();

	vii p_l=L_second_pass_i[i].int_vector.begin();

	while(p_l!=L_second_pass_i[i].int_vector.end()){

		int l=(*p_l);

		//cout<<"in get_L_i_lambda,i="<<i<<",l="<<l<<",lambda_id="<<lambda_id<<endl;

		vector<tooling_configure> TS;



		get_S_i_l_second_pass_Lambda(TS,i,l,lambda_id);



		//cout<<"size of S(i,l,lambda)="<<TS.size()<<endl;

		if((int)TS.size()>0){

			LS.push_back(l);

		}

		p_l++;

	}

}



void AT::initialize_L_second_pass_i()

{//initialize the data structure L_i_second_pass

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



			
                    int l=(*p_l);
					int logpoint=get_second_logpoint(l);
					//int operation=lot_operation[l];
					if(logpoint==(*p_S).logpoint 
						//&& operation==(*p_S).operation
						)
					{
						//end by Zhufeng

						vii p_int=find(L_second_pass_i[i].int_vector.begin(),L_second_pass_i[i].int_vector.end(),(*p_l));



						if(p_int==L_second_pass_i[i].int_vector.end()){



							L_second_pass_i[i].int_vector.push_back((*p_l));



						}



						//added by Zhufeng
					}
					//end by Zhufeng

					p_l++;

					

				}

				

			}

			p_S++;

		}

		p_M++;

	}

}

void AT::show_L_second_pass_i()

{

	//output the data structure L_second_pass_i to a debug file

	ofstream fout("Debug/L_second_pass_i.txt");

	fout<<"show L_second_pass_i ------------------------------------"<<endl;

	for(int i=1;i<=Num_Machine;i++){

		fout<<"i="<<i<<" processes the second pass of lots:";

		vii p_int=L_second_pass_i[i].int_vector.begin();

		while(p_int!=L_second_pass_i[i].int_vector.end()){

			fout<<LOT[(*p_int)-1]<<",";

			p_int++;

		}

		fout<<endl;

	}

	fout.close();

}



void AT::get_S_i_l_second_pass_Lambda(vector<tooling_configure> &TS,int i,int l,int Lambda_id){

	//get the routes that can use machine i to process lot l with setup Lambda_id

	vector<tooling_configure> TS2;	

	get_S_second_pass(TS2,i,l);

	vector<tooling_configure>::iterator p_TS2=TS2.begin();

	vector<int> a;	

	while(p_TS2!=TS2.end()){

		a.clear();	

		int s=(*p_TS2).id;

		vii pt=(*p_TS2).tf.tools.begin();

		while(pt!=(*p_TS2).tf.tools.end()){

			int tf=(*pt);

			int pos=s*Max_Num_Tooling_Family+tf;

			//a.push_back(a_st[s][tf]);

			a.push_back(a_st[pos]);

			pt++;

		}

		int La_id=get_Lambda_id((*p_TS2).tf.tools,a,(*p_TS2).temperature);

		if(La_id==Lambda_id){

			struct tooling_configure one_tc;

			one_tc.mg=(*p_TS2).mg;

			one_tc.device=(*p_TS2).device;

			one_tc.id=(*p_TS2).id;

			one_tc.temperature=(*p_TS2).temperature;

			one_tc.tf.tools.resize((int)(*p_TS2).tf.tools.size());

			copy((*p_TS2).tf.tools.begin(),(*p_TS2).tf.tools.end(),one_tc.tf.tools.begin());

			//added by Zhufeng for logpoint
			one_tc.logpoint=(*p_TS2).logpoint;
			one_tc.operation=(*p_TS2).operation;
			//end by Zhufeng

			TS.push_back(one_tc);

		}

		p_TS2++;	

	}

}



void AT::get_S_second_pass(vector<tooling_configure> & TS,int i,int l){

	//get the routes that can use machine i to process lot l

	//----------------------------------

        int mg=machine_mg[i];

        vector<Machine>::iterator p_M=Machine_vector.begin()+i-1;

        vector<tooling_configure>::iterator p_S=S.begin();

       	int dev=lots_device[l]; 



		//second pass, Dec 31, 2011. Note:not update the operation number yet because all the data now have the same opertation
		int logpoint=get_second_logpoint(l);
		

		

	   int operation=lot_operation[l];

		//end by Zhufeng for logpoint



	while(p_S!=S.end()){

                vii p_int=find((*p_M).temperature_vector.begin(),(*p_M).temperature_vector.end(),(*p_S).temperature);

                if((*p_S).mg==mg && p_int!=(*p_M).temperature_vector.end() && (*p_S).device==dev

					//added by Zhufeng for second pass , Dec 31, 2011
					&& (*p_S).logpoint==logpoint
					//&& (*p_S).operation==operation
					//end by Zhufeng					

					){

                        struct tooling_configure one_tc;

        		one_tc.mg=(*p_S).mg;

			one_tc.device=(*p_S).device;

			one_tc.id=(*p_S).id;

			one_tc.temperature=(*p_S).temperature;



			//added by Zhufeng for logpiont, Aug 31, 2011
			one_tc.logpoint=logpoint;
			one_tc.operation=operation;
			//end by Zhufeng for logpoint



			

			one_tc.tf.tools.resize((int)(*p_S).tf.tools.size());

			copy((*p_S).tf.tools.begin(),(*p_S).tf.tools.end(),one_tc.tf.tools.begin()); 

         	        TS.push_back(one_tc);

                }

                p_S++;

        }

}



int AT::max_rate_s_second_pass(int i,int l,int Lambda_id){
	
	int sim=machine_sim[i];
	
	int index=index_S_sim_l_second_pass_lambda(sim,l,Lambda_id);
	return S_sim_l_lambda[index].s;

}


void AT::initialize_S_sim_l_second_pass(){
	for(int sim=1;sim<=(int)SIM.size();sim++){
		int i=SIM[sim-1].represented_machine_id;
		for(int l=1;l<=Num_Lot;l++){
				
				vector<tooling_configure> TS;

				get_S_second_pass(TS,i,l);

				if((int)TS.size()>0){
					pair< pair<int,int>,vec_int > one_pair;
					one_pair.first.first=sim;
					one_pair.first.second=l;
					vector<tooling_configure>::iterator p_TS=TS.begin();
					while(p_TS!=TS.end()){
						int s=(*p_TS).id;
						one_pair.second.int_vector.push_back(s);
						p_TS++;
					}
					S_sim_l_second_pass.push_back(one_pair);
				}//end of if((int)TS.size()>0){
		}//end of for(int l=1;l<=Num_Lot;l++){
	}//end of for(int sim=1;sim<=(int)SIM.size();sim++){
}

int AT::index_S_sim_l_second_pass(int sim,int l){
	int index=-1;
	for(int id=1;id<=(int)S_sim_l_second_pass.size();id++){
		if(S_sim_l_second_pass[id-1].first.first==sim && S_sim_l_second_pass[id-1].first.second==l){
			index=id-1;
			break;
		}
	}
	if(index<0){
		cout<<"sim="<<sim<<",l="<<l<<",WRONG index!!! in index_S_sim_l_second_pass()"<<endl;
	}
	return index;
}

void AT::show_S_sim_l_second_pass(){
	ofstream fout("Debug/S_sim_l_second_pass.txt");
	for(int id=1;id<=(int)S_sim_l_second_pass.size();id++){
		fout<<"id="<<id-1<<",sim="<<S_sim_l_second_pass[id-1].first.first<<",l="<<S_sim_l_second_pass[id-1].first.second<<",s(sim,l)=";
		vii p=S_sim_l_second_pass[id-1].second.int_vector.begin();
		while(p!=S_sim_l_second_pass[id-1].second.int_vector.end()){
			fout<<(*p)<<",";
			p++;
		}
		fout<<endl;
	}
	fout.close();
}
void AT::initialize_S_sim_l_second_pass_lambda(){
	int count=1; // count is to record the id of the element in the S_sim_l_lambda
	sim_position_S_sim_l_second_pass_lambda.push_back(count);
	for(int sim=1;sim<=(int)SIM.size();sim++){
		int i=SIM[sim-1].represented_machine_id;
		//cout<<"i="<<i<<endl;
		vii p_Lambda=Lambda_i[i].int_vector.begin();
		while(p_Lambda!=Lambda_i[i].int_vector.end()){
			int Lambda_id=(*p_Lambda);
			//cout<<"Lambda_id="<<Lambda_id<<endl;
			int index=index_L_second_pass_sim_lambda(sim,Lambda_id);
			//cout<<"index="<<index<<endl;
			vector<int>::iterator p_L=L_second_pass_sim_lambda[index].second.int_vector.begin();
			while(p_L!=L_second_pass_sim_lambda[index].second.int_vector.end()){
				int l=(*p_L);
				//cout<<"l="<<l<<endl;
				//int d=lots_device[l];
				int flag=0;
				for(int id=1;id<=(int)S_sim_l_second_pass_lambda.size();id++){
					if(sim==S_sim_l_second_pass_lambda[id-1].sim && Lambda_id==S_sim_l_second_pass_lambda[id-1].Lambda_id && l==S_sim_l_second_pass_lambda[id-1].l){
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
				
				get_S_i_l_second_pass_Lambda(TS,i,l,Lambda_id);
                
				vector<tooling_configure>::iterator pTS=TS.begin();
				while(pTS!=TS.end()){
					int s=(*pTS).id;
					if(rate[s]>max_rate){
						max_rate=rate[s];
						max_rate_s=s;
					}
					pTS++;
				}
				
				struct S_sim_l_lambda_item one_item;
				one_item.sim=sim;
				one_item.l=l;
				one_item.Lambda_id=Lambda_id;
				one_item.s=max_rate_s;
				S_sim_l_second_pass_lambda.push_back(one_item);
				count++;
				p_L++;
			}
			p_Lambda++;
		}//end of while(p_Lambda!=Lambda_i[i].int_vector.end()){
         sim_position_S_sim_l_second_pass_lambda.push_back(count);
	}//end of for(int sim=1;sim<=(int)SIM.size();sim++){
}

void AT::show_S_sim_l_second_pass_lambda(){
	ofstream fout("Debug/S_sim_l_second_pass_lambda.txt");
	int position=1;
	for(int id=1;id<=(int)S_sim_l_second_pass_lambda.size();id++){
		fout<<"position="<<position
			<<", sim="<<S_sim_l_second_pass_lambda[id-1].sim<<","
			<<"l="<<S_sim_l_second_pass_lambda[id-1].l<<","
			<<"lambda="<<S_sim_l_second_pass_lambda[id-1].Lambda_id<<","
			<<"s="<<S_sim_l_second_pass_lambda[id-1].s<<endl;
		position++;
	}
	vii p_sim=sim_position_S_sim_l_second_pass_lambda.begin();
	int sim=1;
	while(p_sim!=sim_position_S_sim_l_second_pass_lambda.end())
	{
		fout<<"sim="<<sim<<", position="<<(* p_sim)<<endl;
		sim++;
		p_sim++;
	}

	fout.close();
}

int AT::index_S_sim_l_second_pass_lambda(int sim,int l,int Lambda_id){
	int index=-1;
	if(sim<=0)
	{
		cout<<"WRONG sim in index_S_sim_l_second_pass_lambda() "<<endl;
		return index;
	}
	int start_id=sim_position_S_sim_l_second_pass_lambda[sim-1];
	for(int id=start_id;id<=(int)S_sim_l_second_pass_lambda.size();id++){
		if(sim==S_sim_l_second_pass_lambda[id-1].sim && l==S_sim_l_second_pass_lambda[id-1].l && Lambda_id==S_sim_l_second_pass_lambda[id-1].Lambda_id){
			index = id-1;
			break;
		}
	}
	if(index<0){
		cout<<"Warning!! Wrong S_sim_l_second_pass_lambda index!!"<<endl;
		cout<<"sim="<<sim<<",l="<<l<<",Lambda_id="<<Lambda_id<<endl;
	}
	return index;
}

//end by Zhufeng for mulitpass, Dec 31th, 2011



//Added by Zhufeng for multipass,Heuristics 3, Jan 20, 2012

void AT::readsolution( vector<solution> & first_pass_solution ){

	cout<<"======================================= read solution.csv ================================================ "<<endl;

    //vector <Machine2> target_vector;

	char temp[1000],pdes[500];

	

	ifstream f_machines("Output/solution.csv");

	f_machines.getline(temp,1000,'\n');

	f_machines.getline(temp,1000,'\n');

	trim(temp);	

	while(!f_machines.eof() && strlen(temp)>0)

	{   

		struct solution one_target;

		

		char MachineName[50];

		char MachineFamilyName[50];

		char Initial_Lot_Flag[2];

		int lots;



		char lot_name[50];



		char DeviceName[50];

		char ToolFamilyName[50];

		int certification;

		char CompletionTime[50];



		char setuptime[50];

		double from_setup_to_completion;

		double  weight;

	//	char toolFamilyName[50];

	//	int temperature;

		int logpoint;

		int pass_no;

		int quantity;



	    int l=0,k=0;

	    for(int i=1;i<=50;i++)

		   { pdes[i]=' ';}

        k=strtok2(pdes,temp,l,',');		//Machine_Instance

        trim(pdes);



		if(strlen(pdes)>0)

	{

		//This is the first line of multi-tooling solution

	    strcpy(MachineName,pdes);

		l=strtok2(pdes,temp,k,',');		//Machine_Family_Name

		trim(pdes);

	    strcpy(MachineFamilyName,pdes);

        k=strtok2(pdes,temp,l,',');		//Lots_Name

        trim(pdes);

		strcpy(lot_name,pdes);

		lots=atoi(pdes);



		



		l=strtok2(pdes,temp,k,',');		//Logpoint

        k=l;

		trim(pdes);

		logpoint=atoi(pdes);

		one_target.logpoint=logpoint;





        l=strtok2(pdes,temp,k,',');		//Quantity

		trim(pdes);

		quantity=atoi(pdes);

		one_target.quantity=quantity;

        k=strtok2(pdes,temp,l,',');     //Lot_Weight

		trim(pdes);

		weight=atof(pdes);



        l=strtok2(pdes,temp,k,',');     //Initial_Lot_Flag

        trim(pdes);

		strcpy( Initial_Lot_Flag, pdes); 

        k=strtok2(pdes,temp,l,',');     //Device_Name

		trim(pdes);

        strcpy(DeviceName,pdes);

        l=strtok2(pdes,temp,k,',');     //Tool_Family_Name

        trim(pdes);

        strcpy(ToolFamilyName,pdes);

		

        k=strtok2(pdes,temp,l,',');     //Certification. For initial lots, certification=0

        trim(pdes);

		certification=atoi(pdes);

        l=strtok2(pdes,temp,k,',');     //setup time

		trim(pdes);

        strcpy(setuptime,pdes);



		k=strtok2(pdes,temp,l,',');     //start time

		l=k;



        k=strtok2(pdes,temp,l,',');     //completion time

		trim(pdes);

		strcpy(CompletionTime,pdes);



		l=strtok2(pdes,temp,k,',');  //Pass_No

		trim(pdes);

		pass_no=atoi(pdes);

		one_target.pass_no=pass_no;

		

        //if(strcmp(Initial_Lot_Flag,"Y")==0)

		//	cout<<'1'<<endl;

		

		int machine_id=0;

		int mf_id=0;

		int tf_id=0;

		int dev_id=0;

	

        for(int id=1;id<=(int)MI.size();id++)

		{			

			if(strcmp(MI[id-1],MachineName)==0)

			{

				machine_id=id;

				break;

			}			

		}

		if(machine_id<1)

		{ 

			cout<<"error:Machine "<< MachineName<<" is not found in Machine.csv!!!"<<endl;

			exit(0);

		}else{

			one_target.mi_id=machine_id;

			one_target.weight=(double)weight;



			//one_target.initial_running_lot_id=Machine_vector[machine_id-1].initial_lot; //initial running lot id



			for(int id=1;id<=(int)MF.size();id++)

			{

				if(strcmp(MF[id-1],MachineFamilyName)==0)

				{

					mf_id=id;

					break;

				}

			}

			if(mf_id<1)

			{

				cout<<"error:Machine Family"<<MachineFamilyName<<"is not found in Machine.csv!!!"<<endl;

				exit(0);

			}else{

				one_target.mf_id=mf_id;

				for(int id=1;id<=(int)DEV.size();id++)

				{

					if(strcmp(DEV[id-1],DeviceName)==0)

					{

						dev_id=id;

					}

				}

                if(dev_id<1)

				{

					cout<<"error:device"<<DeviceName<<"is not found in wip.csv!!!"<<endl;

				    exit(0);

			    }else{

					one_target.dev_id=dev_id;

				    

				    one_target.lots=lots;



				   if(strcmp(Initial_Lot_Flag,"Y")==0)

				   {

				       one_target.Initial_Lot_Flag= true ;

				   }

				

				   if (strcmp(Initial_Lot_Flag,"N")==0)

				  {

                       one_target.Initial_Lot_Flag= false;

				  }

                

				   if(one_target.Initial_Lot_Flag) // This is a initial lot,the target tool is just the initial tool setup.

				   {   

					   one_target.tf_id=Machine_vector[machine_id-1].initial_tool;

					   one_target.tf_id_vector.push_back(Machine_vector[machine_id-1].initial_tool);

					   one_target.temperature=Machine_vector[machine_id-1].initial_temperature;



					   one_target.lot_id=0;// 0 denote this is actually an initial lot.



					   strcpy(one_target.time,CompletionTime);   //the completion time of  lots





					   // one_target.temperature_set.push_back(Machine_vector[machine_id-1].initial_temperature);

					   //test

					   //cout<<MI[Machine_vector[machine_id-1].id-1]<<",initial tool:"<<Machine_vector[machine_id-1].initial_tool<<endl;

					   //test

					   strcpy(one_target.time,CompletionTime); //the completion time of the initial lot



					   struct date_time CompletionTime_date_time;

						str_to_date_time(CompletionTime,CompletionTime_date_time );



						one_target.completion_time=CompletionTime_date_time;



						one_target.from_begin_to_start_time=-1; //negative denotes that this is an initial lot

					   //cout<<one_target.time<<endl;

				   }else{

					   //regular lots



					   int lot_id=-1;

					   for(int id=1;id<=(int)LOT.size();id++)

					   {

						   if(strcmp(LOT[id-1], lot_name)==0)

						   {

							   lot_id=id;

							   break;

						   }

					   }

					   if(lot_id<1)

					   {

						   cout<<"error: lot"<<pdes <<"is not found in wip.csv"<<endl;

						   exit(0);

					   }

					   one_target.lot_id=lot_id;  //lot_id



					  for (int id =1;id<=(int)TF.size();id++)

					 {			

			           if(strcmp(TF[id-1],ToolFamilyName)==0)

			             {

				            tf_id=id;

				               break;

			             }			

		              }

					   if(tf_id<1)

				      {

					     cout<<"error:Tooling Family"<<ToolFamilyName<<"is not found in Tooling.csv"<<endl;

					     exit(0);

					  }else{

					     one_target.tf_id=tf_id;

						 one_target.tf_id_vector.push_back(tf_id);

                         one_target.temperature=certification;

                         //one_target.temperature_set.push_back(certification);

					     strcpy(one_target.time,CompletionTime);   //the completion time of  lots



						 strcpy(one_target.setuptime,setuptime); // the setup time of regular lots not initial lots. 

						 struct date_time setuptime_date_time;

						 str_to_date_time(setuptime,setuptime_date_time);

						 struct date_time CompletionTime_date_time;

						 str_to_date_time(CompletionTime,CompletionTime_date_time );



						 one_target.completion_time=CompletionTime_date_time;



						 double from_setup_to_completion=get_date_time_difference(CompletionTime_date_time,setuptime_date_time);//This is just for regular lots

						 one_target.from_setup_to_completion=from_setup_to_completion;



						 double from_begin_to_setup=get_date_time_difference(setuptime_date_time,General_Snapshoot_time);



						 one_target.from_begin_to_setup=from_begin_to_setup;

					   //cout<<TF[one_target.tf_id-1]<<' '<<one_target.time<<endl;				

					   }//end of if(tf_id<1)

				   }//end of if(one_target.Initial_Lot_Flag) 



				}//end of if(dev_id<1)

                 			

			}//end of if(mf_id<1)

		

            first_pass_solution.push_back(one_target);

			f_machines.getline(temp,1000,'\n');

		    trim(temp);



		}//end of if(strlen(pdes)>0)



		}else 

			{// the second line or the third line of multi-tooling

				 strcpy(MachineName,pdes);

		l=strtok2(pdes,temp,k,',');		//Machine_Family_Name

		trim(pdes);

	    strcpy(MachineFamilyName,pdes);

        k=strtok2(pdes,temp,l,',');		//Lots_Name

        trim(pdes);

		lots=atoi(pdes);

		



		l=strtok2(pdes,temp,k,',');	//Logpoint

		k=l;



        l=strtok2(pdes,temp,k,',');		//Quantity

        k=strtok2(pdes,temp,l,',');     //Lot_Weight

        l=strtok2(pdes,temp,k,',');     //Initial_Lot_Flag

        trim(pdes);

		strcpy( Initial_Lot_Flag, pdes); 

        k=strtok2(pdes,temp,l,',');     //Device_Name

		trim(pdes);

        strcpy(DeviceName,pdes);

        l=strtok2(pdes,temp,k,',');     //Tool_Family_Name

        trim(pdes);

        strcpy(ToolFamilyName,pdes);

		

        k=strtok2(pdes,temp,l,',');     //Certification. For initial lots, certification=0

        trim(pdes);

		certification=atoi(pdes);

        l=strtok2(pdes,temp,k,',');     //setup time

        k=strtok2(pdes,temp,l,',');     //completion time

		trim(pdes);

		strcpy(CompletionTime,pdes);

		vector<solution>::iterator p_t=first_pass_solution.end()-1; 

		one_target=(*p_t);

		int tf_id=0;

		 for (int id =1;id<=(int)TF.size();id++)

		 {			

			  if(strcmp(TF[id-1],ToolFamilyName)==0)

			  {

				  tf_id=id;

				      break;

			   }			

		 }

		 if(tf_id<1)

	    {

			cout<<"error:Tooling Family"<<ToolFamilyName<<"is not found in Tooling.csv"<<endl;

			exit(0);

		 }else{

			  one_target.tf_id_vector.push_back(tf_id);

			  //one_target.temperature_set.push_back(certification);

			   }

         first_pass_solution.erase( p_t);

		 first_pass_solution.push_back(one_target);

         f_machines.getline(temp,1000,'\n');

		 trim(temp);



		}//end of if(strlen(pdes)>0) else

	    

	}//end of while



	int mi_id_temp=-1;

	for(int id=1;id<=(int)first_pass_solution.size();id++)

	{ 

		first_pass_solution[id-1].id=id;

		if(!first_pass_solution[id-1].Initial_Lot_Flag)

		{

			if(mi_id_temp!=first_pass_solution[id-1].mi_id)

			{

				first_pass_solution[id-1].from_begin_to_start_time=first_pass_solution[id-1].from_begin_to_setup;

				mi_id_temp=first_pass_solution[id-1].mi_id;

			}else

			{

				first_pass_solution[id-1].from_begin_to_start_time=first_pass_solution[id-2].from_begin_to_setup+first_pass_solution[id-2].from_setup_to_completion;

			}



		}

	}

	f_machines.close();

    

}



void AT::show_first_pass_solution(vector<solution> & first_pass_solution){



	ofstream fout("Debug/first_pass_solution.txt");

	vector<solution>::const_iterator p_M=first_pass_solution.begin();

	while(p_M!=first_pass_solution.end()){

		fout<<"ID="<<(*p_M).id<<',';

		fout<<"Logpoint="<<(*p_M).logpoint<<',';

		if((*p_M).Initial_Lot_Flag)

		{

			fout<<"Initial lot,";

		}

		//fout<<"initial_running_lot_id="<<(*p_M).initial_running_lot_id;

		fout<<"MachineName="<<MI[(*p_M).mi_id-1]<<',';

		fout<<"MachineFamilyName="<<MF[(*p_M).mf_id-1]<<',';

		fout<<"Lot_id="<<(*p_M).lot_id;

		fout<<",Lots="<<(*p_M).lots<<',';

		fout<<"Dev="<<DEV[(*p_M).dev_id-1]<<',';

		if((*p_M).Initial_Lot_Flag)

		{

			fout<<"tooling family id="<<(*p_M).tf_id<<',';

		    fout<<"certification="<<(*p_M).temperature<<',';

		}else

		{

			fout<<"tooling family="<<TF[(*p_M).tf_id-1]<<',';

		    fout<<"certification="<<(*p_M).temperature<<',';

		}

		fout<<"setup_time=" <<(* p_M).from_begin_to_setup;

		fout<<",start_time="<<(* p_M).from_begin_to_start_time;

		struct date_time start_time=increase_date_time(General_Snapshoot_time,(* p_M).from_begin_to_start_time);

		output_date_time(start_time);

		fout<<",start at"<<start_time.str;

		fout<<",completion_time="<<(*p_M).time<<endl;

		struct date_time completion_time=(*p_M).completion_time;

		output_date_time(completion_time);

		fout<<"completion_time(date_time)="<<completion_time.str<<",";

		fout<<"pass_no="<<(*p_M).pass_no;

		fout<<",tooling family vector=";

		vector<int>::const_iterator p_tf=(* p_M).tf_id_vector.begin();

		while(p_tf!=(* p_M).tf_id_vector.end())

		{

			int tf_id=(*p_tf);

			fout<<TF[tf_id-1]<<",";

			p_tf++;

		}

		fout<<"quantity="<<(* p_M).quantity<<", weight="<<(* p_M).weight;

		fout<<endl;

		p_M++;

	} 

	fout.close();



}



//Jan 24, 2012

void AT::initialize_machine_list( vector<multi_mach> & machine_list, vector<solution> & first_pass_solution,const double * H)

{

	cout<<"initialize_machine_list"<<endl;

	for(int id=1;id<=(int)Machine_vector.size();id++)

	{

		

		struct multi_mach one_mach;

		one_mach.mi_id=id;

		one_mach.mf_id=Machine_vector[id-1].mg;

		one_mach.whether_initial_lot=Machine_vector[id-1].hasInitialLot;

		one_mach.initial_running_lot_id=Machine_vector[id-1].initial_lot+1;

		one_mach.whether_need_check_for_new_lot=true;



		//Feb 24

		one_mach.whether_resetup=false;



		one_mach.certification=-1;//intialization;



		double from_begin_to_setup=0;

		double from_begin_to_completion=0;



		



		//Feb 11, 2012, Added by Zhufeng

		if(one_mach.whether_initial_lot)

		{

			if(initial_running_lot[one_mach.initial_running_lot_id-1].time_remain <= H[id] )

			{

				one_mach.whether_finish_initial=true;

			}else

			{

				one_mach.whether_finish_initial=false;

			}

		}else

		{

			one_mach.whether_finish_initial=false;

		}

		//End Feb 11.







		//General_Snapshoot_time



		vector<solution>::const_iterator p_fs=first_pass_solution.begin();

		int num_lot=0;

		while( p_fs != first_pass_solution.end())

		{

			int mi_id=(*p_fs).mi_id;

			//from_begin_to_setup=0;        //make them 0. This is for some machine that doesn't have any lot in the first-pass solution.

			//from_begin_to_completion=0;



			if(id!=mi_id)

			{

				p_fs++;

				continue;

			}else

			{

				

				num_lot++;

				

				if(num_lot<=2)

				{// If the machine just has a initial lot then tf_id_vector would be given the initial tooling, 

					//if the machine has a regular lot then tf_id_vector would be given the tooling of the first regular lot

					//The same rule applys to when complete.

					one_mach.tf_id_vector=(* p_fs).tf_id_vector;

					one_mach.certification=(* p_fs).temperature;

					from_begin_to_completion=get_date_time_difference((* p_fs).completion_time,General_Snapshoot_time);

				}



				if(Machine_vector[id-1].hasInitialLot)

				{ //get the setup time of the first regular lot on the machine.

					if(num_lot==2)

					{

						char lot_setuptime[50];

						strcpy(lot_setuptime,(* p_fs).setuptime);

						struct date_time setup_time;

						str_to_date_time(lot_setuptime,setup_time);

						from_begin_to_setup=get_date_time_difference(setup_time,General_Snapshoot_time);

					}



				}else

				{

					if(num_lot==1)

					{

						

						char lot_setuptime[50];

						strcpy(lot_setuptime,(* p_fs).setuptime);

						struct date_time setup_time;

						str_to_date_time(lot_setuptime,setup_time);

						from_begin_to_setup=get_date_time_difference(setup_time,General_Snapshoot_time);

					}

				}





				if(!(* p_fs).Initial_Lot_Flag)

				{ // If the lot is not initial lot 

				from_begin_to_completion=get_date_time_difference((* p_fs).completion_time,General_Snapshoot_time);



				one_mach.from_begin_to_start_time_list.push_back((* p_fs).from_begin_to_start_time);



				one_mach.from_begin_to_completion_time_list.push_back(from_begin_to_completion);

				one_mach.lot_id_list.push_back((* p_fs).lot_id );

				one_mach.logpoint_list.push_back( (* p_fs).logpoint);

				one_mach.pass_no_list.push_back( (* p_fs).pass_no);



				one_mach.first_pass_solution_id_list.push_back(p_fs-first_pass_solution.begin()+1);



				}



				p_fs++;

			}//end of if(id!=mi_id) else





		}//end of while( p_fs != first_pass_solution.end())



		one_mach.from_begin_to_setup=from_begin_to_setup;

		one_mach.from_begin_to_completion=from_begin_to_completion;

		

		//added by Zhufeng, March 06, 2012 to compute correct operation time

		one_mach.from_begin_to_start=from_begin_to_setup;

		if(one_mach.whether_initial_lot && num_lot==1){

			one_mach.from_begin_to_start = from_begin_to_completion;  //if this machine just has an initial lot assigned during the first-pass solution, then this should be when the initial machine finishes its initial lot. 

		}

		//end by Zhufeng, March 06, 2012

			

		if(num_lot==0)

		{

			one_mach.whether_setup=false;



		}else

		{

			one_mach.whether_setup=true;

		}



		one_mach.horizon=H[id];



		//cout<<"one_mach.from_begin_to_completion="<<one_mach.from_begin_to_completion<<endl;



		machine_list.push_back(one_mach);

	}//end of for(int id=1;id<=(int)Machine_vector.size();id++)



}



void AT::show_machine_list( vector<multi_mach> & machine_list)

{

	ofstream fout("Debug/machine_list_initialization.txt");

	vector<multi_mach>::const_iterator p_m=machine_list.begin();

	while(p_m != machine_list.end())

	{

		int mf_id=(* p_m).mf_id;

		int mi_id=(* p_m).mi_id;

		bool whether_initial_lot=(* p_m).whether_initial_lot; //whether there is a initial lot on this machine

		int initial_running_lot_id=(* p_m).initial_running_lot_id; //initial_running_lot[id-1] is the corresponding initial lot. The reason adding this is  that LOT doesn't include initial lots.	

		

		vector<int> tf_id_vector=(* p_m).tf_id_vector;

		int certification=(* p_m).certification;

		vector<int> lot_id_list=(* p_m).lot_id_list; //regulor lot id list

		vector<int> logpoint_list=(* p_m).logpoint_list; //logpoint list for regular lots

		vector<double> from_begin_to_completion_time_list=(* p_m).from_begin_to_completion_time_list; //completion time for regular lots on the machine

		vector<double> from_begin_to_start_time_list=(* p_m).from_begin_to_start_time_list;

		vector<int> pass_no_list=(* p_m).pass_no_list;



		double from_begin_to_setup=(* p_m).from_begin_to_setup;

		double from_begin_to_completion=(* p_m).from_begin_to_completion;

		bool whether_need_check_for_new_lot=(* p_m).whether_need_check_for_new_lot;

		struct date_time mach_setup_time=increase_date_time(General_Snapshoot_time,from_begin_to_setup);

		struct date_time mach_completion_time=increase_date_time(General_Snapshoot_time,from_begin_to_completion);

		output_date_time(mach_setup_time);

		output_date_time(mach_completion_time);



		if(whether_need_check_for_new_lot)

		{

			fout<<"Y,";

		}else

		{

			fout<<"N,";

		}



		fout<<MF[mf_id-1]<<","<<MI[mi_id-1]<<",TF=";

		vector<int>:: const_iterator p_tf=tf_id_vector.begin();

		while (p_tf!=tf_id_vector.end())

		{

			fout<<TF[(* p_tf)-1]<<",";

			p_tf++;

		}

		fout<<"Certification="<<certification;

		if(whether_initial_lot)

		{

			fout<<",initial lot="<<initial_running_lot[initial_running_lot_id-1].lot_name;

		}else

		{

			fout<<",No initial lot, the initial_id="<<initial_running_lot_id;



		}

		if((* p_m).whether_finish_initial)

		{

			fout<<",intial lots finished";

		}else

		{

			fout<<",intial lots not finished( or no initial lots)";

		}



		if(!(* p_m).whether_setup )

		{

			fout<<",this machine neither initial lot nor setup later since no lot found in the solution.csv";

		}

		

		string reset;



		if((* p_m).whether_resetup)

		{

			reset="Y";

		}else

		{

			reset="N";

		}



		fout<<",when_setup="<<mach_setup_time.str<<",when_completion="<<mach_completion_time.str<<",horizon="<<(* p_m).horizon<<"resetup?="<<reset<<endl;

		fout<<"Lot_list:"<<endl;

		vector<int>:: const_iterator p_l=lot_id_list.begin();

		vector<int>:: const_iterator p_lp=logpoint_list.begin();

		vector<double>:: const_iterator p_time=from_begin_to_completion_time_list.begin();

		vector<double>:: const_iterator p_time2=from_begin_to_start_time_list.begin();

		vector<int>::iterator p_pn=pass_no_list.begin();

		while(p_l!=lot_id_list.end() && p_lp!=logpoint_list.end() && p_time!=from_begin_to_completion_time_list.end())

		{

			fout<<LOT[(* p_l)-1]<<"," <<(* p_lp) <<",start at :";

			struct date_time lot_start_time=increase_date_time(General_Snapshoot_time,(* p_time2));

			output_date_time(lot_start_time);

			fout<<lot_start_time.str;

			fout<<",completed at ";

			struct date_time lot_completion_time=increase_date_time(General_Snapshoot_time,(* p_time));

			output_date_time(lot_completion_time);

			fout<<lot_completion_time.str<<", pass_no = "<<(* p_pn)<<";"<<endl;

			p_l++;

			p_lp++;

			p_time++;

			p_time2++;

			p_pn++;

		}//end of while(p_l!=lot_id_list.end() && p_lp!=logpoint_list.end() && p_time!=from_begin_to_completion_time_list.end())



		fout<<"assigned lot list:"<<endl;

		vector<multi_lot>::const_iterator p_al=(* p_m).assigned_lot_list.begin();

		while( p_al != (* p_m).assigned_lot_list.end())

		{

			if((* p_al).whether_initial_lot)

			{

				fout<<initial_running_lot[(* p_al).initial_running_lot_id-1].lot_name;

			}else

			{



				fout<<LOT[(* p_al).lot_id-1];

			}

			fout<<",logpoint "<<(* p_al).logpoint<<",pass_no="<<(* p_al).pass_no<<",start at ";

			struct date_time lot_start_time=increase_date_time(General_Snapshoot_time,(* p_al).start_time);

			output_date_time(lot_start_time);

			fout<<lot_start_time.str;

			fout<<",completed at ";

			struct date_time lot_completion_time=increase_date_time(General_Snapshoot_time,(* p_al).from_begin_to_completion);

			output_date_time(lot_completion_time);

			fout<<lot_completion_time.str;

			fout<<endl;

			++p_al;

		}



		p_m++;



	}//end of while(p_m != machine_list.end())

}



void AT::initialize_first_pass_lot_vector( vector<multi_lot> & first_pass_lot_vector,vector<solution> & first_pass_solution)

{

	cout<<"initialize first_pass_lot_vector"<<endl;



	//first get first_pass_lot from initial_running_lot, some of them already finished before the start of the time horizon.

	for( int i=1;i<= (int)initial_running_lot.size();i++)

	{

		if(initial_running_lot[i-1].time_remain <=0)

		{

			struct multi_lot one_lot;

		    one_lot.lot_id=0;

			one_lot.logpoint=initial_running_lot[i-1].logpoint;

			one_lot.pass_no=1;

			one_lot.from_begin_to_completion=0;

			one_lot.whether_initial_lot=true;

			one_lot.initial_running_lot_id=i;



			one_lot.quantity=initial_running_lot[i-1].n_l_chips;

			one_lot.weight=initial_running_lot[i-1].w_l;



			one_lot.start_time=-1;//negative deonte that this is  the first-pass initial lot.



			first_pass_lot_vector.push_back(one_lot);

		}

	}



	//second get first_pass_lot from solution.

	vector<solution>::const_iterator p_fs=first_pass_solution.begin();

	int mi_id=-1;

	while( p_fs != first_pass_solution.end())

	{

		struct multi_lot one_lot;

	

		one_lot.start_time=(* p_fs).from_begin_to_start_time;

		one_lot.whether_initial_lot=(* p_fs).Initial_Lot_Flag;

		if((* p_fs).Initial_Lot_Flag)

		{

			one_lot.initial_running_lot_id=Machine_vector[(* p_fs).mi_id-1].initial_lot+1;

			//one_lot.start_time=-1;

		

		}else

		{

			one_lot.initial_running_lot_id=0;

			/*if((* p_fs).mi_id !=mi_id)

			{

				mi_id=(* p_fs).mi_id;

				one_lot.start_time=(* p_fs).from_begin_to_setup;

			}else

			{

				vector<multi_lot>::iterator p_temp=	first_pass_lot_vector.end()-1;

				one_lot.start_time=(* p_temp).from_begin_to_completion;

			}*/

		}



		one_lot.lot_id=(* p_fs).lot_id;



		one_lot.logpoint=(* p_fs).logpoint;

	

		one_lot.pass_no=(* p_fs).pass_no;

	

		one_lot.from_begin_to_completion= get_date_time_difference( (* p_fs).completion_time,General_Snapshoot_time);



		one_lot.quantity=(* p_fs).quantity;



		one_lot.weight = (* p_fs).weight;

		

		first_pass_lot_vector.push_back(one_lot);

		

		

		p_fs++;

	}



}



void AT::show_first_pass_lot_vector(vector<multi_lot> & first_pass_lot_vector)

{

	ofstream f_out("Debug/first_pass_lot_vector.txt");

	int id=1;

	vector<multi_lot>::const_iterator p_f=first_pass_lot_vector.begin();

	while( p_f !=first_pass_lot_vector.end())

	{

		if((* p_f).whether_initial_lot)

		

		{

			f_out<<"ID="<<id<<", Initial lot "<<initial_running_lot[(* p_f).initial_running_lot_id-1].lot_name;

		}else

		{

			f_out<<"ID="<<id<<", lot "<<LOT[(* p_f).lot_id-1];



		}

		f_out<<", logpoint="<<(* p_f).logpoint<<",pass_no="<<(* p_f).pass_no<<",wegiht="<<(* p_f).weight<<",quantity="<<(* p_f).quantity<<",start_time="<<(* p_f).start_time<<",from_begin_to_completion="<<(* p_f).from_begin_to_completion<<endl;

		p_f++;

		id++;



	}

}



void AT::initialize_unassigned_lot_first_pass_vector(vector<multi_lot> & unassigned_lot_first_pass_vector,

													 const vector<multi_lot> & first_pass_lot_vector

													 )

{

	cout<<"initialize_unassigned_lot_first_pass_vector"<<endl;

	//initialize the vecotr, remain_wip_vector by reading wip.csv. Actually it is the all wips excetp initial lots
	//Added by Zhufeng, May 29,2012, do statistics on the perenctage of lot containing key device. 
	//Add dev_id to lot for later using.
	int num_key_lot = 0;
	int num_lot_total = 0;
	double weighted_key_lot = 0;
	double weighted_lot_total = 0;
	//end by Zhufeng, may 29, 2012

	cout<<"-------------------read Debug/wip.csv--------------------------"<<endl;
	ifstream f_wip("Debug/wip.csv");
	char temp[2000],pdes[1000];
	f_wip.getline(temp,1000,'\n');
	f_wip.getline(temp,1000,'\n');
	while(!f_wip.eof())
	{
		struct multi_lot one_lot;
		int l=0,k=0;
		k=strtok2(pdes,temp,l,',');//lot name
        l=k;
		trim(pdes);
		//strcpy(one_wip.lot_name,pdes);
        //one_wip.lot_id=atoi(pdes);
		for(int i=1;i<=(int)LOT.size();i++)
		{
			if(!strcmp(pdes,LOT[i-1]))
			{
				one_lot.lot_id=i;
				break;
			}
		}
	    k=strtok2(pdes,temp,l,',');//get device name
	    l=k;
		trim(pdes);
		//Added by Zhufeng, on May 29,2012,for statistic on percentage of lots containing key device
		one_lot.dev_id = 0;
		for(int i=1;i<=(int)DEV.size();i++)
		{
			if(!strcmp(pdes, DEV[i-1]))
			{
				one_lot.dev_id = i;
			}
		}
		num_lot_total++;

		//end by Zhufeng, May 29,2012

		k=strtok2(pdes,temp,l,',');//package
	    l=k;
		trim(pdes);

		k=strtok2(pdes,temp,l,',');//Pin
	    l=k;
		trim(pdes);

		k=strtok2(pdes,temp,l,',');//Cur_QTY
	    l=k;
		trim(pdes);

		one_lot.quantity=atoi(pdes);

		k=strtok2(pdes,temp,l,',');//weight
		l=k;
		trim(pdes);

		one_lot.weight=(double)atof(pdes);
		//Added by Zhufeng, May 29, 2012, for statistic on percentage of lots containing key device
		weighted_lot_total+=one_lot.weight;

		if(one_lot.dev_id == 0)
		{
			cout<<"error: the device for multi-pass first-pass lot is not found!"<<endl;
		}else
		{
			int type=is_KD_PD(one_lot.dev_id);
			if( type ==1)
			{
				num_key_lot++;
				weighted_key_lot+=one_lot.weight;
			}
		}
		//end by Zhufeng,May 29,2012 

		k=strtok2(pdes,temp,l,',');//LOT_name
		l=k;
		trim(pdes);

		k=strtok2(pdes,temp,l,',');//LPT
		l=k;
		trim(pdes);

		one_lot.logpoint=atoi(pdes);
		one_lot.pass_no=1;

		k=strtok2(pdes,temp,l,',');//OPN
		l=k;
		trim(pdes);

		k=strtok2(pdes,temp,l,',');//TOTAL_PLAN_CT
		l=k;
		trim(pdes);

		k=strtok2(pdes,temp,l,',');//CUM_PLAN_CT
		l=k;
		trim(pdes);

		k=strtok2(pdes,temp,l,',');//LOT_AGE
		l=k;
		trim(pdes);

		k=strtok2(pdes,temp,l,',');	//START_TIME
		l=k;
		trim(pdes);
	
		bool whether_initial_lot=false;
		if(strlen(pdes)>0)
		{
			whether_initial_lot=true;
			
		}else
		{
			whether_initial_lot=false;
			
		}

		one_lot.whether_initial_lot=whether_initial_lot;

		f_wip.getline(temp,1000,'\n');

		

		//check whether this lot is already in the first-pass solution
		bool whether_assigned=false;
		vector<multi_lot>::const_iterator p_l=first_pass_lot_vector.begin();
		while( p_l != first_pass_lot_vector.end())
		{
			if((* p_l).lot_id == one_lot.lot_id && 
			  (* p_l).logpoint == one_lot.logpoint &&
			  (* p_l).pass_no == one_lot.pass_no )
			{
			  whether_assigned=true;
			}
			++p_l;
		}

	
		
		if(!whether_initial_lot && !whether_assigned)
		{
			one_lot.start_time = - 0.001;
			one_lot.from_begin_to_completion = -0.001;
			unassigned_lot_first_pass_vector.push_back(one_lot);
		}
			
	}//end of while(!f_wip.eof())
	//debug
	ofstream fout("Debug/unassigned_lot_first_pass_vecto.csv");
	fout<<"lot_name,logpoint,pass_no,QTY,weight"<<endl;
	for(int i=1;i<=(int)unassigned_lot_first_pass_vector.size();i++)
	{
		fout<<LOT[unassigned_lot_first_pass_vector[i-1].lot_id-1]<<","<<unassigned_lot_first_pass_vector[i-1].logpoint<<","<<unassigned_lot_first_pass_vector[i-1].pass_no<<","
			<<unassigned_lot_first_pass_vector[i-1].quantity<<","<<unassigned_lot_first_pass_vector[i-1].weight<<endl;
	}
	fout<<"number of lots = "<<(int)unassigned_lot_first_pass_vector.size()<<endl;
	//
	fout.close();
	//Added by Zhufeng, May 29, 2012, for statistic on percentage of lots containing key device
	fout.open("Output/statistics.txt", ios::out|ios::app);
	fout<<"percentage of lots containg key device = "<< (double)num_key_lot/ (double)num_lot_total<<endl;
	fout<<"weighted percentage of lots containg key device = "<< weighted_key_lot/weighted_lot_total<<endl;
	//end by Zhufeng, May 29, 2012, for statistic on percentage of lots containing key device
	//debug end

}



int AT::get_next_pass( struct multi_lot one_lot)

{

	//retunr -1 if only single pass for this device;0 if already in the last logpointlogpoint;logpoint, if normal.

	//This function is to get the logpoint of second pass of lot 

	//Jan 27, 2012, Zhufeng

	int dev_id=0;



	if(!one_lot.whether_initial_lot)

	{

		dev_id=lots_device[one_lot.lot_id];

	}else

	{

		dev_id=initial_running_lot[one_lot.initial_running_lot_id-1].dev_id;

	}



	if(dev_id<=0)

	{

		cout<<"error:can not find dev_id in AT::get_next_pass(struct multi_lot one_lot)"<<endl;

		exit(0);

	}



	int logpoint=one_lot.logpoint;



	if(multipass_vector[dev_id-1].whether_multi)

	{

		vii p_logpoint=find(multipass_vector[dev_id-1].logpoint_list.begin(),multipass_vector[dev_id-1].logpoint_list.end(),logpoint);

		if(p_logpoint==multipass_vector[dev_id-1].logpoint_list.end() || p_logpoint==(multipass_vector[dev_id-1].logpoint_list.end()-1))

		{

			//cannot find the logpoint or the logpoint is already the last logpoint

			return 0;

		}else

		{

			++p_logpoint;

			return * p_logpoint;

		}



	}else

	{

		return -1;

	}





}



bool AT::get_candidate_lot_list( vector<multi_lot> & candidate_lot_list, const vector<multi_lot> & first_pass_lot_vector,const vector<multi_lot> & finshied_lot_list, double current_time,double last_time,int & Num_Pass)

{

	//cout<<"get_candidate_lot_list"<<endl;

	bool whether_new_lot=false;

	vector<multi_lot>::const_iterator p_fpl=first_pass_lot_vector.begin();

	if(last_time >0)

	{

		//when last_time>0, get lots finished >last_ime, when last_time==0, get lots finished >= last_time

		while(p_fpl !=first_pass_lot_vector.end())

		{

			double completion_time=(* p_fpl).from_begin_to_completion;

			if( completion_time>last_time && completion_time<=current_time)

			{

				//if this lot is completed between last_time and current_time

				struct multi_lot one_lot=(* p_fpl);

				int next_logpoint=get_next_pass(one_lot);

				if(next_logpoint>0)

				{

					one_lot.logpoint=next_logpoint;

					one_lot.pass_no++;

					candidate_lot_list.push_back( one_lot);

					whether_new_lot=true;

					//added by Zhufeng, May 14, 2012

					if(one_lot.pass_no > Num_Pass)

					{

						//cout<<"New pass considered,"<<one_lot.pass_no<<endl;

						int diff = one_lot.pass_no - Num_Pass;



						for( int i=1;i<=diff;i++)

						{

							int pass_no=Num_Pass+i;

							for(int j=1;j<=Num_Device;j++)

							{

								struct multi_production one_device_production;

								one_device_production.pass_no=pass_no;

								one_device_production.dev_id=j;

								one_device_production.production=0;

								multi_device_production.push_back(one_device_production);

							}

						}

						Num_Pass = one_lot.pass_no;

					}

					//end by Zhufeng,May 14, 2012

				}



			}



			p_fpl++;

		}



		//now get next-pass lots from second-pass lots or third, fourth...

		p_fpl=finshied_lot_list.begin();

		while(p_fpl !=finshied_lot_list.end())

		{

			double completion_time=(* p_fpl).from_begin_to_completion;

			if( completion_time>last_time && completion_time<=current_time)

			{

				//if this lot is completed between last_time and current_time

				struct multi_lot one_lot=(* p_fpl);

				int next_logpoint=get_next_pass(one_lot);

				if(next_logpoint>0)

				{

					one_lot.logpoint=next_logpoint;

					one_lot.pass_no++;

					candidate_lot_list.push_back( one_lot);

					whether_new_lot=true;

					//added by Zhufeng, May 14, 2012

					if(one_lot.pass_no > Num_Pass)

					{

						//cout<<"New pass considered,"<<one_lot.pass_no<<endl;

						int diff = one_lot.pass_no - Num_Pass;



						for( int i=1;i<=diff;i++)

						{

							int pass_no=Num_Pass+i;

							for(int j=1;j<=Num_Device;j++)

							{

								struct multi_production one_device_production;

								one_device_production.pass_no=pass_no;

								one_device_production.dev_id=j;

								one_device_production.production=0;

								multi_device_production.push_back(one_device_production);

							}

						}

						Num_Pass = one_lot.pass_no;

					}

					//end by Zhufeng,May 14, 2012

				}



			}



			p_fpl++;

		}

		

	}else

	{

		//cout<<"last_time=0"<<endl;

		while(p_fpl !=first_pass_lot_vector.end())

		{

			double completion_time=(* p_fpl).from_begin_to_completion;

			if( completion_time>=last_time && completion_time<=current_time)

			{

				//if this lot is completed between last_time and current_time

				struct multi_lot one_lot=(* p_fpl);

				int next_logpoint=get_next_pass(one_lot);

				if(next_logpoint>0)

				{

					one_lot.logpoint=next_logpoint;

					one_lot.pass_no++;

					candidate_lot_list.push_back( one_lot);

					whether_new_lot=true;

					//added by Zhufeng, May 14, 2012

					if(one_lot.pass_no > Num_Pass)

					{

						//cout<<"New pass considered,"<<one_lot.pass_no<<endl;

						int diff = one_lot.pass_no - Num_Pass;



						for( int i=1;i<=diff;i++)

						{

							int pass_no=Num_Pass+i;

							for(int j=1;j<=Num_Device;j++)

							{

								struct multi_production one_device_production;

								one_device_production.pass_no=pass_no;

								one_device_production.dev_id=j;

								one_device_production.production=0;

								multi_device_production.push_back(one_device_production);

							}

						}

						Num_Pass = one_lot.pass_no;

					}

					//end by Zhufeng,May 14, 2012

			

				}



			}



			p_fpl++;

		}



		//now get next-pass lots from second-pass lots or third, fourth...

		p_fpl=finshied_lot_list.begin();

		while(p_fpl !=finshied_lot_list.end())

		{

			double completion_time=(* p_fpl).from_begin_to_completion;

			if( completion_time>=last_time && completion_time<=current_time)

			{

				//if this lot is completed between last_time and current_time

				struct multi_lot one_lot=(* p_fpl);

				int next_logpoint=get_next_pass(one_lot);

				if(next_logpoint>0)

				{

					one_lot.logpoint=next_logpoint;

					one_lot.pass_no++;

					candidate_lot_list.push_back( one_lot);

					whether_new_lot=true;

					//added by Zhufeng, May 14, 2012

					if(one_lot.pass_no > Num_Pass)

					{

						//cout<<"New pass considered,"<<one_lot.pass_no<<endl;

						int diff = one_lot.pass_no - Num_Pass;



						for( int i=1;i<=diff;i++)

						{

							int pass_no=Num_Pass+i;

							for(int j=1;j<=Num_Device;j++)

							{

								struct multi_production one_device_production;

								one_device_production.pass_no=pass_no;

								one_device_production.dev_id=j;

								one_device_production.production=0;

								multi_device_production.push_back(one_device_production);

							}

						}

						Num_Pass = one_lot.pass_no;

					}

					//end by Zhufeng,May 14, 2012

				}



			}



			p_fpl++;

		}



	}

	return whether_new_lot;



}





//Feb 23

bool AT::get_candidate_lot_list( vector<multi_lot> & candidate_lot_list, const vector<multi_lot> & first_pass_lot_vector,const vector<multi_lot> & finished_lot_reset_list, double current_time,double last_time,const vector<multi_lot> finished_lot_list, int & Num_Pass)

{

	cout<<"get_candidate_lot_list"<<endl;

	//cout<<"get_candidate_lot_list"<<endl;

	//cout<<"first_pass_lot_vector.size = "<<first_pass_lot_vector.size()<<endl;

	//cout<<"finished_lot_reset_list.size = "<<finished_lot_reset_list.size()<<endl;

	//cout<<"finished_lot_list.size = "<< finished_lot_list.size()<<endl;



	bool whether_new_lot=false;

	vector<multi_lot>::const_iterator p_fpl=first_pass_lot_vector.begin();

	if(last_time >0)

	{

		//when last_time>0, get lots finished >last_ime, when last_time==0, get lots finished >= last_time

		while(p_fpl !=first_pass_lot_vector.end())

		{

			double completion_time=(* p_fpl).from_begin_to_completion;

			if( completion_time>last_time && completion_time<=current_time)

			{

				//if this lot is completed between last_time and current_time

				struct multi_lot one_lot=(* p_fpl);

				int next_logpoint=get_next_pass(one_lot);

				if(next_logpoint>0)

				{

					one_lot.logpoint=next_logpoint;

					one_lot.pass_no++;

					//check whether this lot is already assigned in the finished_lot_list;

					bool whether_assigned=false;

					vector<multi_lot>::const_iterator p_temp=finished_lot_list.begin();



					while (p_temp != finished_lot_list.end())

					{

						if(one_lot.whether_initial_lot)

						{

							if(one_lot.initial_running_lot_id == (* p_temp).initial_running_lot_id &&

							   one_lot.pass_no == (* p_temp).pass_no

								)

							{

								whether_assigned=true;

								break;

							}



						}else

						{

							if(one_lot.lot_id == (* p_temp).lot_id &&

							   one_lot.pass_no == (* p_temp).pass_no

								)

							{

								whether_assigned=true;

								break;

							}



						}//end of if(one_lot.whether_initial_lot) else 

						++p_temp;

					}//end of while (p_temp != finished_lot_list.end())



					if(!whether_assigned)

					{

						candidate_lot_list.push_back( one_lot);

						whether_new_lot=true;

						//added by Zhufeng, May 14, 2012

						if(one_lot.pass_no > Num_Pass)

						{

							cout<<"New pass considered,"<<one_lot.pass_no<<endl;

							int diff = one_lot.pass_no - Num_Pass;



							for( int i=1;i<=diff;i++)

							{

								int pass_no=Num_Pass+i;

								for(int j=1;j<=Num_Device;j++)

								{

									struct multi_production one_device_production;

									one_device_production.pass_no=pass_no;

									one_device_production.dev_id=j;

									one_device_production.production=0;

									multi_device_production.push_back(one_device_production);

								}

							}

							Num_Pass = one_lot.pass_no;

						}

						//end by Zhufeng,May 14, 2012

					}



				}//end of if(next_logpoint>0)



			}



			p_fpl++;

		}



		//now get next-pass lots from finished_lot_reset_list during resetup.Don't need to check whether assigned 

		p_fpl=finished_lot_reset_list.begin();

		while(p_fpl !=finished_lot_reset_list.end())

		{

			double completion_time=(* p_fpl).from_begin_to_completion;

			if( completion_time>last_time && completion_time<=current_time)

			{

				//if this lot is completed between last_time and current_time

				struct multi_lot one_lot=(* p_fpl);

				int next_logpoint=get_next_pass(one_lot);

				if(next_logpoint>0)

				{

					one_lot.logpoint=next_logpoint;

					one_lot.pass_no++;

					candidate_lot_list.push_back( one_lot);

					whether_new_lot=true;

					//Added by Zhufeng, May 14, 2012

					if(one_lot.pass_no > Num_Pass)

					{

						cout<<"New pass considered,"<<one_lot.pass_no<<endl;

						int diff = one_lot.pass_no - Num_Pass;



						for( int i=1;i<=diff;i++)

						{

							int pass_no=Num_Pass+i;

							for(int j=1;j<=Num_Device;j++)

							{

								struct multi_production one_device_production;

								one_device_production.pass_no=pass_no;

								one_device_production.dev_id=j;

								one_device_production.production=0;

								multi_device_production.push_back(one_device_production);

							}

						}

						Num_Pass = one_lot.pass_no;

					}

					//end by Zhufeng,May 14, 2012

					

				}



			}



			++p_fpl;

		}



		//Need to get next-pass lots from finished_lot_list. Need to check whether assigned



		p_fpl=finished_lot_list.begin();

		while(p_fpl !=finished_lot_list.end())

		{

			double completion_time=(* p_fpl).from_begin_to_completion;

			if( completion_time>last_time && completion_time<=current_time)

			{

				//if this lot is completed between last_time and current_time

				struct multi_lot one_lot=(* p_fpl);

				int next_logpoint=get_next_pass(one_lot);

				if(next_logpoint>0)

				{

					one_lot.logpoint=next_logpoint;

					one_lot.pass_no++;

					//check whether this lot is already assigned in the finished_lot_list;

					bool whether_assigned=false;

					vector<multi_lot>::const_iterator p_temp=finished_lot_list.begin();



					while (p_temp != finished_lot_list.end())

					{

						if(one_lot.whether_initial_lot)

						{

							if(one_lot.initial_running_lot_id == (* p_temp).initial_running_lot_id &&

							   one_lot.pass_no == (* p_temp).pass_no

								)

							{

								whether_assigned=true;

								break;

							}



						}else

						{

							if(one_lot.lot_id == (* p_temp).lot_id &&

							   one_lot.pass_no == (* p_temp).pass_no

								)

							{

								whether_assigned=true;

								break;

							}



						}//end of if(one_lot.whether_initial_lot) else 

						++p_temp;

					}//end of while (p_temp != finished_lot_list.end())



					if(!whether_assigned)

					{

						candidate_lot_list.push_back( one_lot);

						whether_new_lot=true;

						//added by Zhufeng, May 14, 2012

						if(one_lot.pass_no > Num_Pass)

						{

							cout<<"New pass considered,"<<one_lot.pass_no<<endl;

							int diff = one_lot.pass_no - Num_Pass;



							for( int i=1;i<=diff;i++)

							{

								int pass_no=Num_Pass+i;

								for(int j=1;j<=Num_Device;j++)

								{

									struct multi_production one_device_production;

									one_device_production.pass_no=pass_no;

									one_device_production.dev_id=j;

									one_device_production.production=0;

									multi_device_production.push_back(one_device_production);

								}

							}

							Num_Pass = one_lot.pass_no;

						}

						//end by Zhufeng,May 14, 2012

					}



				}



			}



			++p_fpl;

		}



		

	}else

	{

		cout<<"last_time=0"<<endl;

		while(p_fpl !=first_pass_lot_vector.end())

		{

			double completion_time=(* p_fpl).from_begin_to_completion;

			if( completion_time>=last_time && completion_time<=current_time)

			{

				//if this lot is completed between last_time and current_time

				struct multi_lot one_lot=(* p_fpl);

				int next_logpoint=get_next_pass(one_lot);

				if(next_logpoint>0)

				{

					one_lot.logpoint=next_logpoint;

					one_lot.pass_no++;

					//check whether this lot is already assigned in the finished_lot_list;

					bool whether_assigned=false;

					vector<multi_lot>::const_iterator p_temp=finished_lot_list.begin();



					while (p_temp != finished_lot_list.end())

					{

						if(one_lot.whether_initial_lot)

						{

							if(one_lot.initial_running_lot_id == (* p_temp).initial_running_lot_id &&

							   one_lot.pass_no == (* p_temp).pass_no

								)

							{

								whether_assigned=true;

								break;

							}



						}else

						{

							if(one_lot.lot_id == (* p_temp).lot_id &&

							   one_lot.pass_no == (* p_temp).pass_no

								)

							{

								whether_assigned=true;

								break;

							}



						}//end of if(one_lot.whether_initial_lot) else 

						++p_temp;

					}//end of while (p_temp != finished_lot_list.end())



					if(!whether_assigned)

					{



						candidate_lot_list.push_back( one_lot);

						whether_new_lot=true;

						//added by Zhufeng, May 14, 2012

						if(one_lot.pass_no > Num_Pass)

						{

							cout<<"New pass considered,"<<one_lot.pass_no<<endl;

							int diff = one_lot.pass_no - Num_Pass;



							for( int i=1;i<=diff;i++)

							{

								int pass_no=Num_Pass+i;

								for(int j=1;j<=Num_Device;j++)

								{

									struct multi_production one_device_production;

									one_device_production.pass_no=pass_no;

									one_device_production.dev_id=j;

									one_device_production.production=0;

									multi_device_production.push_back(one_device_production);

								}

							}

							Num_Pass = one_lot.pass_no;

						}

						//end by Zhufeng,May 14, 2012

					}

			

				}



			}



			p_fpl++;

		}



		//now get next-pass lots from second-pass lots or third, fourth...Dont need to check whether assigned

		p_fpl=finished_lot_reset_list.begin();

		while(p_fpl !=finished_lot_reset_list.end())

		{

			double completion_time=(* p_fpl).from_begin_to_completion;

			if( completion_time>=last_time && completion_time<=current_time)

			{

				//if this lot is completed between last_time and current_time

				struct multi_lot one_lot=(* p_fpl);

				int next_logpoint=get_next_pass(one_lot);

				if(next_logpoint>0)

				{

					one_lot.logpoint=next_logpoint;

					one_lot.pass_no++;

					candidate_lot_list.push_back( one_lot);

					whether_new_lot=true;

					//added by Zhufeng, May 14, 2012

					if(one_lot.pass_no > Num_Pass)

					{

						cout<<"New pass considered,"<<one_lot.pass_no<<endl;

						int diff = one_lot.pass_no - Num_Pass;



						for( int i=1;i<=diff;i++)

						{

							int pass_no=Num_Pass+i;

							for(int j=1;j<=Num_Device;j++)

							{

								struct multi_production one_device_production;

								one_device_production.pass_no=pass_no;

								one_device_production.dev_id=j;

								one_device_production.production=0;

								multi_device_production.push_back(one_device_production);

							}

						}

						Num_Pass = one_lot.pass_no;

					}

					//end by Zhufeng,May 14, 2012

				}



			}



			p_fpl++;

		}



		p_fpl=finished_lot_list.begin();

		while(p_fpl !=finished_lot_list.end())

		{

			double completion_time=(* p_fpl).from_begin_to_completion;

			if( completion_time>last_time && completion_time<=current_time)

			{

				//if this lot is completed between last_time and current_time

				struct multi_lot one_lot=(* p_fpl);

				int next_logpoint=get_next_pass(one_lot);

				if(next_logpoint>0)

				{

					one_lot.logpoint=next_logpoint;

					one_lot.pass_no++;

					//check whether this lot is already assigned in the finished_lot_list;

					bool whether_assigned=false;

					vector<multi_lot>::const_iterator p_temp=finished_lot_list.begin();



					while (p_temp != finished_lot_list.end())

					{

						if(one_lot.whether_initial_lot)

						{

							if(one_lot.initial_running_lot_id == (* p_temp).initial_running_lot_id &&

							   one_lot.pass_no == (* p_temp).pass_no

								)

							{

								whether_assigned=true;

								break;

							}



						}else

						{

							if(one_lot.lot_id == (* p_temp).lot_id &&

							   one_lot.pass_no == (* p_temp).pass_no

								)

							{

								whether_assigned=true;

								break;

							}



						}//end of if(one_lot.whether_initial_lot) else 

						++p_temp;

					}//end of while (p_temp != finished_lot_list.end())



					if(!whether_assigned)

					{



						candidate_lot_list.push_back( one_lot);

						whether_new_lot=true;

						//added by Zhufeng, May 14, 2012

						if(one_lot.pass_no > Num_Pass)

						{

							cout<<"New pass considered,"<<one_lot.pass_no<<endl;

							int diff = one_lot.pass_no - Num_Pass;



							for( int i=1;i<=diff;i++)

							{

								int pass_no=Num_Pass+i;

								for(int j=1;j<=Num_Device;j++)

								{

									struct multi_production one_device_production;

									one_device_production.pass_no=pass_no;

									one_device_production.dev_id=j;

									one_device_production.production=0;

									multi_device_production.push_back(one_device_production);

								}

							}

							Num_Pass = one_lot.pass_no;

						}

						//end by Zhufeng,May 14, 2012

					}

					

				}



			}



			++p_fpl;



		}

	}//end of if(last_time >0) else

	return whether_new_lot;



}





//Feb 23

void AT::show_candidate_lot_list( vector<multi_lot> & candidate_lot_list, double last_time, double current_time)

{

	ofstream f_out("Debug/candidate_lot_list.txt");

	f_out<<"last_time="<<last_time<<",current_time="<<current_time<<endl;

	

	vector<multi_lot>::const_iterator p_c=candidate_lot_list.begin();

	while( p_c !=candidate_lot_list.end())

	{

		if((* p_c).whether_initial_lot)

		

		{

			f_out<<"Initial lot "<<initial_running_lot[(* p_c).initial_running_lot_id-1].lot_name;

			//cout<<"Initial lot "<<initial_running_lot[(* p_c).initial_running_lot_id-1].lot_name;

		}

		else

		{

			f_out<<"lot "<<LOT[(* p_c).lot_id-1];

			//cout<<"lot "<<LOT[(* p_c).lot_id-1];



		}

		f_out<<", logpoint="<<(* p_c).logpoint<<",pass_no="<<(* p_c).pass_no<<",from_begin_to_completion="<<(* p_c).from_begin_to_completion<<",weight="<<(* p_c).weight<<endl;

		

		//cout<<", logpoint="<<(* p_c).logpoint

			//<<",benefit="<<(* p_c).benefit<<",weight="<<(* p_c).weight<<endl;

			//<<",pass_no="<<(* p_c).pass_no<<",from_begin_to_completion="<<(* p_c).from_begin_to_completion<<endl;

			//<<",from_begin_to_completion="<<(* p_c).from_begin_to_completion<<",weight="<<(* p_c).weight<<endl;

		p_c++;

	



	}



}



void AT::show_candidate_lot_list( vector<multi_lot> & candidate_lot_list)

{

	//ofstream f_out("Debug/candidate_lot_list.txt");

	//f_out<<"last_time="<<last_time<<",current_time="<<current_time<<endl;

	

	vector<multi_lot>::const_iterator p_c=candidate_lot_list.begin();

	while( p_c !=candidate_lot_list.end())

	{

		if((* p_c).whether_initial_lot)

		

		{

			//f_out<<"Initial lot "<<initial_running_lot[(* p_c).initial_running_lot_id-1].lot_name;

			//cout<<"Initial lot "<<initial_running_lot[(* p_c).initial_running_lot_id-1].lot_name;

		}

		else

		{

			//f_out<<"lot "<<LOT[(* p_c).lot_id-1];

			//cout<<"lot "<<LOT[(* p_c).lot_id-1];



		}

		//f_out<<", logpoint="<<(* p_c).logpoint<<",pass_no="<<(* p_c).pass_no<<",from_begin_to_completion="<<(* p_c).from_begin_to_completion<<",weight="<<(* p_c).weight<<endl;

		

		//cout<<", logpoint="<<(* p_c).logpoint

			//<<",pass_no="<<(* p_c).pass_no<<",from_begin_to_completion="<<(* p_c).from_begin_to_completion

			//<<",benefit="<<(* p_c).benefit<<",weight="<<(* p_c).weight<<endl;

			//<<",from_begin_to_completion="<<(* p_c).from_begin_to_completion<<",weight="<<(* p_c).weight<<endl;

		p_c++;

	



	}



}





void AT::assign_lot( vector<multi_mach> & machine_list,

					vector<multi_lot> & candidate_lot_list,

					vector<multi_lot> & finished_lot_list, 

					double current_time,

					 const vector<tool_setup> tool_setup_list,

					 const vector<mf_tool_setup> & mf_tool_setup_list,

					 vector<tf_cert_ti> & tf_cert_ti_list

					 )

{

	cout<<"try to assign new lots to all machines free earlier than current time:"<<current_time<<endl;

	vector<multi_mach>::iterator p_m=machine_list.begin();

	vector<multi_lot>::iterator p_l=candidate_lot_list.begin();

	while ( p_m != machine_list.end())

	{

		if( (*p_m).from_begin_to_completion <= current_time)

		{

			//Added by Zhufeng, for randomness in assigning lots.

			vector<multi_lot> candi_lot_list_this_machine;

			p_l=candidate_lot_list.begin();

			bool whether_mach_assign=false;

			if((*p_m).whether_setup) // This machine has been setup

			{

				int mf_id=(* p_m).mf_id;

				//cout<<"process machine:"<<MI[(* p_m).mi_id-1]<<", at "<<current_time<<endl;

				vector<int> tf_id_vector=(* p_m).tf_id_vector;

				int certification=(* p_m).certification;

				while( p_l !=candidate_lot_list.end())

				{

					int logpoint=(* p_l).logpoint;

					int dev_id=0;

					if((* p_l).whether_initial_lot)

					{

						dev_id=initial_running_lot[(* p_l).initial_running_lot_id-1].dev_id;

					}else

					{

						dev_id=lots_device[(* p_l).lot_id];

					}

					

					bool whether_route=false;

					//bool whether_lot_assigned=false;



					int s_id=0;

					for(int id=1;id<=(int)S.size();id++)

					{



						if(		 S[id-1].tf.tools	    == tf_id_vector

							&&  (S[id-1].logpoint		== logpoint)



							&&	(S[id-1].mg				== mf_id)

							&&	(S[id-1].device			== dev_id)

							&&	(S[id-1].temperature	== certification)	)

						{

							

							s_id=id;

							(* p_l).s_id=s_id;

							whether_route=true;

							break;

						}

					}//end of for(int id=1;id<=(int)S.size();id++)



					if(whether_route)

					{		//now check whether the time on the machine is enough

						double required_time=(double)(* p_l).quantity/S[s_id-1].PPH+Load_Unload_Time;

						double remain_time_mach=(*p_m).horizon-(*p_m).from_begin_to_completion;

						if(remain_time_mach > (*p_m).horizon - (*p_l).from_begin_to_completion )

						{

							remain_time_mach = (*p_m).horizon - (*p_l).from_begin_to_completion;

						}

						//Comment by Zhufeng, Feb 2014

						//if(remain_time_mach >= required_time)

						//end by Zhufeng, Feb 2014

						//Added by Zhufeng Feb 2014, for the last lot beyond the horizon

						if(remain_time_mach >= required_time || beyond_horizon ==1)

							//end by Zhufeng, Feb 2014

						{

							candi_lot_list_this_machine.push_back(* p_l);

						}

					}//end of if(whether_route)

					++p_l;

				}//end of while( p_l !=candidate_lot_list.end())

			}//end of if((*p_m).whether_setup) 



			vector<multi_lot> lot_assigned_temp;

			vector<multi_lot> lot_assigned_best;

			vector<multi_lot> candi_lot_temp=candi_lot_list_this_machine;

			//Added by Zhufeng, April 28

			vector<multi_production> multi_device_production_temp=multi_device_production;

			int Num_Pass_Temp = Num_Pass;

			//end by Zhufeng, April 28



			//Added by Zhufeng, May 23, 2012

			vector<multi_production_step> multi_device_production_step_temp=multi_device_production_step;		

			//end by Zhufeng, May 23, 2012

			struct multi_mach this_mach_temp=(* p_m);

			double benefit_best=0;

			double benefit_temp=0;



			//This is derived from GRASP for 10 times to choose the  best. Change times_grasp to 1, then it is just a random lot-assighment. 

			//Each time randomly choose one from top 5.

			int times_grasp=1;

			int top_number=5;

			for(int i=1;i<=times_grasp;i++)

			{

				//cout<<"grasp iteration:"<<i<<endl;



				//Added by Zhufeng, Feb 14, 2014

				bool already_beyond_horizon = false;

				//End by Zhufeng, Feb 14, 2014



				while(!candi_lot_temp.empty())

				{

					int position;

					if(candi_lot_temp.size()>= top_number)

					{

						position = rand()% top_number;

					}else

					{

						position=rand() % candi_lot_temp.size();

					}



					cout<<"position chosen = "<<position<<endl;



					p_l=candi_lot_temp.begin()+position;

					double required_time=(double)(* p_l).quantity/S[(* p_l).s_id-1].PPH+Load_Unload_Time;

					double remain_time_mach=(this_mach_temp).horizon-(this_mach_temp).from_begin_to_completion;

					if(remain_time_mach > (this_mach_temp).horizon - (*p_l).from_begin_to_completion )

					{

						remain_time_mach = this_mach_temp.horizon - (*p_l).from_begin_to_completion;

					}

					if(remain_time_mach >= required_time

					//Added by Zhufeng, Feb 2014, for the last lot beyond the horizon

					||(!already_beyond_horizon &&beyond_horizon ==1)

					//end by Zhufeng, Feb 2014

					)

					{

						lot_assigned_temp.push_back(* p_l);

						benefit_temp+= (* p_l).benefit;

						cout<<"attach lot";

						if((* p_l).whether_initial_lot)

								cout<<initial_running_lot[(* p_l).initial_running_lot_id-1].lot_name;

						else

								cout<<LOT[(* p_l).lot_id-1];

						cout<<" to mach"<<MI[(* p_m).mi_id-1]<<endl;



						//Added by Zhufeng, April 28,2012

						update_multi_device_production(multi_device_production_temp, (*p_l), Num_Pass_Temp);						

						//end by Zhufeng, Apiril 28, 2012



						//Added by Zhufeng, May 23, 2012

						update_multi_device_production_step(multi_device_production_step_temp, (*p_l));

						//end by Zhufeng, May 23, 2012





						if((*p_l).from_begin_to_completion < this_mach_temp.from_begin_to_completion)

						{

							(* p_l).start_time=this_mach_temp.from_begin_to_completion;

							(*p_l).from_begin_to_completion=this_mach_temp.from_begin_to_completion+required_time;

						}else

						{

							(* p_l).start_time=(* p_l).from_begin_to_completion;

							(*p_l).from_begin_to_completion=(* p_l).from_begin_to_completion+required_time;

						}

						this_mach_temp.from_begin_to_completion=(*p_l).from_begin_to_completion;



						//Added by Zhufeng,Feb 2014, for the last lot beyond the horizon

						if(this_mach_temp.from_begin_to_completion>H[(* p_m).mi_id])

						{

							already_beyond_horizon = true;

						}

						//end by Zhufeng, Feb 2014

					}



					candi_lot_temp.erase(candi_lot_temp.begin()+position);

					

					//Added by Zhufeng, April 28,2012

					//update_lot_benefit( candi_lot_temp,multi_device_production_temp);

					//show_candidate_lot_list( candi_lot_temp);



					//Added by Zhufeng, May 23,2012

					update_lot_benefit( candi_lot_temp,multi_device_production_step_temp);



					//end by Zhufeng, May 23, 2012



				}//end of while(!candi_lot_temp.empty())

				if(benefit_temp > benefit_best)

				{

					//cout<<"best found"<<endl;

					benefit_best=benefit_temp;

					lot_assigned_best=lot_assigned_temp;

				}

				benefit_temp=0;

				this_mach_temp=(* p_m);

				lot_assigned_temp.clear();

				candi_lot_temp=candi_lot_list_this_machine;

				//Added by Zhufeng, April 28, 2012

				multi_device_production_temp=multi_device_production;

				Num_Pass_Temp=Num_Pass;

				//end by Zhufeng, April 28, 2012

				//May 23

				multi_device_production_step_temp = multi_device_production_step;

				//May 23



			}//end of for(int i=1;i<=times_grasp;i++) GRASP end

	



			//Now based on the result of GRASP, operate on candidate_lot_list



			if((*p_m).whether_setup) // This machine has been setup

			{

				int mf_id=(* p_m).mf_id;

				//cout<<"After GRASP, finally process machine:"<<MI[(* p_m).mi_id-1]<<", at "<<current_time<<endl;

				vector<int> tf_id_vector=(* p_m).tf_id_vector;

				int certification=(* p_m).certification;



				for(int i=1;i<=(int)lot_assigned_best.size();i++)

				{

					p_l=candidate_lot_list.begin();

					int lot_id_best=lot_assigned_best[i-1].lot_id;

					int logpoint_best=lot_assigned_best[i-1].logpoint;

					int pass_no_best=lot_assigned_best[i-1].pass_no;

					

					//Added by Zhufeng, May 14, 2012

					int	initial_running_lot_id_best=lot_assigned_best[i-1].initial_running_lot_id;

					//Ended by Zhufeng, May 14, 2012

					int s_id=lot_assigned_best[i-1].s_id;

					while( p_l !=candidate_lot_list.end())

					{

						if( 

							(*p_l).lot_id == lot_id_best &&

							(*p_l).logpoint == logpoint_best &&

							(*p_l).pass_no == pass_no_best &&

							//Added by Zhufeng, May 14, 2012

							(*p_l).initial_running_lot_id == initial_running_lot_id_best

							//Ended by Zhufeng, May 14, 2012

						   )

						{

							(* p_l).s_id=s_id;

							double required_time=(double)(* p_l).quantity/S[s_id-1].PPH+Load_Unload_Time;

							double remain_time_mach=(*p_m).horizon-(*p_m).from_begin_to_completion;



							if( remain_time_mach > (*p_m).horizon - (*p_l).from_begin_to_completion )

							{

								remain_time_mach = (*p_m).horizon - (*p_l).from_begin_to_completion;

							}



							if(remain_time_mach >= required_time

								//Added by Zhufeng, for the last lot beyond the horizon, Feb 2014

								||beyond_horizon ==1

								//end by Zhufeng,Feb 2014

								)

							{



								if((*p_l).from_begin_to_completion < (* p_m).from_begin_to_completion)

								{

									(* p_l).start_time=(* p_m).from_begin_to_completion;

									(*p_l).from_begin_to_completion=(* p_m).from_begin_to_completion+required_time;

								}else

								{

									(* p_l).start_time=(* p_l).from_begin_to_completion;

									(*p_l).from_begin_to_completion=(* p_l).from_begin_to_completion+required_time;

								}



								whether_mach_assign=true;



								cout<<"attach lot";

								if((* p_l).whether_initial_lot)

									cout<<initial_running_lot[(* p_l).initial_running_lot_id-1].lot_name;

								else

									cout<<LOT[(* p_l).lot_id-1];

								//cout<<",benefit = "<<(* p_l).benefit<<",pass_no = "<<(* p_l).pass_no<<", to mach"<<MI[(* p_m).mi_id-1]<<endl;



								(* p_m).assigned_lot_list.push_back((*p_l));

	

								(* p_m).from_begin_to_completion=(*p_l).from_begin_to_completion;



								finished_lot_list.push_back((*p_l));



								update_multi_device_production(multi_device_production, (*p_l),Num_Pass);



								//Added by Zhufeng, May 23, 2012

								update_multi_device_production_step(multi_device_production_step, (*p_l));

								//end by Zhufeng, May 23, 2012

							

								p_l=candidate_lot_list.erase( p_l );



								//Modified by Zhufeng, April 29, 2012

								//update_lot_benefit( candidate_lot_list, multi_device_production,p_l);

								//update_lot_benefit( candidate_lot_list, multi_device_production);

								//end by Zhufeng, April 29, 2012

								

								//May 23, 2012

								update_lot_benefit( candidate_lot_list, multi_device_production_step);

								//May 23, 2012



							}//end of if(remain_time_mach >= required_time)



							break;

						}



						p_l++;

					}//end of while( p_l !=candidate_lot_list.end())



				}//end of for(int i=1;i<=(int)lot_assigned_best.size();i++)

			}//end of if((*p_m).whether_setup)

			(*p_m).whether_need_check_for_new_lot=whether_mach_assign;

			}//end of if( (*p_m).from_begin_to_completion <= current_time)



			//end by Zhufeng, for randomness in assigning lots.April 20,2012



		p_m++;

	}

}



void AT::assign_lot(vector<multi_mach> & machine_list,vector<multi_lot> & candidate_lot_list,vector<multi_lot> & finished_lot_list,double current_time, vector<multi_mach> & min_candidate_list,

					const vector<tool_setup> tool_setup_list,

					const vector<mf_tool_setup> & mf_tool_setup_list,

					vector<tf_cert_ti> & tf_cert_ti_list

					)

{

	cout<<"try to assign new lot on current mahcines at"<<current_time<<endl;

	for(int id=1;(int)id<=min_candidate_list.size();id++)

	{

		int mi_id=min_candidate_list[id-1].mi_id;

		vector<multi_mach>::iterator p_m=machine_list.begin()+mi_id-1;

		vector<multi_lot>::iterator p_l=candidate_lot_list.begin();

		

			if( (*p_m).from_begin_to_completion <= current_time)

			{

				//Added by Zhufeng, for randomness in assigning lots, April 20, 2012.

			vector<multi_lot> candi_lot_list_this_machine;

			p_l=candidate_lot_list.begin();

			bool whether_mach_assign=false;

			if((*p_m).whether_setup) // This machine has been setup

			{

				int mf_id=(* p_m).mf_id;

				//cout<<"process machine:"<<MI[(* p_m).mi_id-1]<<", at "<<current_time<<endl;

				vector<int> tf_id_vector=(* p_m).tf_id_vector;

				int certification=(* p_m).certification;

				while( p_l !=candidate_lot_list.end())

				{

					int logpoint=(* p_l).logpoint;

					int dev_id=0;

					if((* p_l).whether_initial_lot)

					{

						dev_id=initial_running_lot[(* p_l).initial_running_lot_id-1].dev_id;

					}else

					{

						dev_id=lots_device[(* p_l).lot_id];

					}

					

					bool whether_route=false;

					//bool whether_lot_assigned=false;



					int s_id=0;

					for(int id=1;id<=(int)S.size();id++)

					{



						if(		 S[id-1].tf.tools	    == tf_id_vector

							&&  (S[id-1].logpoint		== logpoint)



							&&	(S[id-1].mg				== mf_id)

							&&	(S[id-1].device			== dev_id)

							&&	(S[id-1].temperature	== certification)	)

						{

							

							s_id=id;

							(* p_l).s_id=s_id;

							whether_route=true;

							break;

						}

					}//end of for(int id=1;id<=(int)S.size();id++)



					if(whether_route)

					{		//now check whether the time on the machine is enough

						double required_time=(double)(* p_l).quantity/S[s_id-1].PPH+Load_Unload_Time;

						double remain_time_mach=(*p_m).horizon-(*p_m).from_begin_to_completion;

						if(remain_time_mach > (*p_m).horizon - (*p_l).from_begin_to_completion )

						{

							remain_time_mach = (*p_m).horizon - (*p_l).from_begin_to_completion;

						}

						if(remain_time_mach >= required_time

						//Added by Zhufeng, for the last lot beyond the horizon, Feb 2014

						||beyond_horizon ==1

						//end by Zhufeng, Feb ,2014

							)

						{

							candi_lot_list_this_machine.push_back(* p_l);

						}

					}//end of if(whether_route)

					++p_l;

				}//end of while( p_l !=candidate_lot_list.end())

			}//end of if((*p_m).whether_setup) 



			vector<multi_lot> lot_assigned_temp;

			vector<multi_lot> lot_assigned_best;

			vector<multi_lot> candi_lot_temp=candi_lot_list_this_machine;

			struct multi_mach this_mach_temp=(* p_m);



			//Added by Zhufeng, April 28

			vector<multi_production> multi_device_production_temp=multi_device_production;

			int Num_Pass_Temp = Num_Pass;

			//end by Zhufeng, April 28



			//Added by Zhufeng, May23

			vector<multi_production_step> multi_device_production_step_temp=multi_device_production_step;

			//end by May 23



			double benefit_best=0;

			double benefit_temp=0;



			//GRASP for 10 times to choose the  best. Each time randomly choose one from top 5.

			//May 19, change the times_grasp from 10 to 1. Then this is just random lot assigment.

			int times_grasp=1;

			int top_number=5;

			for(int i=1;i<=times_grasp;i++)

			{

				//Added by Zhufeng, Feb 14, 2014

				bool already_beyond_horizon = false;

				//end by Zhufeng, Feb 14, 2014



				while(!candi_lot_temp.empty())

				{

					int position;

					if(candi_lot_temp.size()>= top_number)

					{

						position = rand()% top_number;

					}else

					{

						position=rand() % candi_lot_temp.size();

					}



					p_l=candi_lot_temp.begin()+position;

					double required_time=(double)(* p_l).quantity/S[(* p_l).s_id-1].PPH+Load_Unload_Time;

					double remain_time_mach=(this_mach_temp).horizon-(this_mach_temp).from_begin_to_completion;

					if(remain_time_mach > (this_mach_temp).horizon - (*p_l).from_begin_to_completion )

					{

						remain_time_mach = this_mach_temp.horizon - (*p_l).from_begin_to_completion;

					}

					if(remain_time_mach >= required_time

						//Added by Zhufeng for the last lot beyond the horizon, Feb 2014

						||(!already_beyond_horizon && beyond_horizon ==1)

						//end by Zhufeng, Feb 2014

						)

					{

						lot_assigned_temp.push_back(* p_l);

						benefit_temp+= (* p_l).benefit;

						//Added by Zhufeng, April 28,2012

						update_multi_device_production(multi_device_production_temp, (*p_l), Num_Pass_Temp);						

						//end by Zhufeng, Apiril 28, 2012



						//Added by Zhufeng, May 23, 2012

						update_multi_device_production_step(multi_device_production_step_temp, (*p_l));

						//end by Zhufeng, May 23, 2012



						if((*p_l).from_begin_to_completion < this_mach_temp.from_begin_to_completion)

						{

							(* p_l).start_time=this_mach_temp.from_begin_to_completion;

							(*p_l).from_begin_to_completion=this_mach_temp.from_begin_to_completion+required_time;

						}else

						{

							(* p_l).start_time=(* p_l).from_begin_to_completion;

							(*p_l).from_begin_to_completion=(* p_l).from_begin_to_completion+required_time;

						}

						this_mach_temp.from_begin_to_completion=(*p_l).from_begin_to_completion;



						//Added by Zhufeng for the last lot beyond the horizon, Feb 2014

						if(this_mach_temp.from_begin_to_completion>H[this_mach_temp.mi_id])

						{

							already_beyond_horizon = true;

						}

						//end by Zhufeng, Feb 2014

					}



					candi_lot_temp.erase(candi_lot_temp.begin()+position);

					//Added by Zhufeng, April 28,2012;May 13, 2012

					//update_lot_benefit( candi_lot_temp,multi_device_production_temp);

					//end by Zhufeng, Apiril 28, 2012;May 13, 2012



					//Added by Zhufeng, May 23,2012

					update_lot_benefit( candi_lot_temp,multi_device_production_step_temp);

					//end by Zhufeng, May 23, 2012



				}//end of while(!candi_lot_temp.empty())

				if(benefit_temp > benefit_best)

				{

					benefit_best=benefit_temp;

					lot_assigned_best=lot_assigned_temp;

				}



				benefit_temp=0;

				this_mach_temp=(* p_m);

				lot_assigned_temp.clear();

				candi_lot_temp=candi_lot_list_this_machine;

				//Added by Zhufeng, April 28, 2012

				multi_device_production_temp=multi_device_production;

				Num_Pass_Temp=Num_Pass;

				//end by Zhufeng, April 28, 2012



				//added by Zhufeng, May 23, 2012

				multi_device_production_step_temp = multi_device_production_step;

				//end by Zhufeng, May 23, 2012

				



			}//end of for(int i=1;i<=times_grasp;i++) GRASP end

			//Now based on the result of GRASP, operate on candidate_lot_list



			if((*p_m).whether_setup) // This machine has been setup

			{

				int mf_id=(* p_m).mf_id;

				cout<<"process machine:"<<MI[(* p_m).mi_id-1]<<", at "<<current_time<<endl;

				vector<int> tf_id_vector=(* p_m).tf_id_vector;

				int certification=(* p_m).certification;



				for(int i=1;i<=(int)lot_assigned_best.size();i++)

				{

					p_l=candidate_lot_list.begin();

					int lot_id_best=lot_assigned_best[i-1].lot_id;

					int logpoint_best=lot_assigned_best[i-1].logpoint;

					int pass_no=lot_assigned_best[i-1].pass_no;

					//Added by Zhufeng, May 14, 2012

					int	initial_running_lot_id_best=lot_assigned_best[i-1].initial_running_lot_id;

					//Ended by Zhufeng, May 14, 2012

					int s_id=lot_assigned_best[i-1].s_id;

					while( p_l !=candidate_lot_list.end())

					{

						if( 

							(*p_l).lot_id == lot_id_best &&

							(*p_l).logpoint == logpoint_best &&

							(*p_l).pass_no == pass_no &&

							//Added by Zhufeng, May 14, 2012

							(*p_l).initial_running_lot_id == initial_running_lot_id_best

							//Ended by Zhufeng, May 14, 2012 

						   )

						{

							(* p_l).s_id=s_id;

							double required_time=(double)(* p_l).quantity/S[s_id-1].PPH+Load_Unload_Time;

							double remain_time_mach=(*p_m).horizon-(*p_m).from_begin_to_completion;



							if( remain_time_mach > (*p_m).horizon - (*p_l).from_begin_to_completion )

							{

								remain_time_mach = (*p_m).horizon - (*p_l).from_begin_to_completion;

							}



							if(remain_time_mach >= required_time

								//Added by Zhufeng for the last lot beyond the horizon, Feb 2014

								||beyond_horizon ==1

								//end by Zhufeng, Feb 2014

								)

							{



								if((*p_l).from_begin_to_completion < (* p_m).from_begin_to_completion)

								{

									(* p_l).start_time=(* p_m).from_begin_to_completion;

									(*p_l).from_begin_to_completion=(* p_m).from_begin_to_completion+required_time;

								}else

								{

									(* p_l).start_time=(* p_l).from_begin_to_completion;

									(*p_l).from_begin_to_completion=(* p_l).from_begin_to_completion+required_time;

								}



								whether_mach_assign=true;



								(* p_m).assigned_lot_list.push_back((*p_l));

	

								(* p_m).from_begin_to_completion=(*p_l).from_begin_to_completion;



								finished_lot_list.push_back((*p_l));



								update_multi_device_production(multi_device_production, (*p_l),Num_Pass);



								//Added by Zhufeng, May 23, 2012

								update_multi_device_production_step(multi_device_production_step, (*p_l));

								//end by Zhufeng, May 23, 2012



							

								p_l=candidate_lot_list.erase( p_l );



								//Modified by Zhufeng, April 29, 2012

								//update_lot_benefit( candidate_lot_list, multi_device_production,p_l);

								//update_lot_benefit( candidate_lot_list, multi_device_production);

								//end by Zhufeng, April 29, 2012



									//Modified by Zhufeng, May 23, 2012

								update_lot_benefit( candidate_lot_list, multi_device_production_step);

								//end by Zhufeng, May 23, 2012



							}//end of if(remain_time_mach >= required_time)



							break;

						}



						p_l++;

					}//end of while( p_l !=candidate_lot_list.end())



				}//end of for(int i=1;i<=(int)lot_assigned_best.size();i++)

			}//end of if((*p_m).whether_setup)

			(*p_m).whether_need_check_for_new_lot=whether_mach_assign;

			}//end of if( (*p_m).from_begin_to_completion <= current_time)



			//end by Zhufeng, for randomness in assigning lots.April 20,2012

			

	}//end of for(int id=1;(int)id<=min_candidate_list.size();id++)

}



bool AT::compute_benefit_resetup_empty( const struct multi_mach & one_mach,

						 vector<multi_lot> & candidate_lot_list,

						 vector<multi_lot> & finished_lot_list,

						 const double current_time,

						 double & best_ts_weight,

						 int & best_tool_setup_id,

						 vector<multi_lot> & assigned_lot_list_best_ts,

						 //vector<double> & from_begin_to_completion_list_best_ts,

						 const vector<tool_setup> tool_setup_list,

						 const vector<mf_tool_setup> mf_tool_setup_list,

						 const vector<tf_cert_ti> & tf_cert_ti_list,

						 double & best_from_begin_to_completion,

						 double & best_from_begin_to_setup,

						 vector<multi_production> & multi_device_production,

						 int & Num_Pass,

						 vector<multi_production_step> & multi_device_production_step

						 )

{

	//based on one_mach, compute the benefit for reset machines that are empty. Find the best_tool_setup_id. Update from_begin_to_setup, from_begin_to_completion, candidate_lot_list,finished_lot_list, multi_device_production, and Num_Pass.  



	//cout<<"compute benefit for resetup"<<MI[one_mach.mi_id-1]<<endl;

	bool whether_resetup=false;

	int mf_id=one_mach.mf_id;

	 best_tool_setup_id=-1;

	 best_ts_weight=0;

	//double ts_weight_best_inner=0;

	//double from_begin_to_completion_best_inner=0;

	double time_remain_mach=one_mach.horizon-one_mach.from_begin_to_completion;



	vector<multi_lot> candidate_lot_list_initial=candidate_lot_list;



	vector<multi_lot> finished_lot_list_initial=finished_lot_list;



	//March 25, for mulit-pass production



	vector<multi_production> multi_device_production_initial=multi_device_production;

	int Num_Pass_initial=Num_Pass;



	//March 25

	//May 23

	vector<multi_production_step> multi_device_production_step_initial=multi_device_production_step;

	//May 23







	for(int ts_id=1;ts_id<=(int)tool_setup_list.size();ts_id++)

	{

		double ts_weight=0;



		double from_begin_to_completion_temp=one_mach.from_begin_to_completion;



		double from_begin_to_setup_temp=one_mach.from_begin_to_setup;



		double time_remain_mach_temp=time_remain_mach;

		//cout<<"time_remain_mach="<<time_remain_mach_temp<<endl;



		//Added by Zhufeng, for resetup time, June 10, 2012

		double tool_setup_time = 0;

		if(one_mach.tf_id_vector != tool_setup_list[ts_id-1].tf_id_vector ||

			one_mach.certification != tool_setup_list[ts_id-1].certification)

		{

			for(int i=1;i<= tool_setup_list[ts_id-1].tf_id_vector.size();i++)

			{

				tool_setup_time+=Tooling_Setup_Time[tool_setup_list[ts_id-1].tf_id_vector[i-1]];

			}

			//cout <<"tool_setup_time = "<<tool_setup_time<<endl;		

		}

		time_remain_mach_temp = time_remain_mach_temp - tool_setup_time;

		//end by Zhufeng,June 10,2012 



		vector<multi_lot> assigned_lot_list;

		//vector<double> from_begin_to_completion_list;

		vector<multi_lot> candidate_lot_list_temp=candidate_lot_list_initial;

		vector<multi_lot> finished_lot_list_temp=finished_lot_list_initial;



		//March 25, for mulit-pass production

		vector<multi_production> multi_device_production_temp=multi_device_production_initial;

		int Num_Pass_temp=Num_Pass_initial;

		//March 25



		//May 23,

		vector<multi_production_step> multi_device_production_step_temp=multi_device_production_step_initial;

		//May 23





		if(mf_tool_setup_list[(mf_id-1)*(int)tool_setup_list.size()+ts_id-1].whether_s)

		{

			//cout<<"check tool setup id "<<ts_id<<endl;

			bool whether_ti_enough=true;

			int cert=tool_setup_list[ts_id-1].certification;

			vector<int> tf_id_vector=tool_setup_list[ts_id-1].tf_id_vector;

			vector<int> num_tool_vector=tool_setup_list[ts_id-1].num_tool_vector;

			int num_lot=0;



			for (int i=1;i<=tf_id_vector.size();i++)

			{

				int tf_id=tf_id_vector[i-1];

				int num_tool_required=num_tool_vector[i-1];

				int num_tool_available=tf_cert_ti_list[(tf_id-1)*Num_Temperature+cert-1].num_ti;

				if(num_tool_required > num_tool_available)

				{

					//cout<<"tooling setup "<<ts_id<<"doesn't have enough tools"<<endl;

					whether_ti_enough=false;

				}



			}



			if(whether_ti_enough)

			{

				//cout<<"tooling setup "<<ts_id<<"has enough ti, check all possible s"<<endl;



				//Added by Zhufeng, for randomness in assigning lots.April 29, 2012



				//build the candidate lot list for the machine and tooling setup first.

				//cout<<"candidate_lot_list_temp.size = "<<candidate_lot_list_temp.size()<<endl;

				vector<multi_lot> candi_lot_list_temp_this;

				vector<multi_lot>::iterator p_l_this=candidate_lot_list_temp.begin();

				//cout<<"mf_id = " << mf_id << "ts_id = "<<ts_id<<endl;

				vector<int> s_id_vector_this=mf_tool_setup_list[(mf_id-1)*tool_setup_list.size()+ts_id-1].s_id_vector;



				//cout<<"s_id_vector_this.size() = "<<s_id_vector_this.size()<<endl;

				for(int i=1;i<=(int)s_id_vector_this.size();i++)

				{

					int id = s_id_vector_this[i-1];

					//cout<<"s id = "<<id<<",mf_id ="<<S[id-1].mg<<"logpoint = "<<S[id-1].logpoint<<",device ="<<DEV[S[id-1].device-1]<<",temp = "<<S[id-1].temperature<<endl;

				}

							

				while( p_l_this !=candidate_lot_list_temp.end())

				{

					int logpoint=(* p_l_this ).logpoint;

					int dev_id=0;

					if((* p_l_this).whether_initial_lot)

					{

						dev_id=initial_running_lot[(* p_l_this).initial_running_lot_id-1].dev_id;

					}else

					{

						dev_id=lots_device[(* p_l_this).lot_id];

					}

					

					bool whether_route=false;







					int s_id=0;

					for(int i=1;i<=(int)s_id_vector_this.size();i++)

					{

						int id = s_id_vector_this[i-1];



						if(		 S[id-1].tf.tools	    == tf_id_vector

							&&  (S[id-1].logpoint		== logpoint)



							&&	(S[id-1].mg				== mf_id)

							&&	(S[id-1].device			== dev_id)

							&&	(S[id-1].temperature	== cert)	)

						{

							

							s_id=id;

							(* p_l_this).s_id=s_id;

							whether_route=true;

							//if((* p_l_this).whether_initial_lot)

							//	cout<<initial_running_lot[(* p_l_this).initial_running_lot_id-1].lot_name<<" has a route id"<<s_id<<endl;

							//else

							//	cout<<LOT[(* p_l_this).lot_id-1]<<" has a route id"<<s_id<<endl;

							break;

						}

					}//end of for(int id=1;id<=(int)S.size();id++)



					if(whether_route)

					{		//now check whether the time on the machine is enough



						double required_time=(double)(* p_l_this ).quantity/S[s_id-1].PPH+Load_Unload_Time;

						double remain_time_mach_this=one_mach.horizon-one_mach.from_begin_to_completion;

					

						//Added by Zhufeng, for resetup time, June 10, 2012

						 remain_time_mach_this =  remain_time_mach_this - tool_setup_time;

						 //end by Zhufeng,June 10,2012 



						if(remain_time_mach_this > one_mach.horizon - (*p_l_this).from_begin_to_completion )

						{

							remain_time_mach_this = one_mach.horizon - (*p_l_this).from_begin_to_completion;

							

						}



						//Added by Zhufeng, for reset at current time. June 19, 2012

						if(remain_time_mach_this > one_mach.horizon - tool_setup_time-current_time)

						{

							remain_time_mach_this = one_mach.horizon - tool_setup_time-current_time;

						}

						//End by Zhufeng, June 19, 2012



						if(remain_time_mach_this >= required_time)

						{

							candi_lot_list_temp_this.push_back(* p_l_this);

						}

					}//end of if(whether_route)

					++p_l_this;

				}//end of while( p_l !=candidate_lot_list_temp.end())



				//cout<<"candi_lot_list_temp_this.size = "<<candi_lot_list_temp_this.size()<<endl;

				

				vector<multi_lot> lot_assigned_temp_this;

				vector<multi_lot> lot_assigned_best_this;

				vector<multi_lot> candi_lot_temp_this=candi_lot_list_temp_this;

				struct multi_mach one_mach_temp_this=one_mach;

				double benefit_best_this=0;

				double benefit_temp_this=0;



				vector<multi_production> multi_device_production_temp_this=multi_device_production_temp;

				int Num_Pass_Temp_this = Num_Pass_temp;



				//Added by Zhufeng, May 23

				vector<multi_production_step> multi_device_production_step_temp_this=multi_device_production_step_temp;

				//end by Zhufeng, May 23





				

				//GRASP for 10 times to choose the  best. Each time randomly choose one from top 5.

				//Change times_grasp to 1, then this is just a random lot assignment.

				int times_grasp=1;

				int top_number=5;

				for(int i=1;i<=times_grasp;i++)

				{

					//cout<<"grasp iteration:"<<i<<endl;



					//Added by Zhufeng, for resetup time, June 10, 2012

					int num_lot_temp = 0;

					//end by Zhufeng,June 10,2012 



				while(!candi_lot_temp_this.empty())

				{

					int position;

					if(candi_lot_temp_this.size()>= top_number)

					{

						position = rand()% top_number;

					}else

					{

						position=rand() % candi_lot_temp_this.size();

					}



					p_l_this=candi_lot_temp_this.begin()+position;

					double required_time=(double)(* p_l_this).quantity/S[(* p_l_this).s_id-1].PPH+Load_Unload_Time;

					//Added by Zhufeng, for resetup time, June 10, 2012

					if(num_lot_temp == 0)

					{

						//This lot is possible the first lot after resetup, so need to consider setup time.

						required_time = required_time + tool_setup_time;

					}

					//end by Zhufeng,June 10,2012 

					double remain_time_mach_this=(one_mach_temp_this).horizon-(one_mach_temp_this).from_begin_to_completion;





					//if((*p_l_this).whether_initial_lot)

					//		cout<<initial_running_lot[(*p_l_this).initial_running_lot_id-1].lot_name;

					//else

					//		cout<<LOT[(* p_l_this).lot_id-1];

					

	

					if(remain_time_mach_this > (one_mach_temp_this).horizon - (*p_l_this).from_begin_to_completion )

					{

						remain_time_mach_this = one_mach_temp_this.horizon - (*p_l_this).from_begin_to_completion;

					}



					//Added by Zhufeng, for reset at current time. June 19, 2012

					if(remain_time_mach_this > one_mach_temp_this.horizon - tool_setup_time - current_time)

					{

						remain_time_mach_this = one_mach_temp_this.horizon - tool_setup_time - current_time;

					}

					//End by Zhufeng, June 19, 2012



					//cout<<"(*p_l_this).from_begin_to_completion = "<<(*p_l_this).from_begin_to_completion<<", remain_time_mach_this = "<<remain_time_mach_this<<endl;



					//cout<<endl<<"required_time = " <<required_time<<",remain_mach_Time_this = "<<remain_time_mach_this<<endl;



					if(remain_time_mach_this >= required_time)

					{

						lot_assigned_temp_this.push_back(* p_l_this);

						benefit_temp_this+= (* p_l_this).benefit;

						cout<<"For tool setup "<<ts_id<<",try to attach lot";

						if((*p_l_this).whether_initial_lot)

							cout<<initial_running_lot[(*p_l_this).initial_running_lot_id-1].lot_name;

						else

							cout<<LOT[(* p_l_this).lot_id-1];

						cout<<" to "<<MI[one_mach.mi_id-1]<<endl;



						update_multi_device_production(multi_device_production_temp_this, (*p_l_this), Num_Pass_Temp_this);	



						update_multi_device_production_step(multi_device_production_step_temp_this, (*p_l_this));





						//Note the start time of the first lot should be added the setup time of toolings.But this is not the final assignment process

						//Don't need to do that now. June 10, 2012. Zhufeng



						//Added by Zhufeng, for reset at current time. June 19, 2012	

						double bigger_time = ( current_time > one_mach_temp_this.from_begin_to_completion ? current_time:one_mach_temp_this.from_begin_to_completion);

						/*if(current_time > one_mach_temp_this.from_begin_to_completion )

						{

							cout<<"current_time = "<<current_time<<", machine.from_begin_to_completion = "<<one_mach_temp_this.from_begin_to_completion<<endl;

						}*/

						//Comment by Zhufeng June 19 ,2012

						//if((*p_l_this).from_begin_to_completion < one_mach_temp_this.from_begin_to_completion)

						//{

							//(* p_l_this).start_time=one_mach_temp_this.from_begin_to_completion;

							//(*p_l_this).from_begin_to_completion=one_mach_temp_this.from_begin_to_completion+required_time;

						//end of comment

						if((*p_l_this).from_begin_to_completion < bigger_time)

						{

							//cout<<"(*p_l_this).from_begin_to_completion = "<<(*p_l_this).from_begin_to_completion<<endl;

							(* p_l_this).start_time=bigger_time;

							(*p_l_this).from_begin_to_completion=bigger_time+required_time;

						//End by Zhufeng, June 19, 2012

						}else

						{							

							(* p_l_this).start_time=(* p_l_this).from_begin_to_completion;

							(*p_l_this).from_begin_to_completion=(* p_l_this).from_begin_to_completion+required_time;

						}

						//Added by Zhufeng, for resetup time, June 10, 2012

						num_lot_temp++;

						//end by Zhufeng,June 10,2012 

						one_mach_temp_this.from_begin_to_completion=(*p_l_this).from_begin_to_completion;



					}



					candi_lot_temp_this.erase(candi_lot_temp_this.begin()+position);

					//May 23, 2012

					//update_lot_benefit( candi_lot_temp_this,multi_device_production_temp_this);

					update_lot_benefit( candi_lot_temp_this,multi_device_production_step_temp_this);

					//May 23, 2012

				}//end of while(!candi_lot_temp.empty())

				if(benefit_temp_this > benefit_best_this)

				{

					benefit_best_this=benefit_temp_this;

					lot_assigned_best_this=lot_assigned_temp_this;

				}

				benefit_temp_this=0;

				one_mach_temp_this=one_mach;

				lot_assigned_temp_this.clear();

				candi_lot_temp_this=candi_lot_list_temp_this;

				multi_device_production_temp_this=multi_device_production_temp;

				Num_Pass_Temp_this=Num_Pass_temp;

				//May 23

				multi_device_production_step_temp_this = multi_device_production_step_temp;

				//May 23



			}//end of for(int i=1;i<=times_grasp;i++) GRASP end



		    struct multi_mach one_mach_temp= one_mach;

			int num_lot=0;

			vector<multi_lot>::iterator p_l=candidate_lot_list_temp.begin();

			//cout<<"show lot assigned best, size = "<<lot_assigned_best_this.size()<<endl;

			for(int i=1;i<=(int)lot_assigned_best_this.size();i++)

			{

				//cout<<"best lot id i ="<<i<<endl;

				p_l=candidate_lot_list_temp.begin();

				

				int lot_id_best=lot_assigned_best_this[i-1].lot_id;

				int logpoint_best=lot_assigned_best_this[i-1].logpoint;

				int pass_no=lot_assigned_best_this[i-1].pass_no;

				int s_id=lot_assigned_best_this[i-1].s_id;

				//Added by Zhufeng, May 14, 2012

				int	initial_running_lot_id_best=lot_assigned_best_this[i-1].initial_running_lot_id;

				//Ended by Zhufeng, May 14, 2012

				while( p_l != candidate_lot_list_temp.end())

				{

					if( 

							(*p_l).lot_id == lot_id_best &&

							(*p_l).logpoint == logpoint_best &&

							(*p_l).pass_no == pass_no &&

							//Added by Zhufeng, May 14, 2012

							(*p_l).initial_running_lot_id == initial_running_lot_id_best

							//Ended by Zhufeng, May 14, 2012 

						   )

					{

						

						(* p_l).s_id=s_id;

						//cout<<"s_id = "<<s_id<<endl;

						double required_time=(double)(* p_l).quantity/S[s_id-1].PPH+Load_Unload_Time;

						//cout<<"required_time = "<<required_time<<endl;

						double remain_time_mach=one_mach_temp.horizon-one_mach_temp.from_begin_to_completion;



						//Added by Zhufeng, for resetup time, June 10, 2012

						if(num_lot ==0)

						{

							remain_time_mach-= tool_setup_time;

						}

						//end by Zhufeng,June 10,2012 



						if( remain_time_mach > (one_mach_temp).horizon - (*p_l).from_begin_to_completion )

						{

							

							remain_time_mach = (one_mach_temp).horizon - (*p_l).from_begin_to_completion;

						}

						



						if(remain_time_mach >= required_time)

						{

							cout<<"The best lot assignment for tool setup "<<ts_id<<",attach lot";

							if((*p_l).whether_initial_lot)

							cout<<initial_running_lot[(*p_l).initial_running_lot_id-1].lot_name;

							else

							cout<<LOT[(* p_l).lot_id-1];

							cout<<" to "<<MI[one_mach.mi_id-1]<<endl;





							//Added by Zhufeng, for reset at current time. June 19, 2012	

							double bigger_time = (current_time > (one_mach_temp).from_begin_to_completion? current_time:(one_mach_temp).from_begin_to_completion);

							//Comment by Zhufeng June 19 ,2012



							/*if((*p_l).from_begin_to_completion < (one_mach_temp).from_begin_to_completion)

							{

								(* p_l).start_time=(one_mach_temp).from_begin_to_completion;*/

							//end of comment, June 19, 2012



								if((*p_l).from_begin_to_completion < bigger_time)

							{

								(* p_l).start_time=bigger_time;

							//End by Zhufeng, June 19, 2012

							}else

							{

								//(* p_l).start_time=(one_mach_temp).from_begin_to_completion;

								(* p_l).start_time=(* p_l).from_begin_to_completion;

								//(*p_l).from_begin_to_completion=(one_mach_temp).from_begin_to_completion+required_time;

							}



							//Added by Zhufeng, for resetup time, June 10, 2012

							if(num_lot ==0)

							{

								(* p_l).start_time+= tool_setup_time;

							}

							//end by Zhufeng,June 10,2012 



						

							(*p_l).from_begin_to_completion=(* p_l).start_time + required_time;

						

							(one_mach_temp).assigned_lot_list.push_back((*p_l));



							(one_mach_temp).from_begin_to_completion=(*p_l).from_begin_to_completion;



							cout<<"(one_mach_temp).from_begin_to_completion = "<<(one_mach_temp).from_begin_to_completion<<endl;



							num_lot++;



							//cout<<"num_lot="<<num_lot<<endl;



							finished_lot_list_temp.push_back((*p_l));



							update_multi_device_production(multi_device_production_temp, (*p_l),Num_Pass_temp);



							update_multi_device_production_step(multi_device_production_step_temp,(*p_l));



							if(num_lot==1)

							{

								//from_begin_to_setup_temp=(* p_l).start_time;

								//Modified by Zhufeng, June 18,2012, to adjust the setup time

								//from_begin_to_setup_temp=(* p_l).start_time; // This means the setup time is the time that finishing setup

								from_begin_to_setup_temp=(* p_l).start_time - tool_setup_time;//This means the setup time is the time that starting setup

								//end by Zhufeng, June 18, 2012

							}



							assigned_lot_list.push_back((*p_l));

							

							ts_weight+=(* p_l).benefit;

								

							//whether_lot_assigned=true;



							whether_resetup=true;



							from_begin_to_completion_temp=(*p_l).from_begin_to_completion;



							time_remain_mach_temp=one_mach.horizon-from_begin_to_completion_temp;



							p_l=candidate_lot_list_temp.erase( p_l );//if erase p_l. Then p_l needn't ++;



							//May 23, 2012



							//update_lot_benefit( candidate_lot_list_temp, multi_device_production_temp);



							update_lot_benefit( candidate_lot_list_temp, multi_device_production_step_temp);

							//May 23, 2912



						}//end of if(remain_time_mach >= required_time)

	

						break;

					}//end of if( 



					p_l++;	

						

				}//end of while( p_l != candidate_lot_list_temp.end())

				



			}//end of for(int i=1;i<=(int)lot_assigned_best.size();i++)



			//end by Zhufeng, April 29, 2012



			

			}//end of if(whether_ti_enough)



			if( ts_weight > best_ts_weight)

			{

				best_ts_weight=ts_weight;

				//cout<<"best_ts_weight="<<best_ts_weight<<endl;

				best_tool_setup_id=ts_id;

				best_from_begin_to_completion=from_begin_to_completion_temp;	



				cout<<" best_from_begin_to_completion = "<<best_from_begin_to_completion<<endl;



				best_from_begin_to_setup=from_begin_to_setup_temp;



				cout<<"best_from_begin_to_setup = "<<best_from_begin_to_setup<<endl;



				assigned_lot_list_best_ts=assigned_lot_list;

				candidate_lot_list=candidate_lot_list_temp;

				finished_lot_list=finished_lot_list_temp;



				//March 25, for mulit-pass production

				multi_device_production=multi_device_production_temp;

				Num_Pass=Num_Pass_temp;

				//March 25



				//May 23

				multi_device_production_step = multi_device_production_step_temp;

				//May 23

			}

		}//end of if(mf_tool_setup_list[(mf_id-1)*(int)tool_setup_list.size()+ts_id-1].whether_s)



	}//end of for(int ts_id=1;ts_id<=(int)tool_setup_list.size();ts_id++)



	return whether_resetup;

}

void AT::show_H()

{

	for(int i=1;i<=Num_Machine;i++)

	{

		cout<<"H["<<i<<"]="<<H[i]<<endl;

	}

}



void AT::rank_candidate_lot( vector<multi_lot> & candidate_lot)

{

	//cout<<"rank candidate lot list"<<endl;

	//rank the candiate_lot based on weight. The higher weight, the rank no. lesser.

	//This is need to be modified to rank based on objective value.

	vector<multi_lot>::iterator p_c=candidate_lot.begin();

	vector<multi_lot>::iterator p_next=candidate_lot.begin()+1;

	struct multi_lot p_temp;

	int num_iter=0;

	//cout<<"candidate_lot size="<<(int)candidate_lot.size()<<endl;



	while(num_iter<=(int)candidate_lot.size()-1)

	{

		//cout<<"num_iter="<<num_iter<<endl;

		p_c=candidate_lot.begin();

		p_next=candidate_lot.begin()+1;

		while( p_next != candidate_lot.end()-num_iter)

		{

			//Updated by Zhufeng, March 27, 2012, change weight to benefit

			//if( (* p_c).weight < (* p_next).weight )

			if( (* p_c).benefit < (* p_next).benefit )

				//end March 27, 2012

			{

				p_temp=(* p_next);

				(* p_next)=(* p_c);

				(* p_c)=p_temp;

			}

			p_c++;

			p_next++;

			//num_iter++;

		}

		num_iter++;

	}

	//cout<<"OK4"<<endl;

}



//Feb 12

void AT::intialize_tool_setup_list(vector<tool_setup> & tool_setup_list,const vector<tooling_configure> & S)

{

	cout<<"initalize tool_setup_list"<<endl;

	

	vector<tooling_configure>::const_iterator p_S=S.begin();

	int count=1;

	while(p_S!=S.end()){

		vector<int> a;  //number of pieces

		int s=(*p_S).id;

		vector<int>::const_iterator pt=(*p_S).tf.tools.begin();

		while(pt!=(*p_S).tf.tools.end()){

			int tf=(*pt);

			int pos=s*Max_Num_Tooling_Family+tf;

			//a.push_back(a_st[s][tf]);

			a.push_back(a_st[pos]);

			pt++;

		}

		vector<tool_setup>::iterator p_t=tool_setup_list.begin();

		while(p_t!=tool_setup_list.end()){

			if(

				(*p_S).tf.tools==(*p_t).tf_id_vector //Since this is vector of int. We can use "==" to directly compare these two vectors.

				&& compare_int_vec(a,(*p_t).num_tool_vector) 

				&& (*p_S).temperature==(*p_t).certification



				)

			{ 

				(*p_t).s_id_vector.push_back((*p_S).id);

				break;}

			p_t++;

		}

		if(p_t==tool_setup_list.end()){

			struct tool_setup one_tool_setup;

			one_tool_setup.id=count;

			count++;

			one_tool_setup.tf_id_vector.resize((int)(*p_S).tf.tools.size());

			copy((*p_S).tf.tools.begin(),(*p_S).tf.tools.end(),one_tool_setup.tf_id_vector.begin());

			one_tool_setup.num_tool_vector.resize((int)a.size());

			copy(a.begin(),a.end(),one_tool_setup.num_tool_vector.begin());

			one_tool_setup.certification=(*p_S).temperature;

			one_tool_setup.s_id_vector.push_back((*p_S).id);

			tool_setup_list.push_back(one_tool_setup);

		}

		p_S++;

	}

}



void AT::show_tool_setup_list(const vector<tool_setup> & tool_setup_list)

{

	cout<<"show tool_setup_list"<<endl;

	

	ofstream fout("Debug/tool_setup_list.txt");

	fout<<"tool_setup_list -------------------------------------"<<endl;

	vector<tool_setup>::const_iterator p_t=tool_setup_list.begin();



	while(p_t!=tool_setup_list.end())

	{

		fout<<"id="<<(*p_t).id<<",tooling=";

		vector<int>::const_iterator p_int=(*p_t).tf_id_vector.begin();

		while(p_int!=(*p_t).tf_id_vector.end())

		{

			int offset=p_int-(*p_t).tf_id_vector.begin();

			vector<int>::const_iterator pb=(* p_t).num_tool_vector.begin()+offset;

			fout<<(*p_int)<<"("<<(*pb)<<"),";

			fout<<","<<TF[(*p_int)-1]<<",";

			p_int++;

		}



		fout<<"temperature="<<(*p_t).certification<<endl;

		fout<<"s_id=";

		p_int=(*p_t).s_id_vector.begin();

		while(p_int!=(*p_t).s_id_vector.end())

		{

			fout<<(*p_int)<<",";

			p_int++;

		}

		fout<<endl;

		p_t++;

	}

	fout.close();

}

//end by Feb 12

//Feb 13

void AT::initialize_mf_tool_setup_list( vector<mf_tool_setup> & mf_tool_setup_list,const vector<tool_setup> & tool_setup_list)

{

	cout<<"initialize mf_tool_setup_list"<<endl;



	for(int i=1;i<=(int)MF.size();i++)

	{

		for(int j=1;j<=(int)tool_setup_list.size();j++)

		{

			struct mf_tool_setup one_mf_tool_setup;

			one_mf_tool_setup.mf_id=i;

			one_mf_tool_setup.tool_setup_list_id=j;

			one_mf_tool_setup.whether_s=false;

			mf_tool_setup_list.push_back(one_mf_tool_setup);

		}

	}

    /*bool * whether_id_in_mf_ts=new bool[(int)MF.size()];*/

	vector<tool_setup>::const_iterator p_ts=tool_setup_list.begin();

	while( p_ts != tool_setup_list.end())

	{

		int tool_setup_list_id=(*p_ts).id; 

		/*for(int i=1;i<=(int)MF.size();i++)

		{

			whether_id_in_mf[(i-1]=false;

		}*/

		vector<int>::const_iterator p_s_id=(* p_ts).s_id_vector.begin();

		while(p_s_id!=(* p_ts).s_id_vector.end())

		{

			int s_id= * p_s_id;

			int mf_id=S[s_id-1].mg;

			mf_tool_setup_list[(mf_id-1)*(int)tool_setup_list.size()+tool_setup_list_id-1].s_id_vector.push_back(s_id);

			mf_tool_setup_list[(mf_id-1)*(int)tool_setup_list.size()+tool_setup_list_id-1].whether_s=true;

			p_s_id++;

		}

		p_ts++;

	}

	//delete[] whether_id_in_mf;

	//vector<tooling_configure> TS;

	//get_S(TS,i);

	//vector<tooling_configure>::iterator p_TS=TS.begin();

	//while(p_TS!=TS.end())

	//{

	//	vector<int> num_tool;

	//	int s=(*p_TS).id;

	//	vii pt=(*p_TS).tf.tools.begin();

	//	while(pt!=(*p_TS).tf.tools.end())

	//	{

	//		int tf=(*pt);

	//		int pos=s*Max_Num_Tooling_Family+tf;

	//		//a.push_back(a_st[s][tf]);

	//		num_tool.push_back(a_st[pos]);

	//		pt++;

	//	}







	//	p_TS++;

	//}



}



//Feb 14

void AT::show_mf_tool_setup_list( const vector<mf_tool_setup> & mf_tool_setup_list)

{

	ofstream f_out("Debug/mf_tool_setup_list.txt");

	f_out<<"show each mf can have which tool_setup_list_id."<<endl;

	vector<mf_tool_setup>::const_iterator p_mts=mf_tool_setup_list.begin();

	while(p_mts != mf_tool_setup_list.end())

	{

		f_out<<MF[(*p_mts).mf_id-1]<<",tool_setup_id="<<(*p_mts).tool_setup_list_id<<endl;

		if((*p_mts).whether_s)

		{

			f_out<<"s_id=";

		}else

		{

			f_out<<"No s.";

		}

		vector<int>::const_iterator p_i=(*p_mts).s_id_vector.begin();

		while( p_i != (*p_mts).s_id_vector.end())

		{

			f_out<<(*p_i)<<",";

			p_i++;

		}

		f_out<<endl;

		p_mts++;



	}

	f_out.close();



}

//End by Feb 14



//Feb 15

void AT::initialize_tf_cert_ti_list( vector<tf_cert_ti> & tf_cert_ti_list, const int Num_Temperature, const vector<Tooling> Tooling_vector)

{

	cout<<"initialize_tf_cert_ti_list"<<endl;

	for(int i=1;i<=(int)TF.size();i++)

	{

		for(int j=1;j<=Num_Temperature;j++)

		{

			struct tf_cert_ti one_tf_cert_ti;

			one_tf_cert_ti.id=(i-1)*Num_Temperature+j;//id -1 is the position;

			one_tf_cert_ti.tf_id=i;

			one_tf_cert_ti.certification=j;

			one_tf_cert_ti.num_ti=0;

			tf_cert_ti_list.push_back(one_tf_cert_ti);



		}

	}

	vector<Tooling>::const_iterator p_t=Tooling_vector.begin();

	while( p_t != Tooling_vector.end())

	{

		int tf_id=(* p_t).tf;

		vector<int>::const_iterator p_temp=(* p_t).temperature_vector.begin();

		while( p_temp!=(* p_t).temperature_vector.end())

		{

			int temperature=(*p_temp);

			tf_cert_ti_list[(tf_id-1)*Num_Temperature+ temperature-1].num_ti++;

			p_temp++;

		}

		p_t++;

	}



}



void AT::show_tf_cert_ti_list( const vector<tf_cert_ti> & tf_cert_ti_list,const int Num_Temperature)

{

	ofstream fout("Debug/tf_cert_ti_list.txt");

	fout<<"num of temperature is "<<Num_Temperature<<endl;



	vector<tf_cert_ti>::const_iterator p_tct=tf_cert_ti_list.begin();

	while(p_tct!=tf_cert_ti_list.end())

	{

		fout<<"id="<<(* p_tct).id<<",tf_id="<<(* p_tct).tf_id<<","<<TF[(* p_tct).tf_id-1]<<",cert="<<(* p_tct).certification<<",num_ti="<<(* p_tct).num_ti<<endl;

		p_tct++;

	}

	fout.close();

}

//Feb 15

//Feb 16

void AT::update_tf_cert_ti_list_begin(vector<tf_cert_ti> & tf_cert_ti_list,const int Num_Temperature,const vector<multi_mach> & machine_list)

{

	//Assume that the toolings of initial setup are used and the toolings of regular setup are used at the beginning.

	cout<<"update_tf_cert_ti_list_begin"<<endl;

	vector<multi_mach>::const_iterator p_m=machine_list.begin();

	while(p_m != machine_list.end())

	{

		bool whether_setup_diff_initial_setup=false; //denote whether the regular setup is different from initial setup



		if((*p_m).whether_initial_lot )

		{

			int mi_id=(*p_m).mi_id;

			int lambda_id=Machine_vector[mi_id-1].initial_tooling_setup;

			int cert=Lambda[lambda_id-1].temperature;

			vector<int> tf_id_vector=Lambda[lambda_id-1].tooling_vec;



			if(tf_id_vector==(*p_m).tf_id_vector)

			{

				whether_setup_diff_initial_setup=true;

			}



			vector<int> num_ti_vector=Lambda[lambda_id-1].b;

			for (int i=1;i<=tf_id_vector.size();i++)

			{

				int tf_id=tf_id_vector[i-1];

				int num_ti=num_ti_vector[i-1];

				if(tf_cert_ti_list[(tf_id-1)*Num_Temperature+cert-1].num_ti<num_ti)

				{

					cout<<"error: "<<MI[(* p_m).mi_id-1]<<",for"<<TF[tf_id-1]<<",number of tooling available is less than num_ti required in the initial tooling setup"<<endl;

					//exit(0);

				}else

				{

					cout<<MI[mi_id-1]<<",intial setup "<<TF[tf_id-1]<<",-"<<num_ti<<endl;

					tf_cert_ti_list[(tf_id-1)*Num_Temperature+cert-1].num_ti-=num_ti;

				}



			}



		}



		if((*p_m).whether_setup && !whether_setup_diff_initial_setup)

		{

			//if not set up yet after finishing initial lots, the tf_id_vector is empty

			int cert=(*p_m).certification;

			vector<int>::const_iterator p_tf=(*p_m).tf_id_vector.begin();

			while(p_tf!=(*p_m).tf_id_vector.end())

			{

				int tf_id=(* p_tf);

				if(tf_cert_ti_list[(tf_id-1)*Num_Temperature+cert-1].num_ti>=1)

				{

				--tf_cert_ti_list[(tf_id-1)*Num_Temperature+cert-1].num_ti;

				

				cout<<MI[(* p_m).mi_id-1]<<","<<TF[tf_id-1]<<",-1"<<endl;

				}else

				{

					cout<<"error,"<<MI[(* p_m).mi_id-1]<<","<<TF[tf_id-1]<<",not enough toolings"<<endl;

				}

				p_tf++;

			}

			

		}

		p_m++;

	}



}

//Feb 16



void AT::Heuristics_three(vector<multi_mach> & machine_list,

						  const vector<multi_lot> & first_pass_lot_vector,

						  const vector<tool_setup> & tool_setup_list,

						  const vector<mf_tool_setup> & mf_tool_setup_list,

						   vector<tf_cert_ti> & tf_cert_ti_list,

						   vector<multi_lot> & finished_lot_list

						  )

{

	cout<<"Heurstics three for multipass issue begins."<<endl;

	double last_time=0;

	double current_time=0;



	bool whether_stop=false;

	vector<multi_mach> candidate_list;  //candidate machine list that can contribute to new lots

	vector<multi_lot> candidate_lot_list; // candidate lot list

	//vector<multi_lot>finished_lot_list; // finished second pass , third pass, or higher pass number lot list.





	int num_iter=0;

	while (!whether_stop)

	{

		num_iter++;

		cout<<"The "<<num_iter<<"th iteration for Heuristics."<<endl;

		candidate_list.clear();

		vector<multi_mach>::const_iterator p_m=machine_list.begin();

		while(p_m!=machine_list.end())

		{

			if( (* p_m).whether_need_check_for_new_lot && (* p_m).from_begin_to_completion <= (* p_m).horizon)

			{

				candidate_list.push_back( (* p_m));

			}

			p_m++;

		}

		if( candidate_list.empty())

		{

			whether_stop=true;   //Stop running the while iteration. Actually ,just break is enough, dont need this. But to be clearly understood, add this. 

			break;

		}



		//find the machines with minimal completion_time

		vector<multi_mach> min_candidate_list;

		double min_completion_time= 1E9;

		vector<multi_mach>::iterator p_c=candidate_list.begin();

		//cout<<",min_completion_time="<<min_completion_time;

		while( p_c != candidate_list.end())

		{

			//cout<<MI[(*p_c).mi_id-1]<<", completion="<<(*p_c).from_begin_to_completion<<endl;

			//cout<<"completion="<<(*p_c).from_begin_to_completion<<","<<MI[(*p_c).mi_id-1]<<endl;

			if( (*p_c).from_begin_to_completion < min_completion_time)

			{

				min_completion_time=(*p_c).from_begin_to_completion;

				//cout<<",min_completion_time="<<min_completion_time;

				min_candidate_list.clear();

				min_candidate_list.push_back(* p_c);

			}else

			{

				if((*p_c).from_begin_to_completion==min_completion_time)

				{

					min_candidate_list.push_back( * p_c);

				}

			}

			p_c++;

		}

		current_time=min_completion_time;

		cout<<"now the current_time="<<current_time<<endl;

		if(min_candidate_list.empty())

		{

			cout<<"wrong in min_candidate_list_id_vector"<<endl;

			exit(0);

		}

		

		cout<<"current_machine";

		p_c=min_candidate_list.begin();

		while( p_c != min_candidate_list.end() )

		{

			cout<<MI[(*p_c).mi_id-1]<<",";

			p_c++;

		}

		cout<<endl;



		//show_multi_device_production();



		bool whether_new_lot=get_candidate_lot_list(candidate_lot_list, first_pass_lot_vector,finished_lot_list,current_time, last_time, Num_Pass);



		if(whether_new_lot)

		{

			//Added March 27, 2012, Zhufeng, compute benefit.

			//compute_lot_benefit( candidate_lot_list, multi_device_production);

			//end by March 27, 2012



			//Added May 23, 2012, Zhufeng, compute benefit.by step

			compute_lot_benefit( candidate_lot_list, multi_device_production_step);

			//end by May 23, 2012

			rank_candidate_lot(candidate_lot_list);

		}

		

		//show_candidate_lot_list(candidate_lot_list,last_time, current_time);

		

		if(whether_new_lot)

		{	//cout<<"Some new lots have been added into consideration during these period. Consider all the machines free earlier than or at current time."<<endl;

			assign_lot(machine_list,candidate_lot_list,finished_lot_list,current_time,tool_setup_list,mf_tool_setup_list, tf_cert_ti_list);

		}else

		{

			//cout<<"No new lots but new machines available.Only check the machines free at the current time."<<endl;

			assign_lot(machine_list,candidate_lot_list,finished_lot_list,current_time,min_candidate_list,tool_setup_list,mf_tool_setup_list, tf_cert_ti_list);

		}



		last_time=current_time;



		



		//exit(0);







	}//end of while (!whether_stop)

}



void AT::show_finished_lot_list( const vector<multi_lot> finished_lot_list)

{

	cout<<"show finished_lot_list"<<endl;

	ofstream fout("Debug/finished_lot_list.txt");

	fout<<"**********************************************"<<endl;

	fout<<"* only show lots with pass-no greater than 1 *"<<endl;

	fout<<"**********************************************"<<endl;

	fout<<"LOT_NAME,pass no,route no(s_id)"<<endl;

	vector<multi_lot>::const_iterator p_l=finished_lot_list.begin();

	while(p_l!=finished_lot_list.end())

	{

		if((* p_l).whether_initial_lot)

		{

			fout<<initial_running_lot[(* p_l).initial_running_lot_id-1].lot_name<<","<<(* p_l).pass_no<<","<<(* p_l).s_id<<endl;

		}else

		{

			fout<<LOT[(* p_l).lot_id-1]<<","<<(* p_l).pass_no<<","<<(* p_l).s_id<<endl;

		}

		++p_l;

	}

	fout<<"num of finished lots are "<<finished_lot_list.size()<<endl;

}

//Modified by Shihui Sep 8,2014

void AT::resetup(vector<multi_mach> & reset_machine_list,

				 const vector<multi_lot> & first_pass_lot_vector,

				 const vector<tool_setup> & tool_setup_list,

				 const vector<mf_tool_setup> & mf_tool_setup_list, 

				 vector<tf_cert_ti> & tf_cert_ti_list,

				 const vector<multi_lot> & finished_lot_list,

				 vector<multi_lot> & finished_lot_reset_list,

				 const vector<multi_lot> & unassigned_lot_first_pass_vector

				 )

{

	cout<<"-------try to reset machines for multi-pass problems----------"<<endl;


	ofstream foutsj("Debug/SJ_setupcount.txt");
	foutsj<<"This file is used to debug for the number of setup count"<<endl;
	foutsj<<"Modified by Shihui Jia Sep 8, 2014" <<endl;

	

	double last_time=0;

	double current_time=0;



	bool whether_stop=false;

	vector<multi_mach> candidate_mach_list;  //candidate machine list that can contribute to new lots

	vector<multi_lot> candidate_lot_list=unassigned_lot_first_pass_vector; // candidate lot list



	cout<<"candidate lots size="<<candidate_lot_list.size()<<endl;
	foutsj<<"candidate lots size="<<candidate_lot_list.size()<<endl;


	//show_candidate_lot_list(candidate_lot_list,last_time, current_time);


	//Add by Shihui Jia, 

	int forced_changeover=0;
	forced_changeover=resetup_option;
	//forced_changeover=30;

	//Add by Shihui Jia, new iteration number num_changeover

	int num_changeover=0;

	vector<int> candiate_mach_list_id_vector;// 

	int num_iter=0;
	
	int num_true_iter=0;
	int setupchange_0910=0;

	while (!whether_stop && num_true_iter<forced_changeover-1)

	{
		//Add by SJ
		bool whether_resetup_sj=false;

		

		num_iter++;

		cout<<"The "<<num_iter<<"th iteration for resetup."<<endl;
		foutsj<<"The "<<num_iter<<"th iteration for resetup."<<endl;
		//foutsj<<"The "<<num_changeover<<"th changeover. "<<endl;

		candidate_mach_list.clear();

		candiate_mach_list_id_vector.clear();



		vector<multi_mach>::const_iterator p_m=reset_machine_list.begin();

		

		//cout<<"reset machine list:"<<endl;

		while(p_m!=reset_machine_list.end())

		{

			//cout<<MI[(* p_m).mi_id - 1];



			if( (* p_m).whether_need_check_for_new_lot && 

				!(* p_m).whether_resetup && 

				(* p_m).from_begin_to_completion <= (* p_m).horizon

				)

			{

				candiate_mach_list_id_vector.push_back( p_m-reset_machine_list.begin()+1);

				candidate_mach_list.push_back( (* p_m));

				//cout<<", into candidate mach";

			}

			p_m++;



			//cout<<endl;

		}

		if( candidate_mach_list.empty())

		{

			whether_stop=true;   //Stop running the while iteration. Actually ,just break is enough, dont need this. But to be clearly understood, add this. 

			break;

		}



		//find the machines with minimal completion_time

		vector<multi_mach> min_candidate_mach_list;

		vector<int> min_candidate_mach_list_id_vector;



		double min_completion_time= 1E9;

		vector<multi_mach>::iterator p_c=candidate_mach_list.begin();

		vii p_i=candiate_mach_list_id_vector.begin();

		//cout<<",min_completion_time="<<min_completion_time;

		while( p_c != candidate_mach_list.end())

		{

			//cout<<MI[(*p_c).mi_id-1]<<", completion="<<(*p_c).from_begin_to_completion<<endl;

			//cout<<"completion="<<(*p_c).from_begin_to_completion<<","<<MI[(*p_c).mi_id-1]<<endl;

			if( (*p_c).from_begin_to_completion < min_completion_time)

			{

				min_completion_time=(*p_c).from_begin_to_completion;

				//cout<<",min_completion_time="<<min_completion_time;

				min_candidate_mach_list.clear();

				min_candidate_mach_list.push_back(* p_c);

				min_candidate_mach_list_id_vector.clear();

				min_candidate_mach_list_id_vector.push_back(* p_i);



			}else

			{

				if((*p_c).from_begin_to_completion==min_completion_time)

				{

					min_candidate_mach_list.push_back( * p_c);

					min_candidate_mach_list_id_vector.push_back(* p_i);

				}

			}

			++p_c;

			++p_i;

		}

		current_time=min_completion_time;

		cout<<"now the current_time="<<current_time<<endl;
		foutsj<<"now the current_time="<<current_time<<endl;
		

		if(min_candidate_mach_list.empty())

		{

			cout<<"wrong in min_candidate_list_id_vector"<<endl;

			exit(0);

		}

		

		cout<<"current_machine ";
		foutsj<<"current_machine ";

		p_c=min_candidate_mach_list.begin();

		while( p_c != min_candidate_mach_list.end() )

		{

			cout<<MI[(*p_c).mi_id-1]<<",";
			foutsj<<MI[(*p_c).mi_id-1]<<",";

			p_c++;

		}

		cout<<endl;
		foutsj<<endl;


		//process machines, release tool setup if there is.

		p_i=min_candidate_mach_list_id_vector.begin();

		p_c=min_candidate_mach_list.begin();



		bool whether_new_tool_released=false;



		while(p_i!=min_candidate_mach_list_id_vector.end())

		{

			if((*p_c).whether_setup)

			{

				cout<<"release tool:"<<endl;

				whether_new_tool_released=true;



				reset_machine_list[(*p_i)-1].whether_setup=false;



				int cert=reset_machine_list[(*p_i)-1].certification;

				cout<<"cert="<<cert<<",";

				vector<int> tf_id_vector=reset_machine_list[(*p_i)-1].tf_id_vector;

                

				vii p_tf_id = tf_id_vector.begin();

				while(p_tf_id!=tf_id_vector.end())

				{

					int tf_id=(* p_tf_id);



					cout<<TF[tf_id-1]<<",";

	

					++tf_cert_ti_list[(tf_id-1)*Num_Temperature+cert-1].num_ti;

					

					++p_tf_id;

				}

				cout<<"from machine "<<MI[reset_machine_list[(*p_i)-1].mi_id-1]<<endl;

				//cout<<"from p_c "<<MI[(* p_c).mi_id-1]<<endl;



			}

			++p_c;

			++p_i;

		}



		cout<<endl;



		//check new lots



		bool whether_new_lot=get_candidate_lot_list(candidate_lot_list, first_pass_lot_vector,finished_lot_reset_list,current_time, last_time,finished_lot_list, Num_Pass);



		if(whether_new_lot)

		{

			//added March 27, 2012, Zhufeng, compute benefit.

			//compute_lot_benefit( candidate_lot_list,  multi_device_production);

			//end by March 27, 2012



			//added March 27, 2012, Zhufeng, compute benefit by step.

			compute_lot_benefit( candidate_lot_list,  multi_device_production_step);

			//end by March 27, 2012

			rank_candidate_lot(candidate_lot_list);

		}

		

		//show_candidate_lot_list(candidate_lot_list,last_time, current_time);


		//Add by SJ: whether_resetup determines whether the resetup is successfully!

		//If it's the local and global are the same value, then define a bool value here, make it equal to the returned value of bool function;

		if( whether_new_lot )

		{

			cout<<"try to assign lots to all earlier machines and resetup ealier machines that have no setup now."<<endl;
			foutsj<<"try to assign lots to all earlier machines and resetup ealier machines that have no setup now."<<endl;
			int type =1;

			cout<<"type = "<<type<<endl;
			foutsj<<"type = "<<type<<endl;

			setupchange_0910=assign_lot_reset(reset_machine_list,candidate_lot_list,finished_lot_reset_list,current_time,tool_setup_list,mf_tool_setup_list, tf_cert_ti_list, type);

			cout<<"In the AT::resetup the value of setupchange_0910 is: "<<setupchange_0910<<endl;

			num_true_iter+=setupchange_0910;
			cout<<"end of assign_lot_reset"<<endl;
			foutsj<<"end of assign_lot_reset"<<endl;

		}else

		{

			if( whether_new_tool_released )

			{

				cout<<"try to reset erlier machines that have no setup now"<<endl;
				foutsj<<"try to reset erlier machines that have no setup now"<<endl;

				//Use this function temporarily. I should write a new function just reseting machines but not assigning lots. 

				int type =2;

				cout<<"type = "<<type<<endl;
				foutsj<<"type = "<<type<<endl;

				setupchange_0910=assign_lot_reset(reset_machine_list,candidate_lot_list,finished_lot_reset_list,current_time,tool_setup_list,mf_tool_setup_list, tf_cert_ti_list,type);

				num_true_iter+=setupchange_0910;

				cout<<"end of assign_lot_reset"<<endl;
				foutsj<<"end of assign_lot_reset"<<endl;

			}else

			{

				cout<<"try to reset current machine"<<endl;
				foutsj<<"try to reset current machine"<<endl;

				//

				assign_lot_reset( reset_machine_list,candidate_lot_list,finished_lot_reset_list,current_time,min_candidate_mach_list_id_vector,tool_setup_list,mf_tool_setup_list,tf_cert_ti_list);

				cout<<"end of assign_lot_reset"<<endl;
				foutsj<<"end of assign_lot_reset"<<endl;


			}

		}


		cout<<"while changing the value of whether_resetup_sj is: "<<whether_resetup_sj<<endl;
		//show_machine_list(reset_machine_list);

		cout<<"Sep12 change num is: "<<setupchange_0910<<endl;
		cout<<"Sep12 total change num is: "<<num_true_iter<<endl;

		last_time=current_time;

		//exit(0);



	}//end of while(!whether_stop)

	cout<<"In AT::resetup the total number of true changeover is: "<<num_true_iter<<endl;

}

//Feb 24

//Changed by SHihui Jia

int AT::assign_lot_reset( vector<multi_mach> & machine_list, //note: when the function is used, this should be reset_machine_list

		             vector<multi_lot> & candidate_lot_list,

					 vector<multi_lot> & finished_lot_reset_list,

					 double current_time,

					 const vector<tool_setup> tool_setup_list,

					 const vector<mf_tool_setup> & mf_tool_setup_list, 

					 vector<tf_cert_ti> & tf_cert_ti_list,

					 const int type

					)

{

	//Add by SHihui
	bool whether_change_sj=false;
	int setupchange_0910=0;
	int changeover_num=0;
	if(type ==1)

		cout<<"try to assign new lots to all machines free earlier than current time and reset some machines:"<<current_time<<endl;

	if(type ==2)

		cout<<"try to reset some machines "<<endl;

	vector<multi_mach>::iterator p_m=machine_list.begin();

	vector<multi_lot>::iterator p_l=candidate_lot_list.begin();



	vector<multi_mach> reset_mach_list;

	

	while ( p_m != machine_list.end())

	{
		//Add by SJ, there maybe more than 1 machines setup in this while loop

		setupchange_0910=0;


		int m_id=p_m-machine_list.begin()+1;

		if( (*p_m).from_begin_to_completion <= current_time && !(*p_m).whether_resetup)

		{

			p_l=candidate_lot_list.begin();

			bool whether_mach_assign=false;

			if((*p_m).whether_setup) // This machine has been setup

			{

				int mf_id=(* p_m).mf_id;

				if(type ==1 )

				cout<<"Type 1 Exception. This is resetup procedure. Assign lots to machine:"<<MI[(* p_m).mi_id-1]<<" that has been setup before, at "<<current_time<<endl;

				if( type ==2)

					cout<<"Type 2 Error:this is resetup procedure, "<<MI[(* p_m).mi_id-1]<<"as been setup before "<<current_time<<endl;

				vector<int> tf_id_vector=(* p_m).tf_id_vector;

				int certification=(* p_m).certification;



				while( p_l !=candidate_lot_list.end())

				{

					int logpoint=(* p_l).logpoint;

					int dev_id=0;

					if((* p_l).whether_initial_lot)

					{

						dev_id=initial_running_lot[(* p_l).initial_running_lot_id-1].dev_id;

					}else

					{

						dev_id=lots_device[(* p_l).lot_id];

					}



					bool whether_route=false;

					bool whether_lot_assigned=false;



					int s_id=0;

					for(int id=1;id<=(int)S.size();id++)

					{



						if(		 S[id-1].tf.tools	    == tf_id_vector

							&&  (S[id-1].logpoint		== logpoint)



							&&	(S[id-1].mg				== mf_id)

							&&	(S[id-1].device			== dev_id)

							&&	(S[id-1].temperature	== certification)	)

						{

							

							s_id=id;

							whether_route=true;

							break;

						}

					}//end of for(int id=1;id<=(int)S.size();id++)

					if(whether_route)

					{		//now check whether the time on the machine is enough

						double required_time=(double)(* p_l).quantity/S[s_id-1].PPH+Load_Unload_Time;

						double remain_time_mach=(*p_m).horizon-(*p_m).from_begin_to_completion;



						//cout<<"horizon="<<(*p_m).horizon<<",(*p_m).from_begin_to_completion="<<(*p_m).from_begin_to_completion<<",";



						//added in Feb 20,

						if(remain_time_mach > (*p_m).horizon - (*p_l).from_begin_to_completion )

						{

							remain_time_mach = (*p_m).horizon - (*p_l).from_begin_to_completion;

						}



						//cout<<",remain_time_mach ="<<remain_time_mach <<endl;

						//end Feb 20



						if(remain_time_mach >= required_time)

						{

							cout<<"	s ="<<s_id<<"	"<<endl;

							//cout<<"The completion of the lot="<<(*p_l).from_begin_to_completion<<endl;

								cout<<", attach this lot to mach"<<MI[(* p_m).mi_id-1]<<endl;



								//Added by Zhufeng, to compute obj value, Feb 27

								(* p_l).s_id=s_id;



								//Feb 27



								if((*p_l).from_begin_to_completion < (* p_m).from_begin_to_completion)

								{

									(* p_l).start_time=(* p_m).from_begin_to_completion;

									(*p_l).from_begin_to_completion=(* p_m).from_begin_to_completion+required_time;

								}else

								{

									(* p_l).start_time=(* p_l).from_begin_to_completion;

									(*p_l).from_begin_to_completion=(* p_l).from_begin_to_completion+required_time;

								}



							//cout<<"Then completion="<<(*p_l).from_begin_to_completion<<endl;

							//(*p_l).from_begin_to_completion+=required_time;



							whether_mach_assign=true;



							whether_lot_assigned=true;



							(* p_m).assigned_lot_list.push_back((*p_l));



							(* p_m).from_begin_to_completion=(*p_l).from_begin_to_completion;



							finished_lot_reset_list.push_back((*p_l));



							update_multi_device_production(multi_device_production, (*p_l),Num_Pass);



                            //May 23

							update_multi_device_production_step(multi_device_production_step, (*p_l));

							//May 23



							p_l=candidate_lot_list.erase( p_l );//if erase p_l. Then p_l needn't ++;



						}



					}



					if(!whether_lot_assigned)

					{

						p_l++;

					}

				}





			}else//corresponding to if((*p_m).whether_setup) 

			{

				

				//this machine is not found in the solution.csv.Three cases:(1)no initial lot, no setup. (2)intial lot finished before start of horizon, no setup.(3)initial lot cannot be finished within horizon.

				//If the first two cases, resetup. If the third doing nothing.



				if((*p_m).whether_initial_lot && !(*p_m).whether_finish_initial)

					{

						cout<<"Machine "<<MI[(*p_m).mi_id-1]<<"cannot finish its initial lot within horizon. No resetup"<<endl;

					}else

					{

						//cout<<"try to resetup Machine "<<MI[(*p_m).mi_id-1]<<endl;

					

						int best_tool_setup_id=-1;

						double best_ts_weight=0;

						vector<multi_lot> assigned_lot_list_best_ts;

						double from_begin_to_completion;

						double from_begin_to_setup;

						vector<multi_lot> candidate_lot_list_temp=candidate_lot_list; //Need to update candidate_lot_list after compute_benefit

						vector<multi_lot> finished_lot_list_temp=finished_lot_reset_list;

						//March 27, for multi-pass production, Zhufeng

						vector<multi_production> multi_device_production_temp=multi_device_production;

						int Num_Pass_temp=Num_Pass;

						//March 27



						//May 23

						vector<multi_production_step> multi_device_production_step_temp = multi_device_production_step;

						//May 23



						bool whether_resetup = compute_benefit_resetup_empty( * p_m,candidate_lot_list_temp,finished_lot_list_temp, current_time,best_ts_weight,best_tool_setup_id,assigned_lot_list_best_ts,tool_setup_list,mf_tool_setup_list,tf_cert_ti_list,from_begin_to_completion,from_begin_to_setup,multi_device_production_temp,Num_Pass_temp,multi_device_production_step_temp);


						//Add by SJ
						cout<<"THe value of whether_resetup is: "<<whether_resetup<<endl;
						//whether_change_sj=whether_resetup++;
						//foutsj<<"THe value of whether_resetup is: "<<whether_resetup<<endl;
					    if(whether_resetup)
						{
							//Added by Shihui JIa; num_changeover++
							//num_changeover++;
							//cout<<"SHihui Jia the value of whether_resetup is: "<<wether_resetup<<endl;
							cout<<"resetup "<<MI[(*p_m).mi_id-1]<<" successfully!"<<endl;

							

							if(best_tool_setup_id==-1)

							{

								cout<<"error: no tool setup are found for reseting up!"<<endl;

							}else

							{

								//update tf_cert_ti_list

								for(int i=1;i<=(int)tool_setup_list[best_tool_setup_id-1].tf_id_vector.size();i++)

								{

									int tf_id=tool_setup_list[best_tool_setup_id-1].tf_id_vector[i-1];

									int certification=tool_setup_list[best_tool_setup_id-1].certification;

									int num_ti=tool_setup_list[best_tool_setup_id-1].num_tool_vector[i-1];



									if(tf_cert_ti_list[(tf_id-1)*Num_Temperature+certification-1].num_ti>=num_ti)

									{

										tf_cert_ti_list[(tf_id-1)*Num_Temperature+certification-1].num_ti-=num_ti;

										cout<<"update tf_cert_ti,"<<(tf_id-1)*Num_Temperature+certification-1<<","<<TF[tf_id-1]<<",certification="<<certification<<",-num_ti:"<<num_ti<<endl;



									}else

									{

										cout<<"error:not enough ti for this resetup!"<<endl;

									}									

								}



								vector<int> tf_id_vector;

								for(int i=1;i<=(int)tool_setup_list[best_tool_setup_id-1].tf_id_vector.size();i++)

								{

									int tf_id=tool_setup_list[best_tool_setup_id-1].tf_id_vector[i-1];

									for(int j=1;j<=tool_setup_list[best_tool_setup_id-1].num_tool_vector[i-1];j++)

									{

										tf_id_vector.push_back(tf_id);

									}

								}

								



							whether_mach_assign=false; //We don't assign lot to this item but to a new item in the reset_mach_list;



							struct multi_mach one_mach=(* p_m);

							one_mach.lot_id_list.clear(); //regulor first-pass lot id list

							one_mach.logpoint_list.clear(); //logpoint list for regular first-pass lots

							one_mach.from_begin_to_start_time_list.clear();  //start_time of regular first-pass lots on machine

							one_mach.from_begin_to_completion_time_list.clear(); //completion time for regular first-pass lots on the machine

							one_mach.pass_no_list.clear();

							one_mach.first_pass_solution_id_list.clear();



							one_mach.whether_initial_lot=false;

							one_mach.whether_setup=true;

							one_mach.whether_resetup=false;

							one_mach.tf_id_vector=tf_id_vector;

							one_mach.certification=tool_setup_list[best_tool_setup_id-1].certification;

							one_mach.assigned_lot_list= assigned_lot_list_best_ts;

							//cout<<"one_mach.assigned_lot_list.size="<<(int)one_mach.assigned_lot_list.size()<<endl;

							one_mach.from_begin_to_completion=from_begin_to_completion;



							//Added by Zhufeng, for fixing a bug during resetup, June 10, 2012

							one_mach.whether_need_check_for_new_lot = true;

							//end by Zhufeng, June 10



							if(one_mach.tf_id_vector == (* p_m).tf_id_vector && one_mach.certification == (* p_m).certification)

							{

								cout<<"The new setup is the same as the old one, don't need to reset."<<endl;

								//added by Zhufeng, March 06, 2012 to compute correct operation time

								one_mach.from_begin_to_start=from_begin_to_setup;

								//end by Zhufeng, March 06, 2012

								

							}else

							{

								one_mach.from_begin_to_setup=from_begin_to_setup;

								//Added by Zhufeng, March 06, 2012

								one_mach.from_begin_to_start=from_begin_to_setup; //Note from_begin_to_start is just to compute the operation time of a machine. 

																				 //So it should denoting the start of the operation.Don't need to add Tooling_Setup_Time 

								//end by Zhufeng, March 06, 2012 





							}



							cout<<"Finally, from_begin_to_setup = "<<one_mach.from_begin_to_setup<<", ";

							cout<<"Finally, from_begin_to_completion = "<<one_mach.from_begin_to_completion<<endl;

							//Add by SHIHUI Jia

							setupchange_0910=1;
							changeover_num+=setupchange_0910;

							//End by Shihui JIa

							reset_mach_list.push_back(one_mach);



						   (*p_m).whether_resetup=true;



							candidate_lot_list=candidate_lot_list_temp;

							finished_lot_reset_list=finished_lot_list_temp;



							//March 25, for multi-pass production, 

							multi_device_production=multi_device_production_temp;

							Num_Pass = Num_Pass_temp;

							//March 25



							//May 23

							multi_device_production_step = multi_device_production_step_temp;

							//May 23



							}

						}

						

					}



			}//else//corresponding to if((*p_m).whether_setup) 

			(*p_m).whether_need_check_for_new_lot=whether_mach_assign;



		}//if( (*p_m).from_begin_to_completion <= current_time)

		p_m++;

	}

	vector<multi_mach>:: iterator p_temp=reset_mach_list.begin();

	while( p_temp != reset_mach_list.end())

	{

		cout<<"push back reseting machine"<<MI[(* p_temp).mi_id-1]<<endl;

		machine_list.push_back((* p_temp));

		++p_temp;



	}
	//Add by Shihui
	cout<<"The value of setupchange added in 0910 is"<<setupchange_0910<<endl;
	cout<<"THe value of whether_resetup at the end is: "<<whether_change_sj<<endl;


	cout<<"The number of total changeover in 0912 is: "<<changeover_num<<endl;

	//return whether_change_sj;
	//return setupchange_0910;
	return changeover_num;
}

//Feb 24

//Feb 25

void AT::assign_lot_reset( vector<multi_mach> & machine_list,

					vector<multi_lot> & candidate_lot_list,

					vector<multi_lot> & finished_lot_list,

					double current_time, 

					const vector<int> & min_candidate_mach_list_id_vector,

					const vector<tool_setup> tool_setup_list,

					const vector<mf_tool_setup> & mf_tool_setup_list, 

					vector<tf_cert_ti> & tf_cert_ti_list

					)

{

	cout<<"resetup current machines"<<endl;

	vector<multi_mach> reset_mach_list;

	for(int id=1;(int)id<=min_candidate_mach_list_id_vector.size();id++)

	{

		int mach_list_id=min_candidate_mach_list_id_vector[id-1];

		vector<multi_mach>::iterator p_m=machine_list.begin()+mach_list_id-1;

		cout<<"try "<<MI[(*p_m).mi_id-1]<<endl;

		vector<multi_lot>::iterator p_l=candidate_lot_list.begin();

		

			if( (*p_m).from_begin_to_completion <= current_time)

			{

				p_l=candidate_lot_list.begin();

				bool whether_mach_assign=false;

				if((*p_m).whether_setup) // This machine has been setup

				{

					int mf_id=(* p_m).mf_id;

					cout<<"process "<<MI[(* p_m).mi_id-1]<<" , at "<<current_time<<endl;

					vector<int> tf_id_vector=(* p_m).tf_id_vector;

					int certification=(* p_m).certification;



					while( p_l !=candidate_lot_list.end())

					{

						int logpoint=(* p_l).logpoint;

						int dev_id=0;

						if((* p_l).whether_initial_lot)

						{

							dev_id=initial_running_lot[(* p_l).initial_running_lot_id-1].dev_id;

						}else

						{

							dev_id=lots_device[(* p_l).lot_id];

						}



						bool whether_route=false;

						bool whether_lot_assigned=false;



						int s_id=0;

						for(int id=1;id<=(int)S.size();id++)

						{



							if(		 S[id-1].tf.tools	    == tf_id_vector

								&&  (S[id-1].logpoint		== logpoint)



								&&	(S[id-1].mg				== mf_id)

								&&	(S[id-1].device			== dev_id)

								&&	(S[id-1].temperature	== certification)	)

							{

								

								s_id=id;

								whether_route=true;

								break;

							}

						}//end of for(int id=1;id<=(int)S.size();id++)

						if(whether_route)

						{		//now check whether the time on the machine is enough

							double required_time=(double)(* p_l).quantity/S[s_id-1].PPH + Load_Unload_Time;

							double remain_time_mach=(*p_m).horizon-(*p_m).from_begin_to_completion;

							

							//cout<<"horizon="<<(*p_m).horizon<<",(*p_m).from_begin_to_completion="<<(*p_m).from_begin_to_completion<<",";



							//added in Feb 20,

							if(remain_time_mach > (*p_m).horizon - (*p_l).from_begin_to_completion )

							{

								remain_time_mach = (*p_m).horizon - (*p_l).from_begin_to_completion;

							}

							//end Feb 20



							//cout<<"remain_time_mach ="<<remain_time_mach<<endl;



							if(remain_time_mach >= required_time)

							{

								cout<<"		s ="<<s_id<<"	"<<endl;

								//cout<<"The completion of the lot="<<(*p_l).from_begin_to_completion<<endl;

								cout<<",attach this lot to mach"<<MI[(* p_m).mi_id-1]<<endl;



								//Added by Zhufeng, to compute obj value, Feb 27

								(* p_l).s_id=s_id;

								

								//Feb 27



								if((*p_l).from_begin_to_completion < (* p_m).from_begin_to_completion)

								{

									(* p_l).start_time=(* p_m).from_begin_to_completion;

									(*p_l).from_begin_to_completion=(* p_m).from_begin_to_completion+required_time;

								}else

								{

									(* p_l).start_time=(* p_l).from_begin_to_completion;

									(*p_l).from_begin_to_completion=(* p_l).from_begin_to_completion+required_time;

								}

								//(*p_l).from_begin_to_completion=(* p_m).from_begin_to_completion+required_time;

								//cout<<"Then completion="<<(*p_l).from_begin_to_completion<<endl;

								whether_mach_assign=true;



								whether_lot_assigned=true;



								(* p_m).assigned_lot_list.push_back((*p_l));



								(* p_m).from_begin_to_completion=(*p_l).from_begin_to_completion;



								finished_lot_list.push_back((*p_l));



								update_multi_device_production(multi_device_production, (*p_l),Num_Pass);



								//May 23

								update_multi_device_production_step(multi_device_production_step, (*p_l));

								//May 23



								p_l=candidate_lot_list.erase( p_l );//if erase p_l. Then p_l needn't ++;



								//Added by Zhufeng, March 28, for Update the lot benefit and re-rank the lot from p_l to the end.



								//update_lot_benefit( candidate_lot_list, multi_device_production,p_l);



								//end by Zhufeng

								

								//May 23, 2012

								update_lot_benefit( candidate_lot_list, multi_device_production_step,p_l);

								//May 23, 2012



							}



						}



						if(!whether_lot_assigned)

						{

							p_l++;

						}

					}





				}else//corresponding to if((*p_m).whether_setup) 

				{//this machine is not found in the solution.csv.Three cases:(1)no initial lot, no setup. (2)intial lot finished before start of horizon, no setup.(3)initial lot cannot be finished within horizon.

				//If the first two cases, resetup. If the third doing nothing.



					if((*p_m).whether_initial_lot && !(*p_m).whether_finish_initial)

					{

						cout<<"Machine "<<MI[(*p_m).mi_id-1]<<"cannot finish its initial lot within horizon. No resetup"<<endl;

					}else

					{

						cout<<"resetup Machine "<<MI[(*p_m).mi_id-1]<<endl;

						int best_tool_setup_id=-1;

						double best_ts_weight=0;

						vector<multi_lot> assigned_lot_list_best_ts;

						double from_begin_to_completion;

						double from_begin_to_setup;

						vector<multi_lot> candidate_lot_list_temp=candidate_lot_list; //Need to update candidate_lot_list after compute_benefit

 

						vector<multi_lot> finished_lot_list_temp=finished_lot_list;



						//March 25, for multi-pass production, 

						vector<multi_production> multi_device_production_temp=multi_device_production;

						int Num_Pass_temp=Num_Pass;

						//March 25

						//May 23

						vector<multi_production_step> multi_device_production_step_temp = multi_device_production_step;

						//May 23



						bool whether_resetup=compute_benefit_resetup_empty( * p_m,candidate_lot_list_temp,finished_lot_list_temp, current_time,best_ts_weight,best_tool_setup_id,assigned_lot_list_best_ts,tool_setup_list,mf_tool_setup_list,tf_cert_ti_list,from_begin_to_completion,from_begin_to_setup,multi_device_production_temp,Num_Pass_temp,multi_device_production_step_temp );

					    if(whether_resetup)

						{

							cout<<"resetup "<< MI[(*p_m).mi_id-1] <<"successfully!"<<endl;

							if(best_tool_setup_id==-1)

							{

								cout<<"error: no tool setup are found for reseting up!"<<endl;

							}else

							{

								vector<int> tf_id_vector;



								for(int i=1;i<=(int)tool_setup_list[best_tool_setup_id-1].tf_id_vector.size();i++)

								{

									int tf_id=tool_setup_list[best_tool_setup_id-1].tf_id_vector[i-1];

									int certification=tool_setup_list[best_tool_setup_id-1].certification;

									int num_ti=tool_setup_list[best_tool_setup_id-1].num_tool_vector[i-1];



									if(tf_cert_ti_list[(tf_id-1)*Num_Temperature+certification-1].num_ti>=num_ti)

									{

										tf_cert_ti_list[(tf_id-1)*Num_Temperature+certification-1].num_ti-=num_ti;

										cout<<"update tf_cert_ti,"<<(tf_id-1)*Num_Temperature+certification-1<<","<<TF[tf_id-1]<<",certification="<<certification<<",-num_ti:"<<num_ti<<endl;



									}else

									{

										cout<<"error:not enough ti for this resetup!"<<endl;

									}									

								}



								for(int i=1;i<=(int)tool_setup_list[best_tool_setup_id-1].tf_id_vector.size();i++)

								{

									int tf_id=tool_setup_list[best_tool_setup_id-1].tf_id_vector[i-1];

									for(int j=1;j<=tool_setup_list[best_tool_setup_id-1].num_tool_vector[i-1];j++)

									{

										tf_id_vector.push_back(tf_id);

									}

								}





								struct multi_mach one_mach=(* p_m);

								one_mach.whether_initial_lot=false;

								one_mach.whether_setup=true;

								one_mach.whether_resetup=false;

								one_mach.tf_id_vector=tf_id_vector;

								one_mach.certification=tool_setup_list[best_tool_setup_id-1].certification;

								one_mach.assigned_lot_list= assigned_lot_list_best_ts;

								cout<<"one_mach.assigned_lot_list.size="<<(int)one_mach.assigned_lot_list.size()<<endl;

								one_mach.from_begin_to_completion=from_begin_to_completion;



								//Added by Zhufeng, for fixing a bug during resetup, June 10, 2012

								one_mach.whether_need_check_for_new_lot = true;

								//end by Zhufeng, June 10



								if(one_mach.tf_id_vector == (* p_m).tf_id_vector && one_mach.certification == (* p_m).certification)

								{

									cout<<"The new setup is the same as the old one, don't need to reset."<<endl;

									//added by Zhufeng, March 06, 2012 to compute correct operation time

									one_mach.from_begin_to_start=from_begin_to_setup;

									//end by Zhufeng, March 06, 2012

								

								}else

								{

									one_mach.from_begin_to_setup=from_begin_to_setup;

									//Added by Zhufeng, March 06, 2012

									one_mach.from_begin_to_start=from_begin_to_setup;

									//end by Zhufeng, March 06, 2012 



								}

								reset_mach_list.push_back(one_mach);



								(*p_m).whether_resetup=true;



								whether_mach_assign=false;

								

								candidate_lot_list=candidate_lot_list_temp;

								finished_lot_list=finished_lot_list_temp;

								//March 25, for multi-pass production, 

								multi_device_production=multi_device_production_temp;

								Num_Pass = Num_Pass_temp;

								//March 25



							}

						}



					}

				}

				

	        	(*p_m).whether_need_check_for_new_lot=whether_mach_assign;		



			}//end of if( (*p_m).from_begin_to_completion <= current_time)

	}//end of for(int id=1;(int)id<=min_candidate_list.size();id++)

	vector<multi_mach>:: iterator p_temp=reset_mach_list.begin();

	while( p_temp != reset_mach_list.end())

	{

		cout<<"push back reseting machine"<<MI[(* p_temp).mi_id-1]<<endl;

		machine_list.push_back((* p_temp));

		++p_temp;



	}



}

//Feb 25

//Feb 26

double AT::compute_obj_multi( const vector<multi_mach> & machine_list,

							 const vector<int> & production_first_pass,

							 const vector<multi_lot> & finished_lot_list,

							 const double & weighted_item, //This record the value from firs-pass solution for the produciton weighted item.w_l[l]+eps[s];

							 const double & weighted_sum_lot_first_pass, //record the value  from firs-pass solution for the sum of weighted item w_l[l]

							 const vector<multi_production_step> multi_device_production_step,

							 const int Max_Num_Step,

							 const bool output

							 )

{

	//just consider key device not considering key package and key pin package right now

	//should include first-pass lot.

	cout<<"compute_obj_multi"<<endl;

	double obj=0.0;

	obj+=weighted_item;

	

	int num_pass=0;

	vector<multi_lot>::const_iterator p_fl=finished_lot_list.begin();

	while(p_fl!=finished_lot_list.end())

	{

		if(num_pass<(* p_fl).pass_no)

		{

			num_pass=(* p_fl).pass_no;

		}

		++p_fl;

	}



	int * p_production_per_pass=new int[num_pass]();



	int * p_key_device_production_per_pass = new int[num_pass]();



	double time_horizon=0;



	double * p_weight_sum_lot = new double[num_pass]();



	p_weight_sum_lot[0]+=weighted_sum_lot_first_pass;



	for (int id=1;id<=(int)production_first_pass.size();id++)

	{

		//note: we think id in production_first_pass is just dev_id

		//p_production[id-1]=production_first_pass[id-1];

		p_production_per_pass[0]+=production_first_pass[id-1];

		int type=is_KD_PD(id);

		if(type ==1)

		{

			p_key_device_production_per_pass[0]+=production_first_pass[id-1]; 

		}



	}



	vector<multi_mach>::const_iterator p_m=machine_list.begin();

	while(p_m != machine_list.end())

	{

		vector<multi_lot>::const_iterator p_l =(* p_m).assigned_lot_list.begin();

		while(p_l != (* p_m).assigned_lot_list.end())

		{

			obj+=-(* p_l).weight+eps[(* p_l).s_id];

			int dev_id=0;

			if((* p_l).whether_initial_lot)

			{

				dev_id=initial_running_lot[(* p_l).initial_running_lot_id-1].dev_id;

			}else

			{

				dev_id=lots_device[(* p_l).lot_id];

			}

			int pass_no=(* p_l).pass_no;



			if( is_KD_PD(dev_id)==1)

			{

				p_key_device_production_per_pass[pass_no-1]+=(* p_l).quantity;

			}



			p_production_per_pass[pass_no-1]+=(* p_l).quantity;



			p_weight_sum_lot[pass_no-1]+=(* p_l).weight;

			

			++p_l;

			

		}



		if((*p_m).from_begin_to_completion>=time_horizon)

		{

			time_horizon=(*p_m).from_begin_to_completion;

		}



		++p_m;



	}



	//May 23, 2012



	//May 23, 2012,

	int shortage_total = 0;

	double weighted_shortage_total = 0;

	int shortage_final_step = 0;

	double weighted_shortage_final_step = 0;



	for(int step_id =1; step_id<= Max_Num_Step;step_id++)

	{

		for(int id=1;id<=(int)Key_Devices.size();id++){

			int d=Key_Devices[id-1];

			//Added by Zhufeng, for shortage only for existing steps,May 29,2012,

			if(multi_device_production_step[(step_id-1)*Num_Device+d-1].whether_exist)

			{

				//end by May 29,2012

				

				int shortage=n_k_minchips[d]-multi_device_production_step[(step_id-1)*Num_Device+d-1].production;

				if(shortage>0){

					obj+=(double)shortage/coef*penalty[d];

					shortage_total+=shortage;

					weighted_shortage_total+=(double)shortage/coef*penalty[d];

					if(multi_device_production_step[(step_id-1)*Num_Device+d-1].whether_final_step)

					{

						shortage_final_step+=shortage;

						weighted_shortage_final_step+=(double)shortage/coef*penalty[d];

					}





				}//end of if(shorage > 0)

				//Added by May 29, 2012

			}

			//end by May 29, 2012

		}//end of for(int id=1;id<=(int)Key_Devices.size();id++){

	}



	

//Added by Zhufeng, Aug 12, 2012, KP and KPP

	//Only if the last step of a device is done, it is counted as finished package, 

	//and as production the KP when it is KP.

	//The same to KPP

	int total_shortage_KP = 0;

	double total_weighted_shortage_KP = 0.0;





	int total_shortage_KPP = 0;

	double total_weighted_shortage_KPP = 0.0;



	int Num_PKG = (int)PKG.size();

	int Num_PIN_PKG = (int)PKG.size()*(int)PIN_order.size()+1;

	

	vector<int> PKG_production( Num_PKG,0);

	vector<int> PIN_PKG_production( Num_PIN_PKG,0);



	for( int step_id=1;step_id<= Max_Num_Step;step_id++)

	{

		for(int d=1;d<=Num_Device;d++){

			if(multi_device_production_step[(step_id-1)*Num_Device+d-1].whether_exist && multi_device_production_step[(step_id-1)*Num_Device+d-1].whether_final_step){

				int pkg_id=device_PKG[d];

				int pin_num=device_PIN[d];

				vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);

				int pin_order=pfind-PIN_order.begin()+1;

				int index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;

				PKG_production[pkg_id-1]+=multi_device_production_step[(step_id-1)*Num_Device+d-1].production;

				PIN_PKG_production[index]+=multi_device_production_step[(step_id-1)*Num_Device+d-1].production;

			}

		}

	}



//now compute the penalties

		for(int kp=1;kp<=(int)Key_PKG.size();kp++){

			int pkg_id=Key_PKG[kp-1];

			int diff=n_kp_minchips[pkg_id]-PKG_production[pkg_id-1];

			

			if(diff>0){

				obj+=diff/coef*PKG_penalty[pkg_id];

				total_shortage_KP+=diff;

				total_weighted_shortage_KP+=diff/coef*PKG_penalty[pkg_id];

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

			if(diff>0){

				obj+=diff/coef*get_PIN_PKG_penalty(pkg_id,pin_num);

				total_shortage_KPP+=diff;

				total_weighted_shortage_KPP+=diff/coef*get_PIN_PKG_penalty(pkg_id,pin_num);

			}

		}



//end by Zhufeng, Aug 12, 2012



	if(output)

	{

		fstream fcout("Output/resultsummery.txt", ios::out|ios::app);
		double total_weight_sum_lot = 0;
		fcout<<endl<<"weighted sum lots per pass:"<<endl;
			for( int pass_no=1;pass_no<=num_pass;pass_no++)
		{
			fcout<<"pass no:"<<pass_no<<",weighted sum lots:"<< p_weight_sum_lot[pass_no-1]<<endl;
			total_weight_sum_lot += p_weight_sum_lot[pass_no-1];
		}

		fcout<<endl<<"total production per pass:"<<endl;
		for( int pass_no=1;pass_no<=num_pass;pass_no++)
		{
			fcout<<"pass no:"<<pass_no<<",production:"<< p_production_per_pass[pass_no-1]<<endl;
		}

		fcout<<endl<<"total key device production per pass:"<<endl;
		for( int pass_no=1;pass_no<=num_pass;pass_no++)
		{
			fcout<<"pass no:"<<pass_no<<",key device production:"<< p_key_device_production_per_pass[pass_no-1]<<endl;
		}

		fcout<<endl
			<<"total weighted sum lots = "<<total_weight_sum_lot<<endl
			<<"total key device shortage: "<<shortage_total<<endl
			<<"total weighted shortage: "<<weighted_shortage_total <<endl
			<<"total finished key device shortage: "<<shortage_final_step<<endl
			<<"total finished weighted key device shortage: "<<weighted_shortage_final_step<<endl
	//Added by Zhufeng, Aug 12, 2012, KP and KPP.	

			<<"total shortage of all key package = "<<total_shortage_KP<<endl

			<<"total weighted shortage of all key pin package = "<<total_weighted_shortage_KP<<endl	

			<<"total shortage of all key pin package = "<<total_shortage_KPP<<endl

			<<"total weighted shortage of all key pin package = "<<total_weighted_shortage_KPP<<endl;	

	//end by Zhufeng, Aug 12, 2012

		fcout<<endl<<"time_span="<<time_horizon<<endl;
		//fcout<<"number of machine used="<<num_machine<<endl;
		fcout.close();

	}



	//delete [] p_production;

	//delete [] p_weight;

	//delete [] p_short;

	delete [] p_weight_sum_lot;

	//delete [] p_weight_short;

	delete [] p_production_per_pass;

	//delete [] p_production_step;

	delete [] p_key_device_production_per_pass;

	return obj;

}



void AT::output_multi(const vector<multi_mach> & machine_list,const vector<solution> & first_pass_solution)

{

	/*cout<<"test get_logpoint"<<endl;

	cout<<"dev_id = "<<18<<",logpoint = "<<7103<<endl;

	cout<<get_num_step_left(18,7103)<<endl;

	exit(0);*/

	cout<<"output the multi-pass solution."<<endl;

	ofstream fout("Output/multi_solution.csv");

	fout<<"Machine_Instance,Machine_Family_Name,Lot_Name,Logpoint,Quantity,Lot_Weight,Initial_Lot_Flag,Device_Name,Tooling_Family_Name,Certification,Setup_Time,Start_Time,Completion_Time,Pass_No,Passes_to_go,Finished"<<endl;

	//first output initial lots.

	for(int j=1;j<=(int)initial_running_lot.size();j++){

		if(initial_running_lot[j-1].time_remain>0){//Only the valid initial lot is output

			//It is okay that the lot will go across the whole horizon. It will still be output but not be counted in the total throughput

			int i=initial_running_lot[j-1].machine_id;

			  int mg=machine_mg[i];
			//added by Zhufeng,April 09, 2013, show initial tooling setup

			int Lambda_id = Machine_vector[i-1].initial_tooling_setup;

			vector<int> initial_tooling_vector = Lambda[Lambda_id-1].tooling_vec;

			int initial_temperature = Lambda[Lambda_id-1].temperature;

			//end by Zhufeng, April 09, 2013

			  //Added by Zhufeng, Dec 21, 2013
			  int pass_to_go = get_num_step_left(initial_running_lot[j-1].dev_id,initial_running_lot[j-1].logpoint);
			struct date_time dt=increase_date_time(General_Snapshoot_time,initial_running_lot[j-1].time_remain);

			output_date_time(dt);



			struct date_time start_time=initial_running_lot[j-1].Start_time;

			output_date_time(start_time);

			//Note that there is neither tooling info nor certification info for the initial lots,so leave it blank

			for(vector<int>::const_iterator p = initial_tooling_vector.begin();p!=initial_tooling_vector.end();++p){

				int tf_id = (*p);

				if(p==initial_tooling_vector.begin()){

					fout<<initial_running_lot[j-1].machine_name<<","<<MF[mg-1]<<","<<initial_running_lot[j-1].lot_name<<","<<initial_running_lot[j-1].logpoint<<","<<initial_running_lot[j-1].n_l_chips

					    <<","<<initial_running_lot[j-1].w_l<<",Y,"<<initial_running_lot[j-1].dev_name<<","<<TF[tf_id-1]<<","<<initial_temperature<<","<<General_Snapshoot_time.str<<","<<start_time.str<<","<<dt.str<<","<<1<<","<<pass_to_go<<endl;

				}else{

				  fout<<",,,,,,Y,,"<<TF[tf_id-1]<<","<<initial_temperature<<",,,,"<<1<<pass_to_go<<endl;

				}

			}

		}

	}



	//Then output all regular lots (multi-pass) by machines

	for(int id=1;id<=(int)machine_list.size();id++)

	{

		//vector<int>::const_iterator p_l= machine_list[id-1].lot_id_list.begin(); //regulor first-pass lot id list

		//vector<int>::const_iterator p_lp= machine_list[id-1].logpoint_list.begin();  //logpoint list for regular first-pass lots

		//vector<double>::const_iterator p_start= machine_list[id-1].begin();

		//vector<double> from_begin_to_start_time_list;  //start_time of regular first-pass lots on machine

		//vector<double> from_begin_to_completion_time_list; //completion time for regular first-pass lots on the machine

		//vector<int> pass_no_list;



		struct date_time setup_time=increase_date_time(General_Snapshoot_time,machine_list[id-1].from_begin_to_setup);

		output_date_time(setup_time);



		vector<int> tf_id_vector=machine_list[id-1].tf_id_vector;



		int certification=machine_list[id-1].certification;



		for(int i=1;i<=(int)machine_list[id-1].first_pass_solution_id_list.size();i++)

		{

			//int lot_id=machine_list[id-1].lot_id_list[i-1];

			//int logpoint=machine_list[id-1].logpoint_list[i-1];

			int first_pass_solution_id=machine_list[id-1].first_pass_solution_id_list[i-1];

			int lot_id=first_pass_solution[first_pass_solution_id-1].lot_id;

			int logpoint=first_pass_solution[first_pass_solution_id-1].logpoint;

			int quantity=first_pass_solution[first_pass_solution_id-1].quantity;

			double lot_wegiht=first_pass_solution[first_pass_solution_id-1].weight;

			int dev_id=	first_pass_solution[first_pass_solution_id-1].dev_id;

			//int tf_id= first_pass_solution[first_pass_solution_id-1].tf_id;

			//int certification= first_pass_solution[first_pass_solution_id-1].temperature;



			//Feb 18, 2013. The starting time of the first lot = the setup time + 1/2 hour(setup time)

			double tool_setup_time = 0;

			if(i==1){

				for(int id=1;id<= tf_id_vector.size();id++)

				{

					//cout<<"id = "<<tf_id_vector[id-1]<<endl;

					tool_setup_time+=Tooling_Setup_Time[tf_id_vector[id-1]];

				}

				//cout <<"tool_setup_time = "<<tool_setup_time<<endl;		

			}



			struct date_time start_time=increase_date_time(General_Snapshoot_time,first_pass_solution[first_pass_solution_id-1].from_begin_to_start_time + tool_setup_time);

			output_date_time(start_time);



			struct date_time completion_time=first_pass_solution[first_pass_solution_id-1].completion_time;

			output_date_time(completion_time);

			

			//Added by Zhufeng, Feb 2014

			string finished = "Y";

			double completion_time_double = get_date_time_difference(completion_time,General_Snapshoot_time);

			if(completion_time_double>H[machine_list[id-1].mi_id])

			{

				finished = "N";

			}

			//end by Zhufeng, Feb 2014



			int pass_no=first_pass_solution[first_pass_solution_id-1].pass_no;

			//cout<<"pass_no = "<<pass_no<<endl;


			//Added by Zhufeng Dec 20, 2013
			int num_pass_to_go = get_num_step_left(dev_id,logpoint);


			for( int j=1;j<=tf_id_vector.size();j++)

			{

				int tf_id=tf_id_vector[j-1];

				if(j==1)

				{

					fout<<MI[machine_list[id-1].mi_id-1]<<","<<MF[machine_list[id-1].mf_id-1]<<","<<LOT[lot_id-1]<<","<<logpoint

					    <<","<<quantity<<","<<lot_wegiht<<",N,"<<DEV[dev_id-1]<<","<<TF[tf_id-1]<<","<<certification<<","<<setup_time.str<<","<<start_time.str<<","<<completion_time.str<<","<<pass_no<<","<<num_pass_to_go<<","<<finished<<endl;

				}else
				{

					fout<<" , , , , , , , ,"<<TF[tf_id-1]<<", , , , , "<<endl;

				}

			}//end of for( int j=1;j<=tf_id_vector.size();j++)

		}//end of for(int i=1;i<=(int)machine_list[id-1].first_pass_solution_id_list.size();i++)



		//Then output second-pass, or more pass lots

		for( int i=1;i<=(int)machine_list[id-1].assigned_lot_list.size();i++)

		{

			const char * lot_name;

			const char * dev_name;

			struct multi_lot one_lot=machine_list[id-1].assigned_lot_list[i-1];

			  int dev_id = 0;
			if(one_lot.whether_initial_lot)

			{

				lot_name=initial_running_lot[one_lot.initial_running_lot_id-1].lot_name;

				dev_name=initial_running_lot[one_lot.initial_running_lot_id-1].dev_name;


				  dev_id = initial_running_lot[one_lot.initial_running_lot_id-1].dev_id;
			}else

			{

				lot_name=LOT[one_lot.lot_id-1];

				dev_id=lots_device[one_lot.lot_id];

				dev_name=DEV[dev_id-1];

			}

			int logpoint = one_lot.logpoint;

			int quantity = one_lot.quantity;

			double lot_wegiht= one_lot.weight;



			struct date_time start_time=increase_date_time(General_Snapshoot_time,one_lot.start_time);

			output_date_time(start_time);



			struct date_time completion_time=increase_date_time(General_Snapshoot_time,one_lot.from_begin_to_completion);

			output_date_time(completion_time);



						

			//Added by Zhufeng, Feb 2014

			string finished = "Y";

			double completion_time_double = get_date_time_difference(completion_time,General_Snapshoot_time);

			if(one_lot.from_begin_to_completion>H[machine_list[id-1].mi_id])

			{

				finished = "N";

			}

			//end by Zhufeng, Feb 2014

			

			int pass_no=one_lot.pass_no;



			  int pass_to_go = 0;
			  pass_to_go = get_num_step_left(dev_id,logpoint);

			for( int j=1;j<=tf_id_vector.size();j++)

			{

				int tf_id=tf_id_vector[j-1];

				if(j==1)

				{

					fout<<MI[machine_list[id-1].mi_id-1]<<","<<MF[machine_list[id-1].mf_id-1]<<","<<lot_name<<","<<logpoint

					    <<","<<quantity<<","<<lot_wegiht<<",N,"<<dev_name<<","<<TF[tf_id-1]<<","<<certification<<","<<setup_time.str<<","<<start_time.str<<","<<completion_time.str<<","<<pass_no<<","<<pass_to_go<<","<<finished<<endl;

				}else

				{

					fout<<" , , , , , , , ,"<<TF[tf_id-1]<<", , , , , "<<endl;

				}

			}//end of for( int j=1;j<=tf_id_vector.size();j++)

	

		}//end of for( int i=1;i<=(int)machine_list[id-1].assigned_lot_list.size();i++)



	}



	ofstream f_time("Output/multi_machine_time.csv");

	f_time<<"Machine,Time"<<endl;

	vector<double> mach_time; //mach_time[mi_id-1]is the operation time for mi_id

	mach_time.resize((int)MI.size(),0);

	for(int id=1;id<=(int)Machine_vector.size();id++)

	{

		if(Machine_vector[id-1].hasInitialLot)

		{

			int initial_lot=Machine_vector[id-1].initial_lot;

			int mi_id=Machine_vector[id-1].id;

			cout<<"time_remain="<<initial_running_lot[initial_lot].time_remain<<endl;

			if(initial_running_lot[initial_lot].time_remain>=0)

			{

			mach_time[mi_id-1]+=initial_running_lot[initial_lot].time_remain;

			}

		}

	}

	for(int id=1;id<=(int)machine_list.size();id++)

	{

		int mi_id=machine_list[id-1].mi_id;



			if(machine_list[id-1].whether_initial_lot && !machine_list[id-1].whether_finish_initial )

			{

				cout<<MI[machine_list[id-1].mi_id-1]<<" cannot finish its initial lot within time horizon."<<endl;

				mach_time[mi_id-1]=H[machine_list[id-1].mi_id];

			}else

			{

				cout<<MI[mi_id-1]<<",from_begin_to_completion = "<<machine_list[id-1].from_begin_to_completion<<",from_begin_to_setup = "<<machine_list[id-1].from_begin_to_setup<<", from_begin_to_start = "<<machine_list[id-1].from_begin_to_start<<endl;

				mach_time[mi_id-1]+=(machine_list[id-1].from_begin_to_completion-machine_list[id-1].from_begin_to_start);

				//cout<<"mi_id="<<mi_id<<", mach_time="<<mach_time[mi_id-1]<<endl;

			}



			

	}



	int num_mach_used=0;

	double mach_time_max=0;

	double mach_time_min=100000;

	double mach_time_average=0;



	for(int id=1;id<=(int)mach_time.size();id++)

	{

		if(mach_time[id-1] > mach_time_max)

		{

			mach_time_max=mach_time[id-1];

		}

		if(mach_time[id-1] < mach_time_min)

		{

			mach_time_min=mach_time[id-1];

		}

		mach_time_average+=mach_time[id-1];



		if(mach_time[id-1] > 0)

		{

			num_mach_used++;

		}



		f_time<<MI[id-1]<<","<<mach_time[id-1]<<endl;

	}



	mach_time_average=(double)mach_time_average/(double)mach_time.size();



	ofstream f_summery("Output/resultsummery.txt", ios::out | ios::app);



	f_summery<<endl<<"num_machine used = "<<num_mach_used<<endl;

	f_summery<<"max maching time = "<<mach_time_max<<endl;

	f_summery<<"min maching time = "<<mach_time_min<<endl;

	f_summery<<"average maching time = "<<mach_time_average<<endl;

};

//end by Zhufeng, Jan 20, 2012

//Added by Zhufeng for multi-pass production, March 25, 2012

void AT::initialize_multi_device_production( const vector<int> & production_first_pass,vector<multi_production> & multi_device_production)

{



	cout<<"initialize multi_device_production"<<endl;



	//multi_device_production.clear();

	

	//vector<int>production_first_pass::const_iterator p_p = production_first_pass.begin();

	for(int i=1; i<= production_first_pass.size();i++)

	{

		struct multi_production one_device_production;

		one_device_production.dev_id=i;

		one_device_production.pass_no=1;

		one_device_production.production=production_first_pass[i-1];

		multi_device_production.push_back(one_device_production);



	}



	int diff = Num_Pass -1;

	for( int i=1;i<=diff;i++)

		{

			int pass_no= i+1;

			for(int j=1;j<=Num_Device;j++)

			{

				struct multi_production one_device_production;

				one_device_production.pass_no=pass_no;

				one_device_production.dev_id=j;

				one_device_production.production=0;

				multi_device_production.push_back(one_device_production);

			}

		}

};



void AT::update_multi_device_production( vector<multi_production> & multi_device_production, const struct multi_lot & one_lot, int & Num_Pass)

{

	//Update the multi_device_production and Num_Pass because one lot is assigned to some machine

	cout<<"update_multi_device_production"<<endl;

	if(one_lot.pass_no > Num_Pass)

	{

		cout<<"New pass considered,"<<one_lot.pass_no<<endl;

		int diff = one_lot.pass_no - Num_Pass;

		

		for( int i=1;i<=diff;i++)

		{

			int pass_no=Num_Pass+i;

			for(int j=1;j<=Num_Device;j++)

			{

				struct multi_production one_device_production;

				one_device_production.pass_no=pass_no;

				one_device_production.dev_id=j;

				one_device_production.production=0;

				multi_device_production.push_back(one_device_production);

			}

		}

		Num_Pass = one_lot.pass_no;

	}



	int dev_id=0;



	if(one_lot.whether_initial_lot)

	{

		dev_id=initial_running_lot[one_lot.initial_running_lot_id-1].dev_id;

	}else

	{

		dev_id=lots_device[one_lot.lot_id];

	}



	int id = (one_lot.pass_no-1)*Num_Device + dev_id;

	multi_device_production[id-1].production+=one_lot.quantity;

};

void AT::show_multi_device_production( const vector<multi_production> & multi_device_production )

{

	ofstream fout("Debug/multi_device_production.csv");

	fout<<"Total pass no:"<<Num_Pass<<endl;

	fout<<"Device,Pass_No,Production"<<endl;

	//cout<<"Device id,Pass_NO,Production"<<endl;

	int production_per_pass[Num_Pass];

	int short_per_pass[Num_Pass];



	for(int i=1;i<=Num_Pass;i++)

	{

		production_per_pass[i-1]=0;

		short_per_pass[i-1]=0;

	}

	

	for( int i=1;i<= (int)multi_device_production.size();i++)

	{

		fout<<DEV[multi_device_production[i-1].dev_id-1]<<","<<multi_device_production[i-1].pass_no<<","<<multi_device_production[i-1].production<<endl;

		//cout<<multi_device_production[i-1].dev_id<<","<<multi_device_production[i-1].pass_no<<","<<multi_device_production[i-1].production<<endl;

		production_per_pass[multi_device_production[i-1].pass_no-1]+=multi_device_production[i-1].production;

	}

	fout<<"total production per pass,pass_no"<<endl;



	for(int i=1;i<=Num_Pass;i++)

	{

		fout<<production_per_pass[i-1]<<","<<i<<endl;

	}

	

	for( int pass_no=1;pass_no<=Num_Pass;pass_no++)

	{

		for(int id=1;id<=(int)Key_Devices.size();id++){

			int d=Key_Devices[id-1];

			int shortage=n_k_minchips[d]-multi_device_production[(pass_no-1)*Num_Device+d-1].production;

			if(shortage>0)

			{

				//obj+=shortage/coef*penalty[d];

				//p_weight[pass_no-1]+=shortage/coef*penalty[d];

				short_per_pass[pass_no-1]+=shortage;

				//p_weight_short[pass_no-1]+=shortage/coef*penalty[d];

			}

		}

	}



	fout<<"total keydevice shortage per pass,pass_no"<<endl;

	for(int i=1;i<=Num_Pass;i++)

	{

		fout<<short_per_pass[i-1]<<","<<i<<endl;

	}



}

//end by Zhufeng, March 25, 2012

//May 22, 2012, for multi-production steps

void AT::initialize_multi_device_production_step( vector<multi_production_step> & multi_device_production, 

												  const vector<multi_lot> & unassigned_lot_first_pass_vector,

												  const vector<multi_lot> & first_pass_lot_vector,

												  const int Max_Num_Step)

{

	multi_device_production.clear();

	multi_device_production.reserve(Max_Num_Step*(int)DEV.size());

	cout<<"initialize multi_device_production_step by 0"<<endl;

	

	for(int step_id=1;step_id<= Max_Num_Step;step_id++)

	{

		for(int dev_id =1;dev_id <= (int)DEV.size();dev_id++)

		{

			struct multi_production_step one_device_production;

			one_device_production.whether_exist = false;

			one_device_production.whether_final_step = false;

			one_device_production.logpoint = 0;

			one_device_production.dev_id = dev_id;

			one_device_production.step_id = step_id;

			one_device_production.production = 0; 

			if(step_id <= (int)multipass_vector[dev_id-1].logpoint_list.size())

			{

				one_device_production.whether_exist = true;

				one_device_production.logpoint = multipass_vector[dev_id-1].logpoint_list[step_id-1];

				if(step_id == (int)multipass_vector[dev_id-1].logpoint_list.size())

				{

					one_device_production.whether_final_step = true;

				}

			}	

			multi_device_production.push_back(one_device_production);			

		}//end of for(int dev_id =1;dev_id <= (int)DEV.size();dev_id++)

	}



	cout<<"initialize multi_device_production_step by unassigned_lot_first_pass_vector"<<endl;

	for(int i=1;i<=unassigned_lot_first_pass_vector.size();i++)

	{

		if(unassigned_lot_first_pass_vector[i-1].whether_initial_lot)

		{

			//Note,because of the process of initializing unassigned_lot_first_pass_vector,no initial_lot_id is given.

			//So we cannot do anything.

			cout<<"Error! Initial lots are already assigned,should not be in unassigned_lot_first_pass_vector"<<endl;

		}else

		{

			//else for if(unassigned_lot_first_pass_vector[i-1].whether_initial_lot)

			int lot_id = unassigned_lot_first_pass_vector[i-1].lot_id;

			int dev_id = lots_device[lot_id];

			int logpoint = unassigned_lot_first_pass_vector[i-1].logpoint;

			int quantity = unassigned_lot_first_pass_vector[i-1].quantity;

			int upcoming_step_id = get_step_id(dev_id,logpoint);

			ofstream fout("Debug/step_id_for_unassigned_lot.txt",ios::out|ios::app);

			fout<<LOT[lot_id-1]<<", logpoint = "<<logpoint<<",upcoming_step_id = "<<upcoming_step_id<<endl;

			fout.close();



			for(int step_id =1;step_id < upcoming_step_id;step_id++)

			{

				//all the step before the upcoming step should be counted in the production but not the coming step since this is unassigned lot

				multi_device_production_step[(step_id-1)*DEV.size() + dev_id-1].production+=quantity;



			}

		}//end of if(unassigned_lot_first_pass_vector[i-1].whether_initial_lot) else

	}//end of for(int i=1;i<=unassigned_lot_first_pass_vector.size();i++)



	cout<<"initialize multi_device_production_step by first_pass_lot_vector"<<endl;

	for(int i=1;i<=first_pass_lot_vector.size();i++)

	{

		int dev_id = 0;

		if(!first_pass_lot_vector[i-1].whether_initial_lot)

		{					

			int lot_id = first_pass_lot_vector[i-1].lot_id;

			dev_id = lots_device[lot_id];

		}else

		{

			int initial_running_lot_id = first_pass_lot_vector[i-1].initial_running_lot_id;

			dev_id = initial_running_lot[initial_running_lot_id-1].dev_id;

		}

			int logpoint = first_pass_lot_vector[i-1].logpoint;

			int quantity = first_pass_lot_vector[i-1].quantity;

			int upcoming_step_id = get_step_id(dev_id,logpoint);

			//ofstream fout("Debug/step_id_for_unassigned_lot.txt",ios::out|ios::app);

			//fout<<LOT[lot_id-1]<<", logpoint = "<<logpoint<<",upcoming_step_id = "<<upcoming_step_id<<endl;

			//fout.close();



			for(int step_id =1;step_id <= upcoming_step_id;step_id++)

			{

				//all the step before the upcoming step should be counted in the production and the coming step since this is assigned lot

				multi_device_production_step[(step_id-1)*DEV.size() + dev_id-1].production+=quantity;



			}

	}//end of for(int i=1;i<=unassigned_lot_first_pass_vector.size();i++)



}



int AT::get_step_id( const int dev_id, const int logpoint)

{

	if(dev_id<1)

	{

		cout<<"Error: dev_id <1 in get_step_id()"<<endl;

		return 0;

	}

	for(int i=1;i<=multipass_vector[dev_id-1].logpoint_list.size();i++)

	{

		if(logpoint == multipass_vector[dev_id-1].logpoint_list[i-1])

		{

			return i;

		}

	}

	cout<<"Error; no step id is found for device id "<<dev_id<<", and logpoint "<<logpoint<<endl;

	return 0;

}



void AT::update_multi_device_production_step( vector<multi_production_step> & multi_device_production_step, const struct multi_lot & one_lot)

{

	//when the lot one_lot is assigned, update the production in the multi_device_production

	int dev_id = 0;

	if(one_lot.whether_initial_lot)

	{

		dev_id = initial_running_lot[one_lot.initial_running_lot_id-1].dev_id;

	}else

	{

		dev_id = lots_device[one_lot.lot_id];

	}

	int logpoint = one_lot.logpoint;

	int step_id = get_step_id(dev_id, logpoint);

	multi_device_production_step[(step_id-1)*DEV.size() + dev_id-1].production+=one_lot.quantity;



}

void AT::show_multi_device_production_step( vector<multi_production_step> & multi_device_production_step, const int Max_Num_Step)

{

	ofstream fout("Debug/multi_device_production_step.csv");

	fout<<"dev_id,dev_id in vector,DEV, Step, Logpoint,Production,Whether_Exist,Whether_Final"<<endl;

	for( int dev_id = 1;dev_id <= (int)DEV.size();dev_id++)

	{

		for( int step_id =1; step_id <= Max_Num_Step;step_id++)

		{

		fout<<dev_id<<","<<multi_device_production_step[(step_id-1)*DEV.size() + dev_id-1].dev_id<<","

			<<DEV[dev_id-1]<<","<<multi_device_production_step[(step_id-1)*DEV.size() + dev_id-1].step_id<<","

			<<multi_device_production_step[(step_id-1)*DEV.size() + dev_id-1].logpoint<<","<<multi_device_production_step[(step_id-1)*DEV.size() + dev_id-1].production<<",";

		if(multi_device_production_step[(step_id-1)*DEV.size() + dev_id-1].whether_exist)

			fout<<"Y";

		else

			fout<<"N";

		fout<<",";

		if(multi_device_production_step[(step_id-1)*DEV.size() + dev_id-1].whether_final_step)

			fout<<"Y";

		else

			fout<<"N";

		fout<<endl;

		}

	}

	  fout.close();
	  //Added by Zhufeng, Dec 21, 2013
	fout.open("Output/multi_device_production.csv");

	fout<<"DEV, Step, Logpoint,Production,Keydevice,Shortage(final step)"<<endl;

	for( int dev_id = 1;dev_id <= (int)DEV.size();dev_id++)

	{

		for( int step_id =1; step_id <= Max_Num_Step;step_id++)

		{

		  if(multi_device_production_step[(step_id-1)*DEV.size() + dev_id-1].whether_exist)
		    {
		      fout<<DEV[dev_id-1]<<","<<multi_device_production_step[(step_id-1)*DEV.size() + dev_id-1].step_id<<","

			<<multi_device_production_step[(step_id-1)*DEV.size() + dev_id-1].logpoint<<","<<multi_device_production_step[(step_id-1)*DEV.size() + dev_id-1].production;
		      vector<int>::const_iterator p = find(Key_Devices.begin(),Key_Devices.end(),dev_id);
		      if(p!=Key_Devices.end())
			{
			  fout<<",Y,";
			  if(multi_device_production_step[(step_id-1)*DEV.size() + dev_id-1].whether_final_step)
			    {
			      fout<<n_k_minchips[dev_id]-multi_device_production_step[(step_id-1)*DEV.size() + dev_id-1].production;
			    }
			  else
			    {
			      fout<<",";
			    }
			  fout<<endl;
			}
		      else
			{
			  fout<<",N,,"<<endl;
			}
		    }

		}

	}

	  fout.close();

}

//March 23, 2012, Zhufeng, for benefit value

void AT::compute_lot_benefit( vector<multi_lot> & candidate_lot_list, 

							 //const vector<multi_production> & multi_device_production, 

							 const vector<multi_production_step> & multi_device_production_step)

{

//May 23, compute the benefit_step. Try using benefit_step instead of benefit



//Added by Zhufeng, Aug 12, 2012, KP and KPP

//Only if the last step of a device is done, it is counted as finished package, 

//and as production the KP when it is KP.

//The same to KPP

	int Num_PKG = (int)PKG.size();

	int Num_PIN_PKG = (int)PKG.size()*(int)PIN_order.size()+1;

	

	vector<int> PKG_production( Num_PKG,0);

	vector<int> PIN_PKG_production( Num_PIN_PKG,0);

	

	const int Num_Step = Max_Num_Step;

	//Compute production of KP and KPP first

	for( int step_id=1;step_id<= Num_Step;step_id++)

	{

		for(int d=1;d<=Num_Device;d++){

			if(multi_device_production_step[(step_id-1)*Num_Device+d-1].whether_exist && multi_device_production_step[(step_id-1)*Num_Device+d-1].whether_final_step){

				int pkg_id=device_PKG[d];

				int pin_num=device_PIN[d];

				vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);

				int pin_order=pfind-PIN_order.begin()+1;

				int index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;

				PKG_production[pkg_id-1]+=multi_device_production_step[(step_id-1)*Num_Device+d-1].production;

				PIN_PKG_production[index]+=multi_device_production_step[(step_id-1)*Num_Device+d-1].production;

			}

		}

	}

//end by Zhufeng, Aug 12, 2012, KP and KPP





	cout<<"compute benefit"<<endl;

	for(int i=1;i<=(int)candidate_lot_list.size();i++)

	{

		candidate_lot_list[i-1].benefit=candidate_lot_list[i-1].weight;



		int dev_id=0;



		if(!candidate_lot_list[i-1].whether_initial_lot)

		{

			dev_id=lots_device[candidate_lot_list[i-1].lot_id];

		}else

		{

			dev_id=initial_running_lot[candidate_lot_list[i-1].initial_running_lot_id-1].dev_id;

		}



		int logpoint = candidate_lot_list[i-1].logpoint;



		int step_id = get_step_id(dev_id,logpoint); 



		int type=is_KD_PD(dev_id);



		if(type==1 ) //this is a key device

		{

			//Added by Zhufeng, for shortage only for existing steps,May 29,2012,

			if(multi_device_production_step[(step_id-1)*Num_Device+dev_id-1].whether_exist)

			{

				//end by May 29,2012

				//int pass_no=candidate_lot_list[i-1].pass_no;



				//int shortage=n_k_minchips[dev_id]-multi_device_production[(pass_no-1)*Num_Device+dev_id-1].production;

				int shortage = n_k_minchips[dev_id]- multi_device_production_step[(step_id-1)*Num_Device+dev_id-1].production;

				//cout<<",weight="<<candidate_lot_list[i-1].weight<<"shortage = "<<shortage;

				if(shortage>0)

				{

					//modified by Zhufeng, April, 29, 2012

					int min=candidate_lot_list[i-1].quantity;

					if(min>shortage){ min=shortage;}

					//cout<<",min="<<min<<",penalty = "<<(double)penalty[dev_id]/coef*min;

					candidate_lot_list[i-1].benefit+=(double)penalty[dev_id]/coef*min;

					//end by Zhufengm April, 29, 2012

				}

				//Added by May 29, 2012

			}

			//end by May 29, 2012

			

		}//end of if(type==1 ) 



//Added by Zhufeng, Aug 12, 2012, KP and KPP

//Only if the last step of a device is done, it is counted as finished package, 

//and as production the KP when it is KP.

//The same to KPP		

		int pkg_id=device_PKG[dev_id];

		int pin_num=device_PIN[dev_id];

		bool is_kp=is_Key_PKG(dev_id);

		bool is_kpp=is_Key_PIN_PKG(dev_id);



		if(is_kp && PKG_production[pkg_id-1]<n_kp_minchips[pkg_id]){//if the device contains a key package and it is short

			int diff=n_kp_minchips[pkg_id]-PKG_production[pkg_id-1];

			int min=candidate_lot_list[i-1].quantity;

			if(min>diff){ min=diff;}

			candidate_lot_list[i-1].benefit+=(double)PKG_penalty[pkg_id]/coef*min;

		}

	

		if( is_kpp){

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



			if(PIN_PKG_production[index]<target){

				int diff=target-PIN_PKG_production[index];

				int min=candidate_lot_list[i-1].quantity;

				if(min>diff){ min=diff;}

				candidate_lot_list[i-1].benefit+=(double)get_PIN_PKG_penalty(pkg_id,pin_num)/coef*min;

			}

		}



//end by Zhufeng, Aug 12, 2012, KP and KPP





	}//end of for(int i=1;i<=(int)candidate_lot_list.size();i++)

}



void AT::update_lot_benefit( vector<multi_lot> & candidate_lot_list, 

							//const vector<multi_production> & multi_device_production,

							const vector<multi_production_step> & multi_device_production_step,

							vector<multi_lot>::iterator p_l)

{

	//cout<<"update lot benefit"<<endl;

//Added by Zhufeng, Aug 12, 2012, KP and KPP

//Only if the last step of a device is done, it is counted as finished package, 

//and as production the KP when it is KP.

//The same to KPP

	int Num_PKG = (int)PKG.size();

	int Num_PIN_PKG = (int)PKG.size()*(int)PIN_order.size()+1;

	

	vector<int> PKG_production( Num_PKG,0);

	vector<int> PIN_PKG_production( Num_PIN_PKG,0);

	

	const int Num_Step = Max_Num_Step;

	//Compute production of KP and KPP first

	for( int step_id=1;step_id<= Num_Step;step_id++)

	{

		for(int d=1;d<=Num_Device;d++){

			if(multi_device_production_step[(step_id-1)*Num_Device+d-1].whether_exist && multi_device_production_step[(step_id-1)*Num_Device+d-1].whether_final_step){

				int pkg_id=device_PKG[d];

				int pin_num=device_PIN[d];

				vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);

				int pin_order=pfind-PIN_order.begin()+1;

				int index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;

				PKG_production[pkg_id-1]+=multi_device_production_step[(step_id-1)*Num_Device+d-1].production;

				PIN_PKG_production[index]+=multi_device_production_step[(step_id-1)*Num_Device+d-1].production;

			}

		}

	}

//end by Zhufeng, Aug 12, 2012, KP and KPP



	vector<multi_lot>::iterator p_l_temp=p_l;

	for(int i=p_l-candidate_lot_list.begin()+1;i<=(int)candidate_lot_list.size();i++)

	{

		candidate_lot_list[i-1].benefit=candidate_lot_list[i-1].weight;



		int dev_id=0;



		if(!candidate_lot_list[i-1].whether_initial_lot)

		{

			dev_id=lots_device[candidate_lot_list[i-1].lot_id];

		}else

		{

			dev_id=initial_running_lot[candidate_lot_list[i-1].initial_running_lot_id-1].dev_id;

		}



		int logpoint = candidate_lot_list[i-1].logpoint;



		int step_id = get_step_id(dev_id,logpoint); 



		int type=is_KD_PD(dev_id);



		if(type==1 ) //this is a key device

		{

			//Added by Zhufeng, for shortage only for existing steps,May 29,2012,

			if(multi_device_production_step[(step_id-1)*Num_Device+dev_id-1].whether_exist)

			{

				//end by May 29,2012

				//int pass_no=candidate_lot_list[i-1].pass_no;



				int shortage=n_k_minchips[dev_id]-multi_device_production_step[(step_id-1)*Num_Device+dev_id-1].production;

				if(shortage>0)

				{

					if(shortage>0)

					{

						int min=candidate_lot_list[i-1].quantity;

						if(min>shortage){ min=shortage;}

						candidate_lot_list[i-1].benefit+=penalty[dev_id]/coef*min;

					}



				}

				//Added by May 29, 2012

			}

			//end by May 29, 2012

		}//end of if(type==1 ) 



//Added by Zhufeng, Aug 12, 2012, KP and KPP

//Only if the last step of a device is done, it is counted as finished package, 

//and as production the KP when it is KP.

//The same to KPP		

		int pkg_id=device_PKG[dev_id];

		int pin_num=device_PIN[dev_id];

		bool is_kp=is_Key_PKG(dev_id);

		bool is_kpp=is_Key_PIN_PKG(dev_id);



		if(is_kp && PKG_production[pkg_id-1]<n_kp_minchips[pkg_id]){//if the device contains a key package and it is short

			int diff=n_kp_minchips[pkg_id]-PKG_production[pkg_id-1];

			int min=candidate_lot_list[i-1].quantity;

			if(min>diff){ min=diff;}

			candidate_lot_list[i-1].benefit+=(double)PKG_penalty[pkg_id]/coef*min;

		}

	

		if( is_kpp){

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



			if(PIN_PKG_production[index]<target){

				int diff=target-PIN_PKG_production[index];

				int min=candidate_lot_list[i-1].quantity;

				if(min>diff){ min=diff;}

				candidate_lot_list[i-1].benefit+=(double)get_PIN_PKG_penalty(pkg_id,pin_num)/coef*min;

			}

		}



//end by Zhufeng, Aug 12, 2012, KP and KPP





	}//end of for(int i=p_l-candidate_lot_list.begin()+1;i<=(int)candidate_lot_list.size();i++)



	//rank the the candidate_lot from p_l to the end.

	cout<<"re-rank candidate_lot_list"<<endl;

	vector<multi_lot>::iterator p_c=p_l;

	vector<multi_lot>::iterator p_next=p_l+1;

	struct multi_lot p_temp;

	int num_iter=0;

	//cout<<"candidate_lot size="<<(int)candidate_lot.size()<<endl;

	int total_iter=candidate_lot_list.end()-p_l;



	while(num_iter<=total_iter-1)

	{

		//cout<<"num_iter="<<num_iter<<endl;

		p_c=p_l;

		p_next=p_l+1;

		while( p_next != candidate_lot_list.end()-num_iter)

		{

			//Updated by Zhufeng, March 27, 2012, change weight to benefit

			//if( (* p_c).weight < (* p_next).weight )

			if( (* p_c).benefit < (* p_next).benefit )

				//end March 27, 2012

			{

				p_temp=(* p_next);

				(* p_next)=(* p_c);

				(* p_c)=p_temp;

			}

			p_c++;

			p_next++;

			//num_iter++;

		}

		num_iter++;

	}



}



//Added by Zhufeng on April 28, 2012

void AT::update_lot_benefit( vector<multi_lot> & candidate_lot_list, 

							//const vector<multi_production> & multi_device_production

							const vector<multi_production_step> & multi_device_production_step)

{

	//Update the benefit value of all lots in candidate_lot_list based on the multi_device_production and rerank the candidate_lot_list

	//cout<<"update lot benefit"<<endl;



	//Added by Zhufeng, Aug 12, 2012, KP and KPP

//Only if the last step of a device is done, it is counted as finished package, 

//and as production the KP when it is KP.

//The same to KPP

	int Num_PKG = (int)PKG.size();

	int Num_PIN_PKG = (int)PKG.size()*(int)PIN_order.size()+1;

	

	vector<int> PKG_production( Num_PKG,0);

	vector<int> PIN_PKG_production( Num_PIN_PKG,0);

	

	const int Num_Step = Max_Num_Step;

	//Compute production of KP and KPP first

	for( int step_id=1;step_id<= Num_Step;step_id++)

	{

		for(int d=1;d<=Num_Device;d++){

			if(multi_device_production_step[(step_id-1)*Num_Device+d-1].whether_exist && multi_device_production_step[(step_id-1)*Num_Device+d-1].whether_final_step){

				int pkg_id=device_PKG[d];

				int pin_num=device_PIN[d];

				vii pfind=find(PIN_order.begin(),PIN_order.end(),pin_num);

				int pin_order=pfind-PIN_order.begin()+1;

				int index=(pkg_id-1)*(int)PIN_order.size()+pin_order-1;

				PKG_production[pkg_id-1]+=multi_device_production_step[(step_id-1)*Num_Device+d-1].production;

				PIN_PKG_production[index]+=multi_device_production_step[(step_id-1)*Num_Device+d-1].production;

			}

		}

	}

//end by Zhufeng, Aug 12, 2012, KP and KPP



	for(int i=1;i<=(int)candidate_lot_list.size();i++)

	{

		candidate_lot_list[i-1].benefit=candidate_lot_list[i-1].weight;



		int dev_id=0;



		if(!candidate_lot_list[i-1].whether_initial_lot)

		{

			dev_id=lots_device[candidate_lot_list[i-1].lot_id];

		}else

		{

			dev_id=initial_running_lot[candidate_lot_list[i-1].initial_running_lot_id-1].dev_id;

		}



		int logpoint = candidate_lot_list[i-1].logpoint;



		int step_id = get_step_id(dev_id,logpoint); 



		int type=is_KD_PD(dev_id);



		if(type==1 ) //this is a key device

		{

			int pass_no=candidate_lot_list[i-1].pass_no;



			//Modified by Zhufeng, April 29, 2012. Add "min" in it. 

			

			//int shortage=n_k_minchips[dev_id]-multi_device_production[(pass_no-1)*Num_Device+dev_id-1].production;

			int shortage=n_k_minchips[dev_id]-multi_device_production[(step_id-1)*Num_Device+dev_id-1].production;

			if(shortage>0)

			{

				int min=candidate_lot_list[i-1].quantity;

				if(min>shortage){ min=shortage;}

				candidate_lot_list[i-1].benefit+=penalty[dev_id]/coef*min;

			}



			//end by Zhufeng, April 29, 2012



			//end by Zhufeng

		}//end of if(type==1 ) 

		

//Added by Zhufeng, Aug 12, 2012, KP and KPP

//Only if the last step of a device is done, it is counted as finished package, 

//and as production the KP when it is KP.

//The same to KPP		

		int pkg_id=device_PKG[dev_id];

		int pin_num=device_PIN[dev_id];

		bool is_kp=is_Key_PKG(dev_id);

		bool is_kpp=is_Key_PIN_PKG(dev_id);



		if(is_kp && PKG_production[pkg_id-1]<n_kp_minchips[pkg_id]){//if the device contains a key package and it is short

			int diff=n_kp_minchips[pkg_id]-PKG_production[pkg_id-1];

			int min=candidate_lot_list[i-1].quantity;

			if(min>diff){ min=diff;}

			candidate_lot_list[i-1].benefit+=(double)PKG_penalty[pkg_id]/coef*min;

		}

	

		if( is_kpp){

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



			if(PIN_PKG_production[index]<target){

				int diff=target-PIN_PKG_production[index];

				int min=candidate_lot_list[i-1].quantity;

				if(min>diff){ min=diff;}

				candidate_lot_list[i-1].benefit+=(double)get_PIN_PKG_penalty(pkg_id,pin_num)/coef*min;

			}

		}



//end by Zhufeng, Aug 12, 2012, KP and KPP



	}//end of for(int i=1;i<=(int)candidate_lot_list.size();i++)



	//rank the the candidate_lot based on the benefit value decreasingly.

	sort(candidate_lot_list.begin(),candidate_lot_list.end());

	reverse(candidate_lot_list.begin(),candidate_lot_list.end());



	//cout<<"re-rank candidate_lot_list"<<endl;

	//vector<multi_lot>::iterator p_c=p_l;

	//vector<multi_lot>::iterator p_next=p_l+1;

	//struct multi_lot p_temp;

	//int num_iter=0;

	////cout<<"candidate_lot size="<<(int)candidate_lot.size()<<endl;

	//int total_iter=candidate_lot_list.end()-p_l;



	//while(num_iter<=total_iter-1)

	//{

	//	//cout<<"num_iter="<<num_iter<<endl;

	//	p_c=p_l;

	//	p_next=p_l+1;

	//	while( p_next != candidate_lot_list.end()-num_iter)

	//	{

	//		//Updated by Zhufeng, March 27, 2012, change weight to benefit

	//		//if( (* p_c).weight < (* p_next).weight )

	//		if( (* p_c).benefit < (* p_next).benefit )

	//			//end March 27, 2012

	//		{

	//			p_temp=(* p_next);

	//			(* p_next)=(* p_c);

	//			(* p_c)=p_temp;

	//		}

	//		p_c++;

	//		p_next++;

	//		//num_iter++;

	//	}

	//	num_iter++;

	//}

}

//Added by Zhufeng, Dec 20, 2013
void AT::build_device_pass_from_S()
{
  cout<<"build device pass!"<<endl;
  for(vector<tooling_configure>::const_iterator p_s = S.begin();p_s!=S.end();++p_s)
    {
      int dev_id = p_s->device;
      map<int,multi_device_pass>::iterator p = device_pass_map.find(dev_id); 
      if(p!=device_pass_map.end())
	{
	  // cout<<"find it!"<<endl;
	  vector<int>::iterator p_l = find(p->second.logpoint_id_vector.begin(),p->second.logpoint_id_vector.end(),p_s->logpoint);
	  if(p_l==p->second.logpoint_id_vector.end()){
	    p->second.logpoint_id_vector.push_back(p_s->logpoint);
	    p->second.num_step++;
	  }
	}
      else
	{
	  multi_device_pass one_device_pass;
	  one_device_pass.logpoint_id_vector.push_back(p_s->logpoint);
	  one_device_pass.num_step =1;
	  device_pass_map[dev_id]= one_device_pass;
	  // cout<<"create it"<<endl;
	}
    }
  // exit(0);
}

void AT::show_device_pass_map()const
{
  ofstream fout("Debug/device_pass_map.csv");
  fout<<"Dev_id,Dev Name,Num steps,logpoint"<<endl;
  for(map<int,multi_device_pass>::const_iterator p = device_pass_map.begin();p!=device_pass_map.end();++p)
    {
      fout<<p->first<<","<<DEV[p->first-1]<<","<<p->second.num_step<<",";
      for(vector<int>::const_iterator p_l = p->second.logpoint_id_vector.begin();p_l!=p->second.logpoint_id_vector.end();++p_l)
	{
	  if(p_l==p->second.logpoint_id_vector.begin())
	    {
	      fout<<*p_l<<endl;
	    }
	  else
	    {
	      fout<<",,,"<<*p_l<<endl;
	    }
	}
    }
  fout.close();

}
//end by Zhufeng, Dec 20, 2013
int AT::get_num_step_left(const int & dev_id,const int & logpoint)const
{
  try
  {
    vector<int>::const_iterator p = find(device_pass_map.at(dev_id).logpoint_id_vector.begin(),device_pass_map.at(dev_id).logpoint_id_vector.end(),logpoint);
    if(p!=device_pass_map.at(dev_id).logpoint_id_vector.end())
      {
		  //cout<<"found "<<*p<<endl;
	return device_pass_map.at(dev_id).logpoint_id_vector.end()-p-1;
      }
    else
      {
	throw 1;
      }
  }
  catch(int i)
    {
      if(i==1)
	{
	  cout<<"In get_num_step_left() this logpoint "<<logpoint<<" is not found in device "<<dev_id<<endl;
	}
    }
 catch(exception & e)
   {
     cout<<"In get_num_step_left() "<<e.what()<<endl;
   }
}
