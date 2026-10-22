# 🚀 빠른 시작 가이드: UPL+ 프로그래밍 언어
## 고성능 결정론적 시스템 언어 및 독립형 C99 네이티브 컴파일러

**UPL+ (Universal Polyglot Layer Plus)** 공식 기술 문서에 오신 것을 환영합니다. UPL+는 극대화된 하드웨어 제어, 크로스-폴리글랏 융합, 그리고 금융 연산의 정확성을 위해 설계된 **컴파일형, 정적 타입, 완전 자율형 시스템 프로그래밍 언어**입니다.

---

### 🌟 UPL+ 핵심 아키텍처 원칙

1. **완전 독립형 컴파일러 (`uplc`):** Python, 무거운 런타임, GC에 의존하지 않는 순수 ANSI C99 기반 (빌드 속도 50ms 미만).
2. **글로벌 다국어 지원:** 렉서(Lexer) 레벨에서 세계 주요 언어의 키워드를 네이티브하게 AST 노드로 정규화.
3. **오차 없는 금융 연산 (`comp3`):** IEEE-754 부동소수점 오차를 완전히 제거한 소프트웨어 팩트 BCD 10진수 타입.
4. **실리콘 직결 융합 (`embed` 및 `asm`):** FFI 오버헤드 없는 순수 C99 코드 및 x86_64 어셈블리 직접 인라인 작성.
5. **고속 아레나 메모리 관리:** 64KB 단위 결정론적 할당으로 단편화를 방지하고 $O(1)$ 즉시 메모리 해제.

---

### 📦 첫 번째 UPL+ 프로그램

```upl
module example;

pub fn main() -> i32 {
    let mut total: i32 = 0;
    let base: i32 = 1000;
    let tax: i32 = 10;

    total = base + (base * tax / 100);
    return 0;
}
```

---

### ⚡ 컴파일 및 실행 방법

```bash
# 1단계 컴파일 및 즉시 실행:
bin/uplc.exe run program.upl

# 네이티브 64비트 바이너리 빌드:
bin/uplc.exe build program.upl -o bin/program.exe
```

---
*Universal Polyglot Layer (UPL) — SafeCreative 지적 재산권 보호 작품.*
