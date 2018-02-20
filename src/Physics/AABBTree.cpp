#include "AABBTree.hpp"

void AABBTree::InsertData(Collider * aCollider)
{
}

void AABBTree::UpdateData(Collider * aCollider)
{
}

void AABBTree::RemoveData(Collider * aCollider)
{
}

void AABBTree::SelfQuery(QueryResults & results)
{
}

AABBTree::Node * AABBTree::GetLeft(Node * aNode)
{
  // TODO: Make this safe
  return &mNodes[aNode->mLeftIndex];
}

AABBTree::Node * AABBTree::GetRight(Node * aNode)
{
  // TODO: Make this safe
  return &mNodes[aNode->mRightIndex];
}

AABBTree::Node * AABBTree::GetParent(Node * aNode)
{
  // TODO: Make this safe
  return &mNodes[aNode->mParentIndex];
}

AABBTree::Node * AABBTree::GetSibling(Node * aNode)
{
  // TODO: Make this safe
  Node * mParent = GetParent(aNode);
  if (mParent->mLeftIndex = aNode->mSelfIndex) {
    return GetRight(mParent);
  }
  return GetLeft(mParent);
}
