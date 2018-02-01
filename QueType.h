
#include <string>


#pragma once
class FullQueue
{

};

class EmptyQueue
{

};

class QueType
{
public:
	QueType(int max);
	QueType();
	~QueType();
	void MakeEmpty();
	bool IsEmpty();
	bool IsFull();
	void Enqueue(int item);
	void Dequeue(int & item);

private:
	int * items;
	int maxQue;
	int front;
	int rear;

	int findInsertionPoint(int item);
};
