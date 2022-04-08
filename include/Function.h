#include <arduino.h>

class Function
{
public:
    virtual void Execute(void* param)
    {

    }   

    Function(){} 
};

class F1 : public Function
{
 public:
    void Execute(void *param)
    {
        Serial.println("F1");
    }
};

class F2 : public Function
{
 public:
    void Execute(void *param)
    {
        Serial.println("F2");
    }
};

class F3 : public Function
{
 public:
    void Execute(void *param)
    {
        Serial.println("F3");
    }
};

class F4 : public Function
{
 public:
    void Execute(void *param)
    {
        Serial.println("F4");
    }
};