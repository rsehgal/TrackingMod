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
using Tomography::TrackPredictor;

///////////////////////////////////////////////////////////////////////////////////////////////////////////

struct TestStruct{
	string data;
};


struct treeNode
{
    string data;
    treeNode *left;
    treeNode *right;
    TestStruct t;
   
    treeNode(){}

    treeNode(string str){
        data = str;
    	t.data = data;
    }
  

};



////////////////////////////////////////////////////////////////////////////////////////////////////////////






treeNode *Insert(treeNode *node, string data)
{
    if(node==NULL)
    {
        treeNode *temp = new treeNode(data);
        //temp=new treeNode;
        //temp = (treeNode *)malloc(sizeof(treeNode));
        //temp -> data = data;
        temp -> left = temp -> right = NULL;
        return temp;
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
    {   pos++;
        return NULL;
    }
    if(data > node->t.data)
    {
        pos++;
        return Find(node->right,data); 				 /* Search in the right sub tree. */
    }
    else if(data < node->t.data)
    {
        pos++;
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




////////////////////////////////////////////////////////////////////////////////////////////////////////////////






int main()
{
   TrackPredictor c;
   std::string line; 
   std::string key;
   int count=0;
   int uni=0;
   string ch; 
   treeNode *root = NULL,*temp;
   TestStruct t;
 
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
						if(!(count%10000))
							cout << "Events processed : " << count << std::endl;
  					}
				}
		

				cout << "NUMBER OF LINES: " <<count<< endl;
				sfile.close();

			
				sfile.open("shacode.txt");
				

				std::cout <<"Array size  : " << arr.size() << std::endl;


				root= Insert(root,arr[0]);	
				uni=1;

				for (int i = 1; i < arr.size(); i++) 
				{
         			  
            
					    Insert(root,arr[i]);
					    uni++ ;
		    
					     if(!(i%10000))
           					 {    
						        cout << "Events Inserted : " << i << std::endl;
	       				            
	                                          }
				}

       
    cout<<"\n Elements in BST are:" <<endl;
    Preorder(root);
    cout<<" The Number of unique shacodes : "<<uni<<endl;
            				
           
    return 0;
}
