#include <iostream>
#include <vector>
using namespace std;
int n;
vector<int> queens;

struct conflicts{
    vector<int> rows;
    vector<int> columns;
    vector<int> mainDiagonals;
    vector<int> secondaryDiagonals;
};

conflicts queenConflicts;

int chooseQueenWithMaxConflicts(){
    int maxConf = 0, queenNumber = 0;
    for(int i = 0; i < n; i++){
        if(conflictsNumber[i].mainDiagonal > maxConf){
            maxConf = conflictsNumber[i].mainDiagonal;
            queenNumber = i;
        }
    }
    return queenNumber;
}

int chooseRowWithMinConflicts(){
    int maxConf = 0, rowNumber = 0;
    for(int i = 0; i < n; i++){
        if(conflictsNumber[i].mainDiagonal > maxConf){
            maxConf = conflictsNumber[i].mainDiagonal;
            queenNumber = i;
        }
    }
    return queenNumber;
}

void solution(){
    int MaxConfqueen = chooseQueenWithMaxConflicts();
    int minConfRoll = chooseRowWithMinConflicts(); 
}


int main(){
    int n;
    cin>>n;
    queens.resize(n, 0);
    queenConflicts.rows.resize(n);
    queenConflicts.columns.resize(n);
    queenConflicts.mainDiagonals.resize(n * 2 - 1);
    queenConflicts.secondaryDiagonals.resize(n * 2 - 1);

    for(int i = 0; i < n; i++){
        queenConflicts.columns[i] = 1;
    }

    solution();

    return 0;
}