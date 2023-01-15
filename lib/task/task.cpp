#include <task.h>
void Task::init(){}
void Task::setChrScreen(ChrScreen* pChrScreen){
  chrScreen = pChrScreen;
}
void Task::setFep(Task* pFep){
  fep = pFep;
}

bool Task::onkeydown(char key, char c, bool ctrl){return true;}
bool Task::onChar(wchar_t c){}
void Task::draw(){}
void Task::setNextTask(Task* t){
  nextTask = t;
}
void Task::terminate(){
  isTerminate = true;
}
