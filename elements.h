#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>
using namespace std;

/*宏定义棋盘边界*/
#define LEFT 75
#define RIGHT 725
#define LOW 150
#define UP 850

/*坐标*/
class Point
{
public:
    double x, y;        //笛卡尔坐标
    Point(){};
    Point(double x1, double y1){
        x=x1;
        y=y1;
    }
    ~Point(){};
    void operator=(const Point& p){
        x=p.x;
        y=p.y;
    }
    void printPoint(){
        cout << "(" << x << "," << y << ")";
    }
};

/*三个辅助函数*/
double Distance(const Point& p1, const Point& p2){
    double d=sqrt( pow((p1.x-p2.x), 2) + pow((p1.y-p2.y), 2) );
    return d;
}
double Max(double x, double y){
    return (x>=y)?x: y;
}
double Min(double x, double y){
    return (x<=y)?x: y;
}

/*辅助栈结构带游标*/
template <class T>
class Stack
{
private:
    T* s;
    int size;
    int top;
public:
    Stack(){};
    Stack(int n){
        size=n;
        s = new T[size];
        top=-1;
    }
    ~Stack(){
        delete [] s;
        s=NULL;
    };
    void push(T val){
        if (top<size-1)
            s[++top]=val;
    }
    T pop(){
        if (top>=0)
            return s[top--];
    }
    T visit(int i){
        if(i>=0 && i<=top)
            return s[i];
    }
    T remove(int i){
        if(i>=0 && i<=top){
            T temp=s[i];
            for (int j = i; j < top; j++)
                s[j]=s[j+1];
            top--;
            return temp;
        }
    }
    bool IsEmpty(){
        if (top<0)
            return true;
        return false;        
    }
    bool IsFull() {
        if (top >= size-1)
            return true;
        return false;
    }
    void Clear(){
        while (top>=0)
            pop();
    }
    int getTop(){
        return top;
    }
};

class Command {
public:
    virtual void execute() = 0;
};



/*攻击*/
class Attackable : public Command
{
private:
    double injury;                      //造成的伤害
    int valid;                          //攻击动作是否有效（或是否发起了攻击）, 0: not attack; 1: magic; 2: physical; 3: real
    int source;                         //攻击者标号
    int drain;                          //被攻击者标号
    
public:
    /*图形化*/
    
    Attackable(){
        setInjury(0);
        setValid(0);
        setSource(0);
        setDrain(0);
    };
    virtual ~Attackable(){};
    virtual void Attack()=0;
    virtual void execute(){
        Attack();
    }
    void setInjury(double val){
        injury=val;
    }
    double getInjury(){
        return injury;
    }
    void setValid(int val){
        valid=val;
    }
    int getValid(){
        return valid;
    }
    void setSource(int val){
        source=val;
    }
    int getSource(){
        return source;
    }
    void setDrain(int val){
        drain=val;
    }
    int getDrain(){
        return drain;
    }
};
class NotAttack: public Attackable
{
public:
    NotAttack(){
        setValid(0);
    };
    virtual ~NotAttack(){};
    virtual void Attack(){
        cout << "Not Attack ";
    }
};
class MagicAttack: public Attackable
{
public:
    MagicAttack(){
        setValid(1);
    };
    virtual ~MagicAttack(){};
    virtual void Attack(){
        cout << "Magic Attack ";
    }
};
class PhysicalAttack: public Attackable
{
public:
    PhysicalAttack(){
        setValid(2);
    };
    virtual ~PhysicalAttack(){};
    virtual void Attack(){
        cout << "Physical Attack ";
    }
};
class RealAttack: public Attackable
{
public:
    RealAttack(){
        setValid(3);
    };
    virtual ~RealAttack(){};
    virtual void Attack(){
        cout << "Real Attack ";
    }
};

/*移动*/
class Movable :public Command
{
private:
    Point source;       //源
    Point drain;        //目标
    double step;        //步长
public:
    Movable(){
        source.x=source.y=0;
        drain.x=drain.y=0;
        double step=0;
    };
    virtual ~Movable(){};
    virtual void Move()=0;
    virtual void execute(){
        Move();
    }
    void setSource(const Point& p){
        source=p;
    }
    Point getSource(){
        return source;
    }
    void setDrain(const Point& p){
        drain=p;
    }
    Point getDrain(){
        return drain;
    }
    void setStep(double s){
        step=s;
    }
    double getStep(){
        return step;
    }
    void forward(int sign){
        drain.x=source.x;
        drain.y=source.y+sign*step;
        /*防止超出边界*/
        drain.x = Max(drain.x, LEFT);
        drain.x = Min(drain.x, RIGHT);
        drain.y = Max(drain.y, LOW);
        drain.y = Min(drain.y, UP);
    }
    void toward(const Point& p){
        double d=Distance(source, p);
        if (d==0 || step==0)
            return;
        double l;
        if (d>step)
            l = step / d;
        else
            l = 1;
        drain.x=(p.x-source.x)*l+source.x;
        drain.y=(p.y-source.y)*l+source.y;
        /*防止超出边界*/
        drain.x = Max(drain.x, LEFT);
        drain.x = Min(drain.x, RIGHT);
        drain.y = Max(drain.y, LOW);
        drain.y = Min(drain.y, UP);
        return;
    }
    void bypass(const Point& p){
        if (p.x>source.x){
            drain.x=Max(source.x-step,LEFT);
            drain.y=source.y;
        }
        else{
            drain.x=Min(source.x+step,RIGHT);
            drain.y=source.y;
        }
    };
};
class Flash:  public Movable
{
public:
    Flash(){};
    ~Flash(){};
    virtual void Move(){
        cout << "Flash!";
    }
};

/*基地*/
class Inhibitor
{
private:
    int num;                                //1=player1 or 2=player2
    Point pos;
    double scale;
    double health;
public:
    /*基地以及血条的图形化属性*/
    sf::Texture texture;                    // 基地的纹理
    sf::Sprite sprite;                      // 基地的精灵
    sf::RectangleShape healthBarRect;       // 替换为你的血量条大小
    double ita;                             // 控制血条变化

    Inhibitor(int n, const Point& p, double r, double h, double x, double y){

        /*基本属性初始化*/
        num=n;
        pos=p;
        scale=r;
        health=h;
        
        /*基地图像属性初始化*/
        if (n==1)                           //player1=blue
            texture.loadFromFile("C:\\project\\c++\\pjGame\\icon\\occupy_blue.png");
        else                                //player2=red
            texture.loadFromFile("C:\\project\\c++\\pjGame\\icon\\occupy_red.png");
        sprite.setScale(0.34f, 0.34f);
        sprite.setPosition( p.x - 0.5*0.34f * texture.getSize().x, (n == 1) ? p.y : p.y- 0.34f * texture.getSize().y);
        sprite.setTexture(texture);

        /*血条图形化属性初始化*/
        healthBarRect.setFillColor(sf::Color::Green);
        healthBarRect.setPosition(x, y);
        healthBarRect.setSize(sf::Vector2f(650, 25));
        ita = 650 / health;

    };
    ~Inhibitor(){};
    void setHealth(int val){
        health=val;
        healthBarRect.setSize(sf::Vector2f(health * ita, 25));
    }
    double getHealth(){
        return health;
    }
    Point getPosition(){
        return pos;
    }
    double getScale(){
        return scale;
    }
    int getNum(){
        return num;
    }
    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
        window.draw(healthBarRect);
    }
};

/*障碍物*/
class Obstacle
{
private:
    Point pos;
    double scale;
public:
    sf::Texture texture;            // 障碍物的纹理
    sf::Sprite sprite;              // 障碍物的精灵

    Obstacle(){};
    virtual ~Obstacle(){};
    void setPos(const Point& p){
        pos=p;
    }
    Point getPosition(){
        return pos;
    }
    void setScale(double r){
        scale=r;
    }
    double getScale(){
        return scale;
    }
    bool IsinWay(const Point& p1, const Point& p2){
        if (Distance(p1, pos)<scale || Distance(p2, pos)<scale)
            return true;
        if (p1.x==p2.x)
            if (pos.x+scale>=p1.x && pos.x-scale<=p1.x && (pos.y-p1.y)*(pos.y-p2.y)<0)
                return true;
            else
                return false;      
        else if(p1.y==p2.y)
            if (pos.y+scale>=p1.y && pos.y-scale<=p1.y && (pos.x-p1.x)*(pos.x-p2.x)<0)
                return true;
            else
                return false;
        else
        {
            Point p;
            p.x=( (p2.y-p1.y)/(p2.x-p1.x)*p1.x+(p2.x-p1.x)/(p2.y-p1.y)*pos.x+pos.y-p1.y )/( (p2.y-p1.y)/(p2.x-p1.x)+(p2.x-p1.x)/(p2.y-p1.y) );
            p.y=(p2.y-p1.y)/(p2.x-p1.x)*(p.x-p1.x)+p1.y;
            double d=Distance(p, pos);
            if (d <= scale){
                if ( (pos.y-(-(p2.x-p1.x)/(p2.y-p1.y)*(pos.x-p1.x)+p1.y))*(pos.y-(-(p2.x-p1.x)/(p2.y-p1.y)*(pos.x-p2.x)+p2.y)) < 0 )               
                    return true;
                return false;
            }
            else
                return false;            
        }
    }
    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }
};
class Mount: public Obstacle
{
public:
    Mount(const Point& p, double r){
        
        /*基本属性*/
        setPos(p);
        setScale(r);

        /*图形化属性*/
        texture.loadFromFile("C:\\project\\c++\\pjGame\\icon\\mountains.png");
        sprite.setScale(0.22f, 0.22f);
        sprite.setPosition(p.x - 0.5 * 0.22f * texture.getSize().x, p.y - 0.5 * 0.22f * texture.getSize().y);
        sprite.setTexture(texture);

    }
    virtual ~Mount(){};
};
class River : public Obstacle
{
public:
    River(const Point& p, double r) {

        /*基本属性*/
        setPos(p);
        setScale(r);

        /*图形化属性*/
        texture.loadFromFile("C:\\project\\c++\\pjGame\\icon\\river.png");
        sprite.setScale(0.22f, 0.22f);
        sprite.setPosition(p.x - 0.5 * 0.22f * texture.getSize().x, p.y - 0.5 * 0.22f * texture.getSize().y);
        sprite.setTexture(texture);

    }
    virtual ~River() {};
};

/*障碍物工厂方法*/
class ObstacleFactory
{
public:
    virtual Obstacle* createObstale(const Point& p, double r)=0;
};
class MountFactory: public ObstacleFactory
{
public:
    Obstacle* createObstale(const Point& p, double r){
        return dynamic_cast <Obstacle*> (new Mount(p,r));
    }
};
class RiverFactory : public ObstacleFactory
{
public:
    Obstacle* createObstale(const Point& p, double r) {
        return dynamic_cast <Obstacle*> (new River(p, r));
    }
};






/*
注意：
左右到底了也没避开障碍物:设置地图时，注意不能发生这种情况；
同时，士兵视野范围比较小，小于障碍物范围，使得绕过障碍物时左右走总有一个可以
视野是使用第六感意念的，不会被障碍物给挡住
*/