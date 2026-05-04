#include<bits/stdc++.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
using namespace std;

struct patientData
{
    int id;
    string firstName;
    string surname;
    int age;
    string phone;
    int dosha;
};

struct doctorData
{
    int id;
    string name;
    string spec;
    string phone;
    float fee;
    string timing;
};

struct medicineData
{
    int id;
    string name;
    int qty;
    string expiry;
    float price;
};

struct visitData
{
    int patientId;
    int doctorId;
    string therapy;
    string time;
};

struct billItemData
{
    int medId;
    string name;
    int qty;
    float price;
    float amount;
};

vector<patientData> patientList;
vector<doctorData> doctorList;
vector<medicineData> medicineList;
vector<visitData> visitList;

int page=0;
string msg="Ready";

string patientFile="Patient.txt";
string doctorFile="doctor.txt";
string medicineFile="data/medicine.txt";
string visitFile="appointments.txt";
string billFile="data/billing.txt";

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

bool validPhone(string phone)
{
    phone=trim(phone);

    if(phone.size()!=10)
    {
        return false;
    }

    if(phone[0]=='0')
    {
        return false;
    }

    for(int i=0;i<phone.size();i++)
    {
        if(!isdigit((unsigned char)phone[i]))
        {
            return false;
        }
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

bool parseDate(string date)
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

    int day=stoi(d);
    int month=stoi(m);
    int year=stoi(y);

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

bool isNearExpiry(string date)
{
    if(!parseDate(date))
    {
        return false;
    }

    int month=stoi(date.substr(3,2));
    int year=stoi(date.substr(6,4));

    int currentYear=2026;
    int currentMonth=5;

    if(year<currentYear)
    {
        return true;
    }

    if(year==currentYear&&month<=currentMonth+1)
    {
        return true;
    }

    return false;
}

string doshaName(int dosha)
{
    if(dosha==0)
    {
        return "VATA";
    }
    if(dosha==1)
    {
        return "PITTA";
    }
    if(dosha==2)
    {
        return "KAPHA";
    }
    return "UNKNOWN";
}

int findPatient(int id)
{
    for(int i=0;i<patientList.size();i++)
    {
        if(patientList[i].id==id)
        {
            return i;
        }
    }

    return -1;
}

int findDoctor(int id)
{
    for(int i=0;i<doctorList.size();i++)
    {
        if(doctorList[i].id==id)
        {
            return i;
        }
    }

    return -1;
}

int findMedicine(int id)
{
    for(int i=0;i<medicineList.size();i++)
    {
        if(medicineList[i].id==id)
        {
            return i;
        }
    }

    return -1;
}

int nextPatientId()
{
    int ans=1;

    for(int i=0;i<patientList.size();i++)
    {
        ans=max(ans,patientList[i].id+1);
    }

    return ans;
}

int nextDoctorId()
{
    int ans=0;

    for(int i=0;i<doctorList.size();i++)
    {
        ans=max(ans,doctorList[i].id+1);
    }

    return ans;
}

int nextMedicineId()
{
    int ans=1;

    for(int i=0;i<medicineList.size();i++)
    {
        ans=max(ans,medicineList[i].id+1);
    }

    return ans;
}

void loadPatients()
{
    patientList.clear();

    ifstream fin(patientFile);

    if(!fin)
    {
        return;
    }

    string line;

    while(getline(fin,line))
    {
        line=trim(line);

        if(line.size()==0)
        {
            continue;
        }

        stringstream ss(line);
        patientData p;
        string extra;

        if(!(ss>>p.id>>p.firstName>>p.surname>>p.age>>p.phone>>p.dosha))
        {
            continue;
        }

        if(ss>>extra)
        {
            continue;
        }

        if(p.id<=0||p.age<=0||p.age>120||!validPhone(p.phone)||p.dosha<0||p.dosha>2)
        {
            continue;
        }

        if(hasSpace(p.firstName)||hasSpace(p.surname)||hasBadChar(p.firstName)||hasBadChar(p.surname))
        {
            continue;
        }

        if(findPatient(p.id)!=-1)
        {
            continue;
        }

        patientList.push_back(p);
    }

    fin.close();
}

bool savePatients()
{
    ofstream fout(patientFile);

    if(!fout)
    {
        msg="Cannot save Patient.txt";
        return false;
    }

    for(int i=0;i<patientList.size();i++)
    {
        fout<<patientList[i].id<<" "
            <<patientList[i].firstName<<" "
            <<patientList[i].surname<<" "
            <<patientList[i].age<<" "
            <<patientList[i].phone<<" "
            <<patientList[i].dosha<<"\n";
    }

    fout.close();
    return true;
}

void loadDoctors()
{
    doctorList.clear();

    ifstream fin(doctorFile);

    if(!fin)
    {
        return;
    }

    string line;

    while(getline(fin,line))
    {
        line=trim(line);

        if(line.size()==0)
        {
            continue;
        }

        vector<string> v=splitPipe(line);

        if(v.size()!=6)
        {
            continue;
        }

        doctorData d;

        try
        {
            d.id=stoi(trim(v[0]));
            d.name=trim(v[1]);
            d.spec=trim(v[2]);
            d.phone=trim(v[3]);
            d.fee=stof(trim(v[4]));
            d.timing=trim(v[5]);
        }
        catch(...)
        {
            continue;
        }

        if(d.id<0||d.name.size()==0||d.spec.size()==0||d.phone.size()==0||d.timing.size()==0||d.fee<0)
        {
            continue;
        }

        if(hasBadChar(d.name)||hasBadChar(d.spec)||hasBadChar(d.phone)||hasBadChar(d.timing))
        {
            continue;
        }

        if(findDoctor(d.id)!=-1)
        {
            continue;
        }

        doctorList.push_back(d);
    }

    fin.close();
}

bool saveDoctors()
{
    ofstream fout(doctorFile);

    if(!fout)
    {
        msg="Cannot save doctor.txt";
        return false;
    }

    for(int i=0;i<doctorList.size();i++)
    {
        fout<<doctorList[i].id<<"|"
            <<doctorList[i].name<<"|"
            <<doctorList[i].spec<<"|"
            <<doctorList[i].phone<<"|"
            <<fixed<<setprecision(2)<<doctorList[i].fee<<"|"
            <<doctorList[i].timing<<"\n";
    }

    fout.close();
    return true;
}

void loadMedicine()
{
    medicineList.clear();

    ifstream fin(medicineFile);

    if(!fin)
    {
        return;
    }

    string line;

    while(getline(fin,line))
    {
        line=trim(line);

        if(line.size()==0)
        {
            continue;
        }

        stringstream ss(line);
        medicineData m;
        string extra;

        if(!(ss>>m.id>>m.name>>m.qty>>m.expiry>>m.price))
        {
            continue;
        }

        if(ss>>extra)
        {
            continue;
        }

        if(m.id<=0||m.name.size()==0||m.qty<0||m.price<0||!parseDate(m.expiry))
        {
            continue;
        }

        if(hasSpace(m.name)||hasBadChar(m.name))
        {
            continue;
        }

        if(findMedicine(m.id)!=-1)
        {
            continue;
        }

        medicineList.push_back(m);
    }

    fin.close();
}

bool saveMedicine()
{
    ofstream fout(medicineFile);

    if(!fout)
    {
        msg="Cannot save data/medicine.txt";
        return false;
    }

    for(int i=0;i<medicineList.size();i++)
    {
        fout<<medicineList[i].id<<"\t"
            <<medicineList[i].name<<"\t"
            <<medicineList[i].qty<<"\t"
            <<medicineList[i].expiry<<"\t"
            <<fixed<<setprecision(2)<<medicineList[i].price<<"\n";
    }

    fout.close();
    return true;
}

void loadVisits()
{
    visitList.clear();

    ifstream fin(visitFile);

    if(!fin)
    {
        return;
    }

    string line;

    while(getline(fin,line))
    {
        line=trim(line);

        if(line.size()==0)
        {
            continue;
        }

        vector<string> v=splitPipe(line);

        if(v.size()!=4)
        {
            continue;
        }

        visitData a;

        try
        {
            a.patientId=stoi(trim(v[0]));
            a.doctorId=stoi(trim(v[1]));
            a.therapy=trim(v[2]);
            a.time=trim(v[3]);
        }
        catch(...)
        {
            continue;
        }

        if(a.patientId<=0||a.doctorId<0||a.therapy.size()==0||a.time.size()==0)
        {
            continue;
        }

        if(hasBadChar(a.therapy)||hasBadChar(a.time))
        {
            continue;
        }

        visitList.push_back(a);
    }

    fin.close();
}

bool saveVisits()
{
    ofstream fout(visitFile);

    if(!fout)
    {
        msg="Cannot save appointments.txt";
        return false;
    }

    for(int i=0;i<visitList.size();i++)
    {
        fout<<visitList[i].patientId<<"|"
            <<visitList[i].doctorId<<"|"
            <<visitList[i].therapy<<"|"
            <<visitList[i].time<<"\n";
    }

    fout.close();
    return true;
}

void loadAll()
{
    loadPatients();
    loadDoctors();
    loadMedicine();
    loadVisits();
}

void setTheme()
{
    ImGui::StyleColorsLight();

    ImGuiStyle& style=ImGui::GetStyle();
    style.WindowRounding=0.0f;
    style.FrameRounding=0.0f;
    style.GrabRounding=0.0f;
    style.TabRounding=0.0f;
    style.ScrollbarRounding=0.0f;
    style.WindowPadding=ImVec2(8,8);
    style.FramePadding=ImVec2(6,4);
    style.ItemSpacing=ImVec2(6,6);
    style.ItemInnerSpacing=ImVec2(4,4);
    style.WindowBorderSize=1.0f;
    style.FrameBorderSize=1.0f;
}

void bigTitle(const char* text)
{
    ImGui::Text("%s",text);
    ImGui::Separator();
}

void metricBox(const char* title,int value)
{
    ImGui::BeginChild(title,ImVec2(220,90),true);
    ImGui::Text("%s",title);
    ImGui::Separator();
    ImGui::Text("%d",value);
    ImGui::EndChild();
}

void sideMenu()
{
    ImGui::SetNextWindowPos(ImVec2(10,10),ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(250,680),ImGuiCond_Always);
    ImGui::Begin("AyurCare Menu",NULL,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoCollapse);

    ImGui::Text("AyurCare");
    ImGui::Text("Management System");
    ImGui::Separator();

    if(ImGui::Button("Dashboard",ImVec2(220,45)))
    {
        page=0;
    }
    if(ImGui::Button("Patients",ImVec2(220,45)))
    {
        page=1;
    }
    if(ImGui::Button("Doctors",ImVec2(220,45)))
    {
        page=2;
    }
    if(ImGui::Button("Inventory",ImVec2(220,45)))
    {
        page=3;
    }
    if(ImGui::Button("Appointments",ImVec2(220,45)))
    {
        page=4;
    }
    if(ImGui::Button("Billing",ImVec2(220,45)))
    {
        page=5;
    }
    if(ImGui::Button("Reports",ImVec2(220,45)))
    {
        page=6;
    }

    ImGui::Separator();

    if(ImGui::Button("Reload Data",ImVec2(220,40)))
    {
        loadAll();
        msg="Data reloaded";
    }

    ImGui::Separator();
    ImGui::TextWrapped("Status: %s",msg.c_str());

    ImGui::End();
}

void beginMainWindow(const char* name)
{
    ImGui::SetNextWindowPos(ImVec2(270,10),ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(1080,680),ImGuiCond_Always);
    ImGui::Begin(name,NULL,ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoCollapse);
}

void showDashboard()
{
    beginMainWindow("Dashboard");
    bigTitle("Ayurvedic Hospital And Panchakarma Centre Management System");

    metricBox("Patients",(int)patientList.size());
    ImGui::SameLine();
    metricBox("Doctors",(int)doctorList.size());
    ImGui::SameLine();
    metricBox("Medicines",(int)medicineList.size());
    ImGui::SameLine();
    metricBox("Appointments",(int)visitList.size());

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("Quick actions");

    if(ImGui::Button("Add Patient",ImVec2(180,45)))
    {
        page=1;
    }
    ImGui::SameLine();
    if(ImGui::Button("Book Appointment",ImVec2(220,45)))
    {
        page=4;
    }
    ImGui::SameLine();
    if(ImGui::Button("Create Bill",ImVec2(180,45)))
    {
        page=5;
    }

    ImGui::End();
}

void showPatients()
{
    static int id=0;
    static int age=0;
    static char firstName[80]="";
    static char surname[80]="";
    static char phone[40]="";
    static int dosha=0;

    beginMainWindow("Patient Management");
    bigTitle("Patient Management");

    if(id==0)
    {
        id=nextPatientId();
    }

    ImGui::BeginChild("patientForm",ImVec2(0,210),true);
    ImGui::InputInt("Patient Id",&id);
    ImGui::InputText("First Name",firstName,80);
    ImGui::InputText("Surname",surname,80);
    ImGui::InputInt("Age",&age);
    ImGui::InputText("Phone",phone,40);
    ImGui::Combo("Dosha",&dosha,"VATA\0PITTA\0KAPHA\0");

    if(ImGui::Button("Add Patient",ImVec2(150,38)))
    {
        string f=trim(firstName);
        string s=trim(surname);
        string ph=trim(phone);

        if(id<=0)
        {
            msg="Invalid patient id";
        }
        else if(f.size()==0||s.size()==0||hasSpace(f)||hasSpace(s)||hasBadChar(f)||hasBadChar(s))
        {
            msg="First name and surname cannot contain space, | or tab";
        }
        else if(age<=0||age>120)
        {
            msg="Invalid age";
        }
        else if(!validPhone(ph))
        {
            msg="Invalid phone number";
        }
        else if(findPatient(id)!=-1)
        {
            msg="Patient id already exists";
        }
        else
        {
            patientData p;
            p.id=id;
            p.firstName=f;
            p.surname=s;
            p.age=age;
            p.phone=ph;
            p.dosha=dosha;

            patientList.push_back(p);

            if(savePatients())
            {
                msg="Patient added";
                id=nextPatientId();
                age=0;
                firstName[0]='\0';
                surname[0]='\0';
                phone[0]='\0';
            }
        }
    }

    ImGui::SameLine();

    if(ImGui::Button("Update Patient",ImVec2(160,38)))
    {
        int pos=findPatient(id);
        string f=trim(firstName);
        string s=trim(surname);
        string ph=trim(phone);

        if(pos==-1)
        {
            msg="Patient id not found";
        }
        else if(f.size()==0||s.size()==0||hasSpace(f)||hasSpace(s)||hasBadChar(f)||hasBadChar(s))
        {
            msg="Invalid name or surname";
        }
        else if(age<=0||age>120)
        {
            msg="Invalid age";
        }
        else if(!validPhone(ph))
        {
            msg="Invalid phone number";
        }
        else
        {
            patientList[pos].firstName=f;
            patientList[pos].surname=s;
            patientList[pos].age=age;
            patientList[pos].phone=ph;
            patientList[pos].dosha=dosha;

            if(savePatients())
            {
                msg="Patient updated";
            }
        }
    }

    ImGui::SameLine();

    if(ImGui::Button("Delete By Id",ImVec2(150,38)))
    {
        int pos=findPatient(id);

        if(pos==-1)
        {
            msg="Patient id not found";
        }
        else
        {
            patientList.erase(patientList.begin()+pos);

            if(savePatients())
            {
                msg="Patient deleted";
                id=nextPatientId();
            }
        }
    }

    ImGui::EndChild();

    if(ImGui::BeginTable("patientTable",6,ImGuiTableFlags_Borders|ImGuiTableFlags_RowBg|ImGuiTableFlags_Resizable|ImGuiTableFlags_ScrollY,ImVec2(0,0)))
    {
        ImGui::TableSetupColumn("Id");
        ImGui::TableSetupColumn("First Name");
        ImGui::TableSetupColumn("Surname");
        ImGui::TableSetupColumn("Age");
        ImGui::TableSetupColumn("Phone");
        ImGui::TableSetupColumn("Dosha");
        ImGui::TableHeadersRow();

        for(int i=0;i<patientList.size();i++)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d",patientList[i].id);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s",patientList[i].firstName.c_str());
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%s",patientList[i].surname.c_str());
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%d",patientList[i].age);
            ImGui::TableSetColumnIndex(4);
            ImGui::Text("%s",patientList[i].phone.c_str());
            ImGui::TableSetColumnIndex(5);
            ImGui::Text("%s",doshaName(patientList[i].dosha).c_str());
        }

        ImGui::EndTable();
    }

    ImGui::End();
}

void showDoctors()
{
    static int id=-1;
    static char name[80]="";
    static char spec[80]="";
    static char phone[40]="";
    static float fee=0.0f;
    static char timing[80]="";

    beginMainWindow("Doctor Management");
    bigTitle("Doctor Management");

    if(id<0)
    {
        id=nextDoctorId();
    }

    ImGui::BeginChild("doctorForm",ImVec2(0,210),true);
    ImGui::InputInt("Doctor Id",&id);
    ImGui::InputText("Name",name,80);
    ImGui::InputText("Specialization",spec,80);
    ImGui::InputText("Phone",phone,40);
    ImGui::InputFloat("Fee",&fee);
    ImGui::InputText("Timing",timing,80);

    if(ImGui::Button("Add Doctor",ImVec2(150,38)))
    {
        string n=trim(name);
        string sp=trim(spec);
        string ph=trim(phone);
        string t=trim(timing);

        if(id<0)
        {
            msg="Invalid doctor id";
        }
        else if(n.size()==0||sp.size()==0||t.size()==0||hasBadChar(n)||hasBadChar(sp)||hasBadChar(t))
        {
            msg="Invalid doctor details";
        }
        else if(!validPhone(ph))
        {
            msg="Invalid phone number";
        }
        else if(fee<0)
        {
            msg="Invalid fee";
        }
        else if(findDoctor(id)!=-1)
        {
            msg="Doctor id already exists";
        }
        else
        {
            doctorData d;
            d.id=id;
            d.name=n;
            d.spec=sp;
            d.phone=ph;
            d.fee=fee;
            d.timing=t;

            doctorList.push_back(d);

            if(saveDoctors())
            {
                msg="Doctor added";
                id=nextDoctorId();
                fee=0;
                name[0]='\0';
                spec[0]='\0';
                phone[0]='\0';
                timing[0]='\0';
            }
        }
    }

    ImGui::SameLine();

    if(ImGui::Button("Update Doctor",ImVec2(160,38)))
    {
        int pos=findDoctor(id);
        string n=trim(name);
        string sp=trim(spec);
        string ph=trim(phone);
        string t=trim(timing);

        if(pos==-1)
        {
            msg="Doctor id not found";
        }
        else if(n.size()==0||sp.size()==0||t.size()==0||hasBadChar(n)||hasBadChar(sp)||hasBadChar(t))
        {
            msg="Invalid doctor details";
        }
        else if(!validPhone(ph))
        {
            msg="Invalid phone number";
        }
        else if(fee<0)
        {
            msg="Invalid fee";
        }
        else
        {
            doctorList[pos].name=n;
            doctorList[pos].spec=sp;
            doctorList[pos].phone=ph;
            doctorList[pos].fee=fee;
            doctorList[pos].timing=t;

            if(saveDoctors())
            {
                msg="Doctor updated";
            }
        }
    }

    ImGui::SameLine();

    if(ImGui::Button("Delete By Id",ImVec2(150,38)))
    {
        int pos=findDoctor(id);

        if(pos==-1)
        {
            msg="Doctor id not found";
        }
        else
        {
            doctorList.erase(doctorList.begin()+pos);

            if(saveDoctors())
            {
                msg="Doctor deleted";
                id=nextDoctorId();
            }
        }
    }

    ImGui::EndChild();

    if(ImGui::BeginTable("doctorTable",6,ImGuiTableFlags_Borders|ImGuiTableFlags_RowBg|ImGuiTableFlags_Resizable|ImGuiTableFlags_ScrollY,ImVec2(0,0)))
    {
        ImGui::TableSetupColumn("Id");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Specialization");
        ImGui::TableSetupColumn("Phone");
        ImGui::TableSetupColumn("Fee");
        ImGui::TableSetupColumn("Timing");
        ImGui::TableHeadersRow();

        for(int i=0;i<doctorList.size();i++)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d",doctorList[i].id);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s",doctorList[i].name.c_str());
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%s",doctorList[i].spec.c_str());
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%s",doctorList[i].phone.c_str());
            ImGui::TableSetColumnIndex(4);
            ImGui::Text("Rs %.2f",doctorList[i].fee);
            ImGui::TableSetColumnIndex(5);
            ImGui::Text("%s",doctorList[i].timing.c_str());
        }

        ImGui::EndTable();
    }

    ImGui::End();
}

void showMedicine()
{
    static int id=0;
    static int qty=0;
    static float price=0.0f;
    static char name[80]="";
    static char expiry[20]="";

    beginMainWindow("Medicine Inventory");
    bigTitle("Medicine Inventory");

    if(id==0)
    {
        id=nextMedicineId();
    }

    ImGui::BeginChild("medicineForm",ImVec2(0,210),true);
    ImGui::InputInt("Medicine Id",&id);
    ImGui::InputText("Name Without Space",name,80);
    ImGui::InputInt("Quantity",&qty);
    ImGui::InputText("Expiry dd/mm/yyyy",expiry,20);
    ImGui::InputFloat("Price",&price);

    if(ImGui::Button("Add Medicine",ImVec2(160,38)))
    {
        string n=trim(name);
        string e=trim(expiry);

        if(id<=0)
        {
            msg="Invalid medicine id";
        }
        else if(n.size()==0||hasSpace(n)||hasBadChar(n))
        {
            msg="Medicine name cannot contain space, | or tab";
        }
        else if(qty<0)
        {
            msg="Quantity cannot be negative";
        }
        else if(price<0)
        {
            msg="Price cannot be negative";
        }
        else if(!parseDate(e))
        {
            msg="Invalid expiry date";
        }
        else if(findMedicine(id)!=-1)
        {
            msg="Medicine id already exists";
        }
        else
        {
            medicineData m;
            m.id=id;
            m.name=n;
            m.qty=qty;
            m.expiry=e;
            m.price=price;

            medicineList.push_back(m);

            if(saveMedicine())
            {
                msg="Medicine added";
                id=nextMedicineId();
                qty=0;
                price=0;
                name[0]='\0';
                expiry[0]='\0';
            }
        }
    }

    ImGui::SameLine();

    if(ImGui::Button("Update Medicine",ImVec2(170,38)))
    {
        int pos=findMedicine(id);
        string n=trim(name);
        string e=trim(expiry);

        if(pos==-1)
        {
            msg="Medicine id not found";
        }
        else if(n.size()==0||hasSpace(n)||hasBadChar(n))
        {
            msg="Invalid medicine name";
        }
        else if(qty<0||price<0)
        {
            msg="Invalid quantity or price";
        }
        else if(!parseDate(e))
        {
            msg="Invalid expiry date";
        }
        else
        {
            medicineList[pos].name=n;
            medicineList[pos].qty=qty;
            medicineList[pos].expiry=e;
            medicineList[pos].price=price;

            if(saveMedicine())
            {
                msg="Medicine updated";
            }
        }
    }

    ImGui::SameLine();

    if(ImGui::Button("Delete By Id",ImVec2(150,38)))
    {
        int pos=findMedicine(id);

        if(pos==-1)
        {
            msg="Medicine id not found";
        }
        else
        {
            medicineList.erase(medicineList.begin()+pos);

            if(saveMedicine())
            {
                msg="Medicine deleted";
                id=nextMedicineId();
            }
        }
    }

    ImGui::EndChild();

    if(ImGui::BeginTable("medicineTable",6,ImGuiTableFlags_Borders|ImGuiTableFlags_RowBg|ImGuiTableFlags_Resizable|ImGuiTableFlags_ScrollY,ImVec2(0,0)))
    {
        ImGui::TableSetupColumn("Id");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Quantity");
        ImGui::TableSetupColumn("Expiry");
        ImGui::TableSetupColumn("Price");
        ImGui::TableSetupColumn("Status");
        ImGui::TableHeadersRow();

        for(int i=0;i<medicineList.size();i++)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d",medicineList[i].id);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s",medicineList[i].name.c_str());
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%d",medicineList[i].qty);
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%s",medicineList[i].expiry.c_str());
            ImGui::TableSetColumnIndex(4);
            ImGui::Text("Rs %.2f",medicineList[i].price);
            ImGui::TableSetColumnIndex(5);

            if(medicineList[i].qty<5&&isNearExpiry(medicineList[i].expiry))
            {
                ImGui::Text("Low + Expiry Alert");
            }
            else if(medicineList[i].qty<5)
            {
                ImGui::Text("Low Stock");
            }
            else if(isNearExpiry(medicineList[i].expiry))
            {
                ImGui::Text("Expiry Alert");
            }
            else
            {
                ImGui::Text("Available");
            }
        }

        ImGui::EndTable();
    }

    ImGui::End();
}

bool doctorBooked(int doctorId,string time)
{
    for(int i=0;i<visitList.size();i++)
    {
        if(visitList[i].doctorId==doctorId&&visitList[i].time==time)
        {
            return true;
        }
    }

    return false;
}

void showVisits()
{
    static int patientId=0;
    static int doctorId=0;
    static char therapy[80]="";
    static char time[80]="";

    beginMainWindow("Appointments");
    bigTitle("Appointments");

    ImGui::BeginChild("visitForm",ImVec2(0,160),true);
    ImGui::InputInt("Patient Id",&patientId);
    ImGui::InputInt("Doctor Id",&doctorId);
    ImGui::InputText("Therapy",therapy,80);
    ImGui::InputText("Time Slot",time,80);

    if(ImGui::Button("Book Appointment",ImVec2(190,38)))
    {
        string th=trim(therapy);
        string tm=trim(time);

        if(patientId<=0||findPatient(patientId)==-1)
        {
            msg="Patient not found";
        }
        else if(doctorId<0||findDoctor(doctorId)==-1)
        {
            msg="Doctor not found";
        }
        else if(th.size()==0||tm.size()==0||hasBadChar(th)||hasBadChar(tm))
        {
            msg="Invalid therapy or time";
        }
        else if(doctorBooked(doctorId,tm))
        {
            msg="Doctor already booked for this time";
        }
        else
        {
            visitData a;
            a.patientId=patientId;
            a.doctorId=doctorId;
            a.therapy=th;
            a.time=tm;

            visitList.push_back(a);

            if(saveVisits())
            {
                msg="Appointment booked";
                patientId=0;
                doctorId=0;
                therapy[0]='\0';
                time[0]='\0';
            }
        }
    }

    ImGui::EndChild();

    if(ImGui::BeginTable("visitTable",4,ImGuiTableFlags_Borders|ImGuiTableFlags_RowBg|ImGuiTableFlags_Resizable|ImGuiTableFlags_ScrollY,ImVec2(0,0)))
    {
        ImGui::TableSetupColumn("Patient Id");
        ImGui::TableSetupColumn("Doctor Id");
        ImGui::TableSetupColumn("Therapy");
        ImGui::TableSetupColumn("Time");
        ImGui::TableHeadersRow();

        for(int i=0;i<visitList.size();i++)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d",visitList[i].patientId);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%d",visitList[i].doctorId);
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%s",visitList[i].therapy.c_str());
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%s",visitList[i].time.c_str());
        }

        ImGui::EndTable();
    }

    ImGui::End();
}

void restoreBillStock(vector<billItemData> &items)
{
    for(int i=0;i<items.size();i++)
    {
        int pos=findMedicine(items[i].medId);

        if(pos!=-1)
        {
            medicineList[pos].qty+=items[i].qty;
        }
    }

    saveMedicine();
}

void showBilling()
{
    static int billId=1;
    static int medId=0;
    static int qty=1;
    static float total=0.0f;
    static vector<billItemData> currentBill;

    beginMainWindow("Billing");
    bigTitle("Billing System");

    ImGui::BeginChild("billingForm",ImVec2(0,210),true);
    ImGui::InputInt("Bill Id",&billId);
    ImGui::InputInt("Medicine Id",&medId);
    ImGui::InputInt("Quantity",&qty);

    if(ImGui::Button("Add Item",ImVec2(130,38)))
    {
        int pos=findMedicine(medId);

        if(billId<=0)
        {
            msg="Invalid bill id";
        }
        else if(pos==-1)
        {
            msg="Medicine not found";
        }
        else if(qty<=0)
        {
            msg="Quantity must be positive";
        }
        else if(medicineList[pos].qty<qty)
        {
            msg="Not enough stock";
        }
        else
        {
            billItemData item;
            item.medId=medicineList[pos].id;
            item.name=medicineList[pos].name;
            item.qty=qty;
            item.price=medicineList[pos].price;
            item.amount=qty*medicineList[pos].price;

            currentBill.push_back(item);
            total+=item.amount;

            medicineList[pos].qty-=qty;
            saveMedicine();

            msg="Item added to bill";
        }
    }

    ImGui::SameLine();

    if(ImGui::Button("Save Bill",ImVec2(130,38)))
    {
        if(billId<=0)
        {
            msg="Invalid bill id";
        }
        else if(currentBill.size()==0)
        {
            msg="Bill is empty";
        }
        else
        {
            ofstream fout(billFile,ios::app);

            if(!fout)
            {
                msg="Cannot save data/billing.txt";
            }
            else
            {
                fout<<"Bill ID: "<<billId<<" Total: "<<fixed<<setprecision(2)<<total<<"\n";

                for(int i=0;i<currentBill.size();i++)
                {
                    fout<<currentBill[i].name<<" | "<<currentBill[i].qty<<" | "<<fixed<<setprecision(2)<<currentBill[i].price<<"\n";
                }

                fout<<"--------------\n";
                fout.close();

                currentBill.clear();
                total=0;
                billId++;
                msg="Bill saved";
            }
        }
    }

    ImGui::SameLine();

    if(ImGui::Button("Cancel Bill",ImVec2(130,38)))
    {
        restoreBillStock(currentBill);
        currentBill.clear();
        total=0;
        msg="Bill cancelled";
    }

    ImGui::SameLine();
    ImGui::Text("Total: Rs %.2f",total);

    if(ImGui::BeginTable("currentBillTable",5,ImGuiTableFlags_Borders|ImGuiTableFlags_RowBg,ImVec2(0,95)))
    {
        ImGui::TableSetupColumn("Med Id");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Qty");
        ImGui::TableSetupColumn("Price");
        ImGui::TableSetupColumn("Amount");
        ImGui::TableHeadersRow();

        for(int i=0;i<currentBill.size();i++)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d",currentBill[i].medId);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s",currentBill[i].name.c_str());
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%d",currentBill[i].qty);
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%.2f",currentBill[i].price);
            ImGui::TableSetColumnIndex(4);
            ImGui::Text("%.2f",currentBill[i].amount);
        }

        ImGui::EndTable();
    }

    ImGui::EndChild();

    if(ImGui::BeginTable("billingMedicineTable",6,ImGuiTableFlags_Borders|ImGuiTableFlags_RowBg|ImGuiTableFlags_Resizable|ImGuiTableFlags_ScrollY,ImVec2(0,0)))
    {
        ImGui::TableSetupColumn("Id");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Available Qty");
        ImGui::TableSetupColumn("Expiry");
        ImGui::TableSetupColumn("Price");
        ImGui::TableSetupColumn("Status");
        ImGui::TableHeadersRow();

        for(int i=0;i<medicineList.size();i++)
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d",medicineList[i].id);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s",medicineList[i].name.c_str());
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%d",medicineList[i].qty);
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%s",medicineList[i].expiry.c_str());
            ImGui::TableSetColumnIndex(4);
            ImGui::Text("Rs %.2f",medicineList[i].price);
            ImGui::TableSetColumnIndex(5);

            if(medicineList[i].qty<5)
            {
                ImGui::Text("Low Stock");
            }
            else
            {
                ImGui::Text("Available");
            }
        }

        ImGui::EndTable();
    }

    ImGui::End();
}

void showReports()
{
    beginMainWindow("Reports");
    bigTitle("Reports");

    if(ImGui::BeginTabBar("reportTabs"))
    {
        if(ImGui::BeginTabItem("Low Stock / Expiry"))
        {
            if(ImGui::BeginTable("lowStockTable",5,ImGuiTableFlags_Borders|ImGuiTableFlags_RowBg|ImGuiTableFlags_Resizable|ImGuiTableFlags_ScrollY,ImVec2(0,560)))
            {
                ImGui::TableSetupColumn("Medicine Id");
                ImGui::TableSetupColumn("Name");
                ImGui::TableSetupColumn("Quantity");
                ImGui::TableSetupColumn("Expiry");
                ImGui::TableSetupColumn("Alert");
                ImGui::TableHeadersRow();

                for(int i=0;i<medicineList.size();i++)
                {
                    if(medicineList[i].qty<5||isNearExpiry(medicineList[i].expiry))
                    {
                        ImGui::TableNextRow();
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("%d",medicineList[i].id);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%s",medicineList[i].name.c_str());
                        ImGui::TableSetColumnIndex(2);
                        ImGui::Text("%d",medicineList[i].qty);
                        ImGui::TableSetColumnIndex(3);
                        ImGui::Text("%s",medicineList[i].expiry.c_str());
                        ImGui::TableSetColumnIndex(4);

                        if(medicineList[i].qty<5&&isNearExpiry(medicineList[i].expiry))
                        {
                            ImGui::Text("Low Stock + Expiry");
                        }
                        else if(medicineList[i].qty<5)
                        {
                            ImGui::Text("Low Stock");
                        }
                        else
                        {
                            ImGui::Text("Expiry Alert");
                        }
                    }
                }

                ImGui::EndTable();
            }

            ImGui::EndTabItem();
        }

        if(ImGui::BeginTabItem("Billing"))
        {
            if(ImGui::BeginTable("billingReportTable",5,ImGuiTableFlags_Borders|ImGuiTableFlags_RowBg|ImGuiTableFlags_Resizable|ImGuiTableFlags_ScrollY,ImVec2(0,560)))
            {
                ImGui::TableSetupColumn("Bill Id");
                ImGui::TableSetupColumn("Medicine");
                ImGui::TableSetupColumn("Qty");
                ImGui::TableSetupColumn("Price");
                ImGui::TableSetupColumn("Bill Total");
                ImGui::TableHeadersRow();

                ifstream fin(billFile);
                string line;
                int currentBillId=0;
                string currentTotal="";

                while(getline(fin,line))
                {
                    line=trim(line);

                    if(line.size()==0||line=="--------------")
                    {
                        continue;
                    }

                    if(line.rfind("Bill ID:",0)==0)
                    {
                        string w1,w2,w3;
                        stringstream ss(line);
                        ss>>w1>>w2>>currentBillId>>w3>>currentTotal;
                    }
                    else
                    {
                        vector<string> v=splitPipe(line);

                        if(v.size()>=3)
                        {
                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                            ImGui::Text("%d",currentBillId);
                            ImGui::TableSetColumnIndex(1);
                            ImGui::Text("%s",trim(v[0]).c_str());
                            ImGui::TableSetColumnIndex(2);
                            ImGui::Text("%s",trim(v[1]).c_str());
                            ImGui::TableSetColumnIndex(3);
                            ImGui::Text("%s",trim(v[2]).c_str());
                            ImGui::TableSetColumnIndex(4);
                            ImGui::Text("%s",currentTotal.c_str());
                        }
                    }
                }

                fin.close();
                ImGui::EndTable();
            }

            ImGui::EndTabItem();
        }

        if(ImGui::BeginTabItem("Appointments"))
        {
            if(ImGui::BeginTable("appointmentReportTable",4,ImGuiTableFlags_Borders|ImGuiTableFlags_RowBg|ImGuiTableFlags_Resizable|ImGuiTableFlags_ScrollY,ImVec2(0,560)))
            {
                ImGui::TableSetupColumn("Patient Id");
                ImGui::TableSetupColumn("Doctor Id");
                ImGui::TableSetupColumn("Therapy");
                ImGui::TableSetupColumn("Time");
                ImGui::TableHeadersRow();

                for(int i=0;i<visitList.size();i++)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%d",visitList[i].patientId);
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d",visitList[i].doctorId);
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%s",visitList[i].therapy.c_str());
                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("%s",visitList[i].time.c_str());
                }

                ImGui::EndTable();
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void showPage()
{
    if(page==0)
    {
        showDashboard();
    }
    else if(page==1)
    {
        showPatients();
    }
    else if(page==2)
    {
        showDoctors();
    }
    else if(page==3)
    {
        showMedicine();
    }
    else if(page==4)
    {
        showVisits();
    }
    else if(page==5)
    {
        showBilling();
    }
    else if(page==6)
    {
        showReports();
    }
}

int main()
{
    loadAll();

    if(!glfwInit())
    {
        return 0;
    }

    const char* glsl="#version 130";
    GLFWwindow* window=glfwCreateWindow(1360,760,"AyurCare GUI",NULL,NULL);

    if(window==NULL)
    {
        glfwTerminate();
        return 0;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io=ImGui::GetIO();
    (void)io;
    io.FontGlobalScale=1.0f;

    ImFont* font=io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/segoeui.ttf",18.0f);

    if(font==NULL)
    {
        io.Fonts->AddFontDefault();
    }

    setTheme();

    ImGui_ImplGlfw_InitForOpenGL(window,true);
    ImGui_ImplOpenGL3_Init(glsl);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        sideMenu();
        showPage();

        ImGui::Render();

        int w,h;
        glfwGetFramebufferSize(window,&w,&h);
        glViewport(0,0,w,h);
        glClearColor(0.88f,0.91f,0.92f,1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
