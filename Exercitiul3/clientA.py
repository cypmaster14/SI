import socket
import sys
import time

from comunication import *
from functions import *
from person import Person

k3 = b'\x17\xc7}\xd3\xe9,\x00\xa1\x03\x82\xbf\x072\xea\xdaW'
KM_IP = "127.0.0.1"
KM_PORT = 7123

A_IP = "127.0.0.1"
A_PORT = 7122

chosen_mode = ""
file = ""
try:
    chosen_mode = bytes(sys.argv[1].encode("UTF-8"))
    file = sys.argv[2]
except Exception as e:
    print(e)
    exit(-1)

sock_key_manager = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock_key_manager.connect((KM_IP, KM_PORT))
sock_key_manager.send(chosen_mode)
encrypted_encryption_key = sock_key_manager.recv(100)
encryption_key = decrypt_encryption_key(encrypted_encryption_key, k3)
print("[ClientA]Encryption key:", encryption_key)
sock_key_manager.close()

sock_client_A = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock_client_A.bind((A_IP, A_PORT))
sock_client_A.listen(5)
sock_client_A.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 5)

while True:
    (connection_B, address) = sock_client_A.accept()
    connection_B.send(chosen_mode)
    confirm_message_B = connection_B.recv(100).decode("UTF-8")
    print("[ClientA]Confirm message from B:", confirm_message_B)
    if "I'm ready" in confirm_message_B:
        A = Person(file, encryption_key)
        encrypted_messages_for_B = encrypt_message(A.fileContent, encryption_key, chosen_mode)
        connection_B.send(bytes(str(len(encrypted_messages_for_B)).encode("UTF-8")))
        for encrypted_message in encrypted_messages_for_B:
            time.sleep(0.05)
            connection_B.send(encrypted_message)
        print("[ClientA] Crypted block have been sended")
        break
sock_client_A.close()
