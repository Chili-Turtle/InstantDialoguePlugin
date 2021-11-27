#include "AssetEditor_InstantDialogue.h"
#include "InstantDialogueEditorPCH.h"
#include "AssetEditorToolbar_InstantDialogue.h"
#include "AssetGraphSchema_InstantDialogue.h"
#include "EditorCommands_InstantDialogue.h"
#include "EdGraph_InstantDialogue.h"
#include "AssetToolsModule.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Framework/Commands/GenericCommands.h"
#include "GraphEditorActions.h"
#include "IDetailsView.h"
#include "PropertyEditorModule.h"
#include "Editor/UnrealEd/Public/Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "EdGraphUtilities.h"
#include "EdGraph_InstantDialogue.h"
#include "Nodes/EdNode_InstantDialogueNode.h"
#include "EdNode_InstantDialogueEdge.h"
//#include "SEdNode_InstantDialogueNode.h"
#include "AutoLayout/TreeLayoutStrategy.h"
#include "AutoLayout/ForceDirectedLayoutStrategy.h"

#define LOCTEXT_NAMESPACE "AssetEditor_InstantDialogue"

const FName InstantDialogueEditorAppName = FName(TEXT("InstantDialogueEditorApp"));

struct FInstantDialogueAssetEditorTabs
{
	// Tab identifiers
	static const FName InstantDialoguePropertyID;
	static const FName ViewportID;
	static const FName InstantDialogueEditorSettingsID;
};

//////////////////////////////////////////////////////////////////////////

const FName FInstantDialogueAssetEditorTabs::InstantDialoguePropertyID(TEXT("InstantDialogueProperty"));
const FName FInstantDialogueAssetEditorTabs::ViewportID(TEXT("Viewport"));
const FName FInstantDialogueAssetEditorTabs::InstantDialogueEditorSettingsID(TEXT("InstantDialogueEditorSettings"));

//////////////////////////////////////////////////////////////////////////

FAssetEditor_InstantDialogue::FAssetEditor_InstantDialogue()
{
	EditingGraph = nullptr;

	GenricGraphEditorSettings = NewObject<UInstantDialogueEditorSettings>(UInstantDialogueEditorSettings::StaticClass());

	OnPackageSavedDelegateHandle = UPackage::PackageSavedEvent.AddRaw(this, &FAssetEditor_InstantDialogue::OnPackageSaved);
}

FAssetEditor_InstantDialogue::~FAssetEditor_InstantDialogue()
{
	UPackage::PackageSavedEvent.Remove(OnPackageSavedDelegateHandle);
}

void FAssetEditor_InstantDialogue::InitInstantDialogueAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr< IToolkitHost >& InitToolkitHost, UInstantDialogue* Graph)
{
	EditingGraph = Graph;
	CreateEdGraph();
	

	FGenericCommands::Register();
	FGraphEditorCommands::Register();
	FEditorCommands_InstantDialogue::Register();

	if (!ToolbarBuilder.IsValid())
	{
		ToolbarBuilder = MakeShareable(new FAssetEditorToolbar_InstantDialogue(SharedThis(this)));
	}

	//BindCommands();

	CreateInternalWidgets();

	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);

	// the extra toolbar widget (sort nodes and graph settings)
	ToolbarBuilder->AddInstantDialogueToolbar(ToolbarExtender);

	// Layout
	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_InstantDialogueEditor_Layout_v1")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.1f) // is this the viewport scale?
				->AddTab(GetToolbarTabId(), ETabState::OpenedTab)->SetHideTabWell(true)
			)
			->Split
			(
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)->SetSizeCoefficient(0.9f)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.65f)
					->AddTab(FInstantDialogueAssetEditorTabs::ViewportID, ETabState::OpenedTab)->SetHideTabWell(true)
				)
				->Split
				(
					FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.7f)
						->AddTab(FInstantDialogueAssetEditorTabs::InstantDialoguePropertyID, ETabState::OpenedTab)->SetHideTabWell(true)
					)
					->Split
					(
						FTabManager::NewStack()
						->SetSizeCoefficient(0.3f)
						->AddTab(FInstantDialogueAssetEditorTabs::InstantDialogueEditorSettingsID, ETabState::OpenedTab)
					)
				)
			)
		);

	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, InstantDialogueEditorAppName, StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, EditingGraph, false);

	RegenerateMenusAndToolbars();
}

void FAssetEditor_InstantDialogue::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_InstantDialogueEditor", "Instant Dialogue Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	// registers the graph viewport space (sDock), if I don't register it the window shows a "unreconginzed tab" text at the place where the tab should be
	InTabManager->RegisterTabSpawner(FInstantDialogueAssetEditorTabs::ViewportID, FOnSpawnTab::CreateSP(this, &FAssetEditor_InstantDialogue::SpawnTab_Viewport))
		.SetDisplayName(LOCTEXT("GraphCanvasTab", "Viewport"))
		.SetGroup(WorkspaceMenuCategoryRef);
		//.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "GraphEditor.EventGraph_16x"));

	InTabManager->RegisterTabSpawner(FInstantDialogueAssetEditorTabs::InstantDialoguePropertyID, FOnSpawnTab::CreateSP(this, &FAssetEditor_InstantDialogue::SpawnTab_Details))
		.SetDisplayName(LOCTEXT("DetailsTab", "Property"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));

	InTabManager->RegisterTabSpawner(FInstantDialogueAssetEditorTabs::InstantDialogueEditorSettingsID, FOnSpawnTab::CreateSP(this, &FAssetEditor_InstantDialogue::SpawnTab_EditorSettings))
		.SetDisplayName(LOCTEXT("EditorSettingsTab", "Instant Dialogue Editor Setttings"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));
}

void FAssetEditor_InstantDialogue::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(FInstantDialogueAssetEditorTabs::ViewportID);
	InTabManager->UnregisterTabSpawner(FInstantDialogueAssetEditorTabs::InstantDialoguePropertyID);
	InTabManager->UnregisterTabSpawner(FInstantDialogueAssetEditorTabs::InstantDialogueEditorSettingsID);
}

FName FAssetEditor_InstantDialogue::GetToolkitFName() const
{
	return FName("FInstantDialogueEditor");
}

FText FAssetEditor_InstantDialogue::GetBaseToolkitName() const
{
	return LOCTEXT("InstantDialogueEditorAppLabel", "Instant Dialogue Editor");
}

FText FAssetEditor_InstantDialogue::GetToolkitName() const
{
	const bool bDirtyState = EditingGraph->GetOutermost()->IsDirty();

	FFormatNamedArguments Args;
	Args.Add(TEXT("InstantDialogueName"), FText::FromString(EditingGraph->GetName()));
	Args.Add(TEXT("DirtyState"), bDirtyState ? FText::FromString(TEXT("*")) : FText::GetEmpty());
	return FText::Format(LOCTEXT("InstantDialogueEditorToolkitName", "{InstantDialogueName}{DirtyState}"), Args);
}

FText FAssetEditor_InstantDialogue::GetToolkitToolTipText() const
{
	return FAssetEditorToolkit::GetToolTipTextForObject(EditingGraph);
}

FLinearColor FAssetEditor_InstantDialogue::GetWorldCentricTabColorScale() const
{
	return FLinearColor::White;
}

FString FAssetEditor_InstantDialogue::GetWorldCentricTabPrefix() const
{
	return TEXT("InstantDialogueEditor");
}

FString FAssetEditor_InstantDialogue::GetDocumentationLink() const
{
	return TEXT("");
}

void FAssetEditor_InstantDialogue::SaveAsset_Execute()
{
	if (EditingGraph != nullptr)
	{
		RebuildInstantDialogue();
	}

	FAssetEditorToolkit::SaveAsset_Execute();
}

void FAssetEditor_InstantDialogue::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(EditingGraph);
	Collector.AddReferencedObject(EditingGraph->EdGraph);
}

UInstantDialogueEditorSettings* FAssetEditor_InstantDialogue::GetSettings() const
{
	return GenricGraphEditorSettings;
}

TSharedRef<SDockTab> FAssetEditor_InstantDialogue::SpawnTab_Viewport(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FInstantDialogueAssetEditorTabs::ViewportID);

	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab)
		.Label(LOCTEXT("ViewportTab_Title", "Viewport"));

	if (ViewportWidget.IsValid())
	{
		SpawnedTab->SetContent(ViewportWidget.ToSharedRef());
	}

	return SpawnedTab;
}

TSharedRef<SDockTab> FAssetEditor_InstantDialogue::SpawnTab_Details(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FInstantDialogueAssetEditorTabs::InstantDialoguePropertyID);

	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("LevelEditor.Tabs.Details"))
		.Label(LOCTEXT("Details_Title", "Property"))
		[
			PropertyWidget.ToSharedRef()
		];
}

TSharedRef<SDockTab> FAssetEditor_InstantDialogue::SpawnTab_EditorSettings(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == FInstantDialogueAssetEditorTabs::InstantDialogueEditorSettingsID);

	//whats in the sDock
	return SNew(SDockTab)
		.Icon(FEditorStyle::GetBrush("LevelEditor.Tabs.Details"))
		.Label(LOCTEXT("EditorSettings_Title", "Instant Dialogue Editor Setttings"))
		[
			EditorSettingsWidget.ToSharedRef()
		];
}

void FAssetEditor_InstantDialogue::CreateInternalWidgets()
{
	// the graph viewport(the place where the nodes live)
	ViewportWidget = CreateViewportWidget();

	FDetailsViewArgs Args;
	Args.bHideSelectionTip = true;
	Args.NotifyHook = this;

	// the right option tab
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyWidget = PropertyModule.CreateDetailView(Args);
	PropertyWidget->SetObject(EditingGraph);
	PropertyWidget->OnFinishedChangingProperties().AddSP(this, &FAssetEditor_InstantDialogue::OnFinishedChangingProperties);

	EditorSettingsWidget = PropertyModule.CreateDetailView(Args);
	EditorSettingsWidget->SetObject(GenricGraphEditorSettings);
}

TSharedRef<SGraphEditor> FAssetEditor_InstantDialogue::CreateViewportWidget()
{
	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = LOCTEXT("AppearanceCornerText_InstantDialogue", "My Instant Dialogue");

	CreateCommandList();

	SGraphEditor::FGraphEditorEvents InEvents;
	InEvents.OnSelectionChanged = SGraphEditor::FOnSelectionChanged::CreateSP(this, &FAssetEditor_InstantDialogue::OnSelectedNodesChanged);
	InEvents.OnNodeDoubleClicked = FSingleNodeEvent::CreateSP(this, &FAssetEditor_InstantDialogue::OnNodeDoubleClicked);

	return SNew(SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		.IsEditable(true)
		.Appearance(AppearanceInfo)
		.GraphToEdit(EditingGraph->EdGraph)
		.GraphEvents(InEvents)
		.AutoExpandActionMenu(true)
		.ShowGraphStateOverlay(false);
}

void FAssetEditor_InstantDialogue::BindCommands()
{
	ToolkitCommands->MapAction(FEditorCommands_InstantDialogue::Get().GraphSettings,
		FExecuteAction::CreateSP(this, &FAssetEditor_InstantDialogue::GraphSettings),
		FCanExecuteAction::CreateSP(this, &FAssetEditor_InstantDialogue::CanGraphSettings)
	);

	/*ToolkitCommands->MapAction(FEditorCommands_InstantDialogue::Get().AutoArrange,
		FExecuteAction::CreateSP(this, &FAssetEditor_InstantDialogue::AutoArrange),
		FCanExecuteAction::CreateSP(this, &FAssetEditor_InstantDialogue::CanAutoArrange)
	);*/
}

void FAssetEditor_InstantDialogue::CreateEdGraph()
{
	if (EditingGraph->EdGraph == nullptr)
	{
		EditingGraph->EdGraph = CastChecked<UEdGraph_InstantDialogue>(FBlueprintEditorUtils::CreateNewGraph(EditingGraph, NAME_None, UEdGraph_InstantDialogue::StaticClass(), UAssetGraphSchema_InstantDialogue::StaticClass()));
		EditingGraph->EdGraph->bAllowDeletion = false;

		// Give the schema a chance to fill out any required nodes (like the results node)
		const UEdGraphSchema* Schema = EditingGraph->EdGraph->GetSchema();
		Schema->CreateDefaultNodesForGraph(*EditingGraph->EdGraph);
	}
}

void FAssetEditor_InstantDialogue::CreateCommandList()
{
	if (GraphEditorCommands.IsValid())
	{
		return;
	}

	GraphEditorCommands = MakeShareable(new FUICommandList);

	// Can't use CreateSP here because derived editor are already implementing TSharedFromThis<FAssetEditorToolkit>
	// however it should be safe, since commands are being used only within this editor
	// if it ever crashes, this function will have to go away and be reimplemented in each derived class

	GraphEditorCommands->MapAction(FEditorCommands_InstantDialogue::Get().GraphSettings,
		FExecuteAction::CreateRaw(this, &FAssetEditor_InstantDialogue::GraphSettings),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_InstantDialogue::CanGraphSettings));

	/*GraphEditorCommands->MapAction(FEditorCommands_InstantDialogue::Get().AutoArrange,
		FExecuteAction::CreateRaw(this, &FAssetEditor_InstantDialogue::AutoArrange),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_InstantDialogue::CanAutoArrange));*/

	GraphEditorCommands->MapAction(FGenericCommands::Get().SelectAll,
		FExecuteAction::CreateRaw(this, &FAssetEditor_InstantDialogue::SelectAllNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_InstantDialogue::CanSelectAllNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
		FExecuteAction::CreateRaw(this, &FAssetEditor_InstantDialogue::DeleteSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_InstantDialogue::CanDeleteNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Copy,
		FExecuteAction::CreateRaw(this, &FAssetEditor_InstantDialogue::CopySelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_InstantDialogue::CanCopyNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Cut,
		FExecuteAction::CreateRaw(this, &FAssetEditor_InstantDialogue::CutSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_InstantDialogue::CanCutNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Paste,
		FExecuteAction::CreateRaw(this, &FAssetEditor_InstantDialogue::PasteNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_InstantDialogue::CanPasteNodes)
	);

	GraphEditorCommands->MapAction(FGenericCommands::Get().Duplicate,
		FExecuteAction::CreateRaw(this, &FAssetEditor_InstantDialogue::DuplicateNodes),
		FCanExecuteAction::CreateRaw(this, &FAssetEditor_InstantDialogue::CanDuplicateNodes)
	);

	/*GraphEditorCommands->MapAction(FInstantDialogue::Get().Rename,
		FExecuteAction::CreateSP(this, &FAssetEditor_InstantDialogue::OnRenameNode),
		FCanExecuteAction::CreateSP(this, &FAssetEditor_InstantDialogue::CanRenameNodes)
	);*/

	GraphEditorCommands->MapAction( FGraphEditorCommands::Get().RemoveExecutionPin,
	FExecuteAction::CreateSP( this, &FAssetEditor_InstantDialogue::OnRemoveExecutionPin ),
	FCanExecuteAction::CreateSP( this, &FAssetEditor_InstantDialogue::CanRemoveExecutionPin )
				);
}

TSharedPtr<SGraphEditor> FAssetEditor_InstantDialogue::GetCurrGraphEditor() const
{
	return ViewportWidget;
}

FGraphPanelSelectionSet FAssetEditor_InstantDialogue::GetSelectedNodes() const
{
	FGraphPanelSelectionSet CurrentSelection;
	TSharedPtr<SGraphEditor> FocusedGraphEd = GetCurrGraphEditor();
	if (FocusedGraphEd.IsValid())
	{
		CurrentSelection = FocusedGraphEd->GetSelectedNodes();
	}

	return CurrentSelection;
}

void FAssetEditor_InstantDialogue::RebuildInstantDialogue()
{
	if (EditingGraph == nullptr)
	{
		LOG_WARNING(TEXT("FInstantDialogueAssetEditor::RebuildInstantDialogue EditingGraph is nullptr"));
		return;
	}

	UEdGraph_InstantDialogue* EdGraph = Cast<UEdGraph_InstantDialogue>(EditingGraph->EdGraph);
	check(EdGraph != nullptr);

	EdGraph->RebuildInstantDialogue();
}

void FAssetEditor_InstantDialogue::SelectAllNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (CurrentGraphEditor.IsValid())
	{
		CurrentGraphEditor->SelectAllNodes();
	}
}

bool FAssetEditor_InstantDialogue::CanSelectAllNodes()
{
	return true;
}

void FAssetEditor_InstantDialogue::DeleteSelectedNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	const FScopedTransaction Transaction(FGenericCommands::Get().Delete->GetDescription());

	CurrentGraphEditor->GetCurrentGraph()->Modify();

	const FGraphPanelSelectionSet SelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		UEdGraphNode* EdNode = Cast<UEdGraphNode>(*NodeIt);
		if (EdNode == nullptr || !EdNode->CanUserDeleteNode())
			continue;;

		if (UEdNode_InstantDialogueNode* EdNode_Node = Cast<UEdNode_InstantDialogueNode>(EdNode))
		{
			EdNode_Node->Modify();

			const UEdGraphSchema* Schema = EdNode_Node->GetSchema();
			if (Schema != nullptr)
			{
				Schema->BreakNodeLinks(*EdNode_Node);
			}

			EdNode_Node->DestroyNode();
		}
		else
		{
			EdNode->Modify();
			EdNode->DestroyNode();
		}
	}
}

bool FAssetEditor_InstantDialogue::CanDeleteNodes()
{
	// If any of the nodes can be deleted then we should allow deleting
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node != nullptr && Node->CanUserDeleteNode())
		{
			return true;
		}
	}

	return false;
}

void FAssetEditor_InstantDialogue::DeleteSelectedDuplicatableNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}

	const FGraphPanelSelectionSet OldSelectedNodes = CurrentGraphEditor->GetSelectedNodes();
	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode())
		{
			CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}

	// Delete the duplicatable nodes
	DeleteSelectedNodes();

	CurrentGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(OldSelectedNodes); SelectedIter; ++SelectedIter)
	{
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter))
		{
			CurrentGraphEditor->SetNodeSelection(Node, true);
		}
	}
}

void FAssetEditor_InstantDialogue::CutSelectedNodes()
{
	CopySelectedNodes();
	DeleteSelectedDuplicatableNodes();
}

bool FAssetEditor_InstantDialogue::CanCutNodes()
{
	return CanCopyNodes() && CanDeleteNodes();
}

void FAssetEditor_InstantDialogue::CopySelectedNodes()
{
	// Export the selected nodes and place the text on the clipboard
	FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();

	FString ExportedText;

	for (FGraphPanelSelectionSet::TIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node == nullptr)
		{
			SelectedIter.RemoveCurrent();
			continue;
		}

		if (UEdNode_InstantDialogueEdge* EdNode_Edge = Cast<UEdNode_InstantDialogueEdge>(*SelectedIter))
		{
			UEdNode_InstantDialogueNode* StartNode = EdNode_Edge->GetStartNode();
			UEdNode_InstantDialogueNode* EndNode = EdNode_Edge->GetEndNode();

			if (!SelectedNodes.Contains(StartNode) || !SelectedNodes.Contains(EndNode))
			{
				SelectedIter.RemoveCurrent();
				continue;
			}
		}

		Node->PrepareForCopying();
	}

	FEdGraphUtilities::ExportNodesToText(SelectedNodes, ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);
}

bool FAssetEditor_InstantDialogue::CanCopyNodes()
{
	// If any of the nodes can be duplicated then we should allow copying
	const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode())
		{
			return true;
		}
	}

	return false;
}

void FAssetEditor_InstantDialogue::PasteNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (CurrentGraphEditor.IsValid())
	{
		PasteNodesHere(CurrentGraphEditor->GetPasteLocation());
	}
}

void FAssetEditor_InstantDialogue::PasteNodesHere(const FVector2D& Location)
{
	// Find the graph editor with focus
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return;
	}
	// Select the newly pasted stuff
	UEdGraph* EdGraph = CurrentGraphEditor->GetCurrentGraph();

	{
		const FScopedTransaction Transaction(FGenericCommands::Get().Paste->GetDescription());
		EdGraph->Modify();

		// Clear the selection set (newly pasted stuff will be selected)
		CurrentGraphEditor->ClearSelectionSet();

		// Grab the text to paste from the clipboard.
		FString TextToImport;
		FPlatformApplicationMisc::ClipboardPaste(TextToImport);

		// Import the nodes
		TSet<UEdGraphNode*> PastedNodes;
		FEdGraphUtilities::ImportNodesFromText(EdGraph, TextToImport, PastedNodes);

		//Average position of nodes so we can move them while still maintaining relative distances to each other
		FVector2D AvgNodePosition(0.0f, 0.0f);

		for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
		{
			UEdGraphNode* Node = *It;
			AvgNodePosition.X += Node->NodePosX;
			AvgNodePosition.Y += Node->NodePosY;
		}

		float InvNumNodes = 1.0f / float(PastedNodes.Num());
		AvgNodePosition.X *= InvNumNodes;
		AvgNodePosition.Y *= InvNumNodes;

		for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
		{
			UEdGraphNode* Node = *It;
			CurrentGraphEditor->SetNodeSelection(Node, true);

			Node->NodePosX = (Node->NodePosX - AvgNodePosition.X) + Location.X;
			Node->NodePosY = (Node->NodePosY - AvgNodePosition.Y) + Location.Y;

			Node->SnapToGrid(16);

			// Give new node a different Guid from the old one
			Node->CreateNewGuid();
		}
	}

	// Update UI
	CurrentGraphEditor->NotifyGraphChanged();

	UObject* GraphOwner = EdGraph->GetOuter();
	if (GraphOwner)
	{
		GraphOwner->PostEditChange();
		GraphOwner->MarkPackageDirty();
	}
}

bool FAssetEditor_InstantDialogue::CanPasteNodes()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (!CurrentGraphEditor.IsValid())
	{
		return false;
	}

	FString ClipboardContent;
	FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);

	return FEdGraphUtilities::CanImportNodesFromText(CurrentGraphEditor->GetCurrentGraph(), ClipboardContent);
}

void FAssetEditor_InstantDialogue::DuplicateNodes()
{
	CopySelectedNodes();
	PasteNodes();
}

bool FAssetEditor_InstantDialogue::CanDuplicateNodes()
{
	return CanCopyNodes();
}

void FAssetEditor_InstantDialogue::GraphSettings()
{
	PropertyWidget->SetObject(EditingGraph);
}

bool FAssetEditor_InstantDialogue::CanGraphSettings() const
{
	return true;
}

/*void FAssetEditor_InstantDialogue::AutoArrange()
{
	UEdGraph_InstantDialogue* EdGraph = Cast<UEdGraph_InstantDialogue>(EditingGraph->EdGraph);
	check(EdGraph != nullptr);

	const FScopedTransaction Transaction(LOCTEXT("InstantDialogueEditorAutoArrange", "Instant Dialogue Editor: Auto Arrange"));

	EdGraph->Modify();

	UAutoLayoutStrategy* LayoutStrategy = nullptr;
	switch (GenricGraphEditorSettings->AutoLayoutStrategy)
	{
	case EAutoLayoutStrategy::Tree:
		LayoutStrategy = NewObject<UAutoLayoutStrategy>(EdGraph, UTreeLayoutStrategy::StaticClass());
		break;
	case EAutoLayoutStrategy::ForceDirected:
		LayoutStrategy = NewObject<UAutoLayoutStrategy>(EdGraph, UForceDirectedLayoutStrategy::StaticClass());
		break;
	default:
		break;
	}

	if (LayoutStrategy != nullptr)
	{
		LayoutStrategy->Settings = GenricGraphEditorSettings;
		LayoutStrategy->Layout(EdGraph);
		LayoutStrategy->ConditionalBeginDestroy();
	}
	else
	{
		LOG_ERROR(TEXT("FAssetEditor_InstantDialogue::AutoArrange LayoutStrategy is null."));
	}
}

bool FAssetEditor_InstantDialogue::CanAutoArrange() const
{
	return EditingGraph != nullptr && Cast<UEdGraph_InstantDialogue>(EditingGraph->EdGraph) != nullptr;
}

void FAssetEditor_InstantDialogue::OnRenameNode()
{
	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (CurrentGraphEditor.IsValid())
	{
		const FGraphPanelSelectionSet SelectedNodes = GetSelectedNodes();
		for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
		{
			UEdGraphNode* SelectedNode = Cast<UEdGraphNode>(*NodeIt);
			if (SelectedNode != NULL && SelectedNode->bCanRenameNode)
			{
				CurrentGraphEditor->IsNodeTitleVisible(SelectedNode, true);
				break;
			}
		}
	}
}

bool FAssetEditor_InstantDialogue::CanRenameNodes() const
{
	UEdGraph_InstantDialogue* EdGraph = Cast<UEdGraph_InstantDialogue>(EditingGraph->EdGraph);
	check(EdGraph != nullptr);

	UInstantDialogue* Graph = EdGraph->GetInstantDialogue();
	check(Graph != nullptr)

	return Graph->bCanRenameNode && GetSelectedNodes().Num() == 1;
}*/

void FAssetEditor_InstantDialogue::OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection)
{
	TArray<UObject*> Selection;

	for (UObject* SelectionEntry : NewSelection)
	{
		Selection.Add(SelectionEntry);
	}

	if (Selection.Num() == 0) 
	{
		PropertyWidget->SetObject(EditingGraph);

	}
	else
	{
		PropertyWidget->SetObjects(Selection);
	}
}

void FAssetEditor_InstantDialogue::OnNodeDoubleClicked(UEdGraphNode* Node)
{
	
}

void FAssetEditor_InstantDialogue::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
	if (EditingGraph == nullptr)
		return;

	EditingGraph->EdGraph->GetSchema()->ForceVisualizationCacheClear();


	//---------tried to updated selected nodes---------
	//SEdNode_InstantDialogueNode::UpdateGraphNode
	//CastChecked<SEdNode_InstantDialogueNode>(GetSelectedNodes()[0]);
	//UEdNode_InstantDialogueNode* MyNode = CastChecked<UEdNode_InstantDialogueNode>(GraphNode);
	/*for (auto SelectedNode : GetSelectedNodes())
	{
		SEdNode_InstantDialogueNode* MyNode = CastChecked<SEdNode_InstantDialogueNode>(SelectedNode);
		MyNode->UpdateGraphNode();
	}*/

	UE_LOG(LogTemp, Warning, TEXT("Asset Editor, OnFinishedChangingProperties"));
}

void FAssetEditor_InstantDialogue::OnPackageSaved(const FString& PackageFileName, UObject* Outer)
{
	RebuildInstantDialogue();
}

void FAssetEditor_InstantDialogue::RegisterToolbarTab(const TSharedRef<class FTabManager>& InTabManager) 
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}

void  FAssetEditor_InstantDialogue::OnRemoveExecutionPin()
{
	 TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	 if (CurrentGraphEditor.IsValid())
	 {
	 	const FScopedTransaction Transaction( LOCTEXT("RemoveExecutionPin", "Remove Execution Pin") );

	 	UEdGraphPin* SelectedPin = CurrentGraphEditor->GetGraphPinForMenu();
	 	UEdGraphNode* OwningNode = SelectedPin->GetOwningNode();

	 	OwningNode->Modify();
	 	SelectedPin->Modify();

	 	if (UEdNode_InstantDialogueNode* InstantDialogueNode = Cast<UEdNode_InstantDialogueNode>(OwningNode))
	 	{
	 		InstantDialogueNode->RemovePinFromInstantDialogueNode(SelectedPin);
	 	}

	 	// Update the graph so that the node will be refreshed
	 	CurrentGraphEditor->NotifyGraphChanged();

	 	// ------->this is in the K2Node Implementation but the last line crashes here
	 	// UEdGraph* CurrentGraph = CurrentGraphEditor->GetCurrentGraph();
	 	// UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraphChecked(CurrentGraph);
	 	// FBlueprintEditorUtils::MarkBlueprintAsModified(Blueprint);
	 }
}

bool FAssetEditor_InstantDialogue::CanRemoveExecutionPin() const
{

	TSharedPtr<SGraphEditor> CurrentGraphEditor = GetCurrGraphEditor();
	if (CurrentGraphEditor.IsValid())
	{
		if (UEdGraphPin* SelectedPin = CurrentGraphEditor->GetGraphPinForMenu())
		{
			UEdGraphNode* OwningNode = SelectedPin->GetOwningNode();

			if (UEdNode_Anwser* AnswerNode = Cast<UEdNode_Anwser>(OwningNode))
			{
				if (SelectedPin->Direction == EGPD_Output)
				{
					return true;
				}
			}
		}
	}

	return false;
}


#undef LOCTEXT_NAMESPACE

