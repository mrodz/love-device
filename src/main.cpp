#include <Arduino.h>
#include <pico/sleep.h>
#include <LiquidCrystal_I2C.h>

const int BUTTON_PIN = 12;
const int RELAY_PIN = 0;
const int LCD_I2C_ADDRESS = 0x27;
const int LCD_COLUMNS = 16;
const int LCD_ROWS = 2;
const int SERIAL_BAUD_RATE = 4800;

void setup_button()
{
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void setup_relay()
{
  pinMode(RELAY_PIN, OUTPUT);
}

void relay_on()
{
  digitalWrite(RELAY_PIN, HIGH);
}

void relay_off()
{
  digitalWrite(RELAY_PIN, LOW);
}

static LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void setup_lcd()
{
  lcd.init();
  lcd.clear();
  lcd.backlight();
}

void setup()
{
  Serial.begin(SERIAL_BAUD_RATE);
  setup_button();
  setup_relay();
}

static volatile byte relayState = LOW;
static volatile int buttonState = 0;
static volatile bool pushed = false;

bool await_loop_button()
{
  buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == HIGH)
  {
    return true;
  }
  else
  {
    return false;
  }
}

const char *lovable_things[50] = {
    "Your unwavering loyalty",
    "You put in constant effort",
    "You know how to cheer me up",
    "You're buff as heck",
    "Your smile",
    "Your face",
    "Your nose",
    "How you comfort me",
    "Your stuffed animals",
    "Your forgiveness",
    "Your empathy",
    "Your candles",
    "The hand-written letter",
    "Eating dinners together",
    "Your joy when I give you flowers",
    "Snoopy",
    "Lamby",
    "Mooteo",
    "Bartholomew",
    "Sushi dates",
    "That you know how to code",
    "That you're driven",
    "Your hair",
    "Your humor",
    "Our study dates",
    "Watching movies by your side",
    "Giving head scratches",
    "Receiving head scratches",
    "Your massages are to die for",
    "You have amazing style",
    "Your music taste is great",
    "You're gorgeous without makeup",
    "... and even prettier with it",
    "Your friends are pretty lit",
    "You know how to have fun",
    "Perfect balance of nerd + hottie",
    "Your hugs",
    "Your eyes",
    "You give me butterflies 24/7",
    "You're generous",
    "I won the lotto to be your bf",
    "Brazilian Baddie",
    "You wait for me every day @ CCHS",
    "We say hi every nutrition",
    "Your gifts are meaningful",
    "You're my tanning buddy",
    "We can be biggies together",
    "You've supported me at my lowest",
    "I love talking to you",
    "You make time for me",
};

void split_str(const char *src, char *dst1, char *dst2)
{
  memset(dst1, 0, 17);
  memset(dst2, 0, 17);

  int len = strlen(src);

  if (len <= 16)
  {
    strncpy(dst1, src, len);
    dst1[len] = '\0';
    return;
  }

  int split_pos = 16;
  while (split_pos > 0 && !isspace(src[split_pos]))
  {
    split_pos--;
  }

  if (split_pos == 0)
  {
    split_pos = 16;
  }

  strncpy(dst1, src, split_pos);
  dst1[split_pos] = '\0';

  const char *remaining = src + split_pos;
  while (*remaining && isspace(*remaining))
  {
    remaining++;
  }

  strncpy(dst2, remaining, 16);
  dst2[16] = '\0';
}

char lcd_lines[2][17];

void loop()
{
  sleep_run_from_xosc();
  sleep_goto_dormant_until_edge_high(BUTTON_PIN);
  Serial.begin(4800);
  setup_button();
  setup_relay();

  relay_on();
  setup_lcd();

  Serial.println("Wake up!");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("It's Valentine's");
  lcd.setCursor(0, 1);
  lcd.print("Day! I love you");

  delay(5000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("In fact, I love");
  lcd.setCursor(0, 1);
  lcd.print("you so much...");

  pushed = false;

  for (int i = 0; i < 1200; i++)
  {
    buttonState = digitalRead(BUTTON_PIN);

    if (buttonState == HIGH)
    {
      if (!pushed)
      {
        pushed = true;
        break; // back to sleep
      }
    }
    else
    {
      if (pushed)
      {
        pushed = false;
      }
    }

    delay(5);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("and I want to");
  lcd.setCursor(0, 1);
  lcd.print("share 50 things");

  for (int i = 0; i < 1200; i++)
  {
    buttonState = digitalRead(BUTTON_PIN);

    if (buttonState == HIGH)
    {
      if (!pushed)
      {
        pushed = true;
        break; // back to sleep
      }
    }
    else
    {
      if (pushed)
      {
        pushed = false;
      }
    }

    delay(5);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("I love about you");
  lcd.setCursor(0, 1);
  lcd.print("    enjoy <3");

  for (int i = 0; i < 1200; i++)
  {
    buttonState = digitalRead(BUTTON_PIN);

    if (buttonState == HIGH)
    {
      if (!pushed)
      {
        pushed = true;
        break; // back to sleep
      }
    }
    else
    {
      if (pushed)
      {
        pushed = false;
      }
    }

    delay(5);
  }

  for (auto &lovable_thing : lovable_things)
  {
    lcd.clear();

    split_str(lovable_thing, lcd_lines[0], lcd_lines[1]);

    lcd.setCursor(0, 0);
    lcd.print(lcd_lines[0]);
    lcd.setCursor(0, 1);
    lcd.print(lcd_lines[1]);

    for (int i = 0; i < 400; i++)
    {
      buttonState = digitalRead(BUTTON_PIN);

      if (buttonState == HIGH)
      {
        if (!pushed)
        {
          pushed = true;
          break; // back to sleep
        }
      }
      else
      {
        if (pushed)
        {
          pushed = false;
        }
      }

      delay(5);
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Thank you for");
  lcd.setCursor(0, 1);
  lcd.print("everything <3");

  delay(10000);

  relay_off();
}