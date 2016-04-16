#include <iostream>
#include <fstream>
#include <cstdio>
#include <winsock2.h>

using namespace std;

#include "http.h"
#include "sockUtils.h"

//Static methode
int http::sendMessage(int csock, const char* message,int length,int flags){
    return send(csock,message,length,flags);
}

int http::sendHeader(const char* property){
    std::string str = string(property)+"\r\n";
    return http::sendMessage(this->_csock(),str.c_str(),str.length(),0);
}

int http::sendData(const char* data, bool withLine){
    std::string str = string(data);
    if(withLine)
        str += "\r\n";
    return http::sendMessage(this->_csock(),str.c_str(),str.length(),0);
}
int http::sendBinaryData(const char* data, int length,bool withLine){
    std::string str = string(data);
    if(withLine)
        str += "\r\n";
    return http::sendMessage(this->_csock(),str.c_str(),length,0);
}

int http::sendDocument(const char* filename) {
    //ouverture en lecture
    string new_filename = this->m_serveur._documentRoot()+string(filename);
    FILE* fichier = fopen(new_filename.c_str(),"rb");

    try{
        int MAX = 4096;
        char buffer[MAX];
        int write=0,written = 0,read=0,readen=0,lastcount=0;
        //while(fgets(buffer,255,fichier)){
        do{
            memset((void*)buffer,0,MAX);
            read=fread(buffer,sizeof(char),MAX,fichier);
            readen+=read;
            write = sendBinaryData(buffer,read,false);
            written += write;

            if(write==SOCKET_ERROR ){
                throw "Fichier corrompu";
            }
        }
        while(!feof(fichier));
        printf("%d caractères lus\n",readen);
        printf("%d caractères émis\n",written);
        fclose(fichier);
    }catch(const char* e){
        throw e;
    }
    return 1;
}

string http::http_decode(string url){
    size_t s = url.find("%20");
    if(s!=std::string::npos)
        return url.replace(s,3, " ");
    return url;
}
