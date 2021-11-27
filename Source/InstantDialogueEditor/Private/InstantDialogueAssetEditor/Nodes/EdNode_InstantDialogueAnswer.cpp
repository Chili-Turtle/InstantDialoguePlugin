// Fill out your copyright notice in the Description page of Project Settings.

#include "EdNode_InstantDialogueAnswer.h"

#include <string>

#include "InstantDialogueAssetEditor/EdGraph_InstantDialogue.h"

#define LOCTEXT_NAMESPACE "EdNode_InstantDialogue"

UEdNode_Anwser::UEdNode_Anwser()
{
	bCanRenameNode = true;
}

UEdNode_Anwser::~UEdNode_Anwser()
{
}

void UEdNode_Anwser::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "MultipleNodes", FName(), TEXT("In EdNode"));
	CreatePin(EGPD_Output, "MultipleNodes", FName(), TEXT("Out EdNode"));
	
	UInstantDialogueNodeAnwser* MyAnswerNode = Cast<UInstantDialogueNodeAnwser>(InstantDialogueNode);
	MyAnswerNode->NameText.Emplace(LOCTEXT("NameTextKey","Enter Name text here"));
}

void UEdNode_Anwser::AddPinToInstantDialogueNode(const int32 Index)
{
	const FScopedTransaction Transaction(LOCTEXT("AddInstancePin", "Add Instance Pin")); //new
	Modify(); //new
	GetGraph()->NotifyGraphChanged(); //new

	UE_LOG(LogTemp, Warning, TEXT("AddPinToInstantDialogueNode, Index == %d"), Index);

	
	
	/*TArray<FStringFormatArg> args;
	args.Add(FStringFormatArg(Index));
	FString myString = FString("this is {0}");
	FString myStringFormated = myString.Format(TEXT("my {0}"), args);*/

	const FEdGraphPinType PinType = FEdGraphPinType(TEXT("exec")/*UEdGraphSchema_K2::PC_Exec*/, FName(NAME_None), nullptr, EPinContainerType::None, false, FEdGraphTerminalType());
	UEdGraphPin* NewPin = CreatePin(EGPD_Output, PinType, TEXT("NewPin") /*FlowPin.PinName*/, Index); //, Index TEXT("NewOutputPin")
	check(NewPin);

	UInstantDialogueNodeAnwser* MyAnswerNode = Cast<UInstantDialogueNodeAnwser>(InstantDialogueNode);
	MyAnswerNode->NameText.Emplace(LOCTEXT("NameTextKey","Enter Name text here"));
	
	//OutputPins.Emplace(NewPin);
}

void UEdNode_Anwser::RemovePinFromInstantDialogueNode(UEdGraphPin* TargetPin)
{

	UInstantDialogueNodeAnwser* MyAnswerNode = Cast<UInstantDialogueNodeAnwser>(InstantDialogueNode);
	UE_LOG(LogTemp, Warning, TEXT("Removeing Index From Array %d"), Pins.Find(TargetPin));
	/*if (MyAnswerNode->AnswerText.IsValidIndex(Pins.Find(TargetPin)))
		{
			UE_LOG(LogTemp, Warning, TEXT("valid index"));
		}*/
//	MyAnswerNode->AnswerText.RemoveAt(Pins.Find(TargetPin)-1); // -1 because of input pin
	MyAnswerNode->NameText.RemoveAt(Pins.Find(TargetPin)-1); // -1 because of input pin
	RemovePin(TargetPin);
	TargetPin->MarkPendingKill();
	Pins.Remove(TargetPin);
	
}

/*void UEdNode_Anwser::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "MultipleNodes", FName(), TEXT("In EdNode"));
	CreatePin(EGPD_Output, "MultipleNodes", FName(), TEXT("Out EdNode"));
}

UEdGraph_InstantDialogue* UEdNode_Anwser::GetInstantDialogueEdGraph()
{
	return Cast<UEdGraph_InstantDialogue>(GetGraph());
}

FText UEdNode_Anwser::GetNodeTitle(ENodeTitleType::Type TitleType) const
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

void UEdNode_Anwser::PrepareForCopying()
{
	InstantDialogueNode->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
}

void UEdNode_Anwser::AutowireNewNode(UEdGraphPin* FromPin)
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

void UEdNode_Anwser::SetInstantDialogueNode(UInstantDialogueNodeAnwser* InNode)
{
	InstantDialogueNode = InNode;
}

FLinearColor UEdNode_Anwser::GetBackgroundColor() const
{
	return InstantDialogueNode == nullptr ? FLinearColor::Black : InstantDialogueNode->GetBackgroundColor();
}

UEdGraphPin* UEdNode_Anwser::GetInputPin() const
{
	return Pins[0];
}

UEdGraphPin* UEdNode_Anwser::GetOutputPin() const
{
	return Pins[1];
}

void UEdNode_Anwser::PostEditUndo()
{
	UEdGraphNode::PostEditUndo();
}*/

#undef LOCTEXT_NAMESPACE
