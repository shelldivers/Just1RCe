## INDEX
- [COMMANDS](#commands)
	- [Connection Messages](#connection-messages)
		- [NICK](#nick)
		- [USER](#user)
		- [PING / PONG](#ping--pong)
		- [QUIT](#quit)
	- [Channel Operations](#channel-operations)
		- [JOIN](#join)
		- [PART](#part)
		- [TOPIC](#topic)
		- [LIST](#list)
		- [NAMES](#names)
		- [INVITE](#invite)
		- [KICK](#kick)
	- [Server Queries and Commands](#server-queries-and-commands)
		- [MODE](#mode)
	- [Sending Messages](#sending-messages)
		- [PRIVMSG](#privmsg)
- [Command Object](#command-object)
- [Command-to-Function Mapping](#command-to-function-mapping)

---
```
timestamp -!-  ___           _
timestamp -!- |_ _|_ _ _____(_)
timestamp -!-  | || '_(_-<_-< |
timestamp -!- |___|_| /__/__/_|
timestamp -!- Irssi v1.5+1-dev-246-gee4471a1-dirty - https://irssi.org

[(status)] /connet -nocap localhost 6667
```

irssi 클라이언트에서 위의 명령어를 통해 들어오는 메시지의 형식은 다음과 같다 

- NICK <nickname>      
- USER <username> <hostname> <servername> :<realname>      
- MODE user +i    
- WHOIS jiwojung    


# COMMANDS

## Connection Messages

### NICK
format:   
```bnf
     Command: NICK
  Parameters: <nickname>
```

`NICK <nickname>`

- 사용중인 별명인지 확인 필요   
- 숫자, 영문, [], {}, \, | 허용   
- 첫 번째 문자 숫자 금지   
- 다른 클라이언트에게 별명 변경을 알려야함   
    NICK b -> :a NICK b
- 매개변수가 없을 경우 무시   

### USER

### PING / PONG

### QUIT
format:   
```bnf
    Command: QUIT
 Parameters: [<reason>]
```   

`/quit :<reason>`

- 클라이언트와 서버와의 연결을 종료할 때 사용   
    서버는 이를 인지하고 클라이언트와의 연결을 종료해야함   
- 같은 채널에 있는 사용자들에게만 `<nickname> :QUIT: <reason>` 양식으로 메시지를 출력   
- 종료 이유가 없는 경우 기본 메시지 제공 (irssi의 경우 "leaving" 이라는 기본 이유를 제공해줌)   
- 타임아웃이나 기타 이유료 강제 종료된 경우, 그에 해당하는 이유를 적어줌     



## Channel Operations

### JOIN
format:   
```bnf
     Command: JOIN
  Parameters: <channel>{,<channel>} [<key>{,<key>}]
  Alt Params: 0
```   

join을 하면 클라이언트는 각 채널마다 고유의 메시지들을 저장하는 공간이 있음
각 채널마다 나

### PART
format:   
```bnf
     Command: JOIN
  Parameters: <channel>{,<channel>} [<key>{,<key>}]
  Alt Params: 0
```   

### TOPIC
format:   
```bnf
     Command: TOPIC
  Parameters: <channel> [<topic>]
```    

`/topic <channel>`

### LIST

### NAMES

### INVITE
format:   
```bnf
     Command: INVITE
  Parameters: <nickname> <channel>
```   

`INVITE`

### KICK
format:   
```bnf
      Command: KICK
   Parameters: <channel> <user> *( "," <user> ) [<comment>]
```   

`KICK`

## Server Queries and Commands

### MODE
format:   
```bnf
     Command: MODE
  Parameters: <target> [<modestring> [<mode arguments>...]]
```   

`MODE`

## Sending Messages

### PRIVMSG
format:   
```bnf
     Command: PRIVMSG
  Parameters: <target>{,<target>} <text to be sent>
```   


# Command Object




# Command-to-Function Mapping
