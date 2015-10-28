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
   - Generate controls requirements json and save

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

## Class Layout

#### `Subsystem`
 - `RegisterHardware(PortStore&, SettingsStore&)`
 - `RegisterValues(ValueStore&)`
 - `RegisterCommands(CommandStore&)`
 - `InitializeHardware(const PortStore&, const SettingsStore&)`
 - `Start()`

#### `ConfigStore<Meta_t>`
 - `Node`
   - `map<string, Node> *`
   - `Meta_t value`
   - `JSON_Object json_node`
   - `ConfigStore<Meta_t> *parent`
   - `GetValue() - Meta_t&`
   - `GetNode(key) - Node`
   - `Get<Container_t>() - Container_t {return Container_t(this);}`
   - `isLeaf() - bool`
 - `Node root`
 - `json_file output`

### Ports

#### `PortSpace`
 - `min - int`
 - `max - int`
 - `used_mask - short` Each bit represents whether a port is in use
 - `isClosed(int) - bool` Returns true if the argument is already a port in use
 - `close(int)`
 - `open(int)`

#### `PortMeta`
 - `shared_ptr<PortObject> obj`

#### `Port`
A type of Container_t
 - `ConfigStore<PortMeta>::Node *node`
 - `Get() - int`
 - `Set(int)` Verifies that arg is in valid range and sets port, closing it in the object


