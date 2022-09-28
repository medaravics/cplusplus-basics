// Create a Map of dynamically created objects and its deallocation of memory.

#include <iostream>
#include <map>
using namespace std;
class Ida {
  private:
    int m_networkId;
    int m_code;
  public:
    Ida(){}
    Ida(int id, int code){
      setId(id);
      setCode(code);
    }
    void setId(int id){
        m_networkId=id;
    }
    int getId() {
       return m_networkId;
    }
    void setCode(int code){
       m_code =code;
    }
    int getCode(){
       return m_code;
    }
};

int main ()
{
  std::map<int,Ida*> mymap;
  Ida *ref = new Ida(1, 2);
  cout << ref->getCode() << ", " << ref->getId() << endl;
  mymap.insert ( pair<int,Ida*>(ref->getId(), ref) );
  Ida *ref1 = new Ida(3, 45);
  mymap.insert ( pair<int,Ida*>(ref1->getId(), ref1) );
  cout << endl << "Size " <<  mymap.size()<< endl;
 while (!mymap.empty())
  {
    delete mymap.begin()->second;
    mymap.erase(mymap.begin());
  }
  cout << endl << "Size " <<  mymap.size()<< endl;
  return 0;
}
