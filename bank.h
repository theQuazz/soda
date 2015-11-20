#ifndef __BANK_H__
#define __BANK_H__

_Monitor Bank {
    const unsigned int numStudents;
    struct Account {
        unsigned int amount;
        unsigned int withdrawlIntent;
        uCondition bench;
    } *accounts;
  public:
    Bank( unsigned int numStudents );
    ~Bank();
    void deposit( unsigned int id, unsigned int amount );
    void withdraw( unsigned int id, unsigned int amount );
};

#endif
