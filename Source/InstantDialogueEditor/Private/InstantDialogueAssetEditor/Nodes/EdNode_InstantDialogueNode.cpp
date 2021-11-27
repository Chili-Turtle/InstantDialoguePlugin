#include "EdNode_InstantDialogueNode.h"
#include "InstantDialogueAssetEditor/EdGraph_InstantDialogue.h"
#include "Kismet2/Kismet2NameValidators.h"
#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "EdNode_InstantDialogue"

UEdNode_InstantDialogueNode::UEdNode_InstantDialogueNode()
{
	bCanRenameNode = true;
}

UEdNode_InstantDialogueNode::~UEdNode_InstantDialogueNode()
{

}
void UEdNode_InstantDialogueNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "MultipleNodes", FName(), TEXT("In EdNode"));
	CreatePin(EGPD_Output, "MultipleNodes", FName(), TEXT("Out EdNode"));
}

UEdGraph_InstantDialogue* UEdNode_InstantDialogueNode::GetInstantDialogueEdGraph()
{
	return Cast<UEdGraph_InstantDialogue>(GetGraph());
}

FText UEdNode_InstantDialogueNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (InstantDialogueNode == nullptr)
	{
		return Super::GetNodeTitle(TitleType);
	}
	else
	{
		return InstantDialogueNode->GetNodeTitle();
	}
}

void UEdNode_InstantDialogueNode::PrepareForCopying()
{
	InstantDialogueNode->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
}

void UEdNode_InstantDialogueNode::AutowireNewNode(UEdGraphPin* FromPin)
{
	Super::AutowireNewNode(FromPin);

	if (FromPin != nullptr)
	{
		if (GetSchema()->TryCreateConnection(FromPin, GetInputPin()))
		{
			FromPin->GetOwningNode()->NodeConnectionListChanged();
		}
	}
}

void UEdNode_InstantDialogueNode::SetInstantDialogueNode(UInstantDialogueNode* InNode)
{
	InstantDialogueNode = InNode;
}

FLinearColor UEdNode_InstantDialogueNode::GetBackgroundColor() const
{
	return InstantDialogueNode == nullptr ? FLinearColor::Black : InstantDialogueNode->GetBackgroundColor();
}

UEdGraphPin* UEdNode_InstantDialogueNode::GetInputPin() const
{
	return Pins[0];
}

UEdGraphPin* UEdNode_InstantDialogueNode::GetOutputPin() const
{
	return Pins[1];
}

void UEdNode_InstantDialogueNode::AddPinToInstantDialogueNode(const int32 Index)
{
	const FScopedTransaction Transaction(LOCTEXT("AddInstancePin", "Add Instance Pin")); //new
	Modify(); //new
	GetGraph()->NotifyGraphChanged(); //new
	UE_LOG(LogTemp, Warning, TEXT("AddPinToInstantDialogueNode, Index == %d"), Index);
	const FEdGraphPinType PinType = FEdGraphPinType(TEXT("exec")/*UEdGraphSchema_K2::PC_Exec*/, FName(NAME_None), nullptr, EPinContainerType::None, false, FEdGraphTerminalType());
	UEdGraphPin* NewPin = CreatePin(EGPD_Output, PinType, TEXT("NewOutputPin")/*FlowPin.PinName*/); //, Index
	check(NewPin);
	//SEdNode
	//InstantDialogueNode
	//OutputPins.Emplace(NewPin);
}

void UEdNode_InstantDialogueNode::RemovePinFromInstantDialogueNode(UEdGraphPin* TargetPin)
{
	UE_LOG(LogTemp, Warning, TEXT("EdNode Remove Pins was called"));
	RemovePin(TargetPin);
	//Do I have to remove the Outputs from the SedNode?
	TargetPin->MarkPendingKill();
	//Pins.Find(TargetPin); // gives the index of the pin
	Pins.Remove(TargetPin);
}

void UEdNode_InstantDialogueNode::PostEditUndo()
{
	UEdGraphNode::PostEditUndo();
}

#undef LOCTEXT_NAMESPACE
