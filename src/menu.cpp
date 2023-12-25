#include "menu.h"

Menu::Menu() {
    system(CLEAR);
    ifstream file(PROGRAM_DATA);
    memberList = new MemberList();
    riderList = new RiderList();

    //if the file exists, we start program normally
    if(file.is_open()){
        startProgram();
        menu();
    //if the file doesn't exist, we create it in firstStart()
    } else {
        firstStart();
        menu();
    }
}

void Menu::firstStart() {
    //create a new season since programdata is empty in this case
    ofstream file(PROGRAM_DATA, ios::out);

    cout << "Wellcome to the MotoGP Fantasy League Manager" << endl;
    cout << "Input a name for the current season (replace spaces with: - ): ";
    getline(cin, seasonName);

    //create member and rider data
    ofstream file2(seasonName + '-' + MEMBER_DATA, ios::out);
    ofstream file3(seasonName + '-' + RIDER_DATA, ios::out);
    file << seasonName << endl;

    //close the three files to save the changes
    file.close();
    file2.close();
    file3.close();
}

void Menu::startProgram() {
    ifstream file(PROGRAM_DATA);

    //get season name
    if(!file.is_open()){
        cout << "Error when opening " << PROGRAM_DATA << endl;
        throw exception();
    }
    getline(file, seasonName);
    //open data files for riders and members
    riderList = riderList->readFromDisk(seasonName + '-' + RIDER_DATA);
    memberList = memberList->readFromDisk(seasonName + '-' + MEMBER_DATA);

    //variables
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

//main menu
void Menu::menu() {
    bool end = false;
    int option;
    do{
        //main menu options
        system(CLEAR);
        cout << "MotoGP Fantasy League Manager. Current season: " << seasonName <<  endl;
        cout << "1. Seasons Manager" << endl;
        cout << "2. Members Manager" << endl;
        cout << "3. Riders Manager" << endl;
        cout << "4. Exit" << endl;
        cout << "Option: ";
        cin >> option;
        switch(option){
            case SEASONS_MANAGER: {
                new SeasonMenu(memberList, riderList, &seasonName);
                break;
            }
            case MEMBERS_MANAGER: {
                new MemberMenu(memberList, riderList, seasonName);
                break;
            }
            case RIDERS_MANAGER: {
                new RiderMenu(memberList, riderList, seasonName);
                break;
            }
            case EXIT_MENU: {
                exit();
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

void Menu::exit() {
    //doesn't do anything but could be usefull
}

void Menu::enterToContinue() {
    cout << "Press enter to continue..." << endl;
    getchar();
}
