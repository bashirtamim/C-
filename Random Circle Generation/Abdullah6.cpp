//Bashir Abdullah
// Random Circle Generation inside a Circle.
#include <iostream>
#include <simple_window.h>
#include <Graph.h>
#include <std_lib_facilities.h>
using namespace Graph_lib;


class CirclesRings
{
    
    const static int WINDOW_WIDTH = 500;      // window size
    const static int WINDOW_HEIGHT = 500;
    
    const static int CENTER_X = WINDOW_WIDTH/2; // position of (0,0) is center of window
    const static int CENTER_Y = WINDOW_HEIGHT/2;
    
    const static int RADIUS = 230; // radius of main circle
    
    const static char* TITLE; // window title
    const static Point WINDOW_OFFSET; // offset of up-left corner of the main window on display
    const static Point ORIGIN; // central point
    const static Point MESSAGE_POSITION; // coordinates of message output
    
    
public:
    
    CirclesRings(): window(NULL), info(NULL)
    {
        srand(time(0));
        
    }
    
    /**
     * Method to process one drawing iteration:
     *
     * 1. create main window;
     * 2. draw picture;
     * 3. wait for `Next` click;
     * 4. release resources.
     */
    void draw(int ringsCount)
    {
        window = new Simple_window(WINDOW_OFFSET, WINDOW_WIDTH, WINDOW_HEIGHT, TITLE);
        drawPicture(ringsCount);
        window->wait_for_button();
        finalize();
    }
    
private:
    
    /**
     * Method to display all required objects.
     */
    void drawPicture(int ringsCount)
    {
        // create main circle, inside wich we will draw rings of circles
        attachCircle(ORIGIN, RADIUS);
        
        // generate given number of rings of circles
        for (int ringNumber = 0; ringNumber < ringsCount; ++ringNumber)
            generateRing(ringNumber);
        
        // create message with info about generations count
        attachInfo(ringsCount);
    }
    
    //randomly generate ring of circles
    
    void generateRing(int ringNumber)
    {
        // generate random number of circles in the ring
        int circlesCount = rand()%max(ringNumber, 5) + 2;
        
        // calculate radius of each circle in the ring
        float angle = M_PI/circlesCount;
        float radius = RADIUS*sin(angle)/(1 + sin(angle));
        
        // create circles
        for (int i = 0; i < circlesCount; ++i)
            createCirlce(2*M_PI/circlesCount*i, radius);
    }
    
    //create circle with given angle and radius
    
    void createCirlce(float angle, float radius)
    {
        float dist = RADIUS-radius; // distance from origin to center of this circle
        float circleX = CENTER_X + dist*cos(angle); // x-coord of the center of this circle
        float circleY = CENTER_Y + dist*sin(angle); // y-coord of the center of this circle
        attachCircle(Point(circleX, circleY), radius);
    }
    
    // attach circle with given center and radius
    void attachCircle(const Point & center, double radius)
    {
        Circle* circle = new Circle(center, radius); // create circle
        circle->set_color(Color(rand()%14)); // random color
        window->attach(*circle); // attach to window
        circles.push_back(circle); // store to detach at the end
    }
    
    // attach text with the number of inner ring generation
    void attachInfo(int ringsCount)
    {
        char message[42];
        sprintf(message, "After %d ring%s generation", ringsCount, ringsCount == 1 ? "" : "s");
        info = new Text(MESSAGE_POSITION, string(message));
        window->attach(*info);
    }
    
    // make detach and release
    void finalize()
    {
        detachAll();
        release();
    }
    
    // detach all attached objects
    void detachAll()
    {
        for (Circle* c: circles)
            window->detach(*c);
        window->detach(*info);
    }
    
    
    //release used resources
    void release()
    {
        for (Circle* circle: circles)
            delete circle;
        vector<Circle*>().swap(circles);
        delete info;
        delete window;
    }
    
    // private fields
    Simple_window* window;
    vector<Circle*> circles;
    Text* info;
};

const char* CirclesRings::TITLE = "Rings of Circles.";
const Point CirclesRings::WINDOW_OFFSET = Point(100, 100);
const Point CirclesRings::ORIGIN = Point(CENTER_X, CENTER_Y);
const Point CirclesRings::MESSAGE_POSITION = Point(CENTER_X-80, WINDOW_HEIGHT-10);

int readNumber()
{
    cout << "Enter the round of inner circle generation: \n" << endl;
    int x;
    std::cin >> x;
    return x;
}

int main()
{
    try
    {
        CirclesRings painter;
        for (int ringsCount; (ringsCount = readNumber()) >= 0; )
            painter.draw(ringsCount);
    }
    catch (exception& e)
    {
        cerr << "exception: " << e.what() << endl;
        return 1;
    }
    catch (...)
    {
        cerr << "exception" << endl;
        return 2;
    }
}

