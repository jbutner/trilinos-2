INCLUDE(PackageAddExecutableAndTest)

PACKAGE_ADD_EXECUTABLE_AND_TEST(
  Epetra_ModalSolversTester
  SOURCES cxx_main.cpp
  COMM serial mpi
  ARGS "-v"
  STANDARD_PASS_OUTPUT
  )
