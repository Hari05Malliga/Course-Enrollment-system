#include "structures.h"
#include "client.h"

void addStudent(int sd)
{
    DEBUG("Entering addStudent().\n")
    int select = 1;
    send (sd, &select, sizeof(int), 0);

    struct student record;
    memset (&record, '\0', sizeof(struct student));
    bool result;

    printf ("Student Name : ");
    getchar();
    scanf ("%[^\n]s", record.userName);


    printf ("Age : ");
    getchar();
    scanf ("%d", &record.age);

    printf ("Email : ");
    getchar();
    scanf ("%[^\n]s", record.email);

    record.status = 1;

    strcpy (record.passwd, DEFAULT_PASSWD);

    strcpy (record.course1, "");
    strcpy (record.course2, "");
    strcpy (record.course3, "");
    strcpy (record.course4, "");
    strcpy (record.course5, "");
    strcpy (record.course6, "");

    record.enrollCount = 0;

    strcpy (record.drop1, "");
    strcpy (record.drop2, "");

    record.dropCount = 0;

    send (sd, &record, sizeof(struct student), 0);
    recv (sd, &record.loginId, sizeof(record.loginId), 0);
    recv (sd, &result, sizeof(bool), 0);

    if (result) {
        printf ("Successfully added Student Account\n");
        printf ("Generated Login Id : %s\n\n", record.loginId);
    }
    else 
        printf ("Error adding Student Account\n\n");

    showMenu (sd);
    DEBUG("Leaving addStudent().\n")
}


void displayStudentAcc (int sd)
{
    DEBUG("Entering displayStudentAcc().\n")

    struct student record;
    bool result;
    memset (&record, '\0', sizeof(struct student));

    printf ("Student Login Id : ");
    getchar();
    scanf ("%[^\n]s", record.loginId);

    if (strncmp(record.loginId, "MT", 2)==0) {
        int select = 2;
        send (sd, &select, sizeof(int), 0);

        send (sd, &record, sizeof(struct student), 0);
        recv (sd, &result, sizeof(bool), 0);

        if (result) {
            recv (sd, &record, sizeof(struct student), 0);

            printf ("Name     : %s\n", record.userName);
            printf ("Age      : %d\n", record.age);
            printf ("Email    : %s\n", record.email);
            printf ("Login ID : %s\n", record.loginId);
            printf ("Status   : %d\n", record.status);
            printf ("Course 1 : %s\n", record.course1);
            printf ("Course 2 : %s\n", record.course2);
            printf ("Course 3 : %s\n", record.course3);
            printf ("Course 4 : %s\n", record.course4);
            printf ("Course 5 : %s\n", record.course5);
            printf ("Course 6 : %s\n", record.course6);
            printf ("Eroll Count : %d\n", record.enrollCount);
            printf ("Drop 1 : %s\n", record.drop1);
            printf ("Drop 2 : %s\n", record.drop2);
            printf ("Drop count : %d\n\n", record.dropCount);
        }
        else {
            printf ("Unable to retrieve user details\n\n");
        }
    }
    else {
        printf ("Invalid login ID.\n\n");
    }

    showMenu (sd);
    DEBUG("Leaving displayStudentAcc().\n")
}

void addFaculty(int sd)
{
    DEBUG("Entering addFaculty().\n")
    int select = 3;
    send (sd, &select, sizeof(int), 0);

    struct faculty record;
    memset (&record, '\0', sizeof(struct faculty));
    bool result;

    getchar();
    printf("User Name : ");
    scanf ("%[^\n]s", record.userName);

    getchar();
    printf("Designation : ");
    scanf ("%[^\n]s", record.designation);

    getchar();
    printf ("Department : ");
    scanf ("%[^\n]s", record.department);

    getchar();
    printf ("Email : ");
    scanf ("%[^\n]s", record.email);

    strcpy (record.passwd, DEFAULT_PASSWD);

    strcpy (record.course1, "");
    strcpy (record.course2, "");
    strcpy (record.course3, "");
    strcpy (record.course4, "");
    strcpy (record.course5, "");

    strcpy (record.course1_Name, "");
    strcpy (record.course2_Name, "");
    strcpy (record.course3_Name, "");
    strcpy (record.course4_Name, "");
    strcpy (record.course5_Name, "");

    record.courseCount = 0;

    send (sd, &record, sizeof(struct faculty), 0);
    recv (sd, &record, sizeof(struct faculty), 0);
    recv (sd, &result, sizeof(int), 0);

    if (result) {
        printf ("Successfully added Faculty Account\n");
        printf ("Generated Login Id : %s\n\n", record.loginId);
    }
    else
        printf ("Error adding Faculty Account\n\n");
    
    showMenu (sd);
    DEBUG("Leaving addFaculty().\n")
}

void displayFacultyAcc(int sd)
{
    DEBUG("Entering displayFacultyAcc().\n")


    struct faculty record;
    memset (&record, '\0', sizeof(struct faculty));
    bool result;

    printf ("Faculty Login Id : ");
    getchar();
    scanf ("%[^\n]s", record.loginId);

    if (strncmp(record.loginId, "FT", 2)==0) {
        int select = 4;
        send (sd, &select, sizeof(int), 0);

        send (sd, &record, sizeof(struct faculty), 0);
        recv (sd, &result, sizeof(bool), 0);

        if (result) {
            recv (sd, &record, sizeof(struct faculty), 0);

            printf ("Name          : %s\n", record.userName);
            printf ("Designation   : %s\n", record.designation);
            printf ("Department    : %s\n", record.department);
            printf ("Email         : %s\n", record.email);
            printf ("Login ID      : %s\n", record.loginId);
            printf ("Course 1 ID   : %s\n", record.course1);
            printf ("Course 1 Name : %s\n", record.course1_Name);
            printf ("Course 2 ID   : %s\n", record.course2);
            printf ("Course 2 Name : %s\n", record.course2_Name);
            printf ("Course 3 ID   : %s\n", record.course3);
            printf ("Course 3 Name : %s\n", record.course3_Name);
            printf ("Course 4 ID   : %s\n", record.course4);
            printf ("Course 4 Name : %s\n", record.course4_Name);
            printf ("Course 5 ID   : %s\n", record.course5);
            printf ("Course 5 Name : %s\n", record.course5_Name);
            printf ("Course Count  : %d\n", record.courseCount);
        }
        else {
            printf ("Unable to retrieve user details\n\n");
        }
    }
    else {
        printf ("Invalid login ID.\n\n");
    }

    showMenu (sd);
    DEBUG("Leaving displayFacultyAcc().\n")
}

void updateStudent(int sd)
{
    DEBUG("Entering updateStudent().\n")
    int select = 5;
    send (sd, &select, sizeof(int), 0);

    struct student record;
    memset (&record, '\0', sizeof(struct student));
    bool result;

    printf ("Student Login Id to be updated : ");
    getchar();
    scanf ("%s", record.loginId);

    send (sd, &record, sizeof(struct student), 0);
    recv (sd, &result, sizeof(bool), 0);

    if (result) {
        char flag;
        bool flow = false;

        printf("Want to update User Name [Y/N] : ");
        getchar();
        scanf(" %c", &flag); 
        if (flag == 'Y' || flag == 'y') {
            flow = true;
            printf("Name : ");
            getchar();
            scanf ("%[^\n]s", record.userName);
        }
        else strcpy (record.userName, "");

        printf("Want to update User Age [Y/N] : ");
        getchar();
        scanf(" %c", &flag);
        if (flag == 'Y' || flag == 'y') {
            flow = true;
            printf("Age : ");
            getchar();
            scanf ("%d", &record.age);
        }
        else  record.age = 0;

        printf("Want to update User email [Y/N] : ");
        getchar();
        scanf(" %c", &flag);
        if (flag == 'Y' || flag == 'y') {
            flow = true;
            printf("Email : "); 
            getchar();
            scanf ("%[^\n]s", record.email);
        }
        else  strcpy (record.userName, "");

        if (flow) {
            send (sd, &record, sizeof(struct student), 0);
            recv(sd, &result, sizeof(bool), 0);

            if (result)
                printf ("Succefully updated Student Account Details\n\n");
            else    
                printf ("Error while trying to update Student Account Details\n\n");
        }
        else {
            printf ("No changes to update Student record\n\n");
        }
    }
    else {
        printf ("invalid Student Login ID\n\n");
    }

    showMenu (sd);
    DEBUG("Leaving updateStudent().\n")
}

void updateFaculty(int sd)
{
    DEBUG("Entering updateFaculty().\n")
    int select = 6;
    send (sd, &select, sizeof(int), 0);

    struct faculty record;
    memset (&record, '\0', sizeof(struct faculty));
    bool result, flow = false;

    printf ("Faculty Login Id to be updated : ");
    scanf ("%s", record.loginId);

    send (sd, &record, sizeof(struct faculty), 0);
    recv (sd, &result, sizeof(bool), 0);

    if (result) {
        char flag;

        printf ("Want to update Faculty Name [Y/N] : ");
        getchar();
        scanf ("%c", &flag);
        if (flag == 'Y' || flag == 'y') {
            flow = true;
            printf ("Name : ");
            getchar();
            scanf ("%[^\n]s", record.userName);
        }
        else strcpy (record.userName, "");

        printf ("Want to update Faculty Designation [Y/N] : ");
        getchar();
        scanf ("%c", &flag);
        if (flag == 'Y' || flag == 'y') {
            flow = true;
            printf ("Designation : ");
            getchar();
            scanf ("%[^\n]s", record.designation);
        }
        else strcpy (record.designation, "");

        printf ("Want to update Faculty Department [Y/N] : ");
        getchar();
        scanf ("%c", &flag);
        if (flag == 'Y' || flag == 'y') {
            flow = true;
            printf ("Department : ");
            getchar();
            scanf ("%[^\n]s", record.department);
        }
        else strcpy (record.department, "");

        printf ("Want to update User email [Y/N] : ");
        getchar();
        scanf ("%c", &flag);
        if (flag == 'Y' || flag == 'y') {
            flow = true;
            printf ("Email : ");
            getchar();
            scanf ("%[^\n]s", record.email);
        }
        else strcpy (record.email, "");

        if (flow) {
            send (sd, &record, sizeof(struct faculty), 0);
            recv(sd, &result, sizeof(bool), 0);

            if (result)
                printf ("Succefully updated Faculty Account Details\n\n");
            else    
                printf ("Error while trying to update Faculty Account Details\n\n");
        }
        else {
            printf ("No changes to update Faculty record\n\n");
        }
    }
    else {
        printf ("Invalid Faculty Login ID\n\n");
    }

    showMenu (sd);
    DEBUG("Leaving updateFaculty().\n")
}

void activateStudentAcc (int sd)
{
    DEBUG("Entering activateStudentAcc().\n")
    int select = 7;
    send (sd, &select, sizeof(int), 0);

    struct student record;
    memset (&record, '\0', sizeof(struct student));
    int result;

    printf ("Student login Id to be Activated : ");
    getchar();
    scanf ("%s", record.loginId);

    send (sd, &record, sizeof(struct student), 0);
    recv (sd, &result, sizeof(int), 0);

    if (result == 0) 
        printf ("Student account is already active\n\n");
    else if (result == 1)   
        printf ("Successfully activated the Student Account\n\n");
    else if (result == 2)
        printf ("Error while attempting to Activate Student Record\n\n");

    showMenu (sd);
    DEBUG("Leaving activateStudentAcc().\n")
}

void deactivateStudentAcc (int sd)
{
    DEBUG("Entering deactivateStudentAcc().\n")
    int select = 8;
    send (sd, &select, sizeof(int), 0);

    struct student record;
    memset (&record, '\0', sizeof(struct student));
    int result;

    printf ("Student login Id to be Deactivated : ");
    getchar();
    scanf ("%s", record.loginId);

    send (sd, &record, sizeof(struct student), 0);
    recv (sd, &result, sizeof(int), 0);

    if (result == 0) 
        printf ("Student account is already Deactive\n\n");
    else if (result == 1)   
        printf ("Successfully Deactivated the Student Account\n\n");
    else if (result == 2)
        printf ("Error while attempting to Deactivate Student Record\n\n");

    showMenu (sd);
    DEBUG("Leaving deactivateStudentAcc().\n")
}
