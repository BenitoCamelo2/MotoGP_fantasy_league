#include "member-list.h"

bool MemberList::isValidPos(MemberNode *memberNode) {
    MemberNode* temp(header);

    while(temp != nullptr){
        if(temp == memberNode) {
            return true;
        }
        temp = temp->getNext();
    }
    return false;
}

MemberList::MemberList() : header(nullptr){}

MemberList::~MemberList() {
    deleteAll();
    delete header;
}

bool MemberList::isEmpty() {
    return header == nullptr;
}

void MemberList::insertData(MemberNode *memberNode, const Member &data) {
    if(memberNode != nullptr && !isValidPos(memberNode)){
        cout << "Invalid position when inserting member, try again after restart" << endl;
        throw exception();
    }

    if(memberNode == nullptr){
        memberNode = header;
    }

    MemberNode* toInsert(new MemberNode(data));

    if(toInsert == nullptr){
        cout << "No more space found for program, maybe out of system ram" << endl;
        throw exception();
    }

    toInsert->setNext(header);
    toInsert->setPrevious(nullptr);

    if(header != nullptr){
        header->setPrevious(toInsert);
    }
    header = toInsert;
}

void MemberList::deleteData(MemberNode *memberNode) {
    MemberNode* temp(header);

    if(memberNode == nullptr){
        cout << "Error when deleting, member doesn't exist" << endl;
        throw exception();
    } else if(temp == nullptr){
        return;
    }

    if(temp == memberNode){
        header = header->getNext();
        free(memberNode);
        return;
    }

    if(memberNode == getLastPos()){
        temp = memberNode;
        temp = temp->getPrevious();
        temp->setNext(nullptr);
        free(memberNode);
        return;
    }

    if(memberNode != header && memberNode != getLastPos()){
        while(temp != memberNode){
            temp = temp->getNext();
        }
        if(temp == memberNode){
            temp->getNext()->setPrevious(temp->getPrevious());
            temp->getPrevious()->setNext(temp->getNext());
            free(memberNode);
        }
    }
}

MemberNode *MemberList::getFirstPos() {
    if(!isEmpty()){
        return header;
    } else {
        return nullptr;
    }
}

MemberNode *MemberList::getLastPos() {
    if(!isEmpty()) {
        MemberNode *temp;
        temp = header;
        while (temp != nullptr){
            if(getNextPos(temp) == nullptr){
                return temp;
            } else {
                temp = temp->getNext();
            }
        }
    }
    return nullptr;
}

MemberNode *MemberList::getNextPos(MemberNode *memberNode) {
    return memberNode->getNext();
}

MemberNode *MemberList::retrievePos(const Member &data) {
    MemberNode* temp(header);

    while(temp != nullptr){
        if(temp->getData() == data){
            return temp;
        }
        temp = temp->getNext();
    }
    return nullptr;
}

void MemberList::sortMembers() {
    header = mergeSort(header);
}

MemberNode *MemberList::split(MemberNode *head) {
    MemberNode *fast = head, *slow = head;
    while(fast->getNext() && fast->getNext()->getNext()){
        fast = fast->getNext()->getNext();
        slow = slow->getNext();
    }
    MemberNode* temp = slow->getNext();
    slow->setNext(nullptr);
    return temp;
}

MemberNode *MemberList::merge(MemberNode *first, MemberNode *second) {
    if(!first){
        return second;
    }

    if(!second){
        return first;
    }

    if(first->getData().getPoints() == second->getData().getPoints()){
        //compare which has better rider picks (determins which one has picks closest to the top 5)
        RiderNode* firstPicks;
        RiderNode* secondPicks;
        firstPicks = first->getDataPointer()->getRiderList()->getFirstPos();
        secondPicks = second->getDataPointer()->getRiderList()->getFirstPos();

        while(firstPicks->getData().getPoints() == secondPicks->getData().getPoints()){
            firstPicks = firstPicks->getNext();
            secondPicks = secondPicks->getNext();
        }

        if(firstPicks->getData().getPoints() > secondPicks->getData().getPoints()){
            first->setNext(merge(first->getNext(), second));
            first->getNext()->setPrevious(first);
            first->setPrevious(nullptr);
            return first;
        } else {
            second->setNext(merge(first, second->getNext()));
            second->getNext()->setPrevious(second);
            second->setPrevious(nullptr);
            return second;
        }
    } else if(first->getData().getPoints() > second->getData().getPoints()){
        first->setNext(merge(first->getNext(), second));
        first->getNext()->setPrevious(first);
        first->setPrevious(nullptr);
        return first;
    } else {
        second->setNext(merge(first, second->getNext()));
        second->getNext()->setPrevious(second);
        second->setPrevious(nullptr);
        return second;
    }
}

MemberNode *MemberList::mergeSort(MemberNode *head) {
    if(!head || !head->getNext()){
        return head;
    }
    MemberNode* second = split(head);

    head = mergeSort(head);
    second = mergeSort(second);

    return merge(head, second);
}

string MemberList::toString() {
    MemberNode* temp(header);
    MemberNode* last(getLastPos());
    Member tempMember;
    string result;

    while(temp != nullptr){
        tempMember = temp->getData();
        result += tempMember.toStringSmall();
        if(temp != last) {
            result += "\n";
        }
        temp = temp->getNext();
    }

    return result;
}

string MemberList::toStringSmallHTML() {
    //currently this is used to save as a html table with html tags
    MemberNode* tempMemberNode(header);
    RiderNode* tempRiderNode = new RiderNode();
    Member tempMember;
    Rider tempRider;
    string result = "";

    int i = 1;
    result += "<table>";
    while(tempMemberNode != nullptr){
        tempMember = tempMemberNode->getData();
        result += "<tr>";
        result += "<td>";
        result += "<b>";
        result += to_string(i);
        result += ". ";
        result += tempMember.getUserName();
        result += " - ";
        result += to_string(tempMember.getPoints());
        result += "</b>";
        result += "</td>";
        tempRiderNode = tempMember.getRiderList()->getFirstPos();
        while(tempRiderNode != nullptr){
            tempRider = tempRiderNode->getData();
            result += "<td>";
            result += tempRider.toStringSmall(false);
            tempRiderNode = tempRiderNode->getNext();
            result += "</td>";
        }
        result += "</tr>\n";
        tempMemberNode = tempMemberNode->getNext();
        i++;
    }
    result += "</table>";

    return result;
}

void MemberList::deleteAll() {
    MemberNode* temp(header);
    MemberNode* prev;

    while(temp != nullptr){
        prev = temp;
        temp = temp->getNext();
        delete prev;
    }
    header = nullptr;
}

void MemberList::writeToDisk(const string &fileName) {
    ofstream file(fileName, ios::out);
    MemberNode* temp(header);
    string tempString;

    if(!file.is_open()){
        cout << "Couldn't open " << fileName << endl;
        return;
    }
    while(temp != nullptr){
        RiderNode* tempRiderNode(temp->getData().getRiderList()->getFirstPos());
        tempString = temp->getData().getUserName();
        tempString += "|";
        while(tempRiderNode != nullptr) {
            tempString += tempRiderNode->getData().getNumber();
            tempString += "|";
            tempRiderNode = tempRiderNode->getNext();
        }
        file << tempString << endl;
        temp = temp->getNext();
    }
    file.close();
}

MemberList *MemberList::readFromDisk(const string &fileName) {
    ifstream file(fileName);
    MemberList *memberList = new MemberList();
    string tempString;

    string userName, rookieNumber;
    Rider tempRider;
    int pointsMember=0;
    Member tempMember;

    string number;

    getline(file, tempString, '|');
    if(tempString.empty() || tempString == " " || tempString == "/n"){
        return new MemberList();
    }
    while(tempString != " " && !tempString.empty()){
        int i;
        userName = tempString;
        for(i = 0; i < RIDER_COUNT; i++){
            getline(file, tempString, '|');
            number = tempString;
            tempRider.setNumber(number);
            tempMember.insertRider(tempRider);
        }
        tempMember.setUserName(userName);
        tempMember.setPoints(pointsMember);
        memberList->insertData(memberList->getFirstPos(), tempMember);
        getline(file, tempString);
        getline(file, tempString, '|');
        tempMember = Member();
    }
    return memberList;
}
