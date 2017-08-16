#include <iostream>
#include <string>
#include <zmqpp/zmqpp.hpp>

using namespace std;
using namespace zmqpp;

int main() {
	cout << "This is the client\n";

	context ctx;
	socket s(ctx, socket_type::req);

	cout << "Connecting to tcp port 5555\n";
	s.connect("tcp://192.168.8.66:5555");//Puerto del cumputador dell profesor

	cout << "Sending Message..\n";
	message m;
	m << "Lorem ipsum dolor sit amet...\n";

	s.send(m);

	int i;
	cin >> i;
       	cout << "Finished\n";
	return 0;
}