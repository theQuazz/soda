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

    for ( ;; ) {
        yield( delay );

        unsigned int student = get_random()( 0, numStudents - 1 );
        unsigned int amount = get_random()( MIN_DEPOSIT, MAX_DEPOSIT );
        bank.deposit( student, amount );

        _Accept( ~Parent ) {
            printer.print( Printer::Parent, 'F' );
        }
        _Else {}
    }
}
