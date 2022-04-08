# Multiple Client Handling TCP Server in C++

The aim of this project is to set up a TCP based server for multiple clients that can send and receive messages.

The server should be able to: 

     - connect by IP address and port number
     - listen for new connections (at least 4)
     - accept the requests of different devices
     - give full access to all its files/file content to at least one user
     
The client should be able to:

      - connect to the server through a socket
      - connect to the server using the exact IP address and port
      - read the responses sent from the server
      - send messages to the server in text form
    
One client (user) should have write(), read(), and execute() privileges while the other users must have only read() permission. 
