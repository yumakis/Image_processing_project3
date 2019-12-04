#include <iostream>

#define max(X, Y) ((X) > (Y) ? (X) : (Y))
#define min(X, Y) ((X) < (Y) ? (X) : (Y))

class Point {
public:
  int x, y;

  Point(int xval = 0, int yval = 0) : x(xval), y(yval) {}

  void operator = (const Point &p){
    x = p.x;
    y = p.y;
  }
};

class Rect {
public:
  Point ul, lr;

  Rect(){}
  Rect(Point a, Point b){
    if(a.x < b.x && a.y < b.y) {
      ul = Point(min(a.x, b.x), min(a.y, b.y));
      lr = Point(max(a.x, b.x), max(a.y, b.y));
    }
    else {
      ul = Point(0,0);
      lr = Point(0,0);
    }
  }

  int area(){
    return (lr.x-ul.x)*(lr.y-ul.y);
  }
};

int get_intersection(Rect a, Rect b){
  Point i_ul(max(a.ul.x, b.ul.x), max(a.ul.y, b.ul.y));
  Point i_lr(min(a.lr.x, b.lr.x), min(a.lr.y, b.lr.y));
  return Rect(i_ul, i_lr).area();
}


int get_union(Rect a, Rect b){
  return a.area()+b.area()-get_intersection(a, b);
}

double compute_IOU(Rect expected, Rect actual){
  int intersection_area = get_intersection(expected, actual);
  int union_area = get_union(expected, actual);
  return (double) intersection_area/union_area;
}

class Test_data {
private:
  Rect a, b;
  int i, u;
public:
  Test_data() {}
  Test_data(Rect const & r1, Rect const & r2, int const inter, int const uni) : a(r1), b(r2), i(inter), u(uni) {}

  int execute(){
    int res = 0;
    if(get_intersection(a, b) != i)
      res += 1;
    if(get_union(a, b) != u)
      res += 2;
    return res;
  }

  int diagnose(int i){
    int r = execute();
    if(r != 0) {
      std::cout << "Test " << i << std::endl;
      std::cout << r << std::endl;
    }
    if(r & 1){
      std::cout << "Wrong intersection" << std::endl;
    }
    if(r & 2){
      std::cout << "Wrong union" << std::endl;
    }
    return r != 0;
  }
};

void test_get_intersection(){
  int len = 27;
  Test_data tests[] = {
    // disjoint rectangles
    {{{0, 0}, {1, 1}}, {{2, 2}, {3, 3}}, 0, 2},
    {{{0, 2}, {1, 3}}, {{2, 0}, {3, 1}}, 0, 2},
    {{{2, 2}, {3, 3}}, {{0, 0}, {1, 1}}, 0, 2},
    {{{2, 0}, {3, 1}}, {{0, 2}, {1, 3}}, 0, 2},

    // rectangles touching from one corner
    {{{0, 0}, {1, 1}}, {{1, 1}, {2, 2}}, 0, 2},
    {{{0, 1}, {1, 2}}, {{1, 0}, {2, 1}}, 0, 2},
    {{{1, 1}, {2, 2}}, {{0, 0}, {1, 1}}, 0, 2},
    {{{1, 0}, {2, 1}}, {{0, 1}, {1, 2}}, 0, 2},

    // rectangles touching from one side
    {{{0, 0}, {1, 1}}, {{0, 1}, {1, 2}}, 0, 2},
    {{{0, 0}, {1, 1}}, {{1, 0}, {2, 1}}, 0, 2},
    {{{0, 1}, {1, 2}}, {{0, 0}, {1, 1}}, 0, 2},
    {{{1, 0}, {2, 1}}, {{0, 0}, {1, 1}}, 0, 2},

    // rectangles intersecting 1
    {{{0, 0}, {2, 2}}, {{1, 1}, {3, 3}}, 1, 7},
    {{{0, 1}, {2, 3}}, {{1, 0}, {3, 2}}, 1, 7},
    {{{1, 1}, {3, 3}}, {{0, 0}, {2, 2}}, 1, 7},
    {{{1, 0}, {3, 2}}, {{0, 1}, {2, 3}}, 1, 7},

    // rectangles intersecting 2
    {{{0, 1}, {2, 3}}, {{1, 0}, {3, 4}}, 2, 10},
    {{{1, 0}, {3, 2}}, {{0, 1}, {4, 3}}, 2, 10},
    {{{1, 1}, {3, 3}}, {{0, 0}, {2, 4}}, 2, 10},
    {{{1, 1}, {3, 3}}, {{0, 0}, {4, 2}}, 2, 10},

    {{{1, 0}, {3, 4}}, {{0, 1}, {2, 3}}, 2, 10},
    {{{0, 1}, {4, 3}}, {{1, 0}, {3, 2}}, 2, 10},
    {{{0, 0}, {2, 4}}, {{1, 1}, {3, 3}}, 2, 10},
    {{{0, 0}, {4, 2}}, {{1, 1}, {3, 3}}, 2, 10},

    // one rectangle included in the other
    {{{1, 1}, {2, 2}}, {{0, 0}, {3, 3}}, 1, 9},
    {{{0, 0}, {3, 3}}, {{1, 1}, {2, 2}}, 1, 9},

    // equal rectangles
    {{{0, 0}, {3, 3}}, {{0, 0}, {3, 3}}, 9, 9},

  };
  int failed = 0;
  for(int t = 0; t < len; t++){
    failed += tests[t].diagnose(t);
  }
  printf("Passed %d/%d tests\n", len-failed, len);
}

int main(){
  test_get_intersection();
  return 0;
}
