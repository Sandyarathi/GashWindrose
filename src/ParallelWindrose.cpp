#include <mpi.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main (int argc, char* argv[]);
//vector<MesoData> readData(String file_name);
//void aggData(vector<MesoData> data);

int main (int argc, char* argv[]){

  
  //Initialize MPI
  MPI::Init ( argc, argv );
  string file_names[] ={"../Data/ACME_2011.csv", "../Data/ACME_2012.csv", "../Data/ACME_2013.csv"};
  string fileName;
  //  Get the number of processes.
  int no_of_processes = MPI::COMM_WORLD.Get_size ( );
  //Determine the current processes's rank.
  int rank_of_current_process = MPI::COMM_WORLD.Get_rank ( );
  if(rank_of_current_process == 0){
  	fileName = file_names[0];
  	for(int i=1;i<no_of_processes;i++){
  		MPI::COMM_WORLD.Send(file_names[i].c_str(), file_names[i].size(), MPI::CHAR, i, 0);
 	}
  }
  else{
  	char *file;
  	file = new char[100];
  	MPI::COMM_WORLD.Recv(file,100, MPI::CHAR, 0,0);
  	fileName= string(file);
  }
  cout << "My rank=" << rank_of_current_process << "\t FileName"<<fileName<<endl;
  MPI::Finalize();
  return 0;
}