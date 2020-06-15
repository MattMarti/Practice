# server_multi.py
# 
# Opens TCP socket and accepts data from multiple clients and prints
# to command line

import socket
import selectors
import types

def run():
    host = "localhost"
    port = 65432
    
    # AF_INET is IPv4 protocol
    # SOCK_STREAM specifies TCP
    sel = selectors.DefaultSelector() # Set listener
    lsock = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # Opens TCP connection
    lsock.bind((host, port)) # Associates socket with specific network interface
    
    lsock.listen() # Enables the server to accept connections. Basically this establishes this program as a server
    print("Listenting on ", (host, port));
    
    # Configure the socket to non-blocking mode. Calls made to socket will no longer block.
    lsock.setblocking(False)
    sel.register(lsock, selectors.EVENT_READ, data = None)
    
    def accept_wrapper(sock):
        connection, address = sock.accept()
        print("Accepted connection from ", address)
        connection.setblocking(False) # Set non-blocking so the server never 'hangs'
        data = types.SimpleNamespace(address=address, inb=b'', outb=b'')
        events = selectors.EVENT_READ | selectors.EVENT_WRITE
        sel.register(connection, events, data=data)
    #
    
    def service_connection(key, mask):
        sock = key.fileobj
        data = key.data
        if mask & selectors.EVENT_READ: # If socket is ready for reading
            recv_data = sock.recv(1024)
            if recv_data:
                data.outb += recv_data
            else:
                print("Closing connection to", data.address)
                sel.unregister(sock)
                sock.close()
            #
        #
        if mask & selectors.EVENT_WRITE:
            if data.outb:
                print("Echo from", data.address, ":", repr(data.outb))
                sent = sock.send(data.outb)
                data.outb = data.outb[sent:]
            #
        #
    #
    
    # Event loop
    while True:
        events = sel.select(timeout = None)
        for key, mask in events:
            if key.data is None:
                accept_wrapper(key.fileobj)
            else:
                service_connection(key, mask)
            #
        #
    #
#

if __name__ == "__main__":
    run()
#