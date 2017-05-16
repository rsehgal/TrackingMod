//test2.cc
#include <iostream>
#include <unistd.h>
//#include "BSCDataReader.h"
//#include "BSCModuleV1721.h"
#include "ModuleV2718.h"
#include "ModuleV1190.h"
#include "BSCDataReader.h"
#include <time.h>

int main(int argv, char ** argc){
        //BSCDataReader Reader;
        //Reader.Initialize( argc[ 1 ] );      
        //Reader.SetNumberOfSpills( 10 );
        //Reader.Run();
        BSCDataReader dr;
        std::cout << "Initializing" << std::endl;
        dr.Initialize( "rpc.ini" );
//std::cout << "sleeping "<< std::endl;
//sleep(20);
//std::cout << " wake up "<< std::endl;
        std::cout << "Running... " << std::endl;
        FILE *stream;

        // Writing start
        time_t          now, now2;
        time(&now);

        dr.Run();

        time(&now2);

        // Writing runtime
        now2 = now2 - now;                              
        if( (stream = fopen( "/home/user/re4data/TDC/runtime", "w" )) != NULL ) {
           fprintf (stream, "%d\n",now2);                    
           fclose( stream );
        }

        /*ModuleV2718 * VmeInt = new ModuleV2718();
        VmeInt->SetBaseAddress( 0 );
        VmeInt->SetBoardId( 0 );
        VmeInt->SetVmeLink( 0 );
        VmeInt->Initialize();
        std::cout << "TDC:" << std::endl;
        ModuleV1190 Tdc1;
        ModuleV1190 Tdc2;
        ModuleV1190 Tdc3;
       
        Tdc1.SetVmeInterface( VmeInt );
        Tdc2.SetVmeInterface( VmeInt );
        Tdc3.SetVmeInterface( VmeInt );

        Tdc1.SetBaseAddress( 0xEE000000 );
        Tdc2.SetBaseAddress( 0x11000000 );
        Tdc3.SetBaseAddress( 0x22000000 );

        if ( !Tdc1.Initialize() ) exit( -1 );
        if ( !Tdc2.Initialize() ) exit( -1 );
        if ( !Tdc3.Initialize() ) exit( -1 );
       
        sleep( 1 );

        std::cout << "Reading!" << std::endl;
        Tdc1.Read();
        Tdc2.Read();
        Tdc3.Read();
        std::cout << "Done!" << std::endl;
        */
}
