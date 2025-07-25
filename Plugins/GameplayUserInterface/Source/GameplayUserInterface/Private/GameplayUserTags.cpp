// Copyright Spike Plugins 2025. All Rights Reserved.

#include "GameplayUserTags.h"

namespace GameplayUserTags
{
	namespace LayerStack
	{
		UE_DEFINE_GAMEPLAY_TAG(LayerStack_Menu,							"GameplayUser.LayerStack.Menu")
		UE_DEFINE_GAMEPLAY_TAG(LayerStack_Game,							"GameplayUser.LayerStack.Game")
		UE_DEFINE_GAMEPLAY_TAG(LayerStack_GameMenu,						"GameplayUser.LayerStack.GameMenu")
		UE_DEFINE_GAMEPLAY_TAG(LayerStack_Modal,						"GameplayUser.LayerStack.Modal")
	}

	namespace Screen
	{
		UE_DEFINE_GAMEPLAY_TAG(Screen_PressAnyKey, 						"GameplayUser.Screen.PressAnyKey")
		UE_DEFINE_GAMEPLAY_TAG(Screen_MainMenu, 						"GameplayUser.Screen.MainMenu")
		UE_DEFINE_GAMEPLAY_TAG(Screen_Story, 							"GameplayUser.Screen.Story")
		UE_DEFINE_GAMEPLAY_TAG(Screen_Options, 							"GameplayUser.Screen.Options")
		UE_DEFINE_GAMEPLAY_TAG(Screen_KeyRemap, 						"GameplayUser.Screen.KeyRemap")
	}

	namespace Description
	{
		UE_DEFINE_GAMEPLAY_TAG(Description_TestImage, 					"GameplayUser.Description.TestImage")
	}
}

