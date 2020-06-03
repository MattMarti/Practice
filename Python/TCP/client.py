# client.py
# 
# Opens TCP socket and prints whatever it receives.

import socket

def run():
    ip = "localhost"
    port = 65432
    
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((ip, port))
    s.sendall(b'Hello LocalHost!')
    data = s.recv(1024)  # 1024 is the buffer size
    
    mystr = ""
    while not mystr == "q":
        print(">> Enter value: ", end='')
        mystr = input();
        s.sendall(str.encode(mystr))
        data = s.recv(1024)  # 1024 is the buffer size
    #
#

if __name__ == "__main__":
    run()
#