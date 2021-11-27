#include "AssetEditorToolbar_InstantDialogue.h"
#include "AssetEditor_InstantDialogue.h"
#include "EditorCommands_InstantDialogue.h"
#include "InstantDialogueEditorStyle.h"

#define LOCTEXT_NAMESPACE "AssetEditorToolbar_InstantDialogue"

void FAssetEditorToolbar_InstantDialogue::AddInstantDialogueToolbar(TSharedPtr<FExtender> Extender)
{
	check(InstantDialogueEditor.IsValid());
	TSharedPtr<FAssetEditor_InstantDialogue> InstantDialoguePtr = InstantDialogueEditor.Pin();

	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension("Asset", EExtensionHook::After, InstantDialoguePtr->GetToolkitCommands(), FToolBarExtensionDelegate::CreateSP( this, &FAssetEditorToolbar_InstantDialogue::FillInstantDialogueToolbar ));
	InstantDialoguePtr->AddToolbarExtender(ToolbarExtender);
}

void FAssetEditorToolbar_InstantDialogue::FillInstantDialogueToolbar(FToolBarBuilder& ToolbarBuilder)
{
	check(InstantDialogueEditor.IsValid());
	TSharedPtr<FAssetEditor_InstantDialogue> InstantDialogueEditorPtr = InstantDialogueEditor.Pin();

	ToolbarBuilder.BeginSection("Instant Dialogue");
	{
		ToolbarBuilder.AddToolBarButton(FEditorCommands_InstantDialogue::Get().GraphSettings,
			NAME_None,
			LOCTEXT("GraphSettings_Label", "Graph Settings"),
			LOCTEXT("GraphSettings_ToolTip", "Show the Graph Settings"),
			FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.GameSettings"));
	}
	ToolbarBuilder.EndSection();

	ToolbarBuilder.BeginSection("Util");
	{
		ToolbarBuilder.AddToolBarButton(FEditorCommands_InstantDialogue::Get().AutoArrange,
			NAME_None,
			LOCTEXT("AutoArrange_Label", "Auto Arrange"),
			LOCTEXT("AutoArrange_ToolTip", "Auto arrange nodes, not perfect, but still handy"),
			FSlateIcon(FInstantDialogueEditorStyle::GetStyleSetName(), "InstantDialogueEditor.AutoArrange"));
	}
	ToolbarBuilder.EndSection();

}


#undef LOCTEXT_NAMESPACE
