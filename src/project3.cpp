#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#define MAX 2147483647
#define B std::array<std::array<int, SIZE>, SIZE>
struct Point {
    int x, y;
	Point() : Point(0, 0) {}
	Point(float x, float y) : x(x), y(y) {}
	bool operator==(const Point& rhs) const {
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const Point& rhs) const {
		return !operator==(rhs);
	}
	Point operator+(const Point& rhs) const {
		return Point(x + rhs.x, y + rhs.y);
	}
	Point operator-(const Point& rhs) const {
		return Point(x - rhs.x, y - rhs.y);
	}
};
struct node{
    int value;
    int alpha;
    int beta;
    node() : value(-MAX), alpha(-MAX), beta(MAX){}
};
const int SIZE = 8;
std::array<std::array<int, SIZE>, SIZE> board;
std::array<int, 3> disc_count;
int player;
enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};
const std::array<Point, 8> directions{{
    Point(-1, -1), Point(-1, 0), Point(-1, 1),
    Point(0, -1), /*{0, 0}, */Point(0, 1),
    Point(1, -1), Point(1, 0), Point(1, 1)
}};
/*const int score[8][8] = {
    {90,-60,10,10,10,10,-60,90},
    {-60,-80,5,5,5,5,-80,-60},
    {10,5,1,1,1,1,5,10},
    {10,5,1,1,1,1,5,10},
    {10,5,1,1,1,1,5,10},
    {10,5,1,1,1,1,5,10},
    {-60,-80,5,5,5,5,-80,-60},
    {90,-60,10,10,10,10,-60,90},
};*/
/*const int score[8][8] = {
    {50,10,10,10,10,10,10,50},
    {10,-10,1,1,1,1,-10,10},
    {10,1,1,1,1,1,1,10},
    {10,1,1,1,1,1,1,10},
    {10,1,1,1,1,1,1,10},
    {10,1,1,1,1,1,1,10},
    {10,-10,1,1,1,1,-10,10},
    {50,10,10,10,10,10,10,50},
};*/
const int score[8][8] = {
    {1000,-240,85,69,69,85,-240,1000},
    {-240,-130,49,23,23,49,-130,-240},
    {85,49,1,9,9,1,49,85},
    {69,23,9,32,32,9,23,69},
    {69,23,9,32,32,9,23,69},
    {85,49,1,9,9,1,49,85},
    {-240,-130,49,23,23,49,-130,-240},
    {1000,-240,85,69,69,85,-240,1000},
};
int get_next_player(int player){
    return 3 - player;
}
bool is_spot_on_board(Point p){
    return 0 <= p.x && p.x < SIZE && 0 <= p.y && p.y < SIZE;
}
int get_disc(Point p){
    return board[p.x][p.y];
}
int get_disc(B boards,Point p){
    return boards[p.x][p.y];
}
void set_disc(Point p, int disc) {
    board[p.x][p.y] = disc;
}
void set_disc(B boards, Point p, int disc) {
    boards[p.x][p.y] = disc;
}
bool is_disc_at(Point p, int disc){
    if (!is_spot_on_board(p))
        return false;
    if (get_disc(p) != disc)
        return false;
    return true;
}
bool is_disc_at(B boards, Point p, int disc){
    if (!is_spot_on_board(p))
        return false;
    if (get_disc(boards, p) != disc)
        return false;
    return true;
}
bool is_spot_valid(Point center,int cur_player){
    if (get_disc(center) != EMPTY)
        return false;
    for (Point dir: directions) {
        // Move along the direction while testing.
        Point p = center + dir;
        if (!is_disc_at(p, get_next_player(cur_player)))
            continue;
        p = p + dir;
        while (is_spot_on_board(p) && get_disc(p) != EMPTY) {
            if (is_disc_at(p, cur_player))
                return true;
            p = p + dir;
        }
    }
    return false;
}
bool is_spot_valid(B boards, Point center,int cur_player){
    if (get_disc(boards, center) != EMPTY)
        return false;
    for (Point dir: directions) {
        // Move along the direction while testing.
        Point p = center + dir;
        if (!is_disc_at(boards, p, get_next_player(cur_player)))
            continue;
        p = p + dir;
        while (is_spot_on_board(p) && get_disc(p) != EMPTY) {
            if (is_disc_at(boards, p, cur_player))
                return true;
            p = p + dir;
        }
    }
    return false;
}
std::vector<Point> flip_discs(Point center,int cur_player) {
    std::vector<Point> ret;
    for (Point dir: directions) {
        // Move along the direction while testing.
        Point p = center + dir;
        if (!is_disc_at(p, get_next_player(cur_player)))
            continue;
        std::vector<Point> discs({p});
        p = p + dir;
        while (is_spot_on_board(p) && get_disc(p) != EMPTY) {
            if (is_disc_at(p, cur_player)) {
                for (Point s: discs) {
                    ret.push_back(s);
                    set_disc(s, cur_player);
                }
                break;
            }
            discs.push_back(p);
            p = p + dir;
        }
    }
    return ret;
}
void flip_discs(B &boards,Point center,int cur_player) {
    for (Point dir: directions) {
        // Move along the direction while testing.
        Point p = center + dir;
        if (!is_disc_at(boards, p, get_next_player(cur_player)))
            continue;
        std::vector<Point> discs({p});
        p = p + dir;
        while (is_spot_on_board(p) && get_disc(boards, p) != EMPTY) {
            if (is_disc_at(boards, p, cur_player)) {
                for (Point s: discs) {
                    boards[s.x][s.y] = cur_player;
                }
                break;
            }
            discs.push_back(p);
            p = p + dir;
        }
    }
}

std::vector<Point> get_valid_spots(int cur_player){
    std::vector<Point> valid_spots;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            Point p = Point(i, j);
            if (board[i][j] != EMPTY)
                continue;
            if (is_spot_valid(p,cur_player))
                valid_spots.push_back(p);
        }
    }
    return valid_spots;
}
std::vector<Point> get_valid_spots(B boards, int cur_player){
    std::vector<Point> valid_spots;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            Point p = Point(i, j);
            if (boards[i][j] != EMPTY)
                continue;
            if (is_spot_valid(p,cur_player))
                valid_spots.push_back(p);
        }
    }
    return valid_spots;
}
void put_disc(Point p,int cur_player) {
    set_disc(p, cur_player);
    disc_count[EMPTY]--;
}
void flip_back(std::vector<Point> p,int cur_player){
    int next_player = get_next_player(cur_player);
    for(auto i:p){
        board[i.x][i.y] = next_player;
    }
}
void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
            disc_count[board[i][j]]++;
        }
    }
}
int count_value(){
    int ret = 0;
    int opponent = get_next_player(player);
    for(int i = 0;i < SIZE;++i){
        for(int j = 0;j < SIZE;++j){
            if(board[i][j] == player)
                ret+=score[i][j];
            else if(board[i][j] == opponent)
                ret-=score[i][j];
        }
    }
    return ret;
}
int count_value(B boards){
    int ret = 0;
    int opponent = get_next_player(player);
    for(int i = 0;i < SIZE;++i){
        for(int j = 0;j < SIZE;++j){
            if(boards[i][j] == player)
                ret+=score[i][j];
            else if(boards[i][j] == opponent)
                ret-=score[i][j];
        }
    }
    return ret;
}
void write_valid_spot(std::ofstream& fout,Point ans) {
    fout << ans.x << " " << ans.y << std::endl;
}
B get_board(B boards,Point p,int cur_player){
    B ret = boards;
    ret[p.x][p.y] = cur_player;
    disc_count[EMPTY]--;
    flip_discs(ret, p, cur_player);
    return ret;
}
int traversal_tree(int cur_player,int depth, int alpha, int beta){
    std::vector<Point> next_valid_spots = get_valid_spots(cur_player);
    std::vector<Point> ready_to_flip;
    int n = next_valid_spots.size();
    int value = cur_player == player ? -MAX : MAX;
    if(depth != 0){
        for(int i = 0;i < n;++i){
            put_disc(next_valid_spots[i],cur_player);
            ready_to_flip = flip_discs(next_valid_spots[i],cur_player);
            if(cur_player == player){
                value = std::max(value, traversal_tree(get_next_player(cur_player),depth - 1, alpha, beta));
                alpha = std::max(alpha, value);
            }
            else{
                value = std::min(value, traversal_tree(get_next_player(cur_player),depth - 1, alpha, beta));
                beta = std::min(beta, value);
            }
            set_disc(next_valid_spots[i],EMPTY);
            disc_count[EMPTY]++;
            flip_back(ready_to_flip,cur_player);
            if(alpha >= beta)
                break;
        }
        return value;
    }else{
        int ret = count_value();
        return ret;
    }
}
void sol(std::ofstream& fout,int cur_player){
    std::vector<Point> next_valid_spots = get_valid_spots(cur_player);
    std::vector<Point> ready_to_flip;
    int n = next_valid_spots.size();
    int alpha = -MAX,beta = MAX;
    int value = -MAX;
    int tmp_value,depth;
    if(n != 0){
        if(disc_count[EMPTY] > 20)
            depth = 7;
        else if(disc_count[EMPTY] > 17)
            depth = 8;
        else if(disc_count[EMPTY] > 15)
            depth = 9;
        else if(disc_count[EMPTY] > 13)
            depth = 10;
        else
            depth = 11;
        write_valid_spot(fout, next_valid_spots[0]);
        for(int i = 0;i < n;++i){
            put_disc(next_valid_spots[i],cur_player);
            ready_to_flip = flip_discs(next_valid_spots[i],cur_player);
            tmp_value = traversal_tree(get_next_player(cur_player), 7, alpha, beta);
            //alpha = std::max(alpha,tmp_value);
            if(tmp_value >= value){
                value = tmp_value;
                write_valid_spot(fout,next_valid_spots[i]);
            }
            set_disc(next_valid_spots[i],EMPTY);
            disc_count[EMPTY]++;
            flip_back(ready_to_flip,cur_player);
            /*B board_tmp = get_board(board, next_valid_spots[i], cur_player);
            tmp_value = traversal_tree(get_next_player(cur_player), depth, alpha, beta, board_tmp);
            //alpha = std::max(alpha,tmp_value);
            if(tmp_value >= value){
                value = tmp_value;
                write_valid_spot(fout,next_valid_spots[i]);
            }
            disc_count[EMPTY]++;*/
        }
    }
}
int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    sol(fout,player);
    fin.close();
    fout.close();
    return 0;
}
