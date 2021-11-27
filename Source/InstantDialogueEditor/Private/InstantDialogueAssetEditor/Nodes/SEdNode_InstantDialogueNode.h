#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"

class UEdNode_InstantDialogueNode;

class SEdNode_InstantDialogueNode : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SEdNode_InstantDialogueNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdNode_InstantDialogueNode* InNode);

	virtual void UpdateGraphNode() override;
	virtual void CreatePinWidgets() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	//virtual bool IsNameReadOnly() const override;

	virtual  void CreateOutputSideAddButton(TSharedPtr<SVerticalBox> OutputBox) override;
	virtual FReply OnAddPin() override;

	virtual FSlateColor GetBorderBackgroundColor() const;
	virtual FSlateColor GetBackgroundColor() const;

	virtual EVisibility GetDragOverMarkerVisibility() const;

	virtual const FSlateBrush* GetNameIcon() const;
	
	/** Create the inner node content area, including the left/right pin boxes */
	virtual TSharedRef<SWidget> CreateNodeContentArea();
	/** @param OwnerPanel  The GraphPanel that this node belongs to */
	virtual void SetOwner( const TSharedRef<SGraphPanel>& OwnerPanel );
	/** Create a single pin widget */
	virtual void CreateStandardPinWidget(UEdGraphPin* Pin);
	virtual TSharedPtr<SGraphPin> CreatePinWidget(UEdGraphPin* Pin) const;


//protected:
	TSharedPtr<SBorder> NodeBody;
	//void OnDialogueCommitted(const FText& InText, ETextCommit::Type CommitInfo);
	TSharedPtr<SHorizontalBox> OutputPinBox;
};
