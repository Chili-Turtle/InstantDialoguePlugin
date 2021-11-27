// Fill out your copyright notice in the Description page of Project Settings.


#include "EdNode_InstantDialogueCondition.h"
#include "Nodes/InstantDialogueNodeTest.h"

#define LOCTEXT_NAMESPACE "EdNode_Condition"

UEdNode_Condition::UEdNode_Condition()
{
	bCanRenameNode = true;
}

UEdNode_Condition::~UEdNode_Condition()
{
}

void UEdNode_Condition::AllocateDefaultPins()
{
	//UInstantDialogueConditionObject* newConditionObject = NewObject<UInstantDialogueConditionObject>();

	CreatePin(EGPD_Input, "MultipleNodes", FName(), TEXT("In EdNode"));
	CreatePin(EGPD_Output, "MultipleNodes", FName(), TEXT("Out EdNode"));
	//CreatePin(EGPD_Output, "MultipleNodes", NAME_None, newConditionObject, TEXT("Out EdNode"));
	//CreatePin(EGPD_Output, "MultipleNodes", NAME_None, UObject::StaticClass(), TEXT("Out EdNode"));
	//CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Byte, UObject::StaticClass(), TEXT("Out EdNode")); // 1 non listed extern

	UInstantDialogueNodeTest* MyAnswerNode = Cast<UInstantDialogueNodeTest>(InstantDialogueNode);
	MyAnswerNode->ConditionsArray.Emplace();
}

void UEdNode_Condition::AddPinToInstantDialogueNode(const int32 Index)
{
	const FScopedTransaction Transaction(LOCTEXT("AddInstancePin", "Add Instance Pin")); //new
	Modify();
	GetGraph()->NotifyGraphChanged();

	//UInstantDialogueConditionObject* newConditionObject = NewObject<UInstantDialogueConditionObject>();

	
	const FEdGraphPinType PinType = FEdGraphPinType(TEXT("exec")/*UEdGraphSchema_K2::PC_Exec*/, FName(NAME_None), nullptr, EPinContainerType::None, false, FEdGraphTerminalType());
	//const FEdGraphPinType PinType = FEdGraphPinType(TEXT("exec")/*UEdGraphSchema_K2::PC_Exec*/, FName(NAME_None), newConditionObject, EPinContainerType::None, false, FEdGraphTerminalType());
	
	UEdGraphPin* NewPin = CreatePin(EGPD_Output, PinType, TEXT("NewPin") /*FlowPin.PinName*/, Index); //, Index TEXT("NewOutputPin")
	//UEdGraphPin* NewPin2 = CreatePin(EGPD_Output, "MultipleNodes", NAME_None, UObject::StaticClass(), TEXT("Out EdNode"));

	///////// Just make an F_String PC_Byte 
	
	//UEdGraphPin* NewPin = CreatePin(EGPD_Output, "MultipleNodes", NAME_None, nullptr, TEXT("Out EdNode"));

	UInstantDialogueNodeTest* MyAnswerNode = Cast<UInstantDialogueNodeTest>(InstantDialogueNode);
	MyAnswerNode->ConditionsArray.Emplace();
	
	check(NewPin);

}

void UEdNode_Condition::RemovePinFromInstantDialogueNode(UEdGraphPin* TargetPin)
{
	UInstantDialogueNodeTest* MyAnswerNode = Cast<UInstantDialogueNodeTest>(InstantDialogueNode);
	MyAnswerNode->ConditionsArray.RemoveAt(Pins.Find(TargetPin)-1);

	RemovePin(TargetPin);
	TargetPin->MarkPendingKill();
	Pins.Remove(TargetPin);
}

#undef LOCTEXT_NAMESPACE