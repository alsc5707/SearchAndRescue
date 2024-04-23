#include "SearchAndRescue.hpp"
#include <vector>
#include <stack>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;


SearchAndRescue::SearchAndRescue(string fileName){
    terrain = new int*[4];
    string line;
    ifstream infile(fileName);
    if (infile.is_open()){
        for(int i = 0; getline(infile, line); i++){   
            terrain[i] = new int[4];
            stringstream ss(line);
            string s;
            for (int j = 0; getline(ss, s, ' '); j++){
                terrain[i][j] = stoi(s);
            }
        }
    }
    path = vector<State*>();
}


SearchAndRescue::~SearchAndRescue(){
    State* to_del = path.back();
    delete to_del;
    path.pop_back();

    for (int i = 0; i < 4; i++){
        delete terrain[i];
    }
    delete terrain;
}


void SearchAndRescue::possibleActions(State* current){
    if(current->y <3){
current->possible_actions.push_back("up");
    }
    if(current->y > 0){
current->possible_actions.push_back("down");
    }
    if(current->x > 0){
current->possible_actions.push_back("left");
    }
   if(current->x < 3){
current->possible_actions.push_back("right");
   }
}

State* SearchAndRescue::result(State* current, string action){
    State* newState = new State{current->x, current->y, current->saved_people, action, vector<string>()};
    if(action == "right"){
        newState->x = newState->x + 1;
    } else if(action == "left"){
        newState->x = newState->x - 1;
    } else if(action == "up"){
        newState->y = newState->y + 1;
    } else{
        newState->y = newState->y - 1;
    }
    return newState;
}


vector<State*> SearchAndRescue::expand(State* current){
    possibleActions(current);
    vector<State*> expansion = vector<State*>();
    for(unsigned int i = 0; i < current->possible_actions.size(); i++){
        State* toAdd = result(current,current->possible_actions[i]);
        expansion.push_back(toAdd);
    }
    return expansion;
}

bool SearchAndRescue::iterativeDeepeningWrapper(State* start){
    int depth = STARTING_DEPTH;
    while (true) {
        path.clear(); 
        path.push_back(start); 
        if (iterativeDeepeningSearch(start, depth)) {
            return true; 
        }
        depth++;
    }
    return false;
    
}


bool SearchAndRescue::iterativeDeepeningSearch(State* current, int depth_limit){
   if (depth_limit == 0) {
        return false; 
    }
    if (isGoal(current)) {
        return true; 
    }
    bool people = false;
    if (terrain[current->x][current->y] == 2) {
        current->saved_people++;
        terrain[current->x][current->y] = 1;
        people = true;
    }
    vector<State*> expansion = expand(current);
    for (int i = 0; i < expansion.size(); i++) {
        path.push_back(expansion[i]); 
        if (iterativeDeepeningSearch(expansion[i], depth_limit - 1)) {
            return true;
        } else{
            State *toDelete = expansion[i];
            delete toDelete;
            path.pop_back();
            
        }
    }
    if (people) {
        current->saved_people--;
        terrain[current->x][current->y] = 2;
    }
    return false; 
}

void SearchAndRescue::printPath(){
    for (int i = 0; i < path.size(); i++) {
        cout << "x: " << path[i]->x << "\ty: " << path[i]->y << "\tprev_action: " << path[i]->prev_action << endl;
    }
}

bool SearchAndRescue::isGoal(State* current){
    if(current->x == 3 && current->y == 3 && current->saved_people == PEOPLE_TO_SAVE){
        return true;
    }
    return false;
}

void SearchAndRescue::printTerrain(){
    for (int i = 3; i >=0; --i) {
        for (int j = 0; j < 4; ++j) {
            cout << terrain[j][i];
            cout << " ";
        }
        cout << endl;
    }
}
