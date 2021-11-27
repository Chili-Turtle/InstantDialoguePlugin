// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/InstantDialogueNodeAnswer.h"

#include "InstantDialogueGraph.h"

#define LOCTEXT_NAMESPACE "InstantDialogueNodeAnswer"


UInstantDialogueNodeAnwser::UInstantDialogueNodeAnwser()
{
	NodeType = EDialogueNodeType::Answer;
//#if WITH_EDITORONLY_DATA
	CompatibleGraphType = UInstantDialogue::StaticClass();

	ContextMenuName = LOCTEXT("ConextMenuNameAnswer", "Dialogue Anwser Node");
//#endif
}

FText* UInstantDialogueNodeAnwser::GetNameTextElement()
{
	return &NameText.Top();
}

FText UInstantDialogueNodeAnwser::GetDescription_Implementation() const
{
	return LOCTEXT("NodeDesc", "Instant Dialogue Answer Node");
}

#undef LOCTEXT_NAMESPACE