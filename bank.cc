#include "bank.h"

Bank::Bank( unsigned int numStudents ) :
  numStudents( numStudents ),
  accounts( new Account[numStudents] )
{
    for ( unsigned int i = 0; i < numStudents; i++ ) {
        accounts[i].amount = 0;
    }
}

Bank::~Bank() {
    delete [] accounts;
}

void Bank::deposit( unsigned int id, unsigned int amount ) {
    Account &account = accounts[id];

    account.amount += amount;

    if ( ! account.bench.empty() && account.amount >= account.withdrawlIntent )
        account.bench.signal();
}

void Bank::withdraw( unsigned int id, unsigned int amount ) {
    Account &account = accounts[id];

    if ( account.amount < amount ) {
        account.withdrawlIntent = amount;
        account.bench.wait();
    }

    account.amount -= amount;
}
