// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Nodes/InstantDialogueNode.h"
#include "Nodes/InstantDialogueNodeAnswer.h"
//#include "SedNode_Anwser.h"
#include "SEdNode_InstantDialogueAnswer.h"
#include "EdNode_InstantDialogueNode.h"


#include "EdNode_InstantDialogueAnswer.generated.h"

//class SedNode_Anwser;
/**
 * 
 */
//class UEdNode_InstantDialogueEdge;
//class UEdGraph_InstantDialogue;

class SedNode_Anwser;
UCLASS(MinimalAPI)
class UEdNode_Anwser : public UEdNode_InstantDialogueNode //UEdGraphNode  UEdNode_InstantDialogueNode
{
	GENERATED_BODY()
	
public:
	SedNode_Anwser* SEdNode;
	
	UEdNode_Anwser();
	virtual ~UEdNode_Anwser();

	void SetInstantDialogueNode(UInstantDialogueNodeAnwser* InNode);
	
	
	virtual void AllocateDefaultPins() override;
	
	virtual void AddPinToInstantDialogueNode(const int32 Index = INDEX_NONE) override;
	virtual void RemovePinFromInstantDialogueNode(UEdGraphPin* TargetPin) override;
	/*UPROPERTY(VisibleAnywhere, Instanced, Category = "InstantDialogue")
	UInstantDialogueNodeAnwser* InstantDialogueNode; // UInstantDialogueNode

	void SetInstantDialogueNode(UInstantDialogueNodeAnwser* InNode);
	UEdGraph_InstantDialogue* GetInstantDialogueEdGraph();

	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void PrepareForCopying() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;

	virtual FLinearColor GetBackgroundColor() const;
	virtual UEdGraphPin* GetInputPin() const;
	virtual UEdGraphPin* GetOutputPin() const;

#if WITH_EDITOR
	virtual void PostEditUndo() override;
#endif*/

};
