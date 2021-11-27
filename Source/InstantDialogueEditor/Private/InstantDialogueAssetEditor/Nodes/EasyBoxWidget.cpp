// Fill out your copyright notice in the Description page of Project Settings.


#include "EasyBoxWidget.h"

// #include "InstantDialogue.h"
//
// //// all paste
// #include "Modules/ModuleManager.h"
// #include "Widgets/SBoxPanel.h"
// #include "Widgets/Images/SImage.h"
// #include "Widgets/Layout/SBox.h"
// #include "Widgets/Input/SEditableTextBox.h"
// #include "Widgets/Input/SButton.h"
// #include "Widgets/Input/SComboButton.h"
// #include "Editor.h"
// #include "IContentBrowserSingleton.h"
// #include "ConditionObject.h"
// #include "ContentBrowserModule.h"
// #include "ScopedTransaction.h"
// #include "Engine/Selection.h"
// #include "AssetRegistryModule.h"
// #include "ClassViewerFilter.h"

#define LOCTEXT_NAMESPACE "SEasyBox"

// class FPropertyEditorClassFilter1 : public IClassViewerFilter
// {
// public:
// 	/** The meta class for the property that classes must be a child-of. */
// 	const UClass* ClassPropertyMetaClass;
//
// 	/** The interface that must be implemented. */
// 	const UClass* InterfaceThatMustBeImplemented;
//
// 	/** Whether or not abstract classes are allowed. */
// 	bool bAllowAbstract;
//
// 	/** Classes that can be picked */
// 	TArray<const UClass*> AllowedClassFilters;
//
// 	/** Classes that can't be picked */
// 	TArray<const UClass*> DisallowedClassFilters;
//
// 	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs ) override
// 	{
// 		return IsClassAllowedHelper(InClass);
// 	}
// 	
// 	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef< const IUnloadedBlueprintData > InBlueprint, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
// 	{
// 		return IsClassAllowedHelper(InBlueprint);
// 	}
//
// private:
//
// 	template <typename TClass>
// 	bool IsClassAllowedHelper(TClass InClass)
// 	{
// 		bool bMatchesFlags = !InClass->HasAnyClassFlags(CLASS_Hidden | CLASS_HideDropDown | CLASS_Deprecated) &&
// 			(bAllowAbstract || !InClass->HasAnyClassFlags(CLASS_Abstract));
//
// 		if (bMatchesFlags && InClass->IsChildOf(ClassPropertyMetaClass)
// 			&& (!InterfaceThatMustBeImplemented || InClass->ImplementsInterface(InterfaceThatMustBeImplemented)))
// 		{
// 			auto PredicateFn = [InClass](const UClass* Class)
// 			{
// 				return InClass->IsChildOf(Class);
// 			};
//
// 			if (DisallowedClassFilters.FindByPredicate(PredicateFn) == nullptr &&
// 				(AllowedClassFilters.Num() == 0 || AllowedClassFilters.FindByPredicate(PredicateFn) != nullptr))
// 			{
// 				return true;
// 			}
// 		}
//
// 		return false;
// 	}
// };
//
// class FInstantConditionFilterViewer1 : public IClassViewerFilter
// {
// public:
// 	TSet<const UClass*> AllowedChildrenOfClasses;
// 	//uint32 DisallowedClassFlags;
// 	EClassFlags DisallowedClassFlags;
// 	
// 	//virtual ~FInstantConditionFilterViewer() override;
// 	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass,
// 		TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
// 	{
// 		return InClass->IsChildOf(UInstantDialogueConditionObject::StaticClass());
// 	}
// 	
// 	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions,
// 		const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData,
// 		TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
// 	{
// 		return !InUnloadedClassData->HasAnyClassFlags(DisallowedClassFlags)
// 		&& InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InUnloadedClassData) != EFilterReturn::Failed;
// 	}
// };
//
// void SEasyBoxWidget::Construct(const FArguments& InArgs) //const FArguments& InArgs
// {
// 		// copy from above
// 		bAllowAbstract = false;
// 		bAllowOnlyPlaceable = false; //true
// 		bIsBlueprintBaseOnly = false;
// 		RequiredInterface = nullptr;
// 		bAllowNone = true;
// 		bShowViewOptions = true;
// 		bShowTree = false; //false
// 		bShowDisplayNames = true;
//
// 		//extras
// 		MetaClass = UInstantDialogueConditionObject::StaticClass();
// 		AllowedClassFilters.Empty();
// 		DisallowedClassFilters.Empty();
// 		OnSetClass = nullptr;
// 		SelectedClass = nullptr;
// 	
// 		CreateClassFilter();
//
// 	SNew(SOverlay)
// 	+SOverlay::Slot()
// 	.VAlign(VAlign_Center)
// 	.HAlign(HAlign_Center)
// 	[
// 		SAssignNew(ComboButton, SComboButton)
// 			.OnGetMenuContent(this, &SEasyBoxWidget::GenerateClassPicker)
// 			.ContentPadding(FMargin(2.0f, 2.0f))
// 			.ToolTipText(this, &SEasyBoxWidget::GetDisplayValueAsString)
// 			.ButtonContent()
// 			[
// 				SNew(STextBlock)
// 				.Text(this, &SEasyBoxWidget::GetDisplayValueAsString)
// 			]
// 		];
// }
//
// void SEasyBoxWidget::CreateClassFilter()
// {
// 	ClassViewerOptions.bShowBackgroundBorder = false;
// 	ClassViewerOptions.bShowUnloadedBlueprints = true;
// 	ClassViewerOptions.bShowNoneOption = bAllowNone;
//
// 	// FClassViewerInitializationOptions
// 	ClassViewerOptions.bIsBlueprintBaseOnly = bIsBlueprintBaseOnly;
// 	ClassViewerOptions.bIsPlaceableOnly = bAllowOnlyPlaceable;
// 	ClassViewerOptions.NameTypeToDisplay = (bShowDisplayNames ? EClassViewerNameTypeToDisplay::DisplayName : EClassViewerNameTypeToDisplay::ClassName);
// 	ClassViewerOptions.DisplayMode = bShowTree ? EClassViewerDisplayMode::TreeView : EClassViewerDisplayMode::ListView;
// 	ClassViewerOptions.bAllowViewOptions = bShowViewOptions;
//
//
// 	///// my Filter
// 	// TSharedPtr<FInstantConditionFilterViewer> InstantConditionFilterViewer = MakeShareable(new FInstantConditionFilterViewer);
// 	// InstantConditionFilterViewer->AllowedChildrenOfClasses.Add(UObject::StaticClass());
// 	// ClassViewerOptions.ClassFilter = InstantConditionFilterViewer;
// 	//
// 	// ClassFilter = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer").CreateClassFilter(ClassViewerOptions);
// 	// ClassFilterFuncs = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer").CreateFilterFuncs();
// 	////
// 	
// 	TSharedPtr<FPropertyEditorClassFilter1> PropEdClassFilter = MakeShareable(new FPropertyEditorClassFilter1);
// 	ClassViewerOptions.ClassFilter = PropEdClassFilter;
//
// 	PropEdClassFilter->ClassPropertyMetaClass = MetaClass;
// 	PropEdClassFilter->InterfaceThatMustBeImplemented = RequiredInterface;
// 	PropEdClassFilter->bAllowAbstract = bAllowAbstract;
// 	PropEdClassFilter->AllowedClassFilters = AllowedClassFilters;
// 	PropEdClassFilter->DisallowedClassFilters = DisallowedClassFilters;
//
// 	ClassFilter = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer").CreateClassFilter(ClassViewerOptions);
// 	ClassFilterFuncs = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer").CreateFilterFuncs();
// }
//
// TSharedRef<SWidget> SEasyBoxWidget::GenerateClassPicker()
// {
// 	FOnClassPicked OnPicked(FOnClassPicked::CreateRaw(this, &SEasyBoxWidget::OnClassPicked));
//
// 	return SNew(SBox)
// 		.WidthOverride(280)
// 		[
// 			SNew(SVerticalBox)
// 			+SVerticalBox::Slot()
// 			.AutoHeight()
// 			.MaxHeight(500)
// 			[
// 				FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer").CreateClassViewer(ClassViewerOptions, OnPicked)
// 			]			
// 		];
// }
//
// void SEasyBoxWidget::OnClassPicked(UClass* InClass)
// {
// 	SelectedClass = InClass;
// 	ComboButton->SetIsOpen(false);
// }
//
// static FString GetClassDisplayName1(const UObject* Object, bool bShowDisplayNames)
// {
// 	const UClass* Class = Cast<UClass>(Object);
// 	if (Class != NULL)
// 	{
// 		UBlueprint* BP = UBlueprint::GetBlueprintFromClass(Class);
// 		if(BP != NULL)
// 		{
// 			return BP->GetName();
// 		}
// 		if (bShowDisplayNames && Class->HasMetaData(TEXT("DisplayName")))
// 		{
// 			return Class->GetMetaData(TEXT("DisplayName"));
// 		}
// 	}
// 	return (Object) ? Object->GetName() : "None";
// }
//
// FText SEasyBoxWidget::GetDisplayValueAsString() const
// {
// 	static bool bIsReentrant = false;
//
// 	// Guard against re-entrancy which can happen if the delegate executed below (SelectedClass.Get()) forces a slow task dialog to open, thus causing this to lose context and regain focus later starting the loop over again
// 	if( !bIsReentrant )
// 	{
// 		TGuardValue<bool>( bIsReentrant, true );
// 		return FText::FromString(GetClassDisplayName1(SelectedClass.Get(), bShowDisplayNames));
// 	}
// 	else
// 	{
// 		return FText::GetEmpty();
// 	}
// 	
// }

#undef LOCTEXT_NAMESPACE