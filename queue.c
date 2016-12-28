#include "queue.h"
#include <stdio.h>


void insertElement(struct_queue_t * queue,int data)
{
	if (!isFull(queue))
	{

		queue->empty = data;
		

		if(queue->rear == MAX- 1)
		{
			queue-> rear = -1;
		}

		queue->intArray[++(queue->rear)] = data;
		queue->count++;
	}


}

int removeElement(struct_queue_t * queue)
{
	int data;
	if(isEmpty(queue))
		return queue->empty;

	data = queue->intArray[(queue->front)++];
	if(queue->front == MAX)
		queue->front = 0;

	queue->count --;
	return data;
}

int isFull(struct_queue_t * queue)
{
	return queue->count == MAX;
}

int isEmpty(struct_queue_t * queue)
{
	return queue->count == 0;
}

int peek(struct_queue_t * queue)
{
	if(isEmpty(queue))
		return queue->empty;
	return queue->intArray[queue->front];
}