#include <iostream>
#include <string>
#include <zmqpp/zmqpp.hpp>

using namespace std;
using namespace zmqpp;

int main(int argc, char *argv[]) {
  // initialize the 0MQ context
  context ctx;

  // generate a reply socket
  socket s(ctx, socket_type::reply);

  // bind to the socket
  cout << "Binding " << endl;
  s.bind("tcp://*:4242");

  while (true) {
    // receive the message
    cout << "Receiving message..." << endl;
    message req;
    s.receive(req);

    // Declaramos los tipos de valores a recibir
    string op;
    int op1 = 0;
    int op2 = 0;
    req >> op >> op1 >> op2;
    int result = 0;

    if(op == "suma"){
        cout << "comparacion exitosa!" << endl;
        result = op1 + op2;
    }
    
    if(op == "producto"){
	result = op1 * op2;
    }

    if(op == "pot2"){
	result = op1 * op1;
    }

    if(op == "factorial"){
	result = 1;
	if(op1 > 0){
	    for(int i = op1; i >= 1; i--){
		result *= i;
	    }
	}
    }

    message rep;
    rep << result;
    s.send(rep);
    cout << "Sent: " << result << endl;
  }
  cout << "Finished." << endl;
}
