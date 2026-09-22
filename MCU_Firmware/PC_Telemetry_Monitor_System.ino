#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  

// --- Buton ve Menü Değişkenleri ---
const int butonPin = 2;
int aktifSayfa = 1;
bool sonButonDurumu = HIGH; // INPUT_PULLUP kullandığımız için basılı değilken HIGH

// Ekranı anında güncellemek için son verileri hafızada tutuyoruz
uint8_t son_cpu = 0;
uint8_t son_ram = 0;
uint8_t son_ping = 0;

// --- Dairesel Tampon (Ring Buffer) ---
#define BUFFER_SIZE 64
uint8_t rx_buffer[BUFFER_SIZE];
uint16_t rx_head = 0, rx_tail = 0, rx_count = 0;

void push_buffer(uint8_t b) {
  if (rx_count < BUFFER_SIZE) {
    rx_buffer[rx_head] = b;
    rx_head = (rx_head + 1) % BUFFER_SIZE;
    rx_count++;
  }
}

bool pop_buffer(uint8_t *b) {
  if (rx_count == 0) return false;
  *b = rx_buffer[rx_tail];
  rx_tail = (rx_tail + 1) % BUFFER_SIZE;
  rx_count--;
  return true;
}

// --- Ayrıştırıcı (Parser) ---
enum ParserState { WAIT_START, WAIT_LENGTH, WAIT_DATA, WAIT_CHECKSUM, WAIT_END };
ParserState state = WAIT_START;
uint8_t data_len = 0, data_buffer[10], data_index = 0, calculated_checksum = 0, received_checksum = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Sistem Hazir");
  
  // Dahili direnci aktif et (Dışarıdan direnç bağlamaya gerek kalmaz)
  pinMode(butonPin, INPUT_PULLUP);
  
  Serial.begin(9600);
}

void loop() {
  // 1. BUTON KONTROLÜ
  bool anlikButonDurumu = digitalRead(butonPin);
  
  // Butona basıldığını (HIGH'dan LOW'a düştüğünü) yakala
  if (sonButonDurumu == HIGH && anlikButonDurumu == LOW) {
    aktifSayfa = (aktifSayfa == 1) ? 2 : 1; // Sayfayı değiştir
    updateLCD(); // Butona basılınca ekranı anında yenile
    delay(200);  // Buton sıçramasını (bounce) engellemek için kısa bekleme
  }
  sonButonDurumu = anlikButonDurumu;

  // 2. SERİ HABERLEŞME (Veri Toplama)
  while (Serial.available() > 0) {
    push_buffer(Serial.read());
  }
  
  // 3. VERİ AYRIŞTIRMA
  uint8_t current_byte;
  while (pop_buffer(&current_byte)) {
    switch (state) {
      case WAIT_START:
        if (current_byte == 0xAA) { state = WAIT_LENGTH; calculated_checksum = 0; }
        break;
      case WAIT_LENGTH:
        data_len = current_byte; calculated_checksum += current_byte; data_index = 0;
        state = (data_len > 0) ? WAIT_DATA : WAIT_CHECKSUM;
        break;
      case WAIT_DATA:
        data_buffer[data_index++] = current_byte; calculated_checksum += current_byte;
        if (data_index >= data_len) state = WAIT_CHECKSUM;
        break;
      case WAIT_CHECKSUM:
        received_checksum = current_byte; state = WAIT_END;
        break;
      case WAIT_END:
        if (current_byte == 0x55 && calculated_checksum == received_checksum && data_len >= 3) {
          // Yeni verileri hafızaya kaydet
          son_cpu = data_buffer[0];
          son_ram = data_buffer[1];
          son_ping = data_buffer[2];
          
          updateLCD(); // Yeni veri gelince ekranı yenile
        }
        state = WAIT_START;
        break;
    }
  }
}

// --- Ekran Güncelleme Fonksiyonu ---
void updateLCD() {
  char line1[17];
  char line2[17];
  char tempStr[20];
  
  if(aktifSayfa == 1) {
    sprintf(line1, "%-16s", "SISTEM DURUMU"); 
    sprintf(tempStr, "CPU:%%%d RAM:%%%d", son_cpu, son_ram);
    sprintf(line2, "%-16s", tempStr); 
  } 
  else {
    sprintf(line1, "%-16s", "AG / PING DURUMU"); 
    if(son_ping > 254) {
      sprintf(line2, "%-16s", "Baglanti Yok!");
    } else {
      sprintf(tempStr, "Gecikme: %d ms", son_ping);
      sprintf(line2, "%-16s", tempStr);
    }
  }
  
  lcd.setCursor(0, 0); lcd.print(line1);
  lcd.setCursor(0, 1); lcd.print(line2);
}