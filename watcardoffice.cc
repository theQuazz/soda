#include "watcardoffice.h"

#include <iostream>

WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) :
    numCouriers( numCouriers ),
    printer( prt ),
    bank( bank ),
    couriers( new Courier*[numCouriers] )
{
    // Create couriers & array
    for ( unsigned int i = 0; i < numCouriers; i++ ) {
        couriers[i] = new Courier( printer, *this, bank, i );
    }
}

WATCardOffice::~WATCardOffice() {
    // Delete couriers & array
    for ( unsigned int i = 0; i < numCouriers; i++ ) {
        delete couriers[i];
    }
    delete [] couriers;
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    // Call transfer without a card will create it for us
    return transfer( sid, amount, NULL );
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
    // Push job for courrier
    Job *job = new Job( { sid, amount, card } );
    jobs.push_back( job );
    return job->result;
}

WATCardOffice::Job *WATCardOffice::requestWork() {
    Job *job = jobs.front();
    jobs.pop_front();
    return job;
}

WATCardOffice::Courier::Courier( Printer &printer, WATCardOffice &office, Bank &bank, unsigned int id ) :
    printer( printer ),
    office( office ),
    bank( bank ),
    id( id ) {}

void WATCardOffice::Courier::main() {
    printer.print( Printer::Courier, id, 'S' );

    // Loop until no more work
    for ( ;; ) {
        // Wait for work to become available
        Job *job = office.requestWork();
        if ( job == NULL ) {
            // No more work, task finished
            break;
        }

        if ( job->result.cancelled() ) {
          if ( job->args.curr ) {
              delete job->args.curr;
          }
          delete job;
          continue;
        }

        printer.print( Printer::Courier, id, 't', job->args.sid, job->args.amount );

        // Wait for money available in bank account for student
        bank.withdraw( job->args.sid, job->args.amount );

        // Lose watcard with 1 in 6 chance
        if ( get_random()( 1, 6 ) == 1 ) {
            if ( job->args.curr ) {
                delete job->args.curr;
            }
            job->result.exception( new Lost() );
        }
        else {
            if ( ! job->args.curr ) {
                // No card provided, create a new one
                job->args.curr = new WATCard();
            }
            // Deposit & deliver watcard
            job->args.curr->deposit( job->args.amount );
            job->result.delivery( job->args.curr );
        }

        printer.print( Printer::Courier, id, 'T', job->args.sid, job->args.amount );

        // Job complete, delete it
        delete job;
    }

    printer.print( Printer::Courier, id, 'F' );
}

void WATCardOffice::main() {
    printer.print( Printer::WATCardOffice, 'S' );

    // Loop until destructed
    for ( ;; ) {
        _When( ! jobs.empty() ) _Accept( requestWork ) { // pass job to worker
            printer.print( Printer::WATCardOffice, 'W' );
        }
        or _Accept( ~WATCardOffice ) {
            // Clean up
            for ( Job *job : jobs ) {
                delete job;
            }

            // Signal no more jobs to couriers
            for ( unsigned int i = 0; i < numCouriers; i++ ) {
                jobs.push_back( NULL );
                _Accept( requestWork );
            }

            printer.print( Printer::WATCardOffice, 'F' );
            // End loop
            break;
        }
        or _Accept( create ) {
            // Create watcard
            printer.print( Printer::WATCardOffice, 'C', jobs.back()->args.sid, jobs.back()->args.amount );
        }
        or _Accept( transfer ) {
            // Transfer funds
            printer.print( Printer::WATCardOffice, 'T', jobs.back()->args.sid, jobs.back()->args.amount );
        }
    }
}
