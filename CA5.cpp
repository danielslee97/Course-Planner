#include "Graph.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

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
            while(finishedLine == false) {
                lineStream >> prereqs;
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
    //Iterate through the file
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
    //Iterate through the file
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
					//getOut = true;
					//break;
		}
                plannedSchedule.push_back(classes);
                classes = "";
                lineStream >> classes;
                //cout << "Class" << classes << endl;
            }
        }
        //if(getOut) return false;
    }
    return true;
    /*for(int i = 0; i < plannedSchedule.size(); i++){
        cout << plannedSchedule[i] << " ";
    }
    cout << endl;
    vector<string>::iterator it;
    for(it = plannedSchedule.begin(); it  != plannedSchedule.end(); it++) {
    	graph->checkCourse((*it), term);
        //cout << (*it) << endl;
    }*/
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

    return 0;
}
