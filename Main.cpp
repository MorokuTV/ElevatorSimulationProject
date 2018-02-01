#include "Simulation.h"
#include <queue>
#include <chrono>

//CONSTRUCT EVENT LIST
Event eventList[NUMBER_OF_PASSENGERS + 1] = { 0 };


//**************************************************************************
//MAIN()
//**************************************************************************
int main()
{
	QueSimulation simulation;
	int menuInput = 0;
	//	Main Program Loop for Input Handling
	do
	{
		//	pring input handling menu
		std::cout << std::endl <<	"Elevator Simulation:" << std::endl
			<< "Please enter a number of elevators to simulate with:";
		
		while (!(std::cin >> menuInput) || menuInput < 0) // Don't move forward until an int is input
		{
			std::cout << "Please enter a number of elevators to simulate with: ";
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n'); // NB: preferred method for flushing cin
		}

		// Simulation loop
		for (int i = 0; i < 10; i++)
		{
			std::random_device rd;
			std::mt19937  rnGenerator(rd());
			
			simulation.createPassengerList(eventList, NUMBER_OF_PASSENGERS + 1, rnGenerator);
			simulation.runSimulation(menuInput, eventList);
			simulation.calcTimeInQueue(eventList, NUMBER_OF_PASSENGERS);
			simulation.dumpToFile(menuInput, eventList, NUMBER_OF_PASSENGERS + 1, "eventlist");
			simulation.writeStatistics(eventList, NUMBER_OF_PASSENGERS, "ElevatorSimulationResults.csv", menuInput);

			std::cout << "Simulation run " << i+1 << " complete, check ElevatorSimulationResults.csv for accuracy!" << std::endl << std::endl;
		}

	} while (menuInput != 0);

	return 0;
}