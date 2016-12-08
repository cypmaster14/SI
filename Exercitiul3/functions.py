from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import padding
from os import urandom


def encrypt_message(file_content: list, key, mode: str) -> list:
    """

    :param file_content:The file that I want to encrypt split in blocks
    :param mode:Cipher operation mode
    :param key: Enryption key that it will be use
    :return: List of encrypted block
    """
    print("Mode:", mode, len(mode))
    if mode == b"ECB":
        return encrypt_message_ECB(file_content, key)
    else:
        iv = urandom(16)
        return encrypt_message_CBC(file_content, key, iv)


def encrypt_message_ECB(text: list, key):
    """
    :param text: List of block i want to encrypt and send
    :param key: Key used for encryption
    :return: List of encrypted blocks
    """

    backend = default_backend()
    cipher = Cipher(algorithms.AES(key), modes.ECB(), backend=backend)

    encrypted_blocks = list()

    for block in text:
        encryptor = cipher.encryptor()
        if len(block) != 16:
            padder = padding.ANSIX923(128).padder()
            padded_data = padder.update(block) + padder.finalize()
            encrypted_text = encryptor.update(padded_data) + encryptor.finalize()
            encrypted_blocks.append(encrypted_text)
        else:
            encrypted_text = encryptor.update(block) + encryptor.finalize()
            encrypted_blocks.append(encrypted_text)

    return encrypted_blocks


def decrypt_message_ECB_mode(encrypted_text: list, key):
    """

    :param encrypted_text: List of block I want to decrypt
    :param key:  Key used for decryption
    :return:
    """

    backend = default_backend()
    cipher = Cipher(algorithms.AES(key), modes.ECB(), backend=backend)

    decrypted_blocks = list()

    for i in range(0, len(encrypted_text)):
        decryptor = cipher.decryptor()
        decrypted_text = decryptor.update(encrypted_text[i]) + decryptor.finalize()
        if i == len(encrypted_text) - 1:
            try:
                unpadded = padding.ANSIX923(128).unpadder()
                decrypted_text = unpadded.update(decrypted_text) + unpadded.finalize()
            except Exception as e:
                # In case if unpadder isn't necessary
                pass
        decrypted_blocks.append(decrypted_text)

    return decrypted_blocks


def sxor(s1, s2):
    # convert strings to a list of character pair tuples
    # go through each tuple, converting them to ASCII code (ord)
    # perform exclusive or on the ASCII code
    # then convert the result back to ASCII (chr)
    # merge the resulting array of characters as a string

    xor_message = bytes('', encoding="utf-8")
    for i in range(0, len(s1)):
        xor = s1[i] ^ s2[i]
        aux = xor.to_bytes((xor.bit_length() + 7) // 8, "big")
        if len(aux) == 0:
            xor_message += b"\x00"
        else:
            xor_message += aux
    return xor_message


def encrypt_message_CBC(text: list, key, iv):
    """

    :param text: List of blocks I want to encrypt and send
    :param key: Key used for encryption
    :param iv: Initialization Vector
    :return:
    """

    encrypted_blocks = list()

    backend = default_backend()
    cipher = Cipher(algorithms.AES(key), modes.ECB(), backend=backend)

    encrypted_blocks.append(iv)
    for i in range(0, len(text)):
        encryptor = cipher.encryptor()
        plainText = text[i]
        encrypted_data = encryptor.update(encrypted_blocks[i]) + encryptor.finalize()
        if len(plainText) != 16:
            padder = padding.ANSIX923(128).padder()
            plainText = padder.update(plainText) + padder.finalize()

        cipherText = sxor(plainText, encrypted_data)
        encrypted_blocks.append(cipherText)

    return encrypted_blocks


def decrypt_message_CFB_node(encrypted_text: list, key):
    """

    :param encrypted_text: List of encrypted texts
    :param key: The key which i use to decrypt each block
    :return:
    """

    plain_text_block = list()

    backend = default_backend()
    cipher = Cipher(algorithms.AES(key), modes.ECB(), backend=backend)

    for i in range(len(encrypted_text) - 1, 0, -1):
        encryptor = cipher.encryptor()
        decrypted_data = encryptor.update(encrypted_text[i - 1]) + encryptor.finalize()
        plain_text = sxor(encrypted_text[i], decrypted_data)
        plain_text_block.append(plain_text)
    try:

        unpadder = padding.ANSIX923(128).unpadder()
        plain_text_block[0] = unpadder.update(plain_text_block[0]) + unpadder.finalize()
    except Exception as e:
        # In case if unpadder isn't necessary
        pass

    return list(reversed(plain_text_block))


def print_message(messages: list):
    original_message = ""
    for message in messages:
        original_message += message.decode("UTF-8")
    print(original_message)
