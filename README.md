# DIContainerModule
A lightweight Unreal Engine C++20 module providing a dependency injection container for registering and resolving UObject instances by class and optional name, with both C++ and Blueprint APIs. It supports type‑safe templates for concrete types and UInterfaces, plus runtime validation and logging hooks for safe registration and resolution flows.

### Features
- Register and resolve UObject instances keyed by class and optional FName via FHashableClassAndNameKey.
- C++ templates for type‑safe Register/Resolve, including support for Unreal UInterface types.
- BlueprintCallable Register, Unregister, and Resolve using FHashableClassAndNameKey and UObject parameters/returns.
- Safety checks with ValidateMsfg and UE_LOG for invalid keys, types, and duplicates.
- Clean Unregister APIs by instance or by key for lifecycle management.
- Unreal Engine module with C++20, depending on Core, CoreUObject, and Engine.

### Tech stack
- C++20 (ModuleRules.CppStandard = Cpp20).
- Unreal Engine module (IModuleInterface with StartupModule/ShutdownModule).
- UE modules: Core (public), CoreUObject and Engine (private).

### Installation
- Add DIContainerModule to the project’s Source or as a plugin module and include it as a dependency from consumer modules.
- In a consumer module’s .Build.cs, depend on DIContainerModule to use UDIContainer.

Example Build.cs snippet (consumer):
```csharp
PublicDependencyModuleNames.AddRange(new string[] { "Core", "DIContainerModule" });
PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine" });
```


### Usage

#### Create a container instance
```cpp
UDIContainer* Container = NewObject<UDIContainer>();
check(Container);
```


#### Register by class key and optional name
```cpp
UObject* MyServiceInstance = /* existing UObject instance implementing UMyService or subclassing UMyServiceBase */;

FHashableClassAndNameKey KeyByClass(UMyServiceBase::StaticClass());
Container->Register(KeyByClass, MyServiceInstance);

FHashableClassAndNameKey KeyNamed(UMyServiceBase::StaticClass(), FName("Default"));
Container->Register(KeyNamed, MyServiceInstance);
```


#### Resolve by key
```cpp
UObject* Resolved = Container->Resolve(KeyByClass);
UObject* ResolvedNamed = Container->Resolve(KeyNamed);
```


#### Type‑safe C++ templates (concrete class)
```cpp
// Register/resolve by concrete base class
Container->Register<UMyServiceBase>(MyServiceInstance);
UMyServiceBase* Service = Container->Resolve<UMyServiceBase>();
```


#### Type‑safe C++ templates (UInterface)
```cpp
// If IMyService is an interface (with UMyService as UClassType)
Container->Register<IMyService>(MyServiceInstance);
IMyService* ServiceIf = Container->Resolve<IMyService>();
```


#### Named registrations with templates
```cpp
TOptional<FName> Name = FName("Default");
Container->Register<UMyServiceBase>(Name, MyServiceInstance);
UMyServiceBase* NamedService = Container->Resolve<UMyServiceBase>(Name);
```


#### Unregister
```cpp
// Remove by instance
Container->UnregisterByInstance(MyServiceInstance);

// Or remove by key
Container->UnregisterByKey(KeyByClass);
```


Notes:
- Registration validates that the instance is valid and either derives from the class or implements the interface, logging warnings or errors via ValidateMsfg/UE_LOG if checks fail.
- Duplicate registration for the same key logs a warning and does not overwrite the existing entry.

### API documentation

#### Types
- FHashableClassAndNameKey: carries UClass* Class and optional FName OptionalName, supports equality and GetTypeHash for map keys.
- UDIContainer: UObject subclass exposing C++ and Blueprint APIs for registering, unregistering, and resolving instances.

#### Blueprint/C++ functions (UFUNCTION)
- void Register(FHashableClassAndNameKey Key, UObject* InInstance).
- void UnregisterByInstance(UObject* InInstance).
- void UnregisterByKey(FHashableClassAndNameKey Key).
- UObject* Resolve(FHashableClassAndNameKey Key).

#### C++ templates
- Register<BaseType>(const DerivedFrom<BaseType> auto& InInstance) and overload handling UInterface’s UClassType.
- Register<BaseType>(const TOptional<FName> OptionalName, const DerivedFrom<BaseType> auto& InInstance) and UInterface overload.
- Resolve<BaseType>() and Resolve<BaseType>(const TOptional<FName> OptionalName) with UInterface overloads.

### Contributing
- Open issues and pull requests following standard GitHub workflows; include clear descriptions, minimal reproductions, and tests where applicable.
- Keep style consistent with Unreal Engine conventions and this module’s headers, and update README/API docs when public APIs change.
