#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_USERNAME_LEN 30
#define MAX_COURSENAME_LEN 20
#define MAX_COURSEDETAIL_LEN 50
#define MAX_PASSWORD_LEN 15
#define MAX_COURSEID_LEN 6
#define MAX_LOGINID_LEN 6
#define MAX_EMAIL_LEN 30
#define MAX_DEPT_LEN  5
#define MAX_DESIGNATION_LEN 15
#define DEFAULT_PASSWD "abc123"

struct admin {
    char userName[MAX_USERNAME_LEN];
    char passwd[MAX_PASSWORD_LEN];
};

struct student {
    int  userId;

    char userName[MAX_USERNAME_LEN];
    char email[MAX_EMAIL_LEN];
    int age;

    char loginId[MAX_LOGINID_LEN];
    char passwd[MAX_PASSWORD_LEN];

    int status;

    char course1[MAX_COURSEDETAIL_LEN];
    char course2[MAX_COURSEDETAIL_LEN];
    char course3[MAX_COURSEDETAIL_LEN]; 
    char course4[MAX_COURSEDETAIL_LEN]; 
    char course5[MAX_COURSEDETAIL_LEN]; 
    char course6[MAX_COURSEDETAIL_LEN];
    int enrollCount;

    char drop1[MAX_COURSEID_LEN];
    char drop2[MAX_COURSEID_LEN];
    int dropCount;
};

struct faculty {
    int userId;


    char userName[MAX_USERNAME_LEN];
    char designation[MAX_DESIGNATION_LEN];
    char department[MAX_DEPT_LEN];
    char email[MAX_EMAIL_LEN];

    char loginId[MAX_LOGINID_LEN];
    char passwd[MAX_PASSWORD_LEN];

    char course1[MAX_COURSEID_LEN]; char course1_Name[MAX_COURSENAME_LEN];
    char course2[MAX_COURSEID_LEN]; char course2_Name[MAX_COURSENAME_LEN];
    char course3[MAX_COURSEID_LEN]; char course3_Name[MAX_COURSENAME_LEN];
    char course4[MAX_COURSEID_LEN]; char course4_Name[MAX_COURSENAME_LEN];
    char course5[MAX_COURSEID_LEN]; char course5_Name[MAX_COURSENAME_LEN];
    int courseCount;
};

struct course {
    char courseName[MAX_COURSENAME_LEN];
    int totalSeat;

    int id;
    int avilSeat;
    char courseId[MAX_COURSEID_LEN];
    char facultyId[MAX_LOGINID_LEN];
};

struct enroll {
    char courseId[MAX_COURSEID_LEN];
    char courseName[MAX_COURSENAME_LEN];

    char studentId[MAX_LOGINID_LEN];
    int status;
};

extern int debug;  /* To enable to disable debugging */
#define  DEBUG(message) if ( debug ) printf("%s\n", message);

#endif
