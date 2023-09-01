// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System;
using System.IO;

public class Test_LidarSample : ModuleRules
{
	private string ModulePath
	{
		get { return ModuleDirectory; }
	}

	// Photon lib 
	private string PhotonPath
	{
		get { return Path.GetFullPath(Path.Combine(ModulePath, "..", "Photon")); }
	}

	// gpg lib // \gpg\lib\c++\armeabi-v7a\libgpg.a
	private string GpgPath
	{
		get { return Path.GetFullPath(Path.Combine(ModulePath, "..", "gpg")); }
	}

	// firebase sdk lib
	private string FirebasePath
	{
		get { return Path.GetFullPath(Path.Combine(ModulePath, "..", "firebase_cpp_sdk")); }
	}

	private delegate void AddLibFunction(ReadOnlyTargetRules target, string name);
	private readonly string[] libraries = { "Common", "Photon", "LoadBalancing", "Chat", "PhotonVoice" };
	private readonly string[] Phton3rdlibraries = { "crypto", "websockets", "ssl" };
	private readonly string[] Firebaselibraries = { "firebase_app", "firebase_auth", "firebase_functions" }; 
	// "firebase_admob", "firebase_analytics",  "firebase_database", "firebase_dynamic_links",
    // "firebase_firestore", "firebase_gma", "firebase_installations", "firebase_messaging", "firebase_remote_config", "firebase_storage"
	// private readonly string libArm = "armeabi-v7a_libc++.a";

	private readonly string[] PhotonlibArm = { "arm64-v8a_libc++.a" }; // "armeabi-v7a_libc++.a", 
	// armeabi-v7a
	// armeabi-v7a_libc++			// 4.27
	// armeabi-v7a_no-rtti 
	// armeabi-v7a_libc++_no-rtti

	// Photon voice 
	// PhotonVoice-cpp_vc16_release_windows_md_Win32.lib
	// PhotonVoice-cpp_vc16_release_windows_md_x64.lib

	private void AddPhotonLibPathWin(ReadOnlyTargetRules target, string name)
	{
		string PlatformString = (target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "Win32";
		PublicAdditionalLibraries.Add(Path.Combine(PhotonPath, "lib", "Windows",
			name + "-cpp_vc16_release_windows_md_" + PlatformString + ".lib"));
	}

	private void AddPhotonLibPathAndroid(ReadOnlyTargetRules target, string name)
	{
		for(int j = 0; j < PhotonlibArm.Length; j++)
        {
			PublicAdditionalLibraries.Add(Path.Combine(PhotonPath, "lib", "Android",
			"lib" + name.ToLower() + "-cpp-static_release_android_" + PhotonlibArm[j]));
		}
	}

	private void AddPhoton3rdPartyLibPathAndroid(ReadOnlyTargetRules target, string name)
	{
		for (int j = 0; j < PhotonlibArm.Length; j++)
		{
			PublicAdditionalLibraries.Add(Path.Combine(PhotonPath, "lib", "Android", "lib" + name.ToLower() + "_release_android_" + PhotonlibArm[j]));
		}
	}

	public void LoadPhotonSDK(ReadOnlyTargetRules target)
	{
		// Include path
		PublicIncludePaths.Add(PhotonPath);
		// PublicIncludePaths.Add(GpgPath);
		
		AddLibFunction AddLibFunc;

		if ((Target.Platform == UnrealTargetPlatform.Win64) )
		{
			PublicDefinitions.Add("_EG_WINDOWS_PLATFORM");
			PublicDefinitions.Add("INTERNAL_EXPERIMENTAL");
			
			AddLibFunc = AddPhotonLibPathWin;

			// // Firebase Test cs
			//for (int i = 0; i < Firebaselibraries.Length; i++)
			//{                                                                                       // MD MT       // Debug Release
			//	PublicAdditionalLibraries.Add(Path.Combine(FirebasePath, "libs", "windows", "VS2019", "MD", "x64", "Release", Firebaselibraries[i] + ".lib"));
			//}
			
			// ���̾� ���̽� x
			// PublicIncludePaths.Add(FirebasePath + "/include");
			// PrivateIncludePaths.Add(FirebasePath + "/include");

			// ���̽�ê �߰� ���̺귯��
			string PlatformString = (target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "Win32";
			PublicAdditionalLibraries.Add(Path.Combine(PhotonPath, "lib", "Windows", "opus_egpv_vc16_release_windows_md_" + PlatformString + ".lib"));
		}
		else if (Target.Platform == UnrealTargetPlatform.Android)
		{
			//Target.AndroidPlatform.Architectures
			PublicDefinitions.Add("_EG_ANDROID_PLATFORM");
			AddLibFunc = AddPhotonLibPathAndroid;

			// ���� ���̺귯�� ���� ���濡 ���� �߰� ����
			for (int i = 0; i < Phton3rdlibraries.Length; i++)
			{
				AddPhoton3rdPartyLibPathAndroid(target, Phton3rdlibraries[i]);
			}

			PrivateDependencyModuleNames.Add("Launch");

			// gpg
			//PublicAdditionalLibraries.Add(Path.Combine(GpgPath, "lib", "c++", "armeabi-v7a", "libgpg.a"));
			//PrivateDependencyModuleNames.Add("OnlineSubsystemGooglePlay");
			//PrivateDependencyModuleNames.Add("AndroidAdvertising");

		}
		else
		{
			throw new Exception("\nTarget platform not supported: " + Target.Platform);
		}

		for (int i = 0; i < libraries.Length; i++)
		{
			AddLibFunc(target, libraries[i]);
		}
	}

	public Test_LidarSample(ReadOnlyTargetRules Target) : base(Target)
	{
		bUseUnity = false;
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Voice" ,
			"Networking", "Sockets","OnlineSubsystem", "OnlineSubsystemUtils",
			"LibOVRPlatform", 
			"Slate", "SlateCore", "WebBrowser", "HTTP",
			"PlayFab", "PlayFabCpp", "PlayFabCommon",
			// "Firebase",
			// "Facebook", 
			"Json" ,
			"JsonUtilities",
			// �ܺ� �÷�����
			"AudioExtensions", //"UROSBridge",
			// "FirebaseAuthentication",
			 });

		PrivateDependencyModuleNames.AddRange(new string[] { "UMG", "WebSockets",
			"OnlineSubsystem" ,
			"OnlineSubsystemUtils",
			"NavigationSystem", "AIModule" ,
		});

		//Load Photon Cloud
		LoadPhotonSDK(Target);
	}
}
