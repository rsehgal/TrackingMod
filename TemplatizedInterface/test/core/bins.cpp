#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>

#include <stack> 
#include <ctime>
#include <unistd.h>


using namespace std ;


///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
struct TestStruct{
	string data;
};
*/

struct treeNode
{
    string data;
    treeNode *left;
    treeNode *right;
  //  TestStruct t;
   
    treeNode(){}

    treeNode(string str){
	data = str;
 //	t.data = data;
    }
  

};



////////////////////////////////////////////////////////////////////////////////////////////////////////////

treeNode* FindMin(treeNode *node)
{
    if(node==NULL)
    {
        /* There is no element in the tree */
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
    if(data >(node->data))
    {
        node->right = Insert(node->right,data);
    }
    else if(data < (node->data))
    {
        node->left = Insert(node->left,data);
    }
    
    return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////




treeNode * Find(treeNode *node, string data)
{
    if(node==NULL)
    {
        return NULL;
    }
    if(data > node->data)
    {
       
        return Find(node->right,data); 				 /* Search in the right sub tree. */
    }
    else if(data < node->data)
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
    cout<<node->data<<" " <<endl;
    
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
		if(temp->data == data)
		{
			cout<<"\nData found at depth: "<<depth;
                        cout<<endl;
			return;
		}
		// Shift pointer to left child.
		else if(temp->data > data)
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

   std::string line; 
   std::string key;
   int count=0;
   string ch; 
   treeNode *root = NULL,*temp;
  
   

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
     ch="ffbc41023898da82690de27606ee5c578916b1148cd948d295ef44fb37cb77d1 ";
     cout<<"ffbc41023898da82690de27606ee5c578916b1148cd948d295ef44fb37cb77d1\n";
     temp = Find(root,ch);
            if(temp==NULL)
            {
                cout<<"Element is not found";
            }
            else
            {
                cout<<"Element "<<temp->data<<" is Found\n";
            }
           
    Search(root,ch);
    return 0;
}
