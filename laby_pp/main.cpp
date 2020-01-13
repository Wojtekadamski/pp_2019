#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

struct Point {
    float x;
    float y;
};

struct PolarCoord
{
  double alpha;
  double d;
};
struct ScreenInfo {
    int width;
    int height;

    float xmin, xmax;
    float ymin, ymax;
};

Point mat2screen(const Point &p, const ScreenInfo &info) {
    Point rv;

    rv.x = info.width / (info.xmax - info.xmin) * p.x  - info.width * info.xmin / (info.xmax - info.xmin);
    rv.y = info.height / (info.ymin - info.ymax) * p.y - info.height * info.ymax / (info.ymin - info.ymax);
    return rv;
}

sf::VertexArray getMatAxes(const ScreenInfo &info) {
    Point p1 { info.xmin, 0 };
    Point p2 { info.xmax, 0 };
    Point p3 { 0, info.ymin };
    Point p4 { 0, info.ymax };
    Point pe;

    sf::VertexArray mat(sf::Lines);

    pe = mat2screen(p1, info);
    mat.append(sf::Vector2f(pe.x, pe.y));
    pe = mat2screen(p2, info);
    mat.append(sf::Vector2f(pe.x, pe.y));
    pe = mat2screen(p3, info);
    mat.append(sf::Vector2f(pe.x, pe.y));
    pe = mat2screen(p4, info);
    mat.append(sf::Vector2f(pe.x, pe.y));

    return mat;
}

sf::VertexArray getFunctionPoints(const ScreenInfo &info) {
    sf::VertexArray points(sf::Points);
    Point pm, pe;
    // dx dobrane tak, ¿eby obliczyæ wartoœæ funkcji dla ka¿dego punktu na ekranie
    // mo¿na gêœciej
    float dx = (info.xmax - info.xmin)/info.width;
    for (pm.x = info.xmin; pm.x < info.xmax; pm.x += dx)
    {
        pm.y = sin(pm.x);
        pe = mat2screen(pm, info);
        points.append(sf::Vector2f(pe.x, pe.y));
    }
    return points;
}


// konwersja ze wspó³rzêdnych biegunowych na kartezjañskie
Point PolarToCart (const PolarCoord & polar){
    Point cart;
    cart.x = polar.d * cos(polar.alpha);
    cart.y = polar.d * sin(polar.alpha);
}

sf::VertexArray generateCardioid (const ScreenInfo & screenInfo, double a, double thStart, double thEnd, double thStep){
    sf::VertexArray rv(sf::LinesStrip);
    PolarCoord pol;
    Point crt;
    Point scr;

    for (pol.alpha = thStart; pol.alpha < thEnd; pol.alpha += thStep)
    {
        // obliczamy wartoœæ funkcji we wsp. biegunowych
        pol.d = a * (1 - cos (pol.alpha));

        // konwertujemy na kartezjañskie
        crt = PolarToCart(pol);

        // konwertujemy na ekranowe
        scr = mat2screen(crt, screenInfo);

        // dodajemy wspó³rzêdne ekranowe do wektora
        rv.append(sf::Vector2f(scr.x, scr.y));
    }

    return rv;
}



int main(){
    const int width = 800;
    const int height = 600;
    ScreenInfo screenInfo { width, height, -10, 10, -8, 8 };

    // Create the main window
    sf::RenderWindow app(sf::VideoMode(width, height), "SFML window");

    sf::VertexArray cardio = generateCardioid(screenInfo, 200, 5, 10*M_PI, 0.1);

    // Start the game loop
    while (app.isOpen())
    {
        // Process events
        sf::Event event;
        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                app.close();
        }

        // Clear screen
        app.clear();

        // Draw the sprite
        app.draw(cardio);

        // Update the window
        app.display();
    }

    return EXIT_SUCCESS;
}

/*int main(int argc, char **argv) {
    ScreenInfo info { 800, 600, -10, 10, -1.5, 1.5 };
    sf::RenderWindow win(sf::VideoMode(info.width, info.height), "foobar");

    sf::VertexArray mat = getMatAxes(info);
    sf::VertexArray fun = getFunctionPoints(info);

    while (win.isOpen()) {
        sf::Event event;
        while (win.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                win.close();
        }

        // clear the window with black color
        win.clear(sf::Color::Black);

        // draw everything here...
        win.draw(mat);
        win.draw(fun);

        // end the current frame
        win.display();
    }

    return 0;
}*/
