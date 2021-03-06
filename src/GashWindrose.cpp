//============================================================================
// Name        : GashWindrose.cpp
// Author      : Sandyarathi
// Version     :
// Copyright   : Your copyright notice
// Description : Windrose code
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
using namespace std;


int NUM_OF_SECTORS = 16;
int SPEED_BUCKETS=5;

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
//The csv has given the direction data in sectors
//int calcDirectBin(float windDir) {
	// 0-360 - cut into linear line 0-359
	//return windDir % NUM_OF_SECTORS;
//}


struct MesoData
{
	int year;
	int month;
	int day;
	string stid;
	int windDir;
	double windSpd;
};

vector<MesoData> readData() {
	vector<MesoData> inData;

	ifstream  inputfile("../InputData/Wind-Rose.csv");
    string line;
    while(getline(inputfile,line))
    {
    	string rowData[6];
        istringstream lineStream(line);
        string token;
        int i=0;
        while(getline(lineStream,token,',')){
        	rowData[i++]=token;
        }
       MesoData mesoData ={atoi(rowData[0].c_str()),atoi(rowData[1].c_str()),atoi(rowData[2].c_str()),rowData[3],atoi(rowData[4].c_str()),atof(rowData[5].c_str())};
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
	cout<<"Final 2DArray for plotting"<<endl;
	cout<<"==========================="<<endl;
	cout<<endl;
	//printing the 2D Array
	for (int i = 0; i < NUM_OF_SECTORS; i++){
		for (int j = 0; j < SPEED_BUCKETS; j++){
				cout << wr[i][j] <<"\t";
		}
		cout <<endl;
	}
}


int main(){
	vector<MesoData> inputData= readData();
	aggData(inputData);

}





