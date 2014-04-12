A C++ library to send/receive commands from/to metatrader.
================================
1) Put the mql_socket.dll in you experts/library MetaTrader 4 Folder. <br>
2) Put the SAMPLE.mq4 in you experts MetaTrader 4 Folder. <br>
3) Run you application in the port 20007 of 127.0.0.1 (you computer)  <br>
4) Compile and run SAMPLE.mq4 and wait for packets you C++ Application  <br>
5) After receive packet in you application immediately send back a string with \n example "9\n"  <br>
6) After sending and receiving text strings create you protocol for communication between MetaTrader 4 and you C++ application over network <br>

How to use in you EA
================================
Build using Visual Studio 8. <br>
Remember to change the dll location to the experts\library folder for you Metatrader 4 folder.<br>
<br>
= Compile the *.mq4 =<br>
<br>
The file SAMPLE.mq4 is a test EA.<br>
The EA send the data and receive in procedural way, you need to send back data to EA after you received data, <br>
for simple test send "0\n" each time the EA send data for you C++ application.<br>
