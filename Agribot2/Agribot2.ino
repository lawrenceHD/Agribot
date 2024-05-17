#include <LiquidCrystal.h>
#include <SD.h>

#define echo 9
#define trigger 10
#define tank_pump 4
#define watering_pump 13
#define moisture_sensor A0

long duration;
int distance;
int moisture_value;
int distance_percent;
int moist_percent;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
File dataFile;

void setup() {
  lcd.begin(16, 2);
  pinMode(echo, INPUT);
  pinMode(moisture_sensor, INPUT);
  pinMode(trigger, OUTPUT);
  digitalWrite(trigger, LOW);
  pinMode(watering_pump, OUTPUT);
  pinMode(tank_pump, OUTPUT);
  digitalWrite(watering_pump, LOW);
  digitalWrite(tank_pump, LOW);
  lcd.setCursor(0, 0);
  lcd.print("IRRIGATION PROJECT");
  lcd.setCursor(0, 1);
  lcd.print("GROUP 3");
  delay(2000);
  lcd.clear();

  Serial.begin(9600);

  if (!SD.begin(4)) {
    Serial.println("Erreur lors de l'initialisation de la carte SD");
    return;
  }

  dataFile = SD.open("data.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println("TANK LEVEL, MOIST CONTENT, W-PUMP STATUS, T-PUMP STATUS");
    dataFile.close();
  } else {
    Serial.println("Erreur lors de l'ouverture du fichier");
  }
}

void loop() {

  // LEVEL SENSOR
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  duration = pulseIn(echo, HIGH);
  distance = duration * 0.034 / 2;
  distance_percent = map(distance, 0, 1023, 0, 100);
  moisture_value = analogRead(moisture_sensor);
  moist_percent = map(moisture_value, 0, 1023, 0, 100);
  condition();

  //Write data to LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tank Level:");
  lcd.setCursor(12, 0);
  lcd.print(distance_percent);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Moisture:");
  lcd.setCursor(11, 1);
  lcd.print(moist_percent);
  lcd.print("%");

  lcd.setCursor(0, 2);
  lcd.print("W-Pump:");
  lcd.setCursor(9, 2);
  if (digitalRead(watering_pump) == HIGH) {
    lcd.print("ON");
  } else {
    lcd.print("OFF");
  }

  lcd.setCursor(0, 3);
  lcd.print("T-Pump:");
  lcd.setCursor(9, 3);
  if (digitalRead(tank_pump) == HIGH) {
    lcd.print("ON");
  } else {
    lcd.print("OFF");
  }

  // Write data to file
  dataFile = SD.open("data.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.print(distance_percent);
    dataFile.print(",");
    dataFile.print(moist_percent);
    dataFile.print(",");
    if (digitalRead(watering_pump) == HIGH) {
      dataFile.print("ON");
    } else {
      dataFile.print("OFF");
    }
    dataFile.print(",");
    if (digitalRead(tank_pump) == HIGH) {
      dataFile.println("ON");
    } else {
      dataFile.println("OFF");
    }
    dataFile.close();
  } else {
    Serial.println("Erreur lors de l'ouverture du fichier");
  }
}

void condition() {
  if (distance_percent < 30) {
    digitalWrite(tank_pump, HIGH);
    delay(1000);
    digitalWrite(tank_pump, LOW);
  }
  if (moist_percent < 40) {
    digitalWrite(watering_pump, HIGH);
    delay(5000);
    digitalWrite(watering_pump, LOW);
    sms();
  }
}
