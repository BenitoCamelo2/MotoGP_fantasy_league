#include "rider.h"

Rider::Rider() {
    firstName = "";
    lastName = "";
    number = "";
    country = "";
    team = "";
    points = 0;
}

bool Rider::setData(string &firstName, string &lastName, string &number, string &country, string &team, int &points) {
    this->firstName = firstName;
    this->lastName = lastName;
    this->number = number;
    this->country = country;
    this->team = team;
    this->points = points;
    return true;
}

void Rider::setNumber(string &number) {
    this->number = number;
}

void Rider::setPoints(int &points) {
    this->points = points;
}

string Rider::getFirstName() {
    return firstName;
}

string Rider::getLastName(){
    return lastName;
}

string Rider::getNumber() {
    return number;
}

string Rider::getCountry() {
    return country;
}

string Rider::getTeam() {
    return team;
}

int Rider::getPoints() {
    return points;
}

//toString is used to print the rider to the screen
string Rider::toString() {
    string result;
    string name = firstName + " " + lastName;

    result += fillSpaces(number, SPACE_NUMBER - number.length());
    result += "|";
    result += fillSpaces(name, SPACE_NAME - name.length());
    result += "|";
    result += fillSpaces(to_string(points), SPACE_POINTS - to_string(points).length());
    result += "|";
    result += fillSpaces(country, SPACE_COUNTRY - country.length());
    result += "|";
    result += fillSpaces(team, SPACE_TEAM - team.length());
    result += "|";
    return result;
}

string Rider::toStringIndexed(int index){
    string result;
    string name = firstName + " " + lastName;
    
    result = to_string(index);
    result += " |";
    result += fillSpaces(name, SPACE_NAME - name.length());
    result += "|";
    result += fillSpaces(to_string(points), SPACE_POINTS - to_string(points).length());
    result += "|";
    result += fillSpaces(country, SPACE_COUNTRY - country.length());
    result += "|";
    result += fillSpaces(team, SPACE_TEAM - team.length());
    result += "|";
    
    return result;
}

//toStringSmall prints the rider but sumarized for listing
string Rider::toStringSmall(bool spacing) {
    string result;

    result += firstName[0];
    result += ".";
    for(int i = 0; i < 3; i++){
        result += lastName[i];
    }
    result += " - ";
    result += to_string(points);

    if(spacing) {
        result = fillSpaces(result, SPACE_PRINT_SMALL - result.length());
    }

    return result;
}

string Rider::toStringDisk() {
    string result;

    result += number;
    result += "|";
    result += firstName;
    result += "|";
    result += lastName;
    result += "|";
    result += country;
    result += "|";
    result += team;
    result += "|";
    result += to_string(points);
    result += "|";
    return result;
}

Rider &Rider::operator=(const Rider &rider) {
    firstName = rider.firstName;
    lastName = rider.lastName;
    number = rider.number;
    country = rider.country;
    team = rider.team;
    points = rider.points;

    return *this;
}

bool Rider::operator==(const Rider &rider) {
    return stoi(number) == stoi(rider.number);
}

bool Rider::operator!=(const Rider &rider) {
    return stoi(number) != stoi(rider.number);
}

bool Rider::operator<(const Rider &rider) {
    return stoi(number) < stoi(rider.number);
}

bool Rider::operator<=(const Rider &rider) {
    return stoi(number) <= stoi(rider.number);
}

bool Rider::operator>(const Rider &rider) {
    return stoi(number) > stoi(rider.number);
}

bool Rider::operator>=(const Rider &rider) {
    return stoi(number) >= stoi(rider.number);
}
