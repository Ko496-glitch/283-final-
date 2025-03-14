1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

The remote shell will detect the End of command by checking for RDSH_EOF_CHAR sent by server. To handle the partial reads it uses a loop with rcv(),appending data until the EOF character is received. Ensuring complete message transmission involves sending null-terminated commands and processing received data in chunks.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?


A networked shell should use delimiters like \0 for commands and RDSH_EOF_CHAR (0x04) for responses to define message boundaries. The client and server must loop with recv(), accumulating data until the delimiter is found. Without this, messages may be split or combined incorrectly, causing execution errors.


3. Describe the general differences between stateful and stateless protocols.


Stateful protocols maintain client session information across multiple requests, allowing for context-aware communication (e.g., FTP, SSH). Stateless protocols treat each request independently, without retaining past interactions (e.g., HTTP, UDP). Stateful protocols require more resources but enable persistent interactions, while stateless ones scale better and are simpler to manage.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UDP is used when low latency and high-speed transmission are more important than reliability, such as in real-time applications like video streaming, VoIP, and online gaming. It avoids the overhead of connection establishment and retransmission, making it faster than TCP. Applications that can tolerate some data loss or implement their own reliability mechanisms benefit from UDP’s efficiency.



5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

The operating system provides the Berkeley Sockets API (or simply sockets) as an interface for applications to use network communications. This abstraction allows programs to create, bind, listen, connect, send, and receive data over network protocols like TCP and UDP. It provides a uniform way to handle network communication across different operating systems.
