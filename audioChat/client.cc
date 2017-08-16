#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <sstream>
#include <SFML/Audio.hpp>
#include <thread>
#include <chrono>

using namespace zmqpp;
using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

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

//la funcion grabar escribe en las referencias de los parametros (chunks,sample_rate y chan_count)
//el sonido grabado durante rsec segundos.
bool record(vector<sf::Int16> &msamples, int &sample_rate, int &chan_count, int rsec){
    if(sf::SoundBufferRecorder::isAvailable()){
        sf::SoundBufferRecorder recorder;
        recorder.start(); //comienza a grabar
        sleep_for(seconds(rsec)); //durante algunos segundos
        recorder.stop();
        const sf::SoundBuffer &buff = recorder.getBuffer();
        //se escribe en el vector el muestreo de la grabacion
        msamples.assign(buff.getSamples() , buff.getSamples() + buff.getSampleCount() );
        sample_rate = buff.getSampleRate();
        chan_count = buff.getChannelCount();
        return true;
    }else{
        cerr << "Resource Unavailable" << endl;
        return false;
    }
}//fin de la funcion grabar

//la funcion reproducir arma un buffer a partir de los chunks y la informacion proporcinada
//luego pone a sonar y congela el hilo hasta que termina de reproducir.
bool play(vector<sf::Int16> chunks, int sample_rate, int chan_count, int psec ){ //chanca, chanca chanca :P
    sf::SoundBuffer tmpbuff;
    tmpbuff.loadFromSamples(&chunks[0], chunks.size(), chan_count, sample_rate );

    sf::Sound tmpsound(tmpbuff);
    tmpsound.play();
    sleep_for(seconds(psec));
    return true;
} //fin de la funcion reproducir

class Client_Message_Manager{
private:
    string user_name;  //nombre del usuario
    unordered_map<string,string> buddys;
    socket &anarch_socket; //no al sistema opresor nombrador de variables
public:
    Client_Message_Manager(socket &s,string un) : anarch_socket(s),user_name(un) {}
    //funcion encargada de leer, interpretar y manejar la entrada de mensajes desde el socket
    int sockin(){
        message socket_message;
        anarch_socket.receive(socket_message);
        //se leen las partes del mensaje
        string command;
        socket_message.get(command,0);
        if("mnsj" == command){
            string uname = socket_message.get(1);
            string umssg;
            for (int i=2; i < socket_message.parts(); i ++ ) umssg += socket_message.get(i) + " ";
            cout << uname <<"> " << umssg << endl;
            return 0;
        } else if ("hblr" == command){

            string uname = socket_message.get(1);
            int sr; //sample_rate
            socket_message.get(sr,2);
            int cc; //chan_count
            socket_message.get(cc,3);
            int seg; //segundos
            socket_message.get(seg,4);

            vector<sf::Int16> vmuestra;
            sf::Int16 little_chunk;
            for (int i = 5; i < socket_message.parts(); i ++) {
              socket_message.get(little_chunk,i);
              vmuestra.push_back(little_chunk);
            }
            cout << uname << "> " << "audio de " << seg << "segundos" << endl;
            play(vmuestra, sr, cc, seg);
            return 0;
        } else if ("slr"==command){
            cout << "system> el servidor se ha snufado, ahora debemos ucadirnos" <<endl;
            return 1; //todo bien, pero hay que salir
        } else if ("errr"==command){
            cout << "system> el nombre de usuario ya se encuentra registrado, intente con otro" <<endl;
            return 1; //todo bien, pero hay que salir
        }
    }//fin de funcion sockin
    //funcion encargada de interpretar lo escrito en la consola y enviar al servidor
    int sockout(){
        string console_string; //creamos una cadena
        //se lee una linea desde la consola y se almacena en la cadena
        getline(cin,console_string); //se obtienen los comandos desde la consola
        if (2 > console_string.length()){
            cout << "system> Has escrito un maluolo comando, videa lo que escribes" <<endl;
        }else{
            //se separa el mensaje de la consola
            vector<string> console_inputv = explode(console_string);
            if("mnsj" == console_inputv[0] ){
                message arecibo_message; //preparamos el mensaje para los alienigenas
                arecibo_message << "mnsj" <<user_name;
                for(int i=1; i < console_inputv.size();i++){
                    arecibo_message << console_inputv[i];
                }
                anarch_socket.send(arecibo_message); //se envia el mensaje
                return 0;
            }else if ("hblr"==console_inputv[0]){
                string dname(console_inputv[1]); //nombre del destinatario
                int rsec = stoi(console_inputv[2]); //tiempo de grabacion
                message arecibo_message; //preparamos el mensaje para los alienigenas
                //se crean las variables para grabar el audio
                vector<sf::Int16> msamples;
                int sample_rate, chan_count;
                //se intenta grabar
                if (!record(msamples,sample_rate,chan_count,rsec)){
                    return 1; //todo bien pero hay que salir
                }
                //se escribe la inforamacion de cabecera en el mensaje
                arecibo_message << "hblr" << user_name << dname << sample_rate << chan_count << rsec;
                //Probaremos si envia la muestra
                for(sf::Int16 chunk : msamples ) arecibo_message << chunk;
                anarch_socket.send(arecibo_message); //se envia el mensaje
                return 0;
            }else if("unr" == console_inputv[0]){
                string group_name(console_inputv[1]);
                message arecibo_message; //preparamos el mensaje para los alienigenas
                arecibo_message << "unr" << user_name << group_name;
                anarch_socket.send(arecibo_message); //se envia el mensaje
                return 0;
            }else if("djr" == console_inputv[0]){
                string group_name(console_inputv[1]);
                message arecibo_message; //preparamos el mensaje para los alienigenas
                arecibo_message << "djr" << user_name << group_name;
                anarch_socket.send(arecibo_message); //se envia el mensaje
                return 0;
            }else if("dfsn" == console_inputv[0]){
                string group_name(console_inputv[1]);
                message arecibo_message; //preparamos el mensaje para los alienigenas
                arecibo_message << "dfsn" << user_name << group_name;
                for(int i=2; i < console_inputv.size();i++){
                    arecibo_message << console_inputv[i];
                }
                anarch_socket.send(arecibo_message); //se envia el mensaje
                return 0;
            }else if("vcr" == console_inputv[0]){
                string group_name(console_inputv[1]);
                int rsec = stoi(console_inputv[2]); //tiempo de grabacion
                message arecibo_message; //preparamos el mensaje para los alienigenas
                arecibo_message << "vcr" << user_name << group_name;
                //se crean las variables para grabar el audio
                vector<sf::Int16> msamples;
                int sample_rate, chan_count;
                //se intenta grabar
                if (!record(msamples,sample_rate,chan_count,rsec)){
                  return 1; //todo bien pero hay que salir
                }
                //se escribe la inforamacion de cabecera en el mensaje
                arecibo_message << sample_rate << chan_count << rsec;
                //se envia la muestra
                for(sf::Int16 chunk : msamples ) arecibo_message << chunk;
                anarch_socket.send(arecibo_message); //se envia el mensaje
                return 0;
            }else if ("slr"==console_inputv[0]){
                message arecibo_message; //preparamos el mensaje para los alienigenas
                arecibo_message << "slr" <<user_name;
                anarch_socket.send(arecibo_message); //se envia el mensaje
                return 1; //todo bien pero hay que salir
            }else if ("ayd"==console_inputv[0]){
                cout << "help> mnsj usuario texto_a_enviar \tenvia un mensaje a un usuario" << endl;
                cout << "help> hblr usuario segundos_a_grabar \tgraba un audio y lo envia" << endl;
                cout << "help> dfsn grupo texto_a_enviar \tenvia un mensaje al grupo" << endl;
                cout << "help> vcr grupo segundos_a_grabar \tenvia un audio al grupo" << endl;
                cout << "help> unr nombre_grupo \t\t\tpermite unirse a un grupo" << endl;
                cout << "help> djr nombre_grupo \t\t\tpermite salirse de un grupo" << endl;
                cout << "help> ayd \t\t\t\tmuestra informacion sobre comandos" << endl;
                cout << "help> slr \t\t\t\tcierra el sistema y apaga el servidor" << endl;
                return 0;
            }else{
                cout << "system> has escrito una chepuca, no logro ponimar lo que scasas" <<endl;
            }
         }
    }//fin de funcion sockout
};//fin de definicion de clase


int main(int argc, char *argv[]){
    if ( not ( 3 == argc ) ) {
    cerr << "Wrong number of arguments, remember to provide the server's ip and your username" << endl;
    return 1;
  }
    //FASE DE INICIACION
    //se toma de los parametros la direccion y el nombre de usuario
    string connection_address(argv[1]);
    string user_name(argv[2]);
    //se inicia con el contexto, la misteriosa caja negra
    context my_context;
    //generamos el socket, que al igual que un muelle recibira mensajes, como los mensajes/mercancias
    //provenientes del mundo griego hacia el egipto pre-alejandrino (el puerto canopus)
    socket canopus_socket(my_context, socket_type::xrequest);
    string ip_to_connect("tcp://");
    ip_to_connect += connection_address;
    //se construye a direccion a conectarse con el protocolo y la direccion ip ingresada
    canopus_socket.connect( ip_to_connect );
    //mensajes
    cout << "system> Cliente inicializado, funcionando dobo " << endl;
    cout << "help> para ver los commandos disponibles escribir 'ayd'" << endl;
    //en cuanto se conecta envia un "saludo" al servidor
    message greetings("sld");
    greetings << user_name;
    canopus_socket.send(greetings);
    cout << "system> Conectado, ahora slusando dobomente al servidor" << endl;
    //se crea el manejador de mensajes
    Client_Message_Manager cmmanager(canopus_socket,user_name);

    //FASE DE POOLING
    //se crea el poll de mensajes desde donde se leeran los mensajes (valga la redundancia) tanto de consola como de sockets
    poller deadpool; //creamos una variable a la que cariñosamente denominaremos "masacre"
    int console = fileno(stdin);
    //se añade al poll, como entrada el socket y la consola
    deadpool.add(canopus_socket, poller::poll_in);
    deadpool.add(console, poller::poll_in);
    //se usa un ciclo infinito de lectura
    while (true){
        if(deadpool.poll()){
            //si hay mensajes de entrada en el socket los muesta
            if(deadpool.has_input(canopus_socket)){
                if(1 == cmmanager.sockin()){
                    return 0;
                }
            }
            //si hay mensajes en la entrada por consola
            if(deadpool.has_input(console)){
                if(1 == cmmanager.sockout()){
                    return 0;
                }
            }
        }
    }

    return 0;
}
