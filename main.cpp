#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
using namespace std;

struct Account {
    char accNo[20];
    char name[30];
    char accType[15];
    double balance;
};

/* ---------- CHECK DIGITS ---------- */
bool isDigitsOnly(const char* str) {
    for (int i = 0; str[i] != '\0'; i++)
        if (!isdigit(str[i])) return false;
    return true;
}

/* ---------- CHECK DUPLICATE ACCOUNT ---------- */
bool accountExists(const char* fileName, const char* accNo) {
    Account a;
    fstream file(fileName, ios::in | ios::binary);
    if (!file) return false;

    while (file.read((char*)&a, sizeof(a))) {
        if (strcmp(a.accNo, accNo) == 0) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

/* ---------- CREATE ACCOUNT ---------- */
void createAccount(const char* fileName) {
    Account a;

    cout << "\n--- Create Account ---\n";
    cout << "Enter Account Number: ";
    cin >> a.accNo;

    if (!isDigitsOnly(a.accNo) || accountExists(fileName, a.accNo)) {
        cout << "❌ Invalid or duplicate account number!\n";
        return;
    }

    cout << "Enter Name: ";
    cin >> a.name;

    cout << "Enter Account Type (saving/current): ";
    cin >> a.accType;

    do {
        cout << "Enter Initial Balance (>=100): ";
        cin >> a.balance;
    } while (a.balance < 100);

    fstream file(fileName, ios::out | ios::app | ios::binary);
    file.write((char*)&a, sizeof(a));
    file.close();

    cout << "✅ Account created successfully!\n";
}

/* ---------- DISPLAY ALL ---------- */
void displayAccounts(const char* fileName) {
    Account a;
    fstream file(fileName, ios::in | ios::binary);

    if (!file) {
        cout << "No accounts found.\n";
        return;
    }

    cout << "\n===== ACCOUNT LIST =====\n";
    while (file.read((char*)&a, sizeof(a))) {
        cout << "Account No : " << a.accNo << endl;
        cout << "Name       : " << a.name << endl;
        cout << "Type       : " << a.accType << endl;
        cout << "Balance    : " << a.balance << endl;
        cout << "-------------------------\n";
    }
    file.close();
}

/* ---------- SEARCH ACCOUNT ---------- */
void searchAccount(const char* fileName) {
    char searchNo[20];
    Account a;
    bool found = false;

    cout << "Enter Account Number to search: ";
    cin >> searchNo;

    fstream file(fileName, ios::in | ios::binary);

    while (file.read((char*)&a, sizeof(a))) {
        if (strcmp(a.accNo, searchNo) == 0) {
            cout << "\n✅ Account Found!\n";
            cout << "Name    : " << a.name << endl;
            cout << "Type    : " << a.accType << endl;
            cout << "Balance : " << a.balance << endl;
            found = true;
            break;
        }
    }

    if (!found)
        cout << "❌ Account not found!\n";

    file.close();
}

/* ---------- DEPOSIT ---------- */
void deposit(const char* fileName) {
    char accNo[20];
    double amount;
    Account a;
    bool found = false;

    cout << "Enter Account Number: ";
    cin >> accNo;

    cout << "Enter Deposit Amount: ";
    cin >> amount;

    if (amount <= 0) {
        cout << "❌ Invalid amount!\n";
        return;
    }

    fstream file(fileName, ios::in | ios::out | ios::binary);

    while (file.read((char*)&a, sizeof(a))) {
        if (strcmp(a.accNo, accNo) == 0) {
            a.balance += amount;
            file.seekp(-sizeof(a), ios::cur);
            file.write((char*)&a, sizeof(a));
            cout << "✅ Deposit successful!\n";
            found = true;
            break;
        }
    }

    if (!found)
        cout << "❌ Account not found!\n";

    file.close();
}

/* ---------- WITHDRAW ---------- */
void withdraw(const char* fileName) {
    char accNo[20];
    double amount;
    Account a;
    bool found = false;

    cout << "Enter Account Number: ";
    cin >> accNo;

    cout << "Enter Withdraw Amount: ";
    cin >> amount;

    fstream file(fileName, ios::in | ios::out | ios::binary);

    while (file.read((char*)&a, sizeof(a))) {
        if (strcmp(a.accNo, accNo) == 0) {
            if (a.balance >= amount) {
                a.balance -= amount;
                file.seekp(-sizeof(a), ios::cur);
                file.write((char*)&a, sizeof(a));
                cout << "✅ Withdrawal successful!\n";
            } else {
                cout << "❌ Insufficient balance!\n";
            }
            found = true;
            break;
        }
    }

    if (!found)
        cout << "❌ Account not found!\n";

    file.close();
}

/* ---------- DELETE ACCOUNT ---------- */
void deleteAccount(const char* fileName) {
    char accNo[20];
    Account a;
    bool found = false;

    cout << "Enter Account Number to delete: ";
    cin >> accNo;

    fstream file(fileName, ios::in | ios::binary);
    fstream temp("temp.dat", ios::out | ios::binary);

    while (file.read((char*)&a, sizeof(a))) {
        if (strcmp(a.accNo, accNo) != 0)
            temp.write((char*)&a, sizeof(a));
        else
            found = true;
    }

    file.close();
    temp.close();

    remove(fileName);
    rename("temp.dat", fileName);

    if (found)
        cout << "✅ Account deleted successfully!\n";
    else
        cout << "❌ Account not found!\n";
}

/* ---------- MAIN ---------- */
int main() {
    const char* fileName = "accounts.dat";
    int choice;

    while (true) {
        cout << "\n===== BANK MENU =====\n";
        cout << "1. Create Account\n";
        cout << "2. Display All Accounts\n";
        cout << "3. Search Account\n";
        cout << "4. Deposit Money\n";
        cout << "5. Withdraw Money\n";
        cout << "6. Delete Account\n";
        cout << "7. Exit\n";
        cout << "Enter choice: ";

        cin >> choice;

        switch (choice) {
        case 1: createAccount(fileName); break;
        case 2: displayAccounts(fileName); break;
        case 3: searchAccount(fileName); break;
        case 4: deposit(fileName); break;
        case 5: withdraw(fileName); break;
        case 6: deleteAccount(fileName); break;
        case 7: return 0;
        default: cout << "❌ Invalid choice!\n";
        }
    }
}

