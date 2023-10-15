#include "structures.h"
#include "client.h"

int main() {

    determine_debug();  /* set the enable or disable of debug logs */
    DEBUG("Entering client main().")

    system ("clear");
    int sd;
    struct sockaddr_in address;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sd) {
        perror("Error: Unable to create socket");
        return -1;
    }
  
    sprintf(buffer_message,"Successfully created a client socket (%d).\n", sd);
    DEBUG(buffer_message);

    // printf("Client Side Socket Created...\n");

    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);

    if (-1 == connect(sd, (struct sockaddr *)&address, sizeof(address))) {
        perror("Error: Unable to connect to the server");
        return -1;
    }

    sprintf(buffer_message,"Successfully connected to the server.\n");
    DEBUG(buffer_message)

    // printf("Client to server connection is successfully established.\n");
    printf ("Welcome to IIITB Academia Portal ... Course Registration\n\n");

    chooseOption(sd);
    showMenu(sd);

    DEBUG("Leaving client main().")    
    close(sd);
    return 0;
}

void chooseOption(int sd)
{
    DEBUG("Entering chooseOption().\n")
    do {
        printf("Login as:\n1. Student\n2. Faculty\n3. Admin\n");
        printf("Your choice [1-3]: ");
        scanf("%d", &choice);
        getchar();

        if (choice < 1 || choice > 3) {
            printf("Invalid selection. Please try again.\n");
        }
    } while (choice < 1 || choice > 3);

    send(sd, &choice, sizeof(int), 0);

    switch (choice) {
        case 1:  attemptStudentLogin(sd); break;
        case 2:  attemptFacultyLogin(sd); break;
        case 3:  attemptAdminLogin(sd);   break;
    }
    DEBUG("Leaving chooseOption().\n")
}

void studentMenu()
{
        printf ("..........Welcome to Student Menu..........\n");
        printf ("1. View All Courses\n");
        printf ("2. Enroll New Course\n");
        printf ("3. Drop\n");
        printf ("4. View Enrolled Course Details\n");
        printf ("5. Change Password\n");
        printf ("6. Logout & Exit\n");
        printf ("Your choice [1-6] : ");
        getchar();
}

void facultyMenu()
{
        printf ("..........Welcome to Faculty Menu..........\n");
        printf ("1. View Offering Courses\n");
        printf ("2. Add New Course\n");
        printf ("3. Remove Course from Catalog\n");
        printf ("4. Update Course Details\n");
        printf ("5. Change Password\n");
        printf ("6. Logout & Exit\n");
        printf ("Your choice [1-6] : ");
        getchar();
}

void adminMenu()
{
        printf ("..........Welcome to Admin Menu..........\n");
        printf ("1. Add Student\n");
        printf ("2. Display Student record\n");
        printf ("3. Add Faculty\n");
        printf ("4. Display Faculty record\n");
        printf ("5. Update Student Details\n");
        printf ("6. Update Faculty Details\n");
        printf ("7. Activate Student Account\n");
        printf ("8. Deactivate Student Account\n");
        printf ("9. Exit\n");
        printf ("Your choice [1-9] : ");
        getchar();
}

void showMenu(int sd)
{
    DEBUG("Entering showMenu().\n")
    int select;
    if (choice == 1) {
        studentMenu();
        scanf ("%d", &select);
        switch (select) {
            case 1: viewAvailableCourse(sd);
                    break;
            case 2: enrollCourse(sd);
                    break;
            case 3: break;
            case 4: viewEnrolledCourses(sd);
                    break;
            case 5: studentChangePasswd(sd);
                    break;
            case 6: send (sd, &select, sizeof(int), 0);
                    printf ("Thanks for using IIITB Academia\n");
                    exit(0);
            default: printf ("Invalid option selecction\n");
        }
    }
    else if (choice == 2) {
        facultyMenu();
        scanf ("%d", &select);
        switch (select) {
            case 1: viewOfferedCourse(sd);
                    break;
            case 2: addCourse(sd);
                    break;
            case 3: break;
            case 4: break;
            case 5: facultyChangePasswd(sd);
                    break;
            case 6: send (sd, &select, sizeof(int), 0);
                    printf ("Thanks for using IIITB Academia\n");
                    exit(0);
            default: printf ("Invalid option selecction\n");
        }
    }
    else if (choice == 3) {
        adminMenu();
        scanf ("%d", &select);
        switch (select) {
            case 1: addStudent(sd);
                    break;
            case 2: displayStudentAcc(sd);
                    break;
            case 3: addFaculty(sd);
                    break;
            case 4: displayFacultyAcc(sd);
                    break;
            case 5: updateStudent(sd);
                    break;
            case 6: updateFaculty(sd);
                    break;
            case 7: activateStudentAcc(sd);
                    break;
            case 8: deactivateStudentAcc(sd);
                    break;
            case 9: send (sd, &select, sizeof(int), 0);
                    printf ("Thanks for using IIITB Academia\n");
                    exit(0);
            default: printf ("Invalid option selecction\n");
                     showMenu (sd);
        }
    }
    DEBUG("Leaving showMenu().\n")
}

void attemptAdminLogin(int sd)
{
    DEBUG("Entering attemptAdminLogin().\n")
    bool result;
    struct admin record;
    memset (&record, '\0', sizeof(struct admin));

    printf ("userName : ");
    scanf ("%[^\n]s", record.userName);

    printf ("Password : ");
    char *pass = getpass("");
    strcpy (record.passwd, pass);

    send(sd, &record, sizeof(struct admin), 0);
    recv(sd, &result, sizeof(bool), 0);

    if (result) 
        printf ("Successfully logged in...\n\n");
    else {
        printf ("Invalid Login credentials...\n\n");
        chooseOption(sd);
    }
    DEBUG("Leaving attemptAdminLogin().\n")
}

void attemptStudentLogin(int sd)
{
    DEBUG("Entering attemptStudentLogin().\n")
    bool result;
    struct student record;
    memset (&record, '\0', sizeof(struct student));

    printf ("Login ID : ");
    scanf("%[^\n]s", record.loginId);

    printf ("Password : ");
    char *pass = getpass("");
    strcpy (record.passwd, pass);

    send(sd, &record, sizeof(struct student), 0);
    recv(sd, &result, sizeof(bool), 0);

    if (result) {
        printf ("Successfully logged in...\n\n");

        int temp_userId;
        sscanf (record.loginId, "MT%d", &temp_userId);
        userId = --temp_userId;
    }
    else {
        printf ("Invalid Login credentials or Account is Deactive...\n\n");
        chooseOption(sd);
    }
    DEBUG("Leaving attemptStudentLogin().\n")
}

void attemptFacultyLogin(int sd)
{
    DEBUG("Entering attemptFacultyLogin().\n")
    bool result;
    struct faculty record;
    memset (&record, '\0', sizeof(struct faculty));

    printf ("Login ID : ");
    scanf("%[^\n]s", record.loginId);

    printf ("Password : ");
    char *pass = getpass("");
    strcpy (record.passwd, pass);

    send(sd, &record, sizeof(struct faculty), 0);
    recv(sd, &result, sizeof(bool), 0);

    if (result) {
        printf ("Successfully logged in...\n\n");

        int temp_userId;
        sscanf (record.loginId, "FT%d", &temp_userId);
        userId = --temp_userId;
    }
    else {
        printf ("Invalid Login credentials...\n\n");
        chooseOption(sd);
    }
    DEBUG("Leaving attemptFacultyLogin().\n")
}

