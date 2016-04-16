#ifndef SOCKUTILS_H_INCLUDED
#define SOCKUTILS_H_INCLUDED

#define TAILLE_NOM 20
#define ENDOFCHAR2  1

typedef int socklen_t;

int sizeOfChar2(void** tab);

char** explode(const char* delimiter, char* str, int* nbElements);
int sendMessage(int csock, const char* message,int flags);
int readBufferLine(const char* buffer, char** read, int line);

#include <sstream>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}


#endif // SOCKUTILS_H_INCLUDED
