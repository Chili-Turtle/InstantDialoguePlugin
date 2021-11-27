// Fill out your copyright notice in the Description page of Project Settings.


#include "ConditionObject.h"

UWorld* UInstantDialogueConditionObject::GetWorld() const // CAN I DELETE THIS?
{
	if (HasAnyFlags(RF_ArchetypeObject | RF_ClassDefaultObject))
	{
		return nullptr;
	}

	// Get from outer
	if (UObject* Outer = GetOuter())
	{
		if (UWorld* World = Outer->GetWorld())
		{
			return World;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("No World found, PC shuts down in 3.....2.....1....."));
	return nullptr; //UGameInstance
}