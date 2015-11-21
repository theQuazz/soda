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
    unsigned int numPurchasesLeft = get_random()( 1, maxPurchases - 1 );
    VendingMachine::Flavours favourite = static_cast<VendingMachine::Flavours>(
            get_random()( 0, 3 )
    );

    printer.print( Printer::Student, id, 'S', favourite, numPurchasesLeft );

    WATCard::FWATCard watcard = cardOffice.create( id, 5 );
    WATCard::FWATCard giftcard = groupoff.giftCard();

    VendingMachine *machine = nameServer.getMachine( id );
    printer.print( Printer::Student, id, 'V', machine->getId() );

    for ( ; numPurchasesLeft > 0; ) {
        unsigned int delay = get_random()( 1, 10 );
        yield( delay );

        for ( ;; ) {
            try {
                _Select( watcard ) {
                    machine->buy( favourite, *watcard );
                }
                or _Select( giftcard ) {
                    machine->buy( favourite, *giftcard );
                    printer.print( Printer::Student, id, 'G', giftcard()->getBalance() );
                    giftcard.reset();
                }
                printer.print( Printer::Student, id, 'B', watcard()->getBalance() );
                numPurchasesLeft -= 1;
                break;
            }
            catch ( WATCardOffice::Lost ) {
                printer.print( Printer::Student, id, 'L' );
                watcard = cardOffice.create( id, 5 );
            }
            catch ( VendingMachine::Funds ) {
                watcard = cardOffice.transfer( id, machine->cost() + 5, watcard );
                break;
            }
            catch ( VendingMachine::Stock ) {
                machine = nameServer.getMachine( id );
                printer.print( Printer::Student, id, 'V', machine->getId() );
                break;
            }
        }
    }

    printer.print( Printer::Student, id, 'F' );
}
