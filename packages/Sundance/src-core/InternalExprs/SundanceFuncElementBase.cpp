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

#include "SundanceFuncElementBase.hpp"


#include "SundanceExpr.hpp"

using namespace Sundance;
using namespace Sundance;

using namespace Sundance;
using namespace Teuchos;

FuncElementBase::FuncElementBase(const std::string& rootName,
  const std::string& suffix, const FunctionIdentifier& fid)
	: ScalarExpr(), FunctionWithID(fid), 
    name_(rootName + suffix), rootName_(rootName),
    suffix_(suffix)
{}


std::ostream& FuncElementBase::toText(std::ostream& os, bool /* paren */) const 
{
	os << name_;
	return os;
}


bool FuncElementBase::lessThan(const ScalarExpr* other) const
{
  const FuncElementBase* f = dynamic_cast<const FuncElementBase*>(other);
  TEST_FOR_EXCEPTION(f==0, InternalError, "cast should never fail at this point");
  
  return (fid() < f->fid());
}

  
