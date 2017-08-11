#include <iostream>
#include <set>
#include <string>
#include <cassert>
#include <TH1F.h>
using namespace std;

class ObjectChecker {
      TH1F *fHist;
   public:
      ObjectChecker();
      ObjectChecker(string n);
      TH1F* GetHist() const {return fHist;}
      bool operator<(const ObjectChecker& right)const; 
      string get_name()const;
      void CheckAndInsert(std::string objObjectChecker);
      std::set<ObjectChecker> GetSet(){return ObjectCheckersSet;}
   private:
      string name;             
      std::set<ObjectChecker> ObjectCheckersSet;
};

void ObjectChecker::CheckAndInsert(std::string objObjectChecker){
for (std::set<ObjectChecker>::iterator it=ObjectCheckersSet.begin(); it!=ObjectCheckersSet.end(); ++it){
if((*it).get_name() == objObjectChecker)
{
    std::cout <<"Attempt to DUPLICATE ... the histogram with name : "<< (*it).GetHist()->GetName() << "  !!! " << std::endl;
//    cout << "HistObjectChecker : "<< (*it).GetHist()->GetObjectChecker() << endl;
    return ;
}
}

std::cout<<"Inserting new Object..... " << std::endl;
ObjectCheckersSet.insert(ObjectChecker(objObjectChecker));

}

ObjectChecker::ObjectChecker(){}

ObjectChecker::ObjectChecker(string n)
{
   name = n;
   fHist = new TH1F(n.c_str(),n.c_str(),100,0,100);
}

bool ObjectChecker::operator<(const ObjectChecker& right)const
{
      return (this->name < right.name);
}

string ObjectChecker::get_name()const
{
      return name;
}

int main()
{

ObjectChecker name;
//set<ObjectChecker>ObjectCheckersSet;


//ObjectCheckersSet.insert(ObjectChecker("Patrick Star"));
//ObjectCheckersSet.insert(ObjectChecker("Jason"));
//ObjectCheckersSet.insert(ObjectChecker("Bob Marl"));
//ObjectCheckersSet.insert(ObjectChecker("Greg"));
//set<ObjectChecker>::iterator pos ; 

std::vector<std::string> nameVect;
nameVect.push_back("Raman");
nameVect.push_back("Sehgal");

for(int i = 0 ; i < nameVect.size() ; i++){

string cusname = nameVect[i];

/*
for (std::set<ObjectChecker>::iterator it=ObjectCheckersSet.begin(); it!=ObjectCheckersSet.end(); ++it){
//if ( ObjectCheckersSet.count(cusname) == 1 )
if((*it).get_name() == cusname)
{
  //  cout << cusname << " exists in set"<<endl;
    std::set<ObjectChecker>::iterator it = ObjectCheckersSet.begin();
    cout << "HistObjectChecker : "<< (*it).GetHist()->GetObjectChecker() << endl;
    return 0;
}
}
*/

//ObjectCheckersSet.insert(ObjectChecker(cusname));
name.CheckAndInsert(cusname);

}

//ObjectCheckersSet.insert(ObjectChecker("raman"));
name.CheckAndInsert("raman");

std::cout<<"------------------ Trying to duplicate -------------------" << std::endl;
//ObjectCheckersSet.insert(ObjectChecker("Raman"));
name.CheckAndInsert("Raman");

std::set<ObjectChecker> ObjectCheckersSet = name.GetSet();

std::cout<<"-------------- Printing All histograms names -------------" << std::endl;
for (std::set<ObjectChecker>::iterator it=ObjectCheckersSet.begin(); it!=ObjectCheckersSet.end(); ++it){
std::cout<<"HistName : "<< (*it).GetHist()->GetName() << std::endl;
}

/*else 
{
    cout<<"Nanda!";
}
*/
   return 0;
}
