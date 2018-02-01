#include "QueType.h"

QueType::QueType(int max)
{
	maxQue = max + 1;
	front = maxQue - 1;
	rear = maxQue - 1;
	items = new int[maxQue];
	std::fill_n(items, maxQue, NULL);
}

QueType::QueType()
{
	maxQue = 501;
	front = maxQue - 1;
	rear = maxQue - 1;
	items = new int[maxQue];
	std::fill_n(items, maxQue, 0);
}

QueType::~QueType()
{
	delete[] items;
}

void QueType::MakeEmpty()
{
	front = maxQue - 1;
	rear = maxQue - 1;
}

bool QueType::IsEmpty()
{
	return (rear == front);
}

bool QueType::IsFull()
{
	return ((rear +1) % maxQue == front);
}

void QueType::Enqueue(int item)
{
	if (IsFull())
	{
		throw FullQueue();
	}
	else
	{
		rear = findInsertionPoint(item);
		items[rear] = item;
	}

}

void QueType::Dequeue(int & item)
{
	if (IsEmpty())
	{
		throw EmptyQueue();
	}
	else
	{
		front = (front + 1) % maxQue;
		item = items[front];
	}
}

int QueType::findInsertionPoint(int item)
{
	int tempRear;
	tempRear = (rear + 1) % maxQue;
	while(items[tempRear] < item)
	{
		tempRear = (tempRear + 1) % maxQue;
	}
	return tempRear;
}
