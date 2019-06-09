// $Id: AAP_lp_branch.cpp,v 1.2 2003/12/03 01:52:30 magh Exp $

/* ------------------------------------------------------------------------
 Author: Matthew Galati (magh@lehigh.edu)

 (c) Copyright 2003 Lehigh University. All Rights Reserved.

 This software is licensed under the Common Public License. Please see 
 accompanying file for terms.    
---------------------------------------------------------------------------*/

#include "AT_lp.hpp"
#include "AT_user_data.hpp"

/*--------------------------------------------------------------------------*/
BCP_branching_decision AT_lp ::select_branching_candidates(const BCP_lp_result& lpres,
			    const BCP_vec<BCP_var*>& vars,
			    const BCP_vec<BCP_cut*>& cuts,
			    const BCP_lp_var_pool& local_var_pool,
			    const BCP_lp_cut_pool& local_cut_pool,
			    BCP_vec<BCP_lp_branching_object*>& cans)
{
	/*
	Create candidate branching objects using a disjunction on:
	x_ijk = sum{s in F'} s_ijk lambda_s

	For x_ijk = 0, for all s in F' such that s_ijk  = 1, set lambda_s = 0
	For x_ijk = 1, for all s in F' such that s_ijk != 1, set lambda_s = 0

	Possible return values are:
	BCP_DoNotBranch_Fathomed : The node should be fathomed without branching
	BCP_DoNotBranch          : BCP should continue to work on this node
	BCP_DoBranch :           : Branching must be done. In this case the 
	method returns the branching object candidates 
	*/

	// Do not branch if new variables have been generated
	if(local_var_pool.size() > 0)
		return BCP_DoNotBranch;

	//As candidates select those closest to 0.5
	branch_close_to_half(lpres, vars, cans);

	if (cans.size() == 0) {
		throw BCP_fatal_error("\
							  LP : No var/cut in pool but couldn't select branching object.\n");
	}
	return BCP_DoBranch;
}

/*--------------------------------------------------------------------------*/
void AT_lp::set_user_data_for_children(BCP_presolved_lp_brobj* best,const int selected){

	// Store the information about which variable was branched on and
	// what it was set to in user_data for use in restricting the column 
	// generation routine in generate_vars.

	const AT_user_data * data = dynamic_cast<const AT_user_data*>(get_user_data());

	AT_user_data * data0 = new AT_user_data(*data);
	AT_user_data * data1 = new AT_user_data(*data);

	vector<pair<int,pair<int,int> > >::const_iterator vi=cand_to_variable.begin()+selected;
	data0->zeros.push_back(make_pair((*vi).second.first,(*vi).second.first));
	data1->ones.push_back(make_pair((*vi).second.first,(*vi).second.first));

	best->user_data()[0] = data0;
	best->user_data()[1] = data1;
}

/*--------------------------------------------------------------------------*/
void AT_lp:: branch_close_to_half(const BCP_lp_result& lpres,const BCP_vec<BCP_var*>& vars,BCP_vec<BCP_lp_branching_object*>& cans){

	//Order first by closeness to 0.5, then by cost 
	//increasing order on | x* - 0.5 |, decreasing on cost
	//x*_ijk = sum{s in F'} s_ijk lambda*_s

	//var_candidates: i,lambda_id -> y_i,lambda_id
	//branch on y(i,lambda)
	
	vector< pair<pair<int,int>,double> > var_candidates;

	for(int i = 1; i<=(*at).Num_Machine; i++){
		vii pL=(*at).Lambda_i[i].int_vector.begin();
		while(pL!=(*at).Lambda_i[i].int_vector.end()){
			int lambda_id=(*pL);
			//filling the values for y(i,lambda_id)
			double y_value=0.0;
			BCP_vec<BCP_var*>::const_iterator pv=vars.begin();
			while(pv!=vars.end()){
				int pos=pv-vars.begin();
				double value=lpres.x()[pos];
				if(value>tolerance){
					const AT_var * mvar=dynamic_cast<const AT_var*>(*pv);
					if(mvar){
						if((*mvar).i==i && (*mvar).lambda_id==lambda_id){
							y_value+=value;
						}
					}else{
						cout<<"dynamic cast fail!!!"<<endl;
					}
				}
				pv++;
			}
			y_value-=0.5; //is it far away from 0.5?
			var_candidates.push_back(make_pair(make_pair(i,lambda_id),y_value)); //temperary set y=0
			pL++;
		}
	}

	const int strong_branch_cands = lp_par.entry(AT_lp_par::AT_StrongBranch_Can);

	//sort var_candidates according to the y value
	partial_sort(var_candidates.begin(),var_candidates.begin()+strong_branch_cands,var_candidates.end(),is_less_than());

	cand_to_variable.clear();
	append_branching_vars(lpres, vars, var_candidates, cans);
}

/*--------------------------------------------------------------------------*/
void AT_lp::append_branching_vars(const BCP_lp_result& lpres,const BCP_vec<BCP_var*>& vars
								  ,const vector< pair<pair<int,int>,double> > & var_candidates
								  ,BCP_vec<BCP_lp_branching_object*>& cans)
{
	const int strong_branch_cands = lp_par.entry(AT_lp_par::AT_StrongBranch_Can);
	const int varnum = vars.size();

	int count = 0;
	vector< pair<pair<int,int>,double> >::const_iterator vi;
	for(vi = var_candidates.begin(); vi != var_candidates.end(); vi++){
		if(count >= strong_branch_cands) break;

		cand_to_variable.push_back(make_pair(count,make_pair((*vi).first.first,(*vi).first.second)));

		/*
		Branching candidates are constructed as a BCP_lp_branching_object 
		with the arguments:

		const int children                 - number of children for this object
		BCP_vec<BCP_var*>* const new_vars  - vars to add
		BCP_vec<BCP_cut*>* const new_cuts  - cuts to add
		const BCP_vec<int>* const fvp      - forced variable position
		const BCP_vec<int>* const fcp      - forced constraint position
		const BCP_vec<double>* const fvb   - forced variable bound
		const BCP_vec<double>* const fcb   - forced constraint bound
		const BCP_vec<int>* const ivp      - implied variable position
		const BCP_vec<int>* const icp      - implied constraint position
		const BCP_vec<double>* const ivb   - implied variable bound
		const BCP_vec<double>* const icb   - implied constraint bound   

		length of fvb = number of children * 2 * length of fvp

		For 2 children, for each position fvb holds the new bound
		for child 1 (x_ijk = 0) lb and ub and child 2 (x_ijk = 1) lb and ub
		*/

		BCP_vec<int> fvp;    
		BCP_vec<double> fvb;
		vector<bool> contains_i_lambda;

		/*
		For x_ijk = 0, for all s in F' such that s_ijk  = 1, set lambda_s = 0
		For x_ijk = 1, for all s in F' such that s_ijk != 1, set lambda_s = 0

		So, essentially, *every* column will have its bounds adjusted in one
		of the  children. If ijk is in this s then we want to reset its bounds 
		in child 1, if not, then we want to reset its bounds in child 2.
		*/

		for(int s = 0; s < varnum; ++s) {//loop over lambda

			//if this variable has already been branched on, skip it
			if(vars[s]->is_fixed()) continue;

			vector<int>::iterator pos;
			AT_var* mvar = dynamic_cast<AT_var*>(vars[s]);
			if(mvar){
				int i = (*vi).first.first;
				int lambda_id=(*vi).first.second;
				if((*mvar).i==i && (*mvar).lambda_id==lambda_id){
					contains_i_lambda.push_back(true);
				}else{
					contains_i_lambda.push_back(false);
				}
				fvp.push_back(s);	 
			}else{
				cout<<"dynamic cast error!!!"<<endl;
			}
		}

		//y_i_lambda = 0 (child 1)
		for(unsigned int i = 0; i < fvp.size(); i++){
			if(contains_i_lambda[i]){//set to 0
				fvb.push_back(vars[fvp[i]]->lb());   
				fvb.push_back(0.0);
			}
			else{              //do nothing
				fvb.push_back(vars[fvp[i]]->lb());   
				fvb.push_back(vars[fvp[i]]->ub());
			}
		}

		//y_i_lambda = 1 (child 2)
		for(unsigned int i = 0; i < fvp.size(); i++){
			if(!contains_i_lambda[i]){//set to 0
				fvb.push_back(vars[fvp[i]]->lb());   
				fvb.push_back(0.0);
			}
			else{                //do nothing
				fvb.push_back(vars[fvp[i]]->lb());   
				fvb.push_back(vars[fvp[i]]->ub());
			}
		}

		cans.push_back(new BCP_lp_branching_object(2, 0, 0, &fvp, 0, &fvb, 0, 0, 0, 0, 0));

#ifdef DBG_PRINT
		cout << "Candidate: " << count << " branch on var with i=" << (*vi).first.first<<",lambda_id="<<(*vi).first.second<<endl;
		for(unsigned int i = 0; i < fvp.size(); i++){
			cout << "pos : " << fvp[i];
			cout << " (lb, ub) : (" << fvb[2*i] << ", " << fvb[2*i+1] << ")" 
				<< " (lb, ub) : (" << fvb[(2*fvp.size()) + (2*i)] 
				<< ", " << fvb[(2*fvp.size()) + (2*i)+1] << ")" << endl;
		}
		for(unsigned int i = 0; i < fvb.size(); i++)
			cout << fvb[i] << " ";
		cout << endl;
#endif
		count++;
	}
}
