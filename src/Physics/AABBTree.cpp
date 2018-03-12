#include "AABBTree.hpp"
#include "Physics/Collider.hpp"

#include <functional>

AABBTree::AABBTree()
  : mRoot(0)
  , mKeyCounter(0)
  , mNodes()
  , mFreeIndex()
  , mPad(0.1f)
{
}

void AABBTree::InsertData(unsigned& key, Collider * aCollider)
{
  key = AcquireKey();
  Node& node = AcquireNode(key);
  node.aabb = aCollider->GetAABB();
  node.aabb.Pad(mPad);
  node.leaf = true;
  node.data = aCollider;

  if (mRoot == 0) {
    mRoot = key;
  }

  if (mRoot == key) {
    return;
  }

  Insert(key);
  Reshape(key);
}

void AABBTree::UpdateData(unsigned key, Collider * aCollider)
{
  Node& node = Nodes(key);

  if (node.aabb.Contains(aCollider->GetAABB())) {
    return;
  }

  node.aabb = aCollider->GetAABB();
  node.aabb.Pad(mPad);

  if (key != mRoot) {
    Reshape(Erase(key));
    Insert(key);
    Reshape(key);
  }
}

void AABBTree::RemoveData(unsigned key)
{
  Reshape(Erase(key));
  ReleaseNode(key);
  ReleaseKey(key);
}

void AABBTree::DebugDraw(std::function<void(const AABB&, unsigned)> draw, int level /*= -1*/)
{
  if (level == -1) {
    for (auto& node : mNodes) {
      if (node.active) {
        //draw(node.aabb, node.self);
      }
    }
  }
}

void AABBTree::SelfQuery(QueryResults & results)
{
  results.clear();
  if (!mRoot) {
    return;
  }

  auto& root = Nodes(mRoot);
  SelfQuery(root, results);

  //if (!results.mPairs.empty())
  //{
  //  results.mPairs.back();
  //}
}

void AABBTree::SelfQuery(Node & node, QueryResults& results)
{
  if (node.leaf)
  {
    return;
  }

  SelfQuery(Nodes(node.left), Nodes(node.right), results);
  SelfQuery(Nodes(node.left), results);
  SelfQuery(Nodes(node.right), results);
}

void AABBTree::SelfQuery(Node & nodeA, Node & nodeB, QueryResults & results)
{
  if (!nodeA.aabb.Collides(nodeB.aabb)) {
    return;
  }

  if (nodeA.leaf && nodeB.leaf) {
    results.emplace_back(nodeA.self, nodeB.self, nodeA.data, nodeB.data);
  }
  else if (nodeA.leaf) {
    SelfQuery(nodeA, Nodes(nodeB.left), results);
    SelfQuery(nodeA, Nodes(nodeB.right), results);
  }
  else if (nodeB.leaf) {
    SelfQuery(nodeB, Nodes(nodeA.left), results);
    SelfQuery(nodeB, Nodes(nodeA.right), results);
  }
  else
  {
    auto& n0L = Nodes(nodeA.left);
    auto& n0R = Nodes(nodeA.right);
    auto& n1L = Nodes(nodeB.left);
    auto& n1R = Nodes(nodeB.right);

    SelfQuery(n0L, n1L, results);
    SelfQuery(n0L, n1R, results);
    SelfQuery(n0R, n1L, results);
    SelfQuery(n0R, n1R, results);
  }
}

void AABBTree::Insert(unsigned key)
{
  unsigned leaf = mRoot;
  // Find the best fitting leaf node
  while (!Nodes(leaf).leaf)
  {
    float a = Heuristic(Nodes(key).aabb, Nodes(Nodes(leaf).left).aabb);
    float b = Heuristic(Nodes(key).aabb, Nodes(Nodes(leaf).right).aabb);
    leaf = a < b ? Nodes(leaf).left : Nodes(leaf).right;
  }

  // Split the leaf node
  unsigned parent = AcquireKey();
  Node& parentNode = AcquireNode(parent);
  if (leaf == mRoot) {
    mRoot = parent;
  }
  else {
    unsigned leaf_parent = Nodes(parent).parent = Nodes(leaf).parent;
    if (Nodes(leaf_parent).left == leaf) {
      Nodes(leaf_parent).left = parent;
    }
    else {
      Nodes(leaf_parent).right = parent;
    }
  }

  Nodes(parent).left = leaf;
  Nodes(parent).right = key;
  Nodes(parent).leaf = false;
  Nodes(key).parent = parent;
  Nodes(leaf).parent = parent;
}

void AABBTree::Reshape(unsigned key)
{
  unsigned original = key;
  Node * node = &Nodes(key);

  if (node->leaf) {
    if (key == mRoot) {
      return;
    }

    key = node->parent;
  }

  // Grow parent until we've grown the root
  while(true) {
    node = &Nodes(key);
    node->aabb = Nodes(node->right).aabb + Nodes(node->left).aabb;

    if (key == mRoot) {
      break;
    }

    key = node->parent;
  }
}

unsigned AABBTree::AcquireKey()
{
  if (mFreeIndex.empty()) {
    return ++mKeyCounter;
  }
  unsigned ret = mFreeIndex.top();
  mFreeIndex.pop();
  return ret;
}

void AABBTree::ReleaseKey(unsigned key)
{
  mFreeIndex.push(key);
}

AABBTree::Node& AABBTree::AcquireNode(unsigned key)
{
  while (key > mNodes.size()) {
    mNodes.emplace_back();
    mNodes.back().self = mNodes.size();
  }
  mNodes[key - 1].active = true;
  return mNodes[key - 1];
}

void AABBTree::ReleaseNode(unsigned key)
{
  Nodes(key).active = false;
}

AABBTree::Node& AABBTree::Nodes(unsigned key)
{
  return mNodes[key - 1];
}

unsigned AABBTree::Erase(unsigned key)
{
  if (!Nodes(key).leaf) {
    return 0;
  }

  if (mRoot == key) {
    return key;
  }

  unsigned parent = Nodes(key).parent;
  unsigned sibling = Nodes(parent).left == key ? Nodes(parent).right : Nodes(parent).left;

  if (mRoot == parent) {
    mRoot = sibling;
  }
  else {
    unsigned grandparent = Nodes(parent).parent;
    Nodes(sibling).parent = grandparent;
    if (Nodes(grandparent).left == parent) {
      Nodes(grandparent).left = sibling;
    }
    else {
      Nodes(grandparent).right = sibling;
    }
  }

  ReleaseNode(parent);
  ReleaseKey(parent);
  return sibling;
}

float AABBTree::Heuristic(const AABB& insertedNode, const AABB& possiblePartner)
{
  return (insertedNode + possiblePartner).GetSurfaceArea() - possiblePartner.GetSurfaceArea();
}
