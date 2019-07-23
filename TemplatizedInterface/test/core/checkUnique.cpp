#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>

#include <stack> 
#include <ctime>
#include <unistd.h>




#include "Track.h"
#include "base/Vector3D.h"
#include "sha256.h"
#include "TrackPredictor.h"

using namespace std ;

int globalCounter=0;

std::vector<int> result;


struct TestStruct{
	string data;
};


struct treeNode
{
    int counter;
    string data;
    treeNode *left;
    treeNode *right;
    TestStruct t;
   
    treeNode(){counter=0;}

    treeNode(string str){
	counter=0;
        data = str;
    	t.data = data;
    }


    //std::vector of string for this node
    std::vector<std::string> strVector;
    

};



////////////////////////////////////////////////////////////////////////////////////////////////////////////




treeNode *Insert(treeNode *node, string data)
{
    if(node==NULL)
    {
        treeNode *temp = new treeNode(data);
  	temp->strVector.push_back(data);
        //temp=new treeNode;
        //temp = (treeNode *)malloc(sizeof(treeNode));
        //temp -> data = data;
        temp -> left = temp -> right = NULL;
        return temp;
    }
    if(data== (node->t.data)) 
    { 
        node->counter++; 
	node->strVector.push_back(data);
	//counter++;
        return node; 
    } 
    if(data >(node->t.data))
    {
        node->right = Insert(node->right,data);
    }
    else if(data < (node->t.data))
    {
        node->left = Insert(node->left,data);
    }
  
    
    
    return node;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////




treeNode * Find(treeNode *node, string data)
{  
    int pos=0;
    if(node==NULL)
    {  
        return NULL;
    }
    if(data > node->t.data)
    {
       
        return Find(node->right,data); 				 /* Search in the right sub tree. */
    }
    else if(data < node->t.data)
    {
        
        return Find(node->left,data);				 /* Search in the left sub tree. */
    }
    else
    {  
        return node;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Preorder(treeNode *node)
{
   
    
    if(node==NULL)
    {
        return;
    }
    Preorder(node->left);
    cout<<node->t.data<<" " <<endl;
    
    Preorder(node->right);
}


void PrintDuplicateCount(treeNode *node)
{
    
    
    if(node==NULL)
    {
        return;
    }

    
    PrintDuplicateCount(node->left);
   

     globalCounter += node->counter;
     
     std::cout <<"String  : " << node->data <<" : Count : " << node->counter << std::endl;
    
    result.push_back(node->counter);
    
    
    


    if(node->strVector.size() > 1){
    std::cout <<"============ Raman ============================"<<std::endl;
    for(auto &str : node->strVector){
	std::cout << str << std::endl;
    }
    std::cout <<"========================================"<<std::endl;
    }
    
    PrintDuplicateCount(node->right);
    
    
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////






int main()
{
   std::ofstream rfile("newfile.txt");
   std::string line; 
   std::string key;
   int count=0;
   int uni=0;
   string ch; 
   treeNode *root = NULL,*temp;

   std::ifstream sfile("shacodeNew.txt");
   
   std::vector<std::string> arr;

   			if(sfile)
				{

  					while(std::getline(sfile,line))
  					{       count++;
     						if(line.size() > 0)
						{
			  			  arr.push_back(line);
                       
						}
						if(!(count%10000))
							cout << "Events processed : " << count << std::endl;
  					}
				}
		

				cout << "NUMBER OF LINES: " <<count<< endl;
				sfile.close();

			
				


				std::cout <<"Array size  : " << arr.size() << std::endl;


				root= Insert(root,arr[0]);
				

				for (int i = 1; i < arr.size(); i++)
				{
         			  
            
					    Insert(root,arr[i]);
					    
		    
					     if(!(i%10000))
           					 {    
						        cout << "Events Inserted : " << i << std::endl;
	       				            
	                                          }
				}

      //cout<<" The Number of duplicates : "<<counter<<endl; 
    cout<<"\n Elements in BST are:" <<endl;
    //Preorder(root);

    PrintDuplicateCount(root);
    cout<<" The Number of duplicates : "<< globalCounter<<endl; 
/*for(int i = 0; i < result.size(); i++)
    {
        std::cout << result[i] <<std::endl;
    }    

for(int i = 0; i < result.size(); i++)
    {
        rfile << result[i] <<endl;
        
    }   */


    rfile.close();       
    return 0;
}




/*
checkUnique.cpp will make a binary search tree from the data. 
The data consists of shacodes from shacode.txt.
This code will insert all the shacodes into the tree and do inorder traversal to print it . 
It will check for unique shacodes .
Currently alll the shacodes are unique and there is no duplicate value . 
*/

