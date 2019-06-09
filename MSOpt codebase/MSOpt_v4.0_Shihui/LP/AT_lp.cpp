// $Id: AAP_lp.cpp,v 1.3 2003/12/03 01:52:30 magh Exp $

/* ------------------------------------------------------------------------
 Author: Matthew Galati (magh@lehigh.edu)

 (c) Copyright 2003 Lehigh University. All Rights Reserved.

 This software is licensed under the Common Public License. Please see 
 accompanying file for terms.    
---------------------------------------------------------------------------*/

#ifdef COIN_USE_CPX
#include "OsiCpxSolverInterface.hpp"
#include "cplex.h"
typedef OsiCpxSolverInterface OsiXxxSolverInterface;
#endif
//#ifdef COIN_USE_CLP
#include "OsiClpSolverInterface.hpp"
typedef OsiClpSolverInterface OsiXxxSolverInterface;
//#endif
#ifdef COIN_USE_OSL
#include "OsiOslSolverInterface.hpp"
typedef OsiOslSolverInterface OsiXxxSolverInterface;
#endif

#include "BCP_lp.hpp"
#include "BCP_lp_node.hpp"
#include "AT_lp.hpp"
#include "AT_var.hpp"
#include "AT_user_data.hpp"

/*--------------------------------------------------------------------------*/
void AT_lp::unpack_module_data(BCP_buffer& buf) {
	//Unpack the information passed from the TM process
	lp_par.unpack(buf); //parameter list for LP
	at = new AT(buf);   //construct the AT instance from buf that passed from TM module
	//cout<<"here"<<endl;
	subprob_col_lb.assign((int)(*at).SIM.size()+1,(double*)NULL);
	subprob_col_ub.assign((int)(*at).SIM.size()+1,(double*)NULL);
	subprob_row_lb.assign((int)(*at).SIM.size()+1,(double*)NULL);
	subprob_row_ub.assign((int)(*at).SIM.size()+1,(double*)NULL);
	//cout<<"prob??"<<endl;
	subprob_M.assign((int)(*at).SIM.size()+1,(CoinPackedMatrix*)NULL);
	create_subprob_matrix();
}

/*--------------------------------------------------------------------------*/
void AT_lp::pack_var_algo(const BCP_var_algo* var, BCP_buffer& buf){
	//Pack an algorithmic variable (AAP_var is derived from BCP_var_algo)
	{
		const AT_var * mvar = dynamic_cast<const AT_var*>(var);
		if(mvar){
			mvar->pack(buf);
			return;
		}else{
			cout<<"dynamic cast erorr!!!"<<endl;
		}
	}
	throw BCP_fatal_error("AT_lp::pack_var_algo() : unknown cut type!\n");
}

/*--------------------------------------------------------------------------*/
BCP_var_algo* AT_lp::unpack_var_algo(BCP_buffer& buf){
	//Unpack an algorithmic variable (AAP_var is derived from BCP_var_algo)
	return new AT_var(buf);
}

/*--------------------------------------------------------------------------*/
void AT_lp::pack_user_data(const BCP_user_data* ud, BCP_buffer& buf){
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
BCP_user_data* AT_lp::unpack_user_data(BCP_buffer& buf){
	return new AT_user_data(buf);
}

/*--------------------------------------------------------------------------*/
OsiSolverInterface* AT_lp::initialize_solver_interface(){
	//Initialize the OSI Xxx solver interface
	OsiXxxSolverInterface * si = new OsiXxxSolverInterface();

#ifdef COIN_USE_CPX
	//Using cplex, we need to shut off the presolver to make sure get a dual 
	//ray in the case of primal infeasibility.
	CPXsetintparam(si->getEnvironmentPtr(),CPX_PARAM_PREIND, 0);
	//use primal simplex to solve the master LP
	si->setHintParam(OsiDoDualInResolve,false,OsiHintDo);
#endif
	return si;
}

/*--------------------------------------------------------------------------*/
void AT_lp::display_lp_solution(const BCP_lp_result& lpres,const BCP_vec<BCP_var*>& vars,const BCP_vec<BCP_cut*>& cuts,const bool final_lp_solution){

#ifdef DBG_PRINT 
	//Display the current LP solution
	cout << "UB: " << upper_bound() << " LB: " 
		<< getLpProblemPointer()->node->true_lower_bound << endl;

	const int varnum = vars.size();
	for (int c = 0; c < varnum; ++c) {   
		if(lpres.x()[c] > tolerance){
			const AT_var* mvar = dynamic_cast<const AT_var*>(vars[c]);
			if(mvar){
				cout << "Column " << c << " : " << lpres.x()[c] << endl; 
				mvar->print();
			}
		}
	}
#endif
	//the following segment is for debug purpose: to see if each lot is actually processed one time at most
	/*
	cout<<"in AT_lp display_lp_solution()"<<endl;
	double * lot_processed = new double[(*at).Num_Lot+1];
	for(int l=0;l<=(*at).Num_Lot;l++){ lot_processed[l]=0.0;}
	struct vec_int var_record[(*at).Num_Lot+1];

	for(int c=0;c<=(int)vars.size();c++){
		if(lpres.x()[c]>tolerance){
			const AT_var * mvar = dynamic_cast<const AT_var*>(vars[c]);
			if(mvar){
				vector<pair<int,int> >::const_iterator pLS=(*mvar).L_S.begin();
				while(pLS!=(*mvar).L_S.end()){
					int l=(*pLS).first;
					lot_processed[l]+=lpres.x()[c];
					var_record[l].int_vector.push_back(c);
					pLS++;
				}
			}
		}
	}
	for(int l=1;l<=(*at).Num_Lot;l++){
		cout<<"l="<<l<<",lot_processed="<<lot_processed[l]<<endl;
		if(lot_processed[l]>1.0+tolerance){
			vii pint=var_record[l].int_vector.begin();
			while(pint!=var_record[l].int_vector.end()){
				cout<<(*pint)<<",";
				pint++;
			}
			cout<<endl;
		}
	}
	delete [] lot_processed;
	*/
}

/*--------------------------------------------------------------------------*/
double AT_lp::compute_lower_bound(const double old_lower_bound,const BCP_lp_result& lpres,const BCP_vec<BCP_var*>& vars,const BCP_vec<BCP_cut*>& cuts){

	// In order to get the bound we are solving the subproblem over the 
	// set of reduced costs. So, if we find any columns with negative 
	// reduced cost, we should save them for generate_vars_in_lp.  

	// Only try to generate_vars here if we have good duals 
	if(lpres.termcode() & BCP_ProvenPrimalInf)
		return -DBL_MAX;

	generated_vars.clear();
	bool new_var = generate_vars(vars,lpres.pi(), false);

	if(new_var){
		return old_lower_bound;
		//return max(old_lower_bound, lpres.objval() + most_reduced_cost);
	}
	else{
		const int tc = lpres.termcode();
		if (tc & BCP_ProvenOptimal)
			return lpres.objval();
		else return old_lower_bound;
	}
}

/*--------------------------------------------------------------------------*/
void AT_lp::generate_vars_in_lp(const BCP_lp_result& lpres,const BCP_vec<BCP_var*>& vars,const BCP_vec<BCP_cut*>& cuts
								,const bool before_fathom,BCP_vec<BCP_var*>& new_vars,BCP_vec<BCP_col*>& new_cols)
{
	/*
	Generate variables with negative reduced cost to be added 
	to the LP by solving an assignment problem. This work has
	already been done in compute_lower_bound and was saved in 
	generated_vars.So we don't need to solve the subproblems again to get the new cols

	The reduced cost of an assignment point is:
	rc_s = sum{ijk in H} s_ijk c_ijk                                    [c_s]
	- sum{i in I} sum{jk in J x K} s_ijk uhat_i       [dual of I constraints]
	- 1 * alpha                                [dual of convexity constraint]
	*/
	cout<<"in generate_vars_in_lp(), number of new vars="<<generated_vars.size()<<endl;
	new_vars.reserve(new_vars.size() + generated_vars.size());
	for (unsigned int i = 0; i < generated_vars.size(); ++i){
		new_vars.push_back(generated_vars[i]);
	}
	generated_vars.clear();
}

/*--------------------------------------------------------------------------*/
void AT_lp::restore_feasibility(const BCP_lp_result& lpres,const std::vector<double*> dual_rays,const BCP_vec<BCP_var*>& vars,
				 const BCP_vec<BCP_cut*>& cuts,BCP_vec<BCP_var*>& vars_to_add,BCP_vec<BCP_col*>& cols_to_add)
{
	// Generate a variable that breaks the certificate of infeasibility 
	// using the dual ray as the cost vector and solving an assignment problem. 

	for(unsigned int r = 0; r < dual_rays.size(); r++){
		generate_vars(vars,dual_rays[r], true);
		vars_to_add.reserve(vars_to_add.size() + generated_vars.size());
		for (unsigned int i = 0; i < generated_vars.size(); ++i){
			vars_to_add.push_back(generated_vars[i]);
		}
		generated_vars.clear();
	}
}

/*--------------------------------------------------------------------------*/
void AT_lp::construct_cost(const double * pi, const bool from_restore,int machine_id,double * costv)
{
	/*
	Assignment Problem objective:
	min {jk in J x K} c'_jk x_jk    

	c_'jk = min{i in I} c_ijk - uhat_i OR
	c_'jk = min{i in I} ray_i
	*/
	cout<<"in construct_cost()"<<endl;
#ifdef DBG_PRINT
	const AT_user_data * datatmp = dynamic_cast<const AT_user_data*>(get_user_data());
	cout << "ONES: ";
	for(unsigned int i = 0; i < datatmp->ones.size(); i++)
		cout <<"("<< datatmp->ones[i].first<<","<<datamp->ones[i].second << ")";
	cout << endl;
	cout << "ZEROS: ";
	for(unsigned int i = 0; i < datatmp->zeros.size(); i++)
		cout <<"("<< datatmp->zeros[i].first>>".">>datamp->zeros[i].second << ")";
	cout << endl;
#endif

	//------------------------------------------------------------------------------------------------map the dual prices (starts)
	double * pi_l = new double[(*at).Num_Lot+1];
	double * mu_t_n=new double[((*at).Num_Tooling_Family)*(int)(*at).Temperature_Set.size()+1];
    double * nu_k = new double[(int)(*at).Key_Devices.size()+1];
	double * eta_j= new double[(int)(*at).Packages.size()+1];
	double * gama_i;
	if(!(*at).SIM_option){
		gama_i=new double[(int)(*at).Num_Machine+1];
	}else{
		gama_i=new double[(int)(*at).SIM.size()+1];
	}
	double * sigma_l=new double[(*at).Num_Lot+1];
	//---------------------------------------------------------
	//cout<<"1"<<endl;
	for(int l=1;l<=(*at).Num_Lot;l++){
		pi_l[l]=pi[l-1];
	}
	int pi_id=(*at).Num_Lot-1;
	//cout<<"2"<<endl;
	for(int tf=1;tf<=(*at).Num_Tooling_Family;tf++){
		for(int n=1;n<=(int)(*at).Temperature_Set.size();n++){
			int index=(tf-1)*((*at).Temperature_Set.size())+n;
			pi_id++;
			mu_t_n[index]=pi[pi_id];
		}
	}
	//cout<<"3"<<endl;
	for(int k=1;k<=(int)(*at).Key_Devices.size();k++){
		pi_id++;
		nu_k[k]=pi[pi_id];
	}
	//cout<<"4"<<endl;
	for(int p=1;p<=(int)(*at).Packages.size();p++){
		pi_id++;
		eta_j[p]=pi[pi_id];
	}
	//cout<<"5"<<endl;
	if(!(*at).SIM_option){
		for(int i=1;i<=(*at).Num_Machine;i++){
			pi_id++;
			gama_i[i]=pi[pi_id];
		}
	}else{
		for(int i=1;i<=(int)(*at).SIM.size();i++){
			pi_id++;
			gama_i[i]=pi[pi_id];
		}
	}
	//cout<<"6"<<endl;
	for(int l=1;l<=(*at).Num_Lot;l++){
		pi_id++;
		sigma_l[l]=pi[pi_id];
	}
	//cout<<"7"<<endl;
	//cout<<"pi_id="<<pi_id<<endl;
	//------------------------------------------------------------------
	int n_rows = (*at).Num_Lot+((*at).Num_Tooling_Family)*(int)((*at).Temperature_Set).size()
		+(int)((*at).Key_Devices).size()+(int)((*at).Packages).size();
	if(!(*at).SIM_option){
		n_rows+=(*at).Num_Machine;
	}else{
		n_rows+=(int)(*at).SIM.size();
	}
	n_rows+=(*at).Num_Lot;
	cout<<"n_rows="<<n_rows<<endl;
	//This segment is for debugging Pi
	ofstream fout("debugPi.txt");
	for(int i=0;i<=n_rows-1;i++){
		fout<<"row="<<i<<",pi="<<pi[i]<<endl;
	}
	fout.close();
	//------------------------------------------------------------------------------------------------map the dual prices (ends)
	//abort();
	//----------------------------------------------------
	//cout<<"output gama_i"<<endl;
	//for(int i=1;i<=(*at).Num_Machine;i++){
	//	cout<<"i="<<i<<",gama_i="<<gama_i[i]<<endl;
	//}
	//the following segment is for debug purpose
	/*
	cout<<"compute dule obj to verify pi"<<endl;
	double dual_obj=0.0;
	for(int l=1;l<=(*at).Num_Lot;l++){
		dual_obj+=pi_l[l];
	}
	for(int tf=1;tf<=(*at).Num_Tooling_Family;tf++){
		for(int n=1;n<=(int)(*at).Temperature_Set.size();n++){
			vector<t_set>::iterator p_set=((*at).Temperature_Set).begin()+n-1;
			vector<t_set> inter_set;
			(*at).get_inter_set((*p_set),inter_set);
			vector<t_set>::iterator pm=inter_set.begin();
			int sum=0;
			while(pm!=inter_set.end()){
				int set_id=(*at).get_set_id(*pm);
				sum+=(*at).n_t_n_tooling[tf][set_id];
				pm++;
			}
			int index=(tf-1)*((*at).Temperature_Set.size())+n;
			dual_obj+=mu_t_n[index]*sum;
		}
	}
	for(int k=1;k<=(int)(*at).Key_Devices.size();k++){
		int dev=(*at).Key_Devices[k-1];
		dual_obj+=nu_k[k]*(*at).n_k_minchips[dev];
	}
	for(int p=1;p<=(int)(*at).Packages.size();p++){
		int dev=(*at).Packages[p-1];
		dual_obj+=eta_j[p]*(*at).n_p_minchips[dev];
	}
	if(!(*at).SIM_option){
		for(int i=1;i<=(*at).Num_Machine;i++){
			dual_obj+=gama_i[i];
		}
	}else{
		for(int i=1;i<=(int)(*at).SIM.size();i++){
			dual_obj+=gama_i[i];
		}
	}
	cout<<"dual_obj="<<dual_obj<<endl;
	*/
	//-----------------------------------------------------------------------------------------------construct the cost vector (starts)
	//cout<<"8"<<endl;
	int cost_id=0;
	vii pl=(*at).L_i[machine_id].int_vector.begin();
	while(pl!=(*at).L_i[machine_id].int_vector.end()){
		int l=(*pl);
		vector<tooling_configure> TS;
		//cout<<"machine_id="<<machine_id<<",l="<<l<<endl;
		(*at).get_S(TS,machine_id,l);
		vector<tooling_configure>::iterator pTS=TS.begin();
		while(pTS!=TS.end()){
			int s=(*pTS).id;
			if(from_restore){
				costv[cost_id]=-pi_l[l];
			}else{
				costv[cost_id]=-((*at).w_l[l]-(*at).eps[s])-pi_l[l];
			}
			double sum=0;
			int dev=(*at).lots_device[l];
			vii pfind=find((*at).Key_Devices.begin(),(*at).Key_Devices.end(),dev);
			if(pfind!=(*at).Key_Devices.end()){
				int k=pfind-(*at).Key_Devices.begin()+1;
				sum+=nu_k[k]*(*at).n_l_chips[l];
			}
			pfind=find((*at).Packages.begin(),(*at).Packages.end(),dev);
			if(pfind!=(*at).Packages.end()){
				int j=pfind-(*at).Packages.begin()+1;
				sum+=eta_j[j]*(*at).n_l_chips[l];
			}
			costv[cost_id]-=sum;
			cost_id++;
			pTS++;
		}
		pl++;
	}

	//cout<<"9"<<endl;
	for(int i=cost_id;i<=cost_id+(int)(*at).Lambda_i[machine_id].int_vector.size()-1;i++){
		costv[i]=0.0;
	}

	for(int tf=1;tf<=(*at).Num_Tooling_Family;tf++){
		for(int n=1;n<=(int)(*at).Temperature_Set.size();n++){
			int id=(tf-1)*(*at).Temperature_Set.size()+n;
			vector<t_set>::iterator p_set=(*at).Temperature_Set.begin()+n-1;
			vii p_int=(*p_set).set_elements.begin();
			while(p_int!=(*p_set).set_elements.end()){
				int tau=(*p_int);
				vector<int> Lambda_i_tf_tau;
				(*at).get_Lambda(Lambda_i_tf_tau,machine_id,tf,tau);
				vii pL=Lambda_i_tf_tau.begin();
				while(pL!=Lambda_i_tf_tau.end()){
					int lambda_id=(*pL);
					vii pfind=find((*at).Lambda_i[machine_id].int_vector.begin(),(*at).Lambda_i[machine_id].int_vector.end(),lambda_id);
					if(pfind==(*at).Lambda_i[machine_id].int_vector.end()){
						cout<<"could not find lambda_id, error"<<endl;
					}else{
						int offset=pfind-(*at).Lambda_i[machine_id].int_vector.begin();
						offset+=cost_id;
						vector<Lambda_item>::iterator p_Lambda=(*at).Lambda.begin()+lambda_id-1;
						vii pp=find((*p_Lambda).tooling_vec.begin(),(*p_Lambda).tooling_vec.end(),tf);
						if(pp!=(*p_Lambda).tooling_vec.end()){
							int offset2=pp-(*p_Lambda).tooling_vec.begin();
							vii pb=(*p_Lambda).b.begin()+offset2;
							costv[offset]-=mu_t_n[id]*(*pb);
						}else{
							cout<<"could not find the tooling"<<endl;
						}
					}
					pL++;
				}
				p_int++;
			}
		}
	}
	//cout<<"10"<<endl;
	//for(int id=0;id<=cost_id+(int)(*at).Lambda_i[machine_id].int_vector.size()-1;id++){
	//	costv[id]=-costv[id];
	//}
	//-----------------------------------------------------------------------------------------------construct the cost vector (ends)
	delete [] pi_l;
	delete [] mu_t_n;
	delete [] nu_k;
	delete [] eta_j;
	delete [] gama_i;
	delete [] sigma_l;
}

/*--------------------------------------------------------------------------*/
bool AT_lp::generate_vars(const BCP_vec<BCP_var*>& vars,const double * pi, const bool from_restore){

	cout<<"in AT_lp generate_vars()"<<endl;
	generated_vars.clear();

	int count_sp_optim=0;
	double gap=0.0;
	int Num_Processor;
	if(!(*at).SIM_option){
		Num_Processor=(*at).Num_Machine;
	}else{
		Num_Processor=(int)(*at).SIM.size();
	}
	int machine_id=-1;
	for(int iter=1;iter<=Num_Processor;iter++){
		if(!(*at).SIM_option){
			machine_id=iter;
		}else{
			vector<SIM_item>::iterator pSIM=(*at).SIM.begin()+iter-1;
			machine_id=(*pSIM).represented_machine_id;
		}
		//cout<<"machine_id="<<machine_id<<endl;
		int n_cols=0;
		//cout<<"size="<<(int)(*at).L_i[machine_id].int_vector.size()<<endl;
		vii pl=(*at).L_i[machine_id].int_vector.begin();
		while(pl!=(*at).L_i[machine_id].int_vector.end()){
			int l=(*pl);
			vector<tooling_configure> TS;
			(*at).get_S(TS,machine_id,l);
			n_cols+=(int)TS.size();
			pl++;
		}
		n_cols+=(int)(*at).Lambda_i[machine_id].int_vector.size();
		cout<<"n_cols="<<n_cols<<endl;
		if(n_cols<1){
			cout<<"machine "<<machine_id<<" is not used at all"<<endl;
			continue;
		}
		double objective_value;
		double * solution  = new double[n_cols];
		double * costv  = new double[n_cols];//BCP will delete

		//cout<<"construct_cost"<<endl;
		construct_cost(pi,from_restore,machine_id,costv);

		//cout<<"solve_subproblem"<<endl;
		//int return_code = solve_subproblem(machine_id, costv, solution, objective_value); 
		int return_code = solve_subproblem_sim(iter,costv,solution,objective_value);

		if(return_code){
			delete [] costv; //if solve_subproblem_sim() is used
			delete [] solution;
			count_sp_optim++;
			continue;
		}

		gap+=objective_value;

		double most_reduced_cost;
		int num_dual=(*at).Num_Lot+((*at).Num_Tooling_Family)*(int)(*at).Temperature_Set.size()
			+(int)(*at).Key_Devices.size()+(int)(*at).Packages.size();

		int offset=0;
		if(!(*at).SIM_option){
			offset=num_dual+machine_id-1;
		}else{
			offset=num_dual+iter-1;
		}
		if(from_restore){
			most_reduced_cost = objective_value - pi[offset];
		}else{
			most_reduced_cost = objective_value - pi[offset];
		}
		//need to do more test
		int test=0;
		if(test){
			vector< pair<int,int> > L_S;
			int index=0;
			pl=(*at).L_i[machine_id].int_vector.begin();
			while(pl!=(*at).L_i[machine_id].int_vector.end()){
				int l=(*pl);
				vector<tooling_configure> TS;
				(*at).get_S(TS,machine_id,l);
				vector<tooling_configure>::iterator pTS=TS.begin();
				while(pTS!=TS.end()){
					int s=(*pTS).id;
					if(solution[index]>tolerance){
						L_S.push_back(make_pair(l,s));
					}
					index++;
					pTS++;
				}
				pl++;
			}
		}

		cout<<"most_reduced_cost="<<most_reduced_cost<<endl;
		if(most_reduced_cost >= -tolerance){
			delete [] costv; //if solve_subproblem_sim() is used
			delete [] solution;
			count_sp_optim++;
			continue;
		}

		//If the column has negative reduced cost, then transform the AP point 
		//back into AAP indices and create an AAP_var.

		vector< pair<int,int> > L_S;
		int index=0;
		pl=(*at).L_i[machine_id].int_vector.begin();
		while(pl!=(*at).L_i[machine_id].int_vector.end()){
			int l=(*pl);
			vector<tooling_configure> TS;
			(*at).get_S(TS,machine_id,l);
			vector<tooling_configure>::iterator pTS=TS.begin();
			while(pTS!=TS.end()){
				int s=(*pTS).id;
				if(solution[index]>tolerance){
					L_S.push_back(make_pair(l,s));
				}
				index++;
				pTS++;
			}
			pl++;
		}
		int lambda_id=-1;
		vii pL=(*at).Lambda_i[machine_id].int_vector.begin();
		while(pL!=(*at).Lambda_i[machine_id].int_vector.end()){
			if(solution[index]>tolerance){
				lambda_id=(*pL);
				break;
			}
			index++;
			pL++;
		}
		AT_var * new_var = new AT_var(machine_id,lambda_id,L_S);
		generated_vars.push_back(new_var);
		delete [] costv; //if solve_subproblem_sim() is used
		delete [] solution;
		//output the vars
		fdebug_vars<<"i="<<machine_id<<",sim="<<(*at).machine_sim[machine_id]<<",lambda_id="<<lambda_id<<",L_S,";
		vector<pair<int,int> >::iterator pLS=L_S.begin();
		while(pLS!=L_S.end()){
			fdebug_vars<<"("<<(*pLS).first<<","<<(*pLS).second<<"),";
			pLS++;
		}
		fdebug_vars<<endl;
		fdebug_vars<<"i="<<machine_id<<",sim="<<(*at).machine_sim[machine_id]<<",lambda_id="<<lambda_id<<",time,";
		pLS=L_S.begin();
		while(pLS!=L_S.end()){
			int l=(*pLS).first;
			int s=(*pLS).second;
			fdebug_vars<<(*at).w_l[l]/(*at).rate[s]<<",";
			pLS++;
		}
		fdebug_vars<<endl;
		fdebug_vars<<"i="<<machine_id<<",sim="<<(*at).machine_sim[machine_id]<<",lambda_id="<<lambda_id<<",benefit,";
		pLS=L_S.begin();
		while(pLS!=L_S.end()){
			int l=(*pLS).first;
			int s=(*pLS).second;
			fdebug_vars<<(*at).w_l[l]-(*at).eps[s]<<",";
			pLS++;
		}
		fdebug_vars<<endl;
	}

	fdebug_vars<<"------------------------------------------------------------------------------------"<<endl;
	cout<<"gap="<<gap<<endl;
	if(count_sp_optim>=Num_Processor){
		return false;
	}else{
		return true;
	}
}

/*--------------------------------------------------------------------------*/
int AT_lp::solve_subproblem(int machine_id,double * costv,double * solution,double & objective_value){

	cout<<"in solve_subproblem()"<<endl;
	OsiXxxSolverInterface * siAP = new OsiXxxSolverInterface();

	vector<pair<int,int> > x_vector;

	int n_cols=0;
	int n_rows=0;
	vii pl=(*at).L_i[machine_id].int_vector.begin();
	while(pl!=(*at).L_i[machine_id].int_vector.end()){
		int l=(*pl);
		vector<tooling_configure> TS;
		(*at).get_S(TS,machine_id,l);
		vector<tooling_configure>::iterator pTS=TS.begin();
		while(pTS!=TS.end()){
			int s=(*pTS).id;
			x_vector.push_back(make_pair(l,s));
			pTS++;
		}
		pl++;
	}
	cout<<"size of x_vector="<<(int)x_vector.size()<<endl;
	n_cols=(int)x_vector.size()+(int)(*at).Lambda_i[machine_id].int_vector.size();
	cout<<"n_cols="<<n_cols<<endl;

	vii pL=(*at).Lambda_i[machine_id].int_vector.begin();
	while(pL!=(*at).Lambda_i[machine_id].int_vector.end()){
		int lambda_id=(*pL);
		//cout<<"lambda_id="<<lambda_id<<endl;
		vector<int> LS;
		(*at).get_L_i_lambda(LS,machine_id,lambda_id);
		//cout<<"size="<<LS.size()<<endl;
		vii pl=LS.begin();
		while(pl!=LS.end()){
			int l=(*pl);
			vector<tooling_configure> TS;
			//cout<<"machine_id="<<machine_id<<",l="<<l<<",lambda_id="<<lambda_id<<endl;
			(*at).get_S_i_l_Lambda(TS,machine_id,l,lambda_id);
			n_rows+=(int)TS.size();
			pl++;
		}
		pL++;
	}
	n_rows+=2;
	
	cout<<"in solving the subproblem, n_cols="<<n_cols<<",n_rows="<<n_rows<<endl;

	double * col_lb    = new double[n_cols];
	double * col_ub    = new double[n_cols];
	double * row_lb    = new double[n_rows];
	double * row_ub    = new double[n_rows];

	CoinFillN(col_lb, n_cols, 0.0);
	CoinFillN(col_ub, n_cols, 1.0);
	CoinFillN(row_lb, n_rows, 0.0);  
	CoinFillN(row_ub, n_rows, DBL_MAX);
	//row_ub[n_rows-2]=1.0;
	//row_ub[n_rows-1]=(*at).H[machine_id];

	cout<<"adjust for branching"<<endl;
	//Adjust for branching:
	const AT_user_data * data = dynamic_cast<const AT_user_data*>(get_user_data());

	//This should have been already taken care of using bigM, but using 
	//bounds should be used to help the LP solver.

	//For x_ijk = 1, for all s in F' such that s_ijk != 1, set lambda_s = 0
	//the branching affects both RMP and SP
	//here taking care of SP
	//set y=1
	cout<<"set y=1"<<endl;
	for(int index = 0; index < (int)data->ones.size(); index++){
		int i=data->ones[index].first;
		int lambda_id=data->ones[index].second;
		if(i==machine_id){
			int offset=0;
			vii pL=(*at).Lambda_i[machine_id].int_vector.begin();
			while(pL!=(*at).Lambda_i[machine_id].int_vector.end()){
				if((*pL)==lambda_id){
					break;
				}
				offset++;
				pL++;
			}
			col_lb[x_vector.size()+offset] = 1.0;
		}
	}
	//set y=0;
	cout<<"set y=0"<<endl;
	for(int index = 0; index < (int)data->zeros.size(); index++){
		int i=data->zeros[index].first;
		int lambda_id=data->zeros[index].second;
		if(i==machine_id){
			int offset=0;
			vii pL=(*at).Lambda_i[machine_id].int_vector.begin();
			while(pL!=(*at).Lambda_i[machine_id].int_vector.end()){
				if((*pL)==lambda_id){
					break;
				}
				offset++;
				pL++;
			}
			col_ub[x_vector.size()+offset] = 0.0;
		}
	}

	cout<<"construct subproblem matrix"<<endl;
	CoinPackedMatrix * M =  new CoinPackedMatrix(false,0,0);
	M->setDimensions(0, n_cols);

	int num_cons=0;
	cout<<"construct x<->y relationship constraints"<<endl;
	pL=(*at).Lambda_i[machine_id].int_vector.begin();
	while(pL!=(*at).Lambda_i[machine_id].int_vector.end()){
		int lambda_id=(*pL);
		vector<int> LS;
		(*at).get_L_i_lambda(LS,machine_id,lambda_id);
		vii pl=LS.begin();
		while(pl!=LS.end()){
			int l=(*pl);
			vector<tooling_configure> TS;
			(*at).get_S_i_l_Lambda(TS,machine_id,l,lambda_id);
			int s=-1;
			vector<tooling_configure>::iterator pTS=TS.begin();
			if((int)TS.size()>1){//s(i,l,lambda) is not unique !!!
				double max_rate=-1E+6;
				int max_s=-1;
				while(pTS!=TS.end()){
					int ss=(*pTS).id;
					if((*at).rate[ss]>max_rate){
						max_rate=(*at).rate[ss];
						max_s=ss;
					}
					pTS++;
				}
				s=max_s;
			}else{
				pTS=TS.begin();
				s=(*pTS).id;
			}
			//locate the positions of the x and y variables
			int y_id=(int)x_vector.size()+pL-(*at).Lambda_i[machine_id].int_vector.begin();
			int x_id=-1;
			vector<pair<int,int> >::iterator px=x_vector.begin();
			while(px!=x_vector.end()){
				if((*px).first==l&&(*px).second==s){
					x_id=px-x_vector.begin();
					break;
				}
				px++;
			}
			CoinPackedVector row;
			row.insert(y_id,1.0);
			row.insert(x_id,-1.0);
			row_lb[num_cons]=0.0;
			row_ub[num_cons]=DBL_MAX;
			M->appendRow(row);
			num_cons++;
			//cout<<"lambda_id="<<lambda_id<<",l="<<l<<",s="<<s<<endl;
			//cout<<"num_cons="<<num_cons<<endl;
			pl++;
		}//end_while
		pL++;
	}//end_while

	CoinPackedVector row;
	cout<<"construct sum(y)<=1 constraints"<<endl;
	pL=(*at).Lambda_i[machine_id].int_vector.begin();
	while(pL!=(*at).Lambda_i[machine_id].int_vector.end()){
		int offset=pL-(*at).Lambda_i[machine_id].int_vector.begin();
		int y_id=(int)x_vector.size()+offset;
		row.insert(y_id,1.0);
		pL++;
	}
	M->appendRow(row);
	row_lb[num_cons]=-DBL_MAX;
	row_ub[num_cons]=1.0;
	num_cons++;
	row.clear();

	cout<<"construct machine time constraints"<<endl;
	pl=(*at).L_i[machine_id].int_vector.begin();
	while(pl!=(*at).L_i[machine_id].int_vector.end()){
		int l=(*pl);
		vector<tooling_configure> TS;
		(*at).get_S(TS,machine_id,l);
		vector<tooling_configure>::iterator pTS=TS.begin();
		while(pTS!=TS.end()){
			int s=(*pTS).id;
			int x_id=-1;
			vector<pair<int,int> >::iterator px=x_vector.begin();
			while(px!=x_vector.end()){
				if((*px).first==l&&(*px).second==s){
					x_id=px-x_vector.begin();
					break;
				}
				px++;
			}
			double coeff=(*at).n_l_chips[l]/(*at).rate[s];
			//cout<<"x_id="<<x_id<<",coeff="<<coeff<<endl;
			row.insert(x_id,coeff);
			pTS++;
		}
		pl++;
	}
	M->appendRow(row);
	row_lb[num_cons]=-DBL_MAX;
	row_ub[num_cons]=(*at).H[machine_id];
	num_cons++;
	row.clear();

	//Note on OSI usage:
	//assignProblem assumes ownership of the arguments (and deletes memory)
	//loadProblem makes a copy of the arguments
	cout<<"assign subproblem"<<endl;
	siAP->assignProblem(M, col_lb, col_ub, costv, row_lb, row_ub);

	for(int i=0;i<=(int)x_vector.size()-1;i++){
		siAP->setInteger(i);
	}

	cout<<"solve the subproblem by bAb"<<endl;
	//solve the SP MIP
	siAP->writeLp("subproblem");
	siAP->branchAndBound();

	int return_code = 0;
	if(siAP->isProvenOptimal()){
		objective_value = siAP->getObjValue();
		CoinDisjointCopyN(siAP->getColSolution(), n_cols, solution);
	}
	else{
		return_code = 1;
	}
	delete siAP;

	return return_code;
}

/*--------------------------------------------------------------------------*/
void AT_lp::vars_to_cols(const BCP_vec<BCP_cut*>& cuts,BCP_vec<BCP_var*>& vars,BCP_vec<BCP_col*>& cols,
			  const BCP_lp_result& lpres,BCP_object_origin origin, bool allow_multiple)
{
	/*
	Expand the variable (AP point) into a column in the LP:
	min {s in F'}                c_s lambda_s             
	sum {s in F', jk in J x K} s_ijk lambda_s = 1, forall i in I (5)
	sum {s in F'}                    lambda_s = 1                (6)
	*/
	cout<<"in AT_lp vars_to_cols()"<<endl;
	const int varnum = vars.size();
	if (varnum == 0)
		return; 

	cols.reserve(varnum);
	for (int c = 0; c < varnum; ++c) {   
		//cout<<"c="<<c<<endl;
		const AT_var* mvar = dynamic_cast<const AT_var*>(vars[c]);
		if(mvar){
			//(*mvar).print();
			//cout<<"cost of a column"<<endl;
			double cost=0.0;
			vector<int> LL;
			int machine_id=(*mvar).i;
			int lambda_id =(*mvar).lambda_id;
			vector<pair<int,int> >::const_iterator ppp=(*mvar).L_S.begin();
			while(ppp!=(*mvar).L_S.end()){
				int l=(*ppp).first;
				int s=(*ppp).second;
				cost+=(*at).w_l[l]-(*at).eps[s];
				ppp++;
			}
			//cout<<"cost="<<cost<<",machine_id="<<machine_id<<",lambda_id="<<lambda_id<<endl;

			//cout<<"lot assignment"<<endl;
			vector<double> coeff;
			coeff.assign((*at).Num_Lot,0.0); //initialize coeff
			vector<pair<int,int> >::const_iterator pLS=(*mvar).L_S.begin();
			while(pLS!=(*mvar).L_S.end()){
				int l=(*pLS).first;
				coeff[l-1]=1.0;
				pLS++;
			}

			//cout<<"tooling restriction"<<endl;
			for(int tf=1;tf<=(*at).Num_Tooling_Family;tf++){
				for(int n=1;n<=(int)(*at).Temperature_Set.size();n++){
					double count=0.0;
					vector<t_set>::iterator p_set=(*at).Temperature_Set.begin()+n-1;
					vii pint=(*p_set).set_elements.begin();
					while(pint!=(*p_set).set_elements.end()){
						int tau=(*pint);
						vector<int> Lambda_i_tf_tau;
						(*at).get_Lambda(Lambda_i_tf_tau,machine_id,tf,tau);
						vii p_Lambda=Lambda_i_tf_tau.begin();
						while(p_Lambda!=Lambda_i_tf_tau.end()){
							if((*p_Lambda)==lambda_id){
								vector<Lambda_item>::iterator pL=(*at).Lambda.begin()+lambda_id-1;
								vii p_find=find((*pL).tooling_vec.begin(),(*pL).tooling_vec.end(),tf);
								int offset=p_find-(*pL).tooling_vec.begin();
								vii pb=(*pL).b.begin()+offset;
								count+=(*pb);
							}
							p_Lambda++;
						}
						pint++;
					}
					coeff.push_back(count);
				}
			}

			//cout<<"key devices requirement"<<endl;
			for(int k=1;k<=(int)(*at).Key_Devices.size();k++){
				int dev=(*at).Key_Devices[k-1];
				//cout<<"dev="<<dev<<endl;
				double count=0.0;
				vector<int> LS;
				(*at).get_L(LS,machine_id,dev);
				vii pl=LS.begin();
				while(pl!=LS.end()){
					int l=(*pl);
					vector<tooling_configure> TS;
					(*at).get_S(TS,machine_id,l);
					vector<tooling_configure>::iterator pTS=TS.begin();
					while(pTS!=TS.end()){
						int s=(*pTS).id;
						vector<pair<int,int> >::const_iterator pLS=(*mvar).L_S.begin();
						while(pLS!=(*mvar).L_S.end()){
							if((*pLS).first==l&&(*pLS).second==s){
								count+=(*at).n_l_chips[l];
								break;
							}
							pLS++;
						}
						pTS++;
					}
					pl++;
				}
				coeff.push_back(count);
			}

			//cout<<"package devices"<<endl;
			//cout<<"size="<<(*at).Packages.size()<<endl;
			vii pj=(*at).Packages.begin();
			while(pj!=(*at).Packages.end()){
				int dev=(*pj);
				double count=0.0;
				vector<int> LS;
				(*at).get_L(LS,machine_id,dev);
				vii pl=LS.begin();
				while(pl!=LS.end()){
					int l=(*pl);
					vector<tooling_configure> TS;
					(*at).get_S(TS,machine_id,l);
					vector<tooling_configure>::iterator pTS=TS.begin();
					while(pTS!=TS.end()){
						int s=(*pTS).id;
						vector<pair<int,int> >::const_iterator pLS=(*mvar).L_S.begin();
						while(pLS!=(*mvar).L_S.end()){
							if((*pLS).first==l&&(*pLS).second==s){
								count+=(*at).n_l_chips[l];
								break;
							}
							pLS++;
						}
						pTS++;
					}
					pl++;
				}
				coeff.push_back(count);
				pj++;
			}
			//cout<<"before conv, size of coeff="<<coeff.size()<<endl;
			//cout<<"convexity constraint"<<endl;
			if(!(*at).SIM_option){
				for(int i=1;i<=(*at).Num_Machine;i++){
					if(i==machine_id){
						coeff.push_back(1.0);
					}else{
						coeff.push_back(0.0);
					}
				}
			}else{
				for(int i=1;i<=(int)(*at).SIM.size();i++){
					vector<SIM_item>::iterator pSIM=(*at).SIM.begin()+i-1;
					if((*pSIM).represented_machine_id==machine_id){
						coeff.push_back(1.0);
					}else{
						coeff.push_back(0.0);
					}
				}
			}
			//additional constraints for CG stability
			for(int l=1;l<=(*at).Num_Lot;l++){
				coeff.push_back(0.0);
			}

			//cout<<"size of coeff="<<coeff.size()<<endl;
			CoinPackedVector col;
			for(int i=0;i<=(int)coeff.size()-1;i++){
				col.insert(i,coeff[i]);
			}
			//cout<<"LB="<<vars[c]->lb()<<",UB="<<vars[c]->ub()<<endl;
			cols.unchecked_push_back(new BCP_col(col,-1.0*cost,vars[c]->lb(), vars[c]->ub()));
		}else{
			cout<<"in var_to_col, dynamic cast error"<<endl;
		}
	}
}

void AT_lp::create_subprob_matrix(){
	cout<<"in AT_lp create_subprob_matrix()"<<endl;
	for(int sim=1;sim<=(int)(*at).SIM.size();sim++){
		cout<<"sim="<<sim<<endl;
		vector<SIM_item>::iterator pSIM=(*at).SIM.begin()+sim-1;
		int machine_id=(*pSIM).represented_machine_id;
		cout<<"machine_id="<<machine_id<<endl;
		//create x_vector for x(l,s) given i
		vector<pair<int,int> > x_vector;
		int n_cols=0;
		int n_rows=0;
		if((int)(*at).L_i[machine_id].int_vector.size()<1){ continue;}// the sim is not used at all;

		cout<<"size of L(i)="<<(*at).L_i[machine_id].int_vector.size()<<endl;
		vii pl=(*at).L_i[machine_id].int_vector.begin();
		while(pl!=(*at).L_i[machine_id].int_vector.end()){
			int l=(*pl);
			vector<tooling_configure> TS;
			(*at).get_S(TS,machine_id,l);
			vector<tooling_configure>::iterator pTS=TS.begin();
			while(pTS!=TS.end()){
				int s=(*pTS).id;
				x_vector.push_back(make_pair(l,s));
				pTS++;
			}
			pl++;
		}
		cout<<"size of x_vector="<<(int)x_vector.size()<<endl;
		n_cols=(int)x_vector.size()+(int)(*at).Lambda_i[machine_id].int_vector.size();
		cout<<"n_cols="<<n_cols<<endl;

		vii pL=(*at).Lambda_i[machine_id].int_vector.begin();
		while(pL!=(*at).Lambda_i[machine_id].int_vector.end()){
			int lambda_id=(*pL);
			//cout<<"lambda_id="<<lambda_id<<endl;
			vector<int> LS;
			(*at).get_L_i_lambda(LS,machine_id,lambda_id);
			//cout<<"size="<<LS.size()<<endl;
			vii pl=LS.begin();
			while(pl!=LS.end()){
				int l=(*pl);
				vector<tooling_configure> TS;
				//cout<<"machine_id="<<machine_id<<",l="<<l<<",lambda_id="<<lambda_id<<endl;
				(*at).get_S_i_l_Lambda(TS,machine_id,l,lambda_id);
				n_rows+=(int)TS.size();
				pl++;
			}
			pL++;
		}
		n_rows+=2;
		cout<<"in creating the subproblem, n_cols="<<n_cols<<",n_rows="<<n_rows<<endl;
		//----------------------------------------------------
		subprob_col_lb[sim]    = new double[n_cols];
		subprob_col_ub[sim]    = new double[n_cols];
		subprob_row_lb[sim]    = new double[n_rows];
		subprob_row_ub[sim]    = new double[n_rows];

		CoinFillN(subprob_col_lb[sim], n_cols, 0.0);
		CoinFillN(subprob_col_ub[sim], n_cols, 1.0);
		CoinFillN(subprob_row_lb[sim], n_rows, 0.0);  
		CoinFillN(subprob_row_ub[sim], n_rows, DBL_MAX);
		//-----------------------------------------------------------------------------
		cout<<"construct subproblem matrix"<<endl;
		subprob_M[sim] =  new CoinPackedMatrix(false,0,0);
		(subprob_M[sim])->setDimensions(0, n_cols);

		int num_cons=0;
		cout<<"construct x<->y relationship constraints"<<endl;
		pL=(*at).Lambda_i[machine_id].int_vector.begin();
		while(pL!=(*at).Lambda_i[machine_id].int_vector.end()){
			int lambda_id=(*pL);
			vector<int> LS;
			(*at).get_L_i_lambda(LS,machine_id,lambda_id);
			vii pl=LS.begin();
			while(pl!=LS.end()){
				int l=(*pl);
				vector<tooling_configure> TS;
				(*at).get_S_i_l_Lambda(TS,machine_id,l,lambda_id);
				int s=-1;
				vector<tooling_configure>::iterator pTS=TS.begin();
				if((int)TS.size()>1){//s(i,l,lambda) is not unique !!!
					double max_rate=-1E+6;
					int max_s=-1;
					while(pTS!=TS.end()){
						int ss=(*pTS).id;
						if((*at).rate[ss]>max_rate){
							max_rate=(*at).rate[ss];
							max_s=ss;
						}
						pTS++;
					}
					s=max_s;
				}else{
					pTS=TS.begin();
					s=(*pTS).id;
				}
				//locate the positions of the x and y variables
				int y_id=(int)x_vector.size()+pL-(*at).Lambda_i[machine_id].int_vector.begin();
				int x_id=-1;
				vector<pair<int,int> >::iterator px=x_vector.begin();
				while(px!=x_vector.end()){
					if((*px).first==l&&(*px).second==s){
						x_id=px-x_vector.begin();
						break;
					}
					px++;
				}
				CoinPackedVector row;
				row.insert(y_id,1.0);
				row.insert(x_id,-1.0);
				subprob_row_lb[sim][num_cons]=0.0;
				subprob_row_ub[sim][num_cons]=DBL_MAX;
				(subprob_M[sim])->appendRow(row);
				num_cons++;
				//cout<<"lambda_id="<<lambda_id<<",l="<<l<<",s="<<s<<endl;
				//cout<<"num_cons="<<num_cons<<endl;
				pl++;
			}//end_while
			pL++;
		}//end_while

		CoinPackedVector row;
		cout<<"construct sum(y)<=1 constraints"<<endl;
		pL=(*at).Lambda_i[machine_id].int_vector.begin();
		while(pL!=(*at).Lambda_i[machine_id].int_vector.end()){
			int offset=pL-(*at).Lambda_i[machine_id].int_vector.begin();
			int y_id=(int)x_vector.size()+offset;
			row.insert(y_id,1.0);
			pL++;
		}
		(subprob_M[sim])->appendRow(row);
		subprob_row_lb[sim][num_cons]=-DBL_MAX;
		subprob_row_ub[sim][num_cons]=1.0;
		num_cons++;
		row.clear();

		cout<<"construct machine time constraints"<<endl;
		pl=(*at).L_i[machine_id].int_vector.begin();
		while(pl!=(*at).L_i[machine_id].int_vector.end()){
			int l=(*pl);
			vector<tooling_configure> TS;
			(*at).get_S(TS,machine_id,l);
			vector<tooling_configure>::iterator pTS=TS.begin();
			while(pTS!=TS.end()){
				int s=(*pTS).id;
				int x_id=-1;
				vector<pair<int,int> >::iterator px=x_vector.begin();
				while(px!=x_vector.end()){
					if((*px).first==l&&(*px).second==s){
						x_id=px-x_vector.begin();
						break;
					}
					px++;
				}
				double coeff=(*at).n_l_chips[l]/(*at).rate[s];
				//cout<<"x_id="<<x_id<<",coeff="<<coeff<<endl;
				row.insert(x_id,coeff);
				pTS++;
			}
			pl++;
		}
		(subprob_M[sim])->appendRow(row);
		subprob_row_lb[sim][num_cons]=-DBL_MAX;
		subprob_row_ub[sim][num_cons]=(*at).H[machine_id];
		num_cons++;
		row.clear();
	}
}
//--------------------------------------------------------------------------------------------------------------
int AT_lp::solve_subproblem_sim(int sim,double * costv,double * solution,double & objective_value){

	cout<<"in solve_subproblem_sim(), sim="<<sim<<endl;
	OsiXxxSolverInterface * siAP = new OsiXxxSolverInterface();

	vector<SIM_item>::iterator pSIM=(*at).SIM.begin()+sim-1;
	int machine_id=(*pSIM).represented_machine_id;

	cout<<"adjust for branching"<<endl;
	//Adjust for branching:
	const AT_user_data * data = dynamic_cast<const AT_user_data*>(get_user_data());

	//This should have been already taken care of using bigM, but using 
	//bounds should be used to help the LP solver.
	int x_vector_size=0;
	vii pl=(*at).L_i[machine_id].int_vector.begin();
	while(pl!=(*at).L_i[machine_id].int_vector.end()){
		int l=(*pl);
		vector<tooling_configure> TS;
		(*at).get_S(TS,machine_id,l);
		vector<tooling_configure>::iterator pTS=TS.begin();
		while(pTS!=TS.end()){
			int s=(*pTS).id;
			x_vector_size++;
			pTS++;
		}
		pl++;
	}
	//For x_ijk = 1, for all s in F' such that s_ijk != 1, set lambda_s = 0
	//the branching affects both RMP and SP
	//here taking care of SP
	//set y=1
	cout<<"set y=1"<<endl;
	for(int index = 0; index < (int)data->ones.size(); index++){
		int i=data->ones[index].first;
		int lambda_id=data->ones[index].second;
		if(i==machine_id){
			int offset=0;
			vii pL=(*at).Lambda_i[machine_id].int_vector.begin();
			while(pL!=(*at).Lambda_i[machine_id].int_vector.end()){
				if((*pL)==lambda_id){
					break;
				}
				offset++;
				pL++;
			}
			subprob_col_lb[sim][x_vector_size+offset] = 1.0;
		}
	}
	//set y=0;
	cout<<"set y=0"<<endl;
	for(int index = 0; index < (int)data->zeros.size(); index++){
		int i=data->zeros[index].first;
		int lambda_id=data->zeros[index].second;
		if(i==machine_id){
			int offset=0;
			vii pL=(*at).Lambda_i[machine_id].int_vector.begin();
			while(pL!=(*at).Lambda_i[machine_id].int_vector.end()){
				if((*pL)==lambda_id){
					break;
				}
				offset++;
				pL++;
			}
			subprob_col_ub[sim][x_vector_size+offset] = 0.0;
		}
	}

	//Note on OSI usage:
	//assignProblem assumes ownership of the arguments (and deletes memory)
	//loadProblem makes a copy of the arguments
	cout<<"load subproblem"<<endl;
	siAP->loadProblem((*subprob_M[sim]), subprob_col_lb[sim], subprob_col_ub[sim], costv, subprob_row_lb[sim], subprob_row_ub[sim]);

	for(int i=0;i<=x_vector_size-1;i++){
		siAP->setInteger(i);
	}

	cout<<"solve the subproblem by bAb"<<endl;
	//solve the SP MIP
	siAP->writeLp("subproblem");
	siAP->branchAndBound();

	int return_code = 0;
	if(siAP->isProvenOptimal()){
		objective_value = siAP->getObjValue();
		int n_cols=x_vector_size+(int)(*at).Lambda_i[machine_id].int_vector.size();
		CoinDisjointCopyN(siAP->getColSolution(), n_cols, solution);
	}
	else{
		return_code = 1;
	}
	delete siAP;

	return return_code;
}
