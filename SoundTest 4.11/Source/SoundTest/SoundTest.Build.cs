// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class SoundTest : ModuleRules
{
	public SoundTest(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "RHI", "Kiss_FFT" });


        PrivateDependencyModuleNames.AddRange(new string[] {  });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");
        // if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
        // {
        //		if (UEBuildConfiguration.bCompileSteamOSS == true)
        //		{
        //			DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
        //		}
        // }

        if (Target.Platform == UnrealTargetPlatform.Win64 || Target.Platform == UnrealTargetPlatform.Win32)
        {
            // VS2015 updated some of the CRT definitions but not all of the Windows SDK has been updated to match.
            // Microsoft provides this shim library to enable building with VS2015 until they fix everything up.
            //@todo: remove when no longer neeeded (no other code changes should be necessary).
            if (WindowsPlatform.Compiler == WindowsCompiler.VisualStudio2015)
            {
                PublicAdditionalLibraries.Add("legacy_stdio_definitions.lib");
            }
        }

        AddThirdPartyPrivateStaticDependencies(Target, "Kiss_FFT");
    }
}
