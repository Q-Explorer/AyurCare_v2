#include <iostream>
#include <fstream>
#include "medicine.h"
using namespace std;

int Medicine::getId() const { return id; }
string Medicine::getName() const { return name; }
int Medicine::getQuantity() const { return quantity; }
string Medicine::getExpiryDate() const { return expiryDate; }
float Medicine::getPrice() const { return price; }

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
                 << med.getPrice() << "\n";
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
                 << " | Price: " << med.getPrice() << "\n";
        }
    }
}

void Inventory::addMedicine(const Medicine &m)
{
    container.push_back(m);
    saveToFile();
    cout << "Medicine added successfully to the record.\n";
}

int main()
{
    Medicine m1(1, "Paracetamol", 10, "1 August 2026", 10.5);
    Inventory i1;
    i1.loadFromFile();
    i1.displayInventory();
    i1.addMedicine(m1);
    i1.displayInventory();
    i1.saveToFile();
    return 0;
}