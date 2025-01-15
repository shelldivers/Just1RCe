## 접속

```
06:32 -!-  ___           _
06:32 -!- |_ _|_ _ _____(_)
06:32 -!-  | || '_(_-<_-< |
06:32 -!- |___|_| /__/__/_|
06:32 -!- Irssi v1.4.5 - https://irssi.org
06:32 -!- Irssi: The following settings were 
          initialized
06:32                        real_name Unknown
06:32                        user_name codespace
06:32                             nick codespace
```

처음 irssi에 접속하게되면 위와 같이 real_name(hostname), user_name(username), nick(nickname)이 정해져서 알려준다    

아래와 같은 명령어로 각 이름들을 설정해 줄 수 있다   
```
/set {target} {원하는 이름}
```

## 서버에 연결
```
/connect -nocap irc.rizon.net 6697
```

서버에 연결하면 특정형식과 각 서버에 따른 메시지들을 받게 된다   

<details>
  <summary>**server message received by irssi:** </summary>
```bash
07:04 [server]
07:04                             nick ken2222222
07:04 -!- Irssi: Looking up irc.rizon.net
07:04 -!- Irssi: Connecting to irc.rizon.net [52.193.79.145] port 6697
07:04 -!- Irssi: Certificate Chain:
07:04 -!- Irssi:   Subject: CN: irc.rizon.net
07:04 -!- Irssi:   Issuer:  C: US, O: Let's Encrypt, CN: R10
07:04 -!- Irssi:   Subject: C: US, O: Let's Encrypt, CN: R10
07:04 -!- Irssi:   Issuer:  C: US, O: Internet Security Research Group, CN: ISRG Root X1
07:04 -!- Irssi: Protocol: TLSv1.3 (256 bit, TLS_AES_256_GCM_SHA384)
07:04 -!- Irssi: EDH Key: 253 bit Unknown
07:04 -!- Irssi: Public Key: 4096 bit RSA, valid from Dec  9 21:06:22 2024 GMT to Mar  9 21:06:21 2025 GMT
07:04 -!- Irssi: Public Key Fingerprint:  4E:56:85:B1:B0:93:DC:06:EA:7A:6A:25:02:36:5F:2E:E7:06:1D:9B:67:B6:F0:10:CD:6D:C5:99:81:7F:17:25 (SHA256)
07:04 -!- Irssi: Certificate Fingerprint: B6:B7:F0:1F:6F:EC:4D:2F:66:E2:F3:DB:05:EC:E1:63:72:83:2D:94:24:15:E3:9E:BE:07:BB:A9:4C:2D:E6:54 (SHA256)
07:04 -!- Irssi: Connection to irc.rizon.net established
07:04 !irc.losslessone.com *** Couldn't look up your hostname (cached)
07:04 !irc.losslessone.com *** Checking Ident
07:04 !irc.losslessone.com *** No Ident response
07:04 !irc.losslessone.com *** Connected securely via TLSv1.3 TLS_AES_256_GCM_SHA384-256
07:04 !irc.losslessone.com *** Your host is masked (8F88ED3F.D981EDD6.6EA05CE5.IP)
07:04 -!- Welcome to the Rizon Internet Relay Chat Network ken2222222 
07:04 -!- Your host is irc.losslessone.com, running version plexus-4(hybrid-8.1.20) 
07:04 -!- This server was created Mar 19 2022 at 21:25:18 
07:04 -!- irc.losslessone.com plexus-4(hybrid-8.1.20) CDGNRSUWagilopqrswxyz BCIMNORSabcehiklmnopqstvz Iabehkloqv 
07:04 -!- CALLERID CASEMAPPING=rfc1459 DEAF=D KICKLEN=180 MODES=4 PREFIX=(qaohv)~&@%+ STATUSMSG=~&@%+ EXCEPTS=e INVEX=I NICKLEN=30 NETWORK=Rizon MAXLIST=beI:250 MAXTARGETS=4 are supported by this server
07:04 -!- CHANTYPES=# CHANLIMIT=#:250 CHANNELLEN=50 TOPICLEN=390 CHANMODES=beI,k,l,BCMNORScimnpstz NAMESX UHNAMES KNOCK WATCH=60 AWAYLEN=180 ELIST=CMNTU SAFELIST are supported by this server
07:04 -!- There are 18 users and 9323 invisible on 18 servers
07:04 -!- 45 IRC Operators online
07:04 -!- 3 unknown connection(s)
07:04 -!- 7770 channels formed
07:04 -!- I have 9341 clients and 18 servers
07:04 -!- Current local users: 9341  Max: 10299
07:04 -!- Current global users: 9341  Max: 10299
07:04 -!- - irc.losslessone.com Message of the Day - 
07:04 -!- -          oo                            
07:04 -!- - 
07:04 -!- - 88d888b. dP d888888b .d8888b. 88d888b. 
07:04 -!- - 88'  `88 88    .d8P' 88'  `88 88'  `88 
07:04 -!- - 88       88  .Y8P    88.  .88 88    88 
07:04 -!- - dP       dP d888888P `88888P' dP    dP
07:04 -!- - 
07:04 -!- - Rizon Chat Network -- http://rizon.net
07:04 -!- - 
07:04 -!- - Listening on ports: 6660 - 6669, 7000. SSL: 6697, 9999
07:04 -!- - 
07:04 -!- - Rules:
07:04 -!- - o No spamming or flooding
07:04 -!- - o No clones or malicious bots
07:04 -!- - o No takeovers
07:04 -!- - o No distribution of child pornography
07:04 -!- - o Clients must respond to VERSION requests
07:04 -!- - o Rizon staff may disconnect clients for any or no reason
07:04 -!- - 
07:04 -!- - First steps:
07:04 -!- - o To register your nick: /msg NickServ HELP
07:04 -!- - o To register your channel: /msg ChanServ HELP
07:04 -!- - o To get a vHost: /msg HostServ HELP REQUEST
07:04 -!- - o For other help with Rizon: /join #help
07:04 -!- - 
07:04 -!- - Usage of this network is a privilege, not a right. Rizon is a
07:04 -!- - transit provider, therefore no person or entity involved with
07:04 -!- - *.rizon.net or irc.losslessone.com takes any responsibility for
07:04 -!- - users' actions. Absolutely no warranty is expressed or implied.
07:04 -!- End of /MOTD command.
07:04 -!- Mode change [+Six] for user ken2222222
07:04 py-ctcp [ctcp@ctcp-scanner.rizon.net] requested CTCP VERSION from ken2222222: 
07:04 -Global(service@rizon.net)- [Logon News - Dec 16 2013] Own a large/active channel or plan to get one going? Please read http://s.rizon.net/authline
07:04 -Global(service@rizon.net)- [Logon News - Feb 16 2022] First time on Rizon? Be sure to check out our wiki! https://wiki.rizon.net
07:04 -Global(service@rizon.net)- [Random News - May 07 2013] Looking for a quotes, trivia, or other extra bots? /msg funserv help
```
</details>
   
<details>
  <summary>**server command format:** </summary>
```
:irc.losslessone.com NOTICE * :*** Looking up your hostname...
:irc.losslessone.com NOTICE * :*** Checking Ident
:irc.losslessone.com NOTICE * :*** Couldn't look up your hostname
:irc.losslessone.com NOTICE * :*** No Ident response
PING :1536239678
[DEBUG] Responding to PING with: PONG :1536239678

:irc.losslessone.com NOTICE ken2222222 :*** Connected securely via TLSv1.3 TLS_AES_256_GCM_SHA384-256
:irc.losslessone.com NOTICE ken2222222 :*** Your host is masked (8F88ED3F.D981EDD6.6EA05CE5.IP)
:irc.losslessone.com 001 ken2222222 :Welcome to the Rizon Internet Relay Chat Network ken2222222
:irc.losslessone.com 002 ken2222222 :Your host is irc.losslessone.com, running version plexus-4(hybrid-8.1.20)
:irc.losslessone.com 003 ken2222222 :This server was created Mar 19 2022 at 21:25:18
:irc.losslessone.com 004 ken2222222 irc.losslessone.com plexus-4(hybrid-8.1.20) CDGNRSUWagilopqrswxyz BCIMNORSabcehiklmnopqstvz Iabehkloqv
:irc.losslessone.com 005 ken2222222 CALLERID CASEMAPPING=rfc1459 DEAF=D KICKLEN=180 MODES=4 PREFIX=(qaohv)~&@%+ STATUSMSG=~&@%+ EXCEPTS=e INVEX=I NICKLEN=30 NETWORK=Rizon MAXLIST=beI:250 MAXTARGETS=4 :are supported by this server
:irc.losslessone.com 005 ken2222222 CHANTYPES=# CHANLIMIT=#:250 CHANNELLEN=50 TOPICLEN=390 CHANMODES=beI,k,l,BCMNORScimnpstz NAMESX UHNAMES KNOCK WATCH=60 AWAYLEN=180 ELIST=CMNTU SAFELIST :are supported by this server
:irc.losslessone.com 251 ken2222222 :There are 18 users and 9283 invisible on 18 servers
:irc.losslessone.com 252 ken2222222 45 :IRC Operators online
:irc.losslessone.com 253 ken2222222 2 :unknown connection(s)
:irc.losslessone.com 254 ken2222222 7771 :channels formed
:irc.losslessone.com 255 ken2222222 :I have 9301 clients and 18 servers
:irc.losslessone.com 265 ken2222222 :Current local users: 9301  Max: 10299
:irc.losslessone.com 266 ken2222222 :Current global users: 9301  Max: 10299
:irc.losslessone.com 375 ken2222222 :- irc.losslessone.com Message of the Day - 
:irc.losslessone.com 372 ken2222222 :-          11oo                            
:irc.losslessone.com 372 ken2222222 :- 
:irc.losslessone.com 372 ken2222222 :- 88d888b. dP d888888b .d8888b. 88d888b. 
:irc.losslessone.com 372 ken2222222 :- 88'  `88 88    .d8P' 88'  `88 88'  `88 
:irc.losslessone.com 372 ken2222222 :- 1188       88  .Y8P    88.  .88 88    88 
:irc.losslessone.com 372 ken2222222 :- 11dP       dP d888888P `88888P' dP    dP
:irc.losslessone.com 372 ken2222222 :- 
:irc.losslessone.com 372 ken2222222 :- Rizon Chat Network -- http://rizon.net
:irc.losslessone.com 372 ken2222222 :- 
:irc.losslessone.com 372 ken2222222 :- Listening on ports: 6660 - 6669, 7000. SSL: 6697, 9999
:irc.losslessone.com 372 ken2222222 :- 
:irc.losslessone.com 372 ken2222222 :- Rules:
:irc.losslessone.com 372 ken2222222 :- o No spamming or flooding
:irc.losslessone.com 372 ken2222222 :- o No clones or malicious bots
:irc.losslessone.com 372 ken2222222 :- o No takeovers
:irc.losslessone.com 372 ken2222222 :- o No distribution of child pornography
:irc.losslessone.com 372 ken2222222 :- o Clients must respond to VERSION requests
:irc.losslessone.com 372 ken2222222 :- o Rizon staff may disconnect clients for any or no reason
:irc.losslessone.com 372 ken2222222 :- 
:irc.losslessone.com 372 ken2222222 :- First steps:
:irc.losslessone.com 372 ken2222222 :- o To register your nick: /msg NickServ HELP
:irc.losslessone.com 372 ken2222222 :- o To register your channel: /msg ChanServ HELP
:irc.losslessone.com 372 ken2222222 :- o To get a vHost: /msg HostServ HELP REQUEST
:irc.losslessone.com 372 ken2222222 :- o For other help with Rizon: /join #help
:irc.losslessone.com 372 ken2222222 :- 
:irc.losslessone.com 372 ken2222222 :- Usage of this network is a privilege, not a right. Rizon is a
:irc.losslessone.com 372 ken2222222 :- transit provider, therefore no person or entity involved with
:irc.losslessone.com 372 ken2222222 :- *.rizon.net or irc.losslessone.com takes any responsibility for
:irc.losslessone.com 372 ken2222222 :- users' actions. Absolutely no warranty is expressed or implied.
:irc.losslessone.com 376 ken2222222 :End of /MOTD command.
:ken2222222!~ken222222@8F88ED3F.D981EDD6.6EA05CE5.IP MODE ken2222222 :+Six
:py-ctcp!ctcp@ctcp-scanner.rizon.net PRIVMSG ken2222222 :VERSION
:Global!service@rizon.net NOTICE ken2222222 :[Logon News - Dec 16 2013] Own a large/active channel or plan to get one going? Please read http://s.rizon.net/authline
:Global!service@rizon.net NOTICE ken2222222 :[Logon News - Feb 16 2022] First time on Rizon? Be sure to check out our wiki! https://wiki.rizon.net
:Global!service@rizon.net NOTICE ken2222222 :[Random News - Mar 20 2009] Idle on Rizon a lot? Why not play our idlerpg game, you can check it out at #RizonIRPG for more information visit the website http://idlerpg.rizon.net -Rizon Staff
```
</details>
서버가 보내는 명령어들의 형식은 `:<hostname> <numeric> <nickname> :<letters>` 으로 이루어졌음을 볼 수 있다

