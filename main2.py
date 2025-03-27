import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque

# === CONFIGURABLE PREFIXES ===
# Add or modify prefixes here. 'ylim' is used to set fixed bounds.
PREFIXES = {
    'BP': {'label': 'Blood Pressure (mmHg)', 'color': 'blue', 'ylim': (-250, 250)},
    'PSENS': {'label': 'Pressure Sensor (mmHg)', 'color': 'red', 'ylim': (-250, 250)},
    'TEMP': {'label': 'Temperature (°C)', 'color': 'green', 'ylim': (-250, 250)},
    'HR': {'label': 'Heart Rate (BPM)', 'color': 'orange', 'ylim': (40, 200)}
}

# === SERIAL CONFIGURATION ===
SERIAL_PORT = 'COM8'  # Change to your serial port, e.g., 'COM3' on Windows
BAUD_RATE = 115200

# Open the serial port with a short timeout to allow non-blocking reads.
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.1)

# === PARAMETERS FOR REAL-TIME PLOTTING ===
max_points = 5000  # Number of points to display on the graph
data_buffers = {prefix: {'x': deque(maxlen=max_points), 'y': deque(maxlen=max_points)} for prefix in PREFIXES}
active_prefixes = set()

# Set up the matplotlib figure and axis
fig, ax = plt.subplots()
lines = {}

# Create a line for each prefix, but don't plot unless data arrives
for prefix, config in PREFIXES.items():
    line, = ax.plot([], [], lw=2, label=config['label'], color=config['color'])
    lines[prefix] = line

ax.set_title('Real-time Data Plot')
ax.set_xlabel('Time (s)')
ax.set_xlim(0, max_points / 1000.0)  # Start with a 0 to 5-second window (assuming 1000 samples/sec)
ax.legend()

# === HARDCODE Y-AXIS LIMITS ===
# Set fixed Y-axis limits to the first active prefix's configured limits
y_min, y_max = None, None


def init():
    """Initialize the lines for the animation."""
    for line in lines.values():
        line.set_data([], [])
    return lines.values()


# A global time counter (in seconds), assuming each sample is 0.001 s apart.
current_time = 0.0


def update(frame):
    global current_time, y_min, y_max
    # Read all available lines from the serial port
    while ser.in_waiting:
        try:
            # Read a line from the serial port and decode it.
            line_bytes = ser.readline()
            line_str = line_bytes.decode('utf-8').strip()

            # Check if the line starts with any known prefix
            for prefix in PREFIXES:
                if line_str.startswith(prefix):
                    try:
                        # Extract and parse the value after the prefix
                        value_str = line_str[len(prefix):].strip()
                        value = float(value_str)

                        # Add data to buffer and mark as active
                        data_buffers[prefix]['x'].append(current_time)
                        data_buffers[prefix]['y'].append(value)
                        active_prefixes.add(prefix)

                        # Increment time based on the assumed sample rate (1 ms per sample)
                        current_time += 0.001
                    except ValueError:
                        print(f"Invalid value for {prefix}: {value_str}")
                    break
        except Exception as e:
            print(f"Error reading line: {e}")

    # Set Y-axis limits once based on the first active prefix
    if y_min is None and active_prefixes:
        first_prefix = list(active_prefixes)[0]
        y_min, y_max = PREFIXES[first_prefix]['ylim']
        ax.set_ylim(y_min, y_max)

    # Update only active data
    for prefix in active_prefixes:
        buffer = data_buffers[prefix]
        if buffer['x'] and buffer['y']:
            lines[prefix].set_data(list(buffer['x']), list(buffer['y']))

    # Adjust x-axis limits dynamically for scrolling effect
    if active_prefixes:
        first_prefix = list(active_prefixes)[0]
        ax.set_xlim(data_buffers[first_prefix]['x'][0], data_buffers[first_prefix]['x'][-1] + 0.001)

    return lines.values()


# Set up the animation; the update function is called every 1 ms.
ani = animation.FuncAnimation(fig, update, init_func=init, blit=True, interval=1)

try:
    plt.show()
except KeyboardInterrupt:
    print("Plot closed.")

# Close the serial port when finished
ser.close()
