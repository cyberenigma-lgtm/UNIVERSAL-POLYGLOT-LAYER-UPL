# MultiLang-ASM Language Pack: Korean (ko)

METADATA = {
    "name": "Korean / 한국어",
    "code": "ko",
    "author": "Neuro-OS Community",
    "version": "1.0",
    "description": "한국어 어셈블러 지원."
}

KEYWORDS = {
    # 데이터 이동
    "이동": "mov", "교환": "xchg", "주소로드": "lea",
    "영확장": "movzx", "부호확장": "movsx",
    
    # 산술 연산
    "더하기": "add", "뺄셈": "sub", "곱하기": "mul",
    "나누기": "div", "증가": "inc", "감소": "dec",
    "부정": "neg",
    
    # 비교
    "비교": "cmp", "테스트": "test",
    
    # 흐름 제어
    "점프": "jmp", "호출": "call", "돌아가기": "ret",
    "같으면": "je", "영이면": "jz", "다르면": "jne",
    "크면": "jg", "작으면": "jl",
    
    # 스택
    "넣기": "push", "빼기": "pop", "플래그넣기": "pushf", "플래그빼기": "popf",
    
    # 문자열
    "바이트이동": "movsb", "바이트저장": "stosb", "바이트로드": "lodsb",
    "반복": "rep",
    
    # 루프
    "루프": "loop", "영이면루프": "loopz",
    
    # 시스템
    "인터럽트": "int", "시스템호출": "syscall", "시스템복귀": "sysret",
    "정지": "hlt", "무작작": "nop",
    
    # 변환
    "바이트를워드로": "cbw", "워드를더블로": "cwd", "더블을쿼드로": "cdq",
}

KIDS_KEYWORDS = {
    "ko": {"neoh": "mov", "deohae": "add", "ppaera": "sub", "boyeo": "syscall"},
}
