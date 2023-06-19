from multiprocessing import shared_memory
import pickle

class RFCList:
    def __init__(self):
        self.head = None
        self.sharedMem = shared_memory.SharedMemory(name="sharedRfcList", create=True, size=1024)
        self.updateSharedMem()

    def getSharedList(sharedMem):
        rfcList = sharedMem.buf
        return pickle.loads(rfcList)

    # adds a new RFC to the front of the list
    def addRfc(self, number, title, hostname, port):
        newRfc = rfc(number, title, hostname, port)
        newRfc.next = self.head
        self.head = newRfc
        self.updateSharedMem()

    # removes all RFCs associated with the given clientAddress
    def removeAll(self, hostname, uploadPort):
        while self.head is not None and self.head.hostname == hostname and self.head.port == uploadPort:
            self.head = self.head.next

        current = self.head
        while current is not None and current.next is not None:
            if current.next.hostname == hostname and current.next.port == uploadPort:
                current.next = current.next.next
            else:
                current = current.next
        self.updateSharedMem()
    
    def lookup(self, number, title):
        hosts = []
        current = self.head
        if current is None:
            hosts
        while current is not None:
            if current.number == number and current.title == title:
                host = []
                host.append(current.hostname)
                host.append(current.port)
                hosts.append(host)
            current = current.next
        return hosts
    
    def getAll(self):
        rfcs = []
        current = self.head
        if current is None:
            return
        while current is not None:
            rfc = []
            rfc.append(current.number)
            rfc.append(current.title)
            rfc.append(current.hostname)
            rfc.append(current.port)
            rfcs.append(rfc)
            current = current.next
        
        return rfcs

    def updateSharedMem(self):
        pickledData = pickle.dumps(self)
        self.sharedMem.buf[:len(pickledData)] = pickledData
    
    def cleanup(self):
        self.sharedMem.unlink()

class rfc:
    def __init__(self, number, title, hostname, port):
        self.number = number
        self.title = title
        self.hostname = hostname
        self.port = port
        self.next = next
