#include<iostream>
#include<fstream>
#include<cstring>
#include<vector>
using namespace std;

enum Dosha{
    VATA,
    PITTA,
    KAPHA,
};
class patient{
    int ID,Age;
    string pName,Surname;
    long long int phNumber;
    Dosha dosha;
public:
    patient(int id,string name,string surname,int age,long long int no,Dosha dosha){
        ID=id;
        pName=name;
        Surname=surname;
        Age=age;
        phNumber=no;
        this->dosha=dosha;
    }

    int getID(){ return ID; }
    string getFirstName(){ return pName;}
    string getSurname(){ return Surname;}
    int getAge(){ return Age; }
    long long int getPh(){ return phNumber; }
    Dosha getDosha(){ return dosha; }

    void setName(string name,string surname){pName=name; Surname=surname;}
    void setAge(int age){Age=age;}
    void setNumber(long long int no){phNumber=no;}
    void setDosha(Dosha dosha){this->dosha=dosha;}

    string d[3]={"VATA","PITTA","KAPHA"};
    void display() {
        cout << ID << " | " << pName+" "+Surname << " | Age: " << Age << " | Dosha: " << d[dosha] << endl;
    }
};

class patientManagement{
    vector<patient> patients;
    int nextID=1;
    public:
    void addPatient(patient p){
        patients.push_back(p);
        nextID++; 
        cout << "Patient Added Successfully!" << endl;
    }
    void updatePatient(int id,string name,string surname,int age,long long int no,Dosha dosha){
        for(patient &p : patients){
            if(p.getID()==id){
                p.setName(name,surname);
                p.setAge(age);
                p.setNumber(no);
                p.setDosha(dosha);
                cout << "Patient Updated Successfully!" << endl;
                return;
            }
        }
        cout << "Patient Not Found!!" << endl;
    }

    void delPatient(int id){
        for(int i=0;i<patients.size();i++){
            if(patients[i].getID()==id){
                patients.erase(patients.begin()+i);
                cout << "Patient's Data Deleted!!" << endl;
                return;
            }
        }
        cout << "Patient Not Found!!" << endl;
    }

    void searchPatient(int id){
        for(patient &p : patients){
            if(p.getID()==id){
                p.display();
                return;
            }
        }
        cout << "Patient Not Found!!" << endl;
    }

    void displayAll(){
        for(patient &p : patients){
            p.display();
        }
    }

    void save(){
        ofstream file("Patient.txt");
        for(patient &p : patients){
            file << p.getID() << " " << p.getFirstName() << " " << p.getSurname() << " " << p.getAge() << " " << p.getPh() << " " << p.getDosha() << endl;            
        }
        file.close();
    }

    void load(){
        ifstream file("Patient.txt");
        if(!file){
            cout << "No previous data found.\n";
            return;
        }
        int id,age,doshaID;
        string name,surname;
        long long int ph;
        while(file >> id >> name >> surname >> age >> ph >> doshaID){
            patients.push_back(patient(id, name, surname, age, ph, (Dosha)doshaID));
        }
    }

    void updateID(){
        for(patient &p : patients){
            if(p.getID()>=nextID){
                nextID=p.getID()+1;
            }
        }
    }
    int getNextID(){return nextID;} 
};

int main(){
    patientManagement manage;
    manage.load();
    manage.updateID();
    int ch;

    do{
        cout << "\n1.Add\n2.Update\n3.Search\n4.Delete\n5.Display All\n6.Save And Exit\nEnter Choice: ";
        cin >> ch;

        int id,age,dosha;
        string name,surname;
        long long int ph;

        switch (ch){
        case 1:
            cout << "Enter First Name: ";
            cin.ignore();
            getline(cin,name);
            cout << "Enter Surname: ";
            getline(cin,surname);
            cout << "Enter Age: ";
            cin >> age;
            cout << "Enter Phone Number: ";
            cin >> ph;
            cout << "Enter Dosha(0=VATA,1=PITTA,2=KAPHA): ";
            cin >> dosha;
            manage.addPatient(patient(manage.getNextID(),name,surname,age,ph,(Dosha)dosha));
            break;
        
        case 2:
            cout << "Enter ID to Update: ";
            cin >> id;
            cout << "Enter First Name: ";
            cin.ignore();
            getline(cin,name);
            cout << "Enter Surname: ";
            getline(cin,surname);
            cout << "Enter Age: ";
            cin >> age;
            cout << "Enter Phone Number: ";
            cin >> ph;
            cout << "Enter Dosha(0=VATA,1=PITTA,2=KAPHA): ";
            cin >> dosha;
            manage.updatePatient(id,name,surname,age,ph,(Dosha)dosha);
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
            cout << "Data saved. Exiting...\n";
            break;
        
        default:
            break;
        }
    } while(ch!=6);

    return 0;
}