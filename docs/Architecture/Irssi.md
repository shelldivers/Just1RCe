## INDEX
- [COMMANDS](#commands)
  - [INFO](#info)
    - [USER](#user)
    - [SERVER](#server)
    - [CLINET](#clinet)
    - [RULES](#rules)
    - [CONNECT](#connect)
  - [  CONNECTION MESSAGES ](#--connection-messages-)
    - [NICK](#nick)
      - [Error Response](#error-response)
    - [USER](#user-1)
      - [FORMAT](#format)
      - [Error Response](#error-response-1)
    - [PING](#ping)
      - [Error Response](#error-response-2)
    - [PONG](#pong)
    - [QUIT](#quit)
  - [  CHANNEL MESSAGES  ](#--channel-messages--)
    - [JOIN](#join)
      - [Error Response](#error-response-3)
    - [PART](#part)
      - [Error Response](#error-response-4)
    - [TOPIC](#topic)
      - [Error Response](#error-response-5)
    - [NAMES](#names)
      - [Error Response](#error-response-6)
    - [LIST](#list)
      - [Error Response](#error-response-7)
    - [INVITE](#invite)
      - [Error Response](#error-response-8)
    - [KICK](#kick)
      - [Error Response](#error-response-9)
  - [SERVER QUERY](#server-query)
    - [MODE](#mode)
      - [Error Response](#error-response-10)
  - [SENDING MESSAGES](#sending-messages)
    - [PRIVMSG](#privmsg)

---

# COMMANDS   

## INFO

### USER
REAL_NAME: real_ken      
USER_NAME: user_ken   
NICK_NAME: nick_ken   

### SERVER
SERVER_NAME: irc.rizon.net     

### CLINET
CLINET_NAME: irssi    
CLINET_VERSION: 1.45      

### RULES

- USER는 3명이 존재한다   
  *_ken: 처음 채널에 들어온 채널 운영자   
  *_ken2: 이후에 들어온 채널 사용자   
  *_ken3: 마지막에 들어온 채널 사용자     

- 테스트한 서버에서는 user_ken 앞에오는 `~`는 검증되지 않은 사용자를 나타낸다     

- 일반적으로 `:`로 시작하는 문자열은 서버가 전송해주는 문자열이다      
  `":string "` 의 형태로 문자열이 감싸져 있게 된다면 해당 문자열은 다른 클라이언트들에게도 보여지는 것이다      

---

### CONNECT

/connect -nocap servername port


NICK nick_ken   
USER user_ken user_ken irc.rizon.net :real_ken   

NICK nick_ken2   
USER user_ken2 user_ken2 irc.rizon.net :real_ken2   

NICK nick_ken3   
USER user_ken3 user_ken3 irc.rizon.net :real_ken3
PING irc.losslessone.com

```
CLIENT: 
  NICK nick_ken    

SERVER:

```   

irssi는 위의 명령어를 통해 서버에 연결시 다음 네가지 명령어를 전송한다   
- NICK nick_ken   
- USER user_ken user_ken SERVER_NAME :real_ken   

위의 USER 명령어를 통해 서버에서 전달받은 NICK_NAME을 MODE의 인자로 전달한다   
- MODE nick_ken +i   
- WHOIS nick_ken   

현 Just1RCe 프로젝트에서는 WHOIS 명령어는 구현하지 않으므로 무시하도록 한다     

## <details> <summary> CONNECTION MESSAGES </summary>

### NICK   

사용자의 NICK_NAME을 설정하는 명령어   

- FORMAT   
```bnf
     Command: NICK
  Parameters: <nickname>
```

- Message Transaction   
```
CLIENT: 
  NICK nick_ken    

SERVER:

```   

#### Error Response

- 인자로 아무것도 들어오지 않은 경우 무시한다   
  ```
  Client:
    NICK
  
  SERVER:
    :irc.losslessone.com 431 nick_ken3 :No nickname given
  ```

- 이미 사용중인 이름인 경우 무시한다      
  ```
  Client:
    NICK nick_ken
  
  SERVER:
    :irc.losslessone.com 433 nick_ken3 nick_ken :Nickname is already in use.
  ```

- 잘못된 형식의 이름인 경우 무시한다   
  ```
  Client:
    !@#%#@$%^
  
  SERVER:
    :irc.losslessone.com 432 nick_ken3 !@#%#@$%^ :Erroneous Nickname
  ```

### USER   

사용자의 유저 이름, 실제 이름등을 서버에 정의하는 명령어   

#### FORMAT

- FORMAT   
```bnf
     Command: USER
  Parameters: <username> 0 * <realname>
```

- Message Transaction   
```
CLIENT: 
  USER user_ken user_ken irc.rizon.net :real_ken    

SERVER:
  WELCOME_MESSAGES and others...
```   

#### Error Response

- 인자가 제대로 들어오지 않은 경우 무시한다   
  ```
  CLIENT: 
    USER    

  SERVER:
    :irc.losslessone.com 461 nick_ken3 USER :Not enough parameters
  ```   

- 이미 등록된 사용자인 경우 무시한다
  ```
  CLIENT: 
    USER user_ken user_ken irc.rizon.net :real_ken    

  SERVER:
    :irc.losslessone.com 462 nick_ken3 :You may not reregister
  ```   

rizon 서버는 다음과 같이 응답한다

```
PING :3340719355
[DEBUG] Responding to PING with: PONG :3340719355

:SERVER_NAME NOTICE nick_ken :*** Your host is masked (Masked-IP.IP)
↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓(this is welcom message from server to client)
:SERVER_NAME 001 nick_ken :Welcome to the Rizon Internet Relay Chat Network nick_ken
:SERVER_NAME 002 nick_ken :Your host is SERVER_NAME, running version SERVER_VERSION
:SERVER_NAME 003 nick_ken :This server was created Mar 19 2022 at 21:25:18
:SERVER_NAME 004 nick_ken SERVER_NAME SERVER_VERSION CDGNRSUWagilopqrswxyz BCIMNORSabcehiklmnopqstvz Iabehkloqv
:SERVER_NAME 005 nick_ken CALLERID CASEMAPPING=rfc1459 DEAF=D KICKLEN=180 MODES=4 PREFIX=(qaohv)~&@%+ STATUSMSG=~&@%+ EXCEPTS=e INVEX=I NICKLEN=30 NETWORK=Rizon MAXLIST=beI:250 MAXTARGETS=4 :are supported by this server
:SERVER_NAME 005 nick_ken CHANTYPES=# CHANLIMIT=#:250 CHANNELLEN=50 TOPICLEN=390 CHANMODES=beI,k,l,BCMNORScimnpstz NAMESX UHNAMES KNOCK WATCH=60 AWAYLEN=180 ELIST=CMNTU SAFELIST :are supported by this server
↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
:SERVER_NAME 251 nick_ken :There are 17 users and 9258 invisible on 18 servers
:SERVER_NAME 252 nick_ken 47 :IRC Operators online
:SERVER_NAME 253 nick_ken 1 :unknown connection(s)
:SERVER_NAME 254 nick_ken 7813 :channels formed
:SERVER_NAME 255 nick_ken :I have 9275 clients and 18 servers
:SERVER_NAME 265 nick_ken :Current local users: 9275  Max: 10299
:SERVER_NAME 266 nick_ken :Current global users: 9275  Max: 10299
:SERVER_NAME 375 nick_ken :- SERVER_NAME Message of the Day - 
:SERVER_NAME 372 nick_ken :-          11oo                            
:SERVER_NAME 372 nick_ken :- 
:SERVER_NAME 372 nick_ken :- 88d888b. dP d888888b .d8888b. 88d888b. 
:SERVER_NAME 372 nick_ken :- 88'  `88 88    .d8P' 88'  `88 88'  `88 
:SERVER_NAME 372 nick_ken :- 1188       88  .Y8P    88.  .88 88    88 
:SERVER_NAME 372 nick_ken :- 11dP       dP d888888P `88888P' dP    dP
:SERVER_NAME 372 nick_ken :- 
:SERVER_NAME 372 nick_ken :- Rizon Chat Network -- http://rizon.net
:SERVER_NAME 372 nick_ken :- 
:SERVER_NAME 372 nick_ken :- Listening on ports: 6660 - 6669, 7000. SSL: 6697, 9999
:SERVER_NAME 372 nick_ken :- 
:SERVER_NAME 372 nick_ken :- Rules:
:SERVER_NAME 372 nick_ken :- o No spamming or flooding
:SERVER_NAME 372 nick_ken :- o No clones or malicious bots
:SERVER_NAME 372 nick_ken :- o No takeovers
:SERVER_NAME 372 nick_ken :- o No distribution of child pornography
:SERVER_NAME 372 nick_ken :- o Clients must respond to VERSION requests
:SERVER_NAME 372 nick_ken :- o Rizon staff may disconnect clients for any or no reason
:SERVER_NAME 372 nick_ken :- 
:SERVER_NAME 372 nick_ken :- First steps:
:SERVER_NAME 372 nick_ken :- o To register your nick: /msg NickServ HELP
:SERVER_NAME 372 nick_ken :- o To register your channel: /msg ChanServ HELP
:SERVER_NAME 372 nick_ken :- o To get a vHost: /msg HostServ HELP REQUEST
:SERVER_NAME 372 nick_ken :- o For other help with Rizon: /join #help
:SERVER_NAME 372 nick_ken :- 
:SERVER_NAME 372 nick_ken :- Usage of this network is a privilege, not a right. Rizon is a
:SERVER_NAME 372 nick_ken :- transit provider, therefore no person or entity involved with
:SERVER_NAME 372 nick_ken :- *.rizon.net or SERVER_NAME takes any responsibility for
:SERVER_NAME 372 nick_ken :- users' actions. Absolutely no warranty is expressed or implied.
:SERVER_NAME 376 nick_ken :End of /MOTD command.
:nick_ken!~user_ken@Masked-IP.IP MODE nick_ken :+ix
:py-ctcp!ctcp@ctcp-scanner.rizon.net PRIVMSG nick_ken :VERSION
:Global!service@rizon.net NOTICE nick_ken :[Logon News - Dec 16 2013] Own a large/active channel or plan to get one going? Please read http://s.rizon.net/authline
:Global!service@rizon.net NOTICE nick_ken :[Logon News - Feb 16 2022] First time on Rizon? Be sure to check out our wiki! https://wiki.rizon.net
:Global!service@rizon.net NOTICE nick_ken :[Random News - Nov 29 2013] Want to make your vhost shiny by adding colors to it? Try your luck in #VIzon
```


### PING   

서버와 연결 여부를 확인하기 위해 사용하는 명령어    


- FORMAT   
```bnf
     Command: PING
  Parameters: <token>
```

- Message Transaction   
```
CLIENT: 
  PING hello     

SERVER:
  :irc.losslessone.com PONG irc.losslessone.com :hello
```   
 

- <token> 은 어떤 문자열이든 올 수 있다   


#### Error Response

- 클라이언트는 연결 등록 과정 중에 PING을 보내지 않아야 한다   
  ```
  SERVER:
    :irc.losslessone.com 451 * :You have not registered (닉네임 등록이 안된 경우)
    :irc.losslessone.com 451 nick_ken3 :You have not registered (닉네임 등록이 되어 있는 경우)
  ```

- 인자가 없는 경우   
  ```
  Client:
    PING
  SERVER: 
    :irc.losslessone.com 409 nick_ken3 :No origin specified
  ```

- 

### PONG

PING 명령어에 응답하는 명령어   

- FORMAT   
```bnf
     Command: PONG
  Parameters: [<server>] <token>
```

- Message Transaction   
```bash
SERVER:
  PING :irc.losslessone.com

CLIENT:
  PONG :irc.losslessone.com
``` 



- PONG 명령어는 반드시 PING에서 받은 <token> 값을 그대로 포함해야 한다

### QUIT

- FORMAT   
```bnf
     Command: QUIT
  Parameters: [<reason>]
```

- Message Transaction   
```bash
CLIENT:
  QUIT: Bye for now!

SERVER:
  :dan-!d@localhost QUIT :Quit: Bye for now!

dan- 이 네트워크에서 나갔음을 알림
```

irssi는 QUIT명령어 사용시 기본 메시지로 *leaving*을 남긴다    
`QUIT :leaving`    

서버는 이에 대해 응답을 하지 않으나, 네트워크에서 퇴장한 사용자의 정보를 해당 사용자가 속해 있던 채널의 다른 사용자들에게 전달하여야 한다    
사용자가 적절한 QUIT 메시지를 남기지 않은 경우 서버는 상황에 알맞은 QUIT 메시지를 전달한다   
퇴장에는 2가지 경우가 존재한다   

1. Client가 QUIT 명령어를 입력한 경우   
  ```
  Client:   
    QUIT Bye for now!

  Server:
    ":nick_ken2!~user_ken2@MASKED_IP.IP QUIT :"  
  ```

  여기서 유저 이름 앞에 붙어있는 ~ 는 인증되지 않은 유저임을 나타냄    
  서버에서는 이유를 받아도 돌려주지 않으며 최소한 `QUIT :`는 작성하는 것을 볼 수 있음

2. Client가 비정상적으로 연결이 종료된 경우   
  ```
  Client:

  Server:
    ":nick_ken2!~user_ken2@MASKED_IP.IP QUIT :Remote host closed the connection"
  ```
  여기서는 서버가 클라이언트의 상태 정보를 작성해준다


</details>


## <details> <summary> CHANNEL MESSAGES  </summary>

### JOIN

채널에 접속하기 위한 명령어

- FORMAT   
```bnf
     Command: JOIN
  Parameters: <channel>{,<channel>} [<key>{,<key>}]
  Alt Params: 0
```

- Message Transaction   
```bash
CLIENT:
  JOIN #test

SERVER:
  ":dan-!d@localhost JOIN #test"    ; dan- 이 채널 test에 들어왔음을 알림
```

- 클라이언트가 존재하지 않는 채널 이름을 입력하면 해당 채널이 생성되며 해당 클라이언트는 채널 운영자가 된다   
  ```
  CLINET:
    JOIN #CHANNEL_NAME

  SERVER:
    :nick_ken!~user_ken@MASKED_IP.IP JOIN :#CHANNEL_NAME
    :SERVER_NAME MODE #CHANNEL_NAME +nt
    :SERVER_NAME 353 nick_ken = #CHANNEL_NAME :@nick_ken
    :SERVER_NAME 366 nick_ken #nick_ken :End of /NAMES list.
  ```   


- 클라이언트가 JOIN 명령을 성공적으로 실행하면, 서버는 다음 순서로 응답을 보내야 한다   
  ```
  CLIENT:
    JOIN #CHANNEL_NAME
  ```

  1. JOIN message (채널에 참여하고 있는 모든 클라이언트들에게) 
  ```
  SERVER:
    ":nick_ken3!~user_ken3@MASKED_IP.IP JOIN :#CHANNEL_NAME"
  ```
  1. TOPIC message (TOPIC 설정이 없다면 생략 가능)   
  ```
  SERVER:
    :SERVER_NAME 332 nick_ken3 #CHANNEL_NAME :hello
    :SERVER_NAME 333 nick_ken3 #CHANNEL_NAME nick_ken!~user_ken@MASKED_IP.IP 1738306949
  ```
  1. 현재 채널에 있는 사용자 목록   
  ```
  SERVER:
    :SERVER_NAME 353 nick_ken3 = #CHANNEL_NAME :nick_ken3 nick_ken2 @nick_ken
    :SERVER_NAME 366 nick_ken3 #CHANNEL_NAME :End of /NAMES list.
  ```

- 클라이언트가 특정 채널(들)에 참여하려고 한다는 것을 나타내며, 필요한 경우 비밀번호(key)를 사용한다    

- 서버는 해당 명령을 받으면 **해당 채널에 참여할 수 있는지 확인** 한 후 요청을 처리 한다    
  외에도 여러가지 이유로 채널 참가가 제한 될 수 있다   

- 각 <channel> 과 <key>는 순서에 맞게 적용된다 (첫 번째 채널에 첫 번째 키, 두 번째 채널에 두 번째 키)   

- 인자로 대신 0을 전달하면 현재 참여중인 모든 채널에서 나가게 된다 (각 채널에서 PART하는 효과를 가짐)   

#### Error Response

- 채널에 참여할 수 있는 클라이언트 제한을 넘은 경우    
  ```
  :SERVER_NAME 471 nick_ken3 #limited_channel :Cannot join channel (+l)
  ```   

- 비밀번호를 잘못 입력한 경우   
  ```
  :SERVER_NAME 475 nick_ken3 #pass_channel :Cannot join channel (+k)
  ```     

- 채널이 초대 전용으로 이루어진 경우   
  ```
  :SERVER_NAME 473 nick_ken3 #invite_only :Cannot join channel (+i)
  ``` 

- 인자가 없는 경우
  ```
  CLIENT: 
    JOIN

  SERVER:
    :irc.losslessone.com 461 nick_ken3 JOIN :Not enough parameters
  ```   

- 존재하지 않는 채널에 JOIN을 시도했으나 채널이 자동 생성되지 않은 경우
  ```
  Client:
    JOIN #nonexistent_channel

  Server:
    :irc.losslessone.com 403 nick_kenn3 #nonexistent_channel :No such channel

  ```   

- 채널 참가 한도가 초과된 경우
  ```
  Client:
    JOIN #channel5

  Server:
    :irc.losslessone.com 405 nick_kenn3 #channel5 :You have joined too many channels
  ```   

- 잘못된 채널 이름인 경우
  ```
  Client:
    JOIN !#$%!

  Server:
    :irc.losslessone.com 476 nick_kenn3 !#$%! :Bad Channel Mask (IRC 문서)
                                  or
    :irc.losslessone.com 479 nick_ken3 !#$%! :Illegal channel name (현재 상용 서버)
  ```   


### PART

채널에서 나가는 명령어   

- FORMAT   
```bnf
     Command: PART
  Parameters: <channel>{,<channel>} [<reason>]
```

- Message Transaction   
```bash
CLIENT:
  PART #test

SERVER:
  :dan-!d@localhost PART #test    ; dan- 가 채널 test를 떠남
```

- `reason`은 선택사항으로 구현하지 않는다   

#### Error Response

- 채널이 존재하지만 사용자가 해당 채널에 참가하고 있지 않는다면 요청을 무시한다   
  ```
  :irc.losslessone.com 442 nick_ken3 #aaa :You're not on that channel
  ```

- 지정된 채널이 존재하지 않는다면 요청을 무시한다    
  ```
  :irc.losslessone.com 403 nick_ken3 #asdf :No such channel
  ```

### TOPIC
- FORMAT   
```bnf
     Command: TOPIC
  Parameters: <channel> [<topic>]
```

- Message Transaction   
```bash
CLIENT:
  TOPIC #SERVER_NAME MESSAGE

SERVER:
  :nick_ken!~user_ken@F6CF112A.8077C83A.8ED361E2.IP TOPIC #CHANNEL_NAME :TOPIC_MESSAGE
```

#### Error Response

### NAMES
- FORMAT   
```bnf

```

- Message Transaction   
```bash
CLIENT:


SERVER:

```

#### Error Response

### LIST
- FORMAT   
```bnf

```

- Message Transaction   
```bash
CLIENT:


SERVER:

```
#### Error Response
### INVITE
- FORMAT   
```bnf

```

- Message Transaction   
```bash
CLIENT:


SERVER:

```
#### Error Response


### KICK
- FORMAT   
```bnf

```

- Message Transaction   
```bash
CLIENT:


SERVER:

```

#### Error Response

</details>




## SERVER QUERY

### MODE   
- FORMAT   
```bnf
     Command: MODE
  Parameters: <target> [<modestring> [<mode arguments>...]]
```

- Message Transaction   
```
CLIENT: 
  MODE nick_ken +i    

SERVER:
  MODE #CHANNEL_NAME +k 11111
  :nick_ken!~user_ken@MASKED_IP.IP MODE #CHANNEL_NAME +k 11111
```
#### Error Response


## SENDING MESSAGES

### PRIVMSG
- FORMAT   
```bnf

```

- Message Transaction   
```bash
CLIENT:


SERVER:

```










