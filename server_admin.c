#include "structures.h"
#include "server.h"

bool checkAdmin(struct admin record) 
{
    DEBUG("Entering checkAdmin().\n")
    int fd = open ( "/home/hari/Desktop/Project/DB/admin_db", O_RDONLY , 0644 );
    bool result;

    struct flock lock;

    lock.l_type   = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start  = 0;    	    
	lock.l_len    = sizeof(struct admin);	           
	lock.l_pid    = getpid();

    fcntl (fd, F_SETLKW, &lock);

    struct admin temp;
    read (fd, &temp, sizeof(struct admin));

    if (strcmp(temp.userName, record.userName)==0 && strcmp(temp.passwd, record.passwd)==0) 
        result = true;
    else
        result = false;

    lock.l_type = F_UNLCK;
	fcntl (fd, F_SETLK, &lock);

	close (fd);
    DEBUG("Leaving checkAdmin().\n")
	return result;
}

bool addStudent(struct student record, int sd)
{
    DEBUG("Entering addStudent().\n")
    int flag = 1, fd = open ("/home/hari/Desktop/Project/DB/student_db", O_RDWR, 0644);
    if ( -1 == fd ) {
        fd = open ("/home/hari/Desktop/Project/DB/student_db", O_RDWR | O_CREAT | O_APPEND, 0644);
        record.userId = 0;
        flag = 0;
    }

    bool result;
    struct student lastRec;

    struct flock lock;
    lock.l_type   = F_WRLCK;
    lock.l_whence = SEEK_END;
    lock.l_start  = (-1) * sizeof(struct student);
    lock.l_len    = sizeof(struct student);
    lock.l_pid    = getpid();

    fcntl(fd, F_SETLKW, &lock);

    lseek (fd, (-1)*sizeof(struct student), SEEK_END);
    read (fd, &lastRec, sizeof(struct student));

    if (flag)
        record.userId = lastRec.userId + 1;

    sprintf (record.loginId, "MT%d", record.userId + 1);

    char id[MAX_LOGINID_LEN];
    strcpy (id, record.loginId);
    send (sd, &id, MAX_LOGINID_LEN, 0);

    int size = write (fd, &record, sizeof(struct student));
    if (size != 0) result = true;
    else result = false;

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    sprintf(buffer_message,"Leaving addStudent() - returning value: %d\n",
             result); 
    DEBUG(buffer_message)
    return result;
}

void displayStudentAcc(struct student record,int sd) 
{
    DEBUG("Entering displayStudentAcc().\n")
    int fd = open ("/home/hari/Desktop/Project/DB/student_db", O_RDWR, 0644);
    bool result = true;

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
    int size;
    size = read (fd, &currRec, sizeof(struct student));

    if (size>0) {
        send (sd, &result, sizeof(bool), 0);
        send (sd, &currRec, sizeof(struct student), 0);
    }
    else {
        result = false;
        send (sd, &result, sizeof(bool), 0);
    }

    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK,&lock);

    close(fd);
    DEBUG("Leaving displayStudentAcc().\n")
}

bool addFaculty(struct faculty record, int sd)
{
    DEBUG("Entering addFaculty().\n")
    int flag = 1, fd = open ("/home/hari/Desktop/Project/DB/faculty_db", O_RDWR, 0644);
    if ( -1 == fd ) {
        fd = open ("/home/hari/Desktop/Project/DB/faculty_db", O_RDWR | O_CREAT | O_APPEND, 0644);
        record.userId = 0;
        flag = 0;
    }

    bool result;
    struct faculty lastRec;

    struct flock lock;
    lock.l_type   = F_WRLCK;
    lock.l_whence = SEEK_END;
    lock.l_start  = (-1) * sizeof(struct faculty);
    lock.l_len    = sizeof(struct faculty);
    lock.l_pid    = getpid();

    fcntl(fd, F_SETLKW, &lock);

    lseek (fd, (-1)*sizeof(struct faculty), SEEK_END);
    read (fd, &lastRec, sizeof(struct faculty));

    if (flag)  record.userId = lastRec.userId + 1;

    sprintf (record.loginId, "FT%d", record.userId + 1);
    send (sd, &record, sizeof(struct faculty), 0);

    int size = write (fd, &record, sizeof(struct faculty));
    if (size != 0)  result = true;
    else  result = false;

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    sprintf(buffer_message,"Leaving addFaculty() - returning value: %d\n",
             result); 
    DEBUG(buffer_message)
    return result;

}

void displayFacultyAcc (struct faculty record, int sd)
{
    DEBUG("Entering displayFacultyAcc().\n")
    int fd = open ("/home/hari/Desktop/Project/DB/faculty_db", O_RDWR, 0644);
    bool result = true;

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
    int size;
    size = read (fd, &currRec, sizeof(struct faculty));

    if (size>0) {
        send (sd, &result, sizeof(bool), 0);
        send (sd, &currRec, sizeof(struct faculty), 0);
    }
    else {
        result = false;
        send (sd, &result, sizeof(bool), 0);
    }

    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK,&lock);

    DEBUG("Leaving displayFacultyAcc().\n")
    close(fd);
}

bool validateStudentLoginId(struct student record)
{
    DEBUG("Entering validateStudentLoginId()")
    int fd = open ("/home/hari/Desktop/Project/DB/student_db", O_RDWR, 0644);
    bool result;

    int i;
    sscanf (record.loginId, "MT%d", &record.userId);
    i = record.userId-1;

    struct flock lock;
    lock.l_type   = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = (i) * sizeof(struct student);
    lock.l_len    = sizeof(struct student);
    lock.l_pid    = getpid();

    struct student currRec;
    lseek (fd, (i)*sizeof(struct student), SEEK_SET);
    read(fd, &currRec, sizeof(struct student));

    if (strcmp(currRec.loginId, record.loginId)==0)
        result = true;
    else
        result = false;

    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK,&lock);

    close(fd);
    DEBUG("Leaving validateStudentLoginId()")

    return result;
}

bool updateStudent(struct student record, int sd)
{
    DEBUG("Entering updateStudent().\n")
    int fd = open ("/home/hari/Desktop/Project/DB/student_db", O_RDWR, 0644);
    bool result = false;

    int i;
    sscanf (record.loginId, "MT%d", &record.userId);
    i = record.userId-1;

    struct flock lock;
    lock.l_type   = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = (i) * sizeof(struct student);
    lock.l_len    = sizeof(struct student);
    lock.l_pid    = getpid();

    fcntl (fd, F_SETLKW, &lock);

    struct student currRec;
    lseek (fd, (i)*sizeof(struct student), SEEK_SET);
    read (fd, &currRec, sizeof(struct student));

    if (strlen(record.userName) > 0) strcpy (currRec.userName, record.userName);
    if (record.age > 0) currRec.age = record.age;
    if (strlen(record.email) > 0) strcpy (currRec.email, record.email);

    sprintf (buffer_message, "currRec.userName : %s, currRec.age : %d, currRec.email : %s\n", currRec.userName, currRec.age, currRec.email);
    DEBUG(buffer_message)

    sprintf (buffer_message, "record.userName : %s, record.age : %d, record.email : %s\n", record.userName, record.age, record.email);
    DEBUG(buffer_message)

    lseek (fd, (-1)*sizeof(struct student), SEEK_CUR);
    int j = write (fd, &currRec, sizeof(struct student));
    if ( j != 0)  result = true;
    else  result = false;

    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK,&lock);

    close(fd);
    sprintf(buffer_message,"Leaving updateStudent() - returning value: %d\n", result); 
    DEBUG(buffer_message)
    return result;
}

bool validateFacultyLoginId(struct faculty record)
{
    DEBUG("Entering validateStudentLoginId()")
    int fd = open ("/home/hari/Desktop/Project/DB/faculty_db", O_RDWR, 0644);
    bool result;

    int i;
    sscanf (record.loginId, "FT%d", &record.userId);
    i = record.userId-1;

    struct flock lock;
    lock.l_type   = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = (i) * sizeof(struct faculty);
    lock.l_len    = sizeof(struct faculty);
    lock.l_pid    = getpid();

    struct faculty currRec;
    lseek (fd, (i)*sizeof(struct faculty), SEEK_SET);
    read(fd, &currRec, sizeof(struct faculty));

    if (strcmp(currRec.loginId, record.loginId)==0)
        result = true;
    else
        result = false;

    result = true;
    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK,&lock);

    close(fd);
    DEBUG("Leaving validateFacultyLoginId()")

    return result;
}

bool updateFaculty (struct faculty record, int sd)
{
    DEBUG("Entering updateFaculty().\n")
    int fd = open ("/home/hari/Desktop/Project/DB/faculty_db", O_RDWR, 0644);
    bool result = false;

    sscanf (record.loginId, "FT%d", &record.userId);
    int i = record.userId-1;

    struct flock lock;
    lock.l_type   = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = (i) * sizeof(struct faculty);
    lock.l_len    = sizeof(struct faculty);
    lock.l_pid    = getpid();

    fcntl (fd, F_SETLKW, &lock);

    struct faculty currRec;
    lseek (fd, (i)*sizeof(struct faculty), SEEK_SET);
    read (fd, &currRec, sizeof(struct faculty));

    if (strlen(record.userName) > 0) strcpy (currRec.userName, record.userName);
    if (strlen(record.designation) > 0) strcpy (currRec.designation, record.designation);
    if (strlen(record.department) > 0) strcpy (currRec.department, record.department);
    if (strlen(record.email) > 0) strcpy (currRec.email, record.email);

    lseek (fd, (-1)*sizeof(struct faculty), SEEK_CUR);
    int j = write (fd, &currRec, sizeof(struct faculty));
    if ( j != 0)  result = true;
    else  result = false;

    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK,&lock);

    close(fd);
    sprintf(buffer_message,"Leaving updateFaculty() - returning value: %d\n",
             result); 
    DEBUG(buffer_message)
    return result;
}

int activateStudentAcc (struct student record) 
{
    DEBUG("Entering activateStudentAcc().\n")
    int fd = open ("/home/hari/Desktop/Project/DB/student_db", O_RDWR, 0644);
    int result;

    sscanf (record.loginId, "MT%d", &record.userId);
    int i = record.userId-1;

    struct flock lock;
    lock.l_type   = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = (i) * sizeof(struct student);
    lock.l_len    = sizeof(struct student);
    lock.l_pid    = getpid();

    int status = fcntl (fd, F_SETLKW, &lock);
    if (-1 == status) {
        sprintf (buffer_message,"fcntl fails trying to lock memory that is not in range of the file.");
        DEBUG(buffer_message)
        result = false;
        return result;
    }

    struct student currRec;
    lseek (fd, (i)*sizeof(struct student), SEEK_SET);
    read (fd, &currRec, sizeof(struct student));

    if (currRec.status == 1) result = 0;
    else {
        currRec.status = 1;

        lseek (fd, (-1)*sizeof(struct student), SEEK_CUR);
        int j = write (fd, &currRec, sizeof(struct student));
        if ( j != 0)  result = true;
        else  result = false;
    }

    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK,&lock);

    close(fd);
    sprintf(buffer_message,"Leaving activateStudentAcc() - returning value: %d\n",
             result); 
    DEBUG(buffer_message)
    return result;
}

int deactivateStudentAcc (struct student record)
{
    DEBUG("Entering deactivateStudentAcc().\n")
    int fd = open ("/home/hari/Desktop/Project/DB/student_db", O_RDWR, 0644);
    int result;

    sscanf (record.loginId, "MT%d", &record.userId);
    int i = record.userId-1;

    struct flock lock;
    lock.l_type   = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start  = (i) * sizeof(struct student);
    lock.l_len    = sizeof(struct student);
    lock.l_pid    = getpid();

    int status = fcntl (fd, F_SETLKW, &lock);
    if (-1 == status) {
        sprintf (buffer_message,"fcntl fails trying to lock memory that is not in range of the file.");
        DEBUG(buffer_message)
        result = false;
        return result;
    }

    struct student currRec;
    lseek (fd, (i)*sizeof(struct student), SEEK_SET);
    read (fd, &currRec, sizeof(struct student));

    if (currRec.status == 0) result = 0;
    else {
        currRec.status = 0;

        lseek (fd, (-1)*sizeof(struct student), SEEK_CUR);
        int j = write (fd, &currRec, sizeof(struct student));
        if ( j != 0)  result = true;
        else  result = false;
    }

    lock.l_type=F_UNLCK;
    fcntl(fd,F_SETLK,&lock);

    close(fd);
    sprintf(buffer_message,"Leaving deactivateStudentAcc() - returning value: %d\n",
             result); 
    DEBUG(buffer_message)
    return result;
}
