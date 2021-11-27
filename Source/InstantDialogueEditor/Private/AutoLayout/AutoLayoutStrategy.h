#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraph.h"
#include "InstantDialogueGraph.h"
#include "InstantDialogueAssetEditor/EdGraph_InstantDialogue.h"
#include "InstantDialogueAssetEditor/Nodes/EdNode_InstantDialogueNode.h"
#include "InstantDialogueAssetEditor/EdNode_InstantDialogueEdge.h"
#include "InstantDialogueAssetEditor/Settings_InstantDialogueEditor.h"
#include "AutoLayoutStrategy.generated.h"

UCLASS(abstract)
class UAutoLayoutStrategy : public UObject
{
	GENERATED_BODY()
public:
	UAutoLayoutStrategy();
	virtual ~UAutoLayoutStrategy();

	virtual void Layout(UEdGraph* G) {};

	class UInstantDialogueEditorSettings* Settings;

protected:
	int32 GetNodeWidth(UEdNode_InstantDialogueNode* EdNode);

	int32 GetNodeHeight(UEdNode_InstantDialogueNode* EdNode);

	FBox2D GetNodeBound(UEdGraphNode* EdNode);

	FBox2D GetActualBounds(UInstantDialogueNode* RootNode);

	virtual void RandomLayoutOneTree(UInstantDialogueNode* RootNode, const FBox2D& Bound);

protected:
	UInstantDialogue* Graph;
	UEdGraph_InstantDialogue* EdGraph;
	int32 MaxIteration;
	int32 OptimalDistance;
};
