"""
Data collection control for teensy for a121 XM125 breakout. 
"""
import serial.tools.list_ports
import serial
import time
import csv
import sys

args = sys.argv
FILE = args[1]
NUM_TRIALS_PER = int(args[2])
NUM_TRIALS_TOT = int(args[3])
MULTIPLE = args[4]
TRIAL = int(args[5])


def main():

    print(f'***Starting Rangefinding Trial. Writing to {FILE}.***')
    print_ports()

    # Create/overwrite the file and write the header
    if MULTIPLE == "true":
        print("Starting Multiple Trials")
        with open(FILE, "w", newline='') as f:
            writer = csv.writer(f, delimiter=",")
            writer.writerow(["trialNum", "resgister", "reading (mm)", "strength", "expected distance (mm)"])

        # Re-open the file in append mode for data collection
        with open(FILE, "a", newline='') as f:
            # intialize serial communication
            serialCom = open_serial()

            # take data, give user a couple seconds to move the apparatus
            k = 0
            while k < NUM_TRIALS_TOT:
                get_reading(f, NUM_TRIALS_PER, serialCom, k)
                # delay for sensor adjustment, 7 seconds right now
                for n in range(10, 0, -1):
                    print(f'Measure in {n}')
                    time.sleep(1)
                k += 1
    else:
        with open(FILE, "a", newline="") as f:
            serialCom = open_serial()
            get_reading(f, NUM_TRIALS_PER, serialCom, TRIAL)

# list which port is being used
def print_ports():
    ports = serial.tools.list_ports.comports()
    if not ports:
        print("No ports detected")
        return
    for port in ports:
        print(port)

def open_serial():
    ports = serial.tools.list_ports.comports()
    if not ports:
        raise IOError("No serial ports found. Make sure your device is connected.")

    # Use the first available port.
    port_name = ports[0].device
    print(f"Connecting to port: {port_name}")

    # communicate with serial port connected to teensy
    serialCom = serial.Serial(port_name, 115200)
    serialCom.setDTR(False)
    time.sleep(1)
    serialCom.flushInput()
    serialCom.setDTR(True)
    return serialCom

def get_raw_message(file, num_trials, serialCom, trial):
    serialCom.write(b'START\n')
    for k in range(num_trials):
        try:
            s_bytes = serialCom.readline()
            decoded_bytes = s_bytes.decode("utf-8").strip('\r\n')
            print(decoded_bytes)
        except:
            print("Line not recorded")
    serialCom.write(b'STOP\n')

def get_reading(file, num_trials, serialCom, trial):
    serialCom.write(b'START\n')
    writer = csv.writer(file, delimiter=",")
    for k in range(num_trials):
        try:
            s_bytes = serialCom.readline()
            decoded_bytes = s_bytes.decode("utf-8").strip('\r\n')
            values = [float(x) for x in decoded_bytes.split()]
            row = [trial, values[0], values[1], values[2], 300 - 10 * trial]
            writer.writerow(row)
            print(values)
        except:
            print("Line not recorded")
    serialCom.write(b'STOP\n')

if __name__ == "__main__":
    main()
