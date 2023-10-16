/** -*- c++ -*-
   Project: Implementation of a Queue in C++.
   Course: enel452
   Programmer: Karim Naqvi

   Description: Implements a queue based on a singly-linked list.
   Element data is assumed to be a value type, and must support the equals method.
*/

#ifndef QUEUE_H
#define QUEUE_H

#define ERR_INSERT_RANGE 1

// Error messages
extern const char *error_messages[];

/** Data stores an (x,y) int pair.  This might represent a screen
    coordinate, for example.
 */
struct Data {
    int x;
    int y;
    bool equals(Data other) const { return x==other.x && y==other.y; }
    Data() : x(0), y(0) {}
    Data(int xx, int yy) : x(xx), y(yy) {}
};

/** A holding class for the data and a pointer to the next element.
 */
class QElement {
    friend class Queue;
private:
    Data data;
    QElement* next;

    QElement(Data d) : data(d), next(0) {}
}; 

/**
   Stores a sequence of elements, accessible via the "normal" queue
   discipline (i.e. insertions at the tail, removals from the head.)

   This queue implementation stores elements of type Data.  It is
   required that clients of this class pass a Data element to the
   insert() method.  The data is copied into the queue, i.e. it's
   treated as a value type.
 */
class Queue {
    
private:
    QElement* head;
    QElement* tail;
    bool verbose;               // if true, emit debugging information
    int nelements;              // num elements in queue 
public:
    /** Construct an empty queue. */
    Queue();

    /**
       Destruct a queue.
       \post All elements will be deleted.
     */
    ~Queue();
    
    /** Insert a new element at the tail of the queue. */
    void insert(Data d);

    /** 
     Insert a new element at a specific position of the queue 
     \post Data element will be inserted IN FRONT of the given position
     If the position is 0,               data element will be inserted at the front of the queue(the head)
     If the position is set to size()-1, data element will insert just in front of the tail
     If the position is set to size(),   data element will be appended to the tail
     If the position is a specfic position, data element will be inserted at that point of the linked list
    */
    void insert(Data d, unsigned position);

    /**
    errorHandle(): This function prints the error message to stdout based on the errorcode that is passed in. 
    The errorcode that is passed in will always be the global variable 'errno' unless otherwise stated. After 
    The error message has been sent, the errno global variable is hard reset to 0.

    Parameters : <1> int errorcode - the code from errno
    Return: None
    */
    void errorHandle(int errorcode);

    /**
       Remove the head element of the queue.

       \pre Queue must be non-empty.

       \post The head element is removed from the queue.  The
       data is copied into the data buffer provided.
     */
    void remove(Data* data);

    /**
       Search the queue for an element equal to 'd'.

       \post Returns a true if found.
     */
    bool search(Data d) const;

    /** Print the elements of the queue, from head to tail. */
    void print() const;

    /** Return the number of elements currently in the queue. */
    unsigned size() const;

    /** Enable extra informative messages for debugging purposes. */
    void setVerbose(bool flag)  { verbose = flag; }
};
        
#endif // QUEUE_H
