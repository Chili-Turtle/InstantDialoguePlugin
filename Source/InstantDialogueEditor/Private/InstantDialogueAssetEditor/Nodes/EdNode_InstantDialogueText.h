#pragma once

#include "CoreMinimal.h"

#include "Nodes/InstantDialogueNodeText.h"
#include "SEdNode_InstantDialogueText.h"
#include "EdNode_InstantDialogueText.generated.h"

class SEdNode_Text;
UCLASS(MinimalAPI)
class UEdNode_Text : public UEdNode_InstantDialogueNode 
{
	GENERATED_BODY()
public:
	SEdNode_Text* SEdNode;

	UPROPERTY(Instanced)
	UInstantDialogueNodeText* InstantDialogueNodeText;
	
	UEdNode_Text();
	virtual ~UEdNode_Text();
	
	void SetInstantDialogueNode(UInstantDialogueNodeText* InNode);
};
