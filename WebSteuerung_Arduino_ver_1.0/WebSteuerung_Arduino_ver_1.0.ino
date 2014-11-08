// Arduino WebServer Steuerung
// geschrieben von Sebastian Bohn, Marko Vukadinovic, Florian Langhorst, Johannes Bagge
// Projekt für die HAW Hamburg
// version 1.0



#include <SPI.h>
#include <Ethernet.h> 

//Schalter mit Adressen deklarieren
int a_ON = 2;
int a_OFF = 3;
int b_ON = 5;
int b_OFF = 6;
int c_ON = 8;
int c_OFF = 9;

boolean a_enabled = false;
boolean b_enabled = false;
boolean c_enabled = false;

//---------------HEIM-Netz--------------------------
// Deklaration der MAC-Adresse des Ethernet-Ports
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// Deklaration einer statischen IP-Adresse innerhalb des Heimnetzwerks
byte ip[] = { 192,168,178,65 }; 
// DNS
byte dns1[] = {192,168,178,65};
// Gateway
byte gateway[] = { 192,168,178,1 }; 
// Subnetz-Maske
byte subnet[] = { 255, 255, 255, 0 };
/*
//---------------HAW NETZ--------------------------
// Deklaration der MAC-Adresse des Ethernet-Ports
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// Deklaration einer statischen IP-Adresse innerhalb des Heimnetzwerks
byte ip[] = { 141,22,50,235 }; 
// DNS
byte dns1[] = {141,22,50,74};
// Gateway
byte gateway[] = { 141,22,50,1 }; 
// Subnetz-Maske
byte subnet[] = { 255, 255, 254, 0 };
*/

// Default-Port 80
EthernetServer server(80);
EthernetClient client;

// String zum lesen der URL
String readString;
// login variablen
String user = "?id=Admin";
String password = "&pass=1234";
boolean security = false;


void setup() {
 // initialisieren des Serial Monitors
  Serial.begin(9600);
   while (!Serial) {
    ; // warten bis der Serial-Port verbunden ist
  }
  
  // PINs als Ausgänge setzen
  
  pinMode(a_ON, OUTPUT);
  pinMode(a_OFF, OUTPUT);
  pinMode(b_ON, OUTPUT);
  pinMode(b_OFF, OUTPUT);
  pinMode(c_ON, OUTPUT);
  pinMode(c_OFF, OUTPUT);
  
  // Pegel der PINs auf HIGH setzen, da die Relais auf LOW-Pegel schalten
  digitalWrite(a_ON, HIGH);
  digitalWrite(a_OFF, HIGH);
  digitalWrite(b_ON, HIGH);
  digitalWrite(b_OFF, HIGH);  
  digitalWrite(c_ON, HIGH);
  digitalWrite(c_OFF, HIGH); 
  
  // starten der Ethernet-Verbindung und des Servers
  Ethernet.begin(mac, ip, dns1, gateway, subnet);
  server.begin();
  
  // Ausgabe der Adresse
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

// Logout schaltet alle Schalter aus
}
void shutdownAllSwitches(){
   digitalWrite(a_OFF, LOW);
   digitalWrite(b_OFF, LOW);
   digitalWrite(c_OFF, LOW);
   delay(500);
   digitalWrite(a_OFF,HIGH);
   digitalWrite(b_OFF,HIGH);
   digitalWrite(c_OFF,HIGH);
   a_enabled = false;
   b_enabled = false;
   c_enabled = false;
}

// Seite für Pin-Control
void control(){
           
           client.println("HTTP/1.1 200 OK");
           client.println("Content-Type: text/html");
           client.println();     
           client.println("<HTML>");
           client.println("<HEAD>");
           client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
           client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
           client.println("<link rel='stylesheet' type='text/css' href='http://randomnerdtutorials.com/ethernetcss.css' />");
           client.println("<TITLE>HAW Hamburg</TITLE>");
           client.println("</HEAD>");
           client.println("<BODY>");
           client.println("<H1>Arduino Home Control</H1>");
           client.println("<hr />");
           client.println("<br />");  
           client.println("<br />");  
           client.println("<H2>Pflanzenbew&aumlsserung</H2>");
           client.println("<br />");
           client.println("<a href=\"/?aON\"\">Schalter A ON</a>");
           client.println("<a href=\"/?aOFF\"\">Schalter A OFF</a><br />");   
           if(a_enabled){
             client.println("<p style= background-color:#00FF00>ON</p>");
           }
           else{
             client.println("<p style= background-color:#FF0000>OFF</p>");
           }
              
           client.println("<br />");
           client.println("<H2>Licht</H2>");
           client.println("<br />");
           client.println("<a href=\"/?bON\"\">Schalter B ON</a>");
           client.println("<a href=\"/?bOFF\"\">Schalter B OFF</a><br />");
           if(b_enabled){
             client.println("<p style= background-color:#00FF00>ON</p>");
           }
           else{
             client.println("<p style= background-color:#FF0000>OFF</p>");
           }
           
           client.println("<br />");
           client.println("<H2>Kaffemaschine</H2>");
           client.println("<br />"); 
           client.println("<a href=\"/?cON\"\">Schalter C ON</a>");
           client.println("<a href=\"/?cOFF\"\">Schalter C OFF</a><br />");
           if(c_enabled){
               client.println("<p style= background-color:#00FF00>ON</p>");
             }
             else{
               client.println("<p style= background-color:#FF0000>OFF</p>");
             }  
            
           client.println("<br />"); 
           client.println("<br />"); 
           client.println("<a href=\"/?logout\"\">Logout</a>");
           client.println("</BODY>");
           client.println("</HTML>");
           
           }
         // Startseite und Login-Page  
         void login(){
           if(!security){
           client.println("HTTP/1.1 200 OK");
           client.println("Content-Type: text/html");
           client.println();     
           client.println("<HTML>");
           client.println("<HEAD>");
           client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
           client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
           client.println("<link rel='stylesheet' type='text/css' href='http://randomnerdtutorials.com/ethernetcss.css' />");
           client.println("<TITLE>Arduino Home Control</TITLE>");
           client.println("</HEAD>");
           client.println("<H1>Arduino Home Control</H1>");
           client.println("<hr />");
           client.println("<br />");
           client.println(" <form class='form-signin' role='form'>");
           client.println(" <h2 class='form-signin-heading'>Login</h2>");  
           
           client.println("<input type='id' name='id' class='form-control' placeholder='Benutzer' required autofocus>");     
           client.println("<br/>");
           client.println("<br/>");
           client.println(" <input type='password' name='pass' placeholder='Passwort' required>"); 
           client.println("<br/>");
           client.println("<br/>");
           client.println("<input type='submit' value='Login'>");     
           client.println("<br />"); 

           client.println(" </form>");  
           client.println("<br />"); 
           client.println("</BODY>");
           client.println("</HTML>");
           
           }
           
         }
      
      // Methode für String-Vergleiche
      void stringReader(){
         //prüfe ob Username und Passwort stimmen
           if (readString.indexOf(user+password)>0){       
             security = true;     
           }
           // Schalter AN und AUS
           if (readString.indexOf("?aON") >0){
               a_enabled = true;
               digitalWrite(a_ON, LOW);
               delay(500);
               digitalWrite(a_ON,HIGH);
               
           }
           if (readString.indexOf("?aOFF") >0){
               a_enabled = false;
               digitalWrite(a_OFF, LOW);
               delay(500);
               digitalWrite(a_OFF,HIGH);
               
               
           }
           if (readString.indexOf("?bON") >0){
               b_enabled = true;
               digitalWrite(b_ON, LOW);
               delay(500);
               digitalWrite(b_ON,HIGH);
               
           }
           if (readString.indexOf("?bOFF") >0){
               b_enabled = false;
               digitalWrite(b_OFF, LOW);
               delay(500);
               digitalWrite(b_OFF,HIGH);
               
           }
           if (readString.indexOf("?cON") >0){
               c_enabled = true;
               digitalWrite(c_ON, LOW);
               delay(500);
               digitalWrite(c_ON,HIGH);
              
           }
           if (readString.indexOf("?cOFF") >0){
               c_enabled = false;
               digitalWrite(c_OFF, LOW);
               delay(500);
               digitalWrite(c_OFF,HIGH);
               
           }
           // Logout und zurück zum Login
           if (readString.indexOf("?logout")>0){       
             shutdownAllSwitches();
             security = false;     
           }
            //clearing string for next read
            readString="";
            
        }

void loop() {
  // Erstellen einer Client-Verbindung (Browser)
  client = server.available();
  // wenn die Verbindung besteht..
  if (client) {
    while (client.connected()) {   
      if (client.available()) {
        char c = client.read();
     
        // auslesen der Chars aus der HTTP
        if (readString.length() < 100) {
          // speichern der Zeichen in einen String
          readString += c;
          // debug Ausgabe
          //Serial.print(c);
          //Serial.println(readString);
         }

         //wenn HTTP Anfrage endet
         if (c == '\n') {
           // debug Ausgabe          
           //Serial.println(readString); 
           
           //URL String lesen
           stringReader();
           
           //Sicherheitsabfrage
           if(!security){
             login();
           }
           
           if(security){
             control();
           }
           
           
           Serial.println(readString);
             
           delay(1);
           //client stoppen
           client.stop();
           
         }
       }
    }
  }
}


