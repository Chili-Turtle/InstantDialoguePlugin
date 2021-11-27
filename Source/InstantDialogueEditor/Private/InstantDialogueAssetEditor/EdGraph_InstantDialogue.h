#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph_InstantDialogue.generated.h"

class UInstantDialogue;
class UInstantDialogueNode;
class UInstantDialogueEdge;
class UEdNode_InstantDialogueNode;
class UEdNode_InstantDialogueEdge;

UCLASS()
class UEdGraph_InstantDialogue : public UEdGraph
{
	GENERATED_BODY()

public:
	UEdGraph_InstantDialogue();
	virtual ~UEdGraph_InstantDialogue();

	virtual void RebuildInstantDialogue();

	UInstantDialogue* GetInstantDialogue() const;

	virtual bool Modify(bool bAlwaysMarkDirty = true) override;
	virtual void PostEditUndo() override;

	UPROPERTY(Transient)
	TMap<UInstantDialogueNode*, UEdNode_InstantDialogueNode*> NodeMap;

	UPROPERTY(Transient)
	TMap<UInstantDialogueEdge*, UEdNode_InstantDialogueEdge*> EdgeMap;

protected:
	void Clear();

	void SortNodes(UInstantDialogueNode* RootNode);
};
