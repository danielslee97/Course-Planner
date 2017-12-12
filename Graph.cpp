#include "Graph.h"
#include <iostream>

Graph::Graph() {
	this->terms.insert(pair<string, term>("E", E));
	this->terms.insert(pair<string, term>("S", S));
	this->terms.insert(pair<string, term>("F", F));
	
	this->tagMap.insert(pair<string, tags>("M", M));
	this->tagMap.insert(pair<string, tags>("R", R));
	this->tagMap.insert(pair<string, tags>("O", O));
	this->tagMap.insert(pair<string, tags>("NA", NA));
	
	this->error = false;
}
bool Graph::insert(string name, string tag, vector<string> prereqs) {
	if(this->vertices.find(name) == this->vertices.end()) {
		Node *n = new Node;
		n->name = name;
		n->choice = 0;
		n->tag = this->tagMap[tag];
		n->color = white;
		/*if(prereqs.size() != 0) {
			for(string course: prereqs) {
				if(this->vertices.find(course) == this->vertices.end()) {
					Node *p = new Node;
					p->name = course;
					this->vertices.insert(pair<string, Node*>(course, p));
					
					n->prereqs.push_back(p);
					this->vertices[course]->postreqs.push_back(n);
				}
				else {
					n->prereqs.push_back(this->vertices[course]);
					this->vertices[course]->postreqs.push_back(n);
				}
			}
		}*/
		this->vertices.insert(pair<string, Node*>(name, n));
	}
	else {
		this->vertices[name]->choice = 0;
		this->vertices[name]->tag = this->tagMap[tag];
		this->vertices[name]->color = white;
	}
	if(prereqs.size() != 0) {
		for(string course: prereqs) {
			if(this->vertices.find(course) == this->vertices.end()) {
				Node *p = new Node;
				p->name = course;
				p->choice = 0;
				p->tag = NA;
				p->color = white;
				this->vertices.insert(pair<string, Node*>(course, p));
					
				this->vertices[name]->prereqs.push_back(p);
				this->vertices[course]->postreqs.push_back(this->vertices[name]);
			}
			else {
				this->vertices[name]->prereqs.push_back(this->vertices[course]);
				this->vertices[course]->postreqs.push_back(this->vertices[name]);
			}
		}
	}
	return true;
}
bool Graph::insertCourses(string name, string option, string availability, int credits) {
	if(this->vertices.find(name) == this->vertices.end()) {
		Node *n = new Node;
		n->name = name;
		n->choice = 0;
		n->tag = NA;
		if(option == "") n->option = "N/A";
		else n->option = option;
		n->availability = terms[availability];
		n->credits = credits;
		n->color = white;
		this->vertices.insert(pair<string, Node*>(name, n));
	}
	else {
		if(option == "") this->vertices[name]->option = "N/A";
		else this->vertices[name]->option = option;
		this->vertices[name]->availability = this->terms[availability];
		this->vertices[name]->credits = credits;
	}
	return true;
}
bool Graph::insertChoices(int key, int min, vector<string> choices) {
	if(this->choices.find(key) != this->choices.end()) {
		cout << "ERROR: DUPLICATE KEY" << endl;
		return false;
	}
	this->choices.insert(pair<int, int>(key, min));
	this->choiceCourses.insert(pair<int, vector<string>>(key, choices));
	for(string course: choices) {
		//cout << "COURSESSSSSS: " << course << endl;
		if(this->vertices.find(course) != this->vertices.end()) {
			this->vertices[course]->choice = key;
		}
		else {
			vector<string> vec;
			insert(course, "NA", vec);
			this->vertices[course]->choice = key;
		}
	}
	return true;
}
bool Graph::insertCredits(string option, int credits) {
	if(this->addCredits.find(option) == this->addCredits.end()) {
		this->optionChars.push_back(option);
		this->addCredits.insert(pair<string, int>(option, credits));
		this->satisfiedCredits.insert(pair<string, int>(option, 0));
		return true;
	}
	return false; //if there are duplicate options
}
void Graph::checkError() {
	if(!this->error) {
		cout << "BAD PLAN. HERES WHY:" << endl;
	}
}	
bool Graph::BFS(vector<string> courses) {
	string semester = courses[0];
	for(int i = 1; i < courses.size(); i++) {
		string course = courses[i];
		if(this->vertices.find(course) == this->vertices.end()) {
			checkError();
			cout << "ERROR: COURSE " << course << " CANNOT BE FOUND IN DATABASE" << endl;
			this->error = true;
			return false;
		}
		if(this->vertices[course]->availability != E && this->vertices[course]->availability != this->terms[semester]) {
			checkError();
			cout << course << " CAN ONLY BE TAKEN";
			switch(this->vertices[course]->availability) {
				case S:
					cout << " DURING SPRING SEMESTER" << endl;
					break;
				case F:
					cout << " DURING FALL SEMESTER" << endl;
					break;
			}
			this->error = true;
			//return false;
		}
		if(this->vertices[course]->prereqs.size() == 0) { //no prereqs to satisify
			this->vertices[course]->color = grey;
		}
		else {
			for(Node *n: this->vertices[course]->prereqs) {
				//cout << "CURRENT COURSE: " << course << endl;
				for(int i = 1; i < courses.size(); i++) {
					//cout << n->name <<  " CHECK FOR THIS: " << courses[i] << endl;
					if(n->name == courses[i]) {
						if(courses[i] == course) {
							this->vertices[course]->color = black;
						}
						else {
							checkError();
							cout << "CANNOT TAKE COURSE: " << course << " CONCURRENTLY WITH PREREQ: " << n->name << endl;
							this->vertices[course]->color = grey;
							n->color = grey;
							this->error = true;
							//return false;
						}
					}
				}
				//cout << endl;
				if(n->name == this->vertices[course]->name) {
					this->vertices[course]->color = black;
				}
				else if(n->color == white) {
					checkError();
					cout << "NEED PREREQ: " << n->name << " FOR COURSE: " << course << endl;
					this->vertices[course]->color = grey;
					this->error = true;
					//return false;
				}
			}
			this->vertices[course]->color = grey;
			for(Node *n: this->vertices[course]->prereqs) n->color = black;
		}
		if(this->vertices[course]->option.size() == 0) {
			if(this->satisfiedCredits.find(this->vertices[course]->option) != this->satisfiedCredits.end()) {
				this->satisfiedCredits[this->vertices[course]->option] += this->vertices[course]->credits; //adds credits to satisfied credits mapped to specified option 
			}
		}
		else {
			for(int i = 0; i < this->vertices[course]->option.size(); i++) {
				if(this->satisfiedCredits.find(this->vertices[course]->option.substr(i, 1)) != this->satisfiedCredits.end()) {
					//cout << '\n' << this->vertices[course]->option.substr(i, 1) << endl;
					this->satisfiedCredits[this->vertices[course]->option.substr(i, 1)] += this->vertices[course]->credits; //adds credits to satisfied credits mapped to specified option 
				}
			}
		}
	}
	return true;
}
/*void Graph::printErrors() {
	if(this->unsatisfiedReqs.size() == 0) {
		cout << "NO ERRORS" << endl;
	}
	for(string course: this->courseErrors) {
		cout << "CANNOT TAKE COURSE: " << course << endl;
		cout << "NEED PREREQ COURSES: ";
		for(string prereq: this->unsatisfiedReqs[course]) { //prints unsatisifed prereqs
			cout << prereq << " ";
		}
		cout << endl;
	}
	
	if(this->optionChars.size() != 0) {
		for(string ch: optionChars) {
			if(this->addCredits[ch] > satisfiedCredits[ch]) {
				int need = this->addCredits[ch] - satisfiedCredits[ch];
				cout << "DID NOT SATISFY " << ch << ". NEED " << need << " MORE CREDITS" << endl;
			}
		}
	}
}*/
bool Graph::setCredit(string course, int credits) {
	if(this->vertices.find(course) == this->vertices.end()) return false;
	this->vertices[course]->credits = credits;
	return true;
}
bool Graph::setAvailability(string course, string availability) {
	if(this->vertices.find(course) == this->vertices.end()) return false;
	this->vertices[course]->availability = this->terms[availability];
	return true;
}
bool Graph::setOption(string course, string option) {
	if(this->vertices.find(course) == this->vertices.end()) return false;
	if(option == "") this->vertices[course]->option = "N/A";
	else this->vertices[course]->option = option;
	return true;
}
void Graph::print() {
	unordered_map<string, Node*>::iterator it;
	for(it = this->vertices.begin(); it != this->vertices.end(); it++) {
		cout << "COURSE NAME: " << (*it).second->name << endl;
		cout << "COURSE CHOICE: " << (*it).second->choice << endl;
		cout << "COURSE TAG: " << (*it).second->tag << endl;
		cout << "COURSE OPTION: " << (*it).second->option << endl;
		cout << "COURSE TERM: " << (*it).second->availability << endl;
		cout << "COURSE CREDITS: " << (*it).second->credits << endl;
		cout << "COURSE COLOR: " << (*it).second->color << endl;
		
		cout << "PREREQS FOR THIS COURSE: ";
		if((*it).second->prereqs.size() != 0) {
			for(int i = 0; i < (*it).second->prereqs.size(); i++) {
				cout << (*it).second->prereqs[i]->name << " ";
			}
		}
		else {
			cout << "NONE";
		}
		cout << endl;
		cout << endl;
		//cout << "NUMBER OF COURSES INCLUDING PREREQ: " << this->vertices.size() << endl;
		cout << endl;
	}
}
void Graph::checkRequirements() {
	bool satisfied = true;
	if(this->optionChars.size() != 0) {
		for(string ch: optionChars) {
			if(this->addCredits[ch] > satisfiedCredits[ch]) {
				int need = this->addCredits[ch] - satisfiedCredits[ch];
				checkError();
				cout << "DID NOT SATISFY CREDIT " << ch << ". NEED " << need << " MORE CREDITS" << endl;
				satisfied = false;
			}
		}
	}
	unordered_map<int, int> compareChoices;
	if(this->choices.size() != 0) {
		unordered_map<int, int>::iterator it;
		for(it = this->choices.begin(); it != this->choices.end(); it++) {
			compareChoices.insert(pair<int, int>((*it).first, 0));
		}
	}
	if(this->vertices.size() != 0) {
		unordered_map<string, Node*>::iterator it;
		for(it = this->vertices.begin(); it != this->vertices.end(); it++) {
			Node *temp = (*it).second;
			if(temp->color == white && (temp->tag == M || temp->tag == R)) {
				checkError();
				cout << "NEED COURSE " << temp->name << endl;
				this->error = true;
				satisfied = false;
				//cout << "TAG: " << temp->tag << endl;
				//return false;
			}
			if(temp->choice != 0 && (temp->color == grey || temp->color == black)) {
				compareChoices[temp->choice]++;
			}
		}
	}
	unordered_map<int, int>::iterator it;
	for(it = this->choices.begin(); it != this->choices.end(); it++) {
		if((*it).second > compareChoices[(*it).first]) {
			int difference = (*it).second - compareChoices[(*it).first];
			checkError();
			cout << "NEED(s) " << difference << " ADDITIONAL COURSE(s): ";
			for(string course: this->choiceCourses[(*it).first]) {
				if(this->vertices[course]->color == white) {
					cout << course << " ";
				}
			}
			cout << endl;
			satisfied = false;
		}
	}
	if(satisfied && !this->error) {
		cout << "GOOD PLAN. GET TO WORK." << endl;
	}
}















