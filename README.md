# PC_Telemetry_Monitor_System
A hardware-software integration project that streams real-time PC performance metrics (CPU, RAM) and ICMP network latency (Ping) to a physical Arduino-driven 16x2 I2C LCD monitor via serial communication.

This project demonstrates a closed-loop engineering approach, featuring a custom byte-packet communication protocol, a non-blocking embedded state machine, and a complete Proteus ISIS hardware simulation.

🚀 Features

Real-Time Telemetry: Monitors CPU usage, RAM utilization, and network ping (to 8.8.8.8) using Python.

Custom UART Protocol: Reliable data transmission using a structured packet with start/end bytes and checksum validation.

Non-Blocking Architecture: Arduino firmware utilizes a Ring Buffer and a State Machine parser to prevent loop blocking and data loss.

Interactive UI: Hardware push-button with internal pull-up resistor allows toggling between "System Stats" and "Network Status" pages on the LCD.

Full Simulation: Includes a Proteus (.pdsprj) simulation file with virtual COM port (COMPIM) integration for testing without physical hardware.

📁 Repository Structure

/PC_Software: Contains the Python backend script (telemetry.py) and requirements.txt.

/MCU_Firmware: Contains the Arduino C++ firmware utilizing state-machine logic.

/Hardware_Simulation: Contains the Proteus ISIS simulation file and schematic.

⚙️ Communication Protocol

To ensure data integrity over the serial port, a custom 7-byte packet structure is used:
[0xAA] [Length: 0x03] [CPU%] [RAM%] [Ping] [Checksum] [0x55]

0xAA & 0x55: Start and End bytes.

Checksum: Modulo 256 sum of Length and Data payload.

🛠️ Hardware Requirements

Arduino Uno R3

16x2 Alphanumeric LCD with I2C module (PCF8574)

Push Button (Connected to Pin 2 and GND)

Jumper wires & Breadboard

💻 Setup & Installation

1. Python Backend

Navigate to the PC_Software folder and install the required dependencies:

pip install -r requirements.txt


Update the ARDUINO_PORT variable in telemetry.py to match your physical Arduino COM port (or virtual COM port for simulation).

2. Arduino Firmware

Open the .ino file in the MCU_Firmware folder using the Arduino IDE. Select your COM port and upload the code to your Arduino Uno.

3. Proteus Simulation (Optional)

To run the simulation:

Create a virtual COM port pair (e.g., COM1 and COM2) using VSPE or com0com.

Open the .pdsprj file in Proteus.

Configure the COMPIM module to use one end of your virtual pair (e.g., COM1).

Export the compiled .hex file from Arduino IDE and load it into the virtual Arduino.

Point the Python script to the other end of the virtual pair (e.g., COM2) and run it.

🇹🇷 Türkçe (Turkish)

PC Telemetri ve Ağ Gecikme Monitörü

Bu proje, bilgisayarın anlık sistem metriklerini (CPU, RAM) ve ICMP ağ gecikmesini (Ping) seri haberleşme üzerinden fiziksel bir Arduino tabanlı 16x2 I2C LCD ekrana aktaran bir donanım-yazılım entegrasyonudur.

Özel bir bayt-paketi haberleşme protokolü, kilitlenmeyen (non-blocking) gömülü durum makinesi ve eksiksiz bir Proteus ISIS donanım simülasyonu barındıran bu proje, kapalı çevrim bir mühendislik yaklaşımını sergilemektedir.

🚀 Özellikler

Gerçek Zamanlı Telemetri: Python kullanarak CPU, RAM ve ağ gecikmesini (8.8.8.8) izler.

Özel UART Protokolü: Başlangıç/bitiş baytları ve checksum doğrulaması içeren yapılandırılmış bir paket ile güvenilir veri aktarımı.

Bloklamayan Mimari (Non-Blocking): Arduino yazılımı, döngü kilitlenmelerini ve veri kaybını önlemek için Ring Buffer (Dairesel Tampon) ve Durum Makinesi (State Machine) kullanır.

İnteraktif Arayüz: Dahili pull-up direncine sahip donanımsal buton, LCD üzerindeki "Sistem Durumu" ve "Ağ Durumu" sayfaları arasında geçiş sağlar.

Tam Simülasyon: Fiziksel donanım olmadan test yapabilmek için sanal COM port (COMPIM) entegrasyonlu bir Proteus (.pdsprj) simülasyon dosyası içerir.

📁 Klasör Yapısı

/PC_Software: Python arka plan betiğini (telemetry.py) ve requirements.txt dosyasını içerir.

/MCU_Firmware: State-machine mantığını kullanan Arduino C++ yazılımını içerir.

/Hardware_Simulation: Proteus ISIS simülasyon dosyasını ve şemasını içerir.

⚙️ Haberleşme Protokolü

Seri port üzerinden veri bütünlüğünü sağlamak için 7 baytlık özel bir paket yapısı kullanılır:
[0xAA] [Uzunluk: 0x03] [CPU%] [RAM%] [Ping] [Checksum] [0x55]

0xAA & 0x55: Başlangıç ve Bitiş baytları.

Checksum: Uzunluk ve veri yükünün toplamının 256'ya bölümünden kalan (Modulo 256).

🛠️ Donanım Gereksinimleri

Arduino Uno R3

16x2 Alfanumerik LCD ve I2C modülü (PCF8574)

Push Buton (Pin 2 ve GND arasına bağlı)

Jumper kablolar & Breadboard

💻 Kurulum ve Kullanım

1. Python Arka Planı

PC_Software klasörüne gidin ve gerekli kütüphaneleri kurun:

pip install -r requirements.txt


telemetry.py içindeki ARDUINO_PORT değişkenini fiziksel Arduino'nun (veya simülasyon için sanal portun) COM adresiyle değiştirin.

2. Arduino Yazılımı

MCU_Firmware klasöründeki .ino dosyasını Arduino IDE ile açın. Portunuzu seçin ve kodu Arduino Uno'ya yükleyin.

3. Proteus Simülasyonu (Opsiyonel)

Simülasyonu çalıştırmak için:

VSPE veya com0com kullanarak sanal bir COM port çifti oluşturun (örn. COM1 ve COM2).

Proteus'ta .pdsprj dosyasını açın.

COMPIM modülünü sanal çiftin bir ucunu (örn. COM1) kullanacak şekilde ayarlayın.

Arduino IDE'den derlenmiş .hex dosyasını dışa aktarıp sanal Arduino'ya yükleyin.

Python betiğini sanal çiftin diğer ucuna (örn. COM2) yönlendirip çalıştırın.
