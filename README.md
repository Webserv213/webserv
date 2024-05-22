# Webserv
C++98 버전과 Unix Kqueue를 사용한 Web Server.

## Requirements
- **MacOS(Unix)** system
- **clang++(Recommanded)** or **g++**
- **make**

## Build and Run
### MacOS(Unix)
```bash
$ make
$ ./webserv <config>

## Code Convention
*google c++ 참고 https://google.github.io/styleguide/cppguide.html#Variable_Names
- 기본적으로 Google의 code convention을 따름. 추가로 ecole42의 orthdox rule도 적용
- class file,hpp file naming은 upper camel case로 한다.
- variable naming은 snake case로 한다.
- function naming은 lower camel case로 한다.
- 하나의 파일엔 하나의 클래스만 작성한다.
- 클래스멤버변수 : 변수끝에 underscore 붙인다.
- test
