using UnrealBuildTool;

public class DIContainerModule : ModuleRules
{
    public DIContainerModule(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        CppStandard = CppStandardVersion.Cpp20;
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );
        
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "UELoggingModule"
            }
        );
    }
}
