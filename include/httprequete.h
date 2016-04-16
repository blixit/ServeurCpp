#ifndef HTTPREQUETE_H
#define HTTPREQUETE_H

using namespace std;

#include "../http.h"

typedef struct query{
    string uri;
    string path;
    string methode;
    string chaineparameters;
    string* parameterskeys;
    string* parametersvalues;
    int nb_parameters;
}t_query;

class httprequete
{
    public:
        /** Default constructor */
        httprequete();
        /** Default destructor */
        virtual ~httprequete();

        void _buffer(const char* buffer){m_buffer = string(buffer);}
        const char* _buffer(){return m_buffer.c_str();};

        t_query* getQuery();

    protected:
    private:
        string m_buffer;
        t_query* m_query;
};

#endif // HTTPREQUETE_H
