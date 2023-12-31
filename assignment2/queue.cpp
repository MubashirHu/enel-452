/**
   Project: Implementation of a Queue in C++.
   Programmer: Mubashir Hussain
   Course: Enel452
*/

#include "queue.h"
#include <iostream>
#include <cstdlib>              // for exit

using namespace std;

const char* error_messages[]= {
    "",
    "error code 1: insert: range error."
};

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

void Queue::insert(Data d, unsigned position)
{
    if((position > size()))
        {
            errno = ERR_INSERT_RANGE;
            errorHandle(errno);
            return;
        }

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
    else
    {
        QElement* el = new QElement(d);
        QElement* current = head; 
        unsigned int i = 0;

        if (size() > 0)
        {
            while (current->next != NULL)
            {
                i++;
                if (i == position)
                {
                    el->next = current->next;
                    current->next = el;
                    break; 
                }
                current = current->next;
            }
        }
        else
        {
            head = el;
            tail = el;
        }
        nelements++;
    }
}

void Queue::errorHandle(int errorcode)
{
    switch (errorcode){
        case ERR_INSERT_RANGE:
            fprintf(stderr, "%s\n", error_messages[ERR_INSERT_RANGE]);
            errno = 0;
            exit(3);
            break;

        default:
            errno = 0;
            break;
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