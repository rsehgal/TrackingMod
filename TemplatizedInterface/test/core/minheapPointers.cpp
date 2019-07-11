#include "heap.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>

#include <stack> 
#include <ctime>
#include <unistd.h>

using namespace std;
//std::ifstream sfile("shacode.txt");
FILE *fp;



struct Node{
         Node *leftChild;
         Node *rightChild;
         string data;
         Node() 
         {}
         Node (string data)
         {
           this->data = data;
           leftChild = rightChild = NULL;
         }
};       



void printPreorder(struct Node* node) 
{    
    
       if (node == NULL) 
           return; 
 
    	cout << node->data << " "<<endl;               		         /* first print data of node */
    	printPreorder(node->leftChild);  				 /* then recur on left sutree */
    	printPreorder(node->rightChild); 				 /* now recur on right subtree */
    

}  
    

bool insertHeap(Node **heap, string data, short index)
    {
        Node *newNode, *temp, *parent;
        string tempData;
        short mask = 0x80;

    try{
        newNode = new Node;
       }
    catch (bad_alloc& e) 
    {
        cerr << "Memory allocation error: " << e.what() << endl;
        return false;
    }

    //while (!(index & mask)) index <<= 1;

    index <<= 1;
    parent = NULL;
    temp = *heap;

    while (temp)
     {
        if (data > temp->data)
	{
            tempData = temp->data;
            temp->data = data;
            data = tempData;
        }       
        parent = temp;

        if (!(index & mask)) temp = temp->leftChild; 
        else temp = temp->rightChild;

        if (temp) index <<= 1;
    }

    newNode->leftChild = NULL;
    newNode->rightChild = NULL;
    newNode->data = data;

    if(!parent)
	{
        	*heap = newNode;
        	return true;
   	 }

    if (!(index & mask)) 
          parent->leftChild = newNode;
    else parent->rightChild = newNode;

    return true;
}




int main()
{

   int i;
   std::string line; 
   Node *heap = NULL;
   char my_character ;
   int count = 0;
   std::ifstream sfile("shacode.txt");
  
   std::vector<std::string> arr;

   if(sfile)
	{

  		while(std::getline(sfile,line))
  		{       count++;
     			if(line.size() > 0)
			{
			    arr.push_back(line);
                       
			}
			if(!(count%1000))
				cout << "Events processed : " << count << std::endl;
  		}
	}


cout << "NUMBER OF LINES: " <<count<< endl;
sfile.close();

//std::cout <<"OPening the file for Heap insertion................." << std::endl;
//sfile.open("shacode.txt");
//std::cout <<"File opened successfully................." << std::endl;
/*   
sfile.clear();
sfile.seekg (0, ios::beg);
*/

std::cout <<"Array size  : " << arr.size() << std::endl;
for (int i = 0; i < arr.size(); i++) 
{
            //std::cout << arr[i] << std::endl;
            insertHeap(&heap,arr[i],i+1); 
	    if(!(i%50000))
		cout << "Events Inserted : " << i << std::endl;
	    //std::cout << "Root Data : " << heap->data << std::endl;
}

std::cout <<"Heap is ready........................"<<std::endl;
sleep(10);
std::cout <<"Releasing the vector memory.................."<< std::endl;
arr.clear();
sleep(10);
std::cout <<"Exiting................" <<std::endl;
/*
cout <<"================== ROOT of final tree ====================" << endl;
std::cout << "Root Data : " << heap->data << std::endl;

cout << "\n-------------------Inorder traversal of binary tree is---------------------- " << endl; 
printPreorder(heap);

*/

 
//std::cout <<"Closing the file after heap insertion.................."<< std::endl;
//sfile.close();
//std::cout <<"File closed............."<< std::endl;
return 0;
}

  























  




















































































