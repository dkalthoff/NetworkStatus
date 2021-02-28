#ifndef DisplayService_h
#define DisplayService_h

#include <LiquidCrystal.h>

const byte rs = 2, en = 3, db4 = 4, db5 = 5, db6 = 6, db7 = 7;
LiquidCrystal _lcd(rs, en, db4, db5, db6, db7);

class DisplayService 
{

  private:

    int _delayBetweenShifts;

    char* substr(const char* arr, int begin, int len)
    {
        char* res = new char[len + 1];
        for (int i = 0; i < len; i++)
            res[i] = *(arr + begin + i);
        res[len] = 0;
        return res;
    }

  public:

    void configureDisplay() 
    {
        _delayBetweenShifts = 100;
        _lcd.begin(16, 2);
    }

    void sendToDisplay(const char* row1Message, const char* row2Message) 
    {
        _lcd.clear();
        _lcd.setCursor(0,0);

        _lcd.print(row1Message);

        if(strlen(row2Message) > 16)
        {
            _lcd.setCursor(0,1);
            _lcd.print(row2Message);
            delay(1500);

            byte i = 0;
            while (i < strlen(row2Message) - 15)
            {
                _lcd.setCursor(0,1);
                _lcd.print(substr(row2Message, i, strlen(row2Message)));
                delay(_delayBetweenShifts);
                i++;
            }
        }
        else
        {
            _lcd.setCursor(0,1);
            _lcd.print(row2Message);
        }
    }

    void sendToDisplay(String row1Message, String row2Message)
    {
        char row1MessageCharArray[row1Message.length() + 1];
        char row2MessageCharArray[row2Message.length() + 1];
        row1Message.toCharArray(row1MessageCharArray, row1Message.length() + 1);
        row2Message.toCharArray(row2MessageCharArray, row2Message.length() + 1);

        sendToDisplay(row1MessageCharArray, row2MessageCharArray);
    }
};

DisplayService displayService;

#endif