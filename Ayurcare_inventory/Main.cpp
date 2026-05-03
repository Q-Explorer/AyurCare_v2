#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

// ----- From Medicine_Billing.cpp (classes + methods; main omitted) -----

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

class Billing
{
    int billId;
    vector<Medicine> purchased;
    float totalAmount;

public:
    Billing(int id) : billId(id), totalAmount(0) {}
    void addItem(const Medicine &m, int qty);
    void printBill();
    void saveBillToFile();
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
        cout << "\nCurrent Inventory:\n";
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

void Billing::addItem(const Medicine &m, int q)
{
    if (m.getQuantity() >= q)
    {
        Medicine temp = m;
        temp.setQuantity(q);
        purchased.push_back(temp);
        totalAmount += (q * m.getPrice());
    }
    else
        cout << "Not enough stock for " << m.getName() << endl;
}

void Billing::printBill()
{
    cout << "\nBill --- #" << billId << endl;
    for (auto &med : purchased)
    {
        cout << med.getName() << " x " << med.getQuantity() << " = " << (med.getQuantity() * med.getPrice()) << endl;
    }
    cout << "Total: " << totalAmount << endl;
}

void Billing::saveBillToFile()
{
    ofstream file("data/billing.txt", ios::app);
    file << "Bill ID: " << billId << " Total: " << totalAmount << endl;
    for (auto &med : purchased)
    {
        file << med.getName() << " | " << med.getQuantity() << " | " << med.getPrice() << endl;
    }
    file << "--------------";
    file.close();
}

// ----- From Patients.cpp (classes + methods; main omitted) -----

enum Dosha
{
    VATA,
    PITTA,
    KAPHA,
};
class patient
{
    int ID, Age;
    string pName, Surname;
    long long int phNumber;
    Dosha dosha;

public:
    patient(int id, string name, string surname, int age, long long int no, Dosha dosha)
    {
        ID = id;
        pName = name;
        Surname = surname;
        Age = age;
        phNumber = no;
        this->dosha = dosha;
    }

    int getID() { return ID; }
    string getFirstName() { return pName; }
    string getSurname() { return Surname; }
    int getAge() { return Age; }
    long long int getPh() { return phNumber; }
    Dosha getDosha() { return dosha; }

    void setName(string name, string surname)
    {
        pName = name;
        Surname = surname;
    }
    void setAge(int age) { Age = age; }
    void setNumber(long long int no) { phNumber = no; }
    void setDosha(Dosha dosha) { this->dosha = dosha; }

    string d[3] = {"VATA", "PITTA", "KAPHA"};
    void display()
    {
        cout << ID << " | " << pName + " " + Surname << " | Age: " << Age << " | Dosha: " << d[dosha] << endl;
    }
};

class patientManagement
{
    vector<patient> patients;
    int nextID = 1;

public:
    void addPatient(patient p)
    {
        patients.push_back(p);
        nextID++;
        cout << "Patient Added Successfully!" << endl;
    }
    void updatePatient(int id, string name, string surname, int age, long long int no, Dosha dosha)
    {
        for (patient &p : patients)
        {
            if (p.getID() == id)
            {
                p.setName(name, surname);
                p.setAge(age);
                p.setNumber(no);
                p.setDosha(dosha);
                cout << "Patient Updated Successfully!" << endl;
                return;
            }
        }
        cout << "Patient Not Found!!" << endl;
    }

    void delPatient(int id)
    {
        for (int i = 0; i < patients.size(); i++)
        {
            if (patients[i].getID() == id)
            {
                patients.erase(patients.begin() + i);
                cout << "Patient's Data Deleted!!" << endl;
                return;
            }
        }
        cout << "Patient Not Found!!" << endl;
    }

    void searchPatient(int id)
    {
        for (patient &p : patients)
        {
            if (p.getID() == id)
            {
                p.display();
                return;
            }
        }
        cout << "Patient Not Found!!" << endl;
    }

    void displayAll()
    {
        for (patient &p : patients)
        {
            p.display();
        }
    }

    void save()
    {
        ofstream file("Patient.txt");
        for (patient &p : patients)
        {
            file << p.getID() << " " << p.getFirstName() << " " << p.getSurname() << " " << p.getAge() << " " << p.getPh() << " " << p.getDosha() << endl;
        }
        file.close();
    }

    void load()
    {
        ifstream file("Patient.txt");
        if (!file)
        {
            cout << "No previous data found.\n";
            return;
        }
        int id, age, doshaID;
        string name, surname;
        long long int ph;
        while (file >> id >> name >> surname >> age >> ph >> doshaID)
        {
            patients.push_back(patient(id, name, surname, age, ph, (Dosha)doshaID));
        }
    }

    void updateID()
    {
        for (patient &p : patients)
        {
            if (p.getID() >= nextID)
            {
                nextID = p.getID() + 1;
            }
        }
    }
    int getNextID() { return nextID; }
};

// ----- From Doctor.cpp (classes + methods; main omitted) -----

class Doctor
{
public:
    int id;
    string name, spec, phone, timing;
    double fee;

    void display()
    {
        cout << "\nID: " << id
             << "\nName: " << name
             << "\nSpecialization: " << spec
             << "\nPhone: " << phone
             << "\nFee: " << fee
             << "\nTiming: " << timing << "\n";
    }

    string serialize()
    {
        return to_string(id) + "|" + name + "|" + spec + "|" + phone + "|" + to_string(fee) + "|" + timing;
    }

    void deserialize(string line)
    {
        stringstream ss(line);
        string temp;
        getline(ss, temp, '|');
        id = stoi(temp);
        getline(ss, name, '|');
        getline(ss, spec, '|');
        getline(ss, phone, '|');
        getline(ss, temp, '|');
        fee = stod(temp);
        getline(ss, timing);
    }
};

class Appointment
{
public:
    int patientId, doctorId;
    string therapy, time;

    string serialize()
    {
        return to_string(patientId) + "|" + to_string(doctorId) + "|" + therapy + "|" + time;
    }

    void deserialize(string line)
    {
        stringstream ss(line);
        string temp;
        getline(ss, temp, '|');
        patientId = stoi(temp);
        getline(ss, temp, '|');
        doctorId = stoi(temp);
        getline(ss, therapy, '|');
        getline(ss, time);
    }
};

class Manager
{
    vector<Doctor> docs;

public:
    void loadDoctors()
    {
        docs.clear();
        ifstream fin("doctor.txt");
        string line;

        while (getline(fin, line))
        {
            if (line.empty())
                continue;
            Doctor d;
            d.deserialize(line);
            docs.push_back(d);
        }
    }

    int nextDoctorId()
    {
        if (docs.empty())
            return 0;
        return docs.back().id + 1;
    }

    bool patientExists(int pid)
    {
        ifstream fin("Patient.txt");
        string line;

        while (getline(fin, line))
        {
            stringstream ss(line);
            string temp;
            getline(ss, temp, '|');
            if (stoi(temp) == pid)
                return true;
        }
        return false;
    }

    void addDoctor()
    {
        loadDoctors();
        Doctor d;
        d.id = nextDoctorId();

        cin.ignore();
        cout << "Name: ";
        getline(cin, d.name);
        cout << "Specialization: ";
        getline(cin, d.spec);
        cout << "Phone: ";
        cin >> d.phone;
        cout << "Fee: ";
        cin >> d.fee;
        cin.ignore();
        cout << "Timing: ";
        getline(cin, d.timing);

        docs.push_back(d);

        ofstream fout("doctor.txt");
        for (auto &x : docs)
            fout << x.serialize() << "\n";

        cout << "Doctor Added with ID: " << d.id << "\n";
    }

    void showDoctors()
    {
        loadDoctors();
        for (auto &d : docs)
            d.display();
    }

    void searchById()
    {
        loadDoctors();
        int id;
        cout << "Enter ID: ";
        cin >> id;

        for (auto &d : docs)
        {
            if (d.id == id)
            {
                d.display();
                return;
            }
        }
        cout << "Doctor not found\n";
    }

    void searchBySpec()
    {
        loadDoctors();
        cin.ignore();
        string s;
        cout << "Enter specialization: ";
        getline(cin, s);

        bool found = false;
        for (auto &d : docs)
        {
            if (d.spec == s)
            {
                d.display();
                found = true;
            }
        }

        if (!found)
            cout << "No match\n";
    }

    void deleteDoctor()
    {
        loadDoctors();
        int id;
        cout << "Enter ID to delete: ";
        cin >> id;

        vector<Doctor> temp;
        bool found = false;

        for (auto &d : docs)
        {
            if (d.id == id)
                found = true;
            else
                temp.push_back(d);
        }

        if (!found)
        {
            cout << "Doctor not found\n";
            return;
        }

        docs = temp;

        ofstream fout("doctor.txt");
        for (auto &x : docs)
            fout << x.serialize() << "\n";

        cout << "Doctor deleted\n";
    }

    void bookAppointment()
    {
        loadDoctors();

        int pid, did;
        cout << "Enter Patient ID: ";
        cin >> pid;

        if (!patientExists(pid))
        {
            cout << "Patient not found\n";
            return;
        }

        cout << "Enter Doctor ID: ";
        cin >> did;

        bool exists = false;
        for (auto &d : docs)
        {
            if (d.id == did)
                exists = true;
        }

        if (!exists)
        {
            cout << "Doctor not found\n";
            return;
        }

        cin.ignore();
        Appointment a;
        a.patientId = pid;
        a.doctorId = did;

        cout << "Enter Therapy: ";
        getline(cin, a.therapy);

        cout << "Enter Time Slot: ";
        getline(cin, a.time);

        ifstream fin("appointments.txt");
        string line;

        while (getline(fin, line))
        {
            Appointment t;
            t.deserialize(line);

            if (t.doctorId == did && t.time == a.time)
            {
                cout << "Doctor already booked\n";
                return;
            }
        }

        ofstream fout("appointments.txt", ios::app);
        fout << a.serialize() << "\n";

        cout << "Appointment booked\n";
    }

    void viewAppointments()
    {
        ifstream fin("appointments.txt");

        if (!fin)
        {
            cout << "No appointments file found\n";
            return;
        }

        string line;
        bool empty = true;

        while (getline(fin, line))
        {
            if (line.empty())
                continue;

            empty = false;
            Appointment a;
            a.deserialize(line);

            cout << "\nPatient ID: " << a.patientId
                 << "\nDoctor ID: " << a.doctorId
                 << "\nTherapy: " << a.therapy
                 << "\nTime: " << a.time << "\n";
        }

        if (empty)
            cout << "No appointments available\n";
    }
};

// ----- Unified multi-level menu (only entry point) -----

static void runDoctorMenu(Manager &m)
{
    int ch;
    do
    {
        cout << "\n--- Doctor module ---\n";
        cout << "1. Add Doctor\n";
        cout << "2. Show Doctors\n";
        cout << "3. Search by ID\n";
        cout << "4. Search by Specialization\n";
        cout << "5. Delete Doctor\n";
        cout << "6. Book Appointment\n";
        cout << "7. View Appointments\n";
        cout << "0. Back to main menu\n";
        cout << "Choice: ";
        cin >> ch;

        switch (ch)
        {
        case 1:
            m.addDoctor();
            break;
        case 2:
            m.showDoctors();
            break;
        case 3:
            m.searchById();
            break;
        case 4:
            m.searchBySpec();
            break;
        case 5:
            m.deleteDoctor();
            break;
        case 6:
            m.bookAppointment();
            break;
        case 7:
            m.viewAppointments();
            break;
        case 0:
            break;
        default:
            cout << "Invalid choice.\n";
            break;
        }
    } while (ch != 0);
}

static void runPatientMenu(patientManagement &manage)
{
    int ch;
    do
    {
        cout << "\n--- Patient module ---\n";
        cout << "1. Add patient\n";
        cout << "2. Update patient\n";
        cout << "3. Search patient\n";
        cout << "4. Delete patient\n";
        cout << "5. Display all patients\n";
        cout << "6. Save to file\n";
        cout << "0. Back to main menu\n";
        cout << "Choice: ";
        cin >> ch;

        int id, age, dosha;
        string name, surname;
        long long int ph;

        switch (ch)
        {
        case 1:
            cout << "Enter First Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Surname: ";
            getline(cin, surname);
            cout << "Enter Age: ";
            cin >> age;
            cout << "Enter Phone Number: ";
            cin >> ph;
            cout << "Enter Dosha(0=VATA,1=PITTA,2=KAPHA): ";
            cin >> dosha;
            manage.addPatient(patient(manage.getNextID(), name, surname, age, ph, (Dosha)dosha));
            break;

        case 2:
            cout << "Enter ID to Update: ";
            cin >> id;
            cout << "Enter First Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Surname: ";
            getline(cin, surname);
            cout << "Enter Age: ";
            cin >> age;
            cout << "Enter Phone Number: ";
            cin >> ph;
            cout << "Enter Dosha(0=VATA,1=PITTA,2=KAPHA): ";
            cin >> dosha;
            manage.updatePatient(id, name, surname, age, ph, (Dosha)dosha);
            break;

        case 3:
            cout << "Enter ID: ";
            cin >> id;
            manage.searchPatient(id);
            break;

        case 4:
            cout << "Enter ID: ";
            cin >> id;
            manage.delPatient(id);
            break;

        case 5:
            manage.displayAll();
            break;

        case 6:
            manage.save();
            cout << "Data saved.\n";
            break;

        case 0:
            break;

        default:
            cout << "Invalid choice.\n";
            break;
        }
    } while (ch != 0);
}

static void runMedicineBillingMenu(Inventory &inv)
{
    int choice;
    do
    {
        cout << "\n--- Medicine & Billing module ---\n";
        cout << "1. Load inventory\n";
        cout << "2. Display inventory\n";
        cout << "3. Add medicine\n";
        cout << "4. Remove medicine\n";
        cout << "5. Search medicine by ID\n";
        cout << "6. Search medicine by name\n";
        cout << "7. Update medicine\n";
        cout << "8. Check stock / expiry alerts\n";
        cout << "9. Generate bill\n";
        cout << "0. Back to main menu\n";
        cout << "Enter your choice: ";
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
            cout << "Enter ID, Name, Quantity, Price, Expiry Date(without space): ";
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
        else if (choice == 9)
        {
            int billId;
            char more;
            cout << "Enter the bill ID: ";
            cin >> billId;

            Billing bill(billId);

            do
            {
                int id, q;
                cout << "Enter medicine ID and quantity: ";
                cin >> id >> q;

                Medicine *med = inv.searchMedicine(id);
                if (med)
                {
                    bill.addItem(*med, q);
                    inv.updateMedicine(id, med->getQuantity() - q, med->getPrice());
                }
                else
                    cout << "Medicine not found.\n";

                cout << "Enter more items(y/n): ";
                cin >> more;
            } while (more == 'y' || more == 'Y');

            bill.saveBillToFile();
            bill.printBill();
        }
        else if (choice == 0)
            ;
        else
            cout << "Enter a valid choice.";
    } while (choice != 0);
}

int main()
{
    patientManagement patients;
    patients.load();
    patients.updateID();

    Manager doctorManager;
    Inventory inventory;

    int root;
    do
    {
        cout << "\n========== Ayurcare ==========\n";
        cout << "1. Doctor\n";
        cout << "2. Patient\n";
        cout << "3. Medicine & Billing\n";
        cout << "0. Exit\n";
        cout << "Select module: ";
        cin >> root;

        if (root == 1)
            runDoctorMenu(doctorManager);
        else if (root == 2)
            runPatientMenu(patients);
        else if (root == 3)
            runMedicineBillingMenu(inventory);
        else if (root == 0)
            cout << "Exiting.\n";
        else
            cout << "Invalid choice.\n";
    } while (root != 0);

    return 0;
}