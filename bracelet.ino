#include <Wire.h>
#include <Adafruit_LSM303.h>
#include <Adafruit_NeoPixel.h>

// ----------------------------------------------
// Set up the two strips
// ----------------------------------------------
#define STRIP1PIN 6  // Pin numbers
#define STRIP2PIN 0

#define STRIP1NUM 2  // Number of pixels
#define STRIP2NUM 1
#define TOTALNUM (STRIP1NUM+STRIP2NUM)

#define MAXACCEL 255  // Limit acceleration values to this
#define THRESHOLD 10  // Required an acceleration change larger than this

// Create the strip objects
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(STRIP1NUM, STRIP1PIN);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(STRIP2NUM, STRIP2PIN);

// Accelerometer object
Adafruit_LSM303 lsm;
int x=0, y=0, z=0;
//char out[100];

// Initialize
void setup() {
  //Serial.begin(9600);
  
  strip1.begin();
  strip2.begin();
  strip1.show(); 
  strip2.show(); 
  
  lsm.begin();
}

void loop() {
  
  while (1) {
    lsm.read();
    uint8_t new_x = min(abs((int)lsm.accelData.x), MAXACCEL);
    uint8_t new_y = min(abs((int)lsm.accelData.y), MAXACCEL);
    uint8_t new_z = min(abs((int)lsm.accelData.z), MAXACCEL);

    uint8_t dx = abs(x-new_x);
    uint8_t dy = abs(y-new_y);
    uint8_t dz = abs(z-new_z);
  
    if (dx <= THRESHOLD && dy <= THRESHOLD && dz <= THRESHOLD) {
      for (uint8_t p=0; p<TOTALNUM; ++p)
        setColor(p, 0, 0, 0);
      continue;
    }

    uint8_t red = (uint8_t)(dx*255/MAXACCEL);
    uint8_t green = (uint8_t)(dy*255/MAXACCEL);
    uint8_t blue = (uint8_t)(dz*255/MAXACCEL);

    x = new_x;
    y = new_y;
    z = new_z;
    
    // Modify a random pixel
    uint8_t pixel = rand() % TOTALNUM;
    //sprintf(out, "px %d: rd=%d, gn=%d, bl=%d\n", pixel, red, green, blue);
    //Serial.print(out);
    setColor(pixel, red, green, blue);
    break;
  }
  delay(100);
}

void setColor(uint8_t pixel, uint8_t red, uint8_t green, uint8_t blue) {
  Adafruit_NeoPixel strip = strip1;
  if (pixel >= STRIP1NUM) {
    strip = strip2;
    pixel -= STRIP1NUM;
  }
  strip.setPixelColor(pixel, red, green, blue);
  strip.show();
}


