#include "Nodes/InstantDialogueNode.h"
#include "InstantDialogueGraph.h"

#define LOCTEXT_NAMESPACE "InstantDialogueNode"

UInstantDialogueNode::UInstantDialogueNode()
{
//#if WITH_EDITORONLY_DATA
	CompatibleGraphType = UInstantDialogue::StaticClass();

	BackgroundColor = FLinearColor::Black;
//#endif
}

UInstantDialogueNode::~UInstantDialogueNode()
{

}

UInstantDialogueEdge* UInstantDialogueNode::GetEdge(UInstantDialogueNode* ChildNode)
{
	return Edges.Contains(ChildNode) ? Edges.FindChecked(ChildNode) : nullptr;
}

FText UInstantDialogueNode::GetDescription_Implementation() const //change standart name here
{
	return LOCTEXT("NodeDesc", "Instant Dialogue Template Node");
}

/*TMap<uint8, FInstantDialoguePinRecord> UInstantDialogueGraphNode::GetWireRecords() const
{
	TMap<uint8, FInstantDialoguePinRecord> Result;
	for (const TPair<FName, TArray<FInstantDialoguePinRecord>>& Record : OutputRecords)
	{
		Result.Emplace(OutputPins.IndexOfByKey(Record.Key), Record.Value.Last());
	}
	return Result;
}*/

//#if WITH_EDITOR

FLinearColor UInstantDialogueNode::GetBackgroundColor() const
{
	return BackgroundColor;
}

#if IF_WITH_EDITORONLY_DATA
void UInstantDialogueNode::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FString s = FString(PropertyChangedEvent.GetPropertyName().ToString());
	UE_LOG(LogTemp, Warning, TEXT("PostEditChange %s"), &s);
	//PropertyChangedEvent
	Super::PostEditChangeProperty(PropertyChangedEvent); // do I need the super?
	
	UE_LOG(LogTemp, Warning, TEXT("PostEditChange %s"), &PropertyChangedEvent.ChangeType);
	
	//OnForcePropertyEditorUpdate.Broadcast();

	
	//FCoreUObjectDelegates::OnObjectPropertyChanged.Broadcast(this, PropertyChangedEvent)
	//CastChecked<UEdNode_InstantDialogueNode>(Graph->EdGraph->node);
	//CastChecked<UEdNode_InstantDialogueNode>(GraphNode);
}
#endif

FText UInstantDialogueNode::GetNodeTitle() const
{
	return NodeTitle.IsEmpty() ? GetDescription() : NodeTitle;
}

void UInstantDialogueNode::SetNodeTitle(const FText& NewTitle)
{
	NodeTitle = NewTitle; //when init/renamed sets the node name
}

bool UInstantDialogueNode::CanCreateConnection(UInstantDialogueNode* Other, FText& ErrorMessage)
{
	return true;
}

//#endif

/*bool UInstantDialogueNode::IsLeafNode() const
{
	return ChildrenNodes.Num() == 0;
}*/

UInstantDialogue* UInstantDialogueNode::GetGraph() const
{
	return Graph;
}

void UInstantDialogueNode::PrintNodeTitle() const
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), &NodeTitle.ToString()); //
}

void UInstantDialogueNode::PrintGetName() const
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetName());
}

void UInstantDialogueNode::PrintClasseres() const
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *this->GetClass()->GetName()); //chinese
	UE_LOG(LogTemp, Warning, TEXT("%s"), this->GetClass()); //
}

void UInstantDialogueNode::PrintOutputPins() const
{
	UE_LOG(LogTemp, Warning, TEXT("%d"), OutputPins.Num());
	UE_LOG(LogTemp, Warning, TEXT("yes this works"));
}

int32 UInstantDialogueNode::GetOutputAmount()
{
	return OutputPins.Num();
}
#undef LOCTEXT_NAMESPACE
