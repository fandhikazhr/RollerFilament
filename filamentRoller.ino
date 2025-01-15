#include <Wire.h>
#include <Keypad.h>
#include <Stepper.h>
#include <LiquidCrystal_I2C.h>
const int stepsPerRevolution = 100;  // change this to fit the number of steps per revolution
// for your motor
// character for loading screen mode
byte five[] = {
  B11111,B11111,B11111,B11111,B11111,B11111,B11111,B11111};

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);
LiquidCrystal_I2C lcd(0x27,16,2);
const byte ROWS = 4;
const byte COLS = 4;
int n = 0;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {13, 12, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  lcd.init(); // initialize the lcd 
  lcd.backlight();
  // Inisialisasi loading screen
  lcd.createChar(5, five);
  loadingScreen();
  lcd.clear();
  // Inisialisasi Halaman Petunjuk Penggunaan
  petunjukPenggunaan();
  // set the speed at 60 rpm:
  myStepper.setSpeed(720);
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
  int n = getNumericInput();
  putaranStepper((n+5)*8);
}

void setStepperIdle() {
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
}

void putaranStepper(int n){
  for(int i=1; i<=n; i++){
    lcd.setCursor(0,0);
    lcd.print("Clockwise ");
    lcd.print(i);
    myStepper.step(stepsPerRevolution);
    setStepperIdle();
    if(i==n){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Filament ");
      lcd.print((n/8)-5);
      lcd.print("g");
      lcd.setCursor(0,1);
      lcd.print("Telah Digulung");
      delay(5000);
      // exit(0);
      setup();
      loop();
    } 
  }
}

int getNumericInput() {
  String inputString = "";
  char key;
  lcd.setCursor(0,1);
  while (key != '#') {
    key = keypad.getKey(); 
    if (key && (key >= '0' && key <= '9')) {
      inputString += key;
      lcd.print(key);
    } else if (key == '*') { // Jika tombol "*" ditekan, hapus karakter terakhir
      if (inputString.length() > 0) {
        inputString.remove(inputString.length() - 1);
        lcd.setCursor(inputString.length(), 1);
        lcd.print(" "); 
        lcd.setCursor(inputString.length(), 1); 
      }
    }
  }
  lcd.clear();
  return inputString.toInt();
}


void updateProgressBar(unsigned long count, unsigned long totalCount, int lineToPrintOn) {
    double factor = totalCount/80.0;          
    int percent = (count+1)/factor;
    int number = percent/5;
    int remainder = percent%5;
    if(number > 0)
    {
       lcd.setCursor(number-1,lineToPrintOn);
       lcd.write(5);
    }   
       lcd.setCursor(number,lineToPrintOn);
       lcd.write(remainder);   
 }

void loadingScreen() {
  for(int i = 0; i <= 100; i++){
      lcd.setCursor(0,0);
      lcd.print("Roller Filament");
      lcd.print("     ");
      updateProgressBar(i, 100,1);
    }
}

void petunjukPenggunaan(){
  lcd.setCursor(0,0);
  lcd.print("Mesin Roller");
  lcd.setCursor(0,1);
  lcd.print("Filament Auto");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Masukkan Dalam");
  lcd.setCursor(0,1);
  lcd.print("Satuan Gram");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("1 Meter(M) = 4g");
  lcd.setCursor(0,1);
  lcd.print("(n) Meter x 4g");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Jumlah Gram :");
  lcd.setCursor(0,1);
  lcd.print("    Gram");
}
