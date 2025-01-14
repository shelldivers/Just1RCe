## INDEX
- [COMMANDS](#commands)
  - [Connection Messages](#connection-messages)
    - [NICK](#nick)
    - [USER](#user)
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

**모든 메시지를 받는 형식은 irssi 클라이언트를 기준으로 작성됨을 알림**

```
timestamp -!-  ___           _
timestamp -!- |_ _|_ _ _____(_)
timestamp -!-  | || '_(_-<_-< |
timestamp -!- |___|_| /__/__/_|
timestamp -!- Irssi v1.5+1-dev-246-gee4471a1-dirty - https://irssi.org

[(status)] /connet -nocap localhost 6667
```

irssi 클라이언트에서 위의 명령어를 통해 들어오는 처리해야할 메시지의 형식은 다음과 같다 
```
NICK <nickname>      
USER <username> <hostname> <servername> :<realname>      
MODE user +i    
```

- 위의 클라이언트를 서버에 등록 후 서버는 다음 순서로 메시지를 반드시 전송해야 한다   
- 위의 메시지를 통해 서버의 이름과 네트워크 이름, 서버가 USER 명령어를 제대로 받았는지 알 수 있기 때문이다   
- 또한 서버가 가짜 호스트 이름을 제공하는 경우 full hostmask의 형식으로 보내서는 안된다 (클라이언트가 참조할 수도 있기 때문)   
```
"<client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]"
"<client> :Your host is <servername>, running version <version>"
``

# COMMANDS

## Connection Messages

### NICK
**format:**  
```bnf
     Command: NICK
  Parameters: <nickname>
```

**send & receive:**
```
/nick ken --> NICK ken
```

- 사용중인 별명인지 확인 필요   
- 숫자, 영문, [], {}, \, | 허용   
- 첫 번째 문자 숫자 금지   
- 다른 클라이언트에게 별명 변경을 알려야함   
    NICK b -> :a NICK b
- 매개변수가 없을 경우 무시   

### USER
**format(irssi):**  
```bnf
     Command: USER
  Parameters: <username> 0 * <realname>
```

**send & receive:**
```
--> USER jiwojung jiwojung localhost :jiwon
```

- 클라이언트가 처음에 서버에 연결시 보내는 명령어로 사용자의 realname, username을 서버에게 알린다   
- 만일 클라이언트가 등록을 완료한 후 다시 USER 명령을 보내려고한다면 시도는 실패하여야 한다   
- <username>에는 길이 제한이 있으며, 이는 서버에 의해 관리되며 이상 길이의 이름은 자동으로 잘리게 된다   
- 또한 최소길이는 1이며, 비울 수 없다 (메시지는 실패된며 연결은 끊어져야한다)    
- 두 번째, 세 번째 매개변수인 `0`과 `*`를 irssi 클라이언트는 위의 두 매개변수를 <username> 과 <address> 로 각각 순서대로 치환하여 보낸다   
- 각 IRC 프로토콜의 버전에 따라 다르기 때문에 이 두 매개변수는 따로 처리할 필요가 없다   
- <realname>은 공백 문자(' ', 0x20) 를 포함할 수 있기에 마지막에 위차하며 필요시 콜론(:)으로 시작한다   


### QUIT
**format: **  
```bnf
    Command: QUIT
 Parameters: [<reason>]
```

**send & receive:**
```
/quit --> QUIT :<default-message>
/quit sleep --> QUIT :sleep
<client disconnected> --> QUIT :<reason>
<to-other-clients>: <full-hostmask> :QUIT: <reason>
```

- 클라이언트와 서버와의 연결을 종료할 때 사용한다   
- 종료 이유가 없는 경우 기본 메시지를 제공한다 (irssi의 경우 "leaving" 이라는 기본 이유를 제공해줌)   
- 타임아웃이나 기타 이유료 강제 종료된 경우, 그에 해당하는 이유를 적어준다        


## Channel Operations

### JOIN
**format: **  
```bnf
     Command: JOIN
  Parameters: <channel>{,<channel>} [<key>{,<key>}]
  Alt Params: 0
```

**send & receive:**
```
/join channel
JOIN #channel
```

- 클라이언트가 특정 채널에 참여하고자 함을 나타내며, 각 채널에 대한 키가 제공되기도 한다   
- 서버는 클라이언트가 해당 채널에 참여할 수 있는지 확인하고 요청을 처리한다   
- 서버는 클라이언트의 요청에서 전달되 매개변수를 리스트 형태로 처리해야 하며, 첫 번째 키는 첫 번째 채널에, 두 번째 키는 두 번째 채널에 사용된다   
- 클라이언트는 채널에 참여 중인 동안, `JOIN` `PART` `KICK` `MODE` 등 채널에 영향을 미치는 모든 관련 정보를 수신한다   
- 클라이언트는 채널에 전송된 모든 `PRIVMSG`를 수신하며, 같은 채널에 참여햔 다른 사용자의 `QUIT` 메시지도 수신한다   

**성공적인 JOIN 처리 (명령 성공시 서버는 다음 순서로 응답을 보내야 한다):**    
- 클라이언트가 메시지의 <source>로 설정된 `JOIN`메시지를 보내며, 참여한 채널은 메시지의 첫 번째 매개변수로 포함된다   
- 채널의 주제는 [RPL_TOPIC(332)](https://modern.ircdocs.horse/#rpltopic-332) 과 선택적으로 [RPL_TOPICWHOTIME(333)](https://modern.ircdocs.horse/#rpltopicwhotime-333) 메시지로 전송되며, 제자가 없는 경우 메시지가 전송되지 않는다   
- 현재 채널에 참여 중인 사용자 목록은 하나 이상의 [RPL_NAMREPLY(353)](https://modern.ircdocs.horse/#rplnamreply-353)와 단일 [RPL_ENDOFNAMES(366)]("<client> <channel> :End of /NAMES list") 숫자 메시지로 전송된다   
- 서버에서 보낸 RPL_NAMREPLY 메시지에는 요청한 클라이언트 자신도 포함되어야한다    


**채널 참여 조건:**    
- key, 클라이언트 수 제한, invite-only 및 기타 채널 모드를 통해 클라이언트가 참여할 수 있는지 확인한다   
- 클라이언트가 제한을 초과하여 채널에 참여할 수 없는 경우 ERR_TOOMANYCHANNELS(405)응답을 반환하며 명령이 실패된다   

### PART
**format: **  
```bnf

```

**send & receive:**
```


```

### TOPIC
**format: **  
```bnf

```

**send & receive:**
```


```

### LIST
**format: **  
```bnf

```

**send & receive:**
```


```
### NAMES
**format: **  
```bnf

```

**send & receive:**
```


```
### INVITE
**format: **  
```bnf

```

**send & receive:**
```


```

### KICK
**format: **  
```bnf

```

**send & receive:**
```


```

## Server Queries and Commands

### MODE
**format: **  
```bnf

```

**send & receive:**
```


```

## Sending Messages

### PRIVMSG
**format: **  
```bnf

```

**send & receive:**
```


```

# Command Object




# Command-to-Function Mapping
