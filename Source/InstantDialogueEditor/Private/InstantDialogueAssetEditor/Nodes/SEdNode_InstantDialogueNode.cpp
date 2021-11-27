#include "SEdNode_InstantDialogueNode.h"
#include "InstantDialogueEditorPCH.h"
//#include "Colors_InstantDialogue.h"
#include "InstantDialogueAssetEditor/Colors_InstantDialogue.h"
#include "SLevelOfDetailBranchNode.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"
#include "SCommentBubble.h"
#include "SlateOptMacros.h"
#include "SGraphPin.h"
#include "GraphEditorSettings.h"
#include "EdNode_InstantDialogueNode.h"
//#include "InstantDialogueEditorStyle.h"
#include "InstantDialogueAssetEditor/InstantDialogueEditorStyle.h"
#include "IDocumentation.h"
#include "NodeFactory.h"
#include "SGraphPanel.h"
#include "TutorialMetaData.h"
#include "Widgets/Text/SMultiLineEditableText.h"
#include "Widgets/Text/SRichTextBlock.h"
#include "SNodePanel.h"
#include "Components/SlateWrapperTypes.h"
#include "KismetPins/SGraphPinExec.h"

#define LOCTEXT_NAMESPACE "EdNode_InstantDialogue"


//////////////////////////////////////////////////////////////////////////
class SInstantDialoguePin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SInstantDialoguePin) {}
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
			.BorderImage(this, &SInstantDialoguePin::GetPinBorder)
			.BorderBackgroundColor(this, &SInstantDialoguePin::GetPinColor)
			.OnMouseButtonDown(this, &SInstantDialoguePin::OnPinMouseDown)
			.Cursor(this, &SInstantDialoguePin::GetPinCursor)
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
};

class SGraphPinExecLabel : public SGraphPinExec
{
	SLATE_BEGIN_ARGS(SGraphPinExecLabel) {}
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, UEdGraphPin* InPin)
	{
		bShowLabel = true;
		SGraphPinExec::Construct(SGraphPinExec::FArguments() , InPin);
	}
	
};
//////////////////////////////////////////////////////////////////////////
void SEdNode_InstantDialogueNode::Construct(const FArguments& InArgs, UEdNode_InstantDialogueNode* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
	InNode->SEdNode = this;
}

//BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SEdNode_InstantDialogueNode::UpdateGraphNode()
{
	//const FMargin NodePadding = FMargin(2.0f);
	
	/*InputPins.Empty();
	OutputPins.Empty();

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();
	OutputPinBox.Reset();

	TSharedPtr<SErrorText> ErrorText;
	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("Graph.StateNode.Body"))
			.Padding(0.0f)
			.BorderBackgroundColor(this, &SEdNode_InstantDialogueNode::GetBorderBackgroundColor)
			[
				SNew(SOverlay)

				// Pins and node details
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SVerticalBox)

					// INPUT PIN AREA
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBox)
						.MinDesiredHeight(NodePadding.Top)
						[
							SAssignNew(LeftNodeBox, SVerticalBox)
						]
					]

					// STATE NAME AREA
					+ SVerticalBox::Slot()
					.Padding(FMargin(NodePadding.Left, 0.0f, NodePadding.Right, 0.0f))
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SAssignNew(NodeBody, SBorder)
							.BorderImage(FEditorStyle::GetBrush("BTEditor.Graph.BTNode.Body"))
							.BorderBackgroundColor(this, &SEdNode_InstantDialogueNode::GetBackgroundColor)
							.HAlign(HAlign_Fill)
							.VAlign(VAlign_Center)
							.Visibility(EVisibility::SelfHitTestInvisible)
							[
								SNew(SOverlay)
								+ SOverlay::Slot()
								.HAlign(HAlign_Fill)
								.VAlign(VAlign_Fill)
								[
									SNew(SVerticalBox)
									+ SVerticalBox::Slot()
									.AutoHeight()
									[

										SNew(SHorizontalBox)
										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											// POPUP ERROR MESSAGE
											SAssignNew(ErrorText, SErrorText)
											.BackgroundColor(this, &SEdNode_InstantDialogueNode::GetErrorColor)
											.ToolTipText(this, &SEdNode_InstantDialogueNode::GetErrorMsgToolTip)
										]

										+ SHorizontalBox::Slot()
										.AutoWidth()
										[
											SNew(SHorizontalBox)
											+ SHorizontalBox::Slot()
											.Padding(FMargin(4.0f, 0.0f, 4.0f, 0.0f))
											[

												SNew(SVerticalBox)
												+ SVerticalBox::Slot()
												.AutoHeight()
												[
													SAssignNew(InlineEditableText, SInlineEditableTextBlock)
													//.Style(FEditorStyle::Get(), "Graph.StateNode.NodeTitleInlineEditableText")
													.Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
													//.OnVerifyTextChanged(this, &SEdNode_InstantDialogueNode::OnVerifyNameTextChanged)
													//.OnTextCommitted(this, &SEdNode_InstantDialogueNode::OnNameTextCommited)
													//.IsReadOnly(this, &SEdNode_InstantDialogueNode::IsNameReadOnly)
													//.IsSelected(this, &SEdNode_InstantDialogueNode::IsSelectedExclusively)
												]
												
												+ SVerticalBox::Slot()
                                                .AutoHeight()
                                                [
                                                    //SNew(SRichTextBlock)
                                                    SNew(SMultiLineEditableText)
                                                	.Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
                                                	.OnTextCommitted(this, &SEdNode_InstantDialogueNode::OnNameTextCommited)
													
                                                ]
												
												+ SVerticalBox::Slot()
												.AutoHeight()
												[
													SNew(SImage)
													.Image(FEditorStyle::GetBrush("Graph.TransitionNode.Icon"))
												]

												+ SVerticalBox::Slot()
												.AutoHeight()
												[
													NodeTitle.ToSharedRef()
												]
											]
										]
									]
								]
							]
						]
					]

					// OUTPUT PIN AREA
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SBox)
						.MinDesiredHeight(NodePadding.Bottom)
						[
							SAssignNew(RightNodeBox, SVerticalBox)
							+ SVerticalBox::Slot()
							  .HAlign(HAlign_Fill)
							  .VAlign(VAlign_Fill)
							  .Padding(20.0f, 0.0f)
							  .FillHeight(1.0f)
							[
								SAssignNew(OutputPinBox, SHorizontalBox)
							]
						]
					]
				]
			]
		];*/
		

	// //----additional stuff----
	// TSharedPtr<SErrorText> ErrorText;
	// //----additional stuff----
	// 	
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
	// //SetupErrorReporting();
 //
	// TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);
	//
	// UEdNode_InstantDialogueNode* MyNode = CastChecked<UEdNode_InstantDialogueNode>(GraphNode);
 //
	// if (!MyNode->InstantDialogueNode->OnForcePropertyEditorUpdate.IsBound())
	// {
	// 	MyNode->InstantDialogueNode->OnForcePropertyEditorUpdate.AddSP(this, &SEdNode_InstantDialogueNode::UpdateGraphNode);
	// }
	//
	// const FSlateBrush* PortraitIcon = nullptr;
	// FSlateBrush MySlater;
	//
 //    //MySlater.SetResourceObject(MyNode->InstantDialogueNode->MySlateBrush.GetResourceObject()); //do it with texture2D
 //    //PortraitIcon = &MySlater;
 //    //with build in SlateBrush
 //
	// // the real one <----------
	// /*MyNode->InstantDialogueNode->MySlateBrush.SetResourceObject(MyNode->InstantDialogueNode->GetPortrait());
	// PortraitIcon = &MyNode->InstantDialogueNode->MySlateBrush;
	// */
 //
	// //SpeakerName = LOCTEXT("Speakerer", "Mr.Turtle");
	//
	// // TSharedPtr<SMultiLineEditableText> mult = SNew(SMultiLineEditableText, GraphNode); that could be maybe and option?
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
	// 				SNew(SHorizontalBox) // HorizontalBox/Header 
	// 				
	// 				+ SHorizontalBox::Slot()
	// 				.HAlign(HAlign_Right)
	// 				.VAlign(VAlign_Center)
	// 				.HAlign(HAlign_Left)
	// 				.Padding(10, 10, 10, 10)
	// 				.AutoWidth()
	// 				[
	// 					SNew(SImage)
	// 					.ColorAndOpacity(FColor::Magenta)
	// 					.Image(PortraitIcon)
	// 				]
	// 	
	// 				+ SHorizontalBox::Slot()
	// 				.HAlign(HAlign_Right)
	// 				.VAlign(VAlign_Center)
	// 				.Padding(0, 0, 5, 0)
	// 				.AutoWidth()
	// 				[
	// 						SNew(SMultiLineEditableText)
	// 						//SAssignNew(SpeakerNameField, SEditableText)
	// 						//SNew(SInlineEditableTextBlock)
	// 						//.OnTextCommitted(this, &SEdNode_InstantDialogueNode::OnSpeakerNameCommitted)
	// 						.Text(LOCTEXT("Key", "this is InstantDialogueNode")) //Use InstantDialoguenode GetSpeakerName
	// 						//.Text(MyNode->InstantDialogueNode->GetSpeakerName())//.Text(this, &SDialogueNodeWidget::GetNodeText) //.Text(this, &SDialogueNodeWidget::GetFieldText)
	// 						//.Text(SpeakerName)
	// 						// get it from edNOde, save it in ed node, get it from multiTexter,
	// 				]		
	// 				
	// 				+ SHorizontalBox::Slot()
	// 				.HAlign(HAlign_Right)
	// 				.VAlign(VAlign_Center)
	// 				.Padding(0, 0, 5, 0)
	// 				.AutoWidth()
	// 				[
	// 						SNew(SCheckBox)
	// 						.IsChecked(true)
	// 						//.IsChecked(MyNode->InstantDialogueNode->MyBoolVar)
	// 						//.OnCheckStateChanged(this, &SEdNode_InstantDialogueNode::OnSpeakerNameCommitted)
	// 				]		
	// 				
	// 				+ SHorizontalBox::Slot()
	// 				.VAlign(VAlign_Top)
	// 				.Padding(FMargin(0.f, 0.f, 4.f, 0.f))
	// 				.AutoWidth()
	// 				[
	// 					SNew(SImage)
	// 					.Image(IconBrush)
	// 					.ColorAndOpacity(this, &SGraphNode::GetNodeTitleIconColor)
	// 				]
	// 				+ SHorizontalBox::Slot()
	// 				[
	// 					SNew(SVerticalBox)
	// 					
	// 					+ SVerticalBox::Slot()
	// 					.AutoHeight()
	// 					[
	// 						//CreateTitleWidget(NodeTitle) //creates the InlIneEditableText
	// 					
	// 						SAssignNew(InlineEditableText, SInlineEditableTextBlock)
	// 						.Visibility(EVisibility::Collapsed)
 //                            //.Style(FEditorStyle::Get(), "Graph.StateNode.NodeTitleInlineEditableText")
 //                            .Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
 //                            //.OnVerifyTextChanged(this, &SEdNode_InstantDialogueNode::OnVerifyNameTextChanged)
 //                            .OnTextCommitted(this, &SEdNode_InstantDialogueNode::OnNameTextCommited)
 //                            //.IsReadOnly(this, &SEdNode_InstantDialogueNode::IsNameReadOnly)
 //                            //.IsSelected(this, &SEdNode_InstantDialogueNode::IsSelectedExclusively)
	// 						
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
	// 			CreateTitleRightWidget() ///** Optionally create a widget to dock to the right in the title bar of the node. */
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
	// 	[
	// 		CreateNodeContentArea()
	// 	];
 //
 //
	// /*// INPUT PIN AREA
	// + SVerticalBox::Slot()
	// .AutoHeight()
	// [
	// 	SNew(SBox)
	// 	.MinDesiredHeight(10.0)
	// 	[
	// 		SAssignNew(LeftNodeBox, SVerticalBox)
	// 	]
	// ];*/
 //
	//
	// 	/*// OUTPUT PIN AREA
	// 	+ SVerticalBox::Slot()
	// 	.AutoHeight()
	// 	[
	// 		SNew(SBox)
	// 		.MinDesiredHeight(1.0)
	// 		[
	// 			SAssignNew(RightNodeBox, SVerticalBox)
	// 			+ SVerticalBox::Slot()
	// 			.HAlign(HAlign_Fill)
	// 			.VAlign(VAlign_Fill)
	// 			.Padding(20.0f, 0.0f)
	// 			.FillHeight(1.0f)
	// 			[
	// 				//SAssignNew(OutputPinBox, SHorizontalBox)
	// 				SAssignNew(LeftNodeBox, SVerticalBox)
	// 			]
	// 		]
	// 	];*/
	// 	
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
	// /*InnerVerticalBox->AddSlot()
	// 	.AutoHeight()
	// 	.Padding(Settings->GetNonPinNodeBodyPadding())
	// 	[
	// 		ErrorReporting->AsWidget()
	// 	];*/
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
 //
	// 			// additional stuff here is just the error (you can delete)
	// 			+SOverlay::Slot()
	// 			[
	// 				SAssignNew(ErrorText, SErrorText)
	// 				.BackgroundColor(this, &SEdNode_InstantDialogueNode::GetErrorColor)
	// 				.ToolTipText(this, &SEdNode_InstantDialogueNode::GetErrorMsgToolTip)
	// 			]
	// 			
	// 		]			
	// 	];
	// 	
 //
	// // Create comment bubble
	// TSharedPtr<SCommentBubble> CommentBubble;
	// const FSlateColor CommentColor = GetDefault<UGraphEditorSettings>()->DefaultCommentNodeTitleColor;
 //
	// SAssignNew(CommentBubble, SCommentBubble)
	// 	.GraphNode(GraphNode)
	// 	.Text(this, &SGraphNode::GetNodeComment)
	// 	.OnTextCommitted(this, &SGraphNode::OnCommentTextCommitted)
	// 	.ColorAndOpacity(CommentColor)
	// 	.AllowPinning(true)
	// 	.EnableTitleBarBubble(true)
	// 	.EnableBubbleCtrls(true)
	// 	.GraphLOD(this, &SGraphNode::GetCurrentLOD)
	// 	.IsGraphNodeHovered(this, &SGraphNode::IsHovered);
 //
	// GetOrAddSlot(ENodeZone::TopCenter)
	// 	.SlotOffset(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetOffset))
	// 	.SlotSize(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetSize))
	// 	.AllowScaling(TAttribute<bool>(CommentBubble.Get(), &SCommentBubble::IsScalingAllowed))
	// 	.VAlign(VAlign_Top)
	// 	[
	// 		CommentBubble.ToSharedRef()
	// 	];
 //
	// // old error you could delete
	// ErrorReporting = ErrorText;
	// ErrorReporting->SetError(ErrorMsg);
	// //CreatePinWidgets(); //Creates the pin area/border
	//
	// CreateBelowWidgetControls(MainVerticalBox);
	// CreatePinWidgets();
	// CreateInputSideAddButton(LeftNodeBox);
	// CreateOutputSideAddButton(RightNodeBox);
	// CreateBelowPinControls(InnerVerticalBox);
	// CreateAdvancedViewArrow(InnerVerticalBox);

	UEdNode_InstantDialogueNode* MyNode = CastChecked<UEdNode_InstantDialogueNode>(GraphNode);
	// if (!MyNode->InstantDialogueNode->OnForcePropertyEditorUpdate.IsBound())
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("connected to the UpdateDelegate in InstantDialogueNode"));
	// 	MyNode->InstantDialogueNode->OnForcePropertyEditorUpdate.AddSP(this, &SEdNode_InstantDialogueNode::UpdateGraphNode);
	// }

	//UE_LOG(LogTemp, Warning, TEXT("All the delegate functions %s"), OnTextCommitted.TryGetBoundFunctionName());

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
			SNew(SImage) // translucent Image
			.Image( FEditorStyle::GetBrush("Graph.Node.TitleGloss") )
			.ColorAndOpacity( this, &SGraphNode::GetNodeTitleIconColor )
		]
		+SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Center)
		[
			//SNew(SHorizontalBox)
			//+ SHorizontalBox::Slot()
			//.HAlign(HAlign_Fill)
			//[
			SNew(SBorder) // color gradient part
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
					SNew(SImage) // this to the Node Title
					//.ColorAndOpacity(FColor::Magenta)
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
						.OnTextCommitted(this, &SEdNode_InstantDialogueNode::OnNameTextCommited)
					]
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						NodeTitle.ToSharedRef()
					]
				]
			]
			//]
			// + SHorizontalBox::Slot()
			// .HAlign(HAlign_Right)
			// .VAlign(VAlign_Center)
			// .Padding(0, 0, 5, 0)
			// .AutoWidth()
			// [
			// 	CreateTitleRightWidget() //* Optionally create a widget to dock to the right in the title bar of the node.
			// ]
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
			InputPin->SetShowLabel(true);
		}
	}

	for (auto OutputPin : this->OutputPins)
	{
		if (OutputPin->GetPinObj()->ParentPin == nullptr)
		{
			OutputPin->SetShowLabel(true);
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

void SEdNode_InstantDialogueNode::CreatePinWidgets()
{
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

void SEdNode_InstantDialogueNode::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
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
}



// ---------------------- additional area start ----------------------
/*void SEdNode_InstantDialogueNode::OnDialogueCommitted(const FText& InText, ETextCommit::Type CommitInfo)
{
	UEdNode_InstantDialogueNode* MyNode = CastChecked<UEdNode_InstantDialogueNode>(GraphNode);
	if (MyNode != nullptr && MyNode->InstantDialogueNode != nullptr)
	{
		const FScopedTransaction Transaction(LOCTEXT("InstantDialogueEditorSetDialogueNode", "Instant Dialogue Editor: Set Dialogue"));
		MyNode->Modify();
		MyNode->InstantDialogueNode->Modify();
		//MyNode->InstantDialogueNode->SetDialogue(InText);
		UpdateGraphNode();
	}
}*/

TSharedRef<SWidget> SEdNode_InstantDialogueNode::CreateNodeContentArea()
{
	UEdNode_InstantDialogueNode* MyNode = CastChecked<UEdNode_InstantDialogueNode>(GraphNode);
	
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
				//.OnTextCommitted(this, &SEdNode_InstantDialogueNode::OnDialogueCommitted)
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

void SEdNode_InstantDialogueNode::CreateOutputSideAddButton(TSharedPtr<SVerticalBox> OutputBox)
{
	const TSharedRef<SWidget> AddPinButton = AddPinButtonContent(
		LOCTEXT("FlowNodeAddPinButton", "Add pin"),
		LOCTEXT("FlowNodeAddPinButton_Tooltip", "Adds an output pin")
	);

	FMargin AddPinPadding = Settings->GetOutputPinPadding();
	AddPinPadding.Top += 6.0f;

	OutputBox->AddSlot()
		.AutoHeight()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Right)
		.Padding(AddPinPadding)
		[
			AddPinButton
		];
}

FReply SEdNode_InstantDialogueNode::OnAddPin()
{
	UEdNode_InstantDialogueNode* MyGraphNode = CastChecked<UEdNode_InstantDialogueNode>(GraphNode);

	const FScopedTransaction Transaction( LOCTEXT("AddExecutionPin", "Add Execution Pin"));
	MyGraphNode->Modify();
	
	MyGraphNode->AddPinToInstantDialogueNode(MyGraphNode->Pins.Num());

	UpdateGraphNode();
	
	GraphNode->GetGraph()->NotifyGraphChanged();
	
	return FReply::Handled();
}

void SEdNode_InstantDialogueNode::SetOwner( const TSharedRef<SGraphPanel>& OwnerPanel )
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

// get called in CreatePinWidgets, creates all the Pins which are currently on the Pin (not just the default Input/Output Pin)
void SEdNode_InstantDialogueNode::CreateStandardPinWidget(UEdGraphPin* CurPin)
{
	const bool bShowPin = ShouldPinBeHidden(CurPin);

	if (bShowPin)
	{
		TSharedPtr<SGraphPin> NewPin = CreatePinWidget(CurPin);
		check(NewPin.IsValid());
		this->AddPin(NewPin.ToSharedRef());
	}
}

TSharedPtr<SGraphPin> SEdNode_InstantDialogueNode::CreatePinWidget(UEdGraphPin* Pin) const
{
	//return FNodeFactory::CreatePinWidget(Pin);
	
	//Pin->GetName();
	//Pin->PinId;
	//return SNew(SGraphPinExecLabel, Pin);
	return SNew(SGraphPin, Pin);
	//return SNew(SGraphPinExec, Pin);
	//return SNew(SInstantDialoguePinExec, Pin);
}

/*bool SEdNode_InstantDialogueNode::IsNameReadOnly() const
{
	UEdNode_InstantDialogueNode* EdNode_Node = Cast<UEdNode_InstantDialogueNode>(GraphNode);
	check(EdNode_Node != nullptr);

	UInstantDialogue* InstantDialogue = EdNode_Node->InstantDialogueNode->Graph;
	check(InstantDialogue != nullptr);

	return !InstantDialogue->bCanRenameNode || SGraphNode::IsNameReadOnly();
}*/

FSlateColor SEdNode_InstantDialogueNode::GetBorderBackgroundColor() const
{
	UEdNode_InstantDialogueNode* MyNode = CastChecked<UEdNode_InstantDialogueNode>(GraphNode);
	return MyNode ? MyNode->GetBackgroundColor() : InstantDialogueColors::NodeBorder::HighlightAbortRange0;
}

FSlateColor SEdNode_InstantDialogueNode::GetBackgroundColor() const
{
	return InstantDialogueColors::NodeBody::Default;
}

EVisibility SEdNode_InstantDialogueNode::GetDragOverMarkerVisibility() const
{
	return EVisibility::Visible;
}

const FSlateBrush* SEdNode_InstantDialogueNode::GetNameIcon() const
{
	return FEditorStyle::GetBrush(TEXT("BTEditor.Graph.BTNode.Icon"));
}

#undef LOCTEXT_NAMESPACE
