// Fill out your copyright notice in the Description page of Project Settings.


#include "BlendTree.h"

BlendTree::BlendTree()
{
}

BlendTree::~BlendTree()
{
}

//Tristan made this
void BlendTree::CreateTree(UModifiedPoseableMeshComponent* meshHeirarchy)
{
	blendTree.heirarchyDescriptor = meshHeirarchy;
	blendTree.nodes.Init({}, meshHeirarchy->GetNumBones());
}

//Tristan made this
void BlendTree::ConfigureNode(FName name, FTransform* outPose, FTransform* inPose1, FTransform* inPose2, BlendOps blendOp)
{
	int nodeIndex = blendTree.heirarchyDescriptor->GetBoneIndex(name);

	//Maybe have a way to pick another node struct that uses a different amount of ctrls and inputs for blend op
	BlendNode2C1I newNode;
	newNode.out = outPose;
	newNode.ctrl[0] = inPose1;
	newNode.ctrl[1] = inPose2;
	newNode.blendOp = blendOp;

	blendTree.nodes[nodeIndex] = newNode;
}

//Tristan and Will made this
void BlendTree::Execute(float u)
{
	TArray<FName> poseNames;
	blendTree.heirarchyDescriptor->GetBoneNames(poseNames);
	for (int i = 0; i < poseNames.Num(); i++)
	{
		BlendNode2C1I node = blendTree.nodes[i];

		switch (node.blendOp)
		{
		case LERP:
			node.out->SetTranslation(Lerp(node.ctrl[0]->GetTranslation(), node.ctrl[1]->GetTranslation(), u));
			node.out->SetRotation(FMath::Lerp(node.ctrl[0]->GetRotation(), node.ctrl[1]->GetRotation(), u));
			node.out->SetScale3D(Lerp(node.ctrl[0]->GetScale3D(), node.ctrl[1]->GetScale3D(), u));
			break;
		case CONCAT:
			node.out->SetTranslation(Add(node.ctrl[0]->GetTranslation(), node.ctrl[1]->GetTranslation()));
			node.out->SetRotation(node.ctrl[0]->GetRotation() * (node.ctrl[1]->GetRotation() * node.ctrl[0]->GetRotation().Inverse()));
			node.out->SetScale3D(Add(node.ctrl[0]->GetScale3D(), node.ctrl[1]->GetScale3D()));
		}
		


		/*node.out->SetLocation(node.ctrl[0]->GetLocation());
		node.out->SetRotation(node.ctrl[0]->GetRotation());
		node.out->SetScale3D(node.ctrl[0]->GetScale3D());*/
		//blendTree.heirarchyDescriptor->SetBoneSpaceTranformByName(outPose, node2C1I->out);
	}
}

//Tristan made this
FVector BlendTree::Lerp(FVector v0, FVector v1, float u)
{
	FVector out;
	out.X = (v1.X - v0.X) * u + v0.X;
	out.Y = (v1.Y - v0.Y) * u + v0.Y;
	out.Z = (v1.Z - v0.Z) * u + v0.Z;
	return out;
}

//Tristan made this
FVector BlendTree::Add(FVector v0, FVector v1)
{
	FVector out;
	out.X = v0.X + (v1.X - v0.X);
	out.Y = v0.Y + (v1.Y - v0.Y);
	out.Z = v0.Z + (v1.Z - v0.Z);
	return out;
}

