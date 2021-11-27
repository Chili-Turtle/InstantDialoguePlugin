// Fill out your copyright notice in the Description page of Project Settings.


#include "SEdNode_InstantDialogueAnswer.h"

#include "EdNode_InstantDialogueAnswer.h"
#include "InstantDialogueGraph.h"
#include "GraphEditorSettings.h"
#include "SLevelOfDetailBranchNode.h"
#include "IDocumentation.h"
#include "NodeFactory.h"
#include "SCommentBubble.h"
#include "SGraphPanel.h"
#include "SGraphPin.h"
#include "SlateOptMacros.h"
#include "TutorialMetaData.h"
#include "InstantDialogueAssetEditor/Colors_InstantDialogue.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"
#include "Widgets/Text/SMultiLineEditableText.h"
#include "SEdNode_InstantDialogueNode.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"

#define LOCTEXT_NAMESPACE "SEdNode_Anwser"

class SEdNode_InstantDialogueNode;

/*class SInstantDialoguePinAnwser : public SGraphPin
{
	SLATE_BEGIN_ARGS(SedNode_Anwser) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
	{
		this->SetCursor(EMouseCursor::Default);

		bShowLabel = true;
		GraphPinObj = InPin;
		check(GraphPinObj != nullptr);

		const UEdGraphSchema* Schema = GraphPinObj->GetSchema();
		check(Schema);

		SBorder::Construct(SBorder::FArguments()
			.BorderImage(this, &SInstantDialoguePinAnwser::GetPinBorder)
			.BorderBackgroundColor(this, &SInstantDialoguePinAnwser::GetPinColor)
			.OnMouseButtonDown(this, &SInstantDialoguePinAnwser::OnPinMouseDown)
			.Cursor(this, &SInstantDialoguePinAnwser::GetPinCursor)
			.Padding(FMargin(10.0f))
		);

	}

protected:
	virtual FSlateColor GetPinColor() const override
	{
		return InstantDialogueColors::Pin::Default;
	}

	virtual TSharedRef<SWidget>	GetDefaultValueWidget() override
	{
		return SNew(STextBlock);
	}

	const FSlateBrush* GetPinBorder() const
	{
		return FEditorStyle::GetBrush(TEXT("Graph.StateNode.Body"));
	}
};*/


//////////////////////////////////////////////////////////////////////////


/*SedNode_Anwser::SedNode_Anwser()
{
}

SedNode_Anwser::~SedNode_Anwser()
{
}*/

void SedNode_Anwser::Construct(const FArguments& InArgs, UEdNode_InstantDialogueNode* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
	InNode->SEdNode = this;
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SedNode_Anwser::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();
	OutputTextBoxes.Empty();

	// error handling set-up
	SetupErrorReporting();

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	UEdNode_Anwser* MyNode = CastChecked<UEdNode_Anwser>(GraphNode);
	//int32 removedDel = MyNode->InstantDialogueNode->OnForcePropertyEditorUpdate.RemoveAll(this);
	//UE_LOG(LogTemp, Error, TEXT("removed Delegated %d"), removedDel);

	
	//MyNode->InstantDialogueNode->OnForcePropertyEditorUpdate.AddSP(this, &SedNode_Anwser::UpdateGraphNode);
	
 	TSharedPtr< SToolTip > NodeToolTip = SNew( SToolTip );
	if (!GraphNode->GetTooltipText().IsEmpty())
	{
		NodeToolTip = IDocumentation::Get()->CreateToolTip( TAttribute< FText >( this, &SGraphNode::GetNodeTooltip ), NULL, GraphNode->GetDocumentationLink(), GraphNode->GetDocumentationExcerptName() );
	}

	// Setup a meta tag for this node
	FGraphNodeMetaData TagMeta(TEXT("Graphnode"));
	PopulateMetaTag(&TagMeta);
	
	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);
		//.Text(GraphNode->GetNodeTitle(ENodeTitleType::FullTitle));



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
							//.Visibility(EVisibility::Collapsed)
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
			SNew( SBox )
			.WidthOverride( 70.f )
			.HeightOverride( 70.f )
			[
				OverlayWidget.ToSharedRef()
			]
		];
	}

	NodeOverlay->AddSlot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.Padding(45.f, 0.f, 45.f, 0.f)
		[
			// MIDDLE
			SNew(SVerticalBox)
			// +SVerticalBox::Slot()
			// .HAlign(HAlign_Center)
			// .AutoHeight()
			// [
			// 	SNew(STextBlock)
			// 	//SAssignNew(InlineEditableText, SInlineEditableTextBlock)
			// 	//.Visibility(EVisibility::Collapsed)
			// 	.TextStyle( FEditorStyle::Get(), "Graph.CompactNode.Title" )
			// 	.Text( NodeTitle.Get(), &SNodeTitle::GetHeadTitle )
			// 	.WrapTextAt(128.0f)
			// 		
			// ]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				NodeTitle.ToSharedRef()
			]
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
		.Padding(0.f, PinPaddingTop, 55.f, 0.f)
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
		.Padding(Settings->GetNonPinNodeBodyPadding())
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
			.Padding( FMargin(0,3) )
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

	//UEdNode_Anwser* MyNode = CastChecked<UEdNode_Anwser>(GraphNode);
	UInstantDialogueNodeAnwser* MyNodeAnswer = CastChecked<UInstantDialogueNodeAnwser>(MyNode->InstantDialogueNode);
	for (int i = 0; i < OutputTextBoxes.Num(); ++i)
	{
		if (MyNodeAnswer->NameText.IsValidIndex(i))
		{
			OutputTextBoxes[i]->SetText(MyNodeAnswer->NameText[i]); //MyNodeAnswer->NameText[i] crash
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
}

void SedNode_Anwser::CreateStandardPinWidget(UEdGraphPin* CurPin)
{
	const bool bShowPin = ShouldPinBeHidden(CurPin);

	if (bShowPin)
	{
		TSharedPtr<SGraphPin> NewPin = CreatePinWidget(CurPin);
		check(NewPin.IsValid());

		this->AddPin(NewPin.ToSharedRef());
	}
}

// ---------------------- additional area start ----------------------
void SedNode_Anwser::OnAnswerTextCommitted(const FText& InText, ETextCommit::Type CommitInfo, int OutputIndex)
{
	UEdNode_Anwser* MyNode = CastChecked<UEdNode_Anwser>(GraphNode);
	UInstantDialogueNodeAnwser* MyNodeAnswer = CastChecked<UInstantDialogueNodeAnwser>(MyNode->InstantDialogueNode);
	if (MyNode != nullptr && MyNode->InstantDialogueNode != nullptr)
	{
		const FScopedTransaction Transaction(LOCTEXT("InstantDialogueEditorSetDialogueNode", "Instant Dialogue Editor: Set Dialogue"));
		MyNode->Modify();
		MyNode->InstantDialogueNode->Modify();
		MyNodeAnswer->NameText[OutputIndex] = InText;
		UE_LOG(LogTemp, Warning, TEXT("OnAnswerTextCommited Index == %d"), OutputIndex)
		//OutputTextBoxes[OutputIndex]->SetText(MyNodeAnswer->NameText[OutputIndex]); 
		UpdateGraphNode();
	}
}

TSharedRef<SWidget> SedNode_Anwser::CreateNodeContentArea() // this is not used I think or is is?
{
	//UEdNode_Anwser* MyNode = CastChecked<UEdNode_Anwser>(GraphNode);
	
	// NODE CONTENT AREA
	return SNew(SBorder)
		.BorderImage( FEditorStyle::GetBrush("NoBorder") )
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding( FMargin(0,3) )
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.AutoWidth()
			[
				// LEFT
				SAssignNew(LeftNodeBox, SVerticalBox)
			]

			+SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.AutoWidth()
			[
				//middle
				SNew(SMultiLineEditableText)
				//.Text(MyNode->InstantDialogueNode->NodeDialogueText)
				//.OnTextCommitted(this, &SedNode_Anwser::OnDialogueCommitted)
			]

			
			+SHorizontalBox::Slot()
			.AutoWidth()
			.HAlign(HAlign_Right)
			[
				// RIGHT
				SAssignNew(RightNodeBox, SVerticalBox)
			]
		];
}

void SedNode_Anwser::SetOwner( const TSharedRef<SGraphPanel>& OwnerPanel )
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
}

void SedNode_Anwser::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));
	TSharedPtr<SMultiLineEditableTextBox> OutputTextBox = SNew(SMultiLineEditableTextBox);
	
	UEdNode_Anwser* MyNode = CastChecked<UEdNode_Anwser>(GraphNode);
	UInstantDialogueNodeAnwser* MyNodeAnswer = CastChecked<UInstantDialogueNodeAnwser>(MyNode->InstantDialogueNode);
	

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
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				[
					SAssignNew(OutputTextBox,SMultiLineEditableTextBox)
					//SNew(SMultiLineEditableTextBox)
					.Text(MyNodeAnswer->NameText[OutputTextBoxes.Num()]) //  MyNodeAnswer->GetNameTextElement() //MyNodeAnswer->NameText.Top() verbose?
					.OnTextCommitted(this, &SedNode_Anwser::OnAnswerTextCommitted, OutputTextBoxes.Num())
					.WrapTextAt(150.0f)
				]
				+ SHorizontalBox::Slot()
				[
					PinToAdd	
				]
			];
		OutputPins.Add(PinToAdd);
		OutputTextBoxes.Add(OutputTextBox);
	}

}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FSlateColor SedNode_Anwser::GetBorderBackgroundColor() const
{
	UEdNode_Anwser* MyNode = CastChecked<UEdNode_Anwser>(GraphNode);
	return MyNode ? MyNode->GetBackgroundColor() : InstantDialogueColors::NodeBorder::HighlightAbortRange0;
}

FSlateColor SedNode_Anwser::GetBackgroundColor() const
{
	return InstantDialogueColors::NodeBody::Default;
}

EVisibility SedNode_Anwser::GetDragOverMarkerVisibility() const
{
	return EVisibility::Visible;
}

const FSlateBrush* SedNode_Anwser::GetNameIcon() const
{
	return FEditorStyle::GetBrush(TEXT("BTEditor.Graph.BTNode.Icon"));
}

#undef LOCTEXT_NAMESPACE