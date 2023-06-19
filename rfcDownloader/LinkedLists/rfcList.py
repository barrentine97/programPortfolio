from multiprocessing import shared_memory
import pickle

class RFCList:
    # start of linked list with reference to the head
    # also uses block of shared memory
    def __init__(self):
        self.head = None
        self.sharedMem = shared_memory.SharedMemory(name="sharedRfcList", create=True, size=1024)
        self.updateSharedMem()

    # returns the rfcList associated with the block of sharedMem
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
    
    # returns a list of hosts that have added the given
    # rfc number and title
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
    
    # returns a list of all rfcs added
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

    # updates the block of shared memory with
    # this rfcList
    def updateSharedMem(self):
        pickledData = pickle.dumps(self)
        self.sharedMem.buf[:len(pickledData)] = pickledData
    
    # unlinks this list from the block of shared memory
    def cleanup(self):
        self.sharedMem.unlink()

# a node in the linked rfcList containing the
# rfc number, rfc title, hostname and port number of
# the peer that has added this rfc, and a reference to
# the next node in the list
class rfc:
    def __init__(self, number, title, hostname, port):
        self.number = number
        self.title = title
        self.hostname = hostname
        self.port = port
        self.next = next
