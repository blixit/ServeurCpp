#ifndef CONSTANTESSERVEUR_H_INCLUDED
#define CONSTANTESSERVEUR_H_INCLUDED
#include <iostream>
#include <fstream>
using namespace std;

class serveur{
    public :
    string documentRoot;

    const char* _documentRoot(){return documentRoot.c_str();};
    void _documentRoot(const char* value){ documentRoot = std::string(value);};

    int sizeDocument(const char* filename);
    bool findDocument(const char* filename);
    string getMimeType(const char* completefilename);

} ;

#endif // CONSTANTESSERVEUR_H_INCLUDED
