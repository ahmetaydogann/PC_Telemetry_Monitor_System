import psutil
import serial
import time
import subprocess
import re

# Proteus ile haberleşecek VSPE sanal portumuz
ARDUINO_PORT = "COM2"
BAUD_RATE = 9600

# Seri portu başlat
try:
    ser = serial.Serial(ARDUINO_PORT, BAUD_RATE, timeout=1)
    print(f"[*] Bağlantı başarılı: {ARDUINO_PORT}")
except Exception as e:
    print(f"[!] Port açılamadı ({ARDUINO_PORT}). VSPE'yi kontrol et.\nHata: {e}")
    exit()

def get_ping(host="8.8.8.8"):
    try:
        # Windows ping komutu
        output = subprocess.check_output(f"ping -n 1 {host}", shell=True, universal_newlines=True)
        # Türkçe "süre=" dahil tüm dil formatlarını yakalayan gelişmiş regex
        match = re.search(r'(zaman|time|süre|sure)[=<]\s*(\d+)', output, re.IGNORECASE)
        if match:
            return int(match.group(2))
        else:
            return 255 # Okunamazsa hata kodu (255) döndür
    except Exception:
        return 255

print("[*] Sistem dinleniyor... Veri aktarımı başlatıldı. (Durdurmak için Ctrl+C)")

while True:
    try:
        # 1. Verileri Topla
        cpu_usage = int(psutil.cpu_percent(interval=None))
        ram_usage = int(psutil.virtual_memory().percent)
        ping_latency = get_ping()

        # 2. Sınırlandırma (Değerlerin 1 Bayt yani 0-255 aralığını aşmamasını garantiye alıyoruz)
        cpu = min(254, max(0, cpu_usage))
        ram = min(254, max(0, ram_usage))
        ping_val = min(255, max(0, ping_latency))

        # 3. Paket Oluşturma (Protokol)
        length = 3
        # Checksum (Hata doğrulama): Uzunluk ve verilerin toplamının 256'ya bölümünden kalan
        checksum = (length + cpu + ram + ping_val) % 256
        
        # [BAŞLANGIÇ] [UZUNLUK] [VERİ 1] [VERİ 2] [VERİ 3] [CHECKSUM] [BİTİŞ]
        packet = bytearray([0xAA, length, cpu, ram, ping_val, checksum, 0x55])
        
        # 4. Veriyi Gönder
        ser.write(packet)
        print(f"Gönderilen -> CPU: %{cpu:<3} | RAM: %{ram:<3} | Ping: {ping_val} ms")
        
        time.sleep(1) # Saniyede 1 kez güncelle

    except KeyboardInterrupt:
        print("\n[*] Çıkış yapılıyor...")
        ser.close()
        break
    except Exception as e:
        print(f"\n[!] Döngü Hatası: {e}")
        time.sleep(1)