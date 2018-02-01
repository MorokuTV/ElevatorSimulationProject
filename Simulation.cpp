#include "Simulation.h"
#include <vector>

#pragma once

void QueSimulation::dumpToFile(int i, Event eventlist[], int size, std::string fileName)
{
	std::fstream outFile;
	fileName.append(std::to_string(i));
	fileName.append(".csv");
	outFile.open(fileName, std::ios::out | std::ios::app);

	// Search for end of file. If it is 0 the file is empty, if not zero, there is data.
	outFile.seekg(0, outFile.end);
	int outPos = outFile.tellg();

	if (!outFile.is_open())
	{
		std::cout << "ERROR OPENING YOUR OUTPUT FILE!" << std::endl;
	}
	else if (outPos == 0)
	{
		//write 1st row of header data
		outFile << "Passenger" << ","
			<< "Arrival" << ","
			<< "Destination" << ","
			<< "Service Start" << ","
			<< "Service End" << ","
			<< "Time Waiting" << ","
			<< "Elevator Number" << std::endl;
		//write 2nd row of header data
		outFile << "Number" << ","
			<< "Time(secs)" << ","
			<< "Floor" << ","
			<< "Time(secs)" << ","
			<< "Time(secs)" << ","
			<< "in Queue(secs)" << std::endl;
		for (int i = 0; i<size; i++)
		{
			outFile << eventlist[i].customerNumber << ","
				<< eventlist[i].arrivalTime << ","
				<< eventlist[i].destinationFloor + 2 << ","
				<< eventlist[i].serviceStartTime << ","
				<< eventlist[i].serviceEndTime << ","
				<< eventlist[i].timeInQueue << ","
				<< eventlist[i].elevatorNumber << std::endl;
		}
	}
	
	/*Uncomment this section to append succesive runs to the event list. 
	 Used to ensure that RNG wasn't repeating itself.*/

	else //write out the data from each row of events in the array	
	{
		for (int i = 0; i < size; i++)
		{
			outFile << eventlist[i].customerNumber << ","
				<< eventlist[i].arrivalTime << ","
				<< eventlist[i].destinationFloor + 2 << ","
				<< eventlist[i].serviceStartTime << ","
				<< eventlist[i].serviceEndTime << ","
				<< eventlist[i].timeInQueue << ","
				<< eventlist[i].elevatorNumber << std::endl;
		}
	}
	outFile.close();
}

void QueSimulation::createPassengerList(Event eventlist[], int size, std::mt19937 rnGenerator)
{
	//Let's set up a standard normal distribution objects to be used for the arrival rates
	//arrival times
	std::normal_distribution<double> normalDistribution(900, 300); // Arrival times to be calculated in seconds, with 0 being the start of the simulation

																   //destination floor: a uniform distribution between 0 and 3
	std::uniform_int_distribution<int> uid(0, 3);

	//Let's create our event list generate arrival times and destination floors


	for (int i = 1; i <= size; i++)
	{
		eventlist[i].customerNumber = i;
		eventlist[i].arrivalTime = int(normalDistribution(rnGenerator));
		if (eventlist[i].arrivalTime < 0)
		{
			eventlist[i].arrivalTime = 0; //No arrivals before 7:30
		}
		//uniformly distributed random numbers between 0 and 3. This is for array purposes, print outs will show floor as between 2 and 5
		eventlist[i].destinationFloor = uid(rnGenerator);
	}


	// Sort passenger list by arrival time.
	// Worst-case, unoptimized Bubble Sort (O(n^2) run time)
	for (int i = 0; i < 400; i++)
	{
		for (int j = 0; j < 400; j++)
		{
			if (eventlist[j].arrivalTime > eventlist[j + 1].arrivalTime)
			{
				Event temp = eventlist[j];
				eventlist[j] = eventlist[j + 1];
				eventlist[j + 1] = temp;
			}
		}
	}
}

void QueSimulation::runSimulation(int numElev, Event eventlist[])
{
	Elevator * elevators = new Elevator[numElev];
	std::queue<int> lobby;

	//Overhead variables
	int lobbyTime = 0;
	int tempTime = 0;
	int loadCount = 0;
	int fullCount = 0;
	int emptyCount = 0;
	int hasPassCount = 0;
	int availibleElevators = 0;
	int passNum;

	// Main loop, iterates thru every passenger
	for (int i = 0; i < 401; i++)
	{

		fullCount = 0;
		emptyCount = 0;
		hasPassCount = 0;
		for (int j = 0; j < numElev; j++)
		{
			if (elevators[j].IsFull())
			{
				fullCount++;
			}
			else if (elevators[j].IsEmpty())
			{
				emptyCount++;
			}
			else if (elevators[j].HasPassengers())
			{
				hasPassCount++;
			}
		}

		/*
		if 50+% of the elevators are empty, send any with passengers

		Send any full elevators

		if the current lobby time is too far ahead of the elevator, send it
		*/
		if ((emptyCount / numElev) >= .5)
		{
			for (int j = 0; j < numElev; j++)
			{
				if (elevators[j].HasPassengers())
				{
					sendElevator(eventlist, &elevators[j]);
				}
			}
		}
		else if (fullCount > 0)
		{
			for (int j = 0; j < numElev; j++)
			{
				if (elevators[j].IsFull())
				{
					sendElevator(eventlist, &elevators[j]);
				}
			}
		}
		else
		{
			for (int j = 0; j < numElev; j++)
			{
				if ((!(lobbyTime < (elevators[j].GetTime() + 3))) && elevators[j].HasPassengers())
				{
					sendElevator(eventlist, &elevators[j]);
				}
			}
		}

		// Provides a count of the availibe elevators
		availibleElevators = 0;
		for (int j = 0; j < numElev; j++)
		{
			if (elevators[j].IsAvailibe())
			{
				availibleElevators++;
			}
		}


		// Loop count is used to protect against infinite loop if elevators and lobby aren't snyched well
		// It allows the lobby clock to move forward in small increments to allow the lobby to clear.
		int loopCount = 0;
		// load elevators until lobby is empty or elevators are unavailible
		while (!lobby.empty() && availibleElevators > 0)
		{
			loopCount++;
			for (int j = 0; j < numElev; j++)
			{
				if (elevators[j].IsAvailibe() && lobbyTime >= elevators[j].GetTime())
				{
					loadElevators(eventlist, &elevators[j], lobby, j);
					loopCount--;
					// recheck if elevators are empty after loading
					availibleElevators = 0;
					for (int j = 0; j < numElev; j++)
					{
						if (elevators[j].IsAvailibe())
						{
							availibleElevators++;
						}
					}
				}
			}
			if (loopCount > 3)
			{
				lobbyTime++;
			}
		}

		// Loads a new arrival into the lobby and advances the lobby clock.
		lobby.push(i);
		lobbyTime = eventlist[i].arrivalTime;
	}

	// Overflow protection, in case last passenger arrives but didn't get into an elevator on this pass thru.
	// Essentially the same as the previous loop, but doesn't increment on passengers, doesn't exit until lobby is empty,
	
	// overhead
	int nextElevatorTime = INT_MAX;
	int nextElevatorIndex = 0;
	while (!lobby.empty())
	{
		fullCount = 0;
		emptyCount = 0;
		hasPassCount = 0;
		for (int j = 0; j < numElev; j++)
		{
			if (elevators[j].IsFull())
			{
				fullCount++;
			}
			else if (elevators[j].IsEmpty())
			{
				emptyCount++;
			}
			else if (elevators[j].HasPassengers())
			{
				hasPassCount++;
			}
		}

		if ((emptyCount / numElev) >= .5)
		{
			for (int j = 0; j < numElev; j++)
			{
				if (elevators[j].HasPassengers())
				{
					sendElevator(eventlist, &elevators[j]);
				}
			}
		}
		else if (fullCount > 0)
		{
			for (int j = 0; j < numElev; j++)
			{
				if (elevators[j].IsFull())
				{
					sendElevator(eventlist, &elevators[j]);
				}
			}
		}
		else
		{
			for (int j = 0; j < numElev; j++)
			{
				if ((!(lobbyTime < (elevators[j].GetTime() + 3))) && elevators[j].HasPassengers())
				{
					sendElevator(eventlist, &elevators[j]);
				}
			}
		}

		// Check for avalible elevators

		availibleElevators = 0;
		for (int j = 0; j < numElev; j++)
		{
			if (elevators[j].IsAvailibe())
			{
				availibleElevators++;
			}
		}

		int loopCount = 0;
		// load elevators until lobby is empty or elevators are unavailible
		while (!lobby.empty() && availibleElevators > 0)
		{
			loopCount++;
			for (int j = 0; j < numElev; j++)
			{
				if (elevators[j].IsAvailibe() && lobbyTime >= elevators[j].GetTime())
				{
					loadElevators(eventlist, &elevators[j], lobby, j);
					loopCount--;
					// recheck if elevators are empty after loading
					availibleElevators = 0;
					for (int j = 0; j < numElev; j++)
					{
						if (elevators[j].IsAvailibe())
						{
							availibleElevators++;
						}
					}
				}
			}
			if (loopCount > 3)
			{
				lobbyTime++;
			}
		}
	}

	// The lobby is empty if you get to here, this just sends the rest of the elevators
	//clear elevators
	for (int j = 0; j < numElev; j++)
	{
		if ((!(lobbyTime < (elevators[j].GetTime() + 3))) && elevators[j].HasPassengers())
		{
			sendElevator(eventlist, &elevators[j]);
		}
	}
}

void QueSimulation::calcTimeInQueue(Event eventlist[], int size)
{
	for (int i = 1; i <= size; i++)
	{
		eventlist[i].timeInQueue = eventlist[i].serviceStartTime - eventlist[i].arrivalTime;
	}
}

void QueSimulation::writeStatistics(Event eventlist[], int size, std::string scenarioFileName, int numElev)
{
	std::fstream outFile;
	outFile.open(scenarioFileName, std::ios::out | std::ios::app);

	// Search for end of file. If it is 0 the file is empty, if not zero, there is data.
	outFile.seekg(0, outFile.end);
	int outPos = outFile.tellg();

	if (!outFile.is_open())
	{
		std::cout << "ERROR OPENING YOUR OUTPUT FILE!" << std::endl;
	}
	else if (outPos == 0)
	{
		//write 1st row of header data
		outFile << "Average Queue" << ","
			<< "Minimum Queue" << ","
			<< "Maximum Queue" << ","
			<< "Percent Serviced in" << ","
			<< "Last Service Complete" << ","
			<< "Elevator(s)" << std::endl;
		//write 2nd row of header data
		outFile << "Time(sec)" << ","
			<< "Time(sec)" << ","
			<< "Time(sec)" << ","
			<< "1 Minute" << ","
			<< "Time(sec)" << ","
			<< "Number of:" << std::endl;

		//write out the data from each row of events in the array	
		outFile << CalcAverageWait(eventlist, size) << ","
			<< findMinimumWait(eventlist, size) << ","
			<< findMaximumWait(eventlist, size) << ","
			<< percentageBelow60Secs(eventlist, size) << ","
			<< findMaximumCompleteTime(eventlist, size) << ","
			<< numElev << ","
			<< std::endl;

		outFile.close();
	}
	else
	{
		//write out the data from each row of events in the array	
		outFile << CalcAverageWait(eventlist, size) << ","
			<< findMinimumWait(eventlist, size) << ","
			<< findMaximumWait(eventlist, size) << ","
			<< percentageBelow60Secs(eventlist, size) << ","
			<< findMaximumCompleteTime(eventlist, size) << ","
			<< numElev << std::endl;

		outFile.close();
	}
}

double QueSimulation::findMinimumWait(Event eventlist[], int size)
{
	double minimum = eventlist[1].timeInQueue;
	for (int i = 0; i < size; i++)
	{
		if (eventlist[i].timeInQueue < minimum)
		{
			minimum = eventlist[i].timeInQueue;
		}
	}
	return minimum;
}

double QueSimulation::findMaximumWait(Event eventlist[], int size)
{
	double maximum = eventlist[1].timeInQueue;
	for (int i = 0; i < size; i++)
	{
		if (eventlist[i].timeInQueue > maximum)
		{
			maximum = eventlist[i].timeInQueue;
		}
	}
	return maximum;
}

double QueSimulation::CalcAverageWait(Event eventlist[], int size)
{
	double average = 0;
	for (int i = 0; i < size; i++)
	{
		average += eventlist[i].timeInQueue;
	}
	return  average / size; //sum of all / total
}

double QueSimulation::percentageBelow60Secs(Event eventlist[], int size)
{
	double percentBelow60 = 0;
	for (int i = 0; i < size; i++)
	{
		if (eventlist[i].timeInQueue < 60)
		{
			percentBelow60++;
		}
	}
	return  percentBelow60 / size; //return calculated percentage
}

int QueSimulation::findMaximumCompleteTime(Event eventlist[], int size)
{
	int maximum = eventlist[1].serviceEndTime;
	for (int i = 0; i < size; i++)
	{
		if (eventlist[i].serviceEndTime > maximum)
		{
			maximum = eventlist[i].serviceEndTime;
		}
	}
	return maximum;
}

void QueSimulation::loadElevators(Event * eventlist, Elevator * elevator, std::queue<int>& lobby, int elevNum)
{
	bool overload = false;
	int tempTime;

	// Only runs if the lobby has passengers to load.
	if (!lobby.empty())
	{
		// Get the next passenger index from the lobby		
		int passNum = lobby.front();
		lobby.pop();

		// If elevator time is greater than arrival time, it means passenger was waiting on elevator
		if (elevator->GetTime() > eventlist[passNum].arrivalTime)
		{
			tempTime = elevator->GetTime();
		}
		else
		{
			// Otherwise the passenger loaded right away
			tempTime = eventlist[passNum].arrivalTime;
		}

		elevator->Enque(passNum, eventlist[passNum].destinationFloor);		// Load passengers into the elevator
		eventlist[passNum].serviceStartTime = tempTime + 3;					// Set service start time to 3 seconds after start of load time.
																			// This covers the loading time for the elevator.
		eventlist[passNum].elevatorNumber = elevNum + 1;					// Only used for data collection
		elevator->SetTime(tempTime + 3);									// Increment elevator time to cover loading time
	}
}

void QueSimulation::sendElevator(Event * eventlist, Elevator * elevator)
{
	int elevatorTime = 0;
	int tempFloor = 0;
	int highestFloor = 0;
	int lastUnload = 0;
	int passNum = 0;

	// Loop until elevator doesn't have passengers.
	while (elevator->HasPassengers())
	{
		// Get first passenger out of elecator
		elevator->Deque(passNum, tempFloor);
		
		// Sets the time that the elevator would arrive to the 2nd floor, only executes on the first loop.
		if (elevatorTime == 0)
		{
			elevatorTime = 15 + elevator->GetTime();	// Elevator time keeps track of traveling and unloading time
		}

		// if next passenger isn't getting off on current floor, add travel time to move to next floor.
		if (tempFloor > highestFloor)
		{
			elevatorTime += 15 * (tempFloor - highestFloor);
			highestFloor = tempFloor;
		}
		elevatorTime += 3;
		eventlist[passNum].serviceEndTime = elevatorTime;
		lastUnload = eventlist[passNum].serviceEndTime; // Store the unload time to calculate when the elvator gets back to the lobby
	}
	elevatorTime = 15 + (15 * highestFloor);			// account for coming down from highest floor reached
	elevator->SetAvailTime(lastUnload + elevatorTime);	// Elevator isn't availible again until it has come back from it's trip.
	elevator->SetTime(elevator->GetAvailTime());		// Sets the time of the elevator
}
