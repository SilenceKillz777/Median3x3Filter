#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

class imageProcessing{
	public:
		
	int numRows, numCols, minVal, maxVal, thresh, newMin, newMax;
	int **imgInAry, **imgOutAry, **mirrorFramedAry, **tempAry;
	int *hist, *neighborAry;
	ifstream input;
	string test;
	ofstream threshold, prettyP, histogram;
	
	//constructor
	imageProcessing(int numRows, int numCols, int minVal, int maxVal){
	
		this->numRows = numRows;
		this->numCols = numCols;
		this->minVal = minVal;
		this->maxVal = maxVal;
		newMin = maxVal;
		newMax = minVal;
		
		imgInAry = new int*[numRows];
		for(int i = 0; i < numRows; i++)
    		imgInAry[i] = new int[numCols];
    	imgOutAry = new int*[numRows];
		for(int i = 0; i < numRows; i++)
    		imgOutAry[i] = new int[numCols];
    	mirrorFramedAry=new int*[numRows+2];
    	for(int i = 0; i < numRows+2; i++)
    		mirrorFramedAry[i] = new int[numCols+2];
		tempAry = new int*[numRows+2];
		for(int i = 0; i < numRows+2; i++)
    		tempAry[i] = new int[numCols+2];
    		
		hist = new int[maxVal+1];
		neighborAry = new int[9];
		
	}
	
	//destructor
	~imageProcessing(){
		for(int i=0;i<numRows+2;i++){
			delete [] mirrorFramedAry[i];
			delete [] tempAry[i];
		}
		for(int i=0;i<numRows;i++){
			delete [] imgInAry[i];
			delete [] imgOutAry[i];
		}
		delete [] hist;
		delete [] neighborAry;
	}
	
	//methods
	void loadImage(int** imgInAry, int** mirrorFramedAry,string test){
		
		ifstream in;
		in.open(test.c_str());
		in>>numRows>>numCols>>minVal>>maxVal;
	
		for(int i=0;i<numRows;i++){
			for(int j=0;j<numCols;j++){
				in>>imgInAry[i][j];
			}
		}	
		in.close();
	
		in.open(test.c_str());
		in>>numRows>>numCols>>minVal>>maxVal;
		for(int i=0;i<numRows;i++){
			for(int j=0;j<numCols;j++){
				in>>mirrorFramedAry[i+1][j+1];
			}
		}
		in.close();	
	}
	
	void ComputeHistogram(int** imgInAry, int* hist){

		for(int i=0;i<numRows;i++){
			for(int j=0;j<numCols;j++) {
				hist[imgInAry[i][j]]++;
			}
		}
	}
	void mirrorFramed(int** mirrorFramedAry){
		for(int i=0;i<numRows;i++) {
			for(int j=0;j<numCols;j++) {
				mirrorFramedAry[0][j] = mirrorFramedAry[1][j];					//mirror top
				mirrorFramedAry[numRows+1][j] = mirrorFramedAry[numRows][j];	//mirror bottom
				mirrorFramedAry[i][0] = mirrorFramedAry[i][1];					//mirror left
				mirrorFramedAry[i][numCols+1] = mirrorFramedAry[i][numCols];	//mirror right
			}
		}				
	}
	
	void computeAVG3X3(int **mirrorFramedAry, int **tempAry){
		for(int i=1;i<numRows+1;i++) {
			for(int j=1;j<numCols+1;j++) {
				int sum = 0;
				neighborAry[0] = mirrorFramedAry[i][j];			//center
				neighborAry[1] = mirrorFramedAry[i-1][j-1];		//top-left
				neighborAry[2] = mirrorFramedAry[i][j-1];		//middle-left
				neighborAry[3] = mirrorFramedAry[i+1][j-1];		//bottom-left
				neighborAry[4] = mirrorFramedAry[i-1][j];		//top-middle
				neighborAry[5] = mirrorFramedAry[i+1][j];		//bottom-middle
				neighborAry[6] = mirrorFramedAry[i-1][j+1];		//top-right
				neighborAry[7] = mirrorFramedAry[i][j+1];		//middle-right
				neighborAry[8] = mirrorFramedAry[i+1][j+1];		//bottom-right
				for(int k=0;k<9;k++) {
					sum += neighborAry[k];
				}
				tempAry[i][j] = sum/9;
				if (tempAry[i][j]<newMin)
					newMin = tempAry[i][j];
				if (tempAry[i][j]>newMax)
					newMax = tempAry[i][j];
			}
		}
	}
	
	void swap(int *a, int *b){
		int temp = *a;
		*a = *b;
		*b = temp;
	}

	void sort(int neighborAry[]){
		int minIndex=0;
		for(int i=0;i<9;i++){
			minIndex = i;
			for(int j=i+1;j<9;j++){
				if(neighborAry[j]<neighborAry[minIndex])
					minIndex = j;
			}
			swap(&neighborAry[minIndex], &neighborAry[i]);
		}
	}
		
	void median3X3(int **mirrorFramedAry, int **tempAry){
		for(int i=1;i<numRows+1;i++) {
			for(int j=1;j<numCols+1;j++) {
				neighborAry[0] = mirrorFramedAry[i][j];			//center
				neighborAry[1] = mirrorFramedAry[i-1][j-1];		//top-left
				neighborAry[2] = mirrorFramedAry[i][j-1];		//middle-left
				neighborAry[3] = mirrorFramedAry[i+1][j-1];		//bottom-left
				neighborAry[4] = mirrorFramedAry[i-1][j];		//top-middle
				neighborAry[5] = mirrorFramedAry[i+1][j];		//bottom-middle
				neighborAry[6] = mirrorFramedAry[i-1][j+1];		//top-right
				neighborAry[7] = mirrorFramedAry[i][j+1];		//middle-right
				neighborAry[8] = mirrorFramedAry[i+1][j+1];		//bottom-right
				sort(neighborAry);
				tempAry[i][j] = neighborAry[4];
				if (tempAry[i][j]<newMin)
					newMin = tempAry[i][j];
				if (tempAry[i][j]>newMax)
					newMax = tempAry[i][j];
			}
		}
	}
	
	void computeThreshold(int **tempAry, int **imgOutAry, int thresh, ofstream& threshold){
		threshold<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<endl;
		for(int i=1;i<numRows+1;i++) {
			for(int j=1;j<numCols+1;j++) {
				if(tempAry[i][j]>=thresh)
					imgOutAry[i-1][j-1]=1;
				else imgOutAry[i-1][j-1] = 0;
			}
		}
		for(int i=0;i<numRows;i++){
			for(int j=0;j<numCols;j++){
				threshold<<imgOutAry[i][j];
			}
			threshold<<endl;
		}
		threshold.close();
	}
	
	void printHist(int* hist,ofstream& histogram,string test){
		histogram<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<endl;
		for(int i=0;i<maxVal+1;i++)
			histogram<<i<<" "<<hist[i]<<endl;
	}
	
	void prettyPrint(int **imgOutAry, ofstream& prettyPrint){	
		prettyPrint<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<endl;
		for(int i=0;i<numRows;i++){
			for(int j=0;j<numCols;j++){
				if(imgOutAry[i][j]==1) prettyPrint<<imgOutAry[i][j];
				else prettyPrint<<" ";
			}
			prettyPrint<<endl;
		}
		prettyPrint.close();
	}
};

int main(int argc, char* argv[]){
	int numRows,numCols,minVal,maxVal,thresh,newMin,newMax,whichMethod;
	ifstream input (argv[1]);
	string fileName =argv[1];
	thresh =atoi(argv[2]);
	ofstream threshold (argv[3]);
	ofstream prettyP (argv[4]);
	ofstream histogram (argv[5]);
	int** imgInAry;
	int** imgOutAry;
	int** mirrorFramedAry;
	int** tempAry;
	int* hist;
	int neighborAry[9];
	
	cout<<"Which method would you like to see performed? Enter 1 for Averaging. Enter 2 for Median: ";
	cin>>whichMethod;
	while(whichMethod!=1 && whichMethod!=2){
		cout<<"Invalid Input. Enter 1 for Averaging. Enter 2 for Median: ";
		cin>>whichMethod;
	}
	
	if(input.is_open()){
		input>>numRows>>numCols>>minVal>>maxVal;
		newMin = maxVal;
		newMax = minVal;
		
		imgInAry = new int*[numRows];
		for(int i = 0; i < numRows; i++)
    		imgInAry[i] = new int[numCols];
    	imgOutAry = new int*[numRows];
		for(int i = 0; i < numRows; i++)
    		imgOutAry[i] = new int[numCols];
    	mirrorFramedAry=new int*[numRows+2];
    	for(int i = 0; i < numRows+2; i++)
    		mirrorFramedAry[i] = new int[numCols+2];
    	tempAry = new int*[numRows+2];
    	for(int i = 0;i < numRows+2; i++)
    		tempAry[i] = new int[numCols+2];
    	
		int hist[maxVal+1];
		for(int i=0;i<maxVal+1;i++){
			hist[i] = 0;
		}
		for(int i=0;i<9;i++){
			neighborAry[i] = 0;
		}
		imageProcessing processor(numRows, numCols, minVal, maxVal);
		processor.loadImage(imgInAry,mirrorFramedAry,fileName);
		processor.ComputeHistogram(imgInAry, hist);
		processor.mirrorFramed(mirrorFramedAry);
		if(whichMethod == 1) 
			processor.computeAVG3X3(mirrorFramedAry, tempAry);
		else if(whichMethod == 2) 
			processor.median3X3(mirrorFramedAry, tempAry);
		processor.computeThreshold(tempAry, imgOutAry, thresh, threshold);
		processor.printHist(hist, histogram,fileName);
		processor.prettyPrint(imgOutAry, prettyP);
		
		input.close();
	}
	else cout<<"Couldn't retrieve data.";
	input.close();  
	return 0;
	
}
