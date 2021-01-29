#include <iostream>
#include <cstring>

using namespace std;
int main()
{
    char sentence[]="This is a sentence with 7 tokens";
    cout <<"The string to be tokenized is: \n"<<sentence << "\n\nThe tokens art:\n\n";
    char *tokenPtr = strtok(sentence," ");
    while(tokenPtr)
    {
        cout<<tokenPtr<<endl;
        tokenPtr =  strtok(NULL," ");
    }
    return 0;

}
                
