#pragma once

#include "CoreMinimal.h"
#include "InstantDialogueNodeData.h"

//#include "InstantDialoguePin.h"
//#include "InstantDialogueNode_Condition.h"
//#include "InstantDialogueGraph.h"
//#include "Nodes/InstantDialogueNode_Condition.h"
//#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
//#include "Components/Image.h"
//#include "Templates/SubclassOf.h"
#include "InstantDialogueNode.generated.h"

class UInstantDialogue;
class UInstantDialogueEdge;
//class UInstantDialogueCondition;

DECLARE_MULTICAST_DELEGATE(FForcePropertyEditorUpdate);

// This class does not need to be modified.


UCLASS(Blueprintable)
class INSTANTDIALOGUERUNTIME_API UInstantDialogueNode : public UObject
{
	GENERATED_BODY()

public:
	UInstantDialogueNode();
	virtual ~UInstantDialogueNode();

	UPROPERTY(VisibleDefaultsOnly, Category = "InstantDialogueNode")
	UInstantDialogue* Graph;

	UPROPERTY(BlueprintReadOnly, Category = "InstantDialogueNode")
	TArray<UInstantDialogueNode*> ParentNodes;

	UPROPERTY(BlueprintReadOnly, Category = "InstantDialogueNode")
	TArray<UInstantDialogueNode*> ChildrenNodes; //Ed_NodeGraph RebuildNodeGraph

	UPROPERTY(BlueprintReadOnly, Category = "InstantDialogueNode")
	TMap<int32, UInstantDialogueNode*> OutputMap; //also childrenNode 
	
	UPROPERTY(EditDefaultsOnly, Category = "InstantDialogueNode")
	TMap<int32, FString> OutputString; 

	UPROPERTY(BlueprintReadOnly, Category = "InstantDialogueNode")
	TMap<UInstantDialogueNode*, UInstantDialogueEdge*> Edges;

	UFUNCTION(BlueprintCallable, Category = "InstantDialogueNode")
	virtual UInstantDialogueEdge* GetEdge(UInstantDialogueNode* ChildNode);

	//UFUNCTION(BlueprintCallable, Category = "InstantDialogueNode")
	//bool IsLeafNode() const;

	UFUNCTION(BlueprintCallable, Category = "InstantDialogueNode")
	UInstantDialogue* GetGraph() const;
	
	UFUNCTION(BlueprintCallable, Category = "InstantDialogueNode")
	void PrintNodeTitle() const;

	UFUNCTION(BlueprintCallable, Category = "InstantDialogueNode")
	void PrintGetName() const;

	UFUNCTION(BlueprintCallable, Category = "InstantDialogueNode")
	void PrintClasseres() const;
	
	UFUNCTION(BlueprintCallable, Category = "InstantDialogueNode")
	void PrintOutputPins() const;
	
	UFUNCTION(BlueprintCallable, Category = "InstantDialogueNode")
	int32 GetOutputAmount();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "InstantDialogueNode")
	FText GetDescription() const;
	virtual FText GetDescription_Implementation() const;

	//////////////////////////////////////////////////////////////////////////
//#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "InstantDialogueNode_Editor")
	FText NodeTitle;

	//FForcePropertyEditorUpdate OnForcePropertyEditorUpdate;

	/*TMap<uint8, FInstantDialoguePinRecord> GetWireRecords() const;
	
	TMap<FName, TArray<FInstantDialoguePinRecord>> InputRecords;
	TMap<FName, TArray<FInstantDialoguePinRecord>> OutputRecords;*/
	
	TArray<UEdGraphPin*> OutputPins;

	//UPROPERTY(EditAnywhere, Category = "InstantDialogueNode_Editor") // Meta = (BitMask),
	//uint32 MyBoolVar : 1;

	UPROPERTY(VisibleDefaultsOnly, Category = "InstantDialogueNode_Editor")
	TSubclassOf<UInstantDialogue> CompatibleGraphType;

	UPROPERTY(EditDefaultsOnly, Category = "InstantDialogueNode_Editor")
	FLinearColor BackgroundColor;

	UPROPERTY(EditDefaultsOnly, Category = "InstantDialogueNode_Editor")
	FText ContextMenuName; //this is used as the standart name for right click menu (not enabled atm)
//#endif

//#if WITH_EDITOR
	virtual FLinearColor GetBackgroundColor() const;

	EDialogueNodeType NodeType = EDialogueNodeType::Default;

	virtual FText GetNodeTitle() const;
	
#if IF_WITH_EDITORONLY_DATA
	virtual void PostEditChangeProperty( struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual void SetNodeTitle(const FText& NewTitle);

	virtual bool CanCreateConnection(UInstantDialogueNode* Other, FText& ErrorMessage);
//#endif
};