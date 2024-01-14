#include "soldiers.h"
#define N2 50
#define N1 7

/*命令*/


class God
{
public:
    Inhibitor inhibt1;
    Inhibitor inhibt2;
    Stack<Obstacle*> things;
    Stack<Soldier*> team1;
    Stack<Soldier*> team2;
    Stack<Soldier*> shared_enemies_1;
    Stack<Soldier*> shared_enemies_2;


    God(): team1(N2), team2(N2), shared_enemies_1(N2), shared_enemies_2(N2), things(N1), inhibt1(1, *(new Point(400,150)), 195, 10000, 75, 80), inhibt2(2, *(new Point(400,850)), 195, 10000, 75, 900) {};
    ~God(){};
    void AddSoldier(int teamNum, Soldier* s){
        switch (teamNum)
        {
        case 1:
            team1.push(s);
            cout << "Team1\'s ";
            s->printName();
            cout << s->getNum() << " is born!" << endl;
            break;
        case 2:
            cout << "Team2\'s ";
            s->printName();
            cout << s->getNum() << " is born!" << endl;
            team2.push(s);
            break;
        default:
            break;
        }              
    }
    void RemoveSoldier(int teamNum, int soldierNum){
        switch (teamNum)
        {
        case 1:
            delete team1.remove(soldierNum);
            break;
        case 2:
            delete team2.remove(soldierNum);
            break;
        default:
            break;
        }
    }
    void AddMount(Obstacle* o){
        things.push(o);
    }

    void Fresh(){
        //清空所有小兵所发现的敌人以及所能攻击的敌人的列表
        for (int i = 0; i <= team1.getTop(); i++){
            team1.visit(i)->enimies.Clear();
            team1.visit(i)->shared_enimies.Clear();
            team1.visit(i)->objects.Clear();
            team1.visit(i)->things.Clear();
        }
        for (int i = 0; i <= team2.getTop(); i++){
            team2.visit(i)->enimies.Clear();
            team2.visit(i)->shared_enimies.Clear();
            team2.visit(i)->objects.Clear();
            team2.visit(i)->things.Clear();
        }

        //更新所有小兵之间的距离与攻击距离
        for (int i = 0; i <= team1.getTop(); i++)
            for (int j = 0; j <= team2.getTop(); j++){
                double d=Distance(team1.visit(i)->getLocation(), team2.visit(j)->getLocation());
                if (d<team1.visit(i)->getVision())                                                  //判断是否在视野范围之内
                {
                    team1.visit(i)->FindEnimy(team2.visit(j));
                    shared_enemies_1.push(team2.visit(j));
                    if (d < team1.visit(i)->getAtkrange()) {                                            //判断是否在攻击范围之内
                        team1.visit(i)->FindObject(team2.visit(j));
                    }
                }
                if (d<team2.visit(j)->getVision())                                                  //判断是否在视野范围之内
                {
                    team2.visit(j)->FindEnimy(team1.visit(i));
                    shared_enemies_2.push(team1.visit(i));
                    if (d<team2.visit(j)->getAtkrange())                                            //判断是否在攻击范围之内
                        team2.visit(j)->FindObject(team1.visit(i));                    
                }
            }

        //更新所有小兵得到的敌人列表
        for (int i = 0; i <= team1.getTop(); i++){
            for (int j = 0; j <= shared_enemies_1.getTop(); j++)
                team1.visit(i)->shared_enimies.push(shared_enemies_1.visit(j));
        }
        for (int i = 0; i <= team2.getTop(); i++){
            for (int j = 0; j <= shared_enemies_2.getTop(); j++)
                team2.visit(i)->shared_enimies.push(shared_enemies_2.visit(j));
        }


        //更新所有小兵到敌方水晶的距离
        for (int i = 0; i <= team1.getTop(); i++){
            double d=Distance(team1.visit(i)->getLocation(), inhibt2.getPosition());
            team1.visit(i)->setDist(d);
            if (d-inhibt2.getScale()<team1.visit(i)->getVision())                                   //判断是否在视野范围内
            {
                team1.visit(i)->setFindInhibitor(1, inhibt2.getPosition());
                if (d-inhibt2.getScale()<team1.visit(i)->getAtkrange())                             //判断是否在攻击范围内
                    team1.visit(i)->setInhibitInATK(1);
            }
        }
        for (int i = 0; i <= team2.getTop(); i++){
            double d=Distance(team2.visit(i)->getLocation(), inhibt1.getPosition());
            team2.visit(i)->setDist(d);
            if (d-inhibt1.getScale()<team2.visit(i)->getVision())                                   //判断是否在视野范围内
            {
                team2.visit(i)->setFindInhibitor(1, inhibt1.getPosition());
                if (d-inhibt1.getScale()<team2.visit(i)->getAtkrange())                             //判断是否在攻击范围内
                    team2.visit(i)->setInhibitInATK(1);
            }
        }

        //更新障碍物与小兵的距离并判断是否在其视野范围内
        for (int i = 0; i <= team1.getTop(); i++)
        {
            for (int j = 0; j <= things.getTop(); j++)
            {
                double d=Distance(team1.visit(i)->getLocation(), things.visit(j)->getPosition());
                if (d<team1.visit(i)->getVision()+things.visit(j)->getScale())
                    team1.visit(i)->things.push(things.visit(j));
            }
        }
        for (int i = 0; i <= team2.getTop(); i++)
        {
            for (int j = 0; j <= things.getTop(); j++)
            {
                double d=Distance(team2.visit(i)->getLocation(), things.visit(j)->getPosition());
                if (d<team2.visit(i)->getVision()+things.visit(j)->getScale())
                    team2.visit(i)->things.push(things.visit(j));
            }
        }

        //对每个小兵决策
        for (int i = 0; i <= team1.getTop(); i++)
            team1.visit(i)->Act();
        for (int i = 0; i <= team2.getTop(); i++)
            team2.visit(i)->Act();

        //更新所有小兵状态，以及双方基地状态
        int act;
        for (int i = 0; i <= team1.getTop(); i++){
            act=team1.visit(i)->Act2();
            if (act==0)             //命令该小兵移动
                team1.visit(i)->update(NULL, team1.visit(i)->move);              
            else if (act==1)        //命令敌人扣血
                team1.visit(i)->objects.visit( (team1.visit(i)->attack->getDrain()) )->update(team1.visit(i)->attack, NULL);
            else if (act==2)        //命令水晶扣血
                inhibt2.setHealth( inhibt2.getHealth()-team1.visit(i)->attack->getInjury() );
            
        }
        for (int i = 0; i <= team2.getTop(); i++){
            act=team2.visit(i)->Act2();
            if (act==0)             //命令该小兵移动
                team2.visit(i)->update(NULL, team2.visit(i)->move);
            else if (act==1)        //命令敌人扣血
                team2.visit(i)->objects.visit( (team2.visit(i)->attack->getDrain()) )->update(team2.visit(i)->attack, NULL);
            else if (act==2)        //命令水晶扣血
                inhibt1.setHealth( inhibt1.getHealth()-team2.visit(i)->attack->getInjury() );
        }

        


        for (int i = 0; i <= team2.getTop(); i++){
            act=team2.visit(i)->getAction();
            if (act==0)             //命令该小兵移动
                team2.visit(i)->update(NULL, team2.visit(i)->move);
            else if (act==1)        //命令敌人扣血
                team2.visit(i)->objects.visit( (team2.visit(i)->attack->getDrain()) )->update(team2.visit(i)->attack, NULL);
            else if (act==2)        //命令水晶扣血
                inhibt1.setHealth( inhibt1.getHealth()-team2.visit(i)->attack->getInjury() );
        }



        //清空
        shared_enemies_1.Clear();
        shared_enemies_2.Clear();
        for (int i = 0; i <= team1.getTop(); i++) {
            team1.visit(i)->enimies.Clear();
            team1.visit(i)->shared_enimies.Clear();
            team1.visit(i)->objects.Clear();
            team1.visit(i)->things.Clear();
        }
        for (int i = 0; i <= team2.getTop(); i++) {
            team2.visit(i)->enimies.Clear();
            team2.visit(i)->shared_enimies.Clear();
            team2.visit(i)->objects.Clear();
            team2.visit(i)->things.Clear();
        }

        //打印场上所有小兵状态，并埋葬牺牲的英雄
        for (int i = 0; i <= team1.getTop(); i++){
            if (team1.visit(i)->getHealth()<=0){
                cout << "Team1\'s ";
                team1.visit(i)->printName();
                cout << team1.visit(i)->getNum() << " sacrifice!" << endl;
                RemoveSoldier(1, i);
                i--;
            }
        }
        for (int i = 0; i <= team2.getTop(); i++){
            if (team2.visit(i)->getHealth()<=0){
                cout << "Team2\'s ";
                team2.visit(i)->printName();
                cout << team2.visit(i)->getNum() << " sacrifice!" << endl;
                RemoveSoldier(2, i);
                i--;
            }
        }
    }
    
    int IsFinished(){
        //player 1 似乎更有优势？
        if (inhibt1.getHealth()<=0 || inhibt2.getHealth()<=0){
            if (inhibt2.getHealth()<=0)
            {
                cout << "Player1 Win!" << endl;
                return 1;
            }
            else
            {
                cout << "Player2 Win!" << endl;
                return 2;
            }
        }
        return 0;
    }

    void printInfo(){
        cout << "inhibior1 health: " << inhibt1.getHealth() << endl;
        for (int i = 0; i <= team1.getTop(); i++){
            team1.visit(i)->printInfo();
        }
        cout << "inhibior2 health: " << inhibt2.getHealth() << endl;
        for (int i = 0; i <= team2.getTop(); i++){
            team2.visit(i)->printInfo();
        }
    }

};

/*
设定障碍物只能设置在离水晶一定距离之外
*/