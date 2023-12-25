#include "util.h"

//fills needed spaces to print everything aligned
string fillSpaces(string line, int spaces){
    if(!spaces){
        return line;
    }
    for(int i = 0; i < spaces; i++){
        line += " ";
    }
    return line;
}

void gotoxy(int x, int y){
    move(y, x);
}

void printMenu(string menuOptions[], int optionCount){
    for(int i = 0; i <= optionCount; i++){
        printw("\n");
        printw("\t%s", menuOptions[i].c_str());
    }
}

void updateMenu(int option, int left, int right){
    gotoxy(left, option);
    printw("->");
    gotoxy(right, option);
    printw("<-");
}

void clearSelection(int start, int end, int left, int right){
    for(int i = start; i < end+1; i++){
        gotoxy(left, i);
        printw("  ");
        gotoxy(right, i);
        printw("  ");
    }
}

int checkIfSelected(const int selections[], int selectionsLength, int query){
    for(int i = 0; i < selectionsLength; i++){
        if(selections[i] == query){
            return i;
        }
    }

    return -1;
}
