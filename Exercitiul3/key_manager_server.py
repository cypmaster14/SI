import os
import socket
from key_manager import KeyManager

IP = "127.0.0.1"
PORT = 7123
k3 = b'\x17\xc7}\xd3\xe9,\x00\xa1\x03\x82\xbf\x072\xea\xdaW'
try:

    KM = KeyManager(os.urandom(16), os.urandom(16), k3)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind((IP, PORT))
    sock.listen(5)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 5)
    print("Key Manager.Waiting connections at:{ip}:{port}".format(ip=IP, port=PORT))
    while True:
        (connection, address) = sock.accept()
        print("Received connection:", address)
        received_data = connection.recv(3)
        print("Received data:", received_data, )
        if b"ECB" == received_data:
            print("Request for ECB key")
            send_message = KM.get_key("ECB")
            connection.send(send_message)
        elif b"CFB" == received_data:
            print("Request for CFB key")
            send_message = KM.get_key("CFB")
            connection.send(send_message)
        elif "exit" in received_data:
            break
    print("Key manager shutting down")

except KeyboardInterrupt as e:
    sock.close()
