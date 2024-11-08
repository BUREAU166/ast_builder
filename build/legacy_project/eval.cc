#include "eval.h"

// ЭТА ФУНКЦИЯ НИ ОТ КОГО НЕ ЗАВИСИТ
// НАМ ВСЁ РАВНО НА НЕё
int eval(void* arg) {
  Adder* adder = (Adder*)arg;

  return adder->val;
}

// ЭТА ФУНКЦИЯ НИ ОТ КОГО НЕ ЗАВИСИТ
// НАМ ВСЁ РАВНО НА НЕё
bool isEven(void* arg) {
  Adder* adder = (Adder*)arg;

  return adder->val % 2 == 0;
}

// ЭТУ ФУНКЦИИ МЫ ХОТИМ ПОСМОТРЕТЬ КАК LEGACY
// ОНА ЗАВИСИТ ОТ eval()
// НАМ НУЖНО ПОЛУЧИТЬ ЕЁ И ВЫЗЫВАЕМЫЕ ЕЙ НАШИ ФУНКЦИИ (to est eval)
std::vector<Adder> filter(std::vector<Adder> adders) {
  Dummy dumm(2);
  for(int i = 0; i < adders.size(); i++) {
    for(int j = 0; j < adders.size(); j++) {
      if(eval(&adders[j]) < eval(&adders[i])) {
        std::swap(adders[j], adders[i]);
      }
    }
  }  
}
