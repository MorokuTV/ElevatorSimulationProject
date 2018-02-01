#include <queue>
#pragma once

class PriorityQue
{
private:
	static const int numPriorities = 4;
	std::queue<int> queues[numPriorities];
	int size;

public:
	PriorityQue();
	~PriorityQue();
	void MakeEmpty();
	bool IsEmpty();
	bool IsFull();
	void Enqueue(int item, int priority);
	void Dequeue(int & item, int & priority);
	int GetSize();
};
