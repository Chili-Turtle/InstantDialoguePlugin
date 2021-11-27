// Fill out your copyright notice in the Description page of Project Settings.


#include "SedNode_InstantDialogueCondition.h"

#include "EasyBoxWidget.h"
#include "EdNode_InstantDialogueCondition.h"
#include "Nodes/InstantDialogueNodeTest.h"
#include "GraphEditorSettings.h"
#include "IDocumentation.h"
#include "SCommentBubble.h"
#include "SLevelOfDetailBranchNode.h"
#include "SGraphPanel.h"
#include "SlateOptMacros.h"
#include "TutorialMetaData.h"
#include "SEdNode_InstantDialogueNode.h"

#include "KismetPins/SGraphPinString.h"
#include "KismetPins/SGraphPinObject.h"

/// debugger
#include "Widgets/SWidget.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Layout/SWrapBox.h"
#include "Widgets/Layout/SBox.h"
//#include "Widgets/Input/SButton.h"
#include "ARFilter.h"
#include "Widgets/Input/SComboButton.h"


#define LOCTEXT_NAMESPACE "SedNodeCondition"

class SEdNode_InstantDialogueNode;
void SedNode_Condition::Construct(const FArguments& InArgs, UEdNode_InstantDialogueNode* InNode)
{
	GraphNode = InNode;
	UpdateGraphNode();
	InNode->SEdNode = this;
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SedNode_Condition::UpdateGraphNode()
{
	UEdNode_Condition* MyNode = CastChecked<UEdNode_Condition>(GraphNode);

	//MyNode->InstantDialogueNode->OnForcePropertyEditorUpdate.RemoveAll(this);
	//MyNode->InstantDialogueNode->OnForcePropertyEditorUpdate.AddSP(this, &SedNode_Condition::UpdateGraphNode);

	InputPins.Empty();
	OutputPins.Empty();
	//MultiBoxArray.Empty();
	ConditionArray.Empty();
	PushyButtons.Empty();
	
	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	//
	//             ______________________
	//            |      TITLE AREA      |
	//            +-------+------+-------+
	//            | (>) L |      | R (>) |
	//            | (>) E |      | I (>) |
	//            | (>) F |      | G (>) |
	//            | (>) T |      | H (>) |
	//            |       |      | T (>) |
	//            |_______|______|_______|
	//
	TSharedPtr<SVerticalBox> MainVerticalBox;
	SetupErrorReporting();

	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

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
							CreateTitleWidget(NodeTitle)
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
				CreateTitleRightWidget()
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

	SetDefaultTitleAreaWidget(DefaultTitleAreaWidget);

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

	
	if (!SWidget::GetToolTip().IsValid())
	{
		TSharedRef<SToolTip> DefaultToolTip = IDocumentation::Get()->CreateToolTip( TAttribute< FText >( this, &SGraphNode::GetNodeTooltip ), NULL, GraphNode->GetDocumentationLink(), GraphNode->GetDocumentationExcerptName() );
		SetToolTip(DefaultToolTip);
	}

	// Setup a meta tag for this node
	FGraphNodeMetaData TagMeta(TEXT("Graphnode"));
	PopulateMetaTag(&TagMeta);
	
	TSharedPtr<SVerticalBox> InnerVerticalBox;
	this->ContentScale.Bind( this, &SGraphNode::GetContentScale );


	InnerVerticalBox = SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Top)
		.Padding(Settings->GetNonPinNodeBodyPadding())
		[
			TitleAreaWidget
		]

		+SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Top)
		[
			CreateNodeContentArea()
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
		.Padding(Settings->GetNonPinNodeBodyPadding())
		[
			ErrorReporting->AsWidget()
		];



	this->GetOrAddSlot( ENodeZone::Center )
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SAssignNew(MainVerticalBox, SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SOverlay)
				.AddMetaData<FGraphNodeMetaData>(TagMeta)
				+SOverlay::Slot()
				.Padding(Settings->GetNonPinNodeBodyPadding())
				[
					SNew(SImage)
					.Image(GetNodeBodyBrush())
					.ColorAndOpacity(this, &SGraphNode::GetNodeBodyColor)
				]
				+SOverlay::Slot()
				[
					InnerVerticalBox.ToSharedRef()
				]
			]			
		];

	bool SupportsBubble = true;
	if (GraphNode != nullptr)
	{
		SupportsBubble = GraphNode->SupportsCommentBubble();
	}

	if (SupportsBubble)
	{
		// Create comment bubble
		TSharedPtr<SCommentBubble> CommentBubble;
		const FSlateColor CommentColor = GetDefault<UGraphEditorSettings>()->DefaultCommentNodeTitleColor;

		SAssignNew(CommentBubble, SCommentBubble)
			.GraphNode(GraphNode)
			.Text(this, &SGraphNode::GetNodeComment)
			.OnTextCommitted(this, &SGraphNode::OnCommentTextCommitted)
			.OnToggled(this, &SGraphNode::OnCommentBubbleToggled)
			.ColorAndOpacity(CommentColor)
			.AllowPinning(true)
			.EnableTitleBarBubble(true)
			.EnableBubbleCtrls(true)
			.GraphLOD(this, &SGraphNode::GetCurrentLOD)
			.IsGraphNodeHovered(this, &SGraphNode::IsHovered);

		GetOrAddSlot(ENodeZone::TopCenter)
			.SlotOffset(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetOffset))
			.SlotSize(TAttribute<FVector2D>(CommentBubble.Get(), &SCommentBubble::GetSize))
			.AllowScaling(TAttribute<bool>(CommentBubble.Get(), &SCommentBubble::IsScalingAllowed))
			.VAlign(VAlign_Top)
			[
				CommentBubble.ToSharedRef()
			];
	}

	CreateBelowWidgetControls(MainVerticalBox);
	CreatePinWidgets();
	CreateInputSideAddButton(LeftNodeBox);
	CreateOutputSideAddButton(RightNodeBox);
	CreateBelowPinControls(InnerVerticalBox);
	CreateAdvancedViewArrow(InnerVerticalBox);

	//Hide pin labels
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
	
	UInstantDialogueNodeTest* MyNodeAnswer = CastChecked<UInstantDialogueNodeTest>(MyNode->InstantDialogueNode);
	for (int i = 0; i < ConditionArray.Num(); ++i)
	{
		if (MyNodeAnswer->ConditionsArray.IsValidIndex(i))
		{
			ConditionArray[i] = MyNodeAnswer->ConditionsArray[i];
			PushyButtons[i].Get()->SelectedClass = MyNodeAnswer->ConditionsArray[i];
		}
	}

	
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SedNode_Condition::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));

	TSharedPtr<SMyPushyButton> MyPushyButton;

    	const UEdGraphPin* PinObj = PinToAdd->GetPinObj();
    	const bool bAdvancedParameter = (PinObj != nullptr) && PinObj->bAdvancedView;
    	if (bAdvancedParameter)
    	{
    		PinToAdd->SetVisibility( TAttribute<EVisibility>(PinToAdd, &SGraphPin::IsPinVisibleAsAdvanced) );
    	}
    
    	if (PinToAdd->GetDirection() == EEdGraphPinDirection::EGPD_Input)
    	{
    		LeftNodeBox->AddSlot()
    			.AutoHeight()
    			.HAlign(HAlign_Left)
    			.VAlign(VAlign_Center)
    			.Padding(Settings->GetInputPinPadding())
    		[
    			PinToAdd
    		];
    		InputPins.Add(PinToAdd);
    	}
    	else // Direction == EEdGraphPinDirection::EGPD_Output
    	{
    		RightNodeBox->AddSlot()
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
			 		//SAssignNew(ConditionBoxVar, SOverlay)
			 		//ConditionBox(ConditionArray.Num())
			 		SAssignNew(MyPushyButton, SMyPushyButton)
			 		.OnMenuClosed(this, &SedNode_Condition::OnConditionCommited, PushyButtons.Num()) //PushyButtons.Num() //because I rebuild it after I picked a Condition, the Condition is send to the Runtime module
			 		//.myStinky(this, &SedNode_Condition::OnConditionCommited1)
			 		
			 		//.OnGetMenuContent

			 		// answer node one -->d
			 		//.OnTextCommitted(this, &SedNode_Anwser::OnAnswerTextCommitted, OutputTextBoxes.Num())
			 		
			 	]
				+ SHorizontalBox::Slot()
				[
					PinToAdd	
				]
		 ];
    		OutputPins.Add(PinToAdd);
    		//MultiBoxArray.Emplace(MultiBox);
    		ConditionArray.Emplace();
    		PushyButtons.Emplace(MyPushyButton);
    		//OutputTextBoxes.Add(OutputTextBox);
		}

}

TSharedRef<SWidget> SedNode_Condition::CreateNodeContentArea()
{
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
			.FillWidth(1.0f)
			[
				// LEFT
				SAssignNew(LeftNodeBox, SVerticalBox)
			]

			////// new Stuff
			+SHorizontalBox::Slot()
			.AutoWidth()
			.HAlign(HAlign_Right)
			[
				SNew(SHorizontalBox)
				// +SHorizontalBox::Slot()
				// .AutoWidth()
				// .HAlign(HAlign_Right)
				// [
				// 	SNew(SVerticalBox)
				// 	+SVerticalBox::Slot()
				// 	.AutoHeight()
				// 	.HAlign(HAlign_Right)
				// 	[
				// 		ConditionBox()
				// 	]
				// ]
				
				+SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Right)
				[
					// RIGHT
					SAssignNew(RightNodeBox, SVerticalBox)
				]
			]
			// /// new Stuff end
			
			// +SHorizontalBox::Slot()
			// .AutoWidth()
			// .HAlign(HAlign_Right)
			// [
			// 	// RIGHT
			// 	SAssignNew(RightNodeBox, SVerticalBox)
			// ]
		];
}

TSharedPtr<SGraphPin> SedNode_Condition::CreatePinWidget(UEdGraphPin* Pin) const
{
	return SNew(SGraphPin, Pin);
	//return SNew(SGraphPinClass, Pin);
}

void SedNode_Condition::CreateStandardPinWidget(UEdGraphPin* CurPin)
{
	const bool bShowPin = ShouldPinBeHidden(CurPin);

	if (bShowPin)
	{
		TSharedPtr<SGraphPin> NewPin = CreatePinWidget(CurPin);
		//NewPin->SetShowLabel(false);
		check(NewPin.IsValid());

		this->AddPin(NewPin.ToSharedRef());
	}
}


///////// condtion
class FPropertyEditorClassFilter2 : public IClassViewerFilter
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

class FInstantConditionFilterViewer2 : public IClassViewerFilter
{
public:
	TSet<const UClass*> AllowedChildrenOfClasses;
	//uint32 DisallowedClassFlags;
	EClassFlags DisallowedClassFlags;
	
	//virtual ~FInstantConditionFilterViewer() override;
	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass,
		TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
	{
		return InClass->IsChildOf(UInstantDialogueConditionObject::StaticClass());
	}
	
	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions,
		const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData,
		TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
	{
		return !InUnloadedClassData->HasAnyClassFlags(DisallowedClassFlags)
		&& InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InUnloadedClassData) != EFilterReturn::Failed;
	}
};

void SedNode_Condition::OnConditionCommited1() //UClass* CondtionClass, int OutputIndex
{
}

void SedNode_Condition::OnConditionCommited(UClass* CondtionClass, int OutputIndex) //, int OutputIndex
{
	UEdNode_Condition* MyNode = CastChecked<UEdNode_Condition>(GraphNode);
	UInstantDialogueNodeTest* MyTexter = CastChecked<UInstantDialogueNodeTest>(MyNode->InstantDialogueNode);
	
	if (MyNode != nullptr && MyNode->InstantDialogueNode != nullptr)
	{
		const FScopedTransaction Transaction(LOCTEXT("InstantDialogueEditorSetDialogueNode", "Instant Dialogue Editor: Set Dialogue"));
		MyNode->Modify();
		MyTexter->Modify();
		//MyTexter->ConditionsArray[PushyButtons.Num()-1] = CondtionClass;
		//UE_LOG(LogTemp, Warning, TEXT("conditionCommitted"))
		if (MyTexter->ConditionsArray.Num() > OutputIndex)
		{
			MyTexter->ConditionsArray[OutputIndex] = CondtionClass;
			UE_LOG(LogTemp, Warning, TEXT("conditionCommitted %d"), OutputIndex);
		}
		//UpdateGraphNode(); //crashes, don't know why you dont need that, this time
		
	}
}

TSharedRef<SOverlay> SedNode_Condition::ConditionBox(int index = -1) //const FArguments& InArgs
{
		// copy from above
		bAllowAbstract = false;
		bAllowOnlyPlaceable = false; //true
		bIsBlueprintBaseOnly = false;
		RequiredInterface = nullptr;
		bAllowNone = true;
		bShowViewOptions = true;
		bShowTree = false; //false
		bShowDisplayNames = true;

		//extras
		MetaClass = UInstantDialogueConditionObject::StaticClass();
		AllowedClassFilters.Empty();
		DisallowedClassFilters.Empty();
		OnSetClass= nullptr;

		UEdNode_Condition* MyNode = CastChecked<UEdNode_Condition>(GraphNode);
		UInstantDialogueNodeTest* MyTexter = CastChecked<UInstantDialogueNodeTest>(MyNode->InstantDialogueNode);

		if (MyTexter->ConditionsArray.Num() > index)
		{
			SelectedClass = MyTexter->ConditionsArray[index];
			
		}
		else
		{
			SelectedClass = nullptr;
		}

		//UClass* Class = Cast<UClass>(&SelectedClass);
		//OnConditionCommited(Class, index);
	
		//OnConditionCommited(MyTexter->ConditionsArray[index], index);

		//FCondtionCommitted myMultDel;
		//myMultDel.AddSP(this, &SedNode_Condition::OnConditionCommited);

		//FCondtionCommitted CommitCondition(FCondtionCommitted::AddSP(this, &SedNode_Condition::OnConditionCommited));
		
		CreateClassFilter();

	return
	SNew(SOverlay)
	+SOverlay::Slot()
	.VAlign(VAlign_Center)
	.HAlign(HAlign_Center)
	[
		SAssignNew(ComboButton, SComboButton)
			.OnGetMenuContent(this, &SedNode_Condition::GenerateClassPicker)
			.ContentPadding(FMargin(2.0f, 2.0f))
			.ToolTipText(this, &SedNode_Condition::GetDisplayValueAsString)
			.ButtonContent()
			[
				SNew(STextBlock)
				.Text(this, &SedNode_Condition::GetDisplayValueAsString)
			]
		];
}

void SedNode_Condition::CreateClassFilter()
{
	ClassViewerOptions.bShowBackgroundBorder = false;
	ClassViewerOptions.bShowUnloadedBlueprints = true;
	ClassViewerOptions.bShowNoneOption = bAllowNone;

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
	
	TSharedPtr<FPropertyEditorClassFilter2> PropEdClassFilter = MakeShareable(new FPropertyEditorClassFilter2);
	ClassViewerOptions.ClassFilter = PropEdClassFilter;

	PropEdClassFilter->ClassPropertyMetaClass = MetaClass;
	PropEdClassFilter->InterfaceThatMustBeImplemented = RequiredInterface;
	PropEdClassFilter->bAllowAbstract = bAllowAbstract;
	PropEdClassFilter->AllowedClassFilters = AllowedClassFilters;
	PropEdClassFilter->DisallowedClassFilters = DisallowedClassFilters;

	ClassFilter = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer").CreateClassFilter(ClassViewerOptions);
	ClassFilterFuncs = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer").CreateFilterFuncs();
}

TSharedRef<SWidget> SedNode_Condition::GenerateClassPicker()
{
	FOnClassPicked OnPicked(FOnClassPicked::CreateRaw(this, &SedNode_Condition::OnClassPicked));

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

void SedNode_Condition::OnClassPicked(UClass* InClass)
{
	SelectedClass = InClass;
	//OnConditionCommited(InClass, ConditionArray.Num()-1);
	ComboButton->SetIsOpen(false);
}

static FString GetClassDisplayName2(const UObject* Object, bool bShowDisplayNames)
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

FText SedNode_Condition::GetDisplayValueAsString() const
{
	static bool bIsReentrant = false;

	// Guard against re-entrancy which can happen if the delegate executed below (SelectedClass.Get()) forces a slow task dialog to open, thus causing this to lose context and regain focus later starting the loop over again
	if( !bIsReentrant )
	{
		TGuardValue<bool>( bIsReentrant, true );
		return FText::FromString(GetClassDisplayName2(SelectedClass.Get(), bShowDisplayNames));
	}
	else
	{
		return FText::GetEmpty();
	}
	
}

#undef LOCTEXT_NAMESPACE
