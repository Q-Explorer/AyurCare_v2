#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

class Medicine
{
    int id;
    string name;
    int quantity;
    string expiryDate;
    float price;

public:
    Medicine(int i, string n, int q, string expiry, float p) : id(i), name(n), quantity(q), expiryDate(expiry), price(p) {}
    Medicine() {}

    int getId() const { return id; }
    string getName() const { return name; }
    int getQuantity() const { return quantity; }
    string getExpiryDate() const { return expiryDate; }
    float getPrice() const { return price; }

    void setId(int i) { id = i; }
    void setName(string n) { name = n; }
    void setQuantity(int q) { quantity = q; }
    void setExpiry(string d) { expiryDate = d; }
    void setPrice(float p) { price = p; }
};

class Inventory
{
    vector<Medicine> container;

public:
    void addMedicine(const Medicine &m);
    void removeMedicine(int id);
    void updateMedicine(int id, int newQty, float newPrice);
    void displayInventory();
    void loadFromFile();
    void saveToFile();
    void checkStock();
    Medicine *searchMedicine(int id);
    Medicine *searchMedicine(string name);
    // void appendFromFile();
};

void Inventory::loadFromFile()
{
    container.clear();

    ifstream file("data/medicine.txt");

    if (!file)
    {
        cout << "Error: could not open the medicine.txt, no records found.\n";
    }
    else
    {
        int tempId, tempQuantity;
        string tempName, tempDate;
        float tempPrice;

        while (file >> tempId >> tempName >> tempQuantity >> tempDate >> tempPrice)
        {
            Medicine m1;
            m1.setId(tempId);
            m1.setName(tempName);
            m1.setQuantity(tempQuantity);
            m1.setExpiry(tempDate);
            m1.setPrice(tempPrice);

            container.push_back(m1);
        }

        file.close();
        cout << "Medicine record loaded successfully.\n";
    }
}

void Inventory::saveToFile()
{
    ofstream file("data/medicine.txt");

    if (!file)
    {
        cout << "Error: could not open the medicine.txt, no records found.\n";
    }
    else
    {
        for (const auto &med : container)
        {
            file << med.getId() << "\t"
                 << med.getName() << "\t"
                 << med.getQuantity() << "\t"
                 << med.getExpiryDate() << "\t"
                 << med.getPrice() << endl;
        }

        file.close();
        cout << "Medicine records saved successfully.\n";
    }
}

void Inventory::displayInventory()
{
    if (container.empty())
    {
        cout << "The inventory is empty.\n";
    }
    else
    {
        cout << "Current Inventory:\n";
        for (const auto &med : container)
        {
            cout << "ID: " << med.getId()
                 << " | Name: " << med.getName()
                 << " | Quantity: " << med.getQuantity()
                 << " | Expiry Date: " << med.getExpiryDate()
                 << " | Price: " << med.getPrice() << endl;
        }
    }
}

void Inventory::addMedicine(const Medicine &m)
{
    container.push_back(m);
    saveToFile();
    cout << "Medicine added successfully to the record.\n";
}

void Inventory::removeMedicine(int id)
{
    for (auto it = container.begin(); it != container.end(); it++)
    {
        if (it->getId() == id)
        {
            container.erase(it);
            saveToFile();
            cout << "The medicine is removed from the record.\n";
            return;
        }
    }
    cout << "Medicine not found.\n";
}

Medicine *Inventory::searchMedicine(int id)
{
    for (auto &med : container)
    {
        if (med.getId() == id)
            return &med;
    }
    return nullptr;
}

Medicine *Inventory::searchMedicine(string name)
{
    for (auto &med : container)
    {
        if (med.getName() == name)
            return &med;
    }
    return nullptr;
}

bool isNearExpiry(const string &date)
{
    int d, m, y;
    char sep;
    stringstream ss(date);
    ss >> d >> sep >> m >> sep >> y;

    int currentYear = 2026, currentMonth = 5;
    return (y == currentYear && m <= currentMonth + 1);
}

void Inventory::checkStock()
{
    for (const auto &med : container)
    {
        if (med.getQuantity() < 5)
            cout << "Low stock alert for " << med.getName() << endl;

        if (isNearExpiry(med.getExpiryDate()))
            cout << "Expiry alert for: " << med.getName() << " expires on " << med.getExpiryDate() << "\n";
    }
}

void Inventory::updateMedicine(int id, int newQty, float newPrice)
{
    for (auto &med : container)
    {
        if (med.getId() == id)
        {
            med.setQuantity(newQty);
            med.setPrice(newPrice);
            saveToFile();
            cout << "Medicine updated successfully.\n";
            return;
        }
    }
    cout << "Medicine not found.\n";
}

int main()
{
    Inventory inv;
    int choice;

    do
    {
        cout << "\nMedicine Inventory Menu\n1. Load inventory\n2. Display inventory\n3. Add medicine\n4. Remove medicine\n5. Search medicine by ID\n6. Search medicine by name\n7. Update medicine\n8. Check stock\n9. Exit\nEnter your choice: ";
        cin >> choice;

        if (choice == 1)
            inv.loadFromFile();
        else if (choice == 2)
            inv.displayInventory();
        else if (choice == 3)
        {
            int id, qty;
            string name, expiry;
            float price;
            cout << "Enter ID, Name, Quantity, Price, Expiry Date(withpuut space): ";
            cin >> id >> name >> qty >> price;
            cin.ignore();
            getline(cin, expiry);
            Medicine m(id, name, qty, expiry, price);
            inv.addMedicine(m);
        }
        else if (choice == 4)
        {
            int id;
            cout << "Enter the id of the medicine to be removed: ";
            cin >> id;
            inv.removeMedicine(id);
        }
        else if (choice == 5)
        {
            int id;
            cout << "Enter the id to search the medicine: ";
            cin >> id;
            Medicine *m = inv.searchMedicine(id);
            if (m)
                cout << "Found: " << m->getName() << " | " << m->getPrice() << " | " << m->getQuantity() << endl;
            else
                cout << "Not found\n";
        }
        else if (choice == 6)
        {
            string name;
            cout << "Enter the name to search the medicine: ";
            cin >> name;
            Medicine *m = inv.searchMedicine(name);
            if (m)
                cout << "Found: " << m->getId() << " | " << m->getPrice() << " | " << m->getQuantity() << endl;
            else
                cout << "Not found\n";
        }
        else if (choice == 7)
        {
            int id, qty;
            float price;
            cout << "Enter the id, new quantity and new price: ";
            cin >> id >> qty >> price;
            inv.updateMedicine(id, qty, price);
        }
        else if (choice == 8)
            inv.checkStock();
        else
            cout << "Enter a valid choice.";
    } while (choice != 9);

    return 0;
}