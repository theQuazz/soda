#ifndef __NAMESERVER_H__
#define __NAMESERVER_H__

#include "printer.h"

_Task VendingMachine;

_Task NameServer {
    Printer &printer;
    VendingMachine **machines;
    unsigned int numVendingMachines, numStudents, *studentMachines;
    VendingMachine *machine;
    unsigned int id;
    uCondition bench;

    void main();
  public:
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
};

#endif
