#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

string trim(string s)
{
    int start=0;
    while(start<s.size()&&isspace((unsigned char)s[start]))
    {
        start++;
    }

    int end=(int)s.size()-1;
    while(end>=start&&isspace((unsigned char)s[end]))
    {
        end--;
    }

    if(start>end)
    {
        return "";
    }

    return s.substr(start,end-start+1);
}

bool parseInt(string text,int &value)
{
    text=trim(text);
    if(text.size()==0)
    {
        return false;
    }

    stringstream ss(text);
    int x;
    char extra;

    if(!(ss>>x))
    {
        return false;
    }

    if(ss>>extra)
    {
        return false;
    }

    value=x;
    return true;
}

bool parseLongLong(string text,long long &value)
{
    text=trim(text);
    if(text.size()==0)
    {
        return false;
    }

    stringstream ss(text);
    long long x;
    char extra;

    if(!(ss>>x))
    {
        return false;
    }

    if(ss>>extra)
    {
        return false;
    }

    value=x;
    return true;
}

bool parseFloat(string text,float &value)
{
    text=trim(text);
    if(text.size()==0)
    {
        return false;
    }

    stringstream ss(text);
    float x;
    char extra;

    if(!(ss>>x))
    {
        return false;
    }

    if(ss>>extra)
    {
        return false;
    }

    value=x;
    return true;
}

bool inputInt(string prompt,int &value,int low=INT_MIN,int high=INT_MAX)
{
    string line;

    while(true)
    {
        cout<<prompt;

        if(!getline(cin,line))
        {
            cout<<"Input error. Program will stop this operation.\n";
            cin.clear();
            return false;
        }

        int temp;
        if(!parseInt(line,temp))
        {
            cout<<"Error: enter a valid integer number.\n";
            continue;
        }

        if(temp<low||temp>high)
        {
            cout<<"Error: number must be between "<<low<<" and "<<high<<".\n";
            continue;
        }

        value=temp;
        return true;
    }
}

bool inputLongLong(string prompt,long long &value,long long low=LLONG_MIN,long long high=LLONG_MAX)
{
    string line;

    while(true)
    {
        cout<<prompt;

        if(!getline(cin,line))
        {
            cout<<"Input error. Program will stop this operation.\n";
            cin.clear();
            return false;
        }

        long long temp;
        if(!parseLongLong(line,temp))
        {
            cout<<"Error: enter a valid number.\n";
            continue;
        }

        if(temp<low||temp>high)
        {
            cout<<"Error: number is out of allowed range.\n";
            continue;
        }

        value=temp;
        return true;
    }
}

bool inputFloat(string prompt,float &value,float low=-FLT_MAX,float high=FLT_MAX)
{
    string line;

    while(true)
    {
        cout<<prompt;

        if(!getline(cin,line))
        {
            cout<<"Input error. Program will stop this operation.\n";
            cin.clear();
            return false;
        }

        float temp;
        if(!parseFloat(line,temp))
        {
            cout<<"Error: enter a valid decimal number.\n";
            continue;
        }

        if(temp<low||temp>high)
        {
            cout<<"Error: value is out of allowed range.\n";
            continue;
        }

        value=temp;
        return true;
    }
}

bool hasBadChar(string text)
{
    for(int i=0;i<text.size();i++)
    {
        if(text[i]=='|'||text[i]=='\t')
        {
            return true;
        }
    }
    return false;
}

bool hasSpace(string text)
{
    for(int i=0;i<text.size();i++)
    {
        if(isspace((unsigned char)text[i]))
        {
            return true;
        }
    }
    return false;
}

bool inputText(string prompt,string &value,bool allowSpace=true)
{
    string line;

    while(true)
    {
        cout<<prompt;

        if(!getline(cin,line))
        {
            cout<<"Input error. Program will stop this operation.\n";
            cin.clear();
            return false;
        }

        line=trim(line);

        if(line.size()==0)
        {
            cout<<"Error: value cannot be empty.\n";
            continue;
        }

        if(hasBadChar(line))
        {
            cout<<"Error: do not use | or tab characters.\n";
            continue;
        }

        if(!allowSpace&&hasSpace(line))
        {
            cout<<"Error: spaces are not allowed here. Use underscore if needed.\n";
            continue;
        }

        value=line;
        return true;
    }
}

bool validPhone(long long phone)
{
    string s=to_string(phone);

    if(s.size()!=10)
    {
        return false;
    }

    if(s[0]=='0')
    {
        return false;
    }

    return true;
}

vector<string> splitPipe(string line)
{
    vector<string> ans;
    string temp="";

    for(int i=0;i<line.size();i++)
    {
        if(line[i]=='|')
        {
            ans.push_back(temp);
            temp="";
        }
        else
        {
            temp+=line[i];
        }
    }

    ans.push_back(temp);
    return ans;
}

bool parseDate(string date,int &day,int &month,int &year)
{
    date=trim(date);

    if(date.size()!=10)
    {
        return false;
    }

    if(date[2]!='/'||date[5]!='/')
    {
        return false;
    }

    string d=date.substr(0,2);
    string m=date.substr(3,2);
    string y=date.substr(6,4);

    for(char c:d+m+y)
    {
        if(!isdigit((unsigned char)c))
        {
            return false;
        }
    }

    day=stoi(d);
    month=stoi(m);
    year=stoi(y);

    if(month<1||month>12)
    {
        return false;
    }

    int daysInMonth[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};

    bool leap=(year%400==0)||(year%4==0&&year%100!=0);
    if(leap)
    {
        daysInMonth[2]=29;
    }

    if(day<1||day>daysInMonth[month])
    {
        return false;
    }

    if(year<2020||year>2100)
    {
        return false;
    }

    return true;
}

bool inputDate(string prompt,string &date)
{
    int d,m,y;

    while(true)
    {
        if(!inputText(prompt,date,false))
        {
            return false;
        }

        if(!parseDate(date,d,m,y))
        {
            cout<<"Error: enter date in dd/mm/yyyy format. Example: 15/06/2027\n";
            continue;
        }

        return true;
    }
}

bool isNearExpiry(const string &date)
{
    int d,m,y;

    if(!parseDate(date,d,m,y))
    {
        return false;
    }

    int currentYear=2026;
    int currentMonth=5;

    if(y<currentYear)
    {
        return true;
    }

    if(y==currentYear&&m<=currentMonth+1)
    {
        return true;
    }

    return false;
}

class Medicine
{
    int id;
    string name;
    int quantity;
    string expiryDate;
    float price;

public:
    Medicine(int i,string n,int q,string expiry,float p)
    {
        id=i;
        name=n;
        quantity=q;
        expiryDate=expiry;
        price=p;
    }

    Medicine()
    {
        id=0;
        quantity=0;
        price=0;
        expiryDate="";
        name="";
    }

    int getId() const
    {
        return id;
    }

    string getName() const
    {
        return name;
    }

    int getQuantity() const
    {
        return quantity;
    }

    string getExpiryDate() const
    {
        return expiryDate;
    }

    float getPrice() const
    {
        return price;
    }

    void setId(int i)
    {
        id=i;
    }

    void setName(string n)
    {
        name=n;
    }

    void setQuantity(int q)
    {
        quantity=q;
    }

    void setExpiry(string d)
    {
        expiryDate=d;
    }

    void setPrice(float p)
    {
        price=p;
    }
};

class Billing
{
    int billId;
    vector<Medicine> purchased;
    float totalAmount;

public:
    Billing(int id)
    {
        billId=id;
        totalAmount=0;
    }

    bool addItem(const Medicine &m,int qty)
    {
        if(qty<=0)
        {
            cout<<"Error: quantity must be greater than zero.\n";
            return false;
        }

        if(m.getId()<=0)
        {
            cout<<"Error: invalid medicine selected.\n";
            return false;
        }

        if(m.getQuantity()<qty)
        {
            cout<<"Error: not enough stock for "<<m.getName()<<". Available: "<<m.getQuantity()<<".\n";
            return false;
        }

        Medicine temp=m;
        temp.setQuantity(qty);
        purchased.push_back(temp);
        totalAmount+=qty*m.getPrice();

        cout<<"Item added to bill.\n";
        return true;
    }

    bool isEmpty()
    {
        return purchased.empty();
    }

    void printBill()
    {
        if(purchased.empty())
        {
            cout<<"No items in bill.\n";
            return;
        }

        cout<<"\nBill --- #"<<billId<<"\n";
        cout<<"---------------------------------\n";

        for(auto &med:purchased)
        {
            cout<<med.getName()<<" x "<<med.getQuantity()<<" = "<<fixed<<setprecision(2)<<(med.getQuantity()*med.getPrice())<<"\n";
        }

        cout<<"---------------------------------\n";
        cout<<"Total: "<<fixed<<setprecision(2)<<totalAmount<<"\n";
    }

    bool saveBillToFile()
    {
        if(purchased.empty())
        {
            cout<<"Error: bill is empty. Nothing saved.\n";
            return false;
        }

        ofstream file("data/billing.txt",ios::app);

        if(!file)
        {
            cout<<"Error: could not open data/billing.txt. Create data folder first.\n";
            return false;
        }

        file<<"Bill ID: "<<billId<<" Total: "<<fixed<<setprecision(2)<<totalAmount<<"\n";

        for(auto &med:purchased)
        {
            file<<med.getName()<<" | "<<med.getQuantity()<<" | "<<fixed<<setprecision(2)<<med.getPrice()<<"\n";
        }

        file<<"--------------\n";
        file.close();

        cout<<"Bill saved successfully.\n";
        return true;
    }
};

class Inventory
{
    vector<Medicine> container;

public:
    bool idExists(int id)
    {
        for(auto &med:container)
        {
            if(med.getId()==id)
            {
                return true;
            }
        }
        return false;
    }

    void addMedicine(const Medicine &m)
    {
        if(m.getId()<=0)
        {
            cout<<"Error: medicine id must be positive.\n";
            return;
        }

        if(m.getName().empty()||hasSpace(m.getName())||hasBadChar(m.getName()))
        {
            cout<<"Error: medicine name cannot be empty and cannot contain spaces, |, or tab.\n";
            return;
        }

        if(m.getQuantity()<0)
        {
            cout<<"Error: quantity cannot be negative.\n";
            return;
        }

        if(m.getPrice()<0)
        {
            cout<<"Error: price cannot be negative.\n";
            return;
        }

        int d,mn,y;
        if(!parseDate(m.getExpiryDate(),d,mn,y))
        {
            cout<<"Error: invalid expiry date.\n";
            return;
        }

        if(idExists(m.getId()))
        {
            cout<<"Error: medicine id already exists.\n";
            return;
        }

        container.push_back(m);

        if(saveToFile())
        {
            cout<<"Medicine added successfully to the record.\n";
        }
    }

    void removeMedicine(int id)
    {
        if(id<=0)
        {
            cout<<"Error: invalid medicine id.\n";
            return;
        }

        if(container.empty())
        {
            cout<<"Inventory is empty.\n";
            return;
        }

        for(auto it=container.begin();it!=container.end();it++)
        {
            if(it->getId()==id)
            {
                container.erase(it);
                saveToFile();
                cout<<"The medicine is removed from the record.\n";
                return;
            }
        }

        cout<<"Medicine not found.\n";
    }

    void updateMedicine(int id,int newQty,float newPrice)
    {
        if(id<=0)
        {
            cout<<"Error: invalid medicine id.\n";
            return;
        }

        if(newQty<0)
        {
            cout<<"Error: quantity cannot be negative.\n";
            return;
        }

        if(newPrice<0)
        {
            cout<<"Error: price cannot be negative.\n";
            return;
        }

        for(auto &med:container)
        {
            if(med.getId()==id)
            {
                med.setQuantity(newQty);
                med.setPrice(newPrice);
                saveToFile();
                cout<<"Medicine updated successfully.\n";
                return;
            }
        }

        cout<<"Medicine not found.\n";
    }

    void displayInventory()
    {
        if(container.empty())
        {
            cout<<"The inventory is empty.\n";
            return;
        }

        cout<<"\nCurrent Inventory:\n";
        cout<<"ID | Name | Quantity | Expiry Date | Price\n";

        for(const auto &med:container)
        {
            cout<<med.getId()<<" | "
                <<med.getName()<<" | "
                <<med.getQuantity()<<" | "
                <<med.getExpiryDate()<<" | "
                <<fixed<<setprecision(2)<<med.getPrice()<<"\n";
        }
    }

    void loadFromFile()
    {
        container.clear();

        ifstream file("data/medicine.txt");

        if(!file)
        {
            cout<<"Warning: could not open data/medicine.txt. No medicine records loaded.\n";
            return;
        }

        string line;
        int lineNo=0;
        int loaded=0;

        while(getline(file,line))
        {
            lineNo++;
            line=trim(line);

            if(line.empty())
            {
                continue;
            }

            stringstream ss(line);

            int tempId,tempQuantity;
            string tempName,tempDate;
            float tempPrice;
            string extra;

            if(!(ss>>tempId>>tempName>>tempQuantity>>tempDate>>tempPrice))
            {
                cout<<"Invalid medicine record skipped at line "<<lineNo<<".\n";
                continue;
            }

            if(ss>>extra)
            {
                cout<<"Invalid medicine record skipped at line "<<lineNo<<" because it has extra data.\n";
                continue;
            }

            if(tempId<=0||tempQuantity<0||tempPrice<0||tempName.empty()||hasSpace(tempName)||hasBadChar(tempName))
            {
                cout<<"Invalid medicine values skipped at line "<<lineNo<<".\n";
                continue;
            }

            int d,m,y;
            if(!parseDate(tempDate,d,m,y))
            {
                cout<<"Invalid expiry date skipped at line "<<lineNo<<".\n";
                continue;
            }

            if(idExists(tempId))
            {
                cout<<"Duplicate medicine id skipped at line "<<lineNo<<".\n";
                continue;
            }

            Medicine m1;
            m1.setId(tempId);
            m1.setName(tempName);
            m1.setQuantity(tempQuantity);
            m1.setExpiry(tempDate);
            m1.setPrice(tempPrice);

            container.push_back(m1);
            loaded++;
        }

        file.close();

        if(loaded==0)
        {
            cout<<"No valid medicine records found.\n";
        }
        else
        {
            cout<<"Medicine records loaded successfully. Count: "<<loaded<<"\n";
        }
    }

    bool saveToFile()
    {
        ofstream file("data/medicine.txt");

        if(!file)
        {
            cout<<"Error: could not open data/medicine.txt for writing. Create data folder first.\n";
            return false;
        }

        for(const auto &med:container)
        {
            file<<med.getId()<<"\t"
                <<med.getName()<<"\t"
                <<med.getQuantity()<<"\t"
                <<med.getExpiryDate()<<"\t"
                <<fixed<<setprecision(2)<<med.getPrice()<<"\n";
        }

        file.close();
        return true;
    }

    void checkStock()
    {
        if(container.empty())
        {
            cout<<"Inventory is empty.\n";
            return;
        }

        bool alert=false;

        for(const auto &med:container)
        {
            if(med.getQuantity()<5)
            {
                cout<<"Low stock alert for "<<med.getName()<<". Quantity: "<<med.getQuantity()<<"\n";
                alert=true;
            }

            int d,m,y;
            if(!parseDate(med.getExpiryDate(),d,m,y))
            {
                cout<<"Invalid expiry date for "<<med.getName()<<": "<<med.getExpiryDate()<<"\n";
                alert=true;
            }
            else if(isNearExpiry(med.getExpiryDate()))
            {
                cout<<"Expiry alert for "<<med.getName()<<" expires on "<<med.getExpiryDate()<<"\n";
                alert=true;
            }
        }

        if(!alert)
        {
            cout<<"No stock or expiry alerts.\n";
        }
    }

    Medicine *searchMedicine(int id)
    {
        if(id<=0)
        {
            return nullptr;
        }

        for(auto &med:container)
        {
            if(med.getId()==id)
            {
                return &med;
            }
        }

        return nullptr;
    }

    Medicine *searchMedicine(string name)
    {
        if(name.empty())
        {
            return nullptr;
        }

        for(auto &med:container)
        {
            if(med.getName()==name)
            {
                return &med;
            }
        }

        return nullptr;
    }
};

enum Dosha
{
    VATA,
    PITTA,
    KAPHA,
};

class patient
{
    int ID,Age;
    string pName,Surname;
    long long int phNumber;
    Dosha dosha;

public:
    patient(int id,string name,string surname,int age,long long int no,Dosha dosha)
    {
        ID=id;
        pName=name;
        Surname=surname;
        Age=age;
        phNumber=no;
        this->dosha=dosha;
    }

    int getID()
    {
        return ID;
    }

    string getFirstName()
    {
        return pName;
    }

    string getSurname()
    {
        return Surname;
    }

    int getAge()
    {
        return Age;
    }

    long long int getPh()
    {
        return phNumber;
    }

    Dosha getDosha()
    {
        return dosha;
    }

    void setName(string name,string surname)
    {
        pName=name;
        Surname=surname;
    }

    void setAge(int age)
    {
        Age=age;
    }

    void setNumber(long long int no)
    {
        phNumber=no;
    }

    void setDosha(Dosha dosha)
    {
        this->dosha=dosha;
    }

    void display()
    {
        string d[3]={"VATA","PITTA","KAPHA"};

        cout<<ID<<" | "<<pName<<" "<<Surname
            <<" | Age: "<<Age
            <<" | Phone: "<<phNumber
            <<" | Dosha: "<<d[dosha]<<"\n";
    }
};

class patientManagement
{
    vector<patient> patients;
    int nextID=1;

public:
    bool validate(int id)
    {
        for(patient &p:patients)
        {
            if(p.getID()==id)
            {
                return true;
            }
        }
        return false;
    }

    void addPatient(patient p)
    {
        if(p.getID()<=0)
        {
            cout<<"Error: invalid patient id.\n";
            return;
        }

        if(validate(p.getID()))
        {
            cout<<"Error: patient id already exists.\n";
            return;
        }

        if(p.getFirstName().empty()||p.getSurname().empty()||hasSpace(p.getFirstName())||hasSpace(p.getSurname())||hasBadChar(p.getFirstName())||hasBadChar(p.getSurname()))
        {
            cout<<"Error: patient first name and surname must not be empty and must not contain spaces, |, or tab.\n";
            return;
        }

        if(p.getAge()<=0||p.getAge()>120)
        {
            cout<<"Error: invalid patient age.\n";
            return;
        }

        if(!validPhone(p.getPh()))
        {
            cout<<"Error: invalid phone number. Use 10 digits and do not start with 0.\n";
            return;
        }

        if(p.getDosha()<VATA||p.getDosha()>KAPHA)
        {
            cout<<"Error: invalid dosha value.\n";
            return;
        }

        patients.push_back(p);

        if(p.getID()>=nextID)
        {
            nextID=p.getID()+1;
        }

        cout<<"Patient added successfully.\n";
    }

    void updatePatient(int id,string name,string surname,int age,long long int no,Dosha dosha)
    {
        if(id<=0)
        {
            cout<<"Error: invalid patient id.\n";
            return;
        }

        if(name.empty()||surname.empty()||hasSpace(name)||hasSpace(surname)||hasBadChar(name)||hasBadChar(surname))
        {
            cout<<"Error: invalid name or surname.\n";
            return;
        }

        if(age<=0||age>120)
        {
            cout<<"Error: invalid age.\n";
            return;
        }

        if(!validPhone(no))
        {
            cout<<"Error: invalid phone number.\n";
            return;
        }

        if(dosha<VATA||dosha>KAPHA)
        {
            cout<<"Error: invalid dosha value.\n";
            return;
        }

        for(patient &p:patients)
        {
            if(p.getID()==id)
            {
                p.setName(name,surname);
                p.setAge(age);
                p.setNumber(no);
                p.setDosha(dosha);
                cout<<"Patient updated successfully.\n";
                return;
            }
        }

        cout<<"Patient not found.\n";
    }

    void delPatient(int id)
    {
        if(id<=0)
        {
            cout<<"Error: invalid patient id.\n";
            return;
        }

        for(int i=0;i<patients.size();i++)
        {
            if(patients[i].getID()==id)
            {
                patients.erase(patients.begin()+i);
                cout<<"Patient data deleted.\n";
                return;
            }
        }

        cout<<"Patient not found.\n";
    }

    void searchPatient(int id)
    {
        if(id<=0)
        {
            cout<<"Error: invalid patient id.\n";
            return;
        }

        for(patient &p:patients)
        {
            if(p.getID()==id)
            {
                p.display();
                return;
            }
        }

        cout<<"Patient not found.\n";
    }

    void displayAll()
    {
        if(patients.empty())
        {
            cout<<"No patient records available.\n";
            return;
        }

        for(patient &p:patients)
        {
            p.display();
        }
    }

    bool save()
    {
        ofstream file("Patient.txt");

        if(!file)
        {
            cout<<"Error: could not open Patient.txt for writing.\n";
            return false;
        }

        for(patient &p:patients)
        {
            file<<p.getID()<<" "
                <<p.getFirstName()<<" "
                <<p.getSurname()<<" "
                <<p.getAge()<<" "
                <<p.getPh()<<" "
                <<p.getDosha()<<"\n";
        }

        file.close();
        cout<<"Patient data saved.\n";
        return true;
    }

    void load()
    {
        patients.clear();
        nextID=1;

        ifstream file("Patient.txt");

        if(!file)
        {
            cout<<"Warning: Patient.txt not found. No previous patient data loaded.\n";
            return;
        }

        string line;
        int lineNo=0;
        int loaded=0;

        while(getline(file,line))
        {
            lineNo++;
            line=trim(line);

            if(line.empty())
            {
                continue;
            }

            stringstream ss(line);
            int id,age,doshaID;
            string name,surname;
            long long int ph;
            string extra;

            if(!(ss>>id>>name>>surname>>age>>ph>>doshaID))
            {
                cout<<"Invalid patient record skipped at line "<<lineNo<<".\n";
                continue;
            }

            if(ss>>extra)
            {
                cout<<"Invalid patient record skipped at line "<<lineNo<<" because it has extra data.\n";
                continue;
            }

            if(id<=0||age<=0||age>120||!validPhone(ph)||doshaID<0||doshaID>2||hasSpace(name)||hasSpace(surname)||hasBadChar(name)||hasBadChar(surname))
            {
                cout<<"Invalid patient values skipped at line "<<lineNo<<".\n";
                continue;
            }

            if(validate(id))
            {
                cout<<"Duplicate patient id skipped at line "<<lineNo<<".\n";
                continue;
            }

            patients.push_back(patient(id,name,surname,age,ph,(Dosha)doshaID));

            if(id>=nextID)
            {
                nextID=id+1;
            }

            loaded++;
        }

        file.close();

        if(loaded==0)
        {
            cout<<"No valid patient records found.\n";
        }
        else
        {
            cout<<"Patient records loaded successfully. Count: "<<loaded<<"\n";
        }
    }

    void updateID()
    {
        for(patient &p:patients)
        {
            if(p.getID()>=nextID)
            {
                nextID=p.getID()+1;
            }
        }
    }

    int getNextID()
    {
        return nextID;
    }
};

class Doctor
{
public:
    int id;
    string name,spec,phone,timing;
    double fee;

    Doctor()
    {
        id=0;
        fee=0;
        name="";
        spec="";
        phone="";
        timing="";
    }

    void display()
    {
        cout<<"\nID: "<<id
            <<"\nName: "<<name
            <<"\nSpecialization: "<<spec
            <<"\nPhone: "<<phone
            <<"\nFee: "<<fixed<<setprecision(2)<<fee
            <<"\nTiming: "<<timing<<"\n";
    }

    string serialize()
    {
        stringstream ss;
        ss<<id<<"|"<<name<<"|"<<spec<<"|"<<phone<<"|"<<fixed<<setprecision(2)<<fee<<"|"<<timing;
        return ss.str();
    }

    bool deserialize(string line)
    {
        vector<string> v=splitPipe(line);

        if(v.size()!=6)
        {
            return false;
        }

        int tempId;
        float tempFee;

        if(!parseInt(v[0],tempId))
        {
            return false;
        }

        if(!parseFloat(v[4],tempFee))
        {
            return false;
        }

        id=tempId;
        name=trim(v[1]);
        spec=trim(v[2]);
        phone=trim(v[3]);
        fee=tempFee;
        timing=trim(v[5]);

        if(id<0||name.empty()||spec.empty()||phone.empty()||timing.empty()||fee<0||hasBadChar(name)||hasBadChar(spec)||hasBadChar(phone)||hasBadChar(timing))
        {
            return false;
        }

        return true;
    }
};

class Appointment
{
public:
    int patientId,doctorId;
    string therapy,time;

    Appointment()
    {
        patientId=0;
        doctorId=0;
        therapy="";
        time="";
    }

    string serialize()
    {
        return to_string(patientId)+"|"+to_string(doctorId)+"|"+therapy+"|"+time;
    }

    bool deserialize(string line)
    {
        vector<string> v=splitPipe(line);

        if(v.size()!=4)
        {
            return false;
        }

        int pid,did;

        if(!parseInt(v[0],pid)||!parseInt(v[1],did))
        {
            return false;
        }

        patientId=pid;
        doctorId=did;
        therapy=trim(v[2]);
        time=trim(v[3]);

        if(patientId<=0||doctorId<0||therapy.empty()||time.empty()||hasBadChar(therapy)||hasBadChar(time))
        {
            return false;
        }

        return true;
    }
};

class Manager
{
    vector<Doctor> docs;

public:
    bool doctorIdExists(int id)
    {
        for(auto &d:docs)
        {
            if(d.id==id)
            {
                return true;
            }
        }
        return false;
    }

    void loadDoctors()
    {
        docs.clear();

        ifstream fin("doctor.txt");

        if(!fin)
        {
            cout<<"Warning: doctor.txt not found. No doctors loaded.\n";
            return;
        }

        string line;
        int lineNo=0;
        int loaded=0;

        while(getline(fin,line))
        {
            lineNo++;
            line=trim(line);

            if(line.empty())
            {
                continue;
            }

            Doctor d;

            if(!d.deserialize(line))
            {
                cout<<"Invalid doctor record skipped at line "<<lineNo<<".\n";
                continue;
            }

            if(doctorIdExists(d.id))
            {
                cout<<"Duplicate doctor id skipped at line "<<lineNo<<".\n";
                continue;
            }

            docs.push_back(d);
            loaded++;
        }

        fin.close();

        if(loaded==0)
        {
            cout<<"No valid doctor records found.\n";
        }
        else
        {
            cout<<"Doctor records loaded successfully. Count: "<<loaded<<"\n";
        }
    }

    bool saveDoctors()
    {
        ofstream fout("doctor.txt");

        if(!fout)
        {
            cout<<"Error: could not open doctor.txt for writing.\n";
            return false;
        }

        for(auto &x:docs)
        {
            fout<<x.serialize()<<"\n";
        }

        fout.close();
        return true;
    }

    int nextDoctorId()
    {
        int ans=0;

        for(auto &d:docs)
        {
            ans=max(ans,d.id+1);
        }

        return ans;
    }

    bool patientExists(int pid)
    {
        if(pid<=0)
        {
            return false;
        }

        ifstream fin("Patient.txt");

        if(!fin)
        {
            cout<<"Error: Patient.txt not found. Add patients first.\n";
            return false;
        }

        string line;

        while(getline(fin,line))
        {
            line=trim(line);

            if(line.empty())
            {
                continue;
            }

            stringstream ss(line);
            int id;
            string name,surname;
            int age,dosha;
            long long ph;

            if(ss>>id>>name>>surname>>age>>ph>>dosha)
            {
                if(id==pid)
                {
                    return true;
                }
            }
        }

        return false;
    }

    void addDoctor()
    {
        loadDoctors();

        Doctor d;
        d.id=nextDoctorId();

        cout<<"New Doctor ID: "<<d.id<<"\n";

        if(!inputText("Name: ",d.name,true))
        {
            return;
        }

        if(!inputText("Specialization: ",d.spec,true))
        {
            return;
        }

        if(!inputText("Phone: ",d.phone,false))
        {
            return;
        }

        long long phCheck;
        if(!parseLongLong(d.phone,phCheck)||!validPhone(phCheck))
        {
            cout<<"Error: phone must be 10 digits and must not start with 0.\n";
            return;
        }

        float tempFee;
        if(!inputFloat("Fee: ",tempFee,0))
        {
            return;
        }

        d.fee=tempFee;

        if(!inputText("Timing: ",d.timing,true))
        {
            return;
        }

        docs.push_back(d);

        if(saveDoctors())
        {
            cout<<"Doctor added with ID: "<<d.id<<"\n";
        }
    }

    void showDoctors()
    {
        loadDoctors();

        if(docs.empty())
        {
            cout<<"No doctors available.\n";
            return;
        }

        for(auto &d:docs)
        {
            d.display();
        }
    }

    void searchById()
    {
        loadDoctors();

        int id;
        if(!inputInt("Enter ID: ",id,0))
        {
            return;
        }

        for(auto &d:docs)
        {
            if(d.id==id)
            {
                d.display();
                return;
            }
        }

        cout<<"Doctor not found.\n";
    }

    void searchBySpec()
    {
        loadDoctors();

        string s;
        if(!inputText("Enter specialization: ",s,true))
        {
            return;
        }

        bool found=false;

        for(auto &d:docs)
        {
            if(d.spec==s)
            {
                d.display();
                found=true;
            }
        }

        if(!found)
        {
            cout<<"No doctor found with this specialization.\n";
        }
    }

    void deleteDoctor()
    {
        loadDoctors();

        int id;
        if(!inputInt("Enter ID to delete: ",id,0))
        {
            return;
        }

        if(docs.empty())
        {
            cout<<"No doctors available.\n";
            return;
        }

        vector<Doctor> temp;
        bool found=false;

        for(auto &d:docs)
        {
            if(d.id==id)
            {
                found=true;
            }
            else
            {
                temp.push_back(d);
            }
        }

        if(!found)
        {
            cout<<"Doctor not found.\n";
            return;
        }

        docs=temp;

        if(saveDoctors())
        {
            cout<<"Doctor deleted.\n";
        }
    }

    void bookAppointment()
    {
        loadDoctors();

        if(docs.empty())
        {
            cout<<"No doctors available. Add doctors first.\n";
            return;
        }

        int pid,did;

        if(!inputInt("Enter Patient ID: ",pid,1))
        {
            return;
        }

        if(!patientExists(pid))
        {
            cout<<"Patient not found.\n";
            return;
        }

        if(!inputInt("Enter Doctor ID: ",did,0))
        {
            return;
        }

        bool exists=false;

        for(auto &d:docs)
        {
            if(d.id==did)
            {
                exists=true;
            }
        }

        if(!exists)
        {
            cout<<"Doctor not found.\n";
            return;
        }

        Appointment a;
        a.patientId=pid;
        a.doctorId=did;

        if(!inputText("Enter Therapy: ",a.therapy,true))
        {
            return;
        }

        if(!inputText("Enter Time Slot: ",a.time,true))
        {
            return;
        }

        ifstream fin("appointments.txt");
        string line;
        int lineNo=0;

        if(fin)
        {
            while(getline(fin,line))
            {
                lineNo++;
                line=trim(line);

                if(line.empty())
                {
                    continue;
                }

                Appointment t;

                if(!t.deserialize(line))
                {
                    cout<<"Invalid appointment record ignored at line "<<lineNo<<".\n";
                    continue;
                }

                if(t.doctorId==did&&t.time==a.time)
                {
                    cout<<"Doctor already booked for this time slot.\n";
                    return;
                }
            }

            fin.close();
        }

        ofstream fout("appointments.txt",ios::app);

        if(!fout)
        {
            cout<<"Error: could not open appointments.txt for writing.\n";
            return;
        }

        fout<<a.serialize()<<"\n";
        fout.close();

        cout<<"Appointment booked.\n";
    }

    void viewAppointments()
    {
        ifstream fin("appointments.txt");

        if(!fin)
        {
            cout<<"No appointments file found.\n";
            return;
        }

        string line;
        bool empty=true;
        int lineNo=0;

        while(getline(fin,line))
        {
            lineNo++;
            line=trim(line);

            if(line.empty())
            {
                continue;
            }

            Appointment a;

            if(!a.deserialize(line))
            {
                cout<<"Invalid appointment record skipped at line "<<lineNo<<".\n";
                continue;
            }

            empty=false;

            cout<<"\nPatient ID: "<<a.patientId
                <<"\nDoctor ID: "<<a.doctorId
                <<"\nTherapy: "<<a.therapy
                <<"\nTime: "<<a.time<<"\n";
        }

        fin.close();

        if(empty)
        {
            cout<<"No appointments available.\n";
        }
    }
};

static void runDoctorMenu(Manager &m)
{
    int ch;

    do
    {
        cout<<"\n--- Doctor module ---\n";
        cout<<"1. Add Doctor\n";
        cout<<"2. Show Doctors\n";
        cout<<"3. Search by ID\n";
        cout<<"4. Search by Specialization\n";
        cout<<"5. Delete Doctor\n";
        cout<<"6. Book Appointment\n";
        cout<<"7. View Appointments\n";
        cout<<"0. Back to main menu\n";

        if(!inputInt("Choice: ",ch,0,7))
        {
            return;
        }

        switch(ch)
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
                cout<<"Invalid choice.\n";
                break;
        }
    }while(ch!=0);
}

static void runPatientMenu(patientManagement &manage)
{
    int ch;

    do
    {
        cout<<"\n--- Patient module ---\n";
        cout<<"1. Add patient\n";
        cout<<"2. Update patient\n";
        cout<<"3. Search patient\n";
        cout<<"4. Delete patient\n";
        cout<<"5. Display all patients\n";
        cout<<"6. Save to file\n";
        cout<<"0. Back to main menu\n";

        if(!inputInt("Choice: ",ch,0,6))
        {
            return;
        }

        int id,age,dosha;
        string name,surname;
        long long int ph;

        switch(ch)
        {
            case 1:
                if(!inputText("Enter First Name: ",name,false))
                {
                    break;
                }
                if(!inputText("Enter Surname: ",surname,false))
                {
                    break;
                }
                if(!inputInt("Enter Age: ",age,1,120))
                {
                    break;
                }
                if(!inputLongLong("Enter Phone Number: ",ph,1000000000LL,9999999999LL))
                {
                    break;
                }
                if(!validPhone(ph))
                {
                    cout<<"Error: phone number must be 10 digits and must not start with 0.\n";
                    break;
                }
                if(!inputInt("Enter Dosha(0=VATA,1=PITTA,2=KAPHA): ",dosha,0,2))
                {
                    break;
                }
                manage.addPatient(patient(manage.getNextID(),name,surname,age,ph,(Dosha)dosha));
                manage.save();
                break;

            case 2:
                if(!inputInt("Enter ID to Update: ",id,1))
                {
                    break;
                }
                if(!manage.validate(id))
                {
                    cout<<"ID not found.\n";
                    break;
                }
                if(!inputText("Enter First Name: ",name,false))
                {
                    break;
                }
                if(!inputText("Enter Surname: ",surname,false))
                {
                    break;
                }
                if(!inputInt("Enter Age: ",age,1,120))
                {
                    break;
                }
                if(!inputLongLong("Enter Phone Number: ",ph,1000000000LL,9999999999LL))
                {
                    break;
                }
                if(!validPhone(ph))
                {
                    cout<<"Error: phone number must be 10 digits and must not start with 0.\n";
                    break;
                }
                if(!inputInt("Enter Dosha(0=VATA,1=PITTA,2=KAPHA): ",dosha,0,2))
                {
                    break;
                }
                manage.updatePatient(id,name,surname,age,ph,(Dosha)dosha);
                manage.save();
                break;

            case 3:
                if(inputInt("Enter ID: ",id,1))
                {
                    manage.searchPatient(id);
                }
                break;

            case 4:
                if(inputInt("Enter ID: ",id,1))
                {
                    manage.delPatient(id);
                    manage.save();
                }
                break;

            case 5:
                manage.displayAll();
                break;

            case 6:
                manage.save();
                break;

            case 0:
                break;

            default:
                cout<<"Invalid choice.\n";
                break;
        }
    }while(ch!=0);
}

static void runMedicineBillingMenu(Inventory &inv)
{
    int choice;

    do
    {
        cout<<"\n--- Medicine & Billing module ---\n";
        cout<<"1. Load inventory\n";
        cout<<"2. Display inventory\n";
        cout<<"3. Add medicine\n";
        cout<<"4. Remove medicine\n";
        cout<<"5. Search medicine by ID\n";
        cout<<"6. Search medicine by name\n";
        cout<<"7. Update medicine\n";
        cout<<"8. Check stock / expiry alerts\n";
        cout<<"9. Generate bill\n";
        cout<<"0. Back to main menu\n";

        if(!inputInt("Enter your choice: ",choice,0,9))
        {
            return;
        }

        if(choice==1)
        {
            inv.loadFromFile();
        }
        else if(choice==2)
        {
            inv.displayInventory();
        }
        else if(choice==3)
        {
            int id,qty;
            string name,expiry;
            float price;

            if(!inputInt("Enter ID: ",id,1))
            {
                continue;
            }
            if(!inputText("Enter Name(without space): ",name,false))
            {
                continue;
            }
            if(!inputInt("Enter Quantity: ",qty,0))
            {
                continue;
            }
            if(!inputFloat("Enter Price: ",price,0))
            {
                continue;
            }
            if(!inputDate("Enter Expiry Date(dd/mm/yyyy): ",expiry))
            {
                continue;
            }

            Medicine m(id,name,qty,expiry,price);
            inv.addMedicine(m);
        }
        else if(choice==4)
        {
            int id;
            if(inputInt("Enter the id of the medicine to be removed: ",id,1))
            {
                inv.removeMedicine(id);
            }
        }
        else if(choice==5)
        {
            int id;
            if(inputInt("Enter the id to search the medicine: ",id,1))
            {
                Medicine *m=inv.searchMedicine(id);

                if(m)
                {
                    cout<<"Found: "<<m->getName()<<" | "<<fixed<<setprecision(2)<<m->getPrice()<<" | "<<m->getQuantity()<<"\n";
                }
                else
                {
                    cout<<"Not found.\n";
                }
            }
        }
        else if(choice==6)
        {
            string name;
            if(inputText("Enter the name to search the medicine: ",name,false))
            {
                Medicine *m=inv.searchMedicine(name);

                if(m)
                {
                    cout<<"Found: "<<m->getId()<<" | "<<fixed<<setprecision(2)<<m->getPrice()<<" | "<<m->getQuantity()<<"\n";
                }
                else
                {
                    cout<<"Not found.\n";
                }
            }
        }
        else if(choice==7)
        {
            int id,qty;
            float price;

            if(!inputInt("Enter the id: ",id,1))
            {
                continue;
            }
            if(!inputInt("Enter new quantity: ",qty,0))
            {
                continue;
            }
            if(!inputFloat("Enter new price: ",price,0))
            {
                continue;
            }

            inv.updateMedicine(id,qty,price);
        }
        else if(choice==8)
        {
            inv.checkStock();
        }
        else if(choice==9)
        {
            inv.loadFromFile();

            int billId;
            if(!inputInt("Enter the bill ID: ",billId,1))
            {
                continue;
            }

            Billing bill(billId);

            while(true)
            {
                int id,q;

                if(!inputInt("Enter medicine ID: ",id,1))
                {
                    break;
                }

                if(!inputInt("Enter quantity: ",q,1))
                {
                    break;
                }

                Medicine *med=inv.searchMedicine(id);

                if(med)
                {
                    int oldQty=med->getQuantity();

                    if(bill.addItem(*med,q))
                    {
                        inv.updateMedicine(id,oldQty-q,med->getPrice());
                    }
                }
                else
                {
                    cout<<"Medicine not found.\n";
                }

                string more;
                if(!inputText("Enter more items(y/n): ",more,false))
                {
                    break;
                }

                if(more!="y"&&more!="Y")
                {
                    break;
                }
            }

            if(!bill.isEmpty())
            {
                bill.saveBillToFile();
                bill.printBill();
            }
            else
            {
                cout<<"Bill cancelled because no valid items were added.\n";
            }
        }
        else if(choice==0)
        {
            // back
        }
        else
        {
            cout<<"Enter a valid choice.\n";
        }
    }while(choice!=0);
}

int main()
{
    patientManagement patients;
    patients.load();
    patients.updateID();

    Manager doctorManager;
    Inventory inventory;
    inventory.loadFromFile();

    int root;

    do
    {
        cout<<"\n========== Ayurcare ==========\n";
        cout<<"1. Doctor\n";
        cout<<"2. Patient\n";
        cout<<"3. Medicine & Billing\n";
        cout<<"0. Exit\n";

        if(!inputInt("Select module: ",root,0,3))
        {
            cout<<"Fatal input error. Exiting.\n";
            return 0;
        }

        if(root==1)
        {
            runDoctorMenu(doctorManager);
        }
        else if(root==2)
        {
            runPatientMenu(patients);
        }
        else if(root==3)
        {
            runMedicineBillingMenu(inventory);
        }
        else if(root==0)
        {
            cout<<"Exiting.\n";
        }
        else
        {
            cout<<"Invalid choice.\n";
        }
    }while(root!=0);

    patients.save();

    return 0;
}
