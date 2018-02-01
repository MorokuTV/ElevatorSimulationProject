#include "PriorityQue.h"
#include <queue>
#include <algorithm>
#pragma once

class Elevator
{
public:
	void Enque(int passNum, int destFloor);
	void Deque(int & passNum, int & destFloor);
	bool IsFull();
	bool IsEmpty();
	bool HasPassengers();
	int GetTime();
	int GetAvailTime();
	void SetTime(int elevTime);
	void SetAvailTime(int availTime);
	bool IsAvailibe();

	Elevator();
	~Elevator();


private:
	int elevatorTime;
	int elevatorAvailbleTime;
	PriorityQue queue;
	bool isAvailibe;
};
