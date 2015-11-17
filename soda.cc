#include <unistd.h>
#include <iostream>

#include "config.h"
#include "printer.h"
#include "bank.h"
#include "parent.h"
#include "watcardoffice.h"
#include "groupoff.h"
#include "nameserver.h"
#include "vendingmachine.h"
#include "bottlingplant.h"
#include "student.h"

int stoi_with_min( const char *argv, const int min ) {
    size_t pos = 0;
    int value = std::stoi( argv, &pos );
    if ( value < min )
        throw std::invalid_argument( "Invalid param ( <= 0 )" );
    if ( pos != strlen( argv ) )
        throw std::invalid_argument( "Invalid param ( non-numeric )" );
    return value;
}

void uMain::main() {
    int seed = getpid();
    char *configFile = "soda.config";
    ConfigParams config;

    try {
        switch ( argc ) {
            default: throw std::invalid_argument( "argc must be less than 5" );
            case 3: seed = stoi_with_min( argv[2], 1 );
            case 2: configFile = argv[1];
            case 1: break;
        }

        processConfigFile( configFile, config );
    }
    catch ( std::logic_error &e ) {
        std::cerr << e.what()
                  << std::endl
                  << "Usage: "
                  << argv[0]
                  << "  "
                  << "[ config-file [ Seed ( > 0 ) ] ]"
                  << std::endl;

        uRetCode = 1;

        return;
    }
    catch ( uFile::Failure ) {
        std::cerr << "Unable to open file "
                  << configFile
                  << ", exiting..."
                  << std::endl;

        uRetCode = 2;

        return;
    }

     /* It then creates in order the printer, bank, parent, WATCard office,
      * groupoff, name server, vending machines, bottling plant, and students.
      * The truck is created by the bottling plant; the couriers are created by
      * the WATCard office.
      */
    Printer printer(
        config.numStudents,
        config.numVendingMachines,
        config.numCouriers
    );

    Bank bank( config.numStudents );
    Parent parent( printer, bank, config.numStudents, config.parentalDelay );
    WATCardOffice office( printer, bank, config.numCouriers );

    Groupoff groupoff(
        config.numStudents,
        config.sodaCost,
        config.groupoffDelay
    );

    NameServer nameServer(
        printer,
        config.numVendingMachines,
        config.numStudents
    );

    VendingMaching *vendingMachines[config.numVendingMachines];

    for ( int id = 0; id < config.numVendingMachines; id++ ) {
        vendingMachine[id] = new VendingMachine(
            printer,
            nameServer,
            id,
            config.sodaCost,
            config.maxStockPerFlavour
        );
    }

    {
        BottlingPlant bottlingPlant(
            printer,
            nameserver,
            config.numVendingMachines,
            config.maxShippedPerFlavour,
            config.maxStockPerFlavour,
            config.timeBetweenShipments
        );

        Student *students[config.numStudents];

        for ( int id = 0; id < config.numStudents; id++ ) {
            students[id] = new Student(
                printer,
                nameServer,
                office,
                groupoff,
                id,
                config.maxPurchases
            );
        }

        /*
        * Delete the bottling plant before deleting the vending machines to allow
        * the truck to complete its final deliveries to the vending machines;
        * otherwise, a deadlock can occur
        */

        for ( int id = 0; id < config.numStudents; id++ ) {
            delete students[id];
        }
    }

    for ( int id = 0; id < config.numVendingMachines; id++ ) {
        delete vendingMachines[id];
    }
}
