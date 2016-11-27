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
            return ("ECB", "")
        elif mode.upper() == "CFB":
            return ("CFB", "")
