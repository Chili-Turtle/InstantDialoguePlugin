#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "Nodes/InstantDialogueNode.h"
#include "EdNode_InstantDialogueNode.generated.h"

class UEdNode_InstantDialogueEdge;
class UEdGraph_InstantDialogue;
class SEdNode_InstantDialogueNode;

UCLASS(MinimalAPI)
class UEdNode_InstantDialogueNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UEdNode_InstantDialogueNode();
	virtual ~UEdNode_InstantDialogueNode();

	UPROPERTY(VisibleAnywhere, Instanced, Category = "InstantDialogue")
	UInstantDialogueNode* InstantDialogueNode;

	void SetInstantDialogueNode(UInstantDialogueNode* InNode);
	UEdGraph_InstantDialogue* GetInstantDialogueEdGraph();

	SEdNode_InstantDialogueNode* SEdNode;

	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void PrepareForCopying() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;

	virtual FLinearColor GetBackgroundColor() const;
	virtual UEdGraphPin* GetInputPin() const;
	virtual UEdGraphPin* GetOutputPin() const;

	virtual void AddPinToInstantDialogueNode(const int32 Index = INDEX_NONE);
	virtual void RemovePinFromInstantDialogueNode(UEdGraphPin* TargetPin);
	////////////////////////////// flowgraph
	//TArray<UEdGraphPin*> InputPins;
	//TArray<UEdGraphPin*> OutputPins;
	//////////////////////////////

#if WITH_EDITOR
	virtual void PostEditUndo() override;
#endif

};
