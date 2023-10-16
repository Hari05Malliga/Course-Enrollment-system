#include "structures.h"
#include "client.h"

void viewAvailableCourse(int sd)
{
    DEBUG("Entering viewAvailableCourse().\n")
    int select = 1;
    send (sd, &select, sizeof(int), 0);

    bool result;
    recv (sd, &result, sizeof(bool), 0);

    if (result) {
        char course_available[1000];
        memset (course_available, '\0', sizeof(course_available));

        recv (sd, &course_available, sizeof(course_available), 0);

        write (1, &course_available, sizeof(course_available));
    }
    else {
        printf ("No course are offered yet. Please check after sometime.\n\n");
    }

    showMenu(sd);
    DEBUG("Leaving viewAvailableCourse().\n")
}

void enrollCourse(int sd) 
{
    DEBUG("Entering enrollCourse().\n")
    int select = 2;
    send (sd, &select, sizeof(int), 0);

    struct enroll record;
    memset (&record, '\0', sizeof(struct enroll));

    printf ("Course ID : ");
    scanf ("%s", record.courseId);

    send (sd, &record, sizeof(struct enroll), 0);

    bool result;
    recv (sd, &result, sizeof(bool), 0);

    if (result) {
        printf ("You have successfully enrolled in course\n\n");
    }
    else {
        printf ("You were unable to Enroll in this course\n\n");
    }

    showMenu(sd);
    DEBUG("Leaving enrollCourse().\n")
}

void dropCourse(int sd)
{
    DEBUG("Entering dropCourse().\n")
    int select = 3;
    send (sd, &select, sizeof(int), 0);

    struct course record;
    bool result;

    printf ("Course ID : ");
    scanf ("%s", record.courseId);

    send (sd, &record, sizeof(struct course), 0);
    recv (sd, &result, sizeof(bool), 0);

    if (result) {
        printf ("Course Successfully Droped.\n\n");
    }
    else {
        printf ("Error: Either not enrolled in course or other issue.\n\n");
    }

    showMenu(sd);
    DEBUG("Leaving dropCourse().\n")
}

void viewEnrolledCourses(int sd)
{
    DEBUG("Entering viewEnrolledCourses().\n")
    int select = 4;
    send (sd, &select, sizeof(int), 0);

    struct student record;
    memset (&record, '\0', sizeof(struct student));

    recv (sd, &record, sizeof(struct student), 0);

    if (record.enrollCount > 0) {
        printf ("Enrolled Courses : \n");
        if (strlen(record.course1) > 0) printf ("\t%s\n", record.course1);
        if (strlen(record.course2) > 0) printf ("\t%s\n", record.course2);
        if (strlen(record.course3) > 0) printf ("\t%s\n", record.course3);
        if (strlen(record.course4) > 0) printf ("\t%s\n", record.course4);
        if (strlen(record.course5) > 0) printf ("\t%s\n", record.course5);
        if (strlen(record.course6) > 0) printf ("\t%s\n", record.course6);
        printf ("\n");
    }
    else {
        printf ("You are currently not enrolled in any courses\n\n");
    }

    showMenu(sd);
    DEBUG("Leaving viewEnrolledCourses().\n")
}

void studentChangePasswd(int sd)
{
    DEBUG("Entering studentChangePasswd().\n")
    int select = 5;
    send (sd, &select, sizeof(int), 0);

    struct student record;
    memset (&record, '\0', sizeof(struct student));

    printf ("New Password : ");
    char *pass = getpass("");
    strcpy (record.passwd, pass);

    sprintf (buffer_message,"record.passwd : %s\n", record.passwd);
    DEBUG(buffer_message)

    send (sd, &record, sizeof(struct student), 0);

    bool result;
    recv (sd, &result, sizeof(bool), 0);

    if (result)
        printf ("Succefully updated Password\n\n");
    else    
        printf ("Password Change attempt failed\n\n");

    showMenu (sd);
    DEBUG("Leaving studentChangePasswd().\n")
}
