#include <iostream>
#include <fstream>
#include "medicine.h"
using namespace std;

int Medicine::getId() const { return id; }
string Medicine::getName() const { return name; }
int Medicine::getQuantity() const { return quantity; }
string Medicine::getExpiryDate() const { return expiryDate; }
float Medicine::getPrice() const { return price; }

void Medicine::setId(int i) { id = i; }
void Medicine::setName(string n) { name = n; }
void Medicine::setQuantity(int q) { quantity = q; }
void Medicine::setExpiry(string d) { expiryDate = d; }
void Medicine::setPrice(float p) { price = p; }

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

void Inventory::checkStock()
{
    for (const auto &med : container)
    {
        if (med.getQuantity() < 5)
            cout << "Low stock alert for " << med.getName() << endl;

        cout << "Expiry check: " << med.getName() << "expires on " << med.getExpiryDate() << "\n";
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
        cout << "\nMedicine Inventory Menu\n1. Load inventory\n2. Display inventory\n3. Add medicine\n4. Remove medicine\n5. Search medicine by ID\n6. Search medicine by name\n7. Update medicine\n8. Check stock\n9. Save inventory\n0. Exit\nEnter your choice: ";
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
            cout << "Enter ID, Name, Quantity, Expiry Date(withpuut space), Price: ";
            cin >> id >> name >> qty >> expiry >> price;
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
        else if (choice == 9)
            inv.saveToFile();
        else
            cout << "Enter a valid choice.";
    } while (choice != 0);

    return 0;
}