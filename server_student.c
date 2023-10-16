#include "structures.h"
#include "server.h"

bool checkStudent (struct student record, int sd)
{
    DEBUG("Entering checkStudent().\n")
    int fd = open ("/home/hari/Desktop/Project/DB/student_db", O_RDWR, 0644);
    bool result = false;

    sscanf (record.loginId, "MT%d", &record.userId);
    int i = record.userId-1;

    struct flock lock;
    lock.l_type   = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = (i) * sizeof(struct student);
    lock.l_len    = sizeof(struct student);
    lock.l_pid    = getpid();

    fcntl (fd, F_SETLKW, &lock);

    struct student currRec;
    lseek (fd, (i)*sizeof(struct student), SEEK_SET);
    read (fd, &currRec, sizeof(struct student));

    if (strcmp(record.loginId, currRec.loginId)==0 && strcmp(record.passwd, currRec.passwd)==0 && currRec.status==1) {
        result = true;
        userId = i;
    }
    else    
        result = false;

    lock.l_type = F_UNLCK;
	fcntl (fd, F_SETLK, &lock);

	close (fd);
    sprintf(buffer_message,"Leaving checkStudent() - returning value: %d\n",
             result);
    DEBUG(buffer_message)
	return result;
}

void viewAvailableCourse (int sd)
{
    DEBUG("Entering viewAvailableCourse().\n")
    bool result;
    int fd = open ("/home/hari/Desktop/Project/DB/course_db", O_RDONLY, 0644);
    if (-1 == fd) {
        result = false;
    }
    else  {
        result = true;
    }  

    if (result) {
        send (sd, &result, sizeof(bool), 0);

        bool course_result, faculty_result;
        struct course currRec;

        struct flock lock;
        lock.l_type   = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start  = 0;
        lock.l_len    = 0;
        lock.l_pid    = getpid();

        fcntl(fd, F_SETLKW, &lock);
        char availCourse[1000], currCourse[100];
        memset (availCourse, '\0', sizeof(availCourse));

        while (read (fd, &currRec, sizeof(struct course)) > 0) {
            if ( currRec.status == 1) {
                memset(currCourse, '\0', sizeof(currCourse));
                sprintf (currCourse, "\t%s - %s - Available Seat : %d\n", currRec.courseId, currRec.courseName, currRec.avilSeat);
                strcat (availCourse, currCourse);
            }
        }

        send (sd, &availCourse, sizeof(availCourse), 0);

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
    }
    else {
        send (sd, &result, sizeof(bool), 0);
    }
    DEBUG("Leaving viewAvailableCourse().\n")
}


bool enrollCourse (struct enroll record)
{
    DEBUG("Entering enrollCourse().\n")
    int fd = open ("/home/hari/Desktop/Project/DB/course_db", O_RDWR, 0644);
    bool result;

    int i;
    sscanf (record.courseId, "CS%d", &i); i--;
    char temp[MAX_LOGINID_LEN];
    sprintf (record.studentId, "MT%d", userId + 1);
    record.status = 1;

    struct flock lock;
    lock.l_type   = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = (i) * sizeof(struct course);
    lock.l_len    = sizeof(struct course);
    lock.l_pid    = getpid();

    fcntl (fd, F_SETLKW, &lock);

    struct course currRec;
    lseek (fd, (i)*sizeof(struct course), SEEK_SET);
    read (fd, &currRec, sizeof(struct course));

    if (currRec.status == 1) {
        strcpy (record.courseName, currRec.courseName);
        if (currRec.avilSeat > 0) {
            if (addStudentToCourse(record)) {
                currRec.avilSeat--;
            }
        }

        lseek (fd, (-1)*sizeof(struct course), SEEK_CUR);
        int size = write (fd, &currRec, sizeof(struct course));
        if ( size != 0)  result = true;
        else  result = false;
    }
    else {
        result = false;
    }

    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK,&lock);

    close(fd);
    sprintf(buffer_message,"Leaving enrollCourse() - returning value: %d\n",
             result);
    DEBUG(buffer_message)
    return result;
}

bool dropCourse (struct course record)
{
    DEBUG("Entering dropCourse().\n")
    int fd_student = open ("/home/hari/Desktop/Project/DB/student_db", O_RDWR, 0644);
    struct student record_student;

    struct flock lock_student;
    lock_student.l_type   = F_WRLCK;
    lock_student.l_whence = SEEK_SET;
    lock_student.l_start  = (userId) * sizeof(struct student);
    lock_student.l_len    = sizeof(struct student);
    lock_student.l_pid    = getpid();

    fcntl (fd_student, F_SETLKW, &lock_student);

    lseek (fd_student, (userId)*sizeof(struct student), SEEK_SET);
    read (fd_student, &record_student, sizeof(struct student));

    int fd_course  = open ("/home/hari/Desktop/Project/DB/course_db", O_RDWR, 0644);
    struct course record_course;

    int i;
    sscanf (record.courseId, "CS%d", &i); i--;

    struct flock lock_course;

    lock_course.l_type   = F_WRLCK;
    lock_course.l_whence = SEEK_SET;
    lock_course.l_start  = (i) * sizeof(struct course);
    lock_course.l_len    = sizeof(struct course);
    lock_course.l_pid    = getpid();

    fcntl (fd_course, F_SETLKW, &lock_course);

    lseek (fd_course, (i)*sizeof(struct course), SEEK_SET);
    read  (fd_course, &record_course, sizeof(struct course));

    sprintf (buffer_message, "Course 1 : %s\n", record_student.course1);
    DEBUG (buffer_message)

    sprintf (buffer_message, "Course 1 : %s\n", strtok(record_student.course1, " "));
    DEBUG (buffer_message)

    sprintf (buffer_message, "Course ID : %s\nCourse Name : %s\n", record_course.courseId, record_course.courseName);
    DEBUG (buffer_message)

    char *temp;
    bool result = false;

    if ( strcmp (strtok(record_student.course1, " "), record_course.courseId)==0) {
        result = true;
        memset (record_student.course1, '\0', MAX_COURSEDETAIL_LEN);

        sprintf (buffer_message, "Course 1 : %s\n", record_student.course1);
        DEBUG (buffer_message)
    }

    else if ( strcmp (strtok(record_student.course2, " "), record_course.courseId)==0) {
        result = true;
        memset (record_student.course2, '\0', MAX_COURSEDETAIL_LEN);
    }

    else if ( strcmp (strtok(record_student.course3, " "), record_course.courseId)==0) {
        result = true;
        memset (record_student.course3, '\0', MAX_COURSEDETAIL_LEN);
    }

    else if ( strcmp (strtok(record_student.course4, " "), record_course.courseId)==0) {
        result = true;
        memset (record_student.course4, '\0', MAX_COURSEDETAIL_LEN);
    }

    else if ( strcmp (strtok(record_student.course5, " "), record_course.courseId)==0) {
        result = true;
        memset (record_student.course5, '\0', MAX_COURSEDETAIL_LEN);
    }

    else if ( strcmp (strtok(record_student.course6, " "), record_course.courseId)==0) {
        result = true;
        memset (record_student.course6, '\0', MAX_COURSEDETAIL_LEN);
    }

    if (result && record_student.dropCount<2) {

        DEBUG("Entered the if condition...\n")

        if (strlen(record_student.drop1)>0) strcpy (record_student.drop1, record.courseId);
        else strcpy (record_student.drop2, record.courseId);

        record_student.dropCount++;
        record_student.enrollCount--;
        lseek (fd_student, (-1)*sizeof(struct student), SEEK_CUR);
        int size = write (fd_student, &record_student, sizeof(struct student));
        if (size == -1) {
            perror ("Student write failed");
            return false;
        }

        sprintf (buffer_message, "Student record Status : %d\n", size);
        DEBUG (buffer_message)

        record_course.avilSeat++;
        lseek (fd_course, (-1)*sizeof(struct course), SEEK_CUR);
        size = write (fd_course, &record_course, sizeof(struct course));

        sprintf (buffer_message, "Course record Status : %d\n", size);
        DEBUG (buffer_message)

        char temp[256];
        strcpy(temp, "/home/hari/Desktop/Project/DB/course/");
        strcat (temp, record.courseId);

        int fd = open (temp, O_RDWR , 0644);

        struct enroll currRec;
        struct flock lock;

        lock.l_type   = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start  = 0;
        lock.l_len    = 0;
        lock.l_pid    = getpid();

        fcntl (fd, F_SETLKW, &lock);

        while (read(fd, &currRec, sizeof(struct enroll))>0) {
            if (strcmp(currRec.studentId, record_student.loginId)==0 && strcmp(currRec.courseId, record_course.courseId)==0) {
                currRec.status = 0;
            }
        }

        lseek (fd, (-1)*sizeof(struct enroll), SEEK_CUR);
        write (fd, &currRec, sizeof(struct enroll));

        lock.l_type=F_UNLCK;
        fcntl(fd,F_SETLK,&lock);

        close(fd);
    }

    lock_student.l_type = F_UNLCK;
    fcntl(fd_student,F_SETLK,&lock_student);

    close(fd_student);

    lock_course.l_type = F_UNLCK;
    fcntl(fd_course,F_SETLK,&lock_course);

    close(fd_course);

    DEBUG("Leaving enrollCourse().\n")
    return result;
}

void viewEnrolledCourses(int sd)
{
    DEBUG("Entering viewEnrolledCourses().\n")
    int fd = open ("/home/hari/Desktop/Project/DB/student_db", O_RDONLY, 0644);
    bool result;

    struct flock lock;
    lock.l_type   = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = (userId) * sizeof(struct student);
    lock.l_len    = sizeof(struct student);
    lock.l_pid    = getpid();

    fcntl (fd, F_SETLKW, &lock);

    struct student currRec;
    memset (&currRec, '\0', sizeof(struct student));

    lseek (fd, (userId)*sizeof(struct student), SEEK_SET);
    int size = read (fd, &currRec, sizeof(struct student));

    sprintf (buffer_message, "Student record Status : %d\nUserId : %d\n", size, userId);
    DEBUG (buffer_message)

    sprintf (buffer_message, "currRec.enrollCount : %d\ncurrRec,course1 : %s\n", currRec.enrollCount, currRec.course1);
    DEBUG (buffer_message)

    struct student record;
    memset (&record, '\0', sizeof(struct student));

    record.enrollCount = currRec.enrollCount;
    strcpy (record.course1, currRec.course1);
    strcpy (record.course2, currRec.course2);
    strcpy (record.course3, currRec.course3);
    strcpy (record.course4, currRec.course4);
    strcpy (record.course5, currRec.course5);
    strcpy (record.course6, currRec.course6);

    send (sd, &record, sizeof(struct student), 0);

    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK,&lock);

    DEBUG("Leaving viewEnrolledCourses().\n")
    close(fd);
}

bool studentChangePasswd (struct student record)
{
    DEBUG("Entering studentChangePasswd().\n")
    int fd = open ("/home/hari/Desktop/Project/DB/student_db", O_RDWR, 0644);
    bool result;

    struct flock lock;
    lock.l_type   = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = (userId) * sizeof(struct student);
    lock.l_len    = sizeof(struct student);
    lock.l_pid    = getpid();

    fcntl (fd, F_SETLKW, &lock);

    struct student currRec;
    lseek (fd, (userId)*sizeof(struct student), SEEK_SET);
    read (fd, &currRec, sizeof(struct student));

    strcpy (currRec.passwd, record.passwd);

    lseek (fd, (-1)*sizeof(struct student), SEEK_CUR);
    int size = write (fd, &currRec, sizeof(struct student));
    if ( size != 0)  result = true;
    else  result = false;

    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK,&lock);

    close(fd);
    sprintf(buffer_message,"Leaving studentChangePasswd() - returning value: %d\n",
             result);
    DEBUG(buffer_message)
    return result;
}

bool addStudentToCourse(struct enroll record)
{
    DEBUG("Entering addStudentToCourse().\n")
    char temp[256];
    strcpy(temp, "/home/hari/Desktop/Project/DB/course/");
    strcat (temp, record.courseId);

    int fd = open (temp, O_RDWR | O_CREAT, 0644);
    bool result;

    struct flock lock;
    lock.l_type   = F_WRLCK;
    lock.l_whence = SEEK_END;
    lock.l_start  = 0;
    lock.l_len    = sizeof(struct enroll);
    lock.l_pid    = getpid();

    fcntl(fd, F_SETLKW, &lock);   

    int size = write (fd, &record, sizeof(struct enroll));
    if (size != 0) result = true;
    else result = false;

    sprintf (temp, "%s - %s", record.courseId, record.courseName); 

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    if (result) {
        fd = open ("/home/hari/Desktop/Project/DB/student_db", O_RDWR, 0644);
        bool result = false;

        struct flock lock;
        lock.l_type   = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start  = (userId) * sizeof(struct student);
        lock.l_len    = sizeof(struct student);
        lock.l_pid    = getpid();

        fcntl (fd, F_SETLKW, &lock);

        struct student currRec;
        lseek (fd, (userId)*sizeof(struct student), SEEK_SET);
        read (fd, &currRec, sizeof(struct student));

        if (strlen(currRec.course1) == 0)  strcpy(currRec.course1, temp);
        else if (strlen(currRec.course2) == 0) strcpy(currRec.course2, temp);
        else if (strlen(currRec.course3) == 0) strcpy(currRec.course3, temp);
        else if (strlen(currRec.course4) == 0) strcpy(currRec.course4, temp);
        else if (strlen(currRec.course5) == 0) strcpy(currRec.course5, temp);

        currRec.enrollCount++;

        lseek (fd, (-1)*sizeof(struct student), SEEK_CUR);
        int j = write (fd, &currRec, sizeof(struct student));
        if ( j != 0)  result = true;
        else  result = false;

        lock.l_type=F_UNLCK;
        fcntl(fd,F_SETLK,&lock);

        close(fd);
    }

    sprintf(buffer_message,"Leaving addStudentToCourse() - returning value: %d\n",
             result);
    DEBUG(buffer_message)
    return result;
}
