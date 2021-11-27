#include "SEdNode_InstantDialogueText.h"

#include "AssetRegistryModule.h"
#include "ContentBrowserModule.h"
#include "EdNode_InstantDialogueText.h"
#include "InstantDialogueGraph.h"
#include "Nodes/InstantDialogueNodeText.h"
#include "GraphEditorSettings.h"
#include "IContentBrowserSingleton.h"
#include "IDocumentation.h"
#include "SCommentBubble.h"
#include "SGraphPanel.h"
#include "SlateOptMacros.h"
#include "SLevelOfDetailBranchNode.h"
#include "SSkeletonWidget.h"
#include "TutorialMetaData.h"
#include "InstantDialogueAssetEditor/Colors_InstantDialogue.h"
#include "KismetPins/SGraphPinExec.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"
#include "InstantDialogueNodeData.h"


//// all paste
#include "Modules/ModuleManager.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboButton.h"
#include "Editor.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "ScopedTransaction.h"
#include "Engine/Selection.h"
#include "AssetRegistryModule.h"
#include "ConditionObject.h"
//#include "EasyBoxWidget.h"


///// propertyEditorClass
#include "Engine/Blueprint.h"
#include "Misc/FeedbackContext.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Layout/SBox.h"
#include "PropertyEditorClasses.h"

#include "DragAndDrop/ClassDragDropOp.h"
#include "DragAndDrop/AssetDragDropOp.h"
#include "ClassViewerModule.h"
#include "ClassViewerFilter.h"
#include "UObject/UObjectIterator.h"
#include "PropertyCustomizationHelpers.h"

#define LOCTEXT_NAMESPACE "SEdNode_Text"

class FPropertyEditorClassFilter : public IClassViewerFilter
{
public:
	/** The meta class for the property that classes must be a child-of. */
	const UClass* ClassPropertyMetaClass;

	/** The interface that must be implemented. */
	const UClass* InterfaceThatMustBeImplemented;

	/** Whether or not abstract classes are allowed. */
	bool bAllowAbstract;

	/** Classes that can be picked */
	TArray<const UClass*> AllowedClassFilters;

	/** Classes that can't be picked */
	TArray<const UClass*> DisallowedClassFilters;

	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs ) override
	{
		return IsClassAllowedHelper(InClass);
	}
	
	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef< const IUnloadedBlueprintData > InBlueprint, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
	{
		return IsClassAllowedHelper(InBlueprint);
	}

private:

	template <typename TClass>
	bool IsClassAllowedHelper(TClass InClass)
	{
		bool bMatchesFlags = !InClass->HasAnyClassFlags(CLASS_Hidden | CLASS_HideDropDown | CLASS_Deprecated) &&
			(bAllowAbstract || !InClass->HasAnyClassFlags(CLASS_Abstract));

		if (bMatchesFlags && InClass->IsChildOf(ClassPropertyMetaClass)
			&& (!InterfaceThatMustBeImplemented || InClass->ImplementsInterface(InterfaceThatMustBeImplemented)))
		{
			auto PredicateFn = [InClass](const UClass* Class)
			{
				return InClass->IsChildOf(Class);
			};

			if (DisallowedClassFilters.FindByPredicate(PredicateFn) == nullptr &&
				(AllowedClassFilters.Num() == 0 || AllowedClassFilters.FindByPredicate(PredicateFn) != nullptr))
			{
				return true;
			}
		}

		return false;
	}
};

class SEdNode_InstantDialogueNode;

class SInstantDialoguePinExec final : public SGraphPinExec
{
public:
	SInstantDialoguePinExec(){}

	SLATE_BEGIN_ARGS(SInstantDialoguePinExec) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
	{
		SGraphPinExec::Construct(SGraphPinExec::FArguments(), InPin);
		bUsePinColorForText = true;
	}
};

//////////////////////////////////////////////////////////////////

void SEdNode_Text::Construct(const FArguments& InArgs, UEdNode_InstantDialogueNode* InNode)
{
	GraphNode = InNode;
	//SetCanTick(false);
	SetCursor(EMouseCursor::CardinalCross);
	UpdateGraphNode();
	InNode->SEdNode = this;
}
BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SEdNode_Text::UpdateGraphNode()
{

	UEdNode_Text* MyNode = CastChecked<UEdNode_Text>(GraphNode);
	const FSlateBrush* PortraitIcon = nullptr;
	
	// the real one <----------
	UInstantDialogueNodeText* MyTexter = CastChecked<UInstantDialogueNodeText>(MyNode->InstantDialogueNode);
	MyTexter->MySlateBrush.SetResourceObject(MyTexter->GetPortrait());
	PortraitIcon = &MyTexter->MySlateBrush;
	// not part of the comment bulk//MyNode->InstantDialogueNode->MySlateBrush.SetResourceObject(MyNode->InstantDialogueNode->GetPortrait());
	//PortraitIcon = &MyNode->InstantDialogueNode->MySlateBrush;
	
	//FSlateBrush MySlater;
	
	//MySlater.SetResourceObject(MyNode->InstantDialogueNode->MySlateBrush.GetResourceObject()); //do it with texture2D
	//PortraitIcon = &MySlater;
	// not part of the comment bulk//with build in SlateBrush

	//MyNode->InstantDialogueNode->OnForcePropertyEditorUpdate.RemoveAll(this);
	//MyNode->InstantDialogueNode->OnForcePropertyEditorUpdate.AddSP(this, &SEdNode_Text::UpdateGraphNode);
	//MyNode->InstantDialogueNode->OnForcePropertyEditorUpdate.AddSP(this, &SEdNode_Text::UpdateGraphNode);
	
	
	InputPins.Empty();
	OutputPins.Empty();
	
	// error handling set-up
	SetupErrorReporting();
	
	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();
	
	
 	TSharedPtr< SToolTip > NodeToolTip = SNew( SToolTip );
	if (!GraphNode->GetTooltipText().IsEmpty())
	{
		NodeToolTip = IDocumentation::Get()->CreateToolTip( TAttribute< FText >( this, &SGraphNode::GetNodeTooltip ), NULL, GraphNode->GetDocumentationLink(), GraphNode->GetDocumentationExcerptName() );
	}
	
	// Setup a meta tag for this node
	FGraphNodeMetaData TagMeta(TEXT("Graphnode"));
	PopulateMetaTag(&TagMeta);
	
	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);
	
	//////////////////////////////Title Area
	// Get node icon
	IconColor = FLinearColor::White;
	const FSlateBrush* IconBrush = nullptr;
	if (GraphNode != NULL && GraphNode->ShowPaletteIconOnNode())
	{
		IconBrush = GraphNode->GetIconAndTint(IconColor).GetOptionalIcon();
	}
	
	TSharedRef<SOverlay> DefaultTitleAreaWidget =
		SNew(SOverlay)
		+SOverlay::Slot()
		[
			SNew(SImage)
			.Image( FEditorStyle::GetBrush("Graph.Node.TitleGloss") )
			.ColorAndOpacity( this, &SGraphNode::GetNodeTitleIconColor )
		]
		+SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			[
				SNew(SBorder)
				.BorderImage( FEditorStyle::GetBrush("Graph.Node.ColorSpill") )
				// The extra margin on the right
				// is for making the color spill stretch well past the node title
				.Padding( FMargin(10,5,30,3) )
				.BorderBackgroundColor( this, &SGraphNode::GetNodeTitleColor )
				[
					SNew(SHorizontalBox)
	
					//////////////////////////////// additional start
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Right)
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Left)
					.Padding(10, 10, 10, 10)
					.AutoWidth()
					[
						SNew(SImage)
						//.ColorAndOpacity(FColor::Magenta)
						.Image(PortraitIcon)
						//.Visibility(EVisibility::Collapsed)
					]
					
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Right)
					.VAlign(VAlign_Center)
					.Padding(0, 0, 5, 0)
					.AutoWidth()
					[
							//SNew(SMultiLineEditableText)
							SNew(SEditableText)
							.OnTextCommitted(this, &SEdNode_Text::OnSpeakerNameCommitted)
							.Text(MyTexter->GetSpeakerName())
					]
					//////////////////////////////// additional start
					
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Top) 
					.Padding(FMargin(0.f, 0.f, 4.f, 0.f))
					.AutoWidth()
					[
						SNew(SImage)
						.Image(IconBrush)
						.ColorAndOpacity(this, &SGraphNode::GetNodeTitleIconColor)
					]
					+ SHorizontalBox::Slot()
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							//CreateTitleWidget(NodeTitle)
							SAssignNew(InlineEditableText, SInlineEditableTextBlock)
							.Visibility(EVisibility::Collapsed)
							.Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							NodeTitle.ToSharedRef()
						]
					]
				]
			]
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Center)
			.Padding(0, 0, 5, 0)
			.AutoWidth()
			[
				CreateTitleRightWidget() //* Optionally create a widget to dock to the right in the title bar of the node.
			]
		]
		+SOverlay::Slot()
		.VAlign(VAlign_Top)
		[
			SNew(SBorder)
			.Visibility(EVisibility::HitTestInvisible)			
			.BorderImage( FEditorStyle::GetBrush( "Graph.Node.TitleHighlight" ) )
			.BorderBackgroundColor( this, &SGraphNode::GetNodeTitleIconColor )
			[
				SNew(SSpacer)
				.Size(FVector2D(20,20))
			]
		];
	
	SetDefaultTitleAreaWidget(DefaultTitleAreaWidget); // do I need that?
	
	TSharedRef<SWidget> TitleAreaWidget = 
		SNew(SLevelOfDetailBranchNode)
		//.UseLowDetailSlot(this, &SGraphNode::UseLowDetailNodeTitles)
		.LowDetail()
		[
			SNew(SBorder)
			.BorderImage( FEditorStyle::GetBrush("Graph.Node.ColorSpill") )
			.Padding( FMargin(75.0f, 22.0f) ) // Saving enough space for a 'typical' title so the transition isn't quite so abrupt
			.BorderBackgroundColor( this, &SGraphNode::GetNodeTitleColor )
		]
		.HighDetail()
		[
			DefaultTitleAreaWidget
		];
	
	
	TSharedRef<SOverlay> NodeOverlay = SNew(SOverlay);
	
	// add optional node specific widget to the overlay:
	TSharedPtr<SWidget> OverlayWidget = GraphNode->CreateNodeImage();
	if(OverlayWidget.IsValid())
	{
		NodeOverlay->AddSlot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SBox)
			.WidthOverride( 70.f )
			.HeightOverride( 70.f )
			[
				OverlayWidget.ToSharedRef()
			]
		];
	}
	
	NodeOverlay->AddSlot()
		//.HAlign(HAlign_Center)
		//.VAlign(VAlign_Center)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(45.f, 0.f, 45.f, 0.f)
		[
			// MIDDLE
			SNew(SVerticalBox)
			
			+SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			//.AutoHeight()
			.FillHeight(1.0f)
			[
				SNew(SMultiLineEditableTextBox)
				//.Padding(FMargin(20, 20,20,20)) //text isn't visible, padding goes to the inside
				//.ForceVolatile(true)
				//SNew(STextBlock)
				//SAssignNew(InlineEditableText, SInlineEditableTextBlock)
				//.TextStyle( FEditorStyle::Get(), "Graph.CompactNode.Title" )
				.Text(MyTexter->NodeDialogueText)
				.OnTextCommitted(this, &SEdNode_Text::OnDialogueCommitted)
				.WrapTextAt(200.0f)
					
			]

			
			// my new stuff
	
			+SVerticalBox::Slot()
			//.HAlign(HAlign_Center)
			//.VAlign(VAlign_Center)
			.AutoHeight()
			[
				SNew(SSpacer)
				.Size((FVector2D(0,0)))
			]
			/*+SVerticalBox::Slot()
			.AutoHeight()
			[
				NodeTitle.ToSharedRef()
			]*/
			
	
		];
	
	// Default to "pure" styling, where we can just center the pins vertically
	// since don't need to worry about alignment with other nodes
	float PinPaddingTop = 0.f;
	EVerticalAlignment PinVerticalAlignment = VAlign_Center;
	
	// But if this is an impure node, we'll align the pins to the top, 
	// and add some padding so that the exec pins line up with the exec pins of other nodes
	/*if (UK2Node* K2Node = Cast<UK2Node>(GraphNode))
	{
		if (!K2Node->IsNodePure())
		{
			PinPaddingTop += 8.0f;
			PinVerticalAlignment = VAlign_Top;
		}
	}*/
	
	NodeOverlay->AddSlot()
		.HAlign(HAlign_Left)
		.VAlign(PinVerticalAlignment)
		.Padding(0.f, PinPaddingTop, 55.f, 0.f)//right 55.f ->05.f
		[
			// LEFT
			SAssignNew(LeftNodeBox, SVerticalBox)
		];
	
	NodeOverlay->AddSlot()
		.HAlign(HAlign_Right)
		.VAlign(PinVerticalAlignment)
		.Padding(55.f, PinPaddingTop, 0.f, 0.f)
		[
			// RIGHT
			SAssignNew(RightNodeBox, SVerticalBox)
		];
	
	//
	//             ______________________
	//            | (>) L |      | R (>) |
	//            | (>) E |      | I (>) |
	//            | (>) F |   +  | G (>) |
	//            | (>) T |      | H (>) |
	//            |       |      | T (>) |
	//            |_______|______|_______|
	//
	this->ContentScale.Bind( this, &SGraphNode::GetContentScale );
	
	TSharedRef<SVerticalBox> InnerVerticalBox =
		SNew(SVerticalBox)
	/////////////////////////////
		+SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Top)
		//.Padding(Settings->GetNonPinNodeBodyPadding())
		[
			TitleAreaWidget
		]
		/////////////////////////////
		+SVerticalBox::Slot()
		[
			// NODE CONTENT AREA
			SNew( SOverlay)
			+SOverlay::Slot()
			[
				SNew(SImage)
				.Image( FEditorStyle::GetBrush("Graph.VarNode.Body") )
			]
			+ SOverlay::Slot()
			[
				SNew(SImage)
				.Image( FEditorStyle::GetBrush("Graph.VarNode.Gloss") )
			]
			+SOverlay::Slot()
			.Padding( FMargin(0,3) )//0, 3
			[
				NodeOverlay // this is the middle part, above is the code for it (should I make it the CreateNodeContentArea?)
			]
		];
	
	TSharedPtr<SWidget> EnabledStateWidget = GetEnabledStateWidget();
	if (EnabledStateWidget.IsValid())
	{
		InnerVerticalBox->AddSlot()
			.AutoHeight()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			.Padding(FMargin(2, 0))
			[
				EnabledStateWidget.ToSharedRef()
			];
	}
	
	InnerVerticalBox->AddSlot()
		.AutoHeight()
		.Padding( FMargin(5.0f, 1.0f) )
		[
			ErrorReporting->AsWidget()
		];
	
	this->GetOrAddSlot( ENodeZone::Center )
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Center)
	[
		InnerVerticalBox
	];
	
	CreatePinWidgets();
	
	// Hide pin labels
	for (auto InputPin: this->InputPins)
	{
		if (InputPin->GetPinObj()->ParentPin == nullptr)
		{
			InputPin->SetShowLabel(false);
		}
	}
	
	for (auto OutputPin : this->OutputPins)
	{
		if (OutputPin->GetPinObj()->ParentPin == nullptr)
		{
			OutputPin->SetShowLabel(false);
		}
	}
	
	// Create comment bubble
	TSharedPtr<SCommentBubble> CommentBubble;
	const FSlateColor CommentColor = GetDefault<UGraphEditorSettings>()->DefaultCommentNodeTitleColor;
	
	SAssignNew( CommentBubble, SCommentBubble )
	.GraphNode( GraphNode )
	.Text( this, &SGraphNode::GetNodeComment )
	.OnTextCommitted( this, &SGraphNode::OnCommentTextCommitted )
	.ColorAndOpacity( CommentColor )
	.AllowPinning( true )
	.EnableTitleBarBubble( true )
	.EnableBubbleCtrls( true )
	.GraphLOD( this, &SGraphNode::GetCurrentLOD )
	.IsGraphNodeHovered( this, &SGraphNode::IsHovered );
	
	GetOrAddSlot( ENodeZone::TopCenter )
	.SlotOffset( TAttribute<FVector2D>( CommentBubble.Get(), &SCommentBubble::GetOffset ))
	.SlotSize( TAttribute<FVector2D>( CommentBubble.Get(), &SCommentBubble::GetSize ))
	.AllowScaling( TAttribute<bool>( CommentBubble.Get(), &SCommentBubble::IsScalingAllowed ))
	.VAlign( VAlign_Top )
	[
		CommentBubble.ToSharedRef()
	];
	
	CreateInputSideAddButton(LeftNodeBox);
	CreateOutputSideAddButton(RightNodeBox);


////////////////////// standart one

	// InputPins.Empty();
	// OutputPins.Empty();
	//
	// // Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	// RightNodeBox.Reset();
	// LeftNodeBox.Reset();
 //
	// //
	// //             ______________________
	// //            |      TITLE AREA      |
	// //            +-------+------+-------+
	// //            | (>) L |      | R (>) |
	// //            | (>) E |      | I (>) |
	// //            | (>) F |      | G (>) |
	// //            | (>) T |      | H (>) |
	// //            |       |      | T (>) |
	// //            |_______|______|_______|
	// //
	// TSharedPtr<SVerticalBox> MainVerticalBox;
	// SetupErrorReporting();
 //
	// TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);
 //
	// // Get node icon
	// IconColor = FLinearColor::White;
	// const FSlateBrush* IconBrush = nullptr;
	// if (GraphNode != NULL && GraphNode->ShowPaletteIconOnNode())
	// {
	// 	IconBrush = GraphNode->GetIconAndTint(IconColor).GetOptionalIcon();
	// }
 //
	// TSharedRef<SOverlay> DefaultTitleAreaWidget =
	// 	SNew(SOverlay)
	// 	+SOverlay::Slot()
	// 	[
	// 		SNew(SImage)
	// 		.Image( FEditorStyle::GetBrush("Graph.Node.TitleGloss") )
	// 		.ColorAndOpacity( this, &SGraphNode::GetNodeTitleIconColor )
	// 	]
	// 	+SOverlay::Slot()
	// 	.HAlign(HAlign_Fill)
	// 	.VAlign(VAlign_Center)
	// 	[
	// 		SNew(SHorizontalBox)
	// 		+ SHorizontalBox::Slot()
	// 		.HAlign(HAlign_Fill)
	// 		[
	// 			SNew(SBorder)
	// 			.BorderImage( FEditorStyle::GetBrush("Graph.Node.ColorSpill") )
	// 			// The extra margin on the right
	// 			// is for making the color spill stretch well past the node title
	// 			.Padding( FMargin(10,5,30,3) )
	// 			.BorderBackgroundColor( this, &SGraphNode::GetNodeTitleColor )
	// 			[
	// 				SNew(SHorizontalBox)
	// 				+ SHorizontalBox::Slot()
	// 				.VAlign(VAlign_Top)
	// 				.Padding(FMargin(0.f, 0.f, 4.f, 0.f))
	// 				.AutoWidth()
	// 				[
	// 					SNew(SImage)
	// 					.Image(IconBrush)
	// 					.ColorAndOpacity(this, &SGraphNode::GetNodeTitleIconColor)
	// 				]
	// 				
	// 				//////////////////////////////// additional start
	//  				+ SHorizontalBox::Slot()
	//  				.HAlign(HAlign_Right)
	//  				.VAlign(VAlign_Center)
	//  				.HAlign(HAlign_Left)
	//  				.Padding(10, 10, 10, 10)
	//  				.AutoWidth()
	//  				[
	//  					SNew(SImage)
	//  					//.ColorAndOpacity(FColor::Magenta)
	//  					.Image(PortraitIcon)
	//  					//.Visibility(EVisibility::Collapsed)
	//  				]
	//  				
	//  				+ SHorizontalBox::Slot()
	//  				.HAlign(HAlign_Right)
	//  				.VAlign(VAlign_Center)
	//  				.Padding(0, 0, 5, 0)
	//  				.AutoWidth()
	//  				[
	//  						//SNew(SMultiLineEditableText)
	//  						SNew(SEditableText)
	//  						.OnTextCommitted(this, &SEdNode_Text::OnSpeakerNameCommitted)
	//  						.Text(MyTexter->GetSpeakerName())
	//  				]
	//  				//////////////////////////////// additional start
	//  				
	// 				+ SHorizontalBox::Slot()
	// 				[
	// 					SNew(SVerticalBox)
	// 					+ SVerticalBox::Slot()
	// 					.AutoHeight()
	// 					[
	// 						//CreateTitleWidget(NodeTitle)
	// 						SAssignNew(InlineEditableText, SInlineEditableTextBlock)
	//  						.Visibility(EVisibility::Collapsed)
 //                            //.Style(FEditorStyle::Get(), "Graph.StateNode.NodeTitleInlineEditableText")
 //                            .Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
 //                            //.OnVerifyTextChanged(this, &SEdNode_InstantDialogueNode::OnVerifyNameTextChanged)
 //                            .OnTextCommitted(this, &SEdNode_Text::OnNameTextCommited)
 //                            //.IsReadOnly(this, &SEdNode_InstantDialogueNode::IsNameReadOnly)
 //                            //.IsSelected(this, &SEdNode_InstantDialogueNode::IsSelectedExclusively)
	// 					]
	// 					+ SVerticalBox::Slot()
	// 					.AutoHeight()
	// 					[
	// 						NodeTitle.ToSharedRef()
	// 					]
	// 				]
	// 			]
	// 		]
	// 		+ SHorizontalBox::Slot()
	// 		.HAlign(HAlign_Right)
	// 		.VAlign(VAlign_Center)
	// 		.Padding(0, 0, 5, 0)
	// 		.AutoWidth()
	// 		[
	// 			CreateTitleRightWidget()
	// 		]
	// 	]
	// 	+SOverlay::Slot()
	// 	.VAlign(VAlign_Top)
	// 	[
	// 		SNew(SBorder)
	// 		.Visibility(EVisibility::HitTestInvisible)			
	// 		.BorderImage( FEditorStyle::GetBrush( "Graph.Node.TitleHighlight" ) )
	// 		.BorderBackgroundColor( this, &SGraphNode::GetNodeTitleIconColor )
	// 		[
	// 			SNew(SSpacer)
	// 			.Size(FVector2D(20,20))
	// 		]
	// 	];
 //
	// SetDefaultTitleAreaWidget(DefaultTitleAreaWidget);
 //
	// TSharedRef<SWidget> TitleAreaWidget = 
	// 	SNew(SLevelOfDetailBranchNode)
	// 	//.UseLowDetailSlot(this, &SGraphNode::UseLowDetailNodeTitles)
	// 	.LowDetail()
	// 	[
	// 		SNew(SBorder)
	// 		.BorderImage( FEditorStyle::GetBrush("Graph.Node.ColorSpill") )
	// 		.Padding( FMargin(75.0f, 22.0f) ) // Saving enough space for a 'typical' title so the transition isn't quite so abrupt
	// 		.BorderBackgroundColor( this, &SGraphNode::GetNodeTitleColor )
	// 	]
	// 	.HighDetail()
	// 	[
	// 		DefaultTitleAreaWidget
	// 	];
 //
	//
	// if (!SWidget::GetToolTip().IsValid())
	// {
	// 	TSharedRef<SToolTip> DefaultToolTip = IDocumentation::Get()->CreateToolTip( TAttribute< FText >( this, &SGraphNode::GetNodeTooltip ), NULL, GraphNode->GetDocumentationLink(), GraphNode->GetDocumentationExcerptName() );
	// 	SetToolTip(DefaultToolTip);
	// }
 //
	// // Setup a meta tag for this node
	// FGraphNodeMetaData TagMeta(TEXT("Graphnode"));
	// PopulateMetaTag(&TagMeta);
	//
	// TSharedPtr<SVerticalBox> InnerVerticalBox;
	// this->ContentScale.Bind( this, &SGraphNode::GetContentScale );
 //
 //
	// InnerVerticalBox = SNew(SVerticalBox)
	// 	+SVerticalBox::Slot()
	// 	.AutoHeight()
	// 	.HAlign(HAlign_Fill)
	// 	.VAlign(VAlign_Top)
	// 	.Padding(Settings->GetNonPinNodeBodyPadding())
	// 	[
	// 		TitleAreaWidget
	// 	]
 //
	// 	+SVerticalBox::Slot()
	// 	.AutoHeight()
	// 	.HAlign(HAlign_Fill)
	// 	.VAlign(VAlign_Top)
	// 	.Padding(45.f, 0.f, 45.f, 0.f) // new one del if text isn't spaced out correctly
	// 	[
	// 		CreateNodeContentArea()
	// 	];
 //
	// TSharedPtr<SWidget> EnabledStateWidget = GetEnabledStateWidget();
	// if (EnabledStateWidget.IsValid())
	// {
	// 	InnerVerticalBox->AddSlot()
	// 		.AutoHeight()
	// 		.HAlign(HAlign_Fill)
	// 		.VAlign(VAlign_Top)
	// 		.Padding(FMargin(2, 0))
	// 		[
	// 			EnabledStateWidget.ToSharedRef()
	// 		];
	// }
 //
	// InnerVerticalBox->AddSlot()
	// 	.AutoHeight()
	// 	.Padding(Settings->GetNonPinNodeBodyPadding())
	// 	[
	// 		ErrorReporting->AsWidget()
	// 	];
 //
 //
 //
	// this->GetOrAddSlot( ENodeZone::Center )
	// 	.HAlign(HAlign_Center)
	// 	.VAlign(VAlign_Center)
	// 	[
	// 		SAssignNew(MainVerticalBox, SVerticalBox)
	// 		+SVerticalBox::Slot()
	// 		.AutoHeight()
	// 		[
	// 			SNew(SOverlay)
	// 			.AddMetaData<FGraphNodeMetaData>(TagMeta)
	// 			+SOverlay::Slot()
	// 			.Padding(Settings->GetNonPinNodeBodyPadding())
	// 			[
	// 				SNew(SImage)
	// 				.Image(GetNodeBodyBrush())
	// 				.ColorAndOpacity(this, &SGraphNode::GetNodeBodyColor)
	// 			]
	// 			+SOverlay::Slot()
	// 			[
	// 				InnerVerticalBox.ToSharedRef()
	// 			]
	// 		]			
	// 	];
 //
	// bool SupportsBubble = true;
	// if (GraphNode != nullptr)
	// {
	// 	SupportsBubble = GraphNode->SupportsCommentBubble();
	// }
 //
	// if (SupportsBubble)
	// {
	// 	// Create comment bubble
	// 	TSharedPtr<SCommentBubble> CommentBubble;
	// 	const FSlateColor CommentColor = GetDefault<UGraphEditorSettings>()->DefaultCommentNodeTitleColor;
 //
	// 	SAssignNew(CommentBubble, SCommentBubble)
	// 		.GraphNode(GraphNode)
	// 		.Text(this, &SGraphNode::GetNodeComment)
	// 		.OnTextCommitted(this, &SGraphNode::OnCommentTextCommitted)
	// 		.OnToggled(this, &SGraphNode::OnCommentBubbleToggled)
	// 		.ColorAndOpacity(CommentColor)
	// 		.AllowPinning(true)
	// 		.EnableTitleBarBubble(true)
	// 		.EnableBubbleCtrls(true)
	// 		.GraphLOD(this, &SGraphNode::GetCurrentLOD)
	// 		.IsGraphNodeHovered(this, &SGraphNode::IsHovered);
 //
	// 	GetOrAddSlot(ENodeZone::TopCenter)
	// 		.SlotOffset(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetOffset))
	// 		.SlotSize(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetSize))
	// 		.AllowScaling(TAttribute<bool>(CommentBubble.Get(), &SCommentBubble::IsScalingAllowed))
	// 		.VAlign(VAlign_Top)
	// 		[
	// 			CommentBubble.ToSharedRef()
	// 		];
	// }
 //
	// CreateBelowWidgetControls(MainVerticalBox);
	// CreatePinWidgets();
	// CreateInputSideAddButton(LeftNodeBox);
	// CreateOutputSideAddButton(RightNodeBox);
	// CreateBelowPinControls(InnerVerticalBox);
	// CreateAdvancedViewArrow(InnerVerticalBox);
 //
	// //Hide pin labels
	// for (auto InputPin: this->InputPins) // should be probably at last/down under
	// 	{
	// 	if (InputPin->GetPinObj()->ParentPin == nullptr)
	// 	{
	// 		InputPin->SetShowLabel(false);
	// 	}
	// 	}
	//
	// for (auto OutputPin : this->OutputPins)
	// {
	// 	if (OutputPin->GetPinObj()->ParentPin == nullptr)
	// 	{
	// 		OutputPin->SetShowLabel(false);
	// 	}
	// }
}


///////////////////////// my new stuff start

TSharedRef<SWidget>	SEdNode_Text::GetDefaultValueWidget()
{
		return
		SNew(SHorizontalBox)
			//.Visibility(true)
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(2,0)
			.MaxWidth(100.0f)
		[
			SAssignNew(AssetPickerAnchor, SComboButton)
			.ButtonStyle( FEditorStyle::Get(), "PropertyEditor.AssetComboStyle" )
			.ForegroundColor( this, &SEdNode_Text::OnGetComboForeground)
			.ContentPadding( FMargin(2,2,2,1) )
			.ButtonColorAndOpacity( this, &SEdNode_Text::OnGetWidgetBackground )
			.MenuPlacement(MenuPlacement_BelowAnchor)
			//.IsEnabled(this, &SGraphPin::IsEditingEnabled)
			.ButtonContent()
			[
				SNew(STextBlock)
				.ColorAndOpacity( this, &SEdNode_Text::OnGetComboForeground )
				.TextStyle( FEditorStyle::Get(), "PropertyEditor.AssetClass" )
				.Font( FEditorStyle::GetFontStyle( "PropertyWindow.NormalFont" ) )
				.Text( this, &SEdNode_Text::OnGetComboTextValue )
				.ToolTipText( this, &SEdNode_Text::GetObjectToolTip )
			]
			.OnGetMenuContent(this, &SEdNode_Text::GenerateAssetPicker)
		];
			// +SHorizontalBox::Slot()
			// .AutoWidth()
			// .Padding(1,0)
			// .VAlign(VAlign_Center)
			// [
			// 	SAssignNew(UseButton, SButton)
			// 	.ButtonStyle( FEditorStyle::Get(), "NoBorder" )
			// 	.ButtonColorAndOpacity( this, &SEdNode_Text::OnGetWidgetBackground )
			// 	.OnClicked( GetOnUseButtonDelegate() )
			// 	.ContentPadding(1.f)
			// 	.ToolTipText(NSLOCTEXT("GraphEditor", "ObjectGraphPin_Use_Tooltip", "Use asset browser selection"))
			// 	//.IsEnabled(this, &SEdNode_Text::IsEditingEnabled)
			// 	[
			// 		SNew(SImage)
			// 		.ColorAndOpacity( this, &SEdNode_Text::OnGetWidgetForeground )
			// 		.Image( FEditorStyle::GetBrush(TEXT("PropertyWindow.Button_Use")) )
			// 	]
			// ]
			// // Browse button
			// +SHorizontalBox::Slot()
			// .AutoWidth()
			// .Padding(1,0)
			// .VAlign(VAlign_Center)
			// [
			// 	SAssignNew(BrowseButton, SButton)
			// 	.ButtonStyle( FEditorStyle::Get(), "NoBorder" )
			// 	.ButtonColorAndOpacity( this, &SEdNode_Text::OnGetWidgetBackground )
			// 	.OnClicked( GetOnBrowseButtonDelegate() )
			// 	.ContentPadding(0)
			// 	.ToolTipText(NSLOCTEXT("GraphEditor", "ObjectGraphPin_Browse_Tooltip", "Browse"))
			// 	[
			// 		SNew(SImage)
			// 		.ColorAndOpacity( this, &SEdNode_Text::OnGetWidgetForeground )
			// 		.Image( FEditorStyle::GetBrush(TEXT("PropertyWindow.Button_Browse")) )
			// 	]
			// ];
}

namespace GraphPinObjectDefs
{
	// Active Combo pin alpha
	static const float ActiveComboAlpha = 1.f;
	// InActive Combo pin alpha
	static const float InActiveComboAlpha = 0.6f;
	// Active foreground pin alpha
	static const float ActivePinForegroundAlpha = 1.f;
	// InActive foreground pin alpha
	static const float InactivePinForegroundAlpha = 0.15f;
	// Active background pin alpha
	static const float ActivePinBackgroundAlpha = 0.8f;
	// InActive background pin alpha
	static const float InactivePinBackgroundAlpha = 0.4f;
};

FSlateColor SEdNode_Text::OnGetComboForeground() const
{
	float Alpha = IsHovered() ? GraphPinObjectDefs::ActiveComboAlpha : GraphPinObjectDefs::InActiveComboAlpha;
	return FSlateColor( FLinearColor( 1.f, 1.f, 1.f, Alpha ) );
}

FSlateColor SEdNode_Text::OnGetWidgetForeground() const
{
	float Alpha = IsHovered() ? GraphPinObjectDefs::ActivePinForegroundAlpha : GraphPinObjectDefs::InactivePinForegroundAlpha;
	return FSlateColor( FLinearColor( 1.f, 1.f, 1.f, Alpha ) );
}

FSlateColor SEdNode_Text::OnGetWidgetBackground() const
{
	float Alpha = IsHovered() ? GraphPinObjectDefs::ActivePinBackgroundAlpha : GraphPinObjectDefs::InactivePinBackgroundAlpha;
	return FSlateColor( FLinearColor( 1.f, 1.f, 1.f, Alpha ) );
}

TSharedRef<SWidget> SEdNode_Text::GenerateAssetPicker() // what about this one ---->UAssetManager<----
{
	// This class and its children are the classes that we can show objects for
	//UClass* AllowedClass = Cast<UClass>(UObject::StaticClass());
	//UClass* AllowedClass = UObject::StaticClass();
	
	//UClass* AllowedClass = UBlueprint::StaticClass();
	//UClass* AllowedClass = (GetParentNativeClass(UInstantDialogueConditionObject::StaticClass()));
	TSubclassOf<UBlueprint> Subclass = UBlueprint::StaticClass();
	UClass* AllowedClass = Subclass;
	 
	
	//UClass* AllowedClass = TSubclassOf<UInstantDialogueConditionObject>().Get()->StaticClass(); 

	//UDataAsset* AllowedClass = UDataAsset::StaticClass();
	
	//AllowedClass = UObject::StaticClass();

	FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));

	
	FAssetPickerConfig AssetPickerConfig;
	AssetPickerConfig.Filter.ClassNames.Add(AllowedClass->GetFName());
	AssetPickerConfig.Filter.bIncludeOnlyOnDiskAssets = false;
	AssetPickerConfig.Filter.bRecursivePaths = true;
	AssetPickerConfig.Filter.bRecursiveClasses = true; // important
	AssetPickerConfig.bAllowNullSelection = true;
	AssetPickerConfig.OnAssetSelected = FOnAssetSelected::CreateSP(this, &SEdNode_Text::OnAssetSelectedFromPicker);
	AssetPickerConfig.OnAssetEnterPressed = FOnAssetEnterPressed::CreateSP(this, &SEdNode_Text::OnAssetEnterPressedInPicker);
	AssetPickerConfig.InitialAssetViewType = EAssetViewType::List;
	AssetPickerConfig.bAllowDragging = false;


	//AssetPickerConfig.Filter.RecursiveClassesExclusionSet.Add(FName("same"));
	
	//Check with the node to see if there is any "AllowClasses" or "DisallowedClasses" metadata for the pin
	FString AllowedClassesFilterString = UInstantDialogueConditionObject::StaticClass()->GetName();
	//FString AllowedClassesFilterString = GraphPinObj->GetOwningNode()->GetPinMetaData(GraphPinObj->PinName, FName(TEXT("AllowedClasses")));
	// if( !AllowedClassesFilterString.IsEmpty() )
	// {
	// 	// Clear out the allowed class names and have the pin's metadata override.
	// 	AssetPickerConfig.Filter.ClassNames.Empty();
	//
	// 	// Parse and add the classes from the metadata
	// 	TArray<FString> AllowedClassesFilterNames;
	// 	AllowedClassesFilterString.ParseIntoArrayWS(AllowedClassesFilterNames, TEXT(","), true);
	// 	
	// 	for(const FString& AllowedClassesFilterName : AllowedClassesFilterNames)
	// 	{
	// 		AssetPickerConfig.Filter.ClassNames.Add(FName(*AllowedClassesFilterName));
	// 	}
	// }
	//
	// FString DisallowedClassesFilterString;
	// if(!DisallowedClassesFilterString.IsEmpty())
	// {
	// 	TArray<FString> DisallowedClassesFilterNames;
	// 	DisallowedClassesFilterString.ParseIntoArrayWS(DisallowedClassesFilterNames, TEXT(","), true);
	// 	for(const FString& DisallowedClassesFilterName : DisallowedClassesFilterNames)
	// 	{
	// 		AssetPickerConfig.Filter.RecursiveClassesExclusionSet.Add(FName(*DisallowedClassesFilterName));
	// 	}
	// }

	return
		SNew(SBox)
		.HeightOverride(300)
		.WidthOverride(300)
		[
			SNew(SBorder)
			.BorderImage( FEditorStyle::GetBrush("Menu.Background") )
			[
				ContentBrowserModule.Get().CreateAssetPicker(AssetPickerConfig)
			]
		];
}

void SEdNode_Text::OnAssetEnterPressedInPicker(const TArray<FAssetData>& InSelectedAssets) //just if I select more than one
{
	if(InSelectedAssets.Num() > 0)
	{
		OnAssetSelectedFromPicker(InSelectedAssets[0]);
	}
}

void SEdNode_Text::OnAssetSelectedFromPicker(const struct FAssetData& AssetData)
{

	//FString myCollString = AssetData.AssetName.ToString();
	UE_LOG(LogTemp, Warning, TEXT("This is asset %s"), &AssetData.AssetClass)

	if (AssetData.IsValid())
	{
		CachedAssetData = AssetData;
	}

//	CachedAssetData.IsValid() ? CachedAssetData = AssetData : CachedAssetData = nullptr;
	
	// const FAssetData& CurrentAssetData = GetAssetData(true);
	// if(CurrentAssetData != AssetData)
	// {
	// 	const FScopedTransaction Transaction( NSLOCTEXT("GraphEditor", "ChangeObjectPinValue", "Change Object Pin Value" ) );
	//
	// 	// Close the asset picker
	// 	AssetPickerAnchor->SetIsOpen(false);
	//
	// 	
	//
	// 	// Set the object found from the asset picker
	// 	//GraphPinObj->GetSchema()->TrySetDefaultValue(*GraphPinObj, AssetData.ObjectPath.ToString());
	// }

	AssetPickerAnchor->SetIsOpen(false);
}

const FAssetData& SEdNode_Text::GetAssetData(bool bRuntimePath) const
{
		// FName ObjectPath = FName(*DefaultValue);
		// if (ObjectPath != CachedAssetData.ObjectPath)
		// {
		// 	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
		//
		// 	CachedAssetData = AssetRegistryModule.Get().GetAssetByObjectPath(ObjectPath);
		//
		// 	if (!CachedAssetData.IsValid())
		// 	{
		// 		FString PackageName = FPackageName::ObjectPathToPackageName(DefaultValue);
		// 		FString PackagePath = FPackageName::GetLongPackagePath(PackageName);
		// 		FString ObjectName = FPackageName::ObjectPathToObjectName(DefaultValue);
		//
		// 		// Fake one
		// 		CachedAssetData = FAssetData(FName(*PackageName), FName(*PackagePath), FName(*ObjectName), UObject::StaticClass()->GetFName());
		// 	}
		// }

	return CachedAssetData;
}

// user button
FOnClicked SEdNode_Text::GetOnUseButtonDelegate()
{
	return FOnClicked::CreateSP( this, &SEdNode_Text::OnClickUse );
}

FReply SEdNode_Text::OnClickUse()
{
	FEditorDelegates::LoadSelectedAssetsIfNeeded.Broadcast();

	//UClass* ObjectClass = Cast<UClass>(GraphPinObj->PinType.PinSubCategoryObject.Get());
	//UClass* ObjectClass = UObject::StaticClass();

	//UPrimaryDataAsset
	UClass* ObjectClass = UInstantDialogueConditionObject::StaticClass();
	if (ObjectClass != NULL)
	{
		UObject* SelectedObject = GEditor->GetSelectedObjects()->GetTop(ObjectClass);
		if(SelectedObject != NULL)
		{
			const FScopedTransaction Transaction(NSLOCTEXT("GraphEditor", "ChangeObjectPinValue", "Change Object Pin Value"));
			//GraphPinObj->Modify();

			//GraphPinObj->GetSchema()->TrySetDefaultObject(*GraphPinObj, SelectedObject);
		}
	}

	return FReply::Handled();
}
// user button end

FOnClicked SEdNode_Text::GetOnBrowseButtonDelegate()
{
	return FOnClicked::CreateSP( this, &SEdNode_Text::OnClickBrowse );
}

FReply SEdNode_Text::OnClickBrowse()
{
	const FAssetData& AssetData = GetAssetData(false);
	if (AssetData.IsValid())
	{
		TArray<FAssetData> Objects;
		Objects.Add(AssetData);

		GEditor->SyncBrowserToObjects(Objects);
	}
	return FReply::Handled();
}

FText SEdNode_Text::OnGetComboTextValue() const
{
	FText Value = GetDefaultComboText();

	if (CachedAssetData.IsValid())
	{
		Value = FText::FromString(CachedAssetData.AssetName.ToString());
	}
	
	//if (GraphPinObj != nullptr) // class UEdGraphPin* GraphPinObj;
	//{
		const FAssetData& CurrentAssetData = GetAssetData(true);

		// ############## YOU NEED THIS, GET NAME OF CURRENT OBJECT ###################
		//UObject* DefaultObject = GraphPinObj->DefaultObject; // DefaultObject == UObject*
		// UObject* DefaultObject = UObject::StaticClass(); // DefaultObject == UObject*
		// if (UField* Field = Cast<UField>(DefaultObject))
		// {
		// 	Value = Field->GetDisplayNameText();
		// }
		// else if (CurrentAssetData.IsValid())
		// {
		// 	Value = FText::FromString(CurrentAssetData.AssetName.ToString());
		// }
	//}

	UE_LOG(LogTemp, Warning, TEXT("Get text %s"), &Value)
	
	return Value;
}

FText SEdNode_Text::GetDefaultComboText() const
{
	return LOCTEXT( "DefaultComboText", "Select Asset" );
}

FText SEdNode_Text::GetObjectToolTip() const
{
	return GetValue();
}

FText SEdNode_Text::GetValue() const
{
	const FAssetData& CurrentAssetData = GetAssetData(true);
	FText Value;
	if (CurrentAssetData.IsValid())
	{
		Value = FText::FromString(CurrentAssetData.GetFullName());
	}
	else
	{
		//if (GraphPinObj->GetSchema()->IsSelfPin(*GraphPinObj))
		//{
			//Value =  FText::FromName(GraphPinObj->PinName);
		//}
		//else
		//{
		Value = FText::GetEmpty();
		//}
	}
	return Value;
}

//////////////////////////////////////////////////////// SPropertyEditorClass
TSharedRef<SWidget> SEdNode_Text::GenerateComboButton() //const FArguments& InArgs, const TSharedPtr< class FPropertyEditor >& InPropertyEditor 
{
	//PropertyEditor = InPropertyEditor;
	
	//if (PropertyEditor.IsValid())
	//{
		//const TSharedRef<FPropertyNode> PropertyNode = PropertyEditor->GetPropertyNode();
		//FProperty* const Property = PropertyNode->GetProperty();
		//if (FClassProperty* const ClassProp = CastField<FClassProperty>(Property))
		//{
			//MetaClass = ClassProp->MetaClass;
		//}
		//else if (FSoftClassProperty* const SoftClassProperty = CastField<FSoftClassProperty>(Property))
		//{
		//	MetaClass = SoftClassProperty->MetaClass;
		//}
		//else
		//{
		//	check(false);
		//}
		
		//bAllowAbstract = Property->GetOwnerProperty()->HasMetaData(TEXT("AllowAbstract"));
		//bAllowOnlyPlaceable = Property->GetOwnerProperty()->HasMetaData(TEXT("OnlyPlaceable"));
		//bIsBlueprintBaseOnly = Property->GetOwnerProperty()->HasMetaData(TEXT("BlueprintBaseOnly"));
		//RequiredInterface = Property->GetOwnerProperty()->GetClassMetaData(TEXT("MustImplement"));
		//bAllowNone = !(Property->PropertyFlags & CPF_NoClear);
		//bShowViewOptions = Property->GetOwnerProperty()->HasMetaData(TEXT("HideViewOptions")) ? false : true;
		//bShowTree = Property->GetOwnerProperty()->HasMetaData(TEXT("ShowTreeView"));
		//bShowDisplayNames = Property->GetOwnerProperty()->HasMetaData(TEXT("ShowDisplayNames"));

		// copy from above
		bAllowAbstract = false;
		bAllowOnlyPlaceable = false; //true
		bIsBlueprintBaseOnly = false;
		RequiredInterface = nullptr;
		bAllowNone = true;
		bShowViewOptions = true;
		bShowTree = true; //false
		bShowDisplayNames = true;

		//extras
		MetaClass = UInstantDialogueConditionObject::StaticClass();
		AllowedClassFilters.Empty();
		DisallowedClassFilters.Empty();
		OnSetClass = nullptr;
		SelectedClass = nullptr;

		// auto FillClassFilters = [&](TArray<const UClass*> &ClassFilters, FName TagName)
		// {
		// 	const FString* ClassesFilterString = &Property->GetMetaData(TagName);
		//
		// 	ClassFilters.Empty();
		//
		// 	if (!ClassesFilterString->IsEmpty())
		// 	{
		// 		TArray<FString> ClassFilterNames;
		// 		ClassesFilterString->ParseIntoArrayWS(ClassFilterNames, TEXT(","), true);
		//
		// 		for (const FString& ClassName : ClassFilterNames)
		// 		{
		// 			UClass* Class = FindObject<UClass>(ANY_PACKAGE, *ClassName);
		//
		// 			if (!Class)
		// 			{
		// 				Class = LoadObject<UClass>(nullptr, *ClassName);
		// 			}
		//
		// 			if (Class)
		// 			{
		// 				// If the class is an interface, expand it to be all classes in memory that implement the class.
		// 				if (Class->HasAnyClassFlags(CLASS_Interface))
		// 				{
		// 					for (TObjectIterator<UClass> ClassIt; ClassIt; ++ClassIt)
		// 					{
		// 						UClass* const ClassWithInterface = (*ClassIt);
		// 						if (ClassWithInterface->ImplementsInterface(Class))
		// 						{
		// 							ClassFilters.Add(ClassWithInterface);
		// 						}
		// 					}
		// 				}
		// 				else
		// 				{
		// 					ClassFilters.Add(Class);
		// 				}
		// 			}
		// 		}
		// 	}
		// };
		//
		// FillClassFilters(AllowedClassFilters, "AllowedClasses");
		// FillClassFilters(DisallowedClassFilters, "DisallowedClasses");
	//}
	//else
	//{
		// check(InArgs._MetaClass);
		// check(InArgs._SelectedClass.IsSet());
		// check(InArgs._OnSetClass.IsBound());
		//
		// MetaClass = InArgs._MetaClass;
		// RequiredInterface = InArgs._RequiredInterface;
		// bAllowAbstract = InArgs._AllowAbstract;
		// bIsBlueprintBaseOnly = InArgs._IsBlueprintBaseOnly;
		// bAllowNone = InArgs._AllowNone;
		// bAllowOnlyPlaceable = false;
		// bShowViewOptions = InArgs._ShowViewOptions;
		// bShowTree = InArgs._ShowTree;
		// bShowDisplayNames = InArgs._ShowDisplayNames;
		// AllowedClassFilters.Empty();
		// DisallowedClassFilters.Empty();
		// SelectedClass = InArgs._SelectedClass;
		// OnSetClass = InArgs._OnSetClass;

	
	//}

	CreateClassFilter();

	return
	SAssignNew(ComboButton, SComboButton)
		.OnGetMenuContent(this, &SEdNode_Text::GenerateClassPicker)
		.ContentPadding(FMargin(2.0f, 2.0f))
		.ToolTipText(this, &SEdNode_Text::GetDisplayValueAsString)
		.ButtonContent()
		[
			SNew(STextBlock)
			.Text(this, &SEdNode_Text::GetDisplayValueAsString)
			//.Font(InArgs._Font)
		];

	// ChildSlot
	// [
	// 	ComboButton.ToSharedRef()
	// ];
}

/** Util to give better names for BP generated classes */
static FString GetClassDisplayName(const UObject* Object, bool bShowDisplayNames)
{
	const UClass* Class = Cast<UClass>(Object);
	if (Class != NULL)
	{
		UBlueprint* BP = UBlueprint::GetBlueprintFromClass(Class);
		if(BP != NULL)
		{
			return BP->GetName();
		}
		if (bShowDisplayNames && Class->HasMetaData(TEXT("DisplayName")))
		{
			return Class->GetMetaData(TEXT("DisplayName"));
		}
	}
	return (Object) ? Object->GetName() : "None";
}

FText SEdNode_Text::GetDisplayValueAsString() const
{
	static bool bIsReentrant = false;

	// Guard against re-entrancy which can happen if the delegate executed below (SelectedClass.Get()) forces a slow task dialog to open, thus causing this to lose context and regain focus later starting the loop over again
	if( !bIsReentrant )
	{
		TGuardValue<bool>( bIsReentrant, true );
		//if(PropertyEditor.IsValid())
		//{
			//UObject* ObjectValue = NULL;
			//FPropertyAccess::Result Result = PropertyEditor->GetPropertyHandle()->GetValue(ObjectValue);

			//if(Result == FPropertyAccess::Success && ObjectValue != NULL)
			//{
			//	return FText::FromString(GetClassDisplayName(ObjectValue, bShowDisplayNames));
			//}

			//return FText::FromString(FPaths::GetBaseFilename(PropertyEditor->GetValueAsString()));
		//}
		//return FText::FromString(FPaths::GetBaseFilename(PropertyEditor->GetValueAsString()));
		return FText::FromString(GetClassDisplayName(SelectedClass.Get(), bShowDisplayNames));
	}
	else
	{
		return FText::GetEmpty();
	}
	
}

void SEdNode_Text::CreateClassFilter()
{
	ClassViewerOptions.bShowBackgroundBorder = false;
	ClassViewerOptions.bShowUnloadedBlueprints = true;
	ClassViewerOptions.bShowNoneOption = bAllowNone;

	// if (PropertyEditor.IsValid())
	// {
	// 	ClassViewerOptions.PropertyHandle = PropertyEditor->GetPropertyHandle();
	// }

	// FClassViewerInitializationOptions
	ClassViewerOptions.bIsBlueprintBaseOnly = bIsBlueprintBaseOnly;
	ClassViewerOptions.bIsPlaceableOnly = bAllowOnlyPlaceable;
	ClassViewerOptions.NameTypeToDisplay = (bShowDisplayNames ? EClassViewerNameTypeToDisplay::DisplayName : EClassViewerNameTypeToDisplay::ClassName);
	ClassViewerOptions.DisplayMode = bShowTree ? EClassViewerDisplayMode::TreeView : EClassViewerDisplayMode::ListView;
	ClassViewerOptions.bAllowViewOptions = bShowViewOptions;


	///// my Filter
	// TSharedPtr<FInstantConditionFilterViewer> InstantConditionFilterViewer = MakeShareable(new FInstantConditionFilterViewer);
	// InstantConditionFilterViewer->AllowedChildrenOfClasses.Add(UObject::StaticClass());
	// ClassViewerOptions.ClassFilter = InstantConditionFilterViewer;
	//
	// ClassFilter = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer").CreateClassFilter(ClassViewerOptions);
	// ClassFilterFuncs = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer").CreateFilterFuncs();
	////
	
	TSharedPtr<FPropertyEditorClassFilter> PropEdClassFilter = MakeShareable(new FPropertyEditorClassFilter);
	ClassViewerOptions.ClassFilter = PropEdClassFilter;

	PropEdClassFilter->ClassPropertyMetaClass = MetaClass;
	PropEdClassFilter->InterfaceThatMustBeImplemented = RequiredInterface;
	PropEdClassFilter->bAllowAbstract = bAllowAbstract;
	PropEdClassFilter->AllowedClassFilters = AllowedClassFilters;
	PropEdClassFilter->DisallowedClassFilters = DisallowedClassFilters;

	ClassFilter = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer").CreateClassFilter(ClassViewerOptions);
	ClassFilterFuncs = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer").CreateFilterFuncs();
}

TSharedRef<SWidget> SEdNode_Text::GenerateClassPicker()
{
	FOnClassPicked OnPicked(FOnClassPicked::CreateRaw(this, &SEdNode_Text::OnClassPicked));

	return SNew(SBox)
		.WidthOverride(280)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight()
			.MaxHeight(500)
			[
				FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer").CreateClassViewer(ClassViewerOptions, OnPicked)
			]			
		];
}

void SEdNode_Text::OnClassPicked(UClass* InClass)
{
	SelectedClass = InClass;
	
	if(!InClass)
	{
		//SendToObjects(TEXT("None"));
	}
	else
	{
		
		//OnSetClass.Execute(InClass);
		//SendToObjects(InClass->GetPathName());
	}

	
	ComboButton->SetIsOpen(false);
}

void SEdNode_Text::SendToObjects(const FString& NewValue)
{
	// if(PropertyEditor.IsValid())
	// {
	// 	const TSharedRef<IPropertyHandle> PropertyHandle = PropertyEditor->GetPropertyHandle();
	// 	PropertyHandle->SetValueFromFormattedString(NewValue);
	// }
	/*else*/ if (!NewValue.IsEmpty() && NewValue != TEXT("None"))
	{
		UClass* NewClass = FindObject<UClass>(ANY_PACKAGE, *NewValue);
		if(!NewClass)
		{
			NewClass = LoadObject<UClass>(nullptr, *NewValue);
		}
		
		OnSetClass.Execute(SelectedClass.Get());
	}
	else
	{
		OnSetClass.Execute(nullptr);
	}

}

///////////////////////////// my new stuff end

TSharedRef<SWidget> SEdNode_Text::CreateNodeContentArea()
{

	UEdNode_Text* MyNode = CastChecked<UEdNode_Text>(GraphNode);
	UInstantDialogueNodeText* MyTexter = CastChecked<UInstantDialogueNodeText>(MyNode->InstantDialogueNode);
	
	return SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush("NoBorder"))
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.FillWidth(1.0f)
			[
				SAssignNew(LeftNodeBox, SVerticalBox)
			]

			+SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
           	.VAlign(VAlign_Fill)
			.AutoWidth()
			.FillWidth(1.0f)
			[
				//middle
				SNew(SMultiLineEditableTextBox)
 				//.Padding(FMargin(20, 20,20,20)) //text isn't visible, padding goes to the inside
 				//.ForceVolatile(true)
 				//SNew(STextBlock)
 				//SAssignNew(InlineEditableText, SInlineEditableTextBlock)
 				//.TextStyle( FEditorStyle::Get(), "Graph.CompactNode.Title" )
 				.Text(MyTexter->NodeDialogueText)
 				.OnTextCommitted(this, &SEdNode_Text::OnDialogueCommitted)
 				.WrapTextAt(200.0f)
			]
			
			+SHorizontalBox::Slot()
			.AutoWidth()
			.HAlign(HAlign_Right)
			[
				SAssignNew(RightNodeBox, SVerticalBox)
			]
		];
}

void SEdNode_Text::CreateStandardPinWidget(UEdGraphPin* Pin)
{
	/*TSharedPtr<SGraphPin> NewPin = SNew(SFlowGraphPinExec, Pin);

	if (!UFlowGraphSettings::Get()->bShowDefaultPinNames && FlowGraphNode->GetFlowNode())
	{
		if (Pin->Direction == EGPD_Input)
		{
			if (FlowGraphNode->GetFlowNode()->GetInputPins().Num() == 1 && Pin->PinName == UFlowNode::DefaultInputPin.PinName)
			{
				NewPin->SetShowLabel(false);
			}
		}
		else
		{
			if (FlowGraphNode->GetFlowNode()->GetOutputPins().Num() == 1 && Pin->PinName == UFlowNode::DefaultOutputPin.PinName)
			{
				NewPin->SetShowLabel(false);
			}
		}
	}

	this->AddPin(NewPin.ToSharedRef());  **/


	const bool bShowPin = ShouldPinBeHidden(Pin);

	if (bShowPin)
	{
		TSharedPtr<SGraphPin> NewPin = CreatePinWidget(Pin);
		check(NewPin.IsValid());

		// test to see what it does --------------
		//NewPin->SetShowLabel(false);
		// end of test area ----------------------

		this->AddPin(NewPin.ToSharedRef());
	}
}

void SEdNode_Text::CreateOutputSideAddButton(TSharedPtr<SVerticalBox> OutputBox)
{
}

FReply SEdNode_Text::OnAddPin()
{
	UEdNode_InstantDialogueNode* MyGraphNode = CastChecked<UEdNode_InstantDialogueNode>(GraphNode);

	const FScopedTransaction Transaction( LOCTEXT("AddExecutionPin", "Add Execution Pin"));
	MyGraphNode->Modify();
	
	MyGraphNode->AddPinToInstantDialogueNode(MyGraphNode->Pins.Num());

	UpdateGraphNode();
	GraphNode->GetGraph()->NotifyGraphChanged();
	
	return FReply::Handled();
}

// TSharedPtr<SGraphPin> SEdNode_Text::CreatePinWidget(UEdGraphPin* Pin) const
// {
// 	//return FNodeFactory::CreatePinWidget(Pin); //CreateK2PinWidget
// 	return SNew(SInstantDialoguePinExec, Pin);
// }

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SEdNode_Text::CreatePinWidgets()
{
	// Create Pin widgets for each of the pins.
	for (int32 PinIndex = 0; PinIndex < GraphNode->Pins.Num(); ++PinIndex)
	{
		UEdGraphPin* CurPin = GraphNode->Pins[PinIndex];

		if ( !ensureMsgf(CurPin->GetOuter() == GraphNode
			, TEXT("Graph node ('%s' - %s) has an invalid %s pin: '%s'; (with a bad %s outer: '%s'); skiping creation of a widget for this pin.")
			, *GraphNode->GetNodeTitle(ENodeTitleType::ListView).ToString()
			, *GraphNode->GetPathName()
			, (CurPin->Direction == EEdGraphPinDirection::EGPD_Input) ? TEXT("input") : TEXT("output")
			,  CurPin->PinFriendlyName.IsEmpty() ? *CurPin->PinName.ToString() : *CurPin->PinFriendlyName.ToString()
			,  CurPin->GetOuter() ? *CurPin->GetOuter()->GetClass()->GetName() : TEXT("UNKNOWN")
			,  CurPin->GetOuter() ? *CurPin->GetOuter()->GetPathName() : TEXT("NULL")) )
		{
			continue;
		}

		CreateStandardPinWidget(CurPin);
	}
	
}

/*void SEdNode_Text::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	const UEdGraphPin* PinObj = PinToAdd->GetPinObj();
	const bool bAdvancedParameter = PinObj && PinObj->bAdvancedView;
	if (bAdvancedParameter)
	{
		PinToAdd->SetVisibility( TAttribute<EVisibility>(PinToAdd, &SGraphPin::IsPinVisibleAsAdvanced) );
	}

	if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Input)
	{
		LeftNodeBox->AddSlot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillHeight(1.0f)
			.Padding(20.0f,0.0f)
			[
				PinToAdd
			];
		InputPins.Add(PinToAdd);
	}
	else // Direction == EEdGraphPinDirection::EGPD_Output
	{
		RightNodeBox->AddSlot() //OutputPinBox
			.AutoHeight()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Center)
			.Padding(Settings->GetOutputPinPadding())
			[
				PinToAdd
			];
		OutputPins.Add(PinToAdd);
	}	
}*/

void SEdNode_Text::OnSpeakerNameCommitted(const FText& InText, ETextCommit::Type CommitInfo)
{
	UEdNode_Text* MyNode = CastChecked<UEdNode_Text>(GraphNode);
	UInstantDialogueNodeText* MyTexter = CastChecked<UInstantDialogueNodeText>(MyNode->InstantDialogueNode);

	if (MyNode != nullptr && MyTexter != nullptr)
	{
		const FScopedTransaction Transaction(LOCTEXT("InstantDialogueEditorRenameNode", "Instant Dialogue Editor: Rename Node"));
		MyNode->Modify();
		MyTexter->Modify();
		MyTexter->SetSpeakerName(InText); // this calls the function to change the property
		UpdateGraphNode();
	}
}

void SEdNode_Text::OnDialogueCommitted(const FText& InText, ETextCommit::Type CommitInfo)
{
	UEdNode_Text* MyNode = CastChecked<UEdNode_Text>(GraphNode);
	UInstantDialogueNodeText* MyTexter = CastChecked<UInstantDialogueNodeText>(MyNode->InstantDialogueNode);
	
	if (MyNode != nullptr && MyNode->InstantDialogueNode != nullptr)
	{
		const FScopedTransaction Transaction(LOCTEXT("InstantDialogueEditorSetDialogueNode", "Instant Dialogue Editor: Set Dialogue"));
		MyNode->Modify();
		MyTexter->Modify();
		MyTexter->SetDialogue(InText);
		UpdateGraphNode();
	}
}

/*void SEdNode_Text::SetOwner( const TSharedRef<SGraphPanel>& OwnerPanel )
{
	check( !OwnerGraphPanelPtr.IsValid() );
	SetParentPanel(OwnerPanel);
	OwnerGraphPanelPtr = OwnerPanel;
	GraphNode->DEPRECATED_NodeWidget = SharedThis(this);

	//Once we have an owner, and if hide Unused pins is enabled, we need to remake our pins to drop the hidden ones
	if(OwnerGraphPanelPtr.Pin()->GetPinVisibility() != SGraphEditor::Pin_Show 
		&& LeftNodeBox.IsValid()
		&& RightNodeBox.IsValid())
	{
		this->LeftNodeBox->ClearChildren();
		this->RightNodeBox->ClearChildren();
		CreatePinWidgets();
	}
}*/

/*bool SEdNode_Text::IsNameReadOnly() const
{
	UEdNode_Text* EdNode_Node = Cast<UEdNode_Text>(GraphNode);
	check(EdNode_Node != nullptr);

	UInstantDialogue* InstantDialogue = EdNode_Node->InstantDialogueNode->Graph;
	check(InstantDialogue != nullptr);

	return !InstantDialogue->bCanRenameNode || SGraphNode::IsNameReadOnly();
}*/

FSlateColor SEdNode_Text::GetBorderBackgroundColor() const
{
	UEdNode_Text* MyNode = CastChecked<UEdNode_Text>(GraphNode);
	return MyNode ? MyNode->GetBackgroundColor() : InstantDialogueColors::NodeBorder::HighlightAbortRange0;
}

FSlateColor SEdNode_Text::GetBackgroundColor() const
{
	return InstantDialogueColors::NodeBody::Default;
}

EVisibility SEdNode_Text::GetDragOverMarkerVisibility() const
{
	return EVisibility::Visible;
}

const FSlateBrush* SEdNode_Text::GetNameIcon() const
{
	return FEditorStyle::GetBrush(TEXT("BTEditor.Graph.BTNode.Icon"));
}


#undef LOCTEXT_NAMESPACE