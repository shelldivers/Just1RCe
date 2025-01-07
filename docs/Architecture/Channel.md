
# Description
서버가 관리해야 하는 채널과 관련된 데이터를 표현하는 class.
# Lifetime & Ownership
heap allocation되며, channel_manager가 pointer의 형태로 관리함
``` C++
std::map<std::string, Channel *> channel_manager;
```
# Inheritance
NONE

# Traits
- no default construction
- unique(non copyable)
- no implicit conversion
# Attribute

## Member Variables

### name_ - const
채널의 이름. 생성과 동시에 결정되는 string 값.

채널 이름에는 접두어가 존재해서, 해당 접두어에 따라 채널의 종류가 다르고, 그 동작도 일부 다르다.
서버 내부적으로 채널 이름이 구분의 기준이 됨
### topic_
채널의 주제를 나타내는 값. string
### key_
채널의 비밀번호. 채널에 secure mod(+k)가 설정되어있는 경우, 해당 값을 기준으로 접근을 통제함.
### mod_
채널의 권한 상태를 표현하는 unsigned long 값. 이러한 권한은 여러 종류가 존재하고, 이들을 종합하여 채널의 모드(mode)라고 한다. 각각의 권한은 정해진 위치의 bit값으로 구성되며, 이를 종합한 bit mask가 곧 mod_다.
### max_user_num_
해당 채널이 유지 가능한 최대 사용자 수를 의미하는 값.

채널 모드 +l이 설정되었고, user_nicknames_의 크기가 max_user_num_ 이상이라면 AddUser는 바로 return하게 됨.
### user_nicknames_
현재 채널에 소속된 사용자의 이름(nick name, std::string)을 저장하는 셋(std::set<std::string>).

현재 해당 채널에 소속된 사용자의 수는 user_nicknames_.size()를 통해서 획득.
## Member Functions
**setter를 비롯한 일부 member function들은 호출하기 전에 채널의 권한과 그 대상이 되는 client의 권한을 비교 및 확인을 할 필요가 있음.**
### constructor

### destructor

### getter

### setter

### GetMode
현재 해당 채널에 설정된 mod, 권한의 bit mask를 반환함.
### GetModeAsString
현재 해당 채널에 설정된 mod, 권한을 문자열의 형태로 format해서 반환함.

현재 해당 채널에 설정된 mod에 대해서 물어보는 커맨드를 위한 메서드임.
### SetMode
인자로 전달된 mask에 세팅된 모드를 채널에 세팅함
### UnsetMode
인자로 전달된 mask에 세팅된 모드를 채널에서 제거함
### CheckMode
인자로 전달된 mask에 세팅된 모드와 현재 채널에 세팅된 모드를 비교하여 공통으로 설정된 mod가 있는지 확인하고, 이를 bool값으로 반환함.

인자의 모드가 일부만 일치해도 true를 반환함.
### AddUser
채널에 사용자를 추가함.

만약 max_user_num_가 user_nicknames_의 size이상이라면 동작은 무시됨.
### DeleteUser
채널에 사용자를 제거함.

해당 채널의 사용자가 아니라면 무시됨.
### CheckUser
채널에 인자로 전달된 이름이 소속되어있는지 확인하여 있다면 true를 반환함.
