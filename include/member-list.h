#ifndef MOTOGP_FANTASY_LEAGUE_MEMBER_LIST_H
#define MOTOGP_FANTASY_LEAGUE_MEMBER_LIST_H

#include <iostream>
#include <fstream>

#include "member-node.h"
#include "rider-list.h"

using namespace std;

class MemberList {
    MemberNode* header;
    string* errorMessage;
    bool isValidPos(MemberNode* memberNode);
public:

    MemberList();
    MemberList(MemberNode* header, string* errorMessage);
    ~MemberList();

    bool isEmpty();
    void insertData(MemberNode* memberNode, const Member& data);
    void deleteData(MemberNode* memberNode);
    MemberNode* getFirstPos();
    MemberNode* getLastPos();
    MemberNode* getNextPos(MemberNode *memberNode);
    MemberNode* retrievePos(const Member &data);
    void sortMembers(RiderNode* riderHead);
    MemberNode* split(MemberNode* head);
    MemberNode* tieBreaker(MemberNode* first, MemberNode* second, RiderNode* riderHead);
    MemberNode* merge(MemberNode* first, MemberNode* second, RiderNode* riderHead);
    MemberNode* mergeSort(MemberNode* head, RiderNode* riderHead);
    string toString();
    string toStringSmallHTML();
    void deleteAll();
    void writeToDisk(const string &fileName);
    MemberList* readFromDisk(const string &fileName);
};

#endif //MOTOGP_FANTASY_LEAGUE_MEMBER_LIST_H
