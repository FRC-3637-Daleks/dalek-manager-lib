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

#### `Container_base<Meta_t, T>`
 - `node - ConfigStore<Meta_t>::Node *`
 - `Get() - T, virtual` 
 - `Set(T), virtual`

### Ports

#### `PortSpace`
 - `min - byte`
 - `max - byte`
 - `used_mask - long long` Each bit represents whether a port is in use
 - `isClosed(int) - bool` Returns true if the argument is already a port in use
 - `close(int)`
 - `open(int)`

#### `PortMeta`
 - `obj - shared_ptr<PortObject>`

#### `Port`
 - extends `Container_base<PortMeta, int>`
 - `Set(int), override` Verifies that arg is in valid range and sets port, closing it in the object

### Settings

#### `SettingsMeta`
 - `SetJson<T>(JSON_Object&, T)`

#### `Setting<T>`
 - extends `Container_base<SettingsMeta, T>`

### Controls

#### `ContinuousControlMeta`
 - `argument_type - {double, bool, int, etc...}`

#### `CommandMeta`
 - `poll_func - functor`

#### `ContinuousControl<T>`
 - extends `Container_base<ContinuousControlMeta, ValueStore<T>::Value>`
 - `Get() - ValueStore<T>::Value`
 - `Set(ValueStore<T>::Value)`

#### `CommandControl`
 - extends `Container_base<CommandMeta, ValueStore<bool>::Value>`
 - `Get() - ValueStore<bool>::Value`
 - `Set(ValueStore<bool>::Value)`

### Values
inspired by DiagRoboRIO, these classes consist of a framework for building a real time updated store of values which can be accessed by name at any time, including initialization.

#### `Gettable<T>, abstract`
 - `Get() - T, pure virtual`

#### `Settable<T>, abstract`
 - `Set(T), pure virtual`

#### `Updateable`
 - `Update(), pure virtual`

#### `Valuable<T>`
 - extends `Gettable<T>`
 - `value - T`
 - `Get() - T, final` - returns value
 - `set(T), protected`
 - `GetAddr() - const T *` - returns address of value

#### `Reference<T>`
 - extends `Gettable<T>`
 - Constructors - `(const T *)`, `(const Valuable<T>&)`
 - `ref - const T *`
 - `Get() - T, final` - returns the value at ref

#### `Pollable<T>, abstract`
 - extends `Valuable<T>, Updateable`
 - `poll() - T, protected, pure virtual`
 - `Update(), final {set(poll());}`

#### `Settable<T>`
 - extends `Valuable<T>`
 - `Set(T)`

#### `FunkyPoll<T>`
 - extends `Pollable<T>`
 - `funk - {function() - T}`
 - `poll() - T {return funk();}`

#### `UpdateStore`
 - extends `Updateable`
 - `objs - map<string, Updateable *>`
 - `Update()` - Updates all update objects
 - `default_store - UpdateStore *, static`

#### `UpdateThread`
 - extends `UpdateStore`
 - `Update()` - Updates all update objects and checks the time they take, possibly logging ones which take too long
 - `period - microseconds` 
 - `thread` - Calls `Update` every period
 - `default_thread - UpdateThread *, static`

#### `ValueStore<T>`
 - `values - map<string, Gettable<T> *>`
 - `Value`
   - `obj - pair<string, Gettable<T> *> *` - Points to the string, value pair in the parent ValueStore
   - `GetKey() - string`
   - `GetValue() - T`
   - `Initialize(obj - Gettable<T> *)` - Sets the mapped value to obj. Throws exception if current value isn't null
   - `Initialize(obj - Pollable<T> *, updateStore = UpdateStore::default_store - UpdateStore*)` - Calls above function and adds the Updateable part of the object to the specified UpdateThread at the same key.
   - `Release() - Gettable<T> *` - This will set the mapped value to null and return what was in there.
 - `Get(key - string) - Value` - Returns and, if needed, creates a mapped object at `key`
 - `Initialize(key - string, obj - Gettable<T> *)`
 - `Initialize(key - string, obj - Pollable<T> *, updateStore = UpdateStore::default_store - UpdateStore*)`
