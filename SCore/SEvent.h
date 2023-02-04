#ifndef SEVENT_H_
#define SEVENT_H_

#include"../SCore/Configer.h"

enum SEventType
{
     ResizeEvent = SDL_USEREVENT,
};

struct SResizeEvent
{
    Uint32 type;        /**< ::ResizeEvent */
    int oldw;
    int oldh;
    int w;
    int h;
};


#endif // !SEVENT_H_
