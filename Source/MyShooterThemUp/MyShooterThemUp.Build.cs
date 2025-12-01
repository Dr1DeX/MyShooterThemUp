// Shoot Them Up Game, All Rights Reserved.

using UnrealBuildTool;

public class MyShooterThemUp : ModuleRules
{
    public MyShooterThemUp(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore",
            "Niagara",
            "PhysicsCore",
            "GamePlayTasks",
            "NavigationSystem",
        });

        PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(new string[] {
            "MyShooterThemUp/Public/Player",
            "MyShooterThemUp/Public/Components",
            "MyShooterThemUp/Public/Dev",
            "MyShooterThemUp/Public/Weapon",
            "MyShooterThemUp/Public/UI",
            "MyShooterThemUp/Public/Animations",
            "MyShooterThemUp/Public/Pickups",
            "MyShooterThemUp/Public/Weapon/Components",
            "MyShooterThemUp/Public/AI",
            "MyShooterThemUp/Public/AI/Tasks",
            "MyShooterThemUp/Public/AI/Services",
            "MyShooterThemUp/Public/AI/EQS",
            "MyShooterThemUp/Public/AI/Decorators",
        });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
