#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#define LINESIZE 128

typedef struct Car Car;
typedef struct Node Node;
typedef struct DoublyLinkedList DLList;

struct Car {
	int year;
	char* manufacturer;
	float cc;
};

struct Node {
	Car car;
	Node* next;
	Node* prev;
};

struct DoublyLinkedList {
	Node* start;
	Node* end;
};

void insertAtBeginning(DLList* list, Car car) {

	Node* newNode = malloc(sizeof(Node));
	newNode->car = car;

	newNode->next = list->start;
	newNode->prev = NULL;

	if (list->start) {
		list->start->prev = newNode;
	}
	else {
		list->end = newNode;
	}

	list->start = newNode;
}

void insertAtEnd(DLList* list, Car car) {

	Node* newNode = malloc(sizeof(Node));
	newNode->car = car;

	newNode->prev = list->end;
	newNode->next = NULL;

	if (list->start) {
		list->end->next = newNode;
	}
	else {
		list->start = newNode;
	}

	list->end = newNode;
}

Car deleteFromBeginning(DLList* list) {

	if (list->start) {

		Node* toBeDeleted = list->start;
		Car deletedCar = toBeDeleted->car;

		if (list->start->next) {
			list->start = list->start->next;
			list->start->prev = NULL;

			free(toBeDeleted);
			return deletedCar;
		}
		else {
			free(toBeDeleted);
			list->start = NULL;
			list->end = NULL;

			return deletedCar;
		}

	}
	else {
		printf("List is empty\n");
		Car err = { .year = 0, .manufacturer = "ERR", .cc = 0 };
		return err;
	}

}

Car deleteFromEnd(DLList* list) {
	if (list->end) {

		Node* toBeDeleted = list->end;
		Car deletedCar = toBeDeleted->car;

		if (list->end->prev) {
			list->end = list->end->prev;
			list->end->next = NULL;

			free(toBeDeleted);
			return deletedCar;
		}
		else {
			free(toBeDeleted);
			list->start = NULL;
			list->end = NULL;

			return deletedCar;
		}

	}
	else {
		printf("List is empty\n");
		Car err = { .year = 0, .manufacturer = "ERR", .cc = 0 };
		return err;
	}
}





void printCar(Car car) {
	printf("%s from %d has %5.2f\n", car.manufacturer, car.year, car.cc);
}

void parseListBtoE(DLList list) {
	if (list.start) {
		while (list.start) {
			printCar(list.start->car);
			list.start = list.start->next;
		}
	}
	else {
		printf("List is empty\n");
	}
}

void parseListEtoB(DLList list) {
	if (list.end) {
		while (list.end) {
			printCar(list.end->car);
			list.end = list.end->prev;
		}
	}
	else {
		printf("List is empty\n");
	}
}



void deleteCarByYear(DLList* list, int year) {
	if (list->start) {
		Node* current = list->start;

		while (current) {

			if (current->car.year < year) {
				Node* toBeDeleted = current;
				if (current->prev) {
					current->prev->next = current->next;
				}
				else {
					list->start = current->next;
				}

				if (current->next) {
					current->next->prev = current->prev;
				}
				else {
					list->end = current->prev;
				}

				current = current->next;
				free(toBeDeleted->car.manufacturer);
				free(toBeDeleted);
			}
			else {
				current = current->next;
			}
		}
	}
	else {
		printf("List is empty\n");
	}
}

Car readCarFromFile(FILE* f) {

	char buffer[LINESIZE];

	fgets(buffer, LINESIZE, f);

	Car car;

	char* carData = NULL;

	carData = strtok(buffer, ",");
	car.year = atoi(carData);

	carData = strtok(NULL, ",\n");
	if (!carData) {
		car.manufacturer = NULL;  // Evităm accesul la un pointer invalid
		return car;
	}

	car.manufacturer = malloc((strlen(carData) + 1) * sizeof(char));
	if (!car.manufacturer) {
		printf("Eroare alocare memorie pentru manufacturer!\n");
		exit(EXIT_FAILURE);
	}

	strcpy(car.manufacturer, carData);

	carData = strtok(NULL, ",");
	car.cc = atof(carData);

	return car;

}

void readCarsFromFile(DLList* start, int* noCars, const char* fileName) {

	FILE* f;
	f = fopen(fileName, "r");

	// Comentam liniile urmatoare pentru ca vom folosi functia feof() pentru a determina finalul fisierului
	// Pastram totusi parametrul noCars si il vom incrementa in while, in cazul in care avem nevoie de numarul de masini in main

	// char buffer[LINESIZE];
	// fgets(buffer, LINESIZE, f);

	// *noCars = (int)strtol(buffer, NULL, 10);

	while (!feof(f)) {
		insertAtEnd(start, readCarFromFile(f));
		*noCars += 1;
	}

	fclose(f);

}

int main() {

	int noCars = 0;
	DLList list;

	list.start = NULL;
	list.end = NULL;

	readCarsFromFile(&list, &noCars, "cars.txt");

	parseListBtoE(list);

	printf("\n%d cars were read from file\n", noCars);

	printf("-------------------\n");

	


	printf("---------------------TEMA--------------------\n");

	printf("\nList of cars before deleting the cars:\n");
	parseListBtoE(list);

	printf("\nDeleting cars with year < 2019\n");
	deleteCarByYear(&list, 2019);

	printf("\nList of cars after we deleted the cars with year less than 2019:\n");
	parseListBtoE(list);


	return 0;
}