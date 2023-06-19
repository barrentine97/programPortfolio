import sys
import os
current_dir = os.path.dirname(os.path.abspath(__file__))
parent_dir = os.path.join(current_dir, "..")
sys.path.append(parent_dir)

import platform
import email.utils
import time

from LinkedLists.peerList import PeerList
from LinkedLists.rfcList import RFCList
from Status.status import Status

from socket import *
import signal

# Create ADD request (adding new RFC)
def addRequest(rfcNumber, rfcTitle, uploadPort):
	hostname = clientSocket.getsockname()[0]
	message = "ADD RFC " + str(rfcNumber) + " P2P-CI/1.0\r\n"\
			  "Host: " + hostname + "\r\n"\
			  "Port: " + str(uploadPort) + "\r\n"\
			  "Title: " + str(rfcTitle) + "\r\n"
	return message

# Create LOOKUP request
def lookupRequest(rfcNumber, rfcTitle, uploadPort):
	hostname, address = clientSocket.getsockname()
	message = "LOOKUP RFC " + str(rfcNumber) + " P2P-CI/1.0\r\n"\
			  "Host: " + hostname + "\r\n"\
			  "Port: " + str(uploadPort) + "\r\n"\
			  "Title: " + str(rfcTitle) + "\r\n"
	return message

# Create GET Request
def getRequest(rfcNumber):
	hostname, address = clientSocket.getsockname()
	message = "GET RFC " + str(rfcNumber) + " P2P-CI/1.0\r\n"\
			  "Host: "+ hostname + "\r\n"\
			  "OS: " + platform.platform() + "\r\n"
	return message

# Create GET Reply, used for P2P communication
def getReply(data, rfcFilePath):
	message = "P2P-CI/1.0 200 OK\r\n"\
				"Date: " + str(email.utils.formatdate(usegmt=True)) + "\r\n"\
			   	"OS: " + str(platform.platform()) + "\r\n"\
			   	"Last-Modified: " + str(time.ctime(os.path.getmtime(rfcFilePath))) + "\r\n"\
				"Content-Length: " + str(len(data)) + "\r\n"\
				"Content-Type: text/plain\r\n"
	return message

# Create LIST Request
def listRequest():
	hostname, address = clientSocket.getsockname()
	message = "LIST ALL P2P-CI/1.0\r\n"\
			  "Host: " + hostname + "\r\n"\
			  "Port: " + str(address) + "\r\n"
	return message

# disconnect the client
def disconnect(s, frame):
	os.kill(pid, signal.SIGTERM)
	clientSocket.close()
	uploadSocket.close()
	exit(0)


# Accepts incoming file upload requests from peers and returns requested file
def uploadRequest():
	# listen for incoming requests
	uploadSocket.listen(1)
	while 1:
		# Incoming request for upload
		downloadSocket = uploadSocket.accept()[0]
		req = downloadSocket.recv(1024)
		lines = req.decode().split("\r\n")

		# parse RFC number
		fieldLine = lines[0].split(" ")
		rfcNumber = fieldLine[2]

		# search for file and get its data
		rfcFilePath = os.getcwd() + "/../RFC/RFC" + rfcNumber + ".txt"
		if os.path.isfile(rfcFilePath):
			newFile = open(rfcFilePath, 'r')
			data = newFile.read()

			# create reply for get request
			reply = getReply(data, rfcFilePath)
			# add rfc data to reply and send back
			reply += data
			downloadSocket.send(reply.encode())
		else:
			# if RFC is not found, send error message instead
			reply = "P2P-CI/1.0 " + str(Status.NOT_FOUND.value) + " " + str(Status.NOT_FOUND.name)
			downloadSocket.send(reply.encode())

# Sends GET request and receives what is returned. Used
# to download RFC from another client
def downloadRFC(rfcNumber, hostname, portNumber):

	# Connect to upload server of the peer with requested file
	requestPeerSocket = socket(AF_INET, SOCK_STREAM)
	try:
		requestPeerSocket.settimeout(5)
		requestPeerSocket.connect((hostname, int(portNumber)))
	except error as e:
		print("P2P-CI/1.0 " + str(Status.BAD_REQUEST.value) + " " + str(Status.BAD_REQUEST.name))
		return

	# create a GET request
	reqMessage = getRequest(rfcNumber)
	print("GET request")
	print(reqMessage)
	# send the GET request to peer
	requestPeerSocket.send(reqMessage.encode())
	
	# parse the reply
	reply = requestPeerSocket.recv(1024)
	lines = reply.decode().split("\r\n")
	print("GET reply")
	print(lines[0])
	fieldLine = lines[0].split(" ")
	status = fieldLine[2]

	# print the status
	if status == Status.OK.name:
		rfcFilePath = os.getcwd() + "/../downloads/RFC" + str(rfcNumber) + ".txt"
		os.makedirs(os.path.dirname(rfcFilePath), exist_ok=True)
		content = lines[6]

		# write the data
		with open(rfcFilePath, 'w') as file:
			file.write(content)

	#Close the socket connection
	requestPeerSocket.close()

# Handle GET, ADD, LIST, LOOKUP, and EXIT requests
def userInput():
	# get the request input from the user
	print("Enter: ADD, GET, LIST, LOOKUP or EXIT:")
	service = input().upper()

	if service == "ADD":
		# get the request input from the user
		print("Enter RFC Number")
		try:
			rfcNumber = int(input())
		except ValueError:
			print("P2P-CI/1.0 " + str(Status.BAD_REQUEST.value) + " " + str(Status.BAD_REQUEST.name))
			return
		print("Enter RFC Title")
		rfcTitle = input()

		rfcFilePath = os.getcwd() + "/../RFC/RFC" + str(rfcNumber) + ".txt"
		if os.path.isfile(rfcFilePath):
			# Create ADD Request and send to sercer
			reqMessage = addRequest(rfcNumber,rfcTitle, uploadPort)
			print("ADD Request")
			print(reqMessage)
			clientSocket.send(reqMessage.encode())
			
			# Receive the response from server and print
			responseReceived = clientSocket.recv(1024)
			print("ADD Response")
			print(responseReceived.decode())
		else:
			print("P2P-CI/1.0 " + str(Status.NOT_FOUND.value) + " " + str(Status.NOT_FOUND.name))
		return

	elif service == "GET":
		# get the request input from the user
		print("Enter RFC Number")
		try:
			rfcNumber = int(input())
		except ValueError:
			print("P2P-CI/1.0 " + str(Status.BAD_REQUEST.value) + " " + str(Status.BAD_REQUEST.name))
			return
		print("Enter Hostname")
		hostname = input()
		print("Enter Upload Port Number")
		try:
			uploadPortNumber = int(input())
		except ValueError:
			print("P2P-CI/1.0 " + str(Status.BAD_REQUEST.value) + " " + str(Status.BAD_REQUEST.name))
			return
		if uploadPortNumber < 0 or uploadPortNumber > 65535:
			print("P2P-CI/1.0 " + str(Status.BAD_REQUEST.value) + " " + str(Status.BAD_REQUEST.name))
			return

		# download the rfc using the given input
		downloadRFC(rfcNumber, hostname, uploadPortNumber)
		return

	elif service == "LIST":
		# send list request to the server
		reqMessage = listRequest()
		print("LIST Request")
		print(reqMessage)
		clientSocket.send(reqMessage.encode())

		# Receive the response from the server
		responseReceived = clientSocket.recv(1024)
		print("LIST Response")
		print(responseReceived.decode())
		return

	elif service == "LOOKUP":
		# get the request input from the user
		print("Enter RFC Number")
		try:
			rfcNumber = int(input())
		except ValueError:
			print("P2P-CI/1.0 " + str(Status.BAD_REQUEST.value) + " " + str(Status.BAD_REQUEST.name))
			return
		print("Enter RFC Title")
		rfcTitle = input()

		# Create a LOOKUP request and send to the server
		reqMessage = lookupRequest(rfcNumber, rfcTitle, uploadPort)
		print("LOOKUP Request")
		print(reqMessage)
		clientSocket.send(reqMessage.encode())

		# Receive the response from the server
		responseReceived = clientSocket.recv(1024)
		print("LOOKUP Response sent from the server")
		print(responseReceived.decode())
		return

	elif service == "EXIT":
		# disconnect the client
		message = "EXIT"
		clientSocket.send(message.encode())
		os.kill(pid, signal.SIGTERM)
		clientSocket.close()
		uploadSocket.close()
		exit(0)
	else:
		print("P2P-CI/1.0 " + str(Status.BAD_REQUEST.value) + " " + str(Status.BAD_REQUEST.name))
		return

# Server Info
serverName = 'localhost'
serverPort = 7734

# Create TCP socket for server
print("creating socket")
clientSocket = socket(AF_INET,SOCK_STREAM)

# Connect to server
clientSocket.connect((serverName, serverPort))
print("connected to server at: " +str(serverName)+" and port: "+str(serverPort))

# Create an upload server socket
uploadSocket = socket()
uploadSocket.bind(('0.0.0.0', 0))

# let server know the upload socket
uploadPort = uploadSocket.getsockname()[1]
clientSocket.send(str(uploadPort).encode())

# catch keyboard interrupt for graceful disconnect
signal.signal(signal.SIGINT, disconnect)

# spawn a new process to handle upload socket
pid = os.fork()
# spawns a pipe for the parent to talk to the child
pipe = os.pipe()
if pid == 0:
	# this is a child process
	# listen for incoming peers
	uploadRequest()
	exit(0)
else:
	# this is a parent process
	# Handle GET,ADD,LOOKUP,LIST and EXIT
	while True:
		userInput()
