import os
import time

# Function to write to files
def write_to_file(filename, data):
    with open(filename, 'w') as f:
        f.write(data)

# Function to read from files
def read_from_file(filename):
    with open(filename, 'r') as f:
        return f.read().strip()

# Initialize files
write_to_file('Control_Panel.ssv', '')
write_to_file('Control_Panel_Flag.ssv', '')
write_to_file('Control_Panel_Finished.ssv', '0')

# Clear screen
os.system('cls')

print('\n')
print('\n')
print('|X|\\')
print('|X|____________NT4 Symbolic Neuro Network Kernel')
print('|X|/')
print('\\X/')

while True:
    # Initialize files
    write_to_file('Control_Panel.ssv', '')
    write_to_file('Control_Panel_Flag.ssv', '')
    write_to_file('Control_Panel_Finished.ssv', '0')

    # Change color
    os.system('color 0a')

    print(' ^ |')

    # Take user input
    command = input('<+>---')

    # Write command to Control_Panel.ssv
    write_to_file('Control_Panel.ssv', command)

    # Set flag to 1
    write_to_file('Control_Panel_Flag.ssv', '1')

    # Reset Control_Panel_Finished.ssv
    write_to_file('Control_Panel_Finished.ssv', '0')

    cou = 0

    while True:
        # Change color
        os.system('color 04')

        # Wait for a while (optional)
        time.sleep(0.1)

        # Read from Control_Panel_Finished.ssv
        cou = int(read_from_file('Control_Panel_Finished.ssv'))

        if cou != 1:
            continue
        else:
            break
