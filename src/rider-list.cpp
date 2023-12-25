#include "rider-list.h"

bool RiderList::isValidPos(RiderNode *riderNode) {
    RiderNode* temp(header);
    while(temp != nullptr){
        if(temp == riderNode){
            return true;
        }
        temp = temp->getNext();
    }
    return false;
}

int RiderList::riderCount() {
    RiderNode* temp(header);
    int i = 0;

    while(temp != nullptr){
        i++;
        temp = temp->getNext();
    }

    return i;
}

RiderList::RiderList() :header(nullptr) {}

RiderList::RiderList(RiderList &riderList) {}

RiderList::~RiderList() {
    delete header;
}

bool RiderList::isEmpty() {
    return header == nullptr;
}

void RiderList::insertData(RiderNode *riderNode, const Rider &data) {
    if(riderNode != nullptr && !isValidPos(riderNode)){
        cout << "Error when inserting data" << endl;
        throw exception();
    }

    RiderNode* toInsert(new RiderNode(data));

    if(toInsert == nullptr){
        cout << "No more ram space available" << endl;
        throw exception();
    }

    if(header == nullptr){
        header = toInsert;
    }else if(riderNode == nullptr) {
        header->setNext(toInsert);
        toInsert->setPrevious(header);
    }else if(riderNode->getNext() == nullptr) {
        riderNode->setNext(toInsert);
        toInsert->setPrevious(riderNode);
    }else if(riderNode->getPrevious() == nullptr){
        toInsert->setNext(riderNode);
        riderNode->setPrevious(toInsert);
    } else {
        toInsert->setNext(riderNode->getNext());
        toInsert->setPrevious(riderNode);
        riderNode->getNext()->setPrevious(toInsert);
        riderNode->setNext(toInsert);
    }

}

void RiderList::insertOrdered(Rider &data) {
    RiderNode* temp(header);
    RiderNode* prev(nullptr);

    while(temp != nullptr && data >= temp->getData()){
        prev = temp;
        temp = temp->getNext();
        if(temp != nullptr && data == temp->getData()){
            prev = temp;
            temp = temp->getNext();
        }
    }
    insertData(prev, data);
}



void RiderList::deleteData(RiderNode *riderNode) {
    RiderNode* temp(header);
    RiderNode* trail(nullptr);

    if(!isValidPos(riderNode)){
        cout << "Rider not found" << endl;
        getchar();
    }
    if(temp->getNext() == nullptr && temp == riderNode){
        free(header);
        header = nullptr;
    } else {
        while(temp != nullptr){
            if(temp == riderNode){
                trail->setNext(getNextPos(temp));
                temp = nullptr;
            } else {
                trail = temp;
                temp = temp->getNext();
            }
        }
        delete temp;
    }
}

RiderNode *RiderList::getFirstPos() {
    if(!isEmpty()){
        return header;
    } else {
        return nullptr;
    }
}

RiderNode *RiderList::getLastPos() {
    RiderNode* temp(header);

    if(isEmpty()){
        return nullptr;
    }

    if(getNextPos(temp) == nullptr){
        return temp;
    } else {
        while(temp != nullptr){
            if(getNextPos(temp) == nullptr){
                return temp;
            } else {
                temp = temp->getNext();
            }
        }
    }
    return nullptr;
}

RiderNode *RiderList::getNextPos(RiderNode* riderNode) {
    return riderNode->getNext();
}

RiderNode *RiderList::retrievePos(Rider &rider) {
    RiderNode* temp(header);
    RiderNode* last(getLastPos());

    if(temp == last){
        if(temp->getData() == rider){
            return temp;
        } else {
            return nullptr;
        }
    } else {
        while(temp != nullptr){
            if(temp->getData() == rider){
                return temp;
            }
            temp = temp->getNext();
        }
    }
    return nullptr;
}

RiderNode *RiderList::retrievePosIndex(int index) {
    RiderNode* temp(header);
    int riderCount = this->riderCount();

    if(index > riderCount){
        return nullptr;
    }

    for(int i = 0; i < index; i++){
        temp = temp->getNext();
    }

    return temp;
}

Rider RiderList::retrieveData(RiderNode *riderNode) {
    RiderNode* temp(header);
    RiderNode* last(getLastPos());

    if(!isValidPos(riderNode)){
        cout << "Error: rider not found" << endl;
        getchar();
    }

    return riderNode->getData();
}

string RiderList::toString() {
    RiderNode* temp = header;
    string result;

    if(isEmpty()){
        return "No riders in list";
    }

    while(temp != nullptr){
        result += temp->getData().toString();
        result += "\n";
        temp = temp->getNext();
    }

    return result;
}

string RiderList::toStringIndexed(){
    RiderNode* temp = header;
    string result;
    int i = 1;

    if(isEmpty()){
        return "No riders in list";
    }

    while(temp != nullptr){
        result += temp->getData().toStringIndexed(i);
        result += "\n";
        temp = temp->getNext();
        i++;
    }

    return result;
}

/*
string* RiderList::toStringArray() {
    string *result[] = {};
    RiderNode* temp(header);
    int i = 0;

    while(temp != nullptr){
        *result[i] = "\t" + temp->getData().toString();
        temp = temp->getNext();
        i++;
    }

    return *result;
}*/

void RiderList::deleteAll() {
    RiderNode* temp(header);
    RiderNode* trail(nullptr);
    RiderNode* last(getLastPos());

    if(temp == last){
        delete temp;
        header = nullptr;
    } else {
        while(temp != nullptr){
            trail = temp;
            temp = temp->getNext();
            delete trail;
        }
        delete temp;
        header = nullptr;
    }
}

void RiderList::writeToDisk(const string &fileName) {
    ofstream file(fileName);
    RiderNode* temp(header);
    string tempString;

    if(!file.is_open()){
        cout << "Couldn't open " << fileName << endl;
        return;
    }
    while(temp != nullptr){
        tempString = temp->getData().toStringDisk();
        file << tempString << endl;
        temp = temp->getNext();
    }
    file.close();
}

RiderList *RiderList::readFromDisk(const string &fileName) {
    ifstream file(fileName);
    RiderList* riderList = new RiderList();
    string tempString;

    string firstName, lastName, country, team, number;
    int points;
    bool chosen;
    Rider *tempRider = new Rider();

    getline(file, tempString, '|');
    if(tempString.empty() || tempString == " "){
        return new RiderList();
    }
    while(tempString != " " && !tempString.empty()){
        number = tempString;
        getline(file, tempString, '|');
        firstName = tempString;
        getline(file, tempString, '|');
        lastName = tempString;
        getline(file, tempString, '|');
        country = tempString;
        getline(file, tempString, '|');
        team = tempString;
        getline(file, tempString, '|');
        points = stoi(tempString);
        getline(file, tempString);
        tempRider->setData(firstName, lastName, number, country, team, points);
        riderList->insertOrdered(*tempRider);
        getline(file, tempString, '|');
    }
    delete tempRider;
    return riderList;
}

RiderList &RiderList::operator=(RiderList *riderList) {
    RiderNode* trailDest(nullptr);
    RiderNode* trailSrc(nullptr);
    RiderNode* tempDest(header);
    RiderNode* tempSrc(riderList->getFirstPos());
    RiderNode* lastDest(getLastPos());
    RiderNode* lastSrc(riderList->getLastPos());

    if(tempSrc == lastSrc){
        tempSrc = tempDest;
        return *this;
    }
    while(tempSrc != nullptr){
        trailDest = tempDest;
        trailSrc = tempSrc;
        tempSrc = tempSrc->getNext();
        tempDest = tempDest->getNext();
        trailDest = trailSrc;
    }
    return *this;
}
