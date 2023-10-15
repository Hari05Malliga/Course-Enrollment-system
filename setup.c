#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "structures.h"

int main () {
	struct admin record;

	int fd = open ( "/home/hari/Desktop/Project/DB/admin_db", O_RDWR | O_CREAT | O_APPEND, 0644 );
	if ( fd == -1 ) {
		perror ("File open error");
		return -1;
	}

	printf("Username: ");
	scanf ("%[^\n]s", record.userName);

	printf("Password : ");
	char *pass = getpass("");
	strcpy (record.passwd, pass);

	size_t size = write (fd, &record, sizeof(struct admin));
	if ( size == -1 ) {
		perror ("File writing error");
		close ( fd );
		return -1;
	}

	close ( fd );
	return 0;
}
