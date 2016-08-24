/* this example shows how you can scale vertically over all CPU cores */
/* threading is up to the user, so you could listen to one port or a range */
/* depending on what you want, I don't care what you chose to do */

#include <iostream>
#include <string>
#include <boost/thread/thread.hpp>
using namespace std;

#include <uWS.h>
using namespace uWS;

#define THREADS 4
Server *threadedServer[THREADS];

int main()
{
    try {
        // you need at least one server listening to a port
        EventSystem es(MASTER);
        Server server(es, 3000);

        server.onUpgrade([](uv_os_sock_t fd, const char *secKey, void *ssl, const char *extensions, size_t extensionsLength) {
            // we transfer the connection to one of the other servers
            threadedServer[rand() % THREADS]->upgrade(fd, secKey, ssl, extensions, extensionsLength);
        });

        // launch the threads with their servers
        for (int i = 0; i < THREADS; i++) {
            new boost::thread([i]{
                EventSystem tes(WORKER);
                threadedServer[i] = new Server(tes, 0);

                // register our events
                threadedServer[i]->onConnection([=](WebSocket socket) {
                    cout << "Connection on thread " << i << endl;
                });

                threadedServer[i]->onDisconnection([=](WebSocket socket, int code, char *message, size_t length) {
                    cout << "Disconnection on thread " << i << endl;
                });

                threadedServer[i]->onMessage([=](WebSocket socket, char *message, size_t length, OpCode opCode) {
                    cout << "Message on thread " << i << ": " << string(message, length) << endl;
                    socket.send(message, length, opCode);
                });

                tes.run();
            });
        }

        // run listener
        es.run();
    } catch (...) {
        cout << "ERR_LISTEN" << endl;
    }

    return 0;
}
