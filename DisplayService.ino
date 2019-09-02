#include <LiquidCrystal.h>

const int rs = 2, en = 3, db4 = 4, db5 = 5, db6 = 6, db7 = 7;
LiquidCrystal lcd(rs, en, db4, db5, db6, db7);

void initDisplay()
{
  lcd.begin(16, 2);
}

void sendToDisplay(String message, int row)
{
  lcd.setCursor(0,row);
  lcd.print(message);
}
