// Copyright (C) 2000, International Business Machines
// Corporation and others.  All Rights Reserved.
#ifndef CglKnapsackCover_H
#define CglKnapsackCover_H

#include <string>

#include "CglCutGenerator.hpp"

/** Knapsack Cover Cut Generator Class */
class CglKnapsackCover : public CglCutGenerator {
   friend void CglKnapsackCoverUnitTest(const OsiSolverInterface * siP,
					const std::string mpdDir );

public:
   /** A method to set which rows should be tested for knapsack covers */
   void setTestedRowIndices(int num, const int* ind);

   /**@name Generate Cuts */
  //@{
  /** Generate knapsack cover cuts for the model of the solver interface, si. 
      Insert the generated cuts into OsiCut, cs.
  */
  virtual void generateCuts(const OsiSolverInterface & si, OsiCuts & cs,
			    const CglTreeInfo info = CglTreeInfo()) const;
  //@}

  /**@name Constructors and destructors */
  //@{
  /// Default constructor 
  CglKnapsackCover ();
 
  /// Copy constructor 
  CglKnapsackCover (
    const CglKnapsackCover &);

  /// Clone
  virtual CglCutGenerator * clone() const;

  /// Assignment operator 
  CglKnapsackCover &
    operator=(
    const CglKnapsackCover& rhs);
  
  /// Destructor 
  virtual
    ~CglKnapsackCover ();
  /// Create C++ lines to get to current state
  virtual std::string generateCpp( FILE * fp);
  //@}


  /**@name Sets and gets */
  //@{
  /// Set limit on number in knapsack
  inline void setMaxInKnapsack(int value)
           { if (value>0) maxInKnapsack_ = value;}
  /// get limit on number in knapsack
  inline int getMaxInKnapsack() const
           {return maxInKnapsack_;}
  /// Switch off expensive cuts
  inline void switchOffExpensive()
  { expensiveCuts_=false;}
  /// Switch on expensive cuts
  inline void switchOnExpensive()
  { expensiveCuts_=true;}
private:
  
 // Private member methods


  /**@name Private methods */
  //@{

  /** deriveAKnapsack 
                 returns 1 if it is able to derive
                 a (canonical) knapsack inequality
                in binary variables of the form ax<=b 
                 from the rowIndex-th  row in the model, 
                returns 0 otherwise.
  */
  int deriveAKnapsack(
    const OsiSolverInterface & si, 
    OsiCuts & cs,
    CoinPackedVector & krow,
    bool treatAsLRow,
    double & b,
    int *  complement,
    double *  xstar,
    int rowIndex,
    int numberElements,
    const int * index,
    const double * element) const;

  int deriveAKnapsack(
    const OsiSolverInterface & si, 
    OsiCuts & cs,
    CoinPackedVector & krow,
    double & b,
    int *  complement,
    double *  xstar,
    int rowIndex,
    const CoinPackedVectorBase & matrixRow) const;

  /** Find a violated minimal cover from 
 a canonical form knapsack inequality by
 solving the -most- violated cover problem
 and postprocess to ensure minimality
  */
  int findExactMostViolatedMinCover(
      int nCols, 
      int row,
      CoinPackedVector & krow,
      double b, 
      double *  xstar, 
      CoinPackedVector & cover,
      CoinPackedVector & remainder) const ;

  /** Find the most violate minimum cover by solving the lp-relaxation of the
      most-violate-min-cover problem 
  */
  int findLPMostViolatedMinCover(
      int nCols,
      int row,
      CoinPackedVector & krow,
      double & b,
      double * xstar, 
      CoinPackedVector & cover,
      CoinPackedVector & remainder) const;  
  
/// find a minimum cover by a simple greedy approach
  int findGreedyCover(
      int row,
      CoinPackedVector & krow,
      double & b,
      double * xstar,
      CoinPackedVector & cover,
      CoinPackedVector & remainder
      ) const;

  /// lift the cover inequality
  int liftCoverCut(
     double & b,
     int nRowElem,
     CoinPackedVector & cover,
     CoinPackedVector & remainder,
     CoinPackedVector & cut ) const;
 
  /// sequence-independent lift and uncomplement and add the resulting cut to the cut set
  int liftAndUncomplementAndAdd(
     double rowub,
     CoinPackedVector & krow,
     double & b,
     int * complement,
     int row,
     CoinPackedVector & cover,
     CoinPackedVector & remainder,
     OsiCuts & cs ) const;

  /// sequence-dependent lift, uncomplement and add the resulting cut to the cut set
void seqLiftAndUncomplementAndAdd(
      int nCols,
      double * xstar, 
      int * complement,
      int row,
      int nRowElem,
      double & b,
      CoinPackedVector & cover,      // need not be violated
      CoinPackedVector & remainder,
      OsiCuts & cs ) const;

  /// sequence-dependent lift binary variables either up or down, uncomplement and add to the cut set
void liftUpDownAndUncomplementAndAdd(
         int nCols,
         double * xstar, 
         int * complement,
         int row,
         int nRowElem,
         double & b,

         // the following 3 packed vectors partition the krow:
         CoinPackedVector & fracCover, // vars have frac soln values in lp relaxation
                                       // and form cover with the vars atOne
         CoinPackedVector & atOne,     // vars have soln value of 1 in lp relaxation
                                       // and together with fracCover form minimal (?) cover. 
         CoinPackedVector & remainder,
         OsiCuts & cs ) const;

  /// find a cover using a variation of the logic found in OSL (w/o SOS)
  int findPseudoJohnAndEllisCover (
     int row,
     CoinPackedVector & krow,
     double & b,
     double * xstar,                     
     CoinPackedVector & cover,  
     CoinPackedVector & remainder) const;

  /// find a cover using the basic logic found in OSL (w/o SOS)
  int findJohnAndEllisCover (
     int row,
     CoinPackedVector & krow,
     double & b,
     double * xstar,                     
     CoinPackedVector & fracCover,  
     CoinPackedVector & atOnes,  
     CoinPackedVector & remainder) const;


  /** A C-style implementation of the Horowitz-Sahni exact solution 
   procedure for solving knapsack problem. 
   
   (ToDo: implement the more efficient dynamic programming approach)

   (Reference: Martello and Toth, Knapsack Problems, Wiley, 1990, p30.)
  */
  int exactSolveKnapsack(
      int n, 
      double c, 
      double const *pp, 
      double const *ww,
      double & z, 
      int * x) const;

  //@}

  // Private member data

  /**@name Private member data */
  //@{
  /// epsilon
  double epsilon_;  
  /// Tolerance to use for violation - bigger than epsilon_
  double epsilon2_;
  /// 1-epsilon
  double onetol_;  
  /// Maximum in knapsack
  int maxInKnapsack_;
   /** which rows to look at. If specified, only these rows will be considered
       for generating knapsack covers. Otherwise all rows will be tried */
   int numRowsToCheck_;
   int* rowsToCheck_;
  /// exactKnapsack can be expensive - this switches off some
  bool expensiveCuts_;
  //@}
};

//#############################################################################
/** A function that tests the methods in the CglKnapsackCover class. The
    only reason for it not to be a member method is that this way it doesn't
    have to be compiled into the library. And that's a gain, because the
    library should be compiled with optimization on, but this method should be
    compiled with debugging. */
void CglKnapsackCoverUnitTest(const OsiSolverInterface * siP,
			      const std::string mpdDir );
  
#endif
