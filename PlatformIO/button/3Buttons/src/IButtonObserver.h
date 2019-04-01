#ifndef __IOBSERVER_H__
#define __IOBSERVER_H__

#include "ButtonLibrary.h"

class IButtonObserver
{
    public:
        virtual void OnButtonPressed(ButtonEventData_t eventData) = 0;
};

#endif // __IOBSERVER_H__