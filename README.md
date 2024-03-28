# Arduino Valve Controller with CAN Support

This project utilizes an Arduino to control a valve based on data received over a CAN bus and input from a physical button. It's designed to operate in automatic or manual mode, responding to RPM data for cold start detection and user inputs for direct valve control.

## Features

- **CAN Bus Integration**: Listens for RPM data on the CAN bus to make decisions.
- **Cold Start Detection**: Automatically detects engine cold starts and manages the valve accordingly.
- **Manual Control**: Allows manual valve control through a button, with support for both short and long presses.
- **Automatic and Manual Modes**: Switch between automatic operation based on CAN data and manual control as desired.

## Hardware Requirements

- Arduino-compatible board
- MCP2515 CAN Bus Module
- Valve controller hardware
- Button for manual input
- LED for status indication

## Wiring

- **MCP2515 CAN Module**: Connect to Arduino SPI pins and set CS to pin 5, INT to pin 21.
- **Valve Controller**: Connect according to your valve controller's specifications (example uses pins 25 and 33).
- **Button**: Connect to pin 32.
- **LED**: Connect to pin 22.


# Arduino Valve Controller with CAN Support

This project utilizes an Arduino to control a valve based on data received over a CAN bus and input from a physical button. It's designed to operate in automatic or manual mode, responding to RPM data for cold start detection and user inputs for direct valve control.

## Features

- **CAN Bus Integration**: Listens for RPM data on the CAN bus to make decisions.
- **Cold Start Detection**: Automatically detects engine cold starts and manages the valve accordingly.
- **Manual Control**: Allows manual valve control through a button, with support for both short and long presses.
- **Automatic and Manual Modes**: Switch between automatic operation based on CAN data and manual control as desired.

## Hardware Requirements

- Arduino-compatible board
- MCP2515 CAN Bus Module
- Valve controller hardware
- Button for manual input
- LED for status indication

## Wiring

- **MCP2515 CAN Module**: Connect to Arduino SPI pins and set CS to pin 5, INT to pin 21.
- **Valve Controller**: Connect according to your valve controller's specifications (example uses pins 25 and 33).
- **Button**: Connect to pin 32.
- **LED**: Connect to pin 22.
                              ![image](https://github.com/kamiladas/ValveController/assets/58427794/b1436c65-0449-4947-88e1-bd8835ea6c9d)

## Software Setup

1. **Arduino IDE**: Ensure you have the Arduino IDE installed.
2. **Libraries**: Install the `mcp_can` library through the Arduino Library Manager.
3. **Upload Code**: Connect your Arduino board to your computer, select the correct board and port in the Arduino IDE, and upload the code.

## Usage

- The device starts in automatic mode, responding to RPM data from the CAN bus.
- Press the button shortly to toggle the valve manually. This functionality is available in manual mode.
- Press and hold the button to switch between automatic and manual modes. The LED will blink 3 times to indicate the switch to automatic mode.

## Customization

Feel free to adapt the code to suit your specific needs, such as adjusting RPM thresholds, debounce times, or adding additional functionality.

## Contributing

Contributions to this project are welcome. Please fork the repository and submit a pull request with your improvements.

## License

This project is open-source and available under the [MIT License](LICENSE).

## Acknowledgments

- Thanks to the Arduino and the open-source community for the support and libraries that made this project possible.

## Software Setup

1. **Arduino IDE**: Ensure you have the Arduino IDE installed.
2. **Libraries**: Install the `mcp_can` library through the Arduino Library Manager.
3. **Upload Code**: Connect your Arduino board to your computer, select the correct board and port in the Arduino IDE, and upload the code.

## Usage

- The device starts in automatic mode, responding to RPM data from the CAN bus.
- Press the button shortly to toggle the valve manually. This functionality is available in manual mode.
- Press and hold the button to switch between automatic and manual modes. The LED will blink 3 times to indicate the switch to automatic mode.

## Customization

Feel free to adapt the code to suit your specific needs, such as adjusting RPM thresholds, debounce times, or adding additional functionality.

## Contributing

Contributions to this project are welcome. Please fork the repository and submit a pull request with your improvements.

## License

This project is open-source and available under the [MIT License](LICENSE).

## Acknowledgments

- Thanks to the Arduino and the open-source community for the support and libraries that made this project possible.
