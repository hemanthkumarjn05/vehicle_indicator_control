# Vehicle Indicator Control

This project is an Arduino-based controller for vehicle indicators and hazard lights. It allows you to control left, right, and hazard indicators using physical buttons, with logic for button combinations and debouncing. The system is suitable for automotive or robotics applications where reliable signaling is required.

## Features

- Toggle left and right indicators with dedicated buttons.
- Activate hazard lights using a dedicated button or by pressing both indicator buttons together.
- Debouncing logic to prevent accidental multiple toggles.
- LEDs blink at a fixed interval when active.
- Serial output for status messages and debugging.

## Hardware Requirements

- Arduino board (Uno, Mega, Nano, etc.)
- 3 push buttons (Left, Right, Hazard)
- 2 LEDs (Left, Right indicators)
- Resistors for LEDs and buttons (as needed)
- Breadboard and jumper wires

## Pin Configuration

| Function        | Pin Number |
|-----------------|------------|
| Left Button     | 18         |
| Right Button    | 19         |
| Hazard Button   | 23         |
| Left LED        | 21         |
| Right LED       | 22         |

Modify the pin numbers in `indicator.ino` if your hardware setup differs.

## How It Works

- On startup, all indicators are off and pins are initialized.
- Button states are checked every 100ms, with debounce logic.
- Hazard lights can be activated by the hazard button or by holding both indicator buttons for more than 1 second.
- LEDs blink every 300ms when their respective indicator is active.
- Status changes are printed to the serial monitor for debugging.

## Usage

1. Connect the hardware as per the pin configuration.
2. Upload `indicator.ino` to your Arduino board.
3. Open the Serial Monitor at 115200 baud to view status messages.
4. Press the left, right, or hazard buttons to control the indicators.

## Customization

- Change blink intervals by modifying the timing logic in `indicator.ino`.
- Adjust debounce delay via `TOGGLE_DEBOUNCE_DELAY`.
- Add more features such as auto-off timers or integration with other vehicle systems.

## File Structure

- `indicator.ino`: Main Arduino sketch implementing the logic.

## Development

- Written in Arduino C++.
- Compatible with PlatformIO and Arduino IDE.
- VS Code recommended for easier development and debugging.

## Troubleshooting

- If LEDs do not blink, check wiring and pin assignments.
- Ensure buttons are connected with pull-up resistors or use `INPUT_PULLUP` as in the code.
- Use the Serial Monitor for real-time feedback.

## License

This project is open-source and free to use for educational and personal projects.

## Author

Developed by Hemanth Kumar. Contributions welcome!

