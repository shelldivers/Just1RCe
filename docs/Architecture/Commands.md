## INDEX
- [COMMANDS](#commands)
  - [COMMAND FORMAT](#command-format)
  - [INFO](#info)
    - [USER](#user)
    - [SERVER](#server)
    - [CLINET](#clinet)
    - [RULES](#rules)
  - [  CONNECTION MESSAGES ](#--connection-messages-)
    - [NICK](#nick)
      - [FORMAT](#format)
      - [Numerics](#numerics)
    - [USER](#user-1)
      - [FORMAT](#format-1)
      - [Numerics](#numerics-1)
    - [PING](#ping)
      - [FORMAT](#format-2)
      - [Numerics](#numerics-2)
    - [PONG](#pong)
      - [FORMAT](#format-3)
    - [QUIT](#quit)
      - [FORMAT](#format-4)
  - [  CHANNEL MESSAGES  ](#--channel-messages--)
    - [JOIN](#join)
      - [FORMAT](#format-5)
      - [Numerics](#numerics-3)
    - [PART](#part)
      - [FORMAT](#format-6)
      - [Numerics](#numerics-4)
    - [TOPIC](#topic)
      - [FORMAT](#format-7)
      - [Numerics](#numerics-5)
    - [NAMES](#names)
      - [FORMAT](#format-8)
      - [Numerics](#numerics-6)
    - [LIST](#list)
      - [FORMAT](#format-9)
      - [Numerics](#numerics-7)
    - [INVITE](#invite)
      - [FORMAT](#format-10)
      - [Numerics](#numerics-8)
    - [KICK](#kick)
      - [FORMAT](#format-11)
      - [Numerics](#numerics-9)
  - [SERVER QUERY](#server-query)
    - [MODE](#mode)
      - [FORMAT](#format-12)
      - [Numerics](#numerics-10)
  - [SENDING MESSAGES](#sending-messages)
    - [PRIVMSG](#privmsg)
      - [FORMAT](#format-13)
      - [NUMERICS](#numerics-11)

---

# COMMANDS   

## COMMAND FORMAT

서버는 클라이언트로부터 오는 메시지를 parsing할 뿐만아니라 client에게 보내는 메시지 또한 올바른 형식으로 조합 하여 보내야 한다   

아래에는 IRC 프로토콜의 메시지 형식을 ABNF로 표현한 것이다   

```ABNF
  message         ::= [':' <source> SPACE] <command> <parameters> <crlf>
  SPACE           ::=  %x20 *( %x20 )   ; space character(s)
  crlf            ::=  %x0D %x0A        ; "carriage return" "linefeed"

  source          ::=  <servername> / <nickname> 
  nick            ::=  <any characters except NUL, CR, LF, chantype character, and SPACE> <possibly empty sequence of any characters except NUL, CR, LF, and SPACE>
  user            ::=  <sequence of any characters except NUL, CR, LF, and SPACE>

  command         ::=  letter* / 3digit

  parameters      ::=  *( SPACE middle ) [ SPACE ":" trailing ]
  nospcrlfcl      ::=  <sequence of any characters except NUL, CR, LF, colon (`:`) and SPACE>
  middle          ::=  nospcrlfcl *( ":" / nospcrlfcl )
  trailing        ::=  *( ":" / " " / nospcrlfcl )
```

IRC 메시지는 크게 3개로 나뉜다   

`source`, `command`, `parameters`    

**source**   
`0 ~ 1` 개가 존재할 수 있다   

server가 client에게 보내는 메시지에 사용된다   
`:`으로 시작해 `SPACE` 로 끝이 나며, 서버 이름 혹은 닉네임으로 이루어진다   

```c++
#define SOURCE_PREFIX ':'
#define SOURCE_SUFFIX ' '

std::string server_name;
std::string nick_name;
std::string full_name;
```

**command**   
반드시 `1` 개가 존재해야 한다   
일반적으로 전부 대문자로 이루어져 있다    
(대소문자 구분은 자율적이다)   

```c++
std::string command;
```

**parameters**   
- `0 ~ 15` 개가 존재할 수 있다   
- 공백(`SPACE`)으로 나뉘며, `:`(콜론)를 기준으로 좌측은 `middle`, 우측은 `trailing`으로 구분된다   

**middle**   
- `middle`은 최대 14개까지 존재할 수 있다   
- `middle`은 공백을 포함할 수 없다    
- `middle`은 쉼표(`,`)를 사용해 내부적으로 여러 개의 값을 가질 수 있다     
  (e.g.: `JOIN #channel1,#channel2 111,111`)  
  → 여기서 `#channel1,#channel2`, `111,111` 두 개의 `middle`이 된다    

**trailing**   
- `trailing`은 오직 **1개만 존재 가능**하다    
- `trailing`은 `:`(콜론)으로 시작해야 한다    
- `trailing`은 **공백을 포함할 수 있다.**   
- `:` 이후에 오는 `trailing`은 하나의 파라미터로 묶이며, 더 이상 공백으로 나뉘지 않는다     
  (e.g.: `PRIVMSG #channel :Hello, how are you?`)     
  → 여기서 `"Hello, how are you?"`는 하나의 `trailing` 파라미터가 된다    


```c++
vector<std::string> middle;
std::string trailing;
```

PARSE를 위해 들어오는 명령어에는 source가 들어오지 않는다    
대부분의 양식이 

```abnf
<command> <parameters> <crlf>
```

으로 들어오게되며 이 둘만 가지고 분석을 수행하며 source의 주체는 command의 종류와 결과값에 따라 나뉜다   

---

## INFO

### USER
REAL_NAME: real_ken      
USER_NAME: user_ken   
NICK_NAME: nick_ken   

### SERVER
{SERVER_NAME}: irc.rizon.net     

### CLINET
CLINET_NAME: irssi    
CLINET_VERSION: 1.45      

### RULES

- USER는 3명이 존재한다   
  *_ken: 처음 채널에 들어온 채널 운영자   
  *_ken2: 이후에 들어온 채널 사용자   
  *_ken3: 마지막에 들어온 채널 사용자     

- user_ken 앞에오는 `~`는 검증되지 않은 사용자를 나타낸다     

- 일반적으로 `:`로 시작하는 문자열은 서버가 전송해주는 문자열이다      
  `":string "` 의 형태로 문자열이 감싸져 있게 된다면 해당 문자열은 다른 클라이언트들에게도 보여지는 것이다      

- 위의 유저, 서버, 클라이언트 이름들과 특정 단어, 문자들은 언제든지 `{}` 를 통해 표기될 수 있다   
  e.g.: {SERVER_NAME}, {MASKED_IP}   

- 서버에서 보내는 모든 메시지들은 임의로 수정하여도 된다   
  이는 그저 상용서버에서 어떻게 보내는지 알리기 위해 적었으며 그대로 작성해도 된다   

- {NICK_NAME}!{USER_NAME}@{HOST or MAKSED_IP} 와 같은 source 양식은 그냥 {NICK_NAME} 만 표기하여도 된다    
  e.g.: :dan-!d@localhost QUIT :Quit: Bye for now! --> :dan-!d QUIT :Quit: Bye for now!    
        :nick_ken!~user_ken@{MASKED_IP}.IP JOIN :#{CHANNEL_NAME} --> nick_ken JOIN :#{CHANNEL_NAME}    


---


irssi는 위의 명령어를 통해 서버에 연결시 다음 네가지 명령어를 전송한다   
- NICK nick_ken   
- USER user_ken user_ken {SERVER_NAME} :real_ken   

위의 USER 명령어를 통해 서버에서 전달받은 NICK_NAME을 MODE의 인자로 전달한다   
- MODE nick_ken +i   
- WHOIS nick_ken   

현 Just1RCe 프로젝트에서는 WHOIS 명령어는 구현하지 않으므로 무시하도록 한다     

## <details> <summary> CONNECTION MESSAGES </summary>

### NICK   

**사용자의 NICK_NAME을 설정하는 명령어**   

#### FORMAT

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

#### Numerics

**ERR_NONICKNAMEGIVEN (431) 인자로 아무것도 들어오지 않은 경우 무시한다**    
```
Client:
  NICK

SERVER:
  :{SERVER_NAME} 431 nick_ken3 :No nickname given
```

**ERR_NICKNAMEINUSE (433) 이미 사용중인 이름인 경우 무시한다**       
```
Client:
  NICK nick_ken

SERVER:
  :{SERVER_NAME} 433 nick_ken3 nick_ken :Nickname is already in use.
```

**ERR_ERRONEUSNICKNAME (432) 잘못된 형식의 이름인 경우 무시한다**   
```
Client:
  !@#%#@$%^

SERVER:
  :{SERVER_NAME} 432 nick_ken3 !@#%#@$%^ :Erroneous Nickname
```

### USER   

**사용자의 유저 이름, 실제 이름등을 서버에 정의하는 명령어**   

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

#### Numerics

**ERR_NEEDMOREPARAMS (461) 인자가 제대로 들어오지 않은 경우 무시한다**   

```
CLIENT: 
  USER    

SERVER:
  :{SERVER_NAME} 461 nick_ken3 USER :Not enough parameters
```   

**ERR_ALREADYREGISTERED (462) 이미 등록된 사용자인 경우 무시한다**
```
CLIENT: 
  USER user_ken user_ken irc.rizon.net :real_ken    

SERVER:
  :{SERVER_NAME} 462 nick_ken3 :You may not reregister
```

---

위의 NICK 명령어와 USER 명령어를 서버가 받아 들이게 되면 서버는 적절한 Welcome Message를 포함한 메시지들을 반환한다   
이 Welcome Message들은 irssi 클라이언트에서서 `001, 002` 까지만 필수이며 그 외에는 작성하지 않아도 된다    

rizon 서버는 다음과 같이 응답한다

```
PING :3340719355
[DEBUG] Responding to PING with: PONG :3340719355

:{SERVER_NAME} NOTICE nick_ken :*** Your host is masked ({Masked_IP}.IP)
↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓(this is welcom message from server to client)
:{SERVER_NAME} 001 nick_ken :Welcome to the Rizon Internet Relay Chat Network nick_ken
:{SERVER_NAME} 002 nick_ken :Your host is {SERVER_NAME}, running version {SERVER_VERSION}
:{SERVER_NAME} 003 nick_ken :This server was created Mar 19 2022 at 21:25:18
:{SERVER_NAME} 004 nick_ken {SERVER_NAME} {SERVER_VERSION} CDGNRSUWagilopqrswxyz BCIMNORSabcehiklmnopqstvz Iabehkloqv
:{SERVER_NAME} 005 nick_ken CALLERID CASEMAPPING=rfc1459 DEAF=D KICKLEN=180 MODES=4 PREFIX=(qaohv)~&@%+ STATUSMSG=~&@%+ EXCEPTS=e INVEX=I NICKLEN=30 NETWORK=Rizon MAXLIST=beI:250 MAXTARGETS=4 :are supported by this server
:{SERVER_NAME} 005 nick_ken CHANTYPES=# CHANLIMIT=#:250 CHANNELLEN=50 TOPICLEN=390 CHANMODES=beI,k,l,BCMNORScimnpstz NAMESX UHNAMES KNOCK WATCH=60 AWAYLEN=180 ELIST=CMNTU SAFELIST :are supported by this server
↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
:{SERVER_NAME} 251 nick_ken :There are 17 users and 9258 invisible on 18 servers
:{SERVER_NAME} 252 nick_ken 47 :IRC Operators online
:{SERVER_NAME} 253 nick_ken 1 :unknown connection(s)
:{SERVER_NAME} 254 nick_ken 7813 :channels formed
:{SERVER_NAME} 255 nick_ken :I have 9275 clients and 18 servers
:{SERVER_NAME} 265 nick_ken :Current local users: 9275  Max: 10299
:{SERVER_NAME} 266 nick_ken :Current global users: 9275  Max: 10299
:{SERVER_NAME} 375 nick_ken :- {SERVER_NAME} Message of the Day - 
:{SERVER_NAME} 372 nick_ken :-          11oo                            
:{SERVER_NAME} 372 nick_ken :- 
:{SERVER_NAME} 372 nick_ken :- 88d888b. dP d888888b .d8888b. 88d888b. 
:{SERVER_NAME} 372 nick_ken :- 88'  `88 88    .d8P' 88'  `88 88'  `88 
:{SERVER_NAME} 372 nick_ken :- 1188       88  .Y8P    88.  .88 88    88 
:{SERVER_NAME} 372 nick_ken :- 11dP       dP d888888P `88888P' dP    dP
:{SERVER_NAME} 372 nick_ken :- 
:{SERVER_NAME} 372 nick_ken :- Rizon Chat Network -- http://rizon.net
:{SERVER_NAME} 372 nick_ken :- 
:{SERVER_NAME} 372 nick_ken :- Listening on ports: 6660 - 6669, 7000. SSL: 6697, 9999
:{SERVER_NAME} 372 nick_ken :- 
:{SERVER_NAME} 372 nick_ken :- Rules:
:{SERVER_NAME} 372 nick_ken :- o No spamming or flooding
:{SERVER_NAME} 372 nick_ken :- o No clones or malicious bots
:{SERVER_NAME} 372 nick_ken :- o No takeovers
:{SERVER_NAME} 372 nick_ken :- o No distribution of child pornography
:{SERVER_NAME} 372 nick_ken :- o Clients must respond to VERSION requests
:{SERVER_NAME} 372 nick_ken :- o Rizon staff may disconnect clients for any or no reason
:{SERVER_NAME} 372 nick_ken :- 
:{SERVER_NAME} 372 nick_ken :- First steps:
:{SERVER_NAME} 372 nick_ken :- o To register your nick: /msg NickServ HELP
:{SERVER_NAME} 372 nick_ken :- o To register your channel: /msg ChanServ HELP
:{SERVER_NAME} 372 nick_ken :- o To get a vHost: /msg HostServ HELP REQUEST
:{SERVER_NAME} 372 nick_ken :- o For other help with Rizon: /join #help
:{SERVER_NAME} 372 nick_ken :- 
:{SERVER_NAME} 372 nick_ken :- Usage of this network is a privilege, not a right. Rizon is a
:{SERVER_NAME} 372 nick_ken :- transit provider, therefore no person or entity involved with
:{SERVER_NAME} 372 nick_ken :- *.rizon.net or {SERVER_NAME} takes any responsibility for
:{SERVER_NAME} 372 nick_ken :- users' actions. Absolutely no warranty is expressed or implied.
:{SERVER_NAME} 376 nick_ken :End of /MOTD command.
:nick_ken!~user_ken@Masked-IP.IP MODE nick_ken :+ix
:py-ctcp!ctcp@ctcp-scanner.rizon.net PRIVMSG nick_ken :VERSION
:Global!service@rizon.net NOTICE nick_ken :[Logon News - Dec 16 2013] Own a large/active channel or plan to get one going? Please read http://s.rizon.net/authline
:Global!service@rizon.net NOTICE nick_ken :[Logon News - Feb 16 2022] First time on Rizon? Be sure to check out our wiki! https://wiki.rizon.net
:Global!service@rizon.net NOTICE nick_ken :[Random News - Nov 29 2013] Want to make your vhost shiny by adding colors to it? Try your luck in #VIzon
```


### PING   

**서버와 연결 여부를 확인하기 위해 사용하는 명령어**    

#### FORMAT
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
  :{SERVER_NAME} PONG {SERVER_NAME} :hello
```   
 

- <token> 은 어떤 문자열이든 올 수 있다 
  (비어있지만 않으면 된다)   


#### Numerics

**- 클라이언트는 연결 등록 과정 중에 PING을 보내지 않아야 한다**   
```
SERVER:
  :{SERVER_NAME} 451 * :You have not registered (닉네임 등록이 안된 경우)
  :{SERVER_NAME} 451 nick_ken3 :You have not registered (닉네임 등록이 되어 있는 경우)
```

**ERR_NOORIGIN (409) 인자가 없는 경우**   
```
Client:
  PING
SERVER: 
  :{SERVER_NAME} 409 nick_ken3 :No origin specified
```

- 

### PONG

**PING 명령어에 응답하는 명령어**   
#### FORMAT
- FORMAT   
```bnf
     Command: PONG
  Parameters: [<server>] <token>
```

- Message Transaction   
```bash
SERVER:
  PING :{SERVER_NAME}

CLIENT:
  PONG :{SERVER_NAME}
``` 

- PONG 명령어는 반드시 PING에서 받은 <token> 값을 그대로 포함해야 한다

### QUIT    

**서버에서 나가는 명령어**   
#### FORMAT
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
  **All received**
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
    **All received**
    :nick_ken2!~user_ken2@{MASKED_IP}.IP QUIT :
  ```

  여기서 유저 이름 앞에 붙어있는 ~ 는 인증되지 않은 유저임을 나타냄    
  서버에서는 이유를 받아도 돌려주지 않으며 최소한 `QUIT :`는 작성하는 것을 볼 수 있음

2. Client가 비정상적으로 연결이 종료된 경우   
  ```
  Client:

  Server:
    **All received**
    :nick_ken2!~user_ken2@{MASKED_IP}.IP QUIT :Remote host closed the connection
  ```
  여기서는 서버가 클라이언트의 상태 정보를 작성해준다


</details>


## <details> <summary> CHANNEL MESSAGES  </summary>

### JOIN

**채널에 접속하기 위한 명령어**
#### FORMAT
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
    JOIN #{CHANNEL_NAME}

  SERVER:
    :nick_ken!~user_ken@{MASKED_IP}.IP JOIN :#{CHANNEL_NAME}
    :{SERVER_NAME} MODE #{CHANNEL_NAME} +nt
    :{SERVER_NAME} 353 nick_ken = #{CHANNEL_NAME} :@nick_ken
    :{SERVER_NAME} 366 nick_ken #nick_ken :End of /NAMES list.
  ```   

- 클라이언트가 JOIN 명령을 성공적으로 실행하면, 서버는 다음 순서로 응답을 보내야 한다   
  ```
  CLIENT:
    JOIN #{CHANNEL_NAME}
  ```

  1. JOIN message (채널에 참여하고 있는 모든 클라이언트들에게) 
    ```
    SERVER:
      **All received**
      :nick_ken3!~user_ken3@{MASKED_IP}.IP JOIN :#{CHANNEL_NAME}
    ```
  2. TOPIC message (TOPIC 설정이 없다면 생략 가능)   
    ```
    SERVER:
      :{SERVER_NAME} 332 nick_ken3 #{CHANNEL_NAME} :hello
    ```
  3. 현재 채널에 있는 사용자 목록   
    ```
    SERVER:
      :{SERVER_NAME} 353 nick_ken3 = #{CHANNEL_NAME} :nick_ken3 nick_ken2 @nick_ken
      :{SERVER_NAME} 366 nick_ken3 #{CHANNEL_NAME} :End of /NAMES list.
    ```

- 클라이언트가 특정 채널(들)에 참여하려고 한다는 것을 나타내며, 필요한 경우 비밀번호(key)를 사용한다    

- 서버는 해당 명령을 받으면 **해당 채널에 참여할 수 있는지 확인** 한 후 요청을 처리 한다    
  외에도 여러가지 이유로 채널 참가가 제한 될 수 있다   

- 각 <channel> 과 <key>는 순서에 맞게 적용된다 (첫 번째 채널에 첫 번째 키, 두 번째 채널에 두 번째 키)   

- 인자로 대신 0을 전달하면 현재 참여중인 모든 채널에서 나가게 된다 (각 채널에서 PART하는 효과를 가짐)   
  ```
  Client:
    JOIN 0
  
  SERVER:
    :nick_ken!~user_ken@{MASKED_IP}.IP PART #ccc
    :nick_ken!~user_ken@{MASKED_IP}.IP PART #bbb
    :nick_ken!~user_ken@{MASKED_IP}.IP PART #aaa
  ```

#### Numerics

**RPL_TOPIC (332) 성공적인 JOIN 명령어 실행**      
```
SERVER:
  :{SERVER_NAME} 332 nick_ken3 #{CHANNEL_NAME} :hello
```

**ERR_CHANNELISFULL (471) 채널에 참여할 수 있는 클라이언트 제한을 넘은 경우**    
```
:{SERVER_NAME} 471 nick_ken3 #limited_channel :Cannot join channel (+l)
```   

**ERR_BADCHANNELKEY (475) 비밀번호를 잘못 입력한 경우**     
```
:{SERVER_NAME} 475 nick_ken3 #pass_channel :Cannot join channel (+k)
```     

**ERR_INVITEONLYCHAN (473) 채널이 초대 전용으로 이루어진 경우**   
```
:{SERVER_NAME} 473 nick_ken3 #invite_only :Cannot join channel (+i)
``` 

**ERR_NEEDMOREPARAMS (461) 인자가 없는 경우**
```
CLIENT: 
  JOIN

SERVER:
  :{SERVER_NAME} 461 nick_ken3 JOIN :Not enough parameters
```   

**ERR_NOSUCHCHANNEL (403) 존재하지 않는 채널에 JOIN을 시도했으나 채널이 자동 생성되지 않은 경우**
```
Client:
  JOIN #nonexistent_channel

Server:
  :{SERVER_NAME} 403 nick_kenn3 #nonexistent_channel :No such channel

```   

**ERR_TOOMANYCHANNELS (405) 채널 참가 한도가 초과된 경우**
```
Client:
  JOIN #channel5

Server:
  :{SERVER_NAME} 405 nick_kenn3 #channel5 :You have joined too many channels
```   

**ERR_BADCHANMASK (476) 잘못된 채널 이름인 경우**
```
Client:
  JOIN !#$%!

Server:
  :{SERVER_NAME} 476 nick_kenn3 !#$%! :Bad Channel Mask (IRC 문서)
                                or
  :{SERVER_NAME} 479 nick_ken3 !#$%! :Illegal channel name (현재 상용 서버)
```   


### PART

**채널에서 나가는 명령어**   
#### FORMAT
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
  **All received**
  :dan-!d@localhost PART #test    ; dan- 가 채널 test를 떠남
```

- `reason`은 선택사항으로 구현하지 않는다   

#### Numerics

**ERR_NOTONCHANNEL (442) 채널이 존재하지만 사용자가 해당 채널에 참가하고 있지 않는다면 요청을 무시한다**   
  ```
  :{SERVER_NAME} 442 nick_ken3 #test :You're not on that channel
  ```

**ERR_NOSUCHCHANNEL (403) 지정된 채널이 존재하지 않는다면 요청을 무시한다**    
  ```
  :{SERVER_NAME} 403 nick_ken3 #test :No such channel
  ```

### TOPIC

**채널의 주제를 불러오거나 설정하는 명령어**   
#### FORMAT
- FORMAT   
```bnf
     Command: TOPIC
  Parameters: <channel> [<topic>]
```

- Message Transaction   
```bash
CLIENT:
  TOPIC #test :New topic          ; 채널 "#test"에 새로운 토픽 "New topic" 을 설정

  TOPIC #test :                   ; 채널 "#test"의 토픽 초기화

  TOPIC #test                     ; 채널 "#test"의 토픽 확인

SERVER:
  **All received**
  :nick_ken!~user_ken@{MASKED_IP}.IP TOPIC #test :New Topic 

  **All received**
  :nick_ken!~user_ken@{MASKED_IP}.IP TOPIC #test :

  :{SERVER_NAME} 331 nick_ken #test :No topic is set.
  
```

- TOPIC 명령어는 지정된 채널의 주제를 변경하거나 조회하는 데 사용된다   

- <topic> 매개변수가 빈 문자열이면, 해당 채널의 주제가 삭제된다   

- 채널의 주제가 변경되거나 삭제되면, 해당 채널의 모든 클라이언트(주제 변경을 수행한 클라이언트 포함)는 새로운 주제를 인자로 포함한 `TOPIC` 명령을 받아 변경 사항을 알게 된다 (주제가 삭제된 경우, 인자는 빈 문자열이 된다)   

- <topic> 매개변수가 제공되었지만 기존 주제와 동일한 경우, 서버는 여전히 변경 요청을 한 사용자 및 다른 사용자에게 이를 알릴 수도 있다   

- 추후 해당 채널에 참여하는 클라이언트는 `RPL_TOPIC` 응답(또는 주제가 없는 경우 해당 응답 없음)을 받게 된다   

#### Numerics

**ERR_NEEDMOREPARAMS (461)	필요한 매개변수가 부족함**    

<topic> 매개변수가 제공되지 않으면, 현재 채널의 주제를 나타내는 `RPL_TOPIC` 또는 주제가 없음을 나타내는 `RPL_NOTOPIC`이 반환된다   

```
Client:
  TOPIC

Server:
  :{SERVER_NAME} 461 nick_ken TOPIC :Not enough parameters
```   

**ERR_NOSUCHCHANNEL (403)	존재하지 않는 채널**   
```
Client:
  TOPIC #notexistchannel

Server:
  :{SERVER_NAME} 403 nick_ken #notexistchannel :No such channel
```   

**ERR_NOTONCHANNEL (442)	채널에 참여하지 않은 상태에서 주제 조회 시도**   

명령을 보낸 클라이언트가 해당 채널에 참여하지 않은 상태에서 주제를 조회하려고 하면, 서버는 `ERR_NOTONCHANNEL (442)` 응답을 반환할 수도 있으며, 명령이 실패할 수 있다   

```
Client:
  TOPIC #notjoinedchannel

Server:
  :{SERVER_NAME} 442 nick_ken #notjoinedchannel :You're not on that channel
```   

**ERR_CHANOPRIVSNEEDED (482)	주제를 변경할 권한이 없음**   

채널에 보호된 주제 모드가 설정되어 있다면, 클라이언트는 채널 주제를 변경할 적절한 권한을 가져야 한다   
클라이언트가 적절한 채널 권한 없이 주제를 변경하려 하면, `ERR_CHANOPRIVSNEEDED (482)` 응답이 반환되며 명령이 실패한다   

```
Client:
  TOPIC #test 11

Server:
  :{SERVER_NAME} 482 nick_ken2 #test :You're not channel operator
```   

**RPL_NOTOPIC (331)	채널에 설정된 주제가 없음**   

```
Client:
  TOPIC #test

Server:
  :{SERVER_NAME} 331 nick_ken #test :No topic is set.
```     

**RPL_TOPIC (332)	채널의 현재 주제**   
**RPL_TOPICWHOTIME (333)	주제 변경을 수행한 사용자 및 변경 시간**    

`RPL_TOPIC` 응답이 클라이언트에게 반환되면, `RPL_TOPICWHOTIME` 응답도 함께 보내야  한다   

```
Client:
  TOPIC #test

Server:
  :{SERVER_NAME} 332 nick_ken #test :Now topic
  :{SERVER_NAME} 333 nick_ken #test nick_ken!~user_ken@{MASKED_IP}.IP {TIME_STAMP}
```   

### NAMES

**특정 채널에 참여한 사용자의 닉네임과 채널 내 권한 접두사를 조회하는  명령어**   
#### FORMAT
- FORMAT   
```bnf
     Command: NAMES
  Parameters: <channel>{,<channel>}
```

- Message Transaction   
```bash
CLIENT:
  NAMES #twilight_zone,#42  ; #twilight_zone과 #42 채널에 있는 사용자의 목록을 조회

  NAMES                     ; 네트워크에서 조회 가능한 모든 사용자의 목록을 요청
                              (그러나 대부분의 서버는 이 요청을 제한하거나 부분적으로 응답할 가능성이 높다)

SERVER:
  :{SERVER_NAME} 366 nick_ken * :End of /NAMES list.

  :{SERVER_NAME} 353 nick_ken2 = #twilight_zone :nick_ken3 nick_ken2 @nick_ken
  :{SERVER_NAME} 366 nick_ken2 #twilight_zone :End of /NAMES list.
```

- 사용자가 `invisible`(보이지 않는) 모드를 설정한 경우, 요청한 클라이언트가 해당 채널에 참여하지 않았다면 해당 사용자는 응답에 포함되지 않는다   
  
- 서버는 사용자가 볼 수 있는 채널에 대해 `RPL_NAMREPLY` 응답을 하나 이상 반환하며, 해당 채널에 참여한 사용자 목록을 포함합니다. 또한 `RPL_ENDOFNAMES` 응답을 한 번 반환한다      

- 다중 채널에 대한 구현을 할 필요는 없다   

    Servers MAY allow more than one target channel.
    서버는 하나 이상의 채널을 대상으로 하는 요청을 허용할 수도 있습니다.

  위의 내용을 통해 여러 채널들을 한번에 받아 여러 채널의 사용자 목록을 조회를 구현할 필요 없음을 알 수 있다   

- 채널 이름이 유효하지 않거나 채널이 존재하지 않는 경우, 서버는 해당 채널 이름을 포함한 `RPL_ENDOFNAMES` 응답을 반환해야 한다

  ```
  Client:
    NAMES #noexistchannel

  Server:
    :{SERVER_NAME} 366 nick_ken #noexistchannel :End of /NAMES list.
  ```     

#### Numerics   

**RPL_NAMREPLY (353)	채널에 있는 사용자 목록 반환**   
**RPL_ENDOFNAMES (366)	사용자 목록 전송 완료 또는 채널 없음**   

```
Client:
  NAMES #{CHANNEL_NAME}

Server:
  :{SERVER_NAME} 353 nick_ken2 = #{CHANNEL_NAME} :nick_ken3 nick_ken2 @nick_ken
  :{SERVER_NAME} 366 nick_ken2 #{CHANNEL_NAME} :End of /NAMES list.
```     
  
### LIST

**채널 목록과 각 채널에 대한 정보를 가져오는 명렁어**   
#### FORMAT
- FORMAT   
```bnf
     Command: LIST
  Parameters: [<channel>{,<channel>}]
```

- Message Transaction   
```bash
CLIENT:
  LIST                            ; 모든 공개 채널을 조회

  LIST #twilight_zone,#42         ; #twilight_zone과 #42 채널의 정보를 조회

SERVER:
  :{SERVER_NAME} 321 nick_ken Channel :Users  Name
  :{SERVER_NAME} 322 nick_ken #ccc {USER_NUMBER} :[+nt] {TOPIC_MESSAGE}
  :{SERVER_NAME} 322 nick_ken #bbb {USER_NUMBER} :[+nt] {TOPIC_MESSAGE}
  :{SERVER_NAME} 322 nick_ken #{CHANNEL_NAME} {USER_NUMBER} :[+ntk] {TOPIC_MESSAGE}
  :{SERVER_NAME} 323 nick_ken :End of /LIST

  :{SERVER_NAME} 321 nick_ken Channel :Users  Name
  :{SERVER_NAME} 323 nick_ken :End of /LIST
```

- 다중 채널 조회 기능을 지원하지 않아도 된다     

    The first possible parameter to this command is a list of channel names, delimited by a comma (",", 0x2C) character.
    이 명령어의 첫 번째 매개변수는 쉼표(",", 0x2C)로 구분된 채널 이름 목록입니다.

    If this parameter is given, the information for only the given channels is returned.
    이 매개변수가 제공되면, 해당 채널들의 정보만 반환됩니다.

    In response to a successful LIST command, the server MAY send one RPL_LISTSTART numeric, MUST send back zero or more RPL_LIST numerics, and MUST send back one RPL_LISTEND numeric.
    LIST 명령어가 성공적으로 처리되면, 서버는 `RPL_LISTSTART(321)` 응답을 보낼 수도 있으며(`MAY`), 반드시(`MUST`) 0개 이상의 `RPL_LIST(322)` 응답을 보내고, 마지막으로 `RPL_LISTEND(323)` 응답을 보내야 합니다.   

  위의 IRC 문서에 적혀있는 내용을 보아 반드시 다중채널을 지원할 필요는 없다고 여겨지며,    
  이를 상용서버에서의 동작 안함과 IRC 문서 내에서 반드시 처리하라는 내용이 없으므로 이를 근거로 다중 채널 명령은 구현하지 않는다   

- :{SERVER_NAME} 322 nick_ken #{CHANNEL_NAME} {USER_NUMBER} :[+ntk] {TOPIC_MESSAGE} 


#### Numerics
**RPL_LISTSTART (321)	채널 목록 시작 (선택적 응답)**     
**RPL_LIST (322)	채널 정보 응답**     
**RPL_LISTEND (323)	채널 목록 끝**     

```  
Client:
  LIST
  
Server:
  :{SERVER_NAME} 321 nick_ken Channel :Users  Name   
  :{SERVER_NAME} 322 nick_ken #ccc {USER_NUMBER} :[+nt] {TOPIC_MESSAGE}
  :{SERVER_NAME} 322 nick_ken #bbb {USER_NUMBER} :[+nt] {TOPIC_MESSAGE}
  :{SERVER_NAME} 322 nick_ken #{CHANNEL_NAME} {USER_NUMBER} :[+ntk] {TOPIC_MESSAGE}
  :{SERVER_NAME} 323 nick_ken :End of /LIST
```     

```
Client:
  LIST #{CHANNEL_NAME}
  
Server:
  :{SERVER_NAME} 321 nick_ken Channel :Users  Name
  :{SERVER_NAME} 322 nick_ken #{CHANNEL_NAME} {USER_NUMBER} :[+ntk] {TOPIC_MESSAGE}
  :{SERVER_NAME} 323 nick_ken :End of /LIST
```     


### INVITE

**사용자를 채널로 초대하는 명령어**   
#### FORMAT
- FORMAT   
```bnf
     Command: INVITE
  Parameters: <nickname> <channel>
```

- Message Transaction   
```bash
CLIENT:
  INVITE Wiz #foo_bar    ; Invite Wiz to #foo_bar

SERVER:
  :dan-!d@localhost INVITE Wiz #test    ; dan- 사용자가 Wiz를 #test 채널로 초대했음을 나타내는 메시지
```

- `<nickname>` 매개변수는 초대할 사용자의 닉네임이며, `<channel>` 매개변수는 초대할 대상 채널이다   
- 다른 채널 참여자들에게는 초대 사실이 알림으로 전송되지 않아야 한다   
- 권한이 없는 사용자가 다른 사용자를 초대하는 경우에 대해서는 처리하지 않아도 된다   

    Servers MAY reject the command with the ERR_CHANOPRIVSNEEDED numeric.
    서버는 `ERR_CHANOPRIVSNEEDED` 응답과 함께 명령을 거부할 수도 있습니다.
    In particular, they SHOULD reject it when the channel has invite-only mode set, and the user is not a channel operator.
    특히, 채널이 `invite-only` 모드로 설정되어 있고 사용자가 운영자가 아니라면, 서버는 초대 요청을 거부해야 합니다.

  위 내용에서 `MAY`, `should` 와 같은 `할 수도`, `권장한다`는 것을 근거로 구현하지 않아도 됨을 알 수 있으며, 실제 rizon 상용서버에서도 구현이 되지 않음을 볼 수 있다   


#### Numerics

**RPL_INVITING (341)  초대가 성공했음을 나타내는 응답**   

초대가 성공하면, 서버는 명령을 보낸 사용자에게 `RPL_INVITING` 응답을 보내야 하며, 초대 메시지를 `<source>`(초대한 사용자)와 함께 초대 대상 사용자에게 전송해야 한다   


```
Client:
  INVITE nick_ken2 #{CHANNEL_NAME}

Server:
  :{SERVER_NAME} 341 nick_ken nick_ken2 #{CHANNEL_NAME}

INVITED CLIENT:
  :nick_ken!~user_ken@MASKED_IP.IP INVITE nick_ken2 :#{CHANNEL_NAME}
```     

**ERR_NEEDMOREPARAMS (461) 필요한 매개변수가 부족할 때**    

```
Client:
  INVITE

Server:
  :{SERVER_NAME} 461 nick_ken INVITE :Not enough parameters
```     

**ERR_NOSUCHNICK (401) 초대하려는 사용자가 없을때 나타내는 응답**   

```
Client:
  INVITE noexistuser #test
  
Server:
  :{SERVER_NAME} 401 nick_ken noexistuser :No such nick/channel
```     

**ERR_NOSUCHCHANNEL (403)  존재하지 않는 채널을 대상으로 초대하려 할 때**    

대상 채널은 존재해야 하며(적어도 한 명 이상의 사용자가 있어야 함), 그렇지 않으면 서버는 `ERR_NOSUCHCHANNEL(403)` 응답과 함께 명령을 거부해야 한   
```
Client:
  INVITE nick_ken2 #noexistchannel
  
Server:
  :{SERVER_NAME} 403 nick_ken noexistchannel :No such channel
```     


**ERR_NOTONCHANNEL (442)  초대한 사용자가 해당 채널에 참여하고 있지 않을 때**   

채널에 참여한 사용자만 다른 사용자를 초대할 수 있으며, 그렇지 않으면 서버는 `ERR_NOTONCHANNEL(442)` 응답과 함께 명령을 반드시 거부해야 한다   

```
Client:
  INVITE nick_ken2 #notjoinedchannel
Server:
  :{SERVER_NAME} 442 nick_ken #notjoinedchannel :You're not on that channel
```     


**ERR_USERONCHANNEL (443)  초대하려는 사용자가 이미 채널에 있을 때**   

초대하려는 사용자가 이미 대상 채널에 참여해 있다면, 서버는 `ERR_USERONCHANNEL` 응답과 함께 명령을 반드시 거부해야 한다   
```
Client:
  INVITE alreadyjoineduser #{CHANNEL_NAME}
Server:
  :{SERVER_NAME} 443 nick_ken alreadyjoineduser #{CHANNEL_NAME} :is already on channel
```     


### KICK

**특정 사용자를 강제로 채널에서 제거하는 명령어**    
#### FORMAT
- FORMAT   
```bnf
      Command: KICK
   Parameters: <channel> <user> *( "," <user> ) [<comment>]
```

- Message Transaction   
```bash
CLIENT:
  KICK #Finnish Matthew           ; #Finnish 채널에서 Matthew 사용자를 강제 퇴장시킴.

  KICK #Finnish John :Speaking English ; (영어를 사용함)이라는 이유를 코멘트로 #Finnish 채널에서 John을 추방함

SERVER:
  :WiZ!jto@tolsun.oulu.fi KICK #Finnish Matthew ; WiZ 사용자가 #Finnish 채널에서 Matthew를 강제 퇴장시켰다는 메시지

  :{NICK_NAME}!{USER_NAME}@{MASKED_IP}.IP KICK #Finnish John :Speaking English
```

- 자기자신을 KICK 할 수 있다     
- 사용자를 KICK 하는 경우 
- 코멘트가 제공되지 않으면, 서버는 기본 메시지를 사용해야 한다    
- 서버는 클라이언트에게 한 번에 여러 사용자를 포함한 KICK 메시지를 보낼 수 없으며, 
  반드시 **각 메시지에 하나의 사용자만 포함하여 개별적으로 전송해야 한다**

#### Numerics

**ERR_NEEDMOREPARAMS (461) 필요한 매개변수가 부족할 때**     
```
Client:
  KICK

Server:
  :{SERVER_NAME} 461 nick_ken KICK :Not enough parameters
```     


**ERR_NOSUCHCHANNEL (403) 존재하지 않는 채널에서 사용자를 강퇴하려 할 때**     
```
Client:
  KICK #noexistchannel nick_ken2
  
Server:
  :{SERVER_NAME} 403 nick_ken #noexistchannel :No such channel
```     


**ERR_CHANOPRIVSNEEDED (482) 강퇴할 권한이 없는 경우**     
```
Client:
  KICK #{CHANNEL_NAME} nick_ken
Server:
  :{SERVER_NAME} 482 nick_ken2 #{CHANNEL_NAME} :You're not channel operator
```     


**ERR_USERNOTINCHANNEL (441) 강퇴하려는 사용자가 해당 채널에 없을 때**     
```
Client:
  KICK #{CHANNEL_NAME} noexistuser
  
Server:
  :{SERVER_NAME} 401 nick_ken noexistuser :No such nick/channel
```     


**ERR_NOTONCHANNEL (442) KICK 요청자가 해당 채널에 속해 있지 않을 때**      
```
Client:
  KICK #{CHANNEL_NAME} nick_ken2
Server:
  :{SERVER_NAME} 442 nick_ken3 #{CHANNEL_NAME} :You're not on that channel
```     


</details>

## SERVER QUERY

### MODE   
#### FORMAT
**특정 대상의 옵션(또는 모드)을 설정하거나 제거하는 명령어**   

- FORMAT   
```bnf
     Command: MODE
  Parameters: <target> [<modestring> [<mode arguments>...]]
```

- 모드 문자열(<modestring>) 형식      
  + → 이후에 나오는 모드를 추가   
  - → 이후에 나오는 모드를 제거   
  a-z, A-Z → 설정 또는 제거할 모드 문자   

- 구현할 MODE 종류   
  - o: 채널 운영자 권한 부여   
  - i: 초대 전용 채널   
  - m: 조용한 채널    
  - n: 외부 메시지 차단   
  - t: 주제 제한   
  - k: 비밀번호 보호   
  - l: 최대 사용자 수 제한   

- Message Transaction   
```
CLIENT: 
  MODE #{CHANNEL_NAME} +i                        ; 채널에 초대 전용 채널 모드를 설정   
  MODE #{CHANNEL_NAME} +k {PASSWORD}  ; 채널에 비밀번호 설정

SERVER:
  :nick_ken!~user_ken@{MASKED_IP}.IP MODE #{CHANNEL_NAME} +i
  :nick_ken!~user_ken@{MASKED_IP}.IP MODE #{CHANNEL_NAME} +k {PASSWORD}
```

- 모드에는 여러 유형으로 나뉘며, 동작이 다르다   
  Type A: 리스트를 관리하는 모드 (+b 밴 목록, +e 예외 목록 등)   
    mode arguments에서 순차적으로 처리      
    인자가 없이 명령이 실행되면, 서버는 해당 목록의 내용을 사용자에게 반환해야 한다   
    (단, 해당 목록이 민감한 정보를 포함하고 있고, 사용자가 이를 볼 권한이 없다면 제외될 수 있음)   

  Type B: 항상 인자가 필요한 모드 (예: +k 채널 키 설정)   
    mode arguments에서 순차적으로 처리   
    필요한 인자가 제공되지 않으면, 서버는 해당 모드를 무시   
  Type C: 설정 시에만 인자가 필요하고, 해제할 때는 인자가 필요하지 않은 모드 (+l 사용자 제한)   
    mode arguments에서 순차적으로 처리   
    필요한 인자가 제공되지 않으면, 서버는 해당 모드를 무시
  Type D: 인자가 필요하지 않은 모드 (+m 모더레이션 모드, +t 주제 변경 제한 등)   

- Type A 
  - o   
    ```
    CLIENT: 
      MODE #aaa +o nick_ken2

      MODE #aaa -o nick_ken2

    SERVER:
      **All received**
      :nick_ken!~user_ken@{MASKED_IP}.IP MODE #aaa +o nick_ken2

      **All received**
      :nick_ken!~user_ken@{MASKED_IP}.IP MODE #aaa -o nick_ken2
    ```   


- Type B 
  - k   
    ```
    CLIENT: 
      MODE #aaa +k 111

      MODE #aaa -k

    SERVER:
      **All received**
      :nick_ken!~user_ken@{MASKED_IP}.IP MODE #aaa +k 111
    
      **All received**
      :nick_ken!~user_ken@{MASKED_IP}.IP MODE #aaa -k *
    ```   

- Type C 
  - l   
    ```
    CLIENT: 
      MODE #aaa +l 3

      MODE #aaa -l 3

    SERVER:
      **All received**
      :nick_ken!~user_ken@{MASKED_IP}.IP MODE #aaa +l 3

      **All received**
      :nick_ken!~user_ken@{MASKED_IP}.IP MODE #aaa -l
    ```   

- Type D   
  - i   
    ```
    CLIENT: 
      MODE #aaa +i

    SERVER:
      **All received**
      :nick_ken!~user_ken@{MASKED_IP}.IP MODE #aaa +i

      **All received**
      :nick_ken!~user_ken@{MASKED_IP}.IP MODE #aaa -i
    ```   

  - t   
    ```
    CLIENT: 
      MODE #aaa +t

      MODE #aaa - t

    SERVER:
      **All received**
      :nick_ken!~user_ken@{MASKED_IP}.IP MODE #aaa +t

      **All received**
      :nick_ken!~user_ken@{MASKED_IP}.IP MODE #aaa -t
    ```   

- 서버가 모드 변경을 처리한 후, 변경된 모드 정보를 포함하는 MODE 메시지를 채널의 모든 사용자에게 전송해야 한다   
- 서버는 모드 변경 시 민감한 정보를 숨길 수도 있다   

#### Numerics

**ERR_NOSUCHCHANNEL (403) 네트워크에 존재하지 않는 채널**   
```
CLIENT: 
  MODE #noexistchannel

SERVER:
  :{SERVER_NAME} 403 nick_ken #noexistchannel :No such channel
```   


**RPL_CHANNELMODEIS (324) modestring이 제공되지 않았을 때**   
```
CLIENT: 
  MODE #aaa

SERVER:
  :{SERVER_NAME} 324 nick_ken #aaa +cmn
  :{SERVER_NAME} 329 nick_ken #aaa {TIME_STAMP}
```   


**ERR_CHANOPRIVSNEEDED (482) 사용자가 적절한 권한 없이 채널 모드를 변경하려 할 때**   
```
CLIENT: 
  MODE #aaa +i

SERVER:
  :{SERVER_NAME} 482 nick_ken3 #aaa :You're not channel operator
```   


## SENDING MESSAGES

### PRIVMSG

**사용자 간 개인 메시지를 전송하거나, 채널에 메시지를 보내는 명령어**   
#### FORMAT
- FORMAT   
```bnf
     Command: PRIVMSG
  Parameters: <target>{,<target>} <text to be sent>
```

- Message Transaction   
```bash
CLIENT:
  PRIVMSG #{CHANNEL_NAME} :hello

  PRIVMSG nick_ken3 :Hello!
                                  ; nick_ken3에게 "yes I'm receiving it!" 메시지를 전송

SERVER:
  **All received**
  :nick_ken!~user_ken@{MASKED_IP}.IP PRIVMSG #{CHANNEL_NAME} :hello

  :nick_ken!~user_ken@{MASKED_IP}.IP PRIVMSG nick_ken3 :Hello!

  :Angel PRIVMSG Wiz :Hello are you receiving this message ?
                                  ; Angel이 Wiz에게 "Hello are you receiving this message?" 메시지를 전송

  :dan!~h@localhost PRIVMSG #coolpeople :Hi everyone!
                                  ; dan이 #coolpeople 채널에 "Hi everyone!" 메시지를 전송
```

- `target`은 메시지를 받을 사용자의 닉네임 또는 채널 이름이다   
- +m(모더레이션) 모드가 활성화된 채널에서는 특정 사용자의 메시지가 차단될 수 있다   
- 기타 **다른 채널 모드(+n, +t, +z 등)**가 메시지 전송에 영향을 미칠 수 있으며, 서버 소프트웨어 및 설정에 따라 다르다   
- 메시지가 채널로 전송될 수 없는 경우, 서버는 ERR_CANNOTSENDTOCHAN (404) 오류 응답을 반환하여 메시지가 전달되지 않았음을 사용자에게 알려야 한다   


#### NUMERICS

**ERR_NOSUCHNICK (401) 존재하지 않는 사용자에게 메시지를 보내려 할 때**   
```
Client:
  PRIVMSG noexistuser :hello

Server:
  :{SERVER_NAME} 401 nick_ken noexistuser :No such nick/channel
```     

**ERR_CANNOTSENDTOCHAN (404) 채널로 메시지를 보낼 수 없을 때 (예: 사용자가 차단되었거나, 채널이 +m 모드일 때)**   
```
Client:
  PRIVMSG # :hello
  PRIVMSG #{CHANNEL_NAME} :hello (channel is now +m mode)

Server:
  :{SERVER_NAME} 404 nick_ken # :Cannot send to channel
```     

**ERR_NORECIPIENT (411) 메시지의 대상이 지정되지 않았을 때**   
```
Client:
  "PRIVMSG   "

Server:
  :{SERVER_NAME} 411 nick_ken :No recipient given (PRIVMSG)
```     

  rizon 상용 서버에서는 `PRIVMSG   :hello` 를 전송시 ERR_NOTEXTTOSEND(412) 에러를 반환함 이는 구현에 따라 다른 것으로 보임   

**ERR_NOTEXTTOSEND (412) 보낼 메시지 내용이 없는 경우**   
```
Client:
  PRIVMSG #{CHANNEL_NAME} :

Server:
  :{SERVER_NAME} 412 nick_ken :No text to send
```     

