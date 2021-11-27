// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SedNode_InstantDialogueCondition.h"
#include "EdNode_InstantDialogueNode.h"


#include "EdNode_InstantDialogueCondition.generated.h"

/**
 * 
 */
 class SedNode_Condition;
 UCLASS(MinimalAPI)
class UEdNode_Condition : public UEdNode_InstantDialogueNode
{
 	GENERATED_BODY()
public:
	UEdNode_Condition();
	~UEdNode_Condition();

 	SedNode_Condition* SEdNode;

 	void SetInstantDialogueNode(UEdNode_Condition* InNode);

 	virtual void AllocateDefaultPins() override;

 	virtual void AddPinToInstantDialogueNode(const int32 Index = INDEX_NONE) override;
 	virtual void RemovePinFromInstantDialogueNode(UEdGraphPin* TargetPin) override;
};
