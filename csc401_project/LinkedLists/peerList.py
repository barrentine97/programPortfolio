from multiprocessing import shared_memory
import pickle

class PeerList:
    def __init__(self):
        self.head = None
        self.sharedMem = shared_memory.SharedMemory(name="sharedPeerList", create=True, size=1024)
        self.updateSharedMem()
    
    def getSharedList(sharedMem):
        peerList = sharedMem.buf
        return pickle.loads(peerList)

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

    def updateSharedMem(self):
        pickledData = pickle.dumps(self)
        self.sharedMem.buf[:len(pickledData)] = pickledData
    
    def cleanup(self):
        self.sharedMem.unlink()

class Peer:
    def __init__(self, hostname, port):
        self.hostname = hostname
        self.port = port
        self.next = next
        