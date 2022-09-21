/**
*    Extract first number occurence from a string. 
*    Author : RCS
**/
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

// Return first occurance of a number in the given string 

string extractFirstNumberOccurence( string str)
{
  stringstream buffer;
  int flag=false; //Found first occurence of a digit in the string.
  for ( std::string::iterator it=str.begin(); it!=str.end(); ++it)
  {    
      if((*it>='0' && *it<='9'))
      {
         buffer.put(*it);
         flag=true;
      }else {
        if(flag) break; //Break the loop if all the digits of first occurance of a number are read;
      }
  }
  if(!flag) { cout << "WARNING: Expecting a Number, but there is no number in the string. "; }
  return buffer.str();
}

int main()
{
        vector<std::string> testStrings {
            "sip:+526632970137;phone-context=ims.mnc028.mcc510.3gppnetwok.org@ims.mnc028.mcc510.3gppnetwok.org;user=phone",
            "sip:526632970137",
            "sip:+6632970137@ims.mnc028.mcc510.3gppnetwok.org",
            "ppnetwok.org@ims.mnc"
            };
        for(int i=0; i<testStrings.size(); i++)
           cout << " Number of words are: " << extractFirstNumberOccurence(testStrings[i])<<endl;
        return 0;
}
