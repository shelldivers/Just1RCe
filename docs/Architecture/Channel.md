
# Description
서버가 관리해야 하는 채널과 관련된 데이터를 표현하는 class.
# Lifetime & Ownership
heap allocation되며,  DBContext에서 pointer의 형태로 관리함

현재 해당 채널에 소속된 사용자 관련 정보 또한 DBContext에서 관리될 예정임.
# Traits
- no default construction
- unique(non copyable)
- no implicit conversion
# Attribute

## Member Variables

### name_ - const
채널의 이름. 생성과 동시에 결정되는 string 값.

채널 이름에는 접두어가 존재해서, 해당 접두어에 따라 채널의 종류가 다르고, 그 동작도 일부 다르다. 채널의 생성은 첫 참가자에 의해서 암시적으로 생성되며, 따라서 채널의 이름은 사용자가 결정함.

채널 이름의 접두어와 그 의미는 다음과 같음.

| 종류               | prefix | 설명                              |
| ---------------- | ------ | ------------------------------- |
| global channel   | #      | 여러 서버에 걸쳐서 존재하는 채널입니다.          |
| local channel    | &      | 한 서버에서만 존재하는 채널입니다.             |
| modeless channel | +      | 별도의 권한이 존재하지 않는, 일종의 임시 채널입니다.  |
| safe channel     | !      | 이름 뒤에 식별자를 추가하여 충돌을 방지하는 채널입니다. |
다만, 현재 과제(ecole42)의 구현 요구사항 상, local channel 외의 부분은 유효하지 않을 예정.

서버 내부적으로 채널 이름이 구분의 기준이 됨.
### topic_
채널의 주제를 나타내는 값. string
### key_
채널의 비밀번호. 채널에 secure mod(+k)가 설정되어있는 경우, 해당 값을 기준으로 접근을 통제함.

당장은 암호화 수행하지 않을 예정.
### mod_
채널의 권한 상태를 표현하는 unsigned long 값. 이러한 권한은 여러 종류가 존재하고, 이들을 종합하여 채널의 모드(mode)라고 한다. 각각의 권한은 정해진 위치의 bit값으로 구성되며, 이를 종합한 bit mask가 곧 mod_다.

| 채널 모드           | 기호  | 설명                                |
| --------------- | --- | --------------------------------- |
| **초대 전용 채널**    | i   | 초대받은 사용자만 채널에 참여                  |
| **조용한 채널**      | m   | 채널 운영자 및 권한이 있는 사용자만 메시지를 보낼 수 있음 |
| **외부 메시지 차단**   | n   | 채널에 참여하지 않은 사용자가 메시지를 보낼 수 없음     |
| **주제 제한**       | t   | 운영자 또는 권한 있는 사용자만 채널 주제를 변경할 수 있음 |
| **비밀번호 보호**     | k   | 채널에 참여하려면 비밀번호(`key`)가 필요         |
| **최대 사용자 수 제한** | l   | 채널에 참여할 수 있는 사용자의 최대 수를 제한        |

### max_user_num_
해당 채널이 유지 가능한 최대 사용자 수를 의미하는 값.

채널에 인원수 제한 모드(+l)이 설정되었고, 현재 채널에 소속된 사용자 수가 max_user_num_ 이상이라면 사용자 추가는 수행되지 않음.
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
