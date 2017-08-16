#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>
#include <SFML/Audio.hpp>
#include <list>
#include <algorithm>
#define SNAME "server"  //nombre del servidor
#define SPYMODE true    //habilita o deshabilita el modo espia

using namespace zmqpp;
using namespace std;

//funcion que divide una cadena en un vector de cadenas
//utilizando como delimitacion el caracter espacio " "
vector<string> explode(string &big_string) {
    stringstream schutzstaffel(big_string);
    vector<string> individual_strings;
    string little_string;
    while (schutzstaffel >> little_string)
        individual_strings.push_back(little_string);
    return individual_strings;
}

class Server_Sound_Manager{

};//fin de la clase

//clase manejadora de mensajes, contiene la lista de distribucion,
//el sistema de grupos y los condicionales para la impresion de mensajes
//o interpretacion de la consola.
class Server_Message_Manager{
private:
    unordered_map< string, string > buddys;
    unordered_map< string, list<string> > guilds;
    socket &devil_s_socket; //detesto que me limiten mi capacidad creativa
public:
    Server_Message_Manager(socket &s) : devil_s_socket(s) {
        guilds["g1"] = list<string>();
        guilds["g2"] = list<string>();
        guilds["g3"] = list<string>();
    }
    //funcion encargada de leer, interpretar y manejar la entrada de mensajes
    //retorna un booleano que indica si se debe o no hacer un send del
    //mensaje de salida, que es recibido como el segundo parametro
    void sockin(){
        message minput;
        message delivery_message;
        devil_s_socket.receive(minput);
        //se separa el comando del resto del mensaje
        string command;
        minput.get(command,1);
        //se inicia un arbol de else-if para interpretar la entrada
        if ("sld" == command){
            //saludo, registra un nuevo usuario
            string uname = minput.get(2); //nombre del usuario
            string ident = minput.get(0); //identidad
            bool ufound = ( buddys.find (uname) != buddys.end() );
            //se almacena en el mapa de sujetos
            if (!ufound){
                buddys[uname]=ident;
                //confirmacion en pantalla
                cout << "system> un cheloveco conectado, dice tener por imya: " << uname << endl;
            }else{
                delivery_message << ident << "errr" << "el nombre de usuario ya existe";
                devil_s_socket.send(delivery_message);
                cout << "system> un maluolo cheloveco quizo entrar, decia ser: " << uname << endl;
            }
        }else if("mnsj" == command){
            //mensaje, envia un mensaje
            //se prepara el remitente y eldestinatario
            string uname = minput.get(2); //nombre del que envia
            string dname = minput.get(3); //nombre del que recibe
            string diden = buddys[dname]; //identidad del que recibe
            //se arma el mensaje a enviar
            string umssg;
            for (int i=4; i < minput.parts(); i ++ ) umssg += minput.get(i) + " ";
            //salida espia, que me muestra en el servidor el paso explicito de mensajes
            if(SPYMODE) cout << "spy> mensaje de " << uname <<" para " << dname <<": "<<umssg << endl; //ojo espia
            delivery_message << diden << "mnsj" << uname << umssg;
            //se envia el mensaje a travez del socket
            devil_s_socket.send(delivery_message);
          }else if("hblr" == command){
            string uname = minput.get(2);
            string dname = minput.get(3);
            string diden = buddys[dname]; //identidad del que recibe
            int sample_rate;
            minput.get(sample_rate,4);
            int chan_count;
            minput.get(chan_count,5);
            int rsec;
            minput.get(rsec,6);
            if(SPYMODE) cout<<"spy> "<< "audio de " << uname << " para " << dname << ": " << rsec << " segundos" << endl;

            sf::Int16 chunk;

            delivery_message << diden << "hblr" << uname << sample_rate << chan_count << rsec;
            for (int i = 7; i < minput.parts(); i++ ){
              minput.get(chunk,i);
              delivery_message << chunk;
            }

            //se envia el mensaje a traves del socket
            devil_s_socket.send(delivery_message);
        }else if("unr" == command){
            //el usuario se quiere unir a un grupos
            string uname = minput.get(2); //nombre de usuario
            string gname = minput.get(3); //nombre de grupo
            //se une el usuario al grupo
            bool gfound = ( guilds.find (gname) != guilds.end() ); //se verifica que el grupo exista
            if(gfound){
                bool ufound = ( find(guilds[gname].begin(), guilds[gname].end(), uname) != guilds[gname].end() );
                if(!ufound){
                    //se añade al grupo
                    guilds[gname].push_back(uname);
                    //se envia mensaje de confirmacion
                    delivery_message << buddys[uname] << "mnsj" << SNAME << "bienvenido al grupo";
                    devil_s_socket.send( delivery_message );
                    if(SPYMODE) cout << "spy> el usuario " << uname << " se ha unido al grupo " << gname << endl;
                }else{
                  //el usuario ya existe
                  delivery_message << buddys[uname] << "mnsj" << SNAME << "el usuario ya estaba registrado en el grupo";
                  devil_s_socket.send( delivery_message );
                }
            }else{
                //el grupo no existe
                delivery_message << buddys[uname] << "mnsj" << SNAME << "el grupo especificado no existe";
                devil_s_socket.send( delivery_message );
            }
        }else if("djr" == command){
            //el usuario se quiere salir de un grupo
            string uname = minput.get(2); //nombre de usuario
            string gname = minput.get(3); //nombre de grupo
            //se une el usuario al grupo
            bool gfound = ( guilds.find (gname) != guilds.end() ); //se verifica que el grupo exista
            if(gfound){
                bool ufound = ( find(guilds[gname].begin(), guilds[gname].end(), uname) != guilds[gname].end() );
                if(ufound){
                    //se añade al grupo
                    guilds[gname].remove(uname);
                    //se envia mensaje de confirmacion
                    delivery_message << buddys[uname] << "mnsj" << SNAME << "el usuario ha abandonado el grupo felizmente";
                    devil_s_socket.send( delivery_message );
                    if(SPYMODE) cout << "spy> el usuario " << uname << " ha abandonado el grupo " << gname << endl;
                }else{
                  //el usuario no existe
                  delivery_message << buddys[uname] << "mnsj" << SNAME << "el usuario no esta registrado en el grupo";
                  devil_s_socket.send( delivery_message );
                }
            }else{
                //el grupo no existe
                delivery_message << buddys[uname] << "mnsj" << SNAME << "el grupo especificado no existe";
                devil_s_socket.send( delivery_message );
            }
        }else if("dfsn" == command){
            //alguien realiza una difusion a un grupo
            string uname = minput.get(2); //nombre de usuario
            string gname = minput.get(3); //nombre de grupo
            //se verifica que el grupo exista
            bool gfound = ( guilds.find (gname) != guilds.end() );
            if(gfound){
                bool ufound = ( find(guilds[gname].begin(), guilds[gname].end(), uname) != guilds[gname].end() );
                if(ufound){
                    //el usuario si pertenece al grupo
                    //se arma el mensaje a enviar
                    string umssg;
                    for (int i=4; i < minput.parts(); i ++ ) umssg += minput.get(i) + " ";
                    //se itera sobre los usuarios del grupo mandando mensajes en cada vuelta
                    for (string dname: guilds[gname]) {
                        string diden = buddys[dname]; //identidad del destinatario
                        message broadcast;
                        broadcast << diden << "mnsj" << gname+":"+uname << umssg;
                        devil_s_socket.send(broadcast);
                    }
                    if(SPYMODE) cout << "spy> mensaje de " << uname <<" para el grupo " << gname <<": "<<umssg << endl; //ojo espia
                }else{
                  //el usuario no pertenece al grupo
                  delivery_message << buddys[uname] << "mnsj" << SNAME << "usted no pertenece al grupo, no puede mandar mensajes";
                  devil_s_socket.send( delivery_message );
                }
            }else{
                //el grupo no existe
                delivery_message << buddys[uname] << "mnsj" << SNAME << "el grupo especificado no existe";
                devil_s_socket.send( delivery_message );
            }
        }else if("vcr" == command){
            //alguien realiza una voceria a un grupo
            string uname = minput.get(2); //nombre de usuario
            string gname = minput.get(3); //nombre de grupo
            int sr,cc,rs;
            minput.get(sr,4); //sample rate
            minput.get(cc,5); //channel count
            minput.get(rs,6); //recorded seconds
            //se verifica que el grupo exista
            bool gfound = ( guilds.find (gname) != guilds.end() );
            if(gfound){
                bool ufound = ( find(guilds[gname].begin(), guilds[gname].end(), uname) != guilds[gname].end() );
                if(ufound){
                    //el usuario si pertenece al grupo
                    //se itera sobre los usuarios del grupo mandando mensajes en cada vuelta
                    for (string dname: guilds[gname]) {
                        string diden = buddys[dname]; //identidad del destinatario
                        sf::Int16 chunk;
                        message broadcaste;
                        broadcaste << diden << "hblr" << gname+":"+uname << sr << cc << rs;
                        for (int i = 7; i < minput.parts(); i++ ){
                          minput.get(chunk,i);
                          broadcaste << chunk;
                        }
                        devil_s_socket.send(broadcaste);
                    }
                    if(SPYMODE) cout << "spy> audio de " << uname <<" para el grupo " << gname <<": "<< rs << " segundos"<< endl; //ojo espia
                }else{
                  //el usuario no pertenece al grupo
                  delivery_message << buddys[uname] << "mnsj" << SNAME << "usted no pertenece al grupo, no puede mandar mensajes";
                  devil_s_socket.send( delivery_message );
                }
            }else{
                //el grupo no existe
                delivery_message << buddys[uname] << "mnsj" << SNAME << "el grupo especificado no existe";
                devil_s_socket.send( delivery_message );
            }
        }else if ("slr"==command){
            //salir, el usuario se desconecto
            string uname = minput.get(2);
            cout << "system> el cheloveco "<< uname <<" se ha ucado" <<endl;
        }
    }//fin de funcion sockin

    //funcion encargada de tomar la entrada de la consola
    //y ejecutar los mensajes o preparar las salidas segun corresponda.
    int sockout(string console_string){
        if (2 > console_string.length()){
            cout << "system> Has escrito un maluolo comando, videa lo que escribes" <<endl;
            return 0; //termino bien, nada que hacer
        }else{
            //se separa el mensaje de la consola
            vector<string> console_inputv = explode(console_string);
            string command = console_inputv[0];
            //se inicia el arbol else-if que interpertaa la entrada de la consola
            if("mnsj" == command ){
                message out_message;
                //mensaje, se va a enviar un mensaje a un sujeto en especifico
                string dname = console_inputv[1]; //nombre del destinatario
                string diden = buddys[dname]; //identidad del destinatario
                //se prepara el mensaje de salida
                out_message << diden << "mnsj" << SNAME;
                for(int i=2; i < console_inputv.size();i++){
                    out_message << console_inputv[i];
                }
                devil_s_socket.send(out_message);
                return 0; //termino bien, nada que hacer
            }else if ("dfsn"==command){
                //se hace difusion amplia, o broadcast, enviando a todos
                string text_to_send;
                for(int i=1; i < console_inputv.size();i++){
                    text_to_send += console_inputv[i] + " ";
                }
                for(auto bud : buddys) {
                    message broadcast_message;
                    string diden = bud.second; //destinaraty identity
                    broadcast_message <<diden << "mnsj" << SNAME << text_to_send;
                    devil_s_socket.send(broadcast_message);
                }
                return 0; //termino bien, nada que hacer
            }else if ("slr"==command){
                for(auto bud : buddys) {
                    message exit_message;
                    string diden = bud.second; //destinaraty identity
                        exit_message <<diden << "slr";
                    devil_s_socket.send(exit_message);
                }
                return 1; //termino bien, pero hay que cerrar el programa
            }else if("ayd"){
                //aaaiiiuuddaaaa (a.k.a. ayuda)
                //imprime en pantalla la informacion de los comandos
                cout << "help> mnsj usuario texto_a_enviar \tenvia un mensaje a un usuario" << endl;
                cout << "help> dfsn texto_a_enviar \t\tenvia un mensaje a todos los usuarios" << endl;
                cout << "help> ayd \t\t\t\tmuestra informacion sobre comandos" << endl;
                cout << "help> slr \t\t\t\tcierra el sistema" << endl;
            }else{
                cout << "system> has escrito una chepuca, no logro ponimar lo que scasas" <<endl;
                return 0; //termino bien, nada que hacer
            }
        }
    }//fin de funcion sockout
};//fin de definicion de clase

int main(int argc, char *argv[]){
    if ( not ( 2 == argc ) ) {
        cerr << "Please, write the number of the port to serve" << endl;
        return 1;
    }
    //ETAPA DE INICIACION
    //se recibe como argumento el numero del puerto
    string port_num(argv[1]);
    //se crea el contexto
    context my_context;
    //se crea el socket, la direccion se arma con el argumento de entrada
    socket server_socket(my_context, socket_type::xreply);
    string address_to_connect("tcp://*:");
    address_to_connect += port_num;
    server_socket.bind( address_to_connect );
    //se imprimen algunos mensajes protocolarios
    cout << "system> Servidor encendido, funcionando dobo " << endl;
    cout << "help> para ver los commandos disponibles escribir 'ayd'" << endl;
    //se crea el manejador de mensajes
    Server_Message_Manager smmanager(server_socket);

    //ETAPA DE POLLING
    //se crea el poll de mensajes desde donde se leeran los mensajes
    poller deadpool; //creamos una variable masacre
    int console = fileno(stdin);
    //se añade al poll, como entrada el socket y la consola
    deadpool.add(server_socket, poller::poll_in);
    deadpool.add(console, poller::poll_in);
    //se usa un ciclo infinito de lectura
    while (true){
        if(deadpool.poll()){
            //si hay mensajes de entrada en el socket los muesta
            if(deadpool.has_input(server_socket)){
                smmanager.sockin();
            }
            //si hay mensajes en la entrada por consola
            if(deadpool.has_input(console)){
                string console_string;
                message out_message;
                //se lee una linea desde la consola y se almacena en la cadena
                getline(cin,console_string);
                if(1 == smmanager.sockout(console_string))return 0;
            }
        }
    }
    return 0;
}
