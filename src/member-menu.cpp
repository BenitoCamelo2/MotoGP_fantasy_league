#include "member-menu.h"
#include "menu.h"
#include "util.h"

MemberMenu::MemberMenu(MemberList *memberList, RiderList *riderList, string &seasonName) {
    errorMessage = "";
    this->memberList = new MemberList(memberList->getFirstPos(), &errorMessage);
    this->riderList = riderList;
    this->seasonName = seasonName;
    saveChanges = false;
    updateMemberPoints();
    this->memberList->sortMembers(riderList->getFirstPos());
    menu();
}

void MemberMenu::updateMemberPoints() {
    int totalPoints;
    MemberNode* tempMemberNode(memberList->getFirstPos());
    Member tempMember;
    Rider tempRookie;
    RiderNode* tempRiderNode = new RiderNode();

    while(tempMemberNode != nullptr){
        totalPoints = 0;
        tempMember = tempMemberNode->getData();
        tempRiderNode = tempMember.getRiderList()->getFirstPos();
        while(tempRiderNode != nullptr) {
            totalPoints += tempRiderNode->getData().getPoints();
            tempRiderNode = tempRiderNode->getNext();
        }
        tempMember.setPoints(totalPoints);
        tempMemberNode->setData(tempMember);
        tempMemberNode = tempMemberNode->getNext();
    }
}

void MemberMenu::menu() {
    bool end = false;
    int option;
    do{
        system(CLEAR);
        cout << "Member Menu, " << seasonName << endl;

        if(!errorMessage.empty()){
            cout << errorMessage;
        }

        cout << "1. Add Member" << endl;
        cout << "2. Delete Member" << endl;
        cout << "3. Modify Member" << endl;
        cout << "4. List Members" << endl;
        cout << "5. Delete ALL Members" << endl;
        cout << "6. Create standings file" << endl;
        cout << "7. Save Changes" << endl;
        cout << "8. Exit" << endl;
        cout << "Option: ";
        errorMessage = "";
        cin >> option;
        switch(option){
            case ADD_MEMBER: {
                if(!saveChanges){
                    saveChanges = addMember();
                } else {
                    addMember();
                }
                updateMemberPoints();
                break;
            }
            case DELETE_MEMBER: {
                if(!saveChanges){
                    saveChanges = deleteMember();
                } else {
                    deleteMember();
                }
                break;
            }
            case MODIFY_MEMBER: {
                if(!saveChanges){
                    saveChanges = modifyMember();
                } else {
                    modifyMember();
                }
                updateMemberPoints();
                this->memberList->sortMembers(riderList->getFirstPos());
                break;
            }
            case LIST_MEMBERS: {
                system(CLEAR);
                cout << memberList->toString() << endl;
                cin.ignore();
                enterToContinue();
                break;
            }
            case DELETE_ALL_MEMBERS: {
                char opt;
                cout << "Are you sure you want to continue? (S/N): ";
                cin >> opt;
                if(opt == 's' || opt == 'S'){
                    memberList->deleteAll();
                }
                enterToContinue();
                saveChanges = true;
                break;
            }
            case CREATE_STANDINGS_FILE: {
                this->memberList->sortMembers(riderList->getFirstPos());

                system(CLEAR);

                ofstream fileTXT(seasonName + '-' + CURRENT_STANDINGS, ios::out);
                ofstream fileHTML(seasonName + '-' + CURRENT_STANDINGS_HTML, ios::out);

                if(!fileTXT.is_open() || !fileHTML.is_open()){
                    cout << "Standings file NOT created successfully, check if administrator privileges are necessary" << endl;
                } else {
                    fileTXT << memberList->toStringSmallHTML() << endl << endl;
                    fileTXT.close();
                    fileHTML << memberList->toStringSmallHTML() << endl << endl;
                    fileHTML.close();
                    cout << "Standings file created successfully" << endl;
                    cout << "To open file standings on macOS:" << endl;
                    cout << "Open Finder, on the top of the screen select Go, in the options select Home and the file should be labelled (season name)-current-standings.txt" << endl;
                    cout << "On windows it should be located in the same directory as the program" << endl;
                    getchar();
                    enterToContinue();
                }
            }
            case SAVE_CHANGES_MEMBER: {
                memberList->writeToDisk(seasonName + '-' + MEMBER_DATA);
                saveChanges = false;
                break;
            }
            case EXIT_MEMBER: {
                if(saveChanges) {
                    char opt;
                    cout << "Would you like to save the unsaved changes? (S/N)" << endl;
                    cout << "->";
                    cin >> opt;
                    if(opt == 's' || opt == 'S'){
                        memberList->writeToDisk(seasonName + '-' + MEMBER_DATA);
                    }
                }
                end = true;
                break;
            }
            default: {
                cout << "Select a valid option" << endl;
                enterToContinue();
                break;
            }
        }
    }while(!end);
}

bool MemberMenu::addMember() {
    int riderCount = riderList->riderCount();
    Member tempMember;
    string userName, tempRiderNumber;
    string riderStringArray[riderCount+1];
    Rider rookie, tempRider;
    RiderNode* tempRiderNode;
    int selections[RIDER_COUNT] = {-1, -1, -1, -1, -1, -1}, amountSelected=0;
    int option = 1, lineOption;
    int key;
    bool exit = false;

    system(CLEAR);
    cout << "Creating member" << endl;
    cout << "Input the user name" << endl;
    cout << "->";
    cin.ignore();getline(cin , userName);
    while(!tempMember.setUserName(userName)){
        cout << "Input a username less than 35 characters" << endl;
        cout << "->";
        getline(cin , userName);
    }

    system(CLEAR);

    if(riderCount != 0){
        int longestStringLength = 0;
        int left, right;

        tempRiderNode = riderList->getFirstPos();
        for(int i = 0; tempRiderNode != nullptr; tempRiderNode = tempRiderNode->getNext(), i++){
            riderStringArray[i] = "\t" + tempRiderNode->getData().toString();
            if(tempRiderNode->getData().toString().length() > longestStringLength){
                longestStringLength = tempRiderNode->getData().toString().length();
            }
        }

        //riderStringArray = riderList->toStringArray(); <- TODO: GET THIS WORKING!!!

        left = 10; //position of left arrows, 10 because of tab
        right = longestStringLength + 19;
        
        initscr();
        cbreak();
        clear();
        noecho();
        keypad(stdscr, TRUE);
        start_color();
        
        curs_set(0);
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        init_pair(2, COLOR_WHITE, COLOR_BLACK);

        printw("Add riders to member's picks\n");
        printw("Navigate with arrow keys\n");
        printw("Enter: add to list\n");
        printw("Backspace: remove from list\n");
        printw("C to cancel\n");

        printMenu(riderStringArray, riderCount);
        attron(COLOR_PAIR(1));
        printw("\tAccept\n");
        attroff(COLOR_PAIR(1));
        attron(COLOR_PAIR(2));

        int messageLine = 8+riderCount, messageStart = 10, acceptLine = riderCount+1;
        bool riderSelectedBoth = false;
        
        while(!exit){
            lineOption = option+5;

            updateMenu(lineOption, left, right);
            if(amountSelected == 5){
                gotoxy(messageStart, messageLine);
                printw("The next rider will be the independent rider");
            }
            refresh();
            key = getch();
            //printw("%d", key); use if need to find keycode
            
            

            switch(key){
                case KEY_DOWN: {
                    if(option == riderCount+1){
                        option = 1;
                    } else {
                        option++;
                    }
                    break;
                }

                case KEY_UP:{
                    if(option == 1){
                        option = riderCount+1;
                    } else {
                        option--;
                    }
                    break;
                }

                case 10: {
                    gotoxy(messageStart, messageLine);
                    printw("                                                     ");

                    if(option == acceptLine){
                        if(amountSelected >= RIDER_COUNT){
                            exit = true;
                        } else {
                            gotoxy(messageStart, messageLine);
                            printw("You must only select 6 riders");
                        }
                        break;
                    }

                    if(amountSelected >= RIDER_COUNT){
                        gotoxy(messageStart, messageLine);
                        printw("You have selected the max amount of riders");
                        break;
                    }
                    
                    int i = 0;
                    for(i; i < RIDER_COUNT; i++){
                        if(selections[i] == -1){
                            break;
                        }
                    }
                                        
                    if(option != acceptLine && checkIfSelected(selections, RIDER_COUNT, option-1) == -1){
                        if(i >= RIDER_COUNT){
                            i = 5;
                        }
                        selections[i] = option-1;
                        gotoxy(right + 5, lineOption);
                        if(i == 5){
                            printw("i");
                        } else {
                            printw("%d", i+1);
                        }
                        amountSelected++;
                    } else if(option != acceptLine && i == 5){
                        riderSelectedBoth = true;
                        selections[i] = option-1;
                        gotoxy(right+7, lineOption);
                        printw("i");
                        amountSelected++;
                    }

                    break;
                }

                case 127: {
                    int selected;

                    gotoxy(messageStart, messageLine);
                    printw("                                                     ");

                    selected = checkIfSelected(selections, RIDER_COUNT, option-1);
                    if(selected != -1){
                        if(riderSelectedBoth){
                            gotoxy(right+7, lineOption);
                            selections[5] = -1;
                            riderSelectedBoth = false;
                        } else {
                            selections[selected] = -1;
                            gotoxy(right+5, lineOption);
                        }
                        printw(" ");
                        amountSelected--;
                    }
                    break;
                }
                    
                case 99: {
                    curs_set(1);
                    endwin();
                    return false;
                }
            }
            clearSelection(lineOption-1, lineOption+1, left, right);
            
        }
        
        curs_set(1);
        endwin();

        cout << endl;

        for(int i = 0; i < RIDER_COUNT; i++){
            tempRiderNode = riderList->retrievePosIndex(selections[i]);
            tempRider = tempRiderNode->getData();
            tempMember.insertRider(tempRider);
        }
    } else {
        cout << "Make sure after adding riders to add them to " << userName << endl;
        cin.ignore();
        enterToContinue();
    }
    memberList->insertData(memberList->getFirstPos(), tempMember);
    return true;
}

bool MemberMenu::deleteMember() {
    system(CLEAR);
    bool exit = false;
    string userName;
    Member tempMember;
    MemberNode* temp = new MemberNode();

    cout << "Delete Member" << endl;
    cout << memberList->toString() << endl;
    cout << "Input the member username to delete" << endl;
    cout << "-> ";
    cin.ignore();getline(cin, userName);
    tempMember.setUserName(userName);

    temp = memberList->retrievePos(tempMember);
    if(temp == nullptr){
        while(!exit || temp != nullptr) {
            cout << "Member not found, try again (0 to cancel)" << endl;
            cout << "-> ";
            getline(cin, userName);
            if(userName == "0"){
                exit = true;
            } else {
                tempMember.setUserName(userName);
                temp = memberList->retrievePos(tempMember);
            }
        }
    }
    if(exit){
        return false;
    }
    memberList->deleteData(temp);
    return true;
}

bool MemberMenu::modifyMember() {
    system(CLEAR);
    bool exit = false;
    int option;
    bool changes = false;

    do {
        cout << "Modify Member" << endl;
        cout << "1. Change Username" << endl;
        cout << "2. Change Rider" << endl;
        cout << "3. Exit" << endl;
        cout << "Option: ";
        cin >> option;
        switch(option){
            case CHANGE_USERNAME: {
                string userName;
                Member tempMember;
                MemberNode* tempNode(memberList->getFirstPos());
                bool endModify = false;
                system(CLEAR);
                cout << "Modify Member" << endl;

                cout << memberList->toString() << endl;
                cout << "Input the username of the member you would like to modify" << endl;
                cout << "-> ";
                getline(cin, userName);
                tempNode = memberList->retrievePos(tempMember);

                while(!endModify && tempNode == nullptr){
                    cout << "Member not found, input again (0 to cancel)" << endl;
                    cout << "-> ";
                    getline(cin, userName);
                    if(userName == "0"){
                        endModify = true;
                    } else {
                        tempMember.setUserName(userName);
                        tempNode = memberList->retrievePos(tempMember);
                    }
                }

                if(endModify){
                    return false;
                }

                cout << "Input the new username" << endl;
                cout << "-> ";
                getline(cin, userName);
                tempMember = tempNode->getData();
                tempMember.setUserName(userName);
                tempNode->setData(tempMember);
                changes = true;
                break;
            }
            case CHANGE_RIDER: {
                system(CLEAR);
                string userName;
                Member tempMember;
                MemberNode* tempMemberNode = new MemberNode();

                int riderIndex;
                Rider tempRider;
                RiderNode* tempRiderNode1 = new RiderNode();
                RiderNode* tempRiderNode2 = new RiderNode();

                bool endModify = false;
                system(CLEAR);
                cout << "Modify Member Rider" << endl;

                cout << memberList->toString() << endl;
                cout << "Input the username of the member's riders you would like to modify (0 to cancel)" << endl;
                cout << "-> ";
                cin.ignore();getline(cin, userName);
                tempMemberNode = memberList->retrievePos(tempMember);

                while(!endModify && tempMemberNode == nullptr){
                    cout << "Member not found, input again (0 to cancel)" << endl;
                    cout << "-> ";
                    getline(cin, userName);
                    if(userName == "0"){
                        endModify = true;
                    } else {
                        tempMember.setUserName(userName);
                        tempMemberNode = memberList->retrievePos(tempMember);
                    }
                }

                if(endModify){
                    return false;
                }

                tempMember = tempMemberNode->getData();

                system(CLEAR);
                cout << "Modify Member Rider" << endl;
                cout << tempMember.getRiderList()->toStringIndexed() << endl;
                cout << "Input rider index (0 to cancel)" << endl;
                cout << "-> ";
                cin >> riderIndex;
                riderIndex--;
                if(riderIndex == -1){
                    return false;
                }
                tempRiderNode1 = tempMember.getRiderList()->retrievePosIndex(riderIndex);
                cout << tempRiderNode1->getData().toString() << endl;
                if(tempRiderNode1 == nullptr){
                    endModify = false;
                    while(tempRiderNode1 == nullptr && !endModify){
                        cout << "Rider not found, input number again (0 to cancel)" << endl;
                        cout << "-> ";
                        cin >> riderIndex;
                        riderIndex--;
                        if(riderIndex == -1){
                            endModify = true;
                        } else {
                            tempRiderNode1 = tempMember.getRiderList()->retrievePosIndex(riderIndex);
                        }
                    }
                    if(endModify){
                        return false;
                    }
                }

                system(CLEAR);
                cout << "Modify Member Rider" << endl;
                cout << riderList->toStringIndexed() << endl;
                cout << "Input new rider index (0 to cancel): " << endl;
                cout << "-> ";
                cin >> riderIndex;
                riderIndex--;
                if(riderIndex == -1){
                    return false;
                }
                tempRiderNode2 = riderList->retrievePosIndex(riderIndex);
                if(tempRiderNode2 == nullptr){
                    endModify = false;
                    while(tempRiderNode2 == nullptr && !endModify){
                        cout << "Rider not found, input number again (0 to cancel)" << endl;
                        cout << "-> ";
                        cin >> riderIndex;
                        riderIndex--;
                        if(riderIndex == -1){
                            endModify = true;
                        } else {
                            tempRiderNode2 = riderList->retrievePosIndex(riderIndex);
                        }
                    }
                    if(endModify){
                        return false;
                    }
                }

                tempRider = tempRiderNode2->getData();
                tempRiderNode1->setData(tempRider);

                changes = true;
                break;
            }
            case EXIT_MODIFY: {
                exit = true;
                break;
            }
            default: {
                cout << "Choose a valid option" << endl;
                cin.ignore();
                enterToContinue();
                break;
            }
        }
    }while(!exit);
    system(CLEAR);
    return changes;
}

void MemberMenu::enterToContinue() {
    cout << "Press enter to continue" << endl;
    getchar();
}
