import socket
import time

from comunication import *
from functions import *

k3 = b'\x17\xc7}\xd3\xe9,\x00\xa1\x03\x82\xbf\x072\xea\xdaW'
KM_IP = "127.0.0.1"
KM_PORT = 7123

A_IP = "127.0.0.1"
A_PORT = 7122

sock_client_A = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock_client_A.connect((A_IP, A_PORT))
mode_wanted = sock_client_A.recv(100)
print("[ClientB]ClientA wants mode:", mode_wanted)

time.sleep(2)
sock_key_manager = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock_key_manager.connect((KM_IP, KM_PORT))
sock_key_manager.send(mode_wanted)
encrypted_encryption_key = sock_key_manager.recv(100)
encryption_key = decrypt_encryption_key(encrypted_encryption_key, k3)
print("[ClientB]Encryption key:", encryption_key)
sock_key_manager.close()

sock_client_A.send(b"I'm ready")

number_of_blocks = int(sock_client_A.recv(8).decode("UTF-8"))
print("Number of blocks:", number_of_blocks)
encrypted_blocks = list()
for i in range(0, number_of_blocks):
    encrypted_block = sock_client_A.recv(16)
    print(encrypted_block)
    print(len(encrypted_block))
    encrypted_blocks.append(encrypted_block)

if mode_wanted == b"ECB":
    decrypted_blocks = decrypt_message_ECB_mode(encrypted_blocks, encryption_key)
else:
    decrypted_blocks = decrypt_message_CFB_node(encrypted_blocks, encryption_key)

print_message(decrypted_blocks)

sock_client_A.close()
