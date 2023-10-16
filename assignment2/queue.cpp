/**
   Project: Implementation of a Queue in C++.
   Programmer: Karim Naqvi
   Course: enel452
*/

#include "queue.h"
#include <iostream>
#include <cstdlib>              // for exit

using namespace std;

Queue::Queue()
{
    head = 0;
    tail = 0;
    nelements = 0;
    verbose = false;
}

Queue::~Queue()
{
    for (QElement* qe = head; qe != 0;)
    {
	QElement* temp = qe;
	qe = qe->next;
	delete(temp);
    }
}

void Queue::remove(Data* d)
{
    if (size() > 0)
    {
        QElement* qe = head;
        head = head->next;
        nelements--;
        *d = qe->data;
	delete qe;
    }
    else
    {
        cerr << "Error: Queue is empty.\n";
        exit(1);
    }
}

void Queue::insert(Data d)
{
    if (verbose) std::cout << "insert(d)\n";
    QElement* el = new QElement(d);
    if (size() > 0)
    {
        tail->next = el;
    }
    else
    {
        head = el;
    }
    tail = el;
    nelements++;
}

/** 
     Insert a new element at a specific position of the queue 
     \post Data element will be inserted IN FRONT of the given position
     If the position is 0,               data element will be inserted at the front of the queue(the head)
     If the position is set to size(),   data element will be appended to the tail
     If the position is set to size()-1, data element will insert just in front of the tail
    */
void Queue::insert(Data d, unsigned position)
{
    if(position == 0)
    {
        QElement* el = new QElement(d);
        if(size() > 0)
        {
            el->next = head;
            head = el;
        }
        else
        {
            head = el;
            tail = el;
        }
        nelements++;
    }
    else if(position == size())
    {
        insert(d);
    }
    else if(position == (size()-1))
    {
        QElement* el = new QElement(d);
        QElement* current;
        if(size() > 0)
        {
            current = head;
            while(current->next != tail)
            {
                current = current->next;
            }

            //current has reached right before the tail
            el->next = current->next;
            current->next = el;
        }
        else
        {
            head = el;
            tail = el;
        }
        nelements++;
    }
    else if(position > 0 && (position < (size()-1)))
    {
        QElement* el = new QElement(d);
        QElement* current = head; // Initialize 'current' to the head of the list
        unsigned int i = 0;

        if (size() > 0)
        {
            while (current->next != NULL && i < (position - 1))
            {
                if (i == position)
                {
                    el->next = current->next;
                    current->next = el;
                    break; // No need to continue iterating
                }
                current = current->next;
                i++;
            }
        }
        else
        {
            head = el;
            tail = el;
        }
    }
}

bool Queue::search(Data otherData) const
{
    QElement* insideEl = head;
    for (int i = 0; i < nelements; i++)
    {
        if (insideEl->data.equals(otherData))
            return true;
        insideEl = insideEl->next;
    }
    return false;
}

void Queue::print() const
{
    QElement* qe = head;
    if (size() > 0)
    {
        for (unsigned i = 0; i < size(); i++)
        {
            cout << i << ":(" << qe->data.x << "," << qe->data.y << ") ";
            qe = qe->next;
        }
    }
    cout << "\n";
}

unsigned Queue::size() const
{
    return nelements;
}
