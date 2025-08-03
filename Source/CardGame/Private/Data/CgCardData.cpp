// Copyright Vinipi. All Rights Reserved.


#include "CardGame/Public/Data/CgCardData.h"

UAnimMontage* UCgCardData::GetRandomAttackMontage() const
{
	return AttackMontages[FMath::RandRange(0, AttackMontages.Num()-1)];
}
