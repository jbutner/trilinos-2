/* @HEADER@ */
// ************************************************************************
// 
//                              Sundance
//                 Copyright (2005) Sandia Corporation
// 
// Copyright (year first published) Sandia Corporation.  Under the terms 
// of Contract DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government 
// retains certain rights in this software.
// 
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//  
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//                                                                                 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA                                                                                
// Questions? Contact Kevin Long (krlong@sandia.gov), 
// Sandia National Laboratories, Livermore, California, USA
// 
// ************************************************************************
/* @HEADER@ */

#ifndef SUNDANCE_NONLINEARUNARYOP_H
#define SUNDANCE_NONLINEARUNARYOP_H

#include "SundanceDefs.hpp"
#include "SundanceUnaryFunctor.hpp"
#include "SundanceDerivative.hpp"
#include "SundanceUnaryExpr.hpp"
#include "SundanceDeriv.hpp"
#include "SundanceMap.hpp"
#include "SundanceSet.hpp"
#include "SundanceMultipleDeriv.hpp"
#include "SundanceNonlinearUnaryOpEvaluator.hpp"


namespace Sundance
{
using namespace Sundance;
using namespace Teuchos;




/**
 *
 */
class NonlinearUnaryOp : public UnaryExpr,
                         public GenericEvaluatorFactory<NonlinearUnaryOp, NonlinearUnaryOpEvaluator>
{
public:
  /** construct with an argument and the functor defining the operation */
  NonlinearUnaryOp(const RCP<ScalarExpr>& arg, 
    const RCP<UnaryFunctor>& op);

  /** virtual destructor */
  virtual ~NonlinearUnaryOp() {;}

  /** Write a simple text description suitable
   * for output to a terminal */
  virtual std::ostream& toText(std::ostream& os, bool paren) const ;

  /** Write in XML */
  virtual XMLObject toXML() const ;

  /** */
  virtual RCP<ExprBase> getRcp() {return rcp(this);}


  /** Access to the operator */
  const UnaryFunctor* op() const {return op_.get();}


  /** Ordering operator for use in transforming exprs to standard form */
  virtual bool lessThan(const ScalarExpr* other) const ;

private:
      
  RCP<UnaryFunctor> op_;

};
}

#endif
