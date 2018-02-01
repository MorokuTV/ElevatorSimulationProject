#include "PriorityQue.h"
#pragma once

PriorityQue::PriorityQue()
{
	for (int i = 0; i < numPriorities; i++)
	{
		while(!queues[i].empty()) queues[i].pop();
	}
	size = 0;
}

PriorityQue::~PriorityQue()
{
}

void PriorityQue::MakeEmpty()
{
	for (int i = 0; i < numPriorities; i++)
	{
		while (!queues[i].empty()) queues[i].pop();
	}
	size = 0;
}

bool PriorityQue::IsEmpty()
{
	bool isEmpty = false;
	if (GetSize() == 0)
	{
		isEmpty = true;
	}
	return isEmpty;
}

bool PriorityQue::IsFull()
{
	bool isFull = false;
	if (GetSize() == 15)
	{
		isFull = true;
	}
	return isFull;
}

void PriorityQue::Enqueue(int item, int priority)
{
	queues[priority].push(item);
	GetSize();
}

void PriorityQue::Dequeue(int & item, int & priority)
{
	if (!queues[0].empty())
	{
		item = queues[0].front();
		queues[0].pop();
		priority = 0;
	}
	else if (!queues[1].empty())
	{
		item = queues[1].front();
		queues[1].pop();
		priority = 1;
	}
	else if (!queues[2].empty())
	{
		item = queues[2].front();
		queues[2].pop();
		priority = 2;
	}
	else if (!queues[3].empty())
	{
		item = queues[3].front();
		queues[3].pop();
		priority = 3;
	}
	else
	{
		item == NULL;
		priority = NULL;
	}
	GetSize();
}

int PriorityQue::GetSize()
{
	size = 0;
	for (int i = 0; i < numPriorities; i++)
	{
		size += queues[i].size();
	}
	return size;
}
