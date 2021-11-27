#include "AssetTypeActions_InstantDialogue.h"
#include "InstantDialogueEditorPCH.h"
#include "EdGraphUtilities.h"
#include "InstantDialogueAssetEditor/Nodes/EdNode_InstantDialogueNode.h"
#include "InstantDialogueAssetEditor/EdNode_InstantDialogueEdge.h"
#include "InstantDialogueAssetEditor/Nodes/SEdNode_InstantDialogueNode.h"
#include "InstantDialogueAssetEditor/SEdNode_InstantDialogueEdge.h"
#include "InstantDialogueAssetEditor/InstantDialogueEditorStyle.h"
#include "InstantDialogueAssetEditor/Nodes/EdNode_InstantDialogueAnswer.h"
#include "InstantDialogueAssetEditor/Nodes/EdNode_InstantDialogueCondition.h"
#include "InstantDialogueAssetEditor/Nodes/EdNode_InstantDialogueText.h"
#include "InstantDialogueAssetEditor/Nodes/SedNode_InstantDialogueCondition.h"

DEFINE_LOG_CATEGORY(InstantDialogueEditor)

#define LOCTEXT_NAMESPACE "Editor_InstantDialogue"

class FGraphPanelNodeFactory_InstantDialogue : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<class SGraphNode> CreateNode(UEdGraphNode* Node) const override
	{
		if (UEdNode_InstantDialogueNode* EdNode_GraphNode = Cast<UEdNode_InstantDialogueNode>(Node))
		{
			return SNew(SEdNode_InstantDialogueNode, EdNode_GraphNode);
		}
		else if (UEdNode_InstantDialogueEdge* EdNode_Edge = Cast<UEdNode_InstantDialogueEdge>(Node))
		{
			return SNew(SEdNode_InstantDialogueEdge, EdNode_Edge);
		}
		return nullptr;
	}
};

class FGraphPanelNodeFactory_InstantDialogueAnwser : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<class SGraphNode> CreateNode(UEdGraphNode* Node) const override
	{
		if (UEdNode_Anwser* EdNode_GraphNodeAnwser = Cast<UEdNode_Anwser>(Node))
		{
			return SNew(SedNode_Anwser, EdNode_GraphNodeAnwser);
		}
		else if (UEdNode_InstantDialogueEdge* EdNode_Edge = Cast<UEdNode_InstantDialogueEdge>(Node))
		{
			return SNew(SEdNode_InstantDialogueEdge, EdNode_Edge);
		}
		return nullptr;
	}
};

class FGraphPanelNodeFactory_InstantDialogueText : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<class SGraphNode> CreateNode(UEdGraphNode* Node) const override
	{
		if (UEdNode_Text* EdNode_GraphNodeAnwser = Cast<UEdNode_Text>(Node))
		{
			return SNew(SEdNode_Text, EdNode_GraphNodeAnwser);
		}
		else if (UEdNode_InstantDialogueEdge* EdNode_Edge = Cast<UEdNode_InstantDialogueEdge>(Node))
		{
			return SNew(SEdNode_InstantDialogueEdge, EdNode_Edge);
		}
		return nullptr;
	}
};

class FGraphPanelNodeFactory_InstantDialogueCondition : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<class SGraphNode> CreateNode(UEdGraphNode* Node) const override
	{
		if (UEdNode_Condition* EdNode_GraphNodeCondition = Cast<UEdNode_Condition>(Node))
		{
			return SNew(SedNode_Condition, EdNode_GraphNodeCondition);
		}
		else if (UEdNode_InstantDialogueEdge* EdNode_Edge = Cast<UEdNode_InstantDialogueEdge>(Node))
		{
			return SNew(SEdNode_InstantDialogueEdge, EdNode_Edge);
		}
		return nullptr;
	}
};

TSharedPtr<FGraphPanelNodeFactory> GraphPanelNodeFactory_InstantDialogue;
TSharedPtr<FGraphPanelNodeFactory_InstantDialogueAnwser> GraphPanelNodeFactory_InstantDialogueAnwser;
TSharedPtr<FGraphPanelNodeFactory_InstantDialogueText> GraphPanelNodeFactory_InstantDialogueText;
TSharedPtr<FGraphPanelNodeFactory_InstantDialogueCondition> GraphPanelNodeFactory_InstantDialogueCondition; 

class FInstantDialogueEditor : public IInstantDialogueEditor//IInstantDialogueEditor
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action);

private:
	TArray< TSharedPtr<IAssetTypeActions> > CreatedAssetTypeActions;

	EAssetTypeCategories::Type InstantDialogueAssetCategoryBit;
};

IMPLEMENT_MODULE( FInstantDialogueEditor, InstantDialogueEditor )

void FInstantDialogueEditor::StartupModule()
{
	FInstantDialogueEditorStyle::Initialize();

	GraphPanelNodeFactory_InstantDialogueCondition = MakeShareable(new FGraphPanelNodeFactory_InstantDialogueCondition());
	FEdGraphUtilities::RegisterVisualNodeFactory(GraphPanelNodeFactory_InstantDialogueCondition);
	
	GraphPanelNodeFactory_InstantDialogueText = MakeShareable(new FGraphPanelNodeFactory_InstantDialogueText());
	FEdGraphUtilities::RegisterVisualNodeFactory(GraphPanelNodeFactory_InstantDialogueText);

	GraphPanelNodeFactory_InstantDialogueAnwser = MakeShareable(new FGraphPanelNodeFactory_InstantDialogueAnwser());
	FEdGraphUtilities::RegisterVisualNodeFactory(GraphPanelNodeFactory_InstantDialogueAnwser);

	GraphPanelNodeFactory_InstantDialogue = MakeShareable(new FGraphPanelNodeFactory_InstantDialogue());
	FEdGraphUtilities::RegisterVisualNodeFactory(GraphPanelNodeFactory_InstantDialogue);

	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	InstantDialogueAssetCategoryBit = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("InstantDialogue")), LOCTEXT("InstantDialogueAssetCategory", "InstantDialogue"));

	RegisterAssetTypeAction(AssetTools, MakeShareable(new FAssetTypeActions_InstantDialogue(InstantDialogueAssetCategoryBit)));


	//add Uobject to rightClick inventory
	EAssetTypeCategories::Type IntaCondition = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("InstaCondition")), LOCTEXT("Instala", "InstaDialogue"));
	TSharedPtr<IAssetTypeActions> Action = MakeShareable(new FAssetTypeActions_InstantCondition(IntaCondition));
	AssetTools.RegisterAssetTypeActions(Action.ToSharedRef());
	CreatedAssetTypeActions.Add(Action);
}


void FInstantDialogueEditor::ShutdownModule()
{
	// Unregister all the asset types that we registered
	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (int32 Index = 0; Index < CreatedAssetTypeActions.Num(); ++Index)
		{
			AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[Index].ToSharedRef());
		}
	}

	if (GraphPanelNodeFactory_InstantDialogue.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualNodeFactory(GraphPanelNodeFactory_InstantDialogue);
		GraphPanelNodeFactory_InstantDialogue.Reset();
	}
	
	if (GraphPanelNodeFactory_InstantDialogueAnwser.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualNodeFactory(GraphPanelNodeFactory_InstantDialogueAnwser);
		GraphPanelNodeFactory_InstantDialogueAnwser.Reset();
	}

	FInstantDialogueEditorStyle::Shutdown();
}

void FInstantDialogueEditor::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	CreatedAssetTypeActions.Add(Action);
}

#undef LOCTEXT_NAMESPACE

