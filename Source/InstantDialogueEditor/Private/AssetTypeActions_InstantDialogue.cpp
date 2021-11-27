#include "AssetTypeActions_InstantDialogue.h"

#include "ConditionObject.h"
#include "InstantDialogueEditorPCH.h"
#include "InstantDialogueAssetEditor/AssetEditor_InstantDialogue.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions_InstantDialogue"

FAssetTypeActions_InstantDialogue::FAssetTypeActions_InstantDialogue(EAssetTypeCategories::Type InAssetCategory)
	: MyAssetCategory(InAssetCategory)
{
}

FText FAssetTypeActions_InstantDialogue::GetName() const
{
	return LOCTEXT("FInstantDialogueAssetTypeActionsName", "Instant Dialogue");
}

FColor FAssetTypeActions_InstantDialogue::GetTypeColor() const
{
	return FColor(129, 196, 115);
}

UClass* FAssetTypeActions_InstantDialogue::GetSupportedClass() const
{
	return UInstantDialogue::StaticClass();
}

void FAssetTypeActions_InstantDialogue::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		if (UInstantDialogue* Graph = Cast<UInstantDialogue>(*ObjIt))
		{
			TSharedRef<FAssetEditor_InstantDialogue> NewGraphEditor(new FAssetEditor_InstantDialogue());
			NewGraphEditor->InitInstantDialogueAssetEditor(Mode, EditWithinLevelEditor, Graph);
		}
	}
}

uint32 FAssetTypeActions_InstantDialogue::GetCategories()
{
	return MyAssetCategory;
	//return EAssetTypeCategories::Animation | MyAssetCategory;
}

//////////////////////////////////////////////////////////////////////////

FAssetTypeActions_InstantCondition::FAssetTypeActions_InstantCondition(EAssetTypeCategories::Type InAssetCategory)
: MyAssetCategory(InAssetCategory)
{
}

FText FAssetTypeActions_InstantCondition::GetName() const
{
	return LOCTEXT("FInstantDialogueAssetTypeActionsName", "Instant Condition");
}

FColor FAssetTypeActions_InstantCondition::GetTypeColor() const
{
	return FColor(255, 85, 115);
}

UClass* FAssetTypeActions_InstantCondition::GetSupportedClass() const
{
	return UInstantDialogueConditionObject::StaticClass();
}

uint32 FAssetTypeActions_InstantCondition::GetCategories()
{
	//return MyAssetCategory;
	return EAssetTypeCategories::Animation | MyAssetCategory;
}

void FAssetTypeActions_InstantCondition::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor)
{
		FSimpleAssetEditor::CreateEditor(EToolkitMode::Standalone, EditWithinLevelEditor, InObjects);
}

#undef LOCTEXT_NAMESPACE