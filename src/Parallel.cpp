/*
 * Parallel.cpp
 *
 *  Created on: Oct 7, 2015
 *      Author: sandyarathidas
 */
#include <mpi.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
int main (int argc, char* argv[]) {
  int rank, size;
  string line;
  MPI_Init (&argc, &argv);      
  MPI_Comm_size (MPI_COMM_WORLD, &size);        
  MPI_Comm_rank (MPI_COMM_WORLD, &rank); 
  cout << "Parallel.cc: Number of tasks="<<size<<" My rank=" << rank <<endl;      
  ifstream txtFile("../Data/ACME_2011.csv");
  txtFile.ignore(100, '\n');
  if (txtFile.is_open()) {
    while ( txtFile.good() ) {
      getline (txtFile,line);
      cout << line << endl;
    }
    txtFile.close();
  }else {
    cout << "Unable to open file";
  }
  MPI_Finalize(); /*end MPI*/
  return 0;
}













