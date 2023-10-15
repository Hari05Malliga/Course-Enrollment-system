#ifndef __CLIENT__
#define __CLIENT__

extern void chooseOption(int );

extern void showMenu(int );

extern void attemptFacultyLogin(int sd);

extern void attemptAdminLogin(int sd);

extern void attemptStudentLogin(int sd);

extern void viewAvailableCourse(int );

extern void enrollCourse(int );

extern void addStudent(int);

extern void addFaculty(int);

extern void updateStudent(int);

extern void updateFaculty(int);

extern void activateStudentAcc(int);

extern void deactivateStudentAcc(int);

extern void displayStudentAcc(int);

extern void displayFacultyAcc(int);

extern void addCourse(int);

extern void viewOfferedCourse (int );

extern void facultyChangePasswd(int);

extern void viewEnrolledCourses(int sd);

extern void studentChangePasswd(int sd);

extern void studentMenu();

extern void facultyMenu();

extern void adminMenu();

extern void enrollCourse(int );

extern void viewEnrolledCourses(int );

extern void determine_debug();

/*
** from client_data.c
*/
extern int choice, userId;
extern int debug;
extern char buffer_message[256];

#endif
