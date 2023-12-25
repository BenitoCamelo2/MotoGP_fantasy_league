#include "season-menu.h"

SeasonMenu::SeasonMenu(MemberList *memberList, RiderList *riderList, string* seasonName) {
    this->memberList = memberList;
    this->riderList = riderList;
    this->seasonName = seasonName;
    menu();
}

void SeasonMenu::menu() {
    bool end = false;
    int option;
    do{
        system(CLEAR);
        cout << "Season Menu: " << *seasonName << endl;
        cout << "1. Create new season" << endl;
        cout << "2. Select season" << endl;
        cout << "3. Change default season" << endl;
        cout << "4. Exit" << endl;
        cin >> option;
        switch(option){
            case CREATE_SEASON: {
                createSeason();
                break;
            }
            case SELECT_SEASON: {
                changeSeason();
                break;
            }
            case CHANGE_DEFAULT_SEASON: {
                changeDefaultSeason();
                break;
            }
            case EXIT_SEASON: {
                end = true;
                break;
            }
            default: {
                cout << "Select a valid option" << endl;
                enterToContinue();
            }
        }
    }while(!end);
}

void SeasonMenu::createSeason() {
    system(CLEAR);

    string seasonName;
    string auxString;
    string seasons[100];
    int i = 1;
    bool finish = false;
    char option;
    ifstream readFile(PROGRAM_DATA, ios::in);

    getline(readFile, auxString);
    while (!auxString.empty()) {
        seasons[i++] = auxString;
        getline(readFile, auxString);
    }
    readFile.close();
    ofstream writeFile(PROGRAM_DATA, ios::out);
    cout << "Creating season" << endl;
    cout << "Input the season name (change spaces for -): ";
    cin.ignore();getline(cin, seasonName);
    while(!finish) {
        int j = 1;
        for(j; j < i; j++){
            if(seasons[j] == seasonName){
                cout << "Two seasons cant have the same name, input name again (0 to cancel): ";
                getline(cin, seasonName);
                if(seasonName == "0"){
                    return;
                }
                break;
            }
        }
        if(j == i){
            finish = true;
        }
    }
    cout << "Would you like to make it the default season? (S/N): ";
    cin >> option;
    ofstream file2(seasonName + '-' + MEMBER_DATA, ios::out);
    ofstream file3(seasonName + '-' + RIDER_DATA, ios::out);
    int j;
    if (option == 'S' || option == 's') {
        seasons[0] = seasonName;
        j = 0;
    } else {
        seasons[i++] = seasonName;
        j = 1;
    }

    if (!writeFile.is_open()) {
        cout << "Error couldn't open " << PROGRAM_DATA << endl;
        enterToContinue();
        return;
    } else {
        for (j; j < i; j++) {
            writeFile << seasons[j] << endl;
        }
    }
    writeFile.close();
    file2.close();
    file3.close();

}

void SeasonMenu::changeSeason() {
    system(CLEAR);
    cout << "Select Season" << endl;

    string seasons[100];
    string auxString;
    bool validSelection = false;
    int selection;
    int i = 0;
    ifstream file(PROGRAM_DATA);

    getline(file, auxString);
    while(auxString != ""){
        seasons[i++] = auxString;
        getline(file, auxString);
    }
    
    cout << seasons[0] << endl;
    cout << seasons[1] << endl;
    
    for(int j = 0; j < i; j++){
        cout << j+1 << " " << seasons[j] << endl;
    }
    cout << "Input the season number to select: ";
    cin >> selection;
    while(!validSelection){
        if(selection < 1 || selection > i){
            cout << "Select a valid season: ";
            cin >> selection;
        } else {
            validSelection = true;
        }
    }

    riderList->deleteAll();
    memberList->deleteAll();

    cout << seasons[selection-1] << endl;
    cout << seasons[selection-1] << endl;
    
    riderList = riderList->readFromDisk(seasons[selection-1] + '-' + RIDER_DATA);
    memberList = memberList->readFromDisk(seasons[selection-1] + '-' + MEMBER_DATA);
    *seasonName = seasons[selection-1];

    MemberNode* tempMemberNode(memberList->getFirstPos());
    Member tempMember;
    Rider tempRookie;
    Rider rider;
    string tempNumber;

    //complete the rider lists and rookies of each member, because readFromDisk only assigns riders with only their numbers
    while(tempMemberNode != nullptr){
        //retrieve rookie number and get full rookie data to assign
        tempMember = tempMemberNode->getData();
        //node to go through member rider list
        RiderNode* tempRiderNode1(tempMember.getRiderList()->getFirstPos());
        while(tempRiderNode1 != nullptr){
            //get full data of each rider, same method as rookie
            RiderNode* tempRiderNode2;
            tempNumber = tempRiderNode1->getData().getNumber();
            rider.setNumber(tempNumber);
            tempRiderNode2 = riderList->retrievePos(rider);
            rider = tempRiderNode2->getData();
            tempRiderNode1->setData(rider);
            tempRiderNode1 = tempRiderNode1->getNext();
        }
        tempMemberNode->setData(tempMember);
        tempMemberNode = tempMemberNode->getNext();
    }
}

void SeasonMenu::changeDefaultSeason() {
    system(CLEAR);
    cout << "Change Default Season" << endl;

    string seasons[100];
    string auxString;
    bool validSelection = false;
    int selection;
    int i = 0;
    ifstream file(PROGRAM_DATA);

    getline(file, auxString);
    while(auxString != ""){
        seasons[i++] = auxString;
    }
    for(int j = 0; j < i; j++){
        cout << j+1 << " " << seasons[j] << endl;
    }
    cout << "Input the season number to select: ";
    cin >> selection;
    while(!validSelection){
        if(selection < 1 || selection > i){
            cout << "Select a valid season: ";
            cin >> selection;
        } else {
            validSelection = true;
        }
    }

    auxString = seasons[selection-1];
    seasons[selection-1] = seasons[0];
    seasons[0] = auxString;

    cout << "Restart the application to take effect" << endl;\
    enterToContinue();
}

void SeasonMenu::enterToContinue() {
    cout << "Press enter to continue" << endl;
    getchar();
}
