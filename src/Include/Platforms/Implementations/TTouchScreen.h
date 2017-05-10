/**
 * @Author DGKokorev@sberbank.ru
 * @date   March 2017
 * @brief  Touch screen device base class
 */

#ifndef _U2_T_TOUCHSCREEN_H
#define _U2_T_TOUCHSCREEN_H

#include "Platforms/Interfaces/ITouchScreen.h"
#include <deque>

class TTouchScreen : public ITouchScreen{
public:
  void Clear(){
    points.clear();
  }

  int IsTouchPressed(){
    return points.size();
  }

  //function will not remove touch from deque
  TouchPoint GetTouch(size_t position = 0){
    if(position < points.size())
      return points[0];
    return TouchPoint();
  }

  //function will remove touch from deque
  TouchPoint GetFirstTouch(){
    if(points.size()){
      TouchPoint point = points[0];
      points.pop_front();
      return point;
    }
    return TouchPoint();
  }

private:
  std::deque<TouchPoint> points;

protected:
  void PutTouch(TouchPoint point){
    points.push_back(point);
  }
};

#endif

