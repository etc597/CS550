#ifndef Physics_AABBTree_hpp
#define Physics_AABBTree_hpp

#include "Physics/AABB.hpp"
#include <stack>

class Collider;

class QueryResults
{
public:
  std::vector<std::pair<Collider*, Collider*>> mPairs;
};

class AABBTree
{
public:
  AABBTree() = default;

  void InsertData(Collider * aCollider);
  void UpdateData(Collider * aCollider);
  void RemoveData(Collider * aCollider);

  void SelfQuery(QueryResults& results);
private:
  class Node
  {
  public:
    Collider * mCollider;
    AABB mAABB;
    int mParentIndex;
    int mLeftIndex;
    int mRightIndex;
    int mSelfIndex;
  };

  Node * GetLeft(Node * aNode);
  Node * GetRight(Node * aNode);
  Node * GetParent(Node * aNode);
  Node * GetSibling(Node * aNode);

  Node * mRoot;
  std::vector<Node> mNodes;
  std::stack<int> mFreeIndex;
};

#endif
