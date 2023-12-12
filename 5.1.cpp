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
    Record tmp; // Создаем объект структуры Record для хранения данных.

    fin.seekg(recordOffset); // Устанавливаем позицию чтения в файле на указанное смещение.

    // Чтение данных из файла
    fin.read((char *) &tmp.key, sizeof(int)); // Считываем целое число (int) и сохраняем его в поле key.


    string name; // Создаем строку для хранения данных о название кинотеатра.
    char ch;
    fin.get(ch);
    while (ch != '\0') {
        name += ch; // Считываем символы до символа '\0' (нулевого символа) и добавляем их к строке name.
        fin.get(ch);
    }
    tmp.name = name; // Присваиваем полученную строку полю name объекта tmp.

    string filmnamee; // Создаем строку для хранения данных о названии фильма.
    fin.get(ch);
    while (ch != '\0') {
        filmnamee += ch; // Считываем символы до символа '\0' (нулевого символа) и добавляем их к строке filmnamee.
        fin.get(ch);
    }
    tmp.adress = filmnamee; // Присваиваем полученную строку полю adress объекта tmp.



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


    while (inputFile >> record.key >> record.name >>
                     record.adress) {
        if (i == N - 1 or i == N / 2 or i == 0)
            cout << "Код для проверки: " << records[i]->key << endl;

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
    // Закрытие файла
    fout.close();
    inputFile.close();
}

class Node {
public:
    int key;
    int offset;  // Смещение записи в файле
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
    // Создаем файл с n случайными записями
    int n = 1000; // Замените на нужное количество записей
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