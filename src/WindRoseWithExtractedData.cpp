/*
 * WindRoseWithExtractedData.cpp
 *
 *  Created on: Oct 19, 2015
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



struct MesoData
{

	//int year;
	//int month;
	//int day;
	string stid;
	double windSpd;
	int windDir;
};

vector<MesoData> readData(string folder_name) {
 cout<<"ReadData:"+folder_name;
  vector<MesoData> inData;
  string file_name="../Data/ACME_2011.csv";
  ifstream  inputfile1(file_name);
  inputfile1.ignore(100, '\n');//To ignore column header line
    string line;
    while(getline(inputfile1,line))
    {
      bool setflag =false;
      string rowData[3];
      istringstream lineStream(line);
      string token;
      int i=0;
      while(getline(lineStream,token,',')){
    	/*if(!rowData[0] == "AP005" ){
    		setflag=true;
    		break;
    	}*/
    	rowData[i++]=token;
      }
     /* if(setflag)
    	  continue;*/
       MesoData mesoData ={
        //atoi(rowData[0].c_str()),
        //atoi(rowData[1].c_str()),
        //atoi(rowData[2].c_str()),
        rowData[0],
        atoi(rowData[1].c_str()),
        atof(rowData[2].c_str())
      };
   // if(mesoData.stid == "AP005")
    	inData.push_back(mesoData);
    }
    return inData;
}

auto aggData(vector<MesoData> data){
  auto wr = new int[NUM_OF_SECTORS][SPEED_BUCKETS]();
  //int** wr= 0;
  //wr=new int*[NUM_OF_SECTORS];
  for (MesoData m : data) {
    int s = calcSpeedBin(m.windSpd);
    int d = m.windDir;
    if (s< NUM_OF_SECTORS and d <SPEED_BUCKETS and d>0)
      wr[d][s]++;
  }
  /*cout<<"          2DArray for plotting                "<<endl;
  cout<<"=============================================="<<endl;
  cout<<endl;
  //printing the 2D Array
  for (int i = 0; i < NUM_OF_SECTORS; i++){
    for (int j = 0; j < SPEED_BUCKETS; j++){
        cout << wr[i][j] <<"\t";
    }
    cout <<endl;
  }*/
  return wr;
}

int main (int argc, char* argv[]){
	//Initialize MPI
	MPI::Init ( argc, argv );
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
	char directories[maxFiles][200];
	for(int i=0;i<yearData.size(); i=i+maxFiles){
		int j=0;
		while(j<maxFiles){
			directories[j]=(dataFolderLocation+yearData[i+j]).c_str();
			j++;
		}
		MPI::COMM_WORLD.Send(directories, maxFiles, MPI::CHAR, maxFiles, 0);

	}
  }
  else{
	char folderlist[maxFiles][150];
	MPI::COMM_WORLD.Recv(folderlist,maxFiles, MPI::CHAR, maxFiles,0);

	for(int i=0;i<maxFiles;i++){
		processfolder.push_back(string(folderlist[i]));
	}

  }
  for(int i=0;i<maxFiles;i++){
	  vector<MesoData> inputData= readData(processfolder[i]);
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
  }


  MPI::Finalize();
  return 0;

}




