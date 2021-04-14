#!/usr/bin/env python3

import socket

HOST = '127.0.0.1'  # Standard loopback interface address (localhost)
PORT = 65432        # Port to listen on (non-privileged ports are > 1023)
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    print("Hey, How's it going? we are listening on IP",HOST," on port", PORT)
    conn, addr = s.accept()
    while True:
       
        print('Connected by', addr)
        data = conn.recv(1024)
        if data:
            print(data)
    conn.close()
