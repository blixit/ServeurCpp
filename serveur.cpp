#include <iostream>
#include <string.h>
#include <cstdlib>
#include <ctype.h> //tolower
using namespace std;

#include "serveur.h"
#include "sockUtils.h"

int serveur::sizeDocument(const char* filename){
    int length = 0;
    string new_filename = _documentRoot()+string(filename);
    ifstream fichier(new_filename.c_str(), ios::in);
    if(fichier.is_open()){
        fichier.seekg(0,fichier.end);
        length = fichier.tellg();
        fichier.seekg (0, fichier.beg);
        fichier.close();
    }
    return length;
}

bool serveur::findDocument(const char* filename){
    string new_filename = _documentRoot()+string(filename);
    ifstream fichier(new_filename.c_str(), ios::in);
    bool found = false;
    if(fichier){
        found = true;
        fichier.close();
    }
    return found;
}

string serveur::getMimeType(const char* completefilename){
    ifstream fichier((documentRoot+string("mimes.txt")).c_str(),ios::in);
    if(!fichier.is_open())
        throw "Le fichier mime est manquant.";

    char** ext = NULL;
    int nb = 0;
    if(string(completefilename).find(".") != std::string::npos){
        ext = explode(".",(char*)completefilename,&nb);
    }
    //dernier élément après le '.'
    nb = nb > 0 ? nb-1 : 0;
    cout << "Ext : " << ext[nb] << endl;
    if(strlen(ext[nb])==0)
        throw "Nom de fichier incorrect.";

    for(int i=0; i<strlen(ext[nb]);i++)
        ext[nb][i] = tolower(ext[nb][i]);

    string line, mime = "";
    bool found = false;
    while(getline(fichier,line) && !found){
        if(line.find("=") != std::string::npos){
            int nb2 = 0;
            char** words = explode("=",(char*)line.c_str(),&nb2);
            if(nb2==2){
                if(strncmp(ext[nb],words[0],strlen(ext[nb]))==0){
                    found = true;
                    mime = string(words[1]);
                }
            }
            free(words);
        }
    }

    free(ext);
    return mime;
}
