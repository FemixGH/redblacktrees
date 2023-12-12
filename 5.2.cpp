// Implementing Red-Black Tree in C++

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>

using namespace std;


struct Record {
    int key;
    string name;
    string adress;
};

class FileManager {
public:
    int counter = 0;

    FileManager() {}

    int lastOffset = 0;
    string filename = "test.bin";

    void createFile(int N);

    bool isUnique(const vector<Record *> &flightRecords, int key);

    Record getRecord(int recordOffset, ifstream &fin);
};

Record FileManager::getRecord(int recordOffset, ifstream &fin) {
    Record tmp; // Создаем объект структуры Record для хранения данных.

    fin.seekg(recordOffset); // Устанавливаем позицию чтения в файле на указанное смещение.

    // Чтение данных из файла
    fin.read(reinterpret_cast<char *>(&tmp.key),
             sizeof(int)); // Считываем целое число (int) и сохраняем его в поле key.

    // Чтение строки name из файла
    std::getline(fin, tmp.name, '\0');

    // Чтение строки adress из файла
    std::getline(fin, tmp.adress, '\0');

    lastOffset = fin.tellg();
    return tmp; // Возвращаем объект tmp, содержащий считанные данные.
}

bool FileManager::isUnique(const vector<Record *> &cinemaRecords, int key) {
    for (const Record *record: cinemaRecords) {
        if (record->key == key) {
            return false;
        }
    }
    return true;
}


void FileManager::createFile(int N) {
    // Инициализация генератора случайных чисел
    srand(time(0));

    ofstream outputFile("text.txt");

    vector<Record *> records;

    string FIO[] = {"Абанин_Фёдор_Дмитриевич", "Харахорин_Всеволод_Ярикович", "Лебедев_Денис_Александрович",
                    "Петрова_Екатерина_Сергеевна", "Иванов_Николай_Петрович", "Смирнова_Анна_Михайловна",
                    "Зайцев_Владимир_Игоревич", "Лебедева_Ольга_Алексеевна", "Кузнецов_Сергей_Александрович",
                    "Соколова_Мария_Дмитриевна", "Федоров_Артем_Владимирович", "Морозова_Татьяна_Павловна",
                    "Волков_Дмитрий_Андреевич", "Кузнецова_Анастасия_Ивановна", "Петров_Павел_Сергеевич",
                    "Смирнов_Иван_Геннадьевич", "Козлова_Елена_Васильевна", "Новиков_Виктор_Алексеевич",
                    "Орлов_Игорь_Валерьевич", "Медведева_Любовь_Петровна"};

    string adresses[] = {"Малая_пироговка_15", "Проспект_Вернадского_78", "Стромынка_20", "Челябинская_14",
                         "Улица_авиаторов_2", "Улица_где_тебе_не_рады_6", "Улица_последней_надежды_69", "Чпок_стрит_45",
                         "Кадашёвский_переулок_7"};


    for (int i = 0; i < N; i++) {
        Record *new_record = new Record;
        int new_key = rand();

        while (!isUnique(records, new_key))
            new_key = rand();

        //new_key = i;

        new_record->key = new_key;


        new_record->name = FIO[rand() % 5];
        new_record->adress = adresses[rand() % 8];


        outputFile << new_record->key << " "
                   << new_record->name << " "
                   << new_record->adress << " "
                   << endl;
        records.push_back(new_record);
    }

    outputFile.close();

    // Открытие бинарного файла для записи
    ifstream inputFile("text.txt");
    ofstream fout("test.bin", ios::binary);
    Record record;
    int i = 0;


    while (inputFile >> record.key >> record.name >> record.adress) {
        if (i == N - 1 or i == N / 2 or i == 0)
            cout << "Код для проверки: " << records[i]->key << endl;

        fout.write(reinterpret_cast<char *>(&record.key), sizeof(int));

        fout.write(record.name.c_str(), record.name.size() + 1);

        fout.write(record.adress.c_str(), record.adress.size() + 1);

        i++;
    }
    // Закрытие файла
    fout.close();
    inputFile.close();
}


struct Node {
    int data;
    Node *parent;
    Node *left;
    Node *right;
    int color;
    int offset;
    /*Node(int k, int o) : data(k), offset(o), */
};

typedef Node *NodePtr;

class RedBlackTree {
public:
    NodePtr root;
    NodePtr TNULL;

    void initializeNULLNode(NodePtr node, NodePtr parent) {
        node->data = 0;
        node->parent = parent;
        node->left = nullptr;
        node->right = nullptr;
        node->color = 0;
        node->offset = 0;
    }

    // Preorder
    void preOrderHelper(NodePtr node) {
        if (node != TNULL) {
            cout << node->data << " ";
            preOrderHelper(node->left);
            preOrderHelper(node->right);
        }
    }

    // Inorder
    void inOrderHelper(NodePtr node) {
        if (node != TNULL) {
            inOrderHelper(node->left);
            cout << node->data << " ";
            inOrderHelper(node->right);
        }
    }

    // Post order
    void postOrderHelper(NodePtr node) {
        if (node != TNULL) {
            postOrderHelper(node->left);
            postOrderHelper(node->right);
            cout << node->data << " ";
        }
    }

    NodePtr searchTreeHelper(NodePtr node, int key) {
        if (key == node->data) {
            return node;
        }
        if (node == TNULL) {
            return nullptr;
        }

        if (key < node->data) {
            return searchTreeHelper(node->left, key);
        }
        return searchTreeHelper(node->right, key);
    }

    // For balancing the tree after deletion
    void deleteFix(NodePtr x) {
        NodePtr s;
        while (x != root && x->color == 0) {
            if (x == x->parent->left) {
                s = x->parent->right;
                if (s->color == 1) {
                    s->color = 0;
                    x->parent->color = 1;
                    leftRotate(x->parent);
                    s = x->parent->right;
                }

                if (s->left->color == 0 && s->right->color == 0) {
                    s->color = 1;
                    x = x->parent;
                } else {
                    if (s->right->color == 0) {
                        s->left->color = 0;
                        s->color = 1;
                        rightRotate(s);
                        s = x->parent->right;
                    }

                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->right->color = 0;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                s = x->parent->left;
                if (s->color == 1) {
                    s->color = 0;
                    x->parent->color = 1;
                    rightRotate(x->parent);
                    s = x->parent->left;
                }

                if ((s->right->color == 0) &&
                    (s->right->color == 0)) {//-----------------------------------------------------
                    s->color = 1;
                    x = x->parent;
                } else {
                    if (s->left->color == 0) {
                        s->right->color = 0;
                        s->color = 1;
                        leftRotate(s);
                        s = x->parent->left;
                    }

                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->left->color = 0;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = 0;
    }

    void rbTransplant(NodePtr u, NodePtr v) {
        if (u->parent == nullptr) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    void deleteNodeHelper(NodePtr node, int key) {
        NodePtr z = TNULL;
        NodePtr x, y;
        while (node != TNULL) {
            if (node->data == key) {
                z = node;
            }

            if (node->data <= key) {
                node = node->right;
            } else {
                node = node->left;
            }
        }

        if (z == TNULL) {
            cout << "Key not found in the tree" << endl;
            return;
        }

        y = z;
        int y_original_color = y->color;
        if (z->left == TNULL) {
            x = z->right;
            rbTransplant(z, z->right);
        } else if (z->right == TNULL) {
            x = z->left;
            rbTransplant(z, z->left);
        } else {
            y = minimum(z->right);
            y_original_color = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y;
            } else {
                rbTransplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            rbTransplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (y_original_color == 0) {
            deleteFix(x);
        }
    }

    // For balancing the tree after insertion
    void insertFix(NodePtr k) {
        NodePtr u;
        while (k->parent->color == 1) {
            if (k->parent == k->parent->parent->right) {
                u = k->parent->parent->left;
                if (u->color == 1) {
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->left) {
                        k = k->parent;
                        rightRotate(k);
                    }
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    leftRotate(k->parent->parent);
                }
            } else {
                u = k->parent->parent->right;

                if (u->color == 1) {
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                } else {
                    if (k == k->parent->right) {
                        k = k->parent;
                        leftRotate(k);
                    }
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    rightRotate(k->parent->parent);
                }
            }
            if (k == root) {
                break;
            }
        }
        root->color = 0;
    }

    void printHelper(NodePtr root, string indent, bool last) {
        if (root != TNULL) {
            cout << indent;
            if (last) {
                cout << "R----";
                indent += "   ";
            } else {
                cout << "L----";
                indent += "|  ";
            }

            string sColor = root->color ? "RED" : "BLACK";
            if (sColor == "RED") {
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, 4);
                cout << "Key:" << root->data << " Offset:" << root->offset << "(" << sColor << ")" << endl;
                SetConsoleTextAttribute(hConsole, 7);
            } else {
                cout << "Key:" << root->data << " Offset:" << root->offset << "(" << sColor << ")" << endl;
            }
            printHelper(root->left, indent, false);
            printHelper(root->right, indent, true);
        }
    }

public:
    RedBlackTree() {
        TNULL = new Node;
        TNULL->color = 0;
        TNULL->left = nullptr;
        TNULL->right = nullptr;
        root = TNULL;
    }

    void preorder() {
        preOrderHelper(this->root);
    }

    void inorder() {
        inOrderHelper(this->root);
    }

    void postorder() {
        postOrderHelper(this->root);
    }

    NodePtr searchTree(int k) {
        return searchTreeHelper(this->root, k);
    }

    NodePtr minimum(NodePtr node) {
        while (node->left != TNULL) {
            node = node->left;
        }
        return node;
    }

    NodePtr maximum(NodePtr node) {
        while (node->right != TNULL) {
            node = node->right;
        }
        return node;
    }

    NodePtr successor(NodePtr x) {
        if (x->right != TNULL) {
            return minimum(x->right);
        }

        NodePtr y = x->parent;
        while (y != TNULL && x == y->right) {
            x = y;
            y = y->parent;
        }
        return y;
    }

    NodePtr predecessor(NodePtr x) {
        if (x->left != TNULL) {
            return maximum(x->left);
        }

        NodePtr y = x->parent;
        while (y != TNULL && x == y->left) {
            x = y;
            y = y->parent;
        }

        return y;
    }

    void leftRotate(NodePtr x) {
        NodePtr y = x->right;
        x->right = y->left;
        if (y->left != TNULL) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            this->root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rightRotate(NodePtr x) {
        NodePtr y = x->left;
        x->left = y->right;
        if (y->right != TNULL) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            this->root = y;
        } else if (x == x->parent->right) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    // Inserting a node
    void insert(int key, int offset) {
        NodePtr node = new Node;
        node->parent = nullptr;
        node->data = key;
        node->left = TNULL;
        node->right = TNULL;
        node->color = 1;
        node->offset = offset;

        NodePtr y = nullptr;
        NodePtr x = this->root;

        while (x != TNULL) {
            y = x;
            if (node->data < x->data) {
                x = x->left;
            } else {
                x = x->right;
            }
        }

        node->parent = y;
        if (y == nullptr) {
            root = node;
        } else if (node->data < y->data) {
            y->left = node;
        } else {
            y->right = node;
        }

        if (node->parent == nullptr) {
            node->color = 0;
            return;
        }

        if (node->parent->parent == nullptr) {
            return;
        }

        insertFix(node);
    }

    NodePtr getRoot() {
        return this->root;
    }

    void deleteNode(FileManager &fm) {
        ifstream fin("test.bin", ios::binary);

        int keyToDelete;
        cout << "Введите ключ узла для удаления: ";
        cin >> keyToDelete;

        NodePtr nodeToDelete = searchTree(keyToDelete);
        if (nodeToDelete == TNULL) {
            cout << "Узел с ключом " << keyToDelete << " не найден в дереве." << endl;
            fin.close();
            return;
        }

        deleteNodeHelper(root, keyToDelete);
        fin.close();
    }

    void printTree() {
        if (root) {
            printHelper(this->root, "", true);
        }
    }

    void search(FileManager &fm) {
        ifstream fin("test.bin", ios::binary);
        int search_key;
        cout << "Введите для поиска:";
        cin >> search_key;

        NodePtr found_node = this->searchTree(search_key);

        if (found_node != nullptr) {
            cout << "Узел с ключом " << search_key << " найден в дереве." << endl;
            Record record = fm.getRecord(found_node->offset, fin);
            cout << found_node->offset;
            std::cout << endl << "Key: " << record.key << ", "
                      << "Name: " << record.name << ", "
                      << "Address: " << record.adress << std::endl;
        } else {
            cout << "Узел с ключом " << search_key << " не найден в дереве." << endl;
        }

        fin.close();
    }
};


int main() {
    setlocale(LC_ALL, "ru");
    int n = 11;
    FileManager fm;
    fm.createFile(n);
    ifstream fin("test.bin", ios::binary);
    RedBlackTree *bst = new RedBlackTree();
    Record record;
    record = fm.getRecord(0, fin);
    bst->insert(record.key, 0);
    for (int i = 0; i < n - 1; i++) {
        int tmp = fm.lastOffset;
        record = fm.getRecord(tmp, fin);
        bst->insert(record.key, tmp);
    }
    fin.close();
    bst->printTree();
    cout << endl << "After deleting" << endl;
    bst->deleteNode(fm);
    bst->printTree();
    bst->search(fm);
}



