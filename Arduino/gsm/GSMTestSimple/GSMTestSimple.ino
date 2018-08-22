
#define SerialAT Serial2

HardwareSerial Serial2(2);

void setup() 
{
  Serial.begin(115200);
  delay(500);

  Serial.println("Inicializando o software");

  Serial2.begin(115200, SERIAL_8N1, 13, 14);
  delay(500);
}

void loop() 
{
    String response = gsmWrite("AT", "OK");
    
    if ( response == "FAIL" )
        return;//Se o GSM nao responder, retorna a funcao (trava o mcu)
    else
        Serial.printf("Resposta: %s", response.c_str() );
    
    delay( 1000 );
}

String gsmWrite(String snd, String rcv)//Funcao que envia dados pro GSM e espera a resposta de ate 30seg
{
    Serial.print("Enviando o comando ");
    Serial.println( snd );
    
    Serial2.println( snd );

    if (rcv.indexOf("+CMGS") > -1)
    {
      delay(150);
      Serial2.write(0x1A);
    }

    for (uint16_t i = 0; i < 1200; i++)
    {
      delay(25);
      if (Serial2.available())
      {
        delay(50);
        String a = Serial2.readString();
  
        if (a.indexOf(rcv) > -1 || rcv.length() == 0)
        {
          return a;
        }
      }
    }
  
    return "FAIL";
}
