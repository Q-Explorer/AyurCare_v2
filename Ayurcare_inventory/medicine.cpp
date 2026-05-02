#include <iostream>
#include <fstream>
#include <medicine.h>
using namespace std;

void Inventory::loadFromFile()
{
    container.clear();

    ifstream file("data/medicine.txt");
    if (!file)
    {
        cout << "Error: could not open the medicine.txt, no records found.\n";
        return;
    }

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

int main()
{

    return 0;
}