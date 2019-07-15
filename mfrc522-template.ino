#include <SPI.h>
#include <MFRC522.h>

String previous_card_number;
unsigned long readTimeout;

// A struct used for passing the UID of a PICC.
typedef struct {
  byte      size;      // Number of bytes in the UID. 4, 7 or 10.
  byte      uidByte[10];
  byte      sak;      // The SAK (Select acknowledge) byte returned from the PICC after successful selection.
} RFIDUid;

// A struct used for passing a MIFARE Crypto1 key
typedef struct {
  byte      keyByte[6];
} MIFARE_Key;

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
  rfidRead();
}
// Check for card and get uid
void rfidRead() {
  String card_number;
  MIFARE_Key key;
  MFRC522::Uid *uid = &(mfrc522.uid);

  // Look for card
  if ( ! mfrc522.PICC_IsNewCardPresent() ) return;

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial() ) return;

  // UID
  for ( byte i = 0; i < uid->size; i++ ) {
    String byteVal = String( uid->uidByte[i], HEX );
    if (byteVal.length() <= 1) {
      byteVal = "0" + byteVal;
    }
    card_number = card_number + byteVal;
  }

  // Only print card if not the same as last read
  if ( card_number != previous_card_number ) {
    Serial.println(card_number);
  }

  // Remember card for next read
  previous_card_number = card_number;
}
