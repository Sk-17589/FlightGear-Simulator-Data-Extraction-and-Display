# FlightGear Data Extractor MFC

## Overview
The FlightGear Data Extractor is an MFC (Microsoft Foundation Classes) application designed to extract real-time flight data from the FlightGear simulator. It retrieves various flight parameters including pitch, yaw, roll, speed, altitude, longitude, latitude, and airspeed via the simulator's HTTP server.

## Features
- Real-time data extraction from FlightGear.
- Continuous updating of flight parameters.
- User-friendly interface displaying data in separate edit boxes.

## Purpose
This application aims to check for the feasiibility of enhancing pilot training within military contexts, particularly at Hindustan Aeronautics Limited (HAL). By enabling the extraction and analysis of flight data from simulators, the tool supports the training of pilots transitioning between different aircraft. This approach allows pilots to practice and refine their skills in a simulated environment without the need for direct engagement with various aircraft, thereby increasing training efficiency and safety.

## Requirements
- Windows operating system.
- Visual Studio 2022 or later.
- FlightGear simulator installed.

## How to Use
1. Clone the repository to your local machine using:
git clone https://github.com/Sk-17589/FlightGearDataExtractorMFC.git
2. Open the project in Visual Studio.
3. Build the solution by selecting **Build > Build Solution** from the menu.
4. Run the application by selecting **Debug > Start Without Debugging**.
5. The application will connect to the FlightGear HTTP server and display real-time flight data.

## Running FlightGear Simulator on HTTP Port 5500
You can run the FlightGear Simulator and set it up to use the HTTP server on port 5500 through different methods:
1. **Command Prompt**: 
   - Open Command Prompt (CMD).
   - Execute the following command, replacing the paths as necessary for your installation:
     ```bash
     "C:\Program Files\FlightGear 2020.3\bin\fgfs.exe" --fg-aircraft="C:\Program Files\FlightGear 2020.3\data\Aircraft" --httpd=5500
     ```
2. **FlightGear Launcher**: 
   - Open the FlightGear Launcher.
   - Navigate to **Settings** > **Additional Settings**.
   - Find the option for **HTTP server** and set the port to **5500**.
   - Launch the simulator from the launcher.

## Acknowledgments
- The FlightGear team for providing the simulator and resources.
- Hindustan Aeronautics Limited (HAL) for their support and collaboration in pilot training initiatives.
- The open-source community for contributions and resources that aided in this project.

## Contact
For any inquiries or further information about this project, please contact:
- **Name**: Sashank Katepalli
- **Email**: katepalli.sashank@gmail.com
