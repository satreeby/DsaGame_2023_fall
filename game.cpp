#include "God.h"
#include "vision.h"
#include <thread>
#include <chrono>
#include <mutex>
#include <SFML/Audio.hpp>

/*多线程*/

std::mutex mtx;

int idodi;
int num1 = 0;
int num2 = 0;
int result=0;


void Ending(int mode, sf::RenderWindow& window,  God& game, sf::Sprite& player1, sf:: Sprite& player2){
    window.create(sf::VideoMode(1200, 1000), "game");
    sf::Event event;
    sf::Font font;
    if (!font.loadFromFile("C:\\project\\c++\\pjGame\\deudhora.ttf")) {
        // 如果加载字体失败，输出错误信息并退出
        std::cerr << "Failed to load font" << std::endl;
        return;
    }
    sf::Text text_headline;
    text_headline.setFont(font); // 设置字体
    text_headline.setCharacterSize(100); // 设置字符大小
    text_headline.setString("Win!"); // 设置显示的文本
    text_headline.setFillColor(sf::Color::White); // 设置填充颜色
    sf::Color textColor = text_headline.getFillColor();
    textColor.a = 255; // 设置透明度，范围是0-255
    text_headline.setFillColor(textColor);
    sf::FloatRect textRect_headline = text_headline.getLocalBounds();
    text_headline.setOrigin(textRect_headline.left + textRect_headline.width / 2.0f, textRect_headline.top + textRect_headline.height / 2.0f);
    text_headline.setPosition(sf::Vector2f(window.getSize().x / 2.0f+200, window.getSize().y / 2.0f));

    player1.setPosition(400,400);
    player2.setPosition(400,400);
    int winner;
    if(game.IsFinished()==1)
        winner = 1;
    else if(game.IsFinished()==2)
        winner = 2;   
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                exit(1);
            }
            sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
            if (text_headline.getGlobalBounds().contains(mousePosition)) {
                text_headline.setFillColor(sf::Color::Yellow);
                // 检查鼠标左键点击
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    exit(1);
                }
            }
            else {
                text_headline.setFillColor(sf::Color::White);
            }
        }
        window.clear();
        window.draw(text_headline);
        if (winner==1)
        {
            window.draw(player1);
        }
        else
        {
            window.draw(player2);
        }
        window.draw(text_headline);
        window.display();
    }
}

void Gamefresh(sf::RenderWindow& window, God& game) {
    while (true) {
        mtx.lock();
        game.Fresh();
        mtx.unlock();
        if ((result=game.IsFinished()) != 0)
            return;
        // 等待主线程一段时间，模拟异步事件的执行
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

}

void Welcome(sf::RenderWindow& window) {
    //窗口
    sf::Event event;
    //字体
    sf::Font font;
    if (!font.loadFromFile("C:\\project\\c++\\pjGame\\deudhora.ttf")) {
        // 如果加载字体失败，输出错误信息并退出
        std::cerr << "Failed to load font" << std::endl;
        return;
    }
    sf::Text text_headline;
    text_headline.setFont(font); // 设置字体
    text_headline.setString("Welcome to TinderGame!"); // 设置显示的文本
    text_headline.setCharacterSize(100); // 设置字符大小
    text_headline.setFillColor(sf::Color::White); // 设置填充颜色
    sf::FloatRect textRect_headline = text_headline.getLocalBounds();
    text_headline.setOrigin(textRect_headline.left + textRect_headline.width / 2.0f, textRect_headline.top + textRect_headline.height / 2.0f);
    text_headline.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));
    //运行
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
                exit(1);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) 
            {
                return;
            }
            sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
            if (text_headline.getGlobalBounds().contains(mousePosition)) {
                text_headline.setFillColor(sf::Color::Yellow);
                // 检查鼠标左键点击
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    return;
                }
            }
            else {
                // 鼠标不在文本区域内，恢复默认颜色
                text_headline.setFillColor(sf::Color::White);
            }
        }
        window.clear();
        window.draw(text_headline);
        window.display();
    }
}

int mode_choose(sf::RenderWindow& window) {
    //窗口
    sf::Event event;
    //字体
    sf::Font font;
    if (!font.loadFromFile("C:\\project\\c++\\pjGame\\deudhora.ttf")) {
        // 如果加载字体失败，输出错误信息并退出
        std::cerr << "Failed to load font" << std::endl;
        exit(1);
    }
    //"Play with Computer"
    sf::Text text_mode1;
    text_mode1.setFont(font); // 设置字体
    text_mode1.setString("Play with Computer"); // 设置显示的文本
    text_mode1.setCharacterSize(100); // 设置字符大小
    text_mode1.setFillColor(sf::Color::White); // 设置填充颜色
    sf::FloatRect textRect_mode1 = text_mode1.getLocalBounds();
    text_mode1.setOrigin(textRect_mode1.left + textRect_mode1.width / 2.0f, textRect_mode1.top + textRect_mode1.height / 2.0f);
    text_mode1.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 4.0f));
    //"2 Players "
    sf::Text text_mode2;
    text_mode2.setFont(font); // 设置字体
    text_mode2.setString("two Players "); // 设置显示的文本
    text_mode2.setCharacterSize(100); // 设置字符大小
    text_mode2.setFillColor(sf::Color::White); // 设置填充颜色
    sf::FloatRect textRect_mode2 = text_mode2.getLocalBounds();
    text_mode2.setOrigin(textRect_mode2.left + textRect_mode2.width / 2.0f, textRect_mode2.top + textRect_mode2.height / 2.0f);
    text_mode2.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 4.0f * 3.0f));
    //运行
    int mode = 0;
    int nextmode = 1;
    while (window.isOpen())
    {

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                exit(1);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }
            sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
            if (text_mode1.getGlobalBounds().contains(mousePosition)) {
                text_mode1.setFillColor(sf::Color::Yellow);
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    return 0;
                }
            }
            else if (text_mode2.getGlobalBounds().contains(mousePosition)) {
                text_mode2.setFillColor(sf::Color::Yellow);
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    return 1;
                }
            }
            else {
                text_mode1.setFillColor(sf::Color::White);
                text_mode2.setFillColor(sf::Color::White);
            }
        }
        window.clear();
        window.draw(text_mode1);
        window.draw(text_mode2);
        window.display();
    }
}





int main(){
    sf::RenderWindow window(sf::VideoMode(1200, 1000), "game");

    
    sf::Music music;
    if (!music.openFromFile("C://project//c++//pjGame//kuai.ogg")) {
        // 处理加载失败的情况
        return -1;
    }

    // 设置音乐循环播放
    music.setLoop(true);

    music.play();

    //欢迎
    Welcome(std::ref(window));
    int mode = mode_choose(std::ref(window));

    sf::Font font;
    if (!font.loadFromFile("C:\\project\\c++\\pjGame\\deudhora.ttf")) {
        // 如果加载字体失败，输出错误信息并退出
        std::cerr << "Failed to load font" << std::endl;
        return 0;
    }

    God game;
    
    /*下一个士兵的图像和位置*/
    sf::Texture texture_1_ap;
    sf::Sprite sprite_1_ap;
    texture_1_ap.loadFromFile("C:\\project\\c++\\pjGame\\icon\\hooded_blue.png");
    sprite_1_ap.setScale(0.3f, 0.3f);
    sprite_1_ap.setPosition(1000, 100);
    sprite_1_ap.setTexture(texture_1_ap);
    
    sf::Texture texture_1_adc;
    sf::Sprite sprite_1_adc;
    texture_1_adc.loadFromFile("C:\\project\\c++\\pjGame\\icon\\bowman_blue.png");
    sprite_1_adc.setScale(0.3f, 0.3f);
    sprite_1_adc.setPosition(1000, 100);
    sprite_1_adc.setTexture(texture_1_adc);
    
    sf::Texture texture_1_ad;
    sf::Sprite sprite_1_ad;
    texture_1_ad.loadFromFile("C:\\project\\c++\\pjGame\\icon\\ninja_blue.png");
    sprite_1_ad.setScale(0.3f, 0.3f);
    sprite_1_ad.setPosition(1000, 100);
    sprite_1_ad.setTexture(texture_1_ad);
    
    sf::Texture texture_2_ap;
    sf::Sprite sprite_2_ap;
    texture_2_ap.loadFromFile("C:\\project\\c++\\pjGame\\icon\\hooded_red.png");
    sprite_2_ap.setScale(0.3f, 0.3f);
    sprite_2_ap.setPosition(1000, 700);
    sprite_2_ap.setTexture(texture_2_ap);
    
    sf::Texture texture_2_adc;
    sf::Sprite sprite_2_adc;
    texture_2_adc.loadFromFile("C:\\project\\c++\\pjGame\\icon\\bowman_red.png");
    sprite_2_adc.setScale(0.3f, 0.3f);
    sprite_2_adc.setPosition(1000, 700);
    sprite_2_adc.setTexture(texture_2_adc);
    
    sf::Texture texture_2_ad;
    sf::Sprite sprite_2_ad;
    texture_2_ad.loadFromFile("C:\\project\\c++\\pjGame\\icon\\ninja_red.png");
    sprite_2_ad.setScale(0.3f, 0.3f);
    sprite_2_ad.setPosition(1000, 700);
    sprite_2_ad.setTexture(texture_2_ad);

    /*设置三种兵营以及其坐标*/
    SoldierFactory* apfactory = new APFactory();
    SoldierFactory* adfactory = new ADFactory();
    SoldierFactory* adcfactory = new ADCFactory();
    Point leftpoint_1(141, 200);
    Point midpoint_1(400, 345);
    Point rightpoint_1(659, 200);
    Point leftpoint_2(141, 800);
    Point midpoint_2(400, 655);
    Point rightpoint_2(659, 800);

    /*玩家头像*/
    sf::Texture texture_player1;
    sf::Sprite sprite_player1;
    texture_player1.loadFromFile("C:\\project\\c++\\pjGame\\icon\\friend.jpg");
    sprite_player1.setScale(0.3f, 0.3f);
    sprite_player1.setPosition(800, 100);
    sprite_player1.setTexture(texture_player1);
    sf::Texture texture_player2;
    sf::Sprite sprite_player2;
    texture_player2.loadFromFile("C:\\project\\c++\\pjGame\\icon\\me.jpg");
    sprite_player2.setScale(0.3f, 0.3f);
    sprite_player2.setPosition(800, 700);
    sprite_player2.setTexture(texture_player2);

    /*文字信息*/
    sf::Text text_vs;
    text_vs.setFont(font); // 设置字体
    text_vs.setString("VS"); // 设置显示的文本
    text_vs.setCharacterSize(100); // 设置字符大小
    text_vs.setFillColor(sf::Color::Yellow); // 设置填充颜色
    sf::FloatRect textRect_vs = text_vs.getLocalBounds();
    text_vs.setOrigin(textRect_vs.left + textRect_vs.width / 2.0f, textRect_vs.top + textRect_vs.height / 2.0f);
    text_vs.setPosition(900,500);

    /*加载地图*/
    sf::Texture texture_map;
    sf::Sprite sprite_map;
    if (!texture_map.loadFromFile("C:\\project\\c++\\pjGame\\icon\\background.png")) {
        return -1;
    }
    sprite_map.setTexture(texture_map);
    sprite_map.setPosition(75,150);

    /*兵营*/
    sf_vision* sf1=new sf_vision(1, 110, 160);
    sf_vision* sf2=new sf_vision(1, 620, 160);
    sf_vision* sf3=new sf_vision(1, 110, 160);
    sf_vision* sf4=new sf_vision(2, 110, 750);
    sf_vision* sf5=new sf_vision(2, 620, 750);
    sf_vision* sf6=new sf_vision(2, 110, 160);
    

    /*设置障碍物*/
    ObstacleFactory* mountfactory = new MountFactory();
    ObstacleFactory* riverfactory = new RiverFactory();
    Obstacle* mount1 = mountfactory->createObstale(*(new Point(120, 325)), 65);
    Obstacle* mount2 = mountfactory->createObstale(*(new Point(680, 325)), 65);
    Obstacle* river1 = riverfactory->createObstale(*(new Point(180, 500)), 65);
    Obstacle* river2 = riverfactory->createObstale(*(new Point(570, 500)), 65);
    Obstacle* mount4 = mountfactory->createObstale(*(new Point(120, 675)), 65);
    Obstacle* mount5 = mountfactory->createObstale(*(new Point(680, 675)), 65);
    river2->sprite.setPosition(600 - 0.5 * 0.22f * river2->texture.getSize().x, 500 - 0.5 * 0.22f * river2->texture.getSize().y);
    river1->sprite.setPosition(220 - 0.5 * 0.22f * river2->texture.getSize().x, 500 - 0.5 * 0.22f * river2->texture.getSize().y);
    mount1->sprite.setPosition(160 - 0.5 * 0.22f * river2->texture.getSize().x, 325 - 0.5 * 0.22f * river2->texture.getSize().y);
    mount4->sprite.setPosition(160 - 0.5 * 0.22f * river2->texture.getSize().x, 675 - 0.5 * 0.22f * river2->texture.getSize().y);
    mount2->sprite.setPosition(670 - 0.5 * 0.22f * river2->texture.getSize().x, 325 - 0.5 * 0.22f * river2->texture.getSize().y);
    mount5->sprite.setPosition(670 - 0.5 * 0.22f * river2->texture.getSize().x, 675 - 0.5 * 0.22f * river2->texture.getSize().y);
    game.AddMount(mount1);
    game.AddMount(mount2);
    game.AddMount(river1);
    game.AddMount(river2);
    game.AddMount(mount4);
    game.AddMount(mount5);


    std::chrono::steady_clock::time_point lastClickTime_player1;
    std::chrono::steady_clock::time_point lastClickTime_player2;
    
    std::thread gamefreshThread(Gamefresh, std::ref(window), std::ref(game));
    
    /*与电脑对战*/
    if (mode == 0)
    {
        sf::Event event;
        lastClickTime_player2 = std::chrono::steady_clock::now();
        while (window.isOpen()  && result==0) 
        {
            auto currentTime = std::chrono::steady_clock::now();
            auto elapsedTime_player1 = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastClickTime_player1);
            auto elapsedTime_player2 = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastClickTime_player2);
            if (elapsedTime_player1.count() > 2 && game.team1.IsFull() == false)
                {
                    Point p;
                    unsigned seed;
                    seed = time(0);
                    srand(seed);
                    switch (rand()%3)
                    {
                    case 0:
                        p = leftpoint_1;
                        break;
                    case 1:
                        p = midpoint_1;
                        break;
                    case 2:
                        p = rightpoint_1;
                        break;
                    default:
                        break;
                    }
                    switch (rand() % 3)
                    {
                    case 0:
                        mtx.lock();
                        game.AddSoldier(1, apfactory->createSoldier(1, num1++, p));
                        mtx.unlock();
                        break;
                    case 1:
                        mtx.lock();
                        game.AddSoldier(1, adcfactory->createSoldier(1, num1++, p));
                        mtx.unlock();
                        break;
                    case 2:
                        mtx.lock();
                        game.AddSoldier(1, adfactory->createSoldier(1, num1++, p));
                        mtx.unlock();
                        break;
                    default:
                        break;
                    }
                    lastClickTime_player1 = currentTime;
                }
            while (window.pollEvent(event)) 
            {
                if (event.type == sf::Event::Closed) 
                {
                    window.close();
                    exit(1);
                }
                if (event.type == sf::Event::Closed)
                    window.close();
                if (elapsedTime_player2.count() > 2 && game.team2.IsFull() == false) {
                    // 点击按钮生成新的士兵
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                    {
                        switch (num2 % 3)
                        {
                        case 0:
                            mtx.lock();
                            game.AddSoldier(2, apfactory->createSoldier(2, num2++, leftpoint_2));
                            mtx.unlock();
                            break;
                        case 1:
                            mtx.lock();
                            game.AddSoldier(2, adcfactory->createSoldier(2, num2++, leftpoint_2));
                            mtx.unlock();
                            break;
                        case 2:
                            mtx.lock();
                            game.AddSoldier(2, adfactory->createSoldier(2, num2++, leftpoint_2));
                            mtx.unlock();
                            break;
                        default:
                            break;
                        }
                        lastClickTime_player2 = currentTime;
                    }
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                    {
                        switch (num2 % 3)
                        {
                        case 0:
                            mtx.lock();
                            game.AddSoldier(2, apfactory->createSoldier(2, num2++, midpoint_2));
                            mtx.unlock();
                            break;
                        case 1:
                            mtx.lock();
                            game.AddSoldier(2, adcfactory->createSoldier(2, num2++, midpoint_2));
                            mtx.unlock();
                            break;
                        case 2:
                            mtx.lock();
                            game.AddSoldier(2, adfactory->createSoldier(2, num2++, midpoint_2));
                            mtx.unlock();
                            break;
                        default:
                            break;
                        }
                        lastClickTime_player2 = currentTime;
                    }
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                    {
                        switch (num2 % 3)
                        {
                        case 0:
                            mtx.lock();
                            game.AddSoldier(2, apfactory->createSoldier(2, num2++, rightpoint_2));
                            mtx.unlock();
                            break;
                        case 1:
                            mtx.lock();
                            game.AddSoldier(2, adcfactory->createSoldier(2, num2++, rightpoint_2));
                            mtx.unlock();
                            break;
                        case 2:
                            mtx.lock();
                            game.AddSoldier(2, adfactory->createSoldier(2, num2++, rightpoint_2));
                            mtx.unlock();
                            break;
                        default:
                            break;
                        }
                        lastClickTime_player2 = currentTime;
                    }

                }
            }

            // 清空窗口
            window.clear();
            // 在窗口中绘制按钮
            if (elapsedTime_player2.count() > 2 && game.team2.IsFull()==false)
            {
                switch (num2 % 3)
                {
                case 0:
                    window.draw(sprite_2_ap);
                    break;
                case 1:
                    window.draw(sprite_2_adc);
                    break;
                case 2:
                    window.draw(sprite_2_ad);
                    break;
                default:
                    break;
                }
            }
            //绘制头像
            window.draw(sprite_player1);
            window.draw(sprite_player2);
            window.draw(text_vs);
            //绘制地图
            window.draw(sprite_map);
            //绘制兵营
            sf1->draw(std::ref(window));
            sf2->draw(std::ref(window));
            sf4->draw(std::ref(window));
            sf5->draw(std::ref(window));
            // 绘制障碍物
            for (int i = 0; i <= game.things.getTop(); i++) {
                game.things.visit(i)->draw(std::ref(window));
            }
            // 绘制基地
            game.inhibt1.draw(std::ref(window));
            game.inhibt2.draw(std::ref(window));
            // 绘制士兵
            for (int i = 0; i <= game.team1.getTop(); i++)
                game.team1.visit(i)->draw(std::ref(window));
            for (int i = 0; i <= game.team2.getTop(); i++)
                game.team2.visit(i)->draw(std::ref(window));
            // 显示绘制的内容
            window.display();
        }
    }
    
    
    
    /*双人对战*/
    else if (mode == 1)
    {
        sf::Event event;
        lastClickTime_player1 = std::chrono::steady_clock::now();
        lastClickTime_player2 = std::chrono::steady_clock::now();
        while (window.isOpen() && result==0) {
            auto currentTime = std::chrono::steady_clock::now();
            auto elapsedTime_player1 = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastClickTime_player1);
            auto elapsedTime_player2 = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastClickTime_player2);
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                    exit(1);
                }
                if (event.type == sf::Event::Closed)
                    window.close();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                    window.close();
                }
                if (elapsedTime_player1.count() > 2 && game.team1.getTop() < N2 - 1) {
                    // 点击按钮生成新的士兵
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp))
                    {
                        switch (num1 % 3)
                        {
                        case 0:
                            mtx.lock();
                            game.AddSoldier(1, apfactory->createSoldier(1, num1++, leftpoint_1));
                            mtx.unlock();
                            break;
                        case 1:
                            mtx.lock();
                            game.AddSoldier(1, adcfactory->createSoldier(1, num1++, leftpoint_1));
                            mtx.unlock();
                            break;
                        case 2:
                            mtx.lock();
                            game.AddSoldier(1, adfactory->createSoldier(1, num1++, leftpoint_1));
                            mtx.unlock();
                            break;
                        default:
                            break;
                        }
                        lastClickTime_player1 = currentTime;
                    }
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                    {
                        switch (num1 % 3)
                        {
                        case 0:
                            mtx.lock();
                            game.AddSoldier(1, apfactory->createSoldier(1, num1++, midpoint_1));
                            mtx.unlock();
                            break;
                        case 1:
                            mtx.lock();
                            game.AddSoldier(1, adcfactory->createSoldier(1, num1++, midpoint_1));
                            mtx.unlock();
                            break;
                        case 2:
                            mtx.lock();
                            game.AddSoldier(1, adfactory->createSoldier(1, num1++, midpoint_1));
                            mtx.unlock();
                            break;
                        default:
                            break;
                        }
                        lastClickTime_player1 = currentTime;
                    }
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown))
                    {
                        switch (num1 % 3)
                        {
                        case 0:
                            mtx.lock();
                            game.AddSoldier(1, apfactory->createSoldier(1, num1++, rightpoint_1));
                            mtx.unlock();
                            break;
                        case 1:
                            mtx.lock();
                            game.AddSoldier(1, adcfactory->createSoldier(1, num1++, rightpoint_1));
                            mtx.unlock();
                            break;
                        case 2:
                            mtx.lock();
                            game.AddSoldier(1, adfactory->createSoldier(1, num1++, rightpoint_1));
                            mtx.unlock();
                            break;
                        default:
                            break;
                        }
                        lastClickTime_player1 = currentTime;
                    }
                }
                if (elapsedTime_player2.count() > 2 && game.team2.getTop() < N2 - 1) {
                    // 点击按钮生成新的士兵
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                    {
                        switch (num2 % 3)
                        {
                        case 0:
                            mtx.lock();
                            game.AddSoldier(2, apfactory->createSoldier(2, num2++, leftpoint_2));
                            mtx.unlock();
                            break;
                        case 1:
                            mtx.lock();
                            game.AddSoldier(2, adcfactory->createSoldier(2, num2++, leftpoint_2));
                            mtx.unlock();
                            break;
                        case 2:
                            mtx.lock();
                            game.AddSoldier(2, adfactory->createSoldier(2, num2++, leftpoint_2));
                            mtx.unlock();
                            break;
                        default:
                            break;
                        }
                        lastClickTime_player2 = currentTime;
                    }
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                    {
                        switch (num2 % 3)
                        {
                        case 0:
                            mtx.lock();
                            game.AddSoldier(2, apfactory->createSoldier(2, num2++, midpoint_2));
                            mtx.unlock();
                            break;
                        case 1:
                            mtx.lock();
                            game.AddSoldier(2, adcfactory->createSoldier(2, num2++, midpoint_2));
                            mtx.unlock();
                            break;
                        case 2:
                            mtx.lock();
                            game.AddSoldier(2, adfactory->createSoldier(2, num2++, midpoint_2));
                            mtx.unlock();
                            break;
                        default:
                            break;
                        }
                        lastClickTime_player2 = currentTime;
                    }
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                    {
                        switch (num2 % 3)
                        {
                        case 0:
                            mtx.lock();
                            game.AddSoldier(2, apfactory->createSoldier(2, num2++, rightpoint_2));
                            mtx.unlock();
                            break;
                        case 1:
                            mtx.lock();
                            game.AddSoldier(2, adcfactory->createSoldier(2, num2++, rightpoint_2));
                            mtx.unlock();
                            break;
                        case 2:
                            mtx.lock();
                            game.AddSoldier(2, adfactory->createSoldier(2, num2++, rightpoint_2));
                            mtx.unlock();
                            break;
                        default:
                            break;
                        }
                        lastClickTime_player2 = currentTime;
                    }
                }
            }

            // 清空窗口
            window.clear();

            // 在窗口中绘制按钮和下一个能产生的士兵
            if (elapsedTime_player1.count() > 2 && game.team1.getTop() < N2 - 1)
            {
                switch (num1 % 3)
                {
                case 0:
                    window.draw(sprite_1_ap);
                    break;
                case 1:
                    window.draw(sprite_1_adc);
                    break;
                case 2:
                    window.draw(sprite_1_ad);
                    break;
                default:
                    break;
                }
            }
            if (elapsedTime_player2.count() > 2 && game.team2.getTop() < N2 - 1)
            {
                switch (num2 % 3)
                {
                case 0:
                    window.draw(sprite_2_ap);
                    break;
                case 1:
                    window.draw(sprite_2_adc);
                    break;
                case 2:
                    window.draw(sprite_2_ad);
                    break;
                default:
                    break;
                }
            }
            //绘制头像
            window.draw(sprite_player1);
            window.draw(sprite_player2);
            window.draw(text_vs);
            //绘制地图
            window.draw(sprite_map);
            //绘制兵营
            sf1->draw(std::ref(window));
            sf2->draw(std::ref(window));
            sf4->draw(std::ref(window));
            sf5->draw(std::ref(window));
            // 绘制障碍物
            for (int i = 0; i <= game.things.getTop(); i++) {
                game.things.visit(i)->draw(std::ref(window));
            }
            // 绘制基地
            game.inhibt1.draw(std::ref(window));
            game.inhibt2.draw(std::ref(window));
            // 绘制士兵
            for (int i = 0; i <= game.team1.getTop(); i++) {
                game.team1.visit(i)->draw(std::ref(window));
            }
            for (int i = 0; i <= game.team2.getTop(); i++) {
                game.team2.visit(i)->draw(std::ref(window));
            }

            // 显示绘制的内容
            window.display();

        }
    }

    Ending(mode, std::ref(window), std::ref(game), std::ref(sprite_player1), std::ref(sprite_player2));
}

/*
7.规则、英雄、说明页
8.代码优化
*/