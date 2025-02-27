// Copyright (C) 2002, International Business Machines
// Corporation and others.  All Rights Reserved.

#ifndef CoinPresolveForcing_H
#define CoinPresolveForcing_H

#include "CoinPresolveMatrix.hpp"

/*!
  \file
*/

#define	IMPLIED_BOUND	7

/*! \class forcing_constraint_action
    \brief Detect and process forcing constraints and useless constraints

  A constraint is useless if the bounds on the variables prevent the constraint
  from ever being violated.

  A constraint is a forcing constraint if the bounds on the constraint force
  the value of an involved variable to one of its bounds. A constraint can
  force more than one variable.
*/
class forcing_constraint_action : public CoinPresolveAction {
  forcing_constraint_action();
  forcing_constraint_action(const forcing_constraint_action& rhs);
  forcing_constraint_action& operator=(const forcing_constraint_action& rhs);

  struct action {
    const int *rowcols;
    const double *bounds;
    int row;
    int nlo;
    int nup;
  };

  const int nactions_;
  // actions_ is owned by the class and must be deleted at destruction
  const action *const actions_;

  forcing_constraint_action(int nactions,
		      const action *actions,
		      const CoinPresolveAction *next) :
    CoinPresolveAction(next),
    nactions_(nactions), actions_(actions) {}

 public:
  const char *name() const;

  static const CoinPresolveAction *presolve(CoinPresolveMatrix * prob,
					 const CoinPresolveAction *next);

  void postsolve(CoinPostsolveMatrix *prob) const;

  ~forcing_constraint_action();
};

#endif
