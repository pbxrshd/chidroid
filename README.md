chidroid
========

C.H.I.Droid

limitFlags
motorFlags
otherFlags



need commands to:

  move up ^
  move dn v
  move lf >
  move rt <
  move in ;
  move ot :
  all stop ! or space
  
  neck servo ~
  gripper servo #
  gripper 
  
  
  no-op query ?
  
  all commands return status flags

move commands can specify no. of steps to move
step size can be set (per axis?)

set vars for:
 gripper open limit
 gripper close limit
 


~
`
!
@
#
$
%
^
&
*
(
)
{
}
[
]
<
>
_
-
+
=
|
\
/
"
'
:
;
?
.
,


check limit switches
update limit flags

update motor flags
update motor pins

check serial
update command flags
return flag statuses

byte limitPins[] = {3,5,7,8};

byte limitFlags = B10011110;

for (byte i = 0; i < sizeof(limitPins); i++) {
  
}

// serviceSerial checks the serial port and updates position with received data
// it expects servo data in the form:
// "180a" writes 180 to servo a
// "90b writes 90 to servo b
void serviceSerial()
{
static int pos = 0;
if ( Serial.available()) {
char ch = Serial.read();
}
}
if(ch >= '0' && ch <= '9')
// is ch a number?
pos = pos * 10 + ch - '0';
// yes, accumulate the value
else if(ch >= 'a' && ch <= 'a'+ SERVOS) // is ch a letter for our servos?
myservo[ch - 'a']

https://www.newbiehack.com/MicrocontrollersConnectorsforPrototyping.aspx


system has:
  limitA0
  limitA1
  controlling motorA

variables:
  flagA0 0|1
  flagA1 0|1
  motorADir 0|1 (fwd|rev)
  motorApwm 0-255

loop:
  if limitA0 tripped     set flagA0 = 1
  if limitA0 not tripped set flagA0 = 0
  if limitA1 tripped     set flagA1 = 1
  if limitA1 not tripped set flagA1 = 0  


http://www.google.com/patents/US4781305 (paper feeder patent)


java facedetection
http://cell0907.blogspot.com/2013/07/detecting-faces-in-your-webcam-stream.html

java serial port
http://www.lucid-control.com/how-to-access-serial-ports-in-java/
http://playground.arduino.cc/interfacing/java



JSSC:
 downloaded https://java-simple-serial-connector.googlecode.com/files/jSSC-2.6.0-Release.zip
 extracted jssc.jar from there
 https://code.google.com/p/java-simple-serial-connector/wiki/jSSC_examples