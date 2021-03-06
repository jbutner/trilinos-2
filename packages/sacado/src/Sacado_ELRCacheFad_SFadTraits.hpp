// $Id$ 
// $Source$ 
// @HEADER
// ***********************************************************************
// 
//                           Sacado Package
//                 Copyright (2006) Sandia Corporation
// 
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
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
// Questions? Contact David M. Gay (dmgay@sandia.gov) or Eric T. Phipps
// (etphipp@sandia.gov).
// 
// ***********************************************************************
//
// The forward-mode AD classes in Sacado are a derivative work of the
// expression template classes in the Fad package by Nicolas Di Cesare.  
// The following banner is included in the original Fad source code:
//
// ************ DO NOT REMOVE THIS BANNER ****************
//
//  Nicolas Di Cesare <Nicolas.Dicesare@ann.jussieu.fr>
//  http://www.ann.jussieu.fr/~dicesare
//
//            CEMRACS 98 : C++ courses, 
//         templates : new C++ techniques 
//            for scientific computing 
// 
//********************************************************
//
//  NumericalTraits class to illustrate TRAITS
//
//********************************************************
// @HEADER

#ifndef SACADO_ELRCACHEFAD_SFADTRAITS_HPP
#define SACADO_ELRCACHEFAD_SFADTRAITS_HPP

#include "Sacado_Traits.hpp"
#include <sstream>

// Forward declarations
namespace Sacado {
  namespace ELRCacheFad {
    template <typename T, int Num> class SFad;
  }
}

namespace Sacado {

  //! Specialization of %Promote to SFad types
  template <typename ValueT, int Num>
  struct Promote< ELRCacheFad::SFad<ValueT,Num>, 
		  ELRCacheFad::SFad<ValueT,Num> > {
    typedef ELRCacheFad::SFad<ValueT,Num> type;
  };

  //! Specialization of %Promote to SFad types
  template <typename ValueT, int Num, typename R>
  struct Promote< ELRCacheFad::SFad<ValueT,Num>, R > {
    typedef typename ValueType< ELRCacheFad::SFad<ValueT,Num> >::type value_type_l;
    typedef typename ValueType<R>::type value_type_r;
    typedef typename Promote<value_type_l,value_type_r>::type value_type;

    typedef ELRCacheFad::SFad<value_type,Num> type;
  };

  //! Specialization of %Promote to SFad types
  template <typename L, typename ValueT, int Num>
  struct Promote< L, ELRCacheFad::SFad<ValueT, Num> > {
  public:

    typedef typename ValueType<L>::type value_type_l;
    typedef typename ValueType< ELRCacheFad::SFad<ValueT,Num> >::type value_type_r;
    typedef typename Promote<value_type_l,value_type_r>::type value_type;

    typedef ELRCacheFad::SFad<value_type,Num> type;
  };

  //! Specialization of %ScalarType to SFad types
  template <typename ValueT, int Num>
  struct ScalarType< ELRCacheFad::SFad<ValueT,Num> > {
    typedef typename ELRCacheFad::SFad<ValueT,Num>::ScalarT type;
  };

  //! Specialization of %ValueType to SFad types
  template <typename ValueT, int Num>
  struct ValueType< ELRCacheFad::SFad<ValueT,Num> > {
    typedef ValueT type;
  };

  //! Specialization of %IsADType to SFad types
  template <typename ValueT, int Num>
  struct IsADType< ELRCacheFad::SFad<ValueT,Num> > {
    static const bool value = true;
  };

  //! Specialization of %IsADType to SFad types
  template <typename ValueT, int Num>
  struct IsScalarType< ELRCacheFad::SFad<ValueT,Num> > {
    static const bool value = false;
  };

  //! Specialization of %Value to SFad types
  template <typename ValueT, int Num>
  struct Value< ELRCacheFad::SFad<ValueT,Num> > {
    typedef typename ValueType< ELRCacheFad::SFad<ValueT,Num> >::type value_type;
    static const value_type& eval(const ELRCacheFad::SFad<ValueT,Num>& x) { 
      return x.val(); }
  };

  //! Specialization of %ScalarValue to SFad types
  template <typename ValueT, int Num>
  struct ScalarValue< ELRCacheFad::SFad<ValueT,Num> > {
    typedef typename ValueType< ELRCacheFad::SFad<ValueT,Num> >::type value_type;
    typedef typename ScalarType< ELRCacheFad::SFad<ValueT,Num> >::type scalar_type;
    static const scalar_type& eval(const ELRCacheFad::SFad<ValueT,Num>& x) { 
      return ScalarValue<value_type>::eval(x.val()); }
  };

  //! Specialization of %StringName to SFad types
  template <typename ValueT, int Num>
  struct StringName< ELRCacheFad::SFad<ValueT,Num> > {
    static std::string eval() { 
      std::stringstream ss;
      ss << "Sacado::ELRCacheFad::SFad< " 
	 << StringName<ValueT>::eval() << ", " << Num << " >";
      return ss.str(); 
    }
  };

} // namespace Sacado

// Define Teuchos traits classes
#ifdef HAVE_SACADO_TEUCHOS
#include "Teuchos_PromotionTraits.hpp"
#include "Teuchos_ScalarTraits.hpp"
#include "Sacado_Fad_ScalarTraitsImp.hpp"

namespace Teuchos {

  //! Specialization of %Teuchos::PromotionTraits to SFad types
  template <typename ValueT, int Num>
  struct PromotionTraits< Sacado::ELRCacheFad::SFad<ValueT,Num>, 
			  Sacado::ELRCacheFad::SFad<ValueT,Num> > {
    typedef typename Sacado::Promote< Sacado::ELRCacheFad::SFad<ValueT,Num>,
				      Sacado::ELRCacheFad::SFad<ValueT,Num> >::type
    promote;
  };

  //! Specialization of %Teuchos::PromotionTraits to SFad types
  template <typename ValueT, int Num, typename R>
  struct PromotionTraits< Sacado::ELRCacheFad::SFad<ValueT,Num>, R > {
    typedef typename Sacado::Promote< Sacado::ELRCacheFad::SFad<ValueT,Num>,
				      R >::type 
    promote;
  };

  //! Specialization of %Teuchos::PromotionTraits to SFad types
  template <typename L, typename ValueT, int Num>
  struct PromotionTraits< L, Sacado::ELRCacheFad::SFad<ValueT,Num> > {
  public:
    typedef typename Sacado::Promote< L, 
				      Sacado::ELRCacheFad::SFad<ValueT,Num> >::type 
    promote;
  };

  //! Specializtion of Teuchos::ScalarTraits
  template <typename ValueT, int Num>
  struct ScalarTraits< Sacado::ELRCacheFad::SFad<ValueT,Num> > :
    public Sacado::Fad::ScalarTraitsImp< Sacado::ELRCacheFad::SFad<ValueT,Num> >
  {};
}
#endif // HAVE_SACADO_TEUCHOS

#endif // SACADO_ELRCACHEFAD_SFADTRAITS_HPP
