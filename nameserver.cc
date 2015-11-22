#include "nameserver.h"
#include "vendingmachine.h"

NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents ) :
    printer( prt ),
    machines( new VendingMachine*[numVendingMachines] ),
    numVendingMachines( numVendingMachines ),
    numStudents( numStudents ),
    studentMachines( new unsigned int[numStudents] )
{
    for ( unsigned int i = 0; i < numStudents; i++ ) {
        studentMachines[i] = i % numVendingMachines;
    }
}

NameServer::~NameServer() {
    delete [] machines;
    delete [] studentMachines;
}

void NameServer::VMregister( VendingMachine *vendingmachine ) {
    unsigned int id = vendingmachine->getId();
    printer.print( Printer::NameServer, 'R', id );
    machines[id] = vendingmachine;
}

VendingMachine *NameServer::getMachine( unsigned int id ) {
    unsigned int orig = studentMachines[id];
    printer.print( Printer::NameServer, 'N', id, orig );
    studentMachines[id] = ( studentMachines[id] + 1 ) % numVendingMachines;
    return machines[orig];
}

VendingMachine **NameServer::getMachineList() {
    return machines;
}

void NameServer::main() {
    printer.print( Printer::NameServer, 'S' );

    for ( unsigned int i = 0; i < numVendingMachines; i++ ) {
        _Accept( VMregister );
    }

    for ( ;; ) {
        _Accept( ~NameServer ) {
            printer.print( Printer::NameServer, 'F' );
            break;
        }
        or _Accept( getMachine ) {}
        or _Accept( getMachineList ) {}
    }
}
