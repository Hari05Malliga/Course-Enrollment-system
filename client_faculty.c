#include "structures.h"
#include "client.h"

void viewOfferedCourse(int sd)
{
    DEBUG("Entering viewOfferedCourse().\n")
    int select = 1;
    send (sd, &select, sizeof(int), 0);

    struct faculty record;
    memset (&record, '\0', sizeof(struct faculty));

    recv (sd, &record, sizeof(struct faculty), 0);

    if (record.courseCount > 0) {
        printf ("Courses offered are : \n");
        if (strlen(record.course1) > 0)  printf ("\t%s - %s\n", record.course1, record.course1_Name);
        if (strlen(record.course2) > 0)  printf ("\t%s - %s\n", record.course2, record.course2_Name);
        if (strlen(record.course3) > 0)  printf ("\t%s - %s\n", record.course3, record.course3_Name);
        if (strlen(record.course4) > 0)  printf ("\t%s - %s\n", record.course4, record.course4_Name);
        if (strlen(record.course5) > 0)  printf ("\t%s - %s\n", record.course5, record.course5_Name);

        printf ("\n");
    }
    else {
        printf ("Your not offering any course yet\n\n");
    }

    showMenu (sd);
    DEBUG("Leaving viewOfferedCourse().\n")
}

void addCourse(int sd)
{
    DEBUG("Entering addCourse().\n")
    int select = 2;
    send (sd, &select, sizeof(int), 0);

    bool result;
    recv (sd, &result, sizeof(bool), 0);

    if (result) {
        struct course record;
        memset (&record, '\0', sizeof(struct course));

        printf ("Course Name : ");
        getchar();
        scanf ("%[^\n]s", record.courseName);

        printf ("Max Number of Enrollment possible : ");
        getchar();
        scanf ("%d", &record.totalSeat);

        record.avilSeat = record.totalSeat;
        record.status = 1;

        send (sd, &record, sizeof(struct course), 0);

        bool course_result, faculty_result;
        recv (sd, &course_result, sizeof(bool), 0);
        recv (sd, &faculty_result, sizeof(bool), 0);

        if (course_result && faculty_result) {
            printf ("Course Succefully added\n");
            recv (sd, &record.courseId, sizeof(record.courseId), 0);
            printf ("Generated Course ID : %s\n\n", record.courseId);
        }
        else {
            printf ("Attempt to add Course failed\n\n");
        }
    }
    else {
        printf ("You are already offering Maximum possible courses.. unable to add new Course..\n\n");
    }

    showMenu (sd);
    DEBUG("Leaving addCourse().\n")
}



void facultyChangePasswd(int sd)
{
    DEBUG("Entering facultyChangePasswd().\n")
    int select = 5;
    send (sd, &select, sizeof(int), 0);

    struct faculty record;
    memset (&record, '\0', sizeof(struct faculty));

    printf ("New Password : ");
    char *pass = getpass("");
    strcpy (record.passwd, pass);

    send (sd, &record, sizeof(struct faculty), 0);

    bool result;
    recv (sd, &result, sizeof(bool), 0);

    if (result)
        printf ("Succefully updated Password\n\n");
    else    
        printf ("Password Change attempt failed\n\n");

    showMenu (sd);
    DEBUG("Leaving facultyChangePasswd().\n")
}
