#include <iostream>
#include <map>
#include <stdexcept>
#include <sstream>
using namespace std;





class InMemoryDB{

private:

    // transaction status in order to track transactions
    bool status = 0;

    // maps in order to store the information and pending transactions
    map<string, int> main;
    map<string, int> transactions;

public:
    int get(const string& key){
        if (status && transactions.find(key) != transactions.end()) {
            return transactions[key];
        } else if (!status && main.find(key) != main.end()) {
            return main[key];
        }
        return -1;
    }

    void put(const string& key, int value){
        if(status){
            transactions[key] = value;
        }
        else
        {
            throw runtime_error("Transaction not in progress");
        }
    }

    void begin_transaction(){
        if(!status){
            status = 1;
            transactions.clear();
        }
        else{
            throw runtime_error("Transaction not in progress");
        }
    }


    void rollback(){
        if (status){
            status = 0;
            transactions.clear();
        }
        else{
            throw runtime_error("Transaction not in progress");
        }
    }


    void changedatabase() {
        for (const auto& k : transactions) {
            main[k.first] = k.second;
        }
    }

    void commit() {
        if (status) {
            changedatabase();
            status= false;
            transactions.clear();
        }
        else{
            throw runtime_error("Transaction not in progress");
        }
    }





};
int main() {
    string line;
    InMemoryDB inmemoryDB;


    while(true) {
        cout << "Enter a Command: ";
        getline(cin, line);

        istringstream iss(line);
        string command;
        iss >> command;

        if (command == "get") {
            string key;
            iss >> key;
            cout << "Result: " << inmemoryDB.get(key) << endl;
        } else if (command == "put") {
            string key;
            int value;
            iss >> key >> value;
            inmemoryDB.put(key, value);
        } else if (command == "begin") {
            inmemoryDB.begin_transaction();
            cout << "Transaction started." << endl;
        } else if (command == "commit") {
            inmemoryDB.commit();
            cout << "Transaction committed." << endl;
        } else if (command == "rollback") {
            inmemoryDB.rollback();
            cout << "Transaction rolled back." << endl;
        } else {
            cout << "Invalid Command";
        }
    }

    return 0;}