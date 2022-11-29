#include <iostream>
#include <string>

#include <conprint.h>

using namespace std;

int main()
{
    string s;
    int i;

    // print a normal text // it's better to use std::cout by itself
    conprint("Console Prin").out();
    conprint("by Eril Shackle").out(ln); // use diferent type of output model in _outprinttype

    // print as an entry of user data
    conprint("Enter something").out(entry);

    // print with color enter method color and the param cor_xxxx
    conprint("Print ").color(cor_redlight).out();
    conprint("with ").color(cor_greenlight).out();
    conprint("color").color(cor_bluelight).outln();

    // highlight something
    conprint("look at me").highlight(cor_default).out();

    // you can print with color using cout
    std::cout << " printing with " << concor.purplelight("color") << " this number: ";// << concor.red(123);
    cout << concor.yellow(1234) << endl;


    cout << "this is " << concor.gray("gray").invert() << " and " << concor.white("white") << endl;

    // great example:
    conprint("Enter your name").color(cor_gray).out(entry_str);
    getline(cin,s);
    conprint("Enter your age").color(cor_gray).out(entry_num);
    cin >> i;
    cout << "Hello " << concor.aqualight(s) << ", now I know you are " << concor.purplelight(i) << " years old." << endl;



    getch();
    return 0;
}
