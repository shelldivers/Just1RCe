---
Date: 2024-11-29
---

# Description
유닉스 소켓 프로그래밍을 위해서 추상화 한 TCP 소켓 객체

# Attributes
## Member Variable
### 1. socket_fd_ - private
소켓의 file descriptor. systemcall에 전달되는 핸들. RAII.

### 2. is_listen_only - private
소켓의 listening 여부를 위한 필드. RAII.

### 3. sockaddr_in
해당 소켓의 바인드 정보를 저장하는 구조체.
프로토콜 정보, 포트 번호, IP 주소를 저장함.

두 가지 주의 점이 존재한다.

- 1. IPv4 mapped IPv6 IP

	>  The address notation for IPv6 is a group of 8 4-digit hexadecimal
       numbers, separated with a ':'.  "::" stands for a string of 0
       bits.  Special addresses are ::1 for loopback and ::FFFF:<IPv4 address> for IPv4-mapped-on-IPv6.
       The port space of IPv6 is shared with IPv4.

- 2. IPv6 only option

	setsocketopt를 호출하여 해당 옵션을 꺼줘야 한다.
	> f this flag is set to true (nonzero), then the socket is restricted to sending and receiving IPv6 packets only.  In this case, an IPv4 and an IPv6 application can bind to a single port at the same time. If this flag is set to false (zero), then the socket can be used to send and receive packets to and from an IPv6 address or an IPv4-mapped IPv6 address. The default value for that file is 0 (false).

- 듀얼스택 소켓? : IPv4와 IPv6, 둘 모두를 지원하는 소켓을 준비해야 하는가?

	-> 이론 참고 : https://stackoverflow.com/questions/1618240/how-to-support-both-ipv4-and-ipv6-connections

	-> 예제 : https://www.joinc.co.kr/w/Site/TCP_IP/IPv6/IPv6Prog

	-> 인용 : https://man7.org/linux/man-pages/man7/ipv6.7.html

## Member Function

