#include "EdGraph_InstantDialogue.h"
#include "InstantDialogueEditorPCH.h"
#include "InstantDialogueGraph.h"
#include "Nodes/EdNode_InstantDialogueNode.h"
#include "EdNode_InstantDialogueEdge.h"
#include "Nodes/InstantDialogueNodeAnswer.h"

UEdGraph_InstantDialogue::UEdGraph_InstantDialogue()
{

}

UEdGraph_InstantDialogue::~UEdGraph_InstantDialogue()
{

}

void UEdGraph_InstantDialogue::RebuildInstantDialogue() // go threw all EdNodes get the linked pins and set the UNode Values
{
	LOG_INFO(TEXT("UInstantDialogueEdGraph::RebuildInstantDialogue has been called"));

	UE_LOG(LogTemp, Warning, TEXT("RebuildInstantDialogue was called"));

	UInstantDialogue* Graph = GetInstantDialogue();

	Clear();

	for (int i = 0; i < Nodes.Num(); ++i)
	{
		if (UEdNode_InstantDialogueNode* EdNode = Cast<UEdNode_InstantDialogueNode>(Nodes[i]))
		{
			if (EdNode->InstantDialogueNode == nullptr)
				continue;

			UInstantDialogueNode* InstantDialogueNode = EdNode->InstantDialogueNode;

			NodeMap.Add(InstantDialogueNode, EdNode);

			Graph->AllNodes.Add(InstantDialogueNode);

			for (int PinIdx = 0; PinIdx < EdNode->Pins.Num(); ++PinIdx)
			{
				UEdGraphPin* Pin = EdNode->Pins[PinIdx];

				if (Pin->Direction != EEdGraphPinDirection::EGPD_Output)
					continue;

				/// here

				for (int LinkToIdx = 0; LinkToIdx < Pin->LinkedTo.Num(); ++LinkToIdx)
				{
					UInstantDialogueNode* ChildNode = nullptr;
					if (UEdNode_InstantDialogueNode* EdNode_Child = Cast<UEdNode_InstantDialogueNode>(Pin->LinkedTo[LinkToIdx]->GetOwningNode()))
					{
						ChildNode = EdNode_Child->InstantDialogueNode;
					}
					else if (UEdNode_InstantDialogueEdge* EdNode_Edge = Cast<UEdNode_InstantDialogueEdge>(Pin->LinkedTo[LinkToIdx]->GetOwningNode()))
					{
						UEdNode_InstantDialogueNode* Child = EdNode_Edge->GetEndNode();;
						if (Child != nullptr)
						{
							ChildNode = Child->InstantDialogueNode;
						}
					}

					if (ChildNode != nullptr)
					{
						InstantDialogueNode->ChildrenNodes.Add(ChildNode);
						/// change them to here
						InstantDialogueNode->OutputPins.Add(Pin); // = UNode->OutputPins
                        InstantDialogueNode->OutputMap.Add(PinIdx, ChildNode);
						ChildNode->ParentNodes.Add(InstantDialogueNode);

						/*if (InstantDialogueNode->GetClass() == UInstantDialogueNodeAnwser::StaticClass())
						{
							UInstantDialogueNodeAnwser* MyAnswer = CastChecked<UInstantDialogueNodeAnwser>(InstantDialogueNode);
							MyAnswer->AnswerText.Add(TEXT("This is New Pin")); //, PinIdx							
						}*/
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Not Error"));
						//LOG_ERROR(TEXT("UEdGraph_Instant Dialogue::RebuildInstantDialogue can't find child node"));
					}
				}
			}
		}
		else if (UEdNode_InstantDialogueEdge* EdgeNode = Cast<UEdNode_InstantDialogueEdge>(Nodes[i]))
		{
			UEdNode_InstantDialogueNode* StartNode = EdgeNode->GetStartNode();
			UEdNode_InstantDialogueNode* EndNode = EdgeNode->GetEndNode();
			UInstantDialogueEdge* Edge = EdgeNode->InstantDialogueEdge;

			if (StartNode == nullptr || EndNode == nullptr || Edge == nullptr)
			{
				//LOG_ERROR(TEXT("UEdGraph_InstantDialogue::RebuildInstantDialogue add edge failed."));
				continue;
			}

			EdgeMap.Add(Edge, EdgeNode);

			Edge->Graph = Graph;
			Edge->Rename(nullptr, Graph, REN_DontCreateRedirectors | REN_DoNotDirty);
			Edge->StartNode = StartNode->InstantDialogueNode;
			Edge->EndNode = EndNode->InstantDialogueNode;
			Edge->StartNode->Edges.Add(Edge->EndNode, Edge);
		}
	}

	for (int i = 0; i < Graph->AllNodes.Num(); ++i)
	{
		UInstantDialogueNode* Node = Graph->AllNodes[i];
		if (Node->ParentNodes.Num() == 0)
		{
			Graph->RootNodes.Add(Node);

			SortNodes(Node);
		}

		Node->Graph = Graph;
		Node->Rename(nullptr, Graph, REN_DontCreateRedirectors | REN_DoNotDirty);
	}

	Graph->RootNodes.Sort([&](const UInstantDialogueNode& L, const UInstantDialogueNode& R)
	{
		UEdNode_InstantDialogueNode* EdNode_LNode = NodeMap[&L];
		UEdNode_InstantDialogueNode* EdNode_RNode = NodeMap[&R];
		return EdNode_LNode->NodePosX < EdNode_RNode->NodePosX;
	});
}

UInstantDialogue* UEdGraph_InstantDialogue::GetInstantDialogue() const
{
	return CastChecked<UInstantDialogue>(GetOuter());
}

bool UEdGraph_InstantDialogue::Modify(bool bAlwaysMarkDirty /*= true*/)
{
	bool Rtn = Super::Modify(bAlwaysMarkDirty);

	GetInstantDialogue()->Modify();

	for (int32 i = 0; i < Nodes.Num(); ++i)
	{
		Nodes[i]->Modify();
	}

	return Rtn;
}

void UEdGraph_InstantDialogue::Clear()
{
	UInstantDialogue* Graph = GetInstantDialogue();

	Graph->ClearGraph();
	NodeMap.Reset();
	EdgeMap.Reset();

	for (int i = 0; i < Nodes.Num(); ++i)
	{
		if (UEdNode_InstantDialogueNode* EdNode = Cast<UEdNode_InstantDialogueNode>(Nodes[i]))
		{
			UInstantDialogueNode* InstantDialogue = EdNode->InstantDialogueNode;
			InstantDialogue->ParentNodes.Reset();
			InstantDialogue->ChildrenNodes.Reset();
			InstantDialogue->OutputPins.Reset();
			InstantDialogue->Edges.Reset();
		}
	}
}

void UEdGraph_InstantDialogue::SortNodes(UInstantDialogueNode* RootNode)
{
	int Level = 0;
	TArray<UInstantDialogueNode*> CurrLevelNodes = { RootNode };
	TArray<UInstantDialogueNode*> NextLevelNodes;

	while (CurrLevelNodes.Num() != 0)
	{
		int32 LevelWidth = 0;
		for (int i = 0; i < CurrLevelNodes.Num(); ++i)
		{
			UInstantDialogueNode* Node = CurrLevelNodes[i];

			auto Comp = [&](const UInstantDialogueNode& L, const UInstantDialogueNode& R)
			{
				UEdNode_InstantDialogueNode* EdNode_LNode = NodeMap[&L];
				UEdNode_InstantDialogueNode* EdNode_RNode = NodeMap[&R];
				return EdNode_LNode->NodePosX < EdNode_RNode->NodePosX;
			};

			Node->ChildrenNodes.Sort(Comp);
			Node->ParentNodes.Sort(Comp);

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

void UEdGraph_InstantDialogue::PostEditUndo()
{
	Super::PostEditUndo();

	NotifyGraphChanged();
}

