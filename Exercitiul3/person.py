

class Person:

    def __init__(self,file_path,k3):
        """

        :param file_path:Path to the file i want to send to B
        :param k3:  The key I will use to decrypt the encrypted key( k1) send by KM
        """
        self.file_path=file_path
        self.k3=k3
