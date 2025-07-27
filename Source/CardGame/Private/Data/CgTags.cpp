// Copyright Vinipi. All Rights Reserved.

#include "Data/CgTags.h"

namespace CgSetByCallerTags
{
	UE_DEFINE_GAMEPLAY_TAG(Cost, FName{TEXTVIEW("CardGame.SetByCaller.Cost")});
}

namespace CgEventTags
{
	UE_DEFINE_GAMEPLAY_TAG(UseCard, FName{TEXTVIEW("CardGame.Event.UseCard")});
}

namespace CgAbilitieTags
{
	UE_DEFINE_GAMEPLAY_TAG(UseCard, FName{TEXTVIEW("CardGame.Ability.UseCard")});
}
