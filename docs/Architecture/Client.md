# Description
IRC 클라이언트와 관련된 데이터를 관리하는 객체.

client라 하면, 서버와 연결된 노드 중에서 서버를 제외한 모든 대상을 뜻한다. 네트워크에서 말단(leaf)를 이루며, 실질적인 통신의 대상이다. 서버는 이들 클라이언트의 요청을 수행하고, 클라이언트의 메시지를 대신 전달하는 매개자에 불과하다.

본래의 IRC 클라이언트에서는 두 개 이상의 서버를 경유하는 통신도 가능하지만, 현재 구현에서는 한 개의 서버를 경유하는 통신만 가능하다.

# Lifetime & Ownership
heap allocation된 후, name(string)을 key로 하는 std::map인  client_manager에서 pointer로 관리됨.

``` C++
std::map<std::string, Client *> client_manager;
```

connection이 해제될 때(quit 혹은 비정상 종료) 명시적으로 제거됨.

# Inheritance
- NON

# Traits
- unique(non-copyable)
- no-default
# Attribute

## Member Variables

### 1. nick_name_ - private, mutable
채널 내에서 사용자를 식별하기 위한 이름(string). 채널 내에서 고유해야 함.

registration 과정에서 NICK command로 획득.  NICK 커맨드에 의해서 변경 가능함.
필요에 따라서 NICK으로 언제든지 변경 가능함.

nick name에는 conventioin이 존재하므로, 초기화 하기 전에 check 필요함.
### 2. user_name_ - private, const
nick name이 채널에서만 유용한, 단순 식별을 위한 이름이라면, user_name은 서버 내부적으로 관리하는 클라이언트 관리를 위한 이름임.  nickname은 채널 내 식별을 위한 이름이라 고유하지만 쉽게 바꿀 수 있으므로 서버는 user name으로 사용자를 구분해야 함.

user name을 획득하는 방법에는 두 가지가 있는데, 하나는 ident protocol로 획득하는 것이고, 다른 하나는 USER 커맨드로 client에서 획득하는 것이다. ident protocol로 획득하는 경우, 이는 잘 인증된 이름이다. 다만, 클라이언트가 실행되는 host 쪽에서 ident server가 준비되어 있어야 인증이 가능 하므로, 획득하지 못할 수 있다.

> ident 서버로 인증된 user name이 우선이며, 단순 USER 커맨드로 획득한 경우, 앞에 '~'를 붙여서 구분한다.

한 번 연결을 확립하면 변경할 수 없음.
### 3. real_name_ - private, const
user name과 마찬가지로, registration 과정 중 USER 커맨드로 획득하는 이름이다.

**채널 내 사용자 사이에서 정보를 공유하기 위한 목적**으로 유지하는 데이터이다.
### 4. socket_ - private, nullable
네트워크 정보를 포함하는 TcpSocket 객체.

**host name에 해당하는 필드임**

### 5. mod_ - private
사용자가 갖는 권한(오퍼레이터 등)을 표현하는 값. 모드는 각각 독자적인 의미를 갖는 플래그의 조합으로 구성되며, 이는 bit-masking으로 구현됨.

### 6. Server connected from(Not Necessary)
IRC 네트워크에서 클라이언트는 해당 서버와 직접 연결되지 않을 수 있다. 이 경우 해당 클라이언트는 경유하는 서버를 갖는데, 이 경유하는 서버의 정보를 알고 있어야 한다.

> 이는 과제의 요구 사항이 아니므로, 생략 가능하다.

### 7. read_buffer_ - private
소켓에서 읽어온(read / recv) 데이터를 임시로 저장하는 string 변수.

non-blocking IO의 특성 상, 한 번의 system-call로 IO 가능한 길이가 가변적이라 남는 데이터를 저장할 buffer가 필요함.
### 8. write_buffer_ - private
해당 소켓에 쓸(write / send) 데이터를 임시로 저장하는 string 변수.

non-blocking IO의 특성 상, 한 번의 system-call로 IO 가능한 길이가 가변적이라 남는 데이터를 저장할 buffer가 필요함.
## Member Functions

### 1. Constructor
프로세스 내에서 유일하게 관리되어야 하는 자원이므로, **복사 불가능**하다. 또한 connection이 없으면 클라이언트는 생성되지 않으므로, default 생성도 불가능하다.

- construct with listening fd : client를 향하는 소켓을 accept하기 위한 server bind된 소켓의 fd
### 2. Destructor

### 3. getter

### 4. GetReceivedMessages - public
서버와 연결된 클라이언트에서 text 데이터를 받아서 message를 구하는 함수.

클라이언트를 향하는 소켓이 non-blocking 소켓인 관계로, 별도의 처리가 필요함.

service routine의 read event가 trigger된 경우 호출.

실행 과정은 다음과 같음
1. TcpSockt의 fd에서 receive_buffer로 read(recv)
2. 기존의 read_buffer_에 recv_buffer의 내용을 append
3. read_buffer_에서 CR-LF로 끝나는, message를 vector에 저장
4. CR-LF로 구분되지 않는, 불완전한 메시지를 read_buffer_에 저장
5. message를 저장한 vector를 optional에 담아서 return

read size를 512byte로 설정. 이는 프로토콜에서 규정하는 메시지 최대 크기로, 이 보다 큰 메시지는 규약 위반임.

#### optional의 사용
- 불충분한 메시지 : **left buffer에 데이터를 저장하고, no-ops를 return함**
- 1개 이상의 메시지 : **온전한 메시지 들은 optional vector string에 담아서 return**

### 5. SetSendMessage - public
client를 향해서 보내야 할 텍스트 데이터를 write_buffer_에 저장한다.

이 때, 메시지의 구분을 위해서 CR-LF를 추가한다.
### 6. SendMessage - public
client를 향해 텍스트 데이터를 보내고, 남은 데이터를 저장한다.

동작 방식은 다음과 같다.
1. write_buffer_의 데이터를 TcpSocket객체의 fd를 향해서 send한다.
2. send의 return 값을 바탕으로, 남은 데이터를 write_buffer_에 저장한다.

service routine의 write event가 trigger된 경우 호출한다.

return 값으로 write_buffer_의 데이터 유무를 bool값으로 반환하는데, 컨텐츠가 있는 경우, 해당 소켓에 대한 write event를 다시 등록한다. 이러한 이벤트 관리 방식을 **level-trigger**하다고 표현한다.
