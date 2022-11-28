#include <iostream>
#include <string>

#include <conprint.h>

using namespace std;

int main()
{
    string s;
    float f;
    int i;

    // conprint(text).color(cor_xxx).out();
    conprint("Your Name").color(cor_gray).out(entry_str);
    cout<<endl;
    conprint("Your Age").color(cor_aqualight).out(entry_num);
    cout<<endl;
    conprint("Evaluation percentage").color(cor_bluelight).out(entry_perc);
    cout<<endl;
    conprint("How much money").color(cor_greenlight).out(entry_money);
    cout<<endl;
    conprint("Enter the param of field").color(cor_yellowlight).out(entry_param);
    cout<<endl;
    conprint("field required").color(cor_white).out(entry_required);
    cout<<endl;
    conprint("This is an output").color(cor_default).out(outpt);

    // highlight
    conprint("Eriil SHackle\n").highlight2(cor_aqualight).outln();


    conprint("I'm on the middle").onCenter(80).out();

    conprint("Printed up here").atPos(0,50).outln();
    // cout << cor_xxx << ... ostream ... >> concor;
    //cout << cor_redlight << "Hi Eril" >> concor;

    //


    getch();
    return 0;
}
