#ifndef __PARENT_H__
#define __PARENT_H__

#include "printer.h"
#include "bank.h"
#include "MPRNG.h"

_Task Parent {
    enum {
        MIN_DEPOSIT = 1,
        MAX_DEPOSIT = 3,
    };

    Printer &printer;
    Bank &bank;
    const unsigned int numStudents, delay;

    void main();
  public:
    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
};

#endif
