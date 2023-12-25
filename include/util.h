#ifndef MOTOGP_FANTASY_LEAGUE_UTIL_H
#define MOTOGP_FANTASY_LEAGUE_UTIL_H

#include <iostream>
#include <ncurses.h>

using namespace std;

string fillSpaces(string line, int spaces);
void gotoxy(int x, int y);
void printMenu(string menuOptions[], int optionCount);
void updateMenu(int option, int left, int right);
void clearSelection(int start, int end, int left, int right);
int checkIfSelected(const int selections[], int selectionsLength, int query);

#endif //MOTOGP_FANTASY_LEAGUE_UTIL_H
