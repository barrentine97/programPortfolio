# RFC Downloader
## Team Members
* Brandon Barrentine: bbarren
* Auma Asiyo: laasiyo

## Usage
This program demonstrates a simple peer-to-server and peer-to-peer paradigm. Clients connect to the server in order to add, list, and lookup any other clients with a specified RFC. Clients can then connect to other clients to download RFCs from them.

## Setup
to run the program, ensure that you have python installed and are running on a Unix-based system. Since the code uses the os.fork() function, it will not work on a windows-based system.

## Server
* navigate to rfcDownloader/Server in a terminal
* use the following command to run the server:
```
python3 server.py
```

## Client
* navigate to rfcDownloader/Client in a different terminal
* use the following command to connect a client to the server:
```
python3 client.py
```
the client will run and prompt the user for input
