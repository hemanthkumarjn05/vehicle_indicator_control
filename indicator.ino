#define LEFT_BUTTON 18
#define RIGHT_BUTTON 19
#define HAZARD_BUTTON 23  
#define LEFT_LED 21
#define RIGHT_LED 22

bool leftOn = false;
bool rightOn = false;
bool hazardOn = false;
bool ledBlinkState = false; 

unsigned long lastSchedulerTime = 0;
unsigned long lastBlinkTime = 0;
unsigned long leftPressStart = 0;
unsigned long rightPressStart = 0;
unsigned long hazardPressStart = 0; 
unsigned long systemStartTime = 0;

bool leftPressedLast = false;
bool rightPressedLast = false;
bool hazardPressedLast = false; 

const unsigned long TOGGLE_DEBOUNCE_DELAY = 200;
unsigned long lastHazardButtonToggleTime = 0; 

enum HazardSource
NONE,             
DEDICATED_BUTTON, 
COMBO_BUTTONS     
;
HazardSource currentHazardSource = NONE; 

void setup() 
{
  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);
  pinMode(HAZARD_BUTTON, INPUT_PULLUP); 

  pinMode(LEFT_LED, OUTPUT);
  pinMode(RIGHT_LED, OUTPUT);

  digitalWrite(LEFT_LED, LOW);
  digitalWrite(RIGHT_LED, LOW);

  Serial.begin(115200);

  delay(500);  

  leftPressedLast = digitalRead(LEFT_BUTTON) == LOW;
  rightPressedLast = digitalRead(RIGHT_BUTTON) == LOW;
  hazardPressedLast = digitalRead(HAZARD_BUTTON) == LOW;

  systemStartTime = millis();

  leftPressStart = leftPressedLast ? systemStartTime : 0;
  rightPressStart = rightPressedLast ? systemStartTime : 0;
  hazardPressStart = hazardPressedLast ? systemStartTime : 0; 

  Serial.println("System Initialized and Ready.");
}

void updateLEDs() 
{
  int ledVal = ledBlinkState ? HIGH : LOW; 

  if (hazardOn) 
  {
    digitalWrite(LEFT_LED, ledVal);
    digitalWrite(RIGHT_LED, ledVal);
  } 
  else if (leftOn) 
  {
    digitalWrite(LEFT_LED, ledVal);
    digitalWrite(RIGHT_LED, LOW);
  } 
  else if (rightOn) 
  {
    digitalWrite(RIGHT_LED, ledVal);
    digitalWrite(LEFT_LED, LOW);
  } 
  else 
  {
    digitalWrite(LEFT_LED, LOW);
    digitalWrite(RIGHT_LED, LOW);
  }
}

void handleButtons(unsigned long currentTime) 
{
  if (currentTime - systemStartTime < 1000)
  {
    leftPressedLast = digitalRead(LEFT_BUTTON) == LOW;
    rightPressedLast = digitalRead(RIGHT_BUTTON) == LOW;
    hazardPressedLast = digitalRead(HAZARD_BUTTON) == LOW;
    return; 
  }

  bool leftPressedNow = digitalRead(LEFT_BUTTON) == LOW;
  bool rightPressedNow = digitalRead(RIGHT_BUTTON) == LOW;
  bool hazardPressedNow = digitalRead(HAZARD_BUTTON) == LOW;

  if (leftPressedNow && !leftPressedLast)
  {
    leftPressStart = currentTime;
    Serial.println("Left button was just pressed.");
  }
  if (rightPressedNow && !rightPressedLast) 
  {
    rightPressStart = currentTime;
    Serial.println("Right button was just pressed.");
  }
  if (hazardPressedNow && !hazardPressedLast)
  {
    hazardPressStart = currentTime;
    Serial.println("Dedicated Hazard button was just pressed.");
  }

  if (!hazardPressedNow && hazardPressedLast) 
  {
    if (currentTime - lastHazardButtonToggleTime > TOGGLE_DEBOUNCE_DELAY) 
    {
      lastHazardButtonToggleTime = currentTime; 

      hazardOn = !hazardOn;
      currentHazardSource = hazardOn ? DEDICATED_BUTTON : NONE;

      leftOn = false;  
      rightOn = false;
      Serial.println(hazardOn ? "Hazard Lights ACTIVATED by Dedicated Button." : "Hazard Lights DEACTIVATED by Dedicated Button.");
    }
  }

  if (leftPressedNow && rightPressedNow &&
      (currentTime - leftPressStart > 1000) && 
      (currentTime - rightPressStart > 1000)) 
  {
    if (!hazardOn || currentHazardSource == COMBO_BUTTONS) 
    {
      if (!hazardOn) 
      {
        Serial.println("Hazard Lights ACTIVATED by Left+Right Button Combo.");
      }
      hazardOn = true;
      currentHazardSource = COMBO_BUTTONS;
      leftOn = false;
      rightOn = false;
    }
  } 
  else 
  {
    if (hazardOn && currentHazardSource == COMBO_BUTTONS) 
    {
      hazardOn = false;
      currentHazardSource = NONE;
      Serial.println("Hazard Lights DEACTIVATED: Left+Right Combo Released.");
    }
  }

  if (hazardOn)
  {
    leftPressedLast = leftPressedNow;
    rightPressedLast = rightPressedNow;
    hazardPressedLast = hazardPressedNow;
    return; 
  }

  if (!leftPressedNow && leftPressedLast)
  {
    leftOn = !leftOn;
    rightOn = false; 
    Serial.println(leftOn ? "Left Indicator ON." : "Left Indicator OFF.");
  }

  if (!rightPressedNow && rightPressedLast) 
  {
    rightOn = !rightOn;
    leftOn = false; 
    Serial.println(rightOn ? "Right Indicator ON." : "Right Indicator OFF.");
  }

  leftPressedLast = leftPressedNow;
  rightPressedLast = rightPressedNow;
  hazardPressedLast = hazardPressedNow;
}

void loop()
{
  unsigned long currentTime = millis();

  if (currentTime - lastSchedulerTime >= 100) 
  {
    lastSchedulerTime = currentTime;
    handleButtons(currentTime);
  }

  if (currentTime - lastBlinkTime >= 300) 
  {
    lastBlinkTime = currentTime;
    ledBlinkState = !ledBlinkState; // Toggle blink state
    updateLEDs(); // Update LEDs based on current state
  }
}
