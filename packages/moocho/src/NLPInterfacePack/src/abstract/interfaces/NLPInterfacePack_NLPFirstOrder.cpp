// @HEADER
// ***********************************************************************
// 
// Moocho: Multi-functional Object-Oriented arCHitecture for Optimization
//                  Copyright (2003) Sandia Corporation
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
// Questions? Contact Roscoe A. Bartlett (rabartl@sandia.gov) 
// 
// ***********************************************************************
// @HEADER

#include "NLPInterfacePack_NLPFirstOrder.hpp"
#include "AbstractLinAlgPack_MatrixOp.hpp"
#include "Teuchos_TestForException.hpp"

namespace {
  const char name_Gc[] = "Gc";
} // end namespace

namespace NLPInterfacePack {

// constructors

NLPFirstOrder::NLPFirstOrder()
  : Gc_(NULL)
{}

void NLPFirstOrder::initialize(bool test_setup) {
  num_Gc_evals_ = 0;
  NLPObjGrad::initialize(test_setup);
}

// BasisSystem

const NLPFirstOrder::basis_sys_ptr_t
NLPFirstOrder::basis_sys() const
{
  return Teuchos::null;
}

// <<std aggr>> members for Gc

void NLPFirstOrder::set_Gc(MatrixOp* Gc)
{
#ifdef TEUCHOS_DEBUG
  TEST_FOR_EXCEPTION( this->m() == 0, std::logic_error, "" );
#endif
  Gc_ = Gc;
}

MatrixOp* NLPFirstOrder::get_Gc()
{
#ifdef TEUCHOS_DEBUG
  TEST_FOR_EXCEPTION( this->m() == 0, std::logic_error, "" );
#endif
  return StandardCompositionRelationshipsPack::get_role_name(Gc_, false, name_Gc);
}

MatrixOp& NLPFirstOrder::Gc()
{
#ifdef TEUCHOS_DEBUG
  TEST_FOR_EXCEPTION( this->m() == 0, std::logic_error, "" );
#endif
  return StandardCompositionRelationshipsPack::role_name(Gc_, false, name_Gc);
}

const MatrixOp& NLPFirstOrder::Gc() const
{
#ifdef TEUCHOS_DEBUG
  TEST_FOR_EXCEPTION( this->m() == 0, std::logic_error, "" );
#endif
  return StandardCompositionRelationshipsPack::role_name(Gc_, false, name_Gc);
}

void NLPFirstOrder::unset_quantities()
{
  NLPObjGrad::unset_quantities();
  Gc_ = NULL;
}

// calculations

void NLPFirstOrder::calc_Gc(const Vector& x, bool newx) const
{
#ifdef TEUCHOS_DEBUG
  TEST_FOR_EXCEPTION( this->m() == 0, std::logic_error, "" );
#endif
  StandardCompositionRelationshipsPack::assert_role_name_set(Gc_, "NLP::calc_Gc()", name_Gc);
  imp_calc_Gc(x,newx,first_order_info());
  num_Gc_evals_++;
}

size_type NLPFirstOrder::num_Gc_evals() const
{
#ifdef TEUCHOS_DEBUG
  TEST_FOR_EXCEPTION( this->m() == 0, std::logic_error, "" );
#endif
  return num_Gc_evals_;
}

}	// end namespace NLPInterfacePack 
