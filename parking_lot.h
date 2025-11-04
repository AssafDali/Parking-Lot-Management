#ifndef PARKING_LOT_H
#define PARKING_LOT_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Car
{
	char* plate;
}Car;

typedef struct Parking
{
	int currentParkingSpot;
	Car currentCar;
	int isOccupied;
}Parking;

typedef struct Node
{
	Parking parking;
	struct Node* next;
}Node;

typedef struct floor
{
	int floorWeOn, maxParking, amountOfParkingLeftInFloor;
	Node* firstParkingInFloor;
}Floor;

typedef struct ParkingLot 
{
	Floor* floors;
	int totalFloors;
}ParkingLot;

// functions declarations 
void printListOfParkingInFloor(Floor floor);
int isValidPlate(const char* plate);
Node* createNode();
Car createCar();
Floor* createFloor(int floorNumber, int maxParkingSpots);
ParkingLot* createParkingLot();
void CarParking(ParkingLot* lot, Car car);
int FirstAvailableParkingSpot(ParkingLot* lot, int* floor);
void FreeParkingLot(ParkingLot* lot);
void RemoveCarFromParking(ParkingLot* lot, Car car);
int saveParkingLotToFile(ParkingLot* parkingLot);
ParkingLot* loadParkinglotFromFile();
void freeFloor(Floor* floor);
void freeCar(Car car);
void freeLinkList(Node** headList);
Car createCarFromCode(const char* plate);
void printMenu();
char* getPlateFromUser();
// End of functios declarations
#endif 