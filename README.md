# Academia 🎓
### A Course Registration Portal

<br>

⚫ <b><i>Project Goal:</i></b>
<br><br>To develop a course registration system which is user-friendly and multi-functional. <p>

This project mainly deals with the concurrent operations in a safe and secure environment.
- Safe: Concurrency handling.
- Secure: Only authorized faculty and students can access and modify the data.

<br>

⚫ <b><i>Features of Academia:</i></b>
- The application can handle concurrent access and operations.
- Socket programming is used to implement client-server model.
- Storage/ Database: Unix File System. i.e., data is stored typically in files only.
- Password protected login system for both admins and customers.

<br>

⚫ <b><i>How to execute?:</i></b>
- Firstly add a new Admin using a file called "setup.c"
- Change the absolute path mentioned in the code.
- Now compile and run the Server:
```
./server.sh
```
- Compile and run the Client:
```
./client.sh
```
- Done!

<br>

⚫ <b><i>Output:</i></b>

<br>

<b>Initial Client Menu -</b>
# ![InitClient](./Outputs/01.Menu.jpeg)

<br>

<b>Admin Functions -</b>
- Add student:
# ![InitClient](./Outputs/02.AdminAddStudent.jpeg)
- View student details:
# ![InitClient](./Outputs/03.AdminViewStudent.jpeg)
- Update Student Details:
# ![InitClient](./Outputs/04.AdminUpdateStudent.jpeg)
- After Updating Details:
# ![InitClient](./Outputs/05.AfterUpdate.jpeg)
- Activate/Block Students:
# ![InitClient](./Outputs/06.BlockStudent.jpeg)


<br>

<b>Faculty Functions -<b>
- View Offered Courses:
# ![InitClient](./Outputs/07.ViewOfferedCourses.jpeg)
- Add a new Course:
# ![InitClient](./Outputs/08.AddNewCourse.jpeg)
- After adding course:
# ![InitClient](./Outputs/09.AfterAdding.jpeg)
- Remove an offered Course:
# ![InitClient](./Outputs/10.RemovingCourse.jpeg)

<br>

<b>Student Functions -</b>
- View all courses:
# ![InitClient](./Outputs/11.ViewAllCourses.jpeg)
- Enroll in a course:
# ![InitClient](./Outputs/12.Enroll.jpeg)
- After Enrolling:
# ![InitClient](./Outputs/13.AfterEnrolling.jpeg)
- Drop Course:
# ![InitClient](./Outputs/14.DropCourse.jpeg)
- After dropping a course student details updated:
# ![InitClient](./Outputs/15.AfterDropping.jpeg)
