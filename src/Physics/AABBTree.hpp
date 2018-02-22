#ifndef Physics_AABBTree_hpp
#define Physics_AABBTree_hpp

#include "Physics/AABB.hpp"
#include <stack>

class Collider;

class QueryResults
{
public:
  std::vector<std::pair<unsigned, unsigned>> mPairs;
};

class AABBTree
{
public:
  AABBTree();

  void InsertData(unsigned& key, Collider * aCollider);
  void UpdateData(unsigned key, Collider * aCollider);
  void RemoveData(unsigned key);

  void SelfQuery(QueryResults& results);
private:
  class Node
  {
  public:
    AABB aabb;
    unsigned parent;
    unsigned self;
    unsigned left;
    unsigned right;
    bool leaf;
    bool crossedChildren;
  };

  void Insert(unsigned key);
  void Reshape(unsigned key);

  unsigned AcquireKey();
  void ReleaseKey(unsigned key);
  Node& AcquireNode(unsigned key);
  Node& Nodes(unsigned key);
  unsigned Erase(unsigned key);
  float Heuristic(const AABB& aabb1, const AABB& aabb2);

  const float mPad;
  unsigned mRoot;
  unsigned mKeyCounter;
  std::vector<Node> mNodes;
  std::stack<int> mFreeIndex;
};

#endif
