#ifndef MOTOGP_FANTASY_LEAGUE_RIDER_NODE_H
#define MOTOGP_FANTASY_LEAGUE_RIDER_NODE_H

#include <iostream>
#include "rider.h"

class RiderNode {
private:
    Rider data;
    RiderNode* next;
    RiderNode* previous;
public:
    RiderNode();
    RiderNode(const Rider &rider);

    Rider getData();
    Rider* getDataReference();
    RiderNode* getNext();
    RiderNode* getPrevious();
    void setData(const Rider &data);
    void setNext(RiderNode* riderNode);
    void setPrevious(RiderNode* riderNode);
};

#endif //MOTOGP_FANTASY_LEAGUE_RIDER_NODE_H
