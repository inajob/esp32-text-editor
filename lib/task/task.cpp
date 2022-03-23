#include <task.h>
void Task::init(){}
void Task::setChrScreen(ChrScreen* pChrScreen){
  chrScreen = pChrScreen;
}
void Task::onkeydown(char key, char c, bool ctrl){}
void Task::draw(){}
void Task::setNextTask(Task* t){
  nextTask = t;
}
