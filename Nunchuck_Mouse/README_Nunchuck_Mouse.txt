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

——————————————————————————————————————————————————————————————

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

——————————————————————————————————————————————————————————————
HARDWARE SETUP

http://todbot.com/blog/ has nice pictures and slides explaining i2c communication.  There is a slight variation in use of pins (as noted above).

So we have:

Looking at the end of the Nunchuck connector, (with the clip for the safety strap on the bottom)

Top Left 		SCK		Digital Pin 3
Top Right		GND		Ground Pin
Bottom L.		+ V		5 Volt Pin
Bottom R.		SDA		Digital Pin 2

Notes:
~ I used the safety strap to secure the end of the nunchuck to my desk so the wires don’t get pulled out.
~ I don’t have an adapter board like he describes. I just plug my wires into the end for now.
