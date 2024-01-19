// TODO: print actual tree structure to debug rotations

#include <iostream>
#include <cstring>
#include <vector>
#include <math.h>

using namespace std;

bool debug = false;

class Node {
    public:

    int data;
    Node* leftChild;
    Node* rightChild;
    int balanceFactor;

    public:
    
    Node(int data) {
        this->data = data;
        leftChild = NULL;
        rightChild = NULL;
    }
};

struct RotationData {
    bool rootFromLeft;
    Node* parentImbRoot;
    Node* imbRoot;
    Node* imbMid;
    Node* imbLast;
    bool firstLeft;
    bool secondLeft;

    RotationData() {
        rootFromLeft = false;
        parentImbRoot = NULL;
        imbRoot = NULL;
        imbMid = NULL;
        imbLast = NULL;
        firstLeft = false;
        secondLeft = false;
    }

    RotationData(bool rootFromLeft_, Node* parentImbRoot_, Node* imbRoot_, Node* imbMid_, Node* imbLast_, bool firstLeft_, bool secondLeft_) {
        rootFromLeft = rootFromLeft_;
        parentImbRoot = parentImbRoot_;
        imbRoot = imbRoot_;
        imbMid = imbMid_;
        imbLast = imbLast_;
        firstLeft = firstLeft_;
        secondLeft = secondLeft_;
    }
};


// ostream& operator<<(ostream &out, Node const& node) {
//     out << (node == NULL) ? NULL : node.data;
//     return out;
// }

ostream& operator<<(ostream &out, RotationData const& data) {
    out << data.rootFromLeft << " : ";
    out << ((data.parentImbRoot == NULL) ? -1 : data.parentImbRoot->data) << " : ";
    out << data.imbRoot->data << " : ";
    out << data.imbMid->data << " : ";
    out << ((data.imbLast == NULL) ? -1 : data.imbLast->data) << " : ";
    out << data.firstLeft << " : ";
    out << data.secondLeft;
    return out;
}

// enum TypeRotation {
//     LeftLeft, RightRight, LeftRight, RightLeft
// };

class HashTable {
    Node* head;
    
    public:
    HashTable() {
        head = NULL;
    }

    int getHeadData() {
        return head == NULL ? -1 : head->data;
    }

    void printTreeStructure() {
        if (debug) {
            printTreeStructure(head);
        }
    }
    // Function to print the hierarchical structure of a binary tree
    void printTreeStructure(Node* root, const std::string& prefix = "", bool isLeft = false) {
        if (root == nullptr) {
            return;
        }

        std::cout << prefix;
        std::cout << (isLeft ? "├── " : "└── ");
        std::cout << root->data << std::endl;

        // Create prefixes for child nodes
        std::string childPrefix = prefix + (isLeft ? "│   " : "    ");

        // Recursively print the left and right subtrees
        printTreeStructure(root->leftChild, childPrefix, true);
        printTreeStructure(root->rightChild, childPrefix, false);
    }

    // void insertNode(int newData) {
    //     Node* newNode = new Node(newData);

    //     Node* currNode = head;
    //     while (true) { //currNode != NULL
            
    //     }
    //     currNode = newNode; // ......
    // }

    void insertNode(int newData) {
        if (head == NULL) {
            Node* newNode = new Node(newData);
            head = newNode;
        }
        else {
            insertNode(head, newData);
        }
        // cout << endl;
        // traversalInOrder();
        // printTreeStructure();
        reCalcAndRotate();
        reCalcAndRotate();
        // traversalInOrder();
        // printTreeStructure();
        // cout << "left=" << (head != NULL ? (head->leftChild ? head->leftChild->data : -1) : -1) << " ";
        // cout << "right=" << (head != NULL ? (head->rightChild ? head->rightChild->data : -1) : -1) << endl;
    }

    void traversalInOrder() {
        if (head == NULL) {
            cout << "EMPTY\n";
            return;
        }
        traversalInOrder(head);
        cout << endl;
    }

    void traversalPreOrder() {
        if (head == NULL) {
            cout << "EMPTY\n";
            return;
        }
        traversalPreOrder(head);
        cout << endl;
    }

    void traversalPostOrder() {
        if (head == NULL) {
            cout << "EMPTY\n";
            return;
        }
        traversalPostOrder(head);
        cout << endl;
    }

    bool deleteNode(int dataToDelete) {
        bool result = false;
        if (head == NULL) {

        }
        //to handle delete head
        //parent NULL to be handled in below fn call

        //doesnt work
        // else if (head->data == dataToDelete) {
        //     delete head;
        //     head = NULL;
        //     result = true;
        // }
        else {
            result = deleteNode(head, dataToDelete, NULL, false);
        }
        // traversalInOrder();
        // printTreeStructure();
        // if (result) {
            reCalcAndRotate();
            reCalcAndRotate();
        // }
        // traversalInOrder();
        // printTreeStructure();
        return result;
    }

    void reCalcAndRotate() {
        printTreeStructure();
        calculateBalanceFactor(head);
        // cout << endl;
        
        RotationData* rotationData = findRootImbalanceNode(head);
        // cout << endl;
        if (rotationData == NULL) {
            // cout << "No imbalance!\n";
            return;
        }
        // cout << "imbRoot: " << rotationData->imbRoot->data << endl;
        // cout << "parentImbRoot: " << rotationData->parentImbRoot->data << endl;
        getRotationData(rotationData);
        if (debug) {
            cout << "rot data: " << *rotationData << endl;
        }
        rotate(*rotationData);
    }

    private:
    RotationData* findRootImbalanceNode(Node* curr) {
        if (curr == NULL) {
            return NULL;
        }

        RotationData* rotationData = findRootImbalanceNode(curr->leftChild);
        if (rotationData != NULL) {
            if (rotationData->parentImbRoot == NULL) {
                rotationData->parentImbRoot = curr;
                // cout << "from left rotationData->parentImbRoot = curr=" << curr->data;
                rotationData->rootFromLeft = true;
            }
            return rotationData;
        }
        rotationData = findRootImbalanceNode(curr->rightChild);
        if (rotationData != NULL) {
            if (rotationData->parentImbRoot == NULL) {
                rotationData->parentImbRoot = curr;
                // cout << "from right rotationData->parentImbRoot = curr=" << curr->data;
                rotationData->rootFromLeft = false;
            }
            return rotationData;
        }
        // cout << curr->data << "=" << curr->balanceFactor << " ";
        if (abs(curr->balanceFactor) > 1) {
            //found first node with imbalance
            RotationData* rotationData = new RotationData();
            rotationData->imbRoot = curr;
            return rotationData;
        }
        return rotationData;
    }

    void getRotationData(RotationData* rotationData) {
        Node* imbRoot = rotationData->imbRoot;
        Node* imbMid;
        if (imbRoot->balanceFactor > 0) {
            //go left
            rotationData->firstLeft = true;
            rotationData->imbMid = imbRoot->leftChild;
        }
        else {
            //go right
            rotationData->firstLeft = false;
            rotationData->imbMid = imbRoot->rightChild;
        }
        
        imbMid = rotationData->imbMid;
        int midBalanceFactor = imbMid->balanceFactor;
        if (midBalanceFactor == 0) {
            rotationData->secondLeft = rotationData->firstLeft;
            rotationData->imbLast = rotationData->secondLeft ? 
                imbMid->leftChild :
                imbMid->rightChild;
        }
        else if (midBalanceFactor > 0) {
            //go left
            rotationData->secondLeft = true;
            rotationData->imbLast = imbMid->leftChild;
        }
        else {
            //go right
            rotationData->secondLeft = false;
            rotationData->imbLast = imbMid->rightChild;
        }
    }

    // RotationData* getRotationDataOld(Node* imbNode) {
    //     RotationData* rotationData = new RotationData();
    //     rotationData->imbRoot = imbNode;
    //     bool isLeft;
    //     for (int i = 0; i < 2; ++i) {
    //         bool hasNoLeft = imbNode->leftChild == NULL;
    //         bool hasNoRight = imbNode->rightChild == NULL;
    //         // cout << "hasNoLeft=" << hasNoLeft << endl;
    //         // cout << "hasNoRight=" << hasNoRight << endl;
    //         isLeft = false;
    //         //atleast one child needed for imbalance
    //         if (hasNoLeft && hasNoRight) {
    //             perror("atleast one child needed for imbalance!");
    //             exit(EXIT_FAILURE);
    //         }
    //         if (hasNoLeft) {
    //             imbNode = imbNode->rightChild; //
    //         }
    //         else if (hasNoRight) {
    //             imbNode = imbNode->leftChild; //
    //             isLeft = true;
    //         }
    //         else {
    //             int left = abs(imbNode->leftChild->balanceFactor);
    //             int right = abs(imbNode->rightChild->balanceFactor);
    //             if (right > left) {
    //                 imbNode = imbNode->rightChild; //
    //             }
    //             else {
    //                 imbNode = imbNode->leftChild; //
    //                 isLeft = true;
    //             }
    //         }
    //         if (i == 0) {
    //             if (imbNode == NULL) {
    //                 perror("mid imbNode is null");
    //                 exit(EXIT_FAILURE);
    //             }
    //             rotationData->imbMid = imbNode;
    //             rotationData->firstLeft = isLeft;
    //         }
    //     }
    //     if (imbNode == NULL) {
    //         perror("last imbNode is null");
    //         exit(EXIT_FAILURE);
    //     }
    //     rotationData->imbLast = imbNode;
    //     rotationData->secondLeft = isLeft;
    //     return rotationData;
    //     // Basically, just store rotation type data, pass to new function which actually does the rotation. And implement 4 rotations separately probably.. and test each one with multiple inputs
    // }

    void rotate(RotationData data) {
        Node* parentImbRoot = data.parentImbRoot;
        Node* imbRoot = data.imbRoot;
        Node* imbMid = data.imbMid;
        // Node* imbLast = data.imbLast;

        // doing L.L first
        if (data.firstLeft && data.secondLeft) {
            // cout << "LL imb, 1 right rot";
            // left left imbalance
            //1 right rotation
            // mid becomes root

            //assigning parent left or right child to mid
            if (parentImbRoot == NULL) {
                head = imbMid;
            }
            else {
                if (data.rootFromLeft) {
                    parentImbRoot->leftChild = imbMid;
                }
                else {
                    parentImbRoot->rightChild = imbMid;
                }
            }
            //taking care of mid right
            Node* midRight = imbMid->rightChild;
            imbRoot->leftChild = midRight; // ... imbRoot->leftChild because data.firstLeft == 1
            imbMid->rightChild = imbRoot; 
            /*
                     3
                1
                    2

                        3
                    2
                1
            */
        }
        else if (!data.firstLeft && !data.secondLeft) {
            // cout << "RR imb, 1 left rot";
            //right right imbalance
            //1 left rotation
            // mid becomes root

            //assigning parent left or right child to mid // same as before
            if (parentImbRoot == NULL) {
                head = imbMid;
            }
            else {
                if (data.rootFromLeft) {
                    // cout << "parentImbRoot->leftChild=imbMid, imbMid=" << (imbMid->data == NULL ? -1 : imbMid->data);
                    parentImbRoot->leftChild = imbMid;
                }
                else {
                    // cout << "parentImbRoot->rightChild=imbMid, imbMid=" << (imbMid->data == NULL ? -1 : imbMid->data);
                    parentImbRoot->rightChild = imbMid;
                }
            }
            // cout << "t\n";
            //taking care of mid left
            Node* midLeft = imbMid->leftChild;
            imbRoot->rightChild = midLeft; // ... imbRoot->rightChild because data.firstLeft == 0
            imbMid->leftChild = imbRoot; 
        }
        else if (data.firstLeft && !data.secondLeft) {
            //Left right imbalance
            //1 left rot, //y, x, T3 ....
            //1 right rot (same as case 1 above)
            RotationData* firstRotData = new RotationData(
                true, data.imbRoot, data.imbMid, data.imbLast, NULL, false, false 
            );
            if (debug) {
                cout << "rot data: " << (*firstRotData) << endl;
            }
            rotate(*firstRotData);
            data.secondLeft = !data.secondLeft;
            Node* temp = data.imbLast;
            data.imbLast = data.imbMid;
            data.imbMid = temp;
            if (debug) {
                cout << "rot data: " << data << endl;
            }
            rotate(data);

        }
        else if (!data.firstLeft && data.secondLeft) {
            //Right left imbalance
            //1 right rot, 
            //1 left rot (same as case 1 above)
            RotationData* firstRotData = new RotationData(
                false, data.imbRoot, data.imbMid, data.imbLast, NULL, true, true 
            );
            if (debug) {
                cout << "rot data: " << (*firstRotData) << endl;
            }
            rotate(*firstRotData);
            data.secondLeft = !data.secondLeft;
            Node* temp = data.imbLast;
            data.imbLast = data.imbMid;
            data.imbMid = temp;
            if (debug) {
                cout << "rot data: " << data << endl;
            }
            rotate(data);
        }
    }

    int calculateBalanceFactor(Node* curr) {
        if (curr == NULL) {
            return 1;
        }

        int left = calculateBalanceFactor(curr->leftChild);
        int right = calculateBalanceFactor(curr->rightChild);
        curr->balanceFactor = left - right;
        if (debug) {
            cout << curr->data << "=" << left << "/" << right << " ";
        }
        // cout << "curr->leftChild=" << (curr->leftChild == NULL ? -1 : curr->leftChild->data) << endl;
        // cout << "curr->rightChild=" << (curr->rightChild == NULL ? -1 : curr->rightChild->data) << endl;
        return max(left, right) + 1;
    }

    void insertNode(Node* node, int newData) {
        int nodeData = node->data;
        // cout << "nodeData=" << nodeData << " ";
        if (newData == nodeData) {
            return;
        }
        else if (newData < nodeData) {
            Node* leftC = node->leftChild;
            if (leftC == NULL) {
                Node* newNode = new Node(newData);
                node->leftChild = newNode;
                if (debug) {
                    cout << "parent=" << node->data << endl;
                }
                return;
            }
            else {
                insertNode(leftC, newData);                
            }
        }
        else {
            Node* rightC = node->rightChild;
            if (rightC == NULL) {
                Node* newNode = new Node(newData);
                node->rightChild = newNode;
                return;
            }
            else {
                insertNode(rightC, newData);                
            }
        }
    }

    void traversalInOrder(Node* curr) {
        if (curr == NULL) {
            return;
        }

        traversalInOrder(curr->leftChild);
        cout << curr->data << " ";
        traversalInOrder(curr->rightChild);
    }

    void traversalPreOrder(Node* curr) {
        if (curr == NULL) {
            return;
        }
        
        cout << curr->data << " ";
        traversalPreOrder(curr->leftChild);
        traversalPreOrder(curr->rightChild);
    }

    void traversalPostOrder(Node* curr) {
        if (curr == NULL) {
            return;
        }

        traversalPostOrder(curr->leftChild);
        traversalPostOrder(curr->rightChild);
        cout << curr->data << " ";
    }

    void deleteNode2Children(Node* nodeToDelete, Node* parentOfNodeToDelete, bool fromLeft) {
        //find max in left subtree and swap (use delete fn again..)
        //else
        //find min in right subtree and swap (use delete fn again..)

        Node* nMax = nodeToDelete->leftChild;
        Node* parentNMax = nodeToDelete;
        bool fromLeftNMax = true;
        while (true) {
            if (nMax->rightChild == NULL) {
                break;
            }
            parentNMax = nMax;
            nMax = nMax->rightChild;
            fromLeftNMax = false;
        }

        //can't call deleteNode fn on nMax since it will deallocate memory
        if (fromLeftNMax) {
            parentNMax->leftChild = nMax->leftChild;
        }
        else {
            parentNMax->rightChild = nMax->leftChild;
        }
        
        if (parentOfNodeToDelete == NULL) {
            head = nMax;
            // cout << "|head = nMax|";
        }
        else {
            if (fromLeft) {
                parentOfNodeToDelete->leftChild = nMax;
            }
            else {
                parentOfNodeToDelete->rightChild = nMax;
            }
        }

        nMax->rightChild = nodeToDelete->rightChild;
        // cout << "nodeToDelete->leftChild == nMax? " << (nodeToDelete->leftChild == nMax);
        // cout << "fromLeftNMax=" << fromLeftNMax;
        if (nodeToDelete->leftChild != nMax) {
            nMax->leftChild = nodeToDelete->leftChild;
        }

        // delete nodeToDelete; DO NOT DELETE, already taken care of in calling fn

    }

    bool deleteNode(Node* currNode, int dataToDelete, Node* parent, bool fromLeft) {
        int nodeData = currNode->data;
        if (dataToDelete == nodeData) {
            //found!
            //logic to delete
            if (debug) {
                cout << "found\n";
            }
            bool hasLeftC = currNode->leftChild != NULL;
            bool hasRightC = currNode->rightChild != NULL;
            int children = (int) (hasLeftC) + (int) (hasRightC); 
            if (children == 0) {
                //leaf node, ez
                if (parent == NULL) {
                    //have to assign head
                    head = NULL;
                }
                else {
                    if (fromLeft) {
                        parent->leftChild = NULL;
                    }
                    else {
                        parent->rightChild = NULL;
                    }
                }
            }
            else if (children == 1) {
                //assign to parent
                //better way than so many nested if(s)?
                if (parent == NULL) {
                    // head == currNode
                    head = hasLeftC ? currNode->leftChild : currNode->rightChild;
                }
                else {
                    if (fromLeft) {
                        if (hasLeftC) {
                            parent->leftChild = currNode->leftChild;
                        }
                        else {
                            parent->leftChild = currNode->rightChild;
                        }
                    }
                    else {
                        if (hasLeftC) {
                            parent->rightChild = currNode->leftChild;
                        }
                        else {
                            parent->rightChild = currNode->rightChild;
                        }
                    }
                }
            }
            else {
                //2 children
                deleteNode2Children(currNode, parent, fromLeft);
            }
            // cout << children << endl;
            
            delete currNode;
            
            return true;
        }
        else if (dataToDelete < nodeData) {
            Node* leftC = currNode->leftChild;
            if (leftC == NULL) {
                //didnt find
                if (debug) {
                    cout << "didnt find\n";
                }
                return false;
            }
            else {
                if (debug) {
                    cout << "lc nodD: " << nodeData << "\n";
                }
                deleteNode(leftC, dataToDelete, currNode, true);                
            }
        }
        else {
            Node* rightC = currNode->rightChild;
            if (rightC == NULL) {
                //didnt find
                if (debug) {
                    cout << "didnt find\n";
                }
                return false;
            }
            else {
                if (debug) {
                    cout << "rc nodD: " << nodeData << "\n";
                }
                deleteNode(rightC, dataToDelete, currNode, false);                
            }
        }
        return false;
    }

    // Node* searchNode(int dataToSearch) {

    // }

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
    // return 0;

    if (argc > 1) {
        debug = true;
    }

    string input;
    getline(cin, input);

    vector<string> mods;
    vector<string> modsDeleteFail;
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

    string traversalTypeStr = input.substr(prevPos, input.length() - prevPos);

    // cout << "mods vector:\n";
    // for (string s : mods)
    // {
    //     //cout << s << "|";
    // }
    

    HashTable avl = HashTable(); 

    for (long unsigned int i = 0; i < mods.size(); ++i) {
        // cout << mods[i] << " | ";
        string mod = mods[i];
        char operation = mod[0];
        int number;
        if (debug) {
            cout << "mod=" << mod << endl;
        }
        // cout << "avl.getHeadData=" << avl.getHeadData() << endl;
        if (operation == 'A') {
            number = stoi(mod.substr(1, mod.length() - 1));
            avl.insertNode(number);
        }
        else {
            // assuming D
            number = stoi(mod.substr(1, mod.length() - 1));
            bool result = avl.deleteNode(number);
            // if (!result) {
            //     modsDeleteFail.push_back(mod);
            // }
        }
        avl.printTreeStructure();
    }

    // for (long unsigned int i = 0; i < modsDeleteFail.size(); ++i) {
    //     // cout << mods[i] << " | ";
    //     string mod = modsDeleteFail[i];
    //     int number;
    //     // assuming D
    //     number = stoi(mod.substr(1, mod.length() - 1));
    //     bool result = avl.deleteNode(number);
    //     if (!result) {
    //         // modsDeleteFail.push_back(mod);

    //     }
    // }

    if (traversalTypeStr == "IN") {
        avl.traversalInOrder();
    }
    else if (traversalTypeStr == "PRE") {
        avl.traversalPreOrder();
    }
    else if (traversalTypeStr == "POST") {
        avl.traversalPostOrder();
    }
    // cout << endl;
    // cout << input << endl;
    // cout << oneInput << endl;
    // cout << pos << endl;
    // cout << prevPos << endl;
    // cout << traversalTypeStr << endl;

    return 0;
}