#include "nameserver.h"
#include "vendingmachine.h"

NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents ) :
    printer( prt ),
    machines( new VendingMachine*[numVendingMachines] ),
    numStudents( numStudents ) {}

NameServer::~NameServer() {
    delete [] machines;
}

void NameServer::VMregister( VendingMachine *vendingmachine ) {
    machines[vendingmachine->getId()] = vendingmachine;
}

VendingMachine *NameServer::getMachine( unsigned int id ) {
    return NULL;
}

VendingMachine **NameServer::getMachineList() {
    return NULL;
}

void NameServer::main() {
}
