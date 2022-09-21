/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>
#include <sstream>
#include <cstring>


using namespace std;


string extractFirstNumberOccurence( string str)
{
  stringstream buffer;
  int i; 
  int tot = str.length();
  int beginFlag=1;
  for ( std::string::iterator it=str.begin(); it!=str.end(); ++it)
  {
       
      if((*it>='0' && *it<='9'))
      {
         buffer.put(*it);
         beginFlag=false;
      }else {
        if(!beginFlag) break;
      }
  }
  return buffer.str();
}
// Driver code
int main()
{
        string s = "geeks for geeks geeks 1 2354 251"
                        "contribution placements";
        string s1 ="sip:+526632970137;phone-context=ims.mnc028.mcc510.3gppnetwok.org@ims.mnc028.mcc510.3gppnetwok.org;user=phone";
        cout << " Number of words are: " << extractFirstNumberOccurence(s1);
        cout << endl ;
        return 0;
}

