#include <stdio.h>
#include <assert.h>
#include <vector>
#include <string>
#include <iostream>


struct point {
    double x, y;
    point(double a=0.0, double b=0.0) : x(a), y(b) {}
};

struct line {
    point p1, p2;
    line(point p, point q) : p1(p), p2(q)
    {
        m = (p2.y-p1.y)/(p2.x-p1.x);
        b = p1.y - (m*p1.x);
    }
    double m; //slope
    double b; //yintercept
};

//return true if l1 and l2 are not parallel, and if such, return the intersection point in intersect.
bool find_intersection_point(const line& l1, const line& l2, point& intersect)
{
    if (l1.m == l2.m) return false;
    //say, intersecting at (x,y).
    //then, y = (l1.m*x) + l1.b = (l2.m*x) + l2.b;
    //so, (l1.m-l2.m)*x = -(l1.b-l2.b);
    //so, 
    double x = -(l1.b-l2.b)/(l1.m-l2.m);
    //and,
    double y = l1.m*x + l1.b;
    intersect.x = x;
    intersect.y = y;
    return true;
}

//if p lies on the infinite straight line l
bool is_colinear(const line& l, const point& p)
{
    if (p.y == ((l.m*p.x) + l.b)) return true;
    return false;
}

//function assumes the point is on the infinite line
bool is_point_within_segment(const line& l, const point& p)
{
#define _Max_(a,b) ((a) > (b) ? (a) : (b))
#define _Min_(a,b) ((a) < (b) ? (a) : (b))

    assert(is_colinear(l,p));
    if (p.x > _Max_(l.p1.x, l.p2.x)) return false;
    if (p.x < _Min_(l.p1.x, l.p2.x)) return false;
    if (p.y > _Max_(l.p1.y, l.p2.y)) return false;
    if (p.y < _Min_(l.p1.y, l.p2.y)) return false;
    return true;
}

//if point of intersection within the line segments
bool is_intersection_within_line_segments(const line& l1, const line& l2)
{
    point intersection;
    if (false == find_intersection_point(l1, l2, intersection)) return false; 
    if (is_point_within_segment(l1, intersection) &&
        is_point_within_segment(l2, intersection)) return true;
    return false;
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <valid expression>" << std::endl;
        return 1;
    }
    return 0;
}


