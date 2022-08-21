# Huge LED Clock
 Huge Seven Segment LED clock using Arduino

This is an old project that took quite a long time to figure out and build.
it is running a barebone Atmega328 chip with the Arduino bootloader
it makes use of Stanley Seow's project in the below link and Pauls 4 digit 7 segment shift register design

http://arduino-for-beginners.blogspot.com/2010/12/digital-clock-with-7-segments-led-and.html

http://www.sqlskills.com/blogs/paulselec/post/Arduino-cascading-shift-registers-to-drive-7-segment-displays.aspx

I had made a few adjustments in the circuit design to make a clock much larger and brighter, however I did come across a few complications.

I have used EEPROM to store the alarm functions and would rather use something that does not have a limited write cycle, any suggestions would be appreciated.
I am looking at using a SD card reader to store alarm funtions and possibly alarm songs/tunes

I don't know how to write libraries for arduino, any help on that would also be appreciated.

@stanleyseow
