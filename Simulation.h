#include<iostream>
#include <iomanip>
#include <fstream>
#include <random>  //the random library
#include <time.h>
#include <string>
#include "Elevator.h"

#pragma once

//IMPORTANT INPUT DATA
const int MEAN_ARRIVAL_RATE = 800; //mean arrival rate is 800 customers per hour
const int NUMBER_OF_PASSENGERS = 400; //total morning arrivals

									  //NEW EVENT LIST STRUCTURE FOR THE ELEVATOR SIMULATION
struct Event
{
	int customerNumber;
	//double interArrivalTime;
	int arrivalTime;
	int destinationFloor;
	int serviceEndTime;
	int serviceStartTime;
	int timeInQueue;
	int elevatorNumber;
};


class QueSimulation
{
public:
	void dumpToFile(int i, Event eventlist[], int size, std::string fileName); //writes event list to a file
	void createPassengerList(Event eventlist[], int size, std::mt19937 rnGenerator);
	void runSimulation(int numElev, Event eventlist[]);
	void calcTimeInQueue(Event a[], int);
	void writeStatistics(Event eventlist[], int size, std::string scenarioFileName, int numElev);

private:
	//function prototypes
	double findMinimumWait(Event eventlist[], int size);
	double findMaximumWait(Event eventlist[], int size);
	double CalcAverageWait(Event eventlist[], int size);
	double percentageBelow60Secs(Event eventlist[], int size);
	int findMaximumCompleteTime(Event eventlist[], int size);
	void loadElevators(Event * eventlist, Elevator * elevator, std::queue<int> & lobby, int elevNum);
	void sendElevator(Event * eventlist, Elevator * elevator);
};