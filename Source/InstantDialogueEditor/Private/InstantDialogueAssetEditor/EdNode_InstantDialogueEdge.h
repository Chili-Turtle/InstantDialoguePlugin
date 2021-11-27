#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "EdNode_InstantDialogueEdge.generated.h"

class UInstantDialogueNode;
class UInstantDialogueEdge;
class UEdNode_InstantDialogueNode;

UCLASS(MinimalAPI)
class UEdNode_InstantDialogueEdge : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class UEdGraph* Graph;

	UPROPERTY(VisibleAnywhere, Instanced, Category = "InstantDialogue")
	UInstantDialogueEdge* InstantDialogueEdge;

	void SetEdge(UInstantDialogueEdge* Edge);

	virtual void AllocateDefaultPins() override;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;

	virtual void PrepareForCopying() override;

	virtual UEdGraphPin* GetInputPin() const { return Pins[0]; }
	virtual UEdGraphPin* GetOutputPin() const { return Pins[1]; }

	void CreateConnections(UEdNode_InstantDialogueNode* Start, UEdNode_InstantDialogueNode* End);

	UEdNode_InstantDialogueNode* GetStartNode();
	UEdNode_InstantDialogueNode* GetEndNode();
};
