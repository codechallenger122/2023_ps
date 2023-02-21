/*
	Date: 2022/08/23
	Brief:

	Reference:
*/
#include <iostream>
#include <string>
#include <cmath>
#include <queue>

#define MAX 11

using namespace std;

string map[MAX];
int n, m;
int res = MAX;

struct pos {
	int x;
	int y;
};

pos redPos;
pos bluePos;

bool inRange(pos p) {
	// size 가 n행, m열 이므로, 0 ~ n-1, 0 ~ m-1 의 index 를 가진다.
	return (0 <= p.x && p.x < n && 0 <= p.y && p.y < m);
}

void input() {
	for (int i = 0; i < n; i++) {
		string tmp;
		cin >> tmp;

		map[i] = "";

		for (int j = 0; j < m; j++) {
			if (tmp[j] == 'R' || tmp[j] == 'B') {
				map[i] += '.';

				if (tmp[j] == 'R') {
					redPos.x = i, redPos.y = j;
				}
				else {
					bluePos.x = i, bluePos.y = j;
				}
				continue;
			}
			map[i] += tmp[j];
		}
	}
}

// 현재 방향의 반대 방향을 반환하는 함수
int inverseDirection(int dir) {
	if (dir == 0)
		return 1;
	else if (dir == 1)
		return 0;
	else if (dir == 2)
		return 3;
	else
		return 2;
}

int distance(pos x, pos y) {
	return abs(x.x - y.x) + abs(x.y - y.y);
}

void BFS(pos red, pos blue) {
	// pos red, pos blue, dir, cnt
	queue<pair<pair<pos, pos>, pair<int, int>>> q;

	int dx[] = { -1, 1, 0, 0 };
	int dy[] = { 0, 0, -1, 1 };

	bool redFlag = false, blueFlag = false;

	// 맨 처음 위치에서 4 개의 방향으로 움직여본다.
	for (int i = 0; i < 4; i++) {
		int dir = i;

		pos next_red = red;
		pos next_blue = blue;

		// 빨간 공.
		while (1) {
			// # : 벽이거나, O : 목표지점 이면, break.
			if (map[next_red.x][next_red.y] == '#')
				break;
			else if (map[next_red.x][next_red.y] == 'O') {
				redFlag = true;
				break;
			}
			// bound 에서 벗어나면, break.
			if (!inRange(next_red))
				break;
			next_red.x += dx[dir], next_red.y += dy[dir];
		}
		next_red.x -= dx[dir], next_red.y -= dy[dir];

		// 파란 공.
		while (1) {
			// # : 벽이거나, O : 목표지점 이면, break.
			if (map[next_blue.x][next_blue.y] == '#')
				break;
			else if (map[next_blue.x][next_blue.y] == 'O') {
				blueFlag = true;
				break;
			}
			// bound 에서 벗어나면, break.
			if (!inRange(next_blue))
				break;
			next_blue.x += dx[dir], next_blue.y += dy[dir];
		}
		next_blue.x -= dx[dir], next_blue.y -= dy[dir];

		// 두 구슬의 위치가 같은 경우
		if (next_red.x == next_blue.x && next_red.y == next_blue.y) {
			int redDist = distance(red, next_red);
			int blueDist = distance(blue, next_blue);

			// 더 많이 움직인 구슬을 한 칸 전으로 옮긴다.
			if (redDist > blueDist) {
				next_red.x -= dx[dir], next_red.y -= dy[dir];
			}
			else {
				next_blue.x -= dx[dir], next_blue.y -= dy[dir];
			}
		}

		// 빨간 공만 구멍에 넣는데 성공한 경우
		if (redFlag && !blueFlag) {
			res = 1;
			redFlag = false, blueFlag = false;
			return;
		}
		// 파란 공이 빠지면 의미 없음
		else if (blueFlag) {
			redFlag = false, blueFlag = false;
			continue;
		}
		// 두 개의 공이 모두 구멍에 빠지지 않은 경우
		else {
			q.push({ {next_red, next_blue}, {dir, 1} });
		}

	}

	while (!q.empty()) {
		pos cur_red = q.front().first.first;
		pos cur_blue = q.front().first.second;

		int dir = q.front().second.first;
		int cnt = q.front().second.second;
		q.pop();

		if (cnt >= res)
			continue;

		for (int i = 0; i < 4; i++) {
			pos next_red = cur_red;
			pos next_blue = cur_blue;

			// 이전 진행 방향과 수평 방향은 탐색 의미가 없다.
			if (i == dir || i == inverseDirection(dir))
				continue;

			int next_dir = i;

			while (1) {
				if (map[next_red.x][next_red.y] == '#')
					break;
				else if (map[next_red.x][next_red.y] == 'O') {
					redFlag = true;
					break;
				}
				next_red.x += dx[next_dir], next_red.y += dy[next_dir];
				if (!inRange(next_red))
					break;
			}
			next_red.x -= dx[next_dir], next_red.y -= dy[next_dir];

			while (1) {
				if (map[next_blue.x][next_blue.y] == '#')
					break;
				else if (map[next_blue.x][next_blue.y] == 'O') {
					blueFlag = true;
					break;
				}
				next_blue.x += dx[next_dir], next_blue.y += dy[next_dir];
				if (!inRange(next_blue))
					break;
			}
			next_blue.x -= dx[next_dir], next_blue.y -= dy[next_dir];

			// 두 구슬의 위치가 같은 경우
			if (next_red.x == next_blue.x && next_red.y == next_blue.y) {
				int redDist = distance(cur_red, next_red);
				int blueDist = distance(cur_blue, next_blue);

				// 더 많이 움직인 구슬을 한 칸 전으로 옮긴다.
				if (redDist > blueDist) {
					next_red.x -= dx[next_dir], next_red.y -= dy[next_dir];
				}
				else {
					next_blue.x -= dx[next_dir], next_blue.y -= dy[next_dir];
				}
			}

			// 빨간 공만 구멍에 넣는데 성공한 경우
			if (redFlag && !blueFlag) {
				res = (res > cnt + 1) ? cnt + 1 : res;
				return;
			}
			// 파란 공이 빠지면 의미 없음
			else if (blueFlag) {
				redFlag = false, blueFlag = false;
				continue;
			}
			// 두 개의 공이 모두 구멍에 빠지지 않은 경우
			else {
				q.push({ {next_red, next_blue}, {next_dir, cnt + 1} });
			}
		}
	}

	return;
}

/*
1. 두 개의 공의 위치를 기준으로 BFS 수행
2. 빨간 공을 특정 방향으로 기울여서 움직인다.
3. 파란 공을 특정 방향으로 기울여서 움직인다.
4. 두 공의 도착 위치가 같은 경우 더 많이 움직인 공을 -1 만큼 이동시킨다.
5. 더 이상 공을 움직일 수 없는 경우 종료
6. 혹은 공이 하나라도 구멍에 빠지는 경우 종료
*/

int main() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> n >> m;

	input();

	BFS(redPos, bluePos);

	if (res >= MAX) cout << -1;
	else cout << res;

	return 0;
}
