/* "Let's make some K-D trees"
	- Taylor Thomas, 3/2021
*/
#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

const int BUCKET_SIZE = 10;
struct node{
	int dim;
	vector<vector<int>> data;
	int median;
	node* leftChild;
	node* rightChild;
};

vector<node*> bottomChildren; 
// Constructs a new internal node with the given bucket of data
node* newNode(vector<vector<int>> data){
    node* newNode = new node();
    newNode->data = data;
    newNode->leftChild = NULL;
    newNode->rightChild = NULL;
    return newNode;
}
// Prints all elements in a vector, separated by a space character
void printVector(vector<int> v){
    for (int i = 0; i < v.size(); i++)
        cout << v[i] << " ";
}
void printMatrix(vector<vector<int>> vec){
    for (int i = 0; i < vec.size(); i++){
        printVector(vec[i]);
        cout << endl;
    }
}

// function to find range of any given vector
int findRange(vector<int> vec){
	int max = vec[0];
	int min = vec[0];
	int range;
	for (int i = 1 ; i < vec.size();i++){
		if (max < vec[i]){ //find max in this vector
			max = vec[i];
		}
		if (min > vec[i]){
			min = vec[i];
		}
	}
	range = max - min;
	return range;
}
// function to find optimal dimension to discriminate on (optimal column)
int findBestDimension(vector <vector<int>> data){
	vector<int> dimensionData;
	vector<int> ranges; //vector of ranges of each dimension
	for (int j = 0 ; j < data[1].size();j++){ // through each dimension
		for (int i = 0 ; i < data.size();i++) //through each row (data point)
		{
			
			dimensionData.push_back(data[i][j]); //accumulate each dimension for each iteration of inside loop
		}
		ranges.push_back(findRange(dimensionData)); //put the range of the latest dimension into the vector of ranges 
		
		
		// test to see if dimensions are captured correctly; they are ! (:
		/*
		for ( int i =0; i < dimensionData.size(); i ++){
			cout << "Data Point [" << i << "] in Dimension [" << j << "]: " << dimensionData[i] << endl ;
		} 
		*/
		
	}
	// find max range of dimensions, break tie: earliest dimension has precedent
	int maxRange = ranges[0];
	int maxDim = 0;
	for (int i = 0; i < ranges.size();i++){
		if (maxRange < ranges[i]){ // if they are ==, the earliest will stay max...
			maxRange = ranges[i];
			maxDim = i;
		}
	}
	return maxDim; //returns which dimension to use
}
// turns vector of vectors into vector based on which dim you want
vector<int> getDimension(int dimension, vector<vector<int>> data){
	vector<int> dimVector;
	for (int i = 0 ; i < data.size();i++){ //through each row (data point)
		dimVector.push_back(data[i][dimension]); //accumulate each dimension for each iteration of inside loop
		}
	return dimVector;
}

// swaps pointers a and b
void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}


// returns position of pivot
int Partition(int arr[], int l, int r)
{
    int lst = arr[r], i = l, j = l;
    while (j < r) {
    
        if (arr[j] < lst) {
            swap(&arr[i], &arr[j]);
            i++;
        }
        j++;
    }
    
    swap(&arr[i], &arr[r]);
    return i; //index of pivot
}
 
 // pick random pivot between l and r; partitions array around that element
int randomPartition(int arr[], int l,
                    int r)
{
    int n = r - l + 1;
    int pivot = rand() % n;
    
    swap(&arr[l + pivot], &arr[r]);
    return Partition(arr, l, r);
}
 

// Utility function to find median
void MedianUtil(int arr[], int l, int r,
                int k, int& a, int& b) // pass a and b by reference
{
    // if l < r
    if (l <= r) {
 
        // Find the partition index
        int partitionIndex
            = randomPartition(arr, l, r);
 
        // index of partition = k, we done
        if (partitionIndex == k) {
            b = arr[partitionIndex];
            if (a != -1)
                return;
        }
 
        // a & b are middles
        else if (partitionIndex == k - 1) {
            a = arr[partitionIndex];
            if (b != -1)
                return;
        }
 
        // recursive step if index > k
        if (partitionIndex >= k)
            return MedianUtil(arr, l,
                              partitionIndex - 1,
                              k, a, b);
 	// recursive step if index < k
        else
            return MedianUtil(arr,
                              partitionIndex + 1,
                              r, k, a, b);
    }
 
    return;
}
 
// Function to find Median
int getMedian(vector<int> vec, int n)
{
	int arr[vec.size()];
	for ( int i = 0;i <vec.size();i++){
		arr[i] = vec[i];
	}
    int med, a = -1, b = -1;
 
    // if size of vec is odd number
    if (n % 2 == 1) {
        MedianUtil(arr, 0, n - 1,
                   n / 2, a, b);
        med = b;
    }
 
    // if size of vecis even
    else {
        MedianUtil(arr, 0, n - 1,
                   n / 2, a, b);
        med = (a + b) / 2;
    }
 
    // return median
    return med;
}

int findBestValue(vector<int> bestDimension){
	int n = bestDimension.size();
	//int kSmallest = bestDimension.size()/2;
	
	int bestValue = getMedian(bestDimension, n);
	return bestValue;

}
void splitData(int median, int dim, vector<vector<int>> vec, vector<vector<int>>& leftVec, vector<vector<int>>& rightVec){
	
	for (int i = 0 ; i < vec.size();i++){
		if ( vec[i][dim]<=median){
		leftVec.push_back(vec[i]);
	}
	else
		rightVec.push_back(vec[i]);	
	}
}
	
	
	
void buildTree(node* node){ 
	vector<vector<int>> bucket = node->data;
	int bestDimNum =findBestDimension(bucket); // find the best dimension (0-k)
	node->dim = bestDimNum;
	
	vector<int> bestDimension = getDimension(bestDimNum, bucket); //get the vector form of bestDim
	int bestValue= findBestValue(bestDimension); //find best value for this dimension
	node->median = bestValue; 

	
	vector<vector<int>> leftVec; //initialize subArrays for each step
	vector<vector<int>> rightVec;
	
	


	splitData(bestValue, bestDimNum, bucket, leftVec, rightVec); //split data into 2 subarrays
	
	//printMatrix(bucket);
	cout << endl << endl;
	node->leftChild = newNode(leftVec);

	if (node->leftChild->data.size() > BUCKET_SIZE)
		buildTree(node->leftChild);
	else
		bottomChildren.push_back(node->leftChild);
	
	node->rightChild = newNode(rightVec);
	if(node->rightChild->data.size() > BUCKET_SIZE)
		buildTree(node->rightChild);
	else
		bottomChildren.push_back(node->rightChild);

}
// Find the bucket the probe would lie within 
vector<vector<int>> findProbeBucket(vector<int> probe, node* rootNode){
    node* currentNode = rootNode;
    // find the bottom node the probe fits in
    while (currentNode->leftChild != NULL){
        int dim = currentNode->dim;
        int val = currentNode->median;
        if (probe[dim] <= val)
            currentNode = currentNode->leftChild;
        else currentNode = currentNode->rightChild;
    }
    // return the bottom nodes data
    return currentNode->data;
}
// EC
// Find the closest point (within the bucket of the probe) to the given probe 
vector<int> findNearestBucketNode(vector<int> probe, vector<vector<int>> vec){
    vector<vector<int>> out;
    int minDistance = 1954324532; // minimum distance between probe and point 
    int pointNumber; // index of point that is closest to the probe in v 
    // iterate through bottomChildren to find which node is closest to probe
    for (int i = 0; i < vec.size(); i++){
        int distance_pow2=0; // distance between 2 points squared, same as sqrt... okokk
        for (int j = 0; j < probe.size(); j++){
            int dimDistance = probe[j] - vec[i][j];
            distance_pow2 += dimDistance*dimDistance;
        }
        if (distance_pow2 < minDistance){ //find min dist
            minDistance = distance_pow2;
            pointNumber = i; // index of closest point
        }
    }
    // return the point closest to the probe
    return vec[pointNumber];
}
int main(int argc, char* argv[]){
	int k = stoi(argv[1]); //number of dimensions
	int n = stoi(argv[2]); //how many data points to place in the tree
	int p = stoi(argv[3]); // number of probes into the tree
	int input;
	
	vector <vector<int>> data; //making a vector of vectors

	//putting random gen values in vector data
	for (int i = 0; i < n ; i++) { //rows of the vector of vectors --
		vector<int> dimensions;
		for (int j = 0; j < k ; j++) { //cols of vector of vectors | |
			cin >> input;
			dimensions.push_back(input);
		
		}
		data.push_back(dimensions); // adding each data point to the vector of vectors
	}
	//initializing probes
	vector<vector<int>> probes; 
	for (int i = 0; i < p; i++){
		vector<int> dataPoints;
		for (int j = 0 ; j <k;j++){
			int temp;
			cin >> temp;
			dataPoints.push_back(temp);
		}
		probes.push_back(dataPoints);
	}
	
	node* rootNode = newNode(data);
	buildTree(rootNode);
	
	// print the output of the probe information
     for (int i = 0; i < probes.size(); i++){
         cout << "probe ";
         printVector(probes[i]);
         cout << " reaches bucket: ";
         vector<vector<int>> probeBucket = findProbeBucket(probes[i], rootNode);
         cout << probeBucket.size() << " elements\n";
        printMatrix(probeBucket);
   	     cout << "Closest to probe: ";
         printVector(findNearestBucketNode(probes[i], probeBucket));
         cout << endl;
        
     }

	

return 0;
}

