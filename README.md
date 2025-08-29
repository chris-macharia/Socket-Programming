# MoneyApp

MoneyApp is a simple client–server application written in C.  
The **server** manages user accounts and commands, while the **client** allows users to interact with the server.

---

## 📦 Compilation

To compile both the server and client, simply run:

```bash
make
This will generate two executables:

server

client

To clean up (remove binaries and object files):

bash
Copy code
make clean
▶️ Running the Application
1. Start the Server
In one terminal window, run:

bash
Copy code
./server
The server will start and listen for incoming client connections.

2. Start the Client
Open another terminal window and run:

bash
Copy code
./client
You should now see the MoneyApp client interface.

💻 Example Usage
Once the client is connected to the server, you can type commands like:

text
Copy code
REGISTER CHRIS
DEPOSIT 100
BALANCE
WITHDRAW 50
HELP
⚠️ At this stage, many commands may return Unknown command. until they are fully implemented in the server.

🛠️ Development Notes
Server code: main.c, server.c, server.h

Client code: client.c

Makefile: used to build both programs

Future updates will add proper logic for commands like REGISTER, DEPOSIT, and BALANCE.

yaml
Copy code

---

This makes it **ready for anyone** (including your teammates or lecturer) to compile, run, and test.  

👉 Do you also want me to add **step-by-step instructions for testing with multiple cli