#include <iostream>
#include <cstdlib>

using namespace std;

#include "../sockUtils.h"
#include "../include/httprequete.h"


httprequete::httprequete()
{
    m_buffer = "";
    m_query = NULL;
}

httprequete::~httprequete()
{
    if(m_query != NULL)
        delete m_query;
}

t_query* httprequete::getQuery(){
    if(m_query != NULL)
        delete m_query;

    m_query = new t_query();
    //on lit la 1ere ligne
    char *buffer_tmp = NULL;

    readBufferLine(m_buffer.c_str(),&buffer_tmp,1);

    char** requete = explode(" ",buffer_tmp,NULL);

    m_query->methode = string(requete[0]);
    m_query->uri = http::http_decode(requete[1]);

    if(m_query->uri.find("?")!=std::string::npos){
        int *nb = (int*)malloc(4);
        char** infos = explode("?",(char*)m_query->uri.c_str(),nb);
        if(*nb>1){
            m_query->path = (infos[0]);
            m_query->chaineparameters = (infos[1]);
        }
        else{
            m_query->path = string(infos[0]);
            m_query->chaineparameters = "";
        }
        free(infos);
    }else{
        m_query->path = m_query->uri;
        m_query->chaineparameters = "";
    }

    if(m_query->path[0] == '/')
        m_query->path = m_query->path.substr(1,m_query->path.length());
printf("path = %s\n",m_query->path.c_str());

    //on
    m_query->nb_parameters = 0;
    m_query->parameterskeys = NULL;
    m_query->parametersvalues = NULL;
    if(m_query->chaineparameters.length()>0){

        //plusieurs paramètres
        if(m_query->chaineparameters.find("&")!=std::string::npos){
            int* nb = (int*)malloc(sizeof(int));
            char** parameters = explode("&",(char*)m_query->chaineparameters.c_str(),nb);
            m_query->nb_parameters = *nb;
            m_query->parameterskeys = new string[m_query->nb_parameters];
            m_query->parametersvalues = new string[m_query->nb_parameters];
            for(int i(0); i<m_query->nb_parameters; i++){
                    printf("$$$ %s\n",parameters[i]);
                if(parameters[i] != NULL && string(parameters[i]).find("=")!=std::string::npos){
                    int* _nb= (int*)malloc(sizeof(int));
                    char** parametre = explode("=",parameters[i],_nb);
                    if(*_nb>1){
                        m_query->parameterskeys[i] = string(parametre[0]);
                        m_query->parametersvalues[i] = string(parametre[1]);
                    }
                    else{
                        m_query->parameterskeys[i] = string(parametre[0]);
                        m_query->parametersvalues[i] = "";
                    }
                    free(_nb);
                    free(parametre);
                }else{
                    m_query->parameterskeys[i] = string(parameters[i] == NULL ? "" : parameters[i]);
                    m_query->parametersvalues[i] = "";
                }
            }
            free(parameters);
            free(nb);
        }
        //un seul paramètre
        else{
            m_query->nb_parameters = 1;
            m_query->parameterskeys = new string[1];
            m_query->parametersvalues = new string[1];
            int _nb=0;
            char** parametre = explode("=",(char*)m_query->chaineparameters.c_str(),&_nb);
            if(_nb>1){
                m_query->parameterskeys[0] = string(parametre[0]);
                m_query->parametersvalues[0] = string(parametre[1]);
            }
            else
                m_query->parameterskeys[0] = string(parametre[0]);
                m_query->parametersvalues[0] = "";
            free(parametre);
        }

    }

    free(buffer_tmp);
    free(requete);

    return m_query;
}
