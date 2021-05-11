#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int SIZE = 8;
int board[SIZE][SIZE];
int results_count = 0; // кол-во решений

void print_board()
{
    for (int a = 0; a < SIZE; ++a) {
        for (int b = 0; b < SIZE; ++b)
            cout << ((board[a][b]) ? "Q  " : "[] ");
        cout << endl;
    }
}

// через рекурсию
bool check_queen(int a, int b) // проверка на установленных ферзей по вертикали и диагонали
{
    for (int i = 0; i < a; ++i)
        if (board[i][b])
            return false;

    for (int i = 1; i <= a && b - i >= 0; ++i)
        if (board[a - i][b - i])
            return false;

    for (int i = 1; i <= a && b + i < SIZE; i++)
        if (board[a - i][b + i])
            return false;

    return true;
}

void find_queen(int a, vector<pair<int, int>>& vec) // нахождение результатов решений
                                                    // a - номер строки, в которую нужно поставить ферзя
{
    if (a == SIZE) {
        print_board();
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (board[i][j] == 1) {
                    vec.push_back(make_pair(i + 1, j + 1));
                    cout << "Q(" << i + 1 << ";" << j + 1 << ")" << endl;
                }
            }
        }
        vec.clear();
        cout << "Result: " << ++results_count << endl;
        return;
    }

    for (int i = 0; i < SIZE; ++i) {
        if (check_queen(a, i)) {   // проверка на то, что ферзь будет единственным в этой строке, столбце и диагонали
            board[a][i] = 1;
            find_queen(a + 1, vec);
            board[a][i] = 0;
        }
    }
}

int main() {
    int flag;
    vector<pair<int, int>> vec;
    cout << "Problem solving method: " << endl << "Recurtion: 1" << endl << "Iteration: 2" << endl;
    cin >> flag;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            board[i][j] = 0;
        }
    }
    if (flag == 1)
        find_queen(0, vec);
    else if (flag == 2)
        cout << "Didn't work :( ";
    else {
        cout << "Wrong chosen!";
        exit(-1);
    }

    return 0;
}