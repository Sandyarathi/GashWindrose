/*
 * Serial.cpp
 *
 *  Created on: Oct 21, 2015
 *      Author: sandyarathidas
 */

#include<unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <sys/time.h>
#include <array>
using namespace std;

const int NUM_OF_SECTORS = 16;
const int SPEED_BUCKETS = 5;

int calcSpeedBin(float windSpd) {
	if (windSpd == 0)
		return 0;
	else if (windSpd > 0 and windSpd <= 5)
		return 1;
	else if (windSpd > 5 and windSpd <= 15)
		return 2;
	else if (windSpd > 15 and windSpd <= 25)
		return 3;
	else
		return 4;
}

int calcDirectBin(double windDir) {
	return windDir / NUM_OF_SECTORS;
}

struct MesoData {

	string stid;
	string stnName;
	string obsDate;
	double windSpd;
	double windDir;
};


vector<MesoData> readData(vector<string> List) {

	vector<MesoData> inData;
	string path = "../InputData/2001-2009CSVs/aggregateData/";
	for (int i = 0; i < List.size(); i++) {
		ifstream inputfile1(path + List[i]);
		string line;
		string rowData[5] ;
		string token;
		int j = 0;
		while (getline(inputfile1, line)) {
			istringstream lineStream(line);
			j = 0;
			while (getline(lineStream, token, ',')&& j<5) {
				rowData[j++] = token;
			}
			MesoData mesoData = { rowData[0], rowData[1], rowData[2], atof(
					rowData[3].c_str()), atof(rowData[4].c_str()) };
			inData.push_back(mesoData);
			lineStream.clear();
		}
		inputfile1.close();
	}
	return inData;
}

auto aggData(vector<MesoData> data) {
	auto wr = new int[NUM_OF_SECTORS][SPEED_BUCKETS]();
	for (MesoData m : data) {
		int s = calcSpeedBin(m.windSpd);
		int d = m.windDir;
		if (s < NUM_OF_SECTORS and d < SPEED_BUCKETS and d > 0)
			wr[d][s]++;
	}
	return wr;
}

vector<string> readFileList(string filepath) {
	vector<string> list;
	ifstream inputfile(filepath);
	string line;
	while (getline(inputfile, line)) {
		list.push_back(line);
	}
	inputfile.close();
	return list;
}


int main(int argc, char* argv[]) {
	string fileListpath = "../InputData/2001-2009CSVs/compileList2.txt";
	vector<string> vectorOfFilePaths = readFileList(fileListpath);
	vector<MesoData> inputData = readData(vectorOfFilePaths);
	auto resultArray = aggData(inputData);
		cout << endl;
		cout << "...................................................." << endl;
		cout << "                  Final 2D Array                    " << endl;
		cout << "...................................................." << endl;

		for (int i = 0; i < NUM_OF_SECTORS; i++) {
			for (int j = 0; j < SPEED_BUCKETS; j++) {
				cout << resultArray[i][j] << "\t";
			}
			cout << endl;
		}
	return 0;

}

