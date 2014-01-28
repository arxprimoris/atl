#include "vector.h"

typedef struct _Employee {
	int id;
	char name[256];
	unsigned char* data;
} Employee;

Vector employeeList;

int addEmployee() {
	Employee* employeeArray;
	Employee newEmployee;
	char input[256];
	
	printf("Name:  ");
	fgets(input, sizeof(input) - 1, stdin);
	sscanf(input, "%s", newEmployee.name);
	printf("ID:  ");
	fgets(input, 4, stdin);
	sscanf(input, "%d", &newEmployee.id);
	
	newEmployee.data = malloc(1024);

	employeeArray = (Employee*) Vector_array(&employeeList);

	for(int i = 0; i < Vector_size(&employeeList); i++) {
		if(employeeArray[i].id > newEmployee.id) {
			Vector_insert(&employeeList, &newEmployee, i);
			return(1);
		}
	}
	Vector_append(&employeeList, &newEmployee);

	return(1);
}

int deleteEmployee() {
	char input[4];
	int id = 0;
	
	printf("Employee ID:  ");
	fgets(input, sizeof(input) - 1, stdin);
	sscanf(input, "%d", &id);

	for(int i = 0; i < Vector_size(&employeeList); i++) {
		if( ( (Employee*)Vector_get(&employeeList, i) )->id == id) {
			Vector_remove(&employeeList, i);
			return(1);
		}
	}		
	printf("Employee ID %d not found!\n\n", id);

	return(0);
}

int listEmployees() {
	Employee* employeeArray;

	employeeArray = (Employee*) Vector_array(&employeeList);

	for(int i = 0; i < Vector_size(&employeeList); i++) {
		printf(" * %s(%d)\n", employeeArray[i].name, employeeArray[i].id);
	}

	return(0);
}

int employeeDestructor(void* employee) {
	Employee theEmployee = *(Employee*) employee;

	free(theEmployee.data);
	printf("Employee data freed!\n");

	return(0);
}

int main (int argc, char *argv[]) {
	char input[10];
	char choice;
	
	Vector_create(&employeeList, 1, sizeof(Employee), employeeDestructor);
	//Employee initEmployee;
	//strncpy(initEmployee.name, "John Doe\0", 10);
	//initEmployee.id = 0;
	//initEmployee.data = NULL;

	//Vector_resize(&employeeList, &initEmployee, 10);
	while(1) {
		printf("Employee Database (%ld/%ld):\n\n", Vector_size(&employeeList), Vector_capacity(&employeeList));
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
				Vector_destroy(&employeeList);
				return(0);
		}
	}
	return(0);
}