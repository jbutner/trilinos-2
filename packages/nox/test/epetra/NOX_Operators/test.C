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
                                                                                
// NOX headers
#include "NOX.H"  // Required headers
#include "NOX_Epetra.H" // Epetra Interface headers
#include "NOX_TestCompare.H" // Test Suite headers
#include "NOX_Epetra_DebugTools.H"

// Trilinos headers
#ifdef HAVE_MPI
#include "Epetra_MpiComm.h"
#else
#include "Epetra_SerialComm.h"
#endif
#include "Epetra_Map.h"
#include "Epetra_Vector.h"
#include "Epetra_RowMatrix.h"
#include "Epetra_CrsMatrix.h"
#include "Epetra_Map.h"
#include "Epetra_LinearProblem.h"
#include "AztecOO.h"

#include "Laplace2D.H"


int main(int argc, char *argv[]) 
{
  // Initialize MPI
#ifdef HAVE_MPI
  MPI_Init(&argc,&argv);
#endif

  // Create a communicator for Epetra objects
#ifdef HAVE_MPI
  Epetra_MpiComm Comm( MPI_COMM_WORLD );
#else
  Epetra_SerialComm Comm;
#endif
 
  bool verbose = false;

  if (argc > 1)
    if (argv[1][0]=='-' && argv[1][1]=='v')
      verbose = true;

  // Get the process ID and the total number of processors
  int MyPID = Comm.MyPID();
#ifdef HAVE_MPI
  int NumProc = Comm.NumProc();
#endif

  // define the parameters of the nonlinear PDE problem
  int nx = 5;
  int ny = 6;  
  double lambda = 1.0;

  PDEProblem Problem(nx,ny,lambda,&Comm);
 
  // starting solution, here a zero vector
  Epetra_Vector InitialGuess(Problem.GetMatrix()->Map());
  InitialGuess.PutScalar(0.0);

  // random vector upon which to apply each operator being tested
  Epetra_Vector directionVec(Problem.GetMatrix()->Map());
  directionVec.Random();

  // Set up the problem interface
  Teuchos::RCP<SimpleProblemInterface> interface = 
    Teuchos::rcp(new SimpleProblemInterface(&Problem) );
  
  // Set up theolver options parameter list
  Teuchos::RCP<Teuchos::ParameterList> noxParamsPtr = Teuchos::rcp(new Teuchos::ParameterList);
  Teuchos::ParameterList & noxParams = *(noxParamsPtr.get());

  // Set the nonlinear solver method
  noxParams.set("Nonlinear Solver", "Line Search Based");

  // Set up the printing utilities
  // Only print output if the "-v" flag is set on the command line
  Teuchos::ParameterList& printParams = noxParams.sublist("Printing");
  printParams.set("MyPID", MyPID); 
  printParams.set("Output Precision", 5);
  printParams.set("Output Processor", 0);
  if( verbose )
    printParams.set("Output Information", 
		NOX::Utils::OuterIteration + 
		NOX::Utils::OuterIterationStatusTest + 
		NOX::Utils::InnerIteration +
		NOX::Utils::Parameters + 
		NOX::Utils::Details + 
		NOX::Utils::Warning +
		NOX::Utils::TestDetails);
  else
    printParams.set("Output Information", NOX::Utils::Error +
		NOX::Utils::TestDetails);

  NOX::Utils printing(printParams);


  // Identify the test problem
  if (printing.isPrintType(NOX::Utils::TestDetails))
    printing.out() << "Starting epetra/NOX_Operators/NOX_Operators.exe" << endl;

  // Identify processor information
#ifdef HAVE_MPI
  if (printing.isPrintType(NOX::Utils::TestDetails)) {
    printing.out() << "Parallel Run" << endl;
    printing.out() << "Number of processors = " << NumProc << endl;
    printing.out() << "Print Process = " << MyPID << endl;
  }
  Comm.Barrier();
  if (printing.isPrintType(NOX::Utils::TestDetails))
    printing.out() << "Process " << MyPID << " is alive!" << endl;
  Comm.Barrier();
#else
  if (printing.isPrintType(NOX::Utils::TestDetails))
    printing.out() << "Serial Run" << endl;
#endif

  int status = 0;

  Teuchos::RCP<NOX::Epetra::Interface::Required> iReq = interface;

  // Need a NOX::Epetra::Vector for constructor
  NOX::Epetra::Vector noxInitGuess(InitialGuess, NOX::DeepCopy);

  // Analytic matrix
  Teuchos::RCP<Epetra_CrsMatrix> A = Teuchos::rcp( Problem.GetMatrix(), false );

  Epetra_Vector A_resultVec(Problem.GetMatrix()->Map());
  interface->computeJacobian( InitialGuess, *A );
  A->Apply( directionVec, A_resultVec );

  // FD operator
  Teuchos::RCP<Epetra_CrsGraph> graph = Teuchos::rcp( const_cast<Epetra_CrsGraph*>(&A->Graph()), false );
  Teuchos::RCP<NOX::Epetra::FiniteDifference> FD = Teuchos::rcp(
    new NOX::Epetra::FiniteDifference(printParams, iReq, noxInitGuess, graph) );
  
  Epetra_Vector FD_resultVec(Problem.GetMatrix()->Map());
  FD->computeJacobian(InitialGuess, *FD);
  FD->Apply( directionVec, FD_resultVec );

  // Matrix-Free operator
  Teuchos::RCP<NOX::Epetra::MatrixFree> MF = Teuchos::rcp(
    new NOX::Epetra::MatrixFree(printParams, iReq, noxInitGuess) );

  Epetra_Vector MF_resultVec(Problem.GetMatrix()->Map());
  MF->computeJacobian(InitialGuess, *MF);
  MF->Apply( directionVec, MF_resultVec );

  // Need NOX::Epetra::Vectors for tests
  NOX::Epetra::Vector noxAvec ( A_resultVec , NOX::DeepCopy );
  NOX::Epetra::Vector noxFDvec( FD_resultVec, NOX::DeepCopy );
  NOX::Epetra::Vector noxMFvec( MF_resultVec, NOX::DeepCopy );

  // Create a TestCompare class
  NOX::Epetra::TestCompare tester( printing.out(), printing);
  double abstol = 1.e-4;
  double reltol = 1.e-4 ;
  //NOX::TestCompare::CompareType aComp = NOX::TestCompare::Absolute;

  status += tester.testVector( noxFDvec, noxAvec, reltol, abstol,
                              "Finite-Difference Operator Apply Test" );
  status += tester.testVector( noxMFvec, noxAvec, reltol, abstol,
                              "Matrix-Free Operator Apply Test" );

  // Summarize test results  
  if( status == 0 )
    printing.out() << "Test passed!" << endl;
  else 
    printing.out() << "Test failed!" << endl;

#ifdef HAVE_MPI
  MPI_Finalize();
#endif

  // Final return value (0 = successfull, non-zero = failure)
  return status;

}

/*
  end of file test.C
*/
