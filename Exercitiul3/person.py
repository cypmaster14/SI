class Person:
    def __init__(self, file_path, key):
        """

        :param file_path:Path to the file i want to send to B
        :param key: The key used to encrypt the messages i want to send
        """
        self.file_path = file_path
        self.key = key
        self.fileContent = self.read_file(file_path)

    def read_file(self, file_path) -> list:
        """
        :param file_path: Path to the file i want to encrypt
        :return:
        """

        fileObject = open(file_path, mode="rb")
        fileContent = list()
        while True:
            content = fileObject.read(16)
            if len(content) == 0:
                break
            fileContent.append(content)
        return fileContent
