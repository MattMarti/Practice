# server.py
# 
# Opens TCP socket and accepts data from a client

import socket

def run():
    ip = "localhost"
    port = 65432
    
    # AF_INET is IPv4 protocol
    # SOCK_STREAM specifies TCP
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # Opens TCP connection
    s.bind((ip, port)) # Associates socket with specific network interface
    
    s.listen() # Enables the server to accept connections. Basically this establishes this program as a server
    connection, address = s.accept() # Waits for an incomming connection
    
    # connection is a new socket object. It is distinct from the s socket that listens for connections
    
    with connection:
        print("Connected by ", address)
        while True:
            data = connection.recv(1024) # 1024 is the buffer size
            if not data:
                break
            #
            connection.sendall(data)
            mystr = data.decode()
            print(mystr)
        #
    #
#

if __name__ == "__main__":
    run()
#