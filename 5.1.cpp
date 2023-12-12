#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <set>

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
    Record tmp; // ������� ��ꥪ� �������� Record ��� �࠭���� ������.

    fin.seekg(recordOffset); // ��⠭�������� ������ �⥭�� � 䠩�� �� 㪠������ ᬥ饭��.

    // �⥭�� ������ �� 䠩��
    fin.read((char *) &tmp.key, sizeof(int)); // ���뢠�� 楫�� �᫮ (int) � ��࠭塞 ��� � ���� key.


    string name; // ������� ��ப� ��� �࠭���� ������ � �������� ����⥠��.
    char ch;
    fin.get(ch);
    while (ch != '\0') {
        name += ch; // ���뢠�� ᨬ���� �� ᨬ���� '\0' (�㫥���� ᨬ����) � ������塞 �� � ��ப� name.
        fin.get(ch);
    }
    tmp.name = name; // ��ᢠ����� ����祭��� ��ப� ���� name ��ꥪ� tmp.

    string filmnamee; // ������� ��ப� ��� �࠭���� ������ � �������� 䨫쬠.
    fin.get(ch);
    while (ch != '\0') {
        filmnamee += ch; // ���뢠�� ᨬ���� �� ᨬ���� '\0' (�㫥���� ᨬ����) � ������塞 �� � ��ப� filmnamee.
        fin.get(ch);
    }
    tmp.adress = filmnamee; // ��ᢠ����� ����祭��� ��ப� ���� adress ��ꥪ� tmp.



    lastOffset = fin.tellg();
    return tmp; // �����頥� ��ꥪ� tmp, ᮤ�ঠ騩 ��⠭�� �����.
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
    // ���樠������ ������� ��砩��� �ᥫ
    srand(time(0));

    ofstream outputFile("text.txt");

    vector<Record *> records;

    string FIO[] = {"������_���_����ਥ���", "����ਭ_�ᥢ����_�ਪ����", "�������_�����_����ᠭ�஢��",
                    "���஢�_����ਭ�_��ࣥ����", "������_�������_���஢��", "���୮��_����_��堩�����",
                    "���楢_��������_���ॢ��", "��������_��죠_����ᥥ���", "�㧭�殢_��ࣥ�_����ᠭ�஢��",
                    "��������_����_����ਥ���", "����஢_��⥬_�������஢��", "��஧���_����ﭠ_��������",
                    "������_����਩_���॥���", "�㧭�殢�_�������_��������", "���஢_�����_��ࣥ����",
                    "���୮�_����_������쥢��", "�������_�����_��ᨫ쥢��", "�������_�����_����ᥥ���",
                    "�૮�_�����_�����쥢��", "���������_���_���஢��"};

    string adresses[] = {"�����_��ண����_15", "��ᯥ��_��ୠ�᪮��_78", "��஬뭪�_20", "����᪠�_14",
                         "����_�����஢_2", "����_���_⥡�_��_ࠤ�_6", "����_��᫥����_�������_69", "����_����_45",
                         "������᪨�_���㫮�_7"};


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

    // ����⨥ ����୮�� 䠩�� ��� �����
    ifstream inputFile("text.txt");
    ofstream fout("test.bin", ios::binary);
    Record record;
    int i = 0;


    while (inputFile >> record.key >> record.name >>
                     record.adress) {
        if (i == N - 1 or i == N / 2 or i == 0)
            cout << "��� ��� �஢�ન: " << records[i]->key << endl;

        fout.write((char *) &records[i]->key, sizeof(int));


        const char *fio = records[i]->name.c_str();
        int fiosize = records[i]->name.size();
        fout.write(fio, fiosize + 1);


        const char *adress = records[i]->adress.c_str();
        int adressize = records[i]->adress.size();
        fout.write(adress, adressize + 1);



        //cnt++;
        i++;
    }
    // �����⨥ 䠩��
    fout.close();
    inputFile.close();
}

class Node {
public:
    int key;
    int offset;  // ���饭�� ����� � 䠩��
    Node *left;
    Node *right;

    Node(int k, int o) : key(k), offset(o), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
public:
    Node *root;
    std::fstream file;

    Node *insert(Node *node, int key, int offset) {
        if (node == nullptr) {
            return new Node(key, offset);
        }

        if (key < node->key) {
            node->left = insert(node->left, key, offset);
        } else if (key > node->key) {
            node->right = insert(node->right, key, offset);
        }

        return node;
    }

    Node *search(Node *node, int key, Node *node2) {
        if (node == nullptr) {
            return nullptr;
        }
        if (node->key == key) {
            return node;
        }


        if (key < node->key) {
            return search(node->left, key, node);
        }

        return search(node->right, key, node);
    }

    Node *findMin(Node *node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    Node *remove(Node *node, int key) {
        if (node == nullptr) {
            return nullptr;
        }

        if (key < node->key) {
            node->left = remove(node->left, key);
        } else if (key > node->key) {
            node->right = remove(node->right, key);
        } else {
            if (node->left == nullptr) {
                Node *temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                Node *temp = node->left;
                delete node;
                return temp;
            }

            Node *temp = findMin(node->right);
            node->key = temp->key;
            node->offset = temp->offset;
            node->right = remove(node->right, temp->key);
        }

        return node;
    }

    void display(Node *node, int level) {
        if (node != nullptr) {
            display(node->right, level + 1);
            cout << endl;

            if (node == root) {
                std::cout << "Root -> ";
            }

            for (int i = 0; i < level && node != root; ++i) {
                std::cout << "        ";
            }

            cout << "Key: " << node->key << ", Offset: " << node->offset;
            display(node->left, level + 1);
        }
    }

public:
    BinarySearchTree(const std::string &filename) : root(nullptr) {
        file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
        }
    }

    ~BinarySearchTree() {
        file.close();
    }

    void insert(int key, int offset) {
        root = insert(root, key, offset);
    }

    Node *search(int key) {
        return search(root, key, root);
    }

    void remove(int key) {
        root = remove(root, key);
    }

    void display() {
        display(root, 1);
    }
};

int main() {
    FileManager fm;
    // ������� 䠩� � n ��砩�묨 �����ﬨ
    int n = 1000; // ������� �� �㦭�� ������⢮ ����ᥩ
    fm.createFile(n);
    ifstream fin("test.bin", ios::binary);
    BinarySearchTree bst("test.bin");
    Record record;
    record = fm.getRecord(0, fin);
    bst.root = new Node(record.key, 0);

    for (int i = 0; i < n - 1; i++) {
        int tmp = fm.lastOffset;
        record = fm.getRecord(tmp, fin);
        bst.insert(record.key, tmp);
    }
    cout << "Binary Search Tree:" << std::endl;
    bst.display();

    int keyToFind = 40;
    cout << endl << "Input key to find:";
    cin >> keyToFind;
    if (bst.search(keyToFind) != nullptr) {
        std::cout << "\nKey " << keyToFind << " found in the tree." << std::endl;
        record = fm.getRecord(bst.search(keyToFind)->offset, fin);
        cout << bst.search(keyToFind)->offset;
        std::cout << endl << "Key: " << record.key << ", "
                  << "FIO: " << record.name << ", "
                  << "Adress: " << record.adress
                  << endl;
    } else {
        cout << "\nKey " << keyToFind << " not found in the tree." << std::endl;
    }

    int keyToRemove = 50;
    cout << endl << "Input key to remove:";
    cin >> keyToRemove;
    cout << "\nRemoving key " << keyToRemove << " from the tree." << std::endl;
    bst.remove(keyToRemove);
    bst.display();
    return 0;
}