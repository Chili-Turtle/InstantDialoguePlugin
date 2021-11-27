#pragma once

#include "CoreMinimal.h"
#include "Nodes/InstantDialogueNode.h"
#include "InstantDialogueEdge.generated.h"

class UInstantDialogue;

UCLASS(Blueprintable)
class INSTANTDIALOGUERUNTIME_API UInstantDialogueEdge : public UObject
{
	GENERATED_BODY()

public:
	UInstantDialogueEdge();
	virtual ~UInstantDialogueEdge();

	UPROPERTY(VisibleAnywhere, Category = "InstantDialogueNode")
	UInstantDialogue* Graph;

	UPROPERTY(BlueprintReadOnly, Category = "InstantDialogueEdge")
	UInstantDialogueNode* StartNode;

	UPROPERTY(BlueprintReadOnly, Category = "InstantDialogueEdge")
	UInstantDialogueNode* EndNode;

	UFUNCTION(BlueprintPure, Category = "InstantDialogueEdge")
	UInstantDialogue* GetGraph() const;
};
