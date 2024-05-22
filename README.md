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
```

## Code convention
- 기본적으로 Google의 Code convention을 따름. <https://google.github.io/styleguide/cppguide.html#Variable_Names> 
- 소스 파일명은 upper camel case로 한다.
- 변수명은 snake case로 한다.
- 함수명은 lower camel case로 한다.
- 하나의 소스파일엔 하나의 클래스만 작성한다.
- 클래스의 멤버 변수명 끝에 underscore(_) 붙인다.
