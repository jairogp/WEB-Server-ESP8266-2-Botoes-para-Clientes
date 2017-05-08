  /* 
  Projeto original de Pedro Minatel.
  Retirado do site: http://pedrominatel.com.br/pt/arduino/web-server-com-esp8266-e-ide-arduino/
  Mudanças para reconexão em caso de perda do sinal wifi.
  Utilização de botão fisico para ligar ou desligar a saida.
  Com apoio do amigo Márcio torres na parte de reconexão.
  */

  #include <ESP8266WiFi.h>

  const char* ssid = "REDE";
  const char* password = "SENHA";
  WiFiServer server(80);

  void setup() {

  Serial.begin(9600);
  delay(10);
    
  // prepare GPIOs
  pinMode(D0, OUTPUT);
  digitalWrite(D0, 0);
  pinMode(D1, OUTPUT);
  digitalWrite(D1, 0);
  pinMode(D2, OUTPUT);
  digitalWrite(D2, 0);
  pinMode(D3, OUTPUT);
  digitalWrite(D3, 0);
  pinMode(D4, OUTPUT);
  digitalWrite(D4, 0);
    
  conectar();
  }

  void conectar() {
  Serial.println();
  Serial.println();
  Serial.print("Connectando a ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("Conectado com sucesso na rede.");
  
  // Start the server
  server.begin();
  Serial.println("IP Obtido: ");
  Serial.print(WiFi.localIP());
  }

  
  void loop() {
       
  if (WiFi.status() != WL_CONNECTED) {conectar();}  // Caso wifi tenha caido conecta novamente
  WiFiClient client = server.available();
  // Volta ao loop aguardando nova conexao do cliente.
  if (!client) { 
    return; 
  }     
  // um loop de 20 laços para nao ficar preso quando da erro na conexao client  problemas com o navegador Chrome           
  int conta = 0 ;
  while(conta < 20)            
  {
  if(!client.available()){ conta = conta +1;}
  else { conta = 20;}
  delay(1);
  } 
  // mesmo passando o laço e ainda nao teve conexao, para e retorna ao inicio eliminando erros de navegadores
  if (!client.available()) 
  { 
    client.stop();
    return;
  } 
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  String buf = "";
 
  buf += "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
  buf += "<html lang=\"pt\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>\r\n";
  buf += "<body style=background-color:#000000>";
  buf += "<title>WEB Server ESP 8266</title>";
  buf += "<h1><font color=#0000FF>CONTROLE DE RELES COM ESP8266</h1></font>";
  buf += "<font color=#FFFFFF>";
  buf += "<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;} input{width:80%;} body{text-align: center;font-family:verdana;} button{border:0;border-radius:0.3rem;background-color:#FFA500;color:#FFFFF;line-height:2.4rem;font-size:1.2rem;width:49%;} .q{float: right;width: 64px;text-align: right;}</style>";
  buf += "<p><h3>CANAL 01</h3></p> <a href=\"?function=D0_on\"><button>LIGA</button></a> <a href=\"?function=D0_off\"><button>DESLIGA</button></a></p>";
  buf += "<p><h3>CANAL 02</h3></p> <a href=\"?function=D1_on\"><button>LIGA</button></a> <a href=\"?function=D1_off\"><button>DESLIGA</button></a></p>";
  buf += "<p><h3>CANAL 03</h3></p> <a href=\"?function=D2_on\"><button>LIGA</button></a> <a href=\"?function=D2_off\"><button>DESLIGA</button></a></p>";
  buf += "<p><h3>CANAL 04</h3></p> <a href=\"?function=D3_on\"><button>LIGA</button></a> <a href=\"?function=D3_off\"><button>DESLIGA</button></a></p>";
  buf += "<p><h3>TODOS CANAIS</h3></p> <a href=\"?function=DT_on\"><button>LIGA</button></a> <a href=\"?function=DT_off\"><button>DESLIGA</button></a></p>";
  buf += "<p><h3>PULSO</h3></p> <a href=\"?function=D4_on\"><button>PULSO</button></a></p>";
    
  client.print(buf);
  delay(10);

  client.flush();

       if (req.indexOf("D0_on") != -1)
    {digitalWrite(D0, 1);}
  else if (req.indexOf("D0_off") != -1)
    {digitalWrite(D0, 0);}  
  else if (req.indexOf("D1_on") != -1)
    digitalWrite(D1, 1);
  else if (req.indexOf("D1_off") != -1)
    digitalWrite(D1, 0);
  else if (req.indexOf("D2_on") != -1)
    digitalWrite(D2, 1);
  else if (req.indexOf("D2_off") != -1)
    digitalWrite(D2, 0);
  else if (req.indexOf("D3_on") != -1)
    digitalWrite(D3, 1);
  else if (req.indexOf("D3_off") != -1)
    digitalWrite(D3, 0);
  else if (req.indexOf("DT_on") != -1)
    {digitalWrite(D0, 1);
    digitalWrite(D1, 1);
    digitalWrite(D2, 1);
    digitalWrite(D3, 1);}
  else if (req.indexOf("DT_off") != -1)
    {digitalWrite(D0, 0);
    digitalWrite(D1, 0);
    digitalWrite(D2, 0);
    digitalWrite(D3, 0);}
  else if (req.indexOf("D4_on") != -1)
    {digitalWrite(D4, 1);
    delay(300);
    digitalWrite(D4, 0);}
  else {
    Serial.println("invalid request");
    client.stop();
  }
  Serial.println("Client disonnected");
  }

