#include "rider-menu.h"

RiderMenu::RiderMenu(MemberList* memberList, RiderList *riderList, string &seasonName) {
    this->memberList = memberList;
    this->riderList = riderList;
    this->seasonName = seasonName;
    saveChanges = false;
    menu();
}

void RiderMenu::menu() {
    bool end = false;
    int option;
    do{
        system(CLEAR);
        cout << "Rider Menu" << endl;
        cout << "1. Add Rider" << endl;
        cout << "2. Add Race Results" << endl;
        cout << "3. Add Sprint Race Results" << endl;
        cout << "4. Delete Rider" << endl;
        cout << "5. List Riders" << endl;
        cout << "6. Delete ALL Riders" << endl;
        cout << "7. Save Changes" << endl;
        cout << "8. Exit" << endl;
        cout << "Option: ";
        cin >> option;
        switch(option){
            case ADD_RIDER: {
                saveChanges = addRider();
                break;
            }
            case ADD_RACE_RESULTS: {
                saveChanges = addRaceResults();
                break;
            }
            case ADD_SPRINT_RACE_RESULTS: {
                saveChanges = addSprintRaceResults();
                break;
            }
            case DELETE_RIDER: {
                saveChanges = deleteRider();
                break;
            }
            case LIST_RIDERS: {
                system(CLEAR);
                cout << riderList->toString() << endl;
                cin.ignore();
                enterToContinue();
                break;
            }
            case DELETE_ALL_RIDERS: {
                char opt;
                cout << "Are you sure you want to continue? (S/N)" << endl;
                cout << "->";
                cin >> opt;
                if(opt == 's' || opt == 'S'){
                    riderList->deleteAll();
                    saveChanges = true;
                }
                break;
            }
            case SAVE_CHANGES_RIDERS: {
                riderList->writeToDisk(seasonName + '-' + RIDER_DATA);
                saveChanges = false;
                break;
            }
            case EXIT_RIDER: {
                char opt;
                if(saveChanges){
                    cout << "Would you like to save the changes made? (S/N)" << endl;
                    cout << "->";
                    cin >> opt;
                    if(opt == 'S' || opt == 's'){
                        riderList->writeToDisk(seasonName + '-' + RIDER_DATA);
                        saveChanges = false;
                    }
                }
                end = true;
                break;
            }
            default: {
                cout << "Select a valid option" << endl;
                cin.ignore();
                enterToContinue();
            }
        }
    }while(!end);
}

bool RiderMenu::addRider() {
    string firstName, lastName, number, country, team;
    bool rookie = false, testRider = false;
    int points;
    Rider tempRider;
    char option;

    system(CLEAR);
    cout << "Add Rider" << endl;
    cout << "Input the first name" << endl;
    cout << "->";
    cin.ignore();
    getline(cin, firstName);
    cout << "Input the last name" << endl;
    cout << "->";
    getline(cin, lastName);
    cout << "Input the number" << endl;
    cout << "->";
    getline(cin, number);
    cout << "Input the country" << endl;
    cout << "->";
    getline(cin, country);
    cout << "Input the team" << endl;
    cout << "->";
    getline(cin, team);
    cout << "Input the points" << endl;
    cout << "->";
    cin >> points;

    tempRider.setData(firstName, lastName, number, country, team, points);

    riderList->insertOrdered(tempRider);
    return true;
}

bool RiderMenu::addRaceResults() {
    system(CLEAR);

    RiderNode* temp(riderList->getFirstPos());
    Rider tempRider;
    int position;
    int points[25] = {25, 20, 16, 13, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    int riderPoints;
    string line;

    cout << "Add Race Results" << endl;
    while(temp != nullptr){
        riderPoints = 0;
        line = temp->getData().toStringSmall(true);
        tempRider = temp->getData();
        cout << line << endl;
        cout << "Input final race position" << endl;
        cout << "->";
        cin >> position;
        if(position < 16 && position > 0) {
            riderPoints += tempRider.getPoints();
            riderPoints += points[position-1];
            tempRider.setPoints(riderPoints);
            temp->setData(tempRider);
        }
        temp = temp->getNext();
    }
    updateMembersRiders();
    return true;
}

bool RiderMenu::addSprintRaceResults() {
    system(CLEAR);

    RiderNode* temp(riderList->getFirstPos());
    Rider tempRider;
    int position;
    int points[25] = {12, 9, 7, 6, 5, 4, 3, 2, 1};
    int riderPoints;
    string line;

    cout << "Add Sprint Race Results" << endl;
    while(temp != nullptr){
        riderPoints = 0;
        line = temp->getData().toStringSmall(true);
        tempRider = temp->getData();
        cout << line << endl;
        cout << "Input final race position" << endl;
        cout << "->";
        cin >> position;
        if(position < 10 && position > 0) {
            riderPoints += tempRider.getPoints();
            riderPoints += points[position-1];
            tempRider.setPoints(riderPoints);
            temp->setData(tempRider);
        }
        temp = temp->getNext();
    }
    updateMembersRiders();
    return true;
}

bool RiderMenu::deleteRider() {
    system(CLEAR);

    string riderNumber;
    Rider tempRider;
    bool found = false;
    RiderNode *tempNode(riderList->getFirstPos());

    cout << "Delete Rider" << endl;
    cout << riderList->toString() << endl;
    cout << "Input rider number to delete: ";
    cin.ignore();getline(cin, riderNumber);
    tempRider.setNumber(riderNumber);

    while(tempNode != nullptr){
        if(tempNode->getData() == tempRider){
            riderList->deleteData(tempNode);
            found = true;
        }
        tempNode = tempNode->getNext();
    }

    if(found){
        cout << "Rider deleted successfully" << endl;
        enterToContinue();
        return true;
    }
    cout << "Rider not deleted successfully" << endl;
    enterToContinue();
    return false;

}

void RiderMenu::updateMembersRiders() {
    int totalPoints;
    MemberNode* tempMemberNode(memberList->getFirstPos());
    Rider tempRider;
    Member tempMember;
    RiderNode* tempRiderNode1;
    RiderNode* tempRiderNode2;

    while(tempMemberNode != nullptr){
        totalPoints = 0;
        tempMember = tempMemberNode->getData();
        tempRiderNode1 = tempMember.getRiderList()->getFirstPos();
        while(tempRiderNode1 != nullptr){
            tempRider = tempRiderNode1->getData();
            tempRiderNode2 = riderList->getFirstPos();
            while(tempRiderNode2 != nullptr){
                if(tempRiderNode2->getData().getNumber() == tempRider.getNumber()){
                    tempRider = tempRiderNode2->getData();
                    break;
                }
                tempRiderNode2 = tempRiderNode2->getNext();
            }
            tempRiderNode1->setData(tempRider);
            totalPoints += tempRiderNode1->getData().getPoints();
            tempRiderNode1 = tempRiderNode1->getNext();
        }
        totalPoints += tempMember.getPoints();
        tempMember.setPoints(totalPoints);
        tempMemberNode->setData(tempMember);
        tempMemberNode = tempMemberNode->getNext();
    }
}

void RiderMenu::enterToContinue() {
    cout << "Press enter to continue" << endl;
    getchar();
}
