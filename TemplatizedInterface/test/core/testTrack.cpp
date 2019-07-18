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

treeNode* FindMin(treeNode *node)
{

    if(node==NULL)
    {
        
        return NULL;
    }
    if(node->left) 
        return FindMin(node->left);
    else
        return node;   
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////


treeNode* FindMax(treeNode *node)
{
    if(node==NULL)
    {
       
        return NULL;
    }
    if(node->right) 
        return(FindMax(node->right));
    else
        return node;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////


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



void Search(treeNode *node, string data)
{
	int depth = 0;
	treeNode *temp = new treeNode;
	temp = node;
	// Run the loop untill temp points to a NULL pointer.
	while(temp != NULL)
	{
		depth++;
		if(temp->t.data == data)
		{
			cout<<"\nData found at depth: "<<depth;
                        cout<<endl;
			//cout<<" Position of node "<<pos<<endl;
        		//cout<<"leftchid  :"<< temp->left <<endl ;
        		///cout<<"right child : "<< temp->right<< endl; 
			return;
		}
		// Shift pointer to left child.
		else if(temp->t.data > data)
			temp = temp->left;
		// Shift pointer to right child.
		else
			temp = temp->right;
	}
 
	cout<<"\n Data not found";
	return;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
   TrackPredictor c;
   std::string line; 
   std::string key;
   int count=0;
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

				//std::cout <<"OPening the file for Binary Search insertion................." << std::endl;
				sfile.open("shacode.txt");
				//std::cout <<"File opened successfully................." << std::endl;


				std::cout <<"Array size  : " << arr.size() << std::endl;


				root= Insert(root,arr[0]);	

				for (int i = 1; i < arr.size(); i++) 
				{
         			   //std::cout << arr[i] << std::endl;
            
					    Insert(root,arr[i]); 
		    
					     if(!(i%10000))
           					 {    
						        cout << "Events Inserted : " << i << std::endl;
	       				            //  std::cout << "Root Data : " << bst->data << std::endl;   
	                                          }
				}

       
    cout<<"\n Elements in BST are:" <<endl;
    Preorder(root);
            				
           
    temp = FindMin(root);
    cout<<"\nMinimum element is :"<<temp->data;
           
       
    temp = FindMax(root);
    cout<<"\nMaximum element is :"<<temp->data;
            
            
     cout<<"\nEnter element to be searched:";
     ch="ff05c307812d01554e43cdf302c9d46c6bc9749ebbb7c00ab69c32072dfeae35 ";
     cout<<"ff05c307812d01554e43cdf302c9d46c6bc9749ebbb7c00ab69c32072dfeae35n"<<endl;
     temp = Find(root,ch);
            if(temp==NULL)
            {
                cout<<"Element is not found";
            }
            else
            {
                cout<<"Element "<<temp->t.data<<" is Found\n";
            }
           
    Search(root,ch);
    cout<<endl;
    cout<<" Root elemet is :   ";
    cout<<root->t.data<<endl;
    return 0;
}
