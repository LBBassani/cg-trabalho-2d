#if !defined UTILS
#define UTILS

#include <string>

#define MOUSE_LEFT_CLICK 256
#define MOUSE_RIGHT_CLICK 257
#define MOUSE_X_COORD 258
#define MOUSE_Y_COORD 259
#define PLAYER_X_COORD 260
#define PLAYER_Y_COORD 261
#define NUM_TECLAS_ASCII 256

std::string first_numberstring(std::string const & str){
    char const* digits = "0123456789";
    std::size_t const n = str.find_first_of(digits);
    if (n != std::string::npos)
    {
    std::size_t const m = str.find_first_not_of(digits, n);
    return str.substr(n, m != std::string::npos ? m-n : m);
    }
    return std::string();
}

#endif