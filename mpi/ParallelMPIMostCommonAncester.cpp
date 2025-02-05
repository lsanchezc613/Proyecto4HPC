#include <mpi.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <bits/stdc++.h>
using namespace std;


vector <char> commonAncestor(vector<vector<char> > &matrix, int const myRank, int const nRanks){
    int m = matrix[0].size();   //columns
    int n = matrix.size();    // rows
    vector<int> A(m, 0.0);
    vector<int> C(m, 0.0);
    vector<int> G(m, 0.0);
    vector<int> T(m, 0.0);

    const int rowsPerProcess = double(m)/double(nRanks);
    const int myFirstRow = 1 + int(rowsPerProcess*myRank);
    const int myLastRow = 1 + int(rowsPerProcess*(myRank+1);

    //fill nitrogenous bases vectors
    for(int i =0; i<m; i++){
        for(int j =0; j<n; j++){
            if(matrix[j][i]=='A'){
                A[i]++;
            } else if(matrix[j][i]=='C'){
                C[i]++;
            } else if(matrix[j][i]=='G'){
                G[i]++;
            } else {
                T[i]++;
            }
        }
	
    }
    
    //consensus vector
    vector<char> consensus(m, 0.0);
    int max=0;
    for(int i =0; i< m ; i++){
        max = A[i];
        consensus[i]= 'A';
        if(max<C[i]){
            max = C[i];
            consensus[i]= 'C';
        } 
        if(max<G[i]){
            max = G[i];
            consensus[i]= 'G';
        } 
         if(max<T[i]){
             max = T[i];
             consensus[i]= 'T';
        }
    }
    return consensus;

}

vector<vector<char> > dnaMatrix(string infile){
    string line;
    int col;
    ifstream MyReadFile(infile.c_str());
    vector<vector<char> > output;
    while (getline (MyReadFile, line)) {
        col = line.length();
        vector<char> lineVector;
        for(int i=0; i< col; i++){ 
            lineVector.push_back(line[i]);
        }
        output.push_back(lineVector);
        lineVector.clear();
    }
    MyReadFile.close();
    return output;
}

void printVector(vector<char> &vector, ofstream &file){
    for (int j = 0; j < vector.size(); j++) { 
            file << vector[j]; 
    }
    file << "\n";
}

int main() {
    const double t0 = omp_get_wtime();
    
    MPI_Init(&argc, &argv);
    int myRank, nRanks;
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    MPI_Comm_size(MPI_COMM_WORLD, &nRanks);

    ofstream outputFile;
    outputFile.open("selectedAncestors.txt");
    vector<vector<char> > datasetMatrix;
    vector<char> output;

    //dataset 1
    datasetMatrix = dnaMatrix("dataset1.txt");
    output = commonAncestor(datasetMatrix, myRank, nRanks);
    printVector(output, outputFile);

    //dataset 2
    datasetMatrix = dnaMatrix("dataset2.txt");
    output = commonAncestor(datasetMatrix, myRank, nRanks);
    printVector(output, outputFile);

    //dataset 3
    datasetMatrix = dnaMatrix("dataset3.txt");
    output = commonAncestor(datasetMatrix, myRank, nRanks);
    printVector(output, outputFile);

    outputFile.close();
    
    //process final dataset
    outputFile.open("output.txt");
    datasetMatrix = dnaMatrix("selectedAncestors.txt");
    output = commonAncestor(datasetMatrix, myRank, nRanks);
    printVector(output, outputFile);
    outputFile.close();

    
   const double t1 = omp_get_wtime();
    printf("Time(sec): %f\n", t1 - t0);
    //    return 0;
}
