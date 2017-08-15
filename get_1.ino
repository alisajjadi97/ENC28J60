// Demo using DHCP and DNS to perform a web client request.
// 2011-06-08 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include <EtherCard.h>
#include <stdio.h>
// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[700];
static uint32_t timer,timer2;
char str[50];
const char website[] PROGMEM = "22.sburc.ir";

// called when the client request is complete
static void my_callback (byte status, word off, word len) {
  //Serial.println(">>>");
  //Ethernet::buffer[off+300] = 0;
  Serial.print((const char*) Ethernet::buffer + off + 307);
}

void setup () {
  Serial.begin(57600);
  Serial.println(F("\n[webClient]"));

  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println(F("Failed to access Ethernet controller"));
  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);  

#if 1
  // use DNS to resolve the website's IP address
  if (!ether.dnsLookup(website))
    Serial.println("DNS failed");
#elif 2
  // if website is a string containing an IP address instead of a domain name,
  // then use it directly. Note: the string can not be in PROGMEM.
  char websiteIP[] = "192.168.1.1";
  ether.parseIp(ether.hisip, websiteIP);
#else
  // or provide a numeric IP address instead of a string
  byte hisip[] = { 192,168,1,1 };
  ether.copyIp(ether.hisip, hisip);
#endif
    
  ether.printIp("SRV: ", ether.hisip);
}

void loop () {
  ether.packetLoop(ether.packetReceive());
  
  if (millis() > timer) {
    timer = millis() + 8000;
    Serial.println();
    sprintf (str,"94242054?input=%d",timer);
    ether.browseUrl(PSTR("/connection/module/getrequest/94242054/"),str, website, my_callback);
  }
  if (millis() > timer2){
      timer2 = millis() + 5000;
      ether.browseUrl(PSTR("/connection/user/getrequest/94242054/"), "94242054?input=hello", website, my_callback);
  }
}
