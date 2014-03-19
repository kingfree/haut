#include <iostream>
using namespace std;
int mix;
int Time;
class headquarter;
class warrior {
  private:
    int HP, ATK, ID;
  public:
    static int rednum;
    static int bluenum;
    friend class headquarter;
    warrior(int n) {
      HP=n;
      ID=1;
    }
    void birth(int flag, int rank);
};
int warrior::rednum=1;
int warrior::bluenum=1;
void warrior::birth(int flag, int rank) {
  if(rank==1) {
    if(flag==1) {
      cout<<Time/100<<Time/10<<Time%10<<" ";
      cout<<"red dragon "<<rednum<<" born with strength "<<HP<<", "<<ID<<" dragon in red headquarter"<<endl;
      ID++;
      rednum++;
    }
    if(flag==2) {
      cout<<Time/100<<Time/10<<Time%10<<" ";
      cout<<"blue dragon "<<bluenum<<" born with strength "<<HP<<", "<<ID<<" dragon in blue headquarter"<<endl;
      ID++;
      bluenum++;
    }
  }
  if(rank==2) {
    if(flag==1) {
      cout<<Time/100<<Time/10<<Time%10<<" ";
      cout<<"red ninja "<<rednum<<" born with strength "<<HP<<", "<<ID<<" ninja in red headquarter"<<endl;
      ID++;
      rednum++;
    }
    if(flag==2) {
      cout<<Time/100<<Time/10<<Time%10<<" ";
      cout<<"blue ninja "<<bluenum<<" born with strength "<<HP<<", "<<ID<<" ninja in blue headquarter"<<endl;
      ID++;
      bluenum++;
    }
  }  if(rank==3) {
    if(flag==1) {
      cout<<Time/100<<Time/10<<Time%10<<" ";
      cout<<"red iceman "<<rednum<<" born with strength "<<HP<<", "<<ID<<" iceman in red headquarter"<<endl;
      ID++;
      rednum++;
    }
    if(flag==2) {
      cout<<Time/100<<Time/10<<Time%10<<" ";
      cout<<"blue iceman "<<bluenum<<" born with strength "<<HP<<", "<<ID<<" iceman in blue headquarter"<<endl;
      ID++;
      bluenum++;
    }
  }  if(rank==4) {
    if(flag==1) {
      cout<<Time/100<<Time/10<<Time%10<<" ";
      cout<<"red lion "<<rednum<<" born with strength "<<HP<<", "<<ID<<" lion in red headquarter"<<endl;
      ID++;
      rednum++;
    }
    if(flag==2) {
      cout<<Time/100<<Time/10<<Time%10<<" ";
      cout<<"blue lion "<<bluenum<<" born with strength "<<HP<<", "<<ID<<" lion in blue headquarter"<<endl;
      ID++;
      bluenum++;
    }
  }  if(rank==5) {
    if(flag==1) {
      cout<<Time/100<<Time/10<<Time%10<<" ";
      cout<<"red wolf "<<rednum<<" born with strength "<<HP<<", "<<ID<<" wolf in red headquarter"<<endl;
      ID++;
      rednum++;
    }
    if(flag==2) {
      cout<<Time/100<<Time/10<<Time%10<<" ";
      cout<<"blue wolf "<<bluenum<<" born with strength "<<HP<<", "<<ID<<" wolf in blue headquarter"<<endl;
      ID++;
      bluenum++;
    }
  }
}
class headquarter {
  private:
    friend class warrior;
    warrior dragon, ninja, iceman, lion, wolf;
  public:
    int Total_life;
    int num;
    headquarter(int D, int N, int I, int L, int W, int M):
      dragon(D), ninja(N), iceman(I), lion(L), wolf(W) {
    Total_life=M;
    num=0;
  }
    int birth(int rank, int flag);
};
int headquarter::birth(int rank, int flag) {
  switch (rank) {
    case 1: {
          if(Total_life>=dragon.HP) {
            Total_life=Total_life-dragon.HP;
            dragon.birth(flag, 1);
            return 2;
          }
          else return 0;
          break;
        }
    case 2: {
          if(Total_life>=ninja.HP) {
            Total_life=Total_life-ninja.HP;
            ninja.birth(flag, 2);
            return 2;
          }
          else return 0;
          break;
        }
    case 3: {
          if(Total_life>=iceman.HP) {
            Total_life=Total_life-iceman.HP;
            iceman.birth(flag, 3);
            return 2;
          }
          else return 0;
          break;
        }
    case 4: {
          if(Total_life>=lion.HP) {
            Total_life=Total_life-lion.HP;
            lion.birth(flag, 4);
            return 2;
          }
          else return 0;
          break;
        }
    case 5: {
          if(Total_life>=wolf.HP) {
            Total_life=Total_life-wolf.HP;
            wolf.birth(flag, 5);
            return 2;
          }
          else return 0;
          break;
        }
  }
}
void BIRTH(headquarter& red, headquarter& blue) {
  int flag_red=0, flag_blue=0;
  int red_step=1, blue_step=1;
  do {
    if(red.Total_life>=mix) {
      for(int i=1; i<=5; i++) {
        switch(red_step%5) {
          case 1:flag_red=flag_red+red.birth(3, 1); break;
          case 2:flag_red=flag_red+red.birth(4, 1); break;
          case 3:flag_red=flag_red+red.birth(5, 1); break;
          case 4:flag_red=flag_red+red.birth(2, 1); break;
          case 0:flag_red=flag_red+red.birth(1, 1); break;
        }
        red_step++;
        if(flag_red==2) {
          flag_red=0;
          break;
        }
        if (flag_red==0)
          continue;
      }
    }
    else if(red.Total_life<mix && flag_red==0 ) {
      cout<<Time/100<<Time/10<<Time%10<<" ";
      cout<<"red headquarter stops making warriors"<<endl;
      flag_red=1;
    }
    if(blue.Total_life<mix && flag_blue==0 ) {
      cout<<Time/100<<Time/10<<Time%10<<" ";
      cout<<"blue headquarter stops making warriors"<<endl;
      flag_blue=1;
    }
    if(blue.Total_life>=mix) {
      for(int i=1; i<=5; i++) {
        switch(blue_step%5) {
          case 1:flag_blue=flag_blue+blue.birth(4, 2); break;
          case 2:flag_blue=flag_blue+blue.birth(1, 2); break;
          case 3:flag_blue=flag_blue+blue.birth(2, 2); break;
          case 4:flag_blue=flag_blue+blue.birth(3, 2); break;
          case 0:flag_blue=flag_blue+blue.birth(5, 2); break;
        }
        blue_step++;
        if(flag_blue==2) {
          flag_blue=0;
          break;
        }
        if(flag_blue==0)
          continue;
      }
    }
    if(red.Total_life<mix && flag_red==0 ) {
      Time++;
      cout<<Time/100<<Time/10<<Time%10<<" ";
      cout<<"red headquarter stops making warriors"<<endl;
      flag_red=1;
      Time--;
    }
    Time++;
  }
  while(red.Total_life>=mix || blue.Total_life >=mix || flag_red==0 || flag_blue==0);
}
void mix_1(int a, int b,  int c, int d, int e) {
  mix=a;
  if(mix>b)
    mix=b;
  if(mix>c)
    mix=c;
  if(mix>d)
    mix=d;
  if(mix>e)
    mix=e;
}
int main() {
  int grope;
  cin>>grope;
  int i=1;
  while(i<=grope) {
    Time=0;
    int M;
    cin>>M;
    int dragon_start, ninja_start, iceman_start, lion_start, wolf_start;
    cin>>dragon_start>>ninja_start>>iceman_start>>lion_start>>wolf_start;
    mix_1(dragon_start, ninja_start, iceman_start, lion_start, wolf_start);
    cout<<"Case:"<<i<<endl;
    headquarter red(dragon_start, ninja_start, iceman_start, lion_start, wolf_start, M);
    headquarter blue(dragon_start, ninja_start, iceman_start, lion_start, wolf_start, M);
    BIRTH(red, blue);
    i++;
    warrior::rednum=1;
    warrior::bluenum=1;
    Time=0;
  }
  return 0;
}
