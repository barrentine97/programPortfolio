from multiprocessing import shared_memory
import pickle

class PeerList:
    # start of linked list with reference to the head
    # also uses block of shared memory
    def __init__(self):
        self.head = None
        self.sharedMem = shared_memory.SharedMemory(name="sharedPeerList", create=True, size=1024)
        self.updateSharedMem()
    
    # returns the peerList associated with the block of sharedMem
    def getSharedList(sharedMem):
        peerList = sharedMem.buf
        return pickle.loads(peerList)

    # returns the port number associated with the
    # given hostname
    def getPortNumber(self, hostname):
        if self.head is None:
            return
        if self.head.hostname == hostname:
            return self.head.port
        current = self.head
        while current is not None:
            if current.hostname == hostname:
                return current.port
            current = current.next

    # adds a new peer to the front of the list
    def addPeer(self, hostname, port):
        newPeer = Peer(hostname, port)
        newPeer.next = self.head
        self.head = newPeer
        self.updateSharedMem()

    # removes a peer with the given hostname
    def removePeer(self, hostname, uploadPort):
        if self.head is None:
            return
        if self.head.hostname == hostname and self.head.port == uploadPort:
            self.head = self.head.next
            self.updateSharedMem()
            return
        current = self.head
        while current.next is not None:
            if current.next.hostname == hostname and current.next.port == uploadPort:
                current.next = current.next.next
                self.updateSharedMem()
                return
            current = current.next

    # updates the block of shared memory with
    # this peerList
    def updateSharedMem(self):
        pickledData = pickle.dumps(self)
        self.sharedMem.buf[:len(pickledData)] = pickledData
    
    # unlinks this list from the block of shared memory
    def cleanup(self):
        self.sharedMem.unlink()

# a node in the linked peerList containing the
# hostname, port number, and a reference to the next
# node in the list
class Peer:
    def __init__(self, hostname, port):
        self.hostname = hostname
        self.port = port
        self.next = next
        