#pragma once

#include "CoreMinimal.h"

class FEditorCommands_InstantDialogue : public TCommands<FEditorCommands_InstantDialogue>
{
public:
	/** Constructor */
	FEditorCommands_InstantDialogue()
		: TCommands<FEditorCommands_InstantDialogue>("InstantDialogueEditor", NSLOCTEXT("Contexts", "InstantDialogueEditor", "Instant Dialogue Editor"), NAME_None, FEditorStyle::GetStyleSetName())
	{
	}
	
	TSharedPtr<FUICommandInfo> GraphSettings;
	TSharedPtr<FUICommandInfo> AutoArrange;

	virtual void RegisterCommands() override;
};
