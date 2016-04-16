#ifndef HTTP_H_INCLUDED
#define HTTP_H_INCLUDED

#include "serveur.h"


class http{
    public:
    int m_csock;
    serveur m_serveur;

    http(int csock, const serveur& apache) : m_csock(csock), m_serveur(apache){};

    int _csock(){ return m_csock;}
    void _csock(int value){ m_csock=value;}


    int sendHeader(const char* property);
    int sendData(const char* data, bool withLine);
    int sendBinaryData(const char* data, int length,bool withLine);
    int sendDocument(const char* filename);

    static int sendMessage(int csock, const char* message, int length, int flags);
    static string http_decode(string url);
};

#endif // HTTP_H_INCLUDED
