/*
 * OpenMPApproach1.cpp
 *
 *  Created on: Oct 22, 2015
 *      Author: sandyarathidas
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <sys/time.h>
#include <omp.h>
#include <cmath>
#include <mm_malloc.h>

using namespace std;

const int NUM_OF_SECTORS = 16;
const int NUM_OF_SPEED = 5;
const int MAX_NUM_DATA_POINTS = pow(2,27); // 2^27 = 134217728
const int STRIP_WIDTH = 64;

struct MesoData {

	int maxDataPoints;
	int numDataPoints;
	string stid;
	string stnName;
	string obsDate;
	float* windSpd;
	float* windDir;

};

typedef int outputData[NUM_OF_SECTORS][NUM_OF_SPEED];

int calcSpeedsBin(float winSpd);

int calcDirectBin(int winDir);

void readData(MesoData & inputData);

void aggData(MesoData & inputData, outputData & outData);


int main(){
	struct timeval start, end;
	double delta;



	cout<<"Hello World... I am processing.." << endl << endl;

	MesoData inputData = {MAX_NUM_DATA_POINTS,
				0,

				(float*) _mm_malloc(sizeof(int)*MAX_NUM_DATA_POINTS, 64),
				(float*) _mm_malloc(sizeof(float)*MAX_NUM_DATA_POINTS, 64)
		};

	outputData outData;
	string fileListpath = "../InputData/2001-2009CSVs/compileList2.txt";
	vector<string> vectorOfFilePaths = readFileList(fileListpath);

	readData(inputData,vectorOfFilePaths);

	gettimeofday(&start, NULL);
	aggData(inputData, outData);

	gettimeofday(&end, NULL);
	delta = (end.tv_sec  - start.tv_sec) +
		         ((end.tv_usec - start.tv_usec) / 1.e6);

	cout<< endl;
	printf("%.6lf seconds elapsed\n", delta);

	free(inputData.windDir);
	free(inputData.windSpd);

}

int calcSpeedsBin(float winSpd) {
	if ( winSpd == 0) return 0;
	else if ( winSpd > 0 and winSpd <= 5) return 1;
	else if ( winSpd > 5 and winSpd <= 15) return 2;
	else if ( winSpd > 15 and winSpd <= 25) return 3;
	else if ( winSpd > 25) return 4;
	else{
		return -1;
	}
}

int calcDirectBin(int winDir) {
	// 0-360 - cut into linear line 0-359
	while(winDir<0){

		winDir = winDir + 360;
	}
	return winDir / NUM_OF_SECTORS;
}


void readData(MesoData & inputData, vector<string> list) {
	struct timeval start, end;
	double delta;

	gettimeofday(&start, NULL);
	omp_set_num_threads(4);

	MesoData mesoArray[4];

	#pragma omp parallel
	{
		string line;
		int count = 0;

		MesoData localInData ={MAX_NUM_DATA_POINTS,
				0,
				(int*)calloc(MAX_NUM_DATA_POINTS, sizeof(int)),
				(float*)calloc(MAX_NUM_DATA_POINTS, sizeof(float))
		};

		int tid = omp_get_thread_num();
		string path ="../InputData/2001-2009CSVs/aggregateData/";
		#pragma omp for
		for(int j=0; j<list.size(); j++){
				ifstream inputFile(path+list[j]);
				//getline(inputFile, line);	// ignoring first line of column names
				string line;
				string rowData[5] ;
				string token;
				while(getline(inputFile, line)){
					istringstream lineStream(line);
					int i=0;
					while(getline(lineStream,token,',')){
							rowData[i++]=token;
					}
					localInData.stid[count] = rowData[0];
					localInData.stnName[count] = rowData[1];
					localInData.obsDate[count] = rowData[2];
					localInData.windDir[count] = strtof(rowData[4].c_str(), NULL);
					localInData.windSpd[count] = strtof(rowData[5].c_str(), NULL);

					count++;
				}
			}

		localInData.numDataPoints = count;
		mesoArray[tid] = localInData;

	}
	gettimeofday(&end, NULL);


	for(int k=0; k< 4; k++){
		int min;
		inputData.numDataPoints += mesoArray[k].numDataPoints;

		if(k == 0)
			min=0;
		else
			min= mesoArray[k-1].numDataPoints;

		for(int n=0; n< mesoArray[k].numDataPoints; n++){
				inputData.windDir[min+n] = mesoArray[k].windDir[n];
				inputData.windSpd[min+n] = mesoArray[k].windSpd[n];
			}

		free(mesoArray[k].windDir);
		free(mesoArray[k].windSpd);
	}


	delta = (end.tv_sec  - start.tv_sec) +
			         ((end.tv_usec - start.tv_usec) / 1.e6);

	cout<< endl;
	printf("%.6lf seconds elapsed in reading the data\n", delta);
}

void aggData(MesoData & inputData, outputData & outData){

	omp_set_num_threads(4);

	cout<<"No. of data points = "<< inputData.numDataPoints << endl;

	#pragma omp parallel
	{
		// initialising local output array for each thread
		outputData localOutData;
		for(int i=0; i< NUM_OF_SECTORS; i++){
			for(int j=0; j< NUM_OF_SPEED; j++)
				localOutData[i][j] = 0;
		}

		#pragma omp for
		for(int k=0; k< inputData.numDataPoints; k+= STRIP_WIDTH){

			int __attribute__((aligned(64))) D[STRIP_WIDTH] ;
			int __attribute__((aligned(64))) S[STRIP_WIDTH];

			const int* winDirection = &(inputData.windDir[k]);
			const float* winSpeed = &(inputData.windSpd[k]);

			#pragma vector aligned
			for(int c=0; c<STRIP_WIDTH; c++){
				D[c] = calcDirectBin(winDirection[c]);
				S[c] = calcSpeedsBin(winSpeed[c]);
			}

			for(int c=0; c<STRIP_WIDTH; c++){
				localOutData[D[c]][S[c]]++;
			}
		}

		#pragma omp for
		for(int i= inputData.numDataPoints % STRIP_WIDTH; i< inputData.numDataPoints;i++){
				int d = calcDirectBin(inputData.windDir[i]);
				int s = calcSpeedsBin(inputData.windSpd[i]);

				localOutData[d][s]++;
		}

		for(int i=0; i< NUM_OF_SECTORS; i++){
			for(int j=0; j< NUM_OF_SPEED; j++){
				#pragma omp atomic
				outData[i][j] += localOutData[i][j];
			}
		}

	}
}





