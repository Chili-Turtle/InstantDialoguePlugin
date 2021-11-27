#include "InstantDialogueGraph.h"

//#include "InstantDialogueRuntimePCH.h"
//#include "Engine/Engine.h"
#include "Nodes/InstantDialogueNodeTest.h"
#include "Nodes/InstantDialogueNodeText.h"
#include "Nodes/InstantDialogueNodeAnswer.h"

#define LOCTEXT_NAMESPACE "InstantDialogue"

UInstantDialogue::UInstantDialogue()
{

	FWorldDelegates::OnPostWorldCreation.AddUObject(this, &UInstantDialogue::OnPostWorldCreation);
	
	NodeType = UInstantDialogueNode::StaticClass();
	EdgeType = UInstantDialogueEdge::StaticClass();

	bEdgeEnabled = false;

	//CurrentNode = nullptr;
	//CurrentNodeTest2 = nullptr;

//#if WITH_EDITORONLY_DATA
	EdGraph = nullptr;
	//UE_LOG(LogTemp, Error, TEXT("EditorOnly_Data"));
	bCanRenameNode = true;
//#endif
}

UInstantDialogue::~UInstantDialogue()
{
	//CurrentNode = nullptr;
	//CurrentNodeTest2 = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("Destructor"));
}

///////////////////////////////////////////////// del if not needed, testing how delegated work
void UInstantDialogue::DialogueStarted()
{
	OnDialogueStarted.Broadcast();
}

void UInstantDialogue::DialogueEnded()
{
	OnDialogueEnded.Broadcast();
}

void UInstantDialogue::PostInitProperties()
{
	UObject::PostInitProperties();

	UE_LOG(LogTemp, Warning, TEXT("when does post init propertyies get called? 1"))
}

void UInstantDialogue::OnPostWorldCreation(UWorld* CurWorld)
{
	UE_LOG(LogTemp, Warning, TEXT("Reset current nodes"))
	CurrentNode = nullptr;
}

void UInstantDialogue::Print(bool ToConsole /*= true*/, bool ToScreen /*= true*/)
{
	int Level = 0;
	TArray<UInstantDialogueNode*> CurrLevelNodes = RootNodes;
	TArray<UInstantDialogueNode*> NextLevelNodes;

	while (CurrLevelNodes.Num() != 0)
	{
		for (int i = 0; i < CurrLevelNodes.Num(); ++i)
		{
			UInstantDialogueNode* Node = CurrLevelNodes[i];
			check(Node != nullptr);

			FString Message = FString::Printf(TEXT("%s, Level %d"), *Node->GetDescription().ToString(), Level);

			// if (ToConsole)
			// {
			// 	LOG_INFO(TEXT("%s"), *Message);
			// }

			if (ToScreen && GEngine != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, Message);
			}

			for (int j = 0; j < Node->ChildrenNodes.Num(); ++j)
			{
				NextLevelNodes.Add(Node->ChildrenNodes[j]);
			}
		}

		CurrLevelNodes = NextLevelNodes;
		NextLevelNodes.Reset();
		++Level;
	}
}

//Loop threw all RootNode(s), and add the children to the currLevelNodes, ++level, return level //gives the number of the longest branch
int UInstantDialogue::GetLevelNum() const
{
	int Level = 0;
	TArray<UInstantDialogueNode*> CurrLevelNodes = RootNodes;
	TArray<UInstantDialogueNode*> NextLevelNodes;

	while (CurrLevelNodes.Num() != 0)
	{
		for (int i = 0; i < CurrLevelNodes.Num(); ++i)
		{
			UInstantDialogueNode* Node = CurrLevelNodes[i];
			check(Node != nullptr);

			for (int j = 0; j < Node->ChildrenNodes.Num(); ++j)
			{
				NextLevelNodes.Add(Node->ChildrenNodes[j]);
			}
		}

		CurrLevelNodes = NextLevelNodes;
		NextLevelNodes.Reset();
		++Level;
	}

	return Level;
}

void UInstantDialogue::GetNodesByLevel(int Level, TArray<UInstantDialogueNode*>& Nodes)
{
	int CurrLEvel = 0;
	TArray<UInstantDialogueNode*> NextLevelNodes;

	Nodes = RootNodes;

	while (Nodes.Num() != 0)
	{
		if (CurrLEvel == Level)
			break;

		for (int i = 0; i < Nodes.Num(); ++i)
		{
			UInstantDialogueNode* Node = Nodes[i];
			check(Node != nullptr);

			for (int j = 0; j < Node->ChildrenNodes.Num(); ++j)
			{
				NextLevelNodes.Add(Node->ChildrenNodes[j]);
			}
		}

		Nodes = NextLevelNodes;
		NextLevelNodes.Reset();
		++CurrLEvel;
	}
}

FNodeData UInstantDialogue::GetNodeData(int32 NextNodeIndex) // make stuff when ever I press space
{
	FNodeData NodeData;

	//////////////////// init NodeDate, do I still need this //////////////////////////////
	//NodeData.NodeType = EDialogueNodeType::Default;
	// //NodeData.SpeakerName = LOCTEXT("SpeakerName","NONE");
	//NodeData.SpeakerName = FText::FromString("NONE");
	//NodeData.Portrait = nullptr;
	//NodeData.DialogueTextData.Add(FText::FromString("No Dialogue detected"));
	/////////////////////////////////////////////////////////////////////////////////////
	
	if (RootNodes.Num() <= 0) //check if have Nodes in Graph
    {
		UE_LOG(LogTemp, Warning, TEXT("return no viable data"));
    	return NodeData; //return no viable data
    }
	
	 if (CurrentNode == nullptr) //Dialogue started, when you have no CurrentNode assigned
     {
     	 	CurrentNode = RootNodes[0];
	 		DialogueStarted();
     	 	UE_LOG(LogTemp, Warning, TEXT("Dialogue Started"));
     }
	 else if (CurrentNode->ChildrenNodes.Num() > 0) //check for children nodes, can I go to next node?
	 {
	 	UE_LOG(LogTemp, Warning, TEXT("childamount %d"), CurrentNode->ChildrenNodes.Num());
	 	
	 	if (NextNodeIndex <= CurrentNode->ChildrenNodes.Num() && NextNodeIndex >= 0) //bound check for answer Nodes
	 	{
	 		//CurrentNodeTest2 = RootNodes[0];
	 		//CurrentNode = CurrentNode->ChildrenNodes[NextNodeIndex];
	 		CurrentNode = CurrentNode->OutputMap[NextNodeIndex + 1]; //+1, because [0] is Input
	 	}else
	 	{
	 		UE_LOG(LogTemp, Warning, TEXT("Out of bounds, no next node selected!, or ConditionNode failed(-1)"));
	 		DialogueEnded();
	 		CurrentNode = nullptr;
	 		//return NodeData;
	 	}
	 	
	 }
	 else //end of dialogue
	 {
	 	UE_LOG(LogTemp, Warning, TEXT("End of dialogue"));
	 	if (ResetDialogue == true)
	 	{
	 		CurrentNode = nullptr;
	 	}
	 	
	 	DialogueEnded();
	 }
	
	//UE_LOG(LogTemp, Warning, TEXT("NodeType %d"), CurrentNode->NodeType);

	if (!IsValid(CurrentNode) || CurrentNode == nullptr)
	{
		return NodeData;
	}

	UInstantDialogueNodeText* TextNode = Cast<UInstantDialogueNodeText>(CurrentNode);
	UInstantDialogueNodeAnwser* AnswerNode = Cast<UInstantDialogueNodeAnwser>(CurrentNode);
	UInstantDialogueNodeTest* ConditionNode = Cast<UInstantDialogueNodeTest>(CurrentNode);
	
	 switch (CurrentNode->NodeType) // set the NodeData
	 {
	 	case EDialogueNodeType::Text: 
	 		NodeData.DialogueTextData.Emplace(TextNode->NodeDialogueText);
	 		NodeData.SpeakerName = TextNode->NodeSpeakerName;
	 		NodeData.Portrait = TextNode->Portrait;
	 		NodeData.NodeType = EDialogueNodeType::Text;
	 		UE_LOG(LogTemp, Warning, TEXT("TextNode"));
	 		break;
	 	case EDialogueNodeType::Answer:
	 		for (int i = 0; i < AnswerNode->NameText.Num(); ++i)
	 		{
	 			NodeData.DialogueTextData.Emplace(AnswerNode->NameText[i]);
	 		}
	 		NodeData.NodeType = EDialogueNodeType::Answer;
	 		NodeData.SpeakerName = FText().FromString("");
	 		NodeData.Portrait = nullptr;
	 		UE_LOG(LogTemp, Warning, TEXT("AnswerNode"));
	 		//CurrentNode = CurrentNode->ChildrenNodes[0]; //Get Answer Index
	 		//change current node, do nothing, how do I do this?
	 		break;
	 	case EDialogueNodeType::Condition:
	 		NodeData.NodeType = EDialogueNodeType::Condition;
		 	UE_LOG(LogTemp, Warning, TEXT("Condition pin %d"), ConditionNode->GetConditionPin());
	 		 return GetNodeData(ConditionNode->GetConditionPin()); //can I just return this? or should I use continue
	 	default:
	 		UE_LOG(LogTemp, Warning, TEXT("Unknown node can't proceed"));
	 		break;
	 }
	//  get the type
	// switch of types
	// if answer, get answer text, if text get next text 
	return NodeData;
}

void UInstantDialogue::ClearGraph()
{
	for (int i = 0; i < AllNodes.Num(); ++i)
	{
		UInstantDialogueNode* Node = AllNodes[i];

		Node->ParentNodes.Empty();
		Node->ChildrenNodes.Empty();
		Node->OutputMap.Empty();
		Node->Edges.Empty();
	}

	AllNodes.Empty();
	RootNodes.Empty();
}

#undef LOCTEXT_NAMESPACE
