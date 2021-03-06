// @HEADER
// ***********************************************************************
// 
//    Thyra: Interfaces and Support for Abstract Numerical Algorithms
//                 Copyright (2004) Sandia Corporation
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
// Questions? Contact Michael A. Heroux (maherou@sandia.gov) 
// 
// ***********************************************************************
// @HEADER

#ifndef THYRA_PHYSICALLY_BLOCKED_LINEAR_OP_WITH_SOLVE_BASE_HPP
#define THYRA_PHYSICALLY_BLOCKED_LINEAR_OP_WITH_SOLVE_BASE_HPP

#include "Thyra_BlockedLinearOpWithSolveBase.hpp"
#include "Thyra_PhysicallyBlockedLinearOpBase.hpp"


namespace Thyra {


/** \brief Base interface for linear operators with a solve that are composed
 * out of individual LOB and LOWSB objects.
 *
 * \ingroup Thyra_Op_Solve_extended_interfaces_code_grp
 *
 * ToDo: Finish Documentation.
 */
template<class Scalar>
class PhysicallyBlockedLinearOpWithSolveBase
  : virtual public BlockedLinearOpWithSolveBase<Scalar>,
    virtual public PhysicallyBlockedLinearOpBase<Scalar>
{
public:

  /** \brief Determines if the block <tt>(i,j)</tt> can be filled with a LOWDB
   * object or not.
   *
   * \param  i  [in] Zero-based index for the block row.
   * \param  j  [in] Zero-based index for the block column.
   *
   * <b>Preconditions:</b><ul>
   * <li>this->blockFillIsActive()==true</tt>
   * <li><tt>i >= 0 && j >= 0</tt>
   * <li>[<tt>this->productRange().get()!=NULL</tt>]
   *       <tt>i < this->productRange()->numBlocks()</tt>
   * <li>[<tt>this->productDomain().get()!=NULL</tt>]
   *       <tt>j < this->productDomain()->numBlocks()</tt>
   * </ul>
   */
  virtual bool acceptsLOWSBlock(const int i, const int j) const = 0;

  /** \brief . */
  virtual void setNonconstLOWSBlock(
    const int i, const int j,
    const Teuchos::RCP<LinearOpWithSolveBase<Scalar> > &block
    ) = 0;
  
  /** \brief . */
  virtual void setLOWSBlock(
    const int i, const int j,
    const Teuchos::RCP<const LinearOpWithSolveBase<Scalar> > &block
    ) = 0;

private:
  
  // Not defined and not to be called
  PhysicallyBlockedLinearOpWithSolveBase<Scalar>&
  operator=(const PhysicallyBlockedLinearOpWithSolveBase<Scalar>&);

};


} // namespace Thyra


#endif // THYRA_PHYSICALLY_BLOCKED_LINEAR_OP_WITH_SOLVE_BASE_HPP
