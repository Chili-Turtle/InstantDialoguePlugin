#pragma once

#include "CoreMinimal.h"
#include "Nodes/InstantDialogueNode.h"
#include "InstantDialogueEdge.h"
#include "GameplayTagContainer.h"
#include "InstantDialogueGraph.generated.h"

//DECLARE_MULTICAST_DELEGATE_OneParam(FWorldEvent, UWorld*);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDialogueStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDialogueEnded);


UCLASS(Blueprintable)
class INSTANTDIALOGUERUNTIME_API UInstantDialogue : public UObject
{
	GENERATED_BODY()

public:
 	UInstantDialogue();
 	virtual ~UInstantDialogue() override;

 	UPROPERTY(EditDefaultsOnly, Category = "xNotVisible")
 	FString Name;

 	UPROPERTY(EditDefaultsOnly, Category = "InstantDialogue")
 	TSubclassOf<UInstantDialogueNode> NodeType;


	//////////////////////////
	UPROPERTY(BlueprintAssignable) // BlueprintImplementableEvent, BlueprintNativeEvent ?
	FDialogueStarted OnDialogueStarted;

	UPROPERTY(BlueprintAssignable)
	FDialogueEnded OnDialogueEnded;

	UFUNCTION(BlueprintCallable)
	void DialogueStarted();
	
	UFUNCTION(BlueprintCallable)
	void DialogueEnded();
	//////////////////////////
	
	
 	UPROPERTY(EditDefaultsOnly, Category = "xDepricated")
 	TSubclassOf<UInstantDialogueEdge> EdgeType;

 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "xNotVisible")
 	FGameplayTagContainer GraphTags;

 	UPROPERTY(BlueprintReadOnly, Category = "xNotVisible")
 	TArray<UInstantDialogueNode*> RootNodes; //Get assigned in EdGraph_InstantDialogue->RebuildGraph,  all nodes with no input occupied

 	UPROPERTY(BlueprintReadOnly, Category = "xNotVisible")
 	TArray<UInstantDialogueNode*> AllNodes;

	/////////////////////////////////////////////
	virtual void PostInitProperties() override;
	
	virtual void OnPostWorldCreation(UWorld* CurWorld);
	////////////////////////////////////////////

 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "xDepricated")
 	bool bEdgeEnabled;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "InstantDialogue")
	bool ResetDialogue = true;

 	UPROPERTY(BlueprintReadOnly, Category = "InstantDialogue")
 	UInstantDialogueNode* CurrentNode = nullptr;

 	UFUNCTION(BlueprintCallable, Category = "InstantDialogue")
 	void Print(bool ToConsole = true, bool ToScreen = true);

 	UFUNCTION(BlueprintCallable, Category = "InstantDialogue")
 	int GetLevelNum() const;

 	UFUNCTION(BlueprintCallable, Category = "InstantDialogue")
 	void GetNodesByLevel(int Level, TArray<UInstantDialogueNode*>& Nodes);

 	UFUNCTION(BlueprintCallable, Category = "InstantDialogue")
 	FNodeData GetNodeData(int32 NextNodeIndex = 0);

 	void ClearGraph();

 //#if WITH_EDITORONLY_DATA
 	UPROPERTY()
 	class UEdGraph* EdGraph;

 	UPROPERTY(EditDefaultsOnly, Category = "InstantDialogue_Editor")
 	bool bCanRenameNode;
 //#endif
};

