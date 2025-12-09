// Fill out your copyright notice in the Description page of Project Settings.


#include "BoneDataNode.h"

//Will made all of these one liner functions
BoneDataNode::BoneDataNode()
{
}

BoneDataNode::BoneDataNode(FTransform trans, FVector location, FQuat rotation, FVector scale)
{
	this->trans = trans;
	this->location = location;
	this->rotation = rotation;
	this->scale = scale;
}

BoneDataNode::~BoneDataNode()
{
}

FTransform BoneDataNode::GetTransform()
{
	return this->trans;

}

FVector BoneDataNode::GetLocation()
{
	return this->location;
}

FQuat BoneDataNode::GetRotation()
{
	return this->rotation;
}

FVector BoneDataNode::GetScale()
{
	return FVector();
}

void BoneDataNode::SetTransform(FTransform inputTransform)
{
	this->trans = inputTransform;
}

void BoneDataNode::SetLocation(FVector inputLocation)
{
	this->location = inputLocation;
}

void BoneDataNode::SetRotation(FQuat inputRotation)
{
	this->rotation = inputRotation;
}

void BoneDataNode::SetScale(FVector intputScale)
{
	this->scale = intputScale;
}

void BoneDataNode::SetBoneName(FName name)
{
	this->boneName = name;
}
void BoneDataNode::SetParentName(FName name)
{
	this->parentBoneName = name;
}

FName BoneDataNode::GetBoneName()
{
	return this->boneName;
}

FName BoneDataNode::GetParentName()
{
	return this->parentBoneName;
}