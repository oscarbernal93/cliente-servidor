//incluimos las bibliotecas
#include <iostream>
#include <zmqpp/zmqpp.hpp>
#include "argos.hpp"

//definimos los tipos de operacion
//#DEFINE UNARY_OP 1
//#DEFINE BINARY_OP 2

using namespace std;
using namespace zmqpp;

int main(int argc, char const *argv[]) {
  //Si no tiene exactamente 4 o 3 argumentos, va a hacer algo que no le va a gustar, se muere :c
  if ( not ( argc == 4  or argc == 3 ) ) {
    cerr << "Please verify (if you wish) the amount of arguments" << endl;
    return 1;
  }
  //Creamos la misteriosa caja oscura (que alberga terribles secretos) a la que carinosamente llamamos contexto
  context ctx;
  //y un socket, sea lo que sea que traduzca en espanol
  socket s(ctx, socket_type::request);
  //Informamos lo que hacemos pa' que el susodicho usuario no se aburra y se vaya.
  cout << "Opening connection" << endl;
  //nos conectamos a el hogar (dulce hogar) en el puerto 42-42 (la respuesta)
  s.connect("tcp://localhost:4242");
  //recolectamos, cual de una cosecha se tratase, los argumentos
  string op(argv[1]);
  int op1 = atoi(argv[2]);
  int op2 = 0;
  if (strcmp)
  if (argc == 4){
    op2 = atoi(argv[3]);
  }

  message req;
  req << op << op1 << op2;
  s.send(req);
  cout << "Request sent." << endl;

  message rep;
  s.receive(rep);
  int result = 0;
  rep >> result;
  cout << "Response " << result << endl;

  cout << "Finished." << endl;
  return 0;
}
