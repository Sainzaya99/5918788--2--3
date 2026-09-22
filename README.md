# 이진트리 프로그램

노드를 넣고, 빼고, 바꾸는 이진트리 프로그램입니다.
노드 위치는 경로로 씁니다. (예: `/A/B/C`)

## 파일

- `btree.h`, `btree.c` — 트리 기능
- `main.c` — 명령어 처리
- `README.md` — 테스트용 입력

## 명령어

| 명령어 | 형식 | 예시 |
|---|---|---|
| Insert (I) | `Insert / A` | 루트 만들기 |
| Insert (I) | `Insert 경로 L|R 값` | `Insert /A L B` |
| Delete (D) | `Delete 경로` | 자식 없는 노드만 삭제 |
| Update (U) | `Update 경로 값` | 값 변경 (경로도 바뀜) |
| Read (R) | `Read 경로` | 자식 보기 |
| Print (P) | `Print` | 트리 전체 보기 |

잘못 입력하면 오류만 나오고, 프로그램은 계속 실행됩니다.
