#ifndef Physics_AABBTree_hpp
#define Physics_AABBTree_hpp

#include "Physics/AABB.hpp"
#include <stack>
#include <functional>

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

  void DebugDraw(std::function<void(const AABB&, unsigned)> draw, int level = -1);

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
    bool active;
  };

  void SelfQuery(Node& node, QueryResults& results);
  void SelfQuery(Node& nodeA, Node& nodeB, QueryResults& results);
  void Insert(unsigned key);
  void Reshape(unsigned key);

  unsigned AcquireKey();
  void ReleaseKey(unsigned key);
  Node& AcquireNode(unsigned key);
  void ReleaseNode(unsigned key);
  Node& Nodes(unsigned key);
  unsigned Erase(unsigned key);
  float Heuristic(const AABB& insertedNode, const AABB& possiblePartner);

  const float mPad;
  unsigned mRoot;
  unsigned mKeyCounter;
  std::vector<Node> mNodes;
  std::stack<int> mFreeIndex;
};

#endif
