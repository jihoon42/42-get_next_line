# get_next_line

*This project has been created as part of the 42 curriculum by jkim2.* 

---

## 1. Description

The `get_next_line` project is a fundamental challenge in the 42 curriculum that involves creating a function to read a line from a file descriptor. The goal is to handle multiple calls to the function to read a text file one line at a time, until the end of the file (EOF) is reached. This project introduces the concept of **static variables** and efficient **buffer management** in C.

### Function Overview

**Prototype**: `char *get_next_line(int fd);`

**Parameters**: File descriptor to read from.

**Return Value**: The read line (including the newline character `\n` if present), or `NULL` if there is nothing else to read or an error occurred.

---

## 2. Instructions

### Compilation

The function is designed to be compiled with a specific `BUFFER_SIZE` using the `-D` flag:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c -o gnl

```

### Development Environment & Verification

**Working Environment**: Developed on macOS (Apple M1 Pro) and cross-verified in the 42 Cluster Ubuntu environment.

**Testing Tools**:
Verified with Tripouille's [gnlTester](https://github.com/Tripouille/gnlTester) to ensure 100% compliance with edge cases (empty files, standard input, large/small buffer sizes).
Manual verification using a custom test suite.


Memory Management:

**macOS**: Checked with `leaks -atExit -- ./test`.

**Ubuntu**: Checked with `valgrind --leak-check=full ./test`.



---

## 3. Algorithm and Data Structure (Justification)

### Dynamic Stash Management (`t_stash`)

Instead of simple static arrays or linked lists, this implementation uses a custom structure named `t_stash` to manage the data remaining between function calls.

**Justification**: The `t_stash` structure maintains the content, size, and current capacity of the buffer. By using `stash_reserve`, the buffer size is dynamically doubled when more space is needed, which optimizes memory allocation frequency and allows for handling lines of any length regardless of the `BUFFER_SIZE`.

### Static Variable for Persistence

A `static t_stash` is used to persist data across multiple calls to `get_next_line`.

**Justification**: The static variable ensures that data read into the buffer but not yet returned as part of a line is "stashed" and available for the next call. This prevents data loss when the `read()` call fetches more than one line at a time.

### Efficient Line Extraction

The `extract_line` and `stash_trim` functions work in tandem to return the current line and shift the remaining contents to the beginning of the stash.

**Justification**: This approach minimizes redundant memory moves and ensures that the stash remains compact and ready for subsequent reads.

---

## 4. Resources

### References & Documentation

**Test Suite**: [42-get_next_line-w-tests](https://github.com/jihoon42/get_next_line-w-tests) (My personal repository including custom test cases and scripts).

**Static Variables in C**: Deep dive into scope and lifetime management in C. [Wikipedia: Static variable](https://en.wikipedia.org/wiki/Static_variable)

**File Descriptors**: Study of `read(2)` and the behavior of FDs on different systems.  [Wikipedia: File descriptor](https://en.wikipedia.org/wiki/File_descriptor)


### AI Usage Disclosure

In accordance with the 42 AI policy, AI tools were utilized as a technical thought partner for:

**Structural Refinement**: Assisted in designing the logic for `stash_reserve` to ensure memory safety when reallocating the dynamic buffer.

**Edge-Case Brainstorming**: Used to identify potential issues with `BUFFER_SIZE=1`, file descriptors pointing to directories, or files that do not end with a newline.

**Documentation Refinement**: Assisted in structuring this README and ensuring the technical justifications are articulated clearly in English.

**Countermeasures**: All logic was manually verified and debugged. The final code passed all mandatory tests in the Tripouille tester without unauthorized external dependencies.

---


---

## 1. 개요 (Description)

`get_next_line` 프로젝트는 파일 디스크립터(file descriptor)로부터 한 줄을 읽어오는 함수를 구현하는 42 커리큘럼의 필수 과제입니다. 이 프로젝트의 목표는 함수를 여러 번 호출하여 텍스트 파일을 EOF(파일의 끝)에 도달할 때까지 한 번에 한 줄씩 읽어오는 것입니다. 이 과정에서 C 언어의 **정적 변수(static variables)** 개념과 효율적인 **버퍼 관리(buffer management)** 기법을 학습하게 됩니다.

### 함수 요약

**프로토타입**: `char *get_next_line(int fd);`

  **매개변수**: 읽어들일 파일 디스크립터(fd)
  
  **반환 값**: 읽어온 한 줄(줄 바꿈 문자 `\n`이 포함됨), 읽을 내용이 더 이상 없거나 에러가 발생한 경우 `NULL` 반환

---

## 2. 지침 (Instructions)

### 컴파일 (Compilation)

이 함수는 컴파일 시 `-D` 플래그를 사용하여 특정 `BUFFER_SIZE`를 지정하도록 설계되었습니다.

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c -o gnl

```

### 개발 환경 및 검증 (Development Environment & Verification)

**작업 환경**: macOS (Apple M1 Pro)에서 개발되었으며, 42 클러스터 Ubuntu 환경에서 교차 검증을 완료하였습니다.

**테스트 도구**:

txt 파일 및 main.c를 직접 작성하면서 수동 검증을 수행하였고 이를 기록해두었습니다. (이하 참조)

Tripouille의 [gnlTester](https://github.com/Tripouille/gnlTester)를 사용하여 예외 상황(빈 파일, 표준 입력, 매우 크거나 작은 버퍼 사이즈 등)에 대한 100% 호환성을 확인하였습니다.


  **메모리 관리**:
  
  **macOS**: `leaks -atExit -- ./test` 명령어로 확인.
  
  **Ubuntu**: `valgrind --leak-check=full ./test` 도구로 검증.



---

## 3. 알고리즘 및 데이터 구조 (정당화)

### 동적 스태시 관리 (`t_stash`)

단순한 정적 배열이나 연결 리스트 대신, 함수 호출 간에 남은 데이터를 관리하기 위해 `t_stash`라는 사용자 정의 구조체를 사용하였습니다.

**정당화 (Justification)**: `t_stash` 구조체는 버퍼의 내용물, 현재 크기, 그리고 할당된 용량(capacity)을 관리합니다. `stash_reserve` 함수를 통해 더 많은 공간이 필요할 때마다 버퍼 크기를 동적으로 2배씩 늘림으로써, 메모리 할당 빈도를 최적화하고 `BUFFER_SIZE`와 관계없이 어떤 길이의 줄이든 처리할 수 있도록 설계하였습니다.

### 데이터 보존을 위한 정적 변수 (Static Variable)

여러 번의 `get_next_line` 호출 사이에서 데이터를 유지하기 위해 `static t_stash`를 사용하였습니다.

**정당화 (Justification)**: 정적 변수를 사용함으로써 버퍼에 읽어왔지만 아직 줄의 일부로 반환되지 않은 데이터를 "스태시(stash)"에 저장하고 다음 호출 시 사용할 수 있습니다. 이는 한 번의 `read()` 호출로 여러 줄을 읽어들였을 때 데이터 손실이 발생하는 것을 방지합니다.

### 효율적인 줄 추출 (Line Extraction)

`extract_line`과 `stash_trim` 함수는 상호작용하며 현재 줄을 반환하고 남은 내용을 스태시의 시작 지점으로 이동시킵니다.

**정당화 (Justification)**: 이 방식은 불필요한 메모리 이동을 최소화하며, 스태시를 항상 최적화된 상태로 유지하여 다음 읽기 작업을 준비할 수 있게 합니다.

---

## 4. 참고 자료 (Resources)

### 참조 및 문서

**테스트**: [42-get_next_line-w-tests](https://github.com/jihoon42/get_next_line-w-tests) (직접 작성한 테스트 케이스와 스크립트가 포함된 개인 레포지토리)

**파일 디스크립터**: `read(2)` 함수의 동작과 운영체제별 FD 동작 방식 학습

### AI 활용 고지 (AI Usage Disclosure)

**구조적 개선**: 동적 버퍼 재할당 시 메모리 안전성을 보장하기 위한 `stash_reserve` 로직 설계 보조.

**예외 상황 브레인스토밍**: `BUFFER_SIZE=1`인 경우, 디렉토리를 가리키는 파일 디스크립터, 혹은 줄 바꿈 문자로 끝나지 않는 파일 등에서 발생할 수 있는 잠재적 이슈 식별.

**문서화 최적화**: 본 README의 구조를 잡고 기술적 정당화 섹션을 명확한 영문(및 국문)으로 다듬는 데 도움을 받음.

**대응 조치**: 모든 로직은 수동으로 검증 및 디버깅되었습니다. 최종 코드는 허용되지 않은 외부 라이브러리 의존성 없이 Tripouille 테스터의 모든 필수 테스트를 통과하였습니다.

