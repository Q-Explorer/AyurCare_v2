#include <string>
#include <vector>
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
    
    int getId() const;
    string getName() const;
    int getQuantity() const;
    string getExpiryDate() const;
    float getPrice() const;
    
    void displayMedicine();
    void updateMedicine();
    
    void setId(int id);
    void setName(string n);
    void setQuantity(int q);
    void setExpiry(string d);
    void setPrice(float p);
};

class Inventory
{
    vector<Medicine> container;

public:
    void addMedicine(const Medicine &m);
    void removeMedicine(int id);
    Medicine *searchMedicine();
    void displayInventory();
    void loadFromFile();
    void appendFromFile();
    void saveToFile();
    void checkStock();
};
