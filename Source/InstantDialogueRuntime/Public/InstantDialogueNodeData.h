// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"

#include "InstantDialogueNodeData.generated.h"
// #include "InstantDialogueNode.h"
// #include "UObject/NoExportTypes.h"
// #include "InstantDialogueNode_Condition.generated.h"

UENUM(BlueprintType)  
enum class EDialogueNodeType : uint8  
{  
	Default,
	Answer,
	Text, 
	Condition, 
};

USTRUCT(BlueprintType) //BlueprintType  
struct FNodeData  
{  
	GENERATED_BODY()  
  
	//FNodeData(/*TArray<FText> DialogueTextData, FText SpeakerName, UTexture* Portrait*/);  
	//     :DialogueTextData(DialogueTextData)  
	//     , SpeakerName(SpeakerName) //     , Portrait(Portrait) // { // }  
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InstantDialogueNode_Editor")
	TArray<FText> DialogueTextData;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InstantDialogueNode_Editor")  
	FText SpeakerName = FText::FromString("NONE");
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InstantDialogueNode_Editor")  
	UTexture* Portrait = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InstantDialogueNode_Editor")
	EDialogueNodeType NodeType = EDialogueNodeType::Default;
};

// UCLASS()
// class InstantDialogueRUNTIME_API UInstantDialogueNode_Condtion : public UInstantDialogueNode
// {
// 	GENERATED_BODY()
// public:
//  	UInstantDialogueNode_Condtion();
//
// 	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InstantDialogueNode_Editor") 
// 	TArray<TSubclassOf<UInstantDialogueConditionObject>> ConditionsArray;
//      
// 	UFUNCTION(BlueprintCallable, Category = "InstantDialogueNode")
// 	virtual void PrintCondition() override;
//  	
//  };











//UCLASS(Blueprintable)
//class InstantDialogueRUNTIME_API UInstantDialogueNode_Condition : public UInstantDialogueNode
//{
//	GENERATED_BODY()

//public:
//	UInstantDialogueNode_Condition();
	
	 //UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "InstantDialogueNode_Editor") //BlueprintCallable
     //TArray<TSubclassOf<UInstantDialogueConditionObject>> ConditionsArray; //works no icon
         
     //UFUNCTION(BlueprintCallable, Category = "InstantDialogueNode")
     //void PrintCondition();
//};