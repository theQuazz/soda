#ifndef __PRINTER_H__
#define __PRINTER_H__

#include <iostream>

_Monitor Printer {
  public:
    enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };

  private:
    const unsigned int numStudents, numVendingMachines, numCouriers, numTotal;

    struct BufferElement {
        bool used;
        Kind kind;
        char state;
        int value1, value2;
    } *buffer;

    void finished( Kind kind, unsigned int lid );
    void flush();

  public:
    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    ~Printer();
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );
};

#endif
