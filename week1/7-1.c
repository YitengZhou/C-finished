#include <stdio.h>
enum day{sun,mon,tue=4,wed,thu,fri=10,sat};
typedef enum day day;
day find_next_day(day d);

int main(void)
{
  day d;
  d=wed;

  printf("%d\n",find_next_day(d));
  return 0;
}

day find_next_day(day d)
{
  day next_day;
  switch (d) {
    case sun/* value */:
    next_day=mon;
    printf("mon\n");
    break;
    case mon/* value */:
    next_day=tue;
    printf("tue\n");
    break;
    case tue/* value */:
    next_day=wed;
    printf("wed\n");
    break;
    case wed/* value */:
    next_day=thu;
    printf("thu\n");
    break;
    case thu/* value */:
    next_day=fri;
    printf("fri\n");
    break;
    case fri/* value */:
    next_day=sat;
    printf("sat\n");
    break;
    case sat/* value */:
    next_day=sun;
    printf("sun\n");
    break;
    default/* value */:
    printf("i wasn't expecting that!\n");
    break;
  }
  return next_day;
}
