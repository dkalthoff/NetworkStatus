#include <LiquidCrystal.h>

const int rs = 2, en = 3, db4 = 4, db5 = 5, db6 = 6, db7 = 7;
LiquidCrystal lcd(rs, en, db4, db5, db6, db7);

void configureDisplay()
{
  lcd.begin(16, 2);
}

void sendToDisplay(String row1Message, String row2Message)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(row1Message);
  lcd.setCursor(0,1);
  lcd.print(row2Message);
}
