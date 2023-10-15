#ifndef __SERVER__
#define __SERVER__

extern void determine_debug();
extern void serverTask(int sd);

/*
** From server_admin.c
*/
extern bool checkAdmin(struct admin record);
extern bool addStudent(struct student record, int sd);
extern bool addFaculty(struct faculty record, int sd);
extern bool updateStudent(struct student record, int sd);
extern bool updateFaculty (struct faculty record, int sd);
extern void displayStudentAcc(struct student record,int sd);
extern void displayFacultyAcc (struct faculty record, int sd);
extern int activateStudentAcc (struct student record);
extern int deactivateStudentAcc (struct student record);

/*
** From server_student.c
*/
extern bool checkStudent (struct student record, int sd);
extern void viewAvailableCourse (int sd);
extern bool enrollCourse (struct enroll record);
extern void viewEnrolledCourses(int sd);
extern bool studentChangePasswd (struct student record);
extern bool addStudentToCourse(struct enroll record);
extern bool validateStudentLoginId(struct student record);


/*
** From server_faculty.c
*/
extern bool checkFaculty (struct faculty record, int sd);
extern bool checkFacultyEligibility ();
extern void addCourse (struct course record, int sd);
extern void viewOfferedCourse(int sd);
extern bool facultyChangePasswd (struct faculty record);
extern bool validateFacultyLoginId(struct faculty record);

/*
** Global data for server.
*/
extern int userId;
extern int debug;
extern char buffer_message[256];

#endif
