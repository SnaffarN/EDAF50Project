#include "connection.h"
#include "connectionclosedexception.h"
#include "server.h"

#include <iostream>

using namespace std;


Server init(int argc, char* argv[])
{
        if (argc != 2) {
                cerr << "Usage: myserver port-number" << endl;
                exit(1);
        }

        int port = -1;
        try {
                port = stoi(argv[1]);
        } catch (exception& e) {
                cerr << "Wrong format for port number. " << e.what() << endl;
                exit(2);
        }

        Server server(port);
        if (!server.isReady()) {
                cerr << "Server initialization error." << endl;
                exit(3);
        }
        return server;
}

int main(int argc, char* argv[])
{
        auto server = init(argc, argv);
}