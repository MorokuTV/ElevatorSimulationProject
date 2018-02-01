#include "Elevator.h"

void Elevator::Enque(int passNum, int destFloor)
{
	queue.Enqueue(passNum, destFloor);
}

void Elevator::Deque(int & passNum, int & destFloor)
{
	queue.Dequeue(passNum, destFloor);
}

bool Elevator::IsFull()
{
	bool isFull = false;
	if (queue.GetSize() > 14)
	{
		isFull = true;
	}
	return isFull;
}

bool Elevator::IsEmpty()
{
	bool isEmpty = false;
	if (queue.GetSize() == 0)
	{
		isEmpty = true;
	}
	return isEmpty;
}

bool Elevator::HasPassengers()
{
	bool hasPass = false;
	if (queue.GetSize() > 0 && queue.GetSize() < 16)
	{
		hasPass = true;
	}
	return hasPass;
}

int Elevator::GetTime()
{
	return elevatorTime;
}

int Elevator::GetAvailTime()
{
	return elevatorAvailbleTime;
}

void Elevator::SetTime(int elevTime)
{
	elevatorTime = elevTime;
}

void Elevator::SetAvailTime(int availTime)
{
	elevatorAvailbleTime = availTime;
}

bool Elevator::IsAvailibe()
{
	bool isAvail = false;
	if ((elevatorTime >= elevatorAvailbleTime) && !IsFull())
	{
		isAvail = true;
	}
	return isAvail;
}

Elevator::Elevator()
{
	queue.MakeEmpty();
	elevatorTime = 0;
	elevatorAvailbleTime = 0;
	isAvailibe = true;
}

Elevator::~Elevator()
{
}
