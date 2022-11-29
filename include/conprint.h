#ifndef CONPRINT_H
#define CONPRINT_H
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>

#include <iostream>
#include <sstream>
//#include <iomanip>

/**
* @author Eril Shackle
* @brief Prints in the console the way you'd like
*/

enum _outprinttype {
    nul, ln, ret, spc, selectable, dot, warn, alert, pt, pt2, tag, quotes,
    entry, entry_num, entry_str, entry_perc, entry_money, entry_param, entry_required,
    inpt, outpt
};

enum _conprint_color {
    cor_black, cor_blue, cor_green, cor_aqua, cor_red, cor_purple, cor_yellow, cor_white, cor_gray,
    cor_bluelight, cor_greenlight, cor_aqualight, cor_redlight, cor_purplelight, cor_yellowlight, cor_whitebright,
    cor_default = 0xFF
};
typedef _conprint_color concolor;
typedef _outprinttype   conprintout;

class _ConPrint
{
    public:

        static _ConPrint *getInstance() {
            if (!_instance)
                _instance = new _ConPrint;
            return _instance;
        }

        struct _ConPrintOut {
            private: _ConPrint* _cpo_;
                bool _ok_cor, _ok_center, _ok_atpos,
                     _ok_tywrite;
                CONSOLE_SCREEN_BUFFER_INFO _csbinfo_;
                COORD _pos;
                int _forecor, _backcor, _scrsize, _speed;
            public:
                _ConPrintOut(_ConPrint *cpo){
                    _cpo_ = cpo;_ok_cor = _ok_center = _ok_atpos =
                    _ok_tywrite = false;
                }
            // methods
            _ConPrintOut& color(_conprint_color);
            _ConPrintOut& color(_conprint_color,_conprint_color);
            _ConPrintOut& highlight(_conprint_color);
            _ConPrintOut& highlight2(_conprint_color);
            _ConPrintOut& onCenter(int=60);
            _ConPrintOut& atPos(short int,short int=0);
            _ConPrintOut& typewrite(short int=25);
            /// print out to console: last methods: conprint("text")...out();
            void out(conprintout opt = nul);
            void outln(void);
        } *_con_print_out;



        /// first methods: conprint("text")
        class _ConPrintOut& operator () (std::string);
        class _ConPrintOut& operator () (int);
        class _ConPrintOut& operator () (float,short int=-1);
        //-------------//
        void pause();
        void wait(int);
        void line(char);
        void option_selector(SHORT, SHORT,BOOL=TRUE, INT=1, BOOL=TRUE);
        void clearscreen(short int = 0);
        void clearscreenY(int);
        void move_cursor(int,int);
        void file_content(std::ifstream&);
        void time(void);
        void date(void);
        void who_made_this_header(void);

    private: static _ConPrint *_instance;
        bool _allows_;
        int _back_cor, _front_cor;
        std::string _buffer;
        int _foregc(void)
        {
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbi);
            int F=csbi.wAttributes;
            return F%16;
        }
        int _backgc(void)
        {
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&csbi);
            int B=csbi.wAttributes;
            B = (B/16)%16;
            return B;
        }
        void _setcolor(int fc,int bc)
        {
            if((fc%16)==(bc%16))fc+=8;
            fc%=16;
            bc%=16;
            unsigned short wAttributes= ((unsigned)bc<<4)|(unsigned)fc;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wAttributes);
        }

        // Private constructor so that no objects can be created.
        _ConPrint() {
            _buffer.clear();
            _allows_ =  false;
            _back_cor = _backgc();
            _con_print_out = new _ConPrintOut(this);
        }

};

static _ConPrint& conprint = *(conprint.getInstance());


/// CORPRINT CONCOR

enum _concorprint {
    corprint = 0x0FF,
    Colored = 0x0FF
};



static struct _concor_{
    private:
        std::string __text;
        int _backcor, _forecor;
        bool _isset_cor = false;
        struct _concor_* __cc = NULL;

        int _color_fg(void) {
            CONSOLE_SCREEN_BUFFER_INFO cbi;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cbi);
            return  cbi.wAttributes;
        }
        int _color_back(void) { return _color_fg() / 16; }
        int _color_fore(void) { return _color_fg() % 16; }

        struct _concor_* _setnewcc(std::string text, int color){
            if(__cc != NULL) delete __cc;
            struct _concor_* cc = new struct _concor_();
            cc->__text = text;
            cc->_backcor = _color_back();
            cc->_forecor = color % 16;
            return cc;
        }
        struct _concor_* _setnewcc(int num, int color){
            return this->_setnewcc(std::to_string(num),color);
        }

    public:

        _concor_(){
            _forecor = _color_fore();
            __text.clear();
        }
        struct _concor_& operator () (std::string text){
            _isset_cor = false;
            __text = text;
            return *this;
        }
        struct _concor_& operator () (int i){
            return (*this)(std::to_string(i));
        }
        struct _concor_& operator () (double f){
            return (*this)(std::to_string(f));
        }

        /// @def set the backgroud of the output
        struct _concor_* operator [] (int cor){
            __text.clear();
            if(!_isset_cor){
                _forecor = cor;
                _backcor = _color_back();
                _isset_cor = true;
            } else {
                _backcor = cor;
            }
            return this;
        }

        template<class E,class T> friend inline std::basic_ostream<E,T>&
        operator << (std::basic_ostream<E,T>& os, struct _concor_& cc);

        // acessible
        int _color(){return _forecor + _backcor * 16;}
        std::string _text(){return __text;}

        // colors
        struct _concor_& black(std::string str)         {return *_setnewcc(str,0x0);}
        struct _concor_& blue(std::string str)          {return *_setnewcc(str,0x1);}
        struct _concor_& green(std::string str)         {return *_setnewcc(str,0x2);}
        struct _concor_& aqua(std::string str)          {return *_setnewcc(str,0x3);}
        struct _concor_& red(std::string str)           {return *_setnewcc(str,0x4);}
        struct _concor_& purple(std::string str)        {return *_setnewcc(str,0x5);}
        struct _concor_& yellow(std::string str)        {return *_setnewcc(str,0x6);}
        struct _concor_& white(std::string str)         {return *_setnewcc(str,0x7);}
        struct _concor_& gray(std::string str)          {return *_setnewcc(str,0x8);}
        struct _concor_& bluelight(std::string str)     {return *_setnewcc(str,0x9);}
        struct _concor_& greenlight(std::string str)    {return *_setnewcc(str,0xA);}
        struct _concor_& aqualight(std::string str)     {return *_setnewcc(str,0xB);}
        struct _concor_& redlight(std::string str)      {return *_setnewcc(str,0xC);}
        struct _concor_& purplelight(std::string str)   {return *_setnewcc(str,0xD);}
        struct _concor_& yellowbright(std::string str)  {return *_setnewcc(str,0xE);}
        struct _concor_& whitebright(std::string str)   {return *_setnewcc(str,0xF);}
        // colors integer
        struct _concor_& black(int num)                 {return *_setnewcc(num,0x0);}
        struct _concor_& blue(int num)                  {return *_setnewcc(num,0x1);}
        struct _concor_& green(int num)                 {return *_setnewcc(num,0x2);}
        struct _concor_& aqua(int num)                  {return *_setnewcc(num,0x3);}
        struct _concor_& red(int num)                   {return *_setnewcc(num,0x4);}
        struct _concor_& purple(int num)                {return *_setnewcc(num,0x5);}
        struct _concor_& yellow(int num)                {return *_setnewcc(num,0x6);}
        struct _concor_& white(int num)                 {return *_setnewcc(num,0x7);}
        struct _concor_& gray(int num)                  {return *_setnewcc(num,0x8);}
        struct _concor_& bluelight(int num)             {return *_setnewcc(num,0x9);}
        struct _concor_& greenlight(int num)            {return *_setnewcc(num,0xA);}
        struct _concor_& aqualight(int num)             {return *_setnewcc(num,0xB);}
        struct _concor_& redlight(int num)              {return *_setnewcc(num,0xC);}
        struct _concor_& purplelight(int num)           {return *_setnewcc(num,0xD);}
        struct _concor_& yellowbright(int num)          {return *_setnewcc(num,0xE);}
        struct _concor_& whitebright(int num)           {return *_setnewcc(num,0xF);}
        // colors integer
        struct _concor_& black(double num)              {return *_setnewcc(num,0x0);}
        struct _concor_& blue(double num)               {return *_setnewcc(num,0x1);}
        struct _concor_& green(double num)              {return *_setnewcc(num,0x2);}
        struct _concor_& aqua(double num)               {return *_setnewcc(num,0x3);}
        struct _concor_& red(double num)                {return *_setnewcc(num,0x4);}
        struct _concor_& purple(double num)             {return *_setnewcc(num,0x5);}
        struct _concor_& yellow(double num)             {return *_setnewcc(num,0x6);}
        struct _concor_& white(double num)              {return *_setnewcc(num,0x7);}
        struct _concor_& gray(double num)               {return *_setnewcc(num,0x8);}
        struct _concor_& bluelight(double num)          {return *_setnewcc(num,0x9);}
        struct _concor_& greenlight(double num)         {return *_setnewcc(num,0xA);}
        struct _concor_& aqualight(double num)          {return *_setnewcc(num,0xB);}
        struct _concor_& redlight(double num)           {return *_setnewcc(num,0xC);}
        struct _concor_& purplelight(double num)        {return *_setnewcc(num,0xD);}
        struct _concor_& yellowbright(double num)       {return *_setnewcc(num,0xE);}
        struct _concor_& whitebright(double num)        {return *_setnewcc(num,0xF);}
        // color advanced
        struct _concor_& invert(){
            __text = this->__text;
            _backcor = this->_forecor;
            _forecor = _color_back();
            return *this;
        }

}concor;


template<class E,class T>
inline std::basic_ostream<E,T>& operator << (std::basic_ostream<E,T>& os, struct _concor_& cc)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), cc._color());      // set cor
    os << cc._text();                                                           // apply cor
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), csbi.wAttributes); // reset cor
    delete &cc;
    return os;
}

/// CORPTINT  @usage: std::cout << concor[cor]("Texts") << ...;

#endif // CONPRINT_H
