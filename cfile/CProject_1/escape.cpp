#include<iostream>
#include<vector>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<chrono>

using namespace std;

/*
Records Direction in English from the absolute location change of the node on the surface.
*/
char Direction[4] = {'N', 'S', 'W', 'E'};

/*
Error info if no path was found.
*/
string error_info = "Escape failed.";

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
    Point *next=NULL;
};

/*
Define a stack structure in terms of points.
LIFO struct.
*/
class Stack{
    public:
        Stack ();
        ~Stack ();
        bool Empty ();
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
        Queue ();
        ~Queue ();
        bool Empty ();
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
    head = new Point;
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
    front = new Point;
    rear = new Point;
    front -> next = rear;
};

Queue::~Queue (void){
    delete front;
    delete rear;
}

bool Queue::Empty (void) {
    if (front -> next == rear) {
        return true;
    }
    return false;
}

void Queue::Push(Point *p){
    rear -> col = p -> col;
    rear -> row = p -> row;
    rear -> next = new Point;
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
bool try_move(Maze maze, Point *to) {
    if (to->row < 0 || to->row >= maze.rows || to->col < 0 || to->col >= maze.cols){
        // cout<<"You are trying to move out of the maze-> Work has been interupted->"<<"("<<to[0]<<","<<to[1]<<")"<<endl;
        return false;
    }
    if (maze.room[to->row][to->col] == '.' || maze.room[to->row][to->col] == 'O' || maze.room[to->row][to->col] == 'T' || maze.room[to->row][to->col] == 'I') {
        return true;
    }
    else if (maze.room[to->row][to->col] == '@') {
        // cout << "You are trying to move into obstacle." << endl;
        return false;
    }
    else if (maze.room[to->row][to->col] == 'V') {
        // cout << "You are trying to visit somewhere you have visited." << endl;
        return false;
    }
    perror("Invalid input occurred in the maze. Have accessed as accessible room.");
    return false;
}

/*
This function is used to generate escape movement from 'O' point.
*/
char get_out(Maze maze, Point *end) {
    if (end->row == 0) return 'N';
    if (end->row == maze.rows-1) return 'S';
    if (end->col == 0) return 'W';
    if (end->col == maze.cols-1) return 'E';
    perror("Out is not on the edge, responds 'G' as substitution.");
    return 'G';
}

/*
This function will search start point to end point with DFS or BFS, and return the first path it finds.
*/
string search(Maze maze, Point *start, Point *end, string arg){
    // Test search function is correctly called.
    /*
    for(int i=0; i<maze.rows; i++){
        for(int j=0; j< maze.cols; j++){
            cout<<maze.room[i][j];
        }
        cout<<endl;
    }
    cout << maze.room[start->row][start->col] << endl;
    cout << maze.room[end->row][end->col] << endl;
    */

    /*
    Get ready to geenrate any path from one point to another. No matter it is I or O.
    The maze is M by N.
    */

    string method;

    // Memory path from start to get each accessible point.
    string path[maze.rows][maze.cols]={""};
    int inherit[maze.rows][maze.cols][2];
    // Initialize inherit to be some point inaccessible.
    for (int i=0; i<maze.rows; i++) {
        for (int j=0; j<maze.cols; j++) {
            inherit[i][j][0] = -1;
            inherit[i][j][1] = -1;
        }
    }

    // Remember the end. In case check way out.
    char END = maze.room[end->row][end->col];

    // Search start from start input by different methods.
    if (arg == "-Stack") {
        Stack waitinglist;
        waitinglist.Push(start);
        while (!waitinglist.Empty()) {
            // Get where I'm standing now.
            Point now = waitinglist.Pop();

            // Denote whether new points are available.
            bool next = false;

            // Records where I have visited.
            maze.room[now.row][now.col] = 'V';
        
            // Go potentially where I can go.
            Point *to = new Point;
            to->row = now.row - 1;
            to->col = now.col;
            if (try_move(maze, to)) {
                if (inherit[to->row][to->col][0] == -1) {
                    waitinglist.Push(to);
                    inherit[to->row][to->col][0] = now.row;
                    inherit[to->row][to->col][1] = now.col;
                    next = true;
                }
            }

            to = new Point;
            to->row = now.row + 1;
            to->col = now.col;
            if (try_move(maze, to)) {
                if (inherit[to->row][to->col][0] == -1) {
                    waitinglist.Push(to);
                    inherit[to->row][to->col][0] = now.row;
                    inherit[to->row][to->col][1] = now.col;
                    next = true;
                }
            }

            to = new Point;
            to->row = now.row;
            to->col = now.col - 1;
            if (try_move(maze, to)) {
                if (inherit[to->row][to->col][0] == -1) {
                    waitinglist.Push(to);
                    inherit[to->row][to->col][0] = now.row;
                    inherit[to->row][to->col][1] = now.col;
                    next = true;
                }
            }

            to = new Point;
            to->row = now.row;
            to->col = now.col + 1;
            if (try_move(maze, to)) {
                if (inherit[to->row][to->col][0] == -1) {
                    waitinglist.Push(to);
                    inherit[to->row][to->col][0] = now.row;
                    inherit[to->row][to->col][1] = now.col;
                    next = true;
                }
            }// If I can go to 'to', I push it into waitinglist and record the direction.
            
            // Formulate path with where I from.
            // Basically happens to starter.
            if (inherit[now.row][now.col][0]==-1) {
                continue;
            }

            // Calculate the direction from 'now'
            int delta_1 = (now.row - inherit[now.row][now.col][0] + 1) / 2;
            int delta_2 = ((now.col - inherit[now.row][now.col][1] + 1) / 2 + 2) * (1 - abs(now.row - inherit[now.row][now.col][0]));
            path[now.row][now.col] = path[inherit[now.row][now.col][0]][inherit[now.row][now.col][1]] + Direction[delta_1 + delta_2];

            // Test whether goes to right path. For debugging.
            // cout << "To get " << now.row << " " << now.col << " : " << method << endl;

            if (now.row==end->row && now.col==end->col) {
                break;
            }
        }
    }

    else if (arg == "-Queue") {
        Queue waitinglist;
        waitinglist.Push(start);

        while (!waitinglist.Empty()) {
            // Check if queue is correctly remembered.
            // waitinglist.Display();

            // Get where I'm standing now.
            Point now = waitinglist.Pop();

            // Denote whether new points are available.
            bool next = false;

            // Records where I have visited.
            maze.room[now.row][now.col] = 'V';

            // Test whether goes to right path.
            /*
            cout << "At" << now.row << " " << now.col << endl;
            cout << "To get there: "<< method << endl;
            */

            // Go potentially where I can go.
            Point *to = new Point;
            to->row = now.row - 1;
            to->col = now.col;
            if (try_move(maze, to)) {
                if (inherit[to->row][to->col][0] == -1) {
                    waitinglist.Push(to);
                    inherit[to->row][to->col][0] = now.row;
                    inherit[to->row][to->col][1] = now.col;
                    next = true;
                }
            }

            to = new Point;
            to->row = now.row + 1;
            to->col = now.col;
            if (try_move(maze, to)) {
                if (inherit[to->row][to->col][0] == -1) {
                    waitinglist.Push(to);
                    inherit[to->row][to->col][0] = now.row;
                    inherit[to->row][to->col][1] = now.col;
                    next = true;
                }
            }

            to = new Point;
            to->row = now.row;
            to->col = now.col - 1;
            if (try_move(maze, to)) {
                if (inherit[to->row][to->col][0] == -1) {
                    waitinglist.Push(to);
                    inherit[to->row][to->col][0] = now.row;
                    inherit[to->row][to->col][1] = now.col;
                    next = true;
                }
            }

            to = new Point;
            to->row = now.row;
            to->col = now.col + 1;
            if (try_move(maze, to)) {
                if (inherit[to->row][to->col][0] == -1) {
                    waitinglist.Push(to);
                    inherit[to->row][to->col][0] = now.row;
                    inherit[to->row][to->col][1] = now.col;
                    next = true;
                }
            }// If I can go to 'to', I push it into waitinglist and record the direction.
            
            // Formulate path with where I from.
            // Basically happens to starter.
            if (inherit[now.row][now.col][0]==-1) {
                continue;
            }

            // Calculate the direction from 'now'
            int delta_1 = (now.row - inherit[now.row][now.col][0] + 1) / 2;
            int delta_2 = ((now.col - inherit[now.row][now.col][1] + 1) / 2 + 2) * (1 - abs(now.row - inherit[now.row][now.col][0]));
            path[now.row][now.col] = path[inherit[now.row][now.col][0]][inherit[now.row][now.col][1]] + Direction[delta_1 + delta_2];

            // End condition: pop out the end point.
            if (now.row==end->row && now.col==end->col) {
                break;
            }

            // For debugging.
            // cout<<method<<endl;
        }
    }
    method = path[end->row][end->col];
    if (END == 'O'){
        method.push_back(get_out(maze, end));
    }
    maze.room[end->row][end->col] = END; // Recover the end for identify tunnel.
    return method;
}

/*
The function is still complicated with readin and processing.
A single step separating readin maze as (maze, start, end) will solve it.
Just throw for loop into search func will do.
*/
int readin(string arg){
    // Get the number first.
    int N;
    cin >> N;
    // cout << "Gonna Process " << N << " Mazes" << endl;

    /*
    This part reads the maze file line by line.
    */
    Point *start = new Point;
    Point *end = new Point;
    
    // If the number N is not given, use while.
    for (int cnt = 0; cnt < N; cnt++){
        // Initialize an unaccessible point.
        end->row = -1;

        // cin each maze's size info.
        Maze maze;
        cin >> maze.rows >> maze.cols;
        maze.room = (char **)malloc(sizeof(char *) * maze.rows);
        for (int i = 0; i < maze.rows; i++) {
            maze.room[i] = (char *)malloc(sizeof(char)*maze.cols);
        }

        for (int i = 0; i < maze.rows; i++){
            for (int j = 0; j < maze.cols; j++) {
                char tmp;
                cin >> tmp;
                if (tmp == 'I') {
                    start->row = i;
                    start->col = j;
                }
                else if (tmp == 'T') {
                    end->row = i;
                    end->col = j;
                }
                else if (tmp == 'O' && end->row == -1) {
                    end->row = i;
                    end->col = j;
                }
                maze.room[i][j] = tmp;
            }
        }

        // Check and search a way out.
        string ans = search(maze, start, end, arg);
        if (ans.size() == 0) {
            cout << error_info;
        }
        else {
            cout << ans;
        }
        cout<<endl;

        // Abnormal dungeon process early stopping.
        if (cnt < N - 1 && maze.room[end->row][end->col] == 'T') {
            cout << "Early escape! Put him back to dungeon: " << endl;
            
        }
        if (cnt == N - 1 && maze.room[end->row][end->col] != 'T') {
            cout << "You have go to the end without escaping. Failed escape." << endl;
        }
    }
    return 0;
}

string check_arg(int argc, char *argv[]){
    if (argc!= 2) {
        cout << "Wrong arguments number: " << argv[0] << endl;
        exit(1);
    }
    if (argv[1] == string("-Stack") || argv[1] == string("-Queue")){
        return argv[1];
    }
    else {
        cout << "Wrong arguments: " << argv[1] << ", should be -Stack/-Queue" << endl;
        exit(1);
    }
}

int main(int argc, char *argv[]){

    // This part is for debugging.
/*
int main(){
    int argc = 2;
    char *argv[] = {(char *) "-Queue", (char *) "-Queue"};
*/    

    // Timer part.
    auto start_time = chrono::steady_clock::now();

    string arg = check_arg(argc, argv);

    int status;
    status = readin(arg);
    if (status == 0) {
        // cout << "Soln Success" << endl;
    }
    else if (status == -1) {
        cout << "Soln Failed Somewhere" << endl;
    }

    auto end_time = chrono::steady_clock::now();
    auto running_time = std::chrono::duration<double, std::milli>(end_time - start_time).count();
    cout<<"Program Finished in "<<running_time<<"ms"<<endl;
}

