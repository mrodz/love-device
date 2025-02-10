#include <Arduino.h>
#include <pico/sleep.h>
#include <LiquidCrystal.h>

const int BUTTON_PIN = 12;
const int RELAY_PIN = 0;

int buttonState = 0;

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

void wake_up()
{
  // todo
}

const int rs = 16, en = 17, d4 = 18, d5 = 19, d6 = 20, d7 = 21;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup()
{
  Serial.begin(4800);
  setup_button();
  setup_relay();
}

volatile byte relayState = LOW;
long lastDebounceTime = 0;
long debounceDelay = 10000;

bool pushed = false;

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

const char *lovable_things[] = {
    "Your smile",
    "Your hair",
    "Your humor",
    "Your hugs",
    "Your eyes",
    "Your face",
    "Your nose",
    "How you comfort me",
    "Your stuffed animals",
};

void split_str(const char *src, char *dst1, char *dst2) {
  memset(dst1, 0, 17);
  memset(dst2, 0, 17);

  int len = strlen(src);

  if (len <= 16) {
      strncpy(dst1, src, len);
      dst1[len] = '\0';
      return;
  }

  int split_pos = 16;
  while (split_pos > 0 && !isspace(src[split_pos])) {
      split_pos--;
  }

  if (split_pos == 0) {
      split_pos = 16;
  }

  strncpy(dst1, src, split_pos);
  dst1[split_pos] = '\0';

  const char *remaining = src + split_pos;
  while (*remaining && isspace(*remaining)) {
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

  Serial.println("Wake up!");

  relay_on();

  lcd.begin(16, 2);

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

  relay_off();
}