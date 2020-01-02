C++ App.

1. Two threads: client and server (+ main thread, which launches them).
2. Client and server exchange data over sockets by UDP protocol.
3. IP addresses are const.
4. Client sends request to server each second, server sends response (random number).
5. Client and server log requests and answers to console.
6. Client works 30 seconds and then sends message to server about shut down.
7. Threads of client and server are terminated and programm closes.
