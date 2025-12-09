// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModifiedPoseableMeshComponent.h"

/**
 * 
 */
class SUPERFINALCOOLPROJ_API BlendTree
{
public:
	enum BlendOps
	{
		LERP,
		CONCAT
	};

	struct BlendNode
	{
		FTransform* out;
		BlendOps blendOp;
	};

	struct BlendNode2C1I : BlendNode
	{
		FTransform* ctrl[2];
	};

	struct NodeBlendTree
	{
		UModifiedPoseableMeshComponent* heirarchyDescriptor;
		TArray<BlendNode2C1I> nodes;
	};

	BlendTree();
	~BlendTree();

	void CreateTree(UModifiedPoseableMeshComponent* meshHeirarchy);
	void ConfigureNode(FName name, FTransform* outPose, FTransform* inPose1, FTransform* inPose2, BlendOps blendOp);
	void Execute(float u);
private:
	NodeBlendTree blendTree;

	//BlendOp Functions
	FVector Lerp(FVector v0, FVector v1, float u);
	FVector Add(FVector v0, FVector v1);
};
