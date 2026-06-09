# AyurCare v2

This is the second version of our Ayurvedic Hospital Management System project.

In sem 1 we made this project in C. It was mostly terminal based and used file handling.  
In sem 2 we improved it using C++ and also added GUI.

The project is for managing basic hospital work like:

- patients
- doctors
- appointments
- medicine inventory
- billing
- reports

The GUI part is made using Dear ImGui, GLFW and OpenGL.  
It has different pages like dashboard, patients, doctors, inventory, appointments, billing and reports.

Data is stored in text files, not database.  
So when we add, update or delete records, it saves the changes in txt files.

Some things included:

- add/update/delete patients
- add/update/delete doctors
- manage medicines
- book appointments
- create bills
- update medicine stock after bill
- low stock report
- basic validations like duplicate id, invalid phone number, low stock etc.

This project was mainly made for our SDF-II project.  
We worked on the GUI part and connected it with the file handling part.

It is not perfect but it helped us learn C++, GUI, file handling and how a normal terminal project can be converted into a desktop app.
