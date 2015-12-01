#include "parent.h"

Parent::Parent(
        Printer &prt,
        Bank &bank,
        unsigned int numStudents,
        unsigned int parentalDelay ) :
    printer( prt ),
    bank( bank ),
    numStudents( numStudents ),
    delay( parentalDelay ) {}

void Parent::main() {
    printer.print( Printer::Parent, 'S' );

    // Loop until destroyed externally
    for ( ;; ) {
        yield( delay );

        // Choose student to give money to
        unsigned int student = get_random()( 0, numStudents - 1 );
        // Choose amount of money to give
        unsigned int amount = get_random()( MIN_DEPOSIT, MAX_DEPOSIT );
        // Make deposit to bank
        bank.deposit( student, amount );
        printer.print( Printer::Parent, 'D', student, amount );

        // Allow destructor to proceed & break out of main loop to end task
        _Accept( ~Parent ) {
            printer.print( Printer::Parent, 'F' );
            break;
        }
        _Else {}
    }
}
