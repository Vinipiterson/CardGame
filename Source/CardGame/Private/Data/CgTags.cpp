// Copyright Vinipi. All Rights Reserved.

#include "Data/CgTags.h"

namespace CgSetByCallerTags
{
	UE_DEFINE_GAMEPLAY_TAG(Cost, FName{TEXTVIEW("CardGame.SetByCaller.Cost")});
}

namespace CgEventTags
{
	UE_DEFINE_GAMEPLAY_TAG(UseCard, FName{TEXTVIEW("CardGame.Event.UseCard")})
}

namespace CgAbilityTags
{
	UE_DEFINE_GAMEPLAY_TAG(UseCard, FName{TEXTVIEW("CardGame.Ability.UseCard")});
	UE_DEFINE_GAMEPLAY_TAG(HitReact, FName{TEXTVIEW("CardGame.Ability.HitReact")});
	UE_DEFINE_GAMEPLAY_TAG(Die, FName{TEXTVIEW("CardGame.Ability.Die")});
}
namespace CgTeamTags
{
	UE_DEFINE_GAMEPLAY_TAG(Team1, FName{TEXTVIEW("CardGame.Team.1")});
	UE_DEFINE_GAMEPLAY_TAG(Team2, FName{TEXTVIEW("CardGame.Team.2")});
}
