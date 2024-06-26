import random
import sys
from collections import deque

def minput(a):
    return map(int, input(a).split())


def print_board(board):
    print('_' * (2 * len(board[0]) + 3))
    for b in board:
        print(f"| {' '.join(b)} |")
    print('_' * (2 * len(board[0]) + 3))


# input
M, N = minput("가로 크기와 세로 크기를 공백으로 구분해 입력해주세요: ")
C = int(input("지뢰 개수를 입력해 주세요: "))
if N * M <= C:
    print("지뢰의 개수가 너무 많아 프로그램을 실행할 수 없어요.")
    sys.exit(0)
mines = random.sample(range(M * N), C)

# make board
board = [['.'] * M for _ in range(N)]
for i in mines:
    board[i % N][i // N] = '#'
board_visual = [['_'] * M for _ in range(N)]
dx = [-1,  0,  1,  1,  1,  0, -1, -1]
dy = [ 1,  1,  1,  0, -1, -1, -1,  0]
for y in range(N):
    for x in range(M):
        if board[y][x] != '#':
            cnt = 0
            for i in range(8):
                ny, nx = y + dy[i], x + dx[i]
                if 0 <= ny < N and 0 <= nx < M and board[ny][nx] == '#':
                    cnt += 1
            board[y][x] = str(cnt)

# game starts here
mines_left = C
blanks_left = M * N - C
while blanks_left:  # 모든 지뢰가 아닌 칸을 열면 게임을 이기기 때문에 종료조건을 이렇게 둠
    # 적당히 입력받음
    print(f"남은 지뢰 개수: {mines_left}")
    print_board(board_visual)
    flag = input("깃발을 꽂으시려면 1을 입력해주세요. 아니라면 칸을 열어보는 것으로 간주합니다: ")
    flag = flag == '1'
    while True:
        nextx, nexty = minput("왼쪽에서 몇 번째, 위에서 몇 번째 칸을 열어볼까요? 공백으로 구분해 입력해주세요. 인덱스는 1부터 시작합니다.")
        nextx -= 1
        nexty -= 1
        check = False
        if 0 <= nexty < N and 0 <= nextx <= M:
            if (board_visual[nexty][nextx] in ['!', '_'] and flag) or (board_visual[nexty][nextx] == '_' and not flag):
                check = True
                break
        if not check:
            print("올바르지 않은 입력이에요.")

    if flag:
        # 깃발꽂는부분
        if board_visual[nexty][nextx] == '_':
            board_visual[nexty][nextx] = '!'
            mines_left -= 1
        else:
            board_visual[nexty][nextx] = '_'
            mines_left += 1
    else:
        # 칸 열어보는 부분
        mine_info = board[nexty][nextx]
        if mine_info == '#':  # 지뢰면 터짐
            board_visual[nexty][nextx] = '#'
            print("지뢰를 클릭하셨어요...")
            break
        elif mine_info != '0':  # 0이 아닌 수면 그 칸만 열어줌
            board_visual[nexty][nextx] = board[nexty][nextx]
            blanks_left -= 1
        else:  # 0이면 BFS로 0인 칸들 다 열어줌
            q = deque([(nexty, nextx)])
            board_visual[nexty][nextx] = '0'
            while q:
                y, x = q.popleft()
                blanks_left -= 1
                for i in range(8):
                    ny, nx = y + dy[i], x + dx[i]
                    if 0 <= ny < N and 0 <= nx < M and board_visual[ny][nx] == '_':
                        if board[ny][nx] == '0':
                            q.append((ny, nx))
                        board_visual[ny][nx] = board[ny][nx]
else:
    print("모든 칸을 열어 게임에서 승리했어요!")

print_board(board)
