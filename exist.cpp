#include <iostream>
#include <set>
#include <string>
#include <cassert>

using namespace std;

class Name {
   public:
      Name();
      Name(string n);
      bool operator<(const Name& right)const; 
      string get_name()const;
   private:
      string name;             
};

Name::Name(){}

Name::Name(string n)
{
   name = n;
}

bool Name::operator<(const Name& right)const
{
      return (this->name < right.name);
}

string Name::get_name()const
{
      return name;
}

int main()
{

set<Name>NamesSet;
NamesSet.insert(Name("Patrick Star"));
NamesSet.insert(Name("Jason"));
NamesSet.insert(Name("Bob Marl"));
NamesSet.insert(Name("Greg"));
set<Name>::iterator pos; 

string cusname = "Greg";

if ( NamesSet.count(cusname) == 1 )
{
    cout << cusname << " exists in set"<<endl;
}
else 
{
    cout<<"Nanda!";
}

   return 0;
}
