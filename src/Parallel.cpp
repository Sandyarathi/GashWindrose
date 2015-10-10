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

struct MesoData
{
  int year;
  int month;
  int day;
  string stid;
  int windDir;
  double windSpd;
};

int main (int argc, char* argv[]);
vector<MesoData> readData(String file_name);
void aggData(vector<MesoData> data);



int main( int argc, char *argv[]){
  String file_name = "../Data/ACME_2011.csv";
  vector<MesoData> inputData= readData(file_name);
  aggData(inputData);

}

vector<MesoData> readData(String file_name) {
  vector<MesoData> inData;
  ifstream  inputfile1(file_name);
  inputfile1.ignore(100, '\n');//To ignore column header line
    string line;
    while(getline(inputfile1,line))
    {
      string rowData[6];
      istringstream lineStream(line);
      string token;
      int i=0;
      while(getline(lineStream,token,',')){
        rowData[i++]=token;
      }
       MesoData mesoData ={
        atoi(rowData[0].c_str()),
        atoi(rowData[1].c_str()),
        atoi(rowData[2].c_str()),
        rowData[3],
        atoi(rowData[4].c_str()),
        atof(rowData[5].c_str())
      };
    inData.push_back(mesoData);
    }
    return inData;
}

void aggData(vector<MesoData> data){
  int wr[NUM_OF_SECTORS][SPEED_BUCKETS]={0};
  for (MesoData m : data) {
    int s = calcSpeedBin(m.windSpd);
    int d = m.windDir;
    if (s< NUM_OF_SECTORS and d <SPEED_BUCKETS and d>0)
      wr[d][s]++;
  }
  cout<<"         Final 2DArray for plotting"<<endl;
  cout<<"=============================================="<<endl;
  cout<<endl;
  //printing the 2D Array
  for (int i = 0; i < NUM_OF_SECTORS; i++){
    for (int j = 0; j < SPEED_BUCKETS; j++){
        cout << wr[i][j] <<"\t";
    }
    cout <<endl;
  }
}





 












