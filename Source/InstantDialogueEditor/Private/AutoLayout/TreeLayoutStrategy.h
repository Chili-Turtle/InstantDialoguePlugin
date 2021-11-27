#pragma once

#include "CoreMinimal.h"
#include "AutoLayoutStrategy.h"
#include "TreeLayoutStrategy.generated.h"

UCLASS()
class UTreeLayoutStrategy : public UAutoLayoutStrategy
{
	GENERATED_BODY()
public:
	UTreeLayoutStrategy();
	virtual ~UTreeLayoutStrategy();

	virtual void Layout(UEdGraph* EdGraph) override;

protected:
	void InitPass(UInstantDialogueNode* RootNode, const FVector2D& Anchor);
	bool ResolveConflictPass(UInstantDialogueNode* Node);

	bool ResolveConflict(UInstantDialogueNode* LRoot, UInstantDialogueNode* RRoot);

	void GetLeftContour(UInstantDialogueNode* RootNode, int32 Level, TArray<UEdNode_InstantDialogueNode*>& Contour);
	void GetRightContour(UInstantDialogueNode* RootNode, int32 Level, TArray<UEdNode_InstantDialogueNode*>& Contour);
	
	void ShiftSubTree(UInstantDialogueNode* RootNode, const FVector2D& Offset);

	void UpdateParentNodePosition(UInstantDialogueNode* RootNode);
};
