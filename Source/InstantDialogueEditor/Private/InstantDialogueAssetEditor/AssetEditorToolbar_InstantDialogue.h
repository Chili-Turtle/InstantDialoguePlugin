
#pragma once

#include "CoreMinimal.h"

class FAssetEditor_InstantDialogue;
class FExtender;
class FToolBarBuilder;

class FAssetEditorToolbar_InstantDialogue : public TSharedFromThis<FAssetEditorToolbar_InstantDialogue>
{
public:
	FAssetEditorToolbar_InstantDialogue(TSharedPtr<FAssetEditor_InstantDialogue> InInstantDialogueEditor)
		: InstantDialogueEditor(InInstantDialogueEditor) {}

	void AddInstantDialogueToolbar(TSharedPtr<FExtender> Extender);

private:
	void FillInstantDialogueToolbar(FToolBarBuilder& ToolbarBuilder);

protected:
	/** Pointer back to the blueprint editor tool that owns us */
	TWeakPtr<FAssetEditor_InstantDialogue> InstantDialogueEditor;
};
