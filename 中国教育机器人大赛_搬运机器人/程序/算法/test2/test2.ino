#define yellow 1
#define white 2
#define red 3
#define black 4
#define blue 5

int A = 2;
int B = 3;
int D = 1;
int E = 5;
int position_old = red;
int position_new = 0;
int jiaodu = 0;
int finish_flag = 0;

int find_color(int color)
{
  if(A=color)
  {
    return(yellow);
  }
  else if(B=color)
  {
    return(white);
  }
  else if(D=color)
  {
    return(black);
  }
  else if(E=color)
  {
    return(blue);
  }
  else
  {
    return(0);
  }
}

if(find_color(red)!=0)
{
  jiaodu=swerve(position_old-find_color(red));
  position_new=find_color(red);
  banyun_back(jiaodu);
  push();
  push_back();
  finish_flag++;
  if(find_color(position_old)!=0)
  {
    jiaodu=swerve(position_old-find_color(position_new));
    position_old=position_new;
    position_new=find_color(position_old);
    banyun_back(jiaodu);
    push();
    push_back();
    finish_flag++;
    if(find_color(position_old)!=0)
    {
      jiaodu=swerve(position_old-find_color(position_new));
      position_old=position_new;
      position_new=find_color(position_old);
      banyun_back(jiaodu);
      push();
      push_back();
      finish_flag++;
      if(find_color(position_old)!=0)
      {
        jiaodu=swerve(position_old-find_color(position_new));
        position_old=position_new;
        position_new=find_color(position_old);
        banyun_back(jiaodu);
        push();
        push_back();
        finish_flag++;
      }
      else
      {
        
      }
    }
  }
}
