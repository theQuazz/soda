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
    // Choose random number of purchases to make
    unsigned int numPurchasesLeft = get_random()( 1, maxPurchases );
    // Choose favourite flavour
    VendingMachine::Flavours favourite = static_cast<VendingMachine::Flavours>(
            get_random()( 0, VendingMachine::NUM_FLAVOURS - 1 )
    );

    printer.print( Printer::Student, id, 'S', favourite, numPurchasesLeft );

    // Create watcard & giftcard
    WATCard::FWATCard watcard = cardOffice.create( id, 5 );
    WATCard::FWATCard giftcard = groupoff.giftCard();

    // Get vending machine
    VendingMachine *machine = nameServer.getMachine( id );
    printer.print( Printer::Student, id, 'V', machine->getId() );

    // For each purchase to make...
    for ( ; numPurchasesLeft > 0; ) {
        unsigned int delay = get_random()( 1, 10 );
        yield( delay );

        // Loop until watcard not lost
        for ( ;; ) {
            try {
                _Select( watcard ) {
                    // Got watcard
                    machine->buy( favourite, *watcard );
                    printer.print( Printer::Student, id, 'B', watcard()->getBalance() );
                }
                or _Select( giftcard ) {
                    // Got giftcard
                    machine->buy( favourite, *giftcard );
                    printer.print( Printer::Student, id, 'G', giftcard()->getBalance() );
                    giftcard.reset();
                }
                numPurchasesLeft -= 1;
                break;
            }
            catch ( WATCardOffice::Lost ) {
                // Watcard office lost, get a new one and loop
                printer.print( Printer::Student, id, 'L' );
                watcard = cardOffice.create( id, 5 );
            }
            catch ( VendingMachine::Funds ) {
                // Out of funds, transfer more funds to watcard
                watcard = cardOffice.transfer( id, machine->cost() + 5, watcard );
                break;
            }
            catch ( VendingMachine::Stock ) {
                // Out of stock, get new machine from nameserver
                machine = nameServer.getMachine( id );
                printer.print( Printer::Student, id, 'V', machine->getId() );
                break;
            }
        }
    }

    if ( watcard.available() ) {
        // Watcard isn't lost, free memory
        try {
            delete static_cast<WATCard*>( watcard );
        } 
        catch ( WATCardOffice::Lost ) {
            printer.print( Printer::Student, id, 'L' );
        }
    } else {
        // Tell courrier that we don't need to replace our lost watcard
        watcard.cancel();
    }

    printer.print( Printer::Student, id, 'F' );
}
