#include "structures.h"
#include "server.h"

int userId;
int debug;
char buffer_message[256];

void serverTask(int sd) 
{
    DEBUG("Entering serverTask().")
    bool result;
    int choice, select;
    while (1) {
        recv(sd, &choice, sizeof(int), 0);
        printf ("Choice : %d\n", choice);

        if (choice == 1) {
            struct student record;
            recv (sd, &record, sizeof(struct student), 0);
            result = checkStudent(record, sd);
            send (sd, &result, sizeof(bool), 0);
        }
        else if ( choice == 2) {
            struct faculty record;
            recv (sd, &record, sizeof(struct faculty), 0);
            result = checkFaculty(record, sd);
            send (sd, &result, sizeof(bool), 0);
        }
        else if ( choice == 3 ) {
            struct admin record;
            recv (sd, &record, sizeof(struct admin), 0);
            result = checkAdmin(record);
            send (sd, &result, sizeof(bool), 0);
        }
        else {
            result = false;
            send (sd, &result, sizeof(bool), 0);        
        }
        if (result) break;
    }

    while (1) {
        recv (sd, &select, sizeof(int), 0);
        if (choice == 1) {
            if (select == 1) {
                viewAvailableCourse(sd);
            }

            if (select == 2) {
                struct enroll record;
                recv (sd, &record, sizeof(struct enroll), 0);
                result = enrollCourse(record);
                send (sd, &result, sizeof(bool), 0);
            }

            if (select == 3) {
                struct course record;
                recv (sd, &record, sizeof(struct course), 0);
                result = dropCourse (record);
                send (sd, &result, sizeof(bool), 0);
            }

            if (select == 4) {
                viewEnrolledCourses(sd);
            }

            if (select == 5) {
                struct student record;
                recv (sd, &record, sizeof(struct student), 0);
                result = studentChangePasswd (record);
                send (sd, &result, sizeof(bool), 0);
            }

            if (select == 6) {
                break;
            }
        }
        else if (choice == 2) {
            if (select == 1) {
                viewOfferedCourse(sd);
            }

            if (select == 2) {
                result = checkFacultyEligibility ();
                send (sd, &result, sizeof(bool), 0);
                if (result) {
                    struct course record;
                    recv (sd, &record, sizeof(struct course), 0);
                    addCourse(record, sd);
                }
            }

            if (select == 3) {

            }

            if (select == 4) {

            }

            if (select == 5) {
                struct faculty record;
                recv (sd, &record, sizeof(struct faculty), 0);
                result = facultyChangePasswd (record);
                send (sd, &result, sizeof(bool), 0);
            }

            if (select == 6) {
                break;
            }
        }
        else if (choice == 3) {
            if (select == 1) {
                struct student record;
                recv (sd, &record, sizeof(struct student), 0);
                result = addStudent(record,sd);
                send (sd, &result, sizeof(bool), 0);
            }

            if (select == 2) {
                struct student record;
                recv (sd, &record, sizeof(struct student), 0);
                displayStudentAcc(record,sd);
            }

            if (select == 3) {
                struct faculty record;
                recv (sd, &record, sizeof(struct faculty), 0);
                result = addFaculty(record, sd);
                send (sd, &result, sizeof(int), 0);
            }

            if (select == 4) {
                struct faculty record;
                recv (sd, &record, sizeof(struct faculty), 0);
                displayFacultyAcc(record, sd);
            }

            if (select == 5) {
                struct student record;
                recv(sd, &record, sizeof(struct student), 0);
                result = validateStudentLoginId(record);
                send (sd, &result, sizeof(bool), 0);
                if (result) {
                    recv (sd, &record, sizeof(struct student), 0);
                    result = updateStudent(record, sd);
                    send (sd, &result, sizeof(bool), 0);
                }
            }

            if (select == 6) {
                struct faculty record;
                recv (sd, &record, sizeof(struct faculty), 0);
                result = validateFacultyLoginId(record);
                send (sd, &result, sizeof(bool), 0);
                if (result) {
                    recv (sd, &record, sizeof(struct faculty), 0);
                    result = updateFaculty(record, sd);
                    send (sd, &result, sizeof(bool), 0);
                }
            }

            if (select == 7) {
                struct student record;
                recv (sd, &record, sizeof(struct student), 0);
                int attempt = activateStudentAcc(record);
                send (sd, &attempt, sizeof(int), 0);
            }

            if (select == 8) {
                struct student record;
                recv (sd, &record, sizeof(struct student), 0);
                int attempt = deactivateStudentAcc(record);
                send (sd, &attempt, sizeof(int), 0);
            }

            if (select == 9) {
                break;
            }
        }
    }
    close(sd);
    DEBUG("Leaving serverTask().")
    printf ("Client session ended\n");
    return;
}


int main() {
    determine_debug();  /* set the enable or disable of debug logs */
    DEBUG("Entering server main().");

    int sd, client_sd;

    struct sockaddr_in address, client;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sd) {
        perror("Unable to create socket");
        return -1;
    }

    sprintf(buffer_message,"Successfully created a server socket (%d).\n", sd);
    DEBUG(buffer_message);

    // printf("Server Side Socket Created...\n");

    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);

    if (-1 == bind(sd, (struct sockaddr *)&address, sizeof(address))) {
        perror("Error while binding name to socket");
        return -1;
    }

    sprintf(buffer_message,"Bind successful on server socket (%d).\n", sd);
    DEBUG(buffer_message);

    // printf("Binding to socket is successful\n");

    if (-1 == listen(sd, 5)) { 
        perror("Error while trying to listen for connection");
        return -1;
    }

    sprintf(buffer_message,"Listen successful on server socket (%d).\n", sd);
    DEBUG(buffer_message);

    // printf("Now listening for connections on socket\n");
    printf ("Welcome to IIITB Academia Portal\n\n");
    printf ("Waiting for client to connect...\n");

    while (1) {
        int clientSize = sizeof(client);
        client_sd = accept(sd, (struct sockaddr *)&client, (socklen_t *)&clientSize);

    sprintf(buffer_message,"accept successful - server socket (%d), client socket %d.\n",sd, client_sd);
    DEBUG(buffer_message);

        if (-1 == client_sd) {
            perror("Error while accepting connection");
        } else {
            if (fork() == 0) {
                // Child process
                close(sd); // Close the listening socket in the child process
                printf("Connected to the client\n");                

                serverTask(client_sd);

                exit(EXIT_SUCCESS);
            } else {
                // Parent process
                close(client_sd); // Close the client socket in the parent process
            }
        }
    }
    DEBUG("Leaving server main().");
    return 0;
}
