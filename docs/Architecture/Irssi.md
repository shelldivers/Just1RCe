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
/connect -nocap localhost 6667
```

nocap 옵션을 넣어서 cap 명령을 통한 협상을 진행하지 않도록 한다   


```
NICK ken
MODE user +i
WHOIS ken
WHOIS ken
PING server_name
```