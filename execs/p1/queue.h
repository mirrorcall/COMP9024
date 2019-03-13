// Integer queue header file

#ifndef QUEUE_H
#define QUEUE_H

void queueInit(); // set up an empty queue
int  qisEmpty(); // check whether the queue is empty
void enqueue(int); // insert int at the end of queue
int  dequeue(); // remove int from the front of queue
void show();

#endif