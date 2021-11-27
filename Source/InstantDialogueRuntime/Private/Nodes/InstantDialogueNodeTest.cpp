// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/InstantDialogueNodeTest.h"

#include "InstantDialogueGraph.h"

#define LOCTEXT_NAMESPACE "InstantDialogueNodeTest"

UInstantDialogueNodeTest::UInstantDialogueNodeTest()
{
	NodeType = EDialogueNodeType::Condition;
//#if WITH_EDITORONLY_DATA
	CompatibleGraphType = UInstantDialogue::StaticClass();
	
	ContextMenuName = LOCTEXT("ConextMenuNameText", "Text Node");
//#endif
}

int UInstantDialogueNodeTest::GetConditionPin()
{
	for (int i = 0; i < ConditionsArray.Num(); ++i)
	{
			if (ConditionsArray[i] != nullptr && ConditionsArray[i].GetDefaultObject()->IsConditionMet())
			{
				return i;
			}
	}

	return -1; // None or false

}

int UInstantDialogueNodeTest::GetConditionPinImplementation()
{
	for (int i = 0; i < ConditionsArray.Num(); ++i)
	{
		if (ConditionsArray[i].GetDefaultObject()->IsConditionMet_Implementation())
		{
			return i;
		}
	}

	return -1;
}

void UInstantDialogueNodeTest::PrintCondition()
{

	UE_LOG(LogTemp, Warning, TEXT("Condition met"));

	UInstantDialogueConditionObject* myCondition = ConditionsArray[0].GetDefaultObject();
	UE_LOG(LogTemp, Warning, TEXT("Condition met %d"), myCondition->IsConditionMet());
	
	//if (IsValid(ConditionsArray[0]))
	//{
		//bool myBool = Cast<UInstantDialogueConditionObject>(ConditionsArray[0])->IsConditionMet();
		//UE_LOG(LogTemp, Warning, TEXT("Condition met %d"), myBool);
	//}
	
	// for (int i = 0; i < ConditionsArray.Num(); ++i)
	// {
	// 	if (CastChecked<UInstantDialogueConditionObject>(ConditionsArray[i])->IsConditionMet() == true)
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("Is true"));
	// 	}
	// 	else
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("Is false"));
	// 	}
 // 		
	// }
}

FText UInstantDialogueNodeTest::GetDescription_Implementation() const
{
	return LOCTEXT("NodeDesc", "Instant Dialogue Condition Node");
}

#undef LOCTEXT_NAMESPACE
