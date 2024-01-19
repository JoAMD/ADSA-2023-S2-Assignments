// TODO: print actual tree structure to debug rotations

#include <iostream>
#include <cstring>
#include <vector>
#include <math.h>

using namespace std;

bool debug = false;
const int TABLE_SIZE = 26;
const int hashOffset = -97;

// ostream& operator<<(ostream &out, RotationData const& data) {
//     out << data.rootFromLeft << " : ";
//     out << ((data.parentImbRoot == NULL) ? -1 : data.parentImbRoot->data) << " : ";
//     out << data.imbRoot->data << " : ";
//     out << data.imbMid->data << " : ";
//     out << ((data.imbLast == NULL) ? -1 : data.imbLast->data) << " : ";
//     out << data.firstLeft << " : ";
//     out << data.secondLeft;
//     return out;
// }

// enum TypeRotation {
//     LeftLeft, RightRight, LeftRight, RightLeft
// };

enum Status {
    NotUsed,
    Occupied,
    Tombstone
};

class Node {
    public:
    string data;
    Status status;

    Node() {
        this->status = NotUsed;
        this->data = "";
    }

    Node(string data) {
        Node();
        this->data = data;
    }
};



struct SearchResult {
    int actualLoc;
    int potentialLoc;

    SearchResult(int actualLoc, int potentialLoc) {
        this->actualLoc = actualLoc;
        this->potentialLoc = potentialLoc;
    }
};
ostream& operator<<(ostream &out, SearchResult const& searchResult) {
    out << searchResult.actualLoc << " " << searchResult.potentialLoc;
    return out;
}

class HashTable {
    Node table[TABLE_SIZE];
    
    public:
    HashTable() {
        // table = new Node[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; ++i) {
            table[i] = Node();
        }
    }

    void printTable() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            cout << ((table[i].status == Occupied) ? table[i].data + " " : "");
        }
        cout << endl;
    }

    void insertNode(string newData) {
        //search for thingymajig

        SearchResult searchResult = search(newData);
        if (debug) {
            cout << "insertNode() location=" << searchResult << endl;
        }
        if (searchResult.actualLoc != -1) {
            if (debug) {
                cout << "found!, not inserting\n";
            }
            return;
        }

        if (debug) {
            cout << "Inserting at hashValue: " << searchResult.potentialLoc << endl;
        }
        table[searchResult.potentialLoc].data = newData;
        table[searchResult.potentialLoc].status = Occupied;
    }

    bool deleteNode(string dataToDelete) {
        if (table == nullptr) {
            return false;
        }

        int location = search(dataToDelete).actualLoc;

        if (location == -1) {
            return false;
        }

        table[location].status = Tombstone;
        // table[location].data = "";
        
        return true;
    }

    private:
    int getHashValue(string data) {
        char hash = data[data.length() - 1];
        return ((int) hash) + hashOffset;
    }

    SearchResult search(string data) {
        int i;
        int hashValue = getHashValue(data);
        if (table[hashValue].status == NotUsed) {
            return SearchResult(-1, hashValue);
        }
        else {
            i = hashValue;
            do {
                // cout << "i=" << i << endl;
                if (table[i].status == Occupied) {
                    // cout << "Occupied\n";
                    if (table[i].data == data) {
                        // cout << "found!";
                        return SearchResult(i, -1);
                    }
                }
                else {
                    //tombised or not used
                    break;
                }
                i++;
                if (i >= TABLE_SIZE) {
                    i = 0;
                    // cout << "reset i to 0\n";
                }
            } while (hashValue != i);
        }
        return SearchResult(-1, i);
    }

};

int main(int argc, char* argv[]) {
    // int tp = 0;
    // string s;
    // getline(cin, s);
    // while ((tp = s.find_first_of(' ', tp + 1)) && tp != -1) {
    //     //cout << tp << endl;
    // }
    // cout << (int) true + (int) false;
    // perror("test perror");
    // cout << "111\n";
    // exit(EXIT_FAILURE);
    // Node* n1_ = new Node(1);
    // Node* n1 = n1_->leftChild;
    // Node* n2 = n1;
    // cout << (n2 == NULL);
    // cout << true << endl;
    // cout << false << endl;
    // cout << 'a' << endl;
    // cout << (int)'a' << endl;
    // cout << (97+25) << endl;
    // cout << (char)(122) << endl;
    // HashTable *hashTable = new HashTable();
    // hashTable->printTable();
    
    // return 0;

    if (argc > 1) {
        debug = true;
    }

    string input;
    getline(cin, input);

    vector<string> mods;
    int pos = 0;
    int prevPos = 0;
    string oneInput;

    while ((pos = input.find_first_of(' ', pos)) && pos != -1) {
        pos = input.find_first_of(' ', pos);
        // cout << "prev pos: " << prevPos << endl; 
        // cout << "pos: " << pos << endl; 
        oneInput = input.substr(prevPos, pos - prevPos);
        // cout << "oneInput: " << oneInput << endl; 

        // if (input[input.length() - 1] == '\r\n') {
        //     break;
        // }
        mods.push_back(oneInput);
        pos++;
        prevPos = pos;
    }

    mods.push_back(input.substr(prevPos, input.length() - prevPos));

    // cout << "mods vector:\n";
    // for (string s : mods)
    // {
    //     cout << s << "|";
    // }
    // cout << endl;
    
    HashTable hashTbl;
    for (long unsigned int i = 0; i < mods.size(); ++i) {
        // cout << mods[i] << " | ";
        string mod = mods[i];
        char operation = mod[0];
        string value;
        if (debug) {
            cout << "mod=" << mod << endl;
        }
        if (operation == 'A') {
            value = mod.substr(1, mod.length() - 1);
            hashTbl.insertNode(value);
        }
        else {
            // assuming D
            value = mod.substr(1, mod.length() - 1);
            bool result = hashTbl.deleteNode(value);
            // if (!result) {
            //     modsDeleteFail.push_back(mod);
            // }
        }
        if (debug) {
            cout << "Printing table: ";
            hashTbl.printTable();
        }
    }
    hashTbl.printTable();

    // cout << "searching " << hashTbl.search("aaa") << endl;
    // cout << "searching " << hashTbl.search("bbb") << endl;
    // cout << "searching " << hashTbl.search("ccc") << endl;
    // cout << "searching " << hashTbl.search("cccc") << endl;
    

    // cout << endl;
    // cout << input << endl;
    // cout << oneInput << endl;
    // cout << pos << endl;
    // cout << prevPos << endl;
    // cout << traversalTypeStr << endl;

    return 0;
}