#include "elements.h"
#define N 50
#define M 7
#define INFINITE 1000

class Soldier
{
private:
    int team;                           //所在队，1或2
    int num;                            //代号
    Point location;                     //位置
    double health;                      //生命值
    double atkpower;                    //攻击力，值得商榷，还可以分类，物理，法术，真实
    double atkrange;                    //攻击范围
    double speed;                       //移动速度
    double shield;                      //护盾
    double vision;                      //视野范围
    int action;                         //士兵的状态, 0: 移向敌人，1：攻击敌人，2：攻击敌方水晶，3：前进 或 为了绕开障碍物而进行左右移动
    Point InhibitorPos;                 //敌方水晶的位置
    double dist;                        //士兵与敌方水晶距离
    int Inhibitor_in_Vision;            //敌方水晶是否在视野内
    int Inhibitor_in_Atkrange;          //敌方水晶是否在攻击范围内
    Command* command;

public:

    sf::Texture texture;                // 士兵的纹理
    sf::Sprite sprite;                  // 士兵的精灵
    sf::Texture texture_atk;                // 攻击状态士兵的纹理
    sf::Sprite sprite_atk;                  // 攻击状态士兵的精灵


    sf::RectangleShape healthBarRect;   // 血量条大小
    sf::RectangleShape shieldBarRect;   // 护盾条大小
    double ita1, ita2;                  // 控制血条、护盾条长度

    Stack<Soldier*> enimies;            //敌人列表
    Stack<Soldier*> shared_enimies;     //共同敌人列表
    Stack<Soldier*> objects;            //可攻击敌人列表
    Stack<Obstacle*> things;            //视野内的障碍物
    
    Attackable* attack;                 //attack 动作
    Movable* move;                      //move 动作

    Soldier():enimies(N), shared_enimies(N), objects(N), things(M), dist(INFINITE){
        Inhibitor_in_Atkrange=0;
        Inhibitor_in_Vision=0;
    };
    virtual ~Soldier(){
        delete [] attack;
        attack =NULL;
        delete [] move;
        move=NULL;
        enimies.Clear();
        shared_enimies.Clear();
        objects.Clear();
    }
    void setTeamnum(int i){
        team=i;
    }
    int getTeamnum(){
        return team;
    }
    void setNum(int i){
        num=i;
    }
    int getNum(){
        return num;
    }
    void setFindInhibitor(int i, const Point& p){
        Inhibitor_in_Vision=i;
        InhibitorPos=p;
    }
    int getFindInhibitor(){
        return Inhibitor_in_Vision;
    }
    void setInhibitInATK(int i){
        Inhibitor_in_Atkrange=i;
    }
    int getInhibitInATK(){
        return Inhibitor_in_Atkrange;
    }
    void setDist(double d){
        dist=d;
    }
    int getDist(){
        return dist;
    }
    void setLocation(const Point& p){
        location=p;
    }
    Point getLocation(){
        return location;
    }
    void setHealth(double val){
        health=val;
        healthBarRect.setSize(sf::Vector2f(health * ita1, 5));
    }
    double getHealth(){
        return health;
    }
    void setAtkpower(double val){
        atkpower=val;
    }
    double getAtkpower(){
        return atkpower;
    }
    void setAtkrange(double val){
        atkrange=val;
    }
    double getAtkrange(){
        return atkrange;
    }
    void setSpeed(double h){
        speed=h;
    }
    double getSpeed(){
        return speed;
    }
    void setShield(double h){
        shield=h;
        shieldBarRect.setSize(sf::Vector2f(health * ita2, 5));
    }
    double getShield(){
        return shield;
    }
    void setVision(double h){
        vision=h;
    }
    double getVision(){
        return vision;
    }
    void setAction(int val){
        action=val;
    }
    int getAction(){
        return action;
    }
    void FindEnimy(Soldier* s){
        enimies.push(s);
    }
    void FindObject(Soldier *s){
        objects.push(s);
    }
    virtual void printName()=0;
    void printInfo(){
        cout << "Team" << getTeamnum() << "\'s ";
        printName();
        cout << "[" << getNum() << "]'s info:" <<  endl;
        cout << "Location: ";
        getLocation().printPoint();
        cout << ";\t";
        cout << "Health: " << health << ";\t";
        cout << "Shield: " << shield << ";\t";
        cout << "Speed: " << speed << endl;
    }
    void IsAttacked(Attackable* a){
        switch (a->getValid())
        {
        case 0:
            break;
        case 1:
            if (getShield() >= a->getInjury())
                setShield(getShield()-a->getInjury());
            else{
                setShield(0);
                setHealth(getHealth()-(a->getInjury()-getShield()));
            }
            break;
        case 2:
            if (getShield() >= a->getInjury())
                setShield(getShield()-a->getInjury());
            else{
                setShield(0);
                setHealth(getHealth()-(a->getInjury()-getShield()));
            }
            break;
        case 3:
            setHealth(getHealth()-a->getInjury());
            break;
        default:
            break;
        }
    };      //可拓展为virtual

    /*小兵决策策略*/
    void Act(){
        //如果敌方水晶在攻击范围内，攻击敌方水晶！
        if (Inhibitor_in_Atkrange == 1) {
            action = 2;
            command=attack;
        }
        //如果攻击范围内有敌人，攻击敌人！（攻击哪个需要优化）
        else if (objects.IsEmpty()==false)
        {
            attack->setDrain(objects.getTop());
            action = 1;
            command=attack;
        }
        //如果敌方水晶在视野范围内，向敌方水晶进发！
        else if (Inhibitor_in_Vision==1)
        {
            move->toward(InhibitorPos);
            action=0;
            command=move;
        }
        //如果视野范围内有敌人，向敌人进发！
        else if (enimies.IsEmpty()==false)
        {
            move->toward( enimies.visit(enimies.getTop())->getLocation() );
            action=0;
            command=move;
        }
        //如果敌人视野范围内有敌人，向敌人进发！
        else if (shared_enimies.IsEmpty()==false)
        {
            move->toward( shared_enimies.visit(shared_enimies.getTop())->getLocation() );
            action=0;
            command=move;
        }
        //前进前进前进！！！
        else{
            move->forward((team==1)?1:-1);
            action=0;
            command=move;
        }
        //判断是否挡路，是，则绕开障碍物
        int tag=-1;
        for (int k = 0; k<=things.getTop(); k++)
        {
            if (action == 0 && things.visit(k)->IsinWay(location, move->getDrain()) == true)
                tag = k;
            /* else if (action == 1 && things.visit(k)->IsinWay(location, objects.visit(attack->getDrain())->getLocation())==true)
                    tag = k;
            else if (action == 2 && things.visit(k)->IsinWay(location, InhibitorPos) == true)
                    tag = k; */
        }
        if (tag>=0)
        {
            move->bypass(things.visit(tag)->getPosition());
            action=0;
            command=move;
        }
    };
    
    /*命令模式——小兵真正 take action，本质是打印信息*/
    int Act2(){
        
        //打印
        cout << "Team" << getTeamnum() << "\'s ";
        printName();
        cout << num << " ";
        
        //移动
        if (action==0)
        {
            command->execute();
            cout << " from ";
            move->getSource().printPoint();
            cout << " to ";
            move->getDrain().printPoint();
            cout << endl;
            return 0;
        }
        //攻击敌人
        if (action==1)
        {
            command->execute();
            cout << "Team" << getTeamnum() << "\'s ";
            objects.visit(attack->getDrain())->printName();
            cout << objects.visit(attack->getDrain())->getNum() << "!" << endl;
            return 1;
        }
        //攻击水晶
        if (action==2){
            command->execute();
            cout << " enimies' inhibitor!" << endl;
            return 2; 
        }
        return 0;
    }
    
    /*更新小兵新状态，包括位置和血量以及图形化*/
    void update(Attackable* a, Movable* m){
        
        //被攻击了
        if (a)
            IsAttacked(a);
        
        //移动了
        if (m){
            
            /*基本属性变化*/
            location=m->getDrain();             //到下一个位置
            move->setSource(location);          //设置新的移动动作的源位置
            sprite.setPosition(location.x, location.y);
            sprite_atk.setPosition(location.x, location.y);
            
            /*图形化设置*/
            healthBarRect.setPosition(getLocation().x, getLocation().y);
            shieldBarRect.setPosition(getLocation().x, getLocation().y-5);

        }
        return;

    }

    /*画出小兵以及血量和护盾*/
    void draw(sf::RenderWindow& window) {
        if (action==0)
            window.draw(sprite);
        else
            window.draw(sprite_atk);
        window.draw(healthBarRect);
        window.draw(shieldBarRect);
    }

};

class AP: public Soldier
{
public:
    AP(int team, int i, const Point& p){

        /*基本属性*/
        setTeamnum(team);
        setNum(i);
        setLocation(p);
        setHealth(300);
        setAtkpower(50);
        setAtkrange(25);
        setSpeed(13);
        setShield(100);
        setVision(80);
        attack=dynamic_cast <Attackable*> (new MagicAttack[1]);
        attack->setInjury(getAtkpower());
        move=dynamic_cast <Movable*> (new Flash[1]);
        move->setSource(getLocation());
        move->setStep(getSpeed());

        /*士兵图形化属性*/
        if (team==1)            //blue
            texture.loadFromFile("C:\\project\\c++\\pjGame\\icon\\hooded_blue.png");
        else                    //red
            texture.loadFromFile("C:\\project\\c++\\pjGame\\icon\\hooded_red.png");
        sprite.setScale(0.1f, 0.1f);
        sprite.setPosition(p.x- 0.5* 0.1f *texture.getSize().x, p.y - 0.5* 0.1f * texture.getSize().y);
        sprite.setTexture(texture);
        if (team == 1)            //blue
            texture_atk.loadFromFile("C:\\project\\c++\\pjGame\\icon\\hooded_blue_2.png");
        else                    //red
            texture_atk.loadFromFile("C:\\project\\c++\\pjGame\\icon\\hooded_red_2.png");
        sprite_atk.setScale(0.1f, 0.1f);
        sprite_atk.setPosition(p.x - 0.5 * 0.1f * texture_atk.getSize().x, p.y - 0.5 * 0.1f * texture_atk.getSize().y);
        sprite_atk.setTexture(texture_atk);

        /*血条和护盾*/
        healthBarRect.setFillColor(sf::Color::Green);
        healthBarRect.setPosition(p.x, p.y);
        healthBarRect.setSize(sf::Vector2f(35, 5));
        ita1 = 35 / getHealth();
        shieldBarRect.setFillColor(sf::Color::Yellow);
        shieldBarRect.setPosition(p.x, p.y-5);
        shieldBarRect.setSize(sf::Vector2f(35, 5));
        ita2 = 35 / getHealth();
        
    };
    virtual ~AP(){};
    void printName(){
        cout << "AP";
    }
};

class ADC: public Soldier
{
public:
    ADC(int team, int i, const Point& p){

        /*基本属性*/
        setTeamnum(team);
        setNum(i);
        setLocation(p);
        setHealth(200);
        setAtkpower(80);
        setAtkrange(80);
        setSpeed(8);
        setShield(100);
        setVision(120);
        attack=dynamic_cast <Attackable*> (new PhysicalAttack[1]);
        attack->setInjury(getAtkpower());
        move=dynamic_cast <Movable*> (new Flash[1]);
        move->setSource(getLocation());
        move->setStep(getSpeed());

        /*士兵图形化属性*/
        if (team==1)            //blue
            texture.loadFromFile("C:\\project\\c++\\pjGame\\icon\\bowman_blue.png");
        else                    //red
            texture.loadFromFile("C:\\project\\c++\\pjGame\\icon\\bowman_red.png");
        sprite.setScale(0.1f, 0.1f);
        sprite.setPosition(p.x - 0.5* 0.1f * texture.getSize().x, p.y - 0.5* 0.1f * texture.getSize().y);
        sprite.setTexture(texture);
        if (team == 1)            //blue
            texture_atk.loadFromFile("C:\\project\\c++\\pjGame\\icon\\bowman_blue_2.png");
        else                    //red
            texture_atk.loadFromFile("C:\\project\\c++\\pjGame\\icon\\bowman_red_2.png");
        sprite_atk.setScale(0.1f, 0.1f);
        sprite_atk.setPosition(p.x - 0.5 * 0.1f * texture_atk.getSize().x, p.y - 0.5 * 0.1f * texture_atk.getSize().y);
        sprite_atk.setTexture(texture_atk);

        /*血条和护盾图形化属性*/
        healthBarRect.setFillColor(sf::Color::Green);
        healthBarRect.setPosition(p.x, p.y);
        healthBarRect.setSize(sf::Vector2f(35, 5));
        ita1 = 35 / getHealth();
        shieldBarRect.setFillColor(sf::Color::Yellow);
        shieldBarRect.setPosition(p.x, p.y-5);
        shieldBarRect.setSize(sf::Vector2f(35, 5));
        ita2 = 35 / getHealth();

    };
    virtual ~ADC(){};
    void printName(){
        cout << "ADC";
    }
};

class AD: public Soldier
{
public:
    AD(int team, int i, const Point& p){

        /*士兵图形化属性*/
        setTeamnum(team);
        setNum(i);
        setLocation(p);
        setHealth(500);
        setShield(300);
        setAtkpower(30);
        setAtkrange(10);
        setSpeed(30);
        setVision(60);
        attack=dynamic_cast <Attackable*> (new RealAttack[1]);
        attack->setInjury(getAtkpower());
        move=dynamic_cast <Movable*> (new Flash[1]);
        move->setSource(getLocation());
        move->setStep(getSpeed());

        /*士兵图片*/
        if (team==1)            //blue
            texture.loadFromFile("C:\\project\\c++\\pjGame\\icon\\ninja_blue.png");
        else                    //red
            texture.loadFromFile("C:\\project\\c++\\pjGame\\icon\\ninja_red.png");
        sprite.setScale(0.1f, 0.1f);
        sprite.setPosition(p.x - 0.5* 0.1f * texture.getSize().x, p.y - 0.5* 0.1f * texture.getSize().y);
        sprite.setTexture(texture);
        if (team == 1)            //blue
            texture_atk.loadFromFile("C:\\project\\c++\\pjGame\\icon\\ninja_blue_2.png");
        else                    //red
            texture_atk.loadFromFile("C:\\project\\c++\\pjGame\\icon\\ninja_red_2.png");
        sprite_atk.setScale(0.1f, 0.1f);
        sprite_atk.setPosition(p.x - 0.5 * 0.1f * texture_atk.getSize().x, p.y - 0.5 * 0.1f * texture_atk.getSize().y);
        sprite_atk.setTexture(texture_atk);

        /*血条和护盾图形化属性*/
        healthBarRect.setFillColor(sf::Color::Green);
        healthBarRect.setPosition(p.x, p.y);
        healthBarRect.setSize(sf::Vector2f(35, 5));
        ita1 = 35 / getHealth();
        shieldBarRect.setFillColor(sf::Color::Yellow);
        shieldBarRect.setPosition(p.x, p.y-5);
        shieldBarRect.setSize(sf::Vector2f(35, 5));
        ita2 = 35 / getHealth();

    };
    virtual ~AD(){};
    void printName(){
        cout << "AD";
    }
};

/*生成士兵的工厂方法*/


class SoldierFactory
{
public: 
    virtual Soldier* createSoldier(int team, int i, const Point& p)=0;
};
class APFactory: public SoldierFactory
{
public:
    Soldier* createSoldier(int team, int i, const Point& p){
        return dynamic_cast <Soldier*> (new AP(team, i, p));
    }
};
class ADCFactory: public SoldierFactory
{
public:
    Soldier* createSoldier(int team, int i, const Point& p){
        return dynamic_cast <Soldier*> (new ADC(team, i, p));
    }
};
class ADFactory: public SoldierFactory
{
public:
    Soldier* createSoldier(int team, int i, const Point& p){
        return dynamic_cast <Soldier*> (new AD(team, i, p));
    }
};
