// Copyright Vinipi. All Rights Reserved.


#include "CardGame/Public/Data/CgCardData.h"

UAnimMontage* UCgCardData::GetRandomMaleeMontage() const
{
	return MaleeMontages[FMath::RandRange(0, MaleeMontages.Num()-1)];
}
