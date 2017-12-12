#include "Graph.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <utility>
#include <functional>
#include <vector>

using namespace std;

/*
void swap(string *a, string *b){
    string t = *a;
    *a = *b;
    *b = t;
}

int partition(vector<string> &vec, int low, int high){
    string pivot = vec[high].substr(1,4);
    int index = (low - 1);
    for(int i = low; i <= high-1; i++){
        if(vec[i].substr(1,4) <= pivot){
            index++;
            swap(&vec[index], &vec[i]);
        }
    }
    swap(&vec[index + 1], &vec[high]);
    return index + 1;
}
void quicksort(vector<string> &vec, int low, int high){
    if(low < high){
        int part = partition(vec, low, high);
        
        quicksort(vec, low, part -1);
        quicksort(vec, part+1, high);
    }
}
*/
/*
=================================================================================================
*/
bool courseCheck(string course) {
    bool retBool = true;
    for(int i = 0; i < 2; i++) {
        if(!(isalpha(course[i]) && isupper(course[i]))) {
            retBool = false;
        }
    }
    for(int j = 2; j < 5; j++) {
        if(!(isdigit(course[j]))) {
            retBool = false;        
        }
    }
    return retBool;
}

void reqParse(Graph *graph, string inputFile) {
    ifstream fileStream(inputFile);
    string inputLine;
    int key = 1;
    //Iterate through the file
    while(getline(fileStream, inputLine)) {
        string identifier; //Either COURSE, TOTAL, CREDIT, or CHOOSE
        stringstream lineStream(inputLine);
        lineStream >> identifier;
        if(identifier == "COURSE") {
            //Ultimately want to push these somewhere else
            vector<string> prereqsList;
            string courseName;
            string option;
            string prereqs;
            bool finishedLine = false;

            lineStream >> courseName;
            if(!courseCheck(courseName)) {
                cout << "INVALID COURSE NAME FOR " << courseName << endl;
                return;
            }
            //cout << "COURSES:" << courseName << endl;
            lineStream >> option;
			lineStream >> prereqs;
            while(finishedLine == false) {
                if(prereqs == "") {
                    finishedLine = true;
                }
                else {
                    prereqsList.push_back(prereqs);
                    prereqs = "";
                    lineStream >> prereqs;
                }
            }
            /*cout << courseName << " " << option << " ";
            for(int i=0; i < prereqsList.size(); i++){
                cout << prereqsList[i] << " ";
            }
            cout << endl;*/
            graph->insert(courseName, option, prereqsList);
        }
        else if(identifier == "CHOOSE") {
            //Ultimately want to push these somewhere else
            vector<string> courseChoice;
            string numCoursesTaken;
            string course;
            bool finishedLine = false;

            lineStream >> numCoursesTaken;
            lineStream >> course;
            if(!courseCheck(course)) {
                cout << "INVALID COURSE NAME FOR " << course << endl;
                return;
            }
            while(finishedLine == false) {
                if(course == "") {
                    finishedLine = true;
                }
                else {
                    courseChoice.push_back(course);
                    course = "";
                    lineStream >> course;
                }
            }
            stringstream buf(numCoursesTaken);
            int min;
            buf >> min;
            graph->insertChoices(key, min, courseChoice);
            key++;
            /*cout << numCoursesTaken << " ";
            for(int i=0; i < courseChoice.size(); i++){
                cout << courseChoice[i] << " ";
            }
            cout << endl;*/
        }
        else if(identifier == "CREDIT") {
            //Ultimately want to push these somewhere else
            string creditType;
            string creditNum;
            int credits;

            lineStream >> creditType;
            lineStream >> creditNum;
            stringstream buf(creditNum);
            buf >> credits;

            //cout << creditType << " " << creditNum << endl;
            graph->insertCredits(creditType, credits);

        }
        else if(identifier == "TOTAL") {
            string totalCreds;

            lineStream >> totalCreds;

            cout << totalCreds << endl;
        }
        else {
            cout << "INVALID COMMAND" << endl;
            return;
        }
    }
}
void courseOfferingParser(Graph *graph, string filename) {
    ifstream fileStream(filename);
    string inputLine;
    vector<string> coursesOffered;
    
    while(getline(fileStream, inputLine)) {
        stringstream lineStream(inputLine);
        bool lineDone = false;

        string credits;
        string semester;
        string options;
        string course;
        lineStream >> course;
        //cout << "COURSE: " << course << endl;
        if(!courseCheck(course)) {
            cout << "INVALID COURSE NAME FOR " << course << endl;
            return;
        }
        
        //cout << "COURSES:" << courseName << endl;
        lineStream >> credits;
        stringstream buf(credits);
        int num;
        buf >> num;
        lineStream >> semester;
        lineStream >> options;
        //cout << options << endl;

        graph->insertCourses(course, options, semester, num);
        //cout << course << " " << credits << " " << semester << " " << options << endl << endl; 
    }
    
}
bool plannedScheduleParser(Graph *graph, string filename) {
    ifstream fileStream(filename);
    string inputLine;
    vector<string> plannedSchedule;
    bool getOut = false;
    vector<string> courseOrder;
    vector<string> sortedCourseOrder;
    vector<vector<string>> vectorOfVectors;
    //Iterate through the file
    while(getline(fileStream, inputLine)){
        //cout << inputLine << endl;
        courseOrder.push_back(inputLine);
    }
    /*
    cout << "Pre Sort: " << endl;
    for(int i =0; i < courseOrder.size(); i++){
        cout << courseOrder[i] << endl;
    }*/
    
    //Sort the vector
    sort(courseOrder.begin(), courseOrder.end());
	/*
    cout << "Post Sort: " << endl;
    for(int i =0; i < courseOrder.size(); i++){
        cout << courseOrder[i] << endl;
    }*/

    int insertLoc = courseOrder.size()/2;
    for(int i = 0; i < courseOrder.size()/2; i++){
        sortedCourseOrder.push_back(courseOrder[i]);
        sortedCourseOrder.push_back(courseOrder[insertLoc]);
        insertLoc++;
    }
	//Verifies if the semester S comes before F in the same years
	if(sortedCourseOrder.size() > 1){
		for(int i =0; i < sortedCourseOrder.size()-1; i++){
			string firstSem = sortedCourseOrder[i]; 
			char firstOp = firstSem[0];
			string firstYear = firstSem.substr(1,4);
			

			string secondSem = sortedCourseOrder[i+1];
			char secondOp = secondSem[0];
			string secondYear = secondSem.substr(1,4);
			//cout << secondOp << " " << secondYear << endl;
					
			if(secondYear == firstYear && secondOp > firstOp){
				string temp = sortedCourseOrder[i];
				sortedCourseOrder[i] = sortedCourseOrder[i + 1];
				sortedCourseOrder[i + 1] = temp;
				//swap first with second
			}
		}
	}
    /*cout << "Final Sort: " << endl;
    for(int i =0; i < sortedCourseOrder.size(); i++){
        cout << sortedCourseOrder[i] << endl;
    }*/

    for(int i = 0; i < sortedCourseOrder.size(); i++){
        vector<string> innerVector;
        stringstream lineStream(sortedCourseOrder[i]);
        string classes = "";
        bool lineDone = false;
        //cout << "here"
        string semester = sortedCourseOrder[i].substr(0,1); //Gets S or F
        //cout << semester << endl;
        innerVector.push_back(semester);
        lineStream >> classes;
        classes = "";
        
        lineStream >> classes;
        while(lineDone == false){ //Run this until the line is empty
            if(classes == ""){
                lineDone = true;
            }else{
                innerVector.push_back(classes);
                classes = "";
                lineStream >> classes;  
            }
        }
        vectorOfVectors.push_back(innerVector); //Push into the vector of vectors
    }
	for(int i = 0; i < vectorOfVectors.size(); i++) {
		if(!graph->BFS(vectorOfVectors[i])) {
			getOut = true;
			break;
		}
	}
//  for(int i = 0; i < vectorOfVectors.size(); i++) {
//      if(!graph->checkCourse(vectorOfVectors[i])) break;
//  }
    /*
    cout << "Post Complete Sort: " << endl;
    for(int i =0; i < sortedCourseOrder.size(); i++){
        cout << sortedCourseOrder[i] << endl;
    }*/
    
    //Iterate through the file
    /*
    while(getline(fileStream, inputLine)) {
        stringstream lineStream(inputLine);
        bool lineDone = false;

        string semester = inputLine.substr(0,1);
        //string term = "" + semester;
        string year = inputLine.substr(1,4);
        string classes; //This is just to get rid of the first part of the string
        lineStream >> classes;
        classes = ""; //Effectively reset the string
        
        lineStream >> classes;
        while(lineDone == false) {
            if(classes == "") {
                lineDone = true;
            }
            else {
                //cout << "COURSE: " << classes << endl;
                if(!courseCheck(classes)) {
                    cout << "INVALID COURSE NAME FOR " << classes << endl;
                    return false;
                }
                //cout << "TERM: " << semester << endl;
                if(!graph->checkCourse(classes, semester)) {
                    getOut = true;
                    break;
        }
                plannedSchedule.push_back(classes);
                classes = "";
                lineStream >> classes;
                //cout << "Class" << classes << endl;
            }
        }
        if(getOut) return false;
    }
    return true;
    */
    

    /*for(int i = 0; i < plannedSchedule.size(); i++){
        cout << plannedSchedule[i] << " ";
    }
    cout << endl;
    vector<string>::iterator it;
    for(it = plannedSchedule.begin(); it  != plannedSchedule.end(); it++) {
        graph->checkCourse((*it), term);
        //cout << (*it) << endl;
    }*/
	if(getOut) return false;
	return true;
}
bool checkFile(string filename) {
    ifstream infile(filename);  
    if(infile.good() != true) {
        return false;
    }
    else {
        return true;
    }
}
int main(int argc, char *argv[]) {
    //Declare the strings that will get the names of the files
    
    string reqFile = argv[1];
    string courseFile = argv[2];
    string planFile = argv[3];
/*
=====================================================================================================================
*/
    
    //This is all file input validation
    //Checks validity of the file
    ifstream infile1(reqFile);
    if(checkFile(reqFile) != true) {
        cout << "Please enter a valid requirements file." << endl << endl;
        return 1;
    }

    if(checkFile(courseFile) != true) {
        cout << "Please enter a valid course offering file." << endl << endl;
        return 1;
    }

    if(checkFile(planFile) != true) {
        cout << "Please enter a valid planned schedule file." << endl << endl;
        return 1;
    }
    

/*
=====================================================================================================================
*/
    
    
    Graph *graph = new Graph();
    reqParse(graph, reqFile);
    courseOfferingParser(graph, courseFile);
    if(plannedScheduleParser(graph, planFile)) graph->checkRequirements();
    
    //graph->print();
    
    //Graph *graph = new Graph();
    //plannedScheduleParser(graph, planFile);
    


    return 0;
}
