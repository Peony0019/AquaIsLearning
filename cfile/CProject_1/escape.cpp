#include<iostream>
#include<vector>
#include<string.h>
#include<stdio.h>
#include<chrono>

using namespace std;

/*
Records Direction in English from the absolute location change of the node on the surface.
*/
char Direction[4] = {'N', 'S', 'W', 'E'};

/*
Error info if no path was found.
*/
string error_info = "Escape failed.\n";

/*
Define a specific structure to denote a maze.
*/
struct Maze{
    int rows, cols;
    char **room;
};

/*
Define 2-len array for recording positions.
Only linked to one next point.
*/
struct Point{
    int row, col;
    Point *next;
};

/*
Define a stack structure in terms of points.
LIFO struct.
*/
class Stack{
    public:
        Stack () {};
        ~Stack () {};
        bool Empty () {};
        void Push(Point *p);
        Point Pop();
        void Display();

    private:
        Point *head;
};

/*
Define a queue structure in terms of points.
FIFO struct.
*/
class Queue{
    public:
        Queue () {} ;
        ~Queue () {};
        bool Empty () {};
        void Push(Point *p);
        Point Pop();
        void Display();

    private:
        Point *front;
        Point *rear;
};

/*----------------------------------------------------------------
This Part is mainly defining basic functions Stack requires.
----------------------------------------------------------------*/

Stack::Stack (void){
    head = NULL;
};

Stack::~Stack (void){
    delete head;
}

bool Stack::Empty (void) {
    if (head -> next == NULL) {
        return true;
    }
    return false;
}

void Stack::Push(Point *p){
    p -> next = head -> next;
    head -> next = p;
}

Point Stack::Pop(){
    if (Empty()){
        throw("Stack is empty. Work has been interupted.");
    }
    Point *p = head -> next;
    head -> next = p -> next;
    return *p;
}

void Stack::Display(){
    Point *p = head;
    printf("Displays as follows:\n");
    while (p -> next != NULL){
        p = p -> next;
        printf("(%d, %d) \n", p -> row, p -> col);
    }
}

/*----------------------------------------------------------------
This Part is mainly defining basic functions Queue requires.
----------------------------------------------------------------*/

Queue::Queue (void){
    front = NULL;
    rear = NULL;
    front -> next = rear;
};

Queue::~Queue (void){
    delete front;
    delete rear;
}

bool Queue::Empty (void) {
    if (front -> next == NULL) {
        return true;
    }
    return false;
}

void Queue::Push(Point *p){
    rear = p;
    rear -> next = NULL;
    rear = rear -> next;
}

Point Queue::Pop(){
    if (Empty()){
        throw("Queue is empty. Work has been interupted.");
    }
    Point *p = front -> next;
    front -> next = p -> next;
    return *p;
}

void Queue::Display(){
    Point *p = front;
    printf("Displays as follows:\n");
    while (p -> next != NULL){
        p = p -> next;
        printf("(%d, %d) \n", p -> row, p -> col);
    }
}

/*----------------------------------------------------------------
This Part tries to figure out the required path, the detailed
algorithms are in search_Stack, search_Queue, search_STL.
----------------------------------------------------------------*/

/*
This function intends to try out a potential move to 'to'.
*/
bool try_move(Maze maze, Maze visited, Point to) {
    if (to.row < 0 || to.row >= maze.rows || to.col < 0 || to.col >= maze.cols){
        // cout<<"You are trying to move out of the maze. Work has been interupted."<<"("<<to[0]<<","<<to[1]<<")"<<endl;
        return false;
    }
    if (maze.room[to.row][to.col] != '@' && !visited.room[to.row][to.col]) {
        return true;
    }
    return false;
}

/*
This function will search start point to end point with DFS, and return the first path it finds.
*/
string search(vector<vector<char>> maze, vector<int> start, vector<int> end){
    // Test search function is correctly called.
    /*
    for(int i=0; i<maze.size(); i++){
        for(int j=0; j< maze[i].size(); j++){
            cout<<maze[i][j];
        }
        cout<<endl;
    }
    cout << maze[start[0]][start[1]] << endl;
    cout << maze[end[0]][end[1]] << endl;
    */
    

    if (start.size() != 2){
        perror("You must input a 2-len vector as starter.");
    }
    if (end.size() != 2){
        perror("You must input a 2-len vector as ender.");
    }
    /*
    Get ready to geenrate any path from one point to another. No matter it is I or O.
    The maze is M by N.
    */
    int M = maze.size();
    int N = maze[0].size();

    vector<int> from(2);
    vector<char> method;
    vector<vector<int>> waitinglist;
    vector<vector<bool>> visited(M, vector<bool>(N));

    // Initialize 'from' as somewhere cannot be visited.
    from[0] = from[1] = -1;

    // Search start from start input.
    waitinglist.push_back(start);
    while (waitinglist.size()>0) {
        // Get where I'm standing now.
        vector<int> to(2);
        vector<int> now = waitinglist.back();
        waitinglist.pop_back();

        // Denote whether new points are available.
        bool next = false;

        // Records where I have visited.
        visited[now[0]][now[1]] = true;

        // Go potentially where I can go.
        vector<vector<int>> potential;
        to[0] = now[0] - 1;
        to[1] = now[1];
        potential.push_back(to);
        to[0] = now[0] + 1;
        to[1] = now[1];
        potential.push_back(to);
        to[0] = now[0];
        to[1] = now[1] - 1;
        potential.push_back(to);
        to[0] = now[0];
        to[1] = now[1] + 1;
        potential.push_back(to);

        for (int k=0; k<4; k++){
            if (try_move(maze, visited, potential[k])){
                waitinglist.push_back(potential[k]);
                next = true;
            }
        } // If I can go to potential[k], I push it into waitinglist and record the direction.

        // End condition: pop out the end point.
        if (now[0]==end[0] && now[1]==end[1]) {
            int delta_1 = (now[0] - from[0] + 1) / 2;
            int delta_2 = ((now[1] - from[1] + 1) / 2 + 2) * (1 - abs(now[0] - from[0]));
            method.push_back(Direction[delta_1 + delta_2]);
            break;
        }

        // Formulate path with where I from.
        if (from[0]==-1 && from[1]==-1) {
            from = now;
            continue;
        }
        else if (!next) {
            method.pop_back();
        }
        else {
            int delta_1 = (now[0] - from[0] + 1) / 2;
            int delta_2 = ((now[1] - from[1] + 1) / 2 + 2) * (1 - abs(now[0] - from[0]));
            method.push_back(Direction[delta_1 + delta_2]);
        }
        from = now;
    }
    return method;
}

/*
The function is still complicated with readin and processing.
Intend to do readin single maze here. But for each maze, begin with last \n and end with \n. 
The last one end with EOF.
*/
int readin(string file){
    // Definition part
    vector<vector<char>> maze;

    // If IO is realized by ifstream / ostream.
    FILE *fp;
    char ch;
    fp = fopen(file.c_str(), "r");
    if (fp == NULL){
        cout << "Error opening file." << endl;
        return -1;
    }
    //Get the number first.
    int N=0;
    while((ch=fgetc(fp))!='\n'){
        int t = int(ch) - 48;
        N = N * 10 + t;
    }
    // cout << "Gonna Process " << N << " Mazes" << endl;

    /*
    This part reads the maze file line by line.
    */
    vector<int> start;
    vector<int> end;
    vector<char> row;
    //If the number N is not given, use while like this.
    while ((ch = fgetc(fp))!= EOF){
        if (ch == ' ') {
            continue;
        }
        else if (ch == '\n') {
            char ci = fgetc(fp);
            // Once the program went through one maze, solve it.
            maze.push_back(row);
            row.clear();
            if (ci == '\n') {
                vector<char> ans = search(maze, start, end);
                maze.clear();
                if (ans.size() == 0) {
                    cout << error_info;
                } // If no path was found.
                else
                for(int k=0; k<ans.size(); k++){
                    cout<<ans[k];
                }cout<<endl;
                start.clear();
                end.clear();
            }
            else {
                ungetc(ci, fp);
            }
        }
        else {
            // Before push in the node, records Input and Output point first.
            if (ch == 'I') {
                start.push_back(maze.size());
                start.push_back(row.size());
            }
            if (ch == 'O' || ch == 'T') {
                end.push_back(maze.size());
                end.push_back(row.size());
            }
            row.push_back(ch);
        }
    }
    // Process if the file ends without inputting the last maze.
    if (maze.size() > 0) {
        if (row.size() > 0){
            maze.push_back(row);
            row.clear();
        }
        vector<char> ans = search(maze, start, end);
        if (ans.size() == 0) {
            cout << error_info;
        }
        else
        for (int k=0; k<ans.size(); k++) {
            cout<<ans[k];
        }cout<<endl;
    }
    fclose(fp);
    return 0;
}

int main(){
    // Timer part.
    auto start_time = chrono::steady_clock::now();

    int status;
    string file="example";
    status = readin(file);
    if (status == 0) {
        cout << "Soln Success" << endl;
    }
    else if (status == -1) {
        cout << "Soln Failed Somewhere" << endl;
    }

    auto end_time = chrono::steady_clock::now();
    auto running_time = std::chrono::duration<double, std::micro>(end_time - start_time).count();
    cout<<"Program Finished in "<<running_time<<"μs"<<endl;
}

