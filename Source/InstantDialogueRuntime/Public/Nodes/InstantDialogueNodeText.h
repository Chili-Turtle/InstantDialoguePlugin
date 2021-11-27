#pragma once

#include "CoreMinimal.h"

#include "InstantDialogueEdge.h"

#include "InstantDialogueNodeText.generated.h"

UCLASS(Blueprintable)
class INSTANTDIALOGUERUNTIME_API UInstantDialogueNodeText : public UInstantDialogueNode
{
	GENERATED_BODY()
public:
	UInstantDialogueNodeText();

//////////////////////////////////////////////////////////////////////////
//#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InstantDialogueNode_Editor")
	FText NodeSpeakerName;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "InstantDialogueNode_Editor")
	FText NodeDialogueText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InstantDialogueNode_Editor")
	UTexture* Portrait;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "InstantDialogueNode_Editor")
	FSlateBrush MySlateBrush;

	virtual FText GetDescription_Implementation() const override;
//#endif
//////////////////////////////////////////////////////////////////////////
//#if WITH_EDITOR
	virtual FText GetSpeakerName() const;
	virtual void SetSpeakerName(const FText& NewTitle);
	UFUNCTION(BlueprintCallable, Category = "InstantDialogueNode")
	virtual UTexture* GetPortrait() const;
	virtual void SetPortrait(UTexture* const NewPortrait);
	virtual void SetDialogue(const FText& NewTitle);

	//EDialogueNodeType NodeType = EDialogueNodeType::Text;
	
/////////////////////////////////////////
	TArray<FName> InputPins;
	TArray<FName> OutputPins;
	
	void RemoveUserInput();
	void RemoveUserOutput();
////////////////////////////////////////
	
//#endif
};