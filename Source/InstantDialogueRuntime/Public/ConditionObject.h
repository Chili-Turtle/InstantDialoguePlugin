// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ConditionObject.generated.h"

//UCLASS(Blueprintable, BlueprintType, abstract, EditInlineNew, HideCategories = ("DoNotShow"), CollapseCategories, AutoExpandCategories = ("Default"))
UCLASS(Blueprintable) //EditInlineNew
class INSTANTDIALOGUERUNTIME_API UInstantDialogueConditionObject final : public UObject
{
	GENERATED_BODY()
	
public:

	// Checks if the condition is met
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Dialogue")
	bool IsConditionMet();
	virtual bool IsConditionMet_Implementation()
	{
		return false;
	}
	
	virtual class UWorld* GetWorld() const override;
};
