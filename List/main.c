#include <stdlib.h>
#include <stdio.h>

#include "list.h"

typedef struct _Employee {
	int id;
	char name[256];
	void* data;
} Employee;

LinkedList employeeList;

int employeeDestructor(void* employee) {
	Employee* theEmployee = (Employee*) employee;

	printf("Employee %s(%d) destructor called...\n", theEmployee->name, theEmployee->id);

	free(theEmployee);

	return(0);
}

int employeeCompare(void* first, void* second) {
	Employee* firstEmployee = (Employee*) first;
	Employee* secondEmployee = (Employee*) second;

	return(firstEmployee->id - secondEmployee->id);
}

int addEmployee() {
	Employee* newEmployee = (Employee*) malloc(sizeof(Employee));
	Employee* curEmployee = (Employee*) List_iteratorBegin(&employeeList);
	char input[256];

	printf("Name:  ");
	fgets(input, sizeof(newEmployee->name) - 1, stdin);
	sscanf(input, "%s", newEmployee->name);
	printf("ID:  ");
	fgets(input, 4, stdin);
	sscanf(input, "%d", &newEmployee->id);

	while(curEmployee != NULL) {
		if(employeeCompare(curEmployee, newEmployee) < 0) {
			List_iteratorInsert(&employeeList, newEmployee);
			return(1);
		}
		curEmployee = (Employee*) List_iteratorNext(&employeeList);
	}
	List_append(&employeeList, newEmployee);

	return(1);
}

int deleteEmployee() {
	Employee dummyEmployee;
	char input[4];

	printf("Employee ID:  ");
	fgets(input, sizeof(input) - 1, stdin);
	sscanf(input, "%d", &dummyEmployee.id);

	if(List_remove(&employeeList, &dummyEmployee, employeeCompare) == LIST_FUNC_SUCCESS) {
		return(0);
	}

	printf("\nEmployee ID %d not found!\n\n", dummyEmployee.id);
	return(1);
}

int listEmployees() {
	Employee* curEmployee = (Employee*) List_iteratorBegin(&employeeList);

	while(curEmployee != NULL) {
		printf(" * %s (%d)\n", curEmployee->name, curEmployee->id);
		curEmployee = (Employee*) List_iteratorNext(&employeeList);
	}

	return(0);
}

int main (int argc, char *argv[]) {
	char input[10];
	char choice;

	List_create(&employeeList, &employeeDestructor);

	while(1) {
		printf("Employee Database:\n\n");
		printf("[A]dd employee\n");
		printf("[D]elete employee\n");
		printf("[L]ist employee(s)\n");
		printf("[Q]uit\n\n:");

		fgets(input, sizeof(input) - 1, stdin);
		sscanf(input, "%c", &choice);

		switch(input[0]) {
			case 'a':
				addEmployee();
				break;
			case 'd':
				deleteEmployee();
				break;
			case 'l':
				listEmployees();
				break;
			case 'q':
				List_destroy(&employeeList);
				return(0);
		}
	}
	return(0);
}
