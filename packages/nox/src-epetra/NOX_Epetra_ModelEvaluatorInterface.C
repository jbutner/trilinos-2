// $Id$ 
// $Source$ 

//@HEADER
// ************************************************************************
// 
//            NOX: An Object-Oriented Nonlinear Solver Package
//                 Copyright (2002) Sandia Corporation
// 
//            LOCA: Library of Continuation Algorithms Package
//                 Copyright (2005) Sandia Corporation
// 
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
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
// 
// Questions? Contact Roger Pawlowski (rppawlo@sandia.gov) or 
// Eric Phipps (etphipp@sandia.gov), Sandia National Laboratories.
// ************************************************************************
//  CVS Information
//  $Source$
//  $Author$
//  $Date$
//  $Revision$
// ************************************************************************
//@HEADER

#include "NOX_Epetra_ModelEvaluatorInterface.H"

#ifdef HAVE_NOX_EPETRAEXT

#include "EpetraExt_ModelEvaluator.h"
#include "Epetra_Vector.h"
#include "Epetra_Operator.h"

// *****************************************************************
// *****************************************************************
NOX::Epetra::ModelEvaluatorInterface::
ModelEvaluatorInterface(const 
			Teuchos::RCP<EpetraExt::ModelEvaluator>& m) :
  model_(m)
{
  inargs_ = model_->createInArgs();
}

// *****************************************************************
// *****************************************************************
NOX::Epetra::ModelEvaluatorInterface::~ModelEvaluatorInterface()
{
}

// *****************************************************************
// *****************************************************************
bool NOX::Epetra::ModelEvaluatorInterface::
computeF(const Epetra_Vector& x, Epetra_Vector& F, const FillType fillFlag)
{
  EpetraExt::ModelEvaluator::OutArgs outargs = model_->createOutArgs();

  inargs_.set_x( Teuchos::rcp(&x, false) );
 
  Teuchos::RCP<Epetra_Vector> f = Teuchos::rcp(&F, false);

  if (fillFlag == NOX::Epetra::Interface::Required::Residual)
    eval_f_.reset(f, EpetraExt::ModelEvaluator::EVAL_TYPE_EXACT); 
  else if (fillFlag == NOX::Epetra::Interface::Required::Jac)
    eval_f_.reset(f, EpetraExt::ModelEvaluator::EVAL_TYPE_APPROX_DERIV);
  else
    eval_f_.reset(f, EpetraExt::ModelEvaluator::EVAL_TYPE_VERY_APPROX_DERIV);
     
  outargs.set_f(eval_f_);

  model_->evalModel(inargs_, outargs);

  inargs_.set_x(Teuchos::null);
 
  return true;
}
   
// *****************************************************************
// ***************************************************************** 
bool NOX::Epetra::ModelEvaluatorInterface::
computeJacobian(const Epetra_Vector& x, Epetra_Operator& Jaco)
{
  EpetraExt::ModelEvaluator::OutArgs outargs = model_->createOutArgs();

  inargs_.set_x( Teuchos::rcp(&x, false) );
  outargs.set_W( Teuchos::rcp(&Jaco, false) );

  model_->evalModel(inargs_, outargs);

  inargs_.set_x(Teuchos::null);

  return true;
}

// *****************************************************************
// *****************************************************************
bool NOX::Epetra::ModelEvaluatorInterface::
computePreconditioner(const Epetra_Vector& x, 
		      Epetra_Operator& M,
		      Teuchos::ParameterList* precParams)
{
  EpetraExt::ModelEvaluator::OutArgs outargs = model_->createOutArgs();

  inargs_.set_x( Teuchos::rcp(&x, false) );
  outargs.set_WPrec( Teuchos::rcp(&M, false) );

  model_->evalModel(inargs_, outargs);

  inargs_.set_x(Teuchos::null);

  return true;
}

// *****************************************************************
// *****************************************************************
bool NOX::Epetra::ModelEvaluatorInterface::
inargs_set_p(const Teuchos::RCP<const Epetra_Vector> p_, const int l)
{
   inargs_.set_p(l, p_);
   return true;
}

// *****************************************************************
// *****************************************************************
bool NOX::Epetra::ModelEvaluatorInterface::
set_inargs(const EpetraExt::ModelEvaluator::InArgs& inargs_in)
{
   inargs_ = inargs_in;
   return true;
}

// *****************************************************************
// *****************************************************************
#endif //HAVE_NOX_EPETRAEXT
