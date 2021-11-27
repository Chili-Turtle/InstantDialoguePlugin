// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ConditionObject.h"
#include "Nodes/InstantDialogueNode.h"
//#include "UObject/NoExportTypes.h"
#include "InstantDialogueNodeTest.generated.h"

class UInstantDialogueConditionObject;
/**
 * 
 */
UCLASS(Blueprintable)
class INSTANTDIALOGUERUNTIME_API UInstantDialogueNodeTest : public UInstantDialogueNode
{
	GENERATED_BODY()
public:
	UInstantDialogueNodeTest();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InstantDialogueNode_Editor") 
	TArray<TSubclassOf<UInstantDialogueConditionObject>> ConditionsArray;
	
	UFUNCTION(BlueprintCallable, Category = "InstantDialogueNode_Editor") 
	int GetConditionPin();

	UFUNCTION(BlueprintCallable, Category = "InstantDialogueNode_Editor")
	int GetConditionPinImplementation();
     
	UFUNCTION(BlueprintCallable, Category = "InstantDialogueNode")
	virtual void PrintCondition();

	virtual FText GetDescription_Implementation() const override;

	//EDialogueNodeType NodeType = EDialogueNodeType::Condition;
};
