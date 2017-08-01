/*
   main.cpp

   Test application for the DOM parser class.

   ------------------------------------------

   Copyright (c) 2013 Vic Hargrave

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "XmlDomDocument.h"

int main(int argc, char** argv)
{
    string value;
    //XmlDomDocument* doc = new XmlDomDocument("./bookstore.xml");
    XmlDomDocument* doc = new XmlDomDocument(argv[1]);
    if (doc) {
        for (int i = 0; i < doc->getChildCount("setup", 0, "rpc"); i++) {
        	std::cout<<"********************************************************" << std::endl;
        	std::cout<<"Child No : " << (i+1) << std::endl;
        	value = doc->getChildAttribute("setup", 0, "rpc", i, "id");
        	std::cout<<"Id : " << value << std::endl;
        	value = doc->getChildValue("rpc",i,"gaps",0);
        	std::cout<<"Number of Gaps : " << value << std::endl;
        	value = doc->getChildValue("rpc",i,"roplanes",0);
        	std::cout<<"Number of RO-Planes : " << value << std::endl;

        	std::cout<<"RPC index : " << i << std::endl;
        	for (int j = 0; j < doc->getChildCount("rpc", i, "plane"); j++) {
        	value = doc->getChildAttribute("rpc", i, "plane", j, "id");
        	std::cout<<"------------------------------------------------------" << std::endl;
        	std::cout<<"Plane ID : " << value << std::endl;

        	//std::cout<<"TDC count in Plane : " << j <<" : " << doc->getChildCount("plane", j, "tdc") << std::endl;
        	value = doc->getChildAttribute("rpc",i,"plane",j,"tdcid");
        	std::cout<<"TDC ID : " << value << std::endl;
        	value = doc->getChildAttribute("rpc",i,"plane",j,"tdcchannelStart");
        	std::cout<<"Channel Start Index : " << value << std::endl;
        	value = doc->getChildAttribute("rpc",i,"plane",j,"tdcchannelEnd");
        	std::cout<<"Channel End Index : " << value << std::endl;


        	}

        }
        delete doc;
    }

    exit(0);
 }
