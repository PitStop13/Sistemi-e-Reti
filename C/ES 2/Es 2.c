#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LENGTH 100
#define BIG_CLASS_THRESHOLD 5

// The Student struct represents a student
// in a course class
typedef struct
{
  // The student first name
  char *first_name;

  // The student last name
  char *last_name;

  // True if the student has been suspended,
  // false otherwise
  bool is_suspended;
} Student;

// The CourseClass struct represents a course class
// in a school.
typedef struct
{
  // The course class name
  char *name;

  // Total students count
  int total_students;

  // The course class students
  Student *students;
} CourseClass;

// The School struct represents a school
typedef struct
{
  // The school name
  char *name;

  // Total classes count
  int total_classes;

  // The school classes
  CourseClass *classes;
} School;

char *strdup(const char *str)
{
  if (str == NULL)
  {
    return NULL;
  }

  char *ret = (char *)malloc(sizeof(char) * (strlen(str) + 1));
  if (ret == NULL)
  {
    return NULL;
  }
  strcpy(ret, str);
  return ret;
}

/**
 * createStudent creates a valid student object. A valid student is not
 * suspended.
 *
 * @param first_name The first name of the student, must not be empty or NULL.
 * @param last_name The last name of the student, must not be empty or NULL.
 *
 * @returns NULL on any error, the Student object otherwise.
 */
Student *createStudent(char *first_name, char *last_name)
{
  if (first_name == NULL || last_name == NULL || strlen(first_name) == 0 || strlen(last_name) == 0)
  {
    return NULL;
  }

  Student *student = (Student *)malloc(sizeof(Student));
  if (student == NULL)
  {
    return NULL;
  }

  student->first_name = strdup(first_name);
  if (student->first_name == NULL)
  {
    free(student);
    return NULL;
  }
  student->last_name = strdup(last_name);
  if (student->last_name == NULL)
  {
    free(student->first_name);
    free(student);
    return NULL;
  }

  student->is_suspended = false;

  return student;
}

/**
 * suspendStudent marks a student as suspended.
 *
 * @param student The student to suspend, must not be NULL.
 *
 * @returns -1 on any error, 0 otherwise.
 */
int suspendStudent(Student *student)
{
  if (student == NULL)
  {
    return -1;
  }
  student->is_suspended = true;
  return 0;
}

/**
 * destroyStudent deallocates the students data
 * and cleans the occupied memory.
 *
 * @param student The student to destroy, must not be NULL.
 *
 * @returns -1 on any error, 0 otherwise.
 */
int destroyStudent(Student *student)
{
  if (student == NULL || student->first_name == NULL || student->last_name == NULL)
  {
    return -1;
  }
  free(student->last_name);
  free(student->first_name);
  free(student);
  return 0;
}

/**
 * createCourseClass creates a valid course class object. A valid course class
 * is never empty and has a non-empty non-NULL name.
 *
 * @param class_name The name of the class, must not be empty or NULL.
 * @param students The list of students, must have exactly students_count
 * elements.
 * @param students_count The number of students in the students array, must be >
 * 0.
 *
 * @returns NULL on any error, the CourseClass object otherwise.
 */
CourseClass *createCourseClass(char *class_name, Student *students, int students_count)
{
  if (class_name == NULL || strlen(class_name) == 0 || students == NULL || students_count <= 0)
  {
    return NULL;
  }
  CourseClass *courseClass = (CourseClass *)malloc(sizeof(CourseClass));
  if (courseClass == NULL)
  {
    return NULL;
  }

  courseClass->name = strdup(class_name);
  if (courseClass->name == NULL)
  {
    free(courseClass);
    return NULL;
  }

  courseClass->total_students = students_count;

  courseClass->students = (Student *)malloc(sizeof(Student) * students_count);
  if (courseClass->students == NULL)
  {
    free(courseClass->name);
    free(courseClass);
    return NULL;
  }

  for (int i = 0; i < students_count; i++)
  {
    Student *source = students + i;
    Student *temp = createStudent(source->first_name, source->last_name);
    if (temp == NULL)
    {

      for (int j = 0; j < i; j++)
      {
        destroyStudent(courseClass->students + j);
      }
      free(courseClass->students);
      free(courseClass->name);
      free(courseClass);
      return NULL;
    }
    temp->is_suspended = source->is_suspended;
    courseClass->students[i] = *temp;
    free(temp);
  }
  return courseClass;
}

/**
 * getValidStudentsCount counts all the students that are not suspended and
 * returns the count.
 *
 * @param course_class The class to analyze.
 *
 * @returns -1 on any error, the correct count otherwise.
 */
int getValidStudentsCount(CourseClass *course_class)
{
  if (course_class == NULL || course_class->students == NULL || course_class->total_students < 0)
  {
    return -1;
  }
  int cont = 0;
  for (int i = 0; i < course_class->total_students; i++)
  {
    if ((course_class->students + i)->is_suspended != true)
    {
      cont++;
    }
  }
  return cont;
}

/**
 * destroyCourseClass deallocates the course class data
 * and cleans the occupied memory.
 *
 * @param student The course class to destroy, must not be NULL.
 *
 * @returns -1 on any error, 0 otherwise.
 */
int destroyCourseClass(CourseClass *course_class)
{
  if (course_class == NULL || course_class->name == NULL || course_class->students == NULL)
  {
    return -1;
  }
  for (int i = 0; i < course_class->total_students; i++)
  {
    destroyStudent(course_class->students + i);
  }
  free(course_class->students);
  free(course_class->name);
  free(course_class);
  return 0;
}

/**
 * createSchool creates a valid school object. A valid school has at least
 * one course class.
 *
 * @param school_name The name of the school, must not be empty or NULL.
 * @param classes The list of classes, must have exactly classes_count
 * elements.
 * @param classes_count The number of classes in the classes array, must be > 0.
 *
 * @returns NULL on any error, the School object otherwise.
 */
School *createSchool(char *name, CourseClass *classes, int classes_count)
{
  if (name == NULL || strlen(name) == 0 || classes == NULL || classes_count <= 0)
  {
    return NULL;
  }

  School *school = (School *)malloc(sizeof(School));
  if (school == NULL)
  {
    return NULL;
  }
  school->name = strdup(name);
  if (school->name == NULL)
  {
    free(school);
    return NULL;
  }

  school->total_classes = classes_count;
  school->classes = (CourseClass *)malloc(sizeof(CourseClass) * classes_count);
  if (school->classes == NULL)
  {
    free(school->name);
    free(school);
    return NULL;
  }
  for (int i = 0; i < classes_count; i++)
  {
    CourseClass *source = classes + i;
    CourseClass *temp = createCourseClass(source->name, source->students, source->total_students);
    if (temp == NULL)
    {
      for (int j = 0; j < i; j++)
      {
        destroyCourseClass(school->classes + j);
      }
      free(school->classes);
      free(school->name);
      free(school);
      return NULL;
    }
    school->classes[i] = *temp;
    free(temp);
  }
  return school;
}

/**
 * getBigClassesCount returns the number of "big course classes" in the
 * specified school. A class is a big class if the number of valid students
 * exceeds or is equal to BIG_CLASS_THRESHOLD.
 *
 * @param school The school to analyze, must not be NULL.
 *
 * @returns -1 on any error, the correct count otherwise.
 */
int getBigClassesCount(School *school)
{
  if (school == NULL || school->name == NULL || school->classes == NULL || school->total_classes <= 0)
  {
    return -1;
  }

  int cont = 0;
  for (int i = 0; i < school->total_classes; i++)
  {
    int numValidStudents = getValidStudentsCount(school->classes+i);
    if (numValidStudents >= BIG_CLASS_THRESHOLD)
    {
      cont++;
    }
  }
  return cont;
}

/**
 * destroySchool deallocates the school data
 * and cleans the occupied memory.
 *
 * @param school The school to destroy, must not be NULL.
 *
 * @returns -1 on any error, 0 otherwise.
 */
int destroySchool(School *school)
{
 if(school == NULL || school->classes == NULL || school->name == NULL){
  return -1;
 }

 for (int i = 0; i < school->total_classes; i++)
 {
  destroyCourseClass(school->classes + i);
 }
 free(school->classes);
 free(school->name);
 free(school);
 return 0;
}

/**
 * createSchoolFromFile creates a school object from the specified file.
 *
 * @param fp The school file pointer, must not be NULL.
 *
 * @returns NULL on any error, the School object otherwise.
 */
School *createSchoolFromFile(FILE *fp)
{
  return 0;
}
