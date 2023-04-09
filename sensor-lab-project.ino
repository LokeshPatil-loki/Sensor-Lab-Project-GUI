#include <ArduinoJson.h>
#include <IRremote.h>

const int SEND_PIN = 6;
const int RECV_PIN = 11;

IRsend irsend(SEND_PIN);
IRrecv irrecv(RECV_PIN);

bool transmitMode = false;

StaticJsonDocument<256> doc;

void setup() {
  Serial.begin(9600);
  Serial.println(transmitMode ? "In Transmit Mode" : "In Recieve Mode");
  irrecv.enableIRIn();
}

void loop() {
  if (Serial.available() > 0) {
    // read string from serial
    String str = Serial.readString();

    if (str == "read") {
      transmitMode = false;
    } else if (str == "send") {
      transmitMode = true;
    } else if (transmitMode) {
      // deserialize string into JSON object
      sendSignal(str);
    }

  }else if(!transmitMode){
    recieveSignal();
  }
}

void sendSignal(String str) {
  deserializeJson(doc, str);
  int protocol = doc["Protocol"];
  uint32_t raw_data = doc["Raw Data"];
  if (doc["Protocol"] == 18) {
    uint32_t key1 =  12582915;
    irsend.sendRC6(raw_data, (uint8_t)24);

  }
  doc.clear();
}

void recieveSignal() {
//  Serial.println("Into Recieve Mode");

  if (irrecv.decode()) {
    Serial.print("{'Raw Data':");
    Serial.print(irrecv.decodedIRData.decodedRawData);
    Serial.print(",'len':");
    Serial.print(irrecv.decodedIRData.rawDataPtr->rawlen);
    Serial.print(",'Protocol':");
    Serial.print(irrecv.decodedIRData.protocol);
    Serial.print(",'Command':");
    Serial.print(irrecv.decodedIRData.command);
    Serial.print(",'Address':");
    Serial.print(irrecv.decodedIRData.address);
    Serial.print(",'Bits':");
    Serial.print(irrecv.decodedIRData.numberOfBits);
    Serial.print(",'Protocol Type':'");
    printProtocol(irrecv.decodedIRData.protocol);
    Serial.print("'}");
    Serial.println("");
    irrecv.resume();
  }
}




void printProtocol(int irProtocol) {
  switch (irProtocol) {
    case UNKNOWN:
      Serial.print("UNKNOWN");
      break;
    case PULSE_WIDTH:
      Serial.print("PULSE_WIDTH");
      break;
    case PULSE_DISTANCE:
      Serial.print("PULSE_DISTANCE");
      break;
    case APPLE:
      Serial.print("APPLE");
      break;

    case DENON:
      Serial.print("DENON");
      break;

    case JVC:
      Serial.print("JVC");
      break;

    case LG:
      Serial.print("LG");
      break;

    case LG2:
      Serial.print("LG2");
      break;

    case NEC:
      Serial.print("NEC");
      break;

    case NEC2:
      Serial.print("NEC2");
      break;

    case ONKYO:
      Serial.print("ONKYO");
      break;

    case PANASONIC:
      Serial.print("PANASONIC");
      break;

    case KASEIKYO:
      Serial.print("KASEIKYO");
      break;

    case KASEIKYO_DENON:
      Serial.print("KASEIKYO_DENON");
      break;

    case KASEIKYO_SHARP:
      Serial.print("KASEIKYO_SHARP");
      break;

    case KASEIKYO_JVC:
      Serial.print("KASEIKYO_JVC");
      break;

    case KASEIKYO_MITSUBISHI:
      Serial.print("KASEIKYO_MITSUBISHI");
      break;

    case RC5:
      Serial.print("RC5");
      break;

    case RC6:
      Serial.print("RC6");
      break;

    case SAMSUNG:
      Serial.print("SAMSUNG");
      break;

    case SAMSUNG48:
      Serial.print("SAMSUNG48");
      break;

    case SAMSUNG_LG:
      Serial.print("SAMSUNG_LG");
      break;

    case SHARP:
      Serial.print("SHARP");
      break;

    case SONY:
      Serial.print("SONY");
      break;

    case BANG_OLUFSEN:
      Serial.print("BANG_OLUFSEN");
      break;

    case BOSEWAVE:
      Serial.print("BOSEWAVE");
      break;

    case LEGO_PF:
      Serial.print("LEGO_PF");
      break;

    case MAGIQUEST:
      Serial.print("MAGIQUEST");
      break;

    case WHYNTER:
      Serial.print("WHYNTER");
      break;
  }
}
