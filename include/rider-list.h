#ifndef MOTOGP_FANTASY_LEAGUE_RIDER_LIST_H
#define MOTOGP_FANTASY_LEAGUE_RIDER_LIST_H

#include <iostream>
#include <fstream>
#include "rider-node.h"

class RiderList {
private:
    RiderNode* header;
    bool isValidPos(RiderNode* riderNode);
public:
    RiderList();
    RiderList(RiderList& riderList);
    ~RiderList();

    bool isEmpty();
    int riderCount();
    void insertData(RiderNode* riderNode, const Rider &data);
    void insertOrdered(Rider& data);
    void deleteData(RiderNode* riderNode);
    RiderNode* getFirstPos();
    RiderNode* getLastPos();
    RiderNode* getNextPos(RiderNode *riderNode);
    RiderNode* retrievePos(Rider& rider);
    RiderNode* retrievePosIndex(int index);
    Rider retrieveData(RiderNode* riderNode);
    //string* toStringArray();
    string toString();
    string toStringIndexed();
    void deleteAll();
    void writeToDisk(const string& fileName);
    RiderList* readFromDisk(const string& fileName);

    RiderList& operator = (RiderList* riderList);
};

#endif //MOTOGP_FANTASY_LEAGUE_RIDER_LIST_H
