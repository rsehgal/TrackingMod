/*
 * Files.h
 *
 *  Created on: July 15, 2018
 *      Author: rsehgal
 */

#ifndef HELPERS_INC_FILES_H_
#define HELPERS_INC_FILES_H_

#include <fstream>
#include <map>
#include <iterator>
#include <iostream>
 #include <cstdarg>
namespace Tomography{

enum operation {read, write, append, close};

class FStream : public std::ifstream, public std::ofstream {
    public:
        int mode;

};

/* A Singleton class that will to open any file anywhere
** and write to that file anytime, and close anywhere anytime
*/
class Files
{
    private:
        /* Here will be the instance stored. */
        static Files* fInstance;
        std::map<std::string,FStream*>  fFileMap;
        //std::map<std::string,std::fstream*>  fFileMap;



        /* Private constructor to prevent instancing. */
        Files(){}

    public:
        /* Static access method. */
        static Files* instance();
        /* Function to open the file, 
        ** mod = 0 , 1 , 2
        ** 0 -> read
        ** 1 -> write
        ** 2 -> append
        */
        void Open(std::string filename, int mode){
            FStream *streamObj;

            /* if file is open in memory then just return and delete it from map
            ** else open the file in desired mode and return
            */
            for (std::map<std::string,FStream*>::iterator it=fFileMap.begin(); it!=fFileMap.end(); ++it){
                if(it->first == filename){
                    return;
                }
            }

            if(mode == operation::read){
                streamObj = (FStream*)(new std::ifstream(filename));
                fFileMap.insert(std::pair<std::string,FStream*>(filename,streamObj));

            }else if(mode == operation::write){
                streamObj = static_cast<FStream*>(new std::ofstream(filename));
                fFileMap.insert(std::pair<std::string,FStream*>(filename,streamObj));

            }else if(mode == operation::append){
                //fFileMap.insert(std::pair<std::string,FStream*>(filename,new std::ofstream(filename,std::ios::app)));
            }
            streamObj->mode = mode;
        }

        
        //Closes all the registered open files
        void Close(){

            for (std::map<std::string,FStream*>::iterator it=fFileMap.begin(); it!=fFileMap.end(); ++it){
                //std::cout << it->first << " => " << it->second << '\n';

                if(it->second->mode == operation::read)
                    static_cast<std::ifstream*>(it->second)->close();
                else
                    static_cast<std::ofstream*>(it->second)->close();

                it->second->mode = operation::close;
            }

        }

        //Closes specified file
        void Close(std::string filename){

            for (std::map<std::string,FStream*>::iterator it=fFileMap.begin(); it!=fFileMap.end(); ++it){
                //std::cout << it->first << " => " << it->second << '\n';
                if(it->first == filename){
                    
                    if(it->second->mode == operation::read)
                        static_cast<std::ifstream*>(it->second)->close();
                    else
                        static_cast<std::ofstream*>(it->second)->close();
                    
                    it->second->mode = operation::close;
                    fFileMap.erase(it->first);

                    break;
                }
            }

        }

        //Writing specific string to file
        void Write(std::string filename, std::string data){
            for (std::map<std::string,FStream*>::iterator it=fFileMap.begin(); it!=fFileMap.end(); ++it){
                if(it->first == filename){
                    if(it->second->mode == operation::read){
                        std::cout << "This file is NOT opened in WRITE mode.." << std::endl;
                    }else{
                        *(static_cast<std::ofstream*>(it->second)) << data << std::endl;
                    }
                }
            }
        }

        //Using Variadic function to write any number of doubles to the file
        void Write(std::string filename, int fmt, ...){
            for (std::map<std::string,FStream*>::iterator it=fFileMap.begin(); it!=fFileMap.end(); ++it){
                if(it->first == filename){
                    if(it->second->mode == operation::read){
                        std::cout << "This file is NOT opened in WRITE mode.." << std::endl;
                    }else{
                            va_list args;
                            va_start(args, fmt);
                            double data;
                            while (fmt) {
                              data = va_arg(args, double);
                              //  std::cout << d << '\n';
                              fmt--;
                              if(fmt)
                                *(static_cast<std::ofstream*>(it->second)) << data << " " ;
                              else
                                *(static_cast<std::ofstream*>(it->second)) << data << std::endl;
                            }
                            va_end(args);
                        
                    }
                }
            }
        }


        void NumOfOpenedFiles(){std::cout << "Num of Opened files : " <<  fFileMap.size() << std::endl; }
};

/* Null, because instance will be initialized on demand. */
/*
Files* Files::fInstance = 0;

Files* Files::instance()
{
    if (fInstance == 0)
    {
        fInstance = new Files();
    }

    return fInstance;
}
*/

}//end of Tomography namespace

//Tester Program for above logic
/*int main(){

    Tomography::Files::instance()->Open("test.txt",Tomography::operation::write);
    Tomography::Files::instance()->Open("test2.txt",Tomography::operation::write);
    
    Tomography::Files::instance()->Write("test.txt","Hello Raman");
    
    Tomography::Files::instance()->NumOfOpenedFiles();
    Tomography::Files::instance()->Close("test.txt");

    Tomography::Files::instance()->NumOfOpenedFiles();

    Tomography::Files::instance()->Write("test2.txt","Hello Raman fdfsdf");
    Tomography::Files::instance()->Close("test2.txt");

    Tomography::Files::instance()->NumOfOpenedFiles();
    //Tomography::Files::instance()->Close();


    return 0;
}*/



#endif


