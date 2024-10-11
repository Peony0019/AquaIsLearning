#include<iostream>
#include<vector>
#include<string.h>
#include<stdio.h>

using namespace std;

vector<char> search(vector<vector<char>> maze){
    vector<char> method;
    
    // Test search function is correctly called.
    /*
    for(int i=0; i<maze.size(); i++){
        for(int j=0; j< maze[i].size(); j++){
            cout<<maze[i][j];
        }
        cout<<endl;
    }
    */
    
    vector<vector<int>> visited(maze.size(), vector<int>(maze[0].size()));
    // Start from the top-left corner and go right, down, left, up.

    


    return method;
}

int readin(string file){
    /*
    The function is still complicated with readin and processing.
    */
    vector<vector<char>> maze;
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
    cout << "Gonna Process " << N << " Mazes" << endl;

    /*
    This part reads the maze file line by line.
    */

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
            row = vector<char> (row.size());
            if (ci == '\n') {
                vector<char> ans = search(maze);
                maze = vector<vector<char>> (0);
                for(int k=0; k<ans.size(); k++){
                    cout<<ans[k];
                }cout<<endl;
            }
            else {
                ungetc(ci, fp);
            }
        }
        else {
            row.push_back(ch);
        }
    }
    // Process if the file ends without inputting the last maze.
    if (maze.size() > 0) {
        if (row.size() > 0){
            maze.push_back(row);
            row = vector<char> (row.size());
        }
        search(maze);
    }
    fclose(fp);
    return 0;
}
/*
Intend to do readin single maze here. But for each maze, begin with last \n and end with \n. 
The last one end with EOF.
*/


int main(){
    int status;
    string file="example";
    status = readin(file);
    if (status == 0) {
        cout << "Success" << endl;
    }
    else if (status == -1) {
        cout << "Failed Somewhere" << endl;
    }
}

