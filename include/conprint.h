#ifndef CONPRINT_H
#define CONPRINT_H
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>

#include <iostream>
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
                bool _ok_cor, _ok_center, _ok_atpos;
                CONSOLE_SCREEN_BUFFER_INFO _csbinfo_;
                COORD _pos;
                int _forecor, _backcor, _scrsize;
            public:
                _ConPrintOut(_ConPrint *cpo){
                    _cpo_ = cpo;_ok_cor = _ok_center = _ok_atpos = false;
                }
            // methods
            _ConPrintOut& color(_conprint_color);
            _ConPrintOut& color(_conprint_color,_conprint_color);
            _ConPrintOut& highlight(_conprint_color);
            _ConPrintOut& highlight2(_conprint_color);
            _ConPrintOut& onCenter(int=60);
            _ConPrintOut& atPos(short int,short int=0);
            /// print out to console: last methods: conprint("text")...out();
            void out(conprintout opt = nul);
            void outln(void);
        } *_con_print_out;


        /// first methods: conprint("text")
        class _ConPrintOut& operator () (std::string);
        //-------------//
        void pause();
        void wait(int);
        void line(char);
        void option_selector(SHORT, SHORT,BOOL=TRUE, INT=1, BOOL=TRUE);
        void clearscreen();
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


/// CORPRINT

enum _concorprint {
    corprint = 0x0FF,
    concor = 0x0FF,
    Colored = 0x0FF
};

static CONSOLE_SCREEN_BUFFER_INFO* _ccp_currentcscorlorinfo = NULL;
static BOOL _ccp_currentcscorlorinfo_ok = TRUE;
static BOOL _ccp_currentcscorlorinfo_isdefined = FALSE;
static concolor _ccp_lastcolor = (concolor) 0x0FF;

template<class elem,class traits>
inline std::basic_ostream<elem,traits>& operator << (std::basic_ostream<elem,traits>& os, concolor cc)
{
    static CONSOLE_SCREEN_BUFFER_INFO _console_current_color;
    if(_ccp_currentcscorlorinfo_isdefined == FALSE)
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &_console_current_color);

    if(_ccp_currentcscorlorinfo_ok)
    {
        _ccp_currentcscorlorinfo = new CONSOLE_SCREEN_BUFFER_INFO;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), _ccp_currentcscorlorinfo);
        //! reset color
        if(_ccp_currentcscorlorinfo_isdefined == TRUE)
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _console_current_color.wAttributes);
        //! os flush
        os.flush();
        //! get background color
        int backcolor =_console_current_color.wAttributes;
        backcolor = (backcolor/16)%16;
        //! set color
        int textcolor = (int)cc;
        if((textcolor%16)==(backcolor%16)) textcolor+=8;
        textcolor%=16;
        backcolor%=16;
        _ccp_currentcscorlorinfo_isdefined = TRUE;
        unsigned short wAttributes= ((unsigned)backcolor<<4)|(unsigned)textcolor;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wAttributes);
        //! last color = cc
        _ccp_lastcolor = cc;
        //! unenable coloring
        _ccp_currentcscorlorinfo_ok = FALSE;
    }
    return os;
}

template<class elem,class traits>
inline std::basic_ostream<elem,traits>& operator >> (std::basic_ostream<elem,traits>& os, _concorprint ccp)
{
    if( (!_ccp_currentcscorlorinfo_ok && (int)ccp == (int)_ccp_lastcolor) || ccp == corprint )
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _ccp_currentcscorlorinfo->wAttributes);
        //os.clear();
        delete _ccp_currentcscorlorinfo;
        _ccp_currentcscorlorinfo_ok = TRUE;
    }
    return os;
}

/// CORPTINT  @usage: std::cout << concolor << "Texts" >> CORPRINT

#endif // CONPRINT_H
