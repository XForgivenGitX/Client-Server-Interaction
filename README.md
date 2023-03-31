# QuickChat

## Network library
It implements convenient methods for reading and writing to the socket, listening to the port and connecting. As a result, all functions call a custom callback function, which takes a socket as the first argument, which allows you to continue the read/listen cycle further. 
### Features: 
- Separate buffers for reading and writing 
- Exception handling 
- Correct socket shutdown.

## Server
The server accepts incoming connections and sends them to the lobby for further authorization. The user can choose between registration and authorization. After authorization, the user has the following choice: 
- Create a room. 
- Join an existing one.\
In the room, the user can send messages that will be delivered to all participants in the room. When a user logs in and out, corresponding messages are sent to all chat participants.

## What's next
- Saving the message history of each room and sending it to new participants. 
- Add the ability to display online participants and rooms to the user.
## Example
<img src="https://github.com/XForgivenGitX/QuickChat/blob/master/example.png" wigth="25%"/>

