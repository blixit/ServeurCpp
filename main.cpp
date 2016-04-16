#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <unistd.h>

using namespace std;

#include "sockUtils.h"
#include "http.h"
#include "serveur.h"
#include "include/httprequete.h"

int main(int argc , char *argv[])

{ /**/
    printf("\t\t Programme Serveur \n\n");
    /**/

    int PORT=80;
    int sock_err;
    int r;
    char *name;
    char buffer[1024];
    int bool_begin = 1;

    serveur Apache = serveur();
    Apache.documentRoot = "C:/wamp/www/sde_html/";

    /**/
    WSADATA wsa;
    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(err < 0)
    {
        puts("WSAStartup failed !");
        exit(EXIT_FAILURE);
    }

    while(bool_begin)
    {
        SOCKET sock;
        /*sock = socket(AF_INET , SOCK_STREAM , 0 );*/
        SOCKADDR_IN sin;
        socklen_t recsize = sizeof(sin);
        sock = socket(AF_INET , SOCK_STREAM , 0 );
        if(sock == INVALID_SOCKET)
        {
            printf("socket()\n");
            exit(-1);
        }

        SOCKET csock;
        SOCKADDR_IN csin;
        socklen_t crecsize = sizeof(csin);

        /*printf("Indiqué un port : "); scanf("%d", &PORT);*/
        sin.sin_addr.s_addr = htonl(INADDR_ANY); /* Adresse IP automatique */
        sin.sin_port = htons(PORT); /* Listage du port */
        sin.sin_family = AF_INET; /* Protocole familial (IP) */

        sock_err = bind(sock, (SOCKADDR*)&sin, recsize);
        sock_err = listen(sock, 5);

        name = (char *) malloc(TAILLE_NOM);
        r = gethostname(name,(size_t) TAILLE_NOM);
        printf("Serveur sur %s initialise sur le PORT %d !\n", name, PORT);
        free(name);

        csock = accept(sock, (SOCKADDR*)&csin, &crecsize);
        printf("Nouvelle connexion\n");

        while(sock_err!=SOCKET_ERROR)
        {
            memset (buffer, 0, sizeof (buffer));
            sock_err=recv(csock, buffer, 1024, 0);
            httprequete requete = httprequete();
            requete._buffer(buffer);

            //si sock_err = 0, alors la connexion a été arrété avec shutdown, donc on arrete la connexion a notre niveau
            if(sock_err==0) sock_err = -1;
            if(buffer[0]!=0)
            {
                printf("Chaine Recu : %s\n", buffer);
            }

            if(sock_err!=-1){

                http response = http(csock,Apache);

                try{
                    t_query* infosurl = requete.getQuery();

                    if(!Apache.findDocument(infosurl->path.c_str()))
                        throw "404-Document not found.";

                    sock_err = response.sendHeader("HTTP/1.1 200 OK");
                    sock_err = response.sendHeader("Serveur : Nacder");
                    string file = Apache.documentRoot + infosurl->path;
                    string mime = "Content-Type : "+Apache.getMimeType(file.c_str());
                    sock_err = response.sendHeader(mime.c_str());

                    //on force le téléchargement des images
                    if(mime.find("image/") != std::string::npos){
                        /*int nb = 0;
                        char** basefile = explode("/",(char*)infosurl->path.c_str(),&nb);
                        string header = "Content-Disposition: attachment; filename=\""+string(nb>1 ? basefile[nb-1] : basefile[0])+"\"";
                        sock_err = response.sendHeader(header.c_str());
                        free(basefile);*/

                        sock_err = response.sendHeader("Content-Transfer-Encoding: binary");
                        sock_err = response.sendHeader((string("Content-Length: ")+patch::to_string(Apache.sizeDocument(infosurl->path.c_str()))).c_str());
                        sock_err = response.sendHeader("Charset:ISO-8859-4");
                        cout << "Size = " << Apache.sizeDocument(infosurl->path.c_str()) << endl;

                    }

                    //sock_err = response.sendHeader("Access-Control-Allow-Origin:*");
                    sock_err = response.sendHeader("");
                    sock_err = response.sendDocument(infosurl->path.c_str());

                }catch(const char* e){

                    sock_err = response.sendHeader("HTTP/1.1 404 NOT FOUND");
                    sock_err = response.sendHeader("Content-Type : text/html");
                    sock_err = response.sendHeader("");
                    sock_err = response.sendData((string(e)+"<br/>").c_str(),false);
                    sock_err = response.sendData("Le fichier demandé n'existe pas.",false);
                    cout << "ERREUR : " << e << endl;
                }

            }
            //printf("\n\n socket_error SEND : %d\n", sock_err);
            shutdown(csock, 2);
            closesocket(csock);
        }
        printf("Fin de connexion\n");
        shutdown(csock, 2);
        closesocket(csock);
        shutdown(sock, 2);
        closesocket(sock);
    }

    WSACleanup();


}
