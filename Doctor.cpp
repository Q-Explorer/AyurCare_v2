#include <bits/stdc++.h>
using namespace std;

class Doctor {
public:
    int id;
    string name, spec, phone, timing;
    double fee;

    void display() {
        cout << "\nID: " << id
             << "\nName: " << name
             << "\nSpecialization: " << spec
             << "\nPhone: " << phone
             << "\nFee: " << fee
             << "\nTiming: " << timing << "\n";
    }

    string serialize() {
        return to_string(id) + "|" + name + "|" + spec + "|" + phone + "|" + to_string(fee) + "|" + timing;
    }

    void deserialize(string line) {
        stringstream ss(line);
        string temp;
        getline(ss, temp, '|'); id = stoi(temp);
        getline(ss, name, '|');
        getline(ss, spec, '|');
        getline(ss, phone, '|');
        getline(ss, temp, '|'); fee = stod(temp);
        getline(ss, timing);
    }
};

class Appointment {
public:
    int patientId, doctorId;
    string therapy, time;

    string serialize() {
        return to_string(patientId) + "|" + to_string(doctorId) + "|" + therapy + "|" + time;
    }

    void deserialize(string line) {
        stringstream ss(line);
        string temp;
        getline(ss, temp, '|'); patientId = stoi(temp);
        getline(ss, temp, '|'); doctorId = stoi(temp);
        getline(ss, therapy, '|');
        getline(ss, time);
    }
};

class Manager {
    vector<Doctor> docs;

public:
    void loadDoctors() {
        docs.clear();
        ifstream fin("doctor.txt");
        string line;

        while (getline(fin, line)) {
            if (line.empty()) continue;
            Doctor d;
            d.deserialize(line);
            docs.push_back(d);
        }
    }

    int nextDoctorId() {
        if (docs.empty()) return 0;
        return docs.back().id + 1;
    }

    bool patientExists(int pid) {
        ifstream fin("Patient.txt");
        string line;

        while (getline(fin, line)) {
            stringstream ss(line);
            string temp;
            getline(ss, temp, '|');
            if (stoi(temp) == pid) return true;
        }
        return false;
    }

    void addDoctor() {
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

    void showDoctors() {
        loadDoctors();
        for (auto &d : docs) d.display();
    }

    void searchById() {
        loadDoctors();
        int id;
        cout << "Enter ID: ";
        cin >> id;

        for (auto &d : docs) {
            if (d.id == id) {
                d.display();
                return;
            }
        }
        cout << "Doctor not found\n";
    }

    void searchBySpec() {
        loadDoctors();
        cin.ignore();
        string s;
        cout << "Enter specialization: ";
        getline(cin, s);

        bool found = false;
        for (auto &d : docs) {
            if (d.spec == s) {
                d.display();
                found = true;
            }
        }

        if (!found) cout << "No match\n";
    }

    void deleteDoctor() {
        loadDoctors();
        int id;
        cout << "Enter ID to delete: ";
        cin >> id;

        vector<Doctor> temp;
        bool found = false;

        for (auto &d : docs) {
            if (d.id == id) found = true;
            else temp.push_back(d);
        }

        if (!found) {
            cout << "Doctor not found\n";
            return;
        }

        docs = temp;

        ofstream fout("doctor.txt");
        for (auto &x : docs)
            fout << x.serialize() << "\n";

        cout << "Doctor deleted\n";
    }

    void bookAppointment() {
        loadDoctors();

        int pid, did;
        cout << "Enter Patient ID: ";
        cin >> pid;

        if (!patientExists(pid)) {
            cout << "Patient not found\n";
            return;
        }

        cout << "Enter Doctor ID: ";
        cin >> did;

        bool exists = false;
        for (auto &d : docs) {
            if (d.id == did) exists = true;
        }

        if (!exists) {
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

        while (getline(fin, line)) {
            Appointment t;
            t.deserialize(line);

            if (t.doctorId == did && t.time == a.time) {
                cout << "Doctor already booked\n";
                return;
            }
        }

        ofstream fout("appointments.txt", ios::app);
        fout << a.serialize() << "\n";

        cout << "Appointment booked\n";
    }

    void viewAppointments() {
        ifstream fin("appointments.txt");

        if (!fin) {
            cout << "No appointments file found\n";
            return;
        }

        string line;
        bool empty = true;

        while (getline(fin, line)) {
            if (line.empty()) continue;

            empty = false;
            Appointment a;
            a.deserialize(line);

            cout << "\nPatient ID: " << a.patientId
                 << "\nDoctor ID: " << a.doctorId
                 << "\nTherapy: " << a.therapy
                 << "\nTime: " << a.time << "\n";
        }

        if (empty) cout << "No appointments available\n";
    }
};

int main() {
    Manager m;
    int ch;

    while (1) {
        cout << "\n1.Add Doctor\n2.Show Doctors\n3.Search by ID\n4.Search by Spec\n5.Delete Doctor\n6.Book Appointment\n7.View Appointments\n8.Exit\nChoice: ";
        cin >> ch;

        switch (ch) {
            case 1: m.addDoctor(); break;
            case 2: m.showDoctors(); break;
            case 3: m.searchById(); break;
            case 4: m.searchBySpec(); break;
            case 5: m.deleteDoctor(); break;
            case 6: m.bookAppointment(); break;
            case 7: m.viewAppointments(); break;
            case 8: return 0;
            default: cout << "Invalid\n";
        }
    }
}
