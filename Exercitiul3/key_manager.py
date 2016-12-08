from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend


class KeyManager(object):
    def __init__(self, k1, k2, k3):
        """

        :param k1: Key used for ECB mode
        :param k2: Key used for CFB mode
        :param k3: Key used to encrypt the key used for one of the above modes
        """
        self.k1 = k1
        self.k2 = k2
        self.k3 = k3

    def get_key(self, mode: str):
        """

        :param mode:The mode the client want to use the cryptosystem
        :return: The encrypted key for the requested mode
        """
        if mode.upper() == "ECB":
            return self.crypt_key(self.k1)
        elif mode.upper() == "CFB":
            return self.crypt_key(self.k2)
        else:
            raise Exception("The mode you want is not available")

    def crypt_key(self, value):
        backend = default_backend()
        cipher = Cipher(algorithms.AES(self.k3), modes.ECB(), backend=backend)
        encryptor = cipher.encryptor()
        cipherText = encryptor.update(value) + encryptor.finalize()
        return cipherText
