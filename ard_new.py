import serial
import datetime
import pandas as pd
import os
import time
count = 0

def main_func():
    data = []  # Store the data temporarily

    # Start serial connection
    arduino = serial.Serial('/dev/ttyTHS1', 250000, timeout=1)
    time.sleep(2)
    print('Establishing serial connection with Arduino...')

    # Collect data for exactly 50 samples (50 milliseconds)
    sample_count = 0

    while sample_count < 100:  # Collect 50 samples (50 milliseconds of data)
        try:
            arduino_data = arduino.readline().decode("utf-8").strip()
        except UnicodeDecodeError as e:
            print("UnicodeDecodeError:", e)
            arduino_data = ""

        # Proceed only if data is not empty
        if arduino_data:
            # Read data
            list_values = arduino_data.split("x")
            print(list_values)
            
            if len(list_values) >= 6:  # Ensure the data format is correct
                try:
                    # Convert values to float
                    bmag = float(list_values[0])
                    brms = float(list_values[1])
                    sat  = float(list_values[2])
                    didt = float(list_values[3])
                    pow1 = float(list_values[4])
                    temp = float(list_values[5])
                   

                    # Get the timestamp
                    time_stamp = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")  # Include microseconds

                    # Prepare data for saving
                    data.append([time_stamp, bmag, brms, sat, didt, pow1, temp])

                    # Increment the sample counter
                    sample_count += 1
                except ValueError:
                    print("Error: Could not convert one of the readings to float.")
            else:
                print("Error: Incomplete data received from Arduino.")

    # Save collected data to CSV after 50 samples
    file_name = 'data_full.csv'
    
    # Write header if the file does not exist; otherwise, append without header
    write_mode = 'w' if not os.path.isfile(file_name) else 'a'
    header = write_mode == 'w'
    
    node = pd.DataFrame(data, columns=['Time stamp', 'TMR', 'RMS', 'sat', 'didt', 'Power', 'Temp'])
    node.to_csv(file_name, index=False, mode=write_mode, header=header)  # Write header if needed
    
    print(f"Data saved in {file_name}")
    
    arduino.close()  # Close the serial connection
    print('Connection closed')
    print('<--------------------------------------------------------------------------->')

# Main lo

while True:
    main_func()  # Collect data for the first 50 samples
    count = count +1;
    time.sleep(10)  # Wait before starting the next collection cycle (optional)
    if count > 30:
        arduino.close()

