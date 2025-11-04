#include "parking_lot.h"
// ----------------MAIN-----------------
int main()
{
	ParkingLot* parkingLot = NULL;
	int choice = 0;
	int running = 1;
	while (running)
	{
		printMenu();
		scanf_s("%d", &choice);
		rewind(stdin);
		switch (choice) 
		{
		case 1:
			if (parkingLot != NULL)
			{
				printf("creating a new parkinglot while already having parking lot in system will cause to delete the current parking lot\n");
				printf("and will create a new one, are you sure that what you want to do? if so enter 1, else enter 2:\n");
				scanf_s("%d", &choice);
				if (choice == 1)
				{
					printf("Freeing existing parking lot...\n");
					FreeParkingLot(parkingLot);
				}
				else
				{
					printf("going back to main menu:\n");
					break;
				}
			}
			parkingLot = createParkingLot();
			if (parkingLot == NULL)
			{
				printf("Failed to create parking lot!\n");
				return -1;
			}
			break;
		case 2:
			if (parkingLot == NULL) 
			{
				printf("Please initialize parking lot first!\n");
				break;
			}
			Car newCar = createCar();
			CarParking(parkingLot, newCar);
			break;
		case 3:
			if (parkingLot == NULL) 
			{
				printf("Please initialize parking lot first!\n");
				break;
			}
			RemoveCarFromParking(parkingLot, createCarFromCode(getPlateFromUser()));
			break;
		case 4: 
			if (parkingLot == NULL)
			{
				printf("Please initialize parking lot first!\n");
				break;
			}
			if (saveParkingLotToFile(parkingLot) == -1)
			{
				printf("Failed to save parking lot!\n");
			}
			break;
		case 5:
			if (parkingLot != NULL) 
			{
				printf("load parkinglot from file will delete the current parkinglot, if you're sure enter 1, else enter 2:\n");
				scanf_s("%d", &choice);
				if (choice == 1)
				{
					printf("Freeing existing parking lot...\n");
					FreeParkingLot(parkingLot);
				}
				else
				{
					printf("staying with current parking lot, going back to main menu:\n");
					break;
				}
			}
			parkingLot = loadParkinglotFromFile();
			if (parkingLot == NULL) 
			{
				printf("Failed to load parking lot!\n");
			}
			break;
		case 6:
			if (parkingLot == NULL) 
			{
				printf("Please initialize parking lot first!\n");
				break;
			}
			int floorNum;
			printf("Enter floor number (1-%d): ", parkingLot->totalFloors);
			scanf_s("%d", &floorNum);
			while (floorNum < 1 || floorNum > parkingLot->totalFloors) 
			{
				printf("Invalid floor number\n");
				printf("Enter floor number (1-%d): ", parkingLot->totalFloors);
				scanf_s("%d", &floorNum);
			}
			printListOfParkingInFloor(parkingLot->floors[floorNum - 1]);
			break;
		case 7: 
			printf("Exiting program..\n");
			if (parkingLot != NULL)
			{
				FreeParkingLot(parkingLot);
			}
			running = 0;
			break;
		default:
			printf("Invalid choice! Please try again.\n");
			break;
		}
	}
	return 0;
}
// ----------------FUNCTIONS DEFINTIONS-----------------
int isValidPlate(const char* plate)
{
	int temp = strlen(plate);
	if (temp == 9)
	{
		if (isdigit(plate[0]) && plate[0] != '0' &&
			isdigit(plate[1]) &&
			plate[2] == '-' &&
			isdigit(plate[3]) &&
			isdigit(plate[4]) &&
			isdigit(plate[5]) &&
			plate[6] == '-' &&
			isdigit(plate[7]) &&
			isdigit(plate[8]))
		{
			return 1;
		}
		return 0;
	}
	else if (temp == 10)
	{
		if (isdigit(plate[0]) && plate[0] != '0' &&
			isdigit(plate[1]) &&
			isdigit(plate[2]) &&
			plate[3] == '-' &&
			isdigit(plate[4]) &&
			isdigit(plate[5]) &&
			plate[6] == '-' &&
			isdigit(plate[7]) &&
			isdigit(plate[8]) &&
			isdigit(plate[9]))
		{
			return 1;
		}
		return 0;
	}
	return 0;
}
void FreeParkingLot(ParkingLot* lot)
{
	for (int i = 0; i < lot->totalFloors; i++) 
	{
		freeFloor(&lot->floors[i]);
	}
	free(lot->floors);
	free(lot);
}
void freeFloor(Floor* floor)
{
	freeLinkList(&floor->firstParkingInFloor);
}
void freeCar(Car car)
{
	free(car.plate);
}
void freeLinkList(Node** headList)
{
	Node* current = *headList;
	while (current != NULL)
	{
		Node* next = current->next;
		if (current->parking.isOccupied)
		{
			freeCar(current->parking.currentCar);
		}
		free(current);
		current = next;
	}
	*headList = NULL;
}
void RemoveCarFromParking(ParkingLot* lot, Car car)
{
	for (int i = 0; i < lot->totalFloors; i++)
	{
		Node* currentNode = lot->floors[i].firstParkingInFloor;
		Node* prevNode = currentNode;
		while (currentNode != NULL)
		{
			if ((currentNode->parking.isOccupied) && (strcmp(currentNode->parking.currentCar.plate, car.plate) == 0))
			{
				printf("Car number: %s left the Parking..\n", car.plate);
				currentNode->parking.isOccupied = 0;
				freeCar(currentNode->parking.currentCar);
				lot->floors[i].amountOfParkingLeftInFloor++;
				if (currentNode->parking.currentParkingSpot == lot->floors[i].firstParkingInFloor->parking.currentParkingSpot)
				{
					Node* nextNode = currentNode->next;
					lot->floors[i].firstParkingInFloor = nextNode;
					free(currentNode);
				}
				else if (currentNode->next == NULL)
				{
					prevNode->next = NULL;
					free(currentNode);
				}
				else
				{
					prevNode->next = currentNode->next;
					free(currentNode);
				}
				return;
			}
			prevNode = currentNode;
			currentNode = currentNode->next;
		}
	}
	printf("car number %s wasn't found in the system, please try again..\n", car.plate);
}
void CarParking(ParkingLot* lot, Car car)
{
	int floorNumber;
	int firstemptyParkingSpot = FirstAvailableParkingSpot(lot, &floorNumber);
	if (firstemptyParkingSpot == -1)
	{
		printf("No parking spots are available at this moment, try again later...\n");
		return;
	}
	Node* currNode = lot->floors[floorNumber].firstParkingInFloor;
	if (currNode == NULL)
	{
		currNode = createNode();
		currNode->parking.isOccupied = 1;
		currNode->parking.currentCar = car;
		lot->floors[floorNumber].amountOfParkingLeftInFloor--;
		lot->floors[floorNumber].firstParkingInFloor = currNode;
		currNode->parking.currentParkingSpot = firstemptyParkingSpot;
		printf("Car number: %s parked on floor %d, in spot %d.\n", car.plate, floorNumber + 1, currNode->parking.currentParkingSpot);
		return;
	}
	Node* prevNode = currNode;
	for (int i = 0; i < firstemptyParkingSpot - 1; i++) 
	{
		prevNode = currNode;
		currNode = currNode->next;
	}
	if (currNode != NULL && firstemptyParkingSpot < currNode->parking.currentParkingSpot)
	{
		if (firstemptyParkingSpot < lot->floors[floorNumber].firstParkingInFloor->parking.currentParkingSpot)
		{
			prevNode = createNode();
			prevNode->parking.isOccupied = 1;
			prevNode->parking.currentCar = car;
			lot->floors[floorNumber].amountOfParkingLeftInFloor--;
			prevNode->parking.currentParkingSpot = firstemptyParkingSpot;
			printf("Car number: %s parked on floor %d, in spot %d.\n", car.plate, floorNumber + 1, prevNode->parking.currentParkingSpot);
			lot->floors[floorNumber].firstParkingInFloor = prevNode;
			prevNode->next = currNode;
			return;
		}
		else
		{
			Node* nextNode = currNode;
			currNode = createNode();
			currNode->parking.isOccupied = 1;
			currNode->parking.currentCar = car;
			lot->floors[floorNumber].amountOfParkingLeftInFloor--;
			currNode->parking.currentParkingSpot = firstemptyParkingSpot;
			printf("Car number: %s parked on floor %d, in spot %d.\n", car.plate, floorNumber + 1, currNode->parking.currentParkingSpot);
			prevNode->next = currNode;
			currNode->next = nextNode;
			return;
		}
	}
	currNode = createNode();
	currNode->parking.isOccupied = 1;
	currNode->parking.currentCar = car;
	lot->floors[floorNumber].amountOfParkingLeftInFloor--;
	currNode->parking.currentParkingSpot = firstemptyParkingSpot;
	prevNode->next = currNode;
	printf("Car number: %s parked on floor %d, in spot %d.\n", car.plate, floorNumber + 1, currNode->parking.currentParkingSpot);
}
int FirstAvailableParkingSpot(ParkingLot* lot, int* floor)
{
	int i = 0;
	for (i; i < lot->totalFloors; i++)
	{
		if (lot->floors[i].amountOfParkingLeftInFloor > 0)
		{
			*floor = i;
			break;
		}
		if (i == lot->totalFloors - 1)
		{
			*floor = -1;
			return -1;
		}
	}
	int parkingNumberInFloor = 1;
	Node* currentNode = lot->floors[i].firstParkingInFloor;
	while (currentNode != NULL)
	{
		if (currentNode->parking.currentParkingSpot != parkingNumberInFloor)
		{
			return parkingNumberInFloor;
		}
		parkingNumberInFloor++;
		currentNode = currentNode->next;
	}
	return parkingNumberInFloor;
}
int saveParkingLotToFile(ParkingLot* lot)
{
	FILE* file = NULL;
	char name[50];
	printf("Please enter the ParkingLot name:\n");
	rewind(stdin);
	if (fgets(name, sizeof(name), stdin) != NULL)
	{
		int len = strlen(name);
		if (len > 0 && name[len - 1] == '\n')
		{
			name[len - 1] = '\0';
		}
	}
	strcat(name,".txt");
	name[sizeof(name) - 1] = '\0';
	file = fopen(name, "w");
	if (!file)
	{
		printf("Failed creating file...\n");
		return -1;
	}
	fprintf(file, "Total Floors: %d\n", lot->totalFloors);
	for (int i = 0; i < lot->totalFloors; i++)
	{
		Floor* floor = &lot->floors[i];
		fprintf(file, "Floor %d:\n", floor->floorWeOn);
		fprintf(file, "Max Parking Spots: %d\n", floor->maxParking);
		fprintf(file, "Amount of Parking Left: %d\n", floor->amountOfParkingLeftInFloor);
		Node* currentNode = floor->firstParkingInFloor;
		while (currentNode != NULL)
		{
			Parking* parkingSpot = &currentNode->parking;
			fprintf(file, "Spot %d: ", parkingSpot->currentParkingSpot);
			if (parkingSpot->isOccupied) 
			{
				fprintf(file, "Occupied - Car Plate: %s\n", parkingSpot->currentCar.plate);
			}
			else 
			{
				fprintf(file, "Empty\n");
			}
			currentNode = currentNode->next;
		}
	}
	fclose(file);
	return 1;
}
char* getPlateFromUser()
{
	char tempPlate[12];
	printf("Please enter a plate of the car you want to park:\n");
	rewind(stdin);
	if (fgets(tempPlate, sizeof(tempPlate), stdin) != NULL)
	{
		int len = strlen(tempPlate);
		if (len > 0 && tempPlate[len - 1] == '\n')
		{
			tempPlate[len - 1] = '\0';
		}
	}
	while (isValidPlate(tempPlate) == 0)
	{
		printf("The plate you've entered is incorrect. Please follow these rules:\n");
		printf("If the plate length is 9 characters, it must be formatted as follows:\n");
		printf("1. Start with a number from 1 to 9.\n");
		printf("2. Followed by another number from 0 to 9.\n");
		printf("3. Then, include a '-'.\n");
		printf("4. Next, provide three digits, each between 0 and 9.\n");
		printf("5. Followed by another '-'.\n");
		printf("6. Finally, end with two more digits, each between 0 and 9.\n");
		printf("for example: 12-123-12\n");
		printf("or\n");
		printf("If the plate length is 10 characters, it must be formatted as follows:\n");
		printf("1. Start with a number from 1 to 9.\n");
		printf("2. Followed by another 2 digits from 0 to 9.\n");
		printf("3. Then, include a '-'.\n");
		printf("4. Next, provide two digits, each between 0 and 9.\n");
		printf("5. Followed by another hyphen ('-').\n");
		printf("6. Finally, end with 3 more digits, each between 0 and 9.\n");
		printf("for example: 123-12-123\n");
		printf("Please enter new plate number:\n");
		if (fgets(tempPlate, sizeof(tempPlate), stdin) != NULL)
		{
			int len = strlen(tempPlate);
			if (len > 0 && tempPlate[len - 1] == '\n')
			{
				tempPlate[len - 1] = '\0';
			}
		}
	}
	char* plate = (char*)malloc((strlen(tempPlate)+1)*sizeof(char));
	if (plate == NULL)
	{
		printf("allocate memory for plate eror.\n");
		return NULL;
	}
	strcpy(plate, tempPlate);
	return plate;
}
Car createCar()
{
	Car newCar;
	char* tempPlate = getPlateFromUser();
	newCar.plate = (char*)malloc(strlen(tempPlate) + 1);
	if (newCar.plate == NULL) 
	{
		printf("allocating memeory for plate failed\n");
		return;
	}
	strcpy(newCar.plate, tempPlate);
	free(tempPlate);
	return newCar;
}
Floor* createFloor(int floorNumber, int maxParkingSpots)
{
	Floor* newFloor = (Floor*)malloc(sizeof(Floor));
	if (newFloor == NULL)
	{
		return NULL;
	}
	newFloor->floorWeOn = floorNumber;
	newFloor->maxParking = maxParkingSpots;
	newFloor->amountOfParkingLeftInFloor = maxParkingSpots;
	newFloor->firstParkingInFloor = NULL;
	return newFloor;
}
ParkingLot* createParkingLot()
{
	int totalFloors = 0;
	printf("enter total floors in parking lot:\n");
	scanf_s("%d", &totalFloors);
	while (totalFloors < 1)
	{
		printf("Invalid input! totalFloors can only be positive numbers! please try again:\n");
		scanf_s("%d", &totalFloors);
	}
	ParkingLot* newParkingLot = (ParkingLot*)malloc(sizeof(ParkingLot));
	if (newParkingLot == NULL)
	{
		return NULL;
	}
	newParkingLot->totalFloors = totalFloors;
	newParkingLot->floors = (Floor*)malloc(totalFloors * sizeof(Floor));
	if (newParkingLot->floors == NULL)
	{
		free(newParkingLot);
		return NULL;
	}
	for (int i = 0; i < totalFloors; i++) 
	{
		int maxParkingSpotsFarFloor;
		printf("Please enter amount of parking spots in floor number %d:\n", i + 1);
		scanf_s("%d", &maxParkingSpotsFarFloor);
		while (maxParkingSpotsFarFloor < 1)
		{
			printf("Amount of Parking spts in floor cannot be lower then 1, please try again:\n");
			scanf_s("%d", &maxParkingSpotsFarFloor);
		}
		newParkingLot->floors[i] = *(createFloor(i + 1, maxParkingSpotsFarFloor));
	}
	return newParkingLot;
}
Node* createNode()
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	if (newNode == NULL)
	{
		return NULL;
	}
	newNode->next = NULL;
	return newNode;
}
ParkingLot* loadParkinglotFromFile()
{
		FILE* file = NULL;
		char name[50];
		printf("Please enter the ParkingLot filename to load (without .txt):\n");
		scanf("%s", name);
		strcat(name, ".txt");
		file = fopen(name, "r");
		if (!file) {
			printf("Failed to open file...\n");
			return NULL;
		}
		int totalFloors;
		if (fscanf(file, "Total Floors: %d\n", &totalFloors) != 1) 
		{
			printf("Error reading total floors from file\n");
			fclose(file);
			return NULL;
		}
		ParkingLot* lot = (ParkingLot*)malloc(sizeof(ParkingLot));
		if (!lot)
		{
			printf("Memory allocation failed for parking lot\n");
			fclose(file);
			return NULL;
		}
		lot->totalFloors = totalFloors;
		lot->floors = (Floor*)malloc(totalFloors * sizeof(Floor));
		if (!lot->floors) 
		{
			printf("Memory allocation failed for floors\n");
			free(lot);
			fclose(file);
			return NULL;
		}
		for (int i = 0; i < totalFloors; i++) 
		{
			Floor* floor = createFloor(i + 1, 0);
			if (floor == NULL)
			{
				printf("Memory allocation failed in create floor\n");
				free(lot);
				fclose(file);
				return NULL;
			}
			lot->floors[i] = *floor;
		}
		for (int i = 0; i < totalFloors; i++)
		{
			char line[200];
			int floorNum, maxSpots, leftSpots;
			if (fscanf(file, "Floor %d:\n", &floorNum) != 1  || fscanf(file, "Max Parking Spots: %d\n", &maxSpots) != 1 || fscanf(file, "Amount of Parking Left: %d\n", &leftSpots) != 1)
			{
				printf("Error reading floor data\n");
				FreeParkingLot(lot);
				fclose(file);
				return NULL;
			}
			lot->floors[i].maxParking = maxSpots;
			lot->floors[i].amountOfParkingLeftInFloor = leftSpots;
			Node* lastNode = NULL;
			while (fgets(line, sizeof(line), file))
			{
				if (line[0] == 'F')
				{
					int x = strlen(line);
					x = -x-1;
					fseek(file, x , SEEK_CUR);
					break;
				}
				else if (line[0] == EOF)
				{
					fclose(file);
					printf("Successfully loaded parking lot from file: %s\n", name);
					return lot;
				}
				int spotNum;
				char plate[12] = "";
				sscanf(line, "Spot %d: Occupied - Car Plate: %s", &spotNum, plate);
				Node* newNode = createNode();
				if (!newNode)
				{
					printf("Memory allocation failed for node\n");
					FreeParkingLot(lot);
					fclose(file);
					return NULL;
				}
				newNode->parking.currentParkingSpot = spotNum;
				if (strlen(plate) > 0)
				{
					newNode->parking.isOccupied = 1;
					newNode->parking.currentCar = createCarFromCode(plate);
				}
				if (lastNode == NULL)
				{
					lot->floors[i].firstParkingInFloor = newNode;
				}
				else
				{
					lastNode->next = newNode;
				}
				lastNode = newNode;
			}
		}
		fclose(file);
		printf("Successfully loaded parking lot from file: %s\n", name);
		return lot;
}
Car createCarFromCode(const char* plate)
{
	char tempPlate[12];
	if (strlen(plate) < 12)
	{
		strcpy(tempPlate, plate);
	}
	else
	{
		tempPlate[0] = '1';
	}
	Car newCar;
	while (isValidPlate(tempPlate) == 0)
	{
		printf("The plate you've entered is incorrect. Please follow these rules:\n");
		printf("If the plate length is 9 characters, it must be formatted as follows:\n");
		printf("1. Start with a number from 1 to 9.\n");
		printf("2. Followed by another number from 0 to 9.\n");
		printf("3. Then, include a '-'.\n");
		printf("4. Next, provide three digits, each between 0 and 9.\n");
		printf("5. Followed by another '-'.\n");
		printf("6. Finally, end with two more digits, each between 0 and 9.\n");
		printf("for example: 12-123-12\n");
		printf("or\n");
		printf("If the plate length is 10 characters, it must be formatted as follows:\n");
		printf("1. Start with a number from 1 to 9.\n");
		printf("2. Followed by another 2 digits from 0 to 9.\n");
		printf("3. Then, include a '-'.\n");
		printf("4. Next, provide two digits, each between 0 and 9.\n");
		printf("5. Followed by another hyphen ('-').\n");
		printf("6. Finally, end with 3 more digits, each between 0 and 9.\n");
		printf("for example: 123-12-123\n");
		printf("Please enter new plate number:\n");
		if (fgets(tempPlate, sizeof(tempPlate), stdin) != NULL)
		{
			int len = strlen(tempPlate);
			if (len > 0 && tempPlate[len - 1] == '\n')
			{
				tempPlate[len - 1] = '\0';
			}
		}
	}
	newCar.plate = (char*)malloc(strlen(tempPlate) + 1);
	if (newCar.plate == NULL)
	{
		printf("allocating memory for plate failed\n");
		return;
	}
	tempPlate[sizeof(tempPlate) - 1] = '\0';
	strcpy(newCar.plate, tempPlate);
	return newCar;
}
void printListOfParkingInFloor(Floor floor)
{
	printf("in floor number %d, we have max of %d parkings, and we have %d occupied parkings\n", floor.floorWeOn,floor.maxParking, floor.maxParking - floor.amountOfParkingLeftInFloor);
	printf("---------------------------------------------start of list--------------------------------------\n");
	Node* currNode = floor.firstParkingInFloor;
	if (currNode == NULL)
	{
		printf("all the parkings are open\n");
		printf("------------------------------------------end of list------------------------------------------\n");
		return;
	}
	while (currNode != NULL)
	{
		printf("In parking %d, car %s parked\n", currNode->parking.currentParkingSpot, currNode->parking.currentCar.plate);
		currNode = currNode->next;
	}
	printf("------------------------------------------end of list------------------------------------------\n");
}
void printMenu()
{
	printf("\n=== Parking Lot Management System ===\n");
	printf("1. Initialize new parking lot\n");
	printf("2. Park a car\n");
	printf("3. Remove a car\n");
	printf("4. Save parking lot to file\n");
	printf("5. Load parking lot from file\n");
	printf("6. Print floor status\n");
	printf("7. Exit\n");
	printf("Please enter your choice (1-7): ");
}