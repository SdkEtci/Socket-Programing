# Socket Programming in C

This project is a basic implementation of socket programming in the C language. It demonstrates a simple TCP-based client-server architecture where the client connects to the server, sends data, and receives responses.

- `client/client.c`: The client application that connects to the server and communicates by sending/receiving data.
- `server/server.c`: The server application that listens for client connections and processes incoming data.
- `ornek1.txt` and `ornek2.txt`: Example text files used for testing input/output behavior.

## ⚙️ How to Run

### 1. Compile the Code

Use GCC or any C compiler to compile the client and server:

gcc server/server.c -o server

gcc client/client.c -o client

### 2. Start the Server

In one terminal, run:

./server

### 3. Start the Client

In a separate terminal, run:

./client

The client will connect to the server and initiate communication using the specified protocol in the code.

## 💡 Features
Simple TCP socket communication

Data exchange via standard input/output

Basic file handling for reading/writing text files

## 📌 Requirements
GCC or another C compiler

Unix-like operating system (e.g., Linux, macOS)
