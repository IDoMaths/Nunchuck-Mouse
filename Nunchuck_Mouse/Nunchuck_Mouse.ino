/*
   Radon Chatterton
   radon@cfangus.com

   Adapted from:
   wii_nunchuck_sevo -- Use a Wii Nunchuck to control a servo
   Tod E. Kurt, http://todbot.com/blog/

   The Wii Nunchuck reading code is taken from Windmeadow Labs
   http://www.windmeadow.com/node/42

   todbot had some slides on how to hook things up.
   Notes: that those diagrams are for the uno (which doesn't have keyboard and mouse capability).
   You will have to use Leonardo or Micro. They have different pin mapping, you will have to use digital pins 2 & 3 for SDA & SDC.
   I have only tested this on the Micro.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
//Includes
//#include <Keyboard.h> // You can use this to make automating text entry and what not
#include <Mouse101.h> // This would normally be <Mouse.h> but I updated and it was throwing errors so I remained some things
#include <Wire.h>

// Global Vars
uint8_t outbuf[6];    // array to store Arduino output
int cnt = 0; // Used to make sure all the data is received from the wii remote
int t = 0;  // when it gets to READ_FREQ, read nunchuck
bool zLast[3] = {1, 1, 1}; // Used to smooth the mouse button operation, yours may work without it
bool cLast[3] = {1, 1, 1}; // Used to smooth the mouse button operation, yours may work without it
int SENSE_MULT = 1; // A sensitivity multiplier for the mouse movement
int READ_FREQ = 15; // Delay between reads, in milliseconds


void setup() {
  Wire.begin ();    // join i2c bus with address 0x52
  nunchuck_init (); // send the initialization handshake
}


void nunchuck_init()
{
  Wire.beginTransmission (0x52);  // transmit to device 0x52
  Wire.write (0x40);   // sends memory address
  Wire.write (0x00);   // sends sent a zero.
  Wire.endTransmission ();  // stop transmitting
}


void send_zero()
{
  Wire.beginTransmission (0x52);  // transmit to device 0x52
  Wire.write(0x00);   // sends one byte
  Wire.endTransmission ();  // stop transmitting
}


void loop() {
  t++;
  if ( t == READ_FREQ ) {
    t = 0;
    Wire.requestFrom (0x52, 6); // request data from nunchuck
    while (Wire.available ()) {
      // receive byte as an integer
      outbuf[cnt] = nunchuk_decode_byte (Wire.read ());
      cnt++;
    }
    // If we recieved the 6 bytes, then go print them
    if (cnt >= 5) {
      printNunchuckData();            // Moves mouse
      float tilt = outbuf[4];             // z-axis, in this case ranges from ~75 - ~185
      tilt = (tilt - 70) * 1.5;           // convert to degrees angle, approximately
    }
    cnt = 0;
    send_zero(); // send the request for next bytes
  }
  delay(1);
}


void printNunchuckData()
{
  int joy_x_axis = outbuf[0];
  int joy_y_axis = outbuf[1];
  int accel_x_axis = outbuf[2]; // * 2 * 2;
  int accel_y_axis = outbuf[3]; // * 2 * 2;
  int accel_z_axis = outbuf[4]; // * 2 * 2;
  int z_button = 0;
  int c_button = 0;
  // byte outbuf[5] contains bits for z and c buttons
  // it also contains the least significant bits for the accelerometer data
  // so we have to check each bit of byte outbuf[5]
  if ((outbuf[5] >> 0) & 1)
    z_button = 1;
  if ((outbuf[5] >> 1) & 1)
    c_button = 1;
  if ((outbuf[5] >> 2) & 1)
    accel_x_axis += 2;
  if ((outbuf[5] >> 3) & 1)
    accel_x_axis += 1;
  if ((outbuf[5] >> 4) & 1)
    accel_y_axis += 2;
  if ((outbuf[5] >> 5) & 1)
    accel_y_axis += 1;
  if ((outbuf[5] >> 6) & 1)
    accel_z_axis += 2;
  if ((outbuf[5] >> 7) & 1)
    accel_z_axis += 1;

  /* Output for testing
    Note: to use this you will have to comment out the 'Calculate and
    Write' section below and I believe #include <Mouse.h> will need to
    be commented out as well.
  */
  /*
    Serial.print (joy_x_axis, DEC);
    Serial.print ("\t");
    Serial.print (joy_y_axis, DEC);
    Serial.print ("\t");

    Serial.print (accel_x_axis, DEC);
    Serial.print ("\t");
    Serial.print (accel_y_axis, DEC);
    Serial.print ("\t");
    Serial.print (accel_z_axis, DEC);
    Serial.print ("\t");

    Serial.print (z_button, DEC);
    Serial.print (" ");
    Serial.print (c_button, DEC);

    Serial.print ("\r\n");
  */
  // Calculate and Write

  cLast[0] = cLast[1];
  cLast[1] = cLast[2];
  cLast[2] = c_button;
  zLast[0] = zLast[1];
  zLast[1] = zLast[2];
  zLast[2] = z_button;
  Mouse.begin();
  //Mouse movement
  Mouse.move(stickWrite(joy_x_axis*SENSE_MULT), -stickWrite(joy_y_axis*SENSE_MULT), 0);
  //Mouse press and release for left button (c on wii remote)
  if (cLast[0] == 1 && cLast[1] == 1 && cLast[2] == 0 && c_button == 0) {
    Mouse.press(MOUSE_LEFT);
  }
  if (cLast[0] == 0 && cLast[1] == 0 && cLast[2] == 1 && c_button == 1) {
    Mouse.release(MOUSE_LEFT);
  }
  //Mouse press and release for right button (z on wii remote)
  if (zLast[0] == 1 && zLast[1] == 1 && zLast[2] == 0 && z_button == 0) {
    Mouse.press(MOUSE_RIGHT);
  }
  if (zLast[0] == 0 && zLast[1] == 0 && zLast[2] == 1 && z_button == 1) {
    Mouse.release(MOUSE_RIGHT);
  }
  Mouse.end();
}
// Calculate the Mouse.move() value based on joystick position
int stickWrite(int jVal) {
  if (jVal <= 25) return 0;
  if (jVal <= 44) return -21;
  if (jVal <= 59) return -17;
  if (jVal <= 74) return -13;
  if (jVal <= 89) return -10;
  if (jVal <= 104) return -4;
  if (jVal <= 111) return -3;
  if (jVal <= 119) return -2;
  if (jVal <= 135) return 0;
  if (jVal <= 155) return 2;
  if (jVal <= 162) return 3;
  if (jVal <= 171) return 4;
  if (jVal <= 187) return 10;
  if (jVal <= 203) return 13;
  if (jVal <= 219) return 17;
  if (jVal <= 235) return 21; // was 230, my nunchuck is having issues so I changed this
  return 0;
}


// Encode data to format that most wiimote drivers except
// only needed if you use one of the regular wiimote drivers
char nunchuk_decode_byte (char x)
{
  x = (x ^ 0x17) + 0x17;
  return x;
}
