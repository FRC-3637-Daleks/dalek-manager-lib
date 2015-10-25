# design

## Initialization Procedure
#### Registration Phase
 - Register hardware ports
   - Each subsystem adds the ports it needs to global map <string, default port>
   - Each subsystem adds the settings it needs to global map <string, default value>
   - Generate ports template json and save
   - Generate settings template json and save
 - Register Values
   - Each subsystem adds its values to global map <string, value>, possibly specifying their source
 - Register Commands
   - Each subsystem adds its commands to global map <string, command>

#### Bind Phase
 - Initialize hardware
   - Load ports json
   - Load settings json
   - Each subsystem initializes its hardware objects with the ports
   - Each subsystem configures itself with the settings
 - Load Triggers
   - Load controls json
   - Lookup each continuous-input control in json and specify the input/source function in the values map
   - Add groups to values map
   - Add command triggers to map with command path as key and trigger as value. The trigger should be watching the specified poll on the specified value to return true, and run that command

#### Activation Phase
 - Start threads
   - Start values checking thread
   - Start Log Thread
   - Start MQTT Thread
   - Start triggers/command scheduler thread
   - Start idling lua
   - Start subsystem threads if needed
 - Autonomous
   - Load script

Ready