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

MemberList::MemberList() : header(nullptr), errorMessage(){}

MemberList::MemberList(MemberNode* header, string* errorMessage){
    cout << "error message location in memberlist constructor: " << errorMessage << endl;
    this->header = header;
    this->errorMessage = errorMessage;
}

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

void MemberList::sortMembers(RiderNode* riderHead) {
    header = mergeSort(header, riderHead);
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

MemberNode *MemberList::tieBreaker(MemberNode *firstMember, MemberNode *secondMember, RiderNode *riderHead) {
    //rider picks of each player (first, second)
    RiderNode* firstPicks = firstMember->getDataPointer()->getRiderList()->getFirstPos();
    RiderNode* secondPicks = secondMember->getDataPointer()->getRiderList()->getFirstPos();
    RiderNode* tempRider = riderHead;
    bool samePicks = true;
    int i = 1;

    //if one of the members have one of the top 5 riders correctly guessed, the one that chose correctly will be ahead
    //the if's are structured so that if they both have the same rider in the same position, then it keeps checking
    while(firstPicks != nullptr && secondPicks != nullptr){
        if(firstPicks == tempRider && secondPicks != tempRider){
            firstMember->setNext(merge(firstMember->getNext(), secondMember, riderHead));
            firstMember->getNext()->setPrevious(firstMember);
            firstMember->setPrevious(nullptr);
            return firstMember;
        } else if(firstPicks != tempRider && secondPicks == tempRider){
            secondMember->setNext(merge(firstMember, secondMember->getNext(), riderHead));
            secondMember->getNext()->setPrevious(secondMember);
            secondMember->setPrevious(nullptr);
            return secondMember;
        }

        firstPicks = firstPicks->getNext();
        secondPicks = secondPicks->getNext();
        tempRider = tempRider->getNext();
    }

    firstPicks = firstMember->getDataPointer()->getRiderList()->getFirstPos();
    secondPicks = secondMember->getDataPointer()->getRiderList()->getFirstPos();
    //check to see if one of the members has a rider in the correct position
    //only goes to getNext to not lcheck the independant rider, as they would be read as 6th position instead of idnependant
    while(secondPicks->getNext() != nullptr && firstPicks->getNext() != nullptr){
        if(secondPicks->getData().getPosition() != firstPicks->getData().getPosition()){
            if(secondPicks->getData().getPosition() == i){
                secondMember->setNext(merge(firstMember, secondMember->getNext(), riderHead));
                secondMember->getNext()->setPrevious(secondMember);
                secondMember->setPrevious(nullptr);
                return secondMember;
            } else if(firstPicks->getData().getPosition() == i){
                firstMember->setNext(merge(firstMember->getNext(), secondMember, riderHead));
                firstMember->getNext()->setPrevious(firstMember);
                firstMember->setPrevious(nullptr);
                return firstMember;
            }
        }

        i++;
        firstPicks = firstPicks->getNext();
        secondPicks = secondPicks->getNext();
    }

    firstPicks = firstMember->getDataPointer()->getRiderList()->getFirstPos();
    secondPicks = secondMember->getDataPointer()->getRiderList()->getFirstPos();
    //iterate until points are not the same
    while((firstPicks->getData().getPoints() == secondPicks->getData().getPoints()) && (firstPicks->getNext() != nullptr && firstPicks->getNext() != nullptr)){
        firstPicks = firstPicks->getNext();
        secondPicks = secondPicks->getNext();
        if(secondPicks->getData().getNumber() != firstPicks->getData().getNumber()){
            samePicks = false;
        }
    }

    if(secondPicks->getData() != firstPicks->getData()){
        samePicks = false;
    }

    if(samePicks && errorMessage){
        *errorMessage += firstMember->getData().getUserName() + " and " + secondMember->getData().getUserName() + " have the same rider picks!\n";
    }

    //conditionals to find which player has a rider with more points
    if(firstPicks->getData().getPoints() > secondPicks->getData().getPoints()){
        firstMember->setNext(merge(firstMember->getNext(), secondMember, riderHead));
        firstMember->getNext()->setPrevious(firstMember);
        firstMember->setPrevious(nullptr);
        return firstMember;
    } else {
        secondMember->setNext(merge(firstMember, secondMember->getNext(), riderHead));
        secondMember->getNext()->setPrevious(secondMember);
        secondMember->setPrevious(nullptr);
        return secondMember;
    }
}

MemberNode *MemberList::merge(MemberNode *first, MemberNode *second, RiderNode* riderHead) {
    if(!first){
        return second;
    }

    if(!second){
        return first;
    }

    //both players have the same amount of points, tiebreaker
    if(first->getData().getPoints() == second->getData().getPoints()){
        MemberNode* temp = tieBreaker(first, second, riderHead);
        return temp;
    } else if(first->getData().getPoints() > second->getData().getPoints()){
        first->setNext(merge(first->getNext(), second, riderHead));
        first->getNext()->setPrevious(first);
        first->setPrevious(nullptr);
        return first;
    } else {
        second->setNext(merge(first, second->getNext(), riderHead));
        second->getNext()->setPrevious(second);
        second->setPrevious(nullptr);
        return second;
    }
}

MemberNode *MemberList::mergeSort(MemberNode *head, RiderNode* riderHead) {
    if(!head || !head->getNext()){
        return head;
    }
    MemberNode* second = split(head);

    head = mergeSort(head, riderHead);
    second = mergeSort(second, riderHead);

    return merge(head, second, riderHead);
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
