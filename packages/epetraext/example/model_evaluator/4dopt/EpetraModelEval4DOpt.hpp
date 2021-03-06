#ifndef EPETRA_MODEL_EVAL_4D_OPT_HPP
#define EPETRA_MODEL_EVAL_4D_OPT_HPP

#include "EpetraExt_ModelEvaluator.h"
#include "Epetra_Map.h"
#include "Epetra_Vector.h"
#include "Epetra_Comm.h"
#include "Epetra_CrsGraph.h"

/** \brief A simple serial example model which includes a parameter subvector
 * and a response function that can be used to define an optimization problem.
 *
 * Represents the model:
 
 \verbatim

    f[0] =        x[0]      + x[1]*x[1] - p[0];
    f[1] = d_ * ( x[0]*x[0] - x[1]      - p[1] );

    g[0] = 0.5 * ( sqr(x[0]-xt0_) + sqr(x[1]-xt1_) + sqr(p[0]-pt0_) + sqr(p[1]-pt1_) );
 
 \endverbatim
 *
 * where there is just one state vector <tt>x = [ x[0], x[1] ]</tt> and one
 * parameter subvector <tt>p = [ p[0], p[1] ]</tt>.
 *
 * See the function <tt>evalModel()</tt> for more details.
 */
class EpetraModelEval4DOpt : public EpetraExt::ModelEvaluator {
public:

  /** \brief . */
  EpetraModelEval4DOpt(
		const double xt0 = 1.0
		,const double xt1 = 1.0
		,const double pt0 = 2.0
		,const double pt1 = 0.0
		,const double d = 10.0
		,const double x00 = 1.0
		,const double x01 = 1.0
		,const double p00 = 2.0
		,const double p01 = 0.0
    );

  /** \brief . */
  void setSupportDerivs( bool supportDerivs );

  /** \brief . */
  void set_p_bounds( double pL0, double pL1, double pU0, double pU1 );

  /** \brief . */
  void set_x_bounds( double xL0, double xL1, double xU0, double xU1 );

  /** \name Overridden from EpetraExt::ModelEvaluator . */
  //@{

  /** \brief . */
  Teuchos::RefCountPtr<const Epetra_Map> get_x_map() const;
  /** \brief . */
  Teuchos::RefCountPtr<const Epetra_Map> get_f_map() const;
  /** \breif . */
  Teuchos::RefCountPtr<const Epetra_Map> get_p_map(int l) const;
  /** \breif . */
  Teuchos::RefCountPtr<const Epetra_Map> get_g_map(int j) const;
  /** \brief . */
  Teuchos::RefCountPtr<const Epetra_Vector> get_x_init() const;
  /** \brief . */
  Teuchos::RefCountPtr<const Epetra_Vector> get_p_init(int l) const;
  /** \brief . */
  Teuchos::RefCountPtr<const Epetra_Vector> get_x_lower_bounds() const;
  /** \brief . */
  Teuchos::RefCountPtr<const Epetra_Vector> get_x_upper_bounds() const;
  /** \brief . */
  Teuchos::RefCountPtr<const Epetra_Vector> get_p_lower_bounds(int l) const;
  /** \brief . */
  Teuchos::RefCountPtr<const Epetra_Vector> get_p_upper_bounds(int l) const;
  /** \brief . */
  Teuchos::RefCountPtr<Epetra_Operator> create_W() const;
  /** \brief . */
  InArgs createInArgs() const;
  /** \brief . */
  OutArgs createOutArgs() const;
  /** \brief . */
  void evalModel( const InArgs& inArgs, const OutArgs& outArgs ) const;

  //@}

private:

  // /////////////////////////////////////
  // Private member data

	double xt0_;
	double xt1_;
	double pt0_;
	double pt1_;
  double d_;

	bool isInitialized_;
  bool supportDerivs_;

  Teuchos::RefCountPtr<const Epetra_Comm> epetra_comm_;
	Teuchos::RefCountPtr<const Epetra_Map> map_x_;
	Teuchos::RefCountPtr<const Epetra_Map> map_p_;
	Teuchos::RefCountPtr<const Epetra_Map> map_g_;

	Teuchos::RefCountPtr<Epetra_Vector> xL_;
	Teuchos::RefCountPtr<Epetra_Vector> xU_;
	Teuchos::RefCountPtr<Epetra_Vector> pL_;
	Teuchos::RefCountPtr<Epetra_Vector> pU_;
	Teuchos::RefCountPtr<Epetra_Vector> gL_;
	Teuchos::RefCountPtr<Epetra_Vector> gU_;
	Teuchos::RefCountPtr<Epetra_Vector> x0_;
	Teuchos::RefCountPtr<Epetra_Vector> p0_;

  Teuchos::RefCountPtr<Epetra_CrsGraph> W_graph_;

  // Note defined and not to be called
  EpetraModelEval4DOpt();

};

#endif // EPETRA_MODEL_EVAL_4D_OPT_HPP
