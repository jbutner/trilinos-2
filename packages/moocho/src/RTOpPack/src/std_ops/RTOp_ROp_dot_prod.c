/*
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
*/

#include "RTOp_ROp_dot_prod.h"
#include "RTOp_obj_null_vtbl.h"
#include "RTOp_obj_value_vtbl.h"
#include "RTOp_reduct_sum_value.h"

/* Implementation functions */

static int RTOp_ROp_dot_prod_apply_op(
  const struct RTOp_RTOp_vtbl_t* vtbl, const void* obj_data
  , const int num_vecs, const struct RTOp_SubVector vecs[]
  , const int num_targ_vecs, const struct RTOp_MutableSubVector targ_vecs[]
  , RTOp_ReductTarget targ_obj )
{
  /* */
  /* Declare local variables */
  /* */

  /* v0 */
  RTOp_index_type        v0_sub_dim;
  const RTOp_value_type  *v0_val;
  ptrdiff_t              v0_val_s;

  /* v1 */
  RTOp_index_type        v1_sub_dim;
  const RTOp_value_type  *v1_val;
  ptrdiff_t              v1_val_s;

  /* dot_prod */
  RTOp_value_type        dot_prod       = 0.0;

  /* Some temporary values */
  register RTOp_index_type k;
  RTOp_index_type sub_dim;

  /* */
  /* Validate the input */
  /* */
  if( num_vecs != 2 )
    return RTOp_ERR_INVALID_NUM_VECS;
  if( num_targ_vecs != 0 )
    return RTOp_ERR_INVALID_NUM_TARG_VECS;
  if( vecs[0].sub_dim != vecs[1].sub_dim )
    return RTOp_ERR_INCOMPATIBLE_VECS;

  /* */
  /* Get pointers to data */
  /* */

  /* v0 */
  v0_sub_dim     = vecs[0].sub_dim;
  v0_val         = vecs[0].values;
  v0_val_s       = vecs[0].values_stride;

  /* v1 */
  v1_sub_dim     = vecs[1].sub_dim;
  v1_val         = vecs[1].values;
  v1_val_s       = vecs[1].values_stride;

  sub_dim = v0_sub_dim;

  /* */
  /* Perform the dot product */
  /* */

  for( k = 0; k < sub_dim; ++k, v0_val += v0_val_s, v1_val += v1_val_s )
    dot_prod += (*v0_val) * (*v1_val);

  /* */
  /* Add this to the result */
  /* */
  *((RTOp_value_type*)targ_obj) += dot_prod;

  return 0; /* success? */
}

/* Virtual function table pointer */
const struct RTOp_RTOp_vtbl_t RTOp_ROp_dot_prod_vtbl =
{
  &RTOp_obj_null_vtbl   /* use null type for instance data */
  ,&RTOp_obj_value_vtbl /* use simple scalar type for target object */
  ,"ROp_dot_prod"
  ,NULL
  ,RTOp_ROp_dot_prod_apply_op
  ,RTOp_reduct_sum_value
  ,RTOp_get_reduct_sum_value_op
};

/* Class specific functions */

int RTOp_ROp_dot_prod_construct( struct RTOp_RTOp* op )
{
  op->obj_data = NULL;
  op->vtbl     = &RTOp_ROp_dot_prod_vtbl;
  return 0;
}

int RTOp_ROp_dot_prod_destroy( struct RTOp_RTOp* op )
{
  op->obj_data = NULL;
  op->vtbl     = NULL;
  return 0;
}

RTOp_value_type RTOp_ROp_dot_prod_val(RTOp_ReductTarget targ_obj)
{
  return *((RTOp_value_type*)targ_obj);
}
