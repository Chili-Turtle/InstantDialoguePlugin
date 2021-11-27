// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Nodes/InstantDialogueNode.h"

#include "InstantDialogueNodeAnswer.generated.h"

/**
 * 
 */
#define LOCTEXT_NAMESPACE "InstantDialogueNodeAnswer"

 
UCLASS(Blueprintable)
class INSTANTDIALOGUERUNTIME_API UInstantDialogueNodeAnwser : public UInstantDialogueNode
{
	GENERATED_BODY()
public:
	UInstantDialogueNodeAnwser();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "InstantDialogueNode_Editor") //BlueprintCallable can't use this why?
	TArray<int32> ConditionIndex;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "InstantDialogueNode_Editor") //BlueprintCallable
    TArray<FText> NameText; //= {LOCTEXT("First", "First Entry")};
    
    UPROPERTY(BlueprintReadWrite,Category = "InstantDialogueNode_Editor") //BlueprintCallable no
    TArray<FName> NameOfName;

	UPROPERTY(BlueprintReadWrite, BlueprintReadWrite,Category = "InstantDialogueNode_Editor")
	uint32 testBool : 1;

	FText* GetNameTextElement();

	virtual FText GetDescription_Implementation() const override;

};

#undef LOCTEXT_NAMESPACE
