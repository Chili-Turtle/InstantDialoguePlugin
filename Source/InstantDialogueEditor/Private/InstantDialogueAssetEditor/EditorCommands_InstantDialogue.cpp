#include "EditorCommands_InstantDialogue.h"

#define LOCTEXT_NAMESPACE "EditorCommands_InstantDialogue"

void FEditorCommands_InstantDialogue::RegisterCommands()
{
	UI_COMMAND(GraphSettings, "Graph Settings", "Graph Settings", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(AutoArrange, "Auto Arrange", "Auto Arrange", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
