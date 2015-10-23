/*
 * Windrose.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: sandyarathidas
 */
#include <mpi.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <array>
using namespace std;

string dataFolderLocation = "/Users/sandyarathidas/Documents/CMPE275_Sandy/Project1/2001-2009CSVs/";

const int NUM_OF_SECTORS = 16;
const int SPEED_BUCKETS=5;

int calcSpeedBin(float windSpd) {
	if ( windSpd == 0)
		return 0;
	else if ( windSpd > 0 and windSpd <= 5)
		return 1;
	else if ( windSpd > 5 and windSpd <= 15)
		return 2;
	else if (windSpd > 15 and windSpd <= 25)
		return 3;
	else return 4;
}

int calcDirectBin(int windDir) {
	 //0-360 - cut into linear line 0-359
	return windDir / NUM_OF_SECTORS;
}


struct MesoData
{

	string stid;
	string stnName;
	string obsDate;
	double windSpd;
	int windDir;
};

vector<MesoData> readData(vector<string> folderListVector) {
 cout<<"ReadData:";
 vector<MesoData> inData;
  string file_name="../Data/07-01mesonet-20010701_2200.csv";
  ifstream  inputfile1(file_name);
  string line;
    while(getline(inputfile1,line))
    {
      string rowData[5];
      istringstream lineStream(line);
      string token;
      int i=0;
      while(getline(lineStream,token,',')){
    	rowData[i++]=token;
      }
       MesoData mesoData ={
        rowData[0].c_str(),
        rowData[1].c_str(),
        rowData[2].c_str(),
        atof(rowData[3].c_str()),
        atoi(rowData[4].c_str())
      };
    inData.push_back(mesoData);
    }

    return inData;

}

auto aggData(vector<MesoData> data){
  cout<<"Aggregate Data:"<<endl;
  auto wr = new int[NUM_OF_SECTORS][SPEED_BUCKETS]();
  for (MesoData m : data) {
    int s = calcSpeedBin(m.windSpd);
    int d = calcDirectBin(m.windDir);
    if (s< NUM_OF_SECTORS and d <SPEED_BUCKETS and d>0)
      wr[d][s]++;
  }
  return wr;
}




int main (int argc, char* argv[]){
	//Initialize MPI
	MPI_Init (&argc, &argv);
	vector<string> yearData={"2001",
	  "2002",
	  "2003",
	  "2004",
	  "2005",
	  "2006",
	  "2007",
	  "2008",
	  "2009"};
  string folderName;
  //  Get the number of processes.
  int no_of_processes = MPI::COMM_WORLD.Get_size ( );
  //Determine the current processes's rank.
  int rank_of_current_process = MPI::COMM_WORLD.Get_rank ( );
  int maxFiles = yearData.size()/no_of_processes;
  vector<string> processfolder;
  if(rank_of_current_process == 0){

	//vector<char> directories;
	  //char directories[maxFiles][200];
	  //char directories[maxFiles][200];
	  const char *directories[maxFiles];
	for(int i=0;i<yearData.size(); i=i+maxFiles){
		int j=0;
		while(j<maxFiles){
			//strcpy(directories[j],dataFolderLocation+yearData[i+j].c_str());
			cout<<"InnerWhile"<<dataFolderLocation+yearData[i+j]<<endl;
			directories[j]=(dataFolderLocation+yearData[i+j]).c_str();
			j++;
		}
		cout<<"lengthOfDirectories"<<sizeof(directories)<<endl;
		for(int i=0;i<maxFiles;i++){
			cout<<"Iteration"<<i<<endl;
			cout<<"directoriespath"<<directories[i]<<endl;
		}
		MPI::COMM_WORLD.Send(directories, maxFiles, MPI::CHAR, maxFiles, 0);

	}
  }
  else{
	const char *folderlist[maxFiles];
	MPI::COMM_WORLD.Recv(folderlist,maxFiles, MPI::CHAR, maxFiles,0);

	for(int i=0;i<maxFiles;i++){
		processfolder.push_back(string(folderlist[i]));
	}

  }

	  vector<MesoData> inputData= readData(processfolder);
	    auto resultArray = aggData(inputData);
	    auto finalArray= new int[NUM_OF_SECTORS][SPEED_BUCKETS]();
	    MPI::COMM_WORLD.Reduce(resultArray, finalArray,NUM_OF_SECTORS* SPEED_BUCKETS, MPI::INT,MPI::SUM, 0);


	   if(rank_of_current_process == 0){
	      cout<<endl;
	      cout<<"...................................................."<<endl;
	      cout<<"                  Final 2D Array                    "<<endl;
	      cout<<"...................................................."<<endl;

	      for (int i = 0; i < NUM_OF_SECTORS; i++){
	        for (int j = 0; j < SPEED_BUCKETS; j++){
	            cout << finalArray[i][j] <<"\t";
	        }
	        cout <<endl;
	      }
	    }




  MPI::Finalize();
  return 0;

}


