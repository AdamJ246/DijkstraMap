#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <climits>
#include <stack>
using namespace std;
#include "HashTable.h"
#include "graph.h"
#include "queue.h"

//The Graph Piece representing one vertex and what it holds:
struct GPiece{
	string name;
	bool mark;
	int dist;
	string prev;
};

void runTheProgram(ifstream& file);
void readFromFile(ifstream& file, Graph<string>& cityGraph, int& numCity, vector<string>& cityNames, vector<GPiece>& GPArray);
void startVertInput(Graph<string>& cityGraph, vector<GPiece>& GPArray, string& startVert);
bool allMark(vector<GPiece>& GPArray);
void markAndAdjacent(Graph<string>& cityGraph, vector<GPiece>& GPArray, string startVert, string& newStartVert, Queue<GPiece>& passedVerts,
 stack<string>& preStack, bool& isCycle);
void PrintCityQ(const Queue<string>& cityQ, Graph<string>& cityGraph, vector<GPiece> GPArray, int toice);
bool leastNotFound(int dist, vector<int>& alreadyLeast);
void printInOrder(vector<GPiece>& GPArray);

/* The purpose of the main function is to open the user input text file specifically
 * made for graph analyzation and input it into the primary "runTheProgram" function
 * which takes care of the rest of the program function.
 * If a file is not found or an incorrect filename is input, then the function will error and no code will run.
*/
int main(int argc, char *argv[]){
	if(argc != 2)
		cerr << "Usage: " << argv[0] << " <filename>" << endl;
	ifstream file(argv[1]);
	if(file.is_open()){
		runTheProgram(file);
		file.close();
	}else{
		cerr << "Error: Unable to open file." << endl;
		return 1;
	}	
return 0;
}

/* The primary runTheProgram function serves to organize the output of the code,
 * as well as define necessary variables used throughout the program.
 * Additionally, and most importantly, it will call the necessary sub functions to perform the code.
 * It only takes in the input file stream.
*/
void runTheProgram(ifstream& file){
	int numCity = 0, trav = 0, counter = 0;
	string startVert, newStartVert, originalStartVert;
	Graph<string> cityGraph(100);
	Queue<string> cityQ(100);
	vector<string> cityNames;
	vector<GPiece> GPArray;
	Queue<GPiece> passedVerts(100);	
	stack<string> preStack;
	bool isCycle = false;	
	
	//FIRST, we read the data from the file and store the data into necessary structures.
	readFromFile(file, cityGraph, numCity, cityNames, GPArray);
	//this commented out printCityQ can be used to see the distances between vectors and their adjacencies.
		//PrintCityQ(cityQ, cityGraph, GPArray, 2);
	cout << endl << setw(60) << "^^^^^^^^^^^^^^^^ DIJKSTRA'S ALGORITHM ^^^^^^^^^^^^^^^^" << endl << endl;
	cout << "A Weighted Graph Has Been Built For These " << numCity << " Cities: " << endl;
	
	//this block sorts and prints out the city names.
	sort(cityNames.begin(), cityNames.end());
	for(int i = 0; i < cityNames.size(); i++){
		if(trav >= 3){
			cout << endl;
			trav = 0;
		}
		trav++;
		cout << setw(18) << cityNames[i] << " ";
	}
	cout << endl << endl;
	//this function takes the users input to determine a starting vertex for the program.
	startVertInput(cityGraph, GPArray, startVert);
	cout << "------------------------------------------------------------------" << endl;
	cout << setw(18) << "Vertex" << setw(18) << "Distance" << setw(18) << "Previous" << endl;
	//this loop displays the chosen vertex
	for(int i=0; i < GPArray.size(); i++){
                if(GPArray[i].name == startVert)
                        cout << setw(18) << GPArray[i].name << setw(18) << GPArray[i].dist << setw(18) << GPArray[i].prev << endl;
        }
	//this loop is the PRIMARY loop of the program, where the program recursively traverses the graph to determine each vertex's
	//shortest distance from the previous starting vertex. a hard failsafe counter is implemented so that when disconnected vertices are
	//involved, the loop does not run forever.
	while(!allMark(GPArray) && counter <= 100){
		markAndAdjacent(cityGraph, GPArray, startVert, newStartVert, passedVerts, preStack, isCycle);
		startVert = newStartVert;
		counter++;
	}
	//After the primary recursion is finished, this loop sets any non marked vertices to have a distance of -1, representing NO CONNECTION
	if(!allMark(GPArray)){
		for(int i=0; i<GPArray.size(); i++){
			if(GPArray[i].mark != true){
				GPArray[i].dist = -1;
			}
		}
	}
	//This function prints out the distances in sorted order.
	printInOrder(GPArray);
	cout << "------------------------------------------------------------------" << endl;
	//this simple if statement determines whether a cycle was detected or not.
	if(isCycle){
		cout << setw(43) << "A cycle has been detected" << endl << endl;
	}else{
		cout << setw(43) << "No cycle detected" << endl;
	}
}

/* This function calls upon the user to input a starting vertex to determine how the program traverses the graph.
 * It will tell the user if the input vertex exists at all in the graph, or if it has no path to traverse out from.
*/
void startVertInput(Graph<string>& cityGraph, vector<GPiece>& GPArray, string& startVert){
	bool startVertCorrect = false, hasEdge = true;
	cout << "Please input your starting vertex: ";
	Queue<string> hasEdgeQ(50);
	while(!startVertCorrect){
       		cin >> startVert;
		cout << endl;
		for(int i=0; i < GPArray.size(); i++){
			if(GPArray[i].name == startVert){
                	       	cityGraph.GetToVertices(GPArray[i].name, hasEdgeQ);
				if(hasEdgeQ.isEmpty()){
					cout << "Vertex cannot be used as a start. Try Again: ";
					hasEdge = false;
				}else{
					GPArray[i].prev = "N/A";
                		        GPArray[i].dist = 0;
       	        	        	GPArray[i].mark = true;
					startVertCorrect = true;
        	        	}
			}
	        }
		if(!startVertCorrect && hasEdge == true){
			cout << "Vertex not found. Try again: ";
		}
		hasEdge = true;
        }
}

/* This function checks to make sure if every vertex in the graph has been marked.
*/
bool allMark(vector<GPiece>& GPArray){
	for(int i=0; i < GPArray.size(); i++){
		if(GPArray[i].mark != true)
			return false;
	}
	return true;
}

/* This large function is the recursive function responsible for traversing a graph.
 * It will start at a starting vertex given, check to see what adjacent vertices there are, determine the smallest
 * distance vertex, and mark that vertex before returning that as the new starting vertex.
 * It will enqueue any vertexes that were unmarked and passed by for the program to later return to when the current
 * path reaches its end.
 * Simoltaneously, it will stack the current paths names to determine if there is a cycle once the end of a path is reached.
*/
void markAndAdjacent(Graph<string>& cityGraph, vector<GPiece>& GPArray, string startVert, string& newStartVert, Queue<GPiece>& passedVerts,
 stack<string>& preStack, bool& isCycle){
	Queue<string> myQ(100);
	GPiece tempPiece, tempPass;
	bool isUnmark = false, isUnmark2 = false;
	int weight = 0, least = INT_MAX;
	//cout << 1;
	//traverse the array of structs, taking whichever index corresponds to the "startVert" and find its adjacent vertices. Additionally,
	//a tempPiece GPiece will store the values of this start vertex to reference as a previous later.
	for(int i=0; i < GPArray.size(); i++){
		if(GPArray[i].name == startVert){
			cityGraph.GetToVertices(GPArray[i].name, myQ);
			tempPiece = GPArray[i];
		}
	}
	//create a new adjVertices vector for this specific runs adjacent vectors. This is so that comparison of the least distance is possible.
	vector<GPiece> adjVert;
	//thus, the queue will be emptied into this new array here:
	while(!myQ.isEmpty()){
		for(int i=0; i<GPArray.size(); i++){
			if(myQ.getFront() == GPArray[i].name){
				adjVert.push_back(GPArray[i]);
			}
		}
		myQ.dequeue();
	}
	//traverse the adjVertices and find their weight on the graph with respect to the starting vertex. if the weight is less than its current
	//weight and the adjVertex is not already marked, a new smaller weight value will be stored.
	for(int i=0; i<adjVert.size(); i++){
		weight = cityGraph.WeightIs(tempPiece.name, adjVert[i].name);
		if((adjVert[i].mark != true) && (adjVert[i].dist > (weight + tempPiece.dist))){
			adjVert[i].dist = weight + tempPiece.dist;
			adjVert[i].prev = tempPiece.name;
			//THIS boolean lets the program know that a unmarked vertex was found.
			isUnmark = true;
		}
	}
	//this determines the least distant adjacent vertex length.
	for(int i=0; i<adjVert.size(); i++){
		if(adjVert[i].mark != true){
			if(adjVert[i].dist < least){
				least = adjVert[i].dist;
			}
		}
	}
	//here, the least distance adjacent vertex is marked and determined as the new starting vertex for the next function run.
	for(int i=0; i<adjVert.size(); i++){
		if((adjVert[i].mark != true) && (adjVert[i].dist == least)){
			adjVert[i].mark = true;
			preStack.push(adjVert[i].name);
			newStartVert = adjVert[i].name;
			//this commented code can be used to print out the adjVert as its FOUND, not sorted: print out the newly marked vertex.
				//cout << setw(18) << adjVert[i].name << setw(18) << adjVert[i].dist << setw(18) << adjVert[i].prev << endl;
		}
	}
	//store away any unused and unmarked verticies if any
	for(int i=0; i<adjVert.size(); i++){
		if(adjVert[i].mark != true){
				passedVerts.enqueue(adjVert[i]);
			
		}
	}
	//return to the original array of structs and update the values for each changed vertex.
	for(int i=0; i<GPArray.size(); i++){
		for(int j=0; j<adjVert.size(); j++){
			if(adjVert[j].name == GPArray[i].name){
				GPArray[i] = adjVert[j];
			}
		}
	}
	//if no unmarked vertex was found in the weight determining process, return to a previous unmarked vertex if there are any and proceed
	//down its own path.
	stack<string> tempStack = preStack;
	if(isUnmark != true){
		while(!preStack.empty()){
			for(int i=0; i<adjVert.size(); i++){
				tempStack = preStack;
				while(!tempStack.empty()){
					if(tempStack.top() == adjVert[i].name){
						isCycle = true;
					}
					tempStack.pop();
				}
			}
			preStack.pop();
		}
		if(!passedVerts.isEmpty()){
			tempPass = passedVerts.getFront();
			newStartVert = tempPass.prev;
			passedVerts.dequeue();
		}
	}
}

/* As stated in the name, this function is responsible for parsing the data from the file into its
 * three seperate components each line: the origin, destination, and distance edge between these two.
 * The function will traverse each line and determine correctly where to put certain values in order to create:
 * the graph, the vector of GPieces, the nameList, and the hash table.
 * The code specifically denies duplicates in any of these lists.
*/
void readFromFile(ifstream& file, Graph<string>& cityGraph, int& numCity, vector<string>& cityNames, vector<GPiece>& GPArray){
	string line;
	int GPIndex = 0;
	bool isDoub = false;
	const string NotFound = "ZZZ";
	HashTable<string> Cities(NotFound, 100);
	int tripLength;
	GPiece tempP;
	string origin, destination;
	//while there is a line in the file, traverse the line
	while(getline(file, line)){
                istringstream iss(line);
		getline(iss, origin, ';');
		getline(iss, destination, ';');
		iss >> tripLength;
		//if there is not already an origin in the hash, store it everywhere
		if((Cities.find(origin) != origin)){
			cityGraph.AddVertex(origin);
			Cities.insert(origin);
			numCity++;
			cityNames.push_back(origin);
		}
		tempP.prev = "N/A";
		//if there is not already a destination in the hash, store it everywhere
		if((Cities.find(destination) != destination)){
			cityGraph.AddVertex(destination);
			Cities.insert(destination);
			numCity++;
			cityNames.push_back(destination);
		}
		//this next block is responsible for storing the created temporary piece into the GPArray.
		tempP.name = destination;
		tempP.dist = INT_MAX;
		for(int i = 0; i < GPArray.size(); i++){
			if(GPArray[i].name == tempP.name){
				isDoub = true;	
			}
		}
		if(!isDoub){
			GPArray.push_back(tempP);
		}
		isDoub = false;
		tempP.name = origin;
		for(int i=0; i < GPArray.size(); i++){
			if(GPArray[i].name == tempP.name){
				isDoub = true;
			}
		}
		if(!isDoub){
			GPArray.push_back(tempP);
		}
		isDoub = false;	
		//add an edge to the graph connecting the origin and destination
		cityGraph.AddEdge(origin, destination, tripLength);
	}
}

/* This function originated as a adjacency printer. However, I have morphed it into a debug function
 * that would display either the adjacent pieces of a specific name OR display the edge distances between
 * every verticie.
*/
void PrintCityQ (const Queue<string>& cityQ, Graph<string>& cityGraph, vector<GPiece> GPArray, int toice){
	Queue<string> TempQ = cityQ;
	Queue<string> TempQ2;
	int weight;
	if(toice == 1){
	while(!TempQ.isEmpty()){
		cout << TempQ.getFront() << endl;
		TempQ.dequeue();
	}
	}else if(toice == 2){
		for(int i=0; i < GPArray.size(); i++){	
			cityGraph.GetToVertices(GPArray[i].name, TempQ2);
			cout << "Verticies of: " << GPArray[i].name << ": ";
			while(!TempQ2.isEmpty()){
				weight = cityGraph.WeightIs(GPArray[i].name, TempQ2.getFront());
				cout << TempQ2.getFront() << "-> ";
				cout << weight;
				TempQ2.dequeue();
			}
			cout << endl;
		}	
	}
}

/* This minor function is used in the printInOrder function, and determines if a value has not already been encountered in the least list.
*/
bool leastNotFound(int dist, vector<int>& alreadyLeast){
	for(int i=0; i<alreadyLeast.size(); i++){
		if(dist == alreadyLeast[i]){
			return false;
		}
	}
	return true;
}

/* This function prints the finalized GPArray in distance sorted order.
*/
void printInOrder(vector<GPiece>& GPArray){
	int leastPrint = INT_MAX, printCnt = 1, justInCaseCnt = 0;	
	vector<int> alreadyLeast;
        alreadyLeast.push_back(0);
	while((printCnt == GPArray.size()-1) || (justInCaseCnt <= 500)){
                for(int i=0; i<GPArray.size(); i++){
                        if(((GPArray[i].dist < leastPrint) && GPArray[i].dist != -1) && leastNotFound(GPArray[i].dist, alreadyLeast)){
                                leastPrint = GPArray[i].dist;
                        }else if(GPArray[i].dist == -1){
                                printCnt++;
                        }
                }
                for(int i=0; i<GPArray.size(); i++){
                        if(GPArray[i].dist == leastPrint){
                                cout << setw(18) << GPArray[i].name << setw(18) << GPArray[i].dist << setw(18) << GPArray[i].prev << endl;
                                printCnt++;
                                alreadyLeast.push_back(leastPrint);
                        }
                }
                leastPrint = INT_MAX;
		justInCaseCnt++;
        }
        for(int i=0; i<GPArray.size(); i++){
                if(GPArray[i].dist == -1){
                        cout << setw(18) << GPArray[i].name << setw(18) << "Not Connected" << setw(18) << GPArray[i].prev << endl;
                }
        }
}
