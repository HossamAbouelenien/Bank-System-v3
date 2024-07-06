#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <iomanip>
#include <fstream>

using namespace std;

void showMainMenu();
void showTransationsMenu();
void showManageUserMenu();
void backToMainMenu();
void login();

const string fileName = "myFile.txt";
const string userFile = "users.txt";


struct sClient {
    string accountNumber;
    string pinCode;
    string Name;
    string Phone;
    double accountBalance;
    bool markToDelet = false;
};

struct stUser {
    string userName;
    string password;
    int permission = 0;
    bool markToDelete = false;
};

enum enMainMenuOptions {
    eShowClientList = 1,
    eAddClient = 2,
    eDeletClient = 3,
    eUpdateClient = 4,
    eFindClient = 5,
    eTransactionsMenu = 6,
    eManageUsers = 7,
    eLogout = 8,
    eExit = 9,
};

enum enTransationsMenuOptions {
    eDeposit = 1,
    eWithdraw = 2,
    eTotalBalance = 3,
    eShowMainMenu = 4,
};

enum enManageUserOptions {
    eShowListUsers = 1,
    eAddNewUser = 2,
    eDeletUser = 3,
    eUpdateUser = 4,
    eFindUser = 5,
    eMainMenu = 6,
};
enum enPermissions {
    epAll = -1,
    epShowClientList = 1,
    epAddClient = 2,
    epDeletClient = 4,
    epUpdateClient = 8,
    epFindClient = 16,
    epTransactionMenu = 32,
    epManageUsers = 64,
};

stUser currentUser;

int readPermissionToSet() {
    int permission = 0;
    char answer = 'n';

    cout << "\nDo you want to give full access ? y/n ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        return -1;
    }

    cout << "\nDo you want to give access to " << endl;

    cout << "\nshow list clients y/n ? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        permission += enPermissions::epShowClientList;
    }

    cout << "\nAdd clients y/n ? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        permission += enPermissions::epAddClient;
    }

    cout << "\nDelete clients y/n ? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        permission += enPermissions::epDeletClient;
    }

    cout << "\nFind clients y/n ? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        permission += enPermissions::epFindClient;
    }

    cout << "\nTransaction Menu  y/n ? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        permission += enPermissions::epTransactionMenu;
    }

    cout << "\nManage users  y/n ? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        permission += enPermissions::epManageUsers;
    }

    return permission;
}

void showAccessDenied() {
    cout << "==================================================" << endl;
    cout << "Access Denied . Contact your Admin for access ." << endl;
    cout << "==================================================" << endl;
}

bool checkAccessPermission(enPermissions permission) {

    if (currentUser.permission == enPermissions::epAll) {
        return true;
    }

    if ((permission & currentUser.permission) == permission) {
        return true;
    }
    else {
        return false;
    }
}

vector<string> splitFunc(string s1, string delimiter = "#//#") {
    int pos = 0;
    string word = "";
    vector<string>vWords;
    while ((pos = s1.find(delimiter)) != string::npos) {
        word = s1.substr(0, pos);
        if (word != "") {
            vWords.push_back(word);
        }
        s1.erase(0, pos + delimiter.length());
    }
    if (s1 != "") {
        vWords.push_back(s1);
    }
    return vWords;
}

sClient convertLineToRecord(string line, string sperator = "#//#") {
    sClient client;
    vector<string>vClient;
    vClient = splitFunc(line, sperator);
    client.accountNumber = vClient[0];
    client.pinCode = vClient[1];
    client.Name = vClient[2];
    client.Phone = vClient[3];
    client.accountBalance = stod(vClient[4]);
    return client;
}

stUser convertLineToUserRecord(string line, string sperator = "#//#") {
    stUser user;
    vector<string>vUser;
    vUser = splitFunc(line, sperator);
    user.userName = vUser[0];
    user.password = vUser[1];
    user.permission = stoi(vUser[2]);

    return user;
}

string convertRecordToLine(sClient client, string delimiter = "#//#") {
    string line = "";
    line += client.accountNumber + delimiter;
    line += client.pinCode + delimiter;
    line += client.Name + delimiter;
    line += client.Phone + delimiter;
    line += to_string(client.accountBalance);
    return line;
}



vector<sClient> loadDataFromFile(string fileName) {
    sClient client;
    fstream file;
    vector<sClient>vClients;
    file.open(fileName, ios::in);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            client = convertLineToRecord(line);
            vClients.push_back(client);
        }
        file.close();
    }
    return vClients;
}

vector<stUser> loadDataFromUserFile(string userFile) {
    vector<stUser>vUsers;
    fstream file;
    string line;
    stUser user;
    file.open(userFile, ios::in);
    if (file.is_open()) {
        while (getline(file, line)) {
            user = convertLineToUserRecord(line);
            vUsers.push_back(user);
        }

        file.close();
    }
    return vUsers;

}

void printClient(sClient client) {
    cout << "| " << left << setw(20) << client.accountNumber;
    cout << "| " << left << setw(20) << client.pinCode;
    cout << "| " << left << setw(20) << client.Name;
    cout << "| " << left << setw(30) << client.Phone;
    cout << "| " << left << setw(20) << client.accountBalance;
}

void printClientInfo(sClient client) {
    cout << "Account Number  : " << client.accountNumber << endl;
    cout << "Pin Code        : " << client.pinCode << endl;
    cout << "Account Name    : " << client.Name << endl;
    cout << "Account Phone   : " << client.Phone << endl;
    cout << "Account Balance : " << client.accountBalance << endl;
}

void showClients() {
    vector<sClient>vClients = loadDataFromFile(fileName);
    for (sClient& c : vClients) {
        printClient(c);
        cout << "------------------------------------------------------------------------------------------------------------------------" << endl;

    }
}

string readAccountNumber() {
    string accountNumber = "";
    cout << "Enter The Account Number :  ";
    cin >> accountNumber;
    return accountNumber;
}

void showClientList() {
    if (!checkAccessPermission(enPermissions::epShowClientList)) {
        showAccessDenied();
        backToMainMenu();
        return;
    }

    cout << "==================================================" << endl;
    cout << "\t\tUsers List Screen" << endl;
    cout << "==================================================" << endl;
    cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "| " << left << setw(20) << "Account Number";
    cout << "| " << left << setw(20) << "Pin Code";
    cout << "| " << left << setw(20) << "Account Name";
    cout << "| " << left << setw(30) << "Phone";
    cout << "| " << left << setw(20) << "Account Balance";
    cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
    showClients();

}

bool checkIfTheClientFound(string fileName, string accountNumber) {
    vector<sClient>vClints = loadDataFromFile(fileName);
    for (sClient& c : vClints) {
        if (c.accountNumber == accountNumber) {
            return true;
        }
    }
    return false;
}

bool checkIfTheClientFound(string fileName, string accountNumber, sClient& client) {
    vector<sClient>vClints = loadDataFromFile(fileName);
    for (sClient& c : vClints) {
        if (c.accountNumber == accountNumber) {
            client = c;
            return true;
        }
    }
    return false;
}

sClient readNewClient() {
    sClient client;
    cout << "Enter Account Number   :  ";
    cin >> client.accountNumber;
    while (checkIfTheClientFound(fileName, client.accountNumber)) {
        cout << "The Client is Already Exit, please Enter Another Account Number ? ";
        cin >> client.accountNumber;
    }
    cout << "Enter Pin Code         :  ";
    cin >> client.pinCode;
    cout << "Enter Name             :  ";
    cin >> client.Name;
    cout << "Enter Phone            :  ";
    cin >> client.Phone;
    cout << "Enter Account Balance  :  ";
    cin >> client.accountBalance;
    return client;
}

sClient readUpdatedClient(string accountNumber) {
    sClient client;
    client.accountNumber = accountNumber;
    cout << "Enter Pin Code         :  ";
    cin >> client.pinCode;
    cout << "Enter Name             :  ";
    cin >> client.Name;
    cout << "Enter Phone            :  ";
    cin >> client.Phone;
    cout << "Enter Account Balance  :  ";
    cin >> client.accountBalance;
    return client;
}

void addClientToFile(string fileName, sClient client) {
    string line;
    fstream file;
    file.open(fileName, ios::out | ios::app);
    if (file.is_open()) {
        line = convertRecordToLine(client);
        file << line << endl;
    }
    file.close();

}

void addNewClient() {
    sClient client;
    client = readNewClient();
    addClientToFile(fileName, client);
}

void addNewClients() {
    cout << "\nAdding New Clients : \n\n";
    char addmore = 'n';
    do {
        addNewClient();
        cout << "Client Added Successfully , do you want to add more Clients ? y/n ";
        cin >> addmore;
    } while (addmore == 'y' || addmore == 'Y');
}

void showAddClient() {

    if (!checkAccessPermission(enPermissions::epAddClient)) {
        showAccessDenied();
        backToMainMenu();
        return;
    }
    cout << "==================================================" << endl;
    cout << "\t\tAdd Clients Screen" << endl;
    cout << "==================================================" << endl;
    addNewClients();
}

void markTheClientToDeleted(vector<sClient>& vClients, string accountNumber) {
    for (sClient& c : vClients) {
        if (c.accountNumber == accountNumber) {
            c.markToDelet = true;
            break;
        }
    }
}

void saveChangesToFile(vector<sClient>vClients, string fileName) {
    string line;
    fstream file;
    file.open(fileName, ios::out);
    if (file.is_open()) {
        for (sClient& c : vClients) {
            if (c.markToDelet == false) {
                line = convertRecordToLine(c);
                file << line << endl;
            }
        }
        file.close();
    }
}

void deleteClient() {
    vector<sClient>vClients = loadDataFromFile(fileName);
    string accountNumber = readAccountNumber();
    sClient client;
    char answer = 'n';
    if (checkIfTheClientFound(fileName, accountNumber, client)) {
        printClientInfo(client);
        cout << "are you sure , you want to delet this client ? y/n  ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y') {
            markTheClientToDeleted(vClients, accountNumber);
            saveChangesToFile(vClients, fileName);
            vClients = loadDataFromFile(fileName);
            cout << "\n\nThe Client has been deleted Successfully !" << endl;
        }
    }
    else {
        cout << "The Client (" << accountNumber << ") is Not Exit" << endl;

    }
}

void DeletClients() {
    cout << "\nDeleting the client : \n" << endl;
    char addmore = 'n';
    do {

        deleteClient();
        cout << "\n\nDo you want to delete more y/n ? ";
        cin >> addmore;
    } while (addmore == 'y' || addmore == 'Y');
}

void showDeletClients() {

    if (!checkAccessPermission(enPermissions::epDeletClient)) {
        showAccessDenied();
        backToMainMenu();
        return;
    }
    cout << "==================================================" << endl;
    cout << "\t\tDelete Clients Screen" << endl;
    cout << "==================================================" << endl;
    DeletClients();
}

void updateClient() {
    vector<sClient>vClients = loadDataFromFile(fileName);
    sClient client;
    char answer = 'n';
    string accountNumber = readAccountNumber();
    if (checkIfTheClientFound(fileName, accountNumber, client)) {
        printClientInfo(client);
        cout << "\nAre you sure to update this client ? y/n ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y') {
            for (sClient& c : vClients) {
                if (c.accountNumber == accountNumber) {
                    c = readUpdatedClient(accountNumber);
                    saveChangesToFile(vClients, fileName);
                    cout << "\n\nThe Client has been Updated Successfully !" << endl;
                    break;
                }
            }
        }
    }
    else {
        cout << "This client(" << accountNumber << ") is Not exit " << endl;
    }
}

void UpdateClients() {
    cout << "\nUpdating the client : \n" << endl;
    char answer = 'n';
    do {
        updateClient();

        cout << "\n\nDo you want to update more Clients ? y/n  ";
        cin >> answer;
    } while (answer == 'y' || answer == 'Y');
}

void showUpdateClients() {

    if (!checkAccessPermission(enPermissions::epUpdateClient)) {
        showAccessDenied();
        return;
    }
    cout << "==================================================" << endl;
    cout << "\t\tUpdate Clients Screen" << endl;
    cout << "==================================================" << endl;
    UpdateClients();
}

void findClient() {
    sClient client;
    string accountNumber = readAccountNumber();
    if (checkIfTheClientFound(fileName, accountNumber, client)) {
        cout << "The following is Client's details : \n";
        printClientInfo(client);
    }
    else {
        cout << "This CLient(" << accountNumber << ") is Not exit. " << endl;
    }
}

void findClients() {
    char answer = 'n';
    do {
        findClient();
        cout << "Do You Want to search about more Clients ?  y/n ";
        cin >> answer;
    } while (answer == 'y' || answer == 'Y');
}

void showFindClients() {

    if (!checkAccessPermission(enPermissions::epFindClient)) {
        showAccessDenied();
        backToMainMenu();
        return;
    }
    cout << "==================================================" << endl;
    cout << "\t\tFind Clients Screen" << endl;
    cout << "==================================================" << endl;
    findClients();
}

void showExit() {
    cout << "==================================================" << endl;
    cout << "\t\tProgram Ends :)" << endl;
    cout << "==================================================" << endl;
}

void depositToClientBalance(vector<sClient>& vClients, string accountNumber, double amount) {
    for (sClient& c : vClients) {
        if (c.accountNumber == accountNumber) {
            c.accountBalance += amount;
            break;
        }
    }
    saveChangesToFile(vClients, fileName);
    cout << "\n\nDone , The Operatino has done Successfully !!\n" << endl;
}

void showDeposit() {
    cout << "==================================================" << endl;
    cout << "\t\tDeposit Screen" << endl;
    cout << "==================================================" << endl;
    char answer = 'n';
    vector<sClient>vClients = loadDataFromFile(fileName);
    string accountNumber = readAccountNumber();
    sClient client;
    while (!checkIfTheClientFound(fileName, accountNumber, client)) {
        cout << "This client(" << accountNumber << ") is Not exit, Enter new account number " << endl;
        cin >> accountNumber;
    }
    cout << "The following is client's details : " << endl;
    printClientInfo(client);
    double amount = 0;
    cout << "\nHow many do you want to deposit ?  ";
    cin >> amount;
    cout << "\nare sure to deposit " << amount << " to the Client " << client.accountNumber << "  y/n  ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        depositToClientBalance(vClients, accountNumber, amount);
    }

}

void showWithdraw() {
    cout << "==================================================" << endl;
    cout << "\t\tWithdraw Screen" << endl;
    cout << "==================================================" << endl;
    char answer = 'n';
    vector<sClient>vClients = loadDataFromFile(fileName);
    string accountNumber = readAccountNumber();
    sClient client;
    while (!checkIfTheClientFound(fileName, accountNumber, client)) {
        cout << "This client(" << accountNumber << ") is Not exit, Enter new account number " << endl;
        cin >> accountNumber;
    }
    cout << "The following is client's details : " << endl;
    printClientInfo(client);
    double amount = 0;
    cout << "\nHow many do you want to withdraw ?  ";
    cin >> amount;
    while (amount > client.accountBalance) {
        cout << "You cannot withdraw " << amount << " from the (" << client.accountNumber << ") you have only " << client.accountBalance << endl;
        cout << "\nHow many do you want to withdraw ?  ";
        cin >> amount;
    }
    cout << "\nare sure to withdraw " << amount << " to the Client " << client.accountNumber << "  y/n  ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y') {
        depositToClientBalance(vClients, accountNumber, amount * -1);
    }
}

void printClientForTransactions(sClient client) {
    cout << "| " << left << setw(37) << client.accountNumber;
    cout << "| " << left << setw(37) << client.Name;
    cout << "| " << left << setw(40) << client.accountBalance;
}

void showClientsForTransactions() {
    vector<sClient>vClients = loadDataFromFile(fileName);
    double total = 0;
    for (sClient& c : vClients) {
        printClientForTransactions(c);
        cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
        total += c.accountBalance;
    }
    cout << "\n\t\t\tTotal Balance =  " << total << endl;
}

void showTotalBalance() {
    cout << "==================================================" << endl;
    cout << "\t\tTotal Balance Screen" << endl;
    cout << "==================================================" << endl;
    cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "| " << left << setw(37) << "Account Number";
    cout << "| " << left << setw(37) << "Account Name";
    cout << "| " << left << setw(40) << "Account Balance";
    cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
    showClientsForTransactions();
}

void backToMainMenu() {
    cout << "\n\nPress any key to go to main menu .....  ";
    system("pause > 0");
    showMainMenu();
}
void backToTransationMenu() {
    cout << "\n\nPress any key to go to Transaction menu .....  ";
    system("pause > 0");
    showTransationsMenu();
}
void backToManageUsersMenu() {
    cout << "\n\nPress any key to go to manage user menu .....  ";
    system("pause > 0");
    showManageUserMenu();
}
void performMainOptions(enTransationsMenuOptions transactionMenuOption) {
    switch (transactionMenuOption) {
    case(enTransationsMenuOptions::eDeposit):
    {
        system("cls");
        showDeposit();
        backToTransationMenu();
        break;
    }
    case(enTransationsMenuOptions::eWithdraw):
    {
        system("cls");
        showWithdraw();
        backToTransationMenu();
        break;
    }

    case(enTransationsMenuOptions::eTotalBalance):
    {
        system("cls");
        showTotalBalance();
        backToTransationMenu();
        break;
    }

    case(enTransationsMenuOptions::eShowMainMenu):
    {
        showMainMenu();
    }
    }
}

short readTransationsOption() {
    cout << "\nChoose, What do you want [ 1 - 4 ]  ?  ";
    int choice = 0;
    cin >> choice;
    return choice;
}

void showTransationsMenu() {

    if (!checkAccessPermission(enPermissions::epTransactionMenu)) {
        showAccessDenied();
        backToMainMenu();
        return;
    }
    system("cls");
    cout << "==================================================" << endl;
    cout << "\t\tTransactions Menu Screen." << endl;
    cout << "==================================================" << endl;
    cout << "\n\t[1] Deposit." << endl;
    cout << "\t[2] Withdraw." << endl;
    cout << "\t[3] TotalBalance." << endl;
    cout << "\t[4] ShowMainMenu." << endl;
    performMainOptions((enTransationsMenuOptions)readTransationsOption());
}

void printUser(stUser user) {
    cout << "| " << left << setw(38) << user.userName;
    cout << "| " << left << setw(38) << user.password;
    cout << "| " << left << setw(38) << user.permission;
}

void showListUsers() {

    cout << "==================================================" << endl;
    cout << "\t\tClients List Screen" << endl;
    cout << "==================================================" << endl;
    cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "| " << left << setw(38) << "User Name";
    cout << "| " << left << setw(38) << "Password";
    cout << "| " << left << setw(38) << "Permission";
    cout << "------------------------------------------------------------------------------------------------------------------------" << endl;


    vector<stUser>vUsers = loadDataFromUserFile(userFile);
    for (stUser& u : vUsers) {
        printUser(u);
    }

}

bool isUserFounded(string userFile, string userName) {
    vector<stUser>vUsers = loadDataFromUserFile(userFile);
    for (stUser& u : vUsers) {
        if (u.userName == userName) {
            return true;
        }
    }
    return false;
}


stUser readUserInfo() {
    stUser user;
    cout << "\nEnter User Name : ";
    getline(cin >> ws, user.userName);
    while (isUserFounded(userFile, user.userName)) {
        cout << "UserName is already Exited , please enter another userName : ";
        getline(cin >> ws, user.userName);
    }
    cout << "\nEnter Password : ";
    getline(cin, user.password);

    user.permission = readPermissionToSet();

    return user;
}

string convertRecordUserToLine(stUser user, string delimiter = "#//#") {
    string line = "";
    line += user.userName + delimiter;
    line += user.password + delimiter;
    line += to_string(user.permission);
    return line;
}

void saveUsersInUserFile(stUser user, string userFile) {

    fstream file;
    file.open(userFile, ios::app | ios::out);
    if (file.is_open()) {
        string line;
        line = convertRecordUserToLine(user, "#//#");
        file << line << endl;
    }
    file.close();

}

void addNewUser() {
    stUser user;
    user = readUserInfo();
    saveUsersInUserFile(user, userFile);
}

void addNewUsers() {
    cout << "==================================================" << endl;
    cout << "\t\tAdd New User Screen" << endl;
    cout << "==================================================" << endl;
    char answer = 'n';
    do {
        addNewUser();
        cout << "\nAdded Successfully, Do you want to add more ? y/n ? ";
        cin >> answer;
    } while (answer == 'y' || answer == 'Y');

}


void markToDelete(stUser user, vector<stUser>& vUsers) {

    for (stUser& u : vUsers) {
        if (u.userName == user.userName) {
            u.markToDelete = true;
        }
    }

}

void saveChangesToUserFile(vector<stUser>vUsers) {


    fstream file;
    file.open(userFile, ios::out);
    if (file.is_open()) {
        string line;
        for (stUser& u : vUsers) {
            if (u.markToDelete == false) {
                line = convertRecordUserToLine(u);
                file << line << endl;
            }
        }

    }
    file.close();
}

void deleteUser() {
    vector<stUser>vUsers = loadDataFromUserFile(userFile);
    stUser user;
    cout << "\nEnter userName to delete it : ";
    getline(cin >> ws, user.userName);
    while (!isUserFounded(userFile, user.userName)) {
        cout << "\nuser " << user.userName << " doesnot exit, please enter another user name ? ";
        getline(cin >> ws, user.userName);
    }
    markToDelete(user, vUsers);
    saveChangesToUserFile(vUsers);
    vUsers = loadDataFromUserFile(userFile);
}

void deleteUsers() {
    cout << "==================================================" << endl;
    cout << "\t\tDelete User Screen" << endl;
    cout << "==================================================" << endl;
    char answer = 'n';
    do {
        deleteUser();
        cout << "\nDeleted Successfully , Do you want to delete more ? y/n ";
        cin >> answer;

    } while (answer == 'y' || answer == 'Y');

}


void readNewInfoForUpdatedUser(stUser user, vector<stUser>& vUsers) {
    for (stUser& u : vUsers) {
        if (u.userName == user.userName) {
            cout << "\nEnter new Password : ";
            getline(cin >> ws, u.password);
            u.permission = readPermissionToSet();
        }
    }
}

void updateUser() {
    vector<stUser>vUsers = loadDataFromUserFile(userFile);
    stUser user;
    cout << "\nEnter user name to update this user : ";
    getline(cin >> ws, user.userName);
    while (!isUserFounded(userFile, user.userName)) {
        cout << "\nUser is " << user.userName << " not Exit, please enter another user name : ";
        getline(cin >> ws, user.userName);
    }
    readNewInfoForUpdatedUser(user, vUsers);
    saveChangesToUserFile(vUsers);
    vUsers = loadDataFromUserFile(userFile);
}

void updateUsers() {
    cout << "==================================================" << endl;
    cout << "\t\tUpdate User Screen" << endl;
    cout << "==================================================" << endl;
    char answer = 'n';
    do {
        updateUser();
        cout << "\nUpdated Successfully , Do you want to update more user ? y/n  ";
        cin >> answer;

    } while (answer == 'y' || answer == 'Y');

}

void findUser() {
    stUser user;
    cout << "\nEnter user name to find this user : ";
    getline(cin >> ws, user.userName);

    if (isUserFounded(userFile, user.userName)) {
        cout << "The user [ " << user.userName << " ] is exit " << endl;
    }
    else {
        cout << "The user [ " << user.userName << " ] is not exit " << endl;
    }


}

void findUsers() {
    cout << "==================================================" << endl;
    cout << "\t\tFind User Screen" << endl;
    cout << "==================================================" << endl;
    char answer = 'n';
    do {
        findUser();
        cout << "\nDo you want to search about more users ? y/n  ";
        cin >> answer;

    } while (answer == 'y' || answer == 'Y');

}
short readManageUserOption() {
    short choice;
    cout << "\nChoose, What do you want [ 1 - 6 ] ? ";
    cin >> choice;
    return choice;
}

void performManageUserMenuOptions(enManageUserOptions manageUserOption) {
    switch (manageUserOption) {
    case(enManageUserOptions::eShowListUsers): {
        system("cls");
        showListUsers();
        backToManageUsersMenu();
        break;
    }
    case(enManageUserOptions::eAddNewUser): {
        system("cls");
        addNewUsers();
        backToManageUsersMenu();
        break;
    }
    case(enManageUserOptions::eDeletUser): {
        system("cls");
        deleteUsers();
        backToManageUsersMenu();
        break;
    }
    case(enManageUserOptions::eUpdateUser): {
        system("cls");
        updateUsers();
        backToManageUsersMenu();
        break;
    }
    case(enManageUserOptions::eFindUser): {
        system("cls");
        findUsers();
        backToManageUsersMenu();
        break;
    }
    case(enManageUserOptions::eMainMenu): {
        system("cls");
        showMainMenu();
        break;
    }
    }
}

void showManageUserMenu() {
 
    if (!checkAccessPermission(enPermissions::epManageUsers)) {
        showAccessDenied();
        backToMainMenu();
        return;
    }
    system("cls");
    cout << "==================================================" << endl;
    cout << "\t\tManage Users Menu Screen." << endl;
    cout << "==================================================" << endl;
    cout << "\n\t[1] List Users." << endl;
    cout << "\t[2] Add New Users." << endl;
    cout << "\t[3] Delete Users." << endl;
    cout << "\t[4] Update Users." << endl;
    cout << "\t[5] Find Users." << endl;
    cout << "\t[6] Main Menu." << endl;
    performManageUserMenuOptions((enManageUserOptions)readManageUserOption());

}

void ExitTheProgram() {
    cout << "==================================================" << endl;
    cout << "\t\tExit Screen." << endl;
    cout << "==================================================" << endl;
    cout << "\nEnter any key ..." << endl;
    system("pause > 0");
}

void performMainOptions(enMainMenuOptions mainMenuOption) {
    switch (mainMenuOption) {
    case (enMainMenuOptions::eShowClientList):
    {
        system("cls");
        showClientList();
        backToMainMenu();
        break;
    }
    case(enMainMenuOptions::eAddClient):
    {
        system("cls");
        showAddClient();
        backToMainMenu();
        break;
    }
    case(enMainMenuOptions::eDeletClient):
    {
        system("cls");
        showDeletClients();
        backToMainMenu();
        break;
    }
    case(enMainMenuOptions::eUpdateClient):
    {
        system("cls");
        showUpdateClients();
        backToMainMenu();
        break;
    }
    case(enMainMenuOptions::eFindClient):
    {
        system("cls");
        showFindClients();
        backToMainMenu();
        break;
    }
    case(enMainMenuOptions::eTransactionsMenu):
    {
        system("cls");
        showTransationsMenu();
        break;
    }
    case(enMainMenuOptions::eManageUsers):
    {
        system("cls");
        showManageUserMenu();
        break;
    }
    case(enMainMenuOptions::eLogout):
    {
        system("cls");
        login();
        break;
    }

    case(enMainMenuOptions::eExit):
    {
        system("cls");
        ExitTheProgram();
        break;
    }
    }
}

short readMainOption() {
    cout << "Choose , What do you want to do [ 1 to 9 ] ? ";
    short choice = 0;
    cin >> choice;
    return choice;
}
void showMainMenu() {
    system("cls");
    cout << "==================================================" << endl;
    cout << "\t\tMain Menu Screen." << endl;
    cout << "==================================================" << endl;
    cout << "\n\t[1] Show Client List." << endl;
    cout << "\t[2] Add New Client." << endl;
    cout << "\t[3] Delet Client." << endl;
    cout << "\t[4] Update Client." << endl;
    cout << "\t[5] Find Client." << endl;
    cout << "\t[6] Transactions Menu." << endl;
    cout << "\t[7] ManageUsers." << endl;
    cout << "\t[8] Logout." << endl;
    cout << "\t[9] Exit.\n" << endl;
    performMainOptions((enMainMenuOptions)readMainOption());

}

bool checkInfoOfUser(string userName, string password, stUser& user) {

    vector<stUser>vUsers = loadDataFromUserFile(userFile);
    for (stUser& u : vUsers) {
        if ((u.userName == userName) && (u.password == password)) {
            user = u;
            return true;
        }
    }
    return false;
}

bool loginUserInfo(string userName, string password) {
    if (checkInfoOfUser(userName, password, currentUser)) {
        return true;
    }
    else {
        return false;
    }
}

void login() {
    system("cls");
    cout << "==================================================" << endl;
    cout << "\t\tLogin Screen." << endl;
    cout << "==================================================" << endl;

    bool loginFailed = false;
    string userName, password;
    do {

        if (loginFailed) {
            cout << "\nInvailed UserName/Password" << endl;
        }
        cout << "\nEnter UserName :  ?  ";
        cin >> userName;

        cout << "\nEnter Password :  ?  ";
        cin >> password;

        loginFailed = !loginUserInfo(userName, password);

    } while (loginFailed);

    showMainMenu();

}

int main()
{

    login();
    system("pause > 0");
    return 0;
}