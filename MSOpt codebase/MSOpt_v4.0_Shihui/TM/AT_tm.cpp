// $Id: AAP_tm.cpp,v 1.2 2003/12/03 01:52:30 magh Exp $

/*-------------------------------------------------------------------------
Author: Matthew Galati (magh@lehigh.edu)

(c) Copyright 2003 Lehigh University. All Rights Reserved.

This software is licensed under the Common Public License. Please see 
accompanying file for terms.    
---------------------------------------------------------------------------*/

//#include "BCP_tm.hpp"
#include "CoinHelperFunctions.hpp"
#include "AT_tm.hpp"
#include "AT_var.hpp"
//#include "AAP_init.hpp"
#include "AT_user_data.hpp"

using namespace std;
/*--------------------------------------------------------------------------*/
void AT_tm::pack_module_data(BCP_buffer & buf, BCP_process_t ptype){
	//Pack information to be passed to the LP process
	lp_par.pack(buf);  //the list of parameters read in
	at->pack(buf);     //the AT instance
	cout<<"size of buf="<<buf.size()<<endl;
}

/*--------------------------------------------------------------------------*/
void AT_tm::pack_var_algo(const BCP_var_algo* var, BCP_buffer& buf){
	//Pack an algorithmic variable (AAP_var is derived from BCP_var_algo)
	{
		const AT_var * mvar = dynamic_cast<const AT_var*>(var);
		if(mvar){
			mvar->pack(buf);
			return;
		}
	}
	throw BCP_fatal_error("AT_tm::pack_var_algo() : unknown cut type!\n");
}

/*--------------------------------------------------------------------------*/
BCP_var_algo* AT_tm::unpack_var_algo(BCP_buffer& buf){
	//Unpack an algorithmic variable (AAP_var is derived from BCP_var_algo)
	return new AT_var(buf);
}

/*--------------------------------------------------------------------------*/
void AT_tm::pack_user_data(const BCP_user_data* ud, BCP_buffer& buf){
	//Pack user data
	{
		const AT_user_data * data = dynamic_cast<const AT_user_data*>(ud);
		if(data){
			data->pack(buf);
			return;
		}
	}
	throw BCP_fatal_error("AT_tm::pack_user_data() : cast failed!\n");
}

/*--------------------------------------------------------------------------*/
BCP_user_data * AT_tm::unpack_user_data(BCP_buffer& buf){
	return new AT_user_data(buf);
}

/*--------------------------------------------------------------------------*/
void AT_tm::display_feasible_solution(const BCP_solution * sol){
	//Display the current feasible solution
#ifdef DBG_PRINT
	const BCP_solution_generic* gsol = dynamic_cast<const BCP_solution_generic*>(sol);
	if(gsol){
		const int varnum = gsol->_vars.size();
		for (int c = 0; c < varnum; ++c) {   
			const AT_var* mvar = dynamic_cast<const AT_var*>(gsol->_vars[c]);
			if(mvar){
				cout << "Column " << gsol->_vars[c]->bcpind() << " : " << gsol->_values[c] << endl; 
				mvar->print();
			}
		}
	}
#else
	BCP_tm_user::display_feasible_solution(sol);
#endif
}

/*--------------------------------------------------------------------------*/
void AT_tm::initialize_core(BCP_vec<BCP_var_core*>& vars,BCP_vec<BCP_cut_core*>& cuts,BCP_lp_relax*& matrix) {
	std::cout<<"in AT_tm::initialize core()"<<endl;
	//This is for RMP
	// Here we define which variables and which constraints are core, i.e., 
	// those which can never be removed from the base model. We have no core 
	// variables. The core constraints are (5) and (6).
	//const int n_cuts = aap->getDimension() + 1; //+1 for convexity constriant
	//-----------------------------------------------------------------------------------------size of the core (starts)
	//the delta_k and delta_p should be core variables
	int num_core_cols=(int)(*at).Key_Devices.size()+(int)(*at).Packages.size();
	//try to remove the dual degeneracy by adding surplus variables
	cout<<"!!!"<<endl;
	num_core_cols+=(*at).Num_Lot;
	cout<<"num_core_cols="<<num_core_cols<<endl;

	double * obj=new double[num_core_cols];
	double * clb=new double[num_core_cols];
	double * cub=new double[num_core_cols];

	CoinPackedMatrix * AT_core=new CoinPackedMatrix(false,0.0,0.0);
	double* cutcoef = new double[num_core_cols];
	int *cutind = new int[num_core_cols];

	int n_cuts = (*at).Num_Lot+((*at).Num_Tooling_Family)*(int)((*at).Temperature_Set).size()
		+(int)((*at).Key_Devices).size()+(int)((*at).Packages).size();
	if(!(*at).SIM_option){
		n_cuts+=(*at).Num_Machine;
	}else{
		n_cuts+=(int)(*at).SIM.size();
	}
	//try to remove the dual degeneracy, CG stability
	n_cuts+=(*at).Num_Lot;

	cuts.reserve(n_cuts);
	cout<<"n_cuts="<<n_cuts<<endl;

	double * rlb=new double[n_cuts];
	double * rub=new double[n_cuts];
	//-----------------------------------------------------------------------------------------size of the core (ends)
	//-----------------------------------------------------------------------------------------specify the arrays for constraints (starts)
	double eps=1E-6;
	int num_cons=0;
	for(int l=1;l<=(*at).Num_Lot;l++){
		//double rnd=rand()*1.0/RAND_MAX;
		//rnd=rnd*(*at).Num_Lot*eps+1.0;
		//cuts.unchecked_push_back(new BCP_cut_core(-DBL_MAX,rnd));
		cuts.unchecked_push_back(new BCP_cut_core(-DBL_MAX,1.0));
		rlb[num_cons]=-DBL_MAX;
		rub[num_cons]=1.0;
		num_cons++;
	}
	for(int tf=1;tf<=(*at).Num_Tooling_Family;tf++){
		for(int n=1;n<=(int)((*at).Temperature_Set).size();n++){
			vector<t_set>::iterator p_set=((*at).Temperature_Set).begin()+n-1;
			vector<t_set> inter_set;
			(*at).get_inter_set((*p_set),inter_set);
			vector<t_set>::iterator pm=inter_set.begin();
			int sum=0;
			while(pm!=inter_set.end()){
				int set_id=(*at).get_set_id(*pm);
				int pos=tf*(*at).Max_Num_Temperature+set_id;
				sum+=(*at).n_t_n_tooling[pos];
				//sum+=(*at).n_t_n_tooling[tf][set_id];
				pm++;
			}
			cuts.unchecked_push_back(new BCP_cut_core(-DBL_MAX,sum));
			rlb[num_cons]=-DBL_MAX;
			rub[num_cons]=sum;
			num_cons++;
		}
	}
	for(int k=1;k<=(int)((*at).Key_Devices.size());k++){
		int dev=(*at).Key_Devices[k-1];
		cuts.unchecked_push_back(new BCP_cut_core((*at).n_k_minchips[dev],DBL_MAX));
		rlb[num_cons]=(*at).n_k_minchips[dev];
		rub[num_cons]=DBL_MAX;
		num_cons++;
	}
	for(int p=1;p<=(int)((*at).Packages.size());p++){
		int dev=(*at).Packages[p-1];
		cuts.unchecked_push_back(new BCP_cut_core((*at).n_p_minchips[dev],DBL_MAX));
		rlb[num_cons]=(*at).n_p_minchips[dev];
		rub[num_cons]=DBL_MAX;
		num_cons++;
	}
	if(!(*at).SIM_option){
		for(int i=1;i<=(*at).Num_Machine;i++){
			cuts.unchecked_push_back(new BCP_cut_core(-DBL_MAX,1.0));
			rlb[num_cons]=-DBL_MAX;
			rub[num_cons]=1.0;
			num_cons++;
		}
	}else{
		for(int sim=1;sim<=(int)(*at).SIM.size();sim++){
			vector<SIM_item>::iterator pSIM=(*at).SIM.begin()+sim-1;
			cuts.unchecked_push_back(new BCP_cut_core(-DBL_MAX,(*pSIM).num_machine));
			rlb[num_cons]=-DBL_MAX;
			rub[num_cons]=(*pSIM).num_machine;
			num_cons++;
		}
	}
	for(int l=1;l<=(*at).Num_Lot;l++){
		//cout<<"l="<<l<<",epsilon="<<epsilon[l]<<endl;
		cuts.unchecked_push_back(new BCP_cut_core(-DBL_MAX,epsilon[l]));
		rlb[num_cons]=-DBL_MAX;
		rub[num_cons]=epsilon[l];
		num_cons++;
	}
	cout<<"num_cons="<<num_cons<<endl;
	//-----------------------------------------------------------------------------------------specify the arrays for constraints (ends)
	//-----------------------------------------------------------------------------------------setup core variables (starts)
	double sum_weight=(*at).sum_weight;

	cout<<"core vars for key devices"<<endl;
	for(int k=1;k<=(int)(*at).Key_Devices.size();k++){
		vars.push_back(new BCP_var_core(BCP_ContinuousVar,sum_weight,0.0,DBL_MAX));
	}
	cout<<"core vars for package devices"<<endl;
	for(int p=1;p<=(int)(*at).Packages.size();p++){
		vars.push_back(new BCP_var_core(BCP_ContinuousVar,sum_weight,0.0,DBL_MAX));
	}
	//core variables y- for the lots to remove dual dengeneracy (redundant rows)
	for(int l=1;l<=(*at).Num_Lot;l++){
		//cout<<"l="<<l<<",delta="<<delta[l]<<endl;
		vars.push_back(new BCP_var_core(BCP_ContinuousVar,-delta[l],0.0,DBL_MAX));
	}
	//-----------------------------------------------------------------------------------------setup core variables (ends)	
	//-----------------------------------------------------------------------------------------setup the core matrix (starts)
	//cout<<"1"<<endl;
	for(int i=0;i<=num_core_cols-1;i++){ cutcoef[i]=0.0; cutind[i]=0;}

	for(int l=1;l<=(*at).Num_Lot;l++){
		int cutnz=1;																//how many variables in this constarints with nonzero coeff (terms)
		cutind[0]=(int)(*at).Key_Devices.size()+(int)(*at).Packages.size()-1+l;     //what the index for these variables
		cutcoef[0]=-1;																//what the coeff for these variables
		AT_core->appendRow(cutnz,cutind,cutcoef);
	}
	for(int i=1;i<=((*at).Num_Tooling_Family)*(int)((*at).Temperature_Set).size();i++){
		int cutnz=0;
		AT_core->appendRow(cutnz,cutind,cutcoef);
	}
	//cout<<"2"<<endl;
	for(int k=1;k<=(int)(*at).Key_Devices.size();k++){
		int cutnz=1;
		cutind[0]=k-1;
		cutcoef[0]=(*at).coef;
		AT_core->appendRow(cutnz,cutind,cutcoef);
	}
	//cout<<"3"<<endl;
	for(int p=1;p<=(int)(*at).Packages.size();p++){
		int cutnz=1;
		cutind[0]=(int)(*at).Key_Devices.size()-1+p;
		cutcoef[0]=(*at).coef;
		AT_core->appendRow(cutnz,cutind,cutcoef);
	}
	//cout<<"4"<<endl;
	if(!(*at).SIM_option){
		for(int i=1;i<=(*at).Num_Machine;i++){
			int cutnz=0;
			cutind[0]=0;
			cutcoef[0]=0.0;
			AT_core->appendRow(cutnz,cutind,cutcoef);
		}
	}else{
		for(int sim=1;sim<=(int)(*at).SIM.size();sim++){
			int cutnz=0;
			cutind[0]=0;
			cutcoef[0]=0.0;
			AT_core->appendRow(cutnz,cutind,cutcoef);
		}
	}
	//cout<<"5"<<endl;
	for(int l=1;l<=(*at).Num_Lot;l++){
		int cutnz=1;
		cutind[0]=(int)(*at).Key_Devices.size()+(int)(*at).Packages.size()-1+l;
		cutcoef[0]=1.0;
		AT_core->appendRow(cutnz,cutind,cutcoef);
	}
	//-----------------------------------------------------------------------------------------setup the core matrix (ends)	
	//-----------------------------------------------------------------------------------------setup arrays for the core variables (starts)	
	for(int i=0;i<=(int)(*at).Key_Devices.size()+(int)(*at).Packages.size()-1;i++){
		obj[i]=sum_weight;
		clb[i]=0.0;
		cub[i]=DBL_MAX;
	}
	for(int i=(int)(*at).Key_Devices.size()+(int)(*at).Packages.size();i<=num_core_cols-1;i++){
		int l=i-(int)(*at).Key_Devices.size()-(int)(*at).Packages.size()+1;
		//cout<<"l="<<l<<endl;
		obj[i]=-delta[l];
		clb[i]=0.0;
		cub[i]=DBL_MAX;
	}
	//-----------------------------------------------------------------------------------------setup arrays for the core variables (ends)	
	//-----------------------------------------------------------------------------------------setup the matrix (starts)	
	//cout<<"6"<<endl;
	matrix=new BCP_lp_relax;
	//cout<<"7"<<endl;
	matrix->copyOf(*AT_core,obj,clb,cub,rlb,rub);
	cout<<"size of vars="<<vars.size()<<",size of cuts="<<cuts.size()<<endl;
	//-----------------------------------------------------------------------------------------setup the matrix (ends)	
	//cout<<"8"<<endl;
	delete [] cutcoef;
	delete [] cutind;
	delete [] obj;
	delete [] clb;
	delete [] cub;
	delete [] rlb;
	delete [] rub;
	//delete AT_core;

	//cout<<"9"<<endl;
}

/*--------------------------------------------------------------------------*/
void AT_tm::create_root(BCP_vec<BCP_var*>& added_vars,BCP_vec<BCP_cut*>& added_cuts,BCP_user_data * & user_data,BCP_pricing_status& pricing_status){

	std::cout<<"in AT_tm::create_root()"<<endl;

	// Here we define the root node of the search tree.

	// The pricing status says which type of variables need to be priced out
	// to find a solution. In our case we are generating algorithmic variables.
	// Algorithmic variables are used when there are too many to count (index).
	pricing_status = BCP_PriceAlgoVars;

	// Initialize the root node's user data
	user_data = new AT_user_data();

	// AP Points (fix i and find an assignment for J x K)
	// sum {ik in I x K}            x_ijk = 1, forall j in J (2)
	// sum {ij in I x J}            x_ijk = 1, forall k in K (3)

	// Seed the root with random AP points such that we have at least one
	// feasible AAP point. Keep it simple for now, just use AP point: x_iii.
	int i, index;
	/*
	const double * assigncost = aap->getAssignCost();
	const int n = aap->getDimension();

	vector<int> ap;
	ap.reserve(n);

	double cost = 0.0;
	for(i = 0; i < n; i++){
		index = index3(i,i,i,n);
		ap.push_back(index);
		cost += assigncost[index];
	}

	added_vars.push_back(new AAP_var(ap, cost));
	*/
	//------------------------------------
	int Root_Option=0;
	ofstream f_core("initial_core.txt");

	if(Root_Option){//kind of a greedy way to initialize the core variables
		int machine_id=0;
		int Num_Processor=0;
		if(!(*at).SIM_option){
			Num_Processor=(*at).Num_Machine;
		}else{
			Num_Processor=(int)(*at).SIM.size();
		}
		for(int iter=1;iter<=Num_Processor;iter++){
			if(!(*at).SIM_option){
				machine_id=iter;
			}else{
				vector<SIM_item>::iterator pSIM=(*at).SIM.begin()+iter-1;
				machine_id=(*pSIM).represented_machine_id;
			}
			vector<int> LS;
			int lambda_id=-1;
			vii pL=(*at).Lambda_i[machine_id].int_vector.begin();
			while(pL!=(*at).Lambda_i[machine_id].int_vector.end()){
				LS.clear();
				lambda_id=(*pL);
				(*at).get_L_i_lambda(LS,machine_id,lambda_id);
				if((int)LS.size()>0){
					break;
				}else{
					lambda_id=-1;
				}
				pL++;
			}//end_while
			if((int)LS.size()<1 || lambda_id<0){ continue;} //the machine is not used at all
			cout<<"machine_id="<<machine_id<<",lambda_id="<<lambda_id<<",size of L(i,lambda)="<<LS.size()<<endl;
			double time_used=0;
			vector< pair<int,int> > L_S;
			vii pl=LS.begin();
			while(pl!=LS.end()){
				int l=(*pl);
				vector<tooling_configure> TS;
				(*at).get_S_i_l_Lambda(TS,machine_id,l,lambda_id);
				int s=(*TS.begin()).id;
				double process_time=(*at).n_l_chips[l]*1.0/(*at).rate[s];
				if(time_used+process_time<=(*at).H[machine_id]){//still got enough time to process the lot
					time_used+=process_time;
					L_S.push_back(make_pair(l,s));
				}
				pl++;
			}
			cout<<"L_S size="<<(int)L_S.size()<<endl;
			added_vars.push_back(new AT_var(machine_id,lambda_id,L_S));

			f_core<<"machine_id="<<machine_id<<",lambda_id="<<lambda_id<<",LS,";
			vector<pair<int,int> >::iterator pLS=L_S.begin();
			while(pLS!=L_S.end()){
				int l=(*pLS).first;
				int s=(*pLS).second;
				f_core<<"("<<l<<","<<s<<"),";
				pLS++;
			}
			f_core<<endl;
		}
	}else{
		//---------------------------------------------------------------------------------------------create root (core variables) from the heuristic
		//the elite heuristic solutions are stored in elite_soln of the AT object
		cout<<"create root from the heuristic solution"<<endl;
		for(int id=1;id<=(int)((*at).elite_soln)[0].sub_soln.size();id++){
			int i=((*at).elite_soln)[0].sub_soln[id-1].i;
			int sim=(*at).machine_sim[i];
			int Lambda_id=((*at).elite_soln)[0].sub_soln[id-1].Lambda_id;
			if(Lambda_id<0){ continue;} //this machine is not used at all
			vector< pair<int,int> > L_S;
			vii p_int=((*at).elite_soln)[0].sub_soln[id-1].lots.int_vector.begin();
			while(p_int!=((*at).elite_soln)[0].sub_soln[id-1].lots.int_vector.end()){
				int l=(*p_int);
				int s=(*at).max_rate_s(i,l,Lambda_id);
				L_S.push_back(make_pair(l,s));
				p_int++;
			}
			if(!(*at).SIM_option){
				added_vars.push_back(new AT_var(i,Lambda_id,L_S));
			}else{
				added_vars.push_back(new AT_var(sim,Lambda_id,L_S));
			}

			f_core<<"i="<<i<<",sim="<<sim<<",Lambda_id="<<Lambda_id<<",LS,";
			vector<pair<int,int> >::iterator pLS=L_S.begin();
			while(pLS!=L_S.end()){
				int l=(*pLS).first;
				int s=(*pLS).second;
				f_core<<"("<<l<<","<<s<<"),";
				pLS++;
			}
			f_core<<endl;
		}
	}//end_if
	f_core.close();
}

/*--------------------------------------------------------------------------*/
void AT_tm::init_new_phase(int phase, BCP_column_generation & colgen) {
	// Initialize the column generation phase
	std::cout<<"in AT_tm::initial_new_phase()"<<endl;
	colgen = BCP_GenerateColumns;
}

