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
    lock.l_len    = 2 * sizeof(struct course);
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
