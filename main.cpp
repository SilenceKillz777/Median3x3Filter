#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;
/*
class Average{
	public:
		
	int numRows, numCols, minVal, maxVal, newMin, newMax;
	int **imgInAry, **imgOutAry, **mirrorFramedAry, **tempAry;
	int *hist, *neighborAry;
	ifstream inFile;
	ofstream outFile, prettyPrint, histogram;
	
	Average(int numRows, int numCols, int minVal, int maxVal, ifstream& inFile, ofstream& outFile, ofstream& prettyPrint, ofstream& histogram){
	
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
		this->inFile = inFile;
		this->outFile = outFile;
		this->prettyPrint = prettyPrint;
		this->histogram = histogram;
	}
	
	Average(string test){
		inFile.open(test.c_str());
		inFile>>numRows>>numCols>>minVal>>maxVal;
		cout<<"Header: "<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<endl;
		inFile.close();
	}
	
	void loadImage(int** imgInAry, int** mirrorFramedAry,string test){
		int a,b,c,junk;
		int numRows, numCols, minVal, maxVal;
		ifstream in;
		in.open(test.c_str());
		in>>numRows>>numCols>>minVal>>maxVal;
	
		for(int i=0;i<numRows;i++){
			for(int j=0;j<numCols;j++){
				in>>imgInAry[i][j];
			}
		}
		in.close();
	
	}
	void loadImage(int **a,int **b){
		imgInAry = a;
		mirrorFramedAry = b;
	}
	
};
*/

void loadImage(int** imgInAry, int** mirrorFramedAry,string test){
	int numRows, numCols, minVal, maxVal;
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

void mirrorFramed(int numRows, int numCols, int** mirrorFramedAry){
	for(int i=0;i<numRows;i++) {
		for(int j=0;j<numCols;j++) {
			mirrorFramedAry[0][j] = mirrorFramedAry[1][j];					//mirror top
			mirrorFramedAry[numRows+1][j] = mirrorFramedAry[numRows][j];	//mirror bottom
			mirrorFramedAry[i][0] = mirrorFramedAry[i][1];					//mirror left
			mirrorFramedAry[i][numCols+1] = mirrorFramedAry[i][numCols];	//mirror right
		}
	}
}

void ComputeHistogram(int** imgInAry, int* hist, int maxVal, int numRows, int numCols){

	for(int i=0;i<numRows;i++){
		for(int j=0;j<numCols;j++) {
			hist[imgInAry[i][j]]++;
		}
	}
	
	for(int j=0;j<maxVal+1;j++){
			cout<<j<<" "<<hist[j]<<endl;
	}
}

void printHist(int* hist,ofstream& histogram,int numRows, int numCols, int minVal, int maxVal,string test){
	histogram<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<endl;
	for(int i=0;i<maxVal+1;i++)
		histogram<<i<<" "<<hist[i]<<endl;
}

void computeAVG3X3(int numRows, int numCols, int **mirrorFramedAry, int **tempAry, int neighborAry[], int &newMin, int &newMax){
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
	/*
	for(int i=0;i<numRows+2;i++){
		for(int j=0;j<numCols+2;j++){
			cout<<mirrorFramedAry[i][j]<<" ";
		}
		cout<<endl;
	}
	*/
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

void median3X3(int numRows, int numCols, int **mirrorFramedAry, int **tempAry, int neighborAry[], int &newMin, int &newMax){
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

void computeThreshold(int numRows, int numCols, int minVal, int maxVal, int **tempAry, int **imgOutAry, int thresh, ofstream& threshold){
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

void prettyPrint(int numRows, int numCols, int minVal, int maxVal, int **imgOutAry, ofstream& prettyPrint){	
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

int main(int argc, char* argv[]){
	int numRows,numCols,minVal,maxVal,thresh,newMin,newMax,whichMethod;
	ifstream input (argv[1]);
	string test=argv[1];
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
	
	cout<<test<<endl;
	cout<<"Which method would you like to see performed? Enter 1 for Averaging. Enter 2 for Median: ";
	cin>>whichMethod;
	while(whichMethod!=1 && whichMethod!=2){
		cout<<"Invalid Input. Enter 1 for Averaging. Enter 2 for Median: ";
		cin>>whichMethod;
	}
	//Average *temp=new Average(test);
	if(input.is_open()){
		input>>numRows>>numCols>>minVal>>maxVal;
		cout<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<endl;
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
    	
	//	int imgInAry[numRows][numCols], imgOutAry[numRows][numCols];
	//	int mirrorFramedAry[numRows+2][numCols+2], tempAry[numRows+2][numCols+2];
		int hist[maxVal+1];
		for(int i=0;i<maxVal+1;i++){
			hist[i] = 0;
		}
		for(int i=0;i<9;i++){
			neighborAry[i] = 0;
		}
		cout<<"Threshold Value: "<<thresh<<endl;
		//Average processor(numRows, numCols, minVal, maxVal, input, threshold, prettyPrint, histogram);
		//Average processor();
		loadImage(imgInAry,mirrorFramedAry,test);
		ComputeHistogram(imgInAry, hist, maxVal, numRows, numCols);
		mirrorFramed(numRows, numCols, mirrorFramedAry);
		if(whichMethod == 1) 
			computeAVG3X3(numRows, numCols, mirrorFramedAry, tempAry, neighborAry, newMin, newMax);
		else if(whichMethod == 2) 
			median3X3(numRows, numCols, mirrorFramedAry, tempAry, neighborAry, newMin, newMax);
		computeThreshold(numRows, numCols, minVal, maxVal, tempAry, imgOutAry, thresh, threshold);
		printHist(hist, histogram,numRows, numCols, minVal, maxVal,test);
		prettyPrint(numRows, numCols, minVal, maxVal, imgOutAry, prettyP);
		input.close();
	}
	else cout<<"Couldn't retrieve data.";
	input.close();  
	return 0;
	
}
