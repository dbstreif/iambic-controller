import serial, numpy as np, sounddevice as sd, queue
import time
import threading

SERIAL_PORT = '/dev/ttyACM0'  # change if different
BAUD_RATE = 9600

FREQ = 600               # tone frequency (Hz)
SAMPLE_RATE = 44100

WPM = 20
DOT_MS = 1200 / WPM
DASH_MS = 3 * DOT_MS
FADE_MS = 5

# open the serial connection
ser = None

start_time = time.time()
end_time = 0

while ser is None and end_time - start_time < 20:
    end_time = time.time()
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.1)
    except Exception as e:
        ser = None
        continue

if ser is None:
    print("Could not connect to device.")
    exit(1)

def make_tone(duration_ms):
    duration_s = duration_ms / 1000.0
    t = np.linspace(0, duration_s, int(SAMPLE_RATE * duration_s), endpoint=False)
    tone = np.sin(2 * np.pi * FREQ * t)

    # apply fade in/out
    fade_len = int(SAMPLE_RATE * FADE_MS / 1000)
    envelope = np.ones_like(tone)
    envelope[:fade_len] = np.linspace(0, 1, fade_len)
    envelope[-fade_len:] = np.linspace(1, 0, fade_len)
    return (tone * envelope).astype(np.float32)

# Pre-generate buffers
DIT = make_tone(DOT_MS)
DAH = make_tone(DASH_MS)
GAP = np.zeros(int(SAMPLE_RATE * (DOT_MS / 1000))) # inter-tone time

key_queue = queue.Queue()
stop_flag = threading.Event()

def oscillate():
    while not stop_flag.is_set():
        nplist = []
        while not key_queue.empty():
            nplist.append(key_queue.get())
        buf = np.concatenate(nplist) if nplist else GAP
        sd.play(buf, SAMPLE_RATE, blocking=True)

print("Listening for paddle input...")

lbits = 0b00
last = 0

try:
    oscillator = threading.Thread(target=oscillate)
    oscillator.start()

    while True:
        line = ser.readline().decode(errors='ignore').strip()
        if "Firmware" in line:
            print(line)
            continue

        if line:
            lbits = int(line, 2)
            print(line)

        if lbits == 0b10:
            last = 0
            key_queue.put(DIT)

        elif lbits == 0b01:
            last = 1
            key_queue.put(DAH)

        elif lbits == 0b11:
            next = DAH if last == 0 else DIT
            key_queue.put(next)
            last = not last

        time.sleep(DOT_MS / 1000)


except KeyboardInterrupt:
    print("\nExiting...")
finally:
    stop_flag.set()
    ser.close()
