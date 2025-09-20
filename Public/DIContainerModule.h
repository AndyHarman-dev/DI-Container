#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * @class FDIContainerModule
 * @brief Represents a module interface for dependency injection within the application.
 *
 * This class implements the IModuleInterface and provides functionality
 * to manage the startup and shutdown of the module.
 */
class FDIContainerModuleModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
