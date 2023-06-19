import sys
import os
current_dir = os.path.dirname(os.path.abspath(__file__))
parent_dir = os.path.join(current_dir, "..")
sys.path.append(parent_dir)

from multiprocessing import shared_memory

from LinkedLists.peerList import PeerList
from LinkedLists.rfcList import RFCList
from Status.status import Status

from socket import *
import signal

# unlinks the linked lists from the block of shared memory,
# closes the serverSocket, and exits
def disconnect(signal, frame):
    print("\ndisconnecting...")
    serverSocket.close()
    peerList.cleanup()
    rfcList.cleanup()
    exit(0)

# removes the client and all associated RFC information
# from the list of peers when a client tries to disconnect
def disconnectClient(hostname, uploadPort, shmPeerList, shmRfcList):
    print("Disconnecting client: ", clientAddress)
    # remove the client from the list of peers
    peerList = PeerList.getSharedList(shmPeerList)
    peerList.removePeer(hostname, uploadPort)
    # remove the client's RFCs from the list of RFCs
    rfcList = RFCList.getSharedList(shmRfcList)
    rfcList.removeAll(hostname, uploadPort)

# processes a new client using a separate process
def processRequest(connectionSocket, clientAddress):

    # add the new peer to the list of peers
    hostname = clientAddress[0]
    uploadPort = int(connectionSocket.recv(1024).decode())

    # access the shared data structures
    shmPeerList = shared_memory.SharedMemory(name="sharedPeerList")
    shmRfcList = shared_memory.SharedMemory(name="sharedRfcList")

    # add the new peer to the shared list
    peerList = PeerList.getSharedList(shmPeerList)
    peerList.addPeer(hostname, uploadPort)
    
    while True:
        # receive the client's request
        data = connectionSocket.recv(1024)
        if not data:
            # if the client disconnects, remove it
            disconnectClient(hostname, uploadPort, shmPeerList, shmRfcList)
            return
        
        # otherwise, parse the request
        lines = data.decode().split("\r\n")
        fieldLine = lines[0].split(" ")
        method = fieldLine[0]

        # if the client wants to exit, disconnect the client
        if method == "EXIT":
            disconnectClient(hostname, uploadPort, shmPeerList, shmRfcList)
            return

        if method == "ADD":
            # if adding, parse the request
            rfcNumber = fieldLine[2]
            portLine = lines[2].split(": ")
            port = int(portLine[1])
            titleLine = lines[3].split(": ")
            title = titleLine[1]
            # add the RFC to the list
            rfcList = RFCList.getSharedList(shmRfcList)
            rfcList.addRfc(rfcNumber, title, hostname, port)
            version = fieldLine[3]
            # set up the response, which is just echoing the information back to the client
            response = version + " " + str(Status.OK.value) + " " + str(Status.OK.name) + "\r\n"
            response += "RFC " + rfcNumber + " " + title + "\r\n"
            # send it to the client
            connectionSocket.send(response.encode())

        elif method == "LOOKUP":
            # if looking up, parse the request
            rfcNumber = fieldLine[2]
            titleLine = lines[3].split(": ")
            title = titleLine[1]
            version = fieldLine[3]
            # search through the list of RFCs
            rfcList = RFCList.getSharedList(shmRfcList)
            hosts = rfcList.lookup(rfcNumber, title)
            # if there are no RFCs, return NOT_FOUND error
            if len(hosts) == 0:
                response = version + " " + str(Status.NOT_FOUND.value) + " " + str(Status.NOT_FOUND.name) + "\r\n"
                # and send it to the client
                connectionSocket.send(response.encode())
            else:
                # otherwise, set up the response
                response = version + " " + str(Status.OK.value) + " " + str(Status.OK.name) + "\r\n"
                # iterate through hosts and add them to the response
                for host in hosts:
                    hostname = host[0]
                    port = host[1]
                    response += "RFC " + rfcNumber + " " + title + " " + hostname + " " + str(port) + "\r\n"
                # and send it to the client
                connectionSocket.send(response.encode())

        elif method == "LIST":
            # if listing, return all of the RFCs
            version = fieldLine[2]
            rfcList = RFCList.getSharedList(shmRfcList)
            allRfcs = rfcList.getAll()
            # if there are no RFCs, return NOT_FOUND error
            if allRfcs is None:
                response = version + " " + str(Status.NOT_FOUND.value) + " " + str(Status.NOT_FOUND.name) + "\r\n"
                connectionSocket.send(response.encode())
            else:
                # set up the response
                response = version + " " + str(Status.OK.value) + " " + str(Status.OK.name) + "\r\n"
                # for each RFC, add it to the response on a new line
                for rfc in allRfcs:
                    rfcNumber = rfc[0]
                    rfcTitle = rfc[1]
                    rfcHostName = rfc[2]
                    rfcPort = rfc[3]
                    response += "RFC " + str(rfcNumber) + " " + rfcTitle + " " + rfcHostName + " " + str(rfcPort) + "\r\n"
                # send it to the client
                connectionSocket.send(response.encode())

# create a TCP socket
print("creating socket")
serverSocket = socket(AF_INET,SOCK_STREAM)

# bind the socket to a specific port
serverPort = 7734
serverSocket.bind(('',serverPort))

# listen for incoming connections
serverSocket.listen(1)
print('The server is ready to receive')

# create the two linked lists
peerList = PeerList()
rfcList = RFCList()

# catch keyboard interrupt for graceful disconnect
signal.signal(signal.SIGINT, disconnect)

# continue waiting for new connections
while 1:
    print('Waiting for a connection...')
    connectionSocket, clientAddress = serverSocket.accept()
    print('Accepted connection from', clientAddress)

    # spawn a new process to handle the connection
    pid = os.fork()
    if pid == 0:
        # this is a child process
        processRequest(connectionSocket, clientAddress)
        connectionSocket.close()
        exit(0)
    else:
        # this is the parent process
        connectionSocket.close()
        