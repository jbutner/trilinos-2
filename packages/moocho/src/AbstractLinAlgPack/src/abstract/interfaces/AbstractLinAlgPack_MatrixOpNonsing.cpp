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

#include <math.h>

#include "AbstractLinAlgPack_MatrixOpNonsing.hpp"
#include "AbstractLinAlgPack_VectorSpace.hpp"
#include "AbstractLinAlgPack_LinAlgOpPack.hpp"
#include "Teuchos_TestForException.hpp"

namespace AbstractLinAlgPack {

MatrixOpNonsing::mat_mwons_mut_ptr_t
MatrixOpNonsing::clone_mwons()
{
  return Teuchos::null;
}

MatrixOpNonsing::mat_mwons_ptr_t
MatrixOpNonsing::clone_mwons() const
{
  return Teuchos::null;
}

const MatrixOp::MatNorm
MatrixOpNonsing::calc_cond_num(
  EMatNormType  requested_norm_type
  ,bool         allow_replacement
  ) const
{
  using BLAS_Cpp::no_trans;
  using BLAS_Cpp::trans;
  using LinAlgOpPack::V_InvMtV;
  const VectorSpace
    &space_cols = this->space_cols(),
    &space_rows = this->space_rows();
  const index_type
    num_cols = space_rows.dim();
  TEST_FOR_EXCEPTION(
    !(requested_norm_type == MAT_NORM_1 || requested_norm_type == MAT_NORM_INF), MethodNotImplemented
    ,"MatrixOp::calc_norm(...): Error, This default implemenation can only "
    "compute the one norm or the infinity norm!"
    );
  //
  // Here we implement Algorithm 2.5 in "Applied Numerical Linear Algebra", Demmel (1997)
  // using the momenclature in the text.  This is applied to the inverse matrix.
  //
  const MatrixOpNonsing
    &B = *this;
  bool
    do_trans = requested_norm_type == MAT_NORM_INF;
  VectorSpace::vec_mut_ptr_t
    x    = (do_trans ? space_rows : space_cols).create_member(1.0/num_cols),
    w    = (do_trans ? space_cols : space_rows).create_member(),
    zeta = (do_trans ? space_cols : space_rows).create_member(),
    z    = (do_trans ? space_rows : space_cols).create_member();
  const index_type max_iter = 5;  // Recommended by Highman 1988, (see Demmel's reference)
  value_type w_nrm = 0.0;
  for( index_type k = 0; k <= max_iter; ++k ) {
    V_InvMtV( w.get(), B, !do_trans ? no_trans : trans, *x );     // w = B*x
    sign( *w, zeta.get() );                                       // zeta = sign(w)
    V_InvMtV( z.get(), B, !do_trans ? trans : no_trans, *zeta );  // z = B'*zeta
    value_type  z_j = 0.0;                                        // max |z(j)| = ||z||inf
    index_type  j   = 0;
    max_abs_ele( *z, &z_j, &j );
    const value_type zTx = dot(*z,*x);                            // z'*x
    w_nrm = w->norm_1();                                        // ||w||1
    if( ::fabs(z_j) <= zTx ) {                                    // Update
      break;
    }
    else {
      *x = 0.0;
      x->set_ele(j,1.0);
    }
  }
  const MatNorm M_nrm = this->calc_norm(requested_norm_type);
  return MatNorm( w_nrm * M_nrm.value ,requested_norm_type );
}

// Overridden from MatrixOp

MatrixOpNonsing::mat_mut_ptr_t
MatrixOpNonsing::clone()
{
  return clone_mwons();
}

MatrixOpNonsing::mat_ptr_t
MatrixOpNonsing::clone() const
{
  return clone_mwons();
}

// Overridden from MatrixNonsing

MatrixOpNonsing::mat_mns_mut_ptr_t
MatrixOpNonsing::clone_mns()
{
  return clone_mwons();
}

MatrixOpNonsing::mat_mns_ptr_t
MatrixOpNonsing::clone_mns() const
{
  return clone_mwons();
}

}	// end namespace AbstractLinAlgPack
