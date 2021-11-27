#include "MyPushyButton.h"

#include "ConditionObject.h"
#include "Engine/Blueprint.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Layout/SBox.h"

#include "UObject/Class.h"

class FPropertyEditorClassFilter3 : public IClassViewerFilter
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

void SMyPushyButton::Construct(const FArguments& InArgs)
{

	OnMenuClosed1 = InArgs._OnMenuClosed;
	
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
	//MetaClass = nullptr;
	AllowedClassFilters.Empty();
	DisallowedClassFilters.Empty();
	//OnSetClass= nullptr;

	//UEdNode_Condition* MyNode = CastChecked<UEdNode_Condition>(GraphNode);
	//UInstantDialogueNodeTest* MyTexter = CastChecked<UInstantDialogueNodeTest>(MyNode->InstantDialogueNode);

	// if (MyTexter->ConditionsArray.Num() > index)
	// {
	// 	SelectedClass = MyTexter->ConditionsArray[index];
	// 		
	// }
	// else
	// {
	// 	SelectedClass = nullptr;
	// }
	SelectedClass = nullptr;
	
	CreateClassFilter();

	// should that be SCombobox:Construct?
	SMenuAnchor::Construct(SMenuAnchor::FArguments()
		.Placement(InArgs._MenuPlacement)
		.Method(InArgs._Method)
		.OnMenuOpenChanged(InArgs._OnMenuOpenChanged)
		.OnGetMenuContent(InArgs._OnGetMenuContent)
		.IsCollapsedByParent(InArgs._CollapseMenuOnParentFocus)
		[
			//old stuff
			SAssignNew(ComboButton, SComboButton)
				.OnGetMenuContent(this, &SMyPushyButton::GenerateClassPicker)
				.ContentPadding(FMargin(2.0f, 2.0f))
				.ToolTipText(this, &SMyPushyButton::GetDisplayValueAsString)
				.ButtonContent()
				[
					SNew(STextBlock)
					.Text(this, &SMyPushyButton::GetDisplayValueAsString)
				]//old end
		]
	);
}

void SMyPushyButton::CreateClassFilter()
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
	
	TSharedPtr<FPropertyEditorClassFilter3> PropEdClassFilter = MakeShareable(new FPropertyEditorClassFilter3);
	ClassViewerOptions.ClassFilter = PropEdClassFilter;

	PropEdClassFilter->ClassPropertyMetaClass = MetaClass;
	PropEdClassFilter->InterfaceThatMustBeImplemented = RequiredInterface;
	PropEdClassFilter->bAllowAbstract = bAllowAbstract;
	PropEdClassFilter->AllowedClassFilters = AllowedClassFilters;
	PropEdClassFilter->DisallowedClassFilters = DisallowedClassFilters;

	ClassFilter = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer").CreateClassFilter(ClassViewerOptions);
	ClassFilterFuncs = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer").CreateFilterFuncs();
}

TSharedRef<SWidget> SMyPushyButton::GenerateClassPicker()
{
	FOnClassPicked OnPicked(FOnClassPicked::CreateRaw(this, &SMyPushyButton::OnClassPicked));

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

void SMyPushyButton::OnClassPicked(UClass* InClass)
{
	SelectedClass = InClass;

	if (ComboButton->IsOpen() & OnMenuClosed1.IsBound())
	{
		//OnMenuClosed1.ExecuteIfBound();
		OnMenuClosed1.Execute(InClass);
	}

	
	
	//FOnClassPicked OnPicked(FOnClassPicked::CreateRaw(this, &SEdNode_Text::OnClassPicked));
	
	ComboButton->SetIsOpen(false);
}

//helper functions
static FString GetClassDisplayName3(const UObject* Object, bool bShowDisplayNames)
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

FText SMyPushyButton::GetDisplayValueAsString() const
{
	static bool bIsReentrant = false;

	// Guard against re-entrancy which can happen if the delegate executed below (SelectedClass.Get()) forces a slow task dialog to open, thus causing this to lose context and regain focus later starting the loop over again
	if( !bIsReentrant )
	{
		TGuardValue<bool>( bIsReentrant, true );
		return FText::FromString(GetClassDisplayName3(SelectedClass.Get(), bShowDisplayNames));
	}
	else
	{
		return FText::GetEmpty();
	}
}


