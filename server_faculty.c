#include "structures.h"
#include "server.h"

bool checkFaculty (struct faculty record, int sd)
{
    DEBUG("Entering checkFaculty().\n")
    int fd = open ("/home/hari/Desktop/Project/DB/faculty_db", O_RDWR, 0644);
    bool result = false;

    sscanf (record.loginId, "FT%d", &record.userId);
    int i = record.userId-1;

    struct flock lock;
    lock.l_type   = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = (i) * sizeof(struct faculty);
    lock.l_len    = sizeof(struct faculty);
    lock.l_pid    = getpid();

    fcntl (fd, F_SETLKW, &lock);

    struct faculty currRec;
    lseek (fd, (i)*sizeof(struct faculty), SEEK_SET);
    read (fd, &currRec, sizeof(struct faculty));

    if (strcmp(record.loginId, currRec.loginId)==0 && strcmp(record.passwd, currRec.passwd)==0) {
        result = true;
        userId = i;
    }
    else    
        result = false;

    lock.l_type = F_UNLCK;
	fcntl (fd, F_SETLK, &lock);

	close (fd);
    sprintf(buffer_message,"Leaving checkFaculty() - returning value: %d\n",
             result);
    DEBUG(buffer_message)
	return result;
}

bool checkFacultyEligibility ()
{
    DEBUG("Entering checkFacultyEligibility().\n")
    int fd = open ("/home/hari/Desktop/Project/DB/faculty_db", O_RDWR, 0644);
    bool result;

    struct flock lock;
    lock.l_type   = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = (userId) * sizeof(struct faculty);
    lock.l_len    = sizeof(struct faculty);
    lock.l_pid    = getpid();

    fcntl (fd, F_SETLKW, &lock);

    struct faculty currRec;
    lseek (fd, (userId)*sizeof(struct faculty), SEEK_SET);
    read (fd, &currRec, sizeof(struct faculty));

    if ( currRec.courseCount >=0 && currRec.courseCount < 5 )   result = true;
    else    result = false;

    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK,&lock);

    close(fd);
    sprintf(buffer_message,"Leaving checkFacultyEligibility() - returning value: %d\n",
             result);
    DEBUG(buffer_message)
    return result;
}

void addCourse (struct course record, int sd)
{
    DEBUG("Entering addCourse().\n")
    int flag = 1, fd = open ("/home/hari/Desktop/Project/DB/course_db", O_RDWR, 0644);
    if ( -1 == fd ) {
        fd = open ("/home/hari/Desktop/Project/DB/course_db", O_RDWR | O_CREAT | O_APPEND, 0644);
        record.id = 0;
        flag = 0;
    }

    bool course_result, faculty_result;
    struct course lastRec;

    struct flock lock;
    lock.l_type   = F_WRLCK;
    lock.l_whence = SEEK_END;
    lock.l_start  = (-1) * sizeof(struct course);
    lock.l_len    = sizeof(struct course);
    lock.l_pid    = getpid();

    fcntl(fd, F_SETLKW, &lock);

    lseek (fd, (-1)*sizeof(struct course), SEEK_END);
    read (fd, &lastRec, sizeof(struct course));

    if (flag)
        record.id = lastRec.id + 1;

    sprintf (record.courseId, "CS%d", record.id + 1);
    sprintf (record.facultyId, "FT%d", userId + 1 );

    int size = write (fd, &record, sizeof(struct course));
    if (size != 0) course_result = true;
    else course_result = false;

    send (sd, &course_result, sizeof(bool), 0);

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);

    fd = open ("/home/hari/Desktop/Project/DB/faculty_db", O_RDWR, 0644);

    lock.l_type   = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = (userId) * sizeof(struct faculty);
    lock.l_len    = sizeof(struct faculty);
    lock.l_pid    = getpid();

    fcntl (fd, F_SETLKW, &lock);

    struct faculty currRec;
    lseek (fd, (userId)*sizeof(struct faculty), SEEK_SET);
    read (fd, &currRec, sizeof(struct faculty));

    if (strlen(currRec.course1) == 0)  {strcpy(currRec.course1, record.courseId); strcpy(currRec.course1_Name, record.courseName);}
    else if (strlen(currRec.course2) == 0) {strcpy(currRec.course2,record.courseId); strcpy(currRec.course2_Name, record.courseName);}
    else if (strlen(currRec.course3) == 0) {strcpy(currRec.course3,record.courseId); strcpy(currRec.course3_Name, record.courseName);}
    else if (strlen(currRec.course4) == 0) {strcpy(currRec.course4,record.courseId); strcpy(currRec.course4_Name, record.courseName);}
    else if (strlen(currRec.course5) == 0) {strcpy(currRec.course5,record.courseId); strcpy(currRec.course5_Name, record.courseName);}

    currRec.courseCount += 1;

    lseek (fd, (-1)*sizeof(struct faculty), SEEK_CUR);
    int j = write (fd, &currRec, sizeof(struct faculty));
    if ( j != 0)  faculty_result = true;
    else  faculty_result = false;

    send (sd, &faculty_result, sizeof(bool), 0);

    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK,&lock);

    close(fd);

    if (course_result && faculty_result) {
        char temp[MAX_LOGINID_LEN];
        memset (temp, '\0', MAX_LOGINID_LEN);

        strcpy (temp, record.courseId);
        send (sd, &temp, MAX_LOGINID_LEN, 0);
    }
    DEBUG("Leaving addCourse().\n")
}

void viewOfferedCourse(int sd)
{
    DEBUG("Entering viewOfferedCourse().\n")
    int fd = open ("/home/hari/Desktop/Project/DB/faculty_db", O_RDONLY, 0644);
    bool result;

    struct flock lock;
    lock.l_type   = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = (userId) * sizeof(struct faculty);
    lock.l_len    = sizeof(struct faculty);
    lock.l_pid    = getpid();

    fcntl (fd, F_SETLKW, &lock);

    struct faculty currRec;
    lseek (fd, (userId)*sizeof(struct faculty), SEEK_SET);
    read (fd, &currRec, sizeof(struct faculty));

    struct faculty record;

    record.courseCount = currRec.courseCount;
    strcpy (record.course1, currRec.course1); strcpy (record.course1_Name, currRec.course1_Name);
    strcpy (record.course2, currRec.course2); strcpy (record.course2_Name, currRec.course2_Name);
    strcpy (record.course3, currRec.course3); strcpy (record.course3_Name, currRec.course3_Name);
    strcpy (record.course4, currRec.course4); strcpy (record.course4_Name, currRec.course4_Name);
    strcpy (record.course5, currRec.course5); strcpy (record.course5_Name, currRec.course5_Name);

    send (sd, &record, sizeof(struct faculty), 0);

    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK,&lock);
    close(fd);
    DEBUG("Leaving viewOfferedCourse().\n")
}

void removeEnrollment (struct enroll record) 
{
    DEBUG ("Entering removeEnrollment().\n")

    int student_ID;
    sscanf (record.studentId, "MT%d", &student_ID); student_ID--;

    int course_ID;
    sscanf (record.courseId, "CS%d", &course_ID); course_ID--;

    int fd_course = open ("/home/hari/Desktop/Project/DB/course_db", O_RDWR, 0644);
    struct course record_course;
    struct flock lock;

    lock.l_type   = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = (course_ID) * sizeof(struct course);
    lock.l_len    = sizeof(struct course);
    lock.l_pid    = getpid();

    fcntl(fd_course, F_SETLKW, &lock);
    lseek (fd_course, (course_ID)*sizeof(struct course), SEEK_SET);
    read (fd_course, &record_course, sizeof(struct course));

    record_course.avilSeat--;
    lseek (fd_course, (-1)*sizeof(struct course), SEEK_CUR);
    write (fd_course, &record_course, sizeof(struct course));

    lock.l_type=F_UNLCK;
    fcntl(fd_course,F_SETLK,&lock);
    close(fd_course);

    int fd_student = open ("/home/hari/Desktop/Project/DB/student_db", O_RDWR, 0644);
    struct student record_student;
    
    lock.l_type   = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = (student_ID) * sizeof(struct student);
    lock.l_len    = sizeof(struct student);
    lock.l_pid    = getpid();

    int val = fcntl (fd_student, F_SETLKW, &lock);
    sprintf (buffer_message, "fcntl return value : %d\n", val);
    DEBUG (buffer_message)

    lseek (fd_student, (student_ID)*sizeof(struct student), SEEK_SET);
    read (fd_student, &record_student, sizeof(struct student));

    char temp1[256];    strcpy (temp1, record_student.course1);
    char temp2[256];    strcpy (temp2, record_student.course2);
    char temp3[256];    strcpy (temp3, record_student.course3);
    char temp4[256];    strcpy (temp4, record_student.course4);
    char temp5[256];    strcpy (temp5, record_student.course5);
    char temp6[256];    strcpy (temp6, record_student.course6);

    sprintf (buffer_message, "record_student.course1 : %s\n", record_course.courseId);
    DEBUG (buffer_message)


    if ( strcmp (strtok(temp1, " "), record_course.courseId)==0) {
        DEBUG ("Inside the if condition: 1")
        memset (record_student.course1, '\0', MAX_COURSEDETAIL_LEN);
    }


    else if ( strcmp (strtok(temp2, " "), record_course.courseId)==0) {
        DEBUG ("Inside the if condition: 2")
        memset (record_student.course2, '\0', MAX_COURSEDETAIL_LEN);
    }


    else if ( strcmp (strtok(temp3, " "), record_course.courseId)==0) {
        DEBUG ("Inside the if condition: 3")
        memset (record_student.course3, '\0', MAX_COURSEDETAIL_LEN);
    }


    else if ( strcmp (strtok(temp4, " "), record_course.courseId)==0) {
        DEBUG ("Inside the if condition: 4")
        memset (record_student.course4, '\0', MAX_COURSEDETAIL_LEN);
    }


    else if ( strcmp (strtok(temp5, " "), record_course.courseId)==0) {
        DEBUG ("Inside the if condition: 5")
        memset (record_student.course5, '\0', MAX_COURSEDETAIL_LEN);
    }


    else if ( strcmp (strtok(temp6, " "), record_course.courseId)==0) {
        DEBUG ("Inside the if condition: 6")
        memset (record_student.course6, '\0', MAX_COURSEDETAIL_LEN);
    }

    record_student.enrollCount--;

    DEBUG ("Before write...\n")

    lseek (fd_student, (-1)*sizeof(struct student), SEEK_CUR);
    int size = write (fd_student, &record_student, sizeof(struct student));

    sprintf (buffer_message, "Written size : %d\n", size);
    DEBUG (buffer_message)

    lock.l_type=F_UNLCK;
    fcntl(fd_course,F_SETLK,&lock);
    close(fd_course);

    DEBUG ("Leaving removeEnrollment().\n")
}

bool removeCourse (struct course record)
{
    DEBUG ("Enterning removeCourse().\n")
    char temp[256];
    strcpy(temp, "/home/hari/Desktop/Project/DB/course/");
    strcat (temp, record.courseId);

    int fd = open (temp, O_RDWR , 0644);
    if (fd == -1) {
        return false;
    }

    struct enroll currRec;
    struct flock lock;

    lock.l_type   = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = 0;
    lock.l_len    = 0;
    lock.l_pid    = getpid();

    fcntl (fd, F_SETLKW, &lock);
    
    lseek (fd, 0, SEEK_SET);
    while (read(fd, &currRec, sizeof(struct enroll))>0 && currRec.status==1) {
        sprintf (buffer_message, "currRec.studentId : %s\n", currRec.studentId);
        DEBUG (buffer_message)
        removeEnrollment(currRec);
    }

    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK,&lock);
    close(fd);

    fd = open ("/home/hari/Desktop/Project/DB/course_db", O_RDWR, 0644);

    int i;
    sscanf (record.courseId, "CS%d", &i); i--;

    lock.l_type   = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = (i) * sizeof(struct course);
    lock.l_len    = sizeof(struct course);
    lock.l_pid    = getpid();

    fcntl(fd, F_SETLKW, &lock);

    lseek (fd, (i)*sizeof(struct course), SEEK_SET);
    read (fd, &record, sizeof(struct course));

    if (record.status == 0) {
        lock.l_type=F_UNLCK;
        fcntl(fd,F_SETLK,&lock);
        close(fd);
        return false;
    }

    record.status = 0;
    lseek (fd, (-1)*sizeof(struct course), SEEK_CUR);
    int size = write (fd, &record, sizeof(struct course));
    bool result_course;
    if (size) result_course = true;
    else result_course = false;

    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK,&lock);
    close(fd);

    fd = open ("/home/hari/Desktop/Project/DB/faculty_db", O_RDWR, 0644);

    struct faculty record_faculty;

    lock.l_type   = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = (userId) * sizeof(struct faculty);
    lock.l_len    = sizeof(struct faculty);
    lock.l_pid    = getpid();

    fcntl (fd, F_SETLKW, &lock);

    lseek (fd, (userId)*sizeof(struct faculty), SEEK_SET);
    read (fd, &record_faculty, sizeof(struct faculty));

    if (strcmp(record_faculty.course1, record.courseId)==0) { strcpy (record_faculty.course1,""); strcpy (record_faculty.course1_Name,""); }
    else if (strcmp(record_faculty.course2, record.courseId)==0) { strcpy (record_faculty.course2,""); strcpy (record_faculty.course2_Name,""); }
    else if (strcmp(record_faculty.course3, record.courseId)==0) { strcpy (record_faculty.course3,""); strcpy (record_faculty.course3_Name,""); }
    else if (strcmp(record_faculty.course4, record.courseId)==0) { strcpy (record_faculty.course4,""); strcpy (record_faculty.course4_Name,""); }
    else if (strcmp(record_faculty.course5, record.courseId)==0) { strcpy (record_faculty.course5,""); strcpy (record_faculty.course5_Name,""); }

    record_faculty.courseCount--;

    lseek (fd, (-1)*sizeof(struct faculty), SEEK_CUR);
    write (fd, &record_faculty, sizeof(struct faculty));

    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK,&lock);
    close(fd);

    DEBUG ("Leaving removeCourse().\n")
    return true;
}

bool facultyChangePasswd (struct faculty record)
{
    DEBUG("Entering facultyChangePasswd().\n")
    int fd = open ("/home/hari/Desktop/Project/DB/faculty_db", O_RDWR, 0644);
    bool result;

    struct flock lock;
    lock.l_type   = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = (userId) * sizeof(struct faculty);
    lock.l_len    = sizeof(struct faculty);
    lock.l_pid    = getpid();

    fcntl (fd, F_SETLKW, &lock);

    struct faculty currRec;
    lseek (fd, (userId)*sizeof(struct faculty), SEEK_SET);
    read (fd, &currRec, sizeof(struct faculty));

    strcpy (currRec.passwd, record.passwd);

    lseek (fd, (-1)*sizeof(struct faculty), SEEK_CUR);
    int size = write (fd, &currRec, sizeof(struct faculty));
    if ( size != 0)  result = true;
    else  result = false;

    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK,&lock);

    close(fd);
    sprintf(buffer_message,"Leaving facultyChangePasswd() - returning value: %d\n",
             result);
    DEBUG(buffer_message)
    return result;
}
