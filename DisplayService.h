#ifndef DisplayService_h
#define DisplayService_h

#include <LiquidCrystal.h>

const byte rs = 2, en = 3, db4 = 4, db5 = 5, db6 = 6, db7 = 7;
LiquidCrystal _lcd(rs, en, db4, db5, db6, db7);

class DisplayService 
{

  private:

    int _delayBetweenShifts;

  public:

    void configureDisplay() 
    {
        _delayBetweenShifts = 150;
        _lcd.begin(16, 2);
    }

    void sendToDisplay(String row1Message, String row2Message)
    {
        _lcd.clear();
        _lcd.setCursor(0,0);

        _lcd.print(row1Message);

        if(row2Message.length() > 16)
        {
            _lcd.setCursor(0,1);
            _lcd.print(row2Message);
            delay(1000);

            byte i = 0;
            while (i < row2Message.length() - 15)
            {
                _lcd.setCursor(0,1);
                _lcd.print(row2Message.substring(i, row2Message.length()));
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
};

DisplayService displayService;

#endif