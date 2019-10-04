//Bashir Abdullah
//Experimenting Buffons needle.
#include <Graph.h>
#include <GUI.h>
#include <Simple_window.h>
using namespace Graph_lib;
/*
 * This class provide functionality to display experiment result message.
 * I.e: To display text like: "Found 17 crossed needles, and estimated pi is 3.14157!" into colored box.
 **/
class Message
{
public:
    
    Message(Graph_lib::Window &win); // constructor
    
    void update(int cross_count, int needle_count); // update message
    
    void show(Graph_lib::Window &win); // show message
    void hide(Graph_lib::Window &win); // hide message
    
    void setVisible(bool visible); // change visibility state
    
private:
    
    bool visible;  // flag to store visibility state of the message
    
    Text text;     // text field with message
    Rectangle box; // colored box around message
};

Message::Message(Graph_lib::Window &win):
visible(false), // by default visible = false
text { Point {win.x_max()/2-150, win.y_max()/2}, ""}, // create text field in center of the window
box  { Point {win.x_max()/2-170, win.y_max()/2-15}, 350, 20 } // create box in center of the window
{
    box.set_color(Color::invisible); // set box-border color to invisible
    box.set_fill_color(Color(Color::cyan)); // set box color to cyan
    text.set_color(Color::white); // set text color to white
}

void Message::update(int cross_count, int needle_count) // update message text
{
    char buf[70];
    sprintf(buf, "Found %d crossed needles, and estimated pi is %.5f!", cross_count, 2.0*needle_count/cross_count);
    text.set_label(string(buf));
}

void Message::show(Graph_lib::Window &win)
{
    if (visible)
    {
        win.attach(box);
        win.attach(text);
    }
}

void Message::hide(Graph_lib::Window &win)
{
    if (visible)
    {
        win.detach(box);
        win.detach(text);
    }
}

void Message::setVisible(bool visible_)
{
    visible = visible_;
}

/*
 * Class with information about one needle:
 *
 * +id
 * +segment
 * +length
 * +(centerX, centerY)
 *
 **/
class Needle
{
public:
    
    Needle(int id, int length, int centerX, int centerY); // constructor
    ~Needle(); // destructor
    
    void show(Graph_lib::Window *win, bool showList); // show needle
    void hide(Graph_lib::Window *win, bool showList); // hide needle
    
    void rotate(); // rotate it needle
    
    bool crossed(Point p1, Point p2); // check if it needle crossed with segment(p1, p2)
    void markAsCrossed(); // color in red
    
private:
    
    int area(Point a, Point b, Point c);
    bool intersect_1(int a, int b, int c, int d);
    
    int length, centerX, centerY;
    
    Text* name;
    Line* segment;
};

Needle::Needle(int id, int length, int centerX, int centerY):
length(length),
centerX(centerX),
centerY(centerY),
name(new Text(Point { centerX+10, centerY }, to_string(id))),
segment(NULL)
{
    rotate();
}

Needle::~Needle()
{
    delete name;
    delete segment;
}

void Needle::show(Graph_lib::Window *win, bool showList)
{
    if (showList)
        win->attach(*name);
    win->attach(*segment);
}

void Needle::hide(Graph_lib::Window *win, bool showList)
{
    if (showList)
        win->detach(*name);
    win->detach(*segment);
}

void Needle::rotate()
{
    double angle = M_PI*rand()/RAND_MAX; // obtain random angle
    int dx = (int) (0.5*length*cos(angle)), dy = (int) (0.5*length*sin(angle)); // calculate new direction
    
    if (segment != NULL)
        delete segment; // release old segment
    segment = new Line(Point { centerX+dx, centerY+dy }, Point { centerX-dx, centerY-dy }); // create new one
}

int Needle::area(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool Needle::intersect_1(int a, int b, int c, int d) {
    if (a > b) swap(a, b);
    if (c > d) swap(c, d);
    return max(a, c) <= min(b, d);
}

bool Needle::crossed(Point a, Point b)
{
    Point c = segment->point(0);
    Point d = segment->point(1);
    return intersect_1(a.x, b.x, c.x, d.x)
    && intersect_1(a.y, b.y, c.y, d.y)
    && area(a, b, c) * area(a, b, d) <= 0
    && area(c, d, a) * area(c, d, b) <= 0;
}

void Needle::markAsCrossed()
{
    segment->set_color(Color::red);
}

/*
 * Canvas to visualize the experiment result
 *
 **/
class Canvas
{
    
    static const int N = 6; // line count
    static const int OFFSET_H = 25; // horizontal offset
    static const int OFFSET_V = 25; // vertical offset
    
public:
    
    Canvas(Graph_lib::Window &win);
    ~Canvas();
    
    void show();
    void hide();
    
    void clear();
    
    void rotate();
    void drop(int needle_count);
    
    void toggleList();
    
    int getNeedleCount();
    int getCrossCount();
    
private:
    
    bool showList;
    int distance;
    
    Lines lines;
    Graph_lib::Window* win;
    
    vector<Needle*> needles;
};

Canvas::Canvas(Graph_lib::Window &win):
win(&win),
showList(false),
distance((win.x_max() - 2*OFFSET_H) / (N+1))
{
    for (int i = 0, x = OFFSET_H + distance; i < N; ++i, x += distance)
        lines.add(Point(x, OFFSET_V), Point(x, win.y_max() - OFFSET_V));
    lines.set_color(Color::yellow);
    win.attach(lines);
}

Canvas::~Canvas()
{
    clear();
    win->detach(lines);
}

void Canvas::show()
{
    for (auto needle: needles)
        needle->show(win, showList);
}

void Canvas::hide()
{
    for (auto needle: needles)
        needle->hide(win, showList);
}

void Canvas::clear()
{
    for (auto needle: needles)
        delete needle;
    vector<Needle*>().swap(needles);
}

void Canvas::rotate()
{
    for (auto needle: needles)
        needle->rotate();
}

void Canvas::drop(int needle_count)
{
    for (int i = 0; i < needle_count; ++i)
    {
        // generate random needle's center
        int x = OFFSET_H+distance/2 + rand()%(win->x_max()-distance-2*OFFSET_H),
        y = OFFSET_V            + rand()%(win->y_max()         -2*OFFSET_V);
        needles.push_back(new Needle(i, distance, x, y));
    }
}

void Canvas::toggleList()
{
    showList = !showList;
}

int Canvas::getNeedleCount()
{
    return needles.size();
}

int Canvas::getCrossCount()
{
    int cross_count = 0;
    for (auto needle: needles)
        for (int line = 0; line < N; ++line)
            if (needle->crossed(lines.point(2*line), lines.point(2*line+1)))
            {
                needle->markAsCrossed();
                ++cross_count;
                break;
            }
    return cross_count;
}

/*
 * Controller.
 *
 * Provide following main actions:
 *
 * +drop
 * +rotate
 * +count
 * +toggleList
 *
 **/
class BuffonsNeedle: Graph_lib::Window
{
    
public:
    
    BuffonsNeedle(Point xy, int width, int height, const string &title);
    ~BuffonsNeedle();
    
    // main functionality
    void drop();
    void toggleList();
    void rotate();
    void count();
    
    void menu_pressed();
    
private:
    
    void clear();
    void show();
    
    Canvas canvas;
    Message message;
    
    Button drop_button;
    Button quit_button;
    In_box drop_count;
    
    Menu motion_menu;
    Button menu_button;
};

BuffonsNeedle::BuffonsNeedle(Point xy, int width, int height, const string &title):

Graph_lib::Window     { xy, width, height, title },

canvas     { *this },
message    { *this },

drop_button { Point {x_max()-150,  0}, 70, 20, "Drop", [](Address, Address pw) { reference_to<BuffonsNeedle>(pw).drop(); } },
quit_button { Point {x_max()- 70,  0}, 70, 20, "Quit", [](Address, Address pw) { reference_to<BuffonsNeedle>(pw).hide(); } },
drop_count  { Point {x_max()-230,  0}, 70, 20, "Enter drop count"},

motion_menu   { Point {x_max() - 70, 40}, 70, 20, Menu::vertical, "Motion menu"},
menu_button { Point {x_max() - 90, 30}, 90, 20, "Motion menu", [](Address, Address pw) { reference_to<BuffonsNeedle>(pw).menu_pressed(); } }
{
    
    attach(drop_button);
    attach(quit_button);
    attach(drop_count);
    
    motion_menu.attach(new Button(Point {0, 0}, 0, 0, "Count", [](Address, Address pw) { reference_to<BuffonsNeedle>(pw).count(); }));
    motion_menu.attach(new Button(Point {0, 0}, 0, 0, "Rotate", [](Address, Address pw) { reference_to<BuffonsNeedle>(pw).rotate(); }));
    motion_menu.attach(new Button(Point {0, 0}, 0, 0, "(Un)list", [](Address, Address pw) { reference_to<BuffonsNeedle>(pw).toggleList(); }));
    
    attach(motion_menu);
    motion_menu.hide();
    attach(menu_button);
}

BuffonsNeedle::~BuffonsNeedle()
{
    detach(drop_button);
    detach(quit_button);
    detach(drop_count);
    
    detach(motion_menu);
    detach(menu_button);
    
    clear();
    canvas.clear();
}

void BuffonsNeedle::drop() // perform drop command
{
    clear();
    
    canvas.clear();
    canvas.drop(drop_count.get_int());
    message.setVisible(false);
    
    show();
}

void BuffonsNeedle::toggleList() // perform (un)list operation
{
    clear();
    
    canvas.toggleList();
    
    show();
}

void BuffonsNeedle::rotate() // rotate needles
{
    clear();
    
    canvas.rotate();
    message.setVisible(false);
    
    show();
}

void BuffonsNeedle::count() // update message
{
    clear();
    
    message.setVisible(true);
    message.update(canvas.getCrossCount(), canvas.getNeedleCount());
    
    show();
}

void BuffonsNeedle::clear()
{
    canvas.hide();
    message.hide(*this);
}

void BuffonsNeedle::show()
{
    canvas.show();
    message.show(*this);
    redraw();
}

void BuffonsNeedle::menu_pressed()
{
    menu_button.hide();
    motion_menu.show();
}

int main()
{
    srand(time(0));
    BuffonsNeedle window{ Point {300, 300}, 800, 600, "Buffons Needle" };
    return gui_main();
}

