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

#ifndef SLAP_MULTI_VECTOR_MUTABLE_COlS_H
#define SLAP_MULTI_VECTOR_MUTABLE_COlS_H

#include <vector>

#include "AbstractLinAlgPack_Types.hpp"
#include "AbstractLinAlgPack_MultiVectorMutable.hpp"
#include "AbstractLinAlgPack_VectorSpace.hpp"
#include "AbstractLinAlgPack_VectorMutable.hpp"

namespace AbstractLinAlgPack {

/** \brief Default subclass for <tt>MultiVectorMutable</tt> implemented using columns
 * of separate abstract vectors.
 *
 * Only column access is permitted.  This is a very bad implementation of a
 * multi-vector but this will work in situations where you need a multi-vector
 * but some underlying linear algebra library does not directly support them.
 *
 * The only difficult issue is what to use for the vector space for the rows
 * of the multi-vector <tt>space_row</tt>.  This has to be carefully chosen
 * so that all will work well.
 *
 * 
 * ToDo: Finish documentation!
 */
class MultiVectorMutableCols
  : virtual public AbstractLinAlgPack::MultiVectorMutable
{
public:

  /** @name Constructors/Initializers */
  //@{

  /// Construct to initialized.
  MultiVectorMutableCols();

  /// Calls <tt>initalize()</tt>.
  MultiVectorMutableCols(
    const  Teuchos::RCP<const VectorSpace>   &space_cols
    ,const  Teuchos::RCP<const VectorSpace>  &space_rows
    ,Teuchos::RCP<VectorMutable>       col_vecs[] = NULL
    );
  
  /** \brief Initialize given the spaces for the columns and rows and possibly the column vectors.
   *
   * @param  space_cols  [in] The space that the columns must lie in.  The underlying
   *                     vector space must not be changed while \c this is in use.
   * @param  space_rows  [in] The space that the rows must lie in.  The underlying
   *                     vector space must not be changed while \c this is in use.
   *                     What this argument really specifies is what vector type
   *                     will be compatible with the vectors that the client may
   *                     try to use to interact with the rows of this multivector.
   * @param  col_vecs    [in] Array (size <tt>space_rows->dim()</tt>) of column
   *                     vectors to use for the columns of <tt>this</tt>.
   *                     It is allowed for <tt>col_vecs==NULL</tt> in which case
   *                     <tt>space_cols->create_member()</tt> will be used to
   *                     create the colmns of <tt>this</tt>.
   *
   * Preconditions:<ul>
   * <li> <tt>space_cols.get() != NULL</tt> (throw <tt>std::invalid_argument</tt>)
   * <li> <tt>space_rows.get() != NULL</tt> (throw <tt>std::invalid_argument</tt>)
   * <li> <tt>space_cols->dim() > 0</tt> (throw <tt>std::invalid_argument</tt>)
   * <li> <tt>space_rows->dim() > 0</tt> (throw <tt>std::invalid_argument</tt>)
   * <li> [<tt>col_vecs != NULL</tt>]
   *      <tt>col_vecs[j-1].get() != NULL && col_vecs[j-1]->space().is_compatible(*space_cols) == true</tt>,
   *      for <tt>j=1..space_rows->dim()</tt>
   * </ul>
   *
   * Postconditions:<ul>
   * <tt> <tt>&this->space_cols() == space_cols.get()</tt>
   * <tt> <tt>&this->space_rows() == space_rows.get()</tt>
   * <li> [<tt>col_vecs != NULL</tt>] <tt>this->col(j).get() == col_vecs[j-1].get()</tt>,
   *      for <tt>j=1..space_rows->dim()</tt>
   * </ul>
   */
  void initialize(
    const  Teuchos::RCP<const VectorSpace>   &space_cols
    ,const  Teuchos::RCP<const VectorSpace>  &space_rows
    ,Teuchos::RCP<VectorMutable>       col_vecs[] = NULL
    );

  /// Set uninitalized.
  void set_uninitialized();

  //@}

  /** @name Overridden from MatrixBase */
  //@{

  /** \brief . */
  size_type rows() const;
  /** \brief . */
  size_type cols() const;

  //@}
  
  /** @name Overridden from MatrixOp */
  //@{

  /** \brief . */
  const VectorSpace& space_cols() const;
  /** \brief . */
  const VectorSpace& space_rows() const;
  /** \brief . */
  void zero_out();
  /** \brief . */
  void Mt_S( value_type alpha );
  /** \brief . */
  MatrixOp& operator=(const MatrixOp& mwo_rhs);
  /** \brief . */
  mat_mut_ptr_t clone();
  /** \brief . */
  mat_ptr_t clone() const;
  /** \brief . */
  void Vp_StMtV(
    VectorMutable* v_lhs, value_type alpha, BLAS_Cpp::Transp trans_rhs1
    , const Vector& v_rhs2, value_type beta) const;
  /** \brief . */
  void Vp_StMtV(
    VectorMutable* v_lhs, value_type alpha, BLAS_Cpp::Transp trans_rhs1
    , const SpVectorSlice& sv_rhs2, value_type beta) const;
  /** \brief . */
  bool syrk(
     BLAS_Cpp::Transp M_trans, value_type alpha
    , value_type beta, MatrixSymOp* sym_lhs ) const;

  /** @name Overridden from MultiVector */
  //@{
  /// Returns <tt>return & COL_ACCESS == true</tt> only.
  access_by_t access_by() const;
  //@}

  /** @name Overridden from MultiVectorMutable */
  //@{
  /** \brief . */
  vec_mut_ptr_t col(index_type j);
  /// Returns <tt>return.get() == NULL</tt>
  vec_mut_ptr_t row(index_type i);
  /// Returns <tt>return.get() == NULL</tt>
  vec_mut_ptr_t diag(int k);
  /** \brief . */
  multi_vec_mut_ptr_t mv_sub_view(const Range1D& row_rng, const Range1D& col_rng);
  //@}

private:
  
#ifdef DOXYGEN_COMPILE
  const VectorSpace                  *space_cols;
  const VectorSpace                  *space_rows;
  VectorMutable                *column_vectors;
#else
  Teuchos::RCP<const VectorSpace>                  space_cols_;
  Teuchos::RCP<const VectorSpace>                  space_rows_;
  std::vector< Teuchos::RCP<VectorMutable> > col_vecs_;
#endif
  
}; // end class MultiVectorMutableCols

} // end namespace AbstractLinAlgPack

#endif // SLAP_MULTI_VECTOR_MUTABLE_COlS_H
