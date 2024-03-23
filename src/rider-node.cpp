#include "rider-node.h"

RiderNode::RiderNode() {
    data = Rider();
    next = nullptr;
    previous = nullptr;
}

RiderNode::RiderNode(const Rider &rider) {
    data = rider;
    next = nullptr;
    previous = nullptr;
}

Rider* RiderNode::getDataReference() {
    return &data;
}

Rider RiderNode::getData() {
    return data;
}

RiderNode *RiderNode::getNext() {
    return next;
}

RiderNode *RiderNode::getPrevious() {
    return previous;
}

void RiderNode::setData(const Rider &data) {
    this->data = data;
}

void RiderNode::setNext(RiderNode *riderNode) {
    next = riderNode;
}

void RiderNode::setPrevious(RiderNode *riderNode) {
    previous = riderNode;
}
