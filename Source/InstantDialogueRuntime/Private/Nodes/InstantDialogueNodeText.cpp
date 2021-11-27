#include "Nodes/InstantDialogueNodeText.h"

#include "InstantDialogueGraph.h"

#define LOCTEXT_NAMESPACE "InstantDialogueNodeAnwser"

UInstantDialogueNodeText::UInstantDialogueNodeText()
{
	NodeType = EDialogueNodeType::Text;
//#if WITH_EDITORONLY_DATA
	CompatibleGraphType = UInstantDialogue::StaticClass();

	ContextMenuName = LOCTEXT("ConextMenuNameText", "Text Node");
//#endif
}

FText UInstantDialogueNodeText::GetDescription_Implementation() const
{
	return LOCTEXT("NodeDesc", "Instant Dialogue Text Node");
}

FText UInstantDialogueNodeText::GetSpeakerName() const
{
	return NodeSpeakerName.IsEmpty() ? LOCTEXT("empty", "Empty") : NodeSpeakerName;
}

void UInstantDialogueNodeText::SetSpeakerName(const FText& NewTitle)
{
	NodeSpeakerName = NewTitle;	
}

UTexture* UInstantDialogueNodeText::GetPortrait() const
{
	return Portrait;
}

void UInstantDialogueNodeText::SetPortrait(UTexture* const NewPortrait)
{
	Portrait = NewPortrait;
}

void UInstantDialogueNodeText::SetDialogue(const FText& NewTitle)
{
	NodeDialogueText = NewTitle;
}



void UInstantDialogueNodeText::RemoveUserInput()
{
	Modify();
	InputPins.RemoveAt(InputPins.Num() - 1);
}

void UInstantDialogueNodeText::RemoveUserOutput()
{
	Modify();
	OutputPins.RemoveAt(OutputPins.Num() - 1);
}

#undef LOCTEXT_NAMESPACE
