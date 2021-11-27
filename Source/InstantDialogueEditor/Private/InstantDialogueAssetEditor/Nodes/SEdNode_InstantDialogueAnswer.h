// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//#include "EdNode_Anwser.h"
#include "EdNode_InstantDialogueAnswer.h"
#include "SGraphNode.h"
#include "SEdNode_InstantDialogueNode.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"

/**
 * 
 */
class UEdNode_InstantDialogueNode;
//class UEdNode_Anwser;

class SedNode_Anwser : public SEdNode_InstantDialogueNode//SEdNode_InstantDialogueNode
{
public:
	//SedNode_Anwser();
	//~SedNode_Anwser();
	
	SLATE_BEGIN_ARGS(SedNode_Anwser) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdNode_InstantDialogueNode* InNode);

	virtual void UpdateGraphNode() override;
	//virtual void CreatePinWidgets() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	//virtual bool IsNameReadOnly() const override;

	virtual FSlateColor GetBorderBackgroundColor() const override;
	virtual FSlateColor GetBackgroundColor() const override;

	virtual EVisibility GetDragOverMarkerVisibility() const override;

	virtual const FSlateBrush* GetNameIcon() const override;
	
	// Create the inner node content area, including the left/right pin boxes
	virtual TSharedRef<SWidget> CreateNodeContentArea() override;
	// @param OwnerPanel  The GraphPanel that this node belongs to
	virtual void SetOwner( const TSharedRef<SGraphPanel>& OwnerPanel ) override;
	// Create a single pin widget
	virtual void CreateStandardPinWidget(UEdGraphPin* Pin) override;

	//virtual TSharedPtr<SGraphPin> CreatePinWidget(UEdGraphPin* Pin) const override;
	TArray<TSharedPtr<SMultiLineEditableTextBox>> OutputTextBoxes;
	//TSharedPtr<SMultiLineEditableTextBox> OutputTextBox = SNew(SMultiLineEditableTextBox); 

//protected:
	void OnAnswerTextCommitted(const FText& InText, ETextCommit::Type CommitInfo,  int myInt);
	/*TSharedPtr<SBorder> NodeBody;
	void OnSpeakerNameCommitted(const FText& InText, ETextCommit::Type CommitInfo);
	void OnPortraitCommitted(UTexture* PortraitTexture);
	void OnDialogueCommitted(const FText& InText, ETextCommit::Type CommitInfo);*/
	//TSharedPtr<SHorizontalBox> OutputPinBox;
};
