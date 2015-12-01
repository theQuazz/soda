#include "nameserver.h"
#include "vendingmachine.h"

NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents ) :
    printer( prt ),
    machines( new VendingMachine*[numVendingMachines] ),
    numVendingMachines( numVendingMachines ),
    numStudents( numStudents ),
    studentMachines( new unsigned int[numStudents] )
{
    // Set up machine assignments
    for ( unsigned int i = 0; i < numStudents; i++ ) {
        studentMachines[i] = i % numVendingMachines;
    }
}

NameServer::~NameServer() {
    // Clean up
    delete [] machines;
    delete [] studentMachines;
}

void NameServer::VMregister( VendingMachine *vendingmachine ) {
    machine = vendingmachine;
    // rest is handled in main
}

VendingMachine *NameServer::getMachine( unsigned int _id ) {
    id = _id;
    return machines[studentMachines[id]];
}

VendingMachine **NameServer::getMachineList() {
    return machines;
}

void NameServer::main() {
    printer.print( Printer::NameServer, 'S' );

    for ( unsigned int i = 0; i < numVendingMachines; i++ ) {
        _Accept( VMregister ) {
            // add machine to array
            unsigned int id = machine->getId();
            printer.print( Printer::NameServer, 'R', id );
            machines[id] = machine;
        }
    }

    // Main loop
    for ( ;; ) {
        _Accept( ~NameServer ) {
            printer.print( Printer::NameServer, 'F' );
            // end task
            break;
        }
        or _Accept( getMachine ) {
            printer.print( Printer::NameServer, 'N', id, studentMachines[id] );
            // update student machine index
            studentMachines[id] = ( studentMachines[id] + 1 ) % numVendingMachines;
            // pass back original machine
        }
        or _Accept( getMachineList ) {}
    }
}
