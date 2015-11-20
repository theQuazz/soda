#include "student.h"

Student::Student(
        Printer &prt,
        NameServer &nameServer,
        WATCardOffice &cardOffice,
        Groupoff &groupoff,
        unsigned int id,
        unsigned int maxPurchases ) :
    printer( prt ),
    nameServer( nameServer ),
    cardOffice( cardOffice ),
    groupoff( groupoff ),
    id( id ),
    maxPurchases( maxPurchases ) {}

void Student::main() {
}
