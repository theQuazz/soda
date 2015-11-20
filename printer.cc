#include "printer.h"

enum {
  SENTINEL = -1
};

Printer::Printer(
        unsigned int numStudents,
        unsigned int numVendingMachines,
        unsigned int numCouriers ) :
    numStudents( numStudents ),
    numVendingMachines( numVendingMachines ),
    numCouriers( numCouriers ),
    numTotal( Student + numStudents + numVendingMachines + numCouriers ),
    buffer( new BufferElement[numTotal] )
{
    for ( unsigned int i = 0; i < numTotal; i++ ) {
        if ( i ) std::cout << '\t';

        if ( i == Parent )
          std::cout << "Parent";
        else if ( i == Groupoff )
          std::cout << "Gropoff";
        else if ( i == WATCardOffice )
          std::cout << "WATOff";
        else if ( i == NameServer )
          std::cout << "Names";
        else if ( i == Truck )
          std::cout << "Truck";
        else if ( i == BottlingPlant )
          std::cout << "Plant";
        else if ( i < Student + numStudents )
          std::cout << "Stud" << i - Student;
        else if ( i < Student + numStudents + numVendingMachines )
          std::cout << "Mach" << i - Student - numStudents;
        else
          std::cout << "Cour" << i - Student - numStudents - numVendingMachines;

        buffer[i].used = false;
    }
    std::cout << std::endl;

    for ( unsigned int i = 0; i < numTotal; i++ ) {
        if ( i ) std::cout << '\t';
        std::cout << "*******";
    }
    std::cout << std::endl;
}

Printer::~Printer() {
    std::cout << "***********************" << std::endl;
    delete [] buffer;
}

void Printer::print( Printer::Kind kind, char state ) {
    print( kind, state, SENTINEL );
}

void Printer::print( Printer::Kind kind, char state, int value1  ) {
    print( kind, state, value1, SENTINEL );
}

void Printer::print( Printer::Kind kind, char state, int value1, int value2 ) {
    print( kind, 0, state, value1, value2 );
}

void Printer::print( Printer::Kind kind, unsigned int lid, char state ) {
    print( kind, lid, state, SENTINEL, SENTINEL );
}

void Printer::print( Printer::Kind kind, unsigned int lid, char state, int value1 ) {
    print( kind, lid, state, value1, SENTINEL );
}

void Printer::print( Printer::Kind kind, unsigned int lid, char state, int value1, int value2 ) {
    BufferElement &el = buffer[kind + lid];

    if ( el.used ) flush();

    el.used = true;
    el.state = state;
    el.value1 = value1;
    el.value2 = value2;

    if ( state == 'F' ) finished( kind, lid );
}

void Printer::flush() {
    std::osacquire out( std::cout );

    for ( unsigned int i = 0; i < numTotal; i++ ) {
        if ( i ) out << '\t';
        if ( !buffer[i].used ) continue;

        buffer[i].used = false;

        out << buffer[i].state;

        if ( buffer[i].value1 != SENTINEL ) {
            out << buffer[i].value1;
        }
        if ( buffer[i].value2 != SENTINEL ) {
            out << ',' << buffer[i].value2;
        }
    }
    out << std::endl;
}

void Printer::finished( Printer::Kind kind, unsigned int lid ) {
    std::osacquire out( std::cout );

    unsigned int id = kind + lid;

    buffer[id].used = false;

    for ( unsigned int i = 0; i < numTotal; i++ ) {
        if ( buffer[i].used ) {
            flush();
            break;
        }
    }

    for ( unsigned int i = 0; i < numTotal; i++ ) {
        if ( i ) std::cout << '\t';

        if ( i == id ) {
            out << buffer[i].state;
        }
        else {
            out << "...";
        }
    }
    out << std::endl;
}
