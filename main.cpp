#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>

#define INVALID -1

using namespace std;

pair<int, int> find_char(const vector<vector<char>> &m, char c) {
  for (int row = 0; row < m.size(); row++)
    for (int col = 0; col < m[row].size(); col++)
      if (m[row][col] == c)
        return make_pair(row, col);

  return make_pair(INVALID, INVALID);
}

pair<int, int> find_goal(const vector<vector<char>> &m) {
  return find_char(m, 'G');
}

pair<int, int> find_start(const vector<vector<char>> &m) {
  return find_char(m, 'S');
}

bool is_valid(const pair<int, int> &new_position,
              const vector<vector<char>> &m) {
  return new_position.first >= 0 && new_position.second >= 0 &&
         new_position.first < m.size() && new_position.second < m.at(0).size();
}

vector<pair<int, int>> get_neighbors(const vector<vector<char>> &m,
                                     const pair<int, int> &node) {
  vector<pair<int, int>> neighbors;
  const vector<pair<int, int>> directions{
      {0, +1},  // RIGHT
      {+1, +1}, // RIGHT BELOW
      {+1, 0},  // BELOW
      {+1, -1}, // LEFT BELOW
      {0, -1},  // LEFT
      {-1, -1}, // LEFT ABOVE
      {-1, 0},  // ABOVE
      {-1, +1}, // RIGHT ABOVE
  };

  for (const auto &direction : directions) {
    pair<int, int> new_position = {node.first + direction.first,
                                   node.second + direction.second};
    if (is_valid(new_position, m)) {
      neighbors.push_back(new_position);
    }
  }
  return neighbors;
}

void display_matrix(const vector<vector<char>> &m) {
  for (const auto &row : m) {
    for (const auto &c : row) {
      cout << c;
    }
    cout << endl;
  }
  cout << endl << endl;
}

int display_path(vector<vector<char>> &m, const pair<int, int> &start,
                 const pair<int, int> &goal,
                 const map<pair<int, int>, pair<int, int>> &prev) {

  int path_lenght = 0;
  auto node_it = prev.find(goal);

  while (node_it != prev.end()) {
    path_lenght++;
    const pair<int, int> node = node_it->first;

    if (m[node.first][node.second] != 'S' &&
        m[node.first][node.second] != 'G') {
      m[node.first][node.second] = '*';
    }
    node_it = prev.find(node_it->second);
  }

  return path_lenght;
}

bool bfs(vector<vector<char>> &m, const pair<int, int> &start,
         const pair<int, int> &goal) {

  bool success = false;
  queue<pair<int, int>> q;
  set<pair<int, int>> visited;
  map<pair<int, int>, pair<int, int>> prev;

  q.push(start);
  visited.insert(start);

  while (!q.empty()) {
    const pair<int, int> node = q.front();

    if (m[node.first][node.second] == 'G') {
      success = true;
      break;
    }

    vector<pair<int, int>> neighbors = get_neighbors(m, node);

    for (const auto &neighbor : neighbors) {
      if (visited.find(neighbor) == visited.end()) {
        q.push(neighbor);
        visited.insert(neighbor);
        prev.insert({neighbor, node});
      }
    }

    if (m[node.first][node.second] != 'S' &&
        m[node.first][node.second] != 'G') {
      m[node.first][node.second] = 'o';
    }
    display_matrix(m);
    q.pop();
    system("sleep 0.15");
  }

  int path_length = display_path(m, start, goal, prev);
  display_matrix(m);

  cout << "Path Lenght: " << path_length << endl;

  return success;
}

int main(int argc, const char **argv) {

  pair<int, int> start{INVALID, INVALID};
  pair<int, int> goal{INVALID, INVALID};

  vector<vector<char>> matrix{
      {'#', '#', '#', '#', '#', '#', '#', '#', '#'},
      {'#', '#', '#', '#', '#', '#', '#', '#', '#'},
      {'#', 'S', '#', '#', '#', '#', '#', '#', '#'},
      {'#', '#', '#', '#', '#', '#', '#', '#', '#'},
      {'#', '#', '#', '#', '#', '#', '#', '#', '#'},
      {'#', '#', '#', '#', '#', '#', '#', '#', '#'},
      {'#', '#', '#', '#', '#', '#', '#', 'G', '#'},
      {'#', '#', '#', '#', '#', '#', '#', '#', '#'},
      {'#', '#', '#', '#', '#', '#', '#', '#', '#'},
  };

  start = find_start(matrix);
  goal = find_goal(matrix);
  const bool goal_reached = bfs(matrix, start, goal);

  return 0;
}
