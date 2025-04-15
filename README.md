# HTTP/1.1 웹서버

- 이 프로젝트는 HTTP/1.1 웹서버입니다.
- C++98 버전을 사용하여 구현되었습니다.
- 이 서버는 간단한 HTTP 요청을 처리하고 정적 파일을 제공하는 기능을 갖추고 있습니다.

## 프로젝트 개요

이 프로젝트는 HTTP/1.1 웹 서버의 기본 원리를 이해하고, C++ 프로그래밍 능력을 향상시키기 위해 진행되었습니다. 주요 목표는 다음과 같습니다:

- HTTP 프로토콜의 이해
- 소켓 프로그래밍 기초 학습 (Using Unix Kqueue)
- C++ 언어의 기본 문법 및 표준 라이브러리 활용

## 주요 기능

- **HTTP/1.1 프로토콜 지원**: GET, POST, DELETE 메소드 처리
- **비동기 이벤트 기반 I/O**: Unix Kqueue를 활용한 효율적인 다중 클라이언트 처리
- **멀티서버 및 가상 호스팅**: 하나의 프로세스에서 여러 서버 동시 운영
- **설정 기반 라우팅**: Nginx 스타일의 유연한 설정 파일 지원
- **자동 디렉토리 인덱싱**: 디렉토리 내용을 HTML로 자동 생성
- **CGI 지원**: 동적 콘텐츠 생성을 위한 CGI 프로토콜 구현
- **쿠키 및 세션 관리**: 상태 유지를 위한 쿠키 기반 세션 구현
- **청크 인코딩 처리**: Transfer-Encoding: chunked 지원
- **다양한 MIME 타입**: 80개 이상의 파일 타입 지원
- **오류 응답 처리**: 403, 404, 405, 413

## 개발 환경

- 컴파일러: Apple clang version 15.0.0 (clang-1500.3.9.4)
- 운영체제: MacOS(Unix) system
- 기타 도구: Make
- 호환 브라우저: Google Chrome, Apple Safari

## 설치 및 실행 방법

1. 저장소를 클론합니다.

    ```sh
    git clone https://github.com/Webserv213/webserv.git
    cd webserv
    ```

2. Makefile을 사용하여 빌드합니다.

    ```sh
    make
    ```

3. 서버를 실행합니다. (config file 인수가 없을 시 ./conf/default.conf 파일을 기본으로 불러옵니다.) 

    ```sh
    ./webserv <config file>
    ```

5. 웹 브라우저를 열고 `http://localhost:8082`에 접속합니다. (Port 번호는 config file에 따라 달라질 수 있습니다.)

## 설정 파일 예시

- 다음은 이 웹서버를 설정할 때 사용한 기본 config file의 예시입니다.
- 이 파일은 nginx의 설정 파일을 참고하여 작성되었습니다.

    ```sh
    http {
      server {
        listen        8082;
        server_name   localhost;
        root          ./YoupiBanane;
        index         index.html;

        location / {
          index index.html;
          access_method GET;
        }

        location .bla {
          access_method POST;
          root ./YoupiBanane;
          cgi_path ./cgi/cgi_bla;
        }

        location /post_body {
          root ./YoupiBanane/post_body;
          access_method POST;
          client_max_body_size 100;
        }

        location /directory  {
          root ./YoupiBanane;
          access_method GET DELETE;
          index youpi.bad_extension;
        }

        location /autoindex {
          root ./YoupiBanane;
          access_method GET;
          autoindex on;
        }

        location .clk {
          root .;
          access_method POST;
          cgi_path ./cgi/cgi_upload_clk;
        }

        location .session {
          root ./var/www/session;
          access_method GET POST;
          cgi_path ./cgi/cgi_session;
          index displayName.html;
        }
      }

      server {
        listen        8083;
        server_name   localhost;
        root          ./YoupiBanane;
        index         index.html;

        location / {
          index   index.html;
          access_method GET;
        }
      }
    }
    ```

## 주요 학습 내용

- HTTP/1.1 프로토콜의 기본 동작 방식 이해
- Kqueue를 사용한 소켓 프로그래밍
- HTTP 요청/응답 처리 로직 구현
- CGI 요청 처리
- 간단한 파일 입출력 및 예외 처리

## Diagram
![webserv drawio](https://github.com/Webserv213/webserv/assets/106380619/b68c305c-3a8c-4747-bd59-0529a3bb8591)

- https://drive.google.com/file/d/11UmG6pbXM9xsuBHdFH4aXS_DtuiRP56v/view?usp=sharing

## 클래스 설명

### HTTP
- 설정 파일 호출 및 Http 블록을 파싱하고 Server 클래스를 벡터 컨테이너로 관리하는 클래스

### Server
- Server 블록의 정보를 파싱하고 Location 클래스를 벡터 컨테이너로 관리하는 클래스

### Location
- Server 블록 안에 있는 Location 블록의 정보를 파싱하고 관리하는 클래스

### Request
- HTTP Request의 Start Line, Headers, Body 정보를 가지고 있는 클래스들을 관리하는 클래스

### RequestLine
- HTTP Request의 Start Line 정보를 관리하는 클래스 (요청 메소드, 경로, HTTP 버전)

### ReqHeaders
- HTTP Request의 Headers 정보를 관리하는 클래스 (Content-Length, Transfer-Encoding 등)

### Response
- HTTP Response의 Status Line, Headers, Body 정보를 가지고 있는 클래스들을 관리하는 클래스

### ResponseLine
- HTTP Response의 Status Line 정보를 관리하는 클래스 (HTTP 버전, 상태코드, 상태 텍스트)

### ResHeaders
- HTTP Response의 Headers 정보를 관리하는 클래스 (Date, Content-Type 등)

### EventRecorder
- Socket의 여러 상태값을 관리하기 위한 클래스 (flag, count 등)

### KeventHandler
- 맵 컨테이너를 활용해 각 Socket 별로 EventRecorder 클래스를 관리하고 Event에 대한 전체적인 처리를 담당하는 클래스

## 참고 자료

- [HTTP/1.1: Protocol Specification (RFC2616)](https://tools.ietf.org/html/rfc2616)
- [The Common Gateway Interface (CGI) Version 1.1 (RFC3875)](https://datatracker.ietf.org/doc/html/rfc3875)

## 프로젝트 멤버

- 이수하, 최서지, 김우성

## Code convention
- 기본적으로 Google의 Code convention을 따른다. <https://google.github.io/styleguide/cppguide.html#Variable_Names> 
- 소스 파일명은 upper camel case로 한다.
- 변수명은 snake case로 한다.
- 함수명은 lower camel case로 한다.
- 하나의 소스파일엔 하나의 클래스만 작성한다.
- 클래스의 멤버 변수명 끝에 underscore(_) 붙인다.
