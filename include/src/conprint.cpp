#include "conprint.h"
#include <cstdio>
#include <iomanip>
#include <string>


_ConPrint *_ConPrint::_instance = 0;

class _ConPrint::_ConPrintOut& _ConPrint::operator()(std::string text)
{
    _buffer = text;
    _allows_ = true;
    return *_con_print_out;
}
class _ConPrint::_ConPrintOut& _ConPrint::operator()(int num){
    //! to use std::string please goto build option and activate c++11
    //std::string text = std::to_string(num);
    char text[16];
    sprintf(text, "%d", num);
    return (*this)(text);
}
class _ConPrint::_ConPrintOut& _ConPrint::operator()(float real, short int deciamals){
    //! to use std::string please goto build option and activate c++11
    //std::string text = std::to_string(real);
    char text[16];
    if(deciamals==0) sprintf(text, "%.0f", real); else
    if(deciamals==1) sprintf(text, "%.1f", real); else
    if(deciamals==2) sprintf(text, "%.2f", real); else
    if(deciamals==3) sprintf(text, "%.3f", real); else
    if(deciamals==4) sprintf(text, "%.4f", real); else
    sprintf(text, "%f", real);
    return (*this)(text);
}

_ConPrint::_ConPrintOut& _ConPrint::_ConPrintOut::color(_conprint_color foreground, _conprint_color background)
{
    if(!_ok_cor)
    {
        _forecor = foreground;
        _backcor = background;
        _ok_cor = true;
    }
    return *this;
}
_ConPrint::_ConPrintOut& _ConPrint::_ConPrintOut::color(_conprint_color foreground)
{
    int background = _cpo_->_backgc();
    return color(foreground,(_conprint_color)background);
}

_ConPrint::_ConPrintOut& _ConPrint::_ConPrintOut::highlight(_conprint_color highlight)
{
    if(!_ok_cor)
    {
        _forecor = _cpo_->_foregc();
        _backcor = highlight;
        if(_forecor == _backcor || highlight == cor_default)
        {
            _forecor = _cpo_->_backgc();
            _backcor = highlight;
        }
        _ok_cor = true;
    }
    return *this;
}
_ConPrint::_ConPrintOut& _ConPrint::_ConPrintOut::highlight2(_conprint_color highlight)
{
    if(!_ok_cor)
    {
        _forecor = _cpo_->_backgc();
        _backcor = highlight;
        _ok_cor = true;
    }
    return *this;
}


_ConPrint::_ConPrintOut& _ConPrint::_ConPrintOut::onCenter(int screenSize){
    if(!_ok_center && !_ok_atpos){
        _cpo_->_buffer += '\n';
        _scrsize = screenSize;
        _ok_center = true;
    }
    return *this;
}

_ConPrint::_ConPrintOut& _ConPrint::_ConPrintOut::atPos(short int y_line, short int x_col){
    if(!_ok_center && !_ok_atpos){
        _pos.X = x_col;
        _pos.Y = y_line;
        _ok_atpos = true;
        _ok_center = false;    // we cannot use onCenter when using atPos
    }
    return *this;
}

_ConPrint::_ConPrintOut& _ConPrint::_ConPrintOut::typewrite(short int miliseconds){
    if(!_ok_tywrite){
        _speed = miliseconds;
        _ok_tywrite = true;
    }
    return *this;
}


void _ConPrint::clearscreen(short int fromline){
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD line, area;
    COORD homepoint = {0, fromline};
    if(hStdOut == INVALID_HANDLE_VALUE) return;
    if(!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
    area = csbi.dwSize.X * csbi.dwSize.Y;
    if(!FillConsoleOutputCharacter(hStdOut,0x20, area, homepoint, &line)) return;
    if(!FillConsoleOutputAttribute(hStdOut,csbi.wAttributes, area, homepoint, &line)) return;
    SetConsoleCursorPosition(hStdOut, homepoint);
    return;
}


void _ConPrint::_ConPrintOut::out(conprintout opt)
{

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &_csbinfo_);

    std::string optstr;
    switch(opt) {
        case nul:           optstr = "\0"; break;
        case ln:            optstr = "\n";break;
        case ret:           optstr = "\r";break;
        case selectable:    optstr = "\r > \b\b";_cpo_->_buffer = " > " + _cpo_->_buffer; break;;
        case entry:         optstr = " >> "; _cpo_->_buffer = " " + _cpo_->_buffer ; break;
        case entry_num:     optstr = " >> "; _cpo_->_buffer = "# " + _cpo_->_buffer; break;
        case entry_str:     optstr = " >> "; _cpo_->_buffer = "@ " + _cpo_->_buffer; break;
        case entry_perc:    optstr = " >> ";_cpo_->_buffer = "% " + _cpo_->_buffer;break;
        case entry_money:   optstr = " >> ";_cpo_->_buffer = "$ " + _cpo_->_buffer;break;
        case entry_param:   optstr = " >> ";_cpo_->_buffer = ": " + _cpo_->_buffer;break;
        case entry_required:optstr = " >> ";_cpo_->_buffer = "! " + _cpo_->_buffer;break;
        case dot:           optstr = ".";break;
        case warn:          optstr = " >";_cpo_->_buffer = "<! " + _cpo_->_buffer;break;
        case alert:         optstr = " <!>";_cpo_->_buffer = "<!> " + _cpo_->_buffer;break;
        case pt:            optstr = "\0";_cpo_->_buffer = "- " + _cpo_->_buffer;break;
        case pt2:           optstr = "\0";_cpo_->_buffer = "* " + _cpo_->_buffer;break;
        case spc:           optstr = " ";break;
        case tag:           optstr = " />";_cpo_->_buffer = "< " + _cpo_->_buffer;break;
        case quotes:        optstr = "\"";_cpo_->_buffer = "\"" + _cpo_->_buffer;break;
        case inpt:          optstr = " >> ";break;
        case outpt:         optstr = " << ";break;
        default:            optstr = "";
    }
    _cpo_->_buffer += optstr;
    // color, highlight
    if(_ok_cor)
    {
        _cpo_->_setcolor(_forecor, _backcor);
        _forecor = _backcor = (int) cor_default;
    }
    // middle
    if(_ok_center){
        int mlen = _cpo_->_buffer.length()/2.0;
        std::cout << std::setw(_scrsize/2-mlen)<<' ';
    }
    // at pos
    if(_ok_atpos){
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), _pos);
    }

    //! printout
    if(_ok_tywrite){
        unsigned int cpms = 0;
        while(cpms < _cpo_->_buffer.length()){ putchar(_cpo_->_buffer[cpms++]); fflush(stdin); Sleep(_speed);}
    }else
    {
        std::cout << _cpo_->_buffer;
    }

    if(_ok_cor) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _csbinfo_.wAttributes);
    if(_ok_atpos) SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), _csbinfo_.dwCursorPosition);
    // desativate
    _ok_cor = _ok_atpos = _ok_center =
    _ok_tywrite = false;
    return;
}
void _ConPrint::_ConPrintOut::outln(){
    _cpo_->_buffer += '\n';
    return out(nul);
}
