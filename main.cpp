#include <SFML/Graphics.hpp>
#include <list>
#include <iostream>
#include <math.h>
#include <unistd.h>

#define PI 3.14159265359
#define RESOLUTION_X 900
#define RESOLUTION_Y 900

struct object
{
        sf::Vector2f    position;
        sf::Vector2f    velocity;
        float           radius;
        float           mass;
        sf::CircleShape shape;
        sf::RenderWindow *window;

        object(sf::Vector2f pos, sf::Vector2f vel,
               float rad, sf::RenderWindow *win) {
                position = pos;
                velocity = vel;
                radius   = rad;
                mass     = PI * rad * rad;
                shape    = sf::CircleShape(rad);
                shape.setFillColor(sf::Color::Blue);
                window   = win;

        };

        void draw() {
                shape.setPosition(position.x-radius, position.y-radius);
                window->draw(shape);
        }
};

double sqr(double x) {return x * x;}

bool check_colision(sf::Vector2f pos_1, double rad_1, sf::Vector2f pos_2, double rad_2) {
        bool res = sqr(pos_1.x - pos_2.x) + sqr(pos_1.y - pos_2.y) < sqr(rad_1 + rad_2);
        // std::cout <<  pos_1.x << " " << pos_1.y << std::endl;
        // std::cout <<  pos_2.x << " " << pos_2.y << std::endl;
        // std::cout <<  res << std::endl;
        return res;
}

sf::Vector2f calculate_velacity(sf::Vector2f v_1, double m_1, sf::Vector2f v_2, double m_2) {
        sf::Vector2f v_end;
        v_end.x = (2 * m_2 * v_2.x + (m_1 - m_2) * v_1.x) / (m_1 + m_2);
        v_end.y = (2 * m_2 * v_2.y + (m_1 - m_2) * v_1.y) / (m_1 + m_2);
        return v_end;
}

float DotProduct(sf::Vector2f rv, sf::Vector2f normal) {
        return (rv.x * normal.x + rv.y * normal.y) / sqrt(sqr(normal.x) + sqr(normal.y));
}

void calculate_velacity_2(auto A, auto B) {
        // Вычисляем относительную скорость
        sf::Vector2f rv = B->velocity - A->velocity;
        sf::Vector2f normal = B->position - A->position;
        float normal_k = sqrt(sqr(normal.x) + sqr(normal.y));
        normal.x /= normal_k;
        normal.y /= normal_k;

        // Вычисляем относительную скорость относительно направления нормали
        float velAlongNormal = DotProduct(rv, normal);

        // Не выполняем вычислений, если скорости разделены
        if(velAlongNormal > 0) {
                return;
        }

        // Вычисляем упругость
        float e = 1;

        // Вычисляем скаляр импульса силы
        float j = -(1 + e) * velAlongNormal;
        j /= 1 / A->mass + 1 / B->mass;

        // Прикладываем импульс силы
        sf::Vector2f impulse = j * normal;
        A->velocity -= 1 / A->mass * impulse;
        B->velocity += 1 / B->mass * impulse;

        std::cout << "velAlongNormal: " << velAlongNormal << std::endl;
        std::cout << "impulse" << impulse.x << std::endl;
        std::cout << "impulse" << impulse.y << std::endl;
        std::cout << "Av x: " << A->velocity.x << std::endl;
        std::cout << "Av y: " << A->velocity.y << std::endl;
        std::cout << "Bv x: " << B->velocity.x << std::endl;
        std::cout << "Bv y: " << B->velocity.y << std::endl;
}
int main()
{
        sf::RenderWindow window(sf::VideoMode(RESOLUTION_X, RESOLUTION_Y),
                                "Phisics test");
        window.setVerticalSyncEnabled(true);

        sf::Vector2f borders_indent(50, 50);
        sf::Vector2f borders_size(800, 800);
        // area of simulation
        sf::RectangleShape bordersShape(borders_size);
        bordersShape.setFillColor(sf::Color::Transparent);
        bordersShape.setOutlineThickness(4);
        bordersShape.setOutlineColor(sf::Color::Blue);
        bordersShape.setPosition(borders_indent);

        // create and fill list of objects
        std::list<object> object_list;
        std::list<object>::iterator object_iterator;
        std::list<object>::iterator object_iterator_2;
        object_list.push_back(object(sf::Vector2f(276, 400), sf::Vector2f(0, 0), 30, &window));
        object_list.push_back(object(sf::Vector2f(338, 400), sf::Vector2f(0, 0), 30, &window));
        object_list.push_back(object(sf::Vector2f(400, 400), sf::Vector2f(0, 0), 30, &window));
        object_list.push_back(object(sf::Vector2f(462, 400), sf::Vector2f(0, 0), 30, &window));
        object_list.push_back(object(sf::Vector2f(524, 400), sf::Vector2f(0, 0), 30, &window));

        object_list.push_back(object(sf::Vector2f(307, 454), sf::Vector2f(0, 0), 30, &window));
        object_list.push_back(object(sf::Vector2f(369, 454), sf::Vector2f(0, 0), 30, &window));
        object_list.push_back(object(sf::Vector2f(431, 454), sf::Vector2f(0, 0), 30, &window));
        object_list.push_back(object(sf::Vector2f(493, 454), sf::Vector2f(0, 0), 30, &window));

        object_list.push_back(object(sf::Vector2f(338, 508), sf::Vector2f(0, 0), 30, &window));
        object_list.push_back(object(sf::Vector2f(400, 508), sf::Vector2f(0, 0), 30, &window));
        object_list.push_back(object(sf::Vector2f(462, 508), sf::Vector2f(0, 0), 30, &window));

        object_list.push_back(object(sf::Vector2f(369, 562), sf::Vector2f(0, 0), 30, &window));
        object_list.push_back(object(sf::Vector2f(431, 562), sf::Vector2f(0, 0), 30, &window));

        object_list.push_back(object(sf::Vector2f(400, 616), sf::Vector2f(0, 0), 30, &window));

        object_list.push_back(object(sf::Vector2f(400, 800), sf::Vector2f(0, 4), 30, &window));
        while (window.isOpen())
        {
                sf::Event event;
                while (window.pollEvent(event))
                {
                        if (event.type == sf::Event::Closed)
                        window.close();
                }

                window.clear(sf::Color::White);
                for (object_iterator=object_list.begin();
                     object_iterator!=object_list.end();
                     object_iterator++) {
                        // calculate position
                        object_iterator->position += object_iterator->velocity;
                        // border collision
                        if (object_iterator->position.x > borders_size.x + borders_indent.x - object_iterator->radius or
                            object_iterator->position.x < borders_indent.x + object_iterator->radius) {
                                object_iterator->velocity.x *= (-1);
                        }
                        if (object_iterator->position.y > borders_size.y + borders_indent.y - object_iterator->radius or
                            object_iterator->position.y < borders_indent.y + object_iterator->radius) {
                                    object_iterator->velocity.y *= (-1);
                        }
                        // detect colision between objects
                        for (object_iterator_2=object_iterator;
                             object_iterator_2!=object_list.end();
                             object_iterator_2++) {
                                if (object_iterator_2 == object_iterator) {
                                        continue;
                                }
                                if (check_colision(object_iterator->position, object_iterator->radius,
                                                   object_iterator_2->position, object_iterator_2->radius)) {
                                        std::cout << std::endl;
                                        std::cout << "start to calculate";
                                        std::cout << std::endl;
                                        // calculate colision
                                        // sf::Vector2f tmp_1 = calculate_velacity(object_iterator->velocity,
                                        //                                         object_iterator->mass,
                                        //                                         object_iterator_2->velocity,
                                        //                                         object_iterator_2->mass);
                                        // sf::Vector2f tmp_2 = calculate_velacity(object_iterator_2->velocity,
                                        //                                         object_iterator_2->mass,
                                        //                                         object_iterator->velocity,
                                        //                                         object_iterator->mass);
                                        // object_iterator->velocity = tmp_1;
                                        // object_iterator_2->velocity = tmp_2;
                                        calculate_velacity_2(object_iterator, object_iterator_2);
                                }
                        }

                        // draw results
                        object_iterator->draw();
                };
                window.draw(bordersShape);
                window.display();
        }

    return 0;
}