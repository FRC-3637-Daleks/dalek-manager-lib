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
   - `GetMetaData() - Meta_t&`
   - `GetNode(key) - Node`
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
 - `obj - shared_ptr<PortSpace>`

#### `Port`
 - extends `Container_base<PortMeta, int>`
 - `Set(int), override` Verifies that arg is in valid range and sets port, closing it in the object

#### `PortStore`
 - extends `ConfigStore<PortMeta>`
 - `GetPort(path - string) - Port`
 - `GetPort(leaf - node) - Port`

### Settings

#### `SettingsMeta`
 - `SetJson<T>(JSON_Object&, T)`

#### `Setting<T>`
 - extends `Container_base<SettingsMeta, T>`

#### SettingStore
 - extends `ConfigStore<SettingsMeta>`
 - `GetSetting<T>(path - string) - Setting<T>`
 - `GetSetting<T>(leaf - Node) - Setting<T>`

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

#### `ContinuousControlStore`
 - extends `ConfigStore<ContinuousControlMeta>`
 - `GetControl<T>(path - string) - ContinuousControl<T>`
 - `GetControl<T>(leaf - Node) - ContinuousControl<T>`

#### `CommandControlStore`
 - extends `ConfigStore<CommandControlMeta>`
 - `GetControl(path - string) - CommandControl`
 - `GetControl(leaf - Node) - CommandControl`

### Values
inspired by DiagRoboRIO, these classes consist of a framework for building a real time updated store of values which can be accessed by name at any time, including initialization.

#### `Gettable<T>, abstract`
 - `Get() - T, pure virtual`

#### `Settable<T>`
 - `Set(T), pure virtual`

#### `Updateable`
 - `Update(), pure virtual`

#### `Valuable<T>`
 - extends `Gettable<T>`
 - `value - std::atomic<T>` - must ensure thread safety
 - `Get() - T, final` - returns value
 - `set(T), protected`
 - `GetAddr() - const T *` - returns address of value

#### `Valuable<string>`
 - extends `Gettable<string>`
 - `value - string`
 - `r, w - mutex`
 - `Get() - T, final` - waits for w. locks r.
 - `set(T), protected` - waits for w and r. locks w.

#### `Reference<T>`
 - extends `Gettable<T>`
 - Constructors - `(const Valuable<T>&)`
 - `ref - const Valuable<T> *`
 - `Get() - T, final` - returns the value at ref

#### `Pollable<T>, abstract`
 - extends `Valuable<T>, Updateable`
 - `poll() - T, protected, pure virtual`
 - `Update(), final {set(poll());}`

#### `SetValue<T>`
 - extends `Valuable<T>, Settable<T>`
 - `Set(T)` - calls Valuable's Set

#### `FunkyPoll<T>`
 - extends `Pollable<T>`
 - `funk - {function() - T}` - upon construction if this were a member function type, it would have to be const qualified
 - `poll() - T {return funk();}`

#### `Convertible<From, To>`
 - extends `Gettable<To>`
 - `source - Gettable<From> *`
 - `Get() - To` - Converts source to To

#### `ConvertSet<From, To>`
 - extends `Gettable<To>, Settable<To>`
 - `source - SetValue<From> *`
 - `Get() - To`
 - `Set(T)`

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
 - `default - ValueStore<T> *, static`
 - `values - map<string, Valuable<T> *>`
 - `mutex r, w`
 - `Value`
   - `obj - pair<string, Valuable<T> *> *` - Points to the string, value pair in the parent ValueStore
   - `setter[=null] - Settable<T> *`
   - `GetKey() - string`
   - `GetValue() - T`
   - `SetValue(val - T)` - If setter is null throws an exception/logs fatal error. Otherwise calls setter->Set(val)
   - `Initialize(obj - Valuable<T> *)` - Sets the mapped value to obj. Throws exception/logs fatal error if current value isn't null
   - `Initialize(obj - Pollable<T> *, updateStore = UpdateStore::default_store - UpdateStore*)` - Calls above function and adds the Updateable part of the object to the specified UpdateThread at the same key.
   - `Initialize(obj - Settable<T> *)` - Calls the override with Gettable but also stores the setter.
   - `Release() - Valuable<T> *` - This will set the mapped value and setter to null and return what was in there.
 - `Get(key - string) - Value` - Returns and, if needed, creates a mapped object at `key`
 - `Initialize(key - string, obj - Valuable<T> *)`
 - `Initialize(key - string, obj - Valuable<T> *, updateStore = UpdateStore::default_store - UpdateStore*)`

### Logs

#### `TextLogListener`
 - `Log(line - log), virtual`
 - `Log(timestamp, LogData, SystemData, message), virtual`

#### `TextLog`
 - `listeners - vector<TextLogListener *>`
 - `Log(timestamp, LogData..., SystemData..., message)`

### Network

#### `MessageWatch`
 - `OnMessage(topic - string, message - string), virtual`
 - `SetFilter(topicPrefix - string)`

#### `MessageBroadcast`
 - `Publish(topic - string, message - string), virtual`
 - `SetFilter(topicPrefix - string)`

#### `MQTT`
 - extends `mosqpp::mosquittopp` or whatever it is, and `MessageBroadcast`
 - `watches - vector<MessageWatch *>`
 - `AddWatch(watch - MessageWatch *)`
 - `Publish(topic - string, message - string)`
 - `Subscribe(topic - string)`
 - `on_message(topic - string, message - string)` dispatch message to all watches

#### `MessageCache`
 - extends `MessageWatch` - The ValueStore is configured with References
 - `cache_obj - ValueStore<string>` - each is a Settable
 - `cache_map - unordered_map<string, ValueStore<string>::Value>` - Map of Values pointing to cache_obj data.
 - `OnMessage(topic - string, message - string)` - set the value in the setters at the topic. 
 - `GetCache() - const ValueStore<string>&`

#### `MessageUpdater`
 - extends `UpdateThread, MessageBroadcast`
 - `publisher - MessageBroadcast *`
 - `UpdateObject`
   - extends `Updateable`
   - `watch - const ValueStore<string>::Value`
   - `publisher - MessageBroadcast *` - parent MessageUpdater
   - `Update()` - Calls publisher's publish for the watch's key and value
 - `Publish(topic - string, message - string)` - Calls publisher's publish.

### Robot Framework

#### `Command`
 - WPI Command

#### `CommandBindBase`
 - `make(args - ArgumentSet) - Command *, virtual {return make();}` - factory function for Commands based on a set of arguments
 - `make() - Command *, virtual` - factory function for Commands with default args presumably

#### `CommandBind<Command_t>`
 - extends `CommandBindBase`
 - `make(args - ArgumentSet) - Command *, {return MakeCommand<Command_t>(args);}` - Calls external function intended to be specialized by user
 - `make() - Command *, {return new Command_t();}` - Expects Command to have a default constructor.

#### `CommandFactory`
 - `commands - map<string, CommandBindBase *>`
 - `MakeCommand(comm - string, args - ArgumentSet) - Command *`
 - `RegisterCommand(string, CommandBindBase *)`

